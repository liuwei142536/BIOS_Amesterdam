//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file VirtualSerial.c
    Create devicepath for the Virtual Serial Device. So that
    SerialIO can consume this device and redirection can be done
    via this Serial device.Also ACPI data for VirtualSerialDevices 
    were updated at ReadyToBootEvent.

**/

//---------------------------------------------------------------------------

#include "VirtualSerial.h"
#include "VirtualSerialDeviceElinks.h"

//---------------------------------------------------------------------------

#ifndef _CR
#define _CR( Record, TYPE, Field ) \
    ( (TYPE *) ( (CHAR8 *) (Record) - (CHAR8 *) &( ( (TYPE *) 0 )->Field ) ) )
#endif

#define VIRTUAL_SERIAL_DATA_FROM_EFISIO(a) \
  _CR ( \
  (a), \
  VIRTUAL_DEV_DATA, \
  EfiSio \
)

EFI_GUID            gDxeSvcTblGuid = DXE_SERVICES_TABLE_GUID;
extern EFI_GUID     gEfiAmiSioProtocolGuid;
extern EFI_GUID     gEfiSioProtocolGuid;
static DXE_SERVICES        *gDxeSvcTbl=NULL;

UINTN MAXIMUM_NO_VIRTUAL_DEVICES; 
VIRTUAL_SERIAL_DETAILS VirtualSerialDevices[]= {
        VIRTUAL_SERIAL_DEVICE_LIST
        {"0",0xFFFF, 0xFFFF, 0xFF}
};

T_ITEM_LIST                                   VirtualSerialResources;
EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR    VirtualSerialBaseAddress;
EFI_ACPI_IRQ_DESCRIPTOR                       VirtualSerialIrq;
EFI_ACPI_EXTENDED_INTERRUPT_DESCRIPTOR        VirtualSerialExtendedIrq;
VOID                                          *ResourceList[2];

//
// AMI SIO protocol API definition
//

/**
    Get the Virtual Serial Device Register details

    @param  This         - Indicates a pointer to the calling context.
    @param  Write        - Specifies the type of the register operation Read/Write.
    @param  ExitCfgMode  - Exit Configuration Mode Indicator.
    @param  Register     - Register number.
    @param  Value        - Buffer to perform Read/Write

    @retval  EFI_UNSUPPORTED - This functionality is not supported 
                               for virtual Serial device

**/
EFI_STATUS
EFIAPI
VirtualSerialRegister (
    IN  AMI_SIO_PROTOCOL     *This,
    IN  BOOLEAN              Write,
    IN  BOOLEAN              ExitCfgMode,
    IN  UINT8                Register,
    IN  OUT UINT8            *Value
 )
{

    return EFI_UNSUPPORTED;
}

