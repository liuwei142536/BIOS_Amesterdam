/*++
  This file contains 'Framework Code' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may not be modified, except as allowed by
  additional terms of your license agreement.
--*/
/*++
// **************************************************************************
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2015 Intel Corp.                             *
// *                                                                        *
// *      This program has been developed by Intel Corporation.             *
// *      Licensee has Intel's permission to incorporate this source code   *
// *      into their product, royalty free.  This source code may NOT be    *
// *      redistributed to anyone without Intel's written permission.       *
// *                                                                        *
// *      Intel specifically disclaims all warranties, express or           *
// *      implied, and all liability, including consequential and other     *
// *      indirect damages, for the use of this code, including liability   *
// *      for infringement of any proprietary rights, and including the     *
// *      warranties of merchantability and fitness for a particular        *
// *      purpose.  Intel does not assume any responsibility for any        *
// *      errors which may appear in this code nor any responsibility to    *
// *      update it.                                                        *
// *                                                                        *
// **************************************************************************
// **************************************************************************
// *                                                                        *
// *  PURPOSE:                                                              *
// *                                                                        *
// *      Data structures for Electrical parameter Data for platform(s).    *
// *                                                                        *
// **************************************************************************
--*/
#include "QpiLib.h"
#include "QpiMisc.h"
#include "SysHost.h"
#include "SysFunc.h"
#include "QpiPlatform.h"

#pragma pack(1)
   
//
// Values for Grantley Aztec City
//
HSX_ALL_LANES_EPARAM_LINK_INFO  QpiAztecCityAllLanesEparamTable_HSX_B0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x353F353F, 6},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x333E343F, 6},
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x343E353F, 6},
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x363F363F, 6},
};

UINT32 QpiAztecCityAllLanesEparamTableSize_HSX_B0 = sizeof(QpiAztecCityAllLanesEparamTable_HSX_B0);


BDX_ALL_LANES_EPARAM_LINK_INFO  QpiAztecCityAllLanesEparamTable_BDX_A0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x353F353F, 6},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x333E343F, 6},
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x343E353F, 6},
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x363F363F, 6},
};

UINT32 QpiAztecCityAllLanesEparamTableSize_BDX_A0 = sizeof(QpiAztecCityAllLanesEparamTable_BDX_A0);


BDX_PER_LANE_EPARAM_LINK_INFO  QpiAztecCity96GPerLaneEparamTable_BDX_A0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQL[20], CTLEPEAK[5]
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 
              0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F, 0x3239383F,
              0x10101010, 0x10101010, 0x10101010, 0x10101010, 0x10101010}, 

        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK1, 0x322808, 0x05410102, 
              0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F,
              0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F, 0x3238393F,
              0x10101010, 0x10101010, 0x10101010, 0x10101010, 0x10101010}, 
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 
              0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F, 0x3038373F,      
              0x10101010, 0x10101010, 0x10101010, 0x10101010, 0x10101010}, 
              
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102,
              0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 
              0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F, 0x2C313A3F,
              0x10101010, 0x10101010, 0x10101010, 0x10101010, 0x10101010}
};

UINT32 QpiAztecCity96GPerLaneEparamTableSize_BDX_A0 = sizeof(QpiAztecCity96GPerLaneEparamTable_BDX_A0);

BDX_ALL_LANES_EPARAM_LINK_INFO  QpiAztecCityAllLanesEparamTable_BDX_B0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x343D363F, 2},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x343E353F, 5},
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x333C363F, 2},
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x333C363F, 2},
};

UINT32 QpiAztecCityAllLanesEparamTableSize_BDX_B0 = sizeof(QpiAztecCityAllLanesEparamTable_BDX_B0);


//
// Values for Grantley Inca City
//
HSX_ALL_LANES_EPARAM_LINK_INFO  QpiIncaCityAllLanesEparamTable_HSX_B0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x313E323F, 6},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x303B343F, 30},
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x313F313F, 30},
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x313B353F, 6},
               
   //Socket 2 **********************************************************************************************************************************************************************************************************************
         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x323C353F, 6},

         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x2D3A323F, 30},
         
   //Socket 3 **********************************************************************************************************************************************************************************************************************
         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x333D353F, 30},
     
         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x333E343F, 06}
};

UINT32 QpiIncaCityAllLanesEparamTableSize_HSX_B0 = sizeof(QpiIncaCityAllLanesEparamTable_HSX_B0);


