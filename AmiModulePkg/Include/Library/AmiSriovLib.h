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

/** @file AmiSriovLib.h
    Library Class for AMI SIO Driver

**/

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <AmiLib.h>
#include <AcpiRes.h>
#include <PciBus.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------


/**
    This function will collect information about SRIOV PCIE Device
    and initialize it based on information collected.

        
    @param Device Pointer to PCI Device Private Data structure.
    @param MaxBusFound maximum number of pci Bus.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS SriovProbeDevice(PCI_DEV_INFO *Device, UINT8 *MaxBusFound);



/**
    

    @param Device Pointer to PCI Device Private Data structure.
    @param SriovCapOffset Offset for PCIE SRIOV DATA.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS SriovAllocateSriovData(PCI_DEV_INFO *Device, UINT32 SriovCapOffset);

/**
    This function will check Sriov compatibility.

    @param Device Pointer to PCI Device Private Data structure.

    @retval BOOLEAN
        TRUE or FALSE based on value passed.

**/
BOOLEAN SriovCheckSriovCompatible(PCI_DEV_INFO	*Device);


/**

    @param Device Pointer to PCI Device Private Data structure.
    @param Db Pointer to the EFI Database Engine DATABASE.
    @param ResType Resource type we are doing optimization for.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS	SriovDbAddVirtualBar(PCI_DEV_INFO *Device, DBE_DATABASE *Db, MRES_TYPE ResType);


/**
    This function will check the type of the bar.

    @param Device Pointer to PCI Device Private Data structure.
    @param BarType Indicating what to check - IO or Memory.

    @retval BOOLEAN
        TRUE or FALSE based on value passed.

**/
BOOLEAN SriovCheckBarType(PCI_DEV_INFO *Device, PCI_BAR_TYPE BarType);

/**
    This function will convert Resource if need.

    @param Device Pointer to PCI Device Private Data structure.
    @param NarrowType type of pci device.
    @param WideType type of pci device.
    @param ConvType Type of Conversion to be performed.
    @param NeedToConvert TRUE/FALSE
    @param CombineMemPmem TRUE/FALSE

    @retval  Nothing.

**/
VOID SriovConvertResources( PCI_DEV_INFO *Device, PCI_BAR_TYPE NarrowType, PCI_BAR_TYPE WideType,
                            RES_CONV_TYPE ConvType, BOOLEAN NeedToConvert, BOOLEAN CombineMemPmem);

/**
    Set/Reset ARI Capable Hierarchy bit in SR-IOV Control register 

    @param Device Pointer to PCI Device Private Data structure.
    @param set    TRUE/FALSE

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus..

**/
EFI_STATUS  SriovAriCapableHierarchy(PCI_DEV_INFO *Device, BOOLEAN Set);

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

