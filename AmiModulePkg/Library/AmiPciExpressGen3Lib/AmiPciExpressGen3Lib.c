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

/** @file AmiPciExpressGen3Lib.c
    Library Class for AMI Pci Express GEN 3 Support Functions.


**/

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <AmiLib.h>
#include <AcpiRes.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/AmiPciBusLib.h>
#include <Library/AmiPciExpressGen4Lib.h>

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
    Function detects if Device PCIe v3.x and up compatible

    @param Device Pointer to device private data structure

    @retval TRUE if Compatible FALSE otherwise.

**/
BOOLEAN Pcie3CheckPcie3Compatible(PCI_DEV_INFO  *Device){
    
    if( Device->PciExpress!=NULL &&
        Device->PciExpress->Pcie2!=NULL &&
        Device->PciExpress->Pcie3!=NULL &&
        Device->DevSetup.Pcie1Disable == 0 &&
        Device->DevSetup.Pcie2Disable == 0
    )return TRUE;
    else return FALSE;
}



/**
    Get Equalization Status and configure accordingly Equalization bit and
	Equalization Request Interrupt

    @param Device Pointer to device private data structure
    @param LnkRetrain TRUE/FALSE 
    @param LinkSpeed check LNK SPEED VECTOR.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid. 

**/
EFI_STATUS Pcie3EqualizeLink(PCI_DEV_INFO *Device, BOOLEAN *LnkRetrain, UINT8 LinkSpeed){
    EFI_STATUS          Status=EFI_SUCCESS;
	PCI_CFG_ADDR		addr;
    PCIE_LNK_CNT3_REG   LnkCnt3;
    PCIE_LNK_STA2_REG   LnkSta2;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_STATUS   EquStatus;
//-----------------------------

    //If device don't have GEN 3 Cap Hdr, or link is operating on less then 8.0 GT/s speed - just exit...
    if(!Pcie3CheckPcie3Compatible(Device)) return EFI_SUCCESS;
    
    //If device don't have GEN 4 Cap Hdr, or link is operating on less then 16.0 GT/s speed - just exit...
    if ((LinkSpeed == PCIE_LINK_SPEED_16G) && !Pcie4CheckPcie4Compatible(Device))  return EFI_SUCCESS;

    //Get LNK_STA2 register
	addr.ADDR=Device->Address.ADDR;
    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_STA2_OFFSET;
    Status=PciCfg16(Device->RbIo, addr, FALSE,&LnkSta2.LNK_STA2);
	ASSERT_EFI_ERROR(Status);

    //Display Content of the LNK_STA2 register.
    DEBUG((DEBUG_VERBOSE," LNK_STA2-> [DrsMsg=%X|DsCpPr=%X|Retim=%X|Twotim=%X|Eq8Req=%X|Eq8P3=%X|EqP2=%X|Eq8P1=%X|Eq8Ok=%X|DeEmp=%X]\n",
        LnkSta2.DrsMsgReceived,
        LnkSta2.DsCompPresence,    
        LnkSta2.RetimerPresDet,
        LnkSta2.TwotimerPresDet,
        LnkSta2.EqRequest,
        LnkSta2.EqPhase3Ok,
        LnkSta2.EqPhase2Ok,
        LnkSta2.EqPhase1Ok,
        LnkSta2.EqComplete,
        LnkSta2.SelDeEmphasis));

    //now with Spec. v3.1-4.0 we have ability to detect presence of the device at the other side of the link...
    //if(Device->PciExpress->Pcie2->)
    
    //Get Content of the PCIE4_EQUALIZATION_STA_REG register.
    Status=Pcie4GetEqualizationStatus(Device,&EquStatus);
    ASSERT_EFI_ERROR(Status);
    
    //Check if equalization was requested or we are about to enter lLNK training session...
    if( LnkSta2.EqRequest || EquStatus.Bits.LinkEqualizationRequest ||*LnkRetrain ){
        //read Lnk Control 3 register in Sec PCIe Ext Cap Header.
        addr.Addr.ExtendedRegister=Device->PciExpress->Pcie3->SecPcieCapOffs+PCIE_LNK_CNT3_OFFSET;
        Status=PciCfg32(Device->RbIo, addr, FALSE,&LnkCnt3.LNK_CNT3);
        ASSERT_EFI_ERROR(Status);

        //Set Perform Equalization bit and disable Equalization Request Interrupt, just in case.
        if((LinkSpeed==PCIE_LINK_SPEED_80G && LnkSta2.EqRequest)||
           (LinkSpeed==PCIE_LINK_SPEED_16G && EquStatus.Bits.LinkEqualizationRequest)||
           *LnkRetrain) 
        {
            DEBUG((DEBUG_VERBOSE," PciE3: Equalization for Device @ [B%X|D%X|F%X] for LinkSpeed %X; LnkRetrain=%X Before\n",
                addr.Addr.Bus,addr.Addr.Device,addr.Addr.Function, LinkSpeed, *LnkRetrain));
            LnkCnt3.LnkEqReqIntEn=0;
            LnkCnt3.PerformEqualiz=1;
            //Write it back into LNK_CNT3 register.
            Status=PciCfg32(Device->RbIo, addr, TRUE, &LnkCnt3.LNK_CNT3);
            ASSERT_EFI_ERROR(Status);
    
            *LnkRetrain=TRUE;
            //Display Content of the LNK_CNT3 register.
            DEBUG((DEBUG_VERBOSE," LNK_CNT3-> [R=%X| LoSpkOsSpeedEn=%X | R=%X| LnkEqIntEn=%X|DoEq=%X] for LinkSpeed %X; LnkRetrain=%X After\n",
                LnkCnt3.Reserved2,    
                LnkCnt3.LoSpkOsSpeedEn,    
                LnkCnt3.Reserved,
                LnkCnt3.LnkEqReqIntEn,
                LnkCnt3.PerformEqualiz,
                LinkSpeed,
                *LnkRetrain));
        } else {
            DEBUG((DEBUG_VERBOSE," PciE3: NO Equalization needed for Device @ [B%X|D%X|F%X] LinkSpeed %X; LnkRetrain=%X \n",
                        addr.Addr.Bus,addr.Addr.Device,addr.Addr.Function, LinkSpeed, *LnkRetrain));
        }
    }
    return Status;
}


