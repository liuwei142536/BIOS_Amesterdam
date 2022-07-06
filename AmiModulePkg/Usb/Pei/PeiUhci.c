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

/** @file PeiUhci.c
    This file is the main source file for the UHCI PEI USB
    recovery module

**/
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include "UsbPei.h"

//----------------------------------------------------------------------------
// Global Constants
//----------------------------------------------------------------------------

#define PCI_REG_UHCI_COMMAND_REGISTER         0x04
#define PCI_REG_UHCI_BASE_ADDRESS_REGISTER    0x20
#define PCI_REG_UHCI_LEGACY_CONTROL_REGISTER  0xc0
#define PCI_REG_UHCI_RESUME_ENABLE_REGISTER   0xc4

#define PEI_UHCI_CONTROLLER                   0x01

#define PCI_REG_EHCI_MEMORY_BAR               0x10

#define EHCI_HC_OPERATIONAL_CONFIG_FLAG_REG     0x40

PCI_BUS_DEV_FUNCTION    *gUhciPciReg;


//----------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------

typedef struct
{
    UINTN IoBase[MAX_USB_PEI_UHCI_HOST];
} PCI_UHCI_DEVICE;

//----------------------------------------------------------------------------
// Function Prototypes
//----------------------------------------------------------------------------

EFI_STATUS GetUhciController (
    IN EFI_PEI_SERVICES           **PeiServices,
    IN PEI_USB_CONTROLLER_PPI     *This,
    IN UINT8                      UsbControllerId,
    IN UINTN                      *ControllerType,
    IN UINTN                      *IoBaseAddress );

EFI_STATUS
EnableUhciController (
    IN EFI_PEI_SERVICES     **PeiServices,
    IN EFI_PEI_PCI_CFG2_PPI *PciCfgPpi,
    IN UINTN                BaseAddress,
    IN UINT8                UsbControllerId );

VOID
ClearEhciConfigFlagReg (
    IN EFI_PEI_SERVICES     **PeiServices,
    IN EFI_PEI_PCI_CFG2_PPI *PciCfgPpi );

//----------------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------------
PCI_BUS_DEV_FUNCTION  *gEhciPciBDFs;
UINT32 *gEhciMemBaseAddresses;

PCI_UHCI_DEVICE gPeiUhciDev;

PEI_USB_CONTROLLER_PPI GetUhciControllerPpi = {
    GetUhciController
};

// PPI to be installed
static
EFI_PEI_PPI_DESCRIPTOR PpiList[] = {
    {EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
     &gPeiUsbControllerPpiGuid, &GetUhciControllerPpi}
};
extern USB_PEI_GLOBAL_DATA    *gUsbPeiData;
//----------------------------------------------------------------------------
// Function Definitions
//----------------------------------------------------------------------------


/**
    This function is the entry point for this PEI. This function
    initializes the UHCI Controller

    @param FfsHeader   Pointer to the FFS file header
        PeiServices Pointer to the PEI services table

    @retval Return Status based on errors that occurred while waiting for
        time to expire.

**/

