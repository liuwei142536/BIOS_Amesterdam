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
#include <Token.h> // AptioV server override
#include "PlatformInfo.h"
#include "Library\PchPlatformLib.h"
#include "Platform.h"
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
  UINT32                    RegEax =0;
  UINT32 					FamilyId =0;
  UINT32 					ExtendedFamilyId =0;
  UINT32 					ModelId =0;
  UINT32 					ExtendedModelId = 0;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  *CpuStepping = (UINT8)(RegEax & 0x0F);

  
  	  //
	  // The Extended Family ID needs to be examined only when the Family ID is 0FH.
	  // If Family ID is 0FH, Family ID = Family ID + Extended Family ID.
	  //
	  FamilyId = BitFieldRead32 (RegEax, 8, 11);
	  if (FamilyId == 0x0f) {
		  ExtendedFamilyId = BitFieldRead32 (RegEax, 20, 27);
		  //FamilyId += ExtendedFamilyId;
	  }
	  
	  //
	  // The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
	  // If Family ID is 06H or 0FH, Model ID = Model ID + (Extended Model ID << 4).
	  //BDX-model if is 0x56
	  //
	  ModelId = BitFieldRead32 (RegEax, 4, 7);
	  if (FamilyId == 0x06 || FamilyId == 0x0f) {
		  ExtendedModelId = BitFieldRead32 (RegEax, 16, 19);
		  ModelId += (ExtendedModelId << 4);
	  }
	  DEBUG ( (EFI_D_ERROR, "	   :: ModelId Socket ModelId# %X \n", ModelId));
	  FamilyId += ExtendedFamilyId;
	  DEBUG ( (EFI_D_ERROR, "	   :: FamilyId Socket CPU Stepping# %X \n", *CpuStepping));
      *CpuType     = (UINT8)ModelId;
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
  UINT8                   FlashID[3];
  UINT8                   Index;   
  UINT8					  SpiStatus;
  UINT8					  FlashIndex;
 
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

  Status = SpiPpi->ReadId (SpiPpi, 0, FlashID);
  
  if (!EFI_ERROR (Status)) {
    SpiStatus = 0; //Flag to indicate if SPI valid was found 
    for (Index = 0; Index < EnumSpiFlashMax, SpiStatus == 0; Index++){
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
  }

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

  if ((PlatformInfoHob->BoardId >= TypePlatformMin) && (PlatformInfoHob->BoardId <= TypePlatformMax)) {
    //
    // Valid Platform Identified
    //
    DEBUG ((EFI_D_ERROR, "Platform Info from PDR: Type = %x\n",PlatformInfoHob->BoardId));
  } else {
    //
    // Reading PIT from SPI PDR Failed or a unknown platform identified
    //
    DEBUG ((EFI_D_ERROR, "PIT from SPI PDR reports Platform ID as 0x%x. This is unknown ID. Assuming default Platform 0x%x!\n", PlatformInfoHob->BoardId, TypePlatformDefault));
    PlatformInfoHob->BoardId = TypePlatformDefault;
  }
  return Status;*/
  return EFI_UNSUPPORTED;
 //AptioVserver override end: PDR region is not supported.So commenting code routine definition and returning EFI_UNSUPPORTED
}

