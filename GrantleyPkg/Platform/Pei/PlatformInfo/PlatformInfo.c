/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  PlatformInfo.c
  
Abstract:

  Platform Info PEIM.

--*/

#include "PlatformInfo.h"
#include "Library\PchPlatformLib.h"
//#include <platform/spifvbservices/SpiFlashDevice.h> AptioV server override: Removed file not available in aptioV
#include <Setup.h>  //AptioV server override: Setting  value of Pci64BitResourceAllocation based on Above4gDecode setup control from PCI bus driver

#define  TEMP_BUS_NUMBER    (0x3F)

EFI_GUID gSetupGuid   = SETUP_GUID;   //AptioV server override: Setting  value of Pci64BitResourceAllocation based on Above4gDecode setup control from PCI bus driver

static EFI_PEI_PPI_DESCRIPTOR       mPlatformInfoPpi = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPlatformInfoGuid,
    NULL
  };


UINT32  
IsSioExist (
  VOID
)
{
  //
  // IBMC will decode 0x4E/0x4F
  // 
  UINT8   SioExit;
  UINT32  Delay;

  SioExit = 0;

  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_UNLOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_LOGICAL_DEVICE);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIII_SIO_DATA_PORT , PILOTIII_SIO_COM1);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  if (IoRead8  (PILOTIII_SIO_DATA_PORT) == PILOTIII_SIO_COM1) {
    SioExit |= PILOTIII_EXIST;
  }
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_LOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);

  //
  // PC SIO will decode 0x2E/0x2F
  //
  IoWrite8 (PC8374_SIO_INDEX_PORT, PC8374_LOGICAL_DEVICE);
  IoWrite8 (PC8374_SIO_DATA_PORT , PC8374_SIO_COM1);
  if (IoRead8  (PC8374_SIO_DATA_PORT) == PC8374_SIO_COM1) {
    SioExit |= PC8374_EXIST;
  }

  return SioExit;
}

VOID
GetCpuInfo (
  UINT8     *CpuType,
  UINT8     *CpuStepping
  )
/*++

Routine Description:
   Returns the Model ID of the CPU. 
   Model ID = EAX[7:4] 

Returns:

--*/

{
  UINT32                    RegEax=0;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  *CpuStepping = (UINT8)(RegEax & 0x0F);
  *CpuType     = (UINT8)((RegEax & 0xF0) >> 4);
}


VOID
InitGSX(
  UINT32                *BAR,
  IN EFI_PEI_SERVICES  **PeiServices
)
{
  UINT32                  Data32;
  UINT32                  GSXBAR;
  UINT32                  Rcba;

  //
  // Have to enable the RCRB first
  // 
  Rcba = *(volatile UINT32*) ((UINTN) (PcdGet64(PcdPciExpressBaseAddress)) + (UINTN) (DEFAULT_PCI_BUS_NUMBER_PCH  << 20) + \
             (UINTN) (PCI_DEVICE_NUMBER_PCH_LPC   << 15) + (UINTN) (PCI_FUNCTION_NUMBER_PCH_LPC << 12) + R_PCH_LPC_RCBA ); 
  
  if (!(Rcba & B_PCH_LPC_RCBA_EN)) {
   *(volatile UINT32*) ((UINTN) (PcdGet64(PcdPciExpressBaseAddress)) + (UINTN) (DEFAULT_PCI_BUS_NUMBER_PCH  << 20) + \
             (UINTN) (PCI_DEVICE_NUMBER_PCH_LPC   << 15) + (UINTN) (PCI_FUNCTION_NUMBER_PCH_LPC << 12) + R_PCH_LPC_RCBA ) =  
            ((PCH_RCBA_ADDRESS & B_PCH_LPC_RCBA_BAR) | B_PCH_LPC_RCBA_EN);
  }
  //
  // Read GSX Base addres: Base Address mem space is aligned to 1KB
  //
  DEBUG ((EFI_D_ERROR, "Initialize GSX \n"));
  Data32 =  *(volatile UINT32 *)(UINT32)(PCH_RCBA_ADDRESS + R_PCH_RCRB_GSXBAR);

  GSXBAR =  Data32 & 0xFFFFFC00;
  if(GSXBAR == 0){
     // set BAR to default values
     *(volatile UINT32 *)(UINT32)(PCH_RCBA_ADDRESS + R_PCH_RCRB_GSXBAR) = 0xFED04000; 
     GSXBAR = *(volatile UINT32 *)(UINT32)(PCH_RCBA_ADDRESS + R_PCH_RCRB_GSXBAR);    
  }

  *BAR  = GSXBAR;
  DEBUG ((EFI_D_ERROR, "GSXBAR = 0x%x \n", GSXBAR));
  //
  // GSX Base addres: Base Address mem space is aligned to 1KB
  //
  Data32 =  (1<<4);
  *(volatile UINT32 *)(UINT32)(PCH_RCBA_ADDRESS + R_PCH_RCRB_GSXCTRL) = Data32;
  //
  // Cx CAP All Boards have 2 Input Expanders 0 0utput Expanders
  //
  Data32 =  2;
  *(volatile UINT32 *)(UINT32)(GSXBAR + R_SB_GSX_CxCAP) = Data32; 
}


