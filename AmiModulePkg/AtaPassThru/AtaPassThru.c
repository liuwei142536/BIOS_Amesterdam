//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AtaPassThru.c
    AtaPassThru Protocol Implementation

**/

//---------------------------------------------------------------------------

#include  "AtaPassThruSupport.h"

//---------------------------------------------------------------------------

#ifndef EXECUTE_DEVICE_DIAGNOSTIC
#define EXECUTE_DEVICE_DIAGNOSTIC       0x90
#endif

AMI_ATA_PASS_THRU_INIT_PROTOCOL *gAtaPassThruInitProtocol;
ATA_PASS_THRU_PROTOCOL          *AtaPassThruInterface;
EFI_ATA_PASS_THRU_MODE          *Mode;

#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
PLATFORM_IDE_PROTOCOL                   *gPlatformIdeProtocol;
#endif

/**@internal
    Driver Entry Point provides services allowing ATA Pass Thru
    commands to be sent to ATA Devices attached to controller

    @param    ImageHandle
    @param    *SystemTable

    @retval  EFI_SUCCESS    The protocol interface was installed.
    @retval  EFI_OUT_OF_RESOURCES  Space for a new handle could not be allocated.
    @retval  EFI_INVALID_PARAMETER Handle or Protocol is NULL
    
    @endinternal
**/

EFI_STATUS
EFIAPI 
AtaPassThruEntry (
    IN  EFI_HANDLE              ImageHandle,
    IN  EFI_SYSTEM_TABLE        *SystemTable
)
{
    EFI_STATUS                  Status;
    EFI_HANDLE                  Handle = NULL;

    InitAmiLib(ImageHandle,SystemTable);

    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof(AMI_ATA_PASS_THRU_INIT_PROTOCOL),
                                (VOID**)&gAtaPassThruInitProtocol );

    if(EFI_ERROR(Status)) return Status;

    // Install AtaPAssThruInit Protocol.
    gAtaPassThruInitProtocol->InstallAtaPassThru        = InstallAtaPassThru;
    gAtaPassThruInitProtocol->StopAtaPassThruSupport    = StopAtaPassThruSupport;

    Status = pBS->InstallProtocolInterface(
                                &Handle,
                                &gAmiAtaPassThruInitProtocolGuid,
                                EFI_NATIVE_INTERFACE,
                                gAtaPassThruInitProtocol
                                );
    ASSERT_EFI_ERROR( Status );

    return Status;
}

/**
    Installs AtaPassThru APIs, which provides services allowing
    ATA commands to be sent to ATA Devices attached to controller

    @param    Controller    Controller handle
    @param    ModeFlag      1 - AHCI interface, 0 - IDE Interface

    @retval   EFI_SUCCESS  EfiAtaPassThru protocol is installed successfully
    @retval   EFI_OUT_OF_RESOURCES The pool requested for the interface could not be allocated.

**/

EFI_STATUS
EFIAPI
InstallAtaPassThru (
  IN    EFI_HANDLE              Controller,
  IN    BOOLEAN                 ModeFlag
)
{
    EFI_STATUS                  Status;
    EFI_HANDLE                  DeviceHandle;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE       *pSataDevInterface   = NULL;
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    AMI_IDE_BUS_PROTOCOL        *pIdeBusInterface    = NULL;
#endif
    UINT16                      DevCount = 0;


    DeviceHandle = (EFI_HANDLE)Controller;

    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof(ATA_PASS_THRU_PROTOCOL),
                                (VOID**)&AtaPassThruInterface
                                );
    if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;

    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof(EFI_ATA_PASS_THRU_MODE),
                                (VOID**)&Mode
                                );
    if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;

    // Initialize all the protocol members
    AtaPassThruInterface->EfiAtaPassThru.Mode              =   Mode;
    AtaPassThruInterface->EfiAtaPassThru.PassThru          =   PassThru;
    AtaPassThruInterface->EfiAtaPassThru.GetNextPort       =   GetNextPort;
    AtaPassThruInterface->EfiAtaPassThru.GetNextDevice     =   GetNextDevice;
    AtaPassThruInterface->EfiAtaPassThru.BuildDevicePath   =   BuildDevicePath;
    AtaPassThruInterface->EfiAtaPassThru.GetDevice         =   GetDevice;
    AtaPassThruInterface->EfiAtaPassThru.ResetPort         =   ResetPort;
    AtaPassThruInterface->EfiAtaPassThru.ResetDevice       =   ResetDevice;

    AtaPassThruInterface->ModeFlag                         =   ModeFlag;
    AtaPassThruInterface->ControllerHandle                 =   Controller;

    // Set the attributes accordingly
    Mode->Attributes    = EFI_ATA_PASS_THRU_ATTRIBUTES_PHYSICAL | EFI_ATA_PASS_THRU_ATTRIBUTES_LOGICAL;
    Mode->IoAlign       = 2;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if(ModeFlag){
        // Get list of handles on which AMI_AHCI_BUS_PROTOCOL is installed and number of drives connected
        Status = GetSataDevCountandAddressBuffer((EFI_ATA_PASS_THRU_PROTOCOL *)AtaPassThruInterface,&pSataDevInterface,&DevCount);
        if (EFI_ERROR(Status) || !DevCount) {
            AtaPassThruInterface->AtaDeviceBuffer   = NULL;
            AtaPassThruInterface->AtaDeviceCount    = 0;
        }else{
            AtaPassThruInterface->AtaDeviceBuffer   = (VOID *)pSataDevInterface;
            AtaPassThruInterface->AtaDeviceCount    = DevCount;
            
            // Initialize PrevPortNum and PrevPortMultiplierPortNum for AtaPassThru to 0xFFFF and 0 respectively
            ((SATA_DEVICE_INTERFACE**)pSataDevInterface)[0]->AhciBusInterface->PrevPortNum = 0xffff;
            ((SATA_DEVICE_INTERFACE**)pSataDevInterface)[0]->AhciBusInterface->PrevPortMultiplierPortNum = 0;   
        }
    }
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    if(!ModeFlag){
        // Get list of handles on which AMI_IDE_BUS_INIT_PROTOCOL is installed and number of drives connected
        Status = GetIdeDevCountandAddressBuffer((EFI_ATA_PASS_THRU_PROTOCOL *)AtaPassThruInterface, &pIdeBusInterface,&DevCount);
        if (EFI_ERROR(Status)) {
            AtaPassThruInterface->AtaDeviceBuffer   = NULL;
            AtaPassThruInterface->AtaDeviceCount    = 0;
        }else{
            AtaPassThruInterface->AtaDeviceBuffer   = (VOID *)pIdeBusInterface;
            AtaPassThruInterface->AtaDeviceCount    = DevCount;

            // Initialize PrevPortNum and PrevPortMultiplierPortNum for AtaPassThru to 0xFFFF and 0 respectively
            ((AMI_IDE_BUS_PROTOCOL**)pIdeBusInterface)[0]->IdeBusInitInterface->PrevPortNum = 0xffff;
            ((AMI_IDE_BUS_PROTOCOL**)pIdeBusInterface)[0]->IdeBusInitInterface->PrevPortMultiplierPortNum = 0;
        }
    }
#endif
    Status = pBS->InstallProtocolInterface(
                &DeviceHandle,
                &gEfiAtaPassThruProtocolGuid,
                EFI_NATIVE_INTERFACE,
                (ATA_PASS_THRU_PROTOCOL*)&(AtaPassThruInterface->EfiAtaPassThru)
                );

    return Status;

}

/**
    Uninstall the AtaPassThru protocol installed on controller handle.

    @param    Controller    Controller Handle
    @param    ModeFlag      1 - AHCI interface, 0 - IDE Interface

    @retval   EFI_SUCCESS    EfiAtaPassThruProtocol interface was removed.
    @retval   EFI_INVALID_PARAMETER EfiAtaPassThruProtocolwas not previously installed on the Handle.

**/
EFI_STATUS
EFIAPI
StopAtaPassThruSupport (
    IN  EFI_HANDLE          Controller,
    IN  BOOLEAN             ModeFlag
)
{
    EFI_STATUS              Status;
    EFI_HANDLE              DeviceHandle;

    DeviceHandle = (EFI_HANDLE)Controller;

    //  Uninstall AtaPassThru Protocol for the controller.
    Status = pBS->UninstallMultipleProtocolInterfaces(
        DeviceHandle,
        &gEfiAtaPassThruProtocolGuid,
        (ATA_PASS_THRU_PROTOCOL*)&(AtaPassThruInterface->EfiAtaPassThru),
        NULL
        );

    if(Status == EFI_SUCCESS ) {
        pBS->FreePool(AtaPassThruInterface->AtaDeviceBuffer);
        pBS->FreePool(AtaPassThruInterface);
        pBS->FreePool(Mode);
    }

    return Status;
}

/**@internal
    Search device path by specific Type and SubType

    @param    DevicePath     Pointer to hold device path,
    @param    Type           Type of device path,
    @param    SubType        SubType of device path

    @retval   EFI_DEVICE_PATH_PROTOCOL - Device path found and the pointer of device path returned
    @retval   NULL  - Specific device path not found
    
    @endinternal
**/

EFI_DEVICE_PATH_PROTOCOL
*SearchDevicePath (
  IN    OUT     EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN    UINT8   Type,
  IN    UINT8   SubType
)
{
    if (DevicePath == NULL) {
        return NULL;
    }

    while (!IsDevicePathEnd (DevicePath)) {
        if ((DevicePathType (DevicePath) == Type) && (DevicePathSubType (DevicePath) == SubType)) {
            return DevicePath;
        }
        DevicePath = NextDevicePathNode (DevicePath);
    }

    return NULL;
}

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
/**@internal
    Returns the Sata device interface for the device, which matches the input Port and 
    PMPort number.

    @param    This     Pointer to AtaPassThruProtocol interface,
    @param    Port     Port specifies the port number of the ATA device for which a
                       device path node is to be allocated and built
    @param    PortMultiplierPort  The port multiplier port number of the ATA device for which a
                                  device path node is to be allocated and built. If there is no
                                  port multiplier, then specify 0xFFFF.

    @retval   SATA_DEVICE_INTERFACE    Returns the Sata device interface.
    
    @endinternal
**/

