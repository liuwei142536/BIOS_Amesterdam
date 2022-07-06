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
;  ProcessorStartupUncore.asm
;
; Abstract:
;
;
;------------------------------------------------------------------------------        
        .586P
        .XMM
        .MODEL  SMALL, c


        
        include ProcessorStartupPlatform.inc
        include ProcessorStartupChipset.inc
        include ProcessorStartupUncore.inc
        include ProcessorStartup.inc

        EXTERN  ReadPciLegacy:NEAR32
        EXTERN  WritePciLegacy:NEAR32
        EXTERN  _OEMCpuSecInit:NEAR32
        EXTERN  TearDownNEMandHalt:NEAR32

;----------------------------------------------------------------------------
;       STARTUP_SEG  S E G M E N T  STARTS
;----------------------------------------------------------------------------
STARTUP_SEG SEGMENT PARA PUBLIC 'CODE' USE32

;----------------------------------------------------------------------------
; SetTargetList executes by all PBSPs. The purpose is to make sure that 
; default IO target will be match as legacy socket ID as at default before
; issue any IO transactions such as port 80. MMIO target also setup here 
;
;  Input:  All general puropse registers undefined
;          Stack may not be available
;          MMX3 = routine return address
;  Output: 
;          All general puropse registers undefined. MMCFG and CPUBUSNO related CSRs are initialized.
;          xmm1  [7:0]= legact socket id,  [27:8]= CPU Family/stepping info from cpuid instruction
;----------------------------------------------------------------------------
SetTargetList    PROC    NEAR    PUBLIC

      ;
      ; In CR4, set OSFXSR bit 9 and OSXMMEXCPT bit 10 to enable access to XMM registers used by this routine
      ;
      mov   eax, cr4
      or    eax, 1 SHL 9 + 1 SHL 10
      mov   cr4, eax

; BIOS HSD 3875167
; Disable LOCK lockcontrol.lockdisable (bus 1, device 0xb, function 0, offset 0x50, bit 0)
      mov   eax, CSR_LEGACY_LOCKCONTROL
      READ_PCI_DWORD_LEGACY               ; ebx = reg dword data
      or    ebx, BIT0                     ; set bit0
      WRITE_PCI_DWORD_LEGACY 

      ; make sure we program IOPORT_TARGET_LIST before can can do any io transaction
      ; 
      ; 1st get legacy socket ID and store in xmm1[7:0]
      mov   eax, CSR_LEGACY_SAD_TARGET_CBO    ; get legacy socket ID
      READ_PCI_DWORD_LEGACY
      and   ebx, 038h                         ; keep bit5:3
      shr   ebx, 3
      movd  xmm1, ebx                         ; save legacy node id in xmm1[7:0]
      ; 2nd program legacy socket ID to IOPORT_TARGET_LIST
; Program the IO Target List; Legacy IIO is the target
      mov   eax, ebx                          
      mov   cl, 3                             ; number of node id bits
      mov   ch, 7     
set_io_target_legacy:
      shl   eax, cl
      or    ebx, eax
      dec   ch
      jnz   set_io_target_legacy
                               
      mov   eax, CSR_LEGACY_IO_TARGET_LIST_CBO
      WRITE_PCI_DWORD_LEGACY 

      mov   eax, CSR_LEGACY_MMIO_TARGET_LIST_CBO
      WRITE_PCI_DWORD_LEGACY 

;  Read CPU Family/stepping info and store in xmm1[27:8]
      mov  eax, 1
      cpuid
      and  eax, 0fffffh
      shl  eax, 8          ; shift to bit[27:8]
      movd ebx, xmm1
      or   ebx, eax
      movd xmm1, ebx

      RET_ESI_MMX3

SetTargetList    ENDP


EarlyUncoreInit  PROC    NEAR    PUBLIC

        ;
        ; In CR4, set OSFXSR bit 9 and OSXMMEXCPT bit 10
        ;
        mov   eax, cr4
        or    eax, 1 SHL 9 + 1 SHL 10
        mov   cr4, eax

        ;
        ; Enable the MMIO Rule3 to cover the 0xFC000000 to 0xFFFFFFFF so that the NEM DATA region can be mapped to it.
        ; The target is set to local IIO.
        ;

        ; Program the MMIO Rule3 Base/Limit
              mov   eax, (CSR_LEGACY_MMIO_RULE3_CBO + 4)
              mov   ebx, 0h                                                       ; Higher bits of the rule are all 0
              WRITE_PCI_DWORD_LEGACY 
              mov   eax, CSR_LEGACY_MMIO_RULE3_CBO
              mov   ebx, 0FF00007Fh                             ; Base = 0xFC000000; Limit = 0xFFFFFFFF; Set the enable bit                     ; Higer bits of the tule are all 0
              WRITE_PCI_DWORD_LEGACY 

        CALL_MMX4   EnableAccessCSR                      ; init MMCFG/CPUBUSNO cnofig scheme

        RET_ESI_MMX3
EarlyUncoreInit  ENDP


MidUncoreInit    PROC    NEAR    PUBLIC

        ;---If Limit CPU ID enabled because of soft reset, disable.
        mov     ecx, 01a0h
        rdmsr
        btr     eax, 22                                 ; Reset bit
        jnc     @f                                      ; If already reset, don't write to 1a0.

        wrmsr
@@:
        ;---Change APIC Address---
        mov   ecx, MSR_XAPIC_BASE                       ; Enable local APIC
        rdmsr
        and   edx, NOT 0fh                              ; Mask local APIC address
        and   eax, NOT 0fffff000h                       ; Mask local APIC address
        or    eax, LOCAL_APIC_BASE
        wrmsr

        RET_ESI_MMX3

MidUncoreInit    ENDP

PreNemUncoreInit    PROC    NEAR    PUBLIC

        ;
        ; Prior to NEM init, use VCU API to notify VCU of "BIOS is about to enable cache"
        ;
        mov  eax, 1
        cpuid
        and eax, 0fffffh
        shr eax, 4
        cmp eax, CPU_FAMILY_HSX                       ; HSX CPU?
        je  hsx_or_bdx_cpu                              ; Jump to hsx_or_bdx_cpu for HSX or BDX
        cmp eax, CPU_FAMILY_BDX_DE                       ; BDX CPU ?
        je  hsx_or_bdx_cpu
        cmp eax, CPU_FAMILY_BDX
        jne pnui_exit                                   ; No, skip   
       
hsx_or_bdx_cpu:
      ; A this point, EnableAccessCsr() has been executed and CPUBUSNO/MMCFG  have been set up already
      ; Information in the xmm registers can be reused here
      ;
      ; check if running on real hardware or simulation tool
      ;
      movd  ecx, xmm2                                ; xmm2[31:24] stores the socketID
      shr   ecx, 24t                                 ; CL = local socket ID
      mov   eax, CSR_EMULATION_FLAG_UBOX
      CALL_MMX6  GetCpuCsrAddrNoStack                 ;[edi] = MMCFG config address of the CSR
      mov   eax, dword ptr ds:[edi]                   ; read CSR
      or    al, al                                    ; read hardware?
      jnz   pnui_exit                                 ; no, skip
      and   edi, 0fff00000h                               ; edi = MMCFG_Base + uncore bus#
      mov   ecx, VCODE_API_DELAY_COUNT                    ; software timeout loop-count
      mov   ebx, CSR_VCU_MAILBOX_INTERFACE AND 000fffffh  ; ebx = Dev:Func:Offset of VCU_MAILBOX_INTERFACE CSR
      mov   edx, CSR_VCU_MAILBOX_DATA AND 000fffffh       ; edx = Dev:Func:Offset of VCU_MAILBOX_DATA CSR
poll_vcu_busy1:
        mov   eax, dword ptr ds:[edi][ebx]              ; read CSR VCU_MAILBOX_INTERFACE
        cmp   eax, 0ffffffffh                 
        je    pnui_vcu_failure                          ; debug: - skip if the CSR is not implemented 
        test  eax, BIT31
        loopnz poll_vcu_busy1
        or    ecx, ecx
        jz    pnui_vcu_failure
       
        ; 
        ;  Call VCU API Open Sequnece (IOT_LLC_SETUP)
        ;
        mov   eax, VCODE_API_SEQ_ID_IOT_LLC_SETUP
        mov   dword ptr ds:[edi][edx], eax              ; write CSR VCU_MAILBOX_DATA

        mov   eax, (BIT31 OR VCODE_API_OPCODE_OPEN_SEQUENCE)
        mov   dword ptr ds:[edi][ebx], eax              ; write CSR VCU_MAILBOX_INTERFACE

        mov   ecx, VCODE_API_DELAY_COUNT                ; software timeout loop-count
