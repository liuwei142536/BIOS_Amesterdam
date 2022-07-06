/**
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**
@copyright
Copyright (c) 2009-2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  IccPlatform.c

Abstract:

  ICC is divided into 4 modules:
  - IccOverClocking: the overclocking library, distributed as binary,
  - IccSetup: code linked with setup browser; supports overclocking and uses overclocking lib
  - IccPlatform: not linked with setup, supports all Icc features except overclocking; does not use overclocking lib
  No part of ICC belongs to BIOS Reference Code.

  IccPlatform disables unused PCI/PCIe clocks and transfers data from MBP to Icc Setup

--*/
#include <Token.h> // AptioV server override
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <PchAccess.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/PlatformInfo.h>
#include <Protocol/PlatformType.h>
#include <Protocol/Wdt.h>
#include <Guid/SetupVariable.h>
#include <Guid/GlobalVariable.h>
#include "Platform.h"
#include "PchRegs/PchRegsSpi.h"
#include "IccPlatform.h"
#include "Protocol/IccDataProtocol.h"
#include "MeState.h"
#include "Library/MeTypeLib.h"

ICC_DATA_PROTOCOL *mIccDataProtocol = NULL;

//AptioV server override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
#define __EFI__H__
#define __HOB__H__
#include <Protocol\PerfTuneProtocol.h>
#include "Protocol/IccOverClocking.h"
#include <Setup.h>
#include <Library/HobLib.h>
#include <Guid/HobList.h>

#define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }

ICC_OVERCLOCKING_PROTOCOL* gIccOverClockingProtocol = NULL;
EFI_GUID gPerfTune3xProtocolGuid           = PERF_TUNE_ASL_PROTOCOL_GUID;
EFI_GUID gPerfTuneDataHobGuid = AMI_PERF_TUNE_DATA_HOB_GUID;
//////EFI_GUID gHobListGuid         = HOB_LIST_GUID;
BOOLEAN	 gFound = FALSE;
BOOLEAN	 gBootCurrent = FALSE;
UINT16   gCurrentFrequency;
UINT16   gPEGDMIRatio;
ICC_CLOCK_SETTINGS	  gClocksetting;

//----------------------------CPU Ratio GACI DATA----------------------------
//	   GACI_DATA	DevNameGaciData {					   ControlID,NumberOfValues,Precision,Flags,DefaultDataValue,MinDataValue,MaxDataValue,MinDisplayValue,MaxDisplayValue}
static GACI_DATA	IccFreqGaciData	= {BIOS_HOST_CLOCK_IMPLEMENTATION,		  8001,		2,	  0,		   10000,		10000,		 18000,			10000,			 18000};

static GACI_DATA	IccPegDmiGaciData	= {BIOS_PEG_DMI_RATIO_IMPLEMENTATION,		  0xFFFF,		2,	  0,		   0,		0,		 0,			0,			 0};
#define ICC_CLOCK_COUNT 8
UINT8   ClockID;
#endif  //PERF_TUNE_SUPPORT
//AMI Override for XTU - END

//AMI Override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
BOOLEAN
EfiCompareGuid (
  IN EFI_GUID *Guid1,
  IN EFI_GUID *Guid2
  )
/*++

Routine Description:

  Compares two GUIDs

Arguments:

  Guid1 - guid to compare

  Guid2 - guid to compare

Returns:
  TRUE     if Guid1 == Guid2
  FALSE    if Guid1 != Guid2

--*/
{
  UINTN Index;

  //
  // compare byte by byte
  //
  for (Index = 0; Index < 16; ++Index) {
    if (*(((UINT8*) Guid1) + Index) != *(((UINT8*) Guid2) + Index)) {
      return FALSE;
    }
  }
  return TRUE;
}

EFI_STATUS
EfiLibGetSystemConfigurationTable (
  IN EFI_GUID *TableGuid,
  IN OUT VOID **Table
  )
