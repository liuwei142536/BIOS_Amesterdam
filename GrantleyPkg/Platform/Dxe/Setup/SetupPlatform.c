/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SetupPlatform.c

Abstract:

  Platform Initialization Driver.

Revision History:

--*/
#include <Token.h> // AptioV server override
#include "SetupPlatform.h"
#include <Setup/IioUniversalData.h>
#include <Setup/IioUdsDefaults.h>
#include <Setup/IioSetupDefaults.h>
//#include <Library/CmosAccessLib.h> // AptioV Server Override: Removed the Intel CMOSACCESSLIB and USING AMICMOS MANAGER
#include "Platform.h"
#include "Library/MeTypeLib.h"
#if defined(ICC_SUPPORT) && ICC_SUPPORT
#include "IccCallbacks.h"
#include "IccSetup.h"
extern BOOLEAN                  gIccReset;
#endif // ICC_SUPPORT
#if defined(AMT_SUPPORT) && AMT_SUPPORT
extern BOOLEAN                  gAmtResetRequests;
#endif // AMT_SUPPORT
//extern EFI_GUID gEfiSecureBootEnableDisableGuid; AptioV server override

//EFI_GUID                        mErrorManagerGuid        = ERROR_MANAGER_GUID; // AptioV Server Override: Unused variables are commented.
EFI_GUID                        mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;
EFI_GUID                        mAmiTseSetupEnterProtocolGuid = AMITSE_SETUP_ENTER_GUID;
EFI_GUID                        mEfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
SYSTEM_CONFIGURATION            mSystemDefaults;
SYSTEM_CONFIGURATION            mSystemConfiguration;
//AptioV server override start: Setup power good reset support
SYSTEM_CONFIGURATION            gOldSystemConfiguration;
//AptioV server override end: Setup power good reset support

EFI_HII_HANDLE                  mAdvancedHiiHandle = NULL;
EFI_HII_HANDLE                  mBootOptionsHiiHandle = NULL;
//EFI_HII_HANDLE                  mErrorManagerHiiHandle = NULL;  // AptioV Server Override: Unused variables are commented.
EFI_HII_HANDLE                  mMainHiiHandle = NULL;
//EFI_HII_HANDLE                  mMaintenanceHiiHandle = NULL;   // AptioV Server Override: Unused variables are commented.
EFI_HII_HANDLE                  mSecurityHiiHandle = NULL;

EFI_HANDLE                      mAdvancedDriverHandle = NULL;
EFI_HANDLE                      mBootOptionsDriverHandle = NULL;
//EFI_HANDLE                      mErrorManagerDriverHandle = NULL;   // AptioV Server Override: Unused variables are commented.
EFI_HANDLE                      mMainDriverHandle = NULL;
//EFI_HANDLE                      mSecurityDriverHandle = NULL; // AptioV Server Override: Unused variables are commented.
EFI_HANDLE                      mImageHandle = NULL;

EFI_HII_DATABASE_PROTOCOL        *mHiiDataBase = NULL;
EFI_HII_CONFIG_ROUTING_PROTOCOL  *mHiiConfigRouting = NULL;
// AptioV Server Override: Changes Variablename to IntelSetup
CHAR16      mVariableName[]     = L"IntelSetup";
CHAR16      mVarStoreName[]     = L"IntelSetup";
//CHAR16      mTempVariableName[] = L"SetupTemp";   //AptioV server override: Setup power good reset support
UINT8       gPwFlag         = VALIDATE_NONE;
UINTN       gRow,gColumn;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mMainConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mAdvancedConfigAccess;
/* AptioV Server Override Start: Unused variables are commented.
EFI_HII_CONFIG_ACCESS_PROTOCOL  mIohConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mEventLogConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mSecurityConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mBootOptionsConfigAccess;
 AptioV Server Override End: Unused variables are commented.*/

EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *mPciRootBridgeIo;
EFI_PLATFORM_TYPE_PROTOCOL      *mPlatformType;

STATIC UINT8                    mSmbusRsvdAddresses[PLATFORM_NUM_SMBUS_RSVD_ADDRESSES] = {
  SMBUS_ADDR_CH_A_1,
  SMBUS_ADDR_CH_A_2,
  SMBUS_ADDR_CH_B_1,
  SMBUS_ADDR_CH_B_2
};

EFI_PLATFORM_POLICY_PROTOCOL    mPlatformPolicyData = {
  PLATFORM_NUM_SMBUS_RSVD_ADDRESSES,
  mSmbusRsvdAddresses
};

// Aptio V server override Start: Remove Ps2 platform code
//EFI_PS2_POLICY_PROTOCOL         mPs2PolicyData = {
//  EFI_KEYBOARD_NUMLOCK,
//  (EFI_PS2_INIT_HARDWARE) Ps2PortSwap
//};
// Aptio V server override End
DXE_PCH_PLATFORM_RESET_POLICY_PROTOCOL mPchPlatformResetPolicy = {
  PlatformResetCallback
};

EFI_BOOT_MODE       mBootMode;

VOID AmiPowerMgtCallback(IN OUT EFI_IFR_TYPE_VALUE *);	    //Aptio V Server Override
VOID UpdateNumCoresSetupQuestion(IN OUT EFI_HII_PACKAGE_HEADER *); //Aptio V Server Override
VOID SetNumCoresSetupQuestionHelpStr();  //Aptio V Server Override

EFI_STATUS
EFIAPI
XmpCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION Action,
  IN EFI_QUESTION_ID KeyValue,
  IN UINT8 Type,
  IN EFI_IFR_TYPE_VALUE *Value,
  OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest OPTIONAL
  );

VOID
UpdateIioDefaultConfigViaBoard (
   IN SYSTEM_CONFIGURATION *Default
   );

VOID
RestoreAdminPassword (
   IN SYSTEM_CONFIGURATION *Defaults
   );


//
// HII Vendor Device Path template for all package lists
// Differentiated by incrementing UniqueId
//
HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePathTemplate = {
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {
          (UINT8) (sizeof (HII_VENDOR_DEVICE_PATH_NODE)),
          (UINT8) ((sizeof (HII_VENDOR_DEVICE_PATH_NODE)) >> 8)
        }
      },
      //
      // {396C010D-219A-458a-BA7D-A54D092B4BE5}
      //
      { 0x396c010d, 0x219a, 0x458a, { 0xba, 0x7d, 0xa5, 0x4d, 0x9, 0x2b, 0x4b, 0xe5 } }
    },
    0
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};



EFI_STATUS
EFIAPI
SetupDriverEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
/*++

Routine Description:

  This is the standard EFI driver point for the Driver. This
  driver is responsible for setting up any platform specific policy or
  initialization information.

Arguments:

  ImageHandle  -  Handle for the image of this driver.
  SystemTable  -  Pointer to the EFI System Table.

Returns:

  EFI_SUCCESS  -  Policy decisions set.

--*/
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;
/* EDK_TODO
  EFI_HII_UPDATE_DATA     UpdateData;
*/
// Aptio V Server Override : Resolving the build error : Unreferenced Local variable
 // SYSTEM_CONFIGURATION    SetupData;
  UINTN                   SetupVariableSize=0;
//  BOOLEAN                 TpmPresent;
//  VOID                    *mEventReg;
//  EFI_EVENT               mEvent;
  UINTN                   BufferSize;
  UINT8                   Index;
  EFI_HANDLE              PchPlatformResetPolicyHandle;
//  UINT8                   *SecureBootEnable = NULL; // Aptio V Server Overrid
//  BOOLEAN                 SecureBoot  = FALSE; // Aptio V Server Overrid
  UINT8                   PfatSupportedPlatforms[]  = {
#ifndef GRANGEVILLE_FLAG
                              TypeMayanCitySRP,
                              TypeMayanCitySRPDDR3,
                              TypeAztecCityCRBSRP,
                              TypeAztecCitySTHI,
                              TypeHedtReversedIdBits,
                              TypeKahunaDVP2SPbg,
                              TypeKahunaDVP2S,
                              TypeHedtSKU1CRB,
                              TypeHedtEv,
                              TypeHedtDDR3
#else
                             TypeBeverlyCoveCRB,
                             TypeBeverlyCoveSODimmCRB
#endif
                             };
  UINT32                  BspCpuidSignature;
  UINT32                  RegEax, RegEbx, RegEcx, RegEdx;