poll_vcu_busy2:
        mov   eax, dword ptr ds:[edi][ebx]              ; read CSR VCU_MAILBOX_INTERFACE
        test  eax, BIT31
        loopnz poll_vcu_busy2
        or    ecx, ecx
        jz    pnui_vcu_failure

        ; 
        ;  Call VCU API SET_PARAMETER with Index = 0
        ;  This step is required although the parameters  is don't care 
        ;

        mov   eax, 0                                    ; set data = 0
        mov   dword ptr ds:[edi][edx], eax              ; write CSR VCU_MAILBOX_DATA
        mov   eax, (BIT31 OR VCODE_API_OPCODE_SET_PARAMETER)
        mov   dword ptr ds:[edi][ebx], eax              ; write CSR VCU_MAILBOX_INTERFACE

        mov   ecx, VCODE_API_DELAY_COUNT                ; software timeout loop-count
poll_vcu_busy3:
        mov   eax, dword ptr ds:[edi][ebx]              ; read CSR VCU_MAILBOX_INTERFACE
        test  eax, BIT31
        loopnz poll_vcu_busy3
        or    ecx, ecx
        jz    pnui_vcu_failure

        ;
        ;  Call VCU API  IOT_LLC_SETUP with Index=1
        ;
        mov   eax, (BIT31 OR VCODE_API_OPCODE_IOT_LLC_SETUP)
        or    eax, (1 SHL 16)                               ; bit[23:16] = Index = 1
        mov   dword ptr ds:[edi][ebx], eax                  ; write CSR VCU_MAILBOX_INTERFACE

        mov   ecx, VCODE_API_DELAY_COUNT                    ; software timeout loop-count
poll_vcu_busy4:
        mov   eax, dword ptr ds:[edi][ebx]                  ; read CSR VCU_MAILBOX_INTERFACE
        test  eax, BIT31
        loopnz poll_vcu_busy4
        or    ecx, ecx
        jz    pnui_vcu_failure

        ; 
        ; Call VCU API Close Sequnece (IOT_LLC_SETUP)
        ;
        mov   eax, VCODE_API_SEQ_ID_IOT_LLC_SETUP
        mov   dword ptr ds:[edi][edx], eax                  ; write CSR VCU_MAILBOX_DATA

        mov   eax, (BIT31 OR VCODE_API_OPCODE_CLOSE_SEQUENCE)
        mov   dword ptr ds:[edi][ebx], eax                  ; write CSR VCU_MAILBOX_INTERFACE
        
        mov   ecx, VCODE_API_DELAY_COUNT                    ; software timeout loop-count
poll_vcu_busy5:
        mov   eax, dword ptr ds:[edi][ebx]                  ; read CSR VCU_MAILBOX_INTERFACE
        test  eax, BIT31
        loopnz poll_vcu_busy5
        or    ecx, ecx
        jz    pnui_vcu_failure

pnui_exit:
        RET_ESI_MMX3
        

pnui_vcu_failure:
        PORT80(0CBh)                                    ; error code to port 80h
        jmp  pnui_exit

PreNemUncoreInit    ENDP



PostNemUncoreInit    PROC    NEAR    PUBLIC

        call ProcessorWorkaround                ; Early CPU WAs
        call QpiProgramMsrForWorkaound          ; Early QPI WAs
        call QpiPassInPllRatio                  ; Pass in Over-Clocking PLL Ratio for QPIRC

        call SetupProcessorCheckinData
    
        ;
        ; Early SBSP Selection
        ;                           
        call GetLocalSktId                      ; CL = Current socketId
        call GetSbspSktId                       ; AL = SBSP socketId
        cmp  cl, al
        jz   ImSbsp

        call ParkNbsp                           ; PBSPs will nott return 

ImSbsp:        
       ;
       ; Only SBSP comes here to continue posting. Other PBSPs are held in a loop.
       ; 
       ret
PostNemUncoreInit    ENDP



;----------------------------------------------------------------------------
;  Initialize PROC MMCFG and CPUBUSNO related registers 
;  Executed by all BSPs present in the system early after reset
;  
;  Input:  All general puropse registers undefined
;          Stack may not be available
;          MMX4 = routine return address
;          xmm1  [7:0]= legact socket id,  [27:8]= CPU Family/stepping info from cpuid instruction
;  Output: All general puropse registers undefined. MMCFG and CPUBUSNO related CSRs are initialized.
;          Following xmm registers hold MMCFG and bus number config scheme info (later used by otehr routines in this file)
;          xmm1  [7:0]= legact socket id,  [27:8]= CPU Family/stepping info from cpuid instruction
;          xmm2 = [31:24] SocketID, [23:0] MMCFG_TARGET_LIST 
;          xmm3 = MMCFG_RULE
;          xmm4/5 = IIO/Uncore Bus # array for socket0:3 Id (a.k.a node ID). 
;              bit  [7:0] Bus# for nodeId 0
;              bit [15:8] Bus# for nodeId 1
;              bit[23:16] Bus# for nodeId 2
;              bit[31:24] Bus# for nodeId 3
;          xmm6/7 = IIO/Uncore Bus # array for socket4:7 Id (a.k.a node ID).
;              bit  [7:0] Bus# for nodeId 4
;              bit [15:8] Bus# for nodeId 5
;              bit[23:16] Bus# for nodeId 6
;              bit[31:24] Bus# for nodeId 7

;----------------------------------------------------------------------------

EnableAccessCSR    PROC    NEAR  PUBLIC

      ; Initialize MMCFG_Target_List and CPUBUSNO to be used for this boot:
      ;   Cold-boot: Use a pre-defined MMCFG_Target_rule and bus # assighment scheme:
      ;     for 4S:  MMCFG_Target_List = [3,3,2,2,1,1,0,0], bus#s of 00, 0x40, 0x80, 0xC0
      ;     for 8S:  MMCFG_Target_List = [7,6,5,4,3,2,1,0], bus#s of 00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0
      ;   Warm boot: Calculate mmcfg/busnumber info using data in the sticky scratch pad reg saved from last boot.

         comment ^ 
         Scratch Pad Register 2 Format
         [7:0]   - CPU present bitmap (QPIRC updates, in all sockets); this list reflects the CPUs after the topology is degraded, if needed
         [31:8]  - used by QPIRC
         Scratch Pad Register 3 Format
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

         Scratch Pad Register 7 Format
         [0]     - ExtRTID Mode enabled or not 
         [4]     - Entered Cold Reset Flow (CPURC Updates as soon as possible, on all sockets)
         [5]     - Comlepted Cold Reset Flow (The code that issues warn reset requested by RC updates this, on all sockets)
         [10:8]  - Local socket id
         [19:16] - SBSP socket id
         [24]    - DCU_MODE select 0/1: 32KB 8-way no-ECC (hardware default) / 16KB 4-way with ECC  (CPU RC updates, in all sockets)
         [other bits] - Reserved

           CPU MMCFG_TARGET_LIST Register Format
               23:21                 11:9     8:6      5:3      2:0      bit
         --------------------------------------------------------------
             | NodeID |    ...    | NodeID | NodeID | NodeID | NodeID |
         --------------------------------------------------------------
               FF-E0                 7F-60   5F-40    3F-20     1F-00    bus-range
               note: legacy socket will always have lowest bus range start from bus0
         end comment ^
      
        ;
        ; In CR4, set OSFXSR bit 9 and OSXMMEXCPT bit 10 to enable access to XMM registers used by this routine
        ;
        mov   eax, cr4
        or    eax, 1 SHL 9 + 1 SHL 10
        mov   cr4, eax

      ; Read sticky scratch pad register for mmcfg/bus number config info 
      mov   eax, SSR_LEGACY_BOOT_CONFIG_INFO_CSR   
      READ_PCI_DWORD_LEGACY                    ; ebx = scratch pad reg dword data
      test  ebx, BIT5                          ; Cold Reset Flow Completed = 1?
      jz    short @f                           ; no
      
      test  ebx, BIT6                          ; hotplug Reset Flow Entered flag = 1?     
      jnz   use_default_cfg_scheme             ; Yes (use default cfg scheme) 
      
      and   bl, NOT (BIT4)                     ; clear bit4 of scratch pad register
      WRITE_PCI_DWORD_LEGACY 
      jmp   calculate_cfg_scheme               ; take the warm boot path

@@:
      test  ebx, BIT4                          ; Cold Reset Flow Entered flag = 1?
      jz    use_default_cfg_scheme             ; No (normal cold boot path) 

      ; Warm reset occured before completion of MRC on the cold boot path.
      ; In this case SBSP triggers a power-good reset while other socket BSPs waits
      ; Note: legacy socket will always SBSP at SEC phase

      ; Read own socketId from CSR
      mov   eax, CSR_LEGACY_SAD_CONTROL_CBO
      READ_PCI_DWORD_LEGACY                    ; ebx = reg dword data
      mov   bh, bl                             ; nodeId 
      and   bl, 00011b
      and   bh, 01000b
      shr   bh, 1
      or    bl, bh                             ; bl = local socketId
      movd  eax,  xmm1                         ; al = legacy socket ID
      cmp   bl, al
      jne   short @f                           ; br if non-legacy socket

      ; Legacy BSP comes here - Triggers a power-good reset.
      mov   al, 0eh
      mov   dx, 0cf9h
      out   dx, al
