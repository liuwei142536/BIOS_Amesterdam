//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file UsbPeim.c
    This file belongs to "Framework".
    This file is modified by AMI to include copyright message,
    appropriate header and integration code.
    This file contains generic routines needed for USB recovery
    PEIM

**/
/**
The module to produce Usb Bus PPI.

Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
  
This program and the accompanying materials
are licensed and made available under the terms and conditions
of the BSD License which accompanies this distribution.  The
full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include "UsbPei.h"

#include "Ppi/Stall.h"
#include "Ppi/LoadFile.h"
#include <Library/DebugLib.h>
#include <Ppi/IoMmu.h>

extern EDKII_IOMMU_PPI *gEdk2IoMmuPpi;

#define PAGESIZE  4096

EFI_STATUS
PeimInitializeUsb (
    IN EFI_FFS_FILE_HEADER *FfsHeader,
    IN EFI_PEI_SERVICES    **PeiServices );

//
// UsbIo PPI interface function
//
static PEI_USB_IO_PPI         gAmiUsbIoPpi = {
    PeiUsbControlTransfer,
    PeiUsbBulkTransfer,
    PeiUsbGetInterfaceDescriptor,
    PeiUsbGetEndpointDescriptor,
    PeiUsbPortReset,
    PeiUsbSyncInterruptTransfer,
};

static EFI_PEI_PPI_DESCRIPTOR gAmiUsbIoPpiList = {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gAmiPeiUsbIoPpiGuid,
    NULL
};

static EFI_PEI_PPI_DESCRIPTOR gUsbIoPpiList = {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gPeiUsbIoPpiGuid,
    NULL
};

//
// Helper functions
//
static
EFI_STATUS
PeiUsbEnumeration (
    IN EFI_PEI_SERVICES            **PeiServices,
    IN PEI_USB_HOST_CONTROLLER_PPI *UsbHcPpi );

static
EFI_STATUS
PeiConfigureUsbDevice (
    IN EFI_PEI_SERVICES **PeiServices,
    IN PEI_USB_DEVICE   *PeiUsbDevice,
    IN UINT8            Port,
    IN OUT UINT8        *DeviceAddress );

static
EFI_STATUS
PeiUsbGetAllConfiguration (
    IN EFI_PEI_SERVICES **PeiServices,
    IN PEI_USB_DEVICE   *PeiUsbDevice );



EFI_STATUS PeimInitializeUsb (
    IN EFI_FFS_FILE_HEADER *FfsHeader,
    IN EFI_PEI_SERVICES    **PeiServices )
{
    EFI_STATUS Status;
    UINTN      i;
    PEI_USB_HOST_CONTROLLER_PPI *UsbHcPpi;

    i = 0;
    while (TRUE) {
        //
        // Get UsbHcPpi at first.
        //
        Status = (**PeiServices).LocatePpi( PeiServices,
            &gAmiPeiUsbHostControllerPpiGuid, i, NULL, (VOID**)&UsbHcPpi );

        if ( EFI_ERROR( Status ) ) {
            break;
        }

        PeiUsbEnumeration( PeiServices, UsbHcPpi );
        i++;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
PeiHubEnumeration (
    IN EFI_PEI_SERVICES **PeiServices,
    IN PEI_USB_DEVICE   *PeiUsbDevice,
    IN UINT8            *CurrentAddress,
    IN BOOLEAN          CheckConnectChange
)
//
// Since we are not going to support hot-plug, we will do a
// dedicated polling process to get all ports change event
// discovered. This can help to avoid introduce interrupt
// transfer into the system, for each hub, we will wait for
// 3s and if one port is enabled we will not wait for others
// We will start parsing each of them in sequence.
//
{
    UINT8                Port;
    EFI_STATUS           Status;
    PEI_USB_IO_PPI       *UsbIoPpi;
    EFI_USB_PORT_STATUS  PortStatus;
	UINT8                *AllocateAddress;
    PEI_USB_DEVICE       *NewPeiUsbDevice;
    EFI_PEI_STALL_PPI    *StallPpi;


    UsbIoPpi = &PeiUsbDevice->UsbIoPpi;
    (**PeiServices).LocatePpi( PeiServices, &gEfiPeiStallPpiGuid,
                               0, NULL, (VOID**)&StallPpi);

    for (Port = 1; Port <= PeiUsbDevice->DownStreamPortNo; Port++) {

        Status = PeiHubGetPortStatus(PeiServices, UsbIoPpi,
            Port, (UINT32 *) &PortStatus);

        if (EFI_ERROR(Status)) {
            continue;
        }
        
        DEBUG((DEBUG_INFO, "Hub port[%d] status: %08x\n", Port, PortStatus));

        if (IsPortConnectChange( PortStatus.PortChangeStatus)) {
            PeiHubClearPortFeature(PeiServices, UsbIoPpi,
                Port, EfiUsbPortConnectChange);
        } else {
            if (CheckConnectChange == TRUE) {
                continue;
            }
        }
        
        if (IsPortConnect(PortStatus.PortStatus)) {

            if (IsPortEnable(PortStatus.PortStatus) == FALSE) {
	            //
	            // First reset and enable this port
	            //
	            PeiResetHubPort( PeiServices, UsbIoPpi, Port );

	            PeiHubGetPortStatus(PeiServices, UsbIoPpi,
	                    Port, (UINT32 *)&PortStatus);

                if (!IsPortConnect(PortStatus.PortStatus) || 
                        !IsPortEnable(PortStatus.PortStatus)) {
                    continue;
                }
            }

            //
            // Begin to deal with the new device
            //
            Status = (*PeiServices)->AllocatePool( PeiServices,
                    sizeof(PEI_USB_DEVICE), (VOID**)&AllocateAddress);
            if (EFI_ERROR(Status)) {
                return EFI_OUT_OF_RESOURCES;
            }

            NewPeiUsbDevice = (PEI_USB_DEVICE *)AllocateAddress;

            (*PeiServices)->SetMem(NewPeiUsbDevice, sizeof(PEI_USB_DEVICE), 0);
            //ZeroMem( NewPeiUsbDevice, sizeof(PEI_USB_DEVICE) );

            NewPeiUsbDevice->Signature =
                    PEI_USB_DEVICE_SIGNATURE;
            NewPeiUsbDevice->DeviceAddress = 0;
            NewPeiUsbDevice->MaxPacketSize0 = 8;
            NewPeiUsbDevice->DataToggle = 0;
            NewPeiUsbDevice->UsbIoPpi = gAmiUsbIoPpi;
            NewPeiUsbDevice->AmiUsbIoPpiList = gAmiUsbIoPpiList;
            NewPeiUsbDevice->AmiUsbIoPpiList.Ppi =
                    &NewPeiUsbDevice->UsbIoPpi;
            NewPeiUsbDevice->UsbIoPpiList = gUsbIoPpiList;
            NewPeiUsbDevice->UsbIoPpiList.Ppi =
                    &NewPeiUsbDevice->UsbIoPpi;
            NewPeiUsbDevice->UsbHcPpi =
                    PeiUsbDevice->UsbHcPpi;
            NewPeiUsbDevice->DeviceSpeed =
                    USB_FULL_SPEED_DEVICE;
            NewPeiUsbDevice->DeviceType = 0x0;
            NewPeiUsbDevice->DownStreamPortNo = 0x0;
            NewPeiUsbDevice->TransactionTranslator =
                    (UINT16)(Port << 7) + PeiUsbDevice->DeviceAddress;
            NewPeiUsbDevice->HubDepth = 0x0;
            
            if (IsPortLowSpeedDeviceAttached(PortStatus.PortStatus)) {
                NewPeiUsbDevice->DeviceSpeed = USB_SLOW_SPEED_DEVICE;
            }

            if (IsPortHighSpeedDeviceAttached(PortStatus.PortStatus)) {
                NewPeiUsbDevice->DeviceSpeed = USB_HIGH_SPEED_DEVICE;
                NewPeiUsbDevice->MaxPacketSize0 = 0x40;
            }

            if (IsPortSuperSpeedDeviceAttached(PortStatus.PortStatus)) {
                NewPeiUsbDevice->DeviceSpeed = USB_SUPER_SPEED_DEVICE;
                NewPeiUsbDevice->MaxPacketSize0 = 0x200;
            }
            if (IsPortSuperSpeedPlusDeviceAttached(PortStatus.PortStatus)) {
                NewPeiUsbDevice->DeviceSpeed = USB_SUPER_SPEED_PLUS_DEVICE;
                NewPeiUsbDevice->MaxPacketSize0 = 0x200;
            }
									//(EIP28255+)>
            if (PeiUsbDevice->DeviceSpeed == USB_SLOW_SPEED_DEVICE ||
					PeiUsbDevice->DeviceSpeed == USB_FULL_SPEED_DEVICE) {
                NewPeiUsbDevice->TransactionTranslator = PeiUsbDevice->TransactionTranslator;
            }
									//<(EIP28255+)
            //
            // Configure that Usb Device
            //
            Status = PeiConfigureUsbDevice(PeiServices,
                    NewPeiUsbDevice, Port, CurrentAddress);

            if (EFI_ERROR(Status)) {
                                        //(EIP61827+)>
                PeiHubClearPortFeature(PeiServices, UsbIoPpi,
                        Port, EfiUsbPortEnable);
                                        //<(EIP61827+)
                continue;
            }

            Status = (**PeiServices).InstallPpi(PeiServices, 
                            &NewPeiUsbDevice->AmiUsbIoPpiList);
            ASSERT(Status == EFI_SUCCESS);
            if (EFI_ERROR(Status)) {
                return Status;
            }

            Status = (**PeiServices).InstallPpi(PeiServices,
                            &NewPeiUsbDevice->UsbIoPpiList);
            ASSERT(Status == EFI_SUCCESS);
            if (EFI_ERROR(Status)) {
                return Status;
            }

            // Initialize DeviceType
            if ((NewPeiUsbDevice->InterfaceDesc->InterfaceClass == BASE_CLASS_MASS_STORAGE)
                && (NewPeiUsbDevice->InterfaceDesc->InterfaceProtocol == PROTOCOL_BOT)) {
                NewPeiUsbDevice->DeviceType = BIOS_DEV_TYPE_STORAGE;
            }

            if (NewPeiUsbDevice->InterfaceDesc->InterfaceClass == BASE_CLASS_HUB) {
                NewPeiUsbDevice->DeviceType = BIOS_DEV_TYPE_HUB;
                NewPeiUsbDevice->HubDepth = PeiUsbDevice->HubDepth + 1;
            }

            if ((NewPeiUsbDevice->InterfaceDesc->InterfaceClass == BASE_CLASS_HID)
                && (NewPeiUsbDevice->InterfaceDesc->InterfaceSubClass == SUB_CLASS_BOOT_DEVICE)
                && (NewPeiUsbDevice->InterfaceDesc->InterfaceProtocol == PROTOCOL_KEYBOARD)) {
                NewPeiUsbDevice->DeviceType = BIOS_DEV_TYPE_KEYBOARD;
            }

            if (NewPeiUsbDevice->DeviceType == BIOS_DEV_TYPE_HUB) {
                Status = PeiDoHubConfig(PeiServices, NewPeiUsbDevice);
                if (EFI_ERROR(Status)) {
                    return Status;
                }
                PeiHubEnumeration(PeiServices, NewPeiUsbDevice,
                        CurrentAddress, FALSE);
            }
        }
    }

    return EFI_SUCCESS;
}


static
EFI_STATUS PeiUsbEnumeration (
    IN EFI_PEI_SERVICES            **PeiServices,
    IN PEI_USB_HOST_CONTROLLER_PPI *UsbHcPpi )
{
    UINT8                NumOfRootPort;
    EFI_STATUS           Status;
    UINT8                Port;
    EFI_USB_PORT_STATUS  PortStatus;
    PEI_USB_DEVICE       *PeiUsbDevice;
	UINT8                *AllocateAddress;
    EFI_PEI_STALL_PPI    *PeiStall;

    (**PeiServices).LocatePpi(
        PeiServices,
        &gEfiPeiStallPpiGuid,
        0,
        NULL,
        (VOID**)&PeiStall
    );

    UsbHcPpi->GetRootHubPortNumber(
        PeiServices,
        UsbHcPpi,
        (UINT8 *) &NumOfRootPort
    );


    for (Port = 1; Port <= NumOfRootPort; Port++) {
        //
        // First get root port status to detect changes happen
        //
        UsbHcPpi->GetRootHubPortStatus(
            PeiServices,
            UsbHcPpi,
            Port,
            &PortStatus
        );

        if ( IsPortConnectChange( PortStatus.PortChangeStatus ) ) {

            //
            // Changes happen, first clear this change status
            //
            UsbHcPpi->ClearRootHubPortFeature(
                PeiServices,
                UsbHcPpi,
                Port,
                EfiUsbPortConnectChange
            );

            if ( IsPortConnect( PortStatus.PortStatus ) ) {

				if (IsPortEnable(PortStatus.PortStatus) == FALSE) {
	                //
	                // First reset and enable this port
	                //
	                ResetRootPort( PeiServices, UsbHcPpi, Port );

	                UsbHcPpi->GetRootHubPortStatus(
	                    PeiServices,
	                    UsbHcPpi,
	                    Port,
	                    &PortStatus
	                );

					if (!IsPortConnect(PortStatus.PortStatus) || 
						!IsPortEnable(PortStatus.PortStatus)) {
						continue;
					}
				}

                //
                // Connect change happen
                //

                if (gEdk2IoMmuPpi) {
                    Status = gEdk2IoMmuPpi->AllocateBuffer (
                                                gEdk2IoMmuPpi,
                                                EfiRuntimeServicesData,
                                                EFI_SIZE_TO_PAGES(sizeof(PEI_USB_DEVICE)),
                                                (VOID**)&AllocateAddress,
                                                EDKII_IOMMU_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                                );
                } else {
                    Status = (*PeiServices)->AllocatePool (
                                                 PeiServices,
                                                 sizeof(PEI_USB_DEVICE),
                                                 (VOID**)&AllocateAddress
                                                 );
                }
                if ( EFI_ERROR( Status ) ) {
                    return EFI_OUT_OF_RESOURCES;
                }

                PeiUsbDevice = (PEI_USB_DEVICE *)AllocateAddress;
                (*PeiServices)->SetMem(PeiUsbDevice, sizeof(PEI_USB_DEVICE), 0);
                //ZeroMem( PeiUsbDevice, sizeof(PEI_USB_DEVICE) );

                PeiUsbDevice->Signature = PEI_USB_DEVICE_SIGNATURE;
                PeiUsbDevice->DeviceAddress = 0;
                PeiUsbDevice->MaxPacketSize0 = 8;
                PeiUsbDevice->DataToggle = 0;
                PeiUsbDevice->UsbIoPpi = gAmiUsbIoPpi;
                PeiUsbDevice->AmiUsbIoPpiList = gAmiUsbIoPpiList;
                PeiUsbDevice->AmiUsbIoPpiList.Ppi = &PeiUsbDevice->UsbIoPpi;
                PeiUsbDevice->UsbIoPpiList = gUsbIoPpiList;
                PeiUsbDevice->UsbIoPpiList.Ppi = &PeiUsbDevice->UsbIoPpi;
                PeiUsbDevice->UsbHcPpi = UsbHcPpi;
                PeiUsbDevice->DeviceSpeed = USB_FULL_SPEED_DEVICE;
                PeiUsbDevice->DeviceType = 0x0;
                PeiUsbDevice->DownStreamPortNo = 0x0;
                PeiUsbDevice->TransactionTranslator = 0x0;
				PeiUsbDevice->HubDepth = 0;

                if (IsPortLowSpeedDeviceAttached(PortStatus.PortStatus)) {
                    PeiUsbDevice->DeviceSpeed = USB_SLOW_SPEED_DEVICE;
                }

                if (IsPortHighSpeedDeviceAttached(PortStatus.PortStatus)) {
                    PeiUsbDevice->DeviceSpeed = USB_HIGH_SPEED_DEVICE;
                    PeiUsbDevice->MaxPacketSize0 = 0x40;
                }

                if (IsPortSuperSpeedDeviceAttached(PortStatus.PortStatus)) {
                    PeiUsbDevice->DeviceSpeed = USB_SUPER_SPEED_DEVICE;
                    PeiUsbDevice->MaxPacketSize0 = 0x200;
                }
                if (IsPortSuperSpeedPlusDeviceAttached(PortStatus.PortStatus)) {
                    PeiUsbDevice->DeviceSpeed = USB_SUPER_SPEED_PLUS_DEVICE;
                    PeiUsbDevice->MaxPacketSize0 = 0x200;
                }

                //
                // Delay some times to enable usb devices to initiate.
                //
                //PeiStall->Stall(
                //    PeiServices,
                //    PeiStall,
                //    5000
                //);

                //
                // Configure that Usb Device
                //
                Status = PeiConfigureUsbDevice(
                    PeiServices,
                    PeiUsbDevice,
                    Port,
                    &UsbHcPpi->CurrentAddress
                         );

                if ( EFI_ERROR( Status ) ) {
                                        //(EIP61827+)>
                    UsbHcPpi->ClearRootHubPortFeature(
                        PeiServices,
                        UsbHcPpi,
                        Port,
                        EfiUsbPortEnable
                    );
                                        //<(EIP61827+)
                    continue;
                }

                Status = (**PeiServices).InstallPpi(PeiServices, 
                            &PeiUsbDevice->AmiUsbIoPpiList);
                ASSERT(Status == EFI_SUCCESS);
                if (EFI_ERROR(Status)) {
                    return Status;
                }

                Status = (**PeiServices).InstallPpi(PeiServices,
                            &PeiUsbDevice->UsbIoPpiList);
                ASSERT(Status == EFI_SUCCESS);
                if (EFI_ERROR(Status)) {
                    return Status;
                }

                // Initialize DeviceType
                if ( (PeiUsbDevice->InterfaceDesc->InterfaceClass == BASE_CLASS_MASS_STORAGE)
                    && (PeiUsbDevice->InterfaceDesc->InterfaceProtocol == PROTOCOL_BOT) )
                {
                    PeiUsbDevice->DeviceType = BIOS_DEV_TYPE_STORAGE;
                }

                if (PeiUsbDevice->InterfaceDesc->InterfaceClass == BASE_CLASS_HUB)
                {
                    PeiUsbDevice->DeviceType = BIOS_DEV_TYPE_HUB;
                }

                if ((PeiUsbDevice->InterfaceDesc->InterfaceClass == BASE_CLASS_HID)
                    && (PeiUsbDevice->InterfaceDesc->InterfaceSubClass == SUB_CLASS_BOOT_DEVICE)
                    && (PeiUsbDevice->InterfaceDesc->InterfaceProtocol == PROTOCOL_KEYBOARD))
                {
                    PeiUsbDevice->DeviceType = BIOS_DEV_TYPE_KEYBOARD;
                }

                if (PeiUsbDevice->DeviceType == BIOS_DEV_TYPE_HUB)
                {
                    Status = PeiDoHubConfig( PeiServices, PeiUsbDevice );
                    if ( EFI_ERROR( Status ) ) {
                        return Status;
                    }

                    PeiHubEnumeration(PeiServices, PeiUsbDevice,
                        &UsbHcPpi->CurrentAddress, FALSE);
                }
            }
        }
    }

    return EFI_SUCCESS;
}


static EFI_STATUS
PeiConfigureUsbDevice (
    IN EFI_PEI_SERVICES **PeiServices,
    IN PEI_USB_DEVICE   *PeiUsbDevice,
    IN UINT8            Port,
    IN OUT UINT8        *DeviceAddress
)
{
    EFI_USB_DEVICE_DESCRIPTOR    DeviceDescriptor;
    EFI_STATUS                   Status;
    PEI_USB_IO_PPI               *UsbIoPpi;
    UINT8                        Index;
    EFI_PEI_STALL_PPI            *StallPpi = NULL;
    EFI_USB_DEVICE_DESCRIPTOR    *DeviceDescriptorPtr = &DeviceDescriptor;
    UINTN                        Size;
    EFI_PHYSICAL_ADDRESS         TempAddress;
    VOID                         *Mapping = NULL;

    ( **PeiServices ).LocatePpi( PeiServices, &gEfiPeiStallPpiGuid,
        0, NULL, (VOID**)&StallPpi );

    if (PeiUsbDevice->UsbHcPpi->PreConfigureDevice != NULL) {
        Status = PeiUsbDevice->UsbHcPpi->PreConfigureDevice( PeiServices,
            PeiUsbDevice, Port);
        if (EFI_ERROR(Status)) {
            DEBUG((DEBUG_ERROR, "Failed to initialize HC specific data for the device\n"));
            return Status;
        }
    }

    UsbIoPpi = &PeiUsbDevice->UsbIoPpi;

    if (gEdk2IoMmuPpi) {
        Size = sizeof(EFI_USB_DEVICE_DESCRIPTOR);
        Status = gEdk2IoMmuPpi->Map (
                     gEdk2IoMmuPpi,
                     EdkiiIoMmuOperationBusMasterWrite,
                     (VOID*)DeviceDescriptorPtr,
                     &Size,
                     (EFI_PHYSICAL_ADDRESS*)&TempAddress,
                     &Mapping
                     );

        if (EFI_ERROR(Status)){
            return Status;
        }

        DeviceDescriptorPtr = (EFI_USB_DEVICE_DESCRIPTOR*)TempAddress;
    }

    //-----------------------------------------------------------------------
    // Try 5 times to read the first 8 bytes to determine the size
    for (Index = 0; Index < 5; Index++) {
        Status = PeiUsbGetDescriptor( PeiServices,
                     UsbIoPpi,
                     USB_DT_DEVICE << 8, // Value = Type << 8 | Index
                     0,                                    // Index
                     8,                                    // DescriptorLength
                     DeviceDescriptorPtr );
        if ((!EFI_ERROR(Status)) || (Status == EFI_TIMEOUT)) {
            break;
        }
        StallPpi->Stall( PeiServices, StallPpi, 100 * 1000 ); // 100msec delay
    }
    if ( EFI_ERROR( Status ) ) {
        DEBUG((DEBUG_ERROR, "Failed to get the first 8 bytes of the device descriptor.\n"));
        return Status;
    }

    //-----------------------------------------------------------------------
    // Set MaxPacketSize0 = 0x40 if packet size is not specified
    if (DeviceDescriptorPtr->BcdUSB >= 0x0300) {
        PeiUsbDevice->MaxPacketSize0 = (UINT16)1 << DeviceDescriptorPtr->MaxPacketSize0;
    } else {
        PeiUsbDevice->MaxPacketSize0 = (DeviceDescriptorPtr->MaxPacketSize0)
                                       ? DeviceDescriptorPtr->MaxPacketSize0
                                       : 0x40;
    }

    //-----------------------------------------------------------------------
    // Get the entire USB device descriptor
    if ((PeiUsbDevice->DeviceSpeed == USB_SLOW_SPEED_DEVICE) ||
        (PeiUsbDevice->DeviceSpeed == USB_FULL_SPEED_DEVICE) ||
        (PeiUsbDevice->DeviceSpeed == USB_HIGH_SPEED_DEVICE)) {
        StallPpi->Stall(PeiServices, StallPpi, PEI_USB_GET_DEVICE_DESCRIPTOR_DELAY_MS * 1000); // 10msec delay
    }
    
    (*DeviceAddress)++;
    
    Status = PeiUsbSetDeviceAddress(
        PeiServices,
        UsbIoPpi,
        *DeviceAddress );
    
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Failed to set device address.\n"));
        return Status;
    }
    PeiUsbDevice->DeviceAddress = *DeviceAddress;
    
    StallPpi->Stall(PeiServices, StallPpi, PEI_USB_SET_ADDRESS_DELAY_MS * 1000); // 2msec delay
    
    //-----------------------------------------------------------------------
    // Get the entire USB device descriptor

    Status = PeiUsbGetDescriptor(
        PeiServices,
        UsbIoPpi,
        USB_DT_DEVICE << 8,   // Value = Type << 8 | Index
        0,                                      // Index
        sizeof(EFI_USB_DEVICE_DESCRIPTOR),      // DescriptorLength
        DeviceDescriptorPtr );
        
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Failed to get the completed device descriptor.\n"));
        return Status;
    }
    //-----------------------------------------------------------------------
    // Get its default configuration and its first interface
    Status = PeiUsbGetAllConfiguration(
        PeiServices,
        PeiUsbDevice );
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    if (PeiUsbDevice->DeviceSpeed == USB_FULL_SPEED_DEVICE) {
        StallPpi->Stall(PeiServices, StallPpi, 100); 
    }

// Note: reducing this delay may cause the keyboard not to respond
//    StallPpi->Stall( PeiServices, StallPpi, 10 * 1000 ); // 10msec delay

    Status = PeiUsbSetConfiguration(
        PeiServices,
        UsbIoPpi );

    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Failed to set configuration.\n"));
        return Status;
    }

    if (Mapping){
        gEdk2IoMmuPpi->Unmap(gEdk2IoMmuPpi, Mapping);
    }

    return EFI_SUCCESS;
}

static EFI_STATUS PeiUsbGetAllConfiguration (
    IN EFI_PEI_SERVICES **PeiServices,
    IN PEI_USB_DEVICE   *Device )
{
    EFI_USB_CONFIG_DESCRIPTOR    ConfigDesc = {0};
    EFI_USB_ENDPOINT_DESCRIPTOR  *EndPointDesc = NULL;  //(EIP32503+)
    EFI_STATUS                   Status;
    PEI_USB_IO_PPI               *UsbIoPpi = &Device->UsbIoPpi;
    UINTN                        Index;
    UINT8                        *LastAddress = 0;
    UINTN                        Size;
    VOID                         *ConfigDescMapping = NULL;
    VOID                         *EndPointDescMapping = NULL;

    if (gEdk2IoMmuPpi) {
        Size = sizeof(EFI_USB_CONFIG_DESCRIPTOR);
        Status = gEdk2IoMmuPpi->Map (
                     gEdk2IoMmuPpi,
                     EdkiiIoMmuOperationBusMasterWrite,
                     (VOID*)&ConfigDesc,
                     &Size,
                     (EFI_PHYSICAL_ADDRESS*)&ConfigDesc,
                     &ConfigDescMapping
                     );
        if (EFI_ERROR(Status)){
            return Status;
        }

        Size = sizeof(EFI_USB_ENDPOINT_DESCRIPTOR);
        Status = gEdk2IoMmuPpi->Map (
                     gEdk2IoMmuPpi,
                     EdkiiIoMmuOperationBusMasterWrite,
                     (VOID*)EndPointDesc,
                     &Size,
                     (EFI_PHYSICAL_ADDRESS*)EndPointDesc,
                     &EndPointDescMapping
                     );
        if (EFI_ERROR(Status)){
            if (ConfigDescMapping){
                gEdk2IoMmuPpi->Unmap(gEdk2IoMmuPpi, ConfigDescMapping);
            }
            return Status;
        }
    }


    // Here we are parsing the descriptors for the device
    // configurations where the hierarchy of descriptors
    // is as follows:
    //
    // +----------------+
    // | Configuration1 |
    // +----------------+
    //        |         +------------+
    //        +---------| Interface1 |----+------------------+
    //        |         +------------+    |                  |
    //        |                     +-----------+       +-------------+
    //        |                     | Endpoint1 |  ...  | EndpointMax |
    //        |                     +-----------+       +-------------+
    //        |                :
    //        |                :
    //        |                :
    //        |
    //        |         +--------------+
    //        +---------| InterfaceMax |----+------------------+
    //                  +--------------+    |                  |
    //        :                       +-----------+       +-------------+
    //        :                       | Endpoint1 |  ...  | EndpointMax |
    //                                +-----------+       +-------------+
    // +------------------+
    // | ConfigurationMax |
    // +------------------+
    //        |         +------------+
    //        +---------| Interface1 |----+------------------+
    //        |         +------------+    |                  |
    //        |                     +-----------+       +-------------+
    //        |                     | Endpoint1 |  ...  | EndpointMax |
    //        |                     +-----------+       +-------------+
    //        |                :
    //        |                :
    //        |                :
    //        |         +--------------+
    //        +---------| InterfaceMax |----+------------------+
    //                  +--------------+    |                  |
    //                                +-----------+       +-------------+
    //                                | Endpoint1 |  ...  | EndpointMax |
    //                                +-----------+       +-------------+


    //-------------------------------------------------------------
    // Fortunately, we are only interested in the first/default
    // configuration and its first/default interface, so life is
    // simple!
    //-------------------------------------------------------------

    //-------------------------------------------------------------
    // First get the device's 9-byte configuration descriptor to
    // determine the length of all descriptors
    Status = PeiUsbGetDescriptor(
        PeiServices,
        UsbIoPpi,
        USB_DT_CONFIG << 8,
        0,
        9,
        &ConfigDesc);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Failed to get the first 9 bytes of the configuration descriptor.\n"));
        return Status;
    }

    if (gEdk2IoMmuPpi) {
        Status = gEdk2IoMmuPpi->AllocateBuffer (
                                    gEdk2IoMmuPpi,
                                    EfiRuntimeServicesData,
                                    EFI_SIZE_TO_PAGES(ConfigDesc.TotalLength),
                                    (VOID**)&Device->ConfigurationData,
                                    EDKII_IOMMU_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                    );
    } else {
        Status = (*PeiServices)->AllocatePool( PeiServices,
            ConfigDesc.TotalLength, (VOID**)&Device->ConfigurationData );
    }
    if (EFI_ERROR(Status)) {
        return Status;
    }
    //-------------------------------------------------------------
    // Get all the descriptors for this configuration using
    // TotalLength from the first 9 bytes previously read.
    // Then, save the Configuration descriptor into the
    // device management structure.
    Status = PeiUsbGetDescriptor(
        PeiServices,
        UsbIoPpi,
        USB_DT_CONFIG << 8,
        0,
        ConfigDesc.TotalLength,
        (VOID *) Device->ConfigurationData);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Failed to get the completed configuration descriptor.\n"));
        return Status;
    }
    Device->ConfigDesc =
        (EFI_USB_CONFIG_DESCRIPTOR *) Device->ConfigurationData;

    LastAddress = Device->ConfigurationData +
                  Device->ConfigDesc->TotalLength - 1;

    if (Device->UsbHcPpi->EnableEndpoints != NULL) {
        Status = Device->UsbHcPpi->EnableEndpoints(PeiServices,
                    Device->UsbHcPpi, Device->DeviceAddress, Device->ConfigurationData);
    
        if (EFI_ERROR(Status)) {
            DEBUG((DEBUG_ERROR, "Failed to enable endpoints\n"));
            return Status;
        }
    }

    //--------------------------------------------------------------
    // Assume the Interface descriptor is directly after the
    // configuration descriptor.
    //--------------------------------------------------------------
    Device->InterfaceDesc = (EFI_USB_INTERFACE_DESCRIPTOR *)
                            ( (UINT8 *) Device->ConfigDesc +
                             Device->ConfigDesc->Length );
										//(EIP32503+)>
	while ((UINT8 *)Device->InterfaceDesc < LastAddress && 
			Device->InterfaceDesc->DescriptorType != USB_DT_INTERFACE) {
    	if (Device->InterfaceDesc->Length==0) {
            return Status;    //(EIP54701+)
        }

		Device->InterfaceDesc = (EFI_USB_INTERFACE_DESCRIPTOR *)
                            ( (UINT8 *) Device->InterfaceDesc +
                             Device->InterfaceDesc->Length );
	}
										//<(EIP32503+)
    //--------------------------------------------------------------
    // Assume the first Endpoint descriptor is directly after the
    // Interface descriptor.
    //--------------------------------------------------------------
    									//(EIP32503)>
    EndPointDesc = (EFI_USB_ENDPOINT_DESCRIPTOR *)
                   ( (UINT8 *) Device->InterfaceDesc +
                   Device->InterfaceDesc->Length );

    for (Index = 0; Index < Device->InterfaceDesc->NumEndpoints && 
		(UINT8 *)EndPointDesc < LastAddress;) {
		if (EndPointDesc->DescriptorType == USB_DT_ENDPOINT) {
			Device->EndpointDesc[Index++] = EndPointDesc;
		}
		EndPointDesc = (EFI_USB_ENDPOINT_DESCRIPTOR *)
    	               ( (UINT8 *) EndPointDesc +
        	           EndPointDesc->Length );
    }
										//<(EIP32503)
    if (gEdk2IoMmuPpi && ConfigDescMapping){
        gEdk2IoMmuPpi->Unmap(gEdk2IoMmuPpi, ConfigDescMapping);
    }
    if (gEdk2IoMmuPpi && EndPointDescMapping){
        gEdk2IoMmuPpi->Unmap(gEdk2IoMmuPpi, EndPointDescMapping);
    }

    return EFI_SUCCESS;
}


//
// Send reset signal over the given root hub port
//
VOID ResetRootPort (
    IN EFI_PEI_SERVICES            **PeiServices,
    IN PEI_USB_HOST_CONTROLLER_PPI *UsbHcPpi,
    UINT8                          PortNum )
{
    EFI_PEI_STALL_PPI *PeiStall;
	EFI_USB_PORT_STATUS  PortStatus;
	UINT8	i;

    ( **PeiServices ).LocatePpi(
        PeiServices,
        &gEfiPeiStallPpiGuid,
        0,
        NULL,
        (VOID**)&PeiStall
    );


    //
    // reset root port
    //
    UsbHcPpi->SetRootHubPortFeature(
        PeiServices,
        UsbHcPpi,
        PortNum,
        EfiUsbPortReset
    );

	for (i = 0; i < 100; i++) {
        UsbHcPpi->GetRootHubPortStatus(
            PeiServices,
            UsbHcPpi,
            PortNum,
            &PortStatus
        );

		if ((PortStatus.PortChangeStatus & USB_PORT_STAT_C_RESET) != 0) {
			break;
		}

	    PeiStall->Stall(
	        PeiServices,
	        PeiStall,
	        1 * 1000            // NVS - Changed to 10 msec (as per AMI USB Code)
	    );
	}

	if ((PortStatus.PortChangeStatus & USB_PORT_STAT_C_RESET) == 0) {
		return;
	}

    //
    // clear reset root port
    //
    UsbHcPpi->ClearRootHubPortFeature(
        PeiServices,
        UsbHcPpi,
        PortNum,
        EfiUsbPortResetChange
    );

    UsbHcPpi->GetRootHubPortStatus(
        PeiServices,
        UsbHcPpi,
        PortNum,
        &PortStatus
    );
    
    // The USB System Software guarantees a minimum of 10 ms for reset recovery.
    PeiStall->Stall(PeiServices, PeiStall, PEI_USB_RESET_PORT_DELAY_MS * 1000);

    return;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
