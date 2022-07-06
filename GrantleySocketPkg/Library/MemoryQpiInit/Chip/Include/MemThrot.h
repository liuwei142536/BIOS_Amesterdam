//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//

/*************************************************************************
 *
 * Memory Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2019 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains data structures for thermal parameters
 *
 ************************************************************************/
#ifndef _memthrot_h
#define _memthrot_h

#include <Token.h> // AptioV server override

#pragma pack(1)
// Base on Grantley DDR3_DDR4 Thermal Offset Table 092613-09.xlsx and
// Grantley_CLTT-Whitepaper_5368520_rev1-4.doc
// and 
// Based on BDX-DE DDR4 Thermal Offset Table 121313-04_Draft.xlsx and
// BDX-DE CLTT implementation whitepaper 121313-03draft.doc
// the tables below assume the following:
//
// DDR Type
//      DDR3 = 0
//      DDR4 = 1
//
// Type
//      RDIMM = 1
//      LRDIMM = 11d
//      UDIMM = 2
//      SODIMM = 3
//
// DRAM Density
//      1Gb  = 2
//      2Gb  = 3
//      4Gb  = 4
//      8Gb  = 5
//      16Gb = 6
//
//#Ranks and Width combined into 1 byte
// #Ranks
//      SR = 0
//      DR = 0x8
//      QR = 0x18
//      OR = 0x38
//
// Width
//      x4 = 0
//      x8 = 1
//
// Frequency
//      800 = 0 
//      1000 = 1
//      1066 = 2
//      1067 = 2
//      1333 = 4
//      1600 = 6
//      1866 = 8 
//      1867 = 8
//      2133 = 0xA
//      2200 = 0xB
//      2400 = 0xC
//      2600 = 0xD
//      2667 = 0xE
//      2800 = 0xF
//      2933 = 0x10
//      3000 = 0x11
//      3200 = 0x12
//
// DPC (Maximum DIMMs per channel on the platform)
//      1, 2, 3
//
// Static
//

#ifdef DE_SKU
#define MAX_TT_ROW 174
#else
#define MAX_TT_ROW 230
#endif

