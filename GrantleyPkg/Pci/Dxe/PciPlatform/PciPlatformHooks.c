/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  2004 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PciPlatformHooks.c
    
Abstract:

    Registers onboard PCI ROMs with PCI.IO.
    
Revision History:

--*/

#include <PiDxe.h>
#include <PciPlatform.h>
#include <PciPlatformHooks.h>
#include <Protocol\IioUds.h>
#include <PciSupportLib.h>
#include <IoApic.h>
#include <Cpu\CpuBaseLib.h>
#include <Iio\MmioAccess.h>
#include <Library\S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol\PchPlatformPolicy.h>


SYSTEM_CONFIGURATION  mSystemConfiguration;
EFI_IIO_UDS_PROTOCOL                    *mIioUds          = NULL;

/*VOID
PcieClearError(
  VOID
  );*/

VOID
ChipsetCallback (
  IN  EFI_HANDLE                                    RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS   PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                     Phase,
  EFI_PCI_CALLBACK_CONTEXT                          *Context
  )
{
  EFI_LIST_ENTRY    *NodeEntry;
  PCI_CALLBACK_DATA *PciCallbackData;

  //
  // Check if the node has been added
  //
  // DEBUG ((DEBUG_ERROR, "PCI Callback (%d,%d,%d)\n",PciAddress.Bus, PciAddress.Device, PciAddress.Function ));
  //
  Context->PciRootBridgeIo  = mPciPrivateData.PciRootBridgeIo;
  NodeEntry                 = GetFirstNode (&mPciPrivateData.PciCallbackList);
  while (!IsNull (&mPciPrivateData.PciCallbackList, NodeEntry)) {
    PciCallbackData = PCI_CALLBACK_DATA_FROM_LINK (NodeEntry);
    if (PciCallbackData->Phase & Phase) {
      (PciCallbackData->Function) (RootBridgeHandle, PciAddress, Phase, Context);
    }

    NodeEntry = GetNextNode (&mPciPrivateData.PciCallbackList, NodeEntry);
  }
}

EFI_STATUS
PciTreeTraverse (
  IN  UINT8                                        StartBus
  )
{
  UINT64  PciAddress;
  UINT8   Device;
  UINT8   Func;
  UINT8   SecondaryBus;
  BOOLEAN MultiFunc;

  for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
    MultiFunc =  FALSE;
    for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
      if (IsPciDevicePresent (
            mPciPrivateData.PciRootBridgeIo,
            &mPciPrivateData.Context.PciHeader,
            StartBus,
            Device,
            Func
            )) {
        if ((Func == 0) && IS_PCI_MULTI_FUNC(&(mPciPrivateData.Context.PciHeader))) {
          MultiFunc = TRUE;
        }
        PciAddress = EFI_PCI_ADDRESS (StartBus, Device, Func, 0);
        ChipsetCallback (
          mPciPrivateData.RootBridgeHandle,
          *(EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &PciAddress,
          mPciPrivateData.PciEnumerationPhase,
          &(mPciPrivateData.Context)
          );
        if (IS_PCI_BRIDGE (&(mPciPrivateData.Context.PciHeader))) {
          PciAddress = EFI_PCI_ADDRESS (StartBus, Device, Func, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
          mPciPrivateData.PciRootBridgeIo->Pci.Read (
                                                mPciPrivateData.PciRootBridgeIo,
                                                EfiPciWidthUint8,
                                                *(UINT64 *) &PciAddress,
                                                1,
                                                &SecondaryBus
                                                );
          if ((SecondaryBus > 0) && (SecondaryBus < 0xFF)) {
            PciTreeTraverse (SecondaryBus);
          }
        }
      }

      if (MultiFunc == FALSE) {
        //
        // Skip sub functions, this is not a multi function device
        //
        Func = PCI_MAX_FUNC;
      }
    }
  }

  return EFI_SUCCESS;
}

VOID
ProgramIoApicId (
  IN UINT32                            IoApicAddress,
  IN UINT8                             IoApicId
  )
