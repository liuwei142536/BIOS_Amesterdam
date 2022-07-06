;*************************************************************************
;*************************************************************************
;**                                                                     **
;**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
;**                                                                     **
;**                       All Rights Reserved.                          **
;**                                                                     **
;**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
;**                                                                     **
;**                       Phone: (770)-246-8600                         **
;**                                                                     **
;*************************************************************************
;*************************************************************************

.686p
.xmm
.model small, c

include token.equ

FV_MAIN_BASE                  EQU 0FFFFFFFCh

ifndef MKF_CSM_SUPPORT
MKF_CSM_SUPPORT EQU 0
endif

EXTERN   SecStartup:NEAR

public SecPlatformLibEntryEnd
EXTRN SecPlatformLibEntry:NEAR

_TEXT_REALMODE      SEGMENT PARA PUBLIC USE16 'CODE'
                    ASSUME  CS:_TEXT_REALMODE, DS:_TEXT_REALMODE

align 4
_ModuleEntryPoint PROC NEAR C PUBLIC
	;---16 bit in 32 bit assembler--
	;---Some 16-bit and 32-bit assmebly is the same, others are not.---
	;---Need to use some machine code.---

    fninit              ;Clear any pending Floating point exceptions
    movd    mm0, eax    ;Save BIST state in MM0

	cli

if MKF_CSM_SUPPORT EQ -1
    mov     ax, 0F000h
    mov     ds, ax
    mov     si, 0FFF0h
    cmp     BYTE PTR [si], 0EAh   ; Is it warm reset ?
    jne     NotWarmReset                                    ; If not.

    db      0EAh                  ; Far jump to F000:E05B (legacy BIOS warm reset entry)
    dw      0E05Bh
    dw      0F000h
NotWarmReset:
endif

    ;Switch to protected mode
    mov  ebx, offset GdtDescriptor
    db 66h
    lgdt fword ptr cs:[bx]

    mov     eax, cr0                      ; Get control register 0
    or      eax, 00000003h                ; Set PE bit (bit #0) & MP bit (bit #1)
    mov     cr0, eax                      ; Activate protected mode
    mov     eax, cr4                      ; Get control register 4
    or      eax, 00000600h                ; Set OSFXSR bit (bit #9) & OSXMMEXCPT bit (bit #10)
    mov     cr4, eax

	;In 16 bit protected mode

    cld
    mov    ax, DATA_SEL
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    fs, ax
    mov    gs, ax

    ;set cs segment
    ;jmp 10:CHANGE_CS
    db  66h, 0eah
    dd  offset ProtectedModeSECStart
    dw  CODE_SEL
_ModuleEntryPoint endp
_TEXT_REALMODE      ENDS
;-----------------------------------------------------------------------------------------------;

;-----------------------------------------------------------------------------------------------;

_TEXT_PROTECTED_MODE      SEGMENT PARA PUBLIC USE32 'CODE'					;
                          ASSUME  CS:_TEXT_PROTECTED_MODE, DS:_TEXT_PROTECTED_MODE		;
;-----------------------------------------------------------------------------------------------;
align 4
;-----------------------------------------------------------------------------------------------;
;					ProtectedModeSECStart					;
;-----------------------------------------------------------------------------------------------;
ProtectedModeSECStart PROC NEAR PUBLIC

jmp	SecPlatformLibEntry
SecPlatformLibEntryEnd::

 	jmp  HandOffToNextModule
ProtectedModeSECStart ENDP

;-----------------------------------------------------------------------------;
HandOffToNextModule   PROC    NEAR	PUBLIC
TransferToSecStartup:

  ;
  ; Add for PI
  ;
  
  mov     eax, esp
  mov     ebx, 087655678h
  push    ebx
  push    eax
  mov     bx,  0
  push    bx
  lidt    fword  ptr [esp]        ;Load IDT register
  pop     bx
  pop     eax  

  ;
  ; Pass BFV into the PEI Core
  ;
  mov     edi, FV_MAIN_BASE             ; 0FFFFFFFCh
  push    Dword Ptr Ds:[edi]

  ;
  ; Pass TempRamBase into the PEI Core for PI
  ;
  push	  MKF_CAR_BASE_ADDRESS
  ;
  ; Pass stack size into the PEI Core
  ;
  push	  MKF_CAR_TOTAL_SIZE
	  

  ;
  ; Pass Control into the PEI Core
  ;
  call SecStartup

HandOffToNextModule   ENDP

align 16
GDT_BASE:
NULL_SEL	equ	$-GDT_BASE	 ;NULL Selector 0
		dd	0, 0

DATA_SEL	equ	$-GDT_BASE     	; Selector 8, Data 0-ffffffff 32 bit
	dd 0000ffffh
	dd 00cf9300h

CODE_SEL	equ	$-GDT_BASE     	; Selector 10h, CODE 0-ffffffff 32 bit
	dd 0000ffffh
	dd 00cf9b00h

; We only need this because Intel DebugSupport driver
; (RegisterPeriodicCallback function) assumes that selector 0x20 is valid
; The funciton sets 0x20 as a code selector in IDT
;
; To switch to 16 bit, Selectors SYS16_CODE_SEL and SYS16_DATA_SEL are used.
;
; System data segment descriptor
;
SYS_DATA_SEL        equ     $ - GDT_BASE               ; Selector [0x18]
	dd 0000FFFFh	;0 - f_ffff
	dd 00cf9300h	;data, expand-up, notwritable, 32-bit

; System code segment descriptor
SYS_CODE_SEL        equ     $ - GDT_BASE               ; Selector [0x20]
	dd 0000FFFFh	;0 - f_ffff
	dd 00cf9b00h	;data, expand-up, writable, 32-bit
SPARE3_SEL  equ $-GDT_BASE            ; Selector [0x28]
	dd 0, 0
SYS_DATA64_SEL    equ $-GDT_BASE          ; Selector [0x30]
	dd 0000FFFFh
	dd 00cf9300h
SYS_CODE64_SEL    equ $-GDT_BASE          ; Selector [0x38]
	dd 0000FFFFh
	dd 00af9b00h
SPARE4_SEL  equ $-GDT_BASE            ; Selector [0x40]
	dd 0, 0
GDT_SIZE	equ     $-GDT_BASE		;Size of Descriptor Table

GdtDescriptor:
	dw	GDT_SIZE - 1			; GDT limit
	dd  offset GDT_BASE	; GDT base	Relative to 4G.

_TEXT_PROTECTED_MODE    ENDS
END