//
// LegacySol_Common_start
//
// Aptio V server override - don't use legacy serial redirection here
//  SYSTEM_CONFIGURATION    SystemConfiguration;
//  UINTN                   VarSize;
//
// LegacySol_Common_end
//
  mImageHandle  = ImageHandle;
  BufferSize    = sizeof (SYSTEM_CONFIGURATION);

  //
  // Get current Boot Mode
  //
  mBootMode = GetBootModeHob ();

  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID**)&mHiiDataBase);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID**)&mHiiConfigRouting);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (
                  &gEfiPlatformTypeProtocolGuid,
                  NULL,
                  &mPlatformType
                  );
  ASSERT_EFI_ERROR (Status);

  // Aptio V Server Override - Start : Initializing mSystemConfiguration before using it.
  SetupVariableSize = sizeof(SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable(
              L"IntelSetup",
              &mSystemConfigurationGuid,
              NULL, 
              &SetupVariableSize,
              &mSystemConfiguration
              );
  // Aptio V Server Override - End : Initializing mSystemConfiguration before using it.

//  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
  ASSERT_EFI_ERROR (Status);
  
  // Aptio V Server Override - Start : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
  ParseIfrPackForIntelRcSetup(mImageHandle);
  // Aptio V Server override - End : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself. 

 // Aptio V Server Override - Start : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
/*
    InstallHiiDataAndGetSettings (
        (EFI_HII_PACKAGE_HEADER *) PlatformStrings,
// AptioV Server Override Start
//        (EFI_HII_PACKAGE_HEADER *) BootOptionsBin,
//        (EFI_HII_PACKAGE_HEADER *) SecurityBin,
        //(EFI_HII_PACKAGE_HEADER *) ErrorManagerBin,
        (EFI_HII_PACKAGE_HEADER *) IntelRCSetupBin,
//        (EFI_HII_PACKAGE_HEADER *) MainBin,
// AptioV Server Override End
        NULL
        );    
  }
  */
// Aptio V Server Override - End : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.  
  
  //
  // Setup local variable according to Setup variables
  //

  // Aptio V server override: Remove Ps2 platform code
/*
  if (!mSystemConfiguration.Numlock) {
    mPs2PolicyData.KeyboardLight &= ~EFI_KEYBOARD_NUMLOCK;
  }
*/
  Status = SetIdeMode ();

  //
  // Process Processor Setup
  //
  Status = ProcessorHtMode ();

  UpdateIioPresent (); // AptioV server override: Changes to update IIOxPresent setup variable

  //
  // Process Memory Setup
  //
  //  Status = MemorySetup();
  //
  // Process dynamic entries
  //
  Status = FindDataRecords ();


  //
  // Initialize PCH registers
  //
  InitDxePchPlatformPolicy(
     &mSystemConfiguration,
      mPlatformType
   );

  //
  // Control the count of Setup question PCI-E Slot Oprom.
  //
  SetupPcieSlotNumber (&mSystemConfiguration);

  //
  //Config the 4S SKU variable according to the Platform board ID
  //
  mSystemConfiguration.FourSocketsSKUFound = FALSE;

#ifndef GRANGEVILLE_FLAG
  if ((mPlatformType->BoardId == TypeArandasVVP4S) || (mPlatformType->BoardId == TypeIncaCityCRB))
  {
        //
        // If platform board id is 4S , then set the variable
        //
    	mSystemConfiguration.FourSocketsSKUFound = TRUE;
  }
#endif
  //
  //Config the Ipmi Supported SKU variable according to the Platform board ID
  //
   	

#ifdef GRANGEVILLE_FLAG
  //
  // Initialize IpmiSupportedPlatform to TRUE
  //
  mSystemConfiguration.IpmiSupportedPlatform = TRUE; 

#endif  


  //
  // Initialize PfatPlatformSupported to FALSE
  //
  mSystemConfiguration.PfatPlatformSupported = FALSE;

  //
  // Need to make sure that only supported Platforms can enable PFAT
  //
  for (Index = 0; Index < sizeof(PfatSupportedPlatforms); Index++) {
    if (mPlatformType->BoardId == PfatSupportedPlatforms[Index]) {
      mSystemConfiguration.PfatPlatformSupported = TRUE;
      break;
    }
  }

  //
  // Make sure PFAT is disabled on unsupported platforms
  //
  if (!mSystemConfiguration.PfatPlatformSupported) {
    mSystemConfiguration.PfatEnabled = FALSE;
  }

  mSystemConfiguration.AntiFlashWearoutSupported = FALSE;
  AsmCpuid (0x01, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  BspCpuidSignature = (RegEax >> 4) & 0x0000FFFF;
  if ( (BspCpuidSignature == CPU_FAMILY_BDX)) {
    mSystemConfiguration.AntiFlashWearoutSupported = TRUE;
  }

#ifndef GRANGEVILLE_FLAG

  if (mPlatformType->BoardId == TypeHedtSKU1CRB ||
      mPlatformType->BoardId == TypeHedtEv ||
      mPlatformType->BoardId == TypeHedtDDR3) {
    //
    // Overclocking support to be enabled.
    //
    mSystemConfiguration.PlatformOCSupport = 1;
	if (mBootMode == BOOT_WITH_FULL_CONFIGURATION) {
	  mSystemConfiguration.MctpEn = 0; //Force MctpEn=Disable for HEDT
	  DEBUG ((EFI_D_INFO, "Disable MCTP!!! \n"));
	}
  }else{
    //
    // Overclocking support to be disabled.
    //
    mSystemConfiguration.PlatformOCSupport = 0;
  }
#else
  mSystemConfiguration.PlatformOCSupport = 0;
#endif

  //
  // If BootMode is Manufacturing Mode, clear Admin and User passwords
  //
  if (mBootMode == BOOT_WITH_MFG_MODE_SETTINGS) {
    ZeroMem (&mSystemConfiguration.AdminPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16)); //Aptio V Server Override macro redefintion
    ZeroMem (&mSystemConfiguration.UserPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16)); //Aptio V Server Override macro redefintion
  }
#if defined(ICC_SUPPORT) && ICC_SUPPORT
  mSystemConfiguration.PlatformType = mPlatformType->BoardId;
#endif  
  Status = gRT->SetVariable(
                  mVariableName,
                  &mSystemConfigurationGuid,
  	              SETUP_VARIABLE_ATTRIBUTES,
                  sizeof (SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );

//AptioV server override start: Setup power good reset support.
  CopyMem (&gOldSystemConfiguration, &mSystemConfiguration, sizeof (SYSTEM_CONFIGURATION));
//AptioV server override end: Setup power good reset support

  SetPasswordState (&mAdminPassword);
  SetPasswordState (&mUserPassword);
  //
  // Install protocol to to allow access to this Policy.
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiPlatformPolicyProtocolGuid,
                  &mPlatformPolicyData,
                  // Aptio V server override: Remove Ps2 platform code,
 //                 &gEfiPs2PolicyProtocolGuid,
  //                &mPs2PolicyData,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Install platform reset protocol on a new handle.
  //
  PchPlatformResetPolicyHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &PchPlatformResetPolicyHandle,
                  &gDxePchPlatformResetPolicyProtocolGuid,
                  &mPchPlatformResetPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  // AptioV server override start. Boot options related code not required to be handled here.
 /*
  Status = GetVariable2 (L"SecureBootEnable", &gEfiSecureBootEnableDisableGuid, (VOID**)&SecureBootEnable, NULL);
  
  if ( !EFI_ERROR( Status ) )
    SecureBoot = *SecureBootEnable;
   if ( ( PcdGetBool (PcdUefiOptimizedBoot)) || ( SecureBoot ) ) {  

      //
      // Locate the setup variable
      //
      SetupVariableSize = sizeof(SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
      Status = gRT->GetVariable(L"IntelSetup",
                                &mSystemConfigurationGuid,
                                NULL,
                                &SetupVariableSize,
                                &SetupData);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "We can't get the setup variable!!!  \n"));
        ASSERT_EFI_ERROR (Status);
        //
        // By returning success we will still boot to the EFI shell, but not
        // provide CSM support which is not a requirement at this time.
        //
        return EFI_SUCCESS;
      }
     
    //
    // We now need to see if we must hack the INT10 vector for Windows UEFI.
    //
    if ( ( SetupData.EfiWindowsInt10Workaround == 0 ) || ( SecureBoot ) )
    {
      //
      // The UEFI Video ROM is dependent upon the following protocol.  If we install it, the UEFI
      // video ROM will take control based upon the EFI over Legacy priority
      //
      Status = gBS->InstallProtocolInterface (
        &Handle,
        &gEfiWindowsInt10Workaround,
        EFI_NATIVE_INTERFACE,
        NULL // Not required as we are just using this as a dummy marker
        );
    } else {
      //
      // Here we are doing an EFI boot, but need the legacy ROM.  As such we need to also signal
      // the legacy boot path.  This has the effect of allowing the legacy infrastructure to be
      // dispatched.  However, all selections of ROM priorities etc are based upon only the deep boot
      // marker
      //
      Handle = NULL;
      Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiLegacyBootMarkerGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL  // Not required as we are just using this as a dummy marker
                    );
    }
  } else {
    //
    // If we reached here, then we are not doing the setup option of EFI
    // Optimized boot.  As such we need to install the legacy BIOS marker protocol guid.
    //
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiLegacyBootMarkerGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL  // Not required as we are just using this as a dummy marker
                    );

  } 
 */
  // AptioV server override end. Boot options related code not required to be handled here.
 //
 // Register Notification for password check before entering into setup
 //
// AptioV server override. Password will be handled by AMI Security support.
/*  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL, 
                  TPL_CALLBACK, 
                  BackupSetupDataAndCheckForPassword, 
                  NULL, 
                  &mEvent);
  ASSERT_EFI_ERROR (Status);
 
   Status = gBS->RegisterProtocolNotify (
                   &mAmiTseSetupEnterProtocolGuid,
                   mEvent,
                   &mEventReg
                    );
   ASSERT_EFI_ERROR (Status);
*/
  // AptioV server override. IntelSetup is got from defaults.bin.
  //
  // Backup Setup variable to SetupDefault variable in first boot
  //
  /*Status  = gRT->GetVariable (
                  L"SetupDefault",
                  &gEfiSetupVariableGuid,
                  NULL,
                  &BufferSize,
                  &mSystemDefaults
                  );

  if(EFI_ERROR (Status)){
    DEBUG ((EFI_D_INFO, "SetupDefault is not found, create it.\n"));
    ZeroMem (&mSystemDefaults, sizeof (mSystemDefaults));

    Status  = gRT->GetVariable (
                    L"IntelSetup", //AptioV server override
                    &gEfiSetupVariableGuid,
                    NULL,
                    &BufferSize,
                    &mSystemDefaults
                    );
    ASSERT_EFI_ERROR (Status);

    Status  = gRT->SetVariable (
                    L"SetupDefault",
                    &gEfiSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(SYSTEM_CONFIGURATION),
                    &mSystemDefaults
                    );

    ASSERT_EFI_ERROR (Status);
  }*/
// AptioV server override - don't use Legacy serial redirection from Intel, use AMI module
/*
//
// LegacySol_Common_start
//
  VarSize = sizeof(SystemConfiguration);
  
  Status = gRT->GetVariable (
                      L"Setup",
                      &gEfiSetupVariableGuid,
                      NULL,
                      &VarSize,
                      &SystemConfiguration
                      );

                   
  if (EFI_ERROR (Status)) {
    PcdSet64 (PcdUartDefaultBaudRate, 115200);
    PcdSet8  (PcdUartDefaultDataBits, 8);
    PcdSet8  (PcdUartDefaultParity, 1);
    PcdSet8  (PcdUartDefaultStopBits, 1);
    PcdSet8  (PcdDefaultTerminalType, 0);
  } else {
    switch (SystemConfiguration.BaudRate) {
     case 1:
      PcdSet64 (PcdUartDefaultBaudRate, 9600); 
      break;

     case 2:
      PcdSet64 (PcdUartDefaultBaudRate, 19200); 
      break;
      
     case 3:
      PcdSet64 (PcdUartDefaultBaudRate, 38400);
      break;
      
     case 4:
      PcdSet64 (PcdUartDefaultBaudRate, 57600);
      break;
      
     case 5:
      PcdSet64 (PcdUartDefaultBaudRate, 115200);
      break;
      
     default:
      PcdSet64 (PcdUartDefaultBaudRate, 115200);
    }

    PcdSet8  (PcdUartDefaultDataBits, SystemConfiguration.DataBits); 
    PcdSet8  (PcdUartDefaultParity, SystemConfiguration.Parity); 
    PcdSet8  (PcdUartDefaultStopBits, SystemConfiguration.StopBits); 
    PcdSet8  (PcdDefaultTerminalType, SystemConfiguration.TerminalType);

  }              
//
// LegacySol_Common_end
//
*/
  //
  // TBD or do in other places.
  // 1. Clear BIS Credentials.  - Wait for Jan's fw protocol
  // 2. Chassis Intrusion, Date/Time, EventLog, Pnp O/S,      - Do we have to do this?
  // 3. Boot (BBS, USB, PXE, VGA) - Jim's code.
  //
  return EFI_SUCCESS;
}

