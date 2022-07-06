/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*************************************************************************
 *
 * QPI Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
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
 *
 ************************************************************************/
#include <Token.h> // AptioV server override
#include "SysFunc.h"
#include "QpiMisc.h"

// Extern definitions for generic SRP platform Eparam tables for HSX and BDX 
extern     HSX_ALL_LANES_EPARAM_LINK_INFO QpiGrantleySRPAllLanesEparamTable_HSX[];
extern     UINT32 QpiGrantleySRPAllLanesEparamTableSize_HSX;
extern     BDX_ALL_LANES_EPARAM_LINK_INFO QpiGrantleySRPAllLanesEparamTable_BDX[];
extern     UINT32 QpiGrantleySRPAllLanesEparamTableSize_BDX;
extern     HSX_PER_LANE_EPARAM_LINK_INFO  QpiGrantleySRP96GPerLaneEparamTable_HSX[];
extern     UINT32 QpiGrantleySRP96GPerLaneEparamTableSize_HSX;
extern     BDX_PER_LANE_EPARAM_LINK_INFO  QpiGrantleySRP96GPerLaneEparamTable_BDX[];
extern     UINT32 QpiGrantleySRP96GPerLaneEparamTableSize_BDX;

// Extern definitions for platform specific Eparam tables for HSX and BDX
extern     HSX_ALL_LANES_EPARAM_LINK_INFO  QpiAztecCityAllLanesEparamTable_HSX_B0[];
extern     UINT32 QpiAztecCityAllLanesEparamTableSize_HSX_B0;

extern     BDX_ALL_LANES_EPARAM_LINK_INFO  QpiAztecCityAllLanesEparamTable_BDX_A0[];
extern     UINT32 QpiAztecCityAllLanesEparamTableSize_BDX_A0;

extern     BDX_ALL_LANES_EPARAM_LINK_INFO  QpiAztecCityAllLanesEparamTable_BDX_B0[];
extern     UINT32 QpiAztecCityAllLanesEparamTableSize_BDX_B0;

extern     HSX_ALL_LANES_EPARAM_LINK_INFO  QpiIncaCityAllLanesEparamTable_HSX_B0[];
extern     UINT32 QpiIncaCityAllLanesEparamTableSize_HSX_B0;

extern     BDX_ALL_LANES_EPARAM_LINK_INFO  QpiIncaCityAllLanesEparamTable_BDX_B0[];
extern     UINT32 QpiIncaCityAllLanesEparamTableSize_BDX_B0;

extern     HSX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_HSX_A0[];
extern     UINT32 QpiMayanCityAllLanesEparamTableSize_HSX_A0;

extern     HSX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_HSX_B0[];
extern     UINT32 QpiMayanCityAllLanesEparamTableSize_HSX_B0;

extern     BDX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_BDX_A0[];
extern     UINT32 QpiMayanCityAllLanesEparamTableSize_BDX_A0;

extern     BDX_ALL_LANES_EPARAM_LINK_INFO  QpiMayanCityAllLanesEparamTable_BDX_B0[];
extern     UINT32 QpiMayanCityAllLanesEparamTableSize_BDX_B0;

// Extern definitions for BDX- platforms 9.6 GT/s per lane
extern BDX_PER_LANE_EPARAM_LINK_INFO  QpiMayanCity96GPerLaneEparamTable_BDX_A0[];
extern UINT32 QpiMayanCity96GPerLaneEparamTableSize_BDX_A0;

extern BDX_PER_LANE_EPARAM_LINK_INFO  QpiArandas96GPerLaneEparamTable_BDX_A0[];
extern UINT32 QpiArandas96GPerLaneEparamTableSize_BDX_A0;

extern BDX_PER_LANE_EPARAM_LINK_INFO  QpiArandas96GPerLaneEparamTable_BDX_B0[];
extern UINT32 QpiArandas96GPerLaneEparamTableSize_BDX_B0;

extern BDX_PER_LANE_EPARAM_LINK_INFO  QpiKahuna96GPerLaneEparamTable_BDX_A0[];
extern UINT32 QpiKahuna96GPerLaneEparamTableSize_BDX_A0;

extern BDX_PER_LANE_EPARAM_LINK_INFO  QpiAztecCity96GPerLaneEparamTable_BDX_A0[];
extern UINT32 QpiAztecCity96GPerLaneEparamTableSize_BDX_A0;