@@:
      jmp   $                                  ; wait for reset to occur


use_default_cfg_scheme:

      ; set Cold Reset Flow Entered flag = 1 in scratch pad register
      or   ebx, BIT4
      WRITE_PCI_DWORD_LEGACY 

      ; Use pre-defined config scheme for cold boot path
      mov   eax, DEFAULT_COLDBOOT_MMCFG_TARGET_LIST
      movd  xmm2, eax
      mov   eax, DEFAULT_COLDBOOT_MMCFG_RULE
      movd  xmm3, eax
      mov   eax, DEFAULT_COLDBOOT_IIO_BUS_NUMS
      movd  xmm4, eax
      mov   eax, DEFAULT_COLDBOOT_UNCORE_BUS_NUMS
      movd  xmm5, eax

IF MAX_CPU_SOCKETS GT 4
      mov   eax, DEFAULT_COLDBOOT_IIO_BUS_NUMS_4_7_S
      movd  xmm6, eax
      mov   eax, DEFAULT_COLDBOOT_UNCORE_BUS_NUMS_4_7_S
      movd  xmm7, eax
ENDIF
     
      ; check to see if legacy socket id is 0 or not
      ; if not, we need to adjust target list and bus range for iio and uncore
      ; 1st adjust target list, 2nd iio bus and 3rd uncore bus
      ; the algorithm is swap nodeid, iio bus, uncore bus with node io 0
      ; note: default node id 0 always is 0
      movd  eax, xmm1         ; get legacy socekt id
      mov   cl, al            
      mov   ch, cl            ; save legacy socket id in ch
      cmp   cl, 0             ; legacy socket id 0?
      je    Write_cfg_scheme_to_CSRs    ; yes, no need to adjust
            
     ; xmm2 format: MMCFG_TARGET_LIST[23:0], local socket id [31:24]
      movd  eax, xmm2         ; get default target list
      and   eax, 000FFFFFFh   ; make sure [31:24] cleared
      mov   ebx, eax

      add   cl, ch             ; socketId * 3  = socketid+socketid+socketid;
      add   cl, ch
IF MAX_CPU_SOCKETS GT 4
      mov   edx, 0111b        ; node id field mask, >= 4s, 3 bit each, so no more add cl
ELSE
      add   cl, ch             ; 4S config, each node use 2 node fields, 2 * 3bit *socket#, add 3 more time
      add   cl, ch
      add   cl, ch
      mov   edx, 0111111b
ENDIF           
      shr   ebx, cl
      and   ebx, edx          ; only keep legacy node id field
      or    eax, ebx          ; now id 0 bit field repelace with legacy socket id
      shl   edx, cl
      not   edx
      and   eax, edx          ; repalce bit field with node id 0 
      movd  xmm2, eax         ; save updated target list back to xmm2

      ; now adjust iio bus
      mov   cl, ch            ; restore legacy node id to cl
      cmp   cl, 3
      jg    get_socket4_7_iiobus
      movd  eax, xmm4         ; is socket3:0, get iio bus range
      jmp   swap_iiobus
get_socket4_7_iiobus:
      movd  eax, xmm6         ; is socket7:4, get iio bus 
      sub   cl, 4             ; adjust for socket7:4 iio bus byte offset
swap_iiobus:
      mov   ebx, eax
      shl   cl, 3             ; socketId * 8 to get corresponding byte offset
      mov   edx, 0FFh         ; bus# bit mask
      shl   edx, cl
      not   edx
      and   eax, edx          ; node id 0 default iio bus always 0, now move to byte offset by leagcy socket id
      cmp   ch, 3
      jg    update_skt4_7_iiobus
 ;ppsong     movd  xmm4, eax
      jmp   update_offset0_iiobus
update_skt4_7_iiobus:
      movd  xmm6, eax
      movd  eax, xmm4
update_offset0_iiobus:        ; now need to update nodeid0 iio bus
      shr   ebx, cl
      and   ebx, 0FFh
      mov   al, bl            ; now we move legacy node id x default iio bus to node 0
      movd  xmm4, eax

      ; now adjust uncore bus
      mov   cl, ch            ; restore legacy node id to cl
      cmp   cl, 3
      jg    get_socket4_7_uncorebus
      movd  eax, xmm5         ; is socket3:0, get uncore bus 
      jmp   swap_uncorebus
get_socket4_7_uncorebus:
      movd  eax, xmm7         ; is socket7:4, get iio bus range
      sub   cl, 4             ; adjust for socket7:4 uncore bus byte offset
swap_uncorebus:
      mov   ebx, eax
      shl   cl, 3             ; socketId * 8 to get corresponding byte offset
      mov   edx, 0FFh         ; bus# bit mask
      shl   edx, cl
      not   edx
      and   eax, edx          ; node id 0 default uncore bus always 3fh for 4S 1Fh for 8S, now move to byte offset by leagcy socket id
IF MAX_CPU_SOCKETS GT 4
      mov   edx, 01Fh         ; > 4S config socket0 uncore bus 0x1F
ELSE
      mov   edx, 03Fh
ENDIF
      shl   edx, cl
      or    eax, edx          ; update legacy socekt uncore bus to default value
      cmp   ch, 3
      jg    update_skt4_7_uncorebus
      jmp   update_offset0_uncorebus
update_skt4_7_uncorebus:
      movd  xmm7, eax         ; save updated socket7:4 uncore bus back to xmm7
      movd  eax, xmm5
update_offset0_uncorebus:           ; now need to update nodeid0 iio bus
      shr   ebx, cl
      and   ebx, 0FFh
      mov   al, bl          ; now we move legacy node id x default iio bus to node 0
      movd  xmm5, eax
      jmp   Write_cfg_scheme_to_CSRs  ; done adjustment


calculate_cfg_scheme:
      mov   eax, LEGACY_BIOSSCRATCHPAD2
      READ_PCI_DWORD_LEGACY                    ; ebx = reg dword data
      and   ebx, 0FFh                          ; keep CPU present bitmap [7:0]
      movd  xmm3, ebx
      ; get bus range info from SSR03, and stored in xmm3 [31:8]
      mov   eax, SSR_LEGACY_BUS_CONFIG_INFO_CSR
      READ_PCI_DWORD_LEGACY                    ; ebx = reg dword data
      shl   ebx, 8h                            ; 
      movd  eax, xmm3
      or    eax, ebx
      movd  xmm3, eax      ; xmm3[31:8]=bus rang x for socket x (x=0..7), [7:0]=socket preset bimap

      xor   ebp, ebp       ; ebp = mmcfg_target_list data
      xor   esi, esi       ; esi = temperary IIO Bus # array [0..3, or 4..7], one byte for each node ID
      xor   edi, edi       ; edi = temperary Uncore Bus # array [0..3, or 4..7], one byte for each node ID
      xor   ecx, ecx       ; ecx[31:16] = total bus # range of all sockets
      xor   edx, edx       ; dx = nodeID, edx[31:16] = bit position of next NodeID field of MMCFG_TARGET_LIST

check_node_present:
      movd  eax, xmm3      ; restore Scratch Pad reg data into eax.
      mov   cx, dx         ; cx = node ID  (0..7)
      bt    ax, cx         ; CPU node present?
      jnc   next_node_id   ; no, skip

      ;build IIO bus# array in esi/xmm4(socket0:3), 6(socekt4:7)
      mov   ebx, ecx
      shr   ebx, 16t       ; bl = total bus # range accumulated so far for all sockets. Initial value = 0.
      or    esi, ebx       ; fill IIO bus# in esi

      ;calculate Target_List field for current NodeID
      shl   cx, 1          ; cx = NodeID * 2
      add   cx, dx         ; cx = NodeID * 3
      add   cx, 8          ; Bus Range info starts from bit8 of eax
      shr   eax, cl        ; eax [2:0] = Bus Range info for current node ID
      and   al, 0111b      ; al[2:0] = bus range size in 32-bus granulirity (0,1,2,3...7)
      inc   al             ; al[3:0] = bus range size in 32-bus granulirity (1,2,3...7,8)
      movzx ebx, dx        ; ebx[2:0] = current nodeID
      ror   edx, 16t       ; swap hi/low words in edx. DX now = bit position for next NodeID field in Target_List 
      mov   cx, dx