STATIC
VOID
AccessExtendedCmos (
  IN      INT8                      Offset,
  IN      UINTN                     Size,
  IN OUT  VOID                      *Data
  )
{
  EFI_STATUS                        Status;
  EFI_CPU_IO_PROTOCOL               *CpuIo;
  BOOLEAN                           ReadCmos;

  Status = gBS->LocateProtocol (
                  &gEfiCpuIoProtocolGuid,
                  NULL,
                  &CpuIo
                  );
  ASSERT_EFI_ERROR (Status);

  if (Offset < 0) {
    ReadCmos = FALSE;
    Offset = -Offset;
  } else {
    ReadCmos = TRUE;
  }

  while (Size > 0) {
    //
    // Write to extended CMOS address register
    //
    CpuIo->Io.Write (CpuIo, EfiCpuIoWidthUint8, R_IOPORT_CMOS_UPPER_INDEX, 1, &Offset);

    //
    // Read/Write from/to the specified location
    //
    if (ReadCmos) {
      CpuIo->Io.Read (CpuIo, EfiCpuIoWidthUint8, R_IOPORT_CMOS_UPPER_DATA, 1, Data);
    } else {
      CpuIo->Io.Write (CpuIo, EfiCpuIoWidthUint8, R_IOPORT_CMOS_UPPER_DATA, 1, Data);
    }

    //
    // Advance to next element
    //
    Size--;
    Offset++;
    Data = (UINT8*)Data + 1;
  }
}

BOOLEAN
IsCMOSBad(
     VOID
)
{
  UINT8           Nmi;
  volatile UINT32 Data32;

  Nmi     = 0;
  Data32  = 0;
  //
  // Preserve NMI bit setting
  //
  MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32) B_PCH_RCRB_GCS_AME);
  Data32  = MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS);
  Nmi     = (IoRead8 (R_IOPORT_CMOS_STANDARD_INDEX) & 0x80);
  MmioAnd32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32)~B_PCH_RCRB_GCS_AME);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  if (IoRead8 (R_IOPORT_CMOS_STANDARD_DATA) & (BIT6 + BIT7)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

VOID
clearCmos0E()
{
  UINT8           Value;
  UINT8           Nmi;
  volatile UINT32 Data32;

  Nmi     = 0;
  Data32  = 0;
  //
  // Preserve NMI bit setting
  //
  MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32) B_PCH_RCRB_GCS_AME);
  Data32  = MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS);
  Nmi     = (IoRead8 (R_IOPORT_CMOS_STANDARD_INDEX) & 0x80);
  MmioAnd32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32)~B_PCH_RCRB_GCS_AME);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  Value = IoRead8 (R_IOPORT_CMOS_STANDARD_DATA);
  IoWrite8 (R_IOPORT_CMOS_STANDARD_DATA, Value & 0x3F);
}

STATIC
VOID
GetCsiMrcDefaultSettings (
   IN SYSTEM_CONFIGURATION *lpDefault
   )
{
  SETUP_INFO   SetupInfo;

  //
  // Get Default Values
  //
  gBS->CopyMem (
             &SetupInfo.Config,
             &IioConfigDefault,
             sizeof (IIO_CONFIG)
               );
  gBS->CopyMem (
             &SetupInfo.CsiConfig,
             &CsiConfigDefault,
             sizeof (CSI_CONFIG)
              );
  //AptioV server override start: Add missing default structure
  gBS->CopyMem (
              &SetupInfo.CommonRcConfig,
              &CommonRcConfigDefault,
              sizeof (COMMONRC_CONFIG)
              ); 
  //AptioV server override end: Add missing default structure  
  gBS->CopyMem (
             &SetupInfo.MemoryConfig,
             &MemoryConfigDefault,
             sizeof (MEMORY_CONFIG)
               );
  gBS->CopyMem (
             &SetupInfo.MiscConfig,
             &MiscConfigDefault,
             sizeof (MISC_CONFIG)
               );

  //
  // Additions for Grangeville / GrantleyRefresh would 
  // go here. Since this function is not currently
  // used, omitting for now.
  //

  gBS->CopyMem (
             &(lpDefault->VariableTag),
             &SetupInfo,
             sizeof (SETUP_INFO)
              );
}

VOID
UpdateIioDefaultConfigViaBoard (
   IN SYSTEM_CONFIGURATION *Default
   )
{
  //
  // Overclocking support preinitialized to disabled.
  //
  Default->PlatformOCSupport = 0;

  switch(mPlatformType->BoardId) {
#ifndef  GRANGEVILLE_FLAG
    //
    // Ensure HEDT sku's stay at the top so only those boards have the Overclocking
    // support enabled
    //
    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
        //
        // Overclocking support to be enabled.
        //
        Default->PlatformOCSupport = 1;

    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCityCRBDDR3:
    case TypeMayanCitySRPDDR3:
    case TypeWildcatPass:
    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
    case TypeHalfWidthCitySRP:


#endif
    default:
      DEBUG((EFI_D_ERROR, "platform type = %d\n", mPlatformType->BoardId));
      //
      // Do nothing since current default settings match SRP board
      //
      break;
    }

}


VOID
RestoreAdminPassword (
   IN SYSTEM_CONFIGURATION *Defaults
   )
/*++

Routine Description:

  Restore Admin password if it was saved in the HOB.

Arguments:
    Defaults - pointer to the structure of the configuration vars

Returns:

  None.

--*/
{

    EFI_HOB_GUID_TYPE       *GuidHob;
    UINT32                  DataSize;

    GuidHob = GetFirstGuidHob(&gEfiAdminPasswordHobGuid);
    if( GuidHob!= NULL) {
        DataSize = GET_GUID_HOB_DATA_SIZE(GuidHob);
        if(sizeof( Defaults->AdminPassword ) >= DataSize){
            CopyMem (
                &(Defaults->AdminPassword),
                GET_GUID_HOB_DATA( GuidHob ),
                DataSize
            );
        }else{
            DEBUG((EFI_D_ERROR, "AdminPassword HOB too large. Password isn't restored.\n"));
        }
    }
}