/*++

Routine Description:

  Return the EFI 1.0 System Tabl entry with TableGuid

Arguments:

  TableGuid - Name of entry to return in the system table
  Table     - Pointer in EFI system table associated with TableGuid

Returns:

  EFI_SUCCESS - Table returned;
  EFI_NOT_FOUND - TableGuid not in EFI system table

--*/
{
  UINTN Index;

  for (Index = 0; Index < gST->NumberOfTableEntries; Index++) {
    if (EfiCompareGuid (TableGuid, &(gST->ConfigurationTable[Index].VendorGuid))) {
      *Table = gST->ConfigurationTable[Index].VendorTable;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

// Compiler (.NET 2008) automatically inserts memcpy/memset fn calls
// in certain cases due to optimization.
// Disable optimization to solve compilation error
#pragma optimize( "g", off )
VOID PerfTuneIcc(
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  VOID	*pPerfTuneDataHobList = NULL;
  PERF_TUNE_DATA_HOB	*pBiosSettingData;
  BIOS_SETTING_DATA  	BiosSettingData;
  UINT16            	RequestFrequency,PEGDMIRatio,bPEGDMIRatio;
  ICC_CLOCK_SETTINGS    RequestSetting;
  EFI_GUID              SetupGuid = SETUP_GUID;
  EFI_STATUS        	Status;
  BOOLEAN               freqConsolidationBypass = TRUE;
  ICC_LIB_STATUS 	IccStatus;
  EFI_PEI_HOB_POINTERS  GuidHob;  
  WDT_PROTOCOL *InternalWdtProtocol;
  UINTN				VarSize = sizeof(BIOS_SETTING_DATA);
  BIOS_SETTING_DATA	DefaultData;
  EFI_GUID			EfiSetupGuid = SETUP_GUID;

  //
  // Get Hob list
  //
  Status = EfiLibGetSystemConfigurationTable (&gEfiHobListGuid, &GuidHob.Raw);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Status = EFI_NOT_FOUND; EFI_ERROR (Status);) {
    if (END_OF_HOB_LIST (GuidHob)) {
      Status = EFI_NOT_FOUND;
      break;
    }
    if (GET_HOB_TYPE (GuidHob) == EFI_HOB_TYPE_GUID_EXTENSION) {
      if (EfiCompareGuid (&gPerfTuneDataHobGuid, &GuidHob.Guid->Name)) {
        Status     = EFI_SUCCESS;
        pBiosSettingData = (PERF_TUNE_DATA_HOB *)(GuidHob.Raw);
        break;
      }
    }
    GuidHob.Raw = GET_NEXT_HOB (GuidHob);
  }

  if (EFI_ERROR(Status)) return;

  if(pBiosSettingData->IsCpuChanged || pBiosSettingData->IsDimmChanged){	  
	Status = gRT->GetVariable(
					L"OcDefault",
					&EfiSetupGuid,
					NULL,
					&VarSize,
					&DefaultData
					);

	BiosSettingData = pBiosSettingData->PerfTuneDataHob;
  	RequestFrequency = DefaultData.HostClockFreq;
  	PEGDMIRatio = DefaultData.PEGDMIRatio;
  }else{	
  	BiosSettingData = pBiosSettingData->PerfTuneDataHob;
  	RequestFrequency = BiosSettingData.HostClockFreq;
  	PEGDMIRatio = BiosSettingData.PEGDMIRatio;
  }

  if ( (PEGDMIRatio == 100) || (PEGDMIRatio == 0) || (PEGDMIRatio == 0xFFFF)){
	bPEGDMIRatio = 0;
  }else if ( (PEGDMIRatio == 125) || (PEGDMIRatio == 1)){
    bPEGDMIRatio = 1;
  }else if ( (PEGDMIRatio == 167) || (PEGDMIRatio == 2) ){
    bPEGDMIRatio = 2;
  }else if ( (PEGDMIRatio == 250) || (PEGDMIRatio == 3) ){
    bPEGDMIRatio = 3;
  }else{
	bPEGDMIRatio = 0;
  }

  // Return if Overclocking Bclk not found
  if((!gFound) || ((RequestFrequency == gCurrentFrequency) && 
     (bPEGDMIRatio == gPEGDMIRatio) && !(pBiosSettingData->IsCpuChanged || pBiosSettingData->IsDimmChanged)) ) return;

  if (bPEGDMIRatio == gPEGDMIRatio){
    gBootCurrent = FALSE;
    RequestSetting.Frequency  = RequestFrequency * 10000;
    RequestSetting.SscMode    = gClocksetting.SscMode;
    if (bPEGDMIRatio==0){
      RequestSetting.SscPercent = 0x32;
    }else{
      RequestSetting.SscPercent = 0;
    }
  RequestSetting.DmiPegRatio = bPEGDMIRatio;
  
  gIccOverClockingProtocol->SetCurrentClockSettings(ClockID, 
                                                    RequestSetting,
                                                    &IccStatus);
  gIccOverClockingProtocol->GetCurrentClockSettings
                                    (ClockID, &RequestSetting, &IccStatus);

  RequestFrequency = RequestSetting.Frequency/10000;
  PEGDMIRatio = RequestSetting.DmiPegRatio;
  }else{
    gBootCurrent = TRUE;
    RequestSetting.Frequency  = RequestFrequency * 10000;
    RequestSetting.SscMode    = gClocksetting.SscMode;
    if (bPEGDMIRatio==0){
      RequestSetting.SscPercent = 0x32;
    }else{
      RequestSetting.SscPercent = 0;
    }
  RequestSetting.DmiPegRatio = bPEGDMIRatio;
  
  gIccOverClockingProtocol->SetBootClockSettings(ClockID, 
                                                    RequestSetting,
                                                    &IccStatus);

  gIccOverClockingProtocol->GetBootClockSettings
                                    (ClockID, &RequestSetting, &IccStatus);


  RequestFrequency = RequestSetting.Frequency/10000;
  PEGDMIRatio = RequestSetting.DmiPegRatio;
  }
       
  if(IccStatus == ICC_LIB_STATUS_SUCCESS)
  {
      // OverClocking Success
      // ReStore back to OcCurrent
      BiosSettingData.HostClockFreq = RequestFrequency;
      IccFreqGaciData.DefaultDataValue = RequestFrequency;
		  if ( PEGDMIRatio == 0 ){
		  				 BiosSettingData.PEGDMIRatio = 100;
               IccPegDmiGaciData.DefaultDataValue = 100;
          }else if ( PEGDMIRatio == 1 ){
          		 BiosSettingData.PEGDMIRatio = 125;
               IccPegDmiGaciData.DefaultDataValue = 125;
          }else if ( PEGDMIRatio == 2 ){
          		 BiosSettingData.PEGDMIRatio = 167;
               IccPegDmiGaciData.DefaultDataValue = 167;
          }else if ( PEGDMIRatio == 3 ){
          		 BiosSettingData.PEGDMIRatio = 250;
               IccPegDmiGaciData.DefaultDataValue = 250;
          }

      Status = gRT->SetVariable(
                    L"OcCurrent",
                    &SetupGuid,
                    EFI_VARIABLE_NON_VOLATILE
                    | EFI_VARIABLE_BOOTSERVICE_ACCESS
                    | EFI_VARIABLE_RUNTIME_ACCESS,
                    sizeof(BIOS_SETTING_DATA),
                    &BiosSettingData);	
	  ASSERT_EFI_ERROR (Status);	  
  }

  if (gBootCurrent == TRUE){
	Status = gBS->LocateProtocol(&gWdtProtocolGuid, NULL, &InternalWdtProtocol);
        if(!EFI_ERROR(Status))
        { 
			InternalWdtProtocol->AllowKnownReset();  
		}  
	IoWrite8 (0xCF9,0x06);//Setting REG/DMI ratio have to reset.
	EFI_DEADLOOP();
  }

  return;
}
#pragma optimize( "g", on )


VOID
IccXtuHandler(
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  UINT8              i;
  EFI_STATUS         Status;
  PERF_TUNE_ASL_PROTOCOL *PerfTune3xProtocol;
  UINTN              Len = 0;
  UINT8              *Buffer = NULL;
  EFI_GUID           IccOverClockingProtocolGuid = ICC_OVERCLOCKING_PROTOCOL_GUID;

  Status = gBS->LocateProtocol(&IccOverClockingProtocolGuid, NULL, &gIccOverClockingProtocol);
  if(EFI_ERROR(Status)) return;
  Status = gBS->LocateProtocol(&gPerfTune3xProtocolGuid, NULL, &PerfTune3xProtocol);
  if(EFI_ERROR(Status)) return;

  for(i = 0; i < ICC_CLOCK_COUNT ; i++)
  {
    ICC_LIB_STATUS     IccStatus;
    ICC_CLOCK_RANGES   ClockRange;

    gIccOverClockingProtocol->GetClockRanges(i, &ClockRange, &IccStatus);

    if ( (ClockRange.UsageMask & (1<<ICC_CLOCK_USAGE_BCLK) ) &&
	     (ClockRange.UsageMask & (1<<ICC_CLOCK_USAGE_DMI) ) &&
         (ClockRange.UsageMask & (1<<ICC_CLOCK_USAGE_PEG) ) &&
	     (IccStatus == ICC_LIB_STATUS_SUCCESS)
    ) {
     if (ClockRange.FrequencyMax != ClockRange.FrequencyMin)
          gIccOverClockingProtocol->GetCurrentClockSettings
                                    (i, &gClocksetting, &IccStatus);
          if(IccStatus != ICC_LIB_STATUS_SUCCESS)
              break;


          gCurrentFrequency = gClocksetting.Frequency/10000;
          if((gCurrentFrequency - 4000) < (UINT16)(ClockRange.FrequencyMin/10000)){
              IccFreqGaciData.MinDataValue = ClockRange.FrequencyMin/10000;
              IccFreqGaciData.MinDisplayValue = ClockRange.FrequencyMin/10000;
          }
          else{
              IccFreqGaciData.MinDataValue = gCurrentFrequency - 4000;
              IccFreqGaciData.MinDisplayValue = gCurrentFrequency - 4000;
          }
          if((gCurrentFrequency + 5000) > (UINT16)(ClockRange.FrequencyMax/10000)){
              IccFreqGaciData.MaxDataValue = ClockRange.FrequencyMax/10000;
              IccFreqGaciData.MaxDisplayValue = ClockRange.FrequencyMax/10000;
          }
          else{
              IccFreqGaciData.MaxDataValue = gCurrentFrequency + 5000;
              IccFreqGaciData.MaxDisplayValue = gCurrentFrequency + 5000;  
          }
          IccFreqGaciData.DefaultDataValue = gCurrentFrequency;
		  IccFreqGaciData.NumberOfValues = IccFreqGaciData.MaxDataValue - IccFreqGaciData.MinDataValue + 1;
          gFound = TRUE;
          ClockID = i;

          gPEGDMIRatio = gClocksetting.DmiPegRatio;

		  if ( gPEGDMIRatio == 0 ){
               IccPegDmiGaciData.DefaultDataValue = 100;
          }else if ( gPEGDMIRatio == 1 ){
               IccPegDmiGaciData.DefaultDataValue = 125;
          }else if ( gPEGDMIRatio == 2 ){
               IccPegDmiGaciData.DefaultDataValue = 167;
          }else if ( gPEGDMIRatio == 3 ){
               IccPegDmiGaciData.DefaultDataValue = 250;
          }
          break;
    }
  }

  PerfTuneIcc(ImageHandle,SystemTable);
  // reference clock frequency
  Len = sizeof(IccFreqGaciData);
  Status = gBS->AllocatePool(EfiBootServicesData, Len, &Buffer);
  gBS->SetMem(Buffer, Len, 0 );

  gBS->CopyMem(Buffer, &IccFreqGaciData, sizeof(IccFreqGaciData));
  gBS->CopyMem(Buffer + sizeof(IccFreqGaciData), &IccFreqGaciData, sizeof(IccFreqGaciData));
  Status = PerfTune3xProtocol->SetGaciData(Buffer, Len);
  //PEG/DMI Ratio
  Len = sizeof(IccPegDmiGaciData);
  Status = gBS->AllocatePool(EfiBootServicesData, Len, &Buffer);
  gBS->SetMem(Buffer, Len, 0 );

  gBS->CopyMem(Buffer, &IccPegDmiGaciData, sizeof(IccPegDmiGaciData));
  Status = PerfTune3xProtocol->SetGaciData(Buffer, Len);
  return;
}
#endif  //PERF_TUNE_SUPPORT
//AptioV server override for XTU - END 

EFI_STATUS
IccPlatformEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  DEBUG ((EFI_D_INFO, "(ICC) Entry Point to ICC_Platform\n"));

  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }

  Status = ReadMainSetupData (ImageHandle);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "(ICC) Failed to read setup data! Status = %r\n", Status));
    return Status;
  }

   Status = EfiCreateEventReadyToBootEx(
                TPL_NOTIFY,
                SendLateIccMessages,
                NULL,
                &Event
                );
  ASSERT_EFI_ERROR (Status);

