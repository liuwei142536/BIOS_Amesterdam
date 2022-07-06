/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2008 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  BootMode.c

Abstract:

  EFI PEIM to provide the platform support functionality

--*/
#include <Token.h> // AptioV server override
#include "PlatformEarlyInit.h"
#include <Iio\MmioAccess.h>

// AptioV Server override - Start: For AMI Recovery support
#include <ppi\PCICfg.h>
#include <ppi\CpuIo.h>
// AptioV Server override - End: For AMI  Recovery support

EFI_PEI_PPI_DESCRIPTOR  mPpiListRecoveryBootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiBootInRecoveryModePpiGuid,
  NULL
};


//Defined in OEMPort.c
// AptioV Server override: For AMI Recovery support
BOOLEAN
IsRecovery(
    EFI_PEI_SERVICES  **PeiServices,
    EFI_PEI_PCI_CFG_PPI  *PciCfg,
    EFI_PEI_CPU_IO_PPI  *CpuIo
);

EFI_STATUS
UpdateBootMode (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN      UINT16                                 PlatformType
  )
{
  EFI_STATUS    Status;
  UINT32        GpiValue;
  UINT32        GpiPin;
  EFI_BOOT_MODE BootMode;
  UINT16        SleepType;
  CHAR16        *strBootMode;
  UINT8         Nmi;
  UINT32        Data32;

  // AptioV Server override - start: For AMI Recovery support
    EFI_PEI_PCI_CFG_PPI         *PciCfg;
    EFI_PEI_CPU_IO_PPI          *CpuIo;
  // AptioV Server override - End: For AMI Recovery support


//Aptio V Server Override - ScleanRequired and LtErrSts aren't used.
#if 0
  UINT8         ScleanRequired;  
  UINT32        LtErrSts;
#endif
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  GetWakeupEventAndSaveToHob (PeiServices);


  // This pin is accessed even before GPIO init, hence marking the pin to Input direction 
  // before accesing the it's corresponding LVL register. 
  GpiPin = IoRead32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_IO_SEL2);
  IoWrite32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_IO_SEL2, GpiPin | BIT23);
  GpiValue = IoRead32 (PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL2);
  // Get Normal/Recovery boot mode from GPIO55
  GpiValue = ((GpiValue & BIT23) >> 23);

  
  //
  // Preserve NMI bit setting
  //
  MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32) B_PCH_RCRB_GCS_AME);
  Data32  = MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS);
  Nmi     = (IoRead8 (R_IOPORT_CMOS_STANDARD_INDEX) & 0x80);
  MmioAnd32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32)~B_PCH_RCRB_GCS_AME);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  //AptioV server override-Start: Always disable recovery jumper in SOFTSDV BIOS
  if (IsSoftSdvPlatform()) {
    GpiValue = BIT0;
  }
  //AptioV server override-End: Always disable recovery jumper in SOFTSDV BIOS
  DEBUG ((EFI_D_ERROR, "GpiValue: %x\n", GpiValue));
  if (IsManufacturingMode()) {
    BootMode = BOOT_WITH_MFG_MODE_SETTINGS;     
    // MFG mode must override all other Boot Modes.

  } else if (GpiValue == 0) {
    BootMode = BOOT_IN_RECOVERY_MODE;
DEBUG((EFI_D_ERROR, "Setting BOOT_IN_RECOVERY_MODE GpiValue == 0\n"));
    Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListRecoveryBootMode);
    ASSERT_EFI_ERROR(Status);
    // Allow recovery mode to override later boot modes.


  } else if ((PciRead8(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_3)) & B_PCH_LPC_GEN_PMCON_RTC_PWR_STS) ||
          ((IoRead8(R_IOPORT_CMOS_STANDARD_DATA) & (BIT6 + BIT7))!=0)) {
    //
    // Check RTC_PWR_STS (B0:D31:F0 Offset:0xA4 BIT02) to Bad/clear CMOS.
    // Also check CMOS Checksum Status and RTCLK Power Status bits for bad CMOS
    // If set load BIOS defaults.
    //
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;

    //
	// If the BIOS defaults are loaded, 
    // Admin password should be saved (to prevent its reset)
    //
    SaveAdminPassToHob(PeiServices);


  } else if (PcdGetBool(PcdBootState)) {
    BootMode = BOOT_WITH_FULL_CONFIGURATION;
    // If this is first boot after flash, override sleep states.

  } else {

    BootMode = BOOT_WITH_MINIMAL_CONFIGURATION;
    // Normal boot mode most of the time. Safe to override it with S states below.

    if (IsSoftSdvPlatform()) {
      GpiValue = BIT0;
    }

//Aptio V Server Override - ScleanRequired and LtErrSts aren't used.
#if 0
  // Check if secrets are in memory.
    LtErrSts = (*(volatile UINT32 *)(UINTN)0xfed30008);
  ScleanRequired = (( LtErrSts & BIT6 ) == 0  ) ? 0 : 1;
  // This bit will always return 1 if board not rework, comment out for now.
  //if( ScleanRequired )
  //{
  //      BootMode = BOOT_ON_S3_RESUME;
  //      goto SCLEAN_USE_S3;
  //}
#endif
    DEBUG ((EFI_D_ERROR, "GpiValue: %x\n", GpiValue));
    DEBUG((EFI_D_ERROR, "GetSleepTypeAfterWakeup (PeiServices, &SleepType) = %x\n", GetSleepTypeAfterWakeup (PeiServices, &SleepType)));
    if (GetSleepTypeAfterWakeup (PeiServices, &SleepType)) {
        DEBUG((EFI_D_ERROR, "SleepType = %x\n", SleepType));
      switch (SleepType) {
        case V_PCH_ACPI_PM1_CNT_S3:
          BootMode = BOOT_ON_S3_RESUME;
          break;

        case V_PCH_ACPI_PM1_CNT_S4:
          BootMode = BOOT_ON_S4_RESUME;
          break;

        case V_PCH_ACPI_PM1_CNT_S5:
          BootMode = BOOT_ON_S5_RESUME;
          break;
      } // switch (SleepType)
    }


    if (PciRead8(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_3)) & B_PCH_LPC_GEN_PMCON_RTC_PWR_STS) {
      BootMode = BOOT_WITH_DEFAULT_SETTINGS;
    }
  }
  // AptioV Server override - start: For AMI Recovery support

    PciCfg = (EFI_PEI_PCI_CFG_PPI *)(*PeiServices)->PciCfg;
    CpuIo = (*PeiServices)->CpuIo;
    if (IsRecovery(PeiServices, PciCfg, CpuIo)){
        DEBUG((EFI_D_ERROR, "Setting BOOT_IN_RECOVERY_MODE, IsRecovery returned TRUE\n"));
      BootMode = BOOT_IN_RECOVERY_MODE;
      Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListRecoveryBootMode);
      ASSERT_EFI_ERROR(Status);
    } 
  // AptioV Server override - End: For AMI Recovery support

  switch (PlatformType) {

#ifndef GRANGEVILLE_FLAG
    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCitySRPDDR3:
    case TypeMayanCityCRBDDR3:
    case TypeWildcatPass:
    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
    case TypeHalfWidthCitySRP:


    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
    case TypeEmeraldPoint: 

        //TODO: Add boot specific code here
        break;
#else
    case TypeBeverlyCoveCRB:
	case TypeBeverlyCoveSODimmCRB:
        break;

    case TypeTrabucoCanyon:
    case TypePiestewaPeak:
    case TypeCamelbackMountain:
    case TypeTlaquepaque:
        break;

#endif

    default:
      DEBUG ((EFI_D_ERROR, "Unknown Platform Type Detected %x\n", PlatformType));
      ASSERT_EFI_ERROR (PlatformType < TypePlatformMax);
  } // switch (PlatformType)

  switch (BootMode) {
    case BOOT_WITH_FULL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION";
      break;
    case BOOT_WITH_MINIMAL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_MINIMAL_CONFIGURATION";
      break;
    case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
      strBootMode = L"BOOT_ASSUMING_NO_CONFIGURATION_CHANGES";
      break;
    case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS";
      break;
    case BOOT_WITH_DEFAULT_SETTINGS:
      strBootMode = L"BOOT_WITH_DEFAULT_SETTINGS";
      break;
    case BOOT_ON_S4_RESUME:
      strBootMode = L"BOOT_ON_S4_RESUME";
      break;
    case BOOT_ON_S5_RESUME:
      strBootMode = L"BOOT_ON_S5_RESUME";
      break;
    case BOOT_ON_S2_RESUME:
      strBootMode = L"BOOT_ON_S2_RESUME";
      break;
    case BOOT_ON_S3_RESUME:
      strBootMode = L"BOOT_ON_S3_RESUME";
      break;
    case BOOT_ON_FLASH_UPDATE:
      strBootMode = L"BOOT_ON_FLASH_UPDATE";
      break;
    case BOOT_IN_RECOVERY_MODE:
      strBootMode = L"BOOT_IN_RECOVERY_MODE";
      break;
	case BOOT_WITH_MFG_MODE_SETTINGS:
      strBootMode = L"BOOT_IN_MANUFACTURING_MODE";	
	  break;
    default:
      strBootMode = L"Unknown boot mode";
  } // switch (BootMode)
  DEBUG ((EFI_D_ERROR, "Setting BootMode to %s\n", strBootMode));

  Status = PeiServicesSetBootMode (BootMode);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

BOOLEAN
GetSleepTypeAfterWakeup (
  IN  EFI_PEI_SERVICES          **PeiServices,
  OUT UINT16                    *SleepType
  )
/*++

Routine Description:

  Get sleep type after wakeup
  
Arguments:

  PeiServices       Pointer to the PEI Service Table.
  SleepType         Sleep type to be returned.

Returns:

  TRUE              A wake event occured without power failure.
  FALSE             Power failure occured or not a wakeup.

--*/
{
  UINT16  Pm1Sts;
  UINT16  Pm1Cnt;

  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS);
  Pm1Cnt  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT);


  //
  // Get sleep type if a wake event occurred and there is no power failure
  //
  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_WAK) != 0 &&
      ((PciRead8(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_3)) & B_PCH_LPC_GEN_PMCON_PWR_FLR) == 0) &&
      ((Pm1Sts & B_PCH_ACPI_PM1_STS_PRBTNOR) == 0)) {
    *SleepType = Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP;
    return TRUE;
  }
  
  //
  // Make sure to clear WAK STS bit since we decided it is not returning from S3
  //
  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_WAK) != 0) {
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS, (B_PCH_ACPI_PM1_STS_WAK));
  }
  
  return FALSE;
}
