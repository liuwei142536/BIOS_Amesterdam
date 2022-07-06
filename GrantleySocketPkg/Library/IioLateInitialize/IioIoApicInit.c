/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioIoApicInit.c

Abstract:

  This file initialize the IoApic device on IIO

--*/
//Aptio V server override start: Include token.h for using NB_IOxAPIC_SUPPORT token and prevent macros redefinition.
#include <Token.h>
#include "IioLateInitialize.h"

VOID
Enable_IOxAPIC(
    IIO_GLOBALS         *IioGlobalData,
    UINT8           Iio
)
{
  UINT16    Data16;
  UINT8     Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

  DEBUG ((EFI_D_INFO, "Enable IIO[%d] IOxAPIC\n",Iio));
  Data16 = PciExpressRead16(PCI_PCIE_ADDR(Bus, APIC_DEV_NUM, APIC_FUNC_NUM, PCI_COMMAND_OFFSET));
  Data16 |= EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(Bus, APIC_DEV_NUM, APIC_FUNC_NUM, PCI_COMMAND_OFFSET), Data16);



}

VOID
IioIoApicInit (  
  PCI_ROOT_BRIDGE_PCI_ADDRESS                    PciAddress,
  IIO_INIT_PHASE                                 Phase,
  IIO_GLOBALS                                    *IioGlobalData
  )
/*++

Routine Description: Intialize IIO IoApic.

Arguments:
 
Returns:
  None
--*/
{  
  UINT32    IoApicAddress=0;
  UINT16    Data16;
  UINT32    Data32;
  UINT8     Data8=0;
  UINT8     i;
  UINT8     *Array = NULL;
  UINT8     BusNumber;

  for ( i = 0 ; i < MaxIIO ; i++ ) {
      if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) {
          break;
      }
  }

  // Exit if not IIO Core device/function
  if ( (i >= MaxIIO) || Phase != IIOInitPhase3) return;

  Array = &(IioGlobalData->SetupData.IOAPICHIDE[0]);
  if (Array[i]) return;


  IoApicAddress = (UINT32)IioGlobalData->IioVData.IoApicBase[i];
  if (IoApicAddress == (UINT32)IioGlobalData->IioVData.PchIoApicBase) {
      IoApicAddress = IoApicAddress + 0x1000;       // dont collide with PCH IOAPIC!!
  }
  
  
  DEBUG ((EFI_D_INFO, "Initialize IIO[%d] IOxAPIC...\n", i));
  
  //
  // Read enable/disable from Setup for this IIO
  //
  Array = &(IioGlobalData->IioVData.DevPresIoApicIio0);
  Data8 = Array[i];

  if (Data8) {
    //                                        
    // Enable IOxAPIC
    //
    //  first make sure the MBAR (offset 10h BAR) is initialized during the regular
    //  PCI enumeration, if not than register for the callback during boot event
    Data32 = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM,
              ONLY_REGISTER_OFFSET(MBAR_IIO_IOAPIC_REG)));
    Data32 &= 0xFFFFFFF0;

    //
    // If MBAR has already been assigned
    //
    if (Data32) {
      //
      // Enable PCI CMD reg bits and no need for a callack
      //
      Enable_IOxAPIC(IioGlobalData, i);
      IioGlobalData->IioVData.IOxAPICCallbackBootEvent = FALSE;
    } else {
      //
      // Else set a callback event to set PCI CMD reg bits
      //
      IioGlobalData->IioVData.IOxAPICCallbackBootEvent = TRUE;
    }
  } else {
    //
    // IOxAPIC is disabled in Setup
    // Hide IOxAPIC PCI device
    //
    BusNumber = IioGlobalData->IioVData.SocketUncoreBusNumber[i]; // uncore bus here
    Data32 = PciExpressRead32(PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM,
                  ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG) + (APIC_FUNC_NUM * sizeof(UINT32))));
    Data32 |= (1 << APIC_DEV_NUM);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM,
        ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG) + (APIC_FUNC_NUM * sizeof(UINT32))), Data32);
    return;
  }

  //
  // Set IOAPIC ABAR
  //
  Data16 = B_APIC_ABAR_ABAR_EN | (0x00000FFF & (UINT16)(IoApicAddress >> 8));
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM, ONLY_REGISTER_OFFSET(ABAR_IIO_IOAPIC_REG)), Data16);
  DEBUG ((EFI_D_INFO, "IIO[%d] IOxAPIC Base=%x\n",i,IoApicAddress));

  //
  // Enable accesses by us and the OS to the IOAPIC register address area from 0xFC00_0000 to 0xFEFF_FFFF
  // Block accesses from 0xFF00_0000 to 0xFFFF_FFFF by anybody but us
  //
  Data32 = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IIO_VTD_DEV, IIO_VTD_FUN, ONLY_REGISTER_OFFSET(TOMMIOL_OB_IIO_VTD_REG)));
  Data32 &= ~0xFFF00000;
  Data32 |= 0xFEF00000;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IIO_VTD_DEV, IIO_VTD_FUN, ONLY_REGISTER_OFFSET(TOMMIOL_OB_IIO_VTD_REG)), Data32);
  DEBUG ((EFI_D_INFO, "IIO[%d] TOMMIOL_OB = %08X\n",i,Data32));

  if (FeaturePcdGet (PcdLockCsrSsidSvidRegister)) {
    //
    // Lock Subsystem Vendor ID
    //
    Data16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM, PCI_SVID_OFFSET));
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM, PCI_SVID_OFFSET), Data16);
    Data32 = (UINT32)Data16;
  
    //
    // Lock Subsystem Device ID
    //
    Data16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM, PCI_SID_OFFSET));
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, 
                    APIC_FUNC_NUM, PCI_SID_OFFSET), Data16);
    Data32 |= ((UINT32)Data16 << 16);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32,
            IioGlobalData->IioVData.PciExpressBase + PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM, PCI_SVID_OFFSET),
            1, &Data32 );
  }

  //
  // Lock PMCAP
  //
  Data32 = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM, ONLY_REGISTER_OFFSET(PMCAP_IIO_IOAPIC_REG))); //fix for s4030546
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, APIC_DEV_NUM, APIC_FUNC_NUM, ONLY_REGISTER_OFFSET(PMCAP_IIO_IOAPIC_REG)), Data32);
}

