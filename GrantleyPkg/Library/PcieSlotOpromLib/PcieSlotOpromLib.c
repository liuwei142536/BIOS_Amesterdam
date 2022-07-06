/** 

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation
  (if any) is furnished under a license and may only be used or
  copied in accordance with the terms of the license.  Except as
  permitted by such license, no part of this software or
  documentation may be reproduced, stored in a retrieval system, or
  transmitted in any form or by any means without the express written
  consent of Intel Corporation.

  Module Name:  PcieSlotOpromLib.c

**/

#include <Token.h> // AptioV server override

#include <Library/PcieSlotOpromLib.h>

//
// Describes PCI-E slot location with each CRB
//
PLATFORM_PCIE_SLOT_STRUCT  MYCCRB [] = { 0x00000300,  //Slot 1
                                         0x00800100,  //Slot 3
                                         0x00800200,  //Slot 4
                                         0x00800302,  //Slot 6
                                         0x00000200,  //Slot 10
                                         0x00000100,  //Slot 11
                                         0x00001C05   //Slot 12
                                        };


PLATFORM_PCIE_SLOT_STRUCT  WCP [] = { 
0x00000100,  // Socket 0               // SAS Module Gen3 x8(1A-B)    
0x00000200,  //						   // IO Module Gen3 x8(2A-B)  
0x00000300,  // 			           // Slot1 Gen3 x16(3A-D)        

0x00800000,  // Socket 1               // Slot8 CPU1 DMI x4  2U PCIe Riser #3(for 2U Only)     
0x00800100,  //                        // Slot3 Gen3 x8(1A-B)
0x00800200,  //                        // Slot4 Gen3 x16(2A-D)
0x00800300,  //                        // Slot7 Gen3 x8 (3A-B)2U PCIe Riser #3(for 2U Only) 
0x00800302,  //                        // Slot6 Gen3 x8(3C-D)
                                       };


#ifndef GRANGEVILLE_FLAG
PLATFORM_PCIE_SLOT_STRUCT  MYCSRP [] = { 0x00000300,  //Slot 1
                                         0x00800100,  //Slot 3
                                         0x00800200,  //Slot 4
                                         0x00800302,  //Slot 6
                                         0x00000200,  //Slot 10
                                         0x00000100,  //Slot 11
                                         0x00001C05,  //Slot 12
                                         0x00800300   //Slot 14
                                        };
                                         
PLATFORM_PCIE_SLOT_STRUCT  YKMCCRB [] = { 0x00001C05,  //Slot 1
                                          0x00800100,  //Slot 2
                                          0x00000100,  //Slot 3
                                          0x00800302,  //Slot 4
                                          0x00800300,  //Slot 5
                                          0x00000300   //Slot 6
                                        };
                                        
PLATFORM_PCIE_SLOT_STRUCT  AZTCCRB [] = { 0x00000100,  //Slot 1
                                          0x00800200,  //Slot 2
                                          0x00001C05,  //Slot 3
                                          0x00000300,  //Slot 4
                                          0x00800100,  //Slot 5
                                          0x00000200   //Slot 6
                                        };
                                        
PLATFORM_PCIE_SLOT_STRUCT  INCCRB [] = { 0x00C00300,  //Slot 1
                                         0x00000200,  //Slot 2
                                         0x00C00200,  //Slot 3
                                         0x00800300,  //Slot 4
                                         0x00400200,  //Slot 5
                                         0x00800200   //Slot 6
                                        };
                                        
PLATFORM_PCIE_SLOT_STRUCT  KHDVP [] = { 0x00000200,  //Slot 0
                                        0x00000300,  //Slot 4
                                        0x00800200,  //Slot 5
                                        0x00800300,  //Slot 6
                                        0x00800100,  //Slot 10
                                        0x00000100   //Slot 13
                                      };
                                      
PLATFORM_PCIE_SLOT_STRUCT  ADVVP [] = { 0x00400100,  //Slot 3
                                        0x00400300,  //Slot 4
                                        0x00400200,  //Slot 5
                                        0x00000100,  //Slot 6
                                        0x00000200,  //Slot 7
                                        0x00C00300,  //Slot 8
                                        0x00800200   //Slot 9
                                      };