EFI_STATUS
GetGnvBoardPartNum (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT CHAR8                   *BoardPartNumStr
  )
{
  EFI_STATUS                Status;
  EFI_PEI_SMBUS2_PPI        *SmbusPpi;
  EFI_SMBUS_DEVICE_COMMAND  SmbusCommand;
  EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress;
  UINT8                     SmbusData[FRU_BOARD_PART_NUM_MAX_LENGTH] = {0x00}; 
  UINTN                     SmbusLength;    
  UINT8                     RetryCount;
  UINTN                     Offset = 0;

  if (BoardPartNumStr == NULL) {
    return EFI_DEVICE_ERROR;
    }
  else{
    AsciiStrCpy(BoardPartNumStr, "");
    }
  //
  // Locate SmBus Ppi
  //
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gEfiPeiSmbus2PpiGuid,
                            0,
                            NULL,
                            &SmbusPpi
                            );
  ASSERT_EFI_ERROR (Status);
  //
  // FRU check sum check has been not added as other error conditions have already taken care of FRU data corruption.
  //
  SlaveAddress.SmbusDeviceAddress = (PLATFORM_FRU_SMB_ADDRESS >> 1);

  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) 
  {
    Offset = FRU_BOARD_AREA_START_OFFSET;

    SmbusLength                     = 1;
    SmbusCommand                    = Offset;
    
    Status = SmbusPpi->Execute( SmbusPpi,
                            SlaveAddress,
                            SmbusCommand,
                            EfiSmbusReadByte,
                            FALSE,
                            &SmbusLength,
                            &SmbusData );

    if (EFI_ERROR(Status)) {
      if (RetryCount == NUM_OF_RETRIES - 1){ 
        DEBUG((EFI_D_ERROR, "Not Locating EEPROM to read fru header data\n"));
        return EFI_DEVICE_ERROR;
        }
       continue;
      }
    else {
      //
      //Board Area Start Data in 8 byte multiples   
      //
      Offset = SmbusData[0] * 8;
      break;
      }   
  } 

  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) 
  {
    //
    //Board Area Start Data in 8 byte multiples   
    //
    Offset = Offset + FRU_BOARD_MFG_OFFSET; //Reach to Board MFG Data Type/Length 
    
    SmbusLength                     = 1;
    SmbusCommand                    = Offset;
    
    Status = SmbusPpi->Execute( SmbusPpi,
                            SlaveAddress,
                            SmbusCommand,
                            EfiSmbusReadByte,
                            FALSE,
                            &SmbusLength,
                            &SmbusData );

    if (EFI_ERROR(Status)) {
      if (RetryCount == NUM_OF_RETRIES - 1){ 
        DEBUG((EFI_D_ERROR, "Not Locating EEPROM to read Board Mfg Type/Length Byte\n"));
        return EFI_DEVICE_ERROR;
        }
       continue;
      }
    else {
      //
      // Mask the Input Port register data [4:0] to get Data Type/Length
      //
      SmbusData[0] &= FRU_BOARD_AREA_LENGTH_MASK;  
      Offset = Offset + SmbusData[0] + 1; //Reach to Board Product Name Data Type/Length    
      break;
      }   
  }    
  
  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) 
  {

    SmbusLength                     = 1;
    SmbusCommand                    = Offset;
    
    Status = SmbusPpi->Execute( SmbusPpi,
                            SlaveAddress,
                            SmbusCommand,
                            EfiSmbusReadByte,
                            FALSE,
                            &SmbusLength,
                            &SmbusData );

    if (EFI_ERROR(Status)) {
      if (RetryCount == NUM_OF_RETRIES - 1){ 
        DEBUG((EFI_D_ERROR, "Not Locating EEPROM to read Board Product Name Type/Length Byte\n"));
        return EFI_DEVICE_ERROR;
        }
       continue;
      }
    else {
      //
      // Mask the Input Port register data [4:0] to get Data Type/Length
      //
      SmbusData[0] &= FRU_BOARD_AREA_LENGTH_MASK; 
      Offset = Offset + SmbusData[0] + 1; //Reach to Board Serial num Data Type/Length
      break;
      }  
  } 

  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) 
  {
    SmbusLength                     = 1;
    SmbusCommand                    = Offset;
    
    Status = SmbusPpi->Execute( SmbusPpi,
                            SlaveAddress,
                            SmbusCommand,
                            EfiSmbusReadByte,
                            FALSE,
                            &SmbusLength,
                            &SmbusData );
    if (EFI_ERROR(Status)) {
      if (RetryCount == NUM_OF_RETRIES - 1){ 
        DEBUG((EFI_D_ERROR, "Not Locating EEPROM to read Board Serial Number Type/Length Byte\n"));
        return EFI_DEVICE_ERROR;
        }
       continue;
      }
    else {
      //
      // Mask the Input Port register data [4:0] to get Data Type/Length
      //
      SmbusData[0] &= FRU_BOARD_AREA_LENGTH_MASK;
      Offset = Offset + SmbusData[0] + 1; //Reach to Board Part num Data Type/Length
      break;
      }
  }


  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) 
  {

    SmbusLength                     = 1;
    SmbusCommand                    = Offset;
    
    Status = SmbusPpi->Execute( SmbusPpi,
                            SlaveAddress,
                            SmbusCommand,
                            EfiSmbusReadByte,
                            FALSE,
                            &SmbusLength,
                            &SmbusData );

    if (EFI_ERROR(Status)) {
      if (RetryCount == NUM_OF_RETRIES - 1){ 
        DEBUG((EFI_D_ERROR, "Not Locating EEPROM to read Board Part Number Type/Length Byte\n"));
        return EFI_DEVICE_ERROR;
        }
       continue;
      }
    else {
      //
      // Mask the Input Port register data [4:0] to get Data Type/Length
      //
      SmbusData[0] &= FRU_BOARD_AREA_LENGTH_MASK; 
      SmbusLength = SmbusData[0] ; //Part num Data Length 
      break;
      }
  }  
  
  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++)   
  {
    Offset = Offset + 1;  //Reach to Board Part num Data read offset
    
    SmbusCommand                    = Offset;   
    
    Status = SmbusPpi->Execute( SmbusPpi,
                            SlaveAddress,
                            SmbusCommand,
                            EfiSmbusReadByte,
                            FALSE,
                            &SmbusLength,
                            &SmbusData);

  if (EFI_ERROR(Status)) {
    if (RetryCount == NUM_OF_RETRIES - 1){ 
       DEBUG((EFI_D_ERROR, "Not Locating Epprom to read Board Part Number data!\n"));
       return EFI_DEVICE_ERROR;
       }
      continue;
      }
   else {     
      SmbusData[SmbusLength] = '\0';
      AsciiStrnCpy(BoardPartNumStr, SmbusData, SmbusLength);
      break;      
   }
  }   
  
  return EFI_SUCCESS;
}

