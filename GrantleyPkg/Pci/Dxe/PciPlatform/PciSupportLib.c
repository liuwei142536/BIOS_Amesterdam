/*++
  This file contains a 'Sample Driver' and is licensed as such  
  under the terms of your license agreement with Intel or your  
  vendor.  This file may be modified by the user, subject to    
  the additional terms of the license agreement                 
--*/
/*++ 

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
    PciSupportLib.c

Abstract:

  Support PCI chipset initialization. 

--*/
#include "PiDxe.h"
#include <Base.h>
#include <Guid/SetupVariable.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "IndustryStandard\Pci.h"
#include <Iio\PciExpress.h>
#include "PciSupportLib.h"
#include "PchRegs.h"
#include "IioRegs.h"
#include <Library/PlatformHooksLib.h>

PCIE_STACK  mPcieStack;

BOOLEAN
IsAnIioRootPort(
    IN  UINT8   Bus,
    IN  UINT8   Device,
    IN  UINT8   Function
);

EFI_STATUS
GetIioPortMaxPayloadSizePlatformPolicy(
    IN  UINT8   Bus,
    IN  UINT8   Device,
    IN  UINT8   Function,
    OUT UINT8   *PortPayloadSize
);

EFI_STATUS
GetIioPortAspmPlatformPolicy(
    IN  UINT64   PciAddress,
    OUT UINT8    *Aspm
);

BOOLEAN
IsPciDevicePresent (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *PciRootBridgeIo,
  PCI_TYPE00                          *Pci,
  UINT8                               Bus,
  UINT8                               Device,
  UINT8                               Func
  )
/*++

Routine Description:

  This routine is used to check whether the pci device is present

Arguments:

Returns:

  None

--*/
// TODO:    PciRootBridgeIo - add argument and description to function comment
// TODO:    Pci - add argument and description to function comment
// TODO:    Bus - add argument and description to function comment
// TODO:    Device - add argument and description to function comment
// TODO:    Func - add argument and description to function comment
// TODO:    EFI_SUCCESS - add return value to function comment
// TODO:    EFI_NOT_FOUND - add return value to function comment
{
  UINT64      Address;
  UINT32      Dummy;
  EFI_STATUS  Status;
  
  Dummy=0xFFFFFFFF;
  //
  // Create PCI address map in terms of Bus, Device and Func
  //
  Address = EFI_PCI_ADDRESS (Bus, Device, Func, 0);

  //
  // Read the Vendor Id register
  //
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32,
                                  Address,
                                  1,
                                  Pci
                                  );
  /// WA Sighting 4168813. Add an extra write cycle as for LSI SAS cards 
  if ((Pci->Hdr).VendorId == 0xffff) {
    /// PCIe card could have been assigned a temporary bus number. 
	/// An write cycle can be used to try to rewrite the Bus number in the card
    /// Try to write the Vendor Id register, and recheck if the card is present. 
    Status = PciRootBridgeIo->Pci.Write(
                                    PciRootBridgeIo, 
                                    EfiPciWidthUint32, 
                                    Address, 
                                    1,							  
                                    &Dummy
                                    ); 
									
	// Retry the previous read after the PCI cycle has been tried. 
    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    1,
                                    Pci
                                    );
  }

  if (!EFI_ERROR (Status) && (Pci->Hdr).VendorId != 0xffff) {

    //
    // Read the entire config header for the device
    //

    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    sizeof (PCI_TYPE00) / sizeof (UINT32),
                                    Pci
                                    );

    return TRUE;
  }

  return FALSE;
}

EFI_STATUS
FindPciCapPtr (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL              *PciRootBridgeIo,
  IN     UINT8                                         CapID,
  IN     UINT64                                        PciAddress,
  IN OUT UINT8                                         *CapPtr
  )