/*++

Routine Description:
  Program Io Apic Id

Arguments:
  IoApicAddress and IoApicId

Returns:
  None

--*/
{

  UINT32  Data;

  mPciPrivateData.CpuIo->Mem.Read (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_INDEX_OFFSET,
                              1,
                              &Data
                              );

  //
  // IOAPIC is not there
  //
  if (Data == (UINT32) -1) {
    return ;
  }
  //
  // Set up IO APIC ID and enable FSB delivery
  // Use CPU IO protocol since the IO APIC ranges
  // are not included in PCI apertures
  //
  Data = EFI_IO_APIC_ID_REGISTER;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_INDEX_OFFSET,
                              1,
                              &Data
                              );

  Data = IoApicId << EFI_IO_APIC_ID_BITSHIFT;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_DATA_OFFSET,
                              1,
                              &Data
                              );

  Data = EFI_IO_APIC_BOOT_CONFIG_REGISTER;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_INDEX_OFFSET,
                              1,
                              &Data
                              );

  Data = EFI_IO_APIC_FSB_INT_DELIVERY;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_DATA_OFFSET,
                              1,
                              &Data
                              );
}

EFI_STATUS
PciExpressCommonInit (
  IN  UINT8                                        StartBus
  )
{
  EFI_STATUS                      Status;
  UINT8                           Device;
  UINT8                           Func;
  UINT8                           ParentPortType;
  UINT8                           ParentCapPtr;
  UINT64                          PciAddressMac;
  PCI_TYPE00                      PciHeader;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;

  PciRootBridgeIo = mPciPrivateData.PciRootBridgeIo;

  for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
    for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
      if (!IsPciDevicePresent (
            mPciPrivateData.PciRootBridgeIo,
            &PciHeader,
            StartBus,
            Device,
            Func
            )) {
        continue;
      }

      PciAddressMac = EFI_PCI_ADDRESS (StartBus, Device, Func, 0);
      Status = FindPciCapPtr (
                PciRootBridgeIo,
                EFI_PCI_CAPABILITY_ID_PCIEXP,
                PciAddressMac,
                &ParentCapPtr
                );
      if (EFI_ERROR (Status)) {
        continue;
      }

      //
      // PCIE Capabilities Register
      //
      PciAddressMac = EFI_PCI_ADDRESS (StartBus, Device, Func, ParentCapPtr + 2);
      Status = PciRootBridgeIo->Pci.Read (
                                      PciRootBridgeIo,
                                      EfiPciWidthUint8,
                                      PciAddressMac,
                                      1,
                                      &ParentPortType // bits [7:4]
                                      );
      ParentPortType >>= 4;
      if (ParentPortType == 4) {
        //SoftSDV Hangs as it does not find PCIE capability for B3:D0:F0.
        //Need to resolve this with SoftSDV team.
        if (!IsSoftSdvPlatform()) {
          ProgramCommonClock (PciRootBridgeIo, StartBus, Device, Func);
        }
        ProgramPcieCommMisc (PciRootBridgeIo, StartBus, Device, Func);
      }

      if (Func == 0 && !IS_PCI_MULTI_FUNC (&PciHeader)) {
        //
        // Skip sub functions, this is not a multi function device
        //
        Func = PCI_MAX_FUNC;
      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
PciPlatformEarlyInit (
  VOID
  )
/*++

Routine Description:

  Platform Pci Express init.

Arguments:

  HostBridgeInstance  -  Pointer to Host Bridge private data
                         does not support 64 bit memory addresses.

Returns:

  EFI_SUCCESS  -  Success.
  
--*/
{
  EFI_STATUS                      Status;
  //
  // Locate the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL,&mIioUds);
  ASSERT_EFI_ERROR (Status);
  return EFI_SUCCESS;
}

EFI_STATUS
PciPlatformMidInit (
  VOID
  )
{
  UINT8 i;

  for (i = 0 ; i < MaxIIO ; i++) {
    if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].Valid) {
      PciExpressCommonInit (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].BusBase);
     }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
PciPlatformPostInit (
  VOID
  )
/*++
  
Routine Description:
  
  Init pci device registers after the device resources have been allocated, so
  that devices behind a bus could be accessed.
    
Arguments:
  
  HostBridgeInstance  -  PCI_HOST_BRIDGE_INSTANCE.

Returns:
  
  EFI_SUCCESS  -  Function has completed successfully.

--*/
{
  //
  // Program all the IOAPIC in system
  //
  UINTN TotalIioFound;
  UINTN IioResourceMapEntry;
  UINT8 ApicId;
  UINT32 IoApicAddress;

  ProgramIoApicId (PCH_IOAPIC_ADDRESS, PCH_IOAPIC_ID);
  
  for ( TotalIioFound = 0, IioResourceMapEntry = 0; 
        ((TotalIioFound < mIioUds->IioUdsPtr->PlatformData.numofIIO) && (IioResourceMapEntry < MaxIIO));
        IioResourceMapEntry++) {

      if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioResourceMapEntry].Valid) continue; // No guarantee that the valid Iios are sequential starting at 0!

      IoApicAddress = 0XFEC00000;
      IoApicAddress = (UINT32)mIioUds->IioUdsPtr->PlatformData.IIO_resource [IioResourceMapEntry].IoApicBase;
      
      if (IoApicAddress == PCH_IOAPIC_ADDRESS) {
          IoApicAddress += 0x1000;
      }

      ApicId = (UINT8)IIO0_IOAPIC_ID + (UINT8)mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioResourceMapEntry].SocketID;

      ProgramIoApicId (IoApicAddress, ApicId);

      TotalIioFound++;
  }

  /*//s3427320 - start
  PcieClearError();
  //s3427320 - end*/
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PlatformPrepController (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_HANDLE                                     RootBridge,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS    PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE   Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
/*++

Routine Description:
  
  The PlatformPrepController() function can be used to notify the platform driver so that 
  it can perform platform-specific actions. No specific actions are required. 
  Several notification points are defined at this time. More synchronization points may be 
  added as required in the future. The PCI bus driver calls the platform driver twice for 
  every PCI controller-once before the PCI Host Bridge Resource Allocation Protocol driver 
  is notified, and once after the PCI Host Bridge Resource Allocation Protocol driver has 
  been notified. 
  This member function may not perform any error checking on the input parameters. It also 
  does not return any error codes. If this member function detects any error condition, it 
  needs to handle those errors on its own because there is no way to surface any errors to 
  the caller.  
    
Arguments:

  This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.  
  HostBridge    -  The associated PCI host bridge handle. 
  RootBridge    -  The associated PCI root bridge handle.
  PciAddress    -  The address of the PCI device on the PCI bus. 
  Phase         -  The phase of the PCI controller enumeration. 
  ChipsetPhase  -  Defines the execution phase of the PCI chipset driver. 
    
Returns:

  EFI_SUCCESS     -  The function completed successfully.
  EFI_UNSUPPORTED -  Not supported.
  
--*/
{
  if (mPciPrivateData.RootBridgeHandle == NULL) {
    mPciPrivateData.RootBridgeHandle = RootBridge;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PhaseNotify (
  IN EFI_PCI_PLATFORM_PROTOCOL                       *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE  Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
/*++

Routine Description:

  Perform initialization by the phase indicated.

Arguments:

  This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.
  HostBridge    -  The associated PCI host bridge handle.
  Phase         -  The phase of the PCI controller enumeration.
  ChipsetPhase  -  Defines the execution phase of the PCI chipset driver.

Returns:

  EFI_SUCCESS  -  Must return with success.

--*/
{
  EFI_STATUS  Status;
  UINT8       i;

  if (ChipsetPhase == ChipsetEntry) {
    return EFI_SUCCESS;
  }
  //
  // If for multiple host bridges, need special consideration
  //
  switch (Phase) {

  case EfiPciHostBridgeBeginEnumeration:
    //
    // Pre-initialization before PCI bus enumeration
    // No bus number and no PCI resource
    //
    Status = gBS->LocateProtocol (
                    &gEfiPciRootBridgeIoProtocolGuid,
                    NULL,
                    &(mPciPrivateData.PciRootBridgeIo)
                    );
    ASSERT_EFI_ERROR (Status);

    Status = gBS->LocateProtocol (
                    &gEfiCpuIoProtocolGuid,
                    NULL,
                    &(mPciPrivateData.CpuIo)
                    );
    ASSERT_EFI_ERROR (Status);
    mPciPrivateData.Context.CpuIo = mPciPrivateData.CpuIo;

    DEBUG ((DEBUG_ERROR, "PCI Platform Pre-Initialization (Before bus scanning)\n"));
    PciPlatformEarlyInit ();
    break;

  case EfiPciHostBridgeEndBusAllocation:
    //
    // There are two rounds PCI bus scanning
    // First round will initilize the PCI hotplug device
    // Second round will be the final one
    //
    if (mPciPrivateData.BusAssignedTime == 0) {
      mPciPrivateData.PciEnumerationPhase = EfiPciEnumerationDeviceScanning;
      for (i = 0 ; i < MaxIIO ; i++) {
          if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].Valid) {
              PciTreeTraverse (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].BusBase);
          }
      }
      mPciPrivateData.BusAssignedTime++;
      DEBUG ((DEBUG_ERROR, "PCI Platform bus assigned\n"));
      /*//s3427069: Sunrise Ridge fails PCIE enumeration with IDT switch card - start
      //wait 1 sec to make sure HP capable slot power stablized
      gBS->Stall (1000000); /* 1 seconds */
      //s3427069 - end*/

    }
    break;

  case EfiPciHostBridgeBeginResourceAllocation:
    //
    // PCI bus number has been assigned, but resource is still empty
    //
    DEBUG ((DEBUG_ERROR, "PCI Platform Mid-Initialization (After bus number assignment)\n"));
    mPciPrivateData.PciEnumerationPhase = EfiPciEnumerationBusNumberAssigned;
    for (i = 0 ; i < MaxIIO ; i++) {
        if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].Valid) {
            PciTreeTraverse (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].BusBase);
        }
    }
    PciPlatformMidInit ();
    break;

  case EfiPciHostBridgeEndResourceAllocation:
    //
    // Resource enumeration is done.
    // Both bus number and resource have been assigned
    // Do any post initialization.
    //
    DEBUG ((DEBUG_ERROR, "PCI Platform Post-Initialization (After resource alloction)\n"));
    mPciPrivateData.PciEnumerationPhase = EfiPciEnumerationResourceAssigned;
    for (i = 0 ; i < MaxIIO ; i++) {
        if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].Valid) {
            PciTreeTraverse (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].BusBase);
        }
    }
    PciPlatformPostInit ();
    break;

  case EfiPciHostBridgeBeginBusAllocation : // Aptio V Server Override added our pcibus.c is calling with this phase.
  case EfiPciHostBridgeAllocateResources :
  case EfiPciHostBridgeSetResources :
	  return EFI_SUCCESS;

  default:
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}