/**
    This function will Get & display Content of the 
    LNK_STA2 register and Check equalization Status.
    
    @param Device Pointer to PCI Device Private Data structure.
    @param LinkSpeed check LNK SPEED VECTOR.

    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid. 

**/
EFI_STATUS Pcie3GetEqualizationStatus(PCI_DEV_INFO *Device, UINT8 LinkSpeed){
    PCI_CFG_ADDR		addr;
    PCIE_LNK_STA2_REG   LnkSta2;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_16_0_STATUS   EquStatus;
    EFI_STATUS          Status=EFI_SUCCESS;
    
//-----------------------------

    //If device don't have GEN 3 Cap Hdr, or link is operating on less then 8.0 GT/s speed - just exit...
    if(!Pcie3CheckPcie3Compatible(Device)) return EFI_SUCCESS;

    //If device don't have GEN 4 Cap Hdr, or link is operating on less then 16.0 GT/s speed - just exit...
    if ((LinkSpeed == PCIE_LINK_SPEED_16G) && !Pcie4CheckPcie4Compatible(Device))  return EFI_SUCCESS;
        
    //Get LNK_STA2 register
	addr.ADDR=Device->Address.ADDR;
    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_STA2_OFFSET;
    Status=PciCfg16(Device->RbIo, addr, FALSE,&LnkSta2.LNK_STA2);
	ASSERT_EFI_ERROR(Status);

    //Display Content of the LNK_STA2 register.
    DEBUG((DEBUG_VERBOSE," LNK_STA2-> [DRSMsg=%X|DsCpPr=%X|Retim=%X|Twotim=%X|Eq8Req=%X|Eq8P3=%X|Eq8P2=%X|Eq8P1=%X|Eq8Ok=%X|CurrDeEmp=%X]\n",
        LnkSta2.DrsMsgReceived,
        LnkSta2.DsCompPresence,
        LnkSta2.RetimerPresDet,
        LnkSta2.TwotimerPresDet,
        LnkSta2.EqRequest,
        LnkSta2.EqPhase3Ok,
        LnkSta2.EqPhase2Ok,
        LnkSta2.EqPhase1Ok,
        LnkSta2.EqComplete,
        LnkSta2.SelDeEmphasis));
    DEBUG((DEBUG_VERBOSE,"Pcie3: LinkSpped set %X\n",LinkSpeed));

    //Get Content of the PCIE4_EQUALIZATION_STA_REG register.
    Status=Pcie4GetEqualizationStatus(Device,&EquStatus);
    ASSERT_EFI_ERROR(Status);
    
    //Display Content of the PCIE4_EQUALIZATION_STA_REG register.
    DEBUG((DEBUG_VERBOSE," Pcie4 : EQUALIZATION_STA-> [EqCompl=%X|EqP1=%X|EqP2=%X|EqP3=%X|EqReq=%X\n",
            EquStatus.Bits.EqualizationComplete,
            EquStatus.Bits.EqualizationPhase1Success,
            EquStatus.Bits.EqualizationPhase2Success,
            EquStatus.Bits.EqualizationPhase3Success,
            EquStatus.Bits.LinkEqualizationRequest
            ));
        
    //Check equalization results...
    switch (LinkSpeed){
    case PCIE_LINK_SPEED_80G : 
        if(LnkSta2.EqComplete && LnkSta2.EqPhase3Ok) return EFI_SUCCESS;
        //Check if any of the equalization phases were completed...
        if(LnkSta2.EqPhase1Ok || LnkSta2.EqPhase2Ok || LnkSta2.EqPhase3Ok) return EFI_NOT_AVAILABLE_YET;
        else     return EFI_DEVICE_ERROR;
        break;
    case PCIE_LINK_SPEED_16G :
        if (EquStatus.Bits.EqualizationComplete && EquStatus.Bits.EqualizationPhase3Success &&
           LnkSta2.EqComplete && LnkSta2.EqPhase3Ok) return EFI_SUCCESS;
        //Check if any of the equalization phases were completed...
        if(EquStatus.Bits.EqualizationPhase1Success || EquStatus.Bits.EqualizationPhase2Success || EquStatus.Bits.EqualizationPhase3Success) return EFI_NOT_AVAILABLE_YET;
        else     return EFI_DEVICE_ERROR;
        break;
    }
    
    return EFI_SUCCESS;
}