/*++
  
  Routine Description:
  
    Find the Offset to a given Capabilities ID
    CAPID list:
      0x01 = PCI Power Management Interface
      0x04 = Slot Identification
      0x05 = MSI Capability
      0x10 = PCI Express Capability
    
  Arguments:
    CapId        -   CAPID to search for  
    PciAddress   -   Pci Bus Number
    CapPtr       -   return CAPID 

  Returns:
  
    EFI_SUCCESS    - CAPID found
    Other          - CAPID not found

--*/
{
  UINT8       Data8;
  EFI_STATUS  Status;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  *PciAddressPtr;

  PciAddressPtr = (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  *)&PciAddress;

  //
  // Read Reg 06 Status Register
  //
  PciAddressPtr->ExtendedRegister = 0;
  PciAddressPtr->Register = PCI_PRIMARY_STATUS_OFFSET;
  Status = PciRootBridgeIo->Pci.Read (
                                   PciRootBridgeIo,
                                   EfiPciWidthUint8,
                                   PciAddress,
                                   1,
                                   &Data8
                                   );
  //
  // Check capabilities list enable
  //
  if ((Data8 & BIT4) == 0) {
    return EFI_UNSUPPORTED;
  }

  //
  // Read Register Capilities Pointer
  //
  PciAddressPtr->Register = PCI_CAPBILITY_POINTER_OFFSET;
  do {
    //
    // Keep looking for a pointer ID that match the flag
    // Read the Capabilities Pointer
    //
    Data8 = 0;
    Status = PciRootBridgeIo->Pci.Read (
                                     PciRootBridgeIo,
                                     EfiPciWidthUint8,
                                     PciAddress,
                                     1,
                                     &Data8
                                     );

    if (Data8 == 0 || Data8 == 0xFF) {
      return EFI_UNSUPPORTED;
    }
 
    PciAddressPtr->Register = Data8;

    //
    // Keep the capability pointer.
    //
    *CapPtr = PciAddressPtr->Register;

    //
    // read the CapID of the pointer
    //
    Status = PciRootBridgeIo->Pci.Read (
                                     PciRootBridgeIo,
                                     EfiPciWidthUint8,
                                     PciAddress,
                                     1,
                                     &Data8
                                     );
    PciAddressPtr->Register++;
  } while (Data8 != CapID);

  //
  // found the capabilities pointer
  //
  return EFI_SUCCESS;
}


VOID
PciRootBridgeIoRw (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL              *PciRootBridgeIo,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH        Width,
  IN UINT64                                       Address,
  IN UINT32                                       SetValue,
  IN UINT32                                       ClrValue
  )
/*++

Routine Description:
  Read/Write Pci register. Width can NOT be 64 bits.

Arguments:
  Pci Root Bridge Io Protocol, Width, Pci Address, Mask, and Value to or.

Returns:
  Nothing

--*/

// GC_TODO:    This - add argument and description to function comment
// GC_TODO:    Width - add argument and description to function comment
// GC_TODO:    Address - add argument and description to function comment
// GC_TODO:    SetValue - add argument and description to function comment
// GC_TODO:    ClrValue - add argument and description to function comment
{
  EFI_STATUS  Status;
  UINT32      Data;

  Status = PciRootBridgeIo->Pci.Read (PciRootBridgeIo, Width, Address, 1, &Data);
  ASSERT_EFI_ERROR (Status);

  Data    = (Data &~ClrValue) | SetValue;
  Status  = PciRootBridgeIo->Pci.Write (PciRootBridgeIo, Width, Address, 1, &Data);
  ASSERT_EFI_ERROR (Status);
}

STATIC
VOID
ResetPcieStack (
  VOID
)
{
  mPcieStack.Top = 0;
}

STATIC
VOID
PushPcieStack (
  IN UINT64 PciAddress
)
{
  ASSERT (mPcieStack.Top != MAX_NESTED_LEVEL);
  mPcieStack.PcieCapInfo[mPcieStack.Top++] = *(PCIE_CAP_INFO *)&PciAddress;
}

STATIC
VOID
PopPcieStack (
  VOID
)
{
  if (mPcieStack.Top > 0) {
    mPcieStack.Top--;
  }
}

STATIC 
VOID
SetPcieAspmPath (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo
)
{
  INTN   Index;
  INTN   EntryNumber;
  UINT32 LinkCap;
  UINT32 DevCap;
  UINT64 PciAddress;
  UINT64 TempPciAddress;
  UINT8  AspmSupport;
  UINT8  L0LinkLatency;
  UINT8  L1LinkLatency;
  UINT8  L0DeviceAcceptLatency;
  UINT8  L1DeviceAcceptLatency;
  UINT32 L0L1Support;
  UINT8  Aspm;
//
// ESS_OVERRIDE_START
//

  UINT16 InstalledIohStepping;

  PciAddress = EFI_PCI_ADDRESS (0, 0, 0, 0x8);  // read IOH stepping
  PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint16,
                                  PciAddress,
                                  1,
                                  &InstalledIohStepping
                                  );