#endif

EFI_PLATFORM_INFO                       *mPlatformInfo;

BOOLEAN
CheckDeviceByRp (
  IN UINTN                      Bus,
  IN UINT32                     CpuNum,
  IN UINT32                     RtPrtDevNum,
  IN UINT32                     RtPrtFunNum
  )
/*++

Routine Description:
  
  Use the PCI Root Bridge protocol and check the Bus number be match Root Bridge or not.

Arguments:

  Bus           -  PCI Bus number.
  CpuNum        -  Number of Slot Bus number.
  RtPrtDevNum   -  Number of Slot Device number.
  RtPrtFunNum   -  Number of Slot Function number.

Returns:
  
  TRUE    -  This device on the PCI-E slot.
  FALSE   -  This is onboard device.

--*/
{
  EFI_STATUS                          Status;
  BOOLEAN                             OnSlot = FALSE;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *gPciRootBridgeIo;
  UINT64                              Address;
  UINT8                               SlotSecBusNum;
  UINT8                               SlotSubBusNum;
  
  //
  // Get PCI Root Bridge Io Protocol
  //
  Status = gBS->LocateProtocol(
	      	    &gEfiPciRootBridgeIoProtocolGuid,
	  	        NULL,
	  	        &gPciRootBridgeIo
	          );

//
// Get the slot sub bus number and second bus number, 
// If the device on the slot it's bus number will between sub bus and second bus.
//
  Address = EFI_PCI_ADDRESS (
              CpuNum, 
              RtPrtDevNum, 
              RtPrtFunNum, 
              PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET 
            );

  Status = gPciRootBridgeIo->Pci.Read(
              gPciRootBridgeIo,
              EfiPciWidthUint8,
              Address,
              1,
              &SlotSecBusNum
            );
                      
  Address = EFI_PCI_ADDRESS (
              CpuNum, 
              RtPrtDevNum, 
              RtPrtFunNum, 
              PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET 
            );
                      
  Status = gPciRootBridgeIo->Pci.Read(
              gPciRootBridgeIo,
              EfiPciWidthUint8,
              Address,
              1,
              &SlotSubBusNum
            );
                      
  if ( ( Bus >= SlotSecBusNum ) && ( Bus <= SlotSubBusNum ) ) {
    OnSlot = TRUE;
  }

  return OnSlot;
}

BOOLEAN
PcieRootPortCheck (
  IN  UINTN                 Bus,
  IN  SYSTEM_CONFIGURATION  *mSystemConfiguration
  )