SATA_DEVICE_INTERFACE
*GetSataDevInterfaceThruPortPMPort (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    IN  UINT16                      Port,
    IN  UINT16                      PortMultiplierPort
)
{
    SATA_DEVICE_INTERFACE   *SataDevInterface = NULL;
    SATA_DEVICE_INTERFACE   *pSataDevInterface = NULL;
    UINT16                  DevCount = 0;
    UINTN                   Index = 0;

    pSataDevInterface = (SATA_DEVICE_INTERFACE *)((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceBuffer;
    if(pSataDevInterface == NULL) return NULL;

    DevCount = ((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceCount;

    // Returns the Device Interface of Corresponding Port/PM
    for(Index = 0; Index < DevCount; Index++){
        SataDevInterface = ((SATA_DEVICE_INTERFACE **)pSataDevInterface)[Index];

        if((UINT8)Port == SataDevInterface->PortNumber) {
            if((UINT8)PortMultiplierPort == SataDevInterface->PMPortNumber) {
                return SataDevInterface;
            }
        }
    }

    return NULL;
}

/**@internal
    Returns  Total drives present and their respective device addresses

    @param    This            Pointer to AtaPassThruProtocol interface                    
    @param    SataAddress     Buffer Pointer to hold device interface address
    @param    NumberOfDevices Returns total drives present

    @retval EFI_SUCCESS           Successfully returned total drives present and its
                                  respective addresses.
    @retval EFI_NOT_FOUND         No device found
    @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated.
    @retval EFI_INVALID_PARAMETER Buffer is NULL.

    @endinternal
**/

EFI_STATUS
GetSataDevCountandAddressBuffer (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    OUT SATA_DEVICE_INTERFACE       **SataAddressBuffer,
    OUT UINT16                      *NumberOfDevices
)
{
    EFI_STATUS              Status  = 0;
    UINT16                  DevCount = 0;
    SATA_DEVICE_INTERFACE   *Devices = NULL;
    EFI_HANDLE              *ControllerHandle = ((ATA_PASS_THRU_PROTOCOL*)This)->ControllerHandle;

    DevCount = CountAhciDrives(ControllerHandle, NULL);
    if(DevCount == 0xFFFF){
        return EFI_NOT_FOUND;
    }
    // For the 1st time CountAhciDrives was called to get the number of connected
    // drives; For the 2nd time it will actually return the device information
    if (DevCount == 0)  {
        *SataAddressBuffer = NULL;
        *NumberOfDevices = 0;
        return EFI_SUCCESS;
    }

    Status = pBS->AllocatePool(EfiBootServicesData, sizeof(SATA_DEVICE_INTERFACE*) * DevCount, (VOID **)&Devices);
    if (EFI_ERROR (Status)) {
        return Status;
    }

    *SataAddressBuffer = Devices;
    CountAhciDrives(ControllerHandle, (SATA_DEVICE_INTERFACE**)Devices);
    *NumberOfDevices = DevCount;

    return EFI_SUCCESS;
}

/**@internal
    This routine returns the number of connected AHCI drives as well
    as it fills the optional SATA_DEVICE_INTERFACE* buffer with the
    device information

    @param HandleBuffer  Controller handle
    @param Devices       Pointer to SataDeviceInterface

    @retval DevCount   - Connected AHCI drives count.
    
    @endinternal
**/

UINT16
CountAhciDrives (
    IN  EFI_HANDLE                  *HandleBuffer,
    IN  OUT SATA_DEVICE_INTERFACE   **Devices
){
    EFI_STATUS              Status;
    UINT16                  DevCount = 0;
    AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface;
    SATA_DEVICE_INTERFACE   *SataDevInterface;
    DLINK                   *dlink;

    // With the Controller handle, find the total devices present and their Address
    Status = pBS->HandleProtocol(HandleBuffer, &gAmiAhciBusProtocolGuid, (VOID **)&AhciBusInterface);
    if(EFI_ERROR(Status)){
        return MAX_UINT16;
    }

    for (dlink = AhciBusInterface->SataDeviceList.pHead; dlink; dlink = dlink->pNext) {
        SataDevInterface = OUTTER(dlink, SataDeviceLink, SATA_DEVICE_INTERFACE);

#if ATAPI_COMMANDS_SUPPORT_IN_ATAPASSTHRU == 1
        // Skip adding Devices if DeviceType is PMPORT
        if (SataDevInterface->DeviceState == DEVICE_CONFIGURED_SUCCESSFULLY &&  SataDevInterface->DeviceType != PMPORT) {
#else
        if (SataDevInterface->DeviceState == DEVICE_CONFIGURED_SUCCESSFULLY &&  SataDevInterface->DeviceType == ATA) {
#endif
            if (Devices != NULL) {
                Devices[DevCount] = SataDevInterface;
            }
            DevCount++;
        }
    }

    return DevCount;
}
#endif

#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
/**@internal
    Checks PM Port

    @param    This     Pointer to AtaPassThruProtocol interface,
    @param    Port     Port specifies the port number of the ATA device for which a
                       device path node is to be allocated and built
    @param    PortMultiplierPort  The port multiplier port number of the ATA device for which a
                                  device path node is to be allocated and built. If there is no
                                  port multiplier, then specify 0xFFFF.

    @retval AMI_IDE_BUS_PROTOCOL
    
    @internal
**/

AMI_IDE_BUS_PROTOCOL
*GetIdeBusInterfaceThruPortPMPort (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    IN  UINT16                      Port,
    IN  UINT16                      PortMultiplierPort
  )
{
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = NULL;
    AMI_IDE_BUS_PROTOCOL    *pIdeBusInterface = NULL;
    UINT16                  DevCount = 0;
    UINTN                   Index = 0;

    pIdeBusInterface = (AMI_IDE_BUS_PROTOCOL *)((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceBuffer;
    if(pIdeBusInterface == NULL) return NULL;

    DevCount = ((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceCount;

    // Run through the total number of drives present
    for(Index = 0; Index < DevCount; Index++){
        IdeBusInterface = ((AMI_IDE_BUS_PROTOCOL **)pIdeBusInterface)[Index];
         if((UINT8)Port == IdeBusInterface->IdeDevice.Channel) {
            if((UINT8)PortMultiplierPort == IdeBusInterface->IdeDevice.Device) {
                return IdeBusInterface;
            }
        }
    }

    return NULL;
}

/**@internal
    Returns  Total drives present and their respective device addresses.

    @param    This            Pointer to AtaPassThruProtocol interface                    
    @param    SataAddress     Buffer Pointer to hold device interface address for Ide mode.
    @param    NumberOfDevices Returns total drives present

    @retval EFI_SUCCESS           Successfully returned total drives present and thier 
                                  respective addresses.
    @retval EFI_NOT_FOUND         No device found
    @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated.
    @retval EFI_INVALID_PARAMETER Buffer is NULL.
    
    @endinternal
**/

EFI_STATUS
GetIdeDevCountandAddressBuffer (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    OUT AMI_IDE_BUS_PROTOCOL        **IdeAddressBuffer,
    OUT UINT16                      *NumberOfDevices
)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    UINT16                  DevCount = 0;
    AMI_IDE_BUS_PROTOCOL    *Devices = NULL;
    EFI_HANDLE              *ControllerHandle = ((ATA_PASS_THRU_PROTOCOL*)This)->ControllerHandle;

    DevCount = CountIdeDrives(ControllerHandle, NULL);
    if(DevCount == 0xFFFF){
        return EFI_NOT_FOUND;
    }
    // For the 1st time CountIdeDrives was called to get the number of connected
    // drives; for the 2nd time it will actually return the device information
    if (DevCount == 0)  {
        *IdeAddressBuffer = NULL;
        *NumberOfDevices = 0;
        return EFI_SUCCESS;
    }

    Status = pBS->AllocatePool(EfiBootServicesData, sizeof(AMI_IDE_BUS_PROTOCOL*) * DevCount, (VOID **)&Devices);
    if (EFI_ERROR (Status)) {
        return Status;
    }

    *IdeAddressBuffer = Devices;
    CountIdeDrives(ControllerHandle, (AMI_IDE_BUS_PROTOCOL**)Devices);
    *NumberOfDevices = DevCount;

    return EFI_SUCCESS;
}

/**@internal
    This routine returns the number of connected AHCI drives as well
    as it fills the optional AMI_IDE_BUS_PROTOCOL* buffer with the
    device information

    @param HandleBuffer  Controller handle
    @param Devices       Pointer to SataDeviceInterface

    @retval DevCount   - Connected IDE drives count.
    
    @endinternal
**/

UINT16
CountIdeDrives (
    IN  EFI_HANDLE              *HandleBuffer,
    IN  AMI_IDE_BUS_PROTOCOL    **Devices
){
    EFI_STATUS                  Status;
    UINT8                       PrimarySecondary;
    UINT8                       MasterSlave;
    UINT16                      DevCount = 0;
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface = NULL;
    AMI_IDE_BUS_INIT_PROTOCOL   *IdeBusInitProtocol;

    // With the Controller handle, find the total devices present and their Address
    Status = pBS->HandleProtocol(HandleBuffer,&gAmiIdeBusInitProtocolGuid,(VOID **)&IdeBusInitProtocol);
    if(EFI_ERROR(Status)){
        return MAX_UINT16;
    }
    for (PrimarySecondary = 0;PrimarySecondary < 2;PrimarySecondary++){
        for (MasterSlave = 0;MasterSlave < 2;MasterSlave++) {
            if (IdeBusInitProtocol->IdeBusInitData[PrimarySecondary][MasterSlave] == DEVICE_CONFIGURED_SUCCESSFULLY) {

                IdeBusInterface = IdeBusInitProtocol->pIdeBusProtocol[PrimarySecondary][MasterSlave];
#if ATAPI_COMMANDS_SUPPORT_IN_ATAPASSTHRU == 0
                if (IdeBusInterface->IdeDevice.DeviceType == ATAPI) continue;
#endif
                if (Devices != NULL) {
                    Devices[DevCount] = IdeBusInterface;
                }

                DevCount++;
            }
        }
    }

    return DevCount;
}

/**@internal
    Reads 1 Byte of data from the IO port

    @param  PciIO         Pointer to PciIoProtocol interface
    @param  Register      Io port address to read data.
    @param  Data8         Pointer to hold the data.

    @retval Returns the byte of data read from IO port.
    
    @endinternal
**/

EFI_STATUS
IdeReadByte(
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    OUT UINT8                   *Data8
) {
    PciIO->Io.Read(
        PciIO,
        EfiPciIoWidthFifoUint8,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        1,
        Data8 );

    return EFI_SUCCESS;
}

/**@internal
    Get the ATA Status Block

    @param  IdeBusInterface   Pointer to IdeBusProtocol interface.
    @param  Asb               Pointer to hold Status block data of Ide devices.

    @retval VOID
    
    @endinternal
**/

VOID
GetIdeAsbStatus (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface,
    IN  OUT EFI_ATA_STATUS_BLOCK    *Asb
  )
{
    IO_REGS     Regs = IdeBusInterface->IdeDevice.Regs;

    ZeroMemory (Asb, sizeof (EFI_ATA_STATUS_BLOCK));

    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.StatusReg,       &Asb->AtaStatus);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.ErrorReg,        &Asb->AtaError);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.SectorCountReg,  &Asb->AtaSectorCount);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.SectorCountReg,  &Asb->AtaSectorCountExp);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBALowReg,       &Asb->AtaSectorNumber);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBALowReg,       &Asb->AtaSectorNumberExp);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,       &Asb->AtaCylinderLow);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,       &Asb->AtaCylinderLowExp);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg,      &Asb->AtaCylinderHigh);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg,      &Asb->AtaCylinderHighExp);
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.DeviceReg,       &Asb->AtaDeviceHead);

    return;
}
#endif

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
/**@internal
    Get the ATA Status Block in AHCI

    @param  SataDevInterface   Pointer to SataDevInterface
    @param  Asb                Pointer to hold Status block data of AHCI devices.

    @retval EFI_STATUS
    
    @endinternal
**/

