/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c) 2009-2014, Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    WheaPlatformHooks.c
   
Abstract:
    Whea Platform hook functions and platform data, which needs to be 
    ported.

--*/


#include "Library\WheaPlatformHooksLib.h"
#include <PchRegs.h>
#include <Library\PciLib.h>
// AptioV Server Override Start: For Preserving NMI_EN bit
#include <Library\IoLib.h>
// AptioV Server Override End: For Preserving NMI_EN bit

// AptioV Server override Start: Program OEM ID and OEM table ID based on SDL token
#include <AcpiOemElinks.h>
#include <Library/BaseMemoryLib.h>
UINT8 PLAFTORM_OEM_ID[6]     = ACPI_OEM_ID_MAK;
UINT8 PLAFTORM_OEM_TBL_ID[8] = ACPI_OEM_TBL_ID_MAK;
// AptioV Server override End: Program OEM ID and OEM table ID based on SDL token

//
// Data definitions & structures
//
#define R_ACPI_GPE_CNTL                     0x42
#define EFI_ACPI_CREATOR_ID                 0x4C544E49  // "INTL"
#define EFI_ACPI_CREATOR_REVISION           0x00000001
#define EFI_ACPI_OEM_TABLE_ID               SIGNATURE_64 ('I', 'V', 'T', 'E', 'X', ' ', ' ') // OEM table id 8 bytes long
#define EFI_WHEA_OEM_REVISION               0x0001;
#define PCH_RCBA_ADDRESS                0xFED1C000  // PCH Root complex base address // AptioV Server Override: For Preserving NMI_EN bit
//////////////////////////////////////////////////////////////////////////
//                     PLATFORM HOOK DATA & TABLES                      //
//                     PLATFORM HOOK DATA & TABLES                      //
//                     PLATFORM HOOK DATA & TABLES                      //
//////////////////////////////////////////////////////////////////////////

//
// Error Sources that are supported in the platform & chipset
//

// HW Error notification method used for FW first model
#define     ERROR_STATUS_GAS_DATADEF        {0, 64, 0, 0, 0x0000}
#define     SCI_ERROR_NOTIFY_DATADEF        {ERR_NOTIFY_SCI, sizeof(HW_NOTIFY_METHOD), 0, 0, 0, 0, 0, 0, 0}
#define     NMI_ERROR_NOTIFY_DATADEF        {ERR_NOTIFY_NMI, sizeof(HW_NOTIFY_METHOD), 0, 0, 0, 0, 0, 0, 0}
#define     POLLED_ERROR_NOTIFY_DATADEF     {ERR_NOTIFY_POLLING, sizeof(HW_NOTIFY_METHOD), 0x3E, 60000, 2, 0, 0, 0, 0} // AptioV Server Override: Added support for Polling for Corrected error

//
// Generic error HW error sources - Tip: list all FW First error sources
// Note: Used 1KB Error Status block for each error source
//
//  RelSrcId,   MaxErrData, ErrStsAddr,                Notify method,           Max ErrSts block Size
GENERIC_HW_ERROR_SOURCE         PlatformCeSrc = \
    {0xffff,    4096,       ERROR_STATUS_GAS_DATADEF,  POLLED_ERROR_NOTIFY_DATADEF,  4096}; // AptioV Server Override: Added support for Polling for Corrected error
GENERIC_HW_ERROR_SOURCE         PlatformUeSrc = \
    {0xffff,    4096,       ERROR_STATUS_GAS_DATADEF,  NMI_ERROR_NOTIFY_DATADEF,  4096};

SYSTEM_GENERIC_ERROR_SOURCE         SysGenErrSources[] = {
//  Type,                     SrcId,  Flags,  NumOfRecords   MaxSecPerRecord     SourceInfo
    {GENERIC_ERROR_CORRECTED,   0,      0,         1,              1,            &PlatformCeSrc},
    {GENERIC_ERROR_FATAL,       0,      0,         1,              1,            &PlatformUeSrc},
};
UINTN NumSysGenericErrorSources     = (sizeof(SysGenErrSources)/sizeof(SYSTEM_GENERIC_ERROR_SOURCE));

