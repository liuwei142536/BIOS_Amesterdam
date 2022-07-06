/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2005 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  Recovery.c
   
Abstract:

  Tiano PEIM to provide the platform recoveyr functionality.

--*/
#include <Token.h> // AptioV server override
#include "PlatformEarlyInit.h"
#include <Setup/IioUniversalData.h>

#include <Ppi/BlockIo.h>
#include <Ppi/PchPlatformPolicy.h> //  AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new 
//
// Capsule Types supported in this platform module
//
#include <Guid/CapsuleOnFatFloppyDisk.h>
#include <Guid/CapsuleOnFatIdeDisk.h>
#include <Guid/CapsuleOnFatUsbDisk.h>
#include <Guid/CapsuleOnDataCD.h>
#include <Ppi/Cache.h>

#include <Ppi/RecoveryModule.h>
#include <Ppi/DeviceRecoveryModule.h>
#include <Ppi/PchUsbPolicy.h>
#include <Ppi/PchInit.h>

#include <Library/PeiServicesLib.h>

//
// Required Service
//
EFI_STATUS
EFIAPI
PlatformRecoveryModule (
  IN EFI_PEI_SERVICES                       **PeiServices,
  IN EFI_PEI_RECOVERY_MODULE_PPI          *This
  );

VOID
AssertNoCapsulesError (
  IN EFI_PEI_SERVICES **PeiServices
  );

VOID
AssertMediaDeviceError (
  IN EFI_PEI_SERVICES **PeiServices
  );

VOID
ReportLoadCapsuleSuccess (
  IN EFI_PEI_SERVICES **PeiServices
  );

VOID
CheckIfMediaPresentOnBlockIoDevice (
  IN EFI_PEI_SERVICES   **PeiServices,
  IN OUT BOOLEAN        *MediaDeviceError,
  IN OUT BOOLEAN        *MediaPresent
  );

//
// Module globals
//
STATIC EFI_PEI_RECOVERY_MODULE_PPI  mRecoveryPpi = { PlatformRecoveryModule };

STATIC EFI_PEI_PPI_DESCRIPTOR         mRecoveryPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiRecoveryModulePpiGuid,
  &mRecoveryPpi
};

EFI_STATUS
EFIAPI
PeimInitializeRecovery (
  IN EFI_PEI_SERVICES     **PeiServices
  )
/*++

Routine Description:

  Provide the functionality of the Recovery Module.

Arguments:

  PeiServices  -  General purpose services available to every PEIM.
    
Returns:

  EFI_SUCCESS  -  If the interface could be successfully
                  installed.

--*/
{
  EFI_STATUS  Status;
// Aptio V Override : AMI Recovery Module is used
  //Status = PeiServicesInstallPpi (&mRecoveryPpiList);
  Status = EFI_SUCCESS;
  return Status;
}

EFI_STATUS
EFIAPI
PlatformRecoveryModule (
  IN EFI_PEI_SERVICES                       **PeiServices,
  IN EFI_PEI_RECOVERY_MODULE_PPI          *This
  )