const struct DimmTTRowEntryStruct DimmTT[] = {
#ifdef DE_SKU
1,  1,  4, 0x1, 6, 1, 6,
1,  1,  4, 0x1, 8, 1, 7,
1,  1,  4, 0x1, 10, 1, 8,
1,  1,  4, 0x1, 12, 1, 9,
1,  1,  4, 0x1, 6, 1, 6,
1,  1,  4, 0x1, 8, 1, 7,
1,  1,  4, 0x1, 10, 1, 9,
1,  1,  4, 0x1, 12, 1, 10,
1,  1,  4, 0x1, 6, 2, 4,
1,  1,  4, 0x1, 8, 2, 5,
1,  1,  4, 0x1, 10, 2, 5,
1,  1,  4, 0x1, 12, 2, 6,
1,  1,  4, 0x0, 6, 1, 8,
1,  1,  4, 0x0, 8, 1, 13,
1,  1,  4, 0x0, 10, 1, 15,
1,  1,  4, 0x0, 12, 1, 15,
1,  1,  4, 0x0, 6, 1, 9,
1,  1,  4, 0x0, 8, 1, 14,
1,  1,  4, 0x0, 10, 1, 15,
1,  1,  4, 0x0, 12, 1, 15,
1,  1,  4, 0x0, 6, 2, 6,
1,  1,  4, 0x0, 8, 2, 8,
1,  1,  4, 0x0, 10, 2, 9,
1,  1,  4, 0x0, 12, 2, 10,
1,  1,  4, 0x9, 6, 1, 6,
1,  1,  4, 0x9, 8, 1, 7,
1,  1,  4, 0x9, 10, 1, 8,
1,  1,  4, 0x9, 12, 1, 9,
1,  1,  4, 0x9, 6, 1, 6,
1,  1,  4, 0x9, 8, 1, 8,
1,  1,  4, 0x9, 10, 1, 9,
1,  1,  4, 0x9, 12, 1, 10,
1,  1,  4, 0x9, 6, 2, 4,
1,  1,  4, 0x9, 8, 2, 5,
1,  1,  4, 0x9, 10, 2, 5,
1,  1,  4, 0x9, 12, 2, 6,
1,  1,  4, 0x8, 6, 1, 2,
1,  1,  4, 0x8, 8, 1, 4,
1,  1,  4, 0x8, 10, 1, 4,
1,  1,  4, 0x8, 12, 1, 4,
1,  1,  4, 0x8, 6, 1, 3,
1,  1,  4, 0x8, 8, 1, 5,
1,  1,  4, 0x8, 10, 1, 5,
1,  1,  4, 0x8, 12, 1, 6,
1,  1,  4, 0x8, 6, 2, 2,
1,  1,  4, 0x8, 8, 2, 3,
1,  1,  4, 0x8, 10, 2, 3,
1,  1,  4, 0x8, 12, 2, 3,
1,  1,  5, 0x1, 6, 1, 6,
1,  1,  5, 0x1, 8, 1, 6,
1,  1,  5, 0x1, 10, 1, 7,
1,  1,  5, 0x1, 12, 1, 7,
1,  1,  5, 0x1, 6, 1, 6,
1,  1,  5, 0x1, 8, 1, 7,
1,  1,  5, 0x1, 10, 1, 7,
1,  1,  5, 0x1, 12, 1, 7,
1,  1,  5, 0x1, 6, 2, 4,
1,  1,  5, 0x1, 8, 2, 4,
1,  1,  5, 0x1, 10, 2, 5,
1,  1,  5, 0x1, 12, 2, 5,
1,  1,  5, 0x0, 6, 1, 6,
1,  1,  5, 0x0, 8, 1, 7,
1,  1,  5, 0x0, 10, 1, 8,
1,  1,  5, 0x0, 12, 1, 10,
1,  1,  5, 0x0, 6, 1, 7,
1,  1,  5, 0x0, 8, 1, 8,
1,  1,  5, 0x0, 10, 1, 9,
1,  1,  5, 0x0, 12, 1, 10,
1,  1,  5, 0x0, 6, 2, 5,
1,  1,  5, 0x0, 8, 2, 5,
1,  1,  5, 0x0, 10, 2, 6,
1,  1,  5, 0x0, 12, 2, 7,
1,  1,  5, 0x9, 6, 1, 6,
1,  1,  5, 0x9, 8, 1, 7,
1,  1,  5, 0x9, 10, 1, 7,
1,  1,  5, 0x9, 12, 1, 7,
1,  1,  5, 0x9, 6, 1, 7,
1,  1,  5, 0x9, 8, 1, 7,
1,  1,  5, 0x9, 10, 1, 8,
1,  1,  5, 0x9, 12, 1, 8,
1,  1,  5, 0x9, 6, 2, 4,
1,  1,  5, 0x9, 8, 2, 5,
1,  1,  5, 0x9, 10, 2, 6,
1,  1,  5, 0x9, 12, 2, 6,
1,  1,  5, 0x8, 6, 1, 2,
1,  1,  5, 0x8, 8, 1, 2,
1,  1,  5, 0x8, 10, 1, 3,
1,  1,  5, 0x8, 12, 1, 3,
1,  1,  5, 0x8, 6, 1, 3,
1,  1,  5, 0x8, 8, 1, 3,
1,  1,  5, 0x8, 10, 1, 4,
1,  1,  5, 0x8, 12, 1, 4,
1,  1,  5, 0x8, 6, 2, 2,
1,  1,  5, 0x8, 8, 2, 2,
1,  1,  5, 0x8, 10, 2, 3,
1,  1,  5, 0x8, 12, 2, 3,
1,  1,  6, 0x1,  8, 1, 6,
1,  1,  6, 0x1, 10, 1, 7,
1,  1,  6, 0x1, 12, 1, 7,
1,  1,  6, 0x1,  8, 1, 7,
1,  1,  6, 0x1, 10, 1, 7,
1,  1,  6, 0x1, 12, 1, 7,
1,  1,  6, 0x1,  8, 2, 4,
1,  1,  6, 0x1, 10, 2, 5,
1,  1,  6, 0x1, 12, 2, 5,
1,  1,  6, 0x9,  8, 1, 7,
1,  1,  6, 0x9, 10, 1, 7,
1,  1,  6, 0x9, 12, 1, 7,
1,  1,  6, 0x9,  8, 1, 7,
1,  1,  6, 0x9, 10, 1, 8,
1,  1,  6, 0x9, 12, 1, 8,
1,  1,  6, 0x9,  8, 2, 5,
1,  1,  6, 0x9, 10, 2, 6,
1,  1,  6, 0x9, 12, 2, 6,
1,  3,  4, 0x1, 6, 1, 8,
1,  3,  4, 0x1, 8, 1, 10,
1,  3,  4, 0x1, 10, 1, 12,
1,  3,  4, 0x1, 12, 1, 14,
1,  3,  4, 0x1, 6, 2, 4,
1,  3,  4, 0x1, 8, 2, 6,
1,  3,  4, 0x1, 10, 2, 7,
1,  3,  4, 0x1, 12, 2, 7,
1,  3,  4, 0x9, 6, 1, 7,
1,  3,  4, 0x9, 8, 1, 9,
1,  3,  4, 0x9, 10, 1, 10,
1,  3,  4, 0x9, 12, 1, 10,
1,  3,  4, 0x9, 6, 2, 4,
1,  3,  4, 0x9, 8, 2, 4,
1,  3,  4, 0x9, 10, 2, 5,
1,  3,  4, 0x9, 12, 2, 5,
1,  3,  5, 0x1, 6, 1, 8,
1,  3,  5, 0x1, 8, 1, 9,
1,  3,  5, 0x1, 10, 1, 10,
1,  3,  5, 0x1, 12, 1, 10,
1,  3,  5, 0x1, 6, 2, 5,
1,  3,  5, 0x1, 8, 2, 5,
1,  3,  5, 0x1, 10, 2, 6,
1,  3,  5, 0x1, 12, 2, 6,
1,  3,  5, 0x9, 6, 1, 7,
1,  3,  5, 0x9, 8, 1, 8,
1,  3,  5, 0x9, 10, 1, 9,
1,  3,  5, 0x9, 12, 1, 8,
1,  3,  5, 0x9, 6, 2, 4,
1,  3,  5, 0x9, 8, 2, 5,
1,  3,  5, 0x9, 10, 2, 5,
1,  3,  5, 0x9, 12, 2, 5,
1,  3,  6, 0x1,  8, 1, 9,
1,  3,  6, 0x1, 10, 1, 10,
1,  3,  6, 0x1, 12, 1, 10,
1,  3,  6, 0x1,  8, 2, 5,
1,  3,  6, 0x1, 10, 2, 6,
1,  3,  6, 0x1, 12, 2, 6,
1,  3,  6, 0x9,  8, 1, 8,
1,  3,  6, 0x9, 10, 1, 9,
1,  3,  6, 0x9, 12, 1, 8,
1,  3,  6, 0x9,  8, 2, 5,
1,  3,  6, 0x9, 10, 2, 5,
1,  3,  6, 0x9, 12, 2, 5,
0,  1,  3, 0x1, 4, 1, 2,
0,  1,  3, 0x1, 6, 1, 2,
0,  1,  3, 0x1, 4, 1, 4,
0,  1,  3, 0x1, 6, 1, 3,
0,  1,  3, 0x1, 4, 2, 3,
0,  1,  3, 0x1, 6, 2, 2,
0,  1,  3, 0x0, 4, 1, 4,
0,  1,  3, 0x0, 6, 1, 5,
0,  1,  3, 0x0, 4, 1, 6,
0,  1,  3, 0x0, 6, 1, 8,
0,  1,  3, 0x0, 4, 2, 4,
0,  1,  3, 0x0, 6, 2, 5,
0,  1,  3, 0x9, 4, 1, 3,
0,  1,  3, 0x9, 6, 1, 2,
0,  1,  3, 0x9, 4, 1, 5,
0,  1,  3, 0x9, 6, 1, 4,
0,  1,  3, 0x9, 4, 2, 4,
0,  1,  3, 0x9, 6, 2, 3,
0,  1,  3, 0x8, 4, 1, 2,
0,  1,  3, 0x8, 6, 1, 3,
0,  1,  3, 0x8, 4, 1, 4,
0,  1,  3, 0x8, 6, 1, 5,
0,  1,  3, 0x8, 4, 2, 2,
0,  1,  3, 0x8, 6, 2, 4,
0,  1,  3, 0x8, 4, 1, 0,
0,  1,  3, 0x8, 6, 1, 0,
0,  1,  3, 0x8, 4, 1, 2,
0,  1,  3, 0x8, 6, 1, 2,
0,  1,  3, 0x8, 4, 2, 1,
0,  1,  3, 0x8, 6, 2, 1,
0,  3,  3, 0x1, 4, 1, 10,
0,  3,  3, 0x1, 6, 1, 15,
0,  3,  3, 0x1, 4, 2, 9,
0,  3,  3, 0x1, 6, 2, 13,
0,  3,  3, 0x9, 4, 1, 7,
0,  3,  3, 0x9, 6, 1, 11,
0,  3,  3, 0x9, 4, 2, 7,
0,  3,  3, 0x9, 6, 2, 9,
0,  3,  4, 0x1, 4, 1, 12,
0,  3,  4, 0x1, 6, 1, 15,
0,  3,  4, 0x1, 4, 2, 10,
0,  3,  4, 0x1, 6, 2, 13,
0,  3,  4, 0x9, 4, 1, 9,
0,  3,  4, 0x9, 6, 1, 12,
0,  3,  4, 0x9, 4, 2, 7,
0,  3,  4, 0x9, 6, 2, 10,
#else
1, 0x1, 4, 0x1, 6, 1, 6,
1, 0x1, 4, 0x1, 8, 1, 8,
1, 0x1, 4, 0x1, 10, 1, 9,
1, 0x1, 4, 0x1, 12, 1, 10,
1, 0x1, 4, 0x1, 6, 2, 4,
1, 0x1, 4, 0x1, 8, 2, 5,
1, 0x1, 4, 0x1, 10, 2, 6,
1, 0x1, 4, 0x1, 12, 2, 6,
1, 0x1, 4, 0x1, 6, 3, 3,
1, 0x1, 4, 0x1, 8, 3, 4,
1, 0x1, 4, 0x1, 10, 3, 4,
1, 0x1, 4, 0x1, 12, 3, 5,
1, 0x1, 4, 0x0, 6, 1, 10,
1, 0x1, 4, 0x0, 8, 1, 14,
1, 0x1, 4, 0x0, 10, 1, 15,
1, 0x1, 4, 0x0, 12, 1, 15,
1, 0x1, 4, 0x0, 6, 2, 6,
1, 0x1, 4, 0x0, 8, 2, 8,
1, 0x1, 4, 0x0, 10, 2, 9,
1, 0x1, 4, 0x0, 12, 2, 10,
1, 0x1, 4, 0x0, 6, 3, 5,
1, 0x1, 4, 0x0, 8, 3, 6,
1, 0x1, 4, 0x0, 10, 3, 7,
1, 0x1, 4, 0x0, 12, 3, 7,
1, 0x1, 4, 0x9, 6, 1, 7,
1, 0x1, 4, 0x9, 8, 1, 8,
1, 0x1, 4, 0x9, 10, 1, 9,
1, 0x1, 4, 0x9, 12, 1, 10,
1, 0x1, 4, 0x9, 6, 2, 4,
1, 0x1, 4, 0x9, 8, 2, 5,
1, 0x1, 4, 0x9, 10, 2, 6,
1, 0x1, 4, 0x9, 12, 2, 6,
1, 0x1, 4, 0x9, 6, 3, 4,
1, 0x1, 4, 0x9, 8, 3, 4,
1, 0x1, 4, 0x9, 10, 3, 4,
1, 0x1, 4, 0x9, 12, 3, 5,
1, 0x1, 4, 0x8, 6, 1, 4,
1, 0x1, 4, 0x8, 8, 1, 5,
1, 0x1, 4, 0x8, 10, 1, 6,
1, 0x1, 4, 0x8, 12, 1, 6,
1, 0x1, 4, 0x8, 6, 2, 3,
1, 0x1, 4, 0x8, 8, 2, 3,
1, 0x1, 4, 0x8, 10, 2, 4,
1, 0x1, 4, 0x8, 12, 2, 4,
1, 0x1, 4, 0x8, 6, 3, 2,
1, 0x1, 4, 0x8, 8, 3, 3,
1, 0x1, 4, 0x8, 10, 3, 3,
1, 0x1, 4, 0x8, 12, 3, 3,
1, 0xB, 4, 0x18, 6, 1, 9,
1, 0xB, 4, 0x18, 8, 1, 13,
1, 0xB, 4, 0x18, 10, 1, 15,
1, 0xB, 4, 0x18, 12, 1, 15,
1, 0xB, 4, 0x18, 6, 2, 4,
1, 0xB, 4, 0x18, 8, 2, 6,
1, 0xB, 4, 0x18, 10, 2, 7,
1, 0xB, 4, 0x18, 12, 2, 8,
1, 0xB, 4, 0x18, 6, 3, 2,
1, 0xB, 4, 0x18, 8, 3, 3,
1, 0xB, 4, 0x18, 10, 3, 4,
1, 0xB, 4, 0x18, 12, 3, 4,
1, 0x1, 5, 0x1, 6, 1, 7,
1, 0x1, 5, 0x1, 8, 1, 7,
1, 0x1, 5, 0x1, 10, 1, 8,
1, 0x1, 5, 0x1, 12, 1, 8,
1, 0x1, 5, 0x1, 6, 2, 5,
1, 0x1, 5, 0x1, 8, 2, 5,
1, 0x1, 5, 0x1, 10, 2, 5,
1, 0x1, 5, 0x1, 12, 2, 5,
1, 0x1, 5, 0x1, 6, 3, 4,
1, 0x1, 5, 0x1, 8, 3, 4,
1, 0x1, 5, 0x1, 10, 3, 4,
1, 0x1, 5, 0x1, 12, 3, 4,
1, 0x1, 5, 0x0, 6, 1, 7,
1, 0x1, 5, 0x0, 8, 1, 8,
1, 0x1, 5, 0x0, 10, 1, 10,
1, 0x1, 5, 0x0, 12, 1, 11,
1, 0x1, 5, 0x0, 6, 2, 5,
1, 0x1, 5, 0x0, 8, 2, 6,
1, 0x1, 5, 0x0, 10, 2, 7,
1, 0x1, 5, 0x0, 12, 2, 7,
1, 0x1, 5, 0x0, 6, 3, 5,
1, 0x1, 5, 0x0, 8, 3, 5,
1, 0x1, 5, 0x0, 10, 3, 5,
1, 0x1, 5, 0x0, 12, 3, 6,
1, 0x1, 5, 0x9, 6, 1, 7,
1, 0x1, 5, 0x9, 8, 1, 8,
1, 0x1, 5, 0x9, 10, 1, 9,
1, 0x1, 5, 0x9, 12, 1, 9,
1, 0x1, 5, 0x9, 6, 2, 5,
1, 0x1, 5, 0x9, 8, 2, 6,
1, 0x1, 5, 0x9, 10, 2, 6,
1, 0x1, 5, 0x9, 12, 2, 6,
1, 0x1, 5, 0x9, 6, 3, 4,
1, 0x1, 5, 0x9, 8, 3, 5,
1, 0x1, 5, 0x9, 10, 3, 5,
1, 0x1, 5, 0x9, 12, 3, 5,
1, 0x1, 5, 0x8, 6, 1, 3,
1, 0x1, 5, 0x8, 8, 1, 4,
1, 0x1, 5, 0x8, 10, 1, 4,
1, 0x1, 5, 0x8, 12, 1, 4,
1, 0x1, 5, 0x8, 6, 2, 3,
1, 0x1, 5, 0x8, 8, 2, 3,
1, 0x1, 5, 0x8, 10, 2, 3,
1, 0x1, 5, 0x8, 12, 2, 3,
1, 0x1, 5, 0x8, 6, 3, 3,
1, 0x1, 5, 0x8, 8, 3, 3,
1, 0x1, 5, 0x8, 10, 3, 3,
1, 0x1, 5, 0x8, 12, 3, 3,
1, 0xB, 5, 0x18, 6, 1, 10,
1, 0xB, 5, 0x18, 8, 1, 11,
1, 0xB, 5, 0x18, 10, 1, 13,
1, 0xB, 5, 0x18, 12, 1, 14,
1, 0xB, 5, 0x18, 6, 2, 7,
1, 0xB, 5, 0x18, 8, 2, 7,
1, 0xB, 5, 0x18, 10, 2, 8,
1, 0xB, 5, 0x18, 12, 2, 9,
1, 0xB, 5, 0x18, 6, 3, 5,
1, 0xB, 5, 0x18, 8, 3, 5,
1, 0xB, 5, 0x18, 10, 3, 6,
1, 0xB, 5, 0x18, 12, 3, 7,
0, 0x1, 3, 0x1, 0, 1, 1,
0, 0x1, 3, 0x1, 2, 1, 2,
0, 0x1, 3, 0x1, 4, 1, 4,
0, 0x1, 3, 0x1, 6, 1, 3,
0, 0x1, 3, 0x1, 8, 1, 5,
0, 0x1, 3, 0x1, 0, 2, 1,
0, 0x1, 3, 0x1, 2, 2, 2,
0, 0x1, 3, 0x1, 4, 2, 3,
0, 0x1, 3, 0x1, 6, 2, 2,
0, 0x1, 3, 0x1, 8, 2, 5,
0, 0x1, 3, 0x1, 0, 3, 2,
0, 0x1, 3, 0x1, 2, 3, 3,
0, 0x1, 3, 0x1, 4, 3, 3,
0, 0x1, 3, 0x1, 6, 3, 3,
0, 0x1, 3, 0x1, 8, 3, 3,
0, 0x1, 3, 0x0, 0, 1, 4,
0, 0x1, 3, 0x0, 2, 1, 5,
0, 0x1, 3, 0x0, 4, 1, 6,
0, 0x1, 3, 0x0, 6, 1, 8,
0, 0x1, 3, 0x0, 8, 1, 10,
0, 0x1, 3, 0x0, 0, 2, 3,
0, 0x1, 3, 0x0, 2, 2, 3,
0, 0x1, 3, 0x0, 4, 2, 4,
0, 0x1, 3, 0x0, 6, 2, 5,
0, 0x1, 3, 0x0, 8, 2, 8,
0, 0x1, 3, 0x0, 0, 3, 3,
0, 0x1, 3, 0x0, 2, 3, 4,
0, 0x1, 3, 0x0, 4, 3, 4,
0, 0x1, 3, 0x0, 6, 3, 5,
0, 0x1, 3, 0x0, 8, 3, 5,
0, 0x1, 3, 0x9, 0, 1, 2,
0, 0x1, 3, 0x9, 2, 1, 3,
0, 0x1, 3, 0x9, 4, 1, 5,
0, 0x1, 3, 0x9, 6, 1, 4,
0, 0x1, 3, 0x9, 8, 1, 5,
0, 0x1, 3, 0x9, 0, 2, 2,
0, 0x1, 3, 0x9, 2, 2, 3,
0, 0x1, 3, 0x9, 4, 2, 4,
0, 0x1, 3, 0x9, 6, 2, 3,
0, 0x1, 3, 0x9, 8, 2, 5,
0, 0x1, 3, 0x9, 0, 3, 2,
0, 0x1, 3, 0x9, 2, 3, 3,
0, 0x1, 3, 0x9, 4, 3, 4,
0, 0x1, 3, 0x9, 6, 3, 3,
0, 0x1, 3, 0x9, 8, 3, 3,
0, 0x1, 3, 0x8, 0, 1, 2,
0, 0x1, 3, 0x8, 2, 1, 3,
0, 0x1, 3, 0x8, 4, 1, 4,
0, 0x1, 3, 0x8, 6, 1, 5,
0, 0x1, 3, 0x8, 8, 1, 8,
0, 0x1, 3, 0x8, 0, 2, 1,
0, 0x1, 3, 0x8, 2, 2, 2,
0, 0x1, 3, 0x8, 4, 2, 2,
0, 0x1, 3, 0x8, 6, 2, 4,
0, 0x1, 3, 0x8, 8, 2, 6,
0, 0x1, 3, 0x8, 0, 3, 1,
0, 0x1, 3, 0x8, 2, 3, 2,
0, 0x1, 3, 0x8, 4, 3, 2,
0, 0x1, 3, 0x8, 6, 3, 3,
0, 0x1, 3, 0x8, 8, 3, 3,
0, 0x1, 3, 0x19, 0, 1, 3,
0, 0x1, 3, 0x19, 2, 1, 4,
0, 0x1, 3, 0x19, 4, 1, 7,
0, 0x1, 3, 0x19, 6, 1, 6,
0, 0x1, 3, 0x19, 8, 1, 7,
0, 0x1, 3, 0x19, 0, 2, 3,
0, 0x1, 3, 0x19, 2, 2, 4,
0, 0x1, 3, 0x19, 4, 2, 7,
0, 0x1, 3, 0x19, 6, 2, 6,
0, 0x1, 3, 0x19, 8, 2, 6,
0, 0x1, 3, 0x18, 0, 1, 8,
0, 0x1, 3, 0x18, 2, 1, 11,
0, 0x1, 3, 0x18, 4, 1, 13,
0, 0x1, 3, 0x18, 6, 1, 15,
0, 0x1, 3, 0x18, 8, 1, 15,
0, 0x1, 3, 0x18, 0, 2, 7,
0, 0x1, 3, 0x18, 2, 2, 10,
0, 0x1, 3, 0x18, 4, 2, 12,
0, 0x1, 3, 0x18, 6, 2, 13,
0, 0x1, 3, 0x18, 8, 2, 15,
0, 0xB, 3, 0x18, 0, 1, 5,
0, 0xB, 3, 0x18, 2, 1, 5,
0, 0xB, 3, 0x18, 4, 1, 6,
0, 0xB, 3, 0x18, 6, 1, 6,
0, 0xB, 3, 0x18, 8, 1, 7,
0, 0xB, 3, 0x18, 0, 2, 4,
0, 0xB, 3, 0x18, 2, 2, 5,
0, 0xB, 3, 0x18, 4, 2, 5,
0, 0xB, 3, 0x18, 6, 2, 5,
0, 0xB, 3, 0x18, 8, 2, 5,
0, 0xB, 3, 0x18, 0, 3, 4,
0, 0xB, 3, 0x18, 2, 3, 4,
0, 0xB, 3, 0x18, 4, 3, 4,
0, 0xB, 3, 0x18, 6, 3, 5,
0, 0xB, 3, 0x18, 8, 3, 5,
0, 0xB, 3, 0x38, 0, 1, 6,
0, 0xB, 3, 0x38, 2, 1, 7,
0, 0xB, 3, 0x38, 4, 1, 8,
0, 0xB, 3, 0x38, 6, 1, 8,
0, 0xB, 3, 0x38, 8, 1, 9,
0, 0xB, 3, 0x38, 0, 2, 5,
0, 0xB, 3, 0x38, 2, 2, 6,
0, 0xB, 3, 0x38, 4, 2, 6,
0, 0xB, 3, 0x38, 6, 2, 7,
0, 0xB, 3, 0x38, 8, 2, 7,
0, 0xB, 3, 0x38, 0, 3, 5,
0, 0xB, 3, 0x38, 2, 3, 5,
0, 0xB, 3, 0x38, 4, 3, 6,
0, 0xB, 3, 0x38, 6, 3, 6,
0, 0xB, 3, 0x38, 8, 3, 7,
#endif
 };