/*VOID
PcieClearError(
  VOID
  )
{
  UINT8  i;
  UINT8  Bus;
  UINT8  Dev; 
  UINT32  Data;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  *PciAddressPtr;
  UINT64  PciAddress;

  PciAddressPtr = (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  *)&PciAddress;

  for (i = 0 ; i < MaxIIO ; i++) {
    if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].Valid) {
      Bus = mIioUds->IioUdsPtr->PlatformData.IIO_resource[i].BusBase;
      for(Dev = 0; Dev <= PCIE_PORT_MAX; Dev++) { 
        PciAddress = EFI_PCI_ADDRESS (Bus, Dev, 0, R_PCIE_PORT_PXPLNKSTS);       
        mPciPrivateData.PciRootBridgeIo->Pci.Read (
                                             mPciPrivateData.PciRootBridgeIo,
                                             EfiPciWidthUint16,
                                             PciAddress,
                                             1,
                                             &Data
                                             );
        Data |= BIT15;
        mPciPrivateData.PciRootBridgeIo->Pci.Write (
                                             mPciPrivateData.PciRootBridgeIo,
                                             EfiPciWidthUint16,
                                             PciAddress,
                                             1,
                                             &Data
                                             );

        PciAddressPtr->Register         = 0;
        PciAddressPtr->Function         = 0;
        PciAddressPtr->Device           = Dev;
        PciAddressPtr->Bus              = Bus;
        PciAddressPtr->ExtendedRegister = R_PCIE_PORT_XPCORERRSTS;
        mPciPrivateData.PciRootBridgeIo->Pci.Read (
                                             mPciPrivateData.PciRootBridgeIo,
                                             EfiPciWidthUint16,
                                             PciAddress,
                                             1,
                                             &Data
                                             );

        Data |= BIT0;

        mPciPrivateData.PciRootBridgeIo->Pci.Write (
                                             mPciPrivateData.PciRootBridgeIo,
                                             EfiPciWidthUint16,
                                             PciAddress,
                                             1,
                                             &Data
                                             );
      }
    }
  }
}*/