VOID
GetAhciAsbStatus (
  IN    SATA_DEVICE_INTERFACE       *SataDevInterface,
  IN    OUT EFI_ATA_STATUS_BLOCK    *Asb
) {

    UINT32  Data = 0;
    volatile    AHCI_RECEIVED_FIS *FISReceiveAddress;

    UINT64    AhciBaseAddr = SataDevInterface->AhciBusInterface->AhciBaseAddress;

    Data = HBA_PORT_REG32 (AhciBaseAddr, SataDevInterface->PortNumber, HBA_PORTS_TFD);
    if (Asb != NULL) {
        ZeroMemory (Asb, sizeof (EFI_ATA_STATUS_BLOCK));

        Asb->AtaStatus  = (UINT8)Data;
        if ((Asb->AtaStatus & BIT0) != 0) {
            Asb->AtaError = (UINT8)(Data >> 8);
        }

        FISReceiveAddress = (AHCI_RECEIVED_FIS*)SataDevInterface->PortFISBaseAddr;

        // Get the pointer to RFIS
        if (FISReceiveAddress->Ahci_Rfis[0] == FIS_REGISTER_D2H) {
            Asb->AtaStatus          = FISReceiveAddress->Ahci_Rfis[2];
            Asb->AtaError           = FISReceiveAddress->Ahci_Rfis[3];
            Asb->AtaSectorCount     = FISReceiveAddress->Ahci_Rfis[12];
            Asb->AtaSectorCountExp  = FISReceiveAddress->Ahci_Rfis[13];
            Asb->AtaSectorNumber    = FISReceiveAddress->Ahci_Rfis[4];
            Asb->AtaSectorNumberExp = FISReceiveAddress->Ahci_Rfis[8];
            Asb->AtaCylinderLow     = FISReceiveAddress->Ahci_Rfis[5];
            Asb->AtaCylinderLowExp  = FISReceiveAddress->Ahci_Rfis[9];
            Asb->AtaCylinderHigh    = FISReceiveAddress->Ahci_Rfis[6];
            Asb->AtaCylinderHighExp = FISReceiveAddress->Ahci_Rfis[10];
            Asb->AtaDeviceHead      = FISReceiveAddress->Ahci_Rfis[7];
            return;
        }

        // Get the pointer to PFIS
        if (FISReceiveAddress->Ahci_Psfis[0] == FIS_PIO_SETUP) {
            Asb->AtaStatus          = FISReceiveAddress->Ahci_Psfis[2];
            Asb->AtaError           = FISReceiveAddress->Ahci_Psfis[3];
            Asb->AtaSectorCount     = FISReceiveAddress->Ahci_Psfis[12];
            Asb->AtaSectorCountExp  = FISReceiveAddress->Ahci_Psfis[13];
            Asb->AtaSectorNumber    = FISReceiveAddress->Ahci_Psfis[4];
            Asb->AtaSectorNumberExp = FISReceiveAddress->Ahci_Psfis[8];
            Asb->AtaCylinderLow     = FISReceiveAddress->Ahci_Psfis[05];
            Asb->AtaCylinderLowExp  = FISReceiveAddress->Ahci_Psfis[9];
            Asb->AtaCylinderHigh    = FISReceiveAddress->Ahci_Psfis[6];
            Asb->AtaCylinderHighExp = FISReceiveAddress->Ahci_Psfis[10];
            Asb->AtaDeviceHead      = FISReceiveAddress->Ahci_Psfis[7];
            return;
        }
    }
    return;
}
#endif

/**
    Sends an ATA command to an ATA device that is attached
    to the ATA controller

  @param This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance. 
  @param Port                The port number of the ATA device to send the command. 
  @param PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                             If there is no port multiplier, then specify 0xFFFF.
  @param Packet              A pointer to the ATA command to send to the ATA device specified by Port
                             and PortMultiplierPort.
  @param Event               If non-blocking I/O is not supported then Event is ignored, and blocking
                             I/O is performed. If Event is NULL, then blocking I/O is performed. If
                             Event is not NULL and non blocking I/O is supported, then non-blocking
                             I/O is performed, and Event will be signaled when the ATA command completes.

  @retval EFI_SUCCESS                The ATA command was sent by the host. For bi-directional commands, 
                                     InTransferLength bytes were transferred from InDataBuffer. For write and
                                     bi-directional commands, OutTransferLength bytes were transferred by OutDataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The ATA command was not executed. The number of bytes that could be transferred
                                     is returned in InTransferLength. For write and bi-directional commands, 
                                     OutTransferLength bytes were transferred by OutDataBuffer.
  @retval EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                     already queued. The caller may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the ATA command.
  @retval EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                     command was not sent, so no additional status information is available.

**/

