;
; This file contains an 'Intel Pre-EFI Module' and is licensed
; for Intel CPUs and Chipsets under the terms of your license 
; agreement with Intel or your vendor.  This file may be      
; modified by the user, subject to additional terms of the    
; license agreement                                           
;
;------------------------------------------------------------------------------
;
; Copyright (c) 1999 - 2017, Intel Corporation. All rights reserved.<BR>
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
;  ProcessorStartupCore.asm
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

        EXTERN  OemFindMicrocode:NEAR32
        EXTERN  GetPlatformCpuSettings:NEAR32
        EXTERN  GetLocalSktId:NEAR32
        EXTERN  GetBusNum:NEAR32

        EXTERN  PcdGet32 (PcdFlashSecCacheRegionBase):DWORD        
        EXTERN  PcdGet32 (PcdFlashSecCacheRegionSize):DWORD        
;----------------------------------------------------------------------------
;	STARTUP_SEG  S E G M E N T  STARTS
;----------------------------------------------------------------------------
STARTUP_SEG SEGMENT PARA PUBLIC 'CODE' USE32

LoadProcessorMicrocode      PROC    NEAR    PUBLIC
IFDEF ALLOW_TXT_DBG_UCODE
;5003493 BDX-DE NS: WA for debug version Microcode
; WA CODE BEGIN
        mov  esi, 0fed40000h  ; read TPM Status – drop the data
        mov  eax, dword ptr ds:[esi]
        mov  ecx, 3ah
        rdmsr
        test  eax, BIT0  ; check if LOCK bit set (warm reset)
        jnz  @f ; do not write MSR if so
        xor  eax, eax
        xor  edx, edx
        wrmsr  ; else write a value of 0x0 to MSR (0x3a)
@@:
        mov  ecx, 0c80h
        rdmsr
        test  eax, BIT0 ; check if DEBUG bit set (ITP installed)
        jnz  @f  ; do not write MSR if so
        mov  eax, 1
        xor  edx, edx
        wrmsr  ; else write a value of 0x1 to MSR (0xC80)
@@:
        mov  ecx, 2e6h
        xor  eax, eax
        xor  edx, edx
        wrmsr  ; write a value of 0x0 to MSR (0x2E6)
; WA CODE END
ENDIF ;;ALLOW_TXT_DBG_UCODE

  ;
  ; Load microcode on all pacakge BSPs.
  ; Input: Stack NOT available
  ;        MMX3 = routine return address

        CALL_MMX4  OemFindMicroCode
  
        or    eax,  eax                                 ; Microcode found?
        jz    MicrocodeLoadDone                         ; JIf not.

        mov   esi,  eax                                 ; ESI -> Microcode Update header.

	    cmp   [esi].UpdateHeaderStruc.dUpdateRevision, 0  ; Microcode version is negative?
        jl    short load_ucode                            ; yes, always load

   ; Read MSR(8Bh) to verify microcode version
        mov   ecx, MSR_IA32_BIOS_SIGN_ID
	xor   eax, eax                                  ; Clear EAX
	xor   edx, edx		                        ; Clear EDX
	wrmsr					        ; Write 0 to MSR 8Bh
	mov   eax, 1
	cpuid                                           ; this will update MSR 8Bh
 	mov   ecx, MSR_IA32_BIOS_SIGN_ID
	rdmsr			                        ; edx = current microcode signature/revision

	cmp   edx, [esi].UpdateHeaderStruc.dUpdateRevision
	jge   short MicrocodeLoadDone                   ; ucode patch already loaded. Skip.

load_ucode:
        mov   eax,  esi                                 ; eax => ucode buffer
        mov   ecx,  MSR_IA32_BIOS_UPDT_TRIG		; MSR 79h
        xor   edx,  edx
        add   eax,  48                                  ; EAX -> Microcode Update data.
        wrmsr                                           ; load the microcode

   ; Read MSR(8Bh) to verify that the microcode loading was successful
        mov   ecx, MSR_IA32_BIOS_SIGN_ID
	xor   eax, eax                                  ; Clear EAX
	xor   edx, edx		                        ; Clear EDX
	wrmsr					        ; Write 0 to MSR 8Bh
	mov   eax, 1
	cpuid                                           ; this will update MSR 8Bh
 	mov   ecx, MSR_IA32_BIOS_SIGN_ID
	rdmsr			                        ; edx = current microcode signature/revision

	cmp   edx, [esi].UpdateHeaderStruc.dUpdateRevision
	je    short MicrocodeLoadDone                   ; successful