EFI_STATUS 
GsxRead(
   UINT32                *Data,
   IN EFI_PEI_SERVICES  **PeiServices
)
{
  UINT32                    GSXBAR;
  UINT32                    Busy;
  UINT32                    CxGPILVL;
  UINT32                    CxGPILVL_DW1;


  InitGSX(&GSXBAR,PeiServices);
  //
  // Check there are no pending transactions
  //
  Busy  = (*(volatile UINT32 *)(UINT32)(GSXBAR + R_SB_GSX_CxCMD)) & (0x1<<1);
  if (!Busy){
     //
     // SW Start serialization process
     //
     *(volatile UINT32 *)(UINT32)(GSXBAR + R_SB_GSX_CxCMD) = 0x1;

     //
     // Wait for the read cycle to finish
     //
     DEBUG ((EFI_D_ERROR, "Waiting GSX command to complete \n"));
     do{
         Busy  = (*(volatile UINT32 *)(UINT32)(GSXBAR + R_SB_GSX_CxCMD)) & (0x1<<1);
     }while(Busy);

     //
     // Read GPIO Input bits [31:0]
     //
     CxGPILVL      = *(volatile UINT32 *)(UINT32)(GSXBAR + R_SB_GSX_CxGPILVL);
     //
     // Read GPIO Input bits [63:32]
     //
     CxGPILVL_DW1  = *(volatile UINT32 *)(UINT32)(GSXBAR + R_SB_GSX_CxGPILVL_DW1);

     Data[0]       = CxGPILVL;
     Data[1]       = CxGPILVL_DW1;

     return EFI_SUCCESS;

  }else{ 
     //
     // GSX operation in transit can not read at this time
     // 
     DEBUG ((EFI_D_ERROR, "GSX is Busy \n"));
     return EFI_UNSUPPORTED;
  }
}

VOID
GetGsxBoardID(
   BOARD_ID             *Data,
   IN EFI_PEI_SERVICES  **PeiServices
)
{

  EFI_STATUS                Status;
  UINT32                    GSXIN[2];
  UINT32                    RetryCount;

  RetryCount = 0;
  GSXIN[0]   = 0;
  GSXIN[1]   = 0;
  
  do {
    Status     = GsxRead(GSXIN, PeiServices);

    if(Status){
      // if EFI_SUCCESS != Success then retry one more time
      RetryCount ++;
    }else{
      // if EFI_SUCCESS read Board ID and exit
      RetryCount = 0xFFFFFFFF;
    }

    if (GSXIN[0] & BIT0)
      Data->BoardID.BoardID0 = 1;

    if (GSXIN[0] & BIT1)
      Data->BoardID.BoardID1 = 1;

    if (GSXIN[0] & BIT2)
      Data->BoardID.BoardID2 = 1;

    if (GSXIN[0] & BIT3)
      Data->BoardID.BoardID3 = 1;

    if (GSXIN[0] & BIT4)
      Data->BoardID.BoardID4 = 1;

    if (GSXIN[0] & BIT5)
      Data->BoardID.BoardRev0 = 1;

    if (GSXIN[0] & BIT6)
      Data->BoardID.BoardRev1 = 1;

  } while(RetryCount < 1);
   
  if(Status){
    //
    // Unhable to read GSX HW error Hang the system
    //
    DEBUG ((EFI_D_ERROR, "ERROR: GSX HW is unavailable, SYSTEM HANG\n")); 
    CpuDeadLoop();
  }
}

UINT8
ReadGpio (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN UINT8                      GpioNumber
  )
/*++

Routine Description:

  Return value of GPIO pin

Arguments:

  PeiServices - Pointer to PeiServices
  GpioNumber  - GPIO pin to read

Returns:

  Level at GPIO pin

--*/
{
  UINT8           GpioUseSel;
  UINT8           GpioIoSel;
  UINT8           GpioLvl;
  UINT32          GpioData;
  UINT32          MaskValue;
    
  MaskValue = (1 << (GpioNumber % 32));
    
  if (GpioNumber < 32) {
    GpioUseSel  = R_PCH_GPIO_USE_SEL;
    GpioIoSel   = R_PCH_GPIO_IO_SEL;
    GpioLvl     = R_PCH_GPIO_LVL;

  } else if (GpioNumber < 64) {
    GpioUseSel  = R_PCH_GPIO_USE_SEL2;
    GpioIoSel   = R_PCH_GPIO_IO_SEL2;
    GpioLvl     = R_PCH_GPIO_LVL2;
  } else {
    GpioUseSel  = R_PCH_GPIO_USE_SEL3;
    GpioIoSel   = R_PCH_GPIO_IO_SEL3;
    GpioLvl     = R_PCH_GPIO_LVL3;
  }

  //
  // Note that GPIO init is done later, so we need to enable
  // the GPI pins for input.
  //
  // Select GPIO.
  //
  GpioData = IoRead32 (PCH_GPIO_BASE_ADDRESS + GpioUseSel);
  GpioData |= MaskValue;
  IoWrite32 (PCH_GPIO_BASE_ADDRESS + GpioUseSel, GpioData);
  
  //
  // Set GPIO.
  //
  GpioData = IoRead32 (PCH_GPIO_BASE_ADDRESS + GpioIoSel);
  GpioData |= MaskValue;
  IoWrite32 (PCH_GPIO_BASE_ADDRESS + GpioIoSel, GpioData);

  //
  // Get the value of GPIO.
  //
  GpioData = IoRead32 (PCH_GPIO_BASE_ADDRESS + GpioLvl);
    
  if (GpioData & MaskValue) {
    return 1;
  } else {
    return 0;
  }
}