// Extern definitions for half speed Eparam tables for HSX and BDX
extern     HSX_ALL_LANES_HS_EPARAM_LINK_INFO QpiGrantley4SAllLanesHalfSpeedEparamTable_HSX[];
extern     UINT32 QpiGrantley4SAllLanesHalfSpeedEparamTableSize_HSX;
extern     BDX_ALL_LANES_HS_EPARAM_LINK_INFO QpiGrantley4SAllLanesHalfSpeedEparamTable_BDX[];
extern     UINT32 QpiGrantley4SAllLanesHalfSpeedEparamTableSize_BDX;



UINT8
OemGetPlatformType (
  struct sysHost          *host
  );



VOID
OemDebugPrintQpi (
  struct sysHost *host,
  UINT32 DbgLevel,
  char* Format,
  ...
  )
/*++

  QPI specific print to serial output

  @param host      - Pointer to the system host (root) structure
  @param DbgLevel  - QPI specific debug level
  @param Format    - string format

  @retval VOID

--*/
{

#ifdef SERIAL_DBG_MSG
  if (host->setup.qpi.DebugPrintLevel & DbgLevel) {
    va_list  Marker;
    va_start (Marker, Format);
    rcVprintf (host, Format, Marker);
// Grangeville AptioV server override - CPP Check
    va_end( Marker );
  }
#endif

  return;
}
VOID
OemWaitTimeForPSBP (
    struct sysHost *host,
    UINT32 *WaitTime
  )
{

  // Implement the hook here that changes the wait time in units of 1000us for PBSP to check in
  *WaitTime = 30000;    // Wait for a maximum of 30 sec
  return;
}

QPI_STATUS
OemQpiGetEParams (
  struct sysHost           *host,
  UINT8                    SocketID,
  UINT8                    Link,
  UINT8                    Freq,
  UINT8                    ProbeType,
  VOID                     *LinkSpeedParameter,
  UINT8                    SpeedType
  )
