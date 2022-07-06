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

/** @file AmiPciHotPlugLib.h
    Library Class for AMI Hot Plug Support Functions. 


**/

#ifndef _AMI_PCI_HOT_PLUG_LIB_H_
#define _AMI_PCI_HOT_PLUG_LIB_H_
#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------

#include <Pci.h>  
#include <PciSetup.h>
#include <PciBus.h>
#include <PciHostBridge.h>
#include <AmiDxeLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHotPlugInit.h> 

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
//extern EFI_PCI_HOT_PLUG_INIT_PROTOCOL	*gRhpcInitProtocol;

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
EFI_STATUS HpCheckRootHotplug(PCI_DEV_INFO *Device, UINT8 MaxBusFound);


/**
    This function will check if "Device" passed is the Bridge
    Type Device with hotplug support.

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval BOOLEAN
        TRUE    If "Device" is a Bridge with HPC on it.
        FALSE   Otherwise.

**/
BOOLEAN HpIsHpb(PCI_DEV_INFO *Device);

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
EFI_STATUS HpGetHpbResPadding(PCI_DEV_INFO *Device);


/**
    Initializes Hot Plug Data.
	
    @param Device Pointer PCI Device Private Data.
    @param HpCapOffset offset for PCI Hot-plug Capability header. 

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid 

**/
EFI_STATUS HpAllocateInitHpData(PCI_DEV_INFO *Device, UINT16 HpCapOffset);

/**
    Checks if Hot Plug Controller features supported by the Device
    
    @param Device Pointer to PCI Device Private Data structure. 

    @retval BOOLEAN
        TRUE or FALSE based on value passed. 

**/
BOOLEAN HpCheckHpCompatible(PCI_DEV_INFO *Device);

/**
    Finds Root Hot Plug Controllers Interface, gets rhpc list and sort out list items.

    @param RootBridge Pointer to PCI Device Private Data. 
    @param PciHpcHost pointer to the Root Structure of the PCI Subsystem.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_INVALID_PARAMETER   When some of the parameters are invalid.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.  

**/
EFI_STATUS HpLocateProtocolSortRhpc(PCI_HOST_INFO *PciHpcHost, PCI_DEV_INFO	*RootBridge);


/**
    Applies Bus resource padding to the hot pluggable slots.

    @param Brg Pointer to PCI Device Private Data structure.
    @param OldMaxBus previous count of maximum number of pci Bus.
    @param MaxBusFound maximum number of pci Bus. 

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid. 

**/

EFI_STATUS HpApplyBusPadding(PCI_DEV_INFO *Brg, UINT8 OldMaxBus, UINT8 *MaxBusFound);


/**
    This function will update information about PCI Device.

    @param PaddingBar Pointer to PCI_BAR structure to clear.
    @param BridgeBar Pointer to PCI_BAR structure to clear.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_INVALID_PARAMETER   When some of the parameters are invalid.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.  

**/
EFI_STATUS HpApplyResPadding(PCI_BAR *PaddingBar, PCI_BAR *BridgeBar);


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
EFI_STATUS HpcInstallHpProtocol (PCI_HOST_BRG_DATA *HostBrg	);

/**
    This function find
    information and initialize an instance of PCI Root Hotplug Controller
    Initialization Protocol.

    @param HostBrg Pointer on Private Data structure for which PCI Hot Plug Init 
           Protocol going to be initialized.
    @param RootBrg Pointer to the Private RootBridge Data Structure.

    @retval EFI_SUCCESS is OK
        EFI_NOT_FOUND no Hotplug slots where found.

    @note  CHIPSET AND/OR BOARD PORTING NEEDED
**/
EFI_STATUS HpcFindSlots(PCI_HOST_BRG_DATA *HostBrg, PCI_ROOT_BRG_DATA *RootBrg);


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
                                 OUT EFI_HPC_PADDING_ATTRIBUTES     *Attributes);



/**
    This function will update QWORD Address Space Resource Descriptor.

    @param Descriptor Pointer to the QWORD Address Space Resource Descriptor.
    @param Length for Descriptor.

    @retval Nothing

**/
VOID HpcFillDescriptor(ASLR_QWORD_ASD *Descriptor, HP_PADD_RES_TYPE PaddType, UINT64  Length);

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
                                    OUT EFI_HPC_STATE                    	*HpcState);



/**
    This function will check if "HostBrg" passed is the Bridge
    with hotplug support.

        
    @param HostBrg Pointer to PCI HB Private Data structure.

    @retval BOOLEAN
        TRUE    If "Device" is a Bridge with HPC on it.
        FALSE   Otherwise.

**/
BOOLEAN HpcCheckHpCompatibleHost(PCI_HOST_BRG_DATA *HostBrg);



/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif //_AMI_PCI_BUS_COMMON_LIB_H_


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