//AptioVserver override start: Function call is commented so comment function definition also.
/*
//
// Initialize SpiPpi interface
// Initialization will be performed on first call. Routine exit with no action if called more than once. 
//
EFI_STATUS
SpiPpiInit (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
//#if DESCRIPTOR_MODE_IMAGE
  EFI_STATUS              Status;
  EFI_SPI_PROTOCOL        *SpiPpi;
  UINT8                   FlashID[3];
  UINT8                   Index;   
  UINT8                   SpiStatus;
  UINT8                   FlashIndex;
 
  //
  // Locate the SPI PPI Interface
  //
  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gPeiSpiPpiGuid,
                    0,
                    NULL,
                    &SpiPpi
                    );
   
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SpiPpi->ReadId (SpiPpi, 0, FlashID);
  
  if (EFI_ERROR (Status)) {
    return Status;
  }
  SpiStatus = 0; //Flag to indicate if SPI valid was found 
    for (Index = 0; (Index < EnumSpiFlashMax) && (SpiStatus == 0); Index++){
	for (FlashIndex = 0; FlashIndex < mSpiInitTable[Index].TypeDataNum; FlashIndex++) {
	  if ((mSpiInitTable[FlashIndex].VendorId == FlashID[0]) &&
		  (mSpiInitTable[FlashIndex].TypeData.DeviceId0 == FlashID[1]) &&
		  (mSpiInitTable[FlashIndex].TypeData.DeviceId1 == FlashID[2])) {
		  //
		  // Supported SPI device found
		  //
		  DEBUG (
			((DEBUG_ERROR | EFI_D_INFO),
			"Supported SPI Flash device found, Vendor Id: 0x%02x, Device ID: 0x%02x%02x!\n",
			FlashID[0],
			FlashID[1],
			FlashID[2])
			);
  
			 mSpiInitTable[FlashIndex].SpiInitTable.BiosStartOffset = mSpiInitTable[FlashIndex].TypeData.BiosStartOffset;
			 Status = SpiPpi->Init(SpiPpi,
											  &(mSpiInitTable[FlashIndex].SpiInitTable)
											  );
			 SpiStatus = 1;
		  break;
		}
	  }
	}

  return Status;
  }
*/
//AptioVserver override End: Function call is commented so comment function definition also.

  //
// SPI Descriptor Mode Routines for Accessing Platform Info from Platform Data Region (PDR)
//
EFI_STATUS
PdrGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  )
{
//#if DESCRIPTOR_MODE_IMAGE
//AptioVserver override start: PDR region is not supported.So commenting code routine definition and returning EFI_UNSUPPORTED
/*  EFI_STATUS              Status;
  EFI_SPI_PROTOCOL        *SpiPpi;
  UINTN                   Size;
 
  //
  // Locate the SPI PPI Interface
  //
  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gPeiSpiPpiGuid,
                    0,
                    NULL,
                    &SpiPpi
                    );
   
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize SpiPpi interface
  //
  Status = SpiPpiInit(PeiServices);

  //
  // Read the PIT (Platform Info Table) from the SPI Flash Platform Data Region
  //
  Size = sizeof (EFI_PLATFORM_INFO);
  Status = SpiPpi->Execute (
                          SpiPpi,
                          SPI_OPCODE_READ_INDEX,  // OpcodeIndex
                          0,                      // PrefixOpcodeIndex
                          TRUE,                   // DataCycle
                          TRUE,                   // Atomic
                          FALSE,                  // ShiftOut
                          PDR_REGION_START_OFFSET,// Address, offset from descriptor region
                          (UINT32) Size,          // Data Number
                          (UINT8 *) PlatformInfoHob,
                          EnumSpiRegionPlatformData
                          );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((PlatformInfoHob->BoardId >= TypePlatformMin) && (PlatformInfoHob->BoardId <= TypePlatformMax)) {
    //
    // Valid Platform Identified
    //
    DEBUG ((EFI_D_ERROR, "Platform Info from PDR: Type = %x\n",PlatformInfoHob->BoardId));
  } else {
    //
    // Reading PIT from SPI PDR Failed or a unknown platform identified
    //
    DEBUG ((EFI_D_ERROR, "PIT from SPI PDR reports Platform ID as %x. This is unknown ID. Assuming Greencity Platform!\n", PlatformInfoHob->BoardId));
    PlatformInfoHob->BoardId = TypePlatformUnknown;
    Status = EFI_INCOMPATIBLE_VERSION;
  }
  return Status;*/
  PlatformInfoHob->BoardId = TypeMayanCityCRB;	
  return EFI_SUCCESS;
 //AptioVserver override end: PDR region is not supported.So commenting code routine definition and returning EFI_UNSUPPORTED
}