@@:
      shl   ebx, cl        ; shift nodeID in ebx to the right position
      or    ebp, ebx       ; fill in NodeID in Target_List for the current bus range pointed to by cl
      mov   cx, 3          ; get shift index in cl ready for the next 32-bus range
      add   dx, cx         ; update bit-pointer for next field in Target_List (3-bit for each NodeID field)

      ror   ecx, 16t       ; swap hi/low words in ecx. CX now = total bus# range of all sockets
      add   cx,  32t       ; keep count of total bus# range size of all sockets accumulated so far
      rol   ecx, 16t

      dec   al             ; done for this node ID?
      jnz   short @b       ; no, loop back
      rol   edx, 16t       ; swap hi/low words in edx. DX now = node ID

      ; build Uncore Bus# array in edi
      mov   ebx, ecx
      shr   ebx, 16t       ; bx = total bus # range accumulated so far for all sockets
      dec   ebx            ; bl = Uncore bus# for current nodeID (ex. 3Fh)
   
      ; following code is commented out to remove the option of uncore bus# = IIO bus# +1
      ;movd  eax, xmm4      ; get original SSR data
      ;;test  eax, BIT24    ; bit24 Uncore Bus Policy
      ;test  eax, 01000000h ; bit24 Uncore Bus Policy
      ;jnz   short @f      
      ;mov   ebx, esi       ; Uncore Bus# = IIO Bus# +1 
      ;and   bl, 0ffh
      ;inc   bl             ; bl = Uncore Bus# = IIO Bus# +1 (ex. 41h) 

      or    edi, ebx        ; fill uncore bus# in edi
    
next_node_id:
      ror   esi, 8         ; rotate esi for next node ID
      ror   edi, 8         ; rotate edi for next node ID
      cmp   dx, 3          ; check current node id
      jg    store_socket4_7_info
      movd  xmm4, esi      ; save iio bus info
      movd  xmm5, edi      ; save uncore bus info

      ;
      ; esi = temporary IIO Bus # array [0..3, or 4..7], one byte for each node ID    
      ; edi = temporary Uncore Bus # array [0..3, or 4..7], one byte for each node ID
      ;
      jb    data_saved     ; Scanning at Socket 3? 
      xor   esi, esi       ; Yes, clear esi edi before using esi edi again for Socket 4-7
      xor   edi, edi       
      jmp   data_saved

store_socket4_7_info:
      movd  xmm6, esi      ; save iio bus info
      movd  xmm7, edi      ; save uncore bus info
data_saved:
      inc   dx             ; inc Node ID in dx
      cmp   dx, MAX_CPU_SOCKETS          ; max 8 sockets/nodes
      jb    check_node_present

      movd  xmm2, ebp      ; save mmcfg target list

   ; xmm2 = calculated MMCFG_TARGET_LIST 
   ; xmm3 = mmcfg base [31:26]
   ; xmm4/6 = calculated IIO Bus # array for socket0:3/4:7
   ; xmm6/7 = calculated Uncore Bus # array for socket0:3/4:7
   ; ecx[31:16] = total bus range for all sockets (64,128,256); 

   ; calculate MMCFG_RULE using edi
      ; get MMCG Base from SSR03[29:24]
      mov   eax, SSR_LEGACY_BUS_CONFIG_INFO_CSR
      READ_PCI_DWORD_LEGACY
      shl   ebx, 2          ; make at 26:31
      mov   edi, ebx
      and   edi, 0fc000000h ; clear other bits
            
      mov   edx, ecx
      shr   edx, 16t       ; dx = total bus range for all sockets (64/128/192/256)
      cmp   dx, 128t
      jbe   @f
      mov   dx, 256t      ; if more than 128, such as 192 for 3S, round up to 256   
 @@:  shr   dx, 7          ; dx[1:0] = 10b/01b/00b 
      not   dl             ; 
      and   dl, 11b        ; dl[1:0] = 01b/10b/11b
      dec   dl             ; dl[1:0] = 00b/01b/10b 
      shl   dl, 1          ; dl[2:1] = 00b/01b/10b  for MMCFG_RULE[2:1]
      or    dl, BIT0       ; Enable = 1
      movzx edx, dl
      or    edi, edx       ; edi = final value for MMCFG_RULE

      movd  xmm3, edi      ; xmm3 <- MMCFG_RULE

Write_cfg_scheme_to_CSRs:
      ; xmm1  [7:0]= legact socket id,  [27:8]= CPU Family/stepping info from cpuid instruction
      ; xmm2 = MMCFG_TARGET_LIST[23:0], local socket id [31:24]
      ; xmm3 = MMCFG_RULE
      ; xmm4 = IIO Bus # array for socket0 - 3
      ; xmm5 = Uncore Bus # array for socket0 - 3
      ; xmm6 = IIO Bus # array for socket4 - 7
      ; xmm7 = Uncore Bus # array for socket4 - 7

      ; Read socketId from CSR
      mov   eax, CSR_LEGACY_SAD_CONTROL_CBO
      READ_PCI_DWORD_LEGACY                           ; ebx = reg dword data
      mov   bh, bl                                    ; nodeId encoding: SASS
      and   bl, 00011b
      and   bh, 01000b
      shr   bh, 1
      or    bl, bh                                    ; local socketId SSS in bl
      cmp   bl, 7                                     ; socket id must be 0..7
      jg    EnableAccessCsr_fatal_error               ; socket must be 0 - MAX_SOCKET_ID

      movzx edx, bl
      movd  eax, xmm2                                 ; save SocketID in xmm2[31:24]
      rol   eax, 8
      or    al, dl
      ror   eax, 8
      movd  xmm2, eax
      shl   edx, 16t                                  ; save SocketID in edx[23:16]
   
      ; Initialize  CPUBUSNO register in UBOX
      mov   cl,  bl                                   ; cl = socketId
      cmp   cl,  4                                    ; check is socket 4 - 7
      jge   is_socket4_7

      shl   cl,  3                                    ; cl = socketId * 8
      movd  ebx, xmm4                                 ; iioBusNum   
      shr   ebx, cl                                   ; ebx[7:0] = IIO Bus Number
      and   ebx, 000000ffh                            ; clear other bits
      movd  eax, xmm5                                 ; uncoreBusNum   
      jmp   set_bus_num

is_socket4_7:
      sub   cl,  4
      shl   cl,  3                                    ; cl = socketId * 8
      movd  ebx, xmm6                                 ; iioBusNum   
      shr   ebx, cl                                   ; ebx[7:0] = IIO Bus Number
      and   ebx, 000000ffh                            ; clear other bits
      movd  eax, xmm7                                 ; uncoreBusNum   

set_bus_num:
      shr   eax, cl                                   ; eax[7:0] = uncore bus #
      shl   eax, 8
      and   eax, 0000ff00h                            ; clear other bits
      or    ebx, eax                                  ; ebx [15:8] = uncore bus, [7:0] = IIO bus
      mov   ecx, ebx                                  ; save in ecx. ch = uncoreBus#, cl = iioBus#
      or    ebx, BIT31                                ; Valid = 1
      mov   eax, CSR_LEGACY_CPUBUSNO_UBOX             ; write CSR with dword in ebx
      WRITE_PCI_DWORD_LEGACY

;-------------------------------------------------------------------------------
; From this point on, valid matching bus# must be used to access all CSRs.
;-------------------------------------------------------------------------------
      ; edx [18:16] = socketID
      ; ecx [15:8] = uncore bus#, [7:0] = IIO bus#

      ; Initialize  MMCFG_Target_List register that matches the bus# assignment above
      mov   eax, CSR_LEGACY_MMCFG_TARGET_LIST_CBO
      and   eax, 0ff00ffffh                     ; clear bus# field [23:16]
      movzx ebx, ch                             ; ebx[7:0]   = uncore bus#
      shl   ebx, 16t                            ; ebx[23:16] = uncore bus#
      or    eax, ebx                            ; eax[23:16] = correct uncore bus#
      movd  ebx, xmm2                           ; ebx[23:0]  = mmcfg Target List
      and   ebx, 00ffffffh                      
      WRITE_PCI_DWORD_LEGACY

      ; Initialize and enable MMCFG_Rule register
      ; a. init Cbo MMCFG_RULE
      mov   ax, CSR_LEGACY_MMCFG_RULE_CBO AND 0000FFFFh  ; keep using the same bus# field in eax[23:16]
      movd  ebx, xmm3                           ; ebx = mmcfgRule
      or    ebx, 1                              ; set enable bit
      WRITE_PCI_DWORD_LEGACY
      and   ebx, (BIT2+BIT1+BIT0)               ; keep mmcfgRule bus length and enable bits [2:0]
      mov   edi,  ebx                           ; save in edi
      ; b. init Ubox MMCFG_RULE
      mov   ax, CSR_LEGACY_MMCFG_RULE_UBOX AND 0000FFFFh
      READ_PCI_DWORD_LEGACY
      and   ebx, NOT (BIT2+BIT1+BIT0)           ; clear bit2:0
      or    ebx, edi                              
      WRITE_PCI_DWORD_LEGACY                    