/*++

Routine Description:

  Provide the functionality of the Ea Recovery Module.

Arguments:

  PeiServices  -  General purpose services available to every PEIM.
  This         -  Pointer to EFI_PEI_RECOVERY_MODULE_PPI.
  
Returns:

  EFI_SUCCESS      -  If the interface could be successfully
                      installed.
  EFI_UNSUPPORTED  -  Not supported.

--*/
{
  EFI_STATUS                            Status;
  EFI_PEI_DEVICE_RECOVERY_MODULE_PPI           *DeviceRecoveryModule;
  UINTN                                 NumberOfImageProviders;
  BOOLEAN                               ProviderAvailable;
  UINTN                                 NumberRecoveryCapsules;
  UINTN                                 RecoveryCapsuleSize;
  EFI_GUID                              DeviceId;
  BOOLEAN                               ImageFound;
  EFI_PHYSICAL_ADDRESS                  Address;
  VOID                                  *Buffer;
  EFI_CAPSULE_HEADER                    *CapsuleHeader;
  EFI_PEI_HOB_POINTERS                  Hob;
  EFI_PEI_HOB_POINTERS                  HobOld;
  EFI_HOB_CAPSULE_VOLUME                *CapsuleHob;
  BOOLEAN                               HobUpdate;
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader;
  UINTN                                 Index;

  static EFI_GUID                       mEfiCapsuleHeaderGuid = EFI_CAPSULE_GUID;

  Index = 0;

  Status                  = EFI_SUCCESS;
  HobUpdate               = FALSE;

  ProviderAvailable       = TRUE;
  ImageFound              = FALSE;
  NumberOfImageProviders  = 0;

  DeviceRecoveryModule    = NULL;

  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery Entry\n"));

  //
  // Search the platform for some recovery capsule if the DXE IPL
  // discovered a recovery condition and has requested a load.
  //
  while (ProviderAvailable) {

    Status = PeiServicesLocatePpi (
              &gEfiPeiDeviceRecoveryModulePpiGuid,
              Index,
              NULL,
              &DeviceRecoveryModule
              );

    if (!EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Device Recovery PPI located\n"));
      NumberOfImageProviders++;

      Status = DeviceRecoveryModule->GetNumberRecoveryCapsules (
                                      PeiServices,
                                      DeviceRecoveryModule,
                                      &NumberRecoveryCapsules
                                      );

      DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Number Of Recovery Capsules: %d\n", NumberRecoveryCapsules));

      if (NumberRecoveryCapsules == 0) {
        Index++;
      } else {
        break;
      }
    } else {
      ProviderAvailable = FALSE;
    }
  }
  //
  // The number of recovery capsules is 0.
  //
  if (!ProviderAvailable) {
    AssertNoCapsulesError (PeiServices);
  }
  //
  // If there is an image provider, get the capsule ID
  //
  if (ProviderAvailable) {
    RecoveryCapsuleSize = 0;

    Status = DeviceRecoveryModule->GetRecoveryCapsuleInfo (
                                    PeiServices,
                                    DeviceRecoveryModule,
                                    0,
                                    &RecoveryCapsuleSize,
                                    &DeviceId
                                    );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery Capsule Size: %d\n", RecoveryCapsuleSize));

    //
    // Only support the 2 capsule types known
    // Future enhancement is to rank-order the selection
    //
    if ((!CompareGuid (&DeviceId, &gPeiCapsuleOnFatIdeDiskGuid)) &&
        (!CompareGuid (&DeviceId, &gPeiCapsuleOnDataCDGuid)) &&
       (!CompareGuid (&DeviceId, &gPeiCapsuleOnFatUsbDiskGuid))
        ) {
      return EFI_UNSUPPORTED;
    }

    Buffer  = NULL;
    Address = (UINTN) AllocatePages ((RecoveryCapsuleSize - 1) / 0x1000 + 1);
    ASSERT (Address);

    Buffer = (UINT8 *) (UINTN) Address;

    Status = DeviceRecoveryModule->LoadRecoveryCapsule (
                                    PeiServices,
                                    DeviceRecoveryModule,
                                    0,
                                    Buffer
                                    );

    DEBUG ((EFI_D_INFO | EFI_D_LOAD, "LoadRecoveryCapsule Returns: %r\n", Status));

    if (Status == EFI_DEVICE_ERROR) {
      AssertMediaDeviceError (PeiServices);
    }

    if (EFI_ERROR (Status)) {
      return Status;
    } else {
      ReportLoadCapsuleSuccess (PeiServices);
    }
    //
    // Update FV Hob if found
    //
    Status      = PeiServicesGetHobList (&Hob.Raw);
    HobOld.Raw  = Hob.Raw;
    while (!END_OF_HOB_LIST (Hob)) {
      if (Hob.Header->HobType == EFI_HOB_TYPE_FV) {
        DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Hob FV Length: %x\n", Hob.FirmwareVolume->Length));
        //
        // BUGBUG Why is it a FV hob if it is greater than 0x50000?
        //
        if (Hob.FirmwareVolume->Length > 0x50000) {
          HobUpdate = TRUE;
          //
          // This looks like the Hob we are interested in
          //
          DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Hob Updated\n"));
          Hob.FirmwareVolume->BaseAddress = (UINTN) Buffer;
          Hob.FirmwareVolume->Length      = RecoveryCapsuleSize;
        }
      }

      Hob.Raw = GET_NEXT_HOB (Hob);
    }
    //
    // Check if the top of the file is a firmware volume header
    //
    FvHeader      = (EFI_FIRMWARE_VOLUME_HEADER *) Buffer;
    CapsuleHeader = (EFI_CAPSULE_HEADER *) Buffer;
    if (FvHeader->Signature == EFI_FVH_SIGNATURE) {
      //
      // build FV Hob if it is not built before
      //
      if (!HobUpdate) {
        DEBUG ((EFI_D_INFO | EFI_D_LOAD, "FV Hob is not found, Build FV Hob then..\n"));
        BuildFvHob (
          (UINTN) Buffer,
          FvHeader->FvLength
          );
        #if (PI_SPECIFICATION_VERSION >= 0x00010000)
          InstallFvInfoPpi(PeiServices, Buffer);
        #endif
      }
      //
      // Point to the location immediately after the FV.
      //
      CapsuleHeader = (EFI_CAPSULE_HEADER *) ((UINT8 *) Buffer + FvHeader->FvLength);
    }
    //
    // Check if pointer is still within the buffer
    //
    if ((UINTN) CapsuleHeader < (UINTN) ((UINT8 *) Buffer + RecoveryCapsuleSize)) {
      //
      // Check if it is a capsule
      //
      if (CompareGuid ((EFI_GUID *) CapsuleHeader, &mEfiCapsuleHeaderGuid)) {
        //
        // Set bootmode to capsule update so the capsule hob gets the right bootmode in the hob header.
        // Status = (**PeiServices).SetBootMode (PeiServices, BOOT_ON_FLASH_UPDATE);
        //
        // Build capsule hob
        //
        Status = PeiServicesCreateHob (
                  EFI_HOB_TYPE_CV,
                  sizeof (EFI_HOB_CAPSULE_VOLUME),
                  &CapsuleHob
                  );
        if (EFI_ERROR (Status)) {
          return Status;
        }
        CapsuleHob->BaseAddress = (UINT64)CapsuleHeader + 
                                  (UINT64)(EFI_CAPSULE_HEADER *)CapsuleHeader->HeaderSize;

        CapsuleHob->Length      = (UINT64)(EFI_CAPSULE_HEADER *)CapsuleHeader->CapsuleImageSize - 
                                  (UINT64)(EFI_CAPSULE_HEADER *)CapsuleHeader->HeaderSize;

        //
        // Find main FV within the capsule.
        // Not implemented yet.
        //
      }
    }
  }

  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery Module Returning: %r\n", Status));
  return Status;
}