BOOLEAN
IsAnIioRootPort(
    IN  UINT8   Bus,
    IN  UINT8   Device,
    IN  UINT8   Function
)
/*++

Routine Description:

  Check if the device belongs is an  IIO Root Port

Arguments:

  Bus               -  PCI bus #.
  Device            -  PCI device #.
  Function          -  PCI function #

Returns:

  True          -  It is an IIO Root Port
  False          - It is not an IIO Root Port

--*/
{
  UINT8   IioIndex;
  UINT8   IioBus;

  if(mIioUds == NULL)
    PciPlatformEarlyInit();

  for(IioIndex = 0; IioIndex < MaxIIO; IioIndex++){
    if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].Valid)
      continue;

    IioBus = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].BusBase;
    if(IioBus == Bus)
      break;
  }

  if(IioIndex >= MaxIIO || Device > PCIE_PORT_3_DEV  || Function > PCIE_PORT_3D_FUNC){
    return FALSE;
  }

  return  TRUE;
}

EFI_STATUS
GetIioPortMaxPayloadSizePlatformPolicy(
    IN  UINT8   Bus,
    IN  UINT8   Device,
    IN  UINT8   Function,
    OUT UINT8   *PortPayloadSize
)
/*++

Routine Description:

  Retrieves IIO Port specific system configuration value as per the setup option
  defined for root port maximum payload size.

Arguments:

  Bus               -  PCI bus #.
  Device            -  PCI device #.
  Function          -  PCI function #
  PortPayloadSize   -  Output value as per the setup option or system configuration

Returns:

  EFI_SUCCESS  -  returns when the input parameter matchs the valid IIO root port.
  EFI_INVALID_PARAMETER - returns when the input parameter does not match the valid root port

--*/
{
  UINT8   Iio;
  UINT8   IioBus;
  UINT8   PortIndex = 0;

  if(mIioUds == NULL)
    PciPlatformEarlyInit();

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[Iio].Valid)
      continue;

    IioBus = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Iio].BusBase;
    if(IioBus == Bus)
      break;
  }

  if(Iio >= MaxIIO || Device > PCIE_PORT_3_DEV  || Function > PCIE_PORT_3D_FUNC){
    return EFI_INVALID_PARAMETER;
  }

  PCIEDEVFUNC2PORTINDEX(PortIndex, Device, Function);

  *PortPayloadSize = \
      mIioUds->IioUdsPtr->SetupInfo.Config.PcieMaxPayload[(Iio * VARIABLE_GROUP_ELEMENTS) + PortIndex];

  return  EFI_SUCCESS;
}

