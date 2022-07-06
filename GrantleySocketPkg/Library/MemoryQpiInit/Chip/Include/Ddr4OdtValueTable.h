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

#ifndef ddr4_odt_value_table_h
#define ddr4_odt_value_table_h

#include <Token.h> // AptioV server override

#include "DataTypes.h"

#pragma pack(1)
struct ddr4OdtValueStruct ddr4OdtValueTable[] = {

#ifdef DE_SKU
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      75,                                       // MC Vref percent
      72,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60,  // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (EMPTY_DIMM << 10)                      | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (EMPTY_DIMM <<  7)                      | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      77,                                       // MC Vref percent
      74,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      76,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      76,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      76,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      76,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      76,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      85,                                       // MC Vref percent
      77,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      85,                                       // MC Vref percent
      77,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)  | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)  | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      85,                                       // MC Vref percent
      77,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      85,                                       // MC Vref percent
      77,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      85,                                       // MC Vref percent
      77,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (DR_DIMM <<  7)                         | // Slot0
      (SR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                     // MC Vref percent
      78,                                     // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)                         | // freq
      (SR_DIMM <<  7)                         | // Slot0
      (DR_DIMM << 10)                         | // Slot1
      (EMPTY_DIMM << 13),                       // slot2
      MCODT_50,                                 // MC ODT encoding
      84,                                       // MC Vref percent
      78,                                       // DRAM Vref percent
      {
         { // dimm0
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_120 << 6)  | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm1
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank0
            (DDR4_RTT_WR_240 << 6)  | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         },
         { // dimm2
              // Rtt_wr              Rtt_prk               Rtt_nom
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank0
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank1
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS, // rank2
            (DDR4_RTT_WR_DIS << 6)  | (DDR4_RTT_NOM_DIS << 3)  | DDR4_RTT_NOM_DIS  // rank3
         }
      }
   },
#else
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60, // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60, // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60, // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60, // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_60, // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      77,                                 // MC Vref percent
      74,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      77,                                 // MC Vref percent
      74,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      77,                                 // MC Vref percent
      74,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      77,                                 // MC Vref percent
      74,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      77,                                 // MC Vref percent
      74,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      85,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      85,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      85,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      85,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      85,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      84,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      89,                                 // MC Vref percent
      83,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      89,                                 // MC Vref percent
      83,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      89,                                 // MC Vref percent
      83,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      89,                                 // MC Vref percent
      83,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      89,                                 // MC Vref percent
      83,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (SR_DIMM << 7)        |             // slot0
      (DR_DIMM << 10)       |             // slot1
      (SR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (DR_DIMM << 7)        |             // slot0
      (SR_DIMM << 10)       |             // slot1
      (DR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      88,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_120,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank0
             (DDR4_RTT_WR_240 << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_240,              // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (LR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (LR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (LR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (LR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (LR_DIMM << 13),                    // slot2
      MCODT_50,                           // MC ODT encoding
      87,                                 // MC Vref percent
      82,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (LR_DIMM << 10)       |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      83,                                 // MC Vref percent
      76,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_120 << 6)   | (DDR4_RTT_NOM_34 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2133 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
   {
      // Config index
      (DDR_2400 << 0)       |             // freq
      (LR_DIMM << 7)        |             // slot0
      (EMPTY_DIMM << 10)    |             // slot1
      (EMPTY_DIMM << 13),                 // slot2
      MCODT_50,                           // MC ODT encoding
      75,                                 // MC Vref percent
      71,                                 // DRAM Vref percent
      {
         { // dimm0
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_60 << 3)   | DDR4_RTT_NOM_DIS,              // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm1
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         },
         { // dimm2
             // Rtt_wr              Rtt_prk               Rtt_nom
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank0
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank1
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS,             // rank2
             (DDR4_RTT_WR_DIS << 6)   | (DDR4_RTT_NOM_DIS << 3)   | DDR4_RTT_NOM_DIS              // rank3
         }
      }
   },
#endif
};

#ifdef DE_SKU
#define  MAX_DDR4_ODT_VALUE_CONFIG    40
#else
#define  MAX_DDR4_ODT_VALUE_CONFIG    85
#endif

#pragma pack()
#endif //ddr4_odt_value_table_h