/**
    Get the Virtual Serial Device current resource

    @param  This      - Indicates a pointer to the calling context.
    @param  Set       - Specifies the type of the register operation.
    @param  Resources - A pointer to T_ITEM_LIST.

    @retval EFI_SUCCESS           - Retrieved the virtual serial device current resource
            EFI_INVALID_PARAMETER - Resources is NULL
            EFI_UNSUPPORTED       - Set operation is not supported

**/ 
EFI_STATUS 
EFIAPI
VirtualSerialCRS (
    IN  AMI_SIO_PROTOCOL     *This,
    IN  BOOLEAN              Set,
    IN  OUT T_ITEM_LIST      **Resources
 )
{

    VIRTUAL_DEV_DATA*    VirtualDevice=(VIRTUAL_DEV_DATA*)This;

    if (!VirtualDevice || !Resources) {
        return EFI_INVALID_PARAMETER;
    }

    if (Set) {
        return EFI_UNSUPPORTED;
    } else {

        // Virtual Serial Device Base address Resource
        VirtualSerialBaseAddress.Header.Bits.Length = sizeof(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR) - sizeof(ACPI_SMALL_RESOURCE_HEADER);
        VirtualSerialBaseAddress.Header.Bits.Name   = ACPI_SMALL_FIXED_IO_PORT_DESCRIPTOR_NAME;
        VirtualSerialBaseAddress.Header.Bits.Type   = ACPI_SMALL_ITEM_FLAG;
        VirtualSerialBaseAddress.BaseAddress        = VirtualDevice->DevBase;
        VirtualSerialBaseAddress.Length             = 0x8;

        
        ResourceList[0]                     =(VOID*)&VirtualSerialBaseAddress;
        
        if (VirtualDevice->DevIrq <= MAXIMUM_ISA_IRQ_NUMBER) {
            //Virtual Device IRQ details.
            VirtualSerialIrq.Header.Bits.Length = sizeof(EFI_ACPI_IRQ_DESCRIPTOR) - sizeof(ACPI_SMALL_RESOURCE_HEADER);
            VirtualSerialIrq.Header.Bits.Name   = ACPI_SMALL_IRQ_DESCRIPTOR_NAME;
            VirtualSerialIrq.Header.Bits.Type   = ACPI_SMALL_ITEM_FLAG;
            VirtualSerialIrq.Mask               = (UINT16)(1 << VirtualDevice->DevIrq);
            
            //Refer ACPI spec v6.2 section 6.4.2.1, in field "IRQ information" 
            //Bit[0] Interrupt Mode - Edge or Level trigger 
            //Bit[3] Interrupt Polarity - Active Low or High
            //Bit[5:4] Interrupt Sharing type
            VirtualSerialIrq.Information  = ((VirtualDevice->IrqInfo.Bits.IntMode  << 0) |\
                                             (VirtualDevice->IrqInfo.Bits.IntLevel << 3) |\
                                             (VirtualDevice->IrqInfo.Bits.IntSharType << 4));
            
            ResourceList[1]               = (VOID*)&VirtualSerialIrq;
        } else {
            VirtualSerialExtendedIrq.Header.Header.Bits.Type   = ACPI_LARGE_ITEM_FLAG;
            VirtualSerialExtendedIrq.Header.Header.Bits.Name   = ACPI_LARGE_EXTENDED_IRQ_DESCRIPTOR_NAME;
            VirtualSerialExtendedIrq.Header.Length = sizeof(EFI_ACPI_EXTENDED_INTERRUPT_DESCRIPTOR) - sizeof(ACPI_LARGE_RESOURCE_HEADER);
            
            //Refer ACPI Spec v6.2 section 6.4.3.6, in field "Interrupt Vector Flags"
            //Bit[0] Consumer/Producer
            //Bit[1] Interrupt Mode - Edge or Level trigger 
            //Bit[2] Interrupt Polarity - Active Low or High
            //Bit[4:3] Interrupt Sharing type
            VirtualSerialExtendedIrq.InterruptVectorFlags = ((RESOURCE_CONSUMER << 0) |\
                                                             (VirtualDevice->IrqInfo.Bits.IntMode  << 1) |\
                                                             (VirtualDevice->IrqInfo.Bits.IntLevel << 2) |\
                                                             (VirtualDevice->IrqInfo.Bits.IntSharType << 3));
            VirtualSerialExtendedIrq.InterruptTableLength = 1;
            VirtualSerialExtendedIrq.InterruptNumber[0]      = VirtualDevice->DevIrq;

            ResourceList[1]                               = (VOID*)&VirtualSerialExtendedIrq;
        }

        //Set the Resource details
        VirtualSerialResources.InitialCount = 2;
        VirtualSerialResources.ItemCount    = 2;
        VirtualSerialResources.Items        =(VOID*)&ResourceList[0];

        *Resources                          = &VirtualSerialResources;
    }

    return EFI_SUCCESS;
}
/**
    Provides combination of resources that can be used by the device
    
    @param  This      - Indicates a pointer to the calling context.
    @param  Set       - Specifies the type of the register operation.
    @param  Resources - A pointer to T_ITEM_LIST.

    @retval EFI_UNSUPPORTED - This functionality is not supported.

**/ 
EFI_STATUS 
EFIAPI
VirtualSerialPRS (
    IN  AMI_SIO_PROTOCOL     *This,
    IN  BOOLEAN              Set,
    IN  OUT T_ITEM_LIST      **Resources
 )
{
    return EFI_UNSUPPORTED;
}