/*++

   Locates the platform EPARAMS based on socket/link

   @param host  - Pointer to the system host (root) structure
   @param SocketData - Socket specific data structure
   @param Socket - SocId we are looking up
   @param Link - Link# on socket
   *LinkSpeedParameter - return ptr
   SpeedType - FULL_SPEED or HALF_SPEED 

   @retval ptr to EPARAM table
   @retval QPI_SUCCESS if entry found
   @retval QPI_FAILURE if no entry found

--*/
{
  UINT8    Ctr;
  HSX_ALL_LANES_EPARAM_LINK_INFO       *ptr = NULL;
  HSX_ALL_LANES_HS_EPARAM_LINK_INFO    *ptr_half = NULL;
  HSX_PER_LANE_EPARAM_LINK_INFO        *per_lane_ptr = NULL;
  HSX_PER_LANE_EPARAM_LINK_INFO        TempPerLaneData;
  UINT32   SizeOfTable = 0, per_lane_SizeOfTable = 0;


  
  //
  // Handle multiple platforms.   Change QPI_PLATFORM_TYPE in qpiplatform.h to
  // choose minibios platform
  //
  switch (host->nvram.common.platformType) {
#ifndef GRANGEVILLE_FLAG
    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    case TypeLongBrookCRB:
      //Select "All Lanes" EParam table
      if (host->var.common.cpuType == CPU_HSX) {
        if (host->var.common.stepping >= B0_REV_HSX) {
          // Use Eparam values specific to Aztec City HSX Stepping B0 or greater
          SizeOfTable = QpiAztecCityAllLanesEparamTableSize_HSX_B0;  //Includes speeds upto 9.6 GT/s
          ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiAztecCityAllLanesEparamTable_HSX_B0;
        } else {
          // Use generic Eparam values for Grantley (as is done below)
          SizeOfTable = QpiGrantleySRPAllLanesEparamTableSize_HSX;  //Includes speeds < 9.6 GT/s
          ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiGrantleySRPAllLanesEparamTable_HSX;
        } // stepping check for HSX
 
        // Select "Per Lane" EParam table
        per_lane_ptr = (HSX_PER_LANE_EPARAM_LINK_INFO *) &QpiGrantleySRP96GPerLaneEparamTable_HSX;
        per_lane_SizeOfTable = QpiGrantleySRP96GPerLaneEparamTableSize_HSX;  //Includes speeds upto 9.6 GT/s
      } else {
        if (host->var.common.stepping >= B0_REV_BDX) {
          // Select Aztec City Eparam values for BDX-B0- and above
          SizeOfTable = QpiAztecCityAllLanesEparamTableSize_BDX_B0;  // Including the speeds of 9.6 GT/s
          ptr = (BDX_ALL_LANES_EPARAM_LINK_INFO *) &QpiAztecCityAllLanesEparamTable_BDX_B0;
        } else {
          // Select Aztec City Eparam values for BDX-A0/A1
          SizeOfTable = QpiAztecCityAllLanesEparamTableSize_BDX_A0;  //Excluding the speeds of 9.6 GT/s
          ptr = (BDX_ALL_LANES_EPARAM_LINK_INFO *) &QpiAztecCityAllLanesEparamTable_BDX_A0;

          // Select Aztec City 'Per Lane' EParam table for BDX-A0/A1
          per_lane_ptr = (BDX_PER_LANE_EPARAM_LINK_INFO *) &QpiAztecCity96GPerLaneEparamTable_BDX_A0;   // Only 9.6 GT/s
          per_lane_SizeOfTable = QpiAztecCity96GPerLaneEparamTableSize_BDX_A0;
        } // stepping check for BDX-
      } // cpuType check
      break;
      
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
      if (host->var.common.cpuType == CPU_HSX) {
        // Select "All Lanes" EParam table
        if (host->var.common.stepping >= B0_REV_HSX) {
          // Use Eparam values specific to Inca City HSX Stepping B0 or greater
          SizeOfTable = QpiIncaCityAllLanesEparamTableSize_HSX_B0;  //Includes speeds upto 9.6 GT/s
          ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiIncaCityAllLanesEparamTable_HSX_B0;
        } else {
        // Use generic Eparam values for Grantley (as is done below)
          SizeOfTable = QpiGrantleySRPAllLanesEparamTableSize_HSX;  //Includes speeds < 9.6 GT/s
          ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiGrantleySRPAllLanesEparamTable_HSX;
        } // stepping check for HSX

        // Select "Per Lane" EParam table
        per_lane_ptr = (HSX_PER_LANE_EPARAM_LINK_INFO *) &QpiGrantleySRP96GPerLaneEparamTable_HSX;
        per_lane_SizeOfTable = QpiGrantleySRP96GPerLaneEparamTableSize_HSX;  //Includes speeds upto 9.6 GT/s
      } else { // For CPU_BDX
        if (host->var.common.stepping >= B0_REV_BDX) {
          // From BDX-B0 and onwards
          SizeOfTable = QpiIncaCityAllLanesEparamTableSize_BDX_B0;  //Includes speeds upto 9.6 GT/s
          ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiIncaCityAllLanesEparamTable_BDX_B0;
        } else {
          // Select generic Eparam values for BDX
          SizeOfTable = QpiGrantleySRPAllLanesEparamTableSize_BDX;  //Includes speeds < 9.6 GT/s
          ptr = (BDX_ALL_LANES_EPARAM_LINK_INFO *) &QpiGrantleySRPAllLanesEparamTable_BDX;

          // Select "Per Lane" BDX EParam table
          per_lane_ptr = (BDX_PER_LANE_EPARAM_LINK_INFO *) &QpiGrantleySRP96GPerLaneEparamTable_BDX;
          per_lane_SizeOfTable = QpiGrantleySRP96GPerLaneEparamTableSize_BDX;  //Includes speed only for 9.6 GT/s
        } // stepping check
      } // cpuType check
      break;

    // Here are SRPs, CRBs, and STHIs
    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCityCRBDDR3:
    case TypeMayanCitySRPDDR3:
    case TypeEmeraldPoint:
    case TypeWildcatPass:
      if (host->var.common.cpuType == CPU_HSX) {
        if (host->var.common.stepping == A0_REV_HSX) {
          SizeOfTable = QpiMayanCityAllLanesEparamTableSize_HSX_A0;
          ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiMayanCityAllLanesEparamTable_HSX_A0; //Includes speed <= 9.6 GT/s
        } else {
          SizeOfTable = QpiMayanCityAllLanesEparamTableSize_HSX_B0;
          ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiMayanCityAllLanesEparamTable_HSX_B0; //Includes speed <= 9.6 GT/s
        } // stepping check for HSX
      }  else { // BDX- CPUs
      	if (host->var.common.stepping == A0_REV_BDX) { // Values for BDX-A0 and BDX-A1
          // Select the MayanCity Eparam values for BDX (These values were used for BDX-EP-P0)
          SizeOfTable = QpiMayanCityAllLanesEparamTableSize_BDX_A0;  // Excluding speeds of 9.6 GT/s
          ptr = (BDX_ALL_LANES_EPARAM_LINK_INFO *) &QpiMayanCityAllLanesEparamTable_BDX_A0;

          // Select MayanCity 'Per Lane' EParam table
          per_lane_ptr = (BDX_PER_LANE_EPARAM_LINK_INFO *) &QpiMayanCity96GPerLaneEparamTable_BDX_A0;   // Only 9.6 GT/s
          per_lane_SizeOfTable = QpiMayanCity96GPerLaneEparamTableSize_BDX_A0;
        } else { // For BDX-B0 and later
          SizeOfTable = QpiMayanCityAllLanesEparamTableSize_BDX_B0;    // includes 9.6 GT/s
          ptr = (BDX_ALL_LANES_EPARAM_LINK_INFO *) &QpiMayanCityAllLanesEparamTable_BDX_B0;        	
        } // stepping check
      } // cpuType check
      break;

    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
    case TypeHalfWidthCitySRP:
    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
      if (host->var.common.cpuType == CPU_HSX) {
        SizeOfTable = QpiGrantleySRPAllLanesEparamTableSize_HSX;
        ptr = (HSX_ALL_LANES_EPARAM_LINK_INFO *) &QpiGrantleySRPAllLanesEparamTable_HSX;

        per_lane_ptr = (HSX_PER_LANE_EPARAM_LINK_INFO *) &QpiGrantleySRP96GPerLaneEparamTable_HSX;
        per_lane_SizeOfTable = QpiGrantleySRP96GPerLaneEparamTableSize_HSX;
      } else {
        // Select generic Eparam values for BDX
        SizeOfTable = QpiGrantleySRPAllLanesEparamTableSize_BDX;  //Includes speeds < 9.6 GT/s
        ptr = (BDX_ALL_LANES_EPARAM_LINK_INFO *) &QpiGrantleySRPAllLanesEparamTable_BDX;

        // Select "Per Lane" BDX EParam table
        per_lane_ptr = (BDX_PER_LANE_EPARAM_LINK_INFO *) &QpiGrantleySRP96GPerLaneEparamTable_BDX;
        per_lane_SizeOfTable = QpiGrantleySRP96GPerLaneEparamTableSize_BDX;  //Includes speed only for 9.6 GT/s
      } // cpuType check
      break;
#endif
    default:
      if (OemPlatformSpecificEParam(host, &SizeOfTable, &per_lane_SizeOfTable, &ptr, &per_lane_ptr) == FALSE) {
        return QPI_FAILURE;
      }
  }

  if (SpeedType == FULL_SPEED){

     // Find per bundle Table first 
     while (SizeOfTable) {
        if ((ptr->SocketID == SocketID) && (ptr->ProbeType & (1<< ProbeType)) && (ptr->Freq & (1 << Freq)) && (ptr->Link == Link))
        {     
           //Convert to per lane data structure format from all lanes data structure           
           TempPerLaneData.SocketID = ptr->SocketID;
           TempPerLaneData.ProbeType = ptr->ProbeType; 
           TempPerLaneData.AllLanesUseSameTxeq = ALL_LANES_TXEQ_ENABLED;
           TempPerLaneData.Freq = ptr->Freq;
           TempPerLaneData.Link = ptr->Link;
           TempPerLaneData.PRT = ptr->PRT;
           TempPerLaneData.PTV = ptr->PTV;
           TempPerLaneData.TXEQL[0] = ptr->AllLanesTXEQ;

#ifndef GRANGEVILLE_FLAG
           if ((host->var.common.cpuType == CPU_HSX) &&
               (host->var.common.stepping == B0_REV_HSX) &&
               // Skip s4167155 W/A for platforms with a custom "All-Lanes" Eparam table for B0.
               (host->nvram.common.platformType != TypeAztecCityCRBSRP) &&
               (host->nvram.common.platformType != TypeAztecCitySTHI) &&
               (host->nvram.common.platformType != TypeIncaCityCRB) &&
               (host->nvram.common.platformType != TypeIncaCitySRP) &&
               (host->nvram.common.platformType != TypeMayanCityCRB) &&
               (host->nvram.common.platformType != TypeMayanCitySRP) &&
               (host->nvram.common.platformType != TypeMayanCityCRBDDR3) &&
               (host->nvram.common.platformType != TypeMayanCitySRPDDR3) &&
               (host->nvram.common.platformType != TypeWildcatPass)) {
             TempPerLaneData.CTLEPEAK[0] = 0x1E; //s4167155 set CTLE peaking to 0x1E for all sockets and all ports on all board designs for the B0
           } else {
#endif
             TempPerLaneData.CTLEPEAK[0] = ptr->CTLEPEAK;
#ifndef GRANGEVILLE_FLAG

           }
#endif
           MemCopy ((UINT8*)LinkSpeedParameter, (UINT8*) &TempPerLaneData, sizeof (HSX_PER_LANE_EPARAM_LINK_INFO));
         
           QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n  EPARAM entry found Socket %u Link %u", SocketID, Link));   
           QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  PRT=0x%x, PTV=0x%x, AllLanesTXEQ=0x%x, CTLEPEAK0=0x%x\n", ptr->PRT, ptr->PTV, ptr->AllLanesTXEQ , ptr->CTLEPEAK ));   
           return   QPI_SUCCESS;
        }
        ptr++;
        SizeOfTable -= sizeof(HSX_ALL_LANES_EPARAM_LINK_INFO);
     } //while  
      
    // Find per lane Table if we cannot find in per bundle table 
    while (per_lane_SizeOfTable) {
        if ((per_lane_ptr->SocketID == SocketID) && (per_lane_ptr->ProbeType & (1<< ProbeType)) && (per_lane_ptr->Freq & (1 << Freq)) && (per_lane_ptr->Link == Link))
        {
#ifndef GRANGEVILLE_FLAG
		  if ((host->var.common.cpuType == CPU_HSX) &&
		       (host->var.common.stepping == B0_REV_HSX) &&
               // Skip s4167155 W/A for platforms with a custom "Per-Lane" Eparam table for B0.
               (host->nvram.common.platformType != TypeMayanCityCRB) &&
               (host->nvram.common.platformType != TypeMayanCitySRP) &&
               (host->nvram.common.platformType != TypeMayanCityCRBDDR3) &&
               (host->nvram.common.platformType != TypeMayanCitySRPDDR3) &&
               (host->nvram.common.platformType != TypeWildcatPass) &&
               (host->nvram.common.platformType != TypeArandasVVP2S) &&
               (host->nvram.common.platformType != TypeArandasVVP4S)) {
		    //s4167155 set CTLE peaking to 0x1E for all sockets and all ports on all board designs for the B0
        per_lane_ptr->CTLEPEAK[0] = 0x1E1E1E1E;
        per_lane_ptr->CTLEPEAK[1] = 0x1E1E1E1E;
        per_lane_ptr->CTLEPEAK[2] = 0x1E1E1E1E;
        per_lane_ptr->CTLEPEAK[3] = 0x1E1E1E1E;
        per_lane_ptr->CTLEPEAK[4] = 0x1E1E1E1E;
      }
       
#endif
           MemCopy ((UINT8*)LinkSpeedParameter, (UINT8*)per_lane_ptr, sizeof (HSX_PER_LANE_EPARAM_LINK_INFO));
         
           QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n PER LANE EPARAM entry found Socket %u Link %u", SocketID, Link));   
           QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  PRT=0x%x, PTV=0x%x\n\n", per_lane_ptr->PRT, per_lane_ptr->PTV ));
           for (Ctr = 0; Ctr < MAX_QPI_LANES; Ctr = Ctr + 2)
           {
             QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  TXEQ_B[%d] = 0x%x %x\n", Ctr/2, per_lane_ptr->TXEQL[Ctr], per_lane_ptr->TXEQL[Ctr+1])); 
           }
           QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n  CTLE[0]=0x%x CTLE[1]=0x%x CTLE[2]=0x%x CTLE[3]=0x%x CTLE[4]=0x%x\n", 
           per_lane_ptr->CTLEPEAK[0], per_lane_ptr->CTLEPEAK[1], per_lane_ptr->CTLEPEAK[2], per_lane_ptr->CTLEPEAK[3], per_lane_ptr->CTLEPEAK[4]));
           return   QPI_SUCCESS;
        }

        per_lane_ptr++;
        per_lane_SizeOfTable -= sizeof(HSX_PER_LANE_EPARAM_LINK_INFO);
     } //while   
 
  } else {
       if (host->var.common.cpuType == CPU_HSX) {
         ptr_half = (HSX_ALL_LANES_HS_EPARAM_LINK_INFO *) &QpiGrantley4SAllLanesHalfSpeedEparamTable_HSX;
         SizeOfTable = QpiGrantley4SAllLanesHalfSpeedEparamTableSize_HSX;
       } else {
         ptr_half = (BDX_ALL_LANES_HS_EPARAM_LINK_INFO *) &QpiGrantley4SAllLanesHalfSpeedEparamTable_BDX;
         SizeOfTable = QpiGrantley4SAllLanesHalfSpeedEparamTableSize_BDX;
       } //cpuType check

      while (SizeOfTable) {
        if ((ptr_half->SocketID == SocketID) && (ptr_half->ProbeType & (1<< ProbeType)) && (ptr_half->Freq  & (1 << Freq)) && (ptr_half->Link == Link))
        {         
           MemCopy ((UINT8*)LinkSpeedParameter, (UINT8*)ptr_half, sizeof (HSX_ALL_LANES_HS_EPARAM_LINK_INFO));
         
           QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n  Half Speed EPARAM entry found Socket %u Link %u", SocketID, Link));   
           QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  TXEQB_HS0=0x%x\n", ptr_half->HS_AllLanesTXEQ)); 
           return   QPI_SUCCESS;
        }

        ptr_half++;
        SizeOfTable -= sizeof(HSX_ALL_LANES_HS_EPARAM_LINK_INFO);
      } // while loop
  }

  return QPI_FAILURE;
}