VOID
IoApicPostInit(
    IIO_GLOBALS                                     *IioGlobalData
)
{
  UINT32      Data32;
  UINT8       Iio;
#ifdef LT_FLAG  

  EFI_HANDLE Handle;
  VOID *     Protocol;
  EFI_STATUS Status;

  //
  // We add a protocol here to hint that scheck is OK to be performed
  //
  Handle = NULL;

  Status = gBS->LocateProtocol (&gReadyForLockProtocolGuid, NULL, &Protocol);
  if (EFI_ERROR(Status)){
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gReadyForLockProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
  }
#endif
  for ( Iio = 0; Iio < MaxIIO; Iio++ ) {
    if(IioGlobalData->IioVData.SocketPresent[Iio]) {
      //
      // Importing the permanent workaround - s3875843
      //  (Interrupts dropped intermittently due to faulty workround)
      //  Note: applicable to IVT
      Data32 = PciExpressRead32(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[Iio],
                CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(IIOMISCCTRL_N1_IIO_VTD_REG)));
      Data32 |= BIT7;   //set disable_new_apic_ordering to 1
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[Iio],
          CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(IIOMISCCTRL_N1_IIO_VTD_REG)), Data32);
    }
  }

}

VOID
IioIoApicInitBootEvent(
    IIO_GLOBALS         *IioGlobalData,
    UINT8           Iio
)
{
  UINT8     *Array = NULL;

  if(IioGlobalData->IioVData.IOxAPICCallbackBootEvent == FALSE)    //no boot event registered
    return;

#if NB_IOxAPIC_SUPPORT   //AptioV server override : Re-enable IIO APIC decode only of IOAPIC SDL control is enabled
  switch(Iio) {   //fix for s4031021
  case 0:
    Array = &(IioGlobalData->IioVData.DevPresIoApicIio0);
    break;
  case 1:
    Array = &(IioGlobalData->IioVData.DevPresIoApicIio1);
    break;
  case 2:
    Array = &(IioGlobalData->IioVData.DevPresIoApicIio2);
    break;
  case 3:
    Array = &(IioGlobalData->IioVData.DevPresIoApicIio3);
    break;
  default:
    return;
  }

  if( *Array )
    Enable_IOxAPIC(IioGlobalData, Iio);
#endif   //AptioV server override : Re-enable IIO APIC decode only of IOAPIC SDL control is enabled
}
