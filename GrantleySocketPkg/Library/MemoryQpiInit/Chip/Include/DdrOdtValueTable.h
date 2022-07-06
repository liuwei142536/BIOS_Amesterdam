//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
#ifndef _odt_value_table_h
#define _odt_value_table_h

#include "DataTypes.h"

#pragma pack(1)
struct odtValueStruct odtValueTable[] = {

   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_120,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_120,         // rank0
             (RTT_WR_INF << 4)   | RTT_NOM_120,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_INF << 4)   | RTT_NOM_40,          // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (QR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (QR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (QR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (QR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (QR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (QR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_60,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (QR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_20,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank2
             (RTT_WR_120 << 4)   | RTT_NOM_INF          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (SR_DIMM << 7)        | // slot0
      (DR_DIMM << 10)       | // slot1
      (SR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (DR_DIMM << 7)        | // slot0
      (SR_DIMM << 10)       | // slot1
      (DR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_120 << 4)   | RTT_NOM_INF,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (LR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (LR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (LR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (LR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (LR_DIMM << 13),        // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (LR_DIMM << 7)        | // slot0
      (LR_DIMM << 10)       | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_120 << 4)   | RTT_NOM_30,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_800 << 0)        | // freq
      (LR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_100,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_120,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1066 << 0)      | // freq
      (LR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_120,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1333 << 0)       | // freq
      (LR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_80,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1600 << 0)       | // freq
      (LR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_60,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
   {
      // Config index
      (DDR_1866 << 0)      | // freq
      (LR_DIMM << 7)        | // slot0
      (EMPTY_DIMM << 10)    | // slot1
      (EMPTY_DIMM << 13),     // slot2
      MCODT_50,
      {
         { // dimm0
             // Rtt_wr           Rtt_nom
             (RTT_WR_INF << 4)   | RTT_NOM_40,          // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm1
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         },
         { // dimm2
             // Rtt_wr           Rtt_nom
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank0
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank1
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS,         // rank2
             (RTT_WR_DIS << 4)   | RTT_NOM_DIS          // rank3
         }
      }
   },
};

#define  MAX_ODT_VALUE_CONFIG    115

#pragma pack()
#endif //_odt_value_table_h