/*
  AssertNoCapsulesError:
  There were no recovery capsules found. 
  Case 1: Report the error that no recovery block io device/media is readable and assert.  
  Case 2: Report the error that there is no media present on any recovery block io device and assert.
  Case 3: There is media present on some recovery block io device, 
          but there is no recovery capsule on it.  Report the error and assert.   
*/
VOID
AssertNoCapsulesError (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  BOOLEAN MediaDeviceError;
  BOOLEAN MediaPresent;

  MediaDeviceError  = TRUE;
  MediaPresent      = FALSE;

  CheckIfMediaPresentOnBlockIoDevice (PeiServices, &MediaDeviceError, &MediaPresent);
/*  if (MediaDeviceError) {
    ReportStatusCode (
      (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
      (EFI_PERIPHERAL_RECOVERY_MEDIA | EFI_P_EC_MEDIA_DEVICE_ERROR)
      );

  } else if (!MediaPresent) {
    ReportStatusCode (
      (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
      (EFI_PERIPHERAL_RECOVERY_MEDIA | EFI_P_EC_MEDIA_NOT_PRESENT)
      );

  } else {
    ReportStatusCode (
      (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
      (EFI_SOFTWARE_PEI_MODULE | EFI_SW_PEIM_EC_NO_RECOVERY_CAPSULE)
      );
  }*/
  //
  // Hang.
  //
  CpuDeadLoop();
}

