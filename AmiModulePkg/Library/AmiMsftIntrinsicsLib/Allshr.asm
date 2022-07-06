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
; Shifts a 64-bit signed value right by a certain number of bits
; and returns a 64-bit result.
;
;  On entry:
;      EDX:EAX : QWORD value to be shifted
;      CL      : Number of bits to shift by
;  On exit:
;      EDX:EAX contains shifted value
;------------------------------------------------------------------------------
_allshr    PROC
    ; Handle shifts of 64 bits or more (if shifting 64 bits or more, the result
    ; depends only on the high order bit of edx).
    cmp     cl,64
    jae     short SIGNRETURN

    ; Handle shifts of between 0 and 31 bits
    cmp     cl, 32
    jae     short MORE32
    shrd    eax,edx,cl
    sar     edx,cl
    ret

    ; Handle shifts of between 32 and 63 bits
MORE32:
    mov     eax,edx
    sar     edx,31
    and     cl,31
    sar     eax,cl
    ret

    ; Return double precision 0 or -1, depending on the sign of edx
SIGNRETURN:
    sar     edx,31
    mov     eax,edx
    ret
_allshr    ENDP

END
