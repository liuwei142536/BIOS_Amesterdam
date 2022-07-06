;
; This file contains an 'Intel Pre-EFI Module' and is licensed
; for Intel CPUs and Chipsets under the terms of your license 
; agreement with Intel or your vendor.  This file may be      
; modified by the user, subject to additional terms of the    
; license agreement                                           
;
;------------------------------------------------------------------------------
;
; Copyright (c) 1999 - 2016, Intel Corporation. All rights reserved.<BR>
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
;  ProcessorStartupOem.asm
;
; Abstract:
;
;
;------------------------------------------------------------------------------
        .586P
        .XMM
	.MODEL  SMALL, c

        EXTERN  GetCpuCsrAddrNoStack:NEAR32

        include ProcessorStartupPlatform.inc
        include ProcessorStartupUncore.inc
        include ProcessorStartup.inc
        include token.equ

   .data

;-------------------------------------------------------
; Microcode Update region
;-------------------------------------------------------

; Microcode updates must be aligned on a paragraph boundary. If more
; than microcode update is included in the microcode storage block,
; each microcode update be forced to to align on paragraph boundary.
;align 16
;PUBLIC microcode_region_start
;PUBLIC microcode_region_size
; This is the microcode update region that will be searched and loaded
; by CPU reference code.

;microcode_region_start label byte
;    include ucode\M01106A4_0000000D.txt
;align 16
;    include ucode\M01106A4_0000000C.txt
;align 16
   ; Include the next microcode update here
   ;
;microcode_region_size EQU ($-microcode_region_start)  ; size of entire microcode update region above

;------------------------------------------------------------------
; ROM-based options. Values in each field can be customized by OEM
;------------------------------------------------------------------
nem_initializers   TEXTEQU  <CODE_REGION_BASE_ADDRESS,\
                             CODE_REGION_SIZE,\
                             DATA_STACK_BASE_ADDRESS,\
                             DATA_STACK_SIZE>

IFDEF MINIBIOS_BUILD
oem_cpu_rom_setup_options     cpu_rom_setup_options <\
         offset microcode_region_start,\                  ; uCode region base address
         microcode_region_size,\                          ; uCode region total size
         nem_initializers\                                ; NEM parameters
         >
ENDIF
;----------------------------------------------------------------------------
;       STARTUP_SEG  S E G M E N T  STARTS
;----------------------------------------------------------------------------
STARTUP_SEG SEGMENT PARA PUBLIC 'CODE' USE32
IFDEF MINIBIOS_BUILD
        EXTERN  GetCpuCsrAddrNoStack:NEAR32



;-----------------------------------------------------------------
;   Executed by SBSP and NBSP
;   Returns build-time platform options related to CPU early init
; Input:
;   Stack not available
;   mmx6 = routine return address
; Output:
;   ebp = pointer to structure cpu_rom_setup_option
;   All other registers destroyed
;-----------------------------------------------------------------

oem_get_cpu_rom_setup_options         PROC   PUBLIC

   ; OEM can override this default implementation

   mov   ebp, offset oem_cpu_rom_setup_options
   RET_ESI_MMX6                   ; return to caller

oem_get_cpu_rom_setup_options         ENDP
ENDIF


;-----------------------------------------------------------------
; This routine returns CPU-specific platform setup options/settings 
; to the caller via ECX register
;-----------------------------------------------------------------
;   Executed by SBSP and NBSP
; Input:
;   Stack not available
;   mmx5 = routine return address
;   SbspNbspGetPlatformCpuSettings PROC has been executed
;   MMCFG/CPUBUSNO has been configured so node BSPs can access CSRs on socket 0, and vice-versa
;   XMM registers initialized by EnableAccessCSR routine
;      XMM2 register [31:24] contains local Socket ID
;      XMM3 register contains MMCFG_RULE
;   XMM4,XMM6 registers contain the IIO bus number array initialized by EnableAccessCSR routine
;       XMM4: socket 0 - 3, XMM6: 4 - 7
;   XMM5,XMM7 registers contain the Uncore bus number array initialized by EnableAccessCSR routine
;       XMM5: socket 0 - 3, XMM7: 4 - 7
;  Output: 
;  Output: 
;    ECX[0]  - DCU_MODE select   0/1:   32KB 8-way non-ECC (hardware default) / 16KB 4-way ECC 
;    ECX[1]  - debugInterfaceEn   1:   Enable Debug Interface
;    ECX[31:2] reserved
;   All other general purpose registers destroyed
;-----------------------------------------------------------------

GetPlatformCpuSettings PROC      PUBLIC

   xor   edx, edx
   movd  ecx, xmm2                                ; xmm2[31:24] stores the socketID
   shr   ecx, 24t                                 ; CL = local socket ID