#define MAX_BLOCK_IO_PPI  32

/*
  CheckIfMediaPresentOnBlockIoDevice: 
  Checks to see whether there was a media device error or to see if there is media present.  
*/
VOID
CheckIfMediaPresentOnBlockIoDevice (
  IN EFI_PEI_SERVICES   **PeiServices,
  IN OUT BOOLEAN        *MediaDeviceError,
  IN OUT BOOLEAN        *MediaPresent
  )
{
  EFI_STATUS                      Status;
  UINTN                           BlockIoPpiInstance;
  EFI_PEI_RECOVERY_BLOCK_IO_PPI *BlockIoPpi;
  UINTN                           NumberBlockDevices;
  EFI_PEI_BLOCK_IO_MEDIA          Media;

  *MediaDeviceError = TRUE;
  *MediaPresent     = FALSE;

  for (BlockIoPpiInstance = 0; BlockIoPpiInstance < MAX_BLOCK_IO_PPI; BlockIoPpiInstance++) {
    Status = PeiServicesLocatePpi (
              &gEfiPeiVirtualBlockIoPpiGuid,
              BlockIoPpiInstance,
              NULL,
              &BlockIoPpi
              );
    if (EFI_ERROR (Status)) {
      //
      // Done with all Block Io Ppis
      //
      break;
    }

    Status = BlockIoPpi->GetNumberOfBlockDevices (
                          PeiServices,
                          BlockIoPpi,
                          &NumberBlockDevices
                          );
    if (EFI_ERROR (Status) || (NumberBlockDevices == 0)) {
      continue;
    }
    //
    // Just retrieve the first block
    //
    Status = BlockIoPpi->GetBlockDeviceMediaInfo (
                          PeiServices,
                          BlockIoPpi,
                          0,
                          &Media
                          );
    if (!EFI_ERROR (Status)) {
      *MediaDeviceError = FALSE;
      if (Media.MediaPresent) {
        *MediaPresent = TRUE;
        break;
      }
    }
  }
}

VOID
AssertMediaDeviceError (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
/*  ReportStatusCode (
    (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
    (EFI_PERIPHERAL_RECOVERY_MEDIA | EFI_P_EC_MEDIA_DEVICE_ERROR)
    );
*/
  CpuDeadLoop ();
}

VOID
ReportLoadCapsuleSuccess (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  //
  // EFI_SW_PEI_PC_CAPSULE_START: (from the status code spec):
  // Loaded the recovery capsule.  About to hand off control to the capsule.
  //
/*  ReportStatusCode (
    EFI_PROGRESS_CODE,
    (EFI_SOFTWARE_PEI_MODULE | EFI_SW_PEIM_PC_CAPSULE_LOAD_SUCCESS)
    );*/
}

STATIC
VOID
GetIioUdsHob (
    IN EFI_PEI_SERVICES         **PeiServices,
    IN IIO_UDS                  **UdsHobPtr

  )
{
  EFI_GUID           UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_HOB_GUID_TYPE  *GuidHob;

  ASSERT(UdsHobPtr);

  *UdsHobPtr = NULL;
  
  GuidHob       = GetFirstGuidHob (&UniversalDataGuid);
  if (GuidHob){
    *UdsHobPtr  = GET_GUID_HOB_DATA (GuidHob);
    return;
  }

  ASSERT(FALSE);
}

#ifndef GRANGEVILLE_FLAG

