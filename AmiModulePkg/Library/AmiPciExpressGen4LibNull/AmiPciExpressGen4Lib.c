//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiPciExpressGen4Lib.c
    Library Class for AMI Pci Express GEN 4 Support Functions. Dummy Instance


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
    This function will Get & display Content of the 
    LNK_STA2 register and Check equalization Status.
    
    @param Device Pointer to PCI Device Private Data structure.
    @param LinkSpeed check LNK SPEED VECTOR.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid. 

**/
EFI_STATUS Pcie4GetEqualizationStatus(PCI_DEV_INFO *Device, UINT8 LinkSpeed){

    return EFI_SUCCESS;
}

/**
    This function will Update Speed encoding and 
    display content of LNK_CAP2_REG.
    
    @param Device Pointer to PCI Device Private Data structure. 

    @retval VOID  

**/
VOID Pcie4InitDisplayPcie4Data(PCI_DEV_INFO *Device){
	return;
}

/**
    This function will Allocate the memory for Pcie4
    and Getting Content of LaneErrSts and MaxLanesCount
    
    @param Device Pointer to PCI Device Private Data structure.
    @param Pcie4CapOffset offset for Secondary PCIe Ext Capability.

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid. 

**/
EFI_STATUS Pcie4AllocateInitPcie4Data(PCI_DEV_INFO *Device, UINT32 Pcie4CapOffset){
    return EFI_SUCCESS;
}

/**
    Function detects if Device PCIe v4.x and up compatible

    @param Device Pointer to device private data structure

    @retval TRUE if Compatible FALSE otherwise.

**/
BOOLEAN Pcie4CheckPcie4Compatible(PCI_DEV_INFO	*Device){
	return FALSE;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

