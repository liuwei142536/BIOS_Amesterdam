//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiPciExpressGen3Lib.c
    Library Class for AMI Pci Express GEN 3 Support Functions. Dummy Instance


**/

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <AmiLib.h>
#include <AcpiRes.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/AmiPciBusLib.h>

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------


/**

    @param 

    @retval 

**/
/*
EFI_STATUS EFIAPI AmiPciExpressLibConstructor(IN EFI_HANDLE  ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable)
{
	EFI_STATUS				Status=EFI_SUCCESS;
//-------------------------------------------------
	InitAmiLib(ImageHandle, SystemTable);

	return Status;;
}
*/


/**
     

    @param Device Pointer to device private data structure
    @param LnkRetrain TRUE/FALSE 
    @param LinkSpeed check LNK SPEED VECTOR.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid. 

**/
EFI_STATUS Pcie3EqualizeLink(PCI_DEV_INFO *Device, BOOLEAN *LnkRetrain, UINT8 LinkSpeed){
    return EFI_SUCCESS;
}


/**
    This function will Get & display Content of the 
    LNK_STA2 register and Check equalization Status.
    
    @param Device Pointer to PCI Device Private Data structure.
    @param LinkSpeed check LNK SPEED VECTOR.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid. 

**/
EFI_STATUS Pcie3GetEqualizationStatus(PCI_DEV_INFO *Device, UINT8 LinkSpeed){

    return EFI_SUCCESS;
}

/**
    This function will Update Speed encoding and 
    display content of LNK_CAP2_REG.
    
    @param Device Pointer to PCI Device Private Data structure. 

    @retval VOID  

**/
VOID Pcie3InitDisplayPcie3Data(PCI_DEV_INFO *Device){
	return;
}

/**
    This function will Allocate the memory for Pcie3
    and Getting Content of LaneErrSts and MaxLanesCount
    
    @param Device Pointer to PCI Device Private Data structure.
    @param Pcie3CapOffset offset for Secondary PCIe Ext Capability.

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid. 

**/
EFI_STATUS Pcie3AllocateInitPcie3Data(PCI_DEV_INFO *Device, UINT32 Pcie3CapOffset){
    return EFI_SUCCESS;
}

/**
    Function detects if Device PCIe v3.x and up compatible

    @param Device Pointer to device private data structure

    @retval TRUE if Compatible FALSE otherwise.

**/
BOOLEAN Pcie3CheckPcie3Compatible(PCI_DEV_INFO	*Device){
	return FALSE;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