// AptioV Server Override - Start : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
EFI_STATUS
ParseIfrPackForIntelRcSetup(
IN EFI_HANDLE        ImageHandle
)
{
	EFI_STATUS							  Status = EFI_SUCCESS;
	EFI_HANDLE                            DriverHandle;
	EFI_HII_HANDLE                        *HiiHandle;
	EFI_GUID                              *FormsetGuid;
	HII_VENDOR_DEVICE_PATH_NODE           *VendorDevicePath;

	UINTN                                 BufferSize;
	BOOLEAN                               ActionFlag;
	BOOLEAN                               SetDefaultFlag;
	EFI_STRING                            ConfigRequestHdr;


	EFI_HII_PACKAGE_LIST_HEADER     	  *PackageList;
	EFI_HII_DATABASE_PROTOCOL             *HiiDatabase;
	UINT16                       		  Offset;
	EFI_IFR_OP_HEADER                     *IfrOpHdr;
// AptioV server override start: Changes done for EIP# 118513 
	EFI_FORM_ID							   				FormId = VFR_FORMID_INTELRCSETUP;
	BOOLEAN                   			  GetFormSet = FALSE;
	BOOLEAN                   			  GetForm = FALSE;
// AptioV server override end: Changes done for EIP# 118513 

	ZeroMem (&mSystemConfiguration, sizeof (mSystemConfiguration));
	ZeroMem (&mSystemDefaults, sizeof (mSystemDefaults));

	ConfigRequestHdr = NULL;
	ActionFlag       = FALSE;
	SetDefaultFlag   = FALSE;
	BufferSize       = sizeof (mSystemConfiguration);
	Status           = gRT->GetVariable (mVariableName, &gEfiSetupVariableGuid, NULL, &BufferSize, &mSystemConfiguration);
	if (EFI_ERROR (Status)) {
	    SetDefaultFlag = TRUE;
	    //
	    // Write the inital zero configuration, the default setting will be set in the following.
	    GetCsiMrcDefaultSettings(&mSystemDefaults);

	    UpdateIioDefaultConfigViaBoard(&mSystemDefaults);

    //
    // Prevent resetting admin password    
    //
    //RestoreAdminPassword(&mSystemDefaults); // AptioV server override: AMI TSE takes care of Passwords.
	    Status = gRT->SetVariable (
	                    			mVariableName,
	                    			&gEfiSetupVariableGuid,
	                    			EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
	                    			sizeof(SYSTEM_CONFIGURATION),
	                    			&mSystemDefaults
	                    		  );
	    ASSERT_EFI_ERROR (Status);
	  }

	Status = gBS->OpenProtocol (
								ImageHandle,
								&gEfiHiiPackageListProtocolGuid,
								(VOID **) &PackageList,
								ImageHandle,
								NULL,
								EFI_OPEN_PROTOCOL_GET_PROTOCOL
								);
	if (EFI_ERROR (Status)) {
	        return Status;
	}

	Status = gBS->LocateProtocol (
	        					&gEfiHiiDatabaseProtocolGuid,
	        					NULL,
	        					&HiiDatabase
	    						 );
	if (EFI_ERROR (Status)) {
	        return Status;
	}

	    //for(i=0; i<NumberOfIfrPackages; i++)
	{
		EFI_HII_PACKAGE_HEADER *IfrPackagePtr;

	    for (IfrPackagePtr = (EFI_HII_PACKAGE_HEADER*)(PackageList+1);
	         IfrPackagePtr < (EFI_HII_PACKAGE_HEADER*)((UINT8*)PackageList+PackageList->PackageLength);
	         IfrPackagePtr = (EFI_HII_PACKAGE_HEADER*)((UINT8*)IfrPackagePtr+IfrPackagePtr->Length)) {
	    	 if (IfrPackagePtr->Type == EFI_HII_PACKAGE_FORMS) {
	    		break;
	    	}
	    }

	    IfrOpHdr   = (EFI_IFR_OP_HEADER *)((UINT8 *) IfrPackagePtr + sizeof (EFI_HII_PACKAGE_HEADER));
// AptioV server override start: Changes done for EIP# 118513 
	    Offset     = sizeof (EFI_HII_PACKAGE_HEADER);
	    while (Offset < IfrPackagePtr->Length) {
	        if (IfrOpHdr->OpCode == EFI_IFR_FORM_SET_OP) {
	            if (CompareGuid((GUID *)(VOID *)&((EFI_IFR_FORM_SET *) IfrOpHdr)->Guid, &mSystemConfigurationGuid)) {
	                GetFormSet = TRUE;
	            }
	            else {
	                GetFormSet = FALSE;
	            }
	        }
	        else if (IfrOpHdr->OpCode == EFI_IFR_FORM_OP || IfrOpHdr->OpCode == EFI_IFR_FORM_MAP_OP) {
	    	    if (CompareMem (&((EFI_IFR_FORM *) IfrOpHdr)->FormId, &FormId, sizeof (EFI_FORM_ID)) == 0) {
	    		    GetForm = TRUE;
	    	    }
	    	    else {
	    		    GetForm = FALSE;
	    	    }
	    	}

	        if (GetFormSet && GetForm) {
// AptioV server override end: Changes done for EIP# 118513 
	    				  DEBUG((EFI_D_ERROR, "INTELRCSETUP page FOUND in IFRPACK \n"));
						  mHiiVendorDevicePathTemplate.Node.UniqueId++;
						  VendorDevicePath = AllocateCopyPool (sizeof (HII_VENDOR_DEVICE_PATH), &mHiiVendorDevicePathTemplate);
						  ASSERT (VendorDevicePath != NULL);

						  DriverHandle = NULL;
						  Status = gBS->InstallMultipleProtocolInterfaces (																	&DriverHandle,
																			&gEfiDevicePathProtocolGuid,
																			VendorDevicePath,
																			NULL
																		  );
						  ASSERT_EFI_ERROR (Status);
	    			 	  mAdvancedDriverHandle = DriverHandle;
	    			      FormsetGuid = &gAdvancedPkgListGuid;
	    			      HiiHandle = &mAdvancedHiiHandle;
	    			      InstallAdvancedCallbackRoutine (mAdvancedDriverHandle, &mAdvancedConfigAccess);
						  break;
	    	}
			//
			// Go to the next Op-Code
			//
			Offset   += IfrOpHdr->Length;
			IfrOpHdr = (EFI_IFR_OP_HEADER *) ((CHAR8 *) (IfrOpHdr) + IfrOpHdr->Length);
	    }

	    if (Offset >= IfrPackagePtr->Length)
	    {
	    	DEBUG((EFI_D_ERROR, "INTELRCSETUP page not found in IFRPACK \n"));
	    	ASSERT_EFI_ERROR (Status);
	    }

	     UpdateNumCoresSetupQuestion(IfrPackagePtr); //Aptio V Server Override - Fix up CoresToEnable to limit maximum number of cores.
    
		 Status = HiiDatabase->NewPackageList(HiiDatabase, PackageList, DriverHandle, HiiHandle);

	     SetNumCoresSetupQuestionHelpStr();  // Aptio V Server Override - Must be called after NewPackageList to init HiiHandle (mAdvancedHiiHandle).
	    
		 ConfigRequestHdr = HiiConstructConfigHdr (&mSystemConfigurationGuid, mVarStoreName, DriverHandle);
		 ASSERT (ConfigRequestHdr != NULL);
		 if (SetDefaultFlag) {
		       ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
		       ASSERT (ActionFlag);
		  } else if (!SetDefaultFlag) { 
#if INTELRCSETUP_HiiValidateSettings	
		 DEBUG((EFI_D_ERROR, "Validating INTELRCSETUP IFR options \n"));			 
		       ActionFlag = HiiValidateSettings (ConfigRequestHdr);
		       ASSERT (ActionFlag);
#endif	
		  }		       
		  FreePool (ConfigRequestHdr);
	}
	return Status;
}
// AptioV Server Override - End ; Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.

// AptioV Server Override - Start ; Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
// Entire routine commented out
#if 0
VOID
EFIAPI
InstallHiiDataAndGetSettings (
  IN EFI_HII_PACKAGE_HEADER      *StringPack,
  ...
  )
/*++

Routine Description:

  Installs a string and ifr pack set.

Arguments:

  Hii         -  pointer to an HII database protocol structure.
  StringPack  -  string pack to store in database and associate with IfrPack.
  IrfPack     -  ifr pack to store in database (it will use StringPack data).

Returns:

  None.

--*/
{
  EFI_STATUS                            Status;
  VA_LIST                               Args;
  EFI_HANDLE                            DriverHandle;
  EFI_HII_HANDLE                        *HiiHandle;
  EFI_HII_PACKAGE_HEADER                *IfrPack;
  EFI_GUID                              *FormsetGuid;
  HII_VENDOR_DEVICE_PATH_NODE           *VendorDevicePath;

  UINTN                           BufferSize;
  BOOLEAN                         ActionFlag;
  BOOLEAN                         SetDefaultFlag;
  EFI_STRING                      ConfigRequestHdr;
  BOOLEAN                         CMOSStatus;

  ZeroMem (&mSystemConfiguration, sizeof (mSystemConfiguration));
  ZeroMem (&mSystemDefaults, sizeof (mSystemDefaults));

  ConfigRequestHdr = NULL;
  ActionFlag       = FALSE;
  SetDefaultFlag   = FALSE;
  BufferSize       = sizeof (mSystemConfiguration);
  Status           = gRT->GetVariable (mVariableName, &gEfiSetupVariableGuid, NULL, &BufferSize, &mSystemConfiguration);
  CMOSStatus       = IsCMOSBad();
  if (EFI_ERROR (Status) || (CMOSStatus) || (mBootMode == BOOT_WITH_DEFAULT_SETTINGS) ) {

    SetDefaultFlag = TRUE;
	DEBUG ((EFI_D_ERROR, "Is CMOS Bad = %x\n", CMOSStatus));
    // If the Clear CMOS is set, reset it.
    if (PciRead8(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_3)) & B_PCH_LPC_GEN_PMCON_RTC_PWR_STS){
      PciAnd8 (
      PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN_PMCON_3),
      (UINT8)~B_PCH_LPC_GEN_PMCON_RTC_PWR_STS
      );
    }

	// If CMOS bits were set, we reset this:
	DEBUG ((EFI_D_INFO, "CMOS bad, force user to enter setup.\n"));
    clearCmos0E ();

    //
    // Restore Setup data from SetupDefault variable
    //
    Status  = gRT->GetVariable (
                    L"SetupDefault",
                    &gEfiSetupVariableGuid,
                    NULL,
                    &BufferSize,
                    &mSystemDefaults
                    );
                    
#ifndef  GRANGEVILLE_FLAG
    //
    // Skip restoring defaults for Aztec STHI - Defaults and current will be the same
    //
    if (!EFI_ERROR (Status) && (mPlatformType->BoardId != TypeAztecCitySTHI)) {
#else
    if (!EFI_ERROR (Status)) {
#endif

      UpdateIioDefaultConfigViaBoard(&mSystemDefaults);

      //
      // Prevent resetting admin password
      //
      RestoreAdminPassword(&mSystemDefaults);


      Status = gRT->SetVariable (
                    mVariableName,
                    &gEfiSetupVariableGuid,
		            SETUP_VARIABLE_ATTRIBUTES,
                    sizeof(SYSTEM_CONFIGURATION),
                    &mSystemDefaults
                    );
      ASSERT_EFI_ERROR (Status);
    }
  }


  HiiHandle = NULL;
  FormsetGuid = NULL;

  IfrPack = NULL;
  VA_START (Args, StringPack);
  while ((IfrPack = VA_ARG (Args, EFI_HII_PACKAGE_HEADER *)) != NULL) {
    //
    // Increment UniqueID to distinguish from different device paths
    //
    mHiiVendorDevicePathTemplate.Node.UniqueId++;

    VendorDevicePath = AllocateCopyPool (sizeof (HII_VENDOR_DEVICE_PATH), &mHiiVendorDevicePathTemplate);
    ASSERT (VendorDevicePath != NULL);

    DriverHandle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &DriverHandle,
                    &gEfiDevicePathProtocolGuid,
                    VendorDevicePath,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);

    if (IfrPack == (EFI_HII_PACKAGE_HEADER *) MainBin) {
      mMainDriverHandle = DriverHandle;
      FormsetGuid = &gMainPkgListGuid;
      HiiHandle = &mMainHiiHandle;
      InstallDefaultCallbackRoutine (mMainDriverHandle, &mMainConfigAccess);
    } else if (IfrPack == (EFI_HII_PACKAGE_HEADER *) AdvancedBin) {
      mAdvancedDriverHandle = DriverHandle;
      FormsetGuid = &gAdvancedPkgListGuid;
      HiiHandle = &mAdvancedHiiHandle;
      InstallAdvancedCallbackRoutine (mAdvancedDriverHandle, &mAdvancedConfigAccess);
    } else if (IfrPack == (EFI_HII_PACKAGE_HEADER *) BootOptionsBin) {
      mBootOptionsDriverHandle = DriverHandle;
      FormsetGuid = &gBootOptionsPkgListGuid;
      HiiHandle = &mBootOptionsHiiHandle;
      InstallDefaultCallbackRoutine (mBootOptionsDriverHandle, &mBootOptionsConfigAccess);
    } else if (IfrPack == (EFI_HII_PACKAGE_HEADER *) SecurityBin) {
      mSecurityDriverHandle = DriverHandle;
      FormsetGuid = &gSecurityPkgListGuid;
      HiiHandle = &mSecurityHiiHandle;
      InstallSecurityCallbackRoutine (mSecurityDriverHandle, &mSecurityConfigAccess);
    } else {
      /*
        if IfrPack is ExitBin, LanguageBin, etc:
        These Vfr/Ifr Bins will not contribute to the default image, mSystemDefaults.
        So we "continue".
      */
      continue;
    }

    *HiiHandle = HiiAddPackages (
                 FormsetGuid,
                 DriverHandle,
                 IfrPack,
                 StringPack,
                 NULL
                 );
    ASSERT (*HiiHandle != NULL);

    ConfigRequestHdr = HiiConstructConfigHdr (&mSystemConfigurationGuid, mVarStoreName, DriverHandle);
    ASSERT (ConfigRequestHdr != NULL);
    if (SetDefaultFlag) {
      ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
      ASSERT (ActionFlag);
    } else if (!SetDefaultFlag) {
//
// HSX 4167642: Long boot up time
// The below functionality consumes ~5 seconds. This is not mandatory
//
//      ActionFlag = HiiValidateSettings (ConfigRequestHdr);
//      ASSERT (ActionFlag);
    }
    FreePool (ConfigRequestHdr);
  }

  VA_END(Args);

  InitMeSetupStrings();