;
; DCU_MODE Select setup data is stored(copied) into sticky scratch pad register SSR07[24] on the warm boot path.
; Read the scratch pad register and returns the DCU_MODE setting to caller
;
   mov   eax, CSR_STICKYSCRATCHPAD07
   CALL_MMX6  GetCpuCsrAddrNoStack                 ;[edi] = MMCFG config address of the CSR
   mov   ebx, dword ptr ds:[edi]                   ; ebx = CSR data

   test  ebx, BIT5                                 ; "Comlepted Cold Reset Flow"?
   jz    short gdms_50                             ; no
   test  ebx,  BIT27                               ; ebx[27] = debugInterfaceEn info saved from cold boot path
   jz    @f
   or    edx, CPU_OPTIONS_BIT_DEBUG_INTERFACE_EN   ; return debugInterfaceEn = 1
@@:
   test  ebx,  BIT24                               ; ebx[24] = DCU Mode Select info saved from cold boot path
   jz    short gdms_50
   or    edx, CPU_OPTIONS_BIT_DCU_MODE_SEL         ; return DCU_MODE select = 1

gdms_50:
   mov   ecx, edx                                  ; return data in ecx
   RET_ESI_MMX5

GetPlatformCpuSettings ENDP


; Stub dummy hook for MiniBIOS implementation
_OEMCpuSecInit  PROC  PUBLIC
       ret

_OEMCpuSecInit  ENDP


FV_LENGTH       EQU (16 + 16)
FV_SIGNATURE    EQU (FV_LENGTH + 8)
FV_HDR_LENGTH   EQU (FV_SIGNATURE + 4 + 4)
FV_EXT_HDR_OFFSET EQU (FV_HDR_LENGTH + 2 + 2)

FV_EXT_HDR_SIZE EQU 16

FFS_TYPE        EQU (16 + 2)
FFS_LENGTH      EQU (FFS_TYPE + 1 + 1)
FFS_HDR_LENGTH  EQU (FFS_LENGTH + 3 + 1)

uCODE_CPU_SIGNATURE EQU 12
uCODE_CPU_FLAGS     EQU 24
uCODE_DATA_SIZE     EQU 28
uCODE_TOTAL_SIZE    EQU 32

ifndef MKF_MPDTable_CREATED
MKF_MPDTable_CREATED EQU 0
endif

IF MKF_Microcode_SUPPORT

MICROCODE_FFS_GUID label dword
    dd  17088572h
    dw  377Fh
    dw  44efh
    db  8Fh,4Eh,0B0h,9Fh,0FFh,46h,0A0h,70h

;public FindMicrocode
;extern FindMicrocodeEnd:NEAR32
FindMicrocode proc
IF  MKF_MICROCODE_SPLIT_BB_UPDATE
    mov     ebx, MKF_FV_MICROCODE_UPDATE_BASE
FindMicrocodeFv:
ELSE
    mov     ebx, MKF_FV_MICROCODE_BASE
ENDIF
    mov     edx, ebx
    cmp     dword ptr [edx + FV_SIGNATURE], 'HVF_'
    jne     Microcode_Not_Found             ;Corrupt FV?
    mov     eax, [edx + FV_HDR_LENGTH]
    and     eax, 0ffffh     ;Get 16 bit FV header length
    add     ebx,  eax       ;ebx = start of FFS
    jc      Microcode_Not_Found             ;Corrupt FV?

    movzx   eax, word ptr [edx + FV_EXT_HDR_OFFSET] ;Optional Header
    cmp ax, 0                               ;Optional exteneded header ?
    je  @f
    lea eax, [edx + eax]                    ;eax = extended header

    mov ecx, [eax + FV_EXT_HDR_SIZE]        ;ecx = extended header size
    mov ebx, eax
    add ebx, ecx
    jc  Microcode_Not_Found                 ;Corrupt FV?
@@:

    add     ebx, 7                          ;Align to 8-bytes for FFs
    and     ebx, not 7

    add     edx, [edx + FV_LENGTH]          ;edx = end of FV.
    jz      @f                              ;zf if Boot Block
    jc      Microcode_Not_Found             ;Corrupt FV?
@@:
    dec     edx                             ;edx = End of FFS

;---Find File ---
;ebx = Start of FFS
;edx = End of FFS
Get_Next_File:
    cmp     byte ptr [ebx + FFS_TYPE], 0f0h ;iqnore guid for pad type
    je  @f
    cmp     dword ptr [ebx], -1     ;Is in end of files, but not firmware volume.
    je      File_not_found
@@:
    mov     ecx, 4
    mov     esi, ebx
    mov     edi, offset MICROCODE_FFS_GUID
    ;compare file guid
    repe    cmpsd
    je      Found_File
;---get next file--
    ;get file length and add to ebx
    mov     eax, [ebx + FFS_LENGTH]
    and     eax, 0ffffffh           ;eax = File length (24 bits)
    add     ebx, eax                ;next file
    ;align file
    add     ebx, 7
    jc      Microcode_Not_Found     ;Check for wrap-around if bad data.
    and     ebx, not 7

    mov     eax, ebx
    add     eax, FFS_HDR_LENGTH     ;eax is microcode data
    cmp     eax, edx                ;Is end of firmware volume?
    jb      Get_Next_File
