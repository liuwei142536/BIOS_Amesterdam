; /*++
; Copyright (c) 2012 Intel Corporation. All rights reserved
; This software and associated documentation (if any) is furnished
; under a license and may only be used or copied in accordance
; with the terms of the license. Except as permitted by such
; license, no part of this software or documentation may be
; reproduced, stored in a retrieval system, or transmitted in any
; form or by any means without the express written consent of
; Intel Corporation.
; --*/

.586P
.XMM
.MODEL  SMALL, c

; Externs
EXTERN  PcdGet32 (PcdFlashFvSecPeiBase):DWORD   
;EXTERN  PcdGet32 (PcdFlashFvBackSecPeiBase):DWORD   
;----------------------------------------------------------------------------
;  STARTUP_SEG  S E G M E N T  STARTS
;----------------------------------------------------------------------------
STARTUP_SEG    SEGMENT    PARA PUBLIC 'CODE' USE32

;----------------------------------------------------------------------------
;
; Procedure:  EarlyChpsetInit_OemHook
;
; Description:  This routine provide oem hook after EarlyChipsetInit
;
; Input:  Stack NOT available
;        MMX3 = routine return address
;
; Output:  None
;
; Modified:  All,
;
;----------------------------------------------------------------------------
EarlyChpsetInit_OemHook    PROC PUBLIC
 ;platform OEM code

 ;the following code is RET_ESI_MMX3
  movd    esi, mm3                      ; restore return address from MM3
  jmp     esi                           ; Absolute jump
EarlyChpsetInit_OemHook    ENDP

;----------------------------------------------------------------------------
;
; Procedure:  LateChipsetInit_OemHook
;
; Description:  This routine provide oem hook after LateChipsetInit
;
; Input:  none
;
; Output:  None
;
; Modified:  All, except EBP and ESP
;
;----------------------------------------------------------------------------
LateChipsetInit_OemHook    PROC PUBLIC
 ;platform OEM code
  ret
LateChipsetInit_OemHook    ENDP



;----------------------------------------------------------------------------
;  STARTUP_SEG  S E G M E N T  ENDS
;----------------------------------------------------------------------------
STARTUP_SEG    ENDS

END