MicrocodeLoadFailure:
	PORT80 (0CDh)					; failure - microcode loading faiure
	jmp	load_ucode


MicrocodeLoadDone:
        
        RET_ESI_MMX3
LoadProcessorMicrocode  ENDP



CheckGenuineIntelCpu PROC    NEAR    PUBLIC
  ; 
  ; Executes the CPUID instruction with EAX = 0, then reads 
  ; the EBX, ECX, and EDX registers to determine if the CPU is "GenuineIntel"
  ; 
        xor   eax, eax
        cpuid
        cmp   ebx, 756e6547h    ; "Genu"
        jne   short cgic_error
        cmp   ecx, 6c65746eh    ; "ntel"
        jne   short cgic_error
        cmp   edx, 49656e69h    ; "ineI"
        jne   short cgic_error

        RET_ESI_MMX3

cgic_error:
        ; Error: non-Intel processor found
        PORT80  (0CAh)
        jmp  $                         ; dead loop
;;        hlt
;;        jmp short cgic_error 

CheckGenuineIntelCpu ENDP



ConfigDfx      PROC    NEAR    PUBLIC
  ;
  ; Config package BSP CPU  DEBUG_INTERFACE_MSR.Enable bit.
  ;        Note:  BIOS is only allowed to write 1 to this bit (never 0).
  ;
  ; Input: Stack NOT available
  ;        MMX3 = routine return address
  ; 

        mov   eax, 1
        cpuid
        test  ecx, BIT11                                ; Is  DEBUG_INTERFACE_MSR supported?
        jz    ConfigDfxExit                             ; No

        CALL_MMX5  GetPlatformCpuSettings	        ; Get CPU specific settings from platform hook
        test  ecx, CPU_OPTIONS_BIT_DEBUG_INTERFACE_EN   ; 1 -> Enable DEBUG_INTERFACE 
        jz    ConfigDfxExit                             ; 0 -> Leave this MSR alone (as is)
        
        mov  ecx, MSR_IA32_DEBUG_INTERFACE              ; Read the MSR
        rdmsr
        test eax, BIT30                                 ; Lock bit set?
        jnz  ConfigDfxExit                              ; yes 
        test eax, BIT0                                  ; Enable bit set already?
        jnz  ConfigDfxExit                              ; yes
        
        or   eax, BIT0                                  ; set ENABLE bit = 1
        wrmsr                                           ; write back to MSR

ConfigDfxExit:
      
        RET_ESI_MMX3

ConfigDfx  ENDP



ConfigDcuMode      PROC    NEAR    PUBLIC
  ;
  ; Config package BSP CPU DCU_MODE (32KB 8-way v.s. 16KB 4-way ECC)
  ; This must be done before CR0.CD is changed from 1 to 0 for the first time after reset, i.e., before NEM init.
  ; Refer to DCU_MODE MSR in CPU BWG
  ;
  ; Input: Stack NOT available
  ;        MMX3 = routine return address
  ; 
        mov   ecx, MSR_PLATFORM_INFO         ; Check if CPU supports DCU Mode select
        rdmsr
        test  eax, (1 SHL 26)                ; DCU Mode Select supported?
        jz    short ConfigDcuModeExit        ; Skip if not
        
        CALL_MMX5  GetPlatformCpuSettings	 ; Get CPU specific settings from platform hook
        test  ecx, CPU_OPTIONS_BIT_DCU_MODE_SEL  ; 0/1 -> 32KB 8-way(HW default) / 16KB 4-way ECC
        jz    ConfigDcuModeExit              ; JIf zero

        mov   ecx,  MSR_DCU_MODE
        rdmsr                                ; read MSR_DCU_MODE
        or    al,   BIT0			         	; set DCU_MODE bit
        wrmsr

ConfigDcuModeExit:
      
        RET_ESI_MMX3

ConfigDcuMode  ENDP




NemInit      PROC    NEAR    PUBLIC
  ;
  ; Input: Stack NOT available
  ;        MMX3 = routine return address
  ;
  mov  ecx, MTRR_CAP
  rdmsr
  movzx  eax, al                        ; EAX = number of variable MTRR pairs
  lea  ebx, [4 * eax + FixedMtrrSize]   ; EBX = size of the MtrrInitTable table
                                        ; to program: 4 * VariableMtrrPairs + FixedMtrrSize
  xor  eax, eax                         ; EAX = 0
  xor  edx, edx                         ; EDX = 0

  GET_ABS_ADDR MtrrInitTable            ; ESI : location of MtrrInitTable 