; ------------------------------------------------------------------
; When all BSPs are done with this step, remote CSR access among sockets is enabled. Valid bus number must be used to
; match the bus number of the target socket. Access to bus0  will be routed to Legacy Socket no matter which socket
; issues the access. 
; ------------------------------------------------------------------

      movd  edi, xmm3
      and   edi, 0fffffff0h                     ; edi = MMCFG_BASE_ADDR

      ; edi = MMCFG_BASE_ADDR
      ; edx [18:16] = socketID
      ; ecx [15:8] = uncore bus, [7:0] = IIO bus

      ; init CPUNODEID CSR
      mov   eax, CSR_CPUNODEID_UBOX
      and   eax, 000fffffh                      ; clear bus# field [27:20]
      movzx ebx, ch                             ; ebx[7:0]   = uncore bus#
      shl   ebx, 20t                            ; ebx[27:20] = uncore bus#
      or    ebx, eax                            ; [23:16] = correct bus#
      add   ebx, edi                            ; ebx = MMCFG_BASE + reg addr
      mov   eax, dword ptr ds:[ebx]             ; read CSR
      and   eax, NOT (07h)                      ; clear bit[2:0]
      ror   edx, 16t                            ; dl[2:0]= my socket ID
      or    al, dl                              ; al[2:0]= my socket ID
      rol   edx, 16t
      mov   dword ptr ds:[ebx], eax             ; write back CSR

      ; init CSR_GLOBAL_PKG_C_S_CONTROL_PCU CSR in PCU
      ;  Note: for a system without Node Controller, legacy socket is always treated as Master,
      ;        which matches the default value (0) of MASTER_NID field 
      movd  eax, xmm1                           ; get legacy node id
      mov   dh, al                              ; save it in dh
      mov   eax, CSR_GLOBAL_PKG_C_S_CONTROL_PCU AND 000fffffh  ; clear bus# field [27:20]
      and   ebx, 0fff00000h                     ; keep using the same uncore bus#
      or    ebx, eax                            ; ebx = MMCFG_BASE + reg addr
      mov   eax, dword ptr ds:[ebx]             ; read CSR into eax
      and   eax, NOT (0700h)                    ; clear MY_NID bit[10:8]
      ror   edx, 8t                             ; dx[10:8] = my socket ID
      or    ah, dh                              ; ax[10:8] = my socket ID
      rol   edx, 8t
      cmp   ah, dh                              ; my socket Id == legacy socket ID?
      jnz   short @f                            ; no, skip
      or    eax, BIT2                           ; set AM_I_MASTER=1 if legacy socket
@@:   mov   dword ptr ds:[ebx], eax             ; write back CSR

      ; init CPU_BUS_NUMBER CSR in PCU2
      mov   eax, CSR_CPU_BUS_NUMBER_PCU2 AND 000fffffh  ; clear bus# field [27:20]
      and   ebx, 0fff00000h                     ; keep using the same uncore bus#
      or    ebx, eax                            ; ebx = MMCFG_BASE + reg addr
      mov   eax, dword ptr ds:[ebx]             ; read CSR into eax
      mov   ax, cx                              ; eax = [15:8] = uncore bus, [7:0] = IIO bus
      mov   dword ptr ds:[ebx], eax             ; write back CSR

      ; save config address to esi for later use
      mov   esi, ebx                            ; esi = config address of CSR_CPU_BUS_NUMBER_PCU2 on uncore bus#

      ; init CPUBUSNO CSR in IIO
      mov   eax, CSR_CPUBUSNO_IIO AND 000fffffh  ; clear bus# field [27:20]
      movzx ebx, cl                             ; ebx[7:0]   = iio bus#
      shl   ebx, 20t                            ; ebx[27:20] = iio bus#
      or    ebx, eax                            ; [23:16] = correct bus#
      add   ebx, edi                            ; ebx = MMCFG_BASE + reg addr
      mov   eax, dword ptr ds:[ebx]             ; read CSR
      mov   ax, cx                              ; eax = [15:8] = uncore bus, [7:0] = IIO bus
      or    eax, BIT16                          ; set Valid = 1
      mov   dword ptr ds:[ebx], eax             ; write back CSR

      ; init MMCFG CSR in IIO

      movd  eax, xmm1                           ; xmm1[27:8] = cpu family/stepping info from cpuid instruction
      shr   eax, 8
      and   eax, 000fffffh                      ; eax[19:0] = cpu family/stepping info from cpuid instruction
      cmp   eax, (CPU_FAMILY_HSX SHL 4) OR A0_REV_HSX    ; HSX-A0 stepping?
      mov   eax, CSR_MMCFG_IIO_A0  AND 000fffffh  ; clear bus# field [27:20]
      mov   edx, CSR_MMCFG_IIO_BASE_LIMIT_GAP_A0  ; offset for CSR_MMCFG_IIO_limit
      jz    @f                                  ; Yes, HSX-A0,BDX
      mov   eax, CSR_MMCFG_IIO  AND 000fffffh   ; No. clear bus# field [27:20] (HSX-B0 Update: Change CSR_MMCFG_IIO_A0 to CSR_MMCFG_IIO)
      mov   edx, CSR_MMCFG_IIO_BASE_LIMIT_GAP   ; offset for CSR_MMCFG_IIO_limit
@@:
      and   ebx, 0fff00000h                     ; keep using the same IIO bus#  
      or    ebx, eax
      mov   dword ptr ds:[ebx], edi             ; write back CSR

      ; clear higher 32-bit of base address if HSX-B0 or later
      cmp   edx, CSR_MMCFG_IIO_BASE_LIMIT_GAP_A0 ; HSX-A0?
      je    @f                                   ; yes, skip
      mov   dword ptr ds:[ebx+4], 0              ; Clear higher 32-bit of mmcfg base address in IIO
@@:
      ; calculate mmcfg limit = mmcfg_base + Max_Bus * 1MB)
      movd  ecx, xmm3                           ; ecx = copy of MMCFG_RULE
      and   ecx, 0110b
      shr   cl, 1                               ; cl = max bus number encoding. 00: 256, 01: 128, 10: 64
      mov   eax, 0100h                          ; eax = 256 (buses)
      shr   eax, cl                             ; eax = 256 / 128 / 64 buses
      shl   eax, 20t                            ; eax = eax * 100000h (i.e.Max_Bus * 1MB) 
   
      add   eax, edi                            ; eax = MMCFG_BASE_ADDR + MMCFG_SIZE
      sub   eax, 1                              ; last decode byte is last byte in previous 64MB block
      and   eax, 0fc000000h                     ; 64MB block address of last block of MMCFG
      add   ebx, edx                            ; CSR_MMCFG_IIO + gap  (MMCFG.Limit) 
      mov   dword ptr ds:[ebx], eax             ; write back CSR

      ; save config address to edi for later use
      mov   edi, ebx                            

      ; esi = config address of CSR_CPU_BUS_NUMBER_PCU2 on uncore bus#
      ; edi = config address of CSR_MMCFG_IIO+4 on IIO bus# for HSX A0, CSR_MMCFG_IIO+8 for HSX B0 and Above
      ; esi[27:20] = uncore bus#
      ; edi[27:20] = IIO bus#

      ; Check CPU stepping and perform stepping-specific tasks

      ; Set the new "Valid" bit in CPU_BUS_NUMBER register of PCU
      ;mov   eax, CSR_CPU_BUS_NUMBER_PCU2 AND 000fffffh  ; clear bus# field [27:20]
      ;and   esi, 0fff00000h                     ; keep using the same uncore bus#
      ;or    esi, eax                            ; esi = register config addr
      mov    eax, dword ptr ds:[esi]             ; read the CSR
      or     eax, BIT31                          ; "Valid" bit = 1
      mov    dword ptr ds:[esi], eax             ; write the CSR

