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

/** @file AmiPciExpressGen4Lib.h
   
    Library Class for AMI Pci Express GEN3 Support Functions.

**/

#ifndef _AMI_PCI_EXPRESS_GEN_4_LIB_H_
#define _AMI_PCI_EXPRESS_GEN_4_LIB_H_

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Efi.h>
#include <IndustryStandard/PciExpress40.h>
#include <Pci.h>
#include <PciE.h>
#include <PciE21.h>
#include <PciE30.h>
#include <PciBus.h>
#include <AmiLib.h>
#include <AcpiRes.h>
#include <Protocol/PciIo.h>

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//Variable, Prototype, and External Declarations
//-------------------------------------------------------------------------


/**
     

    @param Device Pointer to device private data structure
    @param Pcie4LnkStatus Pointer to PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_STATUS structure

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid. 

**/
EFI_STATUS Pcie4GetEqualizationStatus(PCI_DEV_INFO *Device, PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_STATUS *Pcie4LnkStatus);

/**
    This function will Update Speed encoding and 
    display content of LNK_CAP2_REG.
    
    @param Device Pointer to PCI Device Private Data structure. 

    @retval VOID  

**/
VOID Pcie4InitDisplayPcie4Data(PCI_DEV_INFO *Device);




/**
    This function will Allocate the memory for Pcie4
    and Getting Content of LaneErrSts and MaxLanesCount
    
    @param Device Pointer to PCI Device Private Data structure.
    @param Pcie3CapOffset offset for Secondary PCIe Ext Capability.

    @retval   EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid. 

**/
EFI_STATUS Pcie4AllocateInitPcie4Data(PCI_DEV_INFO *Device, UINT32 Pcie3CapOffset);



/**
    Function detects if Device PCIe v4.x and up compatible

    @param Device Pointer to device private data structure

    @retval TRUE if Compatible FALSE otherwise.

**/
BOOLEAN Pcie4CheckPcie4Compatible(PCI_DEV_INFO	*Device);


#endif //_AMI_PCI_EXPRESS_GEN_4_LIB_H_
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