/**
    This function will Update Speed encoding and 
    display content of LNK_CAP2_REG.
    
    @param Device Pointer to PCI Device Private Data structure. 

    @retval VOID  

**/
VOID Pcie3InitDisplayPcie3Data(PCI_DEV_INFO *Device){
    //Do some additional checks if device has GEN 3 Secondary PCIe Cap Header.
    //At that point if we have discovered Secondary PCIe Cap HDR Device->PciExpress->Pcie3 must be initialized.
    if(!Pcie3CheckPcie3Compatible(Device)){
        //Update Speed encoding it is defined differently for devices Supporting V3.0 spec.
        Device->PciExpress->Pcie3->MaxLanesCount=Device->PciExpress->LinkCap.MaxLnkWidth;

        //Display content of LNK_CAP2_REG
        DEBUG((DEBUG_VERBOSE,"PCIe3 -> Device is PCIe v3.0 Compliant!!! LNK_CAP2 present!!! \n LNK_CAP2-> [R2=%X|CrossL=%X|SuppLnkSpeedVect=%X|R1=%X]; LANE_ERR_STA->[%X]; MaxLanes=%X\n",
            Device->PciExpress->Pcie2->LinkCap2.Reserved2,
            Device->PciExpress->Pcie2->LinkCap2.CrossLnk,
            Device->PciExpress->Pcie2->LinkCap2.SuppLnkSpeeds,
            Device->PciExpress->Pcie2->LinkCap2.Reserved1,
            Device->PciExpress->Pcie3->LaneErrSts,
            Device->PciExpress->Pcie3->MaxLanesCount));
    }
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
    PCI_CFG_ADDR	addr;
    EFI_STATUS      Status;
//---------------------------
    DEBUG((DEBUG_VERBOSE,"PciE3: Found SEC PCIe Ext Cap Header @ offset 0x%X\n", Pcie3CapOffset));

    //Allocate Memory
    Device->PciExpress->Pcie3=MallocZ(sizeof(PCIE3_DATA));
    if(Device->PciExpress->Pcie3==NULL) {
        ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
        return EFI_OUT_OF_RESOURCES;
    }

    Device->PciExpress->Pcie3->SecPcieCapOffs=Pcie3CapOffset;

    //Get Content of LaneErrSts and MaxLanesCount
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=Pcie3CapOffset+PCIE_LANE_ERR_STA_OFFSET;

    Status=PciCfg32(Device->RbIo, addr, FALSE,&Device->PciExpress->Pcie3->LaneErrSts);

Pcie3InitDisplayPcie3Data(Device);

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