EFI_STATUS
EFIAPI
PassThru (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    IN  UINT16                      Port,
    IN  UINT16                      PortMultiplierPort,
    IN  OUT                         EFI_ATA_PASS_THRU_COMMAND_PACKET *Packet,
    IN  EFI_EVENT                   Event OPTIONAL
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE           *SataDevInterface = NULL;
    AMI_AHCI_BUS_PROTOCOL           *AhciBusInterface = NULL;
    COMMAND_STRUCTURE               CommandStructure;
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    AMI_IDE_BUS_PROTOCOL            *IdeBusInterface = NULL;
#endif
    EFI_ATA_PASS_THRU_PROTOCOL      *PassThruProtocol = This;
    UINT32                          AlignmentBoundry;
    UINT32                          MaxSectorCount;
    BOOLEAN                         ModeFlag = ((ATA_PASS_THRU_PROTOCOL*)This)->ModeFlag;
    UINT16                          IdentifyDataWord83 = 0;
    #if (( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) ) || ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) ))
    UINT64                          IdentifyData_LBA_48=0;
    #endif
    UINT32                          SectorSize = ATA_SECTOR_BYTES;

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    ZeroMemory (&CommandStructure, sizeof(COMMAND_STRUCTURE));
#endif

    // Check InDataBuffer,OutDataBuffer are properly aligned or  not
    if(PassThruProtocol->Mode->IoAlign>1){
        // Should be align in 2's power
        AlignmentBoundry = PassThruProtocol->Mode->IoAlign;
      
        if(Packet->OutDataBuffer != NULL) {
            if(0 != ((UINTN)(Packet->OutDataBuffer) % AlignmentBoundry)){
                return EFI_INVALID_PARAMETER;
            }
        }
        if(Packet->InDataBuffer != NULL) {
            if(0 != ((UINTN)(Packet->InDataBuffer) % AlignmentBoundry)){
                return EFI_INVALID_PARAMETER;         
            }
        }
        if(Packet->Asb != NULL) {
            if(0 != ((UINTN)(Packet->Asb) % AlignmentBoundry)){
                return EFI_INVALID_PARAMETER;        
            }
        }
    }

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if(ModeFlag){
        //If Port or PortMultiplierPort are not in a valid range for the ATA
        //controller, then EFI_INVALID_PARAMETER is returned
        SataDevInterface = GetSataDevInterfaceThruPortPMPort(This,Port,PortMultiplierPort);
        if( NULL == SataDevInterface){
            return EFI_INVALID_PARAMETER;
        }
        AhciBusInterface=SataDevInterface->AhciBusInterface;

        if( NULL == AhciBusInterface){
            return EFI_INVALID_PARAMETER;
        }

        IdentifyDataWord83 = SataDevInterface->IdentifyData.Command_Set_Supported_83;
        IdentifyData_LBA_48 = SataDevInterface->IdentifyData.LBA_48;

        if((SataDevInterface->IdentifyData.Reserved_104_126[2] & BIT14) &&       // WORD 106 valid? - BIT 14 - 1
            (!(SataDevInterface->IdentifyData.Reserved_104_126[2] & BIT15)) &&   // WORD 106 valid? - BIT 15 - 0
            (SataDevInterface->IdentifyData.Reserved_104_126[2] & BIT12)) {      // WORD 106 bit 12 - SectorSize > 256 words
            // The sector size is in words 117-118.
            SectorSize = (UINT32)(SataDevInterface->IdentifyData.Reserved_104_126[13] +
                                  (SataDevInterface->IdentifyData.Reserved_104_126[14] << 16)) * 2;
        }
    }
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    if(!ModeFlag){

        IdeBusInterface = GetIdeBusInterfaceThruPortPMPort(This,Port,PortMultiplierPort);
        if(NULL == IdeBusInterface){
            return  EFI_INVALID_PARAMETER;
        }

        IdentifyDataWord83 = IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83;
        IdentifyData_LBA_48 = IdeBusInterface->IdeDevice.IdentifyData.LBA_48;

        if((IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT14) &&     // WORD 106 valid? - BIT 14 - 1
           (!(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT15)) &&  // WORD 106 valid? - BIT 15 - 0
           (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT12)) {     // WORD 106 bit 12 - SectorSize > 256 words
            // The sector size is in words 117-118.
            SectorSize = (UINT32)(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[13] +
                                  (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[14] << 16)) * 2;
        }
    }
#endif

    // Transfer Length from sector count to byte.
    if (((Packet->Length & EFI_ATA_PASS_THRU_LENGTH_BYTES) == 0) &&
      (Packet->InTransferLength != 0)) {
        Packet->InTransferLength = Packet->InTransferLength * SectorSize;
    }

    // Transfer Length from sector count to byte.
    if (((Packet->Length & EFI_ATA_PASS_THRU_LENGTH_BYTES) == 0) &&
     (Packet->OutTransferLength != 0)) {
        Packet->OutTransferLength = Packet->OutTransferLength * SectorSize;
    }

    // If, word83: bit15 is zero and bit14 is one and bit10 is one, then ATA device support 48-bit addressing.
    MaxSectorCount = 0x100;
    if ((IdentifyDataWord83 & (BIT10 | BIT15 | BIT14)) == 0x4400) {
        if (IdentifyData_LBA_48 > 0xFFFFFFF) {
            // Capacity exceeds 120GB. 48-bit addressing is really needed
            // In this case, the max sector count is 0x10000
            MaxSectorCount = 0x10000;
        }
    }

    // If the data buffer described by InDataBuffer and InTransferLength is too big to be transferred in a single command,
    // then no data is transferred and EFI_BAD_BUFFER_SIZE is returned. The number of bytes that can be transferred in a
    // single command are returned in InTransferLength. If the data buffer described by OutDataBuffer and OutTransferLength
    // is too big to be transferred in a single command, then no data is transferred and EFI_BAD_BUFFER_SIZE is returned.
    if (((Packet->InTransferLength != 0) && (Packet->InTransferLength > MaxSectorCount * SectorSize)) ||
        ((Packet->OutTransferLength != 0) && (Packet->OutTransferLength > MaxSectorCount * SectorSize))) {
        Packet->InTransferLength = MaxSectorCount * SectorSize;
        return EFI_BAD_BUFFER_SIZE;
    }

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    //  Frame Command Buffer
    CommandStructure.SectorCount    = (UINT16)Packet->Acb->AtaSectorCount;
    CommandStructure.LBALow         = Packet->Acb->AtaSectorNumber;
    CommandStructure.LBAMid         = Packet->Acb->AtaCylinderLow;
    CommandStructure.LBAHigh        = Packet->Acb->AtaCylinderHigh;
    CommandStructure.Command        = Packet->Acb->AtaCommand;
    CommandStructure.Features       = Packet->Acb->AtaFeatures;
	
	if(Packet->Timeout == 0) {
	    CommandStructure.Timeout        = MAX_UINT64; // Setting max value of Timeout 
	} else {
	    if(Packet->Timeout < 10000) {
	        CommandStructure.Timeout = 1;
	    } else {
			CommandStructure.Timeout        = DivU64x32(Packet->Timeout, 10000); // Converting to milli Seconds
	    }	
	}


    // By default initialize with In data. Based on the PassThru protocol
    // these fields will be initialized again.
    CommandStructure.Buffer         = Packet->InDataBuffer;
    CommandStructure.ByteCount      = Packet->InTransferLength;

    if (Check48BitCommand (Packet->Acb->AtaCommand)) {
        CommandStructure.LBALowExp = Packet->Acb->AtaSectorNumberExp;
        CommandStructure.LBAMidExp = Packet->Acb->AtaCylinderLowExp;
        CommandStructure.LBAHighExp = Packet->Acb->AtaCylinderHighExp;
        CommandStructure.Device = 0x40;
    } else {
        CommandStructure.Device = ((UINT8) (Packet->Acb->AtaSectorNumberExp & 0x0f) | 0x40);
    }

    if(ModeFlag) {
        switch(Packet->Protocol){

            case  EFI_ATA_PASS_THRU_PROTOCOL_ATA_HARDWARE_RESET:
                    Status = AhciBusInterface->GeneratePortReset ( AhciBusInterface,
                                                                SataDevInterface,
                                                                (UINT8)Port,
                                                                (UINT8)PortMultiplierPort,
                                                                0,
                                                                0
                                                                );

                    break;

            case  EFI_ATA_PASS_THRU_PROTOCOL_ATA_SOFTWARE_RESET:
                    Status = AhciBusInterface->GeneratePortSoftReset ( SataDevInterface,
                                                                    (UINT8)PortMultiplierPort
                                                                    );

                    break;

            case  EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA:

                    Status = AhciBusInterface->ExecuteNonDataCommand (  SataDevInterface,
                                                                        CommandStructure
                                                                        );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN:
                
                   
                    Status = AhciBusInterface->SataPioDataOut(  SataDevInterface,
                                                                   CommandStructure,
                                                                   FALSE
                                                                );
                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT:
                    CommandStructure.Buffer         = Packet->OutDataBuffer;
                    CommandStructure.ByteCount      = Packet->OutTransferLength;
                    
                    Status = AhciBusInterface->SataPioDataOut(  SataDevInterface,
                                                                CommandStructure,
                                                                TRUE
                                                                );
                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_UDMA_DATA_IN:

                    Status = AhciBusInterface->ExecuteDmaDataCommand ( SataDevInterface,
                                                                    &CommandStructure,
                                                                    FALSE
                                                                    );
                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_UDMA_DATA_OUT:

                    // Initialize the Out Command Data
                    CommandStructure.Buffer         = Packet->OutDataBuffer;
                    CommandStructure.ByteCount      = Packet->OutTransferLength;

                    Status = AhciBusInterface->ExecuteDmaDataCommand ( SataDevInterface,
                                                                    &CommandStructure,
                                                                    TRUE
                                                                    );
                    break;
            case EFI_ATA_PASS_THRU_PROTOCOL_DMA:

                    if(Packet->InTransferLength == 0) {
                        // Initialize the Out Command Data
                        CommandStructure.Buffer         = Packet->OutDataBuffer;
                        CommandStructure.ByteCount      = Packet->OutTransferLength;
                    }

                    //For all write and non data commands where InTransferLength is 0
                    Status = AhciBusInterface->ExecuteDmaDataCommand ( SataDevInterface,
                                                                    &CommandStructure,
                                                                    Packet->InTransferLength == 0 ? TRUE : FALSE
                                                                    );
                    break;
            case EFI_ATA_PASS_THRU_PROTOCOL_DEVICE_DIAGNOSTIC:

                    // Send Device Diagnostic command.
                    ZeroMemory (&CommandStructure, sizeof(COMMAND_STRUCTURE));
                    CommandStructure.Command = EXECUTE_DEVICE_DIAGNOSTIC;
                    Status = AhciBusInterface->ExecuteNonDataCommand (  SataDevInterface,
                                                                        CommandStructure
                                                                        );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_DEVICE_RESET:

                    // Device Reset is not supported for ATA devices.
                    if(SataDevInterface->DeviceType == ATA) {
                        return EFI_UNSUPPORTED;
                    }

                    // Send Device Reset command.
                    ZeroMemory (&CommandStructure, sizeof(COMMAND_STRUCTURE));
                    CommandStructure.Command = DEVICE_RESET;
                    Status = AhciBusInterface->ExecuteNonDataCommand (  SataDevInterface,
                                                                        CommandStructure
                                                                        );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_RETURN_RESPONSE:
                    GetAhciAsbStatus(SataDevInterface, Packet->Asb);
                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_DMA_QUEUED:
            case EFI_ATA_PASS_THRU_PROTOCOL_FPDMA:

            default:
                   return EFI_UNSUPPORTED;
        }
        
        GetAhciAsbStatus(SataDevInterface, Packet->Asb);
        CommandStructure.Timeout = 0;

    }
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    if(!ModeFlag){
        
        Status = pBS->LocateProtocol(
                                    &gAmiPlatformIdeProtocolGuid,
                                    NULL,
                                    (VOID**)&gPlatformIdeProtocol
                                    );
        if (EFI_ERROR (Status)) {
            // If Error Status, Set Command timeout to default sdl value.
            gPlatformIdeProtocol->CommandTimeout = 0;
        } else {
        	if(Packet->Timeout == 0) {
		        gPlatformIdeProtocol->CommandTimeout = MAX_UINT64;  // Setting max value of Timeout 
			} else {
			    
		        if(Packet->Timeout < 10000) {
		            gPlatformIdeProtocol->CommandTimeout = 1;
		        } else {
	                gPlatformIdeProtocol->CommandTimeout = DivU64x32(Packet->Timeout, 10000); // Converting to milli Seconds
		        }   
			}
		}	

        switch(Packet->Protocol){

            case EFI_ATA_PASS_THRU_PROTOCOL_ATA_HARDWARE_RESET:
                    Status = IdeBusInterface->IdeSoftReset( IdeBusInterface
                                                            );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_ATA_SOFTWARE_RESET:

                    return EFI_UNSUPPORTED;

            case  EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA:

                    Status = IdeBusInterface->IdeNonDataCommand(  IdeBusInterface,
                                                                Packet->Acb->AtaFeatures,
                                                                Packet->Acb->AtaSectorCount,
                                                                Packet->Acb->AtaSectorCountExp,
                                                                Packet->Acb->AtaSectorNumber,
                                                                Packet->Acb->AtaSectorNumberExp,
                                                                Packet->Acb->AtaCylinderLow,
                                                                Packet->Acb->AtaCylinderLowExp,
                                                                Packet->Acb->AtaCylinderHigh,
                                                                Packet->Acb->AtaCylinderHighExp,
                                                                Packet->Acb->AtaDeviceHead,
                                                                Packet->Acb->AtaCommand
                                                             );
                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN:

                    Status = IdeBusInterface->AtaPioDataOut ( IdeBusInterface,
                                                            Packet->InDataBuffer,
                                                            Packet->InTransferLength,
                                                            Packet->Acb->AtaFeatures,
                                                            (UINT8)Packet->Acb->AtaSectorCount,
                                                            Packet->Acb->AtaSectorNumber,
                                                            Packet->Acb->AtaSectorNumberExp,
                                                            Packet->Acb->AtaCylinderLow,
                                                            Packet->Acb->AtaCylinderLowExp,
                                                            Packet->Acb->AtaCylinderHigh,
                                                            Packet->Acb->AtaCylinderHighExp,
                                                            Packet->Acb->AtaDeviceHead,
                                                            Packet->Acb->AtaCommand,
                                                            FALSE,
                                                            FALSE
                                                          );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT:

                    Status = IdeBusInterface->AtaPioDataOut ( IdeBusInterface,
                                                            Packet->OutDataBuffer,
                                                            Packet->OutTransferLength,
                                                            Packet->Acb->AtaFeatures,
                                                            (UINT8)Packet->Acb->AtaSectorCount,
                                                            Packet->Acb->AtaSectorNumber,
                                                            Packet->Acb->AtaSectorNumberExp,
                                                            Packet->Acb->AtaCylinderLow,
                                                            Packet->Acb->AtaCylinderLowExp,
                                                            Packet->Acb->AtaCylinderHigh,
                                                            Packet->Acb->AtaCylinderHighExp,
                                                            Packet->Acb->AtaDeviceHead,
                                                            Packet->Acb->AtaCommand,
                                                            TRUE,
                                                            FALSE
                                                          );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_UDMA_DATA_IN:

                    Status = IdeBusInterface->AtaAtapiDmaDataCommand ( IdeBusInterface,
                                                            Packet->InDataBuffer,
                                                            Packet->InTransferLength,
                                                            Packet->Acb->AtaFeatures,
                                                            (UINT32)Packet->Acb->AtaSectorCount,
                                                            Packet->Acb->AtaSectorNumber,
                                                            Packet->Acb->AtaSectorNumberExp,
                                                            Packet->Acb->AtaCylinderLow,
                                                            Packet->Acb->AtaCylinderLowExp,
                                                            Packet->Acb->AtaCylinderHigh,
                                                            Packet->Acb->AtaCylinderHighExp,
                                                            Packet->Acb->AtaDeviceHead,
                                                            Packet->Acb->AtaCommand,
                                                            FALSE
                                                          );
                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_UDMA_DATA_OUT:

                    Status = IdeBusInterface->AtaAtapiDmaDataCommand ( IdeBusInterface,
                                                            Packet->OutDataBuffer,
                                                            Packet->OutTransferLength,
                                                            Packet->Acb->AtaFeatures,
                                                            (UINT32)Packet->Acb->AtaSectorCount,
                                                            Packet->Acb->AtaSectorNumber,
                                                            Packet->Acb->AtaSectorNumberExp,
                                                            Packet->Acb->AtaCylinderLow,
                                                            Packet->Acb->AtaCylinderLowExp,
                                                            Packet->Acb->AtaCylinderHigh,
                                                            Packet->Acb->AtaCylinderHighExp,
                                                            Packet->Acb->AtaDeviceHead,
                                                            Packet->Acb->AtaCommand,
                                                            TRUE
                                                          );
                    break;


            case EFI_ATA_PASS_THRU_PROTOCOL_DMA:
                    if(Packet->InTransferLength == 0) {

                        Status = IdeBusInterface->AtaAtapiDmaDataCommand ( IdeBusInterface,
                                                            Packet->OutDataBuffer,
                                                            Packet->OutTransferLength,
                                                            Packet->Acb->AtaFeatures,
                                                            (UINT32)Packet->Acb->AtaSectorCount,
                                                            Packet->Acb->AtaSectorNumber,
                                                            Packet->Acb->AtaSectorNumberExp,
                                                            Packet->Acb->AtaCylinderLow,
                                                            Packet->Acb->AtaCylinderLowExp,
                                                            Packet->Acb->AtaCylinderHigh,
                                                            Packet->Acb->AtaCylinderHighExp,
                                                            Packet->Acb->AtaDeviceHead,
                                                            Packet->Acb->AtaCommand,
                                                            TRUE
                                                          );


                    } else {

                        Status = IdeBusInterface->AtaAtapiDmaDataCommand ( IdeBusInterface,
                                                            Packet->InDataBuffer,
                                                            Packet->InTransferLength,
                                                            Packet->Acb->AtaFeatures,
                                                            (UINT32)Packet->Acb->AtaSectorCount,
                                                            Packet->Acb->AtaSectorNumber,
                                                            Packet->Acb->AtaSectorNumberExp,
                                                            Packet->Acb->AtaCylinderLow,
                                                            Packet->Acb->AtaCylinderLowExp,
                                                            Packet->Acb->AtaCylinderHigh,
                                                            Packet->Acb->AtaCylinderHighExp,
                                                            Packet->Acb->AtaDeviceHead,
                                                            Packet->Acb->AtaCommand,
                                                            FALSE
                                                          );
                    }

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_DEVICE_DIAGNOSTIC:

                    Status = IdeBusInterface->IdeNonDataCommand(  IdeBusInterface,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                EXECUTE_DEVICE_DIAGNOSTIC
                                                             );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_DEVICE_RESET:

                    if(IdeBusInterface->IdeDevice.DeviceType == ATA) {
                        return EFI_UNSUPPORTED;
                    }

                    Status = IdeBusInterface->IdeNonDataCommand(  IdeBusInterface,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                0,
                                                                DEVICE_RESET
                                                             );

                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_RETURN_RESPONSE:
                    GetIdeAsbStatus(IdeBusInterface, Packet->Asb);
                    break;

            case EFI_ATA_PASS_THRU_PROTOCOL_DMA_QUEUED:
            case EFI_ATA_PASS_THRU_PROTOCOL_FPDMA:

            default:
                   return EFI_UNSUPPORTED;
        }

        GetIdeAsbStatus(IdeBusInterface, Packet->Asb);
        gPlatformIdeProtocol->CommandTimeout = 0;
    }
#endif
   return Status;

}

