/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformTypes.c
  
Abstract:

  Platform type driver to identify different platform.  

--*/

//
// Statements that include other files
//
#include <Token.h> // AptioV server override
#include "PlatformTypes.h"
#include <Library/SerialPortLib/Ns16550.h>
#include <Library/IoLib.h>
#include <Protocol/SmbusHc.h>
#include <Protocol/MpService.h>
#include <Guid/SetupVariable.h>
#include "Flash.h" //AptioV server override: To use AMI flash for reading PDR region



CHAR16                              PchName[40];
CHAR16                              IioName[40];
CHAR16                              PlatformName[40];

//
// Instantiation of Driver's private data.
//
EFI_PLATFORM_DATA_DRIVER_PRIVATE    mPrivatePlatformData;
EFI_IIO_UDS_DRIVER_PRIVATE          mIioUdsPrivateData;
IIO_UDS                             *IioUdsData;          // Pointer to UDS in Allocated Memory Pool

EFI_GUID gEfiSmbusHcProtocolGuid = EFI_SMBUS_HC_PROTOCOL_GUID;
EFI_GUID gEfiMpServiceProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;
EFI_GUID mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;


VOID
GetPchName (
  VOID
  )
{

  switch (mPrivatePlatformData.PlatformType.PchSku) {
#ifdef PCH_SERVER_BIOS_FLAG
    case V_PCH_WBG_LPC_DEVICE_ID_0_UNF:
    case V_PCH_WBG_LPC_DEVICE_ID_0:
    case V_PCH_WBG_LPC_DEVICE_ID_1:
    case V_PCH_WBG_LPC_DEVICE_ID_2:
    case V_PCH_WBG_LPC_DEVICE_ID_3:
    case V_PCH_WBG_LPC_DEVICE_ID_5:
      StrCpy (PchName, L"Wellsburg");
      break;
#else 
    case V_PCH_LPTH_LPC_DEVICE_ID_SVR_0:
    case V_PCH_LPTH_LPC_DEVICE_ID_SVR_1:
    case V_PCH_LPTH_LPC_DEVICE_ID_SVR_2:
    case V_PCH_LPTH_LPC_DEVICE_ID_SVR_3:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_SUPER_SKU:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_0:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_1:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_3:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_4:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_5:
    case V_PCH_LPTH_LPC_DEVICE_ID_DT_6:
      StrCpy (PchName, L"Lynxpoint");
      break;
#endif // PCH_SERVER_BIOS_FLAG
  default:
    StrCpy (PchName, L"Unknown");
  }

  //
  // Type
  //
  switch (mPrivatePlatformData.PlatformType.PchType) {
#ifdef PCH_SERVER_BIOS_FLAG  
    case WBG_SKU_G:
      StrCat (PchName, L" -G");
      break;
    case WBG_SKU_X:
      StrCat (PchName, L" -X");
      break;
    case WBG_SKU_A:
      StrCat (PchName, L" -A");
      break;
#endif //PCH_SERVER_BIOS_FLAG
    default:
      StrCat (PchName, L" SuperSku");
      break;
  }
  

  //
  // Revision
  //
  switch (mPrivatePlatformData.PlatformType.PchRevision) {
#ifdef PCH_SERVER_BIOS_FLAG
    case V_PCH_WBG_LPC_RID_0:
        StrCat (PchName, L" - A0 stepping"); //Wellsburg
      break;
    case V_PCH_WBG_LPC_RID_1:
        StrCat (PchName, L" - B0 stepping");
      break;
    case V_PCH_WBG_LPC_RID_2:
       StrCat (PchName, L" - B1 stepping");
      break;

#else  
    case V_PCH_LPT_LPC_RID_0:
      StrCat (PchName, L" - A0 stepping");
      break;
    case V_PCH_LPT_LPC_RID_1:
      StrCat (PchName, L" - A1 stepping");
      break;
    case V_PCH_LPT_LPC_RID_2:
      StrCat (PchName, L" - B0 stepping");
      break;
    case V_PCH_LPT_LPC_RID_3:
      StrCat (PchName, L" - C0 stepping");
      break;
    case V_PCH_LPT_LPC_RID_4:
      StrCat (PchName, L" - C1 stepping");
      break;
    case V_PCH_LPT_LPC_RID_5:
      StrCat (PchName, L" - C2 stepping");
      break;
#endif // PCH_SERVER_BIOS_FLAG
    default:
      StrCat (PchName, L" - xx");
  }

  mPrivatePlatformData.PlatformType.PchStringPtr = (UINT64)PchName;
  return;
}