EFI_STATUS
GPIOGetPlatformInfo ( 
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  )
{
  UINT8                   FDSS;
  UINT16                  FDSI;
  UINT32                  FDOD;
  UINT32                  FDOC;
  UINT32                  BoardId;
  BOARD_ID                Board_ID;
  UINT8                   RevId;
  EFI_PEI_PCI_CFG2_PPI    *PciCfgPpi;

  PciCfgPpi = (**PeiServices).PciCfg;
  ASSERT (PciCfgPpi != NULL);


/*
typedef union BOARD_ID
{
   struct{             
      UINT8  BoardID0            :1;
      UINT8  BoardID1            :1;
      UINT8  BoardID2            :1;
      UINT8  BoardID3            :1;
      UINT8  BoardID4            :1;
      UINT8  BoardRev0           :1;
      UINT8  BoardRev1           :1;
      UINT8  Rsvd                :1;
   }BoardID;
}BOARD_ID;
*/

  Board_ID.BoardID.BoardID0 = 0;
  Board_ID.BoardID.BoardID1 = 0;
  Board_ID.BoardID.BoardID2 = 0;
  Board_ID.BoardID.BoardID3 = 0;
  Board_ID.BoardID.BoardID4 = 0;
  Board_ID.BoardID.BoardRev0 = 0;
  Board_ID.BoardID.BoardRev1 = 0;

#ifdef PCH_SERVER_BIOS_FLAG
  if ((IS_PCH_WBG_LPC_DEVICE_ID(PlatformInfoHob->PchSku)) 
        && (V_PCH_WBG_LPC_RID_0 == PlatformInfoHob->PchRevision)) {
    DEBUG ((EFI_D_ERROR, "Skipping Platform Identification through GSX - PDR identification will be used\n"));  
    return EFI_UNSUPPORTED;
  }

#else
    //
    // For LPT GSX feature is not working properly so no detect the type of board through this method
    //
  if ( IS_PCH_LPTH_DEVICE_ID (PlatformInfoHob->PchSku)) {

    DEBUG ((EFI_D_ERROR, "LPT-H Found - Skipping Platform Identification through GSX - PDR identification will be used\n")); 
    if (PlatformInfoHob->PchRevision == V_PCH_LPT_LPC_RID_1) {
      PlatformInfoHob->PchRevision = V_PCH_LPT_LPC_RID_2;
      DEBUG ((EFI_D_ERROR, "Overriding LPT-H revision from 1 to 2\n"));
    }
    return EFI_UNSUPPORTED;
 }
#endif //PCH_SERVER_BIOS_FLAG

  //
  // Read PCHSTRAP #9 
  //
  FDSS = 0x4;
  FDSI = 0x9;
  FDOC = (FDSS<<12) | (FDSI<<2);

  *(volatile UINT32 *)(UINT32)(PCH_RCBA_ADDRESS + R_PCH_RCRB_SPI_BASE + R_SB_SPI_FDOC) =  FDOC;
  FDOD =  *(volatile UINT32 *)(UINT32)(PCH_RCBA_ADDRESS + R_PCH_RCRB_SPI_BASE + R_SB_SPI_FDOD);
    
  //
  // Check if Serial GPIO Expander Enable is set
  //
  
  if (FDOD & BIT17) {
    DEBUG ((EFI_D_ERROR, "GSX enabled, using it for platform identification\n"));  
    //
    // Read GSX Board ID info
    // 
    GetGsxBoardID(&Board_ID, (EFI_PEI_SERVICES**) PeiServices);
/*
  TypeMayanCityCRB = 0x00,
  TypeMayanCitySRP,
  TypeMayanCitySRPDDR3,
  TypeYakimaCityCRB = 0x03, 
  TypeYakimaCitySRP, 
  TypeYakimaCitySTHI,
  TypeMayanCityCRBDDR3, 
  TypeAztecCityCRBSRP = 0x07,
  TypeAztecCitySTHI,
  TypeHedtReversedIdBits,
  TypeIncaCityCRB = 0x0A,
  TypeIncaCitySRP,
  TypeHalfWidthCitySRP = 0x0C, 
  TypeKahunaDVP2SPbg = 0x0F,
  TypeKahunaDVP2S,
  TypeArandasVVP2S = 0x11, 
  TypeArandasVVP4S,
  TypeHedtSKU1CRB = 0x13,
  TypeHedtEv,
  TypeHedtDDR3,
  TypeSaxtonRock = 0x18,
  TypeEchoRock,
  TypeBalanceRock,
  TypeEmeraldPoint
*/

    BoardId    = (UINT8)(Board_ID.BoardID.BoardID0 + (Board_ID.BoardID.BoardID1 << 1) + (Board_ID.BoardID.BoardID2 << 2) + \
                  (Board_ID.BoardID.BoardID3 << 3) + (Board_ID.BoardID.BoardID4 << 4));

    switch (BoardId) {
            case 0x00:
                 PlatformInfoHob->BoardId = TypeMayanCityCRB;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeMayanCityCRB\n"));
                 break;

            case 0x01:
                 PlatformInfoHob->BoardId = TypeMayanCitySRP;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeMayanCitySRP\n"));
                 break;

            case 0x02:
                 PlatformInfoHob->BoardId = TypeMayanCitySRPDDR3;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeMayanCitySRPDDR3\n"));
                 break;

            case 0x03:
                 PlatformInfoHob->BoardId = TypeYakimaCityCRB;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeYakimaCityCRB\n"));
                 break;

            case 0x04:
                 PlatformInfoHob->BoardId = TypeYakimaCitySRP;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeYakimaCitySRP\n"));
                 break;

            case 0x05:
                 PlatformInfoHob->BoardId = TypeYakimaCitySTHI;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeYakimaCitySTHI\n"));
                 break;

            case 0x06:
                 PlatformInfoHob->BoardId = TypeMayanCityCRBDDR3;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeMayanCityCRBDDR3\n"));
                 break;

            case 0x07:
                 PlatformInfoHob->BoardId = TypeAztecCityCRBSRP;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeAztecCityCRBSRP\n"));
                 break;

            case 0x08:
                  RevId = Board_ID.BoardID.BoardRev0;
                  RevId |= ((Board_ID.BoardID.BoardRev1) << 1);

                  switch (RevId) {

                    case 3:
                      PlatformInfoHob->BoardId = TypeHedtSKU1CRB;
                      PlatformInfoHob-> TypeRevisionId = HEDT_FAB2;
                      DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtSKU1CRBFab2\n"));
                      break;

                    case 2:
                      PlatformInfoHob->BoardId = TypeHedtSKU1CRB;
                      PlatformInfoHob-> TypeRevisionId = HEDT_FAB1;
                      DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtSKU1CRB\n"));
                      break;

                    case 1:
                      PlatformInfoHob->BoardId = TypeHedtEv;
                      DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtEv\n"));
                      break;

                    default:
                      PlatformInfoHob->BoardId = TypeAztecCitySTHI;
                      DEBUG ((EFI_D_ERROR, "Board ID = TypeAztecCitySTHI\n"));
                      break;
                  }

                  break;

            case 0x09:
                  RevId = Board_ID.BoardID.BoardRev0;
                  RevId |= ((Board_ID.BoardID.BoardRev1) << 1);

                  switch (RevId) {

                    case 3:
                      PlatformInfoHob->BoardId = TypeHedtSKU1CRB;
                      PlatformInfoHob-> TypeRevisionId = HEDT_FAB2;
                      DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtSKU1CRBFab2\n"));
                      break;

                    case 2:
                      PlatformInfoHob->BoardId = TypeHedtSKU1CRB;
                      PlatformInfoHob-> TypeRevisionId = HEDT_FAB1;
                      DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtSKU1CRB\n"));
                      break;

                    case 1:
                      PlatformInfoHob->BoardId = TypeHedtEv;
                      DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtEv\n"));
                      break;

                    default:
                      PlatformInfoHob->BoardId = TypePlatformDefault;
                      DEBUG ((EFI_D_ERROR, "Board ID = %2X Default set to TypePlatformDefault\n",BoardId));
                      break;
                  }

                  break;

            case 0x0A:
                 PlatformInfoHob->BoardId = TypeIncaCityCRB;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeIncaCityCRB\n"));
                 break;

            case 0x0B:
                 PlatformInfoHob->BoardId = TypeIncaCitySRP;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeIncaCitySRP\n"));
                 break;

            case 0x0C:
                 PlatformInfoHob->BoardId = TypeHalfWidthCitySRP;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeHalfWidthCitySRP\n"));
                 break;


            case 0x13:
                 PlatformInfoHob->BoardId = TypeHedtSKU1CRB;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtSKU1CRB\n"));
                 break;

            case 0x14:
                 PlatformInfoHob->BoardId = TypeHedtEv;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtEv\n"));
                 break;

            case 0x15:
                 PlatformInfoHob->BoardId = TypeHedtDDR3;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeHedtDDR3\n"));
                 break;

            case 0x18:
                 PlatformInfoHob->BoardId = TypeSaxtonRock;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeSaxtonRock\n"));
                 break;

            case 0x19:
                 PlatformInfoHob->BoardId = TypeEchoRock;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeEchoRock\n"));
                 break;

            case 0x1A:
                 PlatformInfoHob->BoardId = TypeBalanceRock;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeBalanceRock\n"));
                 break;

            case 0x1B:
                 PlatformInfoHob->BoardId = TypeEmeraldPoint;
                 DEBUG ((EFI_D_ERROR, "Board ID = TypeEmeraldPoint\n"));
                 break;

            default:
                 PlatformInfoHob->BoardId = TypePlatformDefault;
                 DEBUG ((EFI_D_ERROR, "Board ID = %2X Default set to TypePlatformDefault\n",BoardId));
                 break;               
    }
        
  } else {
    // Do not force flipping Strap9 bit17 and enable GSX,
    // use PDR instead for board detection
    DEBUG ((EFI_D_ERROR, "GSX is disabled. Detect Board using PDR.\n"));
    return EFI_UNSUPPORTED;
  }  //end if FDOD BIT 17

  return EFI_SUCCESS;
}