InitMtrrLoop:
  sub  ebx, 2
  movzx  ecx, WORD PTR cs:[esi + ebx]      ; ECX <- address of MTRR to zero
  wrmsr
  jnz  InitMtrrLoop                     ; loop through the whole table

  ;
  ; Configure the default memory type to un-cacheable (UC) in the 
  ; IA32_MTRR_DEF_TYPE MSR.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  and     eax, NOT (00000CFFh)          ; Clear the enable bits and def type UC.
  wrmsr

  ; Configure MTRR_PHYS_MASK_HIGH for proper addressing above 4GB
  ; based on the physical address size supported for this processor
  ; This is based on read from CPUID EAX = 080000008h, EAX bits [7:0]
  ;
  ; Examples:
  ;  MTRR_PHYS_MASK_HIGH = 00000000Fh  For 36 bit addressing
  ;  MTRR_PHYS_MASK_HIGH = 0000000FFh  For 40 bit addressing
  ;
  mov   eax, 80000008h                  ; Address sizes leaf
  cpuid
  sub     al, 32
  movzx   eax, al
  xor     esi, esi
  bts     esi, eax
  dec     esi                           ; esi <- MTRR_PHYS_MASK_HIGH

  ;
  ; Configure the DataStack region as write-back (WB) cacheable memory type
  ; using the variable range MTRRs.
  ;

  ;
  ; Set the base address of the DataStack cache range
  ;
  mov     eax, DATA_STACK_BASE_ADDRESS OR MTRR_MEMORY_TYPE_WB
                                        ; Load the write-back cache value
  xor     edx, edx                      ; clear upper dword
  mov     ecx, MTRR_PHYS_BASE_0         ; Load the MTRR index
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Set the mask for the DataStack cache range
  ;
  mov     eax, DATA_STACK_SIZE_MASK OR MTRR_PHYS_MASK_VALID
                                        ; turn on the Valid flag
  mov     edx, esi                      ; edx <- MTRR_PHYS_MASK_HIGH
  mov     ecx, MTRR_PHYS_MASK_0         ; For proper addressing above 4GB
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Configure the BIOS code region as write-protected (WP) cacheable 
  ; memory type using a single variable range MTRR.
  ;
  ; Platform Specific - ensure region to cache meets MTRR requirements for 
  ; size and alignment.
  ;

  ;
  ; Set the base address of the CodeRegion cache range
  ;
  mov     eax, PcdGet32 (PcdFlashSecCacheRegionSize)
  mov     edi, PcdGet32 (PcdFlashSecCacheRegionBase)

  ;
  ; Round up to page size
  ;
  mov     ecx, eax                      ; Save
  and     ecx, 0FFFF0000h               ; Number of pages in 64K
  and     eax, 0FFFFh                   ; Number of "less-than-page" bytes
  jz      rounded
  mov     eax, 10000h                   ; Add the whole page size

rounded:
  add     eax, ecx                      ; eax - rounded up code cache size

  ;
  ; Define "local" vars for this routine
  ;
  CODE_SIZE_TO_CACHE    TEXTEQU  <mm4>
  CODE_BASE_TO_CACHE    TEXTEQU  <mm5>
  NEXT_MTRR_INDEX       TEXTEQU  <mm6>
  NEXT_MTRR_SIZE        TEXTEQU  <mm7>
  ;
  ; Initialize "locals"
  ;
  sub     ecx, ecx
  movd    NEXT_MTRR_INDEX, ecx          ; Count from 0 but start from MTRR_PHYS_BASE_1

  ;
  ; Save remaining size to cache
  ;
  movd    CODE_SIZE_TO_CACHE, eax       ; Size of code cache region that must be cached
  movd    CODE_BASE_TO_CACHE, edi       ; Base code cache address