STATIC PCH_USB_OVERCURRENT_PIN PlatformSRPOverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Zepher        ,OC0#                        
                                      PchUsbOverCurrentPin1,      //Port01: Read Connector,OC1#                        
                                      PchUsbOverCurrentPinSkip,   //Port02: User bay      ,OC0#
                                      PchUsbOverCurrentPinSkip,   //Port03: iBMC USB 1.1  ,no OCn#                     
                                      PchUsbOverCurrentPinSkip,   //Port04: NONE          ,no OCn#                                  
                                      PchUsbOverCurrentPin2,      //Port05: Read Connector,OC2#                       
                                      PchUsbOverCurrentPin1,      //Port06: Read Connector,OC1#                                              
                                      PchUsbOverCurrentPin2,      //Port07: Read Connector,OC2#                                              
                                      PchUsbOverCurrentPinSkip,   //Port08: NONE          ,no OCn#                                                         
                                      PchUsbOverCurrentPinSkip,   //Port09: NONE          ,no OCn#                                                         
                                      PchUsbOverCurrentPinSkip,   //Port10: iBMC USB 2.0  ,no OCn#                                          
                                      PchUsbOverCurrentPin4,      //Port11: Front Panel   ,OC4#                    
                                      PchUsbOverCurrentPinSkip,   //Port12: NONE          ,no OCn#                                  
                                      PchUsbOverCurrentPin4      //Port13: Front Panel   ,OC4#                    
                                   };   

STATIC PCH_USB_OVERCURRENT_PIN       MayanCityOverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                      PchUsbOverCurrentPinSkip,     // Port00: ssd
                                      PchUsbOverCurrentPin0,        // Port01: back panel
                                      PchUsbOverCurrentPin1,        // Port02: floppy like
                                      PchUsbOverCurrentPinSkip,     // Port03: bmc/internal
                                      PchUsbOverCurrentPin0,        // Port04: back panel
                                      PchUsbOverCurrentPin2,        // Port05: internal
                                      PchUsbOverCurrentPin3,        // Port06: internal
                                      PchUsbOverCurrentPin3,        // Port07: internal
                                      PchUsbOverCurrentPin4,        // Port08: internal
                                      PchUsbOverCurrentPin5,        // Port09: front panel
                                      PchUsbOverCurrentPin5,        // Port10: front panel
                                      PchUsbOverCurrentPin6,        // Port11: internal
                                      PchUsbOverCurrentPin6,        // Port12: internal
                                      PchUsbOverCurrentPinSkip      // Port13: bmc2/internal
                                   };                                          

  PCH_USB_OVERCURRENT_PIN            WildcatOverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin0,      //Port01: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin1,      //Port02: Internal 2x10, Debug_A          ,OC1#
                                      PchUsbOverCurrentPinSkip,   //Port03: Back Panel                      ,
                                      PchUsbOverCurrentPinSkip,   //Port04: BMC1.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPin2,      //Port05: Internal 2x5_A                  ,OC2#
                                      PchUsbOverCurrentPin2,      //Port06: Internal 2x5_B                  ,
                                      PchUsbOverCurrentPinSkip,   //Port07: Internal 2x5_b                  ,
                                      PchUsbOverCurrentPinSkip,   //Port08: eUSB                            ,no OCn#
                                      PchUsbOverCurrentPin6,      //Port09: Internal 2x5_A                  ,
                                      PchUsbOverCurrentPin5,      //Port10: Front Panel (J147 2x10)         ,OC5#
                                      PchUsbOverCurrentPinSkip,   //Port11: Int type A                      ,
                                      PchUsbOverCurrentPinSkip,   //Port12: BMC2.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPin5       //Port13: Front Panel                     ,OC5#
                                   }; 


STATIC PCH_USB_OVERCURRENT_PIN       AztecCityOverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                      PchUsbOverCurrentPin4,        // Port00: back panel
                                      PchUsbOverCurrentPin1,        // Port01: magjck/back panel/Clarkville
                                      PchUsbOverCurrentPinSkip,     // Port02: not used
                                      PchUsbOverCurrentPinSkip,     // Port03: not used
                                      PchUsbOverCurrentPin4,        // Port04: back panel
                                      PchUsbOverCurrentPin1,        // Port05: magjack/back panel/Springville
                                      PchUsbOverCurrentPin2,        // Port06: magjack/back panel/Clarkville
                                      PchUsbOverCurrentPin1,        // Port07: magjack/back panel/Springville
                                      PchUsbOverCurrentPinSkip,     // Port08: internal
                                      PchUsbOverCurrentPin0,        // Port09: internal
                                      PchUsbOverCurrentPinSkip,     // Port10: not used
                                      PchUsbOverCurrentPin4,        // Port11: front panel
                                      PchUsbOverCurrentPin0,        // Port12: internal
                                      PchUsbOverCurrentPin4         // Port13: internal
                                   };                                          