VOID
GetIioName (
  VOID
  )
{

  StrCpy (IioName, L"Unknown");

  mPrivatePlatformData.PlatformType.IioStringPtr = (UINT64)IioName;
  return ;
}


VOID
SetAllMtrrsValid (
  VOID
)
{
  UINT32  Index;
  UINT64  MtrrValue;
  UINT32  VariableMtrrLimit;

  VariableMtrrLimit = (UINT32) (AsmReadMsr64(EFI_IA32_MTRR_CAP) & B_EFI_IA32_MTRR_VARIABLE_SUPPORT);

  //
  // Check all Mtrrs to see if it is valid
  //
  for (Index = EFI_CACHE_VARIABLE_MTRR_BASE + 1; Index <= (EFI_CACHE_VARIABLE_MTRR_BASE + VariableMtrrLimit*2 - 1); Index+=2) {
    if (AsmReadMsr64 (Index - 1) == 0xFE000000) {
      AsmWriteMsr64 (Index - 1, 0);
      AsmWriteMsr64 (Index    , 0);
    } else {
      MtrrValue = AsmReadMsr64 (Index);
      if (MtrrValue && !(MtrrValue & B_EFI_CACHE_MTRR_VALID)) {
        AsmWriteMsr64 (Index, MtrrValue | B_EFI_CACHE_MTRR_VALID);
      }
    }
  } 
}


#define PCAT_RTC_ADDRESS_REGISTER                   0x70
#define PCAT_RTC_DATA_REGISTER                      0x71

#define RTC_ADDRESS_SECONDS           0   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES           2   // R/W  Range 0..59
#define RTC_ADDRESS_HOURS             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_DAY_OF_THE_MONTH  7   // R/W  Range 1..31
#define RTC_ADDRESS_MONTH             8   // R/W  Range 1..12
#define RTC_ADDRESS_YEAR              9   // R/W  Range 0..99

UINT8
RtcReadDec (
  IN  UINT8 Address
  )
/*++

Routine Description:

  Read data register and return contents after convert to Decimal.

Arguments:

  Address - Register address to read

Returns:

  Value of data register contents converted to Decimal

--*/
{
  UINT8 High;
  UINT8 Low;
  UINT8 Result;
  
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, (UINT8) (Address | (UINT8) (IoRead8 (PCAT_RTC_ADDRESS_REGISTER) & 0x80)));
  Result = IoRead8 (PCAT_RTC_DATA_REGISTER);
  
  High = Result >> 4;
  Low = Result - (High << 4);

  return (UINT8) (Low + (High * 10));
}
    