nextMtrr:
  ;
  ; Get remaining size to cache
  ;
  movd    eax, CODE_SIZE_TO_CACHE
  and     eax, eax
  jz      done                          ; If no left size - we are done
  ;
  ; Determine next size to cache.
  ; We start from bottom up. Use the following algorythm:
  ; 1. Get our own alignment. Max size we can cache equals to our alignment
  ; 2. Determine what is bigger - alignment or remaining size to cache.
  ;    If aligment is bigger - cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ;    If remaining size to cache is bigger
  ;      Determine the biggest 2^N part of it and cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ; 3. End when there is no left size to cache or no left MTRRs
  ;
  movd    edi, CODE_BASE_TO_CACHE
  bsf     ecx, edi                      ; Get index of lowest bit set in base address
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 1h
  shl     edx, cl                       ; Alignment is in edx
  cmp     edx, eax                      ; What is bigger, alignment or remaining size?
  jbe     gotSize                       ; JIf aligment is less
  ;
  ; Remaining size is bigger. Get the biggest part of it, 2^N in size
  ;
  bsr     ecx, eax                      ; Get index of highest set bit
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 1
  shl     edx, cl                       ; Size to cache

gotSize:
  mov     eax, edx
  movd    NEXT_MTRR_SIZE, eax           ; Save

  ;
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  dec     eax                           ; eax - size to cache less one byte
  not     eax                           ; eax contains low 32 bits of mask
  or      eax, MTRR_PHYS_MASK_VALID     ; Set valid bit

  ;
  ; Program mask register
  ;
  mov     ecx, MTRR_PHYS_MASK_1         ; setup variable mtrr
  movd    ebx, NEXT_MTRR_INDEX
  add     ecx, ebx

  mov     edx, esi                      ; edx <- MTRR_PHYS_MASK_HIGH
  wrmsr
  ;
  ; Program base register
  ;
  sub     edx, edx
  mov     ecx, MTRR_PHYS_BASE_1         ; setup variable mtrr
  add     ecx, ebx                      ; ebx is still NEXT_MTRR_INDEX

  movd    eax, CODE_BASE_TO_CACHE
  or      eax, MTRR_MEMORY_TYPE_WP      ; set type to write protect
  wrmsr
  ;
  ; Advance and loop
  ; Reduce remaining size to cache
  ;
  movd    ebx, CODE_SIZE_TO_CACHE
  movd    eax, NEXT_MTRR_SIZE
  sub     ebx, eax
  movd    CODE_SIZE_TO_CACHE, ebx

  ;
  ; Increment MTRR index
  ;
  movd    ebx, NEXT_MTRR_INDEX
  add     ebx, 2
  movd    NEXT_MTRR_INDEX, ebx
  ;
  ; Increment base address to cache
  ;
  movd    ebx, CODE_BASE_TO_CACHE 
  movd    eax, NEXT_MTRR_SIZE
  add     ebx, eax
  movd    CODE_BASE_TO_CACHE, ebx 

  jmp     nextMtrr
done:
  ;
  ; Program the variable MTRR's MASK register for WDB
  ; (Write Data Buffer, used in MRC, must be WC type)
  ;
  mov     ecx, MTRR_PHYS_MASK_1
  movd    ebx, NEXT_MTRR_INDEX
  add     ecx, ebx
  mov     edx, esi                      ; edx <- MTRR_PHYS_MASK_HIGH
  mov     eax, WDB_REGION_SIZE
  and     eax, eax
  jz      noWdbInstalled
  sub     eax, 1
  not     eax
  or      eax, MTRR_PHYS_MASK_VALID
  wrmsr
  ;
  ; Program the variable MTRR's BASE register for WDB
  ;
  dec     ecx
  xor     edx, edx
  mov     eax, (WDB_BASE_ADDRESS AND 0FFFFF000h)
  or      eax, MTRR_MEMORY_TYPE_WC
  wrmsr

noWdbInstalled:
  ;
  ; Enable the MTRRs by setting the IA32_MTRR_DEF_TYPE MSR E flag.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  or      eax, MTRR_DEF_TYPE_E          ; Enable variable range MTRRs
  wrmsr

  ;
  ; Enable the logical processor's (BSP) cache: execute INVD and set 
  ; CR0.CD = 0, CR0.NW = 0.
  ;
  mov     eax, cr0
  and     eax, NOT (CR0_CACHE_DISABLE + CR0_NO_WRITE)
  invd
  mov     cr0, eax
  ;
  ; Enable No-Eviction Mode Setup State by setting
  ; NO_EVICT_MODE  MSR 2E0h bit [0] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 1
  wrmsr

  ;
  ; One location in each 64-byte cache line of the DataStack region
  ; must be written to set all cache values to the modified state.
  ;
  mov     edi, DATA_STACK_BASE_ADDRESS
  mov     ecx, DATA_STACK_SIZE / 64
  mov     eax, CACHE_INIT_VALUE
