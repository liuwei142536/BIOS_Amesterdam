;*************************************************************************
;*************************************************************************
;**                                                                     **
;**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
;**                                                                     **
;**                       All Rights Reserved.                          **
;**                                                                     **
;**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
;**                                                                     **
;**                       Phone: (770)-246-8600                         **
;**                                                                     **
;*************************************************************************
;*************************************************************************
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
; Multiplies a 64-bit signed or unsigned value by a 64-bit signed or unsigned value
; and returns a 64-bit result.
;  
;  On entry:
;      [ESP]    : Return Address
;      [ESP+4]  : QWORD. Multiplicand
;      [ESP+12] : QWORD. Multiplier
;  On exit: 
;      EDX:EAX contains the product (Multiplicand*Multiplier)
;
;  NOTE: this routine removes parameters from the stack.
;------------------------------------------------------------------------------
_allmul    PROC
	push	ebx
;   [ESP+8]  : QWORD. Multiplicand
;   [ESP+16] : QWORD. Multiplier
    mov     ebx, [esp + 8]              ; ebx <- Multiplicand[0..31]
    mov     edx, [esp + 16]             ; edx <- Multiplier[0..31]
    mov     ecx, ebx
    mov     eax, edx
    imul    ebx, [esp + 20]             ; ebx <- Multiplicand[0..31] * Multiplier[32..63]
    imul    edx, [esp + 12]             ; edx <- Multiplier[0..31] * Multiplicand[32..63]
    add     ebx, edx                    ; carries are abandoned
    mul     ecx                         ; edx:eax <- Multiplicand[0..31] * Multiplier[0..31]
    add     edx, ebx                    ; carries are abandoned
	pop		ebx
	ret		16
_allmul    ENDP

END