#if defined(ICC_SUPPORT) && ICC_SUPPORT
  if (MeTypeIsAmt())
  {
    InitIccHandles (mAdvancedHiiHandle);
  }
#endif // ICC_SUPPORT

/*
  UpdateIioDefaultConfigViaBoard(&mSystemDefaults);

  Status = ReadOrInitSetupVariable (
             &mSystemConfigurationGuid,
             sizeof (SYSTEM_CONFIGURATION),
             &mSystemDefaults,
             &mSystemConfiguration
             );
  ASSERT_EFI_ERROR(Status);
*/
}
#endif
// Aptio V Server Override - End ; Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.

EFI_STATUS
ReadOrInitSetupVariable (
  IN  EFI_GUID                  *VarGuid,
  IN  UINTN                     RequiredVariableSize,
  IN  VOID                      *DefaultData,
  OUT VOID                      *SetupVariableData
  )
/*++

Routine Description:
  Reads the setup configuration data for a driver.  If the variable doesn't
  exist, then it will be written with the data from DefaultData.

Arguments:

  VarGuid               -  The setup variable guid to read.
  RequiredVariableSize  -  The size that the variable.  If the variable
                           read is not this size, then the variable will be treated as invalid.
                           SetupVariableData is assumed to be large enough to hold
                           RequiredVariableSize bytes.  If DefaultData != NULL, then it is
                           assumed to be RequiredVariableSize bytes.
  DefaultData           -  If the variable is not found, or invalid, then this buffer
                           will be copied into SetupVariableData.
                           This provides a default configuration.
  SetupVariableData     -  The variable will be copied into this buffer.  If the
                           variable is not found, or is invalid, then DefaultData will be copied.

Returns:

  EFI_SUCCESS  -  The variable was read and successfully copied to
                  SetupVariableData.

--*/
{
  EFI_STATUS  Status;
  UINTN       VarSize;
  UINT32	  Attributes;
  VOID        *Buffer;

  VarSize = RequiredVariableSize;
  Buffer  = AllocateZeroPool (VarSize);

  Status  = gRT->GetVariable (mVariableName, VarGuid, &Attributes, &VarSize, Buffer);

  if (!EFI_ERROR (Status)) {
    if (VarSize != RequiredVariableSize) {
      Status = EFI_NOT_FOUND;
    }
  }

  if (!EFI_ERROR (Status)) {
    DefaultData = Buffer;
  } else {
    Status  =  gRT->SetVariable (
                mVariableName,
                VarGuid,
                Attributes,
                RequiredVariableSize,
                DefaultData
                );
  }

  gBS->CopyMem (SetupVariableData, DefaultData, RequiredVariableSize);
  gBS->FreePool (Buffer);

  return Status;
}


EFI_STATUS
EFIAPI
DefaultExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  EFI_HANDLE                       Handle;
  EFI_STRING                       ConfigRequestHdr;
  EFI_STRING                       ConfigRequest;
  UINTN                            Size;
  BOOLEAN                          AllocatedRequest;

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }


  *Progress        = Request;
  //
  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  //
  if ((Request != NULL) && !HiiIsConfigHdrMatch (Request, &mSystemConfigurationGuid, mVarStoreName)) {
    return EFI_NOT_FOUND;
  }

  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  Size             = 0;
  AllocatedRequest = FALSE;

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
            mVariableName,
            &mSystemConfigurationGuid,
            NULL,
            &BufferSize,
            &mSystemConfiguration
            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mSystemConfiguration.PchLanSupported = mPlatformType->PchData.PchLanSupported;

  //
  // synchronize PcdUefiOptimizedBoot and UefiOptimizedBoot
  //
  //mSystemConfiguration.UefiOptimizedBoot = (UINT8) (PcdGetBool (PcdUefiOptimizedBoot));   // AptioV server override. Boot options related code not required to be handled here.
  
  ConfigRequest = Request;
  if (Request == NULL || (StrStr (Request, L"OFFSET") == NULL)) {
    //
    // Return all configurable elements
    //
    if (This == &mAdvancedConfigAccess) {
      Handle = mAdvancedDriverHandle;
    } else if (This == &mMainConfigAccess) {
      Handle = mMainDriverHandle;
    } else {
      return EFI_NOT_FOUND;
    }

    //
    // Request is set to NULL, construct full request string.
    //

    //
    // First Set ConfigRequestHdr string.
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&mSystemConfigurationGuid, mVarStoreName, Handle);
    ASSERT (ConfigRequestHdr != NULL);

    //
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    ASSERT (ConfigRequest != NULL);
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
  }

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = mHiiConfigRouting->BlockToConfig (
                                mHiiConfigRouting,
                                ConfigRequest,
                                (UINT8 *) &mSystemConfiguration,
                                BufferSize,
                                Results,
                                Progress
                                );

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }
  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = (EFI_STRING)(Request + StrLen (Request));
  }

  return Status;
}

EFI_STATUS
EFIAPI
DefaultRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_STATUS                                 Status;
  UINTN                                      BufferSize;
  UINT32									 Attributes;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }


  *Progress = Configuration;
  if (!HiiIsConfigHdrMatch (Configuration, &mSystemConfigurationGuid, mVarStoreName)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
              mVariableName,
              &mSystemConfigurationGuid,
              &Attributes,
              &BufferSize,
              &mSystemConfiguration
              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = sizeof (SYSTEM_CONFIGURATION);
  Status = mHiiConfigRouting->ConfigToBlock (
                               mHiiConfigRouting,
                               Configuration,
                               (UINT8 *) &mSystemConfiguration,
                               &BufferSize,
                               Progress
                               );

  //
  // Store Buffer Storage back to EFI variable
  //
  Status = gRT->SetVariable (
                  mVariableName,
                  &mSystemConfigurationGuid,
				  Attributes,
				  sizeof (SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );

#if defined(AMT_SUPPORT) && AMT_SUPPORT
  if ((Status == EFI_SUCCESS) &&
      MeTypeIsAmt()) {
    Status = MeAdvancedRouteConfig (This, Configuration, Progress);
#if defined(ICC_SUPPORT) && ICC_SUPPORT
    IccRouteConfig();
#endif // ICC_SUPPORT
  }
#endif // AMT_SUPPORT

  //
  // synchronize PcdUefiOptimizedBoot and UefiOptimizedBoot
  //
  //PcdSetBool (PcdUefiOptimizedBoot, (BOOLEAN)mSystemConfiguration.UefiOptimizedBoot);   // AptioV server override. Boot options related code not required to be handled here.

  return Status;
}

EFI_STATUS
EFIAPI
DefaultCallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *FileSystemProtocol;
  EFI_FILE_PROTOCOL                 *Root;
  EFI_FILE_PROTOCOL                 *NewFile;
  UINTN                             Status = EFI_UNSUPPORTED;
  UINTN                             VarSize;
  UINT32					        Attributes;
  UINT32                            VersionNumber;
  SYSTEM_CONFIGURATION              SetupData;

  if (Action == EFI_BROWSER_ACTION_CHANGED) {
    switch(QuestionId) {
      case MAIN_PAGE_KEY_SAVE:
        //
        // Save setup data to file
        //
        Status = gBS->LocateProtocol(
            &mEfiSimpleFileSystemProtocolGuid,
            NULL,
            &FileSystemProtocol
        );
        ASSERT_EFI_ERROR(Status);

        //
        // Create new file on the first file system encountered
        // If file already exists, it is overwritten
        //
        FileSystemProtocol->OpenVolume(FileSystemProtocol, &Root);
        Root->Open(
            Root,
            &NewFile,
            L"SetupSave.txt",
            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
        );

        //
        // Access setup variables and write the firmware version followed by the setup data
        //
        VarSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
        Status = gRT->GetVariable (L"IntelSetup", &mSystemConfigurationGuid, NULL, &VarSize, &SetupData);
        ASSERT_EFI_ERROR(Status);

        VarSize = sizeof(UINT32);
        NewFile->Write(NewFile, &VarSize, &(gST->FirmwareRevision));

        VarSize = sizeof (SYSTEM_CONFIGURATION);
        Status = NewFile->Write(NewFile, &VarSize, &SetupData);

        NewFile->Close(NewFile);
        Root->Close(Root);

        Status = EFI_SUCCESS;
        break;

      case MAIN_PAGE_KEY_RESTORE:
        //
        // Restore setup data from file
        //
        Status = gBS->LocateProtocol(
            &mEfiSimpleFileSystemProtocolGuid,
            NULL,
            &FileSystemProtocol
        );
        ASSERT_EFI_ERROR(Status);

        FileSystemProtocol->OpenVolume(FileSystemProtocol, &Root);

        Status = Root->Open(
            Root,
            &NewFile,
            L"SetupSave.txt",
            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
        );

        if(Status != EFI_SUCCESS) {
          DEBUG((EFI_D_INFO, "File SetupSave.txt could not be found.\n", Status));
          break;
        }

        //
        // Access setup variables
        //
        VarSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
        Status = gRT->GetVariable (L"IntelSetup", &mSystemConfigurationGuid, &Attributes, &VarSize, &SetupData);
        ASSERT_EFI_ERROR(Status);

        //
        // Verify the file was created from the same firmware version
        // Clean up and exit if it isn't
        //
        VarSize = sizeof(UINT32);
        NewFile->Read(NewFile, &VarSize, &VersionNumber);

        if(VersionNumber != gST->FirmwareRevision) {
          DEBUG((EFI_D_INFO, "Setup data save file is for a different firmware version\n", Status));
          NewFile->Close(NewFile);
          Root->Close(Root);
          break;
        }

        VarSize = sizeof (SYSTEM_CONFIGURATION);
        NewFile->Read(NewFile, &VarSize, &SetupData);
  // AptioV server override: Changed variable name to Intelsetup
        gRT->SetVariable(L"IntelSetup", &mSystemConfigurationGuid, Attributes, VarSize, &SetupData);
  
        NewFile->Close(NewFile);
        Root->Close(Root);

        Status = EFI_SUCCESS;
        break;
      default:
        Status = EFI_UNSUPPORTED;
  	break;
    }
  }
  return Status;
}