File_not_found:
    jmp     Microcode_Not_Found
;---Found File---
Found_File:
;ebx = Start of Microcode FFS
;edx = End of FFS + 1

;---Search micocode for match.---
    mov     edi, ebx

    mov     eax, [ebx + FFS_LENGTH]
    and     eax, 0ffffffh           ;eax = File length (24 bits)
    add     edi, eax                ;edi = end of file
    jc      Microcode_Not_Found     ;Check for wrap-around if bad data.

IF MKF_MPDTable_CREATED
    movzx   eax, word ptr [edi-2]           ;eax = MPDT length
    sub     edi, eax                        ;edi = end of microcodes.
ENDIF

    add     ebx, FFS_HDR_LENGTH
    jc      Microcode_Not_Found     ;Check for wrap-around if bad data.

    mov     esi, ebx

    mov     eax, 1
    cpuid
    mov     ebx, eax    ;ebx = CPU signature

    mov     ecx, 17h
    rdmsr               ;read CPUS flags

    shr     edx, 18
    and     dl, 7       ;dl = CPU FLAG
    mov     cl, dl
    mov     dl, 1
    shl     dl, cl      ;dl = bit set for CPU flag

    xchg    ebx, esi    ;ebx = ptr, esi = signature
Next_Microcode:
    cmp     ebx, edi                ;Check to see if microcode at end of FFS File.
    jae     Microcode_Not_Found

    cmp     dword ptr [ebx], 1 ;Check if correct header revision
    jne     Microcode_Not_Found

    mov     ecx, 2048   ;total size if data size in field = 0.
    cmp     dword ptr [ebx + uCODE_DATA_SIZE], 0
    jz      @f
    mov     ecx, [ebx + uCODE_TOTAL_SIZE]   ;Get total size.
@@:

    cmp     esi, [ebx + uCODE_CPU_SIGNATURE]
    jne     Check_Ext_Sig

    mov     eax, ebx                        ;Store uC addr in eax, so if jnz succeeds.
    test    dl, [ebx + uCODE_CPU_FLAGS]     ;uC flags match?
    jnz     Exit_Find_Microcode

Check_Ext_Sig:
    mov     ebp, [ebx + uCODE_TOTAL_SIZE]   ;Get total size.
    mov     eax, [ebx + uCODE_DATA_SIZE]    ;Get Data Size
    add     eax, 48
    cmp     ebp, eax
    jbe     Find_Next_Microcode             ;No extended signature.

    mov     ecx, [ebx + eax]                ;Get extended signature.
    cmp     ecx, 20
    jae     Microcode_Not_Found             ;Corrupt?
    lea     ebp, [ebx + eax + 20]           ;First Proc Signature.

@@:
    cmp     [ebp], esi                      ;Check Signature
    jne     Next_Ext_Sig

    mov     eax, ebx
    test    dl, [ebp + 4]                   ;Check processor flags
    jnz     Exit_Find_Microcode
Next_Ext_Sig:
    add     ebp, 12
    loop    @b
    mov     ecx, [ebx + uCODE_TOTAL_SIZE]   ;Get total size.
Find_Next_Microcode:
    ;align to next block size
IF MKF_PACK_MICROCODE
    add     ecx, 15
    and     ecx, 0fffffff0h
ELSE
    add     ecx, MKF_MICROCODE_BLOCK_SIZE - 1
    and     ecx, NOT (MKF_MICROCODE_BLOCK_SIZE - 1)
ENDIF
    add     ebx, ecx
    jc      Microcode_Not_Found             ;Overflow - bad data.
    jmp     Next_Microcode

Microcode_Not_Found:
IF  MKF_MICROCODE_SPLIT_BB_UPDATE
    cmp     ebx, MKF_FV_MICROCODE_BASE
    jae     @f
    mov     ebx, MKF_FV_MICROCODE_BASE
    jmp     FindMicrocodeFv
@@:
ENDIF
    xor     eax, eax
Exit_Find_Microcode:
jmp FindMicrocodeEnd
FindMicrocode endp

ENDIF ;MKF_Microcode_SUPPORT





;-----------------------------------------------------------------
; This routine returns the address of CPU microcode update
;-----------------------------------------------------------------
;   Executed by SBSP and NBSP
; Input:
;   Stack not available
;   mmx4 = routine return address
; Output:
;   EAX = Zero       No matching microcode found
;   EAX = Non-Zero   Linear address of CPU microcode update header that matches the current CPU
;   All other registers destroyed
;-----------------------------------------------------------------
OemFindMicrocode PROC      PUBLIC

   jmp FindMicrocode
FindMicrocodeEnd::

exit:
   RET_ESI_MMX4                   ; return to caller

OemFindMicrocode ENDP



STARTUP_SEG ENDS

END