EFI_STATUS
GetPDRUUID (
)
/*++

Routine Description:
  This routine reads the PDR information - 64KB size to memory buffer, 
  then copy UUID to platform info data structure.

Arguments:
    
Returns:
  EFI_SUCCESS  -  Function has completed successfully.

--*/
{
  EFI_STATUS            Status;
  EFI_CPU_IO_PROTOCOL   *CpuIo;
  //EFI_SPI_PROTOCOL      *SpiProtocol; // AptioV server override
  UINT8                 *MemSourcePtr;
  UINT32                PDRAddress;
  EFI_PLATFORM_INFO 	*pPIT;

  Status = gBS->LocateProtocol (
                    &gEfiCpuIoProtocolGuid,
                    NULL,
                    &CpuIo
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpuIo->Mem.Read (
              CpuIo,
              EfiCpuIoWidthUint32,
              (SB_RCBA + R_PCH_SPI_FREG4_PLATFORM_DATA),
              1,
              &PDRAddress
              );

  PDRAddress = (PDRAddress & B_PCH_SPI_FREG4_BASE_MASK) << 12;
  //AptioV server override - Start
  // Commenting code to locate Intel SPI protocol. AMI's flash protocol will be used.
  /*
  Status = gBS->LocateProtocol (
                  &gEfiSpiProtocolGuid,
                  NULL,
                  &SpiProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  */
  //AptioV server override - end

  MemSourcePtr = NULL;

  Status = gBS->AllocatePool( EfiBootServicesData,
                              BLOCK_SIZE_64KB,
                              &MemSourcePtr
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (MemSourcePtr, BLOCK_SIZE_64KB);

  pPIT = (EFI_PLATFORM_INFO *)MemSourcePtr;
  //AptioV server override - Start
  // Commenting Intel's flash access code to get the PDR and using AMI's flash access way.
  //
  // copy PDR data (64KB) to memory buffer
  //
  /*Status = SpiProtocol->Execute (
                          SpiProtocol,
                          SPI_OPCODE_READ_INDEX,  // OpcodeIndex
                          0,                      // PrefixOpcodeIndex
                          TRUE,                   // DataCycle
                          TRUE,                   // Atomic
                          FALSE,                  // ShiftOut
                          PDRAddress,             // Address, offset from descriptor region
                          BLOCK_SIZE_64KB,        // Data Number
                          (UINT8 *)MemSourcePtr,
                          EnumSpiRegionAll        // EnumSpiRegionPlatformData
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error during PDR read\n"));
  }*/

  //
  // AMI flash access to Read the PDR flash region.
  //
  FlashRead ( (UINT8 *)PDRAddress, (UINT8 *)MemSourcePtr , BLOCK_SIZE_64KB );
  //AptioV server override - end

    //
    // s5003276: SMBIOS UUID Changes On Every Boot
	// Save PDR UUID to SPI in AcpiSmmPlatform.c
    //
  //
  // update PDR from memory buffer
  //
  CopyMem (
      &mPrivatePlatformData.PlatformType.SystemUuid,
      pPIT->SystemUuid,
      sizeof (EFI_GUID)
      );
  gBS->FreePool (MemSourcePtr);

  return Status;
}



VOID
LocateBoardUuid (
   VOID
   )
{
  EFI_STATUS        Status;
  VOID              *HobList;
 // UINTN             Size;
  UINT8             *Buffer;

//  Status = ManageMfgModeVariable();
  //
  // Create a variable to be used for the version command
  //  
/*  Status = gRT->SetVariable (
    L"FWVERSIONDATA",
    &gEfiSetupVariableGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
    sizeof(FW_VERSION),
    &FwVersionData
    );
  ASSERT_EFI_ERROR(Status);*/

 // Size = sizeof (EFI_GUID);

  //
  // Get Hob list
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  //
  // Check for MFG UUID HOB
  //
  Buffer= GetNextGuidHob (&gEfiMfgUUIDHobGuid, HobList);

  // if (! (Size != sizeof (EFI_GUID) || EFI_ERROR (Status))) {
  if (Buffer != NULL) {

    //
    // If the MFG UUID Hob is present, then delete variable
    //
    Status = gRT->SetVariable (
                      VarMfgUuid,
                      &gEfiSetupVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      0,
                      NULL
                      );
  }

  Status = GetPDRUUID();

}

VOID
EFIAPI
NotifyVariableEventCallback (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
/*++

Routine Description:
  This notification function is invoked when an instance of the
  Variable protocol is produced.

  This routine attempts to locate the board UUID in NV storage.  It should
  only be there if programmed by MFG and it will only be written to flash
  if the system is in MFG mode as controlled by the HW.

Arguments:
  Event - The event that occured
  Context - For EFI compatiblity.  Not used.

Returns:
  Nothing

--*/
{
LocateBoardUuid ();
}

VOID
EFIAPI
SerialPortInitializeForOS (
  EFI_EVENT  Event,
  VOID       *Context
  )
/*++

Routine Description:

  Initialize Serial Port for OS boot

    The Baud Rate Divisor registers are programmed and the LCR
    is used to configure the communications format.

Arguments:

  None

Returns:

  None

--*/
{
//Aptio Server override-start: In simics, it is hanging in this funciton so commenting this function.
#if 0
  UINTN                   VarSize;
  SYSTEM_CONFIGURATION    SetupData;
  UINTN                   Divisor;
  UINT8                   OutputData;
  UINT8                   Data;
  UINT16                  ComBase  = 0x3f8;
  UINTN                   Bps      = 115200;
  UINT8                   DataBits = 8;
  UINT8                   Stop     = 1;
  UINT8                   Parity   = 0;
  UINT8                   BreakSet = 0;
  EFI_STATUS              Status = EFI_SUCCESS;

  

  //
  // Get Baud Rate from Variable
  //
  VarSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable (L"IntelSetup", &mSystemConfigurationGuid, NULL, &VarSize, &SetupData);
 
  if(!EFI_ERROR (Status)){
    Bps = SetupData.SerialBaudRate;
  }
  
  //InitializeSio();
  //
  // Some init is done by the platform status code initialization.
  //
  //
  // Map 5..8 to 0..3
  //
    
  Data = (UINT8) (DataBits - (UINT8) 5);

  //
  // Calculate divisor for baud generator
  //
  Divisor = 115200 / Bps;

  //
  // Set communications format
  //
  OutputData = (UINT8) ((DLAB << 7) | ((BreakSet << 6) | ((Parity << 3) | ((Stop << 2) | Data))));
  IoWrite8 (ComBase + LCR_OFFSET, OutputData);

  //
  // Configure baud rate
  //
  IoWrite8 (ComBase + BAUD_HIGH_OFFSET, (UINT8) (Divisor >> 8));
  IoWrite8 (ComBase + BAUD_LOW_OFFSET, (UINT8) (Divisor & 0xff));

  //
  // Switch back to bank 0
  //
  OutputData = (UINT8) ((~DLAB << 7) | ((BreakSet << 6) | ((Parity << 3) | ((Stop << 2) | Data))));
  IoWrite8 (ComBase + LCR_OFFSET, OutputData);
#endif
//Aptio Server override-End: In simics, it is hanging in this funciton so commenting this function.
  return;
}  


//APTIOV SERVER override commented for Grangeville
/*
VOID
AssertPostGpio (
  EFI_EVENT  Event,
  VOID       *Context
  )
/*++

Routine Description:

  Assert the POST complete GPIO
  
Arguments:

  Event         Pointer to the event that triggered this Ccllback Function
  Context       VOID Pointer required for Ccllback functio

Returns:

  EFI_SUCCESS         - Assertion successfully


--*/
/*
{
  EFI_STATUS                  Status;
  UINT32                      Data32;
  EFI_PLATFORM_TYPE_PROTOCOL  *PlatformType;

  // Locate the platform type protocol
  //
  Status = gBS->LocateProtocol (
             &gEfiPlatformTypeProtocolGuid,
             NULL,
             &PlatformType
             );
 
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Cannot locate platform type protocol\n"));
    return;
  }

  switch (PlatformType->Type) {

    // Include all Grantley platforms except for AztecCity (Workstation - ME9)
    //
    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCitySRPDDR3:
    case TypeMayanCityCRBDDR3:
    case TypeWildcatPass:
    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
    case TypeHalfWidthCitySRP:


    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:

      // If GPIO <= 31
      //
      if (PcdGet64(PcdPostCompleteGpio) <= 31) {
        Data32 = IoRead32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL);
        Data32 &= ~(1 << PcdGet64(PcdPostCompleteGpio));
        IoWrite32((PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL), Data32);
      } else {

        // If GPIO <= 63
        //
        if (PcdGet64(PcdPostCompleteGpio) <= 63) {
          Data32 = IoRead32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL2);
          Data32 &= ~(1 << (PcdGet64(PcdPostCompleteGpio) - 32));
          IoWrite32((PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL2), Data32);
        } else {

          // If GPIO >= 64
          //
          if (PcdGet64(PcdPostCompleteGpio) >= 64) {
            Data32 = IoRead32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL3);
            Data32 &= ~(1 << (PcdGet64(PcdPostCompleteGpio) - 64));
            IoWrite32((PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL3), Data32);
          }
        }
      }

      break;

    default:

      break;
  }
  gBS->CloseEvent(Event); //AptioV server override: Close event so that is isn't called again.
  return;
}

*/ //Server override end

VOID
EFIAPI
PlatformGetProcessorID (
  IN OUT UINT32  *RegEax
  )
/*++

Routine Description:

  Gets the CpuId and fills in the pointer with the value.
  Needed for executing CpuId on other APs.

Arguments:

  RegEax - Pointer to be used to pass the CpuId value

Returns:

  None

--*/
{

  AsmCpuid (EFI_CPUID_VERSION_INFO, RegEax, NULL, NULL, NULL);
}

VOID
EFIAPI
UpdateMcuCpuSteppings (
  EFI_EVENT  Event,
  VOID       *Context
   )
/*++

Routine Description:

  Update MCU with Cpu Steppings

Arguments:

  Event - The event ID that called this function
  Context - Unused

Returns:

  None

--*/
{
  EFI_MP_SERVICES_PROTOCOL      *MpServices = NULL;
  EFI_SMBUS_HC_PROTOCOL         *Smbus = NULL;
  EFI_STATUS                    Status;
  UINTN                         CpuNum;
  UINTN                         CpuNumEnabled;
  UINT32                        RegEax;
  UINTN                         Index;
  EFI_PROCESSOR_INFORMATION     ProcessorInfo;
  UINT8                         SmbusCommand;
  UINTN                         SmbusLength;
  UINT8                         SmbusData[2];
  EFI_SMBUS_DEVICE_ADDRESS      SlaveAddress;

  //
  //  Initialize variables, structs, and arrays
  //
  ZeroMem((VOID*) SmbusData, sizeof(UINT8)*2);
  ZeroMem(&ProcessorInfo, sizeof(EFI_PROCESSOR_INFORMATION));
  
  SmbusCommand = 0x45;  //CMD_WRITE_CPU_STEPPING;
  
  //
  // Locate the Smbus and MpServices protocols
  //
  Status = gBS->LocateProtocol (
                &gEfiSmbusHcProtocolGuid, 
                NULL, 
                (VOID**) &Smbus
                );
  ASSERT_EFI_ERROR(Status);

  Status = gBS->LocateProtocol (
                &gEfiMpServiceProtocolGuid,
                NULL,
                (VOID **) &MpServices
                );
  ASSERT_EFI_ERROR(Status);
  
  //
  // Get number of enabled processors
  // 
  Status = MpServices->GetNumberOfProcessors(
            MpServices,
            &CpuNum,
            &CpuNumEnabled
            );
  ASSERT_EFI_ERROR(Status);
  
  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  //
  // Fill in Proc0 Stepping
  //
  SmbusData[0] = (RegEax & 0x0F);
  
  //
  // Find an AP that belongs to 2nd processor
  //
  for(Index = 1; Index < CpuNumEnabled; Index++){
    Status = MpServices->GetProcessorInfo(
              MpServices,
              Index,
              &ProcessorInfo
              );
    ASSERT_EFI_ERROR(Status);

    if(ProcessorInfo.Location.Package != 0)
      break;
    
  }
  
  //
  // If one is found, then get the CpuId of it
  //
  if(ProcessorInfo.Location.Package > 0){ 
    
    Status = MpServices->StartupThisAP (
        MpServices,
        (EFI_AP_PROCEDURE)PlatformGetProcessorID,
        Index,             // 2nd Cpu
        NULL,
        0,                // No timeout
        &RegEax,
        NULL
    );
    
    ASSERT_EFI_ERROR(Status);
    
    SmbusData[1] = (RegEax & 0x0F);
  }
  
  //
  // Send Cpu Steppings to MCU
  //
  SlaveAddress.SmbusDeviceAddress = (0x72 >> 1);
  SmbusLength  = 2;
  Status = Smbus->Execute( 
                Smbus, 
                SlaveAddress, 
                SmbusCommand,
                EfiSmbusBWBRProcessCall, 
                FALSE, 
                &SmbusLength, 
                &SmbusData
  );
    
  gBS->Stall(20000);
  
  //
  // Ensure that we have back 3 bytes and retry until buffer is free
  //
  if(Status == EFI_SUCCESS)
  {
    while (SmbusData[0] == 1){ // Check if BWBR was deferred by MCU
      SmbusLength = 1; // Set length to 1 to only send buffer ID
      SmbusCommand = 0xb;
      SmbusData[ 0 ] = SmbusData[1]; // Send buffer ID to read
      Status = Smbus->Execute( 
                    Smbus,
                    SlaveAddress,
                    SmbusCommand,
                    EfiSmbusBWBRProcessCall,
                    FALSE,
                    &SmbusLength,
                    &SmbusData
      );
    }
  }

  return;
}

//
// Define platform type check
//
EFI_STATUS
EFIAPI
PlatformTypeInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
/*++

Routine Description:
  Entry point for the driver.

  This routine reads the PlatformType GPI on FWH and produces a protocol 
  to be consumed by the chipset driver to effect those settings.

Arguments:

  ImageHandle  -  Image Handle.
  SystemTable  -  EFI System Table.
  
Returns:

  EFI_SUCCESS  -  Function has completed successfully.
  
--*/
{
  EFI_STATUS                      Status;
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfoHobData = NULL;
  VOID                        	 *VarArch;
  IIO_UDS                         *UdsHobPtr;
  EFI_GUID                        UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
#ifndef GRANGEVILLE_FLAG
  EFI_EVENT   					          ReadyToBootEvent;
#endif



  //
  // Initialize driver private data.
  // Only one instance exists
  //
  ZeroMem (&mPrivatePlatformData, sizeof (mPrivatePlatformData));
  mPrivatePlatformData.Signature            = EFI_PLATFORM_TYPE_DRIVER_PRIVATE_SIGNATURE;

  //
  // Search for the Platform Info PEIM GUID HOB.
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfoHobData  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfoHobData);

  mPrivatePlatformData.PlatformType.BoardId     = PlatformInfoHobData->BoardId;
  mPrivatePlatformData.PlatformType.IioSku      = PlatformInfoHobData->IioSku;
  mPrivatePlatformData.PlatformType.IioRevision = PlatformInfoHobData->IioRevision;
  mPrivatePlatformData.PlatformType.PchSku      = PlatformInfoHobData->PchSku;
  mPrivatePlatformData.PlatformType.PchRevision = PlatformInfoHobData->PchRevision;
  mPrivatePlatformData.PlatformType.PchType     = PlatformInfoHobData->PchType;  //Include PCH SKU type
  mPrivatePlatformData.PlatformType.CpuType     = PlatformInfoHobData->CpuType;
  mPrivatePlatformData.PlatformType.CpuStepping = PlatformInfoHobData->CpuStepping;
  mPrivatePlatformData.PlatformType.PchData.PchPciePortCfg1 = PlatformInfoHobData->PchData.PchPciePortCfg1;
  mPrivatePlatformData.PlatformType.PchData.PchPciePortCfg2 = PlatformInfoHobData->PchData.PchPciePortCfg2;
  mPrivatePlatformData.PlatformType.PchData.PchPcieSBDE     = PlatformInfoHobData->PchData.PchPcieSBDE;
  mPrivatePlatformData.PlatformType.PchData.PchLanSupported = PlatformInfoHobData->PchData.PchLanSupported;
  mPrivatePlatformData.PlatformType.PchData.GbePciePortNum  = PlatformInfoHobData->PchData.GbePciePortNum;
  mPrivatePlatformData.PlatformType.PchData.GbeRegionInvalid  = PlatformInfoHobData->PchData.GbeRegionInvalid;
  
  //
  // Check if platform type recorded in HOB by PEI
  //
  switch (PlatformInfoHobData->BoardId) {
#ifndef GRANGEVILLE_FLAG
    case TypeMayanCityCRB:
      StrCpy (PlatformName, L"MayanCityCRB");
      break;

    case TypeMayanCitySRP:
      StrCpy (PlatformName, L"MayanCitySRP");
      break;

    case TypeMayanCityCRBDDR3:
      StrCpy (PlatformName, L"MayanCityCRBDDR3");
      break;

    case TypeMayanCitySRPDDR3:
      StrCpy (PlatformName, L"MayanCitySRPDDR3");
      break;

    case TypeWildcatPass:
      StrCpy (PlatformName, L"WildcatPass");
      break;

    case TypeYakimaCityCRB:
      StrCpy (PlatformName, L"YakimaCityCRB");
      break;

    case TypeYakimaCitySRP:
      StrCpy (PlatformName, L"YakimaCitySRP");
      break;

    case TypeYakimaCitySTHI:
      StrCpy (PlatformName, L"YakimaCitySTHI");
      break;

    case TypeAztecCityCRBSRP:
      StrCpy (PlatformName, L"AztecCityCRBSRP");
      break;

    case TypeAztecCitySTHI:
      StrCpy (PlatformName, L"AztecCitySTHI");
      break;

    case TypeIncaCityCRB:
      StrCpy (PlatformName, L"IncaCityCRB");
      break;

    case TypeIncaCitySRP:
      StrCpy (PlatformName, L"IncaCitySRP");
      break;

    case TypeHalfWidthCitySRP:
      StrCpy (PlatformName, L"HalfWidthCitySRP");
      break;


    case TypeHedtSKU1CRB:
      if (PlatformInfoHobData->TypeRevisionId == HEDT_FAB2)
      {
          StrCpy (PlatformName, L"HedtSKU1CRBFab2");
      }
      else
      {
          StrCpy (PlatformName, L"HedtSKU1CRB");
      }
      break;

    case TypeHedtEv:
      StrCpy (PlatformName, L"HedtEv");
      break;

    case TypeHedtDDR3:
      StrCpy (PlatformName, L"HedtDDR3");
      break;

    case TypeSaxtonRock:
      StrCpy (PlatformName, L"SaxtonRock");
      break;

    case TypeEchoRock:
      StrCpy (PlatformName, L"EchoRock");
      break;

    case TypeBalanceRock:
      StrCpy (PlatformName, L"BalanceRock");
      break;

    case TypeEmeraldPoint:
      StrCpy (PlatformName, L"EmeraldPoint");
      break;
#else

 

  case TypeBeverlyCoveCRB:
    StrCpy (PlatformName, L"TypeBeverlyCoveCRB");
    break;

  case TypeBeverlyCoveSODimmCRB:
    StrCpy (PlatformName, L"TypeBeverlyCoveSODimmCRB");
    break;

  case TypeTrabucoCanyon:
      StrCpy (PlatformName, L"TypeTrabucoCanyon");
      break;

  case TypePiestewaPeak:
      StrCpy (PlatformName, L"TypePiestewaPeak");
      break;

  case TypeCamelbackMountain:
      StrCpy (PlatformName, L"TypeCamelbackMountain");
      break;

  case TypeTlaquepaque:
      StrCpy (PlatformName, L"TypeTlaquepaque");
      break;

  case TypeEchoCanyon:
	  StrCpy (PlatformName, L"TypeEchoCanyon");
	  break;

#endif

  default:
    StrCpy (PlatformName, L"Unknown");
    break;
}

  mPrivatePlatformData.PlatformType.TypeStringPtr = (UINT64)PlatformName;


  GetPchName();
  GetIioName();
  Status = gBS->LocateProtocol (
                   &gEfiVariableArchProtocolGuid,
                   NULL,
                   &VarArch
                   );

  if ( !EFI_ERROR (Status) ){
      LocateBoardUuid ();
  }
  
  //
  // Register to be notified when the variable write protocol has been produced.
  // Check for UUID in NV area.  If not found or error, then use UUID in Hob.
  //

  if ( EFI_ERROR (Status) ) {
    Status = gBS->CreateEvent (
                   EVT_NOTIFY_SIGNAL,
                   TPL_CALLBACK,
                   NotifyVariableEventCallback,
                   NULL,
                   &mEfiVariableProtocolEvent
                   );
    ASSERT_EFI_ERROR (Status);

    Status = gBS->RegisterProtocolNotify (
                  &gEfiVariableArchProtocolGuid,
                  mEfiVariableProtocolEvent,
                  &mEfiVariableProtocolNotifyReg
                  );
    ASSERT_EFI_ERROR (Status);
  }


  //
  // Install the PlatformType policy.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPrivatePlatformData.Handle,
                  &gEfiPlatformTypeProtocolGuid,
                  &mPrivatePlatformData.PlatformType,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_ERROR, "%s platform is detected!\n", PlatformName));

  //
  // Time to get the IIO_UDS HOB data stored in the PEI driver
  //
  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  UdsHobPtr = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (UdsHobPtr);

  //
  // Allocate Memory Pool for Universal Data Storage so that protocol can expose it
  //
  Status = gBS->AllocatePool ( EfiReservedMemoryType, sizeof (IIO_UDS), (VOID **) &IioUdsData );
  ASSERT_EFI_ERROR (Status);
  
  //
  // Initialize the Pool Memory with the data from the Hand-Off-Block
  //
  CopyMem(IioUdsData, UdsHobPtr, sizeof(IIO_UDS));

  //
  // Build the IIO_UDS driver instance for protocol publishing  
  //
  ZeroMem (&mIioUdsPrivateData, sizeof (mIioUdsPrivateData));
    
  mIioUdsPrivateData.Signature            = EFI_IIO_UDS_DRIVER_PRIVATE_SIGNATURE;
  mIioUdsPrivateData.IioUds.IioUdsPtr     = IioUdsData;
  mIioUdsPrivateData.IioUds.EnableVc      = NULL;

  //
  // Install the IioUds Protocol.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mIioUdsPrivateData.Handle,
                  &gEfiIioUdsProtocolGuid,
                  &mIioUdsPrivateData.IioUds,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);  


#ifndef GRANGEVILLE_FLAG
  //
  // Set up callback to assert the POST Complete GPIO to the iBMC
  //
  Status = EfiCreateEventReadyToBootEx(
             TPL_NOTIFY,
             AssertPostGpio,
             NULL,
             &ReadyToBootEvent
             );
#endif

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