EFI_STATUS
InstallDefaultCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  )
{
  EFI_STATUS          Status;

  ASSERT (ConfigAccess != NULL);

  ConfigAccess->ExtractConfig = DefaultExtractConfig;
  ConfigAccess->RouteConfig   = DefaultRouteConfig;
  ConfigAccess->Callback      = DefaultCallbackRoutine;

  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  ConfigAccess
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AdvancedCallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
   EFI_STATUS                                 Status = EFI_SUCCESS;


  switch(Action)
  {
  case EFI_BROWSER_ACTION_FORM_CLOSE:
     //
     // Do nothing for UEFI CLOSE/OPEN Action
     //
	break;
  case EFI_BROWSER_ACTION_FORM_OPEN:
  break;
  case EFI_BROWSER_ACTION_CHANGING:
    if ((QuestionId == KEY_XMP_PROFILE) || (QuestionId == KEY_XMP_PROFILE1)) {
      XmpCallback( This, Action, QuestionId, Type, Value, ActionRequest);
      break;
    }
#if defined(ICC_SUPPORT) && ICC_SUPPORT
    if (MeTypeIsAmt()) {
      switch (QuestionId)
      {
      case KEY_ICC_RATIO2:
      case KEY_ICC_FREQ2:
      case KEY_ICC_FREQ3:
      case KEY_ICC_SPREAD2:
      case KEY_ICC_SPREAD3:
        IccCallback( This, Action, QuestionId, Type, Value, ActionRequest);
      break;
      case KEY_ICC_PROFILE:
        IccProfileCallback( This, Action, QuestionId, Type, Value, ActionRequest);
      break;
      default:
      break;
      }
    }
#endif // ICC_SUPPORT
  break;
  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    if (IsSoftSdvPlatform() == FALSE) {
      {
        UINTN MyVarSize = sizeof (SYSTEM_CONFIGURATION);

        switch (QuestionId)
        {
         // Update PchPciePortConfig according to softstraps info
         case 0x2001:
           mSystemConfiguration.PciePortConfig1 = mPlatformType->PchData.PchPciePortCfg1;
           mSystemConfiguration.PciePortConfig2 = mPlatformType->PchData.PchPciePortCfg2;
           break;
         case 0x2003:
           mSystemConfiguration.PcieSBDE = mPlatformType->PchData.PchPcieSBDE;
           break;
         case 0x2004:
           mSystemConfiguration.GbePciePortNum = mPlatformType->PchData.GbePciePortNum;         
           break;
         case 0x2005:
           mSystemConfiguration.GbeRegionInvalid = mPlatformType->PchData.GbeRegionInvalid;
           break;
         case 0x2006:
           mSystemConfiguration.PchLanSupported = mPlatformType->PchData.PchLanSupported;
           break;
        }

        Status = gRT->SetVariable(
                     mVariableName,
                     &mSystemConfigurationGuid,
		 			 SETUP_VARIABLE_ATTRIBUTES,
                     MyVarSize,
                     &mSystemConfiguration
                    );
      }
    }
      break;
  case EFI_BROWSER_ACTION_RETRIEVE:
      break;
  default:
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    //Status = EFI_UNSUPPORTED;	//Aptio V Server Override
	break;
  }

//Aptio V Server Override - Start
  if (QuestionId == 0x2800) {
	  if (Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD || Action == EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING)
		  return EFI_UNSUPPORTED;
	  if (Action == EFI_BROWSER_ACTION_CHANGING) AmiPowerMgtCallback(Value);
  }
//Aptio V Server Override - End
  
  return Status;
}

EFI_STATUS
EFIAPI
XmpCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION Action,
  IN EFI_QUESTION_ID KeyValue,
  IN UINT8 Type,
  IN EFI_IFR_TYPE_VALUE *Value,
  OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest OPTIONAL
  )
/*++
Routine Description:

  Setup callback executed

Arguments:

Returns:

  always SUCCESS

--*/
{
  UINT8       profile;
  SYSTEM_CONFIGURATION Setup;
  EFI_STATUS  Status;
  struct SystemMemoryMapHob             *systemMemoryMap;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if ( GuidHob != NULL)
    systemMemoryMap = (struct SystemMemoryMapHob *) GET_GUID_HOB_DATA (GuidHob);
  else {
    systemMemoryMap = NULL;
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
  }

  HiiGetBrowserData (&gEfiSetupVariableGuid, L"IntelSetup", sizeof(SYSTEM_CONFIGURATION), (UINT8*)&Setup);  //AptioV server override

  if ((Value->ref.QuestionId == XMP_PROFILE_1) || (Value->ref.QuestionId == XMP_PROFILE_2)) {

    // Get the profile to use
    profile = ((UINT8)Value->ref.QuestionId - XMP_PROFILE_1);

    Setup.tCAS          = systemMemoryMap->profileMemTime[profile].nCL;
    Setup.tRP           = systemMemoryMap->profileMemTime[profile].nRP;
    Setup.tRCD          = systemMemoryMap->profileMemTime[profile].nRCD;
    Setup.tRAS          = systemMemoryMap->profileMemTime[profile].nRAS;
    Setup.tWR           = systemMemoryMap->profileMemTime[profile].nWR;
    Setup.tRFC          = systemMemoryMap->profileMemTime[profile].nRFC;
    Setup.tRRD          = systemMemoryMap->profileMemTime[profile].nRRD;
    Setup.tRTP          = systemMemoryMap->profileMemTime[profile].nRTP;
    Setup.tWTR          = systemMemoryMap->profileMemTime[profile].nWTR;
    Setup.tFAW          = systemMemoryMap->profileMemTime[profile].nFAW;
    Setup.tCWL          = systemMemoryMap->profileMemTime[profile].nCWL;
    Setup.tRC           = systemMemoryMap->profileMemTime[profile].nRC;
    Setup.commandTiming = systemMemoryMap->profileMemTime[profile].nCMDRate;
    Setup.DdrFreqLimit  = systemMemoryMap->profileMemTime[profile].ddrFreqLimit;
    Setup.tREFI         = systemMemoryMap->profileMemTime[profile].tREFI;
    Setup.Vdd           = systemMemoryMap->profileMemTime[profile].vdd;
  } else if (Setup.XMPMode == XMP_AUTO) {
    Setup.DdrFreqLimit  = DDR3_FREQ_AUTO;
  }

  HiiSetBrowserData(&gEfiSetupVariableGuid, L"IntelSetup", sizeof(SYSTEM_CONFIGURATION), (UINT8 *)&Setup, NULL); //AptioV server override

  return EFI_SUCCESS;
} // XmpCallback


EFI_STATUS
InstallAdvancedCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  )
{
  EFI_STATUS          Status;

  ASSERT (ConfigAccess != NULL);

  ConfigAccess->ExtractConfig = DefaultExtractConfig;
  ConfigAccess->RouteConfig   = DefaultRouteConfig;
  ConfigAccess->Callback      = AdvancedCallbackRoutine;

  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  ConfigAccess
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

VOID
BackupSetupDataAndCheckForPassword (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINTN                    VariableSize;
  SYSTEM_CONFIGURATION     SetupData;
  EFI_STATUS               Status;


  CheckForPassword ();

  VariableSize = sizeof(SYSTEM_CONFIGURATION);
  //
  // Get Buffer Storage data from EFI variable
  //
  Status = gRT->GetVariable (
                  mVariableName,
                  &mSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Failed to get Variable before enter setup!\n"));
    return;
  }
  //
  // We don't need this to be save into flash, just in memory is enough
  //
//AptioV server override start: Setup power good reset support
/*  Status = gRT->SetVariable(
                  mTempVariableName,
                  &gEfiGlobalVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  VariableSize,
                  &SetupData
                  );           
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Failed to save Variable before enter setup!\n"));
    return;
  }*/
//AptioV server override end: Setup power good reset support

  gBS->CloseEvent (Event);

}

VOID
CheckForPassword (
  VOID
  )
{
  EFI_STATUS          Status;
  CHAR16             *ErrorMsgBuf1;
  CHAR16             *ErrorMsgBuf2;
  UINT8               ReTries;
  EFI_INPUT_KEY       Key;
  UINTN               Index;

  ErrorMsgBuf1   = NULL;
  ErrorMsgBuf2   = NULL;
  Status         = EFI_SUCCESS;

  ErrorMsgBuf1 = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));
  ASSERT (ErrorMsgBuf1 != NULL);

  ErrorMsgBuf2 = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));
  ASSERT (ErrorMsgBuf2 != NULL);
  //
  // Check if we need to validate at this point
  //
  gPwFlag = CheckIfPasswordInstalled();

  if (gPwFlag != VALIDATE_NONE ) {
    //
    // User has to be validated
    //
    for (ReTries=0;ReTries <3;) {
      Status = PromptForPassword ();
      if (Status == AUTHENTICATION_PASSED){
        //
        // Passed Authentication done , You are allowed to Enter BIOS Setup
        //
        break;
    }
      else {
         do {

         CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE, &Key, L"Invalid Password", NULL, NULL);
        } while ((Key.UnicodeChar != CHAR_CARRIAGE_RETURN));

     }
      //
      // Maximum of 3 retries are allowed to enter Wrong password
      //
      ReTries++;
    }
  if (Status == AUTHENTICATION_NOT_PASSED){
      //
      // Popup a menu to notice user
      //
    do {
        //
        // Retry Limit is over, Halt the Machine
        //
      StrCpy (ErrorMsgBuf1, L" System Halted ! ");
      StrCpy (ErrorMsgBuf2, L" Press CTRL+ALT+DEL to reset Machine");
      CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE, &Key, ErrorMsgBuf1, ErrorMsgBuf2, NULL);
      gBS->WaitForEvent( 1, gST->ConIn->WaitForKey, &Index );
      gST->ConIn->ReadKeyStroke( gST->ConIn, &Key );
    } while (TRUE);
  }
 }

  FreePool (ErrorMsgBuf1);
  FreePool (ErrorMsgBuf2);
  //gBS->CloseEvent (Event);

}

