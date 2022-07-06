/*++
Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  ddr4OdtActTable.h

Abstract:

  Data structures for thermal and power parameters for platform(s).

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

              DO NOT MODIFY - TABLE IS AUTO-GENERATED

        This table is generated with XML2H.exe

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

--*/

#ifndef ddr4_odt_activation_table_h
#define ddr4_odt_activation_table_h

#include <Token.h> // AptioV server override

#include "DataTypes.h"

#pragma pack(1)
struct odtActStruct ddr4OdtActTable[] = {
#ifdef DE_SKU 

   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (SR_DIMM <<  7)                         | // Slot0
   (EMPTY_DIMM << 10)                      | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (EMPTY_DIMM <<  7)                      | // Slot0
   (SR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (EMPTY_DIMM << 10)                      | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK1 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (EMPTY_DIMM << 10)                      | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (EMPTY_DIMM <<  7)                      | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK1 << 2)                            | // target rank
   (EMPTY_DIMM <<  7)                      | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (SR_DIMM <<  7)                         | // Slot0
   (SR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (SR_DIMM <<  7)                         | // Slot0
   (SR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK1 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK1 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (SR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK1 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (SR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (DR_DIMM <<  7)                         | // Slot0
   (SR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM0 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (SR_DIMM <<  7)                         | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         0,
         // write
         ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK0 << 2)                            | // target rank
   (SR_DIMM <<  7)                         | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
   // Config index
   (DIMM1 << 0)                            | // target dimm
   (RANK1 << 2)                            | // target rank
   (SR_DIMM <<  7)                         | // Slot0
   (DR_DIMM << 10)                         | // Slot1
   (EMPTY_DIMM << 13),                       // Slot2


   // ODT activation matrix
   {
         // read
         MCODT_ACT << 15 |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         0,
         // write
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
         ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
         0,
   },
#else 
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
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
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
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
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
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
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
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
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
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
      0,
       // write
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

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
   (RANK3 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM2 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (LR_DIMM << 13),           // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM2 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
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
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
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
      0,
       // write
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      0,
   },
   // Config index
   (DIMM1 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
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
   (RANK3 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (LR_DIMM << 10)          | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)   |
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
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK1 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      0,
       // write
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK2 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)   |
      0,
   },
   // Config index
   (DIMM0 << 0)             | // target dimm
   (RANK3 << 2)             | // target rank
   (LR_DIMM << 7)           | // slot0
   (EMPTY_DIMM << 10)       | // slot1
   (EMPTY_DIMM << 13),        // slot2

   // ODT activation matrix
   {   // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
       // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)   |
      0,
   },
#endif
};
#ifdef DE_SKU
//#define  MAX_DDR4_ODT_ACT_CONFIG  18
#else
//#define  MAX_DDR4_ODT_ACT_CONFIG    75
#endif
#pragma pack()

#endif //ddr4_odt_activation_table_h