EFI_STATUS
GetPlatformInfo ( 
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  )
{

  PlatformInfoHob->BoardId = TypeEchoCanyon;
  DEBUG ((EFI_D_ERROR, "Using FRU EEPROM, Board ID = TypeEchoCanyon CRB\n"));
  return EFI_SUCCESS;
}

#ifdef DE_SKU

#define CPU_BDX_DE_SERVER       0x56  //AS CPUID-0x5066x
#define C0_REV_BDX              0x02  //V0


VOID
UpdateSCUartStatus(
  IN EFI_PLATFORM_INFO   *PlatformInfoHob
  )
/*++

Routine Description:
  This function updates the SC UART  status based on the platform type

Arguments:

Returns:

--*/
{
	UINT8	  mCpuType;
	UINT8	  mCpuStepping;

  switch (PlatformInfoHob->BoardId) {    


    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
    case TypeTrabucoCanyon:
    case TypeCamelbackMountain:
	case TypeEchoCanyon:
    case TypeTlaquepaque:
    case TypePiestewaPeak:
      PlatformInfoHob->DisableSCUart  = 0;
      break;

    default:
      PlatformInfoHob->DisableSCUart  = 0;   // Default use SC uart
      break;
  }

  
	GetCpuInfo(&mCpuType,&mCpuStepping);
  
  //if( ( mCpuType == CPU_BDX_DE_SERVER) && (mCpuStepping == C0_REV_BDX)){
//	PcdSet64(PcdUartDefaultBaudRate,38400);
//	}else{
//	PcdSet64(PcdUartDefaultBaudRate,115200);
 // 	}

  
}
#endif // DE_SKU

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

	
	case TypeBeverlyCoveCRB:
	case TypeBeverlyCoveSODimmCRB:
    case TypeTrabucoCanyon:
    case TypeCamelbackMountain:
	case TypeEchoCanyon:
    case TypeTlaquepaque:
    case TypePiestewaPeak:  
      PlatformInfoHob->PchData.PchLanSupported  = 1;
	break;
    default:
      PlatformInfoHob->PchData.PchLanSupported  = 1;
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
  UINTN                                 NvMapSize;
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
    NvMapSize    = sizeof (SERVER_MGMT_CONFIGURATION_DATA);

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
    PlatformInfoHob->IsocEn = 1;
    PlatformInfoHob->MeSegEn = 1; 
    PlatformInfoHob->DmiVc1 = 1;
    PlatformInfoHob->DmiVcp = 1;
    PlatformInfoHob->DmiVcm = 1;
#else
    DEBUG((EFI_D_ERROR, "disable all DMI VCx\n"));
    PlatformInfoHob->IsocEn = 0;
    PlatformInfoHob->MeSegEn = 0; 
    PlatformInfoHob->DmiVc1 = 0;
    PlatformInfoHob->DmiVcp = 0;
    PlatformInfoHob->DmiVcm = 0;
#endif // ME_SUPPORT_FLAG
    PlatformInfoHob->PciData.Pci64BitResourceAllocation = 1;
#ifdef  DE_SKU
    PlatformInfoHob->Disable_SC_Cb3Dma = 0;
    PlatformInfoHob->Disable_SC_GbE = 0;
    PlatformInfoHob->SC_GbE_PF0 = 2;
    PlatformInfoHob->SC_GbE_PF1 = 2;
    
    PlatformInfoHob->Disable_SC_CPM = 0;
    PlatformInfoHob->Disable_SC_GbE1 = 0;
    PlatformInfoHob->SC_GbE1_PF0 = 2;
    PlatformInfoHob->SC_GbE1_PF1 = 2;