#define MAX_TTM_ROW 54

//
// Base on Platform_CLTT-Whitepaper_021114_rev0-5 Draft.doc
// the tables below assume the following:
//
// TTMode = Throttling Mode
//      OLTT = 0
//      CLTT = 1
//
// TTDIMMPC = Num of DIMM Per Channel
//      1DPC = 1
//      2DPC = 2
//      3DPC = 3
//
// TTFREQ = Frequency
//      800 = 0 
//      1067 = 2
//      1333 = 4
//      1600 = 6
//      1867 = 8
//      2133 = 0xA
//      2400 = 0xC
//      2667 = 0xE
//      2933 = 0x10
//
// TTWINDOWS = Windows (us)
//      1us = 1
//      2us = 2
//      ...
//
// TTBW_THROTTLE_TF = BW_THROTTLE_TF (hex)
//
// TTTHRT_HI = THRT_HI (hex)
//
// TTBWPercent = Throttle BW (per DIMM)
//      [value is ??% * 10]
//
struct DimmTThrottlingStruct {
  UINT8   TTMode;
  UINT8   TTDIMMPC;
  UINT8   TTFREQ;
  UINT8   TTWINDOWS;
  UINT16  TTBW_THROTTLE_TF;
  UINT8   TTTHRT_HI;
  UINT16  TTBWPercent;
};