//AptioV server override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
  IccXtuHandler(ImageHandle,SystemTable);
#endif  //PERF_TUNE_SUPPORT
//AptioV server override for XTU - END 

  return EFI_SUCCESS;
}

/**
Routine Description:
  Detects which PCIE clocks are not needed and can be turned off.
  PCIE bridge is accessed, one of its registers knows if there are cards
  present in slots
Arguments:
  UsedClocks: bitmask for Clock Enable: 1 = enable, 0 = disable
  AllClocks: bitmask for Clock Enable Mask: 1 = Clock Enable bit is valid, 0 = ignore Clock Enable bit
Returns:
  EFI_SUCCESS in all circumstances except when required protocols can't be located
**/


#ifndef GRANGEVILLE_FLAG
EFI_STATUS
DetectUsedClocks (
  OUT UINT32*UsedClocks,
  OUT UINT32*AllClocks
  )
{
  EFI_STATUS                      Status;
  PLATFORM_PCIE_SLOTS             *PciExpressSlots;
  UINT32                          i;
  EFI_PLATFORM_TYPE_PROTOCOL      *PlatformInfoProtocol;
  UINT8                           PciExpressQnt;
  UINT8                           FunctionNumber;
  UINT16                          VendorID;
  UINT16                          SlotStatus;
  UINT32                          Rcba;
  UINT32                          RootPortFunctionNumber = 0;
  UINT32                          Softstrap9 = 0;
  UINT32                          GbePhyOverPcie = 0;
  UINT32                          GbePortNumber = 0;
  UINT32                          SlotCapability;
  UINT8                           HotPlugSupport;

  // PCI support removed, PCIE stays. We no longer have any PCI ports on boards.

  //
  // all PCIE-related clocks are turned off by default and only turned on when needed
  // such negative logic covers situation where two or more slots share one clock
  //
  *UsedClocks = 0xFFFFFFFF;
  *AllClocks  = 0x0;

  Status = gBS->LocateProtocol (&gEfiPlatformTypeProtocolGuid, NULL, &PlatformInfoProtocol);
  ASSERT_EFI_ERROR(Status);

/* WARNING!
  DO NOT EDIT CLOCK-TO-SLOT ASSIGNMENTS UNLESS YOU CONSULT BOARD SCHEMATICS FIRST.
  If you plan to 'add support' for a new board type and you don't read electrical
  schematics then add your new board in 'default' case. This will make sure ICC
  code doesn't turn off clocks which are vital for the board.
  Boards will work much better without ICC support than with a broken one.
*/

  switch(PlatformInfoProtocol->BoardId) {
  // If board's clock-to-slot mapping is not known, put it below to prevent ICC from disabling its clocks
    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCitySRPDDR3:
    case TypeWildcatPass:
    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
    case TypeMayanCityCRBDDR3:
    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
    case TypeHalfWidthCitySRP:
    case TypeKahunaDVP2SPbg:
    case TypeKahunaDVP2S:
    case TypeArandasVVP2S:
    case TypeArandasVVP4S:
    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
    case TypeSaxtonRock:
    case TypeEchoRock:
    case TypeBalanceRock:
//    case TypeEmeraldPoint:
    default:
      DEBUG ((EFI_D_ERROR, "(ICC) Motherboard with unknown clock-to-PCIE slot mapping.\n"));
      return EFI_UNSUPPORTED;
// TBD : This case is made as example and allow compiler to treeat it ad reacheamle
    case TypeEmeraldPoint:
      DEBUG ((EFI_D_INFO, "(ICC) FlatheadCreekEV\n"));
      PciExpressSlots = PciExpressFlatheadCreekEV;
      PciExpressQnt   = sizeof (PciExpressFlatheadCreekEV) / sizeof (PLATFORM_PCIE_SLOTS);
      break;

  }

  for (i = 0; i < PciExpressQnt; i++) {
    *UsedClocks &= ~(PciExpressSlots[i].Clock);
    *AllClocks |= PciExpressSlots[i].Clock;
  }
  //
  // read RootPortFunctionNumber register, it knows (pcie bridge's function number) - to - (physical slot) mapping
  //
  if (PciExpressQnt > 0) {

    Rcba = MmioRead32 (
             MmPciAddress (
               0,
               DEFAULT_PCI_BUS_NUMBER_PCH,
               PCI_DEVICE_NUMBER_PCH_LPC,
               PCI_FUNCTION_NUMBER_PCH_LPC,
               R_PCH_LPC_RCBA
             )
           );

    Rcba &= B_PCH_LPC_RCBA_BAR;
    RootPortFunctionNumber = MmioRead32 ((UINTN) (Rcba + R_PCH_RCRB_RPFN));

    MmioAndThenOr32 (
      Rcba + R_PCH_SPI_FDOC,
      (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
      (UINT32) (V_PCH_SPI_FDOC_FDSS_PCHS | R_PCH_SPI_STRP9)
    );
    //
    // Check for GbE PHY presence using data from softstrap. It is undetectable as PCIe device.
    //
    Softstrap9 = MmioRead32 (Rcba + R_PCH_SPI_FDOD);
    GbePhyOverPcie = (Softstrap9 & B_PCH_SPI_STRP9_GBE_PCIE_EN) >> 11;
    GbePortNumber = (Softstrap9 & B_PCH_SPI_STRP9_GBE_PCIE_PSC) >> 8;
  }
  //
  // detect cards in PCIE slots
  //
  for (i = 0; i < PciExpressQnt; i++) {
    HotPlugSupport = 0;
    FunctionNumber =
      (
        RootPortFunctionNumber >>
        (S_PCH_RCRB_PRFN_RP_FIELD * PciExpressSlots[i].RootPortNumber)
      ) & 0x00000007;

    VendorID = MmioRead16 (
                 MmPciAddress (
                   0,
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
                   FunctionNumber,
                   R_PCH_PCIE_VENDOR_ID
                 )
               );

    SlotStatus = MmioRead16 (
                   MmPciAddress (
                     0,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
                     FunctionNumber,
                     R_PCH_PCIE_SLSTS
                   )
                 );

    //
    // If Hot Plug is supported, then Hot Plug Capable (bit6) and Hot Plug Surprise (bit5) in 
    // the Slot Capabilities register, D28:F0~7:Reg 54h, will be set.
    //
    SlotCapability =  MmioRead32 (
                        MmPciAddress (
                          0,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
                          FunctionNumber,
                          R_PCH_PCIE_SLCAP
                        )
                      ) & (UINT32) (B_PCH_PCIE_SLCAP_HPC | B_PCH_PCIE_SLCAP_HPS);

    if (VendorID != 0xFFFF && SlotCapability != 0) {
      HotPlugSupport = 1;
    }

    if (HotPlugSupport == 1 ||
         (VendorID != 0xFFFF && ((SlotStatus & B_PCH_PCIE_SLSTS_PDS) != 0)) ||
         ((GbePhyOverPcie != 0) && (PciExpressSlots[i].RootPortNumber == GbePortNumber))
       ) {
      *UsedClocks |= PciExpressSlots[i].Clock;
    }
  }

  DEBUG ((EFI_D_INFO, "(ICC) PCIE Clocks Disabled: 0x%08x\n", (*AllClocks & ~(*UsedClocks)) ));
  DEBUG ((EFI_D_INFO, "(ICC) PCIE Clocks Enabled:  0x%08x\n", (*AllClocks &   *UsedClocks ) ));
  DEBUG ((EFI_D_INFO, "(ICC) Clocks left alone:    0x%08x\n", ~(*AllClocks) ));

  return EFI_SUCCESS;
}
#endif

EFI_STATUS GetMpbData(ICC_MBP_DATA *IccMbpData)
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  DXE_MBP_DATA_PROTOCOL  *MbpData;
  EFI_PLATFORM_TYPE_PROTOCOL *PlatformInfoProtocol;

  DEBUG ((EFI_D_ERROR, "(ICC) GetMpbData() called.\n"));
  if (IccMbpData == NULL) {
     return Status;
  }
  ZeroMem (IccMbpData, sizeof (ICC_MBP_DATA));

  Status = gBS->LocateProtocol (&gEfiPlatformTypeProtocolGuid, NULL, &PlatformInfoProtocol);
  ASSERT_EFI_ERROR(Status);
  Status = gBS->LocateProtocol (&gMeBiosPayloadDataProtocolGuid, NULL, &MbpData);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "(ICC) Failed to read Mbp data! Status = %r\n", Status));
    return Status;
  }
  IccMbpData->Profile = MbpData->MeBiosPayload.IccProfile.IccProfileIndex;

  if ( (MbpData->MeBiosPayload.IccProfile.IccProfileSoftStrap & 0x80) != PROFILE_SELECTED_BY_BIOS ) {
    IccMbpData->ProfileSelectionAllowed = DONT_DISPLAY;
  } else {
    IccMbpData->ProfileSelectionAllowed = DISPLAY;
  }

  if ( MbpData->MeBiosPayload.IccProfile.NumIccProfiles == 0){
    // Firmware is unconfgured, and there are no profiles to select
    IccMbpData->ProfileSelectionAllowed = DONT_DISPLAY;
  }

  IccMbpData->NumProfiles = MbpData->MeBiosPayload.IccProfile.NumIccProfiles;

  //
  // Determine what type of lockmask is being used by firmware and copy to the iccsetupdata.
  //
  switch(MbpData->MeBiosPayload.IccProfile.IccLockRegInfo.RegBundles.BundlesCnt)
  {
    case 0:
      //
      // If lockmask has a zero bundle count this indicates the lockmask is
      // either all locked or all unlocked
      //
      if (MbpData->MeBiosPayload.IccProfile.IccLockRegInfo.RegBundles.AU == 0){
        IccMbpData->LockMask = OPTION_LOCK_ALL;
      }
      else if (MbpData->MeBiosPayload.IccProfile.IccLockRegInfo.RegBundles.AU == 1){
        IccMbpData->LockMask = OPTION_UNLOCK_ALL;
      }
    break;

    case 1:
      //
      // Check if it is set to the static lockmask for LPT-LP. If this does not match exactly,
      // it is considered a custom lockmask value. BundleCount = 1 is only valid on LPT-LP.
      //
      if (/*(PlatformInfoProtocol->Type == TypeUlt) && TBD*/
          (MbpData->MeBiosPayload.IccProfile.IccLockRegInfo.RegMask[0]== STATIC_REGISTERS_MASK0)) {
        IccMbpData->LockMask = OPTION_LOCK_STATIC;
      } else {
        IccMbpData->LockMask = OPTION_USE_OEM;
      }
    break;

    case 3:
      //
      // Check if it is set to the static lockmask for LPT-H. If this does not match exactly,
      // it is considered a custom lockmask value. BundleCount = 3 is only valid on LPT-H.
      //
      if (/*(PlatformInfoProtocol->Type == TypeTrad) && TBD*/
         (MbpData->MeBiosPayload.IccProfile.IccLockRegInfo.RegMask[0]== STATIC_REGISTERS_MASK0) &&
         (MbpData->MeBiosPayload.IccProfile.IccLockRegInfo.RegMask[1]== STATIC_REGISTERS_MASK1) &&
         (MbpData->MeBiosPayload.IccProfile.IccLockRegInfo.RegMask[2]== STATIC_REGISTERS_MASK2)){
        IccMbpData->LockMask = OPTION_LOCK_STATIC;
      } else {
        IccMbpData->LockMask = OPTION_USE_OEM;
      }
    break;

    default:
      //
      // BIOS does not support any lockmask that is not 0 or 3 bundles in length
      // Consider these lockmasks custom
      //
      IccMbpData->LockMask = OPTION_USE_OEM;
    break;
  }

  DEBUG ((EFI_D_ERROR, "(ICC) GetMpbData() returns Status = %r.\n", Status));
  return Status;
}