#endif  //DE_SKU
  } else {
    PlatformInfoHob->IsocEn    = TotalSystemConfigurationPtr->IsocEn;       // ISOC enabled
    PlatformInfoHob->MeSegEn   = TotalSystemConfigurationPtr->MeSegEn;      // ME Seg mode enabled. 
    //
    // 4986450 - hexaii_hsx_mock:C0:  Virtual channels not disabled when Isoc disabled via secondary means
    // Need to set IsocEn=0 on the following conditions: 
    //    * num sockets > 2
    //    * num sockets = 2 and num qpi links = 1
    //    * processor type = EN
    //    * COD enabled
    // Handling here COD enable. 
    if (TotalSystemConfigurationPtr->ClusterOnDieEn)  {
      PlatformInfoHob->IsocEn    = FALSE;
    }

  //
  // 4986285: ESCALATE from hexaii_hsx_mock:C0 BIOS/Isoc issues with 18d15
  // Get the current DMiVcx values based on Isoc/MeSeg knobs
  //
    if ((TotalSystemConfigurationPtr->IsocEn == 0)  && (TotalSystemConfigurationPtr->MeSegEn == 0)) {
        PlatformInfoHob->DmiVcm    = 0; // Vcm Must be disable
    } else {
        PlatformInfoHob->DmiVcm    = TotalSystemConfigurationPtr->DmiVcm; // Read value from setup option
    }

    if (TotalSystemConfigurationPtr->IsocEn == 0) {
        PlatformInfoHob->DmiVc1 = 0; // Vc1 must be disabled
        PlatformInfoHob->DmiVcp = 0; // Vcp must be disabled
    } else {
      PlatformInfoHob->DmiVc1    = TotalSystemConfigurationPtr->DmiVc1;  // Read value from setup option
      PlatformInfoHob->DmiVcp    = TotalSystemConfigurationPtr->DmiVcp;  // Read value from setup option
    }
    PlatformInfoHob->PciData.Pci64BitResourceAllocation = TotalSystemConfigurationPtr->Pci64BitResourceAllocation;
#ifdef  DE_SKU
    PlatformInfoHob->Disable_SC_Cb3Dma = TotalSystemConfigurationPtr->Disable_SC_Cb3Dma;
    PlatformInfoHob->Disable_SC_GbE = TotalSystemConfigurationPtr->Disable_SC_GbE;
    PlatformInfoHob->SC_GbE_PF0 = TotalSystemConfigurationPtr->SC_GbE_PF0;
    PlatformInfoHob->SC_GbE_PF1 = TotalSystemConfigurationPtr->SC_GbE_PF1;
    
    PlatformInfoHob->Disable_SC_CPM = TotalSystemConfigurationPtr->Disable_SC_CPM;
    PlatformInfoHob->Disable_SC_GbE1 = TotalSystemConfigurationPtr->Disable_SC_GbE1;
    PlatformInfoHob->SC_GbE1_PF0 = TotalSystemConfigurationPtr->SC_GbE1_PF0;
    PlatformInfoHob->SC_GbE1_PF1 = TotalSystemConfigurationPtr->SC_GbE1_PF1;
#endif  //DE_SKU
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
  }
    PlatformInfoHob->DisableSCUart = 0;

  return EFI_SUCCESS;
}

VOID
GetSocketSkuType (
  UINT8            *SocketSkuType
  )
/*++

Routine Description:
   Returns Socket SKU Type for DE/NS.

Arguments:
   SocketSkuType     - SKU type in given socket 

Returns:

 --*/
{
  UINT32  Data32;
  UINT8   UncoreBusNo;
  CPUBUSNO_IIO_VTD_STRUCT          CpuBusNo;

  CpuBusNo.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (IIO_BUS_NUM,
                                                      IIO_VTD_DEV,
                                                      IIO_VTD_FUN,
                                                      ONLY_REGISTER_OFFSET(CPUBUSNO_IIO_VTD_REG)));
  UncoreBusNo = (UINT8)CpuBusNo.Bits.bus1;
  