BOOLEAN
OemQpiGetMmioh(
   struct sysHost           *host,
   UINT16                   *MmiohSize,
   UINT16                   *MmiohBaseHi
   )
{
#if 1
  return FALSE;
#else
  MmiohSize[0]= 4;    // 4G
  MmiohSize[1]= 4;
  MmiohSize[2]= 4;
  MmiohSize[3]= 4;

  MmiohBaseHi[0] = 0x3800;
  MmiohBaseHi[1] = 0x3802;
  MmiohBaseHi[2] = 0x3804;
  MmiohBaseHi[3] = 0x3806;

#if MAX_CPU_SOCKETS > 4
  MmiohSize[4]= 4;    // 4G
  MmiohSize[5]= 4;
  MmiohSize[6]= 4;
  MmiohSize[7]= 4;

  MmiohBaseHi[4] = 0x3808;
  MmiohBaseHi[5] = 0x380a;
  MmiohBaseHi[6] = 0x380c;
  MmiohBaseHi[7] = 0x3810;
#endif

  return TRUE;
#endif
}

BOOLEAN
OemQpiGetMmiol(
   struct sysHost           *host,
   UINT8                    *MmiolSize,
   UINT32                   *MmiolBase
   )
{
#if 1
  return FALSE;
#else
  MmiolSize[0]= 4;    //4*64M
  MmiolSize[1]= 4;
  MmiolSize[2]= 4;
  MmiolSize[3]= 4;

  MmiolBase[0] = 0x90000000;
  MmiolBase[1] = 0xa0000000;
  MmiolBase[2] = 0xb0000000;
  MmiolBase[3] = 0xc0000000;

#if MAX_CPU_SOCKETS > 4
  MmiolSize[4]= 2;    //2*64M
  MmiolSize[5]= 2;
  MmiolSize[6]= 2;
  MmiolSize[7]= 2;

  MmiolBase[4] = 0xd8000000;
  MmiolBase[5] = 0xe0000000;
  MmiolBase[6] = 0xe8000000;
  MmiolBase[7] = 0xf0000000;
#endif

  return TRUE;
#endif
}