VOID
GetPchLanSupportInfo(
  IN EFI_PLATFORM_INFO   *PlatformInfoHob
  )
/*++

Routine Description:
  This function initializes the board related flag to indicates if
  PCH and Lan-On-Motherboard (LOM) devices is supported. 

Arguments:

Returns:

--*/
{
  switch (PlatformInfoHob->BoardId) {    
    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:

    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
      PlatformInfoHob->PchData.PchLanSupported  = 1;
      break; 

    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCityCRBDDR3:
    case TypeMayanCitySRPDDR3:
    case TypeWildcatPass:
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
    case TypeHalfWidthCitySRP:
    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP: 
    case TypeYakimaCitySTHI:    


    default:
      PlatformInfoHob->PchData.PchLanSupported  = 0;
      break;
  }
}

EFI_STATUS
GetPlatformSetupPolicy(
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO         *PlatformInfoHob
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  SYSTEM_CONFIGURATION                  TotalSystemConfiguration;
  SYSTEM_CONFIGURATION                  *TotalSystemConfigurationPtr = &TotalSystemConfiguration;
  SETUP_DATA  						SetupData; //AptioV server override: Setting  value of Pci64BitResourceAllocation based on Above4gDecode setup control from PCI bus driver
    
  SetMem(&TotalSystemConfiguration, sizeof(SYSTEM_CONFIGURATION), 0);

  //
  // Locate Variable PPI
  //
  Status = (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );  
  
  if (EFI_ERROR (Status))
  {
        DEBUG((EFI_D_ERROR, "LocatePpi Error in PlatformInfo.c !\n"));
  }
  else
  {
    VariableSize = sizeof (SYSTEM_CONFIGURATION);

  // AptioV server override: Changed variable name to Intelsetup
    Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"IntelSetup",
                          &gEfiSetupVariableGuid,
                          NULL,
                          &VariableSize,
                          TotalSystemConfigurationPtr
                          );
  }
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "First Boot after flash BIOS!\n" \
                        "Use the default DMI parameters..."));
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
    DEBUG((EFI_D_ERROR, "enable all DMI VCx\n")); // to enable UMA on first boot
    PlatformInfoHob->DmiVc1 = 1;
    PlatformInfoHob->DmiVcp = 1;
    PlatformInfoHob->DmiVcm = 1;