/**
  Used to retrieve the list of legal port numbers for ATA devices on an ATA controller.
  These can either be the list of ports where ATA devices are actually present or the
  list of legal port numbers for the ATA controller. Regardless, the caller of this
  function must probe the port number returned to see if an ATA device is actually
  present at that location on the ATA controller.

  The GetNextPort() function retrieves the port number on an ATA controller. If on input
  Port is 0xFFFF, then the port number of the first port on the ATA controller is returned
  in Port and EFI_SUCCESS is returned.

  If Port is a port number that was returned on a previous call to GetNextPort(), then the
  port number of the next port on the ATA controller is returned in Port, and EFI_SUCCESS
  is returned. If Port is not 0xFFFF and Port was not returned on a previous call to
  GetNextPort(), then EFI_INVALID_PARAMETER is returned.

  If Port is the port number of the last port on the ATA controller, then EFI_NOT_FOUND is
  returned.

  @param  This           A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance. 
  @param  Port           On input, a pointer to the port number on the ATA controller.
                         On output, a pointer to the next port number on the ATA
                         controller. An input value of 0xFFFF retrieves the first port
                         number on the ATA controller.

  @retval EFI_SUCCESS           The next port number on the ATA controller was returned in Port.
  @retval EFI_NOT_FOUND         There are no more ports on this ATA controller.
  @retval EFI_INVALID_PARAMETER Port is not 0xFFFF and Port was not returned on a previous call
                                to GetNextPort().

**/