; CPL3 Change Start - Send SET_CORE_RING_RATIO (0xA4) B2P Command in cold boot path for C0 stepping onwards for HSX-EP and A1 stepping onwards for BDX-EP.
      mov   eax, 1
      cpuid
      and eax, 0fffffh
      cmp eax, (CPU_FAMILY_HSX shl 4)+ STEPPING_ID_C0  ; Check for HSX C0  (0x306F2)
      jb EndCpl3
      mov ebx, eax				    ; Preserve CPUID info in EBX, before losing stepping info from EAX
      shr eax, 4
      cmp eax, CPU_FAMILY_HSX                       ; HSX CPU?
      je  StartCpl3
      cmp eax, CPU_FAMILY_BDX_DE                    ; BDX-DE CPU? This feature is not supported on BDX-DE
      je  EndCpl3
      cmp eax, CPU_FAMILY_BDX                       ; BDX CPU?
      jne EndCpl3
      cmp ebx, (CPU_FAMILY_BDX shl 4)+ A0_REV_BDX   ; Check for BDX A0  (0x406F0)
      ja  StartCpl3
      mov eax, CSR_CAPID1_HSX_BDX            ; If not a BDX-EP B0 or above, check for A1 stepping (CAPID1_CSR[20]=1), as this B2P command is supported only from A1 stepping onwards.
      and eax, 0fffffh
      mov ebx, esi
      and ebx, 0fff00000h
      or  eax, ebx
      test  dword ptr ds:[eax], BIT20
      jz EndCpl3
 StartCpl3:
      ;check for warm reset
      mov eax, CSR_STICKYSCRATCHPAD07
      and eax, 0fffffh
      mov ebx, esi
      and ebx, 0fff00000h
      or  eax, ebx
      test  dword ptr ds:[eax], BIT5
      jz EndCpl3
      ;Construct SET_CORE_RING_RATIO command with actual P1 ratio and max uncore ratio
      mov ecx, 0ceh	   ; Read PLATFORM_INFO MSR (0xCE) that contains max non turbo ratio
      rdmsr
      mov ebx, eax
      and ebx, 0ff00h      ; PLATFORM_INFO[15:8] = Max Non Turbo Ratio
      mov ecx, 0620h       ; Read UNCORE_RATIO_LIMIT MSR (0x620) that contains max LLC/Ring Ratio
      rdmsr
      and eax, 0ffh        ; UNCORE_RATIO_LIMIT[7:0] = Max LLC/Ring Ratio
      shl eax, 16
      mov ecx, 0800000a4h  ; Store the Set Core Ring Ratio command data in ECX
      or  ecx, eax
      or  ecx, ebx
      mov eax, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_HSX
      and eax, 0fffffh
      mov ebx, esi
      and ebx, 0fff00000h
      or  eax, ebx
Loop2:
     ; PCU Mailbox Interface
      test  dword ptr ds:[eax], 080000000h     ; Check for RUN_BUSY bit
      jnz   Loop2
      mov   dword ptr ds:[eax], ecx            ; Request for P1 speed
Loop1:
      test   dword ptr ds:[eax], 080000000h    ; Check for RUN_BUSY bit
      jnz   Loop1
EndCpl3:

; CPL3 Change End




eac_exit:
     RET_ESI_MMX4                              ; return


EnableAccessCsr_fatal_error:      ; fatal errors
      ;  debug - write an error code to PORT80()
      PORT80(0CCh)
      jmp  $                         ; dead loop

EnableAccessCSR    ENDP



;-------------------------------------------------------------------------
; CPU Early Workaround
;Executed by all package BSPs 
; Input: 
;    Stack available
; Output:
;    All general purpose registers destroyed
;-------------------------------------------------------------------------
ProcessorWorkaround PROC    NEAR    PUBLIC

  ; HSX 4165788 WA for PROCHOT - start
  pushad

  mov eax, 1
  cpuid
  cmp eax, (CPU_FAMILY_HSX SHL 4) OR A0_REV_HSX  
  jne done_hsx_wa           ; br if not HSX A0 or BDX DE,BDX EP grt than A0
  mov ecx, MSR_POWER_CTL
  rdmsr
  or  eax, BIT0 + BIT21
  wrmsr

done_hsx_wa: 
  popad

  ; HSX WA for PROCHOT - end

  call  _OEMCpuSecInit
  ret
ProcessorWorkaround  ENDP

;-------------------------------------------------------------------------
;
; Routine Description:
;
;    QPIEarly Workaround
;    Executed by all package BSPs, incl. SBSP. 
;    When contingency Silicon w/a are fixed by using MSR programming, it has to be done by each package BSP.
;    SBSP in QPI RC can access uncore CSR but not core MSR in remote packet BSP.
;    The current w/a is:
;         Program MSR as workarounds for Ax steppings.
;         SBSP writs the value into UBOX Sticky Scratch 07.
;         At subsequent warm resets, SBSP as well as all PBSPs will program MSRs as instructed.
;         Silicon fix will be added in B0 stepping and later.
;         See Brickland BIOS sighting 4029893 from Ivytown begeco 4539572 4539608
;        
;         Bit format for UBOX Sticky Scratch 07:
;        
;         [7]:       - 0 = to not program, 1 = to program MSR_AX_STEPPING_CBOBC
;         [9-8]:     - MSR_AX_STEPPING_CBOBC bit[1:0]
;         [10]:      - MSR_AX_STEPPING_CBOBC bit[3]
;         [11]:      - 0 = not program, 1 = program MSR_AX_STEPPING_R3QPI
;         [15-12]:   - MSR_AX_STEPPING_R3QPI bit[3:0]. If 2nd HA/MC fuse is set, bit[7:4] = bit[3:0]
;
;     Note: 
;         This code gets called at every reset, whether cold or warm reset. 
;         But for a sticky CSR, such as CSR_STICKYSCRATCHPAD07, the bits can only be seen as set at a subsequent warm reset following the original cold reset. 
;    
; Input: 
;
;    Stack available
;
; Output:
;
;    All general purpose registers destroyed
;
;-------------------------------------------------------------------------
QpiProgramMsrForWorkaound       PROC    NEAR    PUBLIC
        pushad

        call    GetLocalSktId                           ; CL = own local socket ID
        mov     eax, CSR_STICKYSCRATCHPAD07
        call    ReadCpuCsr                              ; EAX = current CSR data
        push    eax
        test    ax, BIT7
        jz      QpiNoLocHTCred

        mov     ebx, eax
        shr     eax, 8                                  ; bit[9:8] becomes bit[1:0]
        and     al, 03h                                
        shr     ebx, 7                                  ; bit[10] becomes bit[3]
        and     bl, 08h                                
        or      al, bl
        movzx   eax, al

        push    eax
        mov     ecx, MSR_A0_STEPPING_CBOBC
        ; rdmsr                                         ; Temporarily commmented out because the MSR is not yet available.
        pop     eax
        ; wrmsr                                         ; Temporarily commmented out because the MSR is not yet available.
        
QpiNoLocHTCred:
        pop     eax
        test    ax, BIT11
        jz      QpiNoCreditMerging

        shr     eax, 12                                  ; bit[15:12] becomes bit[3:0]
        and     al, 0Fh                                
        movzx   eax, al
        mov     ebx, eax
        shl     ebx, 4                                  ; bit[3:0] becomes bit[7:4]
        or      eax, ebx

        push    eax
        mov     ecx, MSR_A0_STEPPING_R3QPI
        ; rdmsr                                         ; Temporarily commmented out because the MSR is not yet available.
        pop     eax
        ; wrmsr                                         ; Temporarily commmented out because the MSR is not yet available.

QpiNoCreditMerging:

        popad
        ret
QpiProgramMsrForWorkaound  ENDP

;-------------------------------------------------------------------------
; Pass in the Over-Clocking PLL Ratio (MSR-61E Bits[3:2]) for 
; QPIRC via BIOS Non-Sticky Scratch Pad-2.
;
; Input:  None
;   
; Output: None
;
; Registers changed:
;   - CSR_BIOSSCRATCHPAD07 Bits[3:2]
;-------------------------------------------------------------------------
QpiPassInPllRatio PROC    NEAR
        pushad

        ; Assume Over-clocking feature is disabled
        mov     edx, 0                                  ; PLL Ratio (EDX) = 0

        ; Check actual status of Over-Clocking feature
        call    GetLocalSktId                           ; CL = own local socket ID
        mov     eax, CSR_CAPID4_HSX_BDX
        call    ReadCpuCsr
        test    eax, BIT28                              ; Over-clocking enabled?
        jz      QpiPassInPllRatio_StorePllRatio         ; If disabled, do not read PLL Ratio MSR - Just use default value

        ; Over-clocking enabled so get actual PLL Ratio (EDX)
        mov     ecx, MSR_OVER_CLOCK_PLL_RATIO           ; MSR only valid (readable) if Over-clocking is enabled
        rdmsr
        and     eax, (BIT1+BIT0)                        ; PLL Ratio in Bits[1:0]
        mov     edx, eax                                ; Save PLL Ratio for later use

QpiPassInPllRatio_StorePllRatio:
        ; Update scratch pad reg with PLL Ratio (EDX)
        mov     eax, CSR_BIOSNONSTICKYSCRATCHPAD2       ; CL already set to own local socket ID
        call    ReadCpuCsr
        mov     ebx, eax                                ; EBX = current Scratch Pad value

        and     ebx, NOT(BIT3+BIT2)                     ; Clear destination field within Scratch Pad (Bits[3:2])
        shl     edx, 2                                  ; Shift PLL Ratio into position
        or      ebx, edx                                ; Merge PLL Ratio into Scratch Pad
        mov     eax, CSR_BIOSNONSTICKYSCRATCHPAD2       ; CL already set to own local socket ID
        call    WriteCpuCsr                             ; EBX = current Scratch Pad and updated PLL Ratio

        popad
        ret
QpiPassInPllRatio ENDP                  


;-------------------------------------------------------------------------
; Read a Dword from PCI register 00h thru FFh using IO port CF8h/CFCh
; Input:
;   Stack is avaialble
;   
; Output: AL = SBSP Socket ID
; Registers changed: None except AL
;-------------------------------------------------------------------------
GetSbspSktId      PROC    NEAR
        movd eax, xmm1
        and  al, 7
        ret