#else

STATIC PCH_USB_OVERCURRENT_PIN      DurangoOverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                      PchUsbOverCurrentPin0,        // Port00: Bottom Port
                                      PchUsbOverCurrentPin0,        // Port01: Top Port
                                      PchUsbOverCurrentPin1,        // Port02: Bottom Port
                                      PchUsbOverCurrentPin1,        // Port03: Top Port
                                      PchUsbOverCurrentPin2,        // Port04: Bottom Port
                                      PchUsbOverCurrentPin2,        // Port05: Top Port
                                      PchUsbOverCurrentPinSkip,     // Port06: not used
                                      PchUsbOverCurrentPinSkip,     // Port07: not used
                                      PchUsbOverCurrentPinSkip,     // Port08: not used
                                      PchUsbOverCurrentPinSkip,     // Port09: not used
                                      PchUsbOverCurrentPinSkip,     // Port10: not used
                                      PchUsbOverCurrentPinSkip,     // Port11: not used
                                      PchUsbOverCurrentPinSkip,     // Port12: not used
                                      PchUsbOverCurrentPinSkip      // Port13: not used
                                   };                

STATIC PCH_USB_OVERCURRENT_PIN      BeverlyCoveOverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                      PchUsbOverCurrentPinSkip,     // Port00: Bottom Port
                                      PchUsbOverCurrentPinSkip,     // Port01: Top Port
                                      PchUsbOverCurrentPinSkip,     // Port02: Bottom Port
                                      PchUsbOverCurrentPinSkip,     // Port03: Top Port
                                      PchUsbOverCurrentPinSkip,     // Port04: Bottom Port
                                      PchUsbOverCurrentPinSkip,     // Port05: Top Port
                                      PchUsbOverCurrentPinSkip,     // Port06: not used
                                      PchUsbOverCurrentPinSkip,     // Port07: not used
                                      PchUsbOverCurrentPinSkip,     // Port08: not used
                                      PchUsbOverCurrentPinSkip,     // Port09: not used
                                      PchUsbOverCurrentPinSkip,     // Port10: not used
                                      PchUsbOverCurrentPinSkip,     // Port11: not used
                                      PchUsbOverCurrentPinSkip,     // Port12: not used
                                      PchUsbOverCurrentPinSkip      // Port13: not used
                                   };     


#endif

STATIC UINT16 PlatformSRPPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x48,     //Port00: Zepher        ,make it biggest amplitude
                                      0x142,    //Port01: Read Connector,
                                      0x20,     //Port02: User bay      ,make it biggest amplitude
                                      0x90,     //Port03: iBMC USB 1.1  ,
                                      0x00,     //Port04: NONE          ,make it smallest amplitude
                                      0x149,    //Port05: Read Connector,
                                      0x136,    //Port06: Read Connector,
                                      0x148,    //Port07: Read Connector,
                                      0x00,     //Port08: NONE          ,make it smallest amplitude
                                      0x00,     //Port09: NONE          ,make it smallest amplitude
                                      0x81,     //Port10: iBMC USB 2.0  ,
                                      0x59,     //Port11: Front Panel   ,
                                      0x00,     //Port12: NONE          ,make it smallest amplitude
                                      0x56      //Port13: Front Panel   ,
                                   }; 

STATIC UINT16 WildcatPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x48,     //Port00: SSD (J2H1, mini2x5)             ,
                                      0x142,    //Port01: Back Panel(J230 1st of 3stack)  ,
                                      0x20,     //Port02: Floppy (J284)                   ,
                                      0x90,     //Port03: BMC1.0/Internal(J2 2x5)         ,
                                      0x00,     //Port04: Back Panel (J230 2nd of 3Stack) ,
                                      0x149,    //Port05: Back Panel (J230 3rd of 3Stack) ,
                                      0x136,    //Port06: Internal (J1C3 2x5)             ,
                                      0x148,    //Port07: Internal (J1C3 2x5)             ,
                                      0x00,     //Port08: Internal (J229 2x10)            ,
                                      0x00,     //Port09: Front Panel (J147 2x10)         ,
                                      0x81,     //Port10: Front Panel (J147 2x10)         ,
                                      0x59,     //Port11: Internal (J1 2x5)               ,
                                      0x00,     //Port12: Internal (J1 2x5)               ,
                                      0x56      //Port13: BMC2.0/Internal (J2 2x5)        ,
                                   };