EFI_STATUS
EFIAPI
UhciPeiUsbEntryPoint (
    IN EFI_FFS_FILE_HEADER *FfsHeader,
    IN EFI_PEI_SERVICES    **PeiServices )
{

    UINTN      Index;
    EFI_STATUS Status;
    EFI_PEI_PCI_CFG2_PPI *PciCfgPpi;
    UINT8       ClassCode[4];

    if ((gUsbPeiData->UsbPeiFeature & USB_UHCI_PEI_SUPPORT) != USB_UHCI_PEI_SUPPORT)
        return EFI_UNSUPPORTED;
        
    gUhciPciReg = gUsbPeiData->UhciPciDev;
    gEhciPciBDFs = gUsbPeiData->EhciPciDev;
    gEhciMemBaseAddresses = gUsbPeiData->EhciIoResource;

    // Set PCI Config pointer
    PciCfgPpi = (*PeiServices)->PciCfg;

    ClearEhciConfigFlagReg( PeiServices, PciCfgPpi );

    // Assign resources and enable UHCI controllers
    for (Index = 0; Index < gUsbPeiData->UhciHcCount; Index++) {
        // PEI_UHCI_IOBASE = 0x4000
        gPeiUhciDev.IoBase[Index] = gUsbPeiData->UhciIoBase + 0x40 * Index;

        (*PeiServices)->PciCfg->Read(PeiServices,(*PeiServices)->PciCfg,
                                 EfiPeiPciCfgWidthUint32, 
                                 EFI_PEI_PCI_CFG_ADDRESS( gUhciPciReg[Index].Bus, 
                                 gUhciPciReg[Index].Device, 
                                 gUhciPciReg[Index].Function, 
                                 PCI_REG_REVID ), 
                                 ClassCode);
        
        DEBUG((DEBUG_INFO, "USB UHCI #%d PI %x SubClassCode %x BaseCode %x\n", 
                Index, ClassCode[1], ClassCode[2], ClassCode[3]));

        if ((ClassCode[1] != PCI_CLASSC_PI_UHCI) || 
            (ClassCode[2] != PCI_CLASSC_SUBCLASS_SERIAL_USB) ||
            (ClassCode[3] != PCI_CLASSC_BASE_CLASS_SERIAL)) {
            continue;   //This is not an uhci controller.
        }
        
        Status = EnableUhciController( PeiServices, PciCfgPpi, gPeiUhciDev.IoBase[Index], (UINT8) Index );
        if (EFI_ERROR(Status)){
            DEBUG((DEBUG_INFO, "Enable Uhci Controller Status = %r\n", Status));
    
        }
    }

    // Install USB Controller PPI
    (*PeiServices)->InstallPpi(PeiServices, PpiList);
    
    UhcPeimEntry(NULL, PeiServices );
    return EFI_SUCCESS;
}


/**
    This function enables the UHCI controller

    @param **PeiServices       Pointer to the PEI services table
        *PciCfgPpi          Pointer to the PCI Configuration PPI
        BaseAddress         I/O base Address to be programmed for the
        UHCI controller
        UsbControllerId     USB Controller ID

    @retval EFI_STATUS

**/

EFI_STATUS EnableUhciController (
    IN EFI_PEI_SERVICES     **PeiServices,
    IN EFI_PEI_PCI_CFG2_PPI *PciCfgPpi,
    IN UINTN                BaseAddress,
    IN UINT8                UsbControllerId )
{
    UINT16 CmdReg;


    // Validate USB Controller ID
    if (UsbControllerId >= gUsbPeiData->UhciHcCount) {
        return EFI_INVALID_PARAMETER;
    }

    // Assign base address register
    PciCfgPpi->Write( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint32,
                      EFI_PEI_PCI_CFG_ADDRESS( gUhciPciReg[UsbControllerId].Bus, 
                      gUhciPciReg[UsbControllerId].Device, 
                      gUhciPciReg[UsbControllerId].Function, 
                      PCI_REG_UHCI_BASE_ADDRESS_REGISTER ), &BaseAddress );

    // Enable UHCI PCI Command register
    PciCfgPpi->Read( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint16,
                      EFI_PEI_PCI_CFG_ADDRESS( gUhciPciReg[UsbControllerId].Bus, 
                      gUhciPciReg[UsbControllerId].Device, 
                      gUhciPciReg[UsbControllerId].Function, 
                      PCI_REG_UHCI_COMMAND_REGISTER ), &CmdReg );
        
    CmdReg = (UINT16) (CmdReg | 0x05);

    PciCfgPpi->Write( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint16,
                      EFI_PEI_PCI_CFG_ADDRESS( gUhciPciReg[UsbControllerId].Bus, 
                      gUhciPciReg[UsbControllerId].Device, 
                      gUhciPciReg[UsbControllerId].Function, 
                      PCI_REG_UHCI_COMMAND_REGISTER ), &CmdReg );
    return EFI_SUCCESS;
}