GetSbspSktId      ENDP                  

;-------------------------------------------------------------------------
; Returns Socket/Packge ID of the current processor
; Input: 
;   Stack is avaialble
;   XMM registers initialized by EnableAccessCSR routine
;      XMM2[31:24] register contains local Socket ID
; Output: CL = Socket/Package ID of the current processor
; Registers changed: None except ecx
;-------------------------------------------------------------------------
GetLocalSktId      PROC    NEAR    PUBLIC

     movd  ecx, xmm2               ; xmm2[31:24] stores the socketID
     shr   ecx, 24t
     ret

GetLocalSktId      ENDP                  


;----------------------------------------------------------------------------
; SetupProcessorCheckinData
;  Executed by all socket BSPs.  Store data in its own local scratchpad register:
;       bit[0]        - check in flag
;       bit[23:8]   - ApicId  (only 16-bit is used)
;       bit[30:27] - stepping
;  Input:  Stack available
;          EnableAccessCSR has been called
;  Output: All general puropse registers preserved.
;----------------------------------------------------------------------------

SetupProcessorCheckinData    PROC    NEAR    PUBLIC

        pushad

        mov    eax, 1
        cpuid                                               ; ebx[31:24] = initial APIC ID, eax[3:0] = stepping
        and    ebx, 0ff000000h
        shr    ebx, 16t                                     ; APIC ID in ebx[23:8]
        and    eax, 0fh
        shl    eax, 27t                                     ; eax[30:27] = stepping
        mov    esi, eax                                     ; esi [30:27] = setpping
        push   ebx                                          ; save ebx

        ; check to see if processor is x2Apic capable
        mov    eax, 0
        cpuid
        pop    ebx                                          ; restore ebx
        cmp    eax, 0bh                                     ; check to see if processor is x2Apic capable
        jb     SaveDataAndSetCheckIn                        ; processor is not x2APIC capable
      
        ; processor is x2Apic capable, get ApicId from cpuid leaf B
        mov    eax, 0bh
        xor    ecx, ecx
        cpuid  
        movzx  ebx, dx                                      ; though X2Apid is 32bit wide,                             
        shl    ebx, 8                                       ; HW support up to 12 bits (bit31:12=0)

SaveDataAndsetCheckIn:
        ; ebx [23:8] = Apic ID,  esi [30:27] = stepping
        call   GetLocalSktId                                 ; CL = own local socket ID
        mov    eax, SR_PBSP_CHECKIN_CSR
        call   ReadCpuCsr                                    ; EAX = current CSR data
        and    eax, 870000ffh                                ; clear bit[30:27], [23:8]
        or     ebx, eax                                      ;  ebx <- apci id
        or     ebx, esi                                      ;  ebx <- stepping
        or     ebx, 1                                        ; set check in bit
        mov    eax, SR_PBSP_CHECKIN_CSR
        call   WriteCpuCsr

        popad
        ret
  
SetupProcessorCheckinData       ENDP



;-------------------------------------------------------------------------
; Non-SBSPs execute this routine to clear BSP flag in MSR and
; loop on a local scratch pad register for command from SBSP
; Input: None
;   
; Output: None
;-------------------------------------------------------------------------

ParkNbsp      PROC    NEAR

     ; clear bit8 BSP flag in MSR
     mov     ecx, MSR_APIC_BASE
     rdmsr
     and     eax, NOT BIT8                                 ; clear bit8 BSP flag in MSR
     wrmsr

     ; clear the dispatch CSR        
     call  GetLocalSktId                                   ; CL = Local Socketr ID 
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR   ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                        ; Clear the dispactch CSR
     call  WriteCpuCsr 

   ; When SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR becomes non-zero,
   ;       SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR will be the jmp location

SitAndSpin:
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR   ; Pipe Dispatch register in local socket
     call  ReadCpuCsr

     cmp   eax, 0
     jz    SHORT SitAndSpin

     ; RAS requirements START
     cmp   eax, PIPE_DISPATCH_COMMAND_TEAR_NEM
     jne   @f
     call  TearDownNEMandHalt     ; will never return

@@:
     cmp   eax, PIPE_DISPATCH_COMMAND_PREPARE_RESET
     jne   @f
     call  PrepareForReset        ; will never return

@@:
     ; RAS requirements END

     mov   eax, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR   ; Pipe Command register in local socket
     call  ReadCpuCsr
  
     cmp   eax, PIPE_DISPATCH_SYNCH_PSYSHOST
     jz    GoToAction

     ; clear out dispatch register because command is invalid
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR   ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                        ; Clear the dispactch CSR
     call  WriteCpuCsr 
     jmp   SitAndSpin
           
GoToAction:
   
     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR       ; Pipe Data register in local socket
     call  ReadCpuCsr
     mov   esp, eax                                        ; sync up stack pointer with SBSP
  
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR   ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                        ; Clear the dispactch CSR
     call  WriteCpuCsr 

WaitForJmpPointer:
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR   ; Pipe Dispatch register in local socket
     call  ReadCpuCsr

     cmp   eax, 0
     jz    SHORT WaitForJmpPointer

     mov  eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR
     call GetBusNum                                    ; update EAX[27:20] with the correct bus # 
     call GetMmcfgBase                                 ; esi = MMCFG Base address
     add  esi, eax                                     ; form cfg register address in esi
     and  esi, 0fffff000h                              ; keep only Bus:Dev:Func portion of it
  
     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR   ; Pipe Data register in local socket
     call  ReadCpuCsr

   ; EAX = Function pointer to PipeSlaveInit( UINT32 NodeId, UINT32 CfgAddrBDF) 
   ; Emulate a C-style function call
     push   esi                                             ; push input arg (PCIE Config Address w/o register offset)
     push   ecx                                             ; push input arg (local socketId)
     call   eax                                             ; call PipeSlaveInit( UINT32 NodeId, UINT32 CfgAddrBDF) 
                                                            ; Note: Control will not return to here
ParkNbsp      ENDP                  

;-------------------------------------------------------------------------
; Hot Plug CPU execute this routine to go to halt state.  So that SBSP can
; issue reset on this socket.  Clear the Command Dispatch register before
; going to halt, so that SBSP knows that PBSP executed this routine
;
; Input: None
;   
; Output: None
;-------------------------------------------------------------------------

PrepareForReset    PROC    NEAR    PUBLIC

    ; clear out dispatch register to communicate SBSP
    mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR   ; Pipe Dispatch register in local socket
    xor   ebx, ebx                                        ; Clear the dispactch CSR
    call  WriteCpuCsr 

loop1 :
    cli
    hlt
    jmp loop1     

PrepareForReset     ENDP


;-----------------------------------------------------------------------
; AcquireReleaseGlobalSemaphore
;-----------------------------------------------------------------------
;  Executed by a socket BSP to acquire ownership of the global semaphore
;  Input:  AL  1: Acquire    0: Release    
;          Stack available
;          EnableAccessCSR has been executed
;  Output: All general puropse registers preserved
;-----------------------------------------------------------------------

AcquireReleaseGlobalSemaphore    PROC    NEAR    PUBLIC

        pushad

        ; prepare data in ebx based on input CL, CH

        xor    ebx, ebx
        call   GetLocalSktId                                ; CL = local socket ID
        mov    bl, cl
        shl    ebx, 24t                                     ; ebx[26:24] = Requester NodeID
        or     al, al                                       ; Acquire or Release ?
        jz     short @f                                     ; Release Semaphore, keep bit1 = 0
        or     bl,  BIT1                                    ; set bit1 (Acquire)
@@:
        ; calculate the address of the BSP check-in scratch register (on SBSP socket)
 
        push   ecx
        call   GetSbspSktId                                 ; al = SBSP socket ID
        mov    cl, al
        mov    eax, CSR_SYSTEMSEMAPHORE0
        call   GetBusNum                                    ; update EAX[27:20] with the correct bus# 
        call   GetMmcfgBase                                 ; esi = MMCFG Base Address
        add    esi, eax                                     ; form cfg register address in esi
        pop    ecx

        ; Read/modify/write Global Semaphore CSR
        mov    eax, dword ptr ds:[esi]                      ; read CSR into eax
        and    eax, 0F8FFFFFDh                              ; only modify bit[26:24] and bit[1]
        or     eax, ebx
        mov    dword ptr ds:[esi], eax                      ; write back CSR
        test   al, BIT1                                     ; Release?
        jz     args_exit                                    ; yes, done and exit

AcquireLoop:
        ;  CL = local socket ID
        ;  Read Global Semaphore until CurrentNode = local socket ID
        mov    eax, dword ptr ds:[esi]                      ; read CSR into eax
        shr    eax, 2                                       ; shift CurrentNode bit[4:2] to bit [2:0]
        and    al, 07h
        cmp    al, cl
        jne    short AcquireLoop

