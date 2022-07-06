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
;  ProcessorStartupChipset.inc
;
; Abstract:
;
;   Chipset constants and macros
;
;------------------------------------------------------------------------------
;AptioV server override: Use SDL token for PM base address.
include token.equ

IIO_MISCSS          EQU (20 SHL 15) + (0 SHL 12) + 09Ch ; B0:D20:F0:R9Ch
IIO_D20F0_DID       EQU (20 SHL 15) + (0 SHL 12) + 02h  ; B0:D20:F0:R02h

;
; Clarksfield IIO Scratch pad register equates
;
IIO_SPAD_SR00       EQU (20 SHL 15) + (1 SHL 12) + 07Ch ; B0:D20:F1:R7Ch
IIO_SPAD_SR01       EQU (20 SHL 15) + (1 SHL 12) + 080h ; B0:D20:F1:R80h
IIO_SPAD_SR02       EQU (20 SHL 15) + (1 SHL 12) + 084h ; B0:D20:F1:R84h
IIO_SPAD_SR03       EQU (20 SHL 15) + (1 SHL 12) + 088h ; B0:D20:F1:R88h
IIO_SPAD_SR04       EQU (20 SHL 15) + (1 SHL 12) + 08Ch ; B0:D20:F1:R8Ch
IIO_SPAD_SR05       EQU (20 SHL 15) + (1 SHL 12) + 090h ; B0:D20:F1:R90h
IIO_SPAD_SR06       EQU (20 SHL 15) + (1 SHL 12) + 094h ; B0:D20:F1:R94h


;
; APIC register
;
APICID                        EQU 0FEE00020h

;
; Power Management I/O Registers
;
ICH_ACPI_BASE_ADDRESS         EQU MKF_PM_BASE_ADDRESS ;AptioV server override: Use SDL token for PM base address.
ACPI_PM1_STS                  EQU 000h
ACPI_PM1_CNT                  EQU 004h
;AptioV server override start: Added to halt TCO timer
TCO_BASE_ADDRESS              EQU (ICH_ACPI_BASE_ADDRESS + 60h)
ICH_IOREG_TCO1_CNT            EQU 08h
BIT_TCO_TMR_HLT               EQU 0800h
;AptioV server override end: Added to halt TCO timer

;
; ICH RCBA base address
;
ICH_RCRB_BASE                 EQU 0FED1C000h
ICH_RCRB_BASE_REG             EQU 8000F8F0h       ; ICH Register B0:D31:RF0
ICH_RCRB_GCS                  EQU 03410h
ICH_RCRB_RTC_CONF             EQU 03400h
ICH_RCRB_RTC_CONF_UCMOS_EN    EQU 04h
ICH_RCRB_HPET                 EQU 03404h
ICH_RCRB_HPET_DECODE          EQU 080h

;
; HPET compare register
;
HPET_COMP_1                   EQU 0FED00108h
HPET_COMP_2                   EQU 0FED0010Ch
HPET_COMP_3                   EQU 0FED00128h
HPET_COMP_4                   EQU 0FED00148h

;
; PCI registers
;
ICH_LPC_PMBASE_PCI_ADDR       EQU ((1Fh * 8 + 00h) * 1000h + 0040h)
ICH_LPC_ACPICNTL_PCI_ADDR     EQU ((1Fh * 8 + 00h) * 1000h + 0044h)

;
; Use PCI write via CF8/CFC access mechanism to write MMCFG_BASE
; as (4GB -512MB) using GQ1_CR_PCIEXBAR
;
GQ1_CR_PCIEXBAR               EQU 80000150h       ; D0:F1:R50h

BIT1    EQU     002h
BIT2    EQU     004h
BIT3    EQU     008h
BIT4    EQU     010h
BIT7    EQU     080h
BIT9    EQU     0200h
;
;Defines for ICH DEVICE 31
;
SB_BUS                  EQU     00h
SB_PCI2ISA_DEVICE       EQU     31
SB_PCI2ISA_FUNC         EQU     0
SB_SMBUS_FUNC           EQU     3
;Word Equate for Bus, Device, Function number for I/O Controller Hub
SB_PCI2ISA_BUS_DEV_FUNC EQU     (SB_BUS SHL 8) + ((SB_PCI2ISA_DEVICE SHL 3)+ SB_PCI2ISA_FUNC)
SB_SMBUS_BUS_DEV_FUNC   EQU     (SB_BUS SHL 8) + ((SB_PCI2ISA_DEVICE SHL 3) + SB_SMBUS_FUNC)

; Define the equates here
PCI_LPC_BASE            EQU     BIT31 + (SB_PCI2ISA_BUS_DEV_FUNC SHL 8)
P2I_GEN_PMCON_2_OFFSET  EQU     0A2h            ; General PM Configuration 2
  CPUPWR_FLR    EQU     BIT1
P2I_GEN_PMCON_3_OFFSET  EQU     0A4h            ; General PM Configuration 3
  PWR_FLR       EQU     BIT1
  RTC_PWR_STS   EQU     BIT2
  GEN_RST_STS   EQU     BIT9
P2I_PMIR_OFFSET         EQU     0ACh            ; Power Management Intitialization Register.

PCI_SMBUS_BASE          EQU     BIT31 + (SB_SMBUS_BUS_DEV_FUNC SHL 8)
LPC_IO_KBC_ENB          EQU     083h

GPIO_BASE_ADDRESS	EQU	0500h
R_GPIO_USE_SEL2		EQU 030h
R_GPIO_IO_SEL2		EQU	034h
        