#ifdef GRANGEVILLE_FLAG


STATIC UINT16 BeverlyCoveCRBPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x238,     //Not used
                                      0x159,     //Port01: Debug port        ,15.907"
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x101      //Not used
                                   };

#endif




EFI_STATUS
PlatformPchUsbInit (
  IN EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers

Arguments:

  PeiServices   General purpose services available to every PEIM.
  
Returns:
  
  EFI_SUCCESS   Operation success.

--*/
{
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *PeiPchUsbPolicyPpiDesc;
  PCH_USB_POLICY_PPI        *PeiPchUsbPolicyPpi;
  PCH_PLATFORM_POLICY_PPI *PchPlatformPolicyPpi;//  AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new 
//  PCH_USB_CONFIG            *UsbConfig; //  AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new 
  PCH_INIT_PPI              *PchPeiInitPpi;
//  UINTN                     Index;                 AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new 
  IIO_UDS                   *IioUds;
    /*
    AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new  
  EFI_HOB_GUID_TYPE         *GuidHob;
  EFI_PLATFORM_INFO         *PlatformInfo;
  VOID                      *OverCurrentMappings = NULL;
  UINTN                     OverCurrentMappingsSize = 0;
  VOID                      *PortLength = NULL;
  UINTN                     PortLengthSize = 0;
  
   */

  //
  // Locate PchPeiInit Ppi
  //
  Status = (**PeiServices).LocatePpi (PeiServices, &gPchInitPpiGuid, 0, NULL, &PchPeiInitPpi);
  ASSERT_EFI_ERROR(Status);
  
 /*
    AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new  -START
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
    PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
    ASSERT (PlatformInfo);
    AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new  - END
    
 */
  
  //  AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new  -START/////
   Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gPchPlatformPolicyPpiGuid,
                            0,
                            NULL,
                            (VOID **)&PchPlatformPolicyPpi
                            );
   //  AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new  - END////////
  

  //
  // Allocate descriptor and PPI structures.  Since these are dynamically updated
  // we cannot do a global variable PPI.
  //
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (EFI_PEI_PPI_DESCRIPTOR), &PeiPchUsbPolicyPpiDesc);
  ASSERT_EFI_ERROR(Status);

  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (PCH_USB_POLICY_PPI), &PeiPchUsbPolicyPpi);
  ASSERT_EFI_ERROR(Status);