//
// EFI SIO protocol API definition
//

/**
    Accesses the Virtual Serial Device Registers
        
    @param  This         - Indicates a pointer to the calling context.
    @param  Write        - Specifies the type of the register operation Read/Write.
    @param  ExitCfgMode  - Exit Configuration Mode Indicator.
    @param  Register     - Register number.
    @param  Value        - Buffer to perform Read/Write

    @retval  EFI_UNSUPPORTED - This functionality is not supported for virtual Serial device

**/
EFI_STATUS
EFIAPI
VirtualEfiSioRegisterAccess (
    IN  CONST  EFI_SIO_PROTOCOL  *This,
    IN  BOOLEAN                  Write,
    IN  BOOLEAN                  ExitCfgMode,
    IN  UINT8                    Register,
    IN  OUT   UINT8              *Value
 )
{
    return EFI_UNSUPPORTED;
}

/**
    Provides an interface to get a list of the current resources consumed by the device in the ACPI
    Resource Descriptor format.

    @param  This            - Indicates a pointer to the calling context.
    @param  EfiResourceList - A pointer to an ACPI resource descriptor list that defines the current 
                              resources used by the device.

    @retval  EFI_SUCCESS           - The operation completed successfully
    @retval  EFI_INVALID_PARAMETER - EfiResourceList is NULL

**/
EFI_STATUS
EFIAPI
VirtualEfiSioGetResources (
    IN  CONST EFI_SIO_PROTOCOL      *This,
    OUT ACPI_RESOURCE_HEADER_PTR    *EfiResourceList
 )