BDX_ALL_LANES_EPARAM_LINK_INFO  QpiIncaCityAllLanesEparamTable_BDX_B0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x313C343F, 1},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x2D36363F, 12},
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x3238393F, 16},
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x313A363F, 3},
               
   //Socket 2 **********************************************************************************************************************************************************************************************************************
         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x313B353F, 3},

         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x2F39353F, 3},
         
   //Socket 3 **********************************************************************************************************************************************************************************************************************
         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x313C343F, 1},
     
         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x323A373F, 3}
};

UINT32 QpiIncaCityAllLanesEparamTableSize_BDX_B0 = sizeof(QpiIncaCityAllLanesEparamTable_BDX_B0);


//
// Values for Grantley Mayan City
//
HSX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_HSX_A0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x2934343F, 0xC},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x2932363F, 0x8},
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x2B37333F, 0x9},
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x2934343F, 0xC}
};

UINT32 QpiMayanCityAllLanesEparamTableSize_HSX_A0 = sizeof(QpiMayanCityAllLanesEparamTable_HSX_A0);

HSX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_HSX_B0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x2D37353F, 0x6},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x303B343F, 0x1E},
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x3039363F, 0x1E},
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x2E3A333F, 0x6}
};

UINT32 QpiMayanCityAllLanesEparamTableSize_HSX_B0 = sizeof(QpiMayanCityAllLanesEparamTable_HSX_B0);


//
// Values for Grantley Mayan City for BDX- (Used for PO with CTLEp as 1 and TxEQs as HSX-EP-C0)
//
BDX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_BDX_A0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x2D37353F, 1 },

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x303B343F, 1 },
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x3039363F, 1 },
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x2E3A333F, 1 }
};

UINT32 QpiMayanCityAllLanesEparamTableSize_BDX_A0 = sizeof(QpiMayanCityAllLanesEparamTable_BDX_A0);


BDX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_BDX_B0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x2F3A343F, 7 },

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x313B353F, 7 },
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x322808, 0x05410102, 0x323B363F, 7 },
     
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1, 0x322808, 0x05410102, 0x313A363F, 7 }
};

UINT32 QpiMayanCityAllLanesEparamTableSize_BDX_B0 = sizeof(QpiMayanCityAllLanesEparamTable_BDX_B0);


BDX_PER_LANE_EPARAM_LINK_INFO  QpiMayanCity96GPerLaneEparamTable_BDX_A0[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQL[20], CTLEPEAK[5]
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 
              0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F, 0x2B32383F,
              0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E}, 

        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK1, 0x322808, 0x05410102, 
              0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 
              0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F, 0x2C34373F,
              0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E}, 
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 
              0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F, 0x2B33373F,
              0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E}, 
              
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102,
              0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 
              0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F, 0x2B31393F,
              0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E, 0x1E1E1E1E}     
};

UINT32 QpiMayanCity96GPerLaneEparamTableSize_BDX_A0 = sizeof(QpiMayanCity96GPerLaneEparamTable_BDX_A0);


//
// Values for Grantley SRP
//
HSX_ALL_LANES_EPARAM_LINK_INFO  QpiGrantleySRPAllLanesEparamTable_HSX[] = {

  //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7},
     
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7}, 

         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7}, 
      
   //Socket 2 **********************************************************************************************************************************************************************************************************************
         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7},  

         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7}, 
      
   //Socket 3 **********************************************************************************************************************************************************************************************************************
         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7},   

         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),  
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7} 
  };

UINT32 QpiGrantleySRPAllLanesEparamTableSize_HSX = sizeof(QpiGrantleySRPAllLanesEparamTable_HSX);

HSX_PER_LANE_EPARAM_LINK_INFO  QpiGrantleySRP96GPerLaneEparamTable_HSX[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQL[20], CTLEPEAK[5]
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK1, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 
              
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

   //Socket 2 **********************************************************************************************************************************************************************************************************************
        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK0, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

   //Socket 3 **********************************************************************************************************************************************************************************************************************
        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),  
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}
     
};

UINT32 QpiGrantleySRP96GPerLaneEparamTableSize_HSX = sizeof(QpiGrantleySRP96GPerLaneEparamTable_HSX);


