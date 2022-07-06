;*************************************************************************
;*************************************************************************
;**                                                                     **
;**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
;**                                                                     **
;**                       All Rights Reserved.                          **
;**                                                                     **
;**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
;**                                                                     **
;**                       Phone: (770)-246-8600                         **
;**                                                                     **
;*************************************************************************

.code

;if MKF_REDIRECTION_ONLY_MODE EQ 0
;<AMI_PHDR_START>
;----------------------------------------------------------------------------
; Procedure:	PeCoffDriverSpecialBreakpoint
;
; Description:	This function writes some values to rax and rdx and breaks, in order to
;				recognise the special breakpoint on Host side.
;
; Input:		none
;
; Output:		None
;
;----------------------------------------------------------------------------
;<AMI_PHDR_END>
DbgModDriverSpecialBreakpoint	PROC PUBLIC
	push rax
	push rdx
	mov rax,494D4124h
	mov rdx,4442414Eh
	int 3
	pop rdx
	pop rax
	ret
DbgModDriverSpecialBreakpoint	ENDP


;endif

END
;*************************************************************************
;*************************************************************************
;**                                                                     **
;**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
;**                                                                     **
;**                       All Rights Reserved.                          **
;**                                                                     **
;**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
;**                                                                     **
;**                       Phone: (770)-246-8600                         **
;**                                                                     **
;*************************************************************************
