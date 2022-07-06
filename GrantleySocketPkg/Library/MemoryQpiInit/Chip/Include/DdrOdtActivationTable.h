//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
#ifndef _odt_activation_table_h
#define _odt_activation_table_h

#include "DataTypes.h"

#pragma pack(1)
struct odtActStruct odtActTable[] = {

   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (QR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (QR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK2)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (SR_DIMM << 7)           | // slot0
   (DR_DIMM << 10)          | // slot1
   (SR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (DR_DIMM << 7)           | // slot0
   (SR_DIMM << 10)          | // slot1
   (DR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK0 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
};

#define  MAX_ODT_ACT_CONFIG    80

#pragma pack()
#endif //_odt_activation_table_h
