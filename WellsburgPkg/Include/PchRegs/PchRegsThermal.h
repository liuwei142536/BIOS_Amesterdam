/** @file
  Register names for PCH Thermal Device

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values of bits within the registers
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_<generation_name>_" in register/bit names. e.g., "_PCH_LPT_"
  - Registers / bits that are different between SKUs are denoted by "_<SKU_name>"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without <generation_name> inserted.

@copyright
  Copyright (c) 1999 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
#ifndef _PCH_REGS_THERMAL_H_
#define _PCH_REGS_THERMAL_H_

#include <Token.h> // AptioV server override

//
//  Thermal Device Registers (D31:F6)
//
#define PCI_DEVICE_NUMBER_PCH_THERMAL   31
#define PCI_FUNCTION_NUMBER_PCH_THERMAL 6
#define R_PCH_THERMAL_VENDOR_ID         0x00
#define V_PCH_THERMAL_VENDOR_ID         V_PCH_INTEL_VENDOR_ID
#define R_PCH_THERMAL_DEVICE_ID         0x02
#define V_PCH_LPTH_THERMAL_DEVICE_ID     0x8C24
#define V_PCH_LPTLP_THERMAL_DEVICE_ID    0x9C24
#define R_PCH_THERMAL_COMMAND           0x04
#define B_PCH_THERMAL_COMMAND_MSE       BIT1
#define B_PCH_THERMAL_COMMAND_BME       BIT2
#define R_PCH_THERMAL_TBAR              0x10
#define V_PCH_THERMAL_TBAR_SIZE         (4 * 1024)
#define N_PCH_THREMAL_TBAR_ALIGNMENT    12
#define B_PCH_THERMAL_TBAR_MASK         0xFFFFF000
#define R_PCH_THERMAL_TBARH             0x14
#define R_PCH_THERMAL_SVID              0x2C
#define R_PCH_THERMAL_INTLN             0x3C
#define R_PCH_THERMAL_TBARB             0x40
#define V_PCH_THERMAL_TBARB_SIZE        (4 * 1024)
#define N_PCH_THREMAL_TBARB_ALIGNMENT   12
#define B_PCH_THERMAL_SPTYPEN           BIT0
#define R_PCH_THERMAL_TBARBH            0x44
#define B_PCH_THERMAL_TBARB_MASK        0xFFFFF000

#define R_PCH_TBARB_TSC                 0x04
#define B_PCH_TBARB_TSC_PLD             BIT7
#define B_PCH_TBARB_TSC_CPDE            BIT0
#define R_PCH_TBARB_TSS                 0x06
#define R_PCH_TBARB_TSEL                0x08
#define B_PCH_TBARB_TSEL_PLD            BIT7
#define B_PCH_TBARB_TSEL_ETS            BIT0
#define R_PCH_TBARB_TSREL               0x0A
#define R_PCH_TBARB_TSMIC               0x0C
#define B_PCH_TBARB_TSMIC_PLD           BIT7
#define B_PCH_TBARB_TSMIC_SMIE          BIT0
#define R_PCH_TBARB_CTT                 0x10
#define R_PCH_TBARB_TAHV                0x14
#define R_PCH_TBARB_TALV                0x18
#define R_PCH_TBARB_TSPM                0x1C
#define B_PCH_TBARB_TSPM_LTT            (BIT8 | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define V_PCH_TBARB_TSPM_LTT            0x0C8
#define B_PCH_TBARB_TSPM_MAXTSST        (BIT11 | BIT10 | BIT9)
#define V_PCH_TBARB_TSPM_MAXTSST        (0x4 << 9)
#define B_PCH_TBARB_TSPM_MINTSST        BIT12
#define B_PCH_TBARB_TSPM_DTSSIC0        BIT13
#define B_PCH_TBARB_TSPM_DTSSS0EN       BIT14
#define B_PCH_TBARB_TSPM_TSPMLOCK       BIT15
#define R_PCH_TBARB_TL                  0x40
#define B_PCH_TBARB_TL_LOCK             BIT31
#define B_PCH_TBARB_TL_TT13EN           BIT30
#define B_PCH_TBARB_TL_TTEN             BIT29
#define R_PCH_TBARB_PHL                 0x60
#define B_PCH_TBARB_PHLE                BIT15
#define R_PCH_TBARB_PHLC                0x62
#define R_PCH_TBARB_PHLC_PHLLOCK        BIT0
#define R_PCH_TBARB_TAS                 0x80
#define R_PCH_TBARB_TSPIEN              0x82
#define R_PCH_TBARB_TSGPEN              0x84

//
// Default PchTTLevels - Thermal Throttling Levels  (in Centigrade 8bit format)
//
// Conversion: <9bit_register> = ((<8bit_centigrade> + 50) * 2)
// The value of 9bit format field is valid only if it is between 00h and 17Fh.
// 0x17F is the hottest temperature and 0x000 is the lowest temperature.
// Nine bits, in 0.5-degree increments, with 50 degree offset.
//

#ifndef PCH_SERVER_BIOS_FLAG
#ifdef	DE_SKU
//
// BDX-DE_SKU
//
#define TL_T1  108
#define TL_T2  111
#define TL_T3  114
#define TL_PHL 115
#define TL_CTT 130
#define TL_MAX 141
#define TL_STEP  3
//
// Termal levels register values (termal state -1)
// HFR & simple MACRO does not accept arythmetics
//  T1L >= T1 > T0L
//  T2L >= T2 > T1L
//         T3 > T2L
//
#define T0LEVEL_REGISTER 107
#define T1LEVEL_REGISTER 110
#define T2LEVEL_REGISTER 113

#else
//
// Client PCH
//
// Set PCHHOT# to 103C.
// This avoids fan boost due to PchThermTrip.
//
#define TL_T1  109
#define TL_T2  112
#define TL_T3  115
#define TL_PHL 103
#define TL_CTT 120
#define TL_MAX 141
#define TL_STEP  3
//
// Termal levels register values (termal state -1)
// HFR & simple MACRO does not accept rythmetics
//  T1L >= T1 > T0L
//  T2L >= T2 > T1L
//         T3 > T2L
//
#define T0LEVEL_REGISTER 108
#define T1LEVEL_REGISTER 111
#define T2LEVEL_REGISTER 114

#endif //DE_SKU
#else //PCH_SERVER_BIOS_FLAG
//
// Server PCH
// 
// Set PCHHOT# to 103C.
// This avoids fan boost due to PchThermTrip.
// TMSDG For Wellsburg indicates that the value for CTT shoulde be 0x012C (100 C) rather than 0x0154 (120 C) in 
// lynxpoint systems. 
//
#define TL_T1  109
#define TL_T2  112
#define TL_T3  115
#define TL_PHL 103
#define TL_CTT 100
#define TL_MAX 141
#define TL_STEP  3
//
// Termal levels register values (termal state -1)
// HFR & simple MACRO does not accept rythmetics
//  T1L >= T1 > T0L
//  T2L >= T2 > T1L
//         T3 > T2L
//
#define T0LEVEL_REGISTER 108
#define T1LEVEL_REGISTER 111
#define T2LEVEL_REGISTER 114

#endif //PCH_SERVER_BIOS_FLAG
#endif
