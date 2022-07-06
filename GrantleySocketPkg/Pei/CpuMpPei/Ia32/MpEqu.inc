;-------------------------------------------------------------------------------
;
; Copyright (c)  1999 - 2011 Intel Corporation. All rights reserved
; This software and associated documentation (if any) is furnished
; under a license and may only be used or copied in accordance
; with the terms of the license. Except as permitted by such
; license, no part of this software or documentation may be
; reproduced, stored in a retrieval system, or transmitted in any
; form or by any means without the express written consent of
; Intel Corporation.
;
;
; Module Name:
;
;   MpEqu.inc
; 
; Abstract:
; 
;   This is the equates file for HT (Hyper-threading) support
;
;-------------------------------------------------------------------------------

VacantFlag             Equ   00h
NotVacantFlag          Equ   0ffh

LockLocation                  equ        1000h - 0200h
StackStartAddressLocation     equ        LockLocation + 04h
StackSizeLocation             equ        LockLocation + 08h
CProcedureLocation            equ        LockLocation + 0Ch
CProcedureArgumentLocation    equ        LockLocation + 10h
GdtrLocation                  equ        LockLocation + 14h
IdtrLocation                  equ        LockLocation + 1Ah
BufferStartLocation           equ        LockLocation + 20h
PmodeOffsetLocation           equ        LockLocation + 24h
FinishedCountAddressLocation  equ        LockLocation + 28h
WakeupCountAddressLocation    equ        LockLocation + 2Ch
SerializeLockAddressLocation  equ        LockLocation + 30h
StartStateLocation            equ        LockLocation + 34h
SemaphoreCheck                equ        LockLocation + 38h
ApProcedureLocation           equ        LockLocation + 3Ch
NumApsExecuting               equ        LockLocation + 40h

PAUSE32   MACRO
            DB      0F3h
            DB      090h
            ENDM

;-------------------------------------------------------------------------------