EFI_STATUS
ReadStrings(
  IN CHAR16      *StringPtr
){
  UINTN                            Index;
  UINTN                            Count;
  CHAR16                           *TempString;
  CHAR16                           KeyPad[2];
  UINTN                            Maximum;
  EFI_INPUT_KEY                    Key;
  EFI_STATUS                       Status;
  CHAR16                           *BufferedString;

  Maximum = PASSWORD_MAX_SIZE;

  BufferedString = AllocateZeroPool((Maximum + 1)* sizeof (CHAR16));
  ASSERT (BufferedString);
  TempString = AllocateZeroPool ((Maximum + 1)* sizeof (CHAR16));
  ASSERT (TempString);


  do{

   Status = ReadKeyStrokes( &Key);
   ASSERT_EFI_ERROR (Status);

   switch (Key.UnicodeChar) {
   case CHAR_NULL:
     switch (Key.ScanCode) {
     case SCAN_LEFT:
       break;

      case SCAN_RIGHT:
        break;

      case SCAN_ESC:
        FreePool (TempString);
        FreePool(BufferedString);
        return EFI_DEVICE_ERROR;
        break;

      default:
        break;
  }

     break;

   case CHAR_CARRIAGE_RETURN:
     FreePool (TempString);
     FreePool(BufferedString);
     return EFI_SUCCESS;

       break;

   case CHAR_BACKSPACE:
     if (StringPtr[0] != CHAR_NULL) {
      for (Index = 0; StringPtr[Index] != CHAR_NULL; Index++) {
        TempString[Index] = StringPtr[Index];
      }
        //
        // Effectively truncate string by 1 character
        //
      TempString[Index - 1] = CHAR_NULL;
      StrCpy (StringPtr, TempString);
     }

   default:
      //
      // If it is the beginning of the string, don't worry about checking maximum limits
      //
    if ((StringPtr[0] == CHAR_NULL)&&(Key.UnicodeChar!= CHAR_BACKSPACE)) {
      StrnCpy (StringPtr, &Key.UnicodeChar, 1);
      StrnCpy (TempString, &Key.UnicodeChar, 1);
     } else if (((StrLen(StringPtr)) < Maximum) &&(Key.UnicodeChar!= CHAR_BACKSPACE)) {

        KeyPad[0] = Key.UnicodeChar;
        KeyPad[1] = CHAR_NULL;
        StrCat (StringPtr, KeyPad);
        StrCat (TempString, KeyPad);
     }

    for(Count =0; Count<Maximum; Count++)
      BufferedString[Count] = L' ';
   gST->ConOut->SetCursorPosition (gST->ConOut, gColumn, gRow);

    for(Count =0; Count<StrLen(StringPtr); Count++)
      BufferedString[Count] = L'*';
      gST->ConOut->OutputString (gST->ConOut, BufferedString);
    break;
   }


   gST->ConOut->SetCursorPosition (gST->ConOut, gColumn + StrLen(StringPtr), gRow);
  }while(TRUE);

  return Status;

 }


EFI_STATUS
ReadKeyStrokes(
  OUT  EFI_INPUT_KEY            *Key
)
{
  EFI_STATUS                    Status;
  EFI_EVENT                     TimerEvent;
  EFI_EVENT                     WaitList[2];
  UINTN                         Index;

  do{
    do {
     Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);

    //
    // Set a timer event of 1 second expiration
    //
     gBS->SetTimer (
            TimerEvent,
            TimerRelative,
            ONE_SECOND
            );

    //
    // Wait for the keystroke event or the timer
    //
    WaitList[0] = gST->ConIn->WaitForKey;
    WaitList[1] = TimerEvent;
    Status      = gBS->WaitForEvent (2, WaitList, &Index);

    //
    // Check for the timer expiration
    //
      if (!EFI_ERROR (Status) && Index == 1) {
        Status = EFI_TIMEOUT;
      }

    gBS->CloseEvent (TimerEvent);
   } while (Status == EFI_TIMEOUT);

  Status = gST->ConIn->ReadKeyStroke (gST->ConIn, Key);
  }while (EFI_ERROR(Status));
  return Status;
 }

VOID
EFIAPI
DrawPopUp (
 ){
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
  EFI_SIMPLE_TEXT_OUTPUT_MODE      SavedConsoleMode;
  UINTN                            Columns;
  UINTN                            Rows;
  UINTN                            Column;
  UINTN                            Row;
  UINTN                            NumberOfLines;
  UINTN                            MaxLength;
  UINTN                            Length;
  CHAR16                           *Line;
  CHAR16                           *Line1;
  CHAR16                           *StringBuffer;


  MaxLength     = 20;
  NumberOfLines = 3;

  //
  // If the total number of lines in the popup is zero, then ASSERT()
  //
  ASSERT (NumberOfLines != 0);

  //
  // If the maximum length of all the strings is zero, then ASSERT()
  //
  ASSERT (MaxLength != 0);

  //
  // Cache a pointer to the Simple Text Output Protocol in the EFI System Table
  //
  ConOut = gST->ConOut;

  //
  // Save the current console cursor position and attributes
  //
  CopyMem (&SavedConsoleMode, ConOut->Mode, sizeof (SavedConsoleMode));

  //
  // Retrieve the number of columns and rows in the current console mode
  //
  ConOut->QueryMode (ConOut, SavedConsoleMode.Mode, &Columns, &Rows);

  ConOut->ClearScreen (gST->ConOut);

  //
  // Disable cursor and set the foreground and background colors specified by Attribute
  //
  ConOut->EnableCursor (ConOut, TRUE);
  ConOut->SetAttribute (ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE);

  //
  // Compute the starting row and starting column for the popup
  //
  Row    = (Rows - (NumberOfLines + 3)) / 2;
  Column = (Columns - (MaxLength + 2)) / 2;


  //
  // Allocate a buffer for a single line of the popup with borders and a Null-terminator
  //
  Line = AllocateZeroPool ((MaxLength + 3) * sizeof (CHAR16));
  ASSERT (Line != NULL);

  //
  // Draw top of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_DOWN_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_DOWN_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);


  // Draw middle of the popup with strings
  StringBuffer = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));
  StrCpy (StringBuffer, L"Enter Password ");
  Length = StrLen (StringBuffer);


  SetMem16 (Line, (MaxLength + 2) * 2, L' ' );
  Line[0]             = BOXDRAW_VERTICAL;
  Line[MaxLength + 1] = BOXDRAW_VERTICAL;
  Line[MaxLength + 2] = L'\0';
  CopyMem (Line + 1 + (MaxLength - Length) / 2, StringBuffer , Length * sizeof (CHAR16));
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //draw vertical lines for popup

  while ( NumberOfLines > 0) {
  SetMem16 (Line, (MaxLength + 2) * 2, L' ');
  Line[0]             = BOXDRAW_VERTICAL;
  Line[MaxLength + 1] = BOXDRAW_VERTICAL;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);
  NumberOfLines--;
  }

  //
  // Draw bottom of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_UP_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_UP_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Free the allocated line buffer
  //
  FreePool (Line);

 Line1 = AllocateZeroPool ((PASSWORD_MAX_SIZE + 3) * sizeof (CHAR16));
  ASSERT (Line1 != NULL);


  ConOut->SetAttribute (ConOut,EFI_WHITE | EFI_BACKGROUND_BLACK);

//these are for inside popup
  Row=Row-3;
  Column=Column+5;

  gRow=Row;
  gColumn=Column;


//Draw top of inside popup box
  SetMem16 (Line1, (PASSWORD_MAX_SIZE + 2) * 2, L' ');
  Line1[0]              = L' ';
  Line1[PASSWORD_MAX_SIZE + 1] = L' ';
  Line[PASSWORD_MAX_SIZE + 2]  = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line1);

  ConOut->EnableCursor (ConOut, TRUE);

  FreePool (Line1);
  FreePool (StringBuffer);

  //
  // Restore the cursor visibility, position, and attributes
  //
  ConOut->EnableCursor      (ConOut, SavedConsoleMode.CursorVisible);
  ConOut->SetCursorPosition (ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
  ConOut->SetAttribute      (ConOut, SavedConsoleMode.Attribute);


 }
EFI_STATUS
PromptForPassword(
  VOID
)
{
  EFI_STATUS                      Status;
  CHAR16                          *Password;
  UINTN                           VarSize;
  UINT8                           AccessLevel;
  SYSTEM_CONFIGURATION            SetupData;

  AccessLevel  = SYSTEM_PASSWORD_ADMIN ;
  Status       = AUTHENTICATION_NOT_PASSED;
  VarSize      = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status       = gRT->GetVariable (L"IntelSetup", &mSystemConfigurationGuid, NULL, &VarSize, &SetupData);
  //
  // Draw Popup for password prompt
  //
  DrawPopUp ();
  Password = AllocateZeroPool ((PASSWORD_MAX_SIZE +1)* sizeof (CHAR16));

  Status  = ReadStrings(Password);

  if (*Password == L' ' || Status != EFI_SUCCESS)
    Status = AUTHENTICATION_NOT_PASSED;

  EncodePassword(Password);

  switch (gPwFlag) {

   case VALIDATE_ADMIN:
    if (StrnCmp (Password, SetupData.AdminPassword,PASSWORD_MAX_SIZE) == 0) {
      Status = AUTHENTICATION_PASSED;
      AccessLevel = SYSTEM_PASSWORD_ADMIN;

    } else {
      Status = AUTHENTICATION_NOT_PASSED;
    }

    break;
   case VALIDATE_USER:
    if (StrnCmp (Password, SetupData.UserPassword,PASSWORD_MAX_SIZE) == 0) {
      Status = AUTHENTICATION_PASSED;
      AccessLevel = SYSTEM_PASSWORD_USER;
     } else {
       Status = AUTHENTICATION_NOT_PASSED;
     }

    break;
   case VALIDATE_BOTH:
    if (StrnCmp (Password, SetupData.AdminPassword,PASSWORD_MAX_SIZE) == 0){
      Status = AUTHENTICATION_PASSED;
      AccessLevel = SYSTEM_PASSWORD_ADMIN;
     } else if (StrnCmp (Password, SetupData.UserPassword,PASSWORD_MAX_SIZE) == 0){
       Status = AUTHENTICATION_PASSED;
       AccessLevel = SYSTEM_PASSWORD_USER;
     } else {
       Status = AUTHENTICATION_NOT_PASSED;
     }

   break;
  }
  if(Status == AUTHENTICATION_PASSED ) {
   SetPasswordType ( AccessLevel );
  }

  FreePool (Password);
  return Status;
 }