#else
    DEBUG((EFI_D_ERROR, "disable all DMI VCx\n"));
    PlatformInfoHob->DmiVc1 = 0;
    PlatformInfoHob->DmiVcp = 0;
    PlatformInfoHob->DmiVcm = 0;
#endif // ME_SUPPORT_FLAG
    PlatformInfoHob->PciData.Pci64BitResourceAllocation = 1;
  } else {
    //
    // Assign PlatformInfoHob with DmiVcm, DmiVc1 and DmiVcp read from setup options
    //
    PlatformInfoHob->DmiVcm    = TotalSystemConfigurationPtr->DmiVcm;
    PlatformInfoHob->DmiVc1    = TotalSystemConfigurationPtr->DmiVc1;
    PlatformInfoHob->DmiVcp    = TotalSystemConfigurationPtr->DmiVcp;
    
    PlatformInfoHob->PciData.Pci64BitResourceAllocation = TotalSystemConfigurationPtr->Pci64BitResourceAllocation;
  }
  //AptioV server override start: Setting  value of Pci64BitResourceAllocation based on Above4gDecode setup control from PCI bus driver
    VariableSize = sizeof (SETUP_DATA);
    Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"Setup",  
                          &gSetupGuid, 
                          NULL,
                          &VariableSize,
                          &SetupData
                          );
    if (EFI_ERROR (Status)) {
        PlatformInfoHob->PciData.Pci64BitResourceAllocation = 0;
    } else {
#if PCI_SETUP_USE_APTIO_4_STYLE
        PlatformInfoHob->PciData.Pci64BitResourceAllocation = SetupData.Above4gDecode;
#else
        PlatformInfoHob->PciData.Pci64BitResourceAllocation = 1; //!SetupData.Decode4gDisable;        
#endif       
    }
    //AptioV server override end: Setting  value of Pci64BitResourceAllocation based on Above4gDecode setup control from PCI bus driver
    
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
PlatformInfoInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
/*++
Routine Description:

  Platform Type detection. Because the PEI globle variable 
  is in the flash, it could not change directly.So use 
  2 PPIs to distinguish the platform type.

Arguments:

  FfsHeader    -  Pointer to Firmware File System file header.
  PeiServices  -  General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS  -  Memory initialization completed successfully.
  Others       -  All other error conditions encountered result in an ASSERT.

--*/
{
  EFI_STATUS              Status;
  EFI_PEI_PCI_CFG2_PPI    *PciCfgPpi;
  EFI_PLATFORM_INFO       PlatformInfoHob;
  EFI_PLATFORM_INFO       tempPlatformInfoHob;
  UINT8                   ChipId;
  UINT32                  Delay;
  UINT8                   CpuType;
  UINT8                   CpuStepping;  
  UINT32                  Data32;
  UINT16                  Data16;
  UINT8                   Data8;

  PciCfgPpi = (**PeiServices).PciCfg;
  ASSERT (PciCfgPpi != NULL);

  (*PeiServices)->SetMem (
                    &PlatformInfoHob,
                    sizeof (PlatformInfoHob),
                    0
                    );

  //
  // --------------------------------------------------
  //
  // Detect the iBMC SIO for CV/CRB Platforms
  // 0x4E/0x4F decoding has been enabled in MonoStatusCode PEIM.
  // 
  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_UNLOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_CHIP_ID_REG);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  ChipId = IoRead8  (PILOTIII_SIO_DATA_PORT);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_LOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);

  GetPlatformSetupPolicy(PeiServices, &PlatformInfoHob);
  //
  // Update PCH Type
  //
  //
  // Device ID
  //
  PciCfgPpi->Read (
              PeiServices,
              PciCfgPpi,
              EfiPeiPciCfgWidthUint16,
              PEI_PCI_CFG_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, PCI_DEVICE_ID_OFFSET),
              &Data16
              );
  PlatformInfoHob.PchSku= Data16;