//
// ESS_OVERRIDE_END
//

  ASSERT ((mPcieStack.Top > 0) && (!(mPcieStack.Top & 1)));
  
  if ( (IsAnIioRootPort(mPcieStack.PcieCapInfo[0].Bus, mPcieStack.PcieCapInfo[0].Device, mPcieStack.PcieCapInfo[0].Function) == TRUE ) ||
        (IsPchRootPort(mPcieStack.PcieCapInfo[0].Bus, mPcieStack.PcieCapInfo[0].Device, mPcieStack.PcieCapInfo[0].Function) == TRUE)) {
    //
    // Fill the PCIE link and device capability along the path
    //
    EntryNumber = mPcieStack.Top;
    for (Index = 0; Index < EntryNumber; Index++) {
      PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
      TempPciAddress = PciAddress + PCIE_DEVICE_CAPABILITY_OFFSET;
      PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32, 
                                  TempPciAddress,  
                                  1, 
                                  &DevCap
                                  );      
    TempPciAddress = PciAddress + PCIE_LINK_CAPABILITY_OFFSET; 
    PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32, 
                                  TempPciAddress,  
                                  1, 
                                  &LinkCap
                                  );      

    mPcieStack.PcieCapInfo[Index].PcieLnkCap = (UINT16)((LinkCap >> 10) & 0xFF);
    mPcieStack.PcieCapInfo[Index].PcieDevCap = (UINT16)((DevCap >> 6) & 0x3F);
  }

  //
  // Check support for L0s and L1
  //
  //
  AspmSupport   = 0x03;
  L0LinkLatency = 0;
  L1LinkLatency = 0;
  for (Index  = 0; Index < EntryNumber; Index++) {
    AspmSupport    &= mPcieStack.PcieCapInfo[Index].PcieLnkCap;
    PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
    //
    // for the root port obtain the platform policy for aspm based on user configuration in setup 
    // since lnkcap on root port always reflects L1 capable
    // 
    if((IsAnIioRootPort(mPcieStack.PcieCapInfo[0].Bus, mPcieStack.PcieCapInfo[0].Device, mPcieStack.PcieCapInfo[0].Function) == TRUE )){
      if(GetIioPortAspmPlatformPolicy( PciAddress, &Aspm) == EFI_SUCCESS) { 
        AspmSupport  &= Aspm;
      }
    }else if((IsPchRootPort(mPcieStack.PcieCapInfo[0].Bus, mPcieStack.PcieCapInfo[0].Device, mPcieStack.PcieCapInfo[0].Function) == TRUE)){
       if(GetPchPortAspmPlatformPolicy(PciAddress, &Aspm) == EFI_SUCCESS) { 
        AspmSupport  &= Aspm;
      }
    }
//
// ESS_OVERRIDE_START
//
// Bug fix in L0,L1 Link latency calculation -- previous bitshifts for PcieLnkCap were incorrect
//
    L0LinkLatency = (UINT8)MAX ( ((mPcieStack.PcieCapInfo[Index].PcieLnkCap >> 2) & 0x07) , L0LinkLatency );
    L1LinkLatency = (UINT8)MAX ( ((mPcieStack.PcieCapInfo[Index].PcieLnkCap >> 5) & 0x07) , L1LinkLatency );
//
// ESS_OVERRIDE_END
//
  }
  
  L0L1Support = 0;
  if (AspmSupport & BIT0) {
    //
    // Support L0s
    //
    L0DeviceAcceptLatency = (UINT8)(mPcieStack.PcieCapInfo[EntryNumber-1].PcieDevCap >> 0) & 0x07;
    if (L0DeviceAcceptLatency >= L0LinkLatency) {
      L0L1Support |= BIT0;
    }
  }

  if (AspmSupport & BIT1) {
    //
    // Support L1
    //
    L1DeviceAcceptLatency = (UINT8)(mPcieStack.PcieCapInfo[EntryNumber-1].PcieDevCap >> 3) & 0x07;
    if (L1DeviceAcceptLatency >= L1LinkLatency) {
      L0L1Support |= BIT1;      
    }
  }

  if (L0L1Support > 0) {   
    for (Index = 0; Index < EntryNumber; Index++) {
      PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
        TempPciAddress = PciAddress + PCIE_LINK_CONTROL_OFFSET; 

        PciRootBridgeIoRw (
          PciRootBridgeIo,
          EfiPciWidthUint16,
          TempPciAddress,
          L0L1Support,
          BIT0 | BIT1
        );
      }
   }
}
}