UINT8
CheckPerPortASPM(
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  UINT8       Aspm;

  if (((Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex) >= MAX_TOTAL_PORTS) {
    return PCIE_ASPM_DISABLE;
  }
  Aspm = mIioUds->IioUdsPtr->SetupInfo.Config.PcieAspm[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex];

  if(Aspm == PCIE_ASPM_AUTO){    
    Aspm = PCIE_ASPM_DISABLE;     // ASPM = disable
  }

  return Aspm;
}

EFI_STATUS
GetIioPortAspmPlatformPolicy(
    IN  UINT64   PciAddress,
    OUT UINT8    *Aspm
)
/*++

Routine Description:

  Retrieves IIO Port specific system configuration value as per the setup option
  defined for per port aspm or global aspm.

Arguments:

  PciAddress        -  The address of the PCI device on the PCI bus.   
  Aspm              -  Output value as per the setup option or system configuration

Returns:

  EFI_SUCCESS  -  returns when the input parameter matchs the valid IIO root port.
  EFI_INVALID_PARAMETER - returns when the input parameter does not match the valid root port

--*/
{
  UINT8   Iio;
  UINT8   IioBus;
  UINT8   Bus,Device,Function;
  UINT8   PortIndex = 0;

  if(mIioUds == NULL)
    PciPlatformEarlyInit();

  Bus      = (UINT8)((PciAddress >> 24) & 0xff);
  Device   = (UINT8)((PciAddress >> 16) & 0xff);
  Function = (UINT8)((PciAddress >> 8) & 0xff);

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[Iio].Valid)
      continue; 

    IioBus = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Iio].BusBase;
    if(IioBus == Bus)
      break;
  }

  if(Iio >= MaxIIO || Device > PCIE_PORT_3_DEV  || Function > PCIE_PORT_3D_FUNC){
    return EFI_INVALID_PARAMETER;
  }
  
  PCIEDEVFUNC2PORTINDEX(PortIndex, Device, Function);

  if(mIioUds->IioUdsPtr->SetupInfo.Config.PcieGlobalAspm)
    *Aspm = mIioUds->IioUdsPtr->SetupInfo.Config.PcieGlobalAspm;
  else
    *Aspm = CheckPerPortASPM(Iio, (UINT8)PortIndex);

  return  EFI_SUCCESS;
}

