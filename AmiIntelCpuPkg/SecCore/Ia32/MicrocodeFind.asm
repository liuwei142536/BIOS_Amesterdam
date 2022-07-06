.686p
.xmm
.model small, c

include token.equ

ifndef MKF_MPDTable_CREATED
MKF_MPDTable_CREATED EQU 0
endif

ifndef MKF_PACK_MICROCODE
MKF_PACK_MICROCODE EQU 0
endif

ifndef MKF_MICROCODE_SPLIT_BB_UPDATE
MKF_MICROCODE_SPLIT_BB_UPDATE EQU 0
endif

FV_LENGTH		EQU (16 + 16)
FV_SIGNATURE    EQU (FV_LENGTH + 8)
FV_HDR_LENGTH	EQU (FV_SIGNATURE + 4 + 4)
FV_EXT_HDR_OFFSET EQU (FV_HDR_LENGTH + 2 + 2)

FV_EXT_HDR_SIZE EQU 16

FFS_TYPE        EQU (16 + 2)
FFS_LENGTH      EQU (FFS_TYPE + 1 + 1)
FFS_HDR_LENGTH	EQU (FFS_LENGTH + 3 + 1)

uCODE_CPU_SIGNATURE	EQU 12
uCODE_CPU_FLAGS		EQU	24
uCODE_DATA_SIZE		EQU	28
uCODE_TOTAL_SIZE	EQU	32

_TEXT_PROTECTED_MODE      SEGMENT PARA PUBLIC USE32 'CODE'					;
                          ASSUME  CS:_TEXT_PROTECTED_MODE, DS:_TEXT_PROTECTED_MODE		;

IF MKF_Microcode_SUPPORT

MICOCODE_FFS_GUID label dword
	dd	17088572h
	dw	377Fh
	dw	44efh
	db	8Fh,4Eh,0B0h,9Fh,0FFh,46h,0A0h,70h

public FindMicrocode
extern FindMicrocodeEnd:NEAR32
FindMicrocode proc
IF  MKF_MICROCODE_SPLIT_BB_UPDATE
    mov     ebx, MKF_FV_MICROCODE_UPDATE_BASE
FindMicrocodeFv:
ELSE
    mov     ebx, MKF_FV_MICROCODE_BASE
ENDIF
	mov		edx, ebx
    cmp     dword ptr [edx + FV_SIGNATURE], 'HVF_'
    jne     Microcode_Not_Found             ;Corrupt FV?
	mov		eax, [edx + FV_HDR_LENGTH]
	and		eax, 0ffffh		;Get 16 bit FV header length
	add		ebx,  eax		;ebx = start of FFS
    jc      Microcode_Not_Found             ;Corrupt FV?

	movzx   eax, word ptr [edx + FV_EXT_HDR_OFFSET]	;Optional Header
    cmp ax, 0								;Optional exteneded header ?
	je  @f
	lea eax, [edx + eax]					;eax = extended header

	mov ecx, [eax + FV_EXT_HDR_SIZE]		;ecx = extended header size
	mov ebx, eax
	add ebx, ecx
	jc  Microcode_Not_Found                 ;Corrupt FV?
@@:

	add		ebx, 7							;Align to 8-bytes for FFs
	and		ebx, not 7

	add		edx, [edx + FV_LENGTH]			;edx = end of FV.
    jz      @f                              ;zf if Boot Block
    jc      Microcode_Not_Found             ;Corrupt FV?
@@:
    dec     edx                             ;edx = End of FFS

;---Find File ---
;ebx = Start of FFS
;edx = End of FFS
Get_Next_File:
    cmp     byte ptr [ebx + FFS_TYPE], 0f0h ;iqnore guid for pad type
    je  @f
	cmp		dword ptr [ebx], -1		;Is in end of files, but not firmware volume.
	je		File_not_found
@@:
	mov		ecx, 4
	mov		esi, ebx
	mov		edi, offset MICOCODE_FFS_GUID
	;compare file guid
	repe 	cmpsd
	je		Found_File
;---get next file--
	;get file length and add to ebx
	mov		eax, [ebx + FFS_LENGTH]
	and		eax, 0ffffffh			;eax = File length (24 bits)
	add		ebx, eax				;next file
	;align file
	add		ebx, 7
    jc      Microcode_Not_Found     ;Check for wrap-around if bad data.
	and		ebx, not 7

    mov     eax, ebx
    add     eax, FFS_HDR_LENGTH     ;eax is microcode data
    cmp     eax, edx                ;Is end of firmware volume?
	jb		Get_Next_File