STATIC 
VOID
SetPcieLtrPath (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo
)
{
  INTN                                  Index;
  INTN                                  EntryNumber;
  UINT32                                Dev_Cap2;
  UINT64                                PciAddress;
  UINT64                                TempPciAddress;
  EFI_STATUS                            Status;
  UINT64                                Size;
  SYSTEM_CONFIGURATION                  SystemConfiguration;

  ASSERT ((mPcieStack.Top > 0) && (!(mPcieStack.Top & 1)));

  //
  // Get the setup value for PCIe LTR support.  
  //
  Size = sizeof (SystemConfiguration);
  Status = gRT->GetVariable(L"IntelSetup", 
                           &gEfiSetupVariableGuid,
                           NULL,
                           &Size,
                           &SystemConfiguration);
  if (EFI_ERROR (Status)) 
    SystemConfiguration.PCIe_LTR = 1;     // Set it to Enable (Default)

  EntryNumber = mPcieStack.Top;
  if (SystemConfiguration.PCIe_LTR == 1)  { // Enable
    //
    // Fill the PCIE  device capability2 along the path
    //
    for (Index = 0; Index < EntryNumber; Index++) {
      PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
      TempPciAddress = PciAddress + PCIE_DEVICE_CAPABILITY2_OFFSET; 
      PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32,
                                  TempPciAddress,
                                  1,
                                  &Dev_Cap2
                                  );
      mPcieStack.PcieCapInfo[Index].PcieDevCap2 = (UINT16)(Dev_Cap2);
  
      if ((mPcieStack.PcieCapInfo[Index].PcieDevCap2 & B_PCIE_PORT_LTR) == 0) { // Check if device supports the LTR mechanism
        //
        // LTR is not supported in the current path therefore must exit since per PCIe Spec:
        // "It is not required that all Endpoints support LTR to permit
        // enabling LTR in those Endpoints that do support it".
        //
        return;
      }
    }
    Dev_Cap2 = B_PCIE_PORT_PXPDEVCTRL2_LTR_EN;
  } else {  // Disable
    Dev_Cap2 = 0;
  }

  for (Index = 0; Index < EntryNumber; Index++) {
    //
    // Enable the LTR mechanism
    //
    PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
    TempPciAddress = PciAddress + PCIE_DEVICE_CONTROL2_OFFSET;
    PciRootBridgeIoRw (
                        PciRootBridgeIo,
                        EfiPciWidthUint16,
                        TempPciAddress,
                        Dev_Cap2,
                        B_PCIE_PORT_PXPDEVCTRL2_LTR_EN
                      );
  }
}

STATIC 
VOID
SetPcieMaxPayloadPath (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo
)
{
  INTN   Index;
  INTN   EntryNumber;
  UINT32 DevCap;
  UINT64 PciAddress;
  UINT64 TempPciAddress;
  UINT32 MaxPayload;
  UINT8  PortPayloadSize;

  ASSERT ((mPcieStack.Top > 0) && (!(mPcieStack.Top & 1)));
  
  if(GetIioPortMaxPayloadSizePlatformPolicy(
                                             mPcieStack.PcieCapInfo[0].Bus,
                                             mPcieStack.PcieCapInfo[0].Device,
                                             mPcieStack.PcieCapInfo[0].Function,
                                             &PortPayloadSize) == EFI_SUCCESS) {
    //
    // Fill the PCIE device capability along the path
    //
    EntryNumber = mPcieStack.Top;
    MaxPayload = 0x07;

    for (Index = 0; Index < EntryNumber; Index++) {
      PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
      TempPciAddress = PciAddress + PCIE_DEVICE_CAPABILITY_OFFSET;
      PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32,
                                  TempPciAddress,
                                  1,
                                  &DevCap
                                  );
      mPcieStack.PcieCapInfo[Index].PcieDevCap = (UINT16)(DevCap & 0x07);
      //
      // Minimum payload size among the root port, bridge and endpoint device to be selected
      //
      MaxPayload = MIN ( mPcieStack.PcieCapInfo[Index].PcieDevCap, MaxPayload );
    }

    //
    // Compare against the platform policy (as also dictated by user) for the
    // root port max payload size
    if (PortPayloadSize != 2){
      //
      // Check if setup option is different from AUTO
      //   0: 128Bytes
      //   1: 256Bytes
      //   2: AUTO
      //
      // Set the MaxPayLoadSize based on the value of setup option
      //
      MaxPayload = MIN( MaxPayload, (UINT32)PortPayloadSize );
    }

    for (Index = 0; Index < EntryNumber; Index++) {
      PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
      TempPciAddress = PciAddress + PCIE_DEVICE_CONTROL_OFFSET;
      PciRootBridgeIoRw (
              PciRootBridgeIo,
              EfiPciWidthUint16,
              TempPciAddress,
              (MaxPayload << 5),
              BIT5 | BIT6 | BIT7
            );
      DEBUG((DEBUG_INFO, "Set max payload value %d to PCI addr = 0x%x\n", MaxPayload, TempPciAddress));
    }
  }
}

STATIC
VOID
SetPcieMaxReadRequestSize (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo
)
{
  INTN                       EntryNumber;
  UINT64                     PciAddress;
  UINT64                     TempPciAddress;
  EFI_STATUS                 Status;
  UINT64                     Size;
  SYSTEM_CONFIGURATION       SystemConfiguration;

  ASSERT ((mPcieStack.Top > 0) && (!(mPcieStack.Top & 1)));

  EntryNumber = mPcieStack.Top;

  //
  // Get the setup value for PCIe Max Read Request Size
  //
  Size = sizeof (SystemConfiguration);
  Status = gRT->GetVariable(L"IntelSetup",
                           &gEfiSetupVariableGuid,
                           NULL,
                           &Size,
                           &SystemConfiguration);
  if (EFI_ERROR (Status))
    SystemConfiguration.PcieMaxReadRequestSize = 7;     // Set it to AUTO (default)

  if (SystemConfiguration.PcieMaxReadRequestSize != 7) { // If knob is not AUTO
    PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[EntryNumber - 1])); // Set the MRRS in Endpoint
    TempPciAddress = PciAddress + PCIE_DEVICE_CONTROL_OFFSET;
    PciRootBridgeIoRw (
           PciRootBridgeIo,
           EfiPciWidthUint16,
           TempPciAddress,
           (SystemConfiguration.PcieMaxReadRequestSize << 12),
           BIT14 | BIT13 | BIT12
         );
    DEBUG((DEBUG_INFO, "Set Max Read Request Size value %d to PCI addr = 0x%x\n", SystemConfiguration.PcieMaxReadRequestSize, TempPciAddress));
  }
}