const struct DimmTThrottlingStruct DimmTTMODE[] = {
0, 1, 0, 1, 0x32, 0x5, 800,
0, 1, 2, 1, 0x43, 0x7, 836,
0, 1, 4, 1, 0x53, 0x8, 771,
0, 1, 6, 1, 0x64, 0x10, 800,
0, 1, 8, 1, 0x75, 0x12, 821,
0, 1, 0xA, 1, 0x85, 0x13, 782,
0, 1, 0xC, 1, 0x96, 0x15, 800,
0, 1, 0xE, 1, 0xA7, 0x17, 814,
0, 1, 0x10, 1, 0xB7, 0x18, 787,
0, 2, 0, 2, 0x64, 0x5, 400,
0, 2, 2, 2, 0x85, 0x7, 421,
0, 2, 4, 2, 0xA7, 0x8, 383,
0, 2, 6, 2, 0xC8, 0x10, 400,
0, 2, 8, 2, 0xE9, 0x12, 412,
0, 2, 0xA, 2, 0x10B, 0x13, 390,
0, 2, 0xC, 2, 0x12C, 0x15, 400,
0, 2, 0xE, 2, 0x14D, 0x17, 408,
0, 2, 0x10, 2, 0x16F, 0x18, 392,
0, 3, 0, 3, 0x96, 0x5, 267,
0, 3, 2, 3, 0xC8, 0x7, 288,
0, 3, 4, 3, 0xFA, 0x8, 256,
0, 3, 6, 3, 0x12C, 0x10, 267,
0, 3, 8, 3, 0x15E, 0x12, 274,
0, 3, 0xA, 3, 0x190, 0x13, 260,
0, 3, 0xC, 3, 0x1C2, 0x15, 267,
0, 3, 0xE, 3, 0x1F4, 0x17, 272,
0, 3, 0x10, 3, 0x226, 0x18, 262,
1, 1, 0, 3, 0x96, 0x2, 107,
1, 1, 2, 4, 0x10B, 0x3, 90,
1, 1, 4, 3, 0xFA, 0x3, 96,
1, 1, 6, 3, 0x12C, 0x4, 107,
1, 1, 8, 4, 0x1D3, 0x6, 103,
1, 1, 0xA, 4, 0x215, 0x7, 105,
1, 1, 0xC, 4, 0x258, 0x8, 107,
1, 1, 0xE, 3, 0x1F4, 0x6, 96,
1, 1, 0x10, 3, 0x226, 0x7, 102,
1, 2, 0, 3, 0x96, 0x1, 53,
1, 2, 2, 4, 0x10B, 0x2, 60,
1, 2, 4, 4, 0x14D, 0x2, 48,
1, 2, 6, 3, 0x12C, 0x2, 53,
1, 2, 8, 4, 0x1D3, 0x3, 51,
1, 2, 0xA, 4, 0x215, 0x3, 45,
1, 2, 0xC, 4, 0x258, 0x4, 53,
1, 2, 0xE, 4, 0x298, 0x4, 48,
1, 2, 0x10, 4, 0x2DD, 0x5, 55,
1, 3, 0, 4, 0xC8, 0x1, 40,
1, 3, 2, 3, 0xC8, 0x1, 40,
1, 3, 4, 3, 0xFA, 0x1, 32,
1, 3, 6, 4, 0x190, 0x2, 40,
1, 3, 8, 4, 0x1D3, 0x2, 34,
1, 3, 0xA, 4, 0x215, 0x2, 30,
1, 3, 0xC, 3, 0x1C2, 0x2, 36,
1, 3, 0xE, 3, 0x1F4, 0x2, 32,
1, 3, 0x10, 4, 0x2DD, 0x3, 33,
 };

#pragma pack()
#endif  // _memThrot_h