;
; This file contains an 'Intel Pre-EFI Module' and is licensed
; for Intel CPUs and Chipsets under the terms of your license 
; agreement with Intel or your vendor.  This file may be      
; modified by the user, subject to additional terms of the    
; license agreement                                           
;
;------------------------------------------------------------------------------
;
; Copyright (c) 1999 - 2014, Intel Corporation. All rights reserved.<BR>
; This software and associated documentation (if any) is furnished
; under a license and may only be used or copied in accordance
; with the terms of the license. Except as permitted by such
; license, no part of this software or documentation may be
; reproduced, stored in a retrieval system, or transmitted in any
; form or by any means without the express written consent of
; Intel Corporation.
;
; Module Name:
;
;  ProcessorStartup.asm
;
; Abstract:
;
;
;------------------------------------------------------------------------------        
        .586P
        .XMM            
        .MODEL  SMALL, c

        include ProcessorStartupPlatform.inc
        include ProcessorStartupChipset.inc
        include ProcessorStartup.inc

        EXTERN  CheckGenuineIntelCpu:NEAR32
        EXTERN  EarlyUncoreInit:NEAR32
        EXTERN  EarlyChipsetInit:NEAR32
        EXTERN  LoadProcessorMicrocode:NEAR32
        EXTERN  MidUncoreInit:NEAR32
        EXTERN  MidChipsetInit:NEAR32
        EXTERN  PreNemUncoreInit:NEAR32
        EXTERN  ConfigDfx:NEAR32
        EXTERN  ConfigDcuMode:NEAR32
        EXTERN  NemInit:NEAR32
        EXTERN  EstablishNemStack:NEAR32
        EXTERN  TxtInit:NEAR32
        EXTERN  PostNemUncoreInit:NEAR32
        EXTERN  LateChipsetInit:NEAR32
        EXTERN  SetTargetList:NEAR32
        EXTERN  EarlyChpsetInit_OemHook:NEAR32
        EXTERN  LateChipsetInit_OemHook:NEAR32
        EXTERN  _ProcessorStartupEnd:NEAR32   ;AptioV server override
        
;----------------------------------------------------------------------------
;       STARTUP_SEG  S E G M E N T  STARTS
;----------------------------------------------------------------------------
STARTUP_SEG SEGMENT PARA PUBLIC 'CODE' USE32

_ProcessorStartup  PROC  PUBLIC

        movd    mm1, esi
        movd    mm2, edi
        
;       mm0 : Assumed to be BIST state (not used by this module, but cant be destroyed)
;       mm1 : _ProcessorStartup return address
;       mm2 : LAST_ADDRESS of decode region
;       mm3 : Used for stack by CALL_MMX3/RET_ESI_MMX3 macro
;       mm4 : Used for stack by CALL_MMX4/RET_ESI_MMX4 macro
;       mm5 : Used for stack by CALL_MMX5/RET_ESI_MMX5 macro
;       mm6 : Used for stack by CALL_MMX6/RET_ESI_MMX6 macro
        
        CALL_MMX3   CheckGenuineIntelCpu
        ; this must be done before we can do any io transaction such as port 80
        CALL_MMX3    SetTargetList         
          
        CALL_MMX3   EarlyUncoreInit             
        ; After this point all CPU's MMCFG/CPUBUSNO is config'ed and enabled.
        ; CSR accesses to PCIe config space using MMCFG method are allowed.
        ; All CSR accesses must use the correct bus# for the target component 
        ; (IIO, Uncore, etc) of the target CPU socket/node.

        CALL_MMX3   ConfigDfx
        CALL_MMX3   EarlyChipsetInit

        PORT80 (MICROCODE_POST_CODE)
;OEM_HOOK after EarlyChipsetInit
;
        CALL_MMX3 EarlyChpsetInit_OemHook
;
; 
;  
        CALL_MMX3   LoadProcessorMicrocode
        
        CALL_MMX3   MidUncoreInit
        CALL_MMX3   MidChipsetInit

        PORT80 (CACHE_ENABLED_POST_CODE)
        
        CALL_MMX3   PreNemUncoreInit
        CALL_MMX3   ConfigDcuMode
        CALL_MMX3   NemInit
        CALL_MMX3   EstablishNemStack

        ; After this point functions must be called using stack
        ;  Functions after here may NOT destroy ESP!
        
        PORT80 (CPU_EARLY_INIT_POST_CODE)
        call    TxtInit
        call    PostNemUncoreInit
        ;
        ; Only SBSP comes here to continue. All other package BSPs are held in a loop
        ;
        call    LateChipsetInit
;
;OEM_HOOK after LateChipsetInit
;
        call LateChipsetInit_OemHook
;
; 
;          
        
        
        movd    esi, mm1
        jmp     _ProcessorStartupEnd  ;AptioV server override

_ProcessorStartup  ENDP


STARTUP_SEG ENDS

END

