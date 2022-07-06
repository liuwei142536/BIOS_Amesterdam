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
;  ProcessorStartupPlatform.inc
;
; Abstract:
;
;
;------------------------------------------------------------------------------
;
; This is an assembly include file and is      
; licensed for Intel CPUs and chipsets under the terms of your
; license agreement with Intel or your vendor.  This file may 
; be modified by the user, subject to additional terms of the 
; license agreement                                           
;

;AptioV server override: Fix for EIP #115922  - Use SDL and PCD for CAR
include token.equ

MAX_CPU_SOCKETS                 EQU     4                   ; Max CPU sockets. Valid values are 4 or 8.               
MAX_SOCKET_ID                   EQU     (MAX_CPU_NODES - 1) ; Max CPU SocktID. 
;AptioV server override start: dynamic MMCFG base change support.
;AptioV server override: Use MKF_PCIEX_BASE_ADDRESS token instead of hard-coded value

IF MKF_DYNAMIC_MMCFG_BASE_SUPPORT ne 0
PCIEX_BASE_ADDRESS              EQU     80000000h          ; MMCFG / PCIe Config Base Address, used as cold boot default only.;;AptioV server overrride : Dynamic mmcfg base address change
ENDIF                                                         ; Use MMCFG_BASE_ADDRESS in PlatformHost.h file to
                                                            ; define platform choice of PCIe config base address
IF MKF_DYNAMIC_MMCFG_BASE_SUPPORT eq 0
PCIEX_BASE_ADDRESS              EQU     MKF_PCIEX_BASE_ADDRESS          ; MMCFG / PCIe Config Base Address, used as cold boot default only.;;AptioV server overrride : Dynamic mmcfg base address change
ENDIF                                                         ; Use MMCFG_BASE_ADDRESS in PlatformHost.h file to
                                                            ; define platform choice of PCIe config base address
;AptioV server override end: dynamic MMCFG base change support.
SB_RCBA                         EQU     0fed1c000h          ; South Bridge PCH RCBA Address

; IO Port 80h  POST CODEs used
POWER_ON_POST_CODE              EQU     01h
CACHE_ENABLED_POST_CODE         EQU     03h
MICROCODE_POST_CODE             EQU     02h
CPU_EARLY_INIT_POST_CODE        EQU     06h


;
;MTRR Range Size and Alignment Requirement
;A range that is to be mapped to a variable-range MTRR must meet the following “power of 2” size and alignment rules:
;1. The minimum range size is 4 KBytes and the base address of the range must be
;   on at least a 4-KByte boundary.
;2. For ranges greater than 4 KBytes, each range must be of length 2n and its base
;   address must be aligned on a 2n boundary, where n is a value equal to or greater
;   than 12. The base-address alignment value cannot be less than its length. For
;   example, an 8-KByte range cannot be aligned on a 4-KByte boundary. It must be
;   aligned on at least an 8-KByte boundary.

;
; Define the No-Eviction Mode Code Region below 4GB
;


WDB_BASE_ADDRESS              EQU 04000000h
;WDB_REGION_SIZE                      EQU 10000000h
WDB_REGION_SIZE               EQU 0                              ; 0 = No WDB region

;
; Define the No-Eviction Mode DataStack region
; This must be in an address range the chipset decodes
;
;AptioV server override start: Fix for EIP #115922  - Use SDL and PCD for CAR
DATA_STACK_BASE_ADDRESS       EQU MKF_CAR_BASE_ADDRESS
DATA_STACK_SIZE               EQU MKF_CAR_TOTAL_SIZE                        ; Size = 512KB
;AptioV server override end: Fix for EIP #115922  - Use SDL and PCD for CAR
DATA_STACK_SIZE_MASK          EQU (NOT (DATA_STACK_SIZE - 1))

LOCAL_APIC_BASE               EQU 0fee00000h
;
; Cache init and test values
; These are inverted to flip each bit at least once
;
CACHE_INIT_VALUE              EQU 0A5A5A5A5h
CACHE_TEST_VALUE              EQU (NOT CACHE_INIT_VALUE)

BIT0                          EQU 01h
BIT1                          EQU 02h
BIT2                          EQU 04h
BIT3                          EQU 08h
BIT4                          EQU 10h
BIT5                          EQU 20h
BIT6                          EQU 40h
BIT7                          EQU 80h
BIT8                          EQU 100h
BIT9                          EQU 200h
BIT10                         EQU 400h
BIT11                         EQU 800h
BIT12                         EQU 1000h
BIT13                         EQU 2000h
BIT14                         EQU 4000h
BIT15                         EQU 8000h
BIT16                         EQU 10000h
BIT17                         EQU 20000h
BIT18                         EQU 40000h
BIT19                         EQU 80000h
BIT20                         EQU 100000h
BIT21                         EQU 200000h
BIT22                         EQU 400000h
BIT23                         EQU 800000h
BIT24                         EQU 1000000h
BIT25                         EQU 2000000h
BIT26                         EQU 4000000h
BIT27                         EQU 8000000h
BIT28                         EQU 10000000h
BIT29                         EQU 20000000h
BIT30                         EQU 40000000h
BIT31                         EQU 80000000h

