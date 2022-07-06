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
.386
.model  flat,C
.data
RetAddress dd ?
NegateResult db ?
.code
EXTERN  _aulldvrm:PROC

abs64  PROC PRIVATE
    test edx, edx
    jns @F
    xor cl, 1
    jmp neg64
@@:
    ret
abs64 ENDP

neg64  PROC PRIVATE
    neg eax
    not edx
    cmc
    adc edx, 0
    ret
neg64 ENDP

;------------------------------------------------------------------------------
; Divides a 64-bit signed value by another 64-bit signed value and returns
; the 64-bit signed result and the 64-bit signed remainder.
; NOTE: In Microsoft C, the sign of the remainder is always the same as the sign of the first operand.
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
_alldvrm    PROC
    ; Devident = abs(Devident)
    xor cx, cx
    mov NegateResult, 0
    mov eax, [esp+4] ; eax <- DividendLo
    mov edx, [esp+8] ; edx <- DividendHi
    call abs64
    jcxz    @F
    or  NegateResult, 2
@@:
    mov [esp+4], eax
    mov [esp+8], edx
    ; Devident = abs(Divisor)
    mov eax, [esp+12] ; eax <- DivisorLo
    mov edx, [esp+16] ; edx <- DivisorHi
    call abs64
    mov [esp+12], eax
    mov [esp+16], edx
    or  NegateResult, cl

    pop     RetAddress ; save return address
    call    _aulldvrm ; perform unsigned division
    test    NegateResult, 1
    jz      @F
    call   neg64
@@:
    test    NegateResult, 2
    jz      @F
    push    edx
    push    eax
    mov     edx, ebx
    mov     eax, ecx
    call   neg64
    mov     ebx, edx
    mov     ecx, eax
    pop     eax
    pop     edx
@@:
    push    RetAddress
    ret
_alldvrm    ENDP

END
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