args_exit:
        popad
        ret
  
AcquireReleaseGlobalSemaphore     ENDP



;-------------------------------------------------------------------------
; Get MMCFG-based PCI Config Address of a CSR register
; Input: 
;   Stack is NOT available
;   MMX6 = return address via RET_ESI_MMX6 macro
;   XMM registers initialized by EnableAccessCSR routine
;      XMM2 register [31:24] contains local Socket ID
;      XMM3 register contains MMCFG_RULE
;   XMM4/XMM5 registers contain the IIO/Uncore bus number array for socket0:3 initialized by EnableAccessCSR routine
;   XMM6/XMM7 registers contain the IIO/Uncore bus number array for socket4:7 initialized by EnableAccessCSR routine
;   EAX = PCIe config address format with bus# field defined as follows
;     EAX[31:28] = 0h
;     EAX[27:20] = BusType #  ( 0: IIO device,  1: Uncore device)
;     EAX[19:15] = Dev #
;     EAX[14:12] = Func #
;     EAX[11:0]  = Reg offset (dword aligned)
;   CL  = socketId (0,1,2,3) of the target CPU socket
; Output: EDI = Config address of the CSR
;         The caller can then use mov eax, dword ptr [edi] to access the CSR data
; Registers changed: eax, ebx, edi, esi
;-------------------------------------------------------------------------

GetCpuCsrAddrNoStack    PROC  PUBLIC

      mov   ebx, 0ff00000h                            
      cmp   cl, 3                                        ; socket 0 - 3?
      jg    is_s4_7                                      ; no, br

      and   ebx, eax                                     ; ebx[27:20] = bus type (0: IIO bus, 1: Uncore Bus) 
      movd  ebx, xmm4                                    ; ebx = IIO Bus# array
      jz    short @f                                     ; zf set/clear by above "and"
      movd  ebx, xmm5                                    ; ebx = Uncore Bus# array   
      jmp   @f

is_s4_7:
      sub   cl,  4                                       
      and   ebx, eax                                     ; ebx[27:20] = bus type (0: IIO bus, 1: Uncore Bus) 
      movd  ebx, xmm6                                    ; ebx = IIO Bus# array
      jz    short @f
      movd  ebx, xmm7                                    ; ebx = Uncore Bus# array   

@@:
      shl   cl,  3                                       ; cl = socketId * 8
      shr   ebx, cl                                      ; ebx[7:0] = bus #
      shr   cl,  3                                       ; restore cl
      and   ebx, 000000ffh                               ; clear other bits
      shl   ebx, 20t                                     ; ebx[27:20] = bus #
      and   eax, 000fffffh                               ; mask off bus type field in eax
      or    ebx, eax                                     ; replace it with proper bus #
      movd  edi, xmm3                                    ; xmm3 stores MMCFG_RULE
      and   edi, 0fffffff0h                              ; make sure the lower 4 bits are 0s
      add   edi, ebx

      RET_ESI_MMX6

GetCpuCsrAddrNoStack    ENDP


;-------------------------------------------------------------------------
; Read a Dword from PCIe config register using mmcfg method
; Input: 
;   EAX = PCIe config address format with bus# field defined as follows
;     EAX[31:28] = 0h
;     EAX[27:20] = BusType #  ( 0: IIO device,  1: Uncore device)
;     EAX[19:15] = Dev #
;     EAX[14:12] = Func #
;     EAX[11:0]  = Reg offset (dword aligned)
;   CL  = socketId (0,1,2,3,4,5,6,7) of the target CPU
;   XMM registers initialized by EnableAccessCSR routine
;      XMM2 register [31:24] contains local Socket ID
;      XMM3 register contains MMCFG_RULE
;   XMM4/XMM5 registers contain the IIO/Uncore bus number array for socket0:3 initialized by EnableAccessCSR routine
;   XMM6/XMM7 registers contain the IIO/Uncore bus number array for socket4:7 initialized by EnableAccessCSR routine
;   Stack is available
; Output: EAX = regiser dword data
; Registers changed: None except EAX
;-------------------------------------------------------------------------

ReadCpuCsr      PROC    NEAR    PUBLIC

      push  esi
      call  GetBusNum                                    ; update EAX[27:20] with the correct bus# for the socket ID
      call  GetMmcfgBase                                 ; esi = MMCFG Base Address
      add   esi, eax                                     ; form cfg register full address in esi
      mov   eax, dword ptr ds:[esi]                      ; read dword
      pop   esi
      ret

ReadCpuCsr ENDP




;-------------------------------------------------------------------------
; Write a Dword from PCIe config register using mmcfg method
; Input: 
;   EAX = PCIe config address format with bus# field defined as follows
;     EAX[31:28] = 0h
;     EAX[27:20] = BusType #  ( 0: IIO device,  1: Uncore device)
;     EAX[19:15] = Dev #
;     EAX[14:12] = Func #
;     EAX[11:0]  = Reg offset (dword aligned)
;   CL  = socketId (0,1,2,3) of the target CPU
;   XMM registers initialized by EnableAccessCSR routine
;      XMM2 register [31:24] contains local Socket ID
;      XMM3 register contains MMCFG_RULE
;   XMM4/XMM5 registers contain the IIO/Uncore bus number array for socket0:3 initialized by EnableAccessCSR routine
;   XMM6/XMM7 registers contain the IIO/Uncore bus number array for socket4:7 initialized by EnableAccessCSR routine
;   Stack is available
; Output: None
; Registers changed: None
;-------------------------------------------------------------------------

WriteCpuCsr PROC

      push  eax
      push  esi
      call  GetBusNum                                    ; update EAX[27:20] with the correct bus# for the socket ID
      call  GetMmcfgBase                                 ; esi = MMCFG Base Address
      add   esi, eax                                     ; form cfg register full address in esi
      mov   dword ptr ds:[esi], ebx                      ; read dword
      pop   esi
      pop   eax
      ret

WriteCpuCsr ENDP




;-------------------------------------------------------------------------
;  Update EAX[27:20] with the target Bus number ( IIO bus or  Uncore bus)
; Input:
;   EAX = PCIe config address format with bus# field defined as follows
;     EAX[31:28] = 0h
;     EAX[27:20] = BusType #  ( 0: IIO device,  1: Uncore device)
;     EAX[19:15] = Dev #
;     EAX[14:12] = Func #
;     EAX[11:0]  = Reg offset (dword aligned)
;   CL  = socketId (0,1,2,3) of the target CPU
;   XMM4/XMM5 registers contain the IIO/Uncore bus number array for socket0:3 initialized by EnableAccessCSR routine
;   XMM6/XMM7 registers contain the IIO/Uncore bus number array for socket4:7 initialized by EnableAccessCSR routine
; Output: EAX[27:20] updated with the actual bus # intended
; Registers changed: None except EAX[27:20]
;-------------------------------------------------------------------------
GetBusNum      PROC

      push  ebx
      push  ecx
      push  edx

      mov   edx, 0ff00000h                            
      ; if its socket4 - 7  ; try xmm6/7
      cmp   cl, 3
      jg    is_socket4_7
      movd  ebx, xmm4                                    ; ebx = IIO Bus# array
      and   edx, eax                                     ; edx[27:20] = bus type (0: IIO bus, 1: Uncore Bus) 
      jz    short @f
      movd  ebx, xmm5                                    ; ebx = Uncore Bus# array   
      jmp   @f

is_socket4_7:
      sub   cl,  4                                       
      movd  ebx, xmm6                                    ; ebx = IIO Bus# array
      and   edx, eax                                     ; edx[27:20] = bus type (0: IIO bus, 1: Uncore Bus) 
      jz    short @f
      movd  ebx, xmm7                                    ; ebx = Uncore Bus# array   

@@:
      shl   cl,  3                                       ; cl = socketId * 8
      shr   ebx, cl                                      ; ebx[7:0] = bus #
      and   ebx, 000000ffh                               ; clear other bits
      shl   ebx, 20t                                     ; ebx[27:20] = bus #
      and   eax, 000fffffh                               ; mask off bus type field in eax
      or    eax, ebx                                     ; replace it with proper bus #

      pop   edx
      pop   ecx
      pop   ebx
      ret

GetBusNum      ENDP




;-------------------------------------------------------------------------
; Get MMCFG base address
;  Input: 
;   Stack avaialble. 
;   XMM3 register contains MMCFG_RULE
;  Output: ESI = 32-bit MMCFG base address
;  Registers changed: None except ESI
;-------------------------------------------------------------------------
GetMmcfgBase   PROC     NEAR    PUBLIC

      ; PCIe config base address actually written into the MMCFG_RULE CSR
      movd  esi, xmm3                                       ; xmm3 stores MMCFG_RULE
      and   esi, 0fffffff0h                                 ; make sure the lower bits are 0s
      ret

GetMmcfgBase   ENDP


STARTUP_SEG ENDS

END

