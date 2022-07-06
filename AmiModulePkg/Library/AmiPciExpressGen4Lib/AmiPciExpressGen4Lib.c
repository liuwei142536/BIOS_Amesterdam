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

/** @file AmiPciExpressGen4Lib.c
    Library Class for AMI Pci Express GEN 4 Support Functions.


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
    Function detects if Device PCIe v4.x and up compatible

    @param Device Pointer to device private data structure

    @retval TRUE if Compatible FALSE otherwise.

**/
BOOLEAN Pcie4CheckPcie4Compatible(PCI_DEV_INFO  *Device){
    
    if( Device->PciExpress!=NULL &&
        Device->PciExpress->Pcie2!=NULL &&
        Device->PciExpress->Pcie3!=NULL &&
        Device->PciExpress->Pcie4!=NULL &&
        Device->DevSetup.Pcie1Disable == 0 &&
        Device->DevSetup.Pcie2Disable == 0
    )return TRUE;
    else return FALSE;
}



/**
     This function will displays the Content of the PCIE4_EQUALIZATION_STA_REG register.

    @param Device Pointer to device private data structure
    @param EquStatus Pointer to PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_STATUS structure

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid. 

**/
EFI_STATUS Pcie4GetEqualizationStatus(PCI_DEV_INFO *Device, PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_STATUS *EquStatus){
    EFI_STATUS          Status=EFI_SUCCESS;
	PCI_CFG_ADDR		addr;
//-----------------------------

    //If device don't have GEN 4 Cap Hdr, or link is operating on less then 16.0 GT/s speed - just exit...
    if(!Pcie4CheckPcie4Compatible(Device)) return EFI_SUCCESS;

    //Get PCIE_16_0_STA_OFFSET register to check the Equalization status
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=Device->PciExpress->Pcie4->PhyPcieCapOffs+PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_STATUS_OFFSET;
    Status=PciCfg32(Device->RbIo, addr, FALSE,&(EquStatus->Uint32));
    ASSERT_EFI_ERROR(Status);

    //Display Content of the PCIE4_EQUALIZATION_STA_REG register.
    DEBUG((DEBUG_VERBOSE," Pcie4 : EQUALIZATION_STA-> [EqCompl=%X|EqP1=%X|EqP2=%X|EqP3=%X|EqReq=%X\n",
        EquStatus->Bits.EqualizationComplete,
        EquStatus->Bits.EqualizationPhase1Success,
        EquStatus->Bits.EqualizationPhase2Success,
        EquStatus->Bits.EqualizationPhase3Success,
        EquStatus->Bits.LinkEqualizationRequest
        ));

    return Status;
}


/**
    This function will Update Speed encoding and 
    display content of LNK_CAP2_REG.
    
    @param Device Pointer to PCI Device Private Data structure. 

    @retval VOID  

**/
VOID Pcie4InitDisplayPcie4Data(PCI_DEV_INFO *Device){
    //Do some additional checks if device has GEN 4 Secondary PCIe Cap Header.
    //At that point if we have discovered Physical Layer 16.0 PCIe Cap HDR Device->PciExpress->Pcie4 must be initialized.
    if(!Pcie4CheckPcie4Compatible(Device)){
        //Update Speed encoding it is defined differently for devices Supporting V4.0 spec.
        Device->PciExpress->Pcie4->MaxLanesCount=Device->PciExpress->LinkCap.MaxLnkWidth;

        //Display content of LNK_CAP2_REG
        DEBUG((DEBUG_VERBOSE,"PCIe4 -> Device is PCIe v4.0 Compliant!!! LNK_CAP2 present!!! \n LNK_CAP2-> [R2=%X|CrossL=%X|SuppLnkSpeedVect=%X|R1=%X]|[RetimerPresDet=%x][TwotimerPresDet=%x]; LocaLDataParityStatus->[%X] FirstReTimerDataParityStatus->[%X] SecondReTimerDataParityStatus->[%X]; MaxLanes=%X\n",
            Device->PciExpress->Pcie2->LinkCap2.Reserved2,
            Device->PciExpress->Pcie2->LinkCap2.CrossLnk,
            Device->PciExpress->Pcie2->LinkCap2.SuppLnkSpeeds,
            Device->PciExpress->Pcie2->LinkCap2.Reserved1,
            Device->PciExpress->Pcie2->LinkCap2.RetimerPresDet,
            Device->PciExpress->Pcie2->LinkCap2.TwotimerPresDet,
            Device->PciExpress->Pcie4->LocaLDataParityStatus,
            Device->PciExpress->Pcie4->FirstReTimerDataParityStatus,
            Device->PciExpress->Pcie4->SecondReTimerDataParityStatus,
            Device->PciExpress->Pcie4->MaxLanesCount));
    }
}

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
EFI_STATUS Pcie4AllocateInitPcie4Data(PCI_DEV_INFO *Device, UINT32 Pcie4CapOffset){
    PCI_CFG_ADDR	addr;
    EFI_STATUS      Status;
//---------------------------
    DEBUG((DEBUG_VERBOSE,"PciE4: Found Physical Layer 16.0 PCIe Ext Cap Header @ offset 0x%X\n", Pcie4CapOffset));

    //Allocate Memory
    Device->PciExpress->Pcie4=MallocZ(sizeof(PCIE4_DATA));
    if(Device->PciExpress->Pcie4==NULL) {
        ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
        return EFI_OUT_OF_RESOURCES;
    }

    Device->PciExpress->Pcie4->PhyPcieCapOffs=Pcie4CapOffset;

    //Get Content of LocaLDataParityStatus,FirstReTimerDataParityStatus,SecondReTimerDataParityStatus and MaxLanesCount
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=Pcie4CapOffset+PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_LOCAL_DATA_PARITY_STATUS_OFFSET;

    Status=PciCfg32(Device->RbIo, addr, FALSE,&Device->PciExpress->Pcie4->LocaLDataParityStatus);
    
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=Pcie4CapOffset+PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_FIRST_RETIMER_DATA_PARITY_STATUS_OFFSET;

    Status=PciCfg32(Device->RbIo, addr, FALSE,&Device->PciExpress->Pcie4->FirstReTimerDataParityStatus);
        
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=Pcie4CapOffset+PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_SECOND_RETIMER_DATA_PARITY_STATUS_OFFSET;

    Status=PciCfg32(Device->RbIo, addr, FALSE,&Device->PciExpress->Pcie4->SecondReTimerDataParityStatus);

    Pcie4InitDisplayPcie4Data(Device);

    return Status;

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

