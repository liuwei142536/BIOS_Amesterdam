.686p
.xmm 
.model small, c
.code

LOCAL_APIC_BASE equ 0fee00000h

CPU_BIST struct
    ApicId  dd ?
    Bist    dd ?
CPU_BIST ends

PS_GetBist proc near pBist:DWORD
    push esi
    push ebx
        
    mov esi, pBist

    mov dword ptr [esi], 1    ;Only one BIST

    add esi, 4

    ;Get Apic ID
    mov     eax, LOCAL_APIC_BASE + 20h
    mov     ebx, [eax]
    shr     ebx, 24
    mov     (CPU_BIST ptr [esi]).ApicId, ebx

    ; This stores the BIST for SBSP.
    ; The PBSPs BIST is stored in MM0. The value will preserved across INIT-SIPI and captured
    ; in CPU PEI.
    movd    eax, mm0
    mov     (CPU_BIST ptr [esi]).Bist, eax

;    add     esi, size CPU_BIST     ;next entry.

    pop     ebx
    pop     esi
    ret
PS_GetBist endp

END