/*++

Routine Description:
  
  Check the device connect to PCI-E slot or not.

Arguments:

  Bus                   -  PCI Bus number.
  mSystemConfiguration  -  Setup data.

Returns:
  
  TRUE    -  This device on the PCI-E slot and it's Oprom not be run.
  FALSE   -  We wiil run the Oprom from device.

--*/
{
  BOOLEAN                             OnSlot = FALSE;
  UINT16                              Count;
  UINT16                              TableCount = 0;
  EFI_HOB_GUID_TYPE                   *GuidHob;
  UINT8                               SetupInfo[8] = {0}; 
  
  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  mPlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (mPlatformInfo);

  if ( mPlatformInfo != NULL ) {
  
    //
    // Get the setup question informaion.
    //
    SetupInfo[0] = mSystemConfiguration->PcieSlotOprom1;
    SetupInfo[1] = mSystemConfiguration->PcieSlotOprom2;
    SetupInfo[2] = mSystemConfiguration->PcieSlotOprom3;
    SetupInfo[3] = mSystemConfiguration->PcieSlotOprom4;
    SetupInfo[4] = mSystemConfiguration->PcieSlotOprom5;
    SetupInfo[5] = mSystemConfiguration->PcieSlotOprom6;
    SetupInfo[6] = mSystemConfiguration->PcieSlotOprom7;
    SetupInfo[7] = mSystemConfiguration->PcieSlotOprom8;
    
	//
	// Check the platform type and set PCI-E slot describes.
	//
    switch(mPlatformInfo->BoardId) {
#ifndef GRANGEVILLE_FLAG    
	  //
	  // Use MYCCRB table check for TypeMayanCityCRB, TypeMayanCityCRBDDR3
	  //
      case TypeMayanCityCRB:
      case TypeMayanCityCRBDDR3:
      default:
  
        TableCount = (( sizeof ( MYCCRB )) / ( sizeof ( UINT32 )));
        DEBUG((EFI_D_INFO, "TableCount - %x\n", TableCount));
        
        for ( Count = 0 ; Count < TableCount ; Count++ ) {
          if ( !SetupInfo[Count] ) {
          
            OnSlot = CheckDeviceByRp ( 
                        Bus, 
                        MYCCRB[Count].RootPrt.CpuNum,
                        MYCCRB[Count].RootPrt.RtPrtDevNum,
                        MYCCRB[Count].RootPrt.RtPrtFunNum
                      );
          }
        }

        break;
      
	  //
	  // Use MYCSRP table check for TypeMayanCitySRP, TypeMayanCitySRPDDR3.
	  //
      case TypeMayanCitySRP:  
      case TypeMayanCitySRPDDR3:
  
        TableCount = (( sizeof ( MYCSRP )) / ( sizeof ( UINT32 )));
        DEBUG((EFI_D_INFO, "TableCount - %x\n", TableCount));
        
        for ( Count = 0 ; Count < TableCount ; Count++ ) {
          if ( !SetupInfo[Count] ) {
          
            OnSlot = CheckDeviceByRp ( 
                        Bus, 
                        MYCSRP[Count].RootPrt.CpuNum,
                        MYCSRP[Count].RootPrt.RtPrtDevNum,
                        MYCSRP[Count].RootPrt.RtPrtFunNum
                      );
          }
        }

        break;
      
	  //
	  // Use WCP table check for TypeWildcatPass
	  //
      case TypeWildcatPass:
  
        TableCount = (( sizeof ( WCP )) / ( sizeof ( UINT32 )));
        DEBUG((EFI_D_INFO, "TableCount - %x\n", TableCount));
        
        for ( Count = 0 ; Count < TableCount ; Count++ ) {
          if ( !SetupInfo[Count] ) {
          
            OnSlot = CheckDeviceByRp ( 
                        Bus, 
                        WCP[Count].RootPrt.CpuNum,
                        WCP[Count].RootPrt.RtPrtDevNum,
                        WCP[Count].RootPrt.RtPrtFunNum
                      );
          }
        }

        break;
      
	  //
	  // Use YKMCCRB table check for TypeYakimaCityCRB, TypeYakimaCitySRP, TypeYakimaCitySTHI.
	  //
      case TypeYakimaCityCRB:
      case TypeYakimaCitySRP:  
      case TypeYakimaCitySTHI:
  
        TableCount = (( sizeof ( YKMCCRB )) / ( sizeof ( UINT32 )));
        DEBUG((EFI_D_INFO, "TableCount - %x\n", TableCount));
        
        for ( Count = 0 ; Count < TableCount ; Count++ ) {
          if ( !SetupInfo[Count] ) {
          
            OnSlot = CheckDeviceByRp ( 
                        Bus, 
                        YKMCCRB[Count].RootPrt.CpuNum,
                        YKMCCRB[Count].RootPrt.RtPrtDevNum,
                        YKMCCRB[Count].RootPrt.RtPrtFunNum
                      );
          }
        }

        break;
      
	  //
	  // Use AZTCCRB table check for TypeAztecCityCRBSRP, TypeAztecCitySTHI.
	  //
      case TypeAztecCityCRBSRP:  
      case TypeAztecCitySTHI:
  
        TableCount = (( sizeof ( AZTCCRB )) / ( sizeof ( UINT32 )));
        DEBUG((EFI_D_INFO, "TableCount - %x\n", TableCount));
        
        for ( Count = 0 ; Count < TableCount ; Count++ ) {
          if ( !SetupInfo[Count] ) {
          
            OnSlot = CheckDeviceByRp ( 
                        Bus, 
                        AZTCCRB[Count].RootPrt.CpuNum,
                        AZTCCRB[Count].RootPrt.RtPrtDevNum,
                        AZTCCRB[Count].RootPrt.RtPrtFunNum
                      );
          }
        }
        
        break;
      
	  //
	  // Use INCCRB table check for TypeIncaCityCRB, TypeIncaCitySRP.
	  //
      case TypeIncaCityCRB:  
      case TypeIncaCitySRP:
  
        TableCount = (( sizeof ( INCCRB )) / ( sizeof ( UINT32 )));
        DEBUG((EFI_D_INFO, "TableCount - %x\n", TableCount));
        
        for ( Count = 0 ; Count < TableCount ; Count++ ) {
          if ( !SetupInfo[Count] ) {
          
            OnSlot = CheckDeviceByRp ( 
                        Bus, 
                        INCCRB[Count].RootPrt.CpuNum,
                        INCCRB[Count].RootPrt.RtPrtDevNum,
                        INCCRB[Count].RootPrt.RtPrtFunNum
                      );
          }
        }
        
        break;
#else
   
  //
  // Use MYCCRB table check for TypeMayanCityCRB, TypeMayanCityCRBDDR3
  //
  case TypeBeverlyCoveCRB:
  case TypeBeverlyCoveSODimmCRB:
  
    TableCount = (( sizeof ( MYCCRB )) / ( sizeof ( UINT32 )));
    DEBUG((EFI_D_INFO, "TableCount - %x\n", TableCount));
        
    for ( Count = 0 ; Count < TableCount ; Count++ ) {
      if ( !SetupInfo[Count] ) {
         
        OnSlot = CheckDeviceByRp ( 
                 Bus, 
                 MYCCRB[Count].RootPrt.CpuNum,
                 MYCCRB[Count].RootPrt.RtPrtDevNum,
                 MYCCRB[Count].RootPrt.RtPrtFunNum
                 );
       }
    }

    break;
#endif      
	
  }

  	}
  //
  // Return the result.
  //
  return OnSlot;
}