UINT8
CheckIfPasswordInstalled(
  VOID
)
{
  UINT8                   State;
  UINTN                   VarSize;
  EFI_STATUS              Status;
  SYSTEM_CONFIGURATION    SetupData;

  gPwFlag            = VALIDATE_NONE;
  VarSize           = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status  = gRT->GetVariable (
                   L"IntelSetup",
                   &mSystemConfigurationGuid, 
                   NULL,
                   &VarSize,
                   &SetupData
                   );
  if (!EFI_ERROR(Status)) {
    //
   // User trying to enter setup , Validate the password here if it is set
   //
   // Verify if Admin Password is Installed
   State = (UINT8) ((((CHAR16 *) SetupData.AdminPassword)[0] == 0) ? PW_NOT_SET : (((CHAR16 *) SetupData.AdminPassword)[0]!= 0x0053) ? PW_SET : PW_NOT_SET);
   if (State == PW_SET ){
    //
    // Admin P/W installed
    //
    gPwFlag |= VALIDATE_ADMIN;
   }
   State = 0;
   State = (UINT8) ((((CHAR16 *) SetupData.UserPassword)[0] == 0) ? PW_NOT_SET : (((CHAR16 *) SetupData.UserPassword)[0]!= 0x0053) ? PW_SET : PW_NOT_SET);
   if (State == PW_SET ){
    //
    // Yes User P/W installed
    //
    gPwFlag |= VALIDATE_USER;
   }
  }

  return gPwFlag;
 }

BOOLEAN
IsPasswordCheck (
 IN  BOOLEAN         Check,
 IN  CHAR16          Key
)
{
  if(Check){
    if(CharIsAlphaNumeric(Key))
      return TRUE;
      else
       return FALSE;
  }
  else
    return TRUE;
 }

VOID
SetPasswordType(
 IN UINT8         Type
)
{

  SYSTEM_CONFIGURATION    SetupData;
  UINTN                   VariableSize;
  UINT32				  Attributes;
  EFI_STATUS              Status;

  VariableSize = sizeof(SYSTEM_CONFIGURATION);

  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &SetupData
                  );
  ASSERT_EFI_ERROR(Status);

  SetupData.Access = Type;
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->SetVariable(
                   L"IntelSetup",
                   &gEfiSetupVariableGuid,
                   Attributes,
                   VariableSize,
                   &SetupData
                   );
  ASSERT_EFI_ERROR(Status);

  return;
 }

VOID
EFIAPI
PlatformResetCallback (
  PCH_EXTENDED_RESET_TYPES        *PchExtendedResetType
  )
{
  EFI_STATUS                      Status;
  SYSTEM_CONFIGURATION            OldVarData;
  UINTN                           VariableSize;
  UINT32						  Attributes;
  //
  // Reset Flag
  //
  BOOLEAN                         GlobalResetRequired;
  BOOLEAN                         PowerCycleResetRequired;
  UINT8                           *MemPtr1, *MemPtr2;
  UINT32                          MemSize;

  GlobalResetRequired = FALSE;
  PowerCycleResetRequired = FALSE;

  DEBUG((EFI_D_ERROR, "Platform reset callback entered!\n"));

  VariableSize = sizeof(SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &mSystemConfiguration
                  );
  //
  // ASSERT_EFI_ERROR(Status);
  //
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Failed to get Variable in reset hook!\n"));
    return ;

  }

//AptioV server override start: Setup power good reset support
  /*VariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                  mTempVariableName,
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &VariableSize,
                  &OldVarData
                  );

  //
  // ASSERT_EFI_ERROR(Status);
  //
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Failed to get saved Variable in reset hook!\n"));  
    return ;
  }*/
  CopyMem (&OldVarData, &gOldSystemConfiguration, sizeof (SYSTEM_CONFIGURATION));
//AptioV server override end: Setup power good reset support

//
  // Make sure enable all 3 of them if ProcessorTxtEnable is "enabled"
  //
  if (mSystemConfiguration.ProcessorLtsxEnable) {
DEBUG((EFI_D_ERROR, "mSystemConfiguration.ProcessorLtsxEnable = %d\n",mSystemConfiguration.ProcessorLtsxEnable));
    mSystemConfiguration.ProcessorSmxEnable = 1;
    mSystemConfiguration.ProcessorVmxEnable = 1;
    mSystemConfiguration.LockChipset = 1;
  }

//MWAIT changes-start ->enable MWAIT if c state is enabled

//MWAIT changes-end

    Status = gRT->SetVariable(
                  mVariableName,
                  &mSystemConfigurationGuid,
                  Attributes,
                  sizeof (SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );


#if defined(ICC_SUPPORT) && ICC_SUPPORT
  if (gIccReset) {
    PowerCycleResetRequired = TRUE;
  }
#endif // ICC_SUPPORT
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  if (gAmtResetRequests) {
    GlobalResetRequired = TRUE;
  }
#endif // AMT_SUPPORT

  //
  // Update Use1GPageTable PCD with setup value on exit
  //
  //Aptio V Server Override - Remove Question use only PCD.
  //PcdSetBool (PcdUse1GPageTable, mSystemConfiguration.Use1GPageTable ?  TRUE : FALSE);
  
  //
  // Set eSmm PCDs to new value if changed
  //
  if (mSystemConfiguration.SmmSaveState != OldVarData.SmmSaveState) {
    PcdSetBool (PcdCpuSmmMsrSaveStateEnable, mSystemConfiguration.SmmSaveState ? TRUE : FALSE);
  }
  
  if (mSystemConfiguration.TargetedSmi != OldVarData.TargetedSmi) {
    PcdSetBool (PcdCpuSmmUseDelayIndication, mSystemConfiguration.TargetedSmi ? TRUE : FALSE);
    PcdSetBool (PcdCpuSmmUseBlockIndication, mSystemConfiguration.TargetedSmi ? TRUE : FALSE);
    PcdSetBool (PcdCpuSmmUseSmmEnableIndication, mSystemConfiguration.TargetedSmi ? TRUE : FALSE);
  }
  
  //
  // If this setup variable changed, a complete power cycle is needed to flush previous page tables
  //  
  //Aptio V Server Override - Remove Question use only PCD.
  //if (mSystemConfiguration.Use1GPageTable != OldVarData.Use1GPageTable){
  //  PowerCycleResetRequired = TRUE;
  //}

  //
  // If QPI has changed any setup items, make sure that setup exit reset gets power good reset. (cold reset)
  // The tags are not included for testing QPI setup changes.
  //
  MemPtr1 = &mSystemConfiguration.QpiSetupNvVariableStartTag;
  MemPtr2 = &OldVarData.QpiSetupNvVariableStartTag;
  MemSize = (UINT32) (&mSystemConfiguration.QpiSetupNvVariableEndTag - &mSystemConfiguration.QpiSetupNvVariableStartTag);
  while(--MemSize) {
    if (*(++MemPtr1) != *(++MemPtr2)) {
      PowerCycleResetRequired = TRUE;
      break;
    }
  }

  //
  // SMX/VMX setup option changes require power good reset
  //
  if ( (mSystemConfiguration.ProcessorVmxEnable != OldVarData.ProcessorVmxEnable) ||
       (mSystemConfiguration.ProcessorSmxEnable != OldVarData.ProcessorSmxEnable) ||
       (mSystemConfiguration.ProcessorMsrLockControl != OldVarData.ProcessorMsrLockControl) ) {
     PowerCycleResetRequired = TRUE;
  }

  if (mSystemConfiguration.UFSDisable != OldVarData.UFSDisable) {      
      PowerCycleResetRequired = TRUE;
  }

  //
  // Change of any option from common ref. code requires power good reset
  //
  if ( (mSystemConfiguration.MmcfgBase != OldVarData.MmcfgBase ) ||
		  (mSystemConfiguration.MmiohBase != OldVarData.MmiohBase ) ||
		  (mSystemConfiguration.MmiohSize != OldVarData.MmiohSize ) ||
		  (mSystemConfiguration.IsocEn != OldVarData.IsocEn ) ||
		  (mSystemConfiguration.MeSegEn != OldVarData.MeSegEn ) ||
		  (mSystemConfiguration.NumaEn != OldVarData.NumaEn ) ) {
       PowerCycleResetRequired = TRUE;
  }

  //
  // Change of RasMode setup option requires power good reset
  //
  if ( (mSystemConfiguration.RASMode != OldVarData.RASMode ) ) {
         PowerCycleResetRequired = TRUE;
  }

  if (mSystemConfiguration.ScrambleEn != OldVarData.ScrambleEn) {
    PowerCycleResetRequired = TRUE;
  }

  if(GlobalResetRequired) {
    PchExtendedResetType->PowerCycleReset   = 1;
    PchExtendedResetType->GlobalReset  = 1;
    PchExtendedResetType->GlobalResetWithEc  = 1;
  } else if (PowerCycleResetRequired) {
    PchExtendedResetType->PowerCycleReset   = 1;
    PchExtendedResetType->GlobalReset  = 0;
    PchExtendedResetType->GlobalResetWithEc  = 0;
  }
}