STATIC
VOID
SetExtendedTagSupport (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo
)
{
  INTN                      Index;
  INTN                      EntryNumber;
  UINT32                    DevCap;
  UINT32                    ExtTagFieldEnable;
  UINT64                    PciAddress;
  UINT64                    TempPciAddress;

  EFI_STATUS                Status;
  UINT64                    Size;
  SYSTEM_CONFIGURATION      SystemConfiguration;

  ASSERT ((mPcieStack.Top > 0) && (!(mPcieStack.Top & 1)));
  
  //
  // Fill the PCIE link and device capability along the path
  //
  EntryNumber = mPcieStack.Top;
  for (Index = 0; Index < EntryNumber; Index++) {
    PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
    TempPciAddress = PciAddress + PCIE_DEVICE_CAPABILITY_OFFSET; 
    PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32, 
                                  TempPciAddress,  
                                  1, 
                                  &DevCap
                                  );      
    mPcieStack.PcieCapInfo[Index].PcieDevCap =  (UINT16)(DevCap & BIT5);
  }

  //
  // Get the setup configuration 
  //
  Size = sizeof (SystemConfiguration);
  Status = gRT->GetVariable(L"IntelSetup", 
                           &gEfiSetupVariableGuid,
                           NULL,
                           &Size,
                           &SystemConfiguration);
  if (EFI_ERROR (Status)) 
    SystemConfiguration.PcieExtendedTagField = 2;         // Auto = 2

  //
  // Program Extended Tag Field Enable in Device Control Register 
  // as per capability and configuration
  // 
  if (SystemConfiguration.PcieExtendedTagField != 2) {

    for (Index = 0; Index < EntryNumber; Index++) {

      if ((mPcieStack.PcieCapInfo[Index].PcieDevCap & BIT5) != 0) {  // Check if Extended Tag Field supported
        //
        // Set DeviceControlRegister.ExtendedTagFieldEnable as per configuration
        //      
        if (SystemConfiguration.PcieExtendedTagField == 1){
          ExtTagFieldEnable = 1 << 8;         // Set Extended Tag Field Enable bit
        } else {
          ExtTagFieldEnable = 0;
        }
    
        PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
        TempPciAddress = PciAddress + PCIE_DEVICE_CONTROL_OFFSET; 
        PciRootBridgeIoRw (
                  PciRootBridgeIo,
                  EfiPciWidthUint16,
                  TempPciAddress,
                  ExtTagFieldEnable,
                  BIT8 
                );
      }
  
    } //for (Index = 0; Index < EntryNumber; Index++)
    
  } //if (SystemConfiguration.ExtendedTagField != 2)
  
}