VOID
SetupPcieSlotNumber (
  IN  SYSTEM_CONFIGURATION  *mSystemConfiguration
  )
/*++

Routine Description:
  
  Count of the PCI-E slot by CRB.

Arguments:

  mSystemConfiguration  -  Setup data.

Returns:
  
  VOID.

--*/
{
  EFI_HOB_GUID_TYPE                   *GuidHob;
  
  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  mPlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (mPlatformInfo);
  
  if ( mPlatformInfo != NULL ) {
    
	//
	// Check the platform type and set count of internal PCI-E slot.
	//
	switch(mPlatformInfo->BoardId) {
#ifndef GRANGEVILLE_FLAG      
	  //
	  // Set the PCI-E slot count to 7.
	  //
      case TypeMayanCityCRB:
      case TypeMayanCityCRBDDR3:

      
        mSystemConfiguration->PcieSlotItemCtrl = 1;
      break;
      
	  //
	  // Set the PCI-E slot count to 8.
	  //
      case TypeMayanCitySRP:  
      case TypeMayanCitySRPDDR3:
      case TypeWildcatPass:
        mSystemConfiguration->PcieSlotItemCtrl = 2;
      break;
      
	  //
	  // Set the PCI-E slot count to 6.
	  //
      case TypeYakimaCityCRB:
      case TypeYakimaCitySRP:  
      case TypeYakimaCitySTHI:
      case TypeAztecCityCRBSRP:  
      case TypeAztecCitySTHI:
      case TypeIncaCityCRB:  
      case TypeIncaCitySRP:


      default:
        mSystemConfiguration->PcieSlotItemCtrl = 0;
      break;
    }
#else
  case TypeBeverlyCoveCRB:
  case TypeBeverlyCoveSODimmCRB:
    mSystemConfiguration->PcieSlotItemCtrl = 1;
    break;
   }   

#endif
  }
}