EFI_STATUS
EFIAPI
GetNextPort (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    IN  OUT UINT16                  *Port
)
{
    UINT16                  DevCount = 0;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE   *SataDevices = NULL;
    SATA_DEVICE_INTERFACE   *pSataDevices = NULL;
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    AMI_IDE_BUS_PROTOCOL    *IdeDevices = NULL;
    AMI_IDE_BUS_PROTOCOL    *pIdeDevices = NULL;
#endif
    UINT8                   DevIndex = 0;
    UINT16                  PreviousPort = 0;
    BOOLEAN                 ModeFlag = ((ATA_PASS_THRU_PROTOCOL*)This)->ModeFlag;

    // Port is NULL return Invalid Parameter.
    if(Port == NULL){
        return EFI_INVALID_PARAMETER;
    }

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if(ModeFlag) {

        pSataDevices = (SATA_DEVICE_INTERFACE *)((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceBuffer;
        if(pSataDevices == NULL) return EFI_NOT_FOUND;

        DevCount = ((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceCount;

        SataDevices = ((SATA_DEVICE_INTERFACE **)pSataDevices)[DevIndex];
        if((SataDevices == NULL) || (DevCount == 0)) {
            return EFI_NOT_FOUND;
        }

        PreviousPort = SataDevices->AhciBusInterface->PrevPortNum;
        // If Port is not 0xFFFF or Previous Port Number returned on earlier call
        // return invalid parameter
        if(( 0xFFFF != *Port) && (PreviousPort != *Port)) {
            return EFI_INVALID_PARAMETER;
        }
        // If on input Port is 0xFFFF, then the port number of the first port on the
        // ATA controller is returned in Port and EFI_SUCCESS is returned.
        if( 0xFFFF == *Port){
            SataDevices->AhciBusInterface->PrevPortNum = SataDevices->PortNumber;
            *Port = SataDevices->PortNumber;
            return EFI_SUCCESS;
        }

        // If Port is a port number that was returned on a previous call to GetNextPort(), then
        // the port number of the next port on the ATA controller is returned in Port, and
        // EFI_SUCCESS is returned.
        if( PreviousPort == *Port){
            for(DevIndex = 0; DevIndex < DevCount; DevIndex++){
                SataDevices = ((SATA_DEVICE_INTERFACE **)pSataDevices)[DevIndex];
                if(SataDevices->PortNumber > *Port) {
                    SataDevices->AhciBusInterface->PrevPortNum = SataDevices->PortNumber;
                    *Port = SataDevices->PortNumber;
                    return EFI_SUCCESS;
                }
            }
        }
    }
#endif
    
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    if(!ModeFlag){

        pIdeDevices = (AMI_IDE_BUS_PROTOCOL *)((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceBuffer;
        if(pIdeDevices == NULL) return EFI_NOT_FOUND;

        DevCount = ((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceCount;

        IdeDevices = ((AMI_IDE_BUS_PROTOCOL **)pIdeDevices)[DevIndex];
        if((IdeDevices == NULL) || (DevCount == 0)) {
            return EFI_NOT_FOUND;
        }

        PreviousPort = IdeDevices->IdeBusInitInterface->PrevPortNum;
        // If Port is not 0xFFFF or Previous Port Number returned on earlier call
        // return invalid parameter
        if(( 0xFFFF != *Port) && (PreviousPort != *Port)) {
            return EFI_INVALID_PARAMETER;
        }

        // If on input Port is 0xFFFF, then the port number of the first port on the
        // ATA controller is returned in Port and EFI_SUCCESS is returned.
        if( 0xFFFF == *Port){
            IdeDevices->IdeBusInitInterface->PrevPortNum = IdeDevices->IdeDevice.Channel;
            *Port = IdeDevices->IdeDevice.Channel;
            return EFI_SUCCESS;
        }

        // If Port is a port number that was returned on a previous call to GetNextPort(), then
        // the port number of the next port on the ATA controller is returned in Port, and
        // EFI_SUCCESS is returned.
        if( PreviousPort == *Port){
            for(DevIndex = 0; DevIndex < DevCount; DevIndex++){
                IdeDevices = ((AMI_IDE_BUS_PROTOCOL **)pIdeDevices)[DevIndex];
                if(IdeDevices->IdeDevice.Channel > *Port) {
                    IdeDevices->IdeBusInitInterface->PrevPortNum = IdeDevices->IdeDevice.Channel;
                    *Port = IdeDevices->IdeDevice.Channel;
                    return EFI_SUCCESS;
                }
            }
        }
    }
#endif
    
    return EFI_NOT_FOUND;
}

/**
  Used to retrieve the list of legal port multiplier port numbers for ATA devices on a port of an ATA 
  controller. These can either be the list of port multiplier ports where ATA devices are actually 
  present on port or the list of legal port multiplier ports on that port. Regardless, the caller of this 
  function must probe the port number and port multiplier port number returned to see if an ATA 
  device is actually present.

  The GetNextDevice() function retrieves the port multiplier port number of an ATA device 
  present on a port of an ATA controller.
  
  If PortMultiplierPort points to a port multiplier port number value that was returned on a 
  previous call to GetNextDevice(), then the port multiplier port number of the next ATA device
  on the port of the ATA controller is returned in PortMultiplierPort, and EFI_SUCCESS is
  returned.
  
  If PortMultiplierPort points to 0xFFFF, then the port multiplier port number of the first 
  ATA device on port of the ATA controller is returned in PortMultiplierPort and 
  EFI_SUCCESS is returned.
  
  If PortMultiplierPort is not 0xFFFF and the value pointed to by PortMultiplierPort
  was not returned on a previous call to GetNextDevice(), then EFI_INVALID_PARAMETER
  is returned.
  
  If PortMultiplierPort is the port multiplier port number of the last ATA device on the port of 
  the ATA controller, then EFI_NOT_FOUND is returned.

  @param This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param Port                The port number present on the ATA controller.
  @param PortMultiplierPort  On input, a pointer to the port multiplier port number of an
                             ATA device present on the ATA controller. 
                             If on input a PortMultiplierPort of 0xFFFF is specified, 
                             then the port multiplier port number of the first ATA device
                             is returned. On output, a pointer to the port multiplier port
                             number of the next ATA device present on an ATA controller.

  @retval EFI_SUCCESS                The port multiplier port number of the next ATA device on the port
                                     of the ATA controller was returned in PortMultiplierPort.
  @retval EFI_NOT_FOUND              There are no more ATA devices on this port of the ATA controller.
  @retval EFI_INVALID_PARAMETER      PortMultiplierPort is not 0xFFFF, and PortMultiplierPort was not
                                     returned on a previous call to GetNextDevice().


**/
EFI_STATUS
EFIAPI
GetNextDevice (
    IN EFI_ATA_PASS_THRU_PROTOCOL   *This,
    IN UINT16                       Port,
    IN OUT UINT16                   *PortMultiplierPort
)
{
    UINT16                  CurrentPortNumber = 0;
    UINT16                  CurrentPortMultiplierNumber = 0;
    UINT16                  PreviousPortMultiplierNumber = 0;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE   *SataDevices = 0;
    SATA_DEVICE_INTERFACE   *pSataDevices = 0;
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    AMI_IDE_BUS_PROTOCOL    *IdeDevices = 0;
    AMI_IDE_BUS_PROTOCOL    *pIdeDevices = 0;
#endif
    UINT16                  DevCount = 0;
    UINT8                   DevIndex = 0;
    BOOLEAN                 ModeFlag = ((ATA_PASS_THRU_PROTOCOL*)This)->ModeFlag;

    // If PMPort is NULL, return Invalid parameter
    if (PortMultiplierPort == NULL) {
        return EFI_INVALID_PARAMETER;
    }

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if(ModeFlag){

        pSataDevices = (SATA_DEVICE_INTERFACE *)((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceBuffer;
        if(pSataDevices == NULL) return EFI_NOT_FOUND;

        DevCount = ((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceCount;

        SataDevices = ((SATA_DEVICE_INTERFACE **)pSataDevices)[DevIndex];
        if((SataDevices == NULL) || (DevCount == 0)) {
            return EFI_NOT_FOUND;
        }
        
        // If input Port number is not equal to GetNextDeviceApi Previous Port Number returned on earlier call
        // and input PortMultiplierPort is not 0xFFFF, return invalid parameter
        if((SataDevices->AhciBusInterface->GetNextDeviceApiPrevPortNum != Port) && \
                (0xFFFF != *PortMultiplierPort) ) {
            return EFI_INVALID_PARAMETER;
        }

        PreviousPortMultiplierNumber = SataDevices->AhciBusInterface->PrevPortMultiplierPortNum;
		// If a device is directly attached to a port, previous call to this
         // function will return the value 0xFFFF for PortMultiplierPort. In
         // this case, there should be no more device on the port multiplier.
         if( (SataDevices->AhciBusInterface->GetNextDeviceApiPrevPortNum == Port) && (PreviousPortMultiplierNumber == 0xFFFF)){
             SataDevices->AhciBusInterface->PrevPortMultiplierPortNum = 0;
             return EFI_NOT_FOUND;
         }

        // If PortMultiplier is not 0xFFFF or Previous Port multiplier Number returned on earlier call
        // return invalid parameter
        if(( 0xFFFF != *PortMultiplierPort) && (PreviousPortMultiplierNumber != *PortMultiplierPort)) {
            return EFI_INVALID_PARAMETER;
        }
        // If PortMultiplierPort points to 0xFFFF, then the port multiplier port number of the first ATA device on port
        // of the ATA controller is returned in PortMultiplierPort and EFI_SUCCESS is returned.
        if(0xFFFF == *PortMultiplierPort) {
            for(DevIndex = 0; DevIndex < DevCount; DevIndex++){
                SataDevices = ((SATA_DEVICE_INTERFACE **)pSataDevices)[DevIndex];
                CurrentPortNumber = SataDevices->PortNumber;
                CurrentPortMultiplierNumber = SataDevices->PMPortNumber == 0xFF ? 0xFFFF : SataDevices->PMPortNumber;

                if( CurrentPortNumber == Port) {
                    SataDevices->AhciBusInterface->GetNextDeviceApiPrevPortNum = Port;
                    SataDevices->AhciBusInterface->PrevPortMultiplierPortNum = CurrentPortMultiplierNumber;
                    *PortMultiplierPort = CurrentPortMultiplierNumber;
                    return EFI_SUCCESS;
                }
            }
        }

        // If PortMultiplierPort points to a port multiplier port number value that was returned on a previous call to
        // GetNextDevice(), then the port multiplier port number of the next ATA device on the port of the
        // ATA controller is returned in PortMultiplierPort, and EFI_SUCCESS is returned.
        if(PreviousPortMultiplierNumber == *PortMultiplierPort) {
            for(DevIndex = 0; DevIndex < DevCount; DevIndex++){
                SataDevices = ((SATA_DEVICE_INTERFACE **)pSataDevices)[DevIndex];
                CurrentPortNumber = SataDevices->PortNumber;
                CurrentPortMultiplierNumber = SataDevices->PMPortNumber == 0xFF ? 0xFFFF : SataDevices->PMPortNumber;

                if(( CurrentPortNumber == Port) && (CurrentPortMultiplierNumber > *PortMultiplierPort)) {
                    SataDevices->AhciBusInterface->GetNextDeviceApiPrevPortNum = Port;
                    SataDevices->AhciBusInterface->PrevPortMultiplierPortNum = CurrentPortMultiplierNumber;
                    *PortMultiplierPort = CurrentPortMultiplierNumber;
                    return EFI_SUCCESS;
                }
            }
        }
    }
#endif
    
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    if(!ModeFlag){

        pIdeDevices = (AMI_IDE_BUS_PROTOCOL *)((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceBuffer;
        if(pIdeDevices == NULL) return EFI_NOT_FOUND;

        DevCount = ((ATA_PASS_THRU_PROTOCOL*)This)->AtaDeviceCount;

        IdeDevices = ((AMI_IDE_BUS_PROTOCOL **)pIdeDevices)[DevIndex];
        if((IdeDevices == NULL) || (DevCount == 0)) {
            return EFI_NOT_FOUND;
        }


        PreviousPortMultiplierNumber = IdeDevices->IdeBusInitInterface->PrevPortMultiplierPortNum;
        // If PortMultiplier is not 0xFFFF or Previous Port multiplier Number returned on earlier call
        // return invalid parameter
        if(( 0xFFFF != *PortMultiplierPort) && (PreviousPortMultiplierNumber != *PortMultiplierPort)) {
            return EFI_INVALID_PARAMETER;
        }

        // If PortMultiplierPort points to 0xFFFF, then the port multiplier port number of the first ATA device on port
        // of the ATA controller is returned in PortMultiplierPort and EFI_SUCCESS is returned.
        if( 0xFFFF == *PortMultiplierPort){

            for(DevIndex = 0; DevIndex < DevCount; DevIndex++){
                IdeDevices = ((AMI_IDE_BUS_PROTOCOL **)pIdeDevices)[DevIndex];
                CurrentPortNumber = IdeDevices->IdeDevice.Channel;
                CurrentPortMultiplierNumber = IdeDevices->IdeDevice.Device;

                if( CurrentPortNumber == Port) {
                    IdeDevices->IdeBusInitInterface->PrevPortMultiplierPortNum = CurrentPortMultiplierNumber;
                    *PortMultiplierPort = CurrentPortMultiplierNumber;
                    return EFI_SUCCESS;
                }
            }
        }

        // If PortMultiplierPort points to a port multiplier port number value that was returned on a previous call to
        // GetNextDevice(), then the port multiplier port number of the next ATA device on the port of the
        // ATA controller is returned in PortMultiplierPort, and EFI_SUCCESS is returned.
        if(PreviousPortMultiplierNumber == *PortMultiplierPort) {
            for(DevIndex = 0; DevIndex < DevCount; DevIndex++){
                IdeDevices = ((AMI_IDE_BUS_PROTOCOL **)pIdeDevices)[DevIndex];
                CurrentPortNumber = IdeDevices->IdeDevice.Channel;
                CurrentPortMultiplierNumber = IdeDevices->IdeDevice.Device;

                if(( CurrentPortNumber == Port) && (CurrentPortMultiplierNumber > *PortMultiplierPort)) {
                    IdeDevices->IdeBusInitInterface->PrevPortMultiplierPortNum = CurrentPortMultiplierNumber;
                    *PortMultiplierPort = CurrentPortMultiplierNumber;
                    return EFI_SUCCESS;
                }
            }
        }
    }
#endif
    
    return EFI_NOT_FOUND;
}

/**
  Used to allocate and build a device path node for an ATA device on an ATA controller.

  The BuildDevicePath() function allocates and builds a single device node for the ATA
  device specified by Port and PortMultiplierPort. If the ATA device specified by Port and
  PortMultiplierPort is not present on the ATA controller, then EFI_NOT_FOUND is returned.
  If DevicePath is NULL, then EFI_INVALID_PARAMETER is returned. If there are not enough
  resources to allocate the device path node, then EFI_OUT_OF_RESOURCES is returned.

  Otherwise, DevicePath is allocated with the boot service AllocatePool(), the contents of
  DevicePath are initialized to describe the ATA device specified by Port and PortMultiplierPort,
  and EFI_SUCCESS is returned.

  @param This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param Port                Port specifies the port number of the ATA device for which a
                             device path node is to be allocated and built.
  @param PortMultiplierPort  The port multiplier port number of the ATA device for which a
                             device path node is to be allocated and built. If there is no
                             port multiplier, then specify 0xFFFF.
  @param DevicePath          A pointer to a single device path node that describes the ATA
                             device specified by Port and PortMultiplierPort. This function
                             is responsible for allocating the buffer DevicePath with the
                             boot service AllocatePool(). It is the caller's responsibility
                             to free DevicePath when the caller is finished with DevicePath.
                             
  @retval EFI_SUCCESS                The device path node that describes the ATA device specified by
                                     Port and PortMultiplierPort was allocated and returned in DevicePath.
  @retval EFI_NOT_FOUND              The ATA device specified by Port and PortMultiplierPort does not
                                     exist on the ATA controller.
  @retval EFI_INVALID_PARAMETER      DevicePath is NULL.
  @retval EFI_OUT_OF_RESOURCES       There are not enough resources to allocate DevicePath.

**/

EFI_STATUS
EFIAPI
BuildDevicePath (
    IN  EFI_ATA_PASS_THRU_PROTOCOL      *This,
    IN  UINT16                          Port,
    IN  UINT16                          PortMultiplierPort,
    IN  OUT EFI_DEVICE_PATH_PROTOCOL    **DevicePath
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    SATA_DEVICE_PATH            *SataDevicePath = NULL;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE       *SataDevInterface = NULL;
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    ATAPI_DEVICE_PATH           *AtapiDevicePath = NULL;
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface = NULL;
#endif
    EFI_DEVICE_PATH_PROTOCOL    *DevPath;
    BOOLEAN                     ModeFlag = ((ATA_PASS_THRU_PROTOCOL*)This)->ModeFlag;

    // If DevicePath is null return EFI_INVALID_PARAMETER
    if(NULL == DevicePath){
        return  EFI_INVALID_PARAMETER;
    }

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if(ModeFlag) {
        // Allocate memory
        Status = pBS->AllocatePool(EfiBootServicesData,sizeof(SATA_DEVICE_PATH),(VOID**)&SataDevicePath);
        if(EFI_ERROR(Status)){
            return EFI_OUT_OF_RESOURCES;
        }

        // If Port Number and PortMultiplierPort is invalid, return with EFI_INVALID_PARAMETER
        SataDevInterface = GetSataDevInterfaceThruPortPMPort(This,Port,PortMultiplierPort);
        if(NULL == SataDevInterface){
            return  EFI_NOT_FOUND;
        }

        // Get SATA Device Path Node
        DevPath = SearchDevicePath (SataDevInterface->DevicePathProtocol, MESSAGING_DEVICE_PATH, MSG_SATA_DP);
        if(NULL == DevPath){
            return EFI_NOT_FOUND;
        }
        MemCpy(SataDevicePath,DevPath,sizeof(EFI_DEVICE_PATH_PROTOCOL));

        if (!(SataDevicePath == NULL)){
            SataDevicePath->HBAPortNumber = Port;
            SataDevicePath->PortMultiplierPortNumber = PortMultiplierPort;
            SataDevicePath->Lun = 0;
        }

        // Update The Device path
        *DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) SataDevicePath;

    }
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
    if(!ModeFlag){
        // Allocate memory
        Status = pBS->AllocatePool(EfiBootServicesData,sizeof(ATAPI_DEVICE_PATH),(VOID**)&AtapiDevicePath);
        if(EFI_ERROR(Status)){
            return EFI_OUT_OF_RESOURCES;
        }
        // If Port Number and PortMultiplierPort is invalid, return with EFI_INVALID_PARAMETER
        IdeBusInterface = GetIdeBusInterfaceThruPortPMPort(This,Port,PortMultiplierPort);
        if(NULL == IdeBusInterface){
            return  EFI_NOT_FOUND;
        }

        // Get ATAPI Device Path Node
        DevPath = SearchDevicePath (IdeBusInterface->DevicePathProtocol, MESSAGING_DEVICE_PATH, MSG_ATAPI_DP);
        if(NULL == DevPath){
            return EFI_NOT_FOUND;
        }
        MemCpy(AtapiDevicePath,DevPath,sizeof(EFI_DEVICE_PATH_PROTOCOL));

        if (!(AtapiDevicePath == NULL)){
            AtapiDevicePath->PrimarySecondary = (UINT8)Port;
            AtapiDevicePath->SlaveMaster = (UINT8)PortMultiplierPort;
            AtapiDevicePath->Lun = 0;
        }

        // Update The Device path
        *DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AtapiDevicePath;
    }
#endif

    return EFI_SUCCESS;
}

/**
  Used to translate a device path node to a port number and port multiplier port number.

  The GetDevice() function determines the port and port multiplier port number associated with
  the ATA device described by DevicePath. If DevicePath is a device path node type that the
  ATA Pass Thru driver supports, then the ATA Pass Thru driver will attempt to translate the contents 
  DevicePath into a port number and port multiplier port number.

  If this translation is successful, then that port number and port multiplier port number are returned
  in Port and PortMultiplierPort, and EFI_SUCCESS is returned.

  If DevicePath, Port, or PortMultiplierPort are NULL, then EFI_INVALID_PARAMETER is returned.

  If DevicePath is not a device path node type that the ATA Pass Thru driver supports, then 
  EFI_UNSUPPORTED is returned.

  If DevicePath is a device path node type that the ATA Pass Thru driver supports, but there is not 
  a valid translation from DevicePath to a port number and port multiplier port number, then 
  EFI_NOT_FOUND is returned.

  @param This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param DevicePath          A pointer to the device path node that describes an ATA device on the
                             ATA controller.
  @param Port                On return, points to the port number of an ATA device on the ATA controller.
  @param PortMultiplierPort  On return, points to the port multiplier port number of an ATA device
                             on the ATA controller.

  @retval EFI_SUCCESS             DevicePath was successfully translated to a port number and port multiplier
                                  port number, and they were returned in Port and PortMultiplierPort.
  @retval EFI_INVALID_PARAMETER   DevicePath is NULL.
  @retval EFI_INVALID_PARAMETER   Port is NULL.
  @retval EFI_INVALID_PARAMETER   PortMultiplierPort is NULL.
  @retval EFI_UNSUPPORTED         This driver does not support the device path node type in DevicePath.
  @retval EFI_NOT_FOUND           A valid translation from DevicePath to a port number and port multiplier
                                  port number does not exist.
**/

EFI_STATUS
EFIAPI
GetDevice (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    IN  EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
    OUT UINT16                      *Port,
    OUT UINT16                      *PortMultiplierPort
)
{
    EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath = DevicePath;
    UINT16                       DevicePathLength = 0;
    BOOLEAN                      ModeFlag = ((ATA_PASS_THRU_PROTOCOL*)This)->ModeFlag;
    SATA_DEVICE_PATH            *SataDevicePath = NULL;
    ATAPI_DEVICE_PATH           *AtapiDevicePath = NULL;

    if((DevicePath == NULL) || (Port == NULL)||(PortMultiplierPort == NULL)){
        return EFI_INVALID_PARAMETER;
    }

    DevicePathLength = (DevicePath)->Length[0] | (DevicePath)->Length[1] << 8;

    //
    // Check for Validity
    //
    if(ModeFlag) {
        if ((DevicePath->Type != MESSAGING_DEVICE_PATH) ||
            (DevicePath->SubType != MSG_SATA_DP) ||
            (DevicePathLength != sizeof(SATA_DEVICE_PATH))) {
            return EFI_UNSUPPORTED;
        }

        SataDevicePath = (SATA_DEVICE_PATH *)RemainingDevicePath;
        if (!(SataDevicePath == NULL)){
            *Port                = SataDevicePath->HBAPortNumber;
            *PortMultiplierPort  = SataDevicePath->PortMultiplierPortNumber;
        }
    } else {
        if ((DevicePath->Type != MESSAGING_DEVICE_PATH) ||
            (DevicePath->SubType != MSG_ATAPI_DP) ||
            (DevicePathLength != sizeof(ATAPI_DEVICE_PATH))) {
            return EFI_UNSUPPORTED;
        }

        AtapiDevicePath = (ATAPI_DEVICE_PATH *)RemainingDevicePath;
        if (!(AtapiDevicePath == NULL)){
            *Port                = AtapiDevicePath->PrimarySecondary;
            *PortMultiplierPort  = AtapiDevicePath->SlaveMaster;
        }
    }
    
    return EFI_SUCCESS;
}

/**
  Resets a specific port on the ATA controller. This operation also resets all the ATA devices
  connected to the port.

  The ResetChannel() function resets an a specific port on an ATA controller. This operation
  resets all the ATA devices connected to that port. If this ATA controller does not support
  a reset port operation, then EFI_UNSUPPORTED is returned.

  If a device error occurs while executing that port reset operation, then EFI_DEVICE_ERROR is
  returned.

  If a timeout occurs during the execution of the port reset operation, then EFI_TIMEOUT is returned.

  If the port reset operation is completed, then EFI_SUCCESS is returned.

  @param This          A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param Port          The port number on the ATA controller.

  @retval EFI_SUCCESS       The ATA controller port was reset.
  @retval EFI_UNSUPPORTED   The ATA controller does not support a port reset operation.
  @retval EFI_DEVICE_ERROR  A device error occurred while attempting to reset the ATA port.
  @retval EFI_TIMEOUT       A timeout occurred while attempting to reset the ATA port.

**/

EFI_STATUS
EFIAPI
ResetPort (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    IN  UINT16                      Port
)
{
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    EFI_STATUS              Status;
    SATA_DEVICE_INTERFACE   *SataDevInterface;
    AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface;
    BOOLEAN                 ModeFlag = ((ATA_PASS_THRU_PROTOCOL*)This)->ModeFlag;

    if(ModeFlag){
        //
        //Get SATA device interface
        //
        SataDevInterface = GetSataDevInterfaceThruPortPMPort(This,Port,0xFFFF);
        if(NULL == SataDevInterface){
            return  EFI_INVALID_PARAMETER;
        }

        AhciBusInterface = SataDevInterface->AhciBusInterface;
        Status = SataDevInterface->AhciBusInterface->GeneratePortReset( AhciBusInterface,
                                                                        SataDevInterface,(UINT8) Port,
                                                                        (UINT8)0xFF,
                                                                        0x00,
                                                                        0x00
                                                                        );
        return Status;
    }
#endif
    return EFI_UNSUPPORTED;
}

/**
  Resets an ATA device that is connected to an ATA controller.

  The ResetDevice() function resets the ATA device specified by Port and PortMultiplierPort.
  If this ATA controller does not support a device reset operation, then EFI_UNSUPPORTED is
  returned.

  If Port or PortMultiplierPort are not in a valid range for this ATA controller, then 
  EFI_INVALID_PARAMETER is returned.

  If a device error occurs while executing that device reset operation, then EFI_DEVICE_ERROR
  is returned.

  If a timeout occurs during the execution of the device reset operation, then EFI_TIMEOUT is
  returned.

  If the device reset operation is completed, then EFI_SUCCESS is returned.

  @param This                    A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param Port                    Port represents the port number of the ATA device to be reset.
  @param PortMultiplierPort      The port multiplier port number of the ATA device to reset.
                                 If there is no port multiplier, then specify 0xFFFF.
                                 
  @retval EFI_SUCCESS            The ATA device specified by Port and PortMultiplierPort was reset.
  @retval EFI_UNSUPPORTED        The ATA controller does not support a device reset operation.
  @retval EFI_INVALID_PARAMETER  Port or PortMultiplierPort are invalid.
  @retval EFI_DEVICE_ERROR       A device error occurred while attempting to reset the ATA device
                                 specified by Port and PortMultiplierPort.
  @retval EFI_TIMEOUT            A timeout occurred while attempting to reset the ATA device
                                 specified by Port and PortMultiplierPort.

**/

EFI_STATUS
EFIAPI
ResetDevice (
    IN  EFI_ATA_PASS_THRU_PROTOCOL  *This,
    IN  UINT16                      Port,
    IN  UINT16                       PortMultiplierPort
)
{
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    EFI_STATUS              Status;
    SATA_DEVICE_INTERFACE   *SataDevInterface;
    BOOLEAN                 ModeFlag = ((ATA_PASS_THRU_PROTOCOL*)This)->ModeFlag;

    if(ModeFlag){
        //
        // Get SATA device interface
        //
        SataDevInterface = GetSataDevInterfaceThruPortPMPort(This,Port,PortMultiplierPort);
        if(NULL == SataDevInterface){
            return  EFI_INVALID_PARAMETER;
        }

        Status = SataDevInterface->AhciBusInterface->GeneratePortReset( SataDevInterface->AhciBusInterface,
                                                                        SataDevInterface,(UINT8) Port,
                                                                        (UINT8) PortMultiplierPort,
                                                                        0x00,
                                                                        0x00
                                                                      );

        return Status;
    }
#endif
    return EFI_UNSUPPORTED;

}

/**@internal
    Checks if the command is for 48-bit LBA

    @param  Command   Input command

    @retval TRUE  Command is for 48-bit LBA
    @retval FALSE  Command is not for 48-bit LBA
    
    @endinternal
**/
BOOLEAN
Check48BitCommand (
    IN UINT8        Command
 )
{
    if ( Command == READ_SECTORS_EXT ||
         Command == READ_MULTIPLE_EXT ||
         Command == WRITE_SECTORS_EXT ||
         Command == WRITE_MULTIPLE_EXT ||
         Command == READ_DMA_EXT    ||
         Command == WRITE_DMA_EXT )
        return TRUE;
    else
        return FALSE;
}