#ifdef PCH_SERVER_BIOS_FLAG  
  if (PlatformInfoHob.PchSku == V_PCH_WBG_LPC_DEVICE_ID_2){
      PlatformInfoHob.PchType = WBG_SKU_G; // WBG -G (Grantley) SKU
    } else if (PlatformInfoHob.PchSku == V_PCH_WBG_LPC_DEVICE_ID_5){
      PlatformInfoHob.PchType = WBG_SKU_A; // WBG -A (Grantley) SKU
    } else if (PlatformInfoHob.PchSku == V_PCH_WBG_LPC_DEVICE_ID_4){
      PlatformInfoHob.PchType = WBG_SKU_X; // WBG -X (HEDT Hight End Desktop)SKU
    }else{
      PlatformInfoHob.PchType = 0; // SuperSKU
    }
#else
  switch (PlatformInfoHob.PchSku) {
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_0:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_1:
      PlatformInfoHob.PchType = 1; // LPT Z series 
      break;
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_2:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_6:
      PlatformInfoHob.PchType = 2; // LPT H series 
     break;
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_3:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_4:
      PlatformInfoHob.PchType = 3; // LPT Q series 
     break;
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_5:
      PlatformInfoHob.PchType = 4; // LPT B series 
     break;
    default:
      PlatformInfoHob.PchType = 0; // LPT SuperSKU
     break;
    }
