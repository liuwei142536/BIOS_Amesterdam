;*************************************************************************
;*************************************************************************
;**                                                                     **
;**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
;**                                                                     **
;**                       All Rights Reserved.                          **
;**                                                                     **
;**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
;**                                                                     **
;**                       Phone: (770)-246-8600                         **
;**                                                                     **
;*************************************************************************
;*************************************************************************
.386
.model  flat,C
.code
EXTERN  _alldvrm:PROC

;------------------------------------------------------------------------------
; Divides a 64-bit signed value by another 64-bit signed value and returns
; the 64-bit signed remainder.
; NOTE: In Microsoft C, the sign of the remainder is always the same as the sign of the first operand.
;
;  On entry:
;      [ESP]    : Return Address
;      [ESP+4]  : QWORD. Dividend
;      [ESP+12] : QWORD. Divisor
;  On exit:
;      EDX:EAX contains the remainder (divided % divisor)
;
;  NOTE: this routine removes parameters from the stack.
;------------------------------------------------------------------------------
_allrem    PROC
    mov    [esp-4], ebx ; push ebx

    ; Copy function parameters to the top of the stack
    mov    eax, [esp+4] ; DividentLo
    mov    [esp-20], eax
    mov    eax, [esp+8] ; DividentHi
    mov    [esp-16], eax
    mov    eax, [esp+12] ; DivisorLo
    mov    [esp-12], eax
    mov    eax, [esp+16] ; DivisorHi
    mov    [esp-8], eax

    sub    esp, 20 ; Adjust stack sizeof(Divisor) + sizeof(Dividend) + sizeof(EBX) = 20

    call   _alldvrm
    mov    edx, ebx
    mov    eax, ecx
    pop    ebx
    ret    16
_allrem    ENDP

END
;*************************************************************************
;*************************************************************************
;**                                                                     **
;**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
;**                                                                     **
;**                       All Rights Reserved.                          **
;**                                                                     **
;**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
;**                                                                     **
;**                       Phone: (770)-246-8600                         **
;**                                                                     **
;*************************************************************************
;*************************************************************************
