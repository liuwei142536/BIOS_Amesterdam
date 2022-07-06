//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiPciHotPlugLib.c
    Library Class for AMI Hot Plug Support Functions. Dummy Instance


**/

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <AmiLib.h>
#include <AcpiRes.h>
#include <PciBus.h>
#include <PciHostBridge.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHotPlugInit.h>
//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
//ONLY ONE INSTANCE ALLOWED IN THE SYSTEM!!!
EFI_PCI_HOT_PLUG_INIT_PROTOCOL	*gRhpcInitProtocol=NULL;
BOOLEAN                          gRhpcNotFound=FALSE;

/**
    This function will check if "HostBrg" passed is the Bridge
    with hotplug support.

        
    @param HostBrg Pointer to PCI HB Private Data structure.

    @retval BOOLEAN
        TRUE    If "Device" is a Bridge with HPC on it.
        FALSE   Otherwise.

**/
BOOLEAN HpcCheckHpCompatibleHost(PCI_HOST_BRG_DATA *HostBrg){
	return FALSE;
}

/**
    This function will check if "Device" passed is the Bridge
    Type Device with hotplug support.

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval BOOLEAN
        TRUE    If "Device" is a Bridge with HPC on it.
        FALSE   Otherwise.

**/
BOOLEAN HpIsHpb(PCI_DEV_INFO *Device){

	return FALSE;
}

/**
    This function will update pointer to PCI_RHPC_INFO of
    Bridge Type device which creates a hot plug bus. Also if "Device"
    creates the 'home bus' for Root HPC it will initialize Root HPC and
    record the HPC state;

    @param Device Pointer to PCI Device Private Data structure.
    @param MaxBusFound maximum number of pci Bus. 

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS HpCheckRootHotplug(PCI_DEV_INFO *Device, UINT8 MaxBusFound){
	return EFI_SUCCESS;
}

/**
    This function will get and apply resource padding
    requirements for the PCI to PCI Bridge or Card Bus Bridge, if this
    Bridge is supports hot plug.

    @param Device Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS HpGetHpbResPadding(PCI_DEV_INFO *Device){
	return EFI_SUCCESS;
}


/**
    Checks if Hot Plug Controller features supported by the Device
    
    @param Device Pointer to PCI Device Private Data structure. 

    @retval BOOLEAN
        TRUE or FALSE based on value passed. 

**/
BOOLEAN HpCheckHpCompatible(PCI_DEV_INFO *Device){
	return FALSE;
}

/**
    Initializes Hot Plug Data.

    @param Device Pointer PCI Devices Private Data.
    @param HpCapOffset offset for PCI Hot-plug Capability header. 

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid 

**/
EFI_STATUS HpAllocateInitHpData(PCI_DEV_INFO *Device, UINT16 HpCapOffset){
	return EFI_SUCCESS;
}


/**
    Finds Root Hot Plug Controllers Interface, gets rhpc list and sort out list items.

    @param RootBridge Pointer to PCI Device Private Data. 
    @param PciHpcHost pointer to the Root Structure of the PCI Subsystem.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_INVALID_PARAMETER   When some of the parameters are invalid.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.  

**/
EFI_STATUS HpLocateProtocolSortRhpc(PCI_HOST_INFO	*PciHpcHost, PCI_DEV_INFO	*RootBridge){
	return EFI_SUCCESS;
}


/**
    This function will update information about PCI Device.

    @param PaddingBar Pointer to PCI_BAR structure to clear.
    @param BridgeBar Pointer to PCI_BAR structure to clear.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_INVALID_PARAMETER   When some of the parameters are invalid.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.  

**/
EFI_STATUS HpApplyResPadding(PCI_BAR *PaddingBar, PCI_BAR *BridgeBar){
	return EFI_UNSUPPORTED;
}


/**

    Applies Bus resource padding to the hot pluggable slots.

    @param Brg Pointer to PCI Device Private Data structure.
    @param MaxBusFound maximum number of pci Bus. 

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid. 

**/
EFI_STATUS HpApplyBusPadding(PCI_DEV_INFO *Brg, UINT8 OldMaxBus, UINT8 *MaxBusFound){
	return EFI_SUCCESS;

}

/**

    @param HostBrg Pointer to Private Host Bridge Data structure 
    @param RootBrg Pointer to the Private RootBridge Data Structure.     
    @param HpBrgSdlData Pointer to the Pci device information.
    @param HpBrgIndex hot plug Bridge index.

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.  

**/
EFI_STATUS HpcUpdateLocationData(
                                PCI_HOST_BRG_DATA 		*HostBrg, 
                                PCI_ROOT_BRG_DATA 		*RootBrg, 
                                AMI_SDL_PCI_DEV_INFO	*HpBrgSdlData,
                                UINTN					HpBrgIndex)
{
	return EFI_SUCCESS;
}


