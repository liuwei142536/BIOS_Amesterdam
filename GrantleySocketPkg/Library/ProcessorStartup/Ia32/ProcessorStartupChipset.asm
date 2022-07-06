;
; This file contains an 'Intel Pre-EFI Module' and is licensed
; for Intel CPUs and Chipsets under the terms of your license 
; agreement with Intel or your vendor.  This file may be      
; modified by the user, subject to additional terms of the    
; license agreement                                           
;
;------------------------------------------------------------------------------
;
; Copyright (c) 1999 - 2012, Intel Corporation. All rights reserved.<BR>
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
;  ProcessorStartupChipset.asm
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

        EXTERN  GetLocalSktId:NEAR32
        EXTERN  GetCpuCsrAddrNoStack:NEAR32
        EXTERN  GetMmcfgBase:NEAR32

;----------------------------------------------------------------------------
;       STARTUP_SEG  S E G M E N T  STARTS
;----------------------------------------------------------------------------
STARTUP_SEG SEGMENT PARA PUBLIC 'CODE' USE32

EarlyChipsetInit  PROC    NEAR    PUBLIC
        ;
        ; Program PCH RCBA Base Address
        ;
        ; Get the socket id. 
        movd  ecx, xmm2               ; xmm2[31:24] stores the socketID
        shr   ecx, 24t
        movd  eax, xmm1               ; al = legacy socket ID
        cmp   cl, al                  ; am I legacy socket?
        jne   ExitEarly               ; no, skip

        mov   eax,0F80F0h                           ; 0:1F:0:F0
        CALL_MMX6 GetCpuCsrAddrNoStack
        mov   DWORD PTR [edi], SB_RCBA OR 1         ; Set the Enable bit.
        ;
        ; Send Port80 codes to LPC.
        ;
        mov     edi,  SB_RCBA + 3410h                 ; RCBA + 3410h, ICH_RCRB_GCS
        and     DWORD PTR [edi], NOT (1 SHL 2)        ; Clear bit [2]
ExitEarly:
        RET_ESI_MMX3
EarlyChipsetInit  ENDP

MidChipsetInit    PROC    NEAR    PUBLIC
        RET_ESI_MMX3
MidChipsetInit    ENDP

LateChipsetInit    PROC    NEAR    PUBLIC

        ;
        ; Get PCIe Base address
        ;
        call  GetLocalSktId                         ; cl = my socket id
        mov   eax,0F80DCh                           ; 0:1F:0:F0
        CALL_MMX6 GetCpuCsrAddrNoStack

        ;
        ; Enable SPI prefetching
        ;
        or    DWORD PTR [edi], (1 SHL 3)                ; Bits [3:2] = '10' - enable prefetching and caching.

        call   GetMmcfgBase                             ; esi = MMCFG Base Address
        ;
        ; Program and Enable ACPI PM Base.
        ;     
        and     edi, 0FFF00000h                           ; clear D:F:R filed
        or      edi, ICH_LPC_PMBASE_PCI_ADDR AND 0FFFFFh  ; 0:1F:0:40h
        or      edi, esi
        mov     eax, ICH_ACPI_BASE_ADDRESS + 1
        mov     Dword Ptr [edi], eax
        and     edi, 0FFF00000h
        or      edi, ICH_LPC_ACPICNTL_PCI_ADDR AND 0FFFFFh
        or      edi, esi
        or      Dword Ptr [edi], 00000080h

        ;
        ; Enable GPIO BASE I/O registers
        ;
        mov     eax, PCI_LPC_BASE + 48h
        mov     dx, 0CF8h
        out     dx, eax
        mov     eax, GPIO_BASE_ADDRESS
        add     dx, 4
        out     dx, eax

        mov     eax, PCI_LPC_BASE + 4Ch
        mov     dx, 0CF8h
        out     dx, eax
        add     dx, 4
        in      al, dx
        or      al, BIT4     ; GPIOBASE Enable
        out     dx, al

        ;GPIO_USE_SEL2 Register -> 1 = GPIO 0 = Native
        mov     dx, GPIO_BASE_ADDRESS + R_GPIO_USE_SEL2
        in      eax, dx
        or      eax, 002h    ;Enable GPIO33
        out     dx, eax
        ;GP_IO_SEL2 Register -> 1 = Input 0 = Output if Native Mode not care
        mov     dx, GPIO_BASE_ADDRESS + R_GPIO_IO_SEL2
        in      eax, dx
        or      eax, 002h    ;Configure GPIO33 as Input
        out     dx, eax

        ;
        ; Enable HPET decode in PCH.
        ;
        mov     esi, ICH_RCRB_BASE + ICH_RCRB_HPET
        mov     eax, ICH_RCRB_HPET_DECODE
        mov     Dword Ptr [esi], eax
        mov     eax, Dword ptr [esi]
        xor     eax, eax
        mov     esi, HPET_COMP_1
        mov     Dword Ptr [esi], eax
        mov     esi, HPET_COMP_2
        mov     Dword ptr [esi], eax

        ;
        ; Enable the upper 128-byte bank of RTC RAM.
        ;
        mov     esi, ICH_RCRB_BASE + ICH_RCRB_RTC_CONF
        mov     eax, Dword Ptr [esi]
        or      eax, ICH_RCRB_RTC_CONF_UCMOS_EN
        mov     Dword Ptr [esi], eax
        
;AptioV server override start: Added to halt TCO timer        
        ;
        ; Halt TCO timer.
        ;  
        mov		dx, TCO_BASE_ADDRESS
        or		dx, ICH_IOREG_TCO1_CNT     
        in		ax, dx
        or		ax, BIT_TCO_TMR_HLT
        out		dx, ax
;AptioV server override end: Added to halt TCO timer         
             
        ret
LateChipsetInit    ENDP


STARTUP_SEG ENDS

END