//
// Native error sources - Tip: Errors that can be handled by OS directly
// Note: Used 1KB Error Status block for each error source
//
HW_NOTIFY_METHOD NotifyMthd ={0,sizeof(HW_NOTIFY_METHOD),0,0,0,0x9,0,0x9,0};

SYSTEM_NATIVE_ERROR_SOURCE          SysNativeErrSources[] = {
00
};


//UINTN NumNativeErrorSources         = (sizeof(SysNativeErrSources)/sizeof(SYSTEM_NATIVE_ERROR_SOURCE));
UINTN NumNativeErrorSources         = 00;

UINT32 mWheaPlatformType     = 0xFFFF;
UINT16 mWheaPlatformFlavor   = 0xFFFF;

//////////////////////////////////////////////////////////////////////////
//                      PLATFORM HOOK FUNCTIONS                         //
//                      PLATFORM HOOK FUNCTIONS                         //
//                      PLATFORM HOOK FUNCTIONS                         //
//////////////////////////////////////////////////////////////////////////

EFI_STATUS
InitWheaPlatformHookLib (void)
{
/* uncomment after moving to Platform package
  EFI_PLATFORM_TYPE_PROTOCOL  *PlatformType;
  EFI_STATUS                  Status;
  Status  = EFI_SUCCESS;

  if (mWheaPlatformType == 0xFFFF) {
    Status = gBS->LocateProtocol (
      &gEfiPlatformTypeProtocolGuid,
      NULL,
      &PlatformType
      );
    if(EFI_ERROR(Status)) {
       mWheaPlatformType = PlatformType->Type;
    }
  }
  return Status;
*/
return EFI_SUCCESS;
}

//
// Update Platform & OEM ID's in Whea ACPI tables.
//
VOID
UpdateAcpiTableIds(
  EFI_ACPI_DESCRIPTION_HEADER     *TableHeader
  )
{
//
// Update the OEMID, Creator ID, and Creator revision.
//

// AptioV Server override Start: Program OEM ID and OEM table ID based on SDL token
  //*(UINT32 *) (TableHeader->OemId)      = 'I' + ('N' << 8) + ('T' << 16) + ('E' << 24);
  //*(UINT16 *) (TableHeader->OemId + 4)  = 'L' + (' ' << 8);
    CopyMem (TableHeader->OemId, PLAFTORM_OEM_ID, 6);
    CopyMem ((UINT8 *)&TableHeader->OemTableId, PLAFTORM_OEM_TBL_ID, 8);
// AptioV Server override End: Program OEM ID and OEM table ID based on SDL token

  TableHeader->CreatorId                = EFI_ACPI_CREATOR_ID;
  TableHeader->CreatorRevision          = EFI_ACPI_CREATOR_REVISION;
// TableHeader->OemTableId               = EFI_ACPI_OEM_TABLE_ID;
//
// Update OEM revision and OEM TABLE ID based on the platform/SKU
//
  TableHeader->OemRevision              = EFI_WHEA_OEM_REVISION;
}

