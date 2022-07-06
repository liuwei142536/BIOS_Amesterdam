/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++
Copyright (c) 2006 - 2013, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  UpdateASL.c

Abstract:

  This code updates ASL code that supports SPS firmware in ME.

--*/
#include <PiDxe.h>
    
#include <Guid/GlobalVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/EventGroup.h>
#include <Guid/EventLegacyBios.h>
#include <Guid/HobList.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Guid/MeFwModesGuid.h>
#include <Guid/SpsAcpiTableLocatorGuid.h>
    
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/AcpiAml.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSupport.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/CpuIo.h>
#include <Protocol/MpService.h>
#include "Library/MeTypeLib.h"

// AptioV Server override Start: Program OEM ID and OEM table ID based on SDL token
#include <AcpiOemElinks.h>
UINT8 PLAFTORM_OEM_ID[6]     = ACPI_OEM_ID_MAK;
UINT8 PLAFTORM_OEM_TBL_ID[8] = ACPI_OEM_TBL_ID_MAK;
// AptioV Server override End: Program OEM ID and OEM table ID based on SDL token

/*****************************************************************************
 * Definitions
 *****************************************************************************/
#define AML_REGTYPE_SYSMEM 0
#define AML_REGTYPE_SYSIO  1
#define AML_REGTYPE_PCICFG 2
#define AML_REGTYPE_EMBEDD 3
#define AML_REGTYPE_SMBUS  4
#define AML_REGTYPE_CMOS   5
#define AML_REGTYPE_PCIBAR 6
#define AML_REGTYPE_IPMI   7
#define AML_REGTYPE_GPIO   8
#define AML_REGTYPE_GSBUS  9

#define AML_SIGNATURE_16(A, B)        ((A) | (B << 8))
#define AML_SIGNATURE_32(A, B, C, D)  (AML_SIGNATURE_16(A, B) | (AML_SIGNATURE_16(C, D) << 16))
#define AML_SIGNATURE_64(A, B, C, D, E, F, G, H) \
       (AML_SIGNATURE_32(A, B, C, D) | ((UINT64)(AML_SIGNATURE_32(E, F, G, H)) << 32))

#define SPS_ASL_SIG_NVS  AML_SIGNATURE_64('S', 'p', 's', 'N', 'v', 's', ' ', ' ')
#define SPS_ASL_SIG_NM   AML_SIGNATURE_64('S', 'p', 's', 'N', 'm', ' ', ' ', ' ')
#define SPS_ASL_SIG_PRAD AML_SIGNATURE_64('S', 'p', 's', 'P', 'r', 'A', 'g', 'g')

#define GetHeci1Bar() ((PciRead32(PCI_LIB_ADDRESS(0, 0x16, 0, 0x10)) & ~0xF) |\
                        (UINT64)PciRead32(PCI_LIB_ADDRESS(0, 0x16, 0, 0x14)) << 32)
#define GetHeci2Bar() ((PciRead32(PCI_LIB_ADDRESS(0, 0x16, 1, 0x10)) & ~0xF) |\
                        (UINT64)PciRead32(PCI_LIB_ADDRESS(0, 0x16, 1, 0x14)) << 32)

typedef struct _SPS_ME_BIOS_NVS_REGION
{
  UINT8  THNU;
  UINT8  H1UI;       // HECI-1 visible in user interface
  UINT8  H2UI;       // HECI-2 visible in user interface
  UINT8  OSLoad;                // 833
} SPS_MEBIOS_NVS_REGION;


/*****************************************************************************
 * Prototypes
 *****************************************************************************/


/*****************************************************************************
 * Variables
 *****************************************************************************/

SPS_MEBIOS_NVS_REGION  *pSpsNvs = NULL;


/*****************************************************************************
 * Functions
 *****************************************************************************/