{
    VIRTUAL_DEV_DATA*    VirtualDevice;
    VOID                 *EfiSioRes = NULL;
    UINT8                *TempRes = NULL;
    UINT32               Size = 0;

    if (!This || !EfiResourceList)
        return EFI_INVALID_PARAMETER;

    VirtualDevice = VIRTUAL_SERIAL_DATA_FROM_EFISIO(This);

    // Virtual Serial Device Base address Resource
    VirtualSerialBaseAddress.Header.Bits.Length = sizeof(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR) - sizeof(ACPI_SMALL_RESOURCE_HEADER);
    VirtualSerialBaseAddress.Header.Bits.Name   = ACPI_SMALL_FIXED_IO_PORT_DESCRIPTOR_NAME;
    VirtualSerialBaseAddress.Header.Bits.Type   = ACPI_SMALL_ITEM_FLAG;
    VirtualSerialBaseAddress.BaseAddress        = VirtualDevice->DevBase;
    VirtualSerialBaseAddress.Length             = 0x8;
    Size += sizeof(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR);

    if (VirtualDevice->DevIrq <= MAXIMUM_ISA_IRQ_NUMBER) {
        //Virtual Device IRQ details.
        VirtualSerialIrq.Header.Bits.Length = sizeof(EFI_ACPI_IRQ_DESCRIPTOR) - sizeof(ACPI_SMALL_RESOURCE_HEADER);
        VirtualSerialIrq.Header.Bits.Name   = ACPI_SMALL_IRQ_DESCRIPTOR_NAME;
        VirtualSerialIrq.Header.Bits.Type   = ACPI_SMALL_ITEM_FLAG;
        VirtualSerialIrq.Mask               = (UINT16)(1 << VirtualDevice->DevIrq);
        
        //Refer ACPI spec v6.2 section 6.4.2.1, in field "IRQ information" 
        //Bit[0] Interrupt Mode - Edge or Level trigger 
        //Bit[3] Interrupt Polarity - Active Low or High
        //Bit[5:4] Interrupt Sharing type
        VirtualSerialIrq.Information  = ((VirtualDevice->IrqInfo.Bits.IntMode  << 0) |\
                                         (VirtualDevice->IrqInfo.Bits.IntLevel << 3) |\
                                         (VirtualDevice->IrqInfo.Bits.IntSharType << 4));
                
        Size += sizeof(EFI_ACPI_IRQ_DESCRIPTOR);
    } else {
        VirtualSerialExtendedIrq.Header.Header.Bits.Type   = ACPI_LARGE_ITEM_FLAG;
        VirtualSerialExtendedIrq.Header.Header.Bits.Name   = ACPI_LARGE_EXTENDED_IRQ_DESCRIPTOR_NAME;
        VirtualSerialExtendedIrq.Header.Length = sizeof(EFI_ACPI_EXTENDED_INTERRUPT_DESCRIPTOR) - sizeof(ACPI_LARGE_RESOURCE_HEADER);
        
        //Refer ACPI Spec v6.2 section 6.4.3.6, in field "Interrupt Vector Flags"
        //Bit[0] Consumer/Producer
        //Bit[1] Interrupt Mode - Edge or Level trigger 
        //Bit[2] Interrupt Polarity - Active Low or High
        //Bit[4:3] Interrupt Sharing type
        VirtualSerialExtendedIrq.InterruptVectorFlags = ((RESOURCE_CONSUMER << 0) |\
                                                         (VirtualDevice->IrqInfo.Bits.IntMode  << 1) |\
                                                         (VirtualDevice->IrqInfo.Bits.IntLevel << 2) |\
                                                         (VirtualDevice->IrqInfo.Bits.IntSharType << 3));
        VirtualSerialExtendedIrq.InterruptTableLength = 1;
        VirtualSerialExtendedIrq.InterruptNumber[0]      = VirtualDevice->DevIrq;
        
        Size += sizeof(EFI_ACPI_EXTENDED_INTERRUPT_DESCRIPTOR);
    }

    EfiSioRes =  TempRes = Malloc(Size + sizeof(EFI_ACPI_END_TAG_DESCRIPTOR));

    if(!EfiSioRes)
        return EFI_OUT_OF_RESOURCES;

    MemCpy(TempRes, &VirtualSerialBaseAddress, sizeof(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR));
    TempRes += sizeof(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR);
    
    if (VirtualDevice->DevIrq <= MAXIMUM_ISA_IRQ_NUMBER) {
        MemCpy(TempRes, &VirtualSerialIrq, sizeof(EFI_ACPI_IRQ_DESCRIPTOR));
        TempRes += sizeof(EFI_ACPI_IRQ_DESCRIPTOR);
        
    } else {
        MemCpy(TempRes, &VirtualSerialExtendedIrq, sizeof(EFI_ACPI_EXTENDED_INTERRUPT_DESCRIPTOR));
        TempRes += sizeof(EFI_ACPI_EXTENDED_INTERRUPT_DESCRIPTOR);
    }
    ((EFI_ACPI_END_TAG_DESCRIPTOR*)TempRes)->Desc     = ACPI_END_TAG_DESCRIPTOR;
    ((EFI_ACPI_END_TAG_DESCRIPTOR*)TempRes)->Checksum = 0;

    EfiResourceList->SmallHeader = EfiSioRes;

    return EFI_SUCCESS;
}

/**
    Sets the Virtual Serial Device current resource

    @param  This            - Indicates a pointer to the calling context.
    @param  EfiResourceList - A pointer to an ACPI resource descriptor list that defines the current resources used by
                              the device

    @retval  EFI_UNSUPPORTED - This functionality is not supported for virtual Serial device

**/
EFI_STATUS
EFIAPI
VirtualEfiSioSetResources (
    IN  CONST  EFI_SIO_PROTOCOL     *This,
    IN  ACPI_RESOURCE_HEADER_PTR    ResourceList
 )
{
    return EFI_UNSUPPORTED;
}

/**
    Provides combination of resources that can be used by the device

    @param  This                - Indicates a pointer to the calling context.
    @param  ResourceCollection  - Collection of the resource descriptor lists.

    @retval  EFI_UNSUPPORTED - This functionality is not supported for virtual Serial device

**/
EFI_STATUS
EFIAPI
VirtualEfiSioPossibleResources (
    IN  CONST EFI_SIO_PROTOCOL     *This,
    OUT ACPI_RESOURCE_HEADER_PTR   *ResourceCollection
 )
{
    return EFI_UNSUPPORTED;
}