STATIC 
VOID
SetPcieAtomicOpRequest (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo
)
{
  INTN                                  Index;
  INTN                                  EntryNumber;
  UINT32                                AtomicOpReq;
  UINT32                                AtomicOpEgress;
  UINT32                                Dev_Cap2;
  UINT64                                PciAddress;
  UINT64                                TempPciAddress;
  EFI_STATUS                            Status;
  UINT64                                Size;
  SYSTEM_CONFIGURATION                  SystemConfiguration;

  ASSERT ((mPcieStack.Top > 0) && (!(mPcieStack.Top & 1)));

  //
  // Fill the PCIE  device capability2 along the path
  //
  EntryNumber = mPcieStack.Top;
  for (Index = 0; Index < EntryNumber; Index++) {
    PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
    TempPciAddress = PciAddress + PCIE_DEVICE_CAPABILITY2_OFFSET; 
    PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32, 
                                  TempPciAddress,  
                                  1, 
                                  &Dev_Cap2
                                  );      
    mPcieStack.PcieCapInfo[Index].PcieDevCap2 =  (UINT16)(Dev_Cap2);
  }     //for (Index = 0; Index < EntryNumber; Index++) 
  //
  //Get the setup value for PCIe_AtomicOpReq
  //
  Size = sizeof (SystemConfiguration);
  Status = gRT->GetVariable(L"IntelSetup", 
                           &gEfiSetupVariableGuid,
                           NULL,
                           &Size,
                           &SystemConfiguration);
  if (EFI_ERROR (Status)) 
    SystemConfiguration.PCIe_AtomicOpReq = 0;     //Set it to default.

  //
  //For every device in the heap, enable/disable AtomicOp Request support option based on the setup option and capability.
  //
  for (Index = 0; Index < EntryNumber; Index++) {
    if ((mPcieStack.PcieCapInfo[Index].PcieDevCap2 & 0x40) != 0) {  //check for Atomicop Req capability
      if (SystemConfiguration.PCIe_AtomicOpReq == 1){
        AtomicOpReq = 0x40;
      } else {
        AtomicOpReq = 0x00;
      }
  
      PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index]));
      TempPciAddress = PciAddress + PCIE_DEVICE_CONTROL2_OFFSET; 
      PciRootBridgeIoRw (
                PciRootBridgeIo,
                EfiPciWidthUint16,
                TempPciAddress,
                AtomicOpReq,
                BIT6 
              );
    }

    // If Index = 0, then it should be root port. If it does not support AtomicOp, then
  	// AtomicOp Request will not be set in the above PciRootBridgeIoRw. No request will
	  // be passed to the child device. so the condition for Index!=0 is added. 
    if ((Index != 0) && ((mPcieStack.PcieCapInfo[Index].PcieDevCap2 & 0x40) == 0)){ 
      AtomicOpEgress = 0x80;

      PciAddress     = (UINT64)(*(UINT32 *)&(mPcieStack.PcieCapInfo[Index -1]));
      TempPciAddress = PciAddress + PCIE_DEVICE_CONTROL2_OFFSET; 
      PciRootBridgeIoRw (
              PciRootBridgeIo,
              EfiPciWidthUint16,
              TempPciAddress,
              AtomicOpEgress,
              BIT7 
            );
    }     //if ((Index != 0) && ((mPcieStack.PcieCapInfo[Index].PcieDevCap2 & 0x40) == 0))
  }       // for (Index = 0; Index < EntryNumber; Index++)
}


VOID
ProgramPcieCommMisc (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo,
  IN UINT8                             Bus,
  IN UINT8                             Device,
  IN UINT8                             Function
  )
/*++

Routine Description:
  Program ASPM support for PCIE. This is called recursively for child bridges.

Arguments:
  PciRootBridgeIo
  Bus
  Device
  Function
  
Returns:
  Nothing

--*/
{
  EFI_STATUS                           Status;
  UINT8                                SecBus;
  UINT8                                ChildDevice;
  UINT8                                ChildFunc;
  UINT64                               PciAddressMac; 
  UINT8                                ChildPortType;
  UINT8                                ParentPortType;
  UINT8                                ParentCapPtr;
  UINT8                                ChildCapPtr;
  PCI_TYPE00                           ParentPci;
  PCI_TYPE00                           ChildPci;
  BOOLEAN                              IsEndPoint;
  BOOLEAN                              IsSwitch;

  
  if (!IsPciDevicePresent (PciRootBridgeIo, &ParentPci, Bus, Device, Function)) {
    return;
  }

  if (!IS_PCI_BRIDGE(&ParentPci)) {
    return;
  } 

  PciAddressMac  = EFI_PCI_ADDRESS (Bus, Device, Function, 0);
  Status =  FindPciCapPtr (
                            PciRootBridgeIo, 
                            EFI_PCI_CAPABILITY_ID_PCIEXP, 
                            PciAddressMac, 
                            &ParentCapPtr 
                           );
  ASSERT_EFI_ERROR (Status);
  
  
  PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint8, 
                                  PciAddressMac,  
                                  1, 
                                  &SecBus
                                  );      
  ASSERT (SecBus !=0 && SecBus != 0xFF);

  PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, Function, ParentCapPtr + 2); //PCIE Capabilities Register
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint8, 
                                  PciAddressMac,  
                                  1, 
                                  &ParentPortType         //bits [7:4]
                                  );      
          
  //
  // Parent Port Type is in bits [3:0]
  //
  ParentPortType >>= 4;
  if (ParentPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_ROOT_PORT) {
    ResetPcieStack ();
    PushPcieStack (EFI_PCI_ADDRESS (Bus, Device, Function, ParentCapPtr));
  }

  //
  // Get child information. Validate valid PCIE child
  // Traverse all valid devices.
  //
  for (ChildDevice = 0; ChildDevice <= PCI_MAX_DEVICE; ChildDevice++) {
    for (ChildFunc = 0; ChildFunc <= PCI_MAX_FUNC; ChildFunc++) {
      if (IsPciDevicePresent (PciRootBridgeIo, &ChildPci, SecBus, ChildDevice, ChildFunc)) {
        //
        // Must be a PCIE device.
        //
        PciAddressMac  = EFI_PCI_ADDRESS (SecBus, ChildDevice, ChildFunc, 0);
        Status =  FindPciCapPtr (
                            PciRootBridgeIo, 
                            EFI_PCI_CAPABILITY_ID_PCIEXP, 
                            PciAddressMac,  
                            &ChildCapPtr 
                           );
        if (EFI_ERROR (Status)) {
          return;
        } 
          
        //
        // Get Child Port Type.
        //
        PciAddressMac = EFI_PCI_ADDRESS (SecBus, ChildDevice, ChildFunc, ChildCapPtr + PCIE_CAPABILITY_OFFSET);
        Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint8, 
                                    PciAddressMac,  
                                    1, 
                                    &ChildPortType
                                    );          
        ChildPortType >>= 4;   

        IsEndPoint = FALSE;
        IsSwitch   = FALSE;
        if ( ChildPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_ENDPOINT || \
             ChildPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_LEGACY_ENDPOINT ) {
          IsEndPoint = TRUE;
        } 
        
        if ( (ChildPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_UP_PORT) || \
             (ChildPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_DOWN_PORT)) {  
          if (IS_PCI_BRIDGE(&ChildPci)) {
            IsSwitch = TRUE;
          }
        }

        if (IsSwitch || IsEndPoint) {
          PushPcieStack (EFI_PCI_ADDRESS (SecBus, ChildDevice, ChildFunc, ChildCapPtr));
          if (IsSwitch) {
            ProgramPcieCommMisc (PciRootBridgeIo, SecBus, ChildDevice, ChildFunc);
          } else {
            SetPcieAspmPath            (PciRootBridgeIo);
            SetPcieMaxReadRequestSize  (PciRootBridgeIo);
            SetPcieMaxPayloadPath      (PciRootBridgeIo);
            SetExtendedTagSupport      (PciRootBridgeIo);
            SetPcieAtomicOpRequest     (PciRootBridgeIo);
            SetPcieLtrPath             (PciRootBridgeIo);
          }
          PopPcieStack ();
        }
      }

      if (ChildFunc == 0 && !IS_PCI_MULTI_FUNC (&ChildPci)) {
        //
        // Skip sub functions, this is not a multi function device
        //
        ChildFunc = PCI_MAX_FUNC;
      }
    }
  }
}