EFI_STATUS
ReadMainSetupData (
   IN EFI_HANDLE ImageHandle
  )
/*++
Routine Description:
  reads SETUP_DATA and creates IccConfig with all Icc-related setup informations
Arguments:
Returns:
  filled SETUP_DATA struct
--*/
{
  EFI_STATUS   Status;
  ICC_MBP_DATA IccMbpData;

  Status = GetMpbData(&IccMbpData);
  if (Status == EFI_SUCCESS) {
    Status = gRT->SetVariable (ICC_MBP_DATA_NAME,
                              &gIccDataGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS,
                              sizeof(ICC_MBP_DATA),
                              &IccMbpData);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "(ICC) Failed to write volatile configuration!  Status = %r\n", Status));
      //
      // Install gIccDataProtocol
      //
      mIccDataProtocol  =   AllocateZeroPool (sizeof (ICC_DATA_PROTOCOL));
      if (mIccDataProtocol != NULL) {
        mIccDataProtocol->GetMpbData = GetMpbData;
        Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle,
                                                         &gIccDataProtocolGuid,
                                                         mIccDataProtocol,
                                                         NULL
                                                        );
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "(ICC) Failed to install gIccDataProtocol, Status = %r\n", Status));
        }
      }
    }
  }

  return Status;
}

/**
Routine Description:
  Sends SetClockEnables Icc Heci message
Arguments:
  none
Returns:
  nothing
**/
VOID
SendLateIccMessages (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                      Status;
  UINT32                          MeMode;
  EFI_HECI_PROTOCOL               *Heci;
  UINTN                           Size;
  SYSTEM_CONFIGURATION            SetupData;
  UINT32                          ClkEnables=0xFFFFFFFF;
  UINT32                          ClkEnablesMask=0;

  Size  = sizeof (SYSTEM_CONFIGURATION);

  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
  ASSERT_EFI_ERROR(Status);

  Status = Heci->GetMeMode(&MeMode);
  ASSERT_EFI_ERROR(Status);

  Status = gRT->GetVariable (L"IntelSetup",  //AptioV server override
                             &gEfiSetupVariableGuid,
                             NULL,
                             &Size,
                             &SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to read SYSTEM_CONFIGURATION!  Status = %r\n", Status));
    return;
  }

  if (SetupData.IccSetClockEnables == OPTION_ENABLED) {
#ifndef GRANGEVILLE_FLAG
    DetectUsedClocks (&ClkEnables, &ClkEnablesMask);
#endif

    HeciSetIccClockEnables (ClkEnables, ClkEnablesMask, ICC_RESPONSE_MODE_SKIP);
  }

  gBS->CloseEvent(Event);
  return;

}