/**
    Interface for multiple registers programming with single call

    @param  This             - Indicates a pointer to the calling context.
    @param  Command          - A pointer to an array of NumberOfCommands EFI_SIO_REGISTER_MODIFY structures. 
    @param  NumberOfCommands - Number of elements in the Command array.

    @retval  EFI_UNSUPPORTED - This functionality is not supported for virtual Serial device

**/
EFI_STATUS
EFIAPI
VirtualEfiSioModify (
    IN  CONST EFI_SIO_PROTOCOL         *This,
    IN  CONST EFI_SIO_REGISTER_MODIFY  *Command,
    IN  UINTN                          NumberOfCommands
 )
{
    return EFI_UNSUPPORTED;
}

/**
    Call back for READY_TO_BOOT_EVENT to update VirtualSerial Device
    Base address, UID and IRQ values in ACPI table.

    @param Event   - Signaled event
    @param Context - Pointer to event context
    
    @retval VOID

**/
VOID 
EFIAPI 
UpdateVirtualSerialAcpiData (
    IN  EFI_EVENT Event,
    IN  VOID      *Context
 )
{
    EFI_STATUS              Status;
    ACPI_HDR                *dsdt;
    EFI_PHYSICAL_ADDRESS    DsdtAddress = 0;   
    UINTN i;
    
    // Get DSDT table pointer
    Status = LibGetDsdt(&DsdtAddress,EFI_ACPI_TABLE_VERSION_ALL);
    if(EFI_ERROR(Status)){
        return;
    } else {
        dsdt = (ACPI_HDR*)DsdtAddress;
    }
    
    //Update UID, BaseAddress and IRQ values for all VirtualSerial devices in ACPI table 
    for(i=0;i<MAXIMUM_NO_VIRTUAL_DEVICES;i++) {       
        Status = UpdateAslNameOfDevice(dsdt, VirtualSerialDevices[i].AslDevicename, "UIDN", VirtualSerialDevices[i].Uid);  
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return;

        Status = UpdateAslNameOfDevice(dsdt, VirtualSerialDevices[i].AslDevicename, "VBAN", VirtualSerialDevices[i].BaseAddress);  
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return;
       
        Status = UpdateAslNameOfDevice(dsdt, VirtualSerialDevices[i].AslDevicename, "VIRQ", VirtualSerialDevices[i].IrqNum);  
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return;
        
        Status = UpdateAslNameOfDevice(dsdt, VirtualSerialDevices[i].AslDevicename, "VINM", VirtualSerialDevices[i].InterruptMode);  
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return;
        
        Status = UpdateAslNameOfDevice(dsdt, VirtualSerialDevices[i].AslDevicename, "VILL", VirtualSerialDevices[i].InterruptLevel);  
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return;

        Status = UpdateAslNameOfDevice(dsdt, VirtualSerialDevices[i].AslDevicename, "VISH", VirtualSerialDevices[i].InterruptShareType);  
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return;
    }
    // Now update DSDT table checksum 
    dsdt->Checksum = 0;
    dsdt->Checksum = ChsumTbl((UINT8*)dsdt, dsdt->Length);
        
    pBS->CloseEvent(Event);
        
    return;
}

/**
    Initialize the Virtual Serial Device

    @param - VOID

    @retval - EFI_SUCCESS

**/
EFI_STATUS
InitilizeVirtualSerialDevice (
 )
{
    return EFI_SUCCESS;
}