#endif // PCH_SERVER_BIOS_FLAG

  PciCfgPpi->Read (
              PeiServices,
              PciCfgPpi,
              EfiPeiPciCfgWidthUint8,
              PEI_PCI_CFG_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, PCI_REVISION_ID_OFFSET),
              &Data8
              );
  PlatformInfoHob.PchRevision = Data8;

  //
  // Wildcat Pass has no GSX.
  // GPIOGetPlatformInfo will erroneously return TypeMayanCity.
  // Check the PDR first.
  //
  Status = PdrGetPlatformInfo(PeiServices, &tempPlatformInfoHob);
  if (!EFI_ERROR(Status)) {
    // Remember read BoardId in case GSX failure
    PlatformInfoHob.BoardId = tempPlatformInfoHob.BoardId;
    PlatformInfoHob.TypeRevisionId = tempPlatformInfoHob.TypeRevisionId;
  } else {
    PlatformInfoHob.BoardId = TypePlatformUnknown;
  }
  if (PlatformInfoHob.BoardId != TypeWildcatPass) {
  // use tempPlatformInfoHob to preserve read from PDR BoardId
  Status = GPIOGetPlatformInfo(PeiServices, &tempPlatformInfoHob);
  if (!EFI_ERROR(Status)) {
    // Success use value obtained using GSX
    PlatformInfoHob.BoardId = tempPlatformInfoHob.BoardId;
    PlatformInfoHob.TypeRevisionId = tempPlatformInfoHob.TypeRevisionId;
  }
  }
  
  //
  // Update IIO Type
  //
  PlatformInfoHob.IioRevision = 0;

  //
  // Update PCIe riser type(s)
  //
  Data8 = (ReadGpio(PeiServices, 64) << 1);      // ID3
  Data8 |= ReadGpio(PeiServices, 66);            // ID2
  PlatformInfoHob.PcieRiser2Type = Data8;

  Data8 = (ReadGpio(PeiServices, 65) << 1);      // ID1
  Data8 |= ReadGpio(PeiServices, 12);            // ID0
  PlatformInfoHob.PcieRiser1Type = Data8;

  //
  // If Inca City, check the riser present bits (active low)
  // These GPIOs are only valid on Inca City
  // FM_RISER1_PRSNT_N = GPIO14
  // FM_RISER2_PRSNT_N = GPIO34
  // 
  if ( (PlatformInfoHob.BoardId == TypeIncaCityCRB) ||
    (PlatformInfoHob.BoardId == TypeIncaCitySRP) ) {

    DEBUG ((EFI_D_INFO, "PlatformInfoInit(): Inca City detected, reading riser ID bits \n"));

    if (ReadGpio(PeiServices, 14)) {
      PlatformInfoHob.PcieRiser1Type = 0XFF;
      DEBUG ((EFI_D_INFO, "PlatformInfoInit(): Riser 1 not detected\n"));
    } else {
      DEBUG ((EFI_D_INFO, "PlatformInfoInit(): Riser 1 detected, type %d\n", PlatformInfoHob.PcieRiser1Type));
    }

    if (ReadGpio(PeiServices, 34)) {
      PlatformInfoHob.PcieRiser2Type = 0XFF;
      DEBUG ((EFI_D_INFO, "PlatformInfoInit(): Riser 2 not detected\n"));
    } else {
      DEBUG ((EFI_D_INFO, "PlatformInfoInit(): Riser 2 detected, type %d\n", PlatformInfoHob.PcieRiser2Type));
    }
  }



  //
  // Get Subtractive decode enable bit from descriptor
  //
  if ((MmioRead16 (PCH_RCRB_BASE + R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FDV) == B_PCH_SPI_HSFS_FDV) {
  MmioAndThenOr32(
      PCH_RCRB_BASE + R_PCH_SPI_FDOC,
      (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
         (UINT32) (V_PCH_SPI_FDOC_FDSS_PCHS | R_PCH_SPI_STRP9)
      );
      PlatformInfoHob.PchData.PchPcieSBDE = (UINT8) ((MmioRead32 (PCH_RCRB_BASE + R_PCH_SPI_FDOD) & B_PCH_SPI_STRP9_PCIE_SBDE_EN) >> N_PCH_SPI_STRP9_PCIE_SBDE_EN);
    }

   if (PchIsGbeRegionValid (PCH_RCRB_BASE) == FALSE) {      
          PlatformInfoHob.PchData.GbeRegionInvalid = 1;
	 }
    else {
         PlatformInfoHob.PchData.GbeRegionInvalid = 0;
         }
  GetPchLanSupportInfo(&PlatformInfoHob);
  PlatformInfoHob.PchData.GbePciePortNum = 0xFF;

  PciCfgPpi->Read (
              PeiServices,
              PciCfgPpi,
              EfiPeiPciCfgWidthUint32,
              PEI_PCI_CFG_ADDRESS (0, PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1, R_PCH_PCIE_STRPFUSECFG),
              &Data32
              );
  if ((Data32 & B_PCH_PCIE_STRPFUSECFG_GBE_PCIE_PEN) != 0) {
    PlatformInfoHob.PchData.GbePciePortNum = (UINT8)((Data32 & B_PCH_PCIE_STRPFUSECFG_GBE_PCIEPORTSEL) >> N_PCH_PCIE_STRPFUSECFG_GBE_PCIEPORTSEL);
  }

  PlatformInfoHob.PchData.PchPciePortCfg1 = (UINT8)((Data32 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC_4);

  PciCfgPpi->Read (
              PeiServices,
              PciCfgPpi,
              EfiPeiPciCfgWidthUint32,
              PEI_PCI_CFG_ADDRESS (0, PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5, R_PCH_PCIE_STRPFUSECFG),
              &Data32
              );
  PlatformInfoHob.PchData.PchPciePortCfg2 = (UINT8)((Data32 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC_4);
  PlatformInfoHob.SysData.SysSioExist = (UINT8)IsSioExist();

  GetCpuInfo (&CpuType, &CpuStepping);
  PlatformInfoHob.CpuType     = CpuType;
  PlatformInfoHob.CpuStepping = CpuStepping;

  //
  // Build HOB for setup memory information
  //

  BuildGuidDataHob (
      &gEfiPlatformInfoGuid,
      &(PlatformInfoHob),
      sizeof (EFI_PLATFORM_INFO)
      );

  Status = (**PeiServices).InstallPpi (PeiServices, &mPlatformInfoPpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Save PlatformInfoHob.BoardId in CMOS
  //
  IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_LO);
  IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)PlatformInfoHob.BoardId);

  IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_HI);
  IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)((PlatformInfoHob.PcieRiser2Type << 4) + (PlatformInfoHob.PcieRiser1Type)));

  return EFI_SUCCESS;
}