/*****************************************************************************
 @brief
  Load AML files required to support SPS firmware.

  @param[in] TBD
  @param[in] TBD

  @return Standard status codes are returned.
**/
EFI_STATUS
SpsUpdateAslForMe(
    IN EFI_HANDLE               ImageHandle,
    IN EFI_SYSTEM_TABLE         *pSystemTable)
{
  BOOLEAN                       GotTableToPublish = FALSE;
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *pFwVolProtocol;
  EFI_HANDLE                    *pHandleBuffer;
  UINTN                         NumberOfHandles, ii;
  EFI_FV_FILETYPE               FileType;
  UINT32                        FvStatus;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINTN                         Size;
  INTN                          Instance;
//  EFI_ACPI_SUPPORT_PROTOCOL     *pAcpiProtocol = NULL;
  EFI_ACPI_TABLE_PROTOCOL       *pAcpiProtocol;
  EFI_ACPI_DESCRIPTION_HEADER   *pAcpiHeader;
  UINTN                         AcpiTableHandle;
  EFI_ACPI_TABLE_VERSION        Version;
  UINT32                        Signature;
  EFI_PHYSICAL_ADDRESS          AcpiNvsAddress;
  UINT8                         *pCurr;
  SYSTEM_CONFIGURATION          SysCfg;
  EFI_HOB_GUID_TYPE             *pHobGuid;
  EFI_HOB_TYPE_SPS_INFO         *pHobData;
  SPS_FEATURE_SET               FeatureSet;
  UINTN                         MpsThreads, MpsThreadsEnabled;
  EFI_MP_SERVICES_PROTOCOL      *pMpServices;
  EFI_GUID                      MpServicesGuid = EFI_MP_SERVICES_PROTOCOL_GUID;

  pHobGuid = GetFirstGuidHob(&gEfiSpsInfoHobGuid);
  pHobData = GET_GUID_HOB_DATA(pHobGuid);
  if (pHobData == NULL)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate SPS HOB\n"));
    FeatureSet.Data = 0;
  }
  else
  {
    FeatureSet.Data = pHobData->FeatureSet;
  }
  //
  // Get system configuration variable
  //
  Size = sizeof(SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable(L"IntelSetup", &gEfiSetupVariableGuid, NULL, &Size, &SysCfg);
  ASSERT_EFI_ERROR(Status);
  //
  // Get ACPI protocol, will need it later
  //
//  Status = gBS->LocateProtocol(&gEfiAcpiSupportProtocolGuid, NULL, &pAcpiProtocol);
  Status = gBS->LocateProtocol(&gEfiAcpiTableProtocolGuid, NULL, &pAcpiProtocol);
  ASSERT_EFI_ERROR(Status);
  
  AcpiNvsAddress = 0xFFFFFFFF; // Max 32-bit address limit for AllecatePages()
  Status = gBS->AllocatePages(AllocateMaxAddress, EfiACPIMemoryNVS,
                              EFI_SIZE_TO_PAGES(sizeof(*pSpsNvs)), &AcpiNvsAddress);
  ASSERT_EFI_ERROR(Status);
  
  pSpsNvs = (SPS_MEBIOS_NVS_REGION*)AcpiNvsAddress;
  ZeroMem(pSpsNvs, sizeof(*pSpsNvs));
  //
  // To get the number of enabled processor threads use MP Services protocol.
  //
  Status = gBS->LocateProtocol(&MpServicesGuid, NULL, &pMpServices);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate MP Services protocol (%r)\n", Status));
  }
  else
  {
    Status = pMpServices->GetNumberOfProcessors(pMpServices, &MpsThreads, &MpsThreadsEnabled);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: GetNumberOfProcessors failed (%r)\n", Status));
    }
    else
    {
      pSpsNvs->THNU = (UINT8)MpsThreadsEnabled;
    }
  }
  //
  // Locate  firmware volume handles.
  // There is little chance we can't find an FV protocol
  //
  Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiFirmwareVolume2ProtocolGuid, NULL,
                                   &NumberOfHandles, &pHandleBuffer);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,
           "[SPS] ERROR: Connot locate firmware volume handles (status: %r)\n", Status));
    return Status;
  }
  //
  // Looking for FV with SPS AML code file
  //
  pFwVolProtocol = NULL;
  for (ii = 0; ii < NumberOfHandles; ii++)
  {
    Status = gBS->HandleProtocol(pHandleBuffer[ii],
                                 &gEfiFirmwareVolume2ProtocolGuid, &pFwVolProtocol);
    if (!EFI_ERROR(Status))
    { //
      // See if it has the SPS AML file
      //
      Size      = 0;
      FvStatus  = 0;
      Status = pFwVolProtocol->ReadFile(pFwVolProtocol, &gEfiSpsAcpiTableLocatorGuid, NULL,
                                        &Size, &FileType, &Attributes, &FvStatus);
      //
      // If we found it, then we are done
      //
      if (!EFI_ERROR(Status))
      {
        break;
      }
    }
    else
    {
      ASSERT_EFI_ERROR(Status);
    }
  }
  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  gBS->FreePool(pHandleBuffer);
  
  if (pFwVolProtocol == NULL)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Connot locate SPS AML firmware volume\n"));
    return EFI_NOT_FOUND;
  }
  
  Version = EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;

  //
  // Read tables from the storage file.
  //
  for (Instance = 0, pAcpiHeader = NULL;
       !EFI_ERROR(pFwVolProtocol->ReadSection(pFwVolProtocol, &gEfiSpsAcpiTableLocatorGuid,
                                              EFI_SECTION_RAW, Instance,
                                              (VOID**)&pAcpiHeader, &Size, &FvStatus));
       Instance++, pAcpiHeader = NULL)
  {
//Grangeville AptioV server override start - CPP Check
    if( pAcpiHeader != NULL )
        {
    //
    // Check the table ID to modify the table
    //
    DEBUG((EFI_D_INFO, "[SPS] AML file '%c%c%c%c%c%c%c%c' found\n",
            ((pAcpiHeader->OemTableId >>  0) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >>  0) & 0xFF) : '.',
            ((pAcpiHeader->OemTableId >>  8) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >>  8) & 0xFF) : '.',
            ((pAcpiHeader->OemTableId >> 16) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 16) & 0xFF) : '.',
            ((pAcpiHeader->OemTableId >> 24) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 24) & 0xFF) : '.',
            ((pAcpiHeader->OemTableId >> 32) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 32) & 0xFF) : '.',
            ((pAcpiHeader->OemTableId >> 40) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 40) & 0xFF) : '.',
            ((pAcpiHeader->OemTableId >> 48) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 48) & 0xFF) : '.',
            ((pAcpiHeader->OemTableId >> 56) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 56) & 0xFF) : '.'));
    switch (pAcpiHeader->OemTableId)
    {
      case SPS_ASL_SIG_NVS:
        //
        // In this table need to update 'MEVS' region address with the actuall address
        //
        for (pCurr = (UINT8*)&pAcpiHeader[1];
             pCurr < (UINT8*)pAcpiHeader + pAcpiHeader->Length; pCurr++)
        {
          Signature = *(UINT32*)pCurr;
          if (Signature == AML_SIGNATURE_32('M', 'E', 'V', 'S'))
          {
            ASSERT(pCurr[4] == AML_REGTYPE_SYSMEM && pCurr[5] == AML_DWORD_PREFIX);
            DEBUG((EFI_D_INFO, "[SPS] Updating 'MEVS' address 0x%08X -> 0x%08X\n",
                   *(UINT32*)&pCurr[6], (UINT32)(INTN)pSpsNvs));
            *(UINT32*)&pCurr[6] = (UINT32)(INTN)pSpsNvs;
            break; // for (pCurr...)
          }
        }
        break; // switch ()
        
      case SPS_ASL_SIG_NM:
#if 0 // load unconditionally, need this file for system wide _WAK/_PTS compiled unconditionally
        if (!FeatureSet.Bits.NodeManager && !FeatureSet.Bits.DCMI)
        {
          goto SkippTable; // power limiting not enabled, do not load HECI AML code
        }
#endif
        //
        // In this table need to update 'H1CR' and 'H2CR' region address with the actuall address
        // In case PCIBARTarget was used (currently not supported by several OSes) do not modify.
        //
        for (pCurr = (UINT8*)&pAcpiHeader[1];
             pCurr < (UINT8*)pAcpiHeader + pAcpiHeader->Length; pCurr++)
        {
          Signature = *(UINT32*)pCurr;
          if (Signature == AML_SIGNATURE_32('H', '1', 'C', 'S') && pCurr[4] == AML_REGTYPE_SYSMEM)
          {
            ASSERT(pCurr[5] == AML_QWORD_PREFIX);
            DEBUG((EFI_D_INFO, "[SPS] Updating 'H1CR' address 0x%08X%08X -> 0x%08X%08X\n",
                   *(UINT32*)&pCurr[10], *(UINT32*)&pCurr[6],
                   (UINT32)(GetHeci1Bar() >> 32), (UINT32)GetHeci1Bar()));
            *(UINT64*)&pCurr[6] = GetHeci1Bar();
          }
          if (Signature == AML_SIGNATURE_32('H', '2', 'C', 'S') && pCurr[4] == AML_REGTYPE_SYSMEM)
          {
            ASSERT(pCurr[5] == AML_QWORD_PREFIX);
            DEBUG((EFI_D_INFO, "[SPS] Updating 'H2CR' address 0x%08X%08X -> 0x%08X%08X\n",
                   *(UINT32*)&pCurr[10], *(UINT32*)&pCurr[6],
                   (UINT32)(GetHeci2Bar() >> 32), (UINT32)GetHeci2Bar()));
            *(UINT64*)&pCurr[6] = GetHeci2Bar();
          }
        }
        break;
        
      case SPS_ASL_SIG_PRAD:
#if 0 // load unconditionally, need this file for system wide _WAK/_PTS compiled unconditionally
        if (!FeatureSet.Bits.NodeManager)
        {
          goto SkippTable; // Node Manager not enabled, do not load processor aggregator AML code
        }
#endif

		//
		// Update OEM table ID for PRAD table
		//
        CopyMem ((UINT8 *)&pAcpiHeader->OemTableId, PLAFTORM_OEM_TBL_ID, 8); // AptioV Server override: Program OEM ID and OEM table ID based on SDL token

        break;
        
      default:
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: Unknown AML file '%c%c%c%c%c%c%c%c' found\n",
               ((pAcpiHeader->OemTableId >>  0) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >>  0) & 0xFF) : '.',
               ((pAcpiHeader->OemTableId >>  8) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >>  8) & 0xFF) : '.',
               ((pAcpiHeader->OemTableId >> 16) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 16) & 0xFF) : '.',
               ((pAcpiHeader->OemTableId >> 24) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 24) & 0xFF) : '.',
               ((pAcpiHeader->OemTableId >> 32) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 32) & 0xFF) : '.',
               ((pAcpiHeader->OemTableId >> 40) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 40) & 0xFF) : '.',
               ((pAcpiHeader->OemTableId >> 48) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 48) & 0xFF) : '.',
               ((pAcpiHeader->OemTableId >> 56) & 0xFF) >= ' ' ? ((pAcpiHeader->OemTableId >> 56) & 0xFF) : '.'));
        goto SkippTable; // do not load unknown file
    }
    AcpiTableHandle = 0;
    DEBUG((EFI_D_INFO, "[SPS] Registering AML file in ACPI\n"));

    //
	// Update OEM ID for all tables
	//
    CopyMem ((UINT8 *)&pAcpiHeader->OemId, PLAFTORM_OEM_ID, 6); // AptioV Server override Start: Program OEM ID and OEM table ID based on SDL token

//    Status = pAcpiProtocol->SetAcpiTable(pAcpiProtocol, pAcpiHeader, TRUE, Version, &AcpiTableHandle);
    Status = pAcpiProtocol->InstallAcpiTable(pAcpiProtocol, pAcpiHeader, Size, &AcpiTableHandle);
    ASSERT_EFI_ERROR(Status);
    GotTableToPublish = TRUE;
    }
 //Grangeville AptioV server override end - CPP Check
   SkippTable:
    gBS->FreePool(pAcpiHeader);
  } // for (Instance...)
  if (GotTableToPublish)
  {
    DEBUG((EFI_D_INFO, "[SPS] Publishing ACPI tables\n"));
//    Status = pAcpiProtocol->PublishTables(pAcpiProtocol, Version);
  }
  else
  {
    DEBUG((EFI_D_INFO, "[SPS] ERROR: No SPS ACPI table found\n"));
  }
  return EFI_SUCCESS;
} // SpsUpdateAslForMe()