/*   AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new
 
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (PCH_USB_CONFIG), &UsbConfig);
  ASSERT_EFI_ERROR(Status);
  
*/
  
  //
  // Initiate PCH USB policy.
  //
  PeiPchUsbPolicyPpi->Revision        = PCH_USB_POLICY_PPI_REVISION_1;

  /*   AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new -START
   
   
  UsbConfig->UsbPerPortCtl = PCH_DEVICE_ENABLE;
  UsbConfig->Ehci1Usbr     = PCH_DEVICE_DISABLE;
  UsbConfig->Ehci2Usbr     = PCH_DEVICE_DISABLE;

  UsbConfig->Usb20Settings[0].Enable = PCH_DEVICE_ENABLE;
  UsbConfig->Usb20Settings[1].Enable = PCH_DEVICE_ENABLE;
  
  // AptioV server override start:  Disable XHCI support for recovery. Otherwise ports are not routed to EHCI and USB recovery doesn't work.During ATAPI recovery also
  // USB will not work.
  UsbConfig->Usb30Settings.Mode = PCH_XHCI_MODE_OFF;
  UsbConfig->Usb30Settings.PreBootSupport = PCH_DEVICE_DISABLE;
  UsbConfig->Usb30Settings.ManualMode = PCH_DEVICE_DISABLE;  
  //AptioV server override end  
  
  for (Index = 0; Index < LPTH_USB_MAX_PHYSICAL_PORTS; Index++) {
    UsbConfig->PortSettings[Index].Enable = PCH_DEVICE_ENABLE;
  }

  switch(PlatformInfo->BoardId) {
    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCityCRBDDR3:
    case TypeMayanCitySRPDDR3:
      OverCurrentMappings     = MayanCityOverCurrentMappings;
      PortLength              = PlatformSRPPortLength;
      OverCurrentMappingsSize = sizeof(MayanCityOverCurrentMappings);
      PortLengthSize          = sizeof(PlatformSRPPortLength);
      break;
    case TypeWildcatPass:
      OverCurrentMappings     = WildcatOverCurrentMappings;
      PortLength              = WildcatPortLength;
      OverCurrentMappingsSize = sizeof(WildcatOverCurrentMappings);
      PortLengthSize          = sizeof(WildcatPortLength);
      break;

    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:

    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
      OverCurrentMappings     = AztecCityOverCurrentMappings;
      PortLength              = PlatformSRPPortLength;
      OverCurrentMappingsSize = sizeof(AztecCityOverCurrentMappings);
      PortLengthSize          = sizeof(PlatformSRPPortLength);
      break;

    case TypeIncaCityCRB:
    case TypeIncaCitySRP:

    case TypeHalfWidthCitySRP:


    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:

    default:
      OverCurrentMappings     = PlatformSRPOverCurrentMappings;
      PortLength              = PlatformSRPPortLength;
      OverCurrentMappingsSize = sizeof(PlatformSRPOverCurrentMappings);
      PortLengthSize          = sizeof(PlatformSRPPortLength);

      DEBUG ((EFI_D_ERROR, "Unknown Platform Type Detected, Recovery from USB may fail\n"));
      ASSERT_EFI_ERROR (PlatformInfo->BoardId < TypePlatformMax);
      break;
  }

  ASSERT((OverCurrentMappings != NULL) && (OverCurrentMappingsSize != 0));
  ASSERT((PortLength != NULL) && (PortLengthSize != 0));

  CopyMem(&(UsbConfig->Usb20OverCurrentPins[0]), OverCurrentMappings, OverCurrentMappingsSize); 
  
  for(Index=0;Index<LPTH_USB_MAX_PHYSICAL_PORTS;Index++){
      UsbConfig->PortSettings[Index].Usb20PortLength = ((UINT16 *)PortLength)[Index];
  }


AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new -END   */
  
  PeiPchUsbPolicyPpi->Mode = EHCI_MODE;

  //
  // Highly platform specific, refer to PublishHobData() in OemMemoryQpiInit.c
  //
  GetIioUdsHob(PeiServices, &IioUds);
  PeiPchUsbPolicyPpi->EhciMemBaseAddr = (UINTN)IioUds->PlatformData.IIO_resource[0].PciResourceMem32Limit + 16*1024 + 1;
  DEBUG((EFI_D_ERROR, "EhciMemBaseAddr:%x\n", PeiPchUsbPolicyPpi->EhciMemBaseAddr));

  PeiPchUsbPolicyPpi->EhciMemLength = (UINT32) 0x400 * PchEhciControllerMax;

//  PeiPchUsbPolicyPpi->UsbConfig = UsbConfig;   // AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new

  PeiPchUsbPolicyPpi->UsbConfig =  PchPlatformPolicyPpi->UsbConfig;// AptioV Server Override - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new
  PeiPchUsbPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PeiPchUsbPolicyPpiDesc->Guid  = &gPchUsbPolicyPpiGuid;
  PeiPchUsbPolicyPpiDesc->Ppi   = PeiPchUsbPolicyPpi;
  
  //
  // Install PCH USB Policy PPI
  //
  Status = (**PeiServices).InstallPpi (PeiServices, PeiPchUsbPolicyPpiDesc);
  ASSERT_EFI_ERROR(Status);
  
  //
  // Init PCH USB
  //
  DEBUG((EFI_D_ERROR, "Recovery: Begin to Init PCH USB\n"));

  Status = PchPeiInitPpi->UsbInit(PeiServices);
  ASSERT_EFI_ERROR(Status);

  return Status;
}