VOID
ProgramCommonClock (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo,
  IN UINT8                             Bus,
  IN UINT8                             Device,
  IN UINT8                             Function
  )
/*++

Routine Description:
  Program Common Clock Configuration. This is called recursively for child bridges.

Arguments:
  PciRootBridgeIo
  Bus
  Device
  Function
  
Returns:
  Nothing

--*/
{
  EFI_STATUS                           Status;
  UINT8                                SecBus;
  UINT8                                ChildDevice;
  UINT8                                Func;
  UINT8                                TotalFuncs;
  UINT16                               LinkStatus;
  UINT64                               PciAddressMac; 
  UINT8                                ParentPortType;
  UINT8                                ChildPortType;
  UINT8                                ParentSlotClock;
  UINT8                                ChildSlotClock;  
  UINT8                                ParentCapPtr;
  UINT8                                ChildCapPtr;
  PCI_TYPE00                           ParentPci;
  PCI_TYPE00                           ChildPci;
  BOOLEAN                              DevicePresent;
  
  if (!IsPciDevicePresent (PciRootBridgeIo, &ParentPci, Bus, Device, Function)) {
    return;
  }

  if (!IS_PCI_BRIDGE(&ParentPci)) {
    return;
  }
  
  PciAddressMac  = EFI_PCI_ADDRESS (Bus, Device, Function, 0);
  Status =  FindPciCapPtr (
                            PciRootBridgeIo, 
                            EFI_PCI_CAPABILITY_ID_PCIEXP, 
                            PciAddressMac, 
                            &ParentCapPtr 
                           );
  if (IsSoftSdvPlatform() == FALSE) {
    ASSERT_EFI_ERROR (Status);
  }
  
  PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint8, 
                                  PciAddressMac,  
                                  1, 
                                  &SecBus
                                  );      
  ASSERT (SecBus !=0 && SecBus != 0xFF);

  PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, Function, ParentCapPtr + 2); //PCIE Capabilities Register
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint8, 
                                  PciAddressMac,  
                                  1, 
                                  &ParentPortType         //bits [7:4]
                                  );      
          
  //
  // Parent Port Type is in bits [3:0]
  //
  ParentPortType >>= 4;

  //
  // Get child information. Validate valid PCIE child
  // Traverse all valid devices.
  //
  DevicePresent = FALSE;
  for (ChildDevice = 0; ChildDevice <= PCI_MAX_DEVICE; ChildDevice++) {    
    DevicePresent = IsPciDevicePresent (PciRootBridgeIo, &ChildPci, SecBus, ChildDevice, 0);
    if (!DevicePresent) {
      continue;
    }    
          
    //
    // Must be a PCIE device.
    //
    PciAddressMac  = EFI_PCI_ADDRESS (SecBus, ChildDevice, 0, 0);
    Status =  FindPciCapPtr (
                            PciRootBridgeIo, 
                            EFI_PCI_CAPABILITY_ID_PCIEXP, 
                            PciAddressMac,  
                            &ChildCapPtr 
                           );
    if (IsSoftSdvPlatform() == FALSE) {
      ASSERT_EFI_ERROR (Status);
    }
      
    //
    // Get Child Port Type.
    //
    PciAddressMac = EFI_PCI_ADDRESS (SecBus, ChildDevice, 0, ChildCapPtr + PCIE_CAPABILITY_OFFSET);
    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint8, 
                                    PciAddressMac,  
                                    1, 
                                    &ChildPortType
                                    );          
    ChildPortType >>= 4;   
    
    
    if (ParentPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_ROOT_PORT || \
        ParentPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_DOWN_PORT) {
        
      //
      // Get and Program Slot Clock Configuration.
      //    
      PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, 0, ParentCapPtr + PCIE_LINK_STATUS_OFFSET);
      Status = PciRootBridgeIo->Pci.Read (
                                      PciRootBridgeIo,
                                      EfiPciWidthUint16, 
                                      PciAddressMac,  
                                      1, 
                                      &LinkStatus
                                      );         
      ParentSlotClock = (LinkStatus & B_PCIE_LINK_STATUS_SLOT_CLOCK) ? 1:0;
      
      PciAddressMac = EFI_PCI_ADDRESS (SecBus, ChildDevice, 0, ChildCapPtr + PCIE_LINK_STATUS_OFFSET);
      Status = PciRootBridgeIo->Pci.Read (
                                      PciRootBridgeIo,
                                      EfiPciWidthUint16, 
                                      PciAddressMac,  
                                      1, 
                                      &LinkStatus
                                      );                                            
      ChildSlotClock = (LinkStatus & B_PCIE_LINK_STATUS_SLOT_CLOCK) ? 1:0;
      
      //
      // Check if both are 1. & (not &&) is correct.
      //   
      if (ParentSlotClock & ChildSlotClock) {   
        //
        // Set Common Clock and retrain.
        //
        PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, Function, ParentCapPtr + PCIE_LINK_CONTROL_OFFSET);
        PciRootBridgeIoRw (
          PciRootBridgeIo,
          EfiPciWidthUint8,
          PciAddressMac,
          B_PCIE_LINK_CONTROL_COMMON_CLOCK,
          0
        );
        
        TotalFuncs = IS_PCI_MULTI_FUNC(&ChildPci) ? 7 : 0;
        for (Func = 0; Func <= TotalFuncs; Func++) {
          if (!IsPciDevicePresent (PciRootBridgeIo, &ChildPci, SecBus, ChildDevice, 0)) {
            continue;
          }
          
          //
          // Must be a PCIE device.
          //
          PciAddressMac  = EFI_PCI_ADDRESS (SecBus, ChildDevice, 0, 0);
          Status =  FindPciCapPtr (
                              PciRootBridgeIo, 
                              EFI_PCI_CAPABILITY_ID_PCIEXP, 
                              PciAddressMac,  
                              &ChildCapPtr 
                             );
          if (EFI_ERROR(Status)) {
            continue;        //PCIE capable?        
          }
          
          PciAddressMac = EFI_PCI_ADDRESS (SecBus, ChildDevice, Func, ChildCapPtr + PCIE_LINK_CONTROL_OFFSET);
          PciRootBridgeIoRw (
            PciRootBridgeIo,
            EfiPciWidthUint8,
            PciAddressMac,
            B_PCIE_LINK_CONTROL_COMMON_CLOCK,
            0
          );
        }
      
        //
        // Retrain Link
        //
        PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, Function, ParentCapPtr + PCIE_LINK_CONTROL_OFFSET);
        PciRootBridgeIoRw (
          PciRootBridgeIo,
          EfiPciWidthUint8,
          PciAddressMac,
          B_PCIE_LINK_CONTROL_RETRAIN_LINK,
          0
        );
      
        PciAddressMac = EFI_PCI_ADDRESS (Bus, Device, Function, ParentCapPtr + PCIE_LINK_STATUS_OFFSET);
        do {
          Status = PciRootBridgeIo->Pci.Read (
                                          PciRootBridgeIo,
                                          EfiPciWidthUint16, 
                                          PciAddressMac, 
                                          1, 
                                          &LinkStatus
                                          );                
        } while (LinkStatus & B_PCIE_LINK_STATUS_LINK_TRAINING);     
      }
    }
  
    //
    //If upstream Port transverse downstream ports.
    //
    if ( (ChildPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_UP_PORT) || \
         (ChildPortType == V_PCIE_LINK_CAPABILITY_PORT_TYPE_DOWN_PORT)) {                            
      //
      // Program Common Clock Configuration for downstream.
      //
      ProgramCommonClock (PciRootBridgeIo, SecBus, ChildDevice, 0);
    }
  }
}