@@:
  mov     [edi], eax
  sfence
  add     edi, 64
  loopd   @b

  ;
  ; Enable No-Eviction Mode Run State by setting
  ; NO_EVICT_MODE MSR 2E0h bit [1] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 2
  wrmsr

  ;
  ; Finished with cache configuration
  ;

  ;
  ; Optionally Test the Region...
  ;

  ;
  ; Test area by writing and reading
  ;
  cld
  mov     edi, DATA_STACK_BASE_ADDRESS
  mov     ecx, DATA_STACK_SIZE / 4
  mov     eax, DWORD PTR CACHE_TEST_VALUE

TestDataStackArea:
  stosd
  cmp     eax, DWORD PTR [edi-4]
  jnz     DataStackTestFail
  loop    TestDataStackArea 
  jmp     DataStackTestPass

  ;
  ; Cache test failed
  ;
DataStackTestFail:
  PORT80 (0D0h)
  jmp     $

  ;
  ; Configuration test failed
  ;
ConfigurationTestFailed:
  PORT80 (0D1h)
  jmp     $

DataStackTestPass:

  ;
  ; At this point you may continue normal execution.  Typically this would include 
  ; reserving stack, initializing the stack pointer, etc.
  ;

  ;
  ; After memory initialization is complete, please follow the algorithm in the BIOS
  ; Writer's Guide to properly transition to a normal system configuration.
  ; The algorithm covers the required sequence to properly exit this mode.
  ;
        
        
        RET_ESI_MMX3
NemInit      ENDP


EstablishNemStack      PROC    NEAR    PUBLIC
        ; Establish stack
        ;
        ; Fill the stack with a recognizable pattern.
        ;
        mov   esp, DATA_STACK_BASE_ADDRESS
        add   esp, DATA_STACK_SIZE
        sub   esp, 40h
        mov   edi, DATA_STACK_BASE_ADDRESS
        cld
        mov   ecx, DATA_STACK_SIZE
        shr   ecx, 2
        mov   eax, 05AA55AA5h
        rep   stosd

        RET_ESI_MMX3
EstablishNemStack  ENDP

;-------------------------------------------------------------------------
; Hot Plug CPU execute this routine to tear down NEM.  This routine is a duplicate
; of the function, which is in PEI module for the regular boot flow.  
; In the hot-plug flow, we want PBSP to execute the code from the flash. 
; Clear the Command Dispatch register before going to halt, 
; so that SBSP knows that PBSP executed this routine
;
; Input: None
;   
; Output: None
;-------------------------------------------------------------------------

TearDownNEMandHalt      PROC    NEAR    PUBLIC

    call GetLocalSktId
    mov  eax, 000100000h        ; get uncore bus#
    call GetBusNum
    movd ebx, xmm3              ; get mmcfg base from xmm3[31:28]
    and  ebx, 0f0000000h        
    or   eax, ebx
    mov  edi, eax               ; save MMCFG + UncoreBus# in edi

    ; set CR0.CD bit to globally disable cache
    mov  eax, cr0
    or   eax, BIT30
    mov  cr0, eax

    ; invalid/flush cache
    invd

    ; clear Global MTRR Enable bit
    mov ecx, 02FFh
    rdmsr
    and eax, NOT (0800h)
    wrmsr

    ; clear No Eviction Mode Setup/Run bits
    mov ecx, 02E0h
    rdmsr
    btr eax, 1
    wrmsr
    btr eax, 0
    wrmsr

    ; Clear pipe dispatch command mailbox
    mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR
    and   eax, 0000FFFFFh   ; keep D:F:R field
    mov   dword ptr[edi+eax], 0 ; 
      
loop1:
    cli
    hlt
    jmp loop1

TearDownNEMandHalt     ENDP


MtrrInitTable LABEL WORD
  DW  MTRR_DEF_TYPE
  DW  MTRR_FIX_64K_00000
  DW  MTRR_FIX_16K_80000
  DW  MTRR_FIX_16K_A0000
  DW  MTRR_FIX_4K_C0000
  DW  MTRR_FIX_4K_C8000
  DW  MTRR_FIX_4K_D0000
  DW  MTRR_FIX_4K_D8000
  DW  MTRR_FIX_4K_E0000
  DW  MTRR_FIX_4K_E8000
  DW  MTRR_FIX_4K_F0000
  DW  MTRR_FIX_4K_F8000