/**

    @param **PeiServices       Pointer to the PEI services table
        *PciCfgPpi          Pointer to the PCI Configuration PPI

**/
VOID ClearEhciConfigFlagReg (
    IN EFI_PEI_SERVICES     **PeiServices,
    IN EFI_PEI_PCI_CFG2_PPI *PciCfgPpi )
{
    UINT32 Index;
    UINT16 CmdReg;
    UINT32 ConfigureFlagAddress;
    
    if (!gUsbPeiData->EhciHcCount) return;

    for (Index = 0; Index < gUsbPeiData->EhciHcCount; Index++) {
        // Assign Memory Base Address
        PciCfgPpi->Write( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint32,
                     EFI_PEI_PCI_CFG_ADDRESS( gEhciPciBDFs[Index].Bus, 
                     gEhciPciBDFs[Index].Device, 
                     gEhciPciBDFs[Index].Function, 
                     PCI_REG_EHCI_MEMORY_BAR ),
                     &gEhciMemBaseAddresses[Index] );

        // Set the Memory Space Enable and Bus Master Enable bits in the
        // PCI Command register.
        PciCfgPpi->Read( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint16,
                     EFI_PEI_PCI_CFG_ADDRESS( gEhciPciBDFs[Index].Bus, 
                     gEhciPciBDFs[Index].Device, 
                     gEhciPciBDFs[Index].Function, 
                     PCI_REG_UHCI_COMMAND_REGISTER ),
                     &CmdReg );
            
        CmdReg = (UINT16) (CmdReg | 0x06);

        PciCfgPpi->Write( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint16,
                     EFI_PEI_PCI_CFG_ADDRESS( gEhciPciBDFs[Index].Bus, 
                     gEhciPciBDFs[Index].Device, 
                     gEhciPciBDFs[Index].Function, 
                     PCI_REG_UHCI_COMMAND_REGISTER ),
                     &CmdReg );

        //Clear Config Flag Register
        ConfigureFlagAddress=gEhciMemBaseAddresses[Index]+(UINT32)MmioRead8(gEhciMemBaseAddresses[Index])+EHCI_HC_OPERATIONAL_CONFIG_FLAG_REG;
        MmioWrite32(ConfigureFlagAddress, 0);

        // Clear the Memory Space Enable and Bus Master Enable bits in the
        // PCI Command register.
        PciCfgPpi->Read( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint16,
                     EFI_PEI_PCI_CFG_ADDRESS( gEhciPciBDFs[Index].Bus, 
                     gEhciPciBDFs[Index].Device, 
                     gEhciPciBDFs[Index].Function, 
                     PCI_REG_UHCI_COMMAND_REGISTER ), 
                     &CmdReg );
            
        CmdReg &= ~(0x6);

        PciCfgPpi->Write( PeiServices, PciCfgPpi, EfiPeiPciCfgWidthUint16,
                     EFI_PEI_PCI_CFG_ADDRESS( gEhciPciBDFs[Index].Bus, 
                     gEhciPciBDFs[Index].Device, 
                     gEhciPciBDFs[Index].Function, 
                     PCI_REG_UHCI_COMMAND_REGISTER ), 
                     &CmdReg );
    }
}

/**
    This function returns controller type and I/O base address
    value for the controller specified

    @param **PeiServices       Pointer to the PEI services table
        *PciCfgPpi          Pointer to the PCI Configuration PPI
        UsbControllerId     USB Controller ID

    @retval EFI_STATUS
        *ControllerType     Type of the USB controller
        *IoBaseAddress      I/O base Address programmed for this
        controller

**/

EFI_STATUS
GetUhciController(
    IN EFI_PEI_SERVICES           **PeiServices,
    IN PEI_USB_CONTROLLER_PPI     *This,
    IN UINT8                      UsbControllerId,
    OUT UINTN                     *ControllerType,
    OUT UINTN                     *IoBaseAddress
)
{
    UINT8   ClassCode[4];

    if (UsbControllerId >= gUsbPeiData->UhciHcCount) {
        return EFI_INVALID_PARAMETER;
    }

    (*PeiServices)->PciCfg->Read(PeiServices,(*PeiServices)->PciCfg,
                                 EfiPeiPciCfgWidthUint32, 
                                 EFI_PEI_PCI_CFG_ADDRESS( gUhciPciReg[UsbControllerId].Bus, 
                                 gUhciPciReg[UsbControllerId].Device, 
                                 gUhciPciReg[UsbControllerId].Function, 
                                 PCI_REG_REVID ), 
                                 ClassCode);
        
    DEBUG((DEBUG_INFO, "USB UHCI #%d PI %x SubClassCode %x BaseCode %x\n",
            UsbControllerId, ClassCode[1], ClassCode[2], ClassCode[3]));

    if ((ClassCode[1] != PCI_CLASSC_PI_UHCI) || 
        (ClassCode[2] != PCI_CLASSC_SUBCLASS_SERIAL_USB) ||
        (ClassCode[3] != PCI_CLASSC_BASE_CLASS_SERIAL)) {
        *ControllerType = 0; //This is not an uhci controller.
        return EFI_SUCCESS;
    }

    *ControllerType = PEI_UHCI_CONTROLLER;
    *IoBaseAddress = gPeiUhciDev.IoBase[UsbControllerId];

    return EFI_SUCCESS;
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
