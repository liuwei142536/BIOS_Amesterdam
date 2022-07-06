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
; Shifts a 64-bit signed or unsigned value left by a certain number of bits
; and returns a 64-bit result.
;  
;  On entry:
;      EDX:EAX : QWORD value to be shifted
;      CL      : Number of bits to shift by
;  On exit: 
;      EDX:EAX contains shifted value
;------------------------------------------------------------------------------
_allshl    PROC
    ; Handle shifting of 64 or more bits (return 0)
    cmp     cl, 64
    jae     short ReturnZero

    ; Handle shifting of between 0 and 31 bits
    cmp     cl, 32
    jae     short More32
    shld    edx, eax, cl
    shl     eax, cl
    ret

    ; Handle shifting of between 32 and 63 bits
More32:
    mov     edx, eax
    xor     eax, eax
    and     cl, 31
    shl     edx, cl
    ret

ReturnZero:
    xor     eax,eax
    xor     edx,edx
    ret
_allshl    ENDP

END