//
// Values for Grantley SRP - BDX
//
BDX_ALL_LANES_EPARAM_LINK_INFO  QpiGrantleySRPAllLanesEparamTable_BDX[] = {

  //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQ, CTLEPEAK
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7},

         {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7},
     
   //Socket 1 **********************************************************************************************************************************************************************************************************************
         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7}, 

         {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT), 
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7}, 
      
   //Socket 2 **********************************************************************************************************************************************************************************************************************
         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7},  

         {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7}, 
      
   //Socket 3 **********************************************************************************************************************************************************************************************************************
         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK0, 0x322808, 0x05410102, 0x2B35353F, 0x7},   

         {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),  
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT),
               LINK1, 0x322808, 0x05410102, 0x2B35353F, 0x7} 
  };

UINT32 QpiGrantleySRPAllLanesEparamTableSize_BDX = sizeof(QpiGrantleySRPAllLanesEparamTable_BDX);

BDX_PER_LANE_EPARAM_LINK_INFO  QpiGrantleySRP96GPerLaneEparamTable_BDX[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, PRT, PTV, TXEQL[20], CTLEPEAK[5]
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

        {0x0, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK1, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 
         
   //Socket 1 **********************************************************************************************************************************************************************************************************************
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 
              
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

   //Socket 2 **********************************************************************************************************************************************************************************************************************
        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK0, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102,
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

   //Socket 3 **********************************************************************************************************************************************************************************************************************
        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT), 
              LINK0, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}, 

        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),  
              PER_LANES_TXEQ_ENABLED,
              (1 << SPEED_REC_96GT),
              LINK1, 0x322808, 0x05410102, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 
              0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F, 0x2B35353F,
              0x7070707, 0x7070707, 0x7070707, 0x7070707, 0x7070707}
     
};

UINT32 QpiGrantleySRP96GPerLaneEparamTableSize_BDX = sizeof(QpiGrantleySRP96GPerLaneEparamTable_BDX);


//
// Values for Grantley Half Speed Parameters - HSX
//
HSX_ALL_LANES_HS_EPARAM_LINK_INFO  QpiGrantley4SAllLanesHalfSpeedEparamTable_HSX[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, TXEQ
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
  
        {0x0,  (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x2C32393F},

        {0x0,  (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1,0x2C32393F},
     
   //Socket 1 **********************************************************************************************************************************************************************************************************************
     
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
              LINK0, 0x2C32393F}, 

        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
              LINK1, 0x2C32393F}, 
      
   //Socket 2 **********************************************************************************************************************************************************************************************************************
        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK0, 0x2C32393F},  

        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK1, 0x2C32393F},  
      
   //Socket 3 **********************************************************************************************************************************************************************************************************************
        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK0, 0x2C32393F},   

        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),  
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK1, 0x2C32393F}        

};

UINT32 QpiGrantley4SAllLanesHalfSpeedEparamTableSize_HSX = sizeof (QpiGrantley4SAllLanesHalfSpeedEparamTable_HSX);        

//
// Values for Grantley Half Speed Parameters - BDX
//
BDX_ALL_LANES_HS_EPARAM_LINK_INFO  QpiGrantley4SAllLanesHalfSpeedEparamTable_BDX[] = {
   
   //
   //SocketID, ProbeType, Freq, Link, TXEQ
   //

   //Socket 0 **********************************************************************************************************************************************************************************************************************
  
        {0x0,  (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK0, 0x2C32393F},

        {0x0,  (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
               (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
               LINK1,0x2C32393F},
     
   //Socket 1 **********************************************************************************************************************************************************************************************************************
     
        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
              LINK0, 0x2C32393F}, 

        {0x1, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT), 
              LINK1, 0x2C32393F}, 
      
   //Socket 2 **********************************************************************************************************************************************************************************************************************
        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK0, 0x2C32393F},  

        {0x2, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK1, 0x2C32393F},  
      
   //Socket 3 **********************************************************************************************************************************************************************************************************************
        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR), 
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK0, 0x2C32393F},   

        {0x3, (1 << PROBE_TYPE_NO_PROBE) | (1 << PROBE_TYPE_COHASSET) | (1 << PROBE_TYPE_VSR),  
              (1 << SPEED_REC_64GT) | (1 << SPEED_REC_72GT) | (1 << SPEED_REC_80GT) | (1 << SPEED_REC_96GT),
              LINK1, 0x2C32393F}        

};

UINT32 QpiGrantley4SAllLanesHalfSpeedEparamTableSize_BDX = sizeof(QpiGrantley4SAllLanesHalfSpeedEparamTable_BDX);

#pragma pack()