File_not_found:
    jmp     Microcode_Not_Found
;---Found File---
Found_File:
;ebx = Start of Microcode FFS
;edx = End of FFS + 1

;---Search micocode for match.---
	mov		edi, ebx

	mov		eax, [ebx + FFS_LENGTH]
	and		eax, 0ffffffh			;eax = File length (24 bits)
	add		edi, eax				;edi = end of file
    jc      Microcode_Not_Found     ;Check for wrap-around if bad data.

IF MKF_MPDTable_CREATED
    movzx   eax, word ptr [edi-2]           ;eax = MPDT length
    sub     edi, eax                        ;edi = end of microcodes.
ENDIF

	add		ebx, FFS_HDR_LENGTH
    jc      Microcode_Not_Found     ;Check for wrap-around if bad data.

	mov		esi, ebx

    mov     eax, 1
    cpuid
    mov		ebx, eax	;ebx = CPU signature

	mov		ecx, 17h
	rdmsr				;read CPUS flags

	shr		edx, 18
	and		dl, 7		;dl = CPU FLAG
	mov		cl, dl
	mov		dl, 1
	shl		dl, cl		;dl = bit set for CPU flag

    xchg    ebx, esi    ;ebx = ptr, esi = signature
Next_Microcode:
	cmp		ebx, edi                ;Check to see if microcode at end of FFS File.
	jae		Microcode_Not_Found

	cmp		dword ptr [ebx], 1 ;Check if correct header revision
	jne		Microcode_Not_Found

	mov		ecx, 2048	;total size if data size in field = 0.
	cmp		dword ptr [ebx + uCODE_DATA_SIZE], 0
	jz		@f
	mov		ecx, [ebx + uCODE_TOTAL_SIZE]	;Get total size.
@@:

	cmp		esi, [ebx + uCODE_CPU_SIGNATURE]
	jne		Check_Ext_Sig

    mov     eax, ebx                        ;Store uC addr in eax, so if jnz succeeds.
	test	dl, [ebx + uCODE_CPU_FLAGS]     ;uC flags match?
	jnz		Exit_Find_Microcode

Check_Ext_Sig:
    mov     ebp, [ebx + uCODE_TOTAL_SIZE]	;Get total size.
    mov     eax, [ebx + uCODE_DATA_SIZE]    ;Get Data Size
    add     eax, 48
    cmp     ebp, eax
    jbe     Find_Next_Microcode             ;No extended signature.

    mov     ecx, [ebx + eax]                ;Get extended signature.
    cmp     ecx, 20
    jae     Microcode_Not_Found             ;Corrupt?
    lea     ebp, [ebx + eax + 20]           ;First Proc Signature.

@@:
    cmp     [ebp], esi                      ;Check Signature
    jne     Next_Ext_Sig

    mov     eax, ebx
    test    dl, [ebp + 4]                   ;Check processor flags
    jnz     Exit_Find_Microcode
Next_Ext_Sig:
    add     ebp, 12
    loop    @b
    mov     ecx, [ebx + uCODE_TOTAL_SIZE]	;Get total size.
Find_Next_Microcode:
    ;align to next block size
IF MKF_PACK_MICROCODE
    add     ecx, 15
    and     ecx, 0fffffff0h
ELSE
    add     ecx, MKF_MICROCODE_BLOCK_SIZE - 1
    and     ecx, NOT (MKF_MICROCODE_BLOCK_SIZE - 1)
ENDIF
	add		ebx, ecx
    jc      Microcode_Not_Found             ;Overflow - bad data.
	jmp		Next_Microcode

Microcode_Not_Found:
IF  MKF_MICROCODE_SPLIT_BB_UPDATE
    cmp     ebx, MKF_FV_MICROCODE_BASE
    jae     @f
    mov     ebx, MKF_FV_MICROCODE_BASE
    jmp     FindMicrocodeFv
@@:
ENDIF
	xor		eax, eax
Exit_Find_Microcode:
jmp FindMicrocodeEnd
FindMicrocode endp

ENDIF ;MKF_Microcode_SUPPORT


_TEXT_PROTECTED_MODE    ENDS
END