/**
    This function find
    information and initialize an instance of PCI Root Hotplug Controller
    Initialization Protocol.

    @param HostBrg Pointer to Private Host Bridge Data structure 
    @param RootBrg Pointer to the Private RootBridge Data Structure.

    @retval EFI_SUCCESS is OK
        EFI_NOT_FOUND no Hotplug slots where found.

    @note  CHIPSET AND/OR BOARD PORTING NEEDED
**/
EFI_STATUS HpcFindSlots(PCI_HOST_BRG_DATA *HostBrg, PCI_ROOT_BRG_DATA *RootBrg)
{
    return EFI_SUCCESS;
}

/**
    GetRootHpcList() Function of the PCI Root Hotplug Controller
    Initialization Protocol. See PI 1.1 Spec or details
    
    @param[in]  This Pointer to the EFI_PCI_HOT_PLUG_INIT_PROTOCOL instance.
    @param[out] HpcCount Number of Root Bridges Hot Plug Location Data.
    @param[out] HpcList List of Root Bridges Hot Plug Location Data.
    
    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid. 

**/
EFI_STATUS HpcInitGetRootHpcList(IN EFI_PCI_HOT_PLUG_INIT_PROTOCOL      *This,
                                 OUT UINTN                              *HpcCount,
                                 OUT EFI_HPC_LOCATION                   **HpcList)
{
    return EFI_SUCCESS;
}


/**
    This function will Find PCI Hot Plug Controller Location Data.

    @param[in]  HpcDevicePath Device path acquired from BootXXXX EFI variable.
    @param[in]  HpcData pointer to the Private HostBridge Data Structure.
    @param[out] HpcLocData  information related to HPC Location in the System.

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.  

**/
EFI_STATUS FindHpcLocData(  IN EFI_DEVICE_PATH_PROTOCOL *HpcDevicePath,
                            IN PCI_HPC_DATA             *HpcData,
                            OUT HPC_LOCATION_DATA       **HpcLocData )
{
    return EFI_SUCCESS;

}


/**
    InitializeRootHpc() Function of the PCI Root Hotplug Controller
    Initialization Protocol. See PI 1.1 Spec or details

    
    @param[in]  This Pointer to the EFI_PCI_HOT_PLUG_INIT_PROTOCOL instance.
    @param[in]  HpcDevicePath Device path acquired from BootXXXX EFI variable.
    @param[in]  HpcPciAddress pci address 
    @param[in]  Event The event signaled.
    @param[out] HpcState current state of an HPC. 
    
    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid. 
    @note  
        This function call will be used to overwrite default resource Padding settings.
**/
EFI_STATUS HpcInitInitializeRootHpc(IN EFI_PCI_HOT_PLUG_INIT_PROTOCOL       *This,
                                    IN  EFI_DEVICE_PATH_PROTOCOL            *HpcDevicePath,
                                    IN  UINT64                              HpcPciAddress,
                                    IN  EFI_EVENT                         	Event, OPTIONAL
                                    OUT EFI_HPC_STATE                    	*HpcState)
{
    return EFI_SUCCESS;
}


/**
    This function will update QWORD Address Space Resource Descriptor.

    @param Descriptor Pointer to the QWORD Address Space Resource Descriptor.
    @param Length for Descriptor.

    @retval VOID

**/
VOID HpcFillDescriptor(ASLR_QWORD_ASD *Descriptor, HP_PADD_RES_TYPE PaddType, UINT64  Length){
    return;
}


/**
    This function will Get hot plug resource padding.

    @param[in]  This Pointer to the EFI_PCI_HOT_PLUG_INIT_PROTOCOL instance.
    @param[in]  HpcDevicePath Device path acquired from BootXXXX EFI variable
    @param[in]  HpcPciAddress pci address
    @param[out] HpcState The state of the HPC hardware.
    @param[out] Padding Pointer to the Pointer to the QWORD Address Space Resource Descriptor.
    @param[out] Attributes Describes how padding is accounted for. The padding
                is returned in the form of ACPI 2.0 resource descriptors.   

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.  

**/
EFI_STATUS HpcGetResourcePadding(IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL *This,
                                 IN  EFI_DEVICE_PATH_PROTOCOL       *HpcDevicePath,
                                 IN  UINT64                         HpcPciAddress,
                                 OUT EFI_HPC_STATE                  *HpcState,
                                 OUT VOID                           **Padding,
                                 OUT EFI_HPC_PADDING_ATTRIBUTES     *Attributes)
{
    return EFI_SUCCESS;
}


/**
    This function will collect all information HP related
    information and initialize an instance of PCI Root Hotplug Controller
    Initialization Protocol.

    @param HostBrg Pointer on Private Data
        structure for which PCI Hot Plug Init Protocol going to
        be initialized

    @retval EFI_SUCCESS is OK
        EFI_NOT_FOUND no Hotplug slots where found.

    @note  CHIPSET AND/OR BOARD PORTING NEEDED
**/
EFI_STATUS HpcInstallHpProtocol(PCI_HOST_BRG_DATA *HostBrg)
{
    return EFI_SUCCESS;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