EFI_STATUS
WheaGetMemoryFruInfo (
  IN MEMORY_DEV_INFO    *MemInfo,
  OUT EFI_GUID          *FruId,
  OUT CHAR8             *FruString,
  OUT UINT16            *Card
)
{
  CHAR8                     SlotStr[] = {'D', 'I', 'M', 'M', ' ', '?', '?', 0};
  UINT32                    SlotLetter;

  //
  // Data for Mayan City memory.
  // FruString buffer is 20 bytes.
  //

  //
  // DIMM slot letter.
  //
  if ((MemInfo->ValidBits & PLATFORM_MEM_NODE_VALID) &&
      (MemInfo->ValidBits & PLATFORM_MEM_CARD_VALID)) {
    SlotLetter = (((UINT32) (MemInfo->Node)) << 1) + (UINT32) (MemInfo->Channel) + 'A';
    if (SlotLetter <= 'Z') {
      SlotStr[5] = (CHAR8) (SlotLetter);
    }
  }

  //
  // DIMM slot number within channel (1-based).
  //
  if (MemInfo->ValidBits & PLATFORM_MEM_MODULE_VALID) {
    SlotStr[6] = (CHAR8) (MemInfo->Dimm + '1');
  }

  AsciiStrCpy (FruString, SlotStr);

  //
  // This is the channel information on the memory controller on the CPU socket (0-based).
  //
  if (MemInfo->ValidBits & PLATFORM_MEM_CARD_VALID) {
    *Card = MemInfo->Channel;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
TriggerWheaSci (
  EFI_SMM_BASE2_PROTOCOL         *SmmBase,
  EFI_SMM_SYSTEM_TABLE2          *Smst
  )
{
  UINT16      PmBase;
  UINT8       GpeCntl;
  UINT16      GpeSts;

  //
  // Get the PM Base Address
  //
  PmBase = PciRead16 (
                  PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  R_PCH_LPC_ACPI_BASE)
                  );

  //
  // Base Address should be in bits 15:7 of PMBASE register 9.1.1 page 280, but 0x400 is bigger
  // so they use the whole 16 bit
  //
  PmBase = (UINT16) (PmBase & 0xFF80);

  //SWGPE_EN and SCI_EN should be set to 1(This is already set)
  //Setting SWGPE_CTRL to 1 to trigger SCI (This will set the SWGPE_STS register to raise SCI)
  Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT8, (PmBase + R_ACPI_GPE_CNTL), 1, &GpeCntl);
  GpeCntl |= BIT1;   //SWGPE_CTRL;
  Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, (PmBase + R_ACPI_GPE_CNTL), 1, &GpeCntl);

  //
  // Status Check
  //
  while(TRUE) {
	Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT16, PmBase + R_PCH_ACPI_GPE0a_STS , 1, &GpeSts );
	  if(GpeSts & BIT2)  {
	    break;
	  }
  }

  return EFI_SUCCESS;

}

EFI_STATUS
TriggerWheaNmi (
  EFI_SMM_BASE2_PROTOCOL         *SmmBase,
  EFI_SMM_SYSTEM_TABLE2          *Smst
  )
{
  UINT16          PmBase;
  UINT8           Data;
  UINT8           Save_Nmi2Smi_En;
  UINT8           Save_Port70;
  UINT16          Data16;
  EFI_STATUS      Status;

  //
  // Get the PM Base Address
  //
  PmBase = PciRead16 (
                  PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  R_PCH_LPC_ACPI_BASE)
                  );
 
  PmBase = (UINT16) (PmBase & 0xFF80);

  // Read the NMI2SMI_EN bit, save it for future restore
  Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT8, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), 1, &Save_Nmi2Smi_En);

  // Set the NMI2SMI_EN bit to 0
  // This also clears NMI_NOW if it is already set
  Data = (UINT8)(Save_Nmi2Smi_En & 0xFD);
  Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), 1, &Data);

// AptioV Server Override Start: For Preserving NMI_EN bit
  // Enable Alternate Access Mode
  MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32)B_PCH_RCRB_GCS_AME);
  while ((MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS) & B_PCH_RCRB_GCS_AME) == 0);

  // Read Port 70h and save NMI_EN bit
  Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT8, R_PCH_NMI_EN, 1, &Save_Port70);

  // Disble Alternate Access Mode
  MmioAnd32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32) ~(B_PCH_RCRB_GCS_AME));
  while (MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS) & B_PCH_RCRB_GCS_AME);
// AptioV Server Override End: For Preserving NMI_EN bit

  // Enable NMI_EN
