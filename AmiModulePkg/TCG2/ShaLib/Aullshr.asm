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
; Shifts a 64-bit unsigned value right by a certain number of bits
; and returns a 64-bit result.
;  
;  On entry:
;      EDX:EAX : QWORD value to be shifted
;      CL      : Number of bits to shift by
;  On exit: 
;      EDX:EAX contains shifted value
;------------------------------------------------------------------------------
_aullshr    PROC
    ; Handle 64 bit shifting or more
    cmp     cl, 64
    jae     _Exit

    ; Handle shifting between 0 and 31 bits
    cmp     cl, 32
    jae     More32
    shrd    eax, edx, cl
    shr     edx, cl
    ret

    ; Handle shifting of 32-63 bits
More32:
    mov     eax, edx
    xor     edx, edx
    and     cl, 31
    shr     eax, cl
    ret

    ; Shift on 64 bits or more is always 0
_Exit:
    xor     eax, eax
    xor     edx, edx
    ret
_aullshr    ENDP

END
