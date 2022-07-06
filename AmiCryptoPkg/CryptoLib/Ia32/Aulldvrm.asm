;***********************************************************************
;*                                                                     *
;*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
;*                                                                     *
;*      All rights reserved. Subject to AMI licensing agreement.       *
;*                                                                     *
;***********************************************************************

;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;------------------------------------------------------------------------------
.386
.model  flat,C
.code

;------------------------------------------------------------------------------
; Divides a 64-bit unsigned value by another 64-bit unsigned value and returns
; the 64-bit result and the 64-bit remainder.
;  
;  On entry:
;      [ESP]    : Return Address
;      [ESP+4]  : QWORD. Dividend
;      [ESP+12] : QWORD. Divisor
;  On exit: 
;      EDX:EAX contains the quotient (dividend/divisor)
;      EBX:ECX contains the remainder (divided % divisor)
;
;  NOTE: this routine removes parameters from the stack.
;------------------------------------------------------------------------------
_aulldvrm    PROC
    mov     ecx, [esp + 16]             ; ecx <- divisor[32..63]
    test    ecx, ecx
    jnz     _@DivRemU64x64              ; call _@DivRemU64x64 if Divisor > 2^32
    jmp     _@DivRemU64x32				; call _@DivRemU64x32 if Divisor < 2^32
_aulldvrm    ENDP

_@DivRemU64x64  PROC PRIVATE
	push	esi
	push	edi
;   [ESP+12] : QWORD. Dividend
;   [ESP+20] : QWORD. Divisor
	
    mov     edx, dword ptr [esp + 16]
    mov     eax, dword ptr [esp + 12]   ; edx:eax <- dividend
    mov     edi, edx
    mov     esi, eax                    ; edi:esi <- dividend
    mov     ebx, dword ptr [esp + 20]   ; ecx:ebx <- divisor (ecx has been initialized before jumping to _@DivRemU64x64)
@@:
    shr     edx, 1
    rcr     eax, 1
    shrd    ebx, ecx, 1
    shr     ecx, 1
    jnz     @B
    div     ebx
    mov     ebx, eax                    ; ebx <- quotient
    mov     ecx, [esp + 24]             ; ecx <- high dword of divisor
    mul     dword ptr [esp + 20]        ; edx:eax <- quotient * divisor[0..31]
    imul    ecx, ebx                    ; ecx <- quotient * divisor[32..63]
    add     edx, ecx                    ; edx <- (quotient * divisor)[32..63]
    jc      @TooLarge                   ; product > 2^64
    cmp     edi, edx                    ; compare high 32 bits
    ja      @Correct
    jb      @TooLarge                   ; product > dividend
    cmp     esi, eax
    jae     @Correct                    ; product <= dividend
@TooLarge:
    dec     ebx                         ; adjust quotient by -1
    sub     eax, dword ptr [esp + 20]
    sbb     edx, dword ptr [esp + 24]   ; edx:eax <- (quotient - 1) * divisor
@Correct:
    sub     esi, eax
    sbb     edi, edx                    ; edi:esi <- remainder
    mov     eax, ebx                    ; eax <- quotient
    xor     edx, edx                    ; quotient is 32 bits long
    mov		ebx, edi                	; ebx <- Remainder[32..63]
	mov     ecx, esi                	; ecx <- Remainder[0..31]
	
	pop		edi
	pop		esi
    ret		16							; remove parameters from the stack
_@DivRemU64x64  ENDP

_@DivRemU64x32    PROC PRIVATE
    mov     ecx, [esp + 12]         ; ecx <- divisor
    mov     eax, [esp + 8]          ; eax <- dividend[32..63]
    xor     edx, edx
    div     ecx                     ; eax <- quotient[32..63], edx <- remainder
    push    eax
    mov     eax, [esp + 8]          ; eax <- dividend[0..31]
    div     ecx                     ; eax <- quotient[0..31]
    xor		ebx, ebx                ; ebx <- Remainder[32..63] (always zero)
	mov     ecx, edx                ; ecx <- Remainder[0..31]
    pop     edx                     ; edx <- quotient[32..63]
    ret		16						; remove parameters from the stack
_@DivRemU64x32    ENDP

END