/**
    Virtual Serial device Entry point. It creates the device path
    for the Virtual Serial device

    @param  ImageHandle  -  Image handle for this driver.
    @param  SystemTable  -  Pointer to the EFI system table.

    @retval EFI_SUCCESS          - The function completed successfully.
            EFI_NOT_FOUND        - DXE_SERVICES_TABLE is not found.
            EFI_OUT_OF_RESOURCES - The pool requested could not be allocated.

**/ 
EFI_STATUS
EFIAPI
VirtualSerialEntryPoint (
    IN  EFI_HANDLE           ImageHandle,
    IN  EFI_SYSTEM_TABLE     *SystemTable
 )
{
    EFI_STATUS              Status;
    EFI_HANDLE              VirtualDeviceHandle=NULL;
    EFI_PHYSICAL_ADDRESS    addr;
    UINT16                  Len =8;
    UINT8                   Aln=0;
    UINT16                  IrqMask;
    UINT32                  Count=0;
    BOOLEAN                 Flag = FALSE;
    EFI_EVENT               UpdateVirtualSerialAcpiDataEvent = NULL;
    VIRTUAL_SERIAL_DEVICEPATH   *VirtualSerialDevicePath = NULL;
    VIRTUAL_DEV_DATA            *VirtualDevice = NULL;

    // Initialize the EFI driver library
    InitAmiLib(ImageHandle, SystemTable);

    gDxeSvcTbl=(DXE_SERVICES*)GetEfiConfigurationTable(SystemTable,
                                &gDxeSvcTblGuid);
    
    if(!gDxeSvcTbl) {
        return EFI_NOT_FOUND;
    }
    //Calculate VirtualSerialDevice Count
    MAXIMUM_NO_VIRTUAL_DEVICES = (sizeof(VirtualSerialDevices) / sizeof(VIRTUAL_SERIAL_DETAILS)) - 1;

    // Initialize the Virtual Serial Device
    InitilizeVirtualSerialDevice();
    
    // Create an event to update VirtualSerialDevice resources on ReadyToBootEvent
    Status = CreateReadyToBootEvent(TPL_NOTIFY,
                                UpdateVirtualSerialAcpiData,
                                NULL,
                                &UpdateVirtualSerialAcpiDataEvent);
       
    if (EFI_ERROR(Status)) {
        return Status;
   }
    
    Status = pBS->AllocatePool(EfiBootServicesData, ((sizeof (VIRTUAL_SERIAL_DEVICEPATH))*MAXIMUM_NO_VIRTUAL_DEVICES), (VOID**)&VirtualSerialDevicePath);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) {
        pBS->CloseEvent(UpdateVirtualSerialAcpiDataEvent);
        return EFI_OUT_OF_RESOURCES;
    }
    Status = pBS->AllocatePool(EfiBootServicesData, ((sizeof (VIRTUAL_DEV_DATA))*MAXIMUM_NO_VIRTUAL_DEVICES), (VOID**)&VirtualDevice);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) {
        pBS->CloseEvent(UpdateVirtualSerialAcpiDataEvent);
        pBS->FreePool(VirtualSerialDevicePath);
        return EFI_OUT_OF_RESOURCES;
    }
    for(Count=0;Count<MAXIMUM_NO_VIRTUAL_DEVICES;Count++) {

        addr=(EFI_PHYSICAL_ADDRESS)VirtualSerialDevices[Count].BaseAddress;
        Status=gDxeSvcTbl->AddIoSpace (
                            EfiGcdIoTypeIo,
                            addr,
                            Len
                            );
        if(EFI_ERROR(Status)) {
            continue;
        }

        // Inform the generic driver that Virtual Serial device base address
        //  is used. So that nobody else can use the resource
        Status=gDxeSvcTbl->AllocateIoSpace(EfiGcdAllocateAddress,
                                        EfiGcdIoTypeIo,
                                        Aln,
                                        Len, 
                                        &addr,
                                        ImageHandle,
                                        NULL
                                        );
        if(EFI_ERROR(Status)|| 
                ((UINT16)addr != VirtualSerialDevices[Count].BaseAddress)) {
            continue;
        }
        
        if (VirtualSerialDevices[Count].IrqNum <= MAXIMUM_ISA_IRQ_NUMBER) {
            Status=AmiIsaIrqMask(&IrqMask, TRUE);

            if(Status==EFI_NOT_FOUND){
                IrqMask=ISA_IRQ_MASK;
                IrqMask |= (UINT16)(1 << VirtualSerialDevices[Count].IrqNum);
                Status = AmiIsaIrqMask(&IrqMask, FALSE);
                ASSERT_EFI_ERROR(Status);
            } else {
                IrqMask |= (UINT16)(1 << VirtualSerialDevices[Count].IrqNum);
                Status=AmiIsaIrqMask(&IrqMask, FALSE);
                ASSERT_EFI_ERROR(Status);
            }
        }
        //Create DevicePath for each VirtualSerial device
        VirtualSerialDevicePath[Count].AcpiDevicePath.Header.Type = ACPI_DEVICE_PATH;
        VirtualSerialDevicePath[Count].AcpiDevicePath.Header.SubType =  ACPI_DP;    
        VirtualSerialDevicePath[Count].AcpiDevicePath.Header.Length[0] = (UINT8) (sizeof (ACPI_HID_DEVICE_PATH));   
        VirtualSerialDevicePath[Count].AcpiDevicePath.Header.Length[1] = (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8);    
        VirtualSerialDevicePath[Count].AcpiDevicePath.HID = EISA_PNP_ID (0x501),
        VirtualSerialDevicePath[Count].AcpiDevicePath.UID = VirtualSerialDevices[Count].Uid;
                        
        VirtualSerialDevicePath[Count].EndDevicePath.Type = END_DEVICE_PATH;    
        VirtualSerialDevicePath[Count].EndDevicePath.SubType = END_ENTIRE_SUBTYPE;
        VirtualSerialDevicePath[Count].EndDevicePath.Length[0] =  (UINT8)sizeof(EFI_DEVICE_PATH_PROTOCOL);
        VirtualSerialDevicePath[Count].EndDevicePath.Length[1] =  0;
        
        VirtualDevice[Count].AmiSio.Access          = VirtualSerialRegister;
        VirtualDevice[Count].AmiSio.CurrentRes      = VirtualSerialCRS;
        VirtualDevice[Count].AmiSio.PossibleRes     = VirtualSerialPRS;

        VirtualDevice[Count].EfiSio.RegisterAccess     = VirtualEfiSioRegisterAccess;
        VirtualDevice[Count].EfiSio.GetResources       = VirtualEfiSioGetResources;
        VirtualDevice[Count].EfiSio.SetResources       = VirtualEfiSioSetResources;
        VirtualDevice[Count].EfiSio.PossibleResources  = VirtualEfiSioPossibleResources;
        VirtualDevice[Count].EfiSio.Modify             = VirtualEfiSioModify;

        VirtualDevice[Count].DevBase        = VirtualSerialDevices[Count].BaseAddress;
        VirtualDevice[Count].DevIrq         = VirtualSerialDevices[Count].IrqNum;
        VirtualDevice[Count].DevDma         = 0;
        
        VirtualDevice[Count].IrqInfo.Bits.IntLevel = VirtualSerialDevices[Count].InterruptLevel;
        VirtualDevice[Count].IrqInfo.Bits.IntMode = VirtualSerialDevices[Count].InterruptMode;
        VirtualDevice[Count].IrqInfo.Bits.IntSharType = (0x3 & VirtualSerialDevices[Count].InterruptShareType);
        
		VirtualDevice[Count].Signature = VIRTUAL_SERIAL_SIG;
        
        Status=pBS->InstallMultipleProtocolInterfaces( 
                                            &VirtualDeviceHandle, 
                                            &guidDevicePath, 
                                            &VirtualSerialDevicePath[Count], 
                                            &gEfiAmiSioProtocolGuid, 
                                            &VirtualDevice[Count].AmiSio, 
                                            &gEfiSioProtocolGuid,
                                            &VirtualDevice[Count].EfiSio,
                                            NULL 
                                            );
        if(!EFI_ERROR(Status)) {
            Flag = TRUE;
        }
    }
    if(Flag == FALSE) {
        return EFI_UNSUPPORTED;  
    }

    return EFI_SUCCESS;
}