FixedMtrrSize  EQU  ($ - MtrrInitTable)

  DW  MTRR_PHYS_BASE_0
  DW  MTRR_PHYS_MASK_0
  DW  MTRR_PHYS_BASE_1
  DW  MTRR_PHYS_MASK_1
  DW  MTRR_PHYS_BASE_2
  DW  MTRR_PHYS_MASK_2
  DW  MTRR_PHYS_BASE_3
  DW  MTRR_PHYS_MASK_3
  DW  MTRR_PHYS_BASE_4
  DW  MTRR_PHYS_MASK_4
  DW  MTRR_PHYS_BASE_5
  DW  MTRR_PHYS_MASK_5
  DW  MTRR_PHYS_BASE_6
  DW  MTRR_PHYS_MASK_6
  DW  MTRR_PHYS_BASE_7
  DW  MTRR_PHYS_MASK_7
  DW  MTRR_PHYS_BASE_8
  DW  MTRR_PHYS_MASK_8
  DW  MTRR_PHYS_BASE_9
  DW  MTRR_PHYS_MASK_9



;-------------------------------------------------------------------------
; TxtInit
;   LTsx-related CPU early init that must be executed by all package BSPs
;
; Input: 
;   AccessCSR proc has been executed
;   No Eviction Mode (NEM) is enabled, stack is available
; Output: 
;-------------------------------------------------------------------------

TxtInit     PROC PUBLIC

     TXT_PUBLIC_BASE        EQU  0FED30000h
     TPM_STATUS_REG_ADDRESS EQU  0FED40000h

    ;
    ; Check if CPU is fused for LTSX support
    ;
    mov     eax, 1
    cpuid
    test    ecx, BIT6
    jz      short TxtInitExit         ; no, skip

    ;
    ; Check bitmask at TXT_PUBLIC_BASE + 10h (each socket has a corresponding bit) 
    ;

    call   GetLocalSktId              ; CL = own socket ID (0, 1, 2, 3)
    mov    esi, TXT_PUBLIC_BASE
    mov    eax, dword ptr ds:[esi+10h]
    bt     eax, ecx
    jnc    short TxtInitExit          ; no, skip

    ;
    ;  Do a read to TPM Status (and drop the data)
    ;

    mov    esi, TPM_STATUS_REG_ADDRESS
    mov    eax, dword ptr ds:[esi]

    ;
    ;  Unlock memory by writing 0 to MSR 2E6h
    ;

    mov     ecx, 2E6h
    xor     eax, eax
    xor     edx, edx
    wrmsr

TxtInitExit:
    ret

TxtInit     ENDP



;-------------------------------------------------------------------------
; Read a Dword from PCI register 00h thru FFh using IO port CF8h/CFCh
; Assumptions: 
;   No Eviction Mode (NEM) is enabled, stack is available
; Input: 
;   EAX = legacy PCI address format
;   EAX[31:24] = 80h
;   EAX[23:16] = Bus #
;   EAX[15:11] = Dev #
;   EAX[10:8]  = Func #
;   EAX[7:0]   = Reg offset (dword aligned)
;   
; Output: EAX = regiser dword data
; Registers changed: None except EAX
;-------------------------------------------------------------------------

ReadPciLegacy PROC      PUBLIC

   push  edx
   mov   dx, 0cf8h
   out   dx, eax
   mov   dx, 0cfch
   in    eax, dx
   pop   edx
   ret

ReadPciLegacy ENDP


;-------------------------------------------------------------------------
; Write a Dword to a PCI register 00h thru FFh using IO port CF8h/CFCh
; Assumptions: 
;   No Eviction Mode (NEM) is enabled, stack is available
; Input: 
;   EAX = legacy PCI address format
;   EAX[31:24] = 80h
;   EAX[23:16] = Bus #
;   EAX[15:11] = Dev #
;   EAX[10:8]  = Func #
;   EAX[7:0]   = Reg offset (dword aligned)
;   EBX = dword data to be written
; Output: None
; Registers changed: None
;-------------------------------------------------------------------------

WritePciLegacy PROC     PUBLIC

   push  edx
   mov   dx, 0cf8h
   out   dx, eax
   mov   dx, 0cfch
   xchg  eax, ebx
   out   dx, eax
   xchg  eax, ebx
   pop   edx

   ret

WritePciLegacy ENDP
STARTUP_SEG ENDS

END