//  Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT8, 0x74, 1, &Save_Port70); // AptioV Server Override Start: For Preserving NMI_EN bit, Commenting this line as we are reading the bit in Alternate Access Mode
  Data = (UINT8)(Save_Port70 & 0x7F);
  Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, R_PCH_NMI_EN, 1, &Data);

  // Set NMI_NOW = 1
  Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT8, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), 1, &Data);
  Data = (UINT8) (Data | 0x01);
  Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), 1, &Data);

  // AptioV Server Override: For preserving NMI2SMI_EN bit
  // Restore NMI2SMI_EN
  Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), 1, &Save_Nmi2Smi_En);

  // Clear the MCHSERR_STS bit, bit 12
  Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT16, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_STS)), 1, &Data16);
  Data16 = (Data16 | 0x1000);
  Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT16, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_STS)), 1, &Data16);

  // Clear the NMI2SMI_STS bit if set
  Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT16, (PmBase + (PCH_TCO_BASE + R_PCH_TCO1_STS)), 1, &Data16);
  if (Data16 & 0x0001) {
    // check port 0x61
    Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT8, R_PCH_NMI_SC, 1, &Data);
    if (Data & 0x80) {
      Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT8, R_PCH_NMI_SC, 1, &Data);
      Data = (UINT8) (Data | 0x04);
      Data = (UINT8) (Data & 0x0F);
      Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, R_PCH_NMI_SC, 1, &Data);
      Data = (UINT8) (Data & 0x0B);
      Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, R_PCH_NMI_SC, 1, &Data);
    }
  }

  // Restore NMI_EN
  Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT8, R_PCH_NMI_EN, 1, &Save_Port70);

  return EFI_SUCCESS;
}

EFI_STATUS
ProgramWheaSignalsHook (
  BOOLEAN                     EnableWhea,
  EFI_SMM_BASE2_PROTOCOL       *SmmBase,
  EFI_SMM_SYSTEM_TABLE2        *Smst
  )
{
  UINT32                      Data32;
  UINT32                      GpiRouteData;
  UINT16                      PmBase;
  EFI_STATUS                  Status;

  //
  // Get the PM Base Address
  //
  PmBase = PciRead16 (
                  PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  R_PCH_LPC_ACPI_BASE)
                  );

  //
  // Base Address should be in bits 15:7 of PMBASE register 9.1.1 page 280, but 0x400 is bigger
  // so they use the whole 16 bit
  //
  PmBase = (UINT16) (PmBase & 0xFF80);

  //
  // Update OEM revision and OEM TABLE ID 
  //
      //
  // Clear ALT_GPI_SMI_EN for GPI0 so that it doesn't cause a SMI
      //
      Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT32, (PmBase + R_PCH_ALT_GP_SMI_EN), 1, &Data32);
      Data32 &= ~BIT0; 
      Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT32, (PmBase + R_PCH_ALT_GP_SMI_EN), 1, &Data32);

      //
  // Set GPE0_EN for GPI0 to generate SCI when asserted
      //
      // GPIn_EN - R/W. These bits enable the corresponding GPI[n]_STS bits being set to cause a SCI,
      // and/or wake event. These bits are cleared by RSMRST#.

      Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT32, (PmBase + R_PCH_ACPI_GPE0a_EN), 1, &Data32);
      Data32 |= BIT16; 
      Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT32, (PmBase + R_PCH_ACPI_GPE0a_EN), 1, &Data32);

      //
      // Route the PCH GPI to generate SCI
      //
      GpiRouteData = PciRead32 (
                  PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  R_PCH_LPC_GPI_ROUT)
              );
      GpiRouteData = (GpiRouteData & ~(BIT1 | BIT0)) | BIT1;  
                    // Route GPI0 to SCI#.
      PciWrite32 (
                  PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  R_PCH_LPC_GPI_ROUT),
              GpiRouteData
              );

      //
      // Clear GPE0_STS in case SCI is asserted
      //
      Status = Smst->SmmIo.Io.Read (&Smst->SmmIo, SMM_IO_UINT32, (PmBase + R_PCH_ACPI_GPE0a_STS), 1, &Data32);
      Data32 |= BIT16; 
      Status = Smst->SmmIo.Io.Write (&Smst->SmmIo, SMM_IO_UINT32, (PmBase + R_PCH_ACPI_GPE0a_STS), 1, &Data32);

  return EFI_SUCCESS;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2009-2010 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

-----------------------------------------------------------------------------*/