BOOLEAN
IsPchRootPort(
    IN  UINT8   Bus,
    IN  UINT8   Device,
    IN  UINT8   Function
)
/*++

Routine Description:

  Check if the device belongs is a PCH Root Port

Arguments:

  Bus              -  PCI bus #.
  Device           -  PCI device #.
  Function         -  PCI function #

Returns:

  True             -  It is a PCH Root Port
  False            - It is not a PCH Root Port

--*/
{
  
  if((Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && (Device == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS) && (Function <= PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8)){
	return TRUE;
  } else {
      return  FALSE;
  }
}

EFI_STATUS
GetPchPortAspmPlatformPolicy(
    IN  UINT64   PciAddress,
    OUT UINT8    *Aspm
)
/*++

Routine Description:

  Retrieves PCH Port specific system configuration value as per the setup option
  defined for per port aspm or global aspm.

Arguments:

  PciAddress        -  The address of the PCI device on the PCI bus.   
  Aspm              -  Output value as per the setup option or system configuration

Returns:

  EFI_SUCCESS  -  returns when the input parameter matchs the valid IIO root port.
  EFI_INVALID_PARAMETER - returns when the input parameter does not match the valid root port

--*/
{
  DXE_PCH_PLATFORM_POLICY_PROTOCOL   *PchPlatformPolicy;
  UINT8   Bus,Device,Function;
  UINT8   PortIndex = 0;
  EFI_STATUS                        Status;

  Bus = (UINT8)((PciAddress >> 24) & 0xff);
  Device   = (UINT8)((PciAddress >> 16) & 0xff);
  Function = (UINT8)((PciAddress >> 8) & 0xff);

  if( (Bus != DEFAULT_PCI_BUS_NUMBER_PCH) || (Device != PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS) || (Function > PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8)){
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                &gDxePchPlatformPolicyProtocolGuid,
                NULL,
                &PchPlatformPolicy
                );
  ASSERT_EFI_ERROR (Status);

  if( (Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && (Device == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS)){
    PortIndex = Function;
  }

  if(PchPlatformPolicy->PciExpressConfig->PchPcieGlobalAspm)
    *Aspm = PchPlatformPolicy->PciExpressConfig->PchPcieGlobalAspm;
  else {
    *Aspm = PchPlatformPolicy->PciExpressConfig->RootPort[PortIndex].Aspm;
      if(*Aspm == PCIE_ASPM_AUTO){    
        *Aspm = PCIE_ASPM_DISABLE;     // ASPM = disable
      }
  }
  return  EFI_SUCCESS;
}