//  Data32 = QpiReadPciCfg (host, SocId, 0, CAPID0_PCU_FUN3_REG);
  Data32 = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                               PCU22_DEV_NUM,
                                               PCU22_FUNC_NUM,
                                               ONLY_REGISTER_OFFSET(CAPID0_PCU_FUN3_REG)));
  *SocketSkuType = DefaultSkuID;
  
  if (Data32 & (1 << SKU_DE)) {
    // HSD 4987295: CLONE from broadwell_server: PO fuses creating wrong SKU type for BDX-DE CAPID0
    // Workaround to address additional bits in CAPID0[4:0] being set in addition to CAPID0[5] for DE-SKU
    DEBUG ((EFI_D_ERROR, "\n CAPID0[5] is set. SKU Detected as DE."));
    *SocketSkuType = DESkuID;
	if (Data32 & (1 << SKU_NS)) {
      *SocketSkuType = SKU_NS;
      // identify the specified sku of NS
      //Data32 = QpiReadPciCfg (host, SocId, 0, CAPID1_PCU_FUN3_REG);
      Data32 = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                   PCU22_DEV_NUM,
                                                   PCU22_FUNC_NUM,
                                                   ONLY_REGISTER_OFFSET(CAPID1_PCU_FUN3_REG)));      
      if (Data32 & (UINT32)NS_STORAGE_SEG_SEL) {
        DEBUG ((EFI_D_ERROR, "\n NS PO MSG: GetSocketSkuType()-> NSStorageSkuID\n"));
        *SocketSkuType = NSStorageSkuID;
      } else {
        DEBUG ((EFI_D_ERROR, "\n NS PO MSG: GetSocketSkuType()-> NSCommsSkuID\n"));
        *SocketSkuType = NSCommsSkuID;    
      }
	}
  } 

  DEBUG ((EFI_D_ERROR, "\n NS PO MSG: GetSocketSkuType()-> SocketSkuType = %x\n", *SocketSkuType));
  return ;
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
#ifdef DE_SKU  
  UINT8                   SocketSkuType;
#endif

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

  Status = GetPlatformInfo(PeiServices, &PlatformInfoHob);
    
  if(EFI_ERROR(Status)) {
    Status = PdrGetPlatformInfo(PeiServices, &tempPlatformInfoHob);
    PlatformInfoHob.BoardId = tempPlatformInfoHob.BoardId;
    if (EFI_ERROR(Status)) {
		  PlatformInfoHob.BoardId = TypePlatformDefault;
    }
  }  

#ifdef DE_SKU
  //
  // Update SC UART status based on PlatformType or setup option
  //
  UpdateSCUartStatus(&PlatformInfoHob);
#endif // DE_SKU
  //
  // Update IIO Type
  //
  PlatformInfoHob.IioRevision = 0;

  //
  // Update PCIe riser type(s)
  //
  //  Data8 = (ReadGpio(PeiServices, 64) << 1);      // ID3
  //  Data8 |= ReadGpio(PeiServices, 66);            // ID2
  //  PlatformInfoHob.PcieRiser2Type = Data8;

  //  Data8 = (ReadGpio(PeiServices, 65) << 1);      // ID1
  //  Data8 |= ReadGpio(PeiServices, 12);            // ID0
  //  PlatformInfoHob.PcieRiser1Type = Data8;

  //
  // If Inca City, check the riser present bits (active low)
  // These GPIOs are only valid on Inca City
  // FM_RISER1_PRSNT_N = GPIO14
  // FM_RISER2_PRSNT_N = GPIO34
  // 
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

#ifdef DE_SKU  
  GetSocketSkuType (&SocketSkuType);
  PlatformInfoHob.DE_NS_SocketSkuType = SocketSkuType;
  
  if (SocketSkuType == NSCommsSkuID) {
    if (
        (PlatformInfoHob.BoardId != TypeEchoCanyon)) {
      DEBUG ((DEBUG_ERROR, "Error: ASSERT for NS Comms SKU Socket is installed on non-COMMS NS platform! \n"));                 
      ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    }    
  } else if (SocketSkuType == NSStorageSkuID) {
    // to-do  add the misconfig check when board of NS storage SKU is defined
//    if ((PlatformInfoHob->BoardId != TypeDurangoPONS) && (PlatformInfoHob->BoardId != TypeDurangoPPVNS) && 
//        (PlatformInfoHob->BoardId != TypeEchoCanyon)) {
//      DEBUG ((DEBUG_ERROR, "Error: ASSERT for NS Storage SKU Socket is installed on non-Storage NS platform! \n"));                 
//      ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
//    }  
  }
#endif

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
  DEBUG ((EFI_D_ERROR, "Publish PlatformInfoPPI \n"));
  //
  // Save PlatformInfoHob.BoardId in CMOS
  //
  IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_LO);
  IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)PlatformInfoHob.BoardId);

  // IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_HI);
  // IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)((PlatformInfoHob.PcieRiser2Type << 4) + (PlatformInfoHob.PcieRiser1Type)));

  return EFI_SUCCESS;
}
