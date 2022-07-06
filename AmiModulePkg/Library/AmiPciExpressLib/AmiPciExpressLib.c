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

/** @file AmiPciExpressLib.c
    Library Class for AMI Pci Express Support Functions.


**/

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
#include <Protocol/S3SaveState.h>
#include <Library/AmiPciBusLib.h>
#include <Library/AmiSriovLib.h>
#include <Library/AmiPciExpressGen2Lib.h>
#include <Library/AmiPciExpressGen3Lib.h>
#include <Library/AmiPciExpressGen4Lib.h>
#include <Library/PcdLib.h>

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
T_ITEM_LIST  gPcieEpList = {0,0,NULL};

PCI_COMMON_SETUP_DATA    gPciCommonSetupData;

UINT64  *mPciExBaseArrayPcd=NULL; 

/**
    Returns PciExpress 

    @param Device pointer to device internal data.
    @param Register Offset of PCI Configuration space.

    @retval UINT64 Returns PCIe Config Space MMIO Address of the requested Register of the Device.

**/
UINT64 PcieGetPciExpressCfgMmioAddrress(PCI_DEV_INFO *Device, UINT16 Register){
    EFI_PCI_CONFIGURATION_ADDRESS  *Address=&Device->Address.Addr;
    UINT64  PciAddress = ((Address->Bus<<20)+(Address->Device<<15)+(Address->Function<<12)+Register);
//-------------------------------------------
    if(mPciExBaseArrayPcd==NULL){
        mPciExBaseArrayPcd=PcdGetPtr(AmiPcdPcieBaseAddressBuffer);
    }
    
    // If the Segment 0 Entry value is 0, Get Pcie Base as in Single Segment and update
    if(mPciExBaseArrayPcd[0] == 0) {
        mPciExBaseArrayPcd[0] =  PcdGet64(PcdPciExpressBaseAddress);
        PciAddress+=mPciExBaseArrayPcd[0];
    } else {
        // MultiSegment with Valid offset.Fetch value
        PciAddress+=mPciExBaseArrayPcd[Device->RbIo->SegmentNumber];
    }

    return PciAddress;
}

/*
EFI_STATUS EFIAPI AmiPciExpressLibConstructor(IN EFI_HANDLE  ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable)
{
    EFI_STATUS              Status=EFI_SUCCESS;
//-------------------------------------------------
    InitAmiLib(ImageHandle, SystemTable);

    return Status;;
}
*/


/**
    Checks if PCIe features supported by the Device
    
    @param Device pointer to device internal data. 

    @retval BOOLEAN
        TRUE    Device is Function 0 of Multifunctional device.
        FALSE   Device is not Function 0 of Multifunctional device.

**/
BOOLEAN PcieCheckPcieCompatible(PCI_DEV_INFO    *Device){
    if(Device->DevSetup.Pcie1Disable==FALSE && Device->PciExpress!=NULL) return TRUE;
    else return FALSE;
}


/**

    @param Brg Pointer to PCI Device Private Data structure.
    @param DevArrd Device Address on PCI Bus.
    @param VenDevId Pointer Data Buffer.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieDoubleCheckCard(PCI_DEV_INFO *Brg, PCI_CFG_ADDR *DevAddr, UINT32 *VenDevId){
    EFI_STATUS          Status;
    PCI_CFG_ADDR        addr;
    PCIE_SLT_STA_REG    slts;
    UINTN               RetryCount;
    UINTN               TimeOut;
//    PCIE_LNK_STA_REG    lnks;
//    PCIE_DEV_STA_REG    devs;
//------------------------
    RetryCount=Brg->PciExpress->Pcie1Setup.LnkTrRetry;
    TimeOut=Brg->PciExpress->Pcie1Setup.LnkTrTimeout;

    //Read PCIe.DEV_STA_REG;
    addr.ADDR=Brg->Address.ADDR;
/*
    addr.Addr.Register=Brg->PciExpress->PcieOffs+PCIE_DEV_STA_OFFSET;
    Status=PciCfg16(Brg->RbIo, addr, FALSE, &devs.DEV_STA);
    ASSERT_EFI_ERROR(Status);

    //Read PCIe.LNK_STA_REG;
    addr.Addr.Register=Brg->PciExpress->PcieOffs+PCIE_LNK_STA_OFFSET;
    Status=PciCfg16(Brg->RbIo, addr, FALSE, &lnks.LNK_STA);
    ASSERT_EFI_ERROR(Status);
*/
    //Read PCIe.SLT_STA_REG;
    addr.Addr.Register=Brg->PciExpress->PcieOffs+PCIE_SLT_STA_OFFSET;
    Status=PciCfg16(Brg->RbIo, addr, FALSE, &slts.SLT_STA);
    ASSERT_EFI_ERROR(Status);

/*
    TRACE((-1, "\n!!!!!!DEV_STA[ 0x%X ].TrasactPend(5)=%d;\n",devs.DEV_STA, devs.TrasactPend));
    TRACE((-1, "!!!!!!LNK_STA[0x%X].LnkTraining(11)=%d;\n",lnks.LNK_STA, lnks.LnkTraining));
    TRACE((-1, "!!!!!!SLT_STA[%X].CardPresent(6)=%d;\n",slts.SLT_STA, slts.CardPresent));
    TRACE((-1, "!!!!!!for the BRIDGE @ [B%X|D%X|F%X];\n",Brg->Address.Addr.Bus,Brg->Address.Addr.Device, Brg->Address.Addr.Function));
    TRACE((-1, "!!!!!!VID/DID=%X of [B%X|D0|F0]\n",*VenDevId, addr.Addr.Bus));
*/

    //if we are here that means card did not come up first time...
    //so try to read it one more time, before taking some invasive actions.
    addr.ADDR = DevAddr->ADDR;
    addr.Addr.Register=0;

    //SOMETHING present there, but need some time to completely come up
    if(slts.CardPresent != 0  || slts.PresenceDet !=0 ){
        UINTN   i;
    //---------
        for(i=0; i<=RetryCount; i++){

            Status=PciCfg32(Brg->RbIo, addr, FALSE, VenDevId);
            ASSERT_EFI_ERROR(Status);
            if( (*VenDevId) != 0xFFFFFFFF ) break;

            //Use same setup value for retry timeut and number
            pBS->Stall(TimeOut);
        }
//  TRACE((-1, "!!!!!!VID/DID=%X after %d retry (timeout=%d) \n",*VenDevId, i+1, gPciSetupData->LnkTrTimeout*100));
    }

    //If nothing worked just exit...
    return Status;
}

/**
    This function will create S3 Resume Boot Script
    for Device's Virtual Channels feature initialization if device
    supports Virtual Channels.

        
    @param Brg Pointer to PCI Device Private Data structure.
    @param S3SaveStateProtocol Pointer to EFI_S3_SAVE_STATE_PROTOCOL instance
    @param RecordScript TRUE/FALSE

    @retval VOID

**/
VOID PcieRecordBootScript(PCI_DEV_INFO *Brg, EFI_S3_SAVE_STATE_PROTOCOL *S3SaveStateProtocol, BOOLEAN RecordScript){
    PCI_DEV_INFO    *dev;
    PCI_BRG_EXT     *ext=(PCI_BRG_EXT*)(Brg+1);
    UINTN           i,c;
    PCI_CFG_ADDR    addr;
    EFI_STATUS      Status;
    UINT64          v=0;
//------------------------------
    //If we were not found gS3SaveState - nothing to save here
    if(S3SaveStateProtocol==NULL) return;
    
    for(i=0; i<ext->ChildCount; i++){
        dev=ext->ChildList[i];
        if(!dev->PciExpress) continue;
        //It could be the case when we have PCI 2 PCI bridges chain and behind
        //them we will found PCI 2 PCIE bridge. So starting from ROOT we should not
        //skip bridges without PCIE Capabilities but must go inside and check!

        addr.ADDR=dev->Address.ADDR;
        addr.Addr.ExtendedRegister=0;
        //if device is PciExpress check if it has VC
        if(dev->Type==tPci2PciBrg || dev->Type==tPci2CrdBrg){
            //record Base bus Subordinate bus Secondary bus registers programming
            //to have device visible on the bus
            addr.Addr.Register=PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET;

            Status=PciCfg32(dev->RbIo,addr,FALSE,(UINT32*)&v);
            BOOT_SCRIPT_S3_PCI_CONFIG_WRITE_MACRO(
                S3SaveStateProtocol,        //This
                EfiBootScriptWidthUint32,   //Width
                addr.ADDR,1,&v);            //Address, Data
            //call the same function recursively
            PcieRecordBootScript(dev,S3SaveStateProtocol, RecordScript);

        }

        if(dev->PciExpress->VcData){
        //if(dev->PciExpress!=NULL && dev->PciExpress->VcData!=NULL){
            for(c=0; c<dev->PciExpress->VcData->VcCount; c++){
                VC_RES_DATA *vcrd=dev->PciExpress->VcData->VcResData[c];
            //---------------------------------------
                addr.Addr.ExtendedRegister=vcrd->VcResOffset+4;
                Status=PciCfg32(dev->RbIo,addr,FALSE,(UINT32*)&v);

                BOOT_SCRIPT_S3_MEM_WRITE_MACRO(
                    S3SaveStateProtocol,        //This
                    EfiBootScriptWidthUint32,   //Width
                    (VOID*)PcieGetPciExpressCfgMmioAddrress(dev, addr.Addr.ExtendedRegister),
                    1,&v);                      //Data
            }
        }

        //create a setup question to restore PCIE regs for Win XP only.
        if (RecordScript){
            addr.ADDR=dev->Address.ADDR;
            addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_DEV_CNT_OFFSET;
            Status=PciCfg16(dev->RbIo,addr,FALSE,(UINT16*)&v);
            ASSERT_EFI_ERROR(Status);
            
            BOOT_SCRIPT_S3_PCI_CONFIG_WRITE_MACRO(
                       S3SaveStateProtocol,    //This
                       EfiBootScriptWidthUint16, //Width
                       addr.ADDR,
                       1,&v);      //Data

            addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
            Status=PciCfg16(dev->RbIo,addr,FALSE,(UINT16*)&v);
            ASSERT_EFI_ERROR(Status);

               BOOT_SCRIPT_S3_PCI_CONFIG_WRITE_MACRO(
                       S3SaveStateProtocol,    //This
                       EfiBootScriptWidthUint16, //Width
                       addr.ADDR,
                       1,&v);      //Data

            if(dev->PciExpress->PcieCap.SlotImpl){
                addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_SLT_CNT_OFFSET;
                Status=PciCfg16(dev->RbIo,addr,FALSE,(UINT16*)&v);
                ASSERT_EFI_ERROR(Status);

                   BOOT_SCRIPT_S3_PCI_CONFIG_WRITE_MACRO(
                           S3SaveStateProtocol,    //This
                           EfiBootScriptWidthUint16, //Width
                           addr.ADDR,
                           1,&v);      //Data
            }

            if(Pcie2CheckPcie2Compatible(dev)){
                addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_DEV_CNT2_OFFSET;
                Status=PciCfg16(dev->RbIo,addr,FALSE,(UINT16*)&v);
                ASSERT_EFI_ERROR(Status);

                   BOOT_SCRIPT_S3_PCI_CONFIG_WRITE_MACRO(
                           S3SaveStateProtocol,    //This
                           EfiBootScriptWidthUint16, //Width
                           addr.ADDR,
                           1,&v);      //Data

                   addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_LNK_CNT2_OFFSET;
                   Status=PciCfg16(dev->RbIo,addr,FALSE,(UINT16*)&v);
                ASSERT_EFI_ERROR(Status);

                BOOT_SCRIPT_S3_PCI_CONFIG_WRITE_MACRO(
                           S3SaveStateProtocol,    //This
                           EfiBootScriptWidthUint16, //Width
                           addr.ADDR,
                           1,&v);      //Data
            }
        } //if(RecordScript)
    }//for
}

/**

    @param Device Pointer to PCI Device Private Data structure.
    @param MaxPayload Reduced Max Payload Value.
    @param MaxReadReq Reduced Read Request Value.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieUpdateMplMrr(PCI_DEV_INFO *Device, UINT16 MaxPayload, UINT16 MaxReadReq){
    EFI_STATUS       Status=EFI_SUCCESS;
    PCI_CFG_ADDR     addr;
    PCIE_DEV_CNT_REG devc;
//----------------------------    
    //Select Device Control Register
    addr.ADDR= Device->Address.ADDR;
    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_DEV_CNT_OFFSET;
    //Read what we have there
    Status=PciCfg16(Device->RbIo,addr,FALSE,&devc.DEV_CNT);
    ASSERT_EFI_ERROR(Status);
    
    if (MaxPayload != PCI_SETUP_DONT_TOUCH)
    devc.MaxPayload=MaxPayload;
    
    if (MaxReadReq != PCI_SETUP_DONT_TOUCH)
    devc.MaxReadReq=MaxReadReq;
    
    Status=PciCfg16(Device->RbIo,addr,TRUE, &devc.DEV_CNT);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    Device->PciExpress->MaxPayload=devc.MaxPayload;
    return Status;
}


/**
    This function will reprogram Max Payload and Read Request Size of
    other (lesser numbered) functions of upstream port of switch if such ugly configuration 
    detected by PcieCalcMaxPayLoad() function and 

        
    @param Device Pointer to PCI Device Private Data structure.
    @param MaxPayload Reduced Max Payload Value.
    @param MaxReadReq Reduced Read Request Value.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  
  RootPortComplex<=>Switch0...<=>...SwitchN<=>EndPoint
**/
EFI_STATUS PcieCheckMplMrrOfOtherFunc(PCI_DEV_INFO *Device, UINT16 MaxPayload, UINT16 MaxReadReq){
    EFI_STATUS       Status=EFI_SUCCESS;
    PCI_DEV_INFO     *dev=Device;
    BOOLEAN          wrMaxPayload=FALSE;
    BOOLEAN          wrMaxReadReq=FALSE;
    UINTN            i;
//------------    

    if((dev->Type==tPciDevice)&&(dev->PciExpress->DevSet)){
        if((dev->PciExpress->Pcie1Setup.MaxPayload!=PCI_SETUP_DONT_TOUCH)&&
                (dev->PciExpress->MaxPayload >= MaxPayload)) wrMaxPayload=TRUE;
        if(dev->PciExpress->Pcie1Setup.MaxReadRequest!=PCI_SETUP_DONT_TOUCH){
          //Leave CHIPSET INIT value if AUTO selected.
          if(MaxReadReq != PCI_SETUP_AUTO_VALUE) wrMaxReadReq=TRUE;
        }
    
        if(wrMaxReadReq || wrMaxPayload){
            if(MaxReadReq == PCI_SETUP_AUTO_VALUE) MaxReadReq = PCIE_MAXPL_512B;
            if (wrMaxReadReq && wrMaxPayload )
            Status=PcieUpdateMplMrr(dev,MaxPayload,MaxReadReq);
            else {
                if (wrMaxReadReq == 1  && wrMaxPayload == 0)
                    Status=PcieUpdateMplMrr(dev,PCI_SETUP_DONT_TOUCH,MaxReadReq);
                if (wrMaxPayload == 1  &&  wrMaxReadReq == 0)
                    Status=PcieUpdateMplMrr(dev,MaxPayload,PCI_SETUP_DONT_TOUCH);
            }
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)) return Status;
        }
    }   

    //now process other functions of this device...
    //This function will be called only for Mulformed Mf Devices. 
    for(i=0; i<Device->FuncCount; i++){
        wrMaxReadReq=FALSE;
        wrMaxPayload = FALSE;
        dev=Device->DevFunc[i];
        
        if((dev->Type==tPciDevice)&&(dev->PciExpress->DevSet)){
            if((dev->PciExpress->Pcie1Setup.MaxPayload!=PCI_SETUP_DONT_TOUCH)&&
                    (dev->PciExpress->MaxPayload >= MaxPayload)) wrMaxPayload=TRUE;
            if(dev->PciExpress->Pcie1Setup.MaxReadRequest!=PCI_SETUP_DONT_TOUCH){
              //Leave CHIPSET INIT value if AUTO selected.
              if(MaxReadReq != PCI_SETUP_AUTO_VALUE) wrMaxReadReq=TRUE;
            }
        
            if(wrMaxReadReq || wrMaxPayload){
                if(MaxReadReq == PCI_SETUP_AUTO_VALUE) MaxReadReq = PCIE_MAXPL_512B;
                if (wrMaxReadReq && wrMaxPayload )
                Status=PcieUpdateMplMrr(dev,MaxPayload,MaxReadReq);
                else {
                    if (wrMaxReadReq == 1  && wrMaxPayload == 0)
                        Status=PcieUpdateMplMrr(dev,PCI_SETUP_DONT_TOUCH,MaxReadReq);
                    if (wrMaxPayload == 1  &&  wrMaxReadReq == 0)
                        Status=PcieUpdateMplMrr(dev,MaxPayload,PCI_SETUP_DONT_TOUCH);
                }
                ASSERT_EFI_ERROR(Status);
                if(EFI_ERROR(Status)) return Status;
            }
        }   
    }
    return Status;
}


/**
    This function will reprogram Max Payload and Read Request Size
    in upstream link, if initialization code founds that one of Nodes down stream
    has lesser Max Pay load size than currently programmed

        
    @param Device Pointer to PCI Device Private Data structure.
    @param MaxPayload Reduced Max Payload Value.
    @param MaxReadReq Reduced Read Request Value.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  
  RootPortComplex<=>Switch0...<=>...SwitchN<=>EndPoint
**/
EFI_STATUS PcieProgramPayloadUp(PCI_DEV_INFO *Device, UINT16 MaxPayload, UINT16 MaxReadReq){
    EFI_STATUS      Status=EFI_SUCCESS;
    PCI_DEV_INFO    *dev=Device;
//-------------------------

    //It has to be PCIE Switch device so it must have PCI2PCI Bridge Header structure.
    if(dev->Type!=tPci2PciBrg) return EFI_DEVICE_ERROR;

    //We are coming here if Payload and MaxReadRequest been set for the device and
    //Device is NOT ENDPOINT Device. Means we hit switch here and it is handling more than
    //one ENDPOINT Devices. So check if MPL and MRRS is Bigger or Equal then passed Payload parameter.
    if(dev->PciExpress->MaxPayload > MaxPayload){
        UINTN               i;
        PCI_BRG_EXT         *ext=(PCI_BRG_EXT*)(dev+1);
        PCI_CFG_ADDR        addr;
        PCIE_DEV_CNT_REG    devc;
    //-------------
        //if condition met: one or some of the upstream device(s) are capable and programmed
        //to generate TLP bigger than current proposed programming stored in "mrr" and "mpl".

           //Select Device Control Register
        addr.ADDR= dev->Address.ADDR;
        addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_DEV_CNT_OFFSET;
        //Read what we have there
        Status=PciCfg16(dev->RbIo,addr,FALSE,&devc.DEV_CNT);
        ASSERT_EFI_ERROR(Status);

           DEBUG((DEBUG_VERBOSE,"PciE: Going UP @ [B%X|D%X|F%X] Updating: MPL %X to %X\n",
            Device->Address.Addr.Bus,Device->Address.Addr.Device, Device->Address.Addr.Function,
            devc.MaxPayload, MaxPayload));

        for(i=0;i<ext->ChildCount; i++){
            dev=(PCI_DEV_INFO*)ext->ChildList[i];
            if(PcieCheckPcieCompatible(dev)){
                //Reprogram only devices we have programmed already.
                if(dev->PciExpress->DevSet){
                    //If we have other switches in upstream link, call this function recursively.
                    if(dev->Type==tPci2PciBrg) {
                        PcieProgramPayloadUp(dev, MaxPayload, MaxReadReq);
                        ASSERT_EFI_ERROR(Status);
                        if(EFI_ERROR(Status)) return Status;
                    }
                    
                    //Select Device Control Register
                    addr.ADDR = dev->Address.ADDR;
                    addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_DEV_CNT_OFFSET;
                    //Read what we have there
                    Status=PciCfg16(dev->RbIo,addr,FALSE,&devc.DEV_CNT);
                    ASSERT_EFI_ERROR(Status);
                    if(EFI_ERROR(Status)) return Status;

                    //Check if Setup says don't touch this option...
                    if(dev->PciExpress->Pcie1Setup.MaxPayload!=PCI_SETUP_DONT_TOUCH){
                        DEBUG((DEBUG_VERBOSE,"PciE: Reprogram Payload @ [B%X|D%X|F%X] Updating: MPL %X to %X\n", 
                        dev->Address.Addr.Bus,dev->Address.Addr.Device, dev->Address.Addr.Function,
                        devc.MaxPayload, MaxPayload));
                        
                        devc.MaxPayload=MaxPayload;
                    } else {
                        dev->PciExpress->MaxPayload=devc.MaxPayload;
                        DEBUG((DEBUG_VERBOSE,"PciE: Leaving   Payload @ [B%X|D%X|F%X] = %X; Setup==DONT_TOUCH\n",
                                dev->PciExpress->MaxPayload));
                    }
                    
                    if(dev->PciExpress->Pcie1Setup.MaxReadRequest!=PCI_SETUP_DONT_TOUCH){
                        //Leave CHIPSET INIT value if AUTO selected.
                        if(MaxReadReq != PCI_SETUP_AUTO_VALUE) devc.MaxReadReq=MaxReadReq;

                    }
                    
                    Status=PciCfg16(dev->RbIo,addr,TRUE, &devc.DEV_CNT);
                    ASSERT_EFI_ERROR(Status);
                    if(EFI_ERROR(Status)) return Status;

                    dev->PciExpress->MaxPayload=devc.MaxPayload;
                }
            } else {
                //Such condition MUST NOT happened, since Parent is UPSTREAM PORT and childs must be a PciE only!
                DEBUG((DEBUG_ERROR, "PciE: ERROR! PciE Data Uninitialized! Child#=%d. TotalChlds=%d.\n", i, ext->ChildCount));
                return EFI_DEVICE_ERROR;
            }
        }
    } else {
           DEBUG((DEBUG_VERBOSE,"PciE: NOT Going UP @ [B%X|D%X|F%X] Device's Link MPL %X <= New MPL %X\n",
            Device->Address.Addr.Bus,Device->Address.Addr.Device, Device->Address.Addr.Function,
            dev->PciExpress->MaxPayload, MaxPayload));
    }

    return Status;
}

/**
    This function will calculate MAX PayLoad Size needed to work
    correctly.

        
    @param EndPoint Pointer to PCI Device Private Data structure.

    @retval UINT16

**/
UINT16 PcieCalcMaxPayLoad(PCI_DEV_INFO *EndPoint){
    UINT16          tmpl=0xFFFF, lmpl;
    PCI_DEV_INFO    *dn, *up;
    BOOLEAN         progup, badtopology;
    UINT16             MaxReadRequest;
#if EFI_DEBUG == 1 //debug
    UINTN           i=1;
#endif
//--------------------
    up=EndPoint;
    DEBUG((DEBUG_VERBOSE,"    --------------------------------------------------------\n"));
    DEBUG((DEBUG_VERBOSE,"    Calculate MPL :\n"));

#if EFI_DEBUG == 1 //debug
        DEBUG((DEBUG_VERBOSE," [B%X|D%X|F%X]<-up;\n", 
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function ));
#endif
       
    //Down stream port and F0 holds optimal MPL for all elements on this link
    do{
        if(up==NULL || up->ParentBrg==NULL)break;
        progup=FALSE;
        badtopology=IsNotFunc0OfMfDev(up);
        
        if(!IsFunc0(up)) up=up->Func0;

#if EFI_DEBUG == 1 //debug
        DEBUG((DEBUG_VERBOSE," [B%X|D%X|F%X]<-up;\n", 
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function ));
#endif
        
        //Init this link mpl var (dn<->up)
        lmpl=up->PciExpress->MaxPayload;

        dn=up->ParentBrg;

#if EFI_DEBUG == 1 //debug
        DEBUG((DEBUG_VERBOSE," dn->[B%X|D%X|F%X]<->[B%X|D%X|F%X]<-up;\n", 
            dn->Address.Addr.Bus,dn->Address.Addr.Device, dn->Address.Addr.Function,
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function ));
#endif
        
        if(dn->PciExpress->MaxPayload<lmpl){
            //if UP link has bigger MPL value then DN link
            //Update UP link recoded value...
            lmpl=dn->PciExpress->MaxPayload;

            //set the Flag that we have to reprogram MPL since downstream MPL has been reduced
            progup=TRUE;
        }

        //Here lmpl holds smallest value of the link,
        //check if accumulated MPL exceeds MPL of this link...
        if(tmpl!=lmpl) progup=TRUE;
        if(tmpl>lmpl) tmpl=lmpl;

        //Get Smallest setup option from the both sides of the link;
        MaxReadRequest = up->PciExpress->Pcie1Setup.MaxReadRequest;
        if(MaxReadRequest > dn->PciExpress->Pcie1Setup.MaxReadRequest)
            MaxReadRequest = dn->PciExpress->Pcie1Setup.MaxReadRequest;


#if EFI_DEBUG == 1 //debug
        DEBUG((DEBUG_VERBOSE," dn->[B%X|D%X|F%X]<->[B%X|D%X|F%X]<-up; Downstream PortType=%d Downstream DevSet=%d Upstream PortType=%d Upstream DevSet=%d\n MaxReadRequest=%d tmpl=%x progup=%d", 
            dn->Address.Addr.Bus,dn->Address.Addr.Device, dn->Address.Addr.Function,
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function,
            dn->PciExpress->PcieCap.PortType,
            dn->PciExpress->DevSet,
            up->PciExpress->PcieCap.PortType,
            up->PciExpress->DevSet,
            MaxReadRequest,
            tmpl,
            progup));
#endif
        
        //PcieProgramPayloadUp has double check if MPL needs to be programmed
        if(progup && up->PciExpress->PcieCap.PortType==PCIE_TYPE_UPS_PORT && up->PciExpress->DevSet){
            if(MaxReadRequest!=PCI_SETUP_AUTO_VALUE) PcieProgramPayloadUp(up, tmpl, MaxReadRequest);
            else PcieProgramPayloadUp(up, tmpl, PCIE_MAXPL_512B);
        }

        up->PciExpress->MaxPayload=tmpl;

        //if we have upstream port not F0 we need to check other functions they might have higher 
        //MPL and MRR values set it must be only EndPoints in this hierarchy
        //"up" already points to Func 0 of that device with "bad topology"...
        if(badtopology)PcieCheckMplMrrOfOtherFunc(up, tmpl, MaxReadRequest);
            
#if EFI_DEBUG == 1 //debug
        DEBUG((DEBUG_VERBOSE,"        Link # %d, Link MPL=%X; Total MPL=%X dn->[B%X|D%X|F%X]<->[B%X|D%X|F%X]<-up;\n", i,lmpl, tmpl,
            dn->Address.Addr.Bus,dn->Address.Addr.Device, dn->Address.Addr.Function,
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function ));
        i++;
#endif
        
        up=dn->ParentBrg;
#if EFI_DEBUG == 1 //debug
        DEBUG((DEBUG_VERBOSE," dn->[B%X|D%X|F%X]<->[B%X|D%X|F%X]<-up;\n", 
            dn->Address.Addr.Bus,dn->Address.Addr.Device, dn->Address.Addr.Function,
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function ));
#endif
        
    } while (up!=NULL && up->ParentBrg!=NULL && up->ParentBrg->Type!=tPciRootBrg && PcieCheckPcieCompatible(up->ParentBrg)); //(EIP49743)

    DEBUG((DEBUG_VERBOSE,"    Calculated MPL %X\n\n", tmpl));

    return tmpl;
}

/**
    This function will convert Latency value from Values read from
    PPCIe.LNK_CNT_REG to Nanoseconds and vice versa.

        
    @param Latency Latency Value to convert
    @param L0s Tells if L0s or L1 Exit latency
    @param ToNs Tells how to convert:
        TRUE  - to the units of nano Seconds;
        FALSE - to the LNK_CNT_REG.LxExitLat register value

    @retval 
        UINTN   Converted Value (-1) - means Invalid Value passed;

**/
//Return value in nS of Max L0s, L1 Exit latency .
UINTN PcieConvertLatency(UINTN Latency, BOOLEAN L0s, BOOLEAN ToNs){
    UINTN   l0s[]={64,128,256,512,1000,2000,4000,8000};
    UINTN   l1[]= {1000, 2000, 4000, 8000, 16000,32000,64000,128000};
    UINTN   *a, max,i=0;
//---------------------
    //Init stuff
    if(L0s) a=&l0s[0];
    else a=&l1[0];

    if (ToNs) max=7;
    else{
        if(L0s) max=8000;
        else max=128000;
    }

    //Check parameters;
    if (Latency > max){
        return max;
    }

    if(ToNs) return a[Latency];
    else {
        while(i<8){
            if(a[i]<Latency)i++;
            else break;
        }
        if(i==8) i=7;

        return i;
    }
}


/**
    This function will allocate memory for Device's PCI Express.
    
    @param Device Pointer to PCI Device Private Data structure. 
    @param PcieCapOffset Current Offset of PCIE Std Compatibility Registers Block.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS Set successfully.
    @retval EFI_INVALID_PARAMETER the Input parameter is invalid.

**/
EFI_STATUS PcieAllocateInitPcieData(PCI_DEV_INFO *Device, UINT8 PcieCapOffset){

    Device->PciExpress=MallocZ(sizeof(PCIE_DATA));
    ASSERT(Device->PciExpress);
    if(!Device->PciExpress) return EFI_OUT_OF_RESOURCES;

    //Save Current Offset of PCIE Std Compatibility Registers Block
    Device->PciExpress->PcieOffs=PcieCapOffset;
    Device->PciExpress->Owner=Device;
    return EFI_SUCCESS;
}

/**
    This function will update Device's referenced as "Func0"
    Clock Power Management based on Setup Settings and hardware capabilities.

        
    @param Func0 Pointer to PCI Device Private Data structure.
    @param LnkCnt Pointer to the Device's LNK_CNT register data.

    @retval VOID

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
VOID PcieUpdateClockPm(PCI_DEV_INFO *Func0, PCIE_LNK_CNT_REG *LnkCnt){
    UINTN           i;
    PCI_DEV_INFO    *dev;
//------------------------
    //Check if Clock PM was set for F0 then we need not to scan caps of the other devices.
    if(LnkCnt->ClockPm == FALSE) return;
    //we will come here if we get a multifunction NON-ARI device and Device Points @ Function 0
    //"LnkCnt" has parameters filled in calculated for Func0 of a MF device.
    //Func0->DevFunc[] includes all other functions except Func 0;
    for(i=0; i<Func0->FuncCount; i++){
        dev=Func0->DevFunc[i];
        //For a non-ARI multi-Function device, power-managementconfiguration
        //software must only Set this bit if all Functions of
        //the multi-Function device indicate a 1b in the Clock Power
        //Management bit of the Link Capabilities register. The
        //component is permitted to use the CLKREQ# signal to power
        //manage Link clock only if this bit is Set for all Functions.

        //For ARI Devices, Clock Power Management is enabled solely by
        //the setting in Function 0. The settings in the other Functions
        //always return whatever value software programmed for each,
        //but otherwise are ignored by the component.
        if(dev->PciExpress->LinkCap.ClockPm == FALSE) {
            LnkCnt->ClockPm=FALSE;
            return;
        }
    }
}

/**
    This function will Disable Link of DNSTREAM port referenced as
    "Device" based on Setup Settings and hardware STATUS. Used to disable "EMPTY"
    links to save some POWER or "Force" LINK to Disable state if Link can not be
    trained correctly.

        
    @param Device Pointer to PCI Device Private Data structure.
    @param Force Flag to indicate to disable link unconditionally.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.
    
    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
EFI_STATUS PcieDisableLink(PCI_DEV_INFO *Device, BOOLEAN Force){
    EFI_STATUS          Status;
    PCI_CFG_ADDR        addr;
    PCIE_LNK_CNT_REG    lcnt;
    BOOLEAN             val;
//------------------------------------------

    //Get Device Link Control Register
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;

    Status=PciCfg16(Device->RbIo, addr, FALSE, &lcnt.LNK_CNT);
    ASSERT_EFI_ERROR(Status);

    if(Force)val=TRUE;
    else val=Device->PciExpress->Pcie1Setup.LnkDisable; //gPciSetupData->LnkDisable;

    //Program Link only if it different from what we have.
    if((UINT8)lcnt.LnkDisable!=val){
        lcnt.LnkDisable=val;
        Status=PciCfg16(Device->RbIo, addr, TRUE, &lcnt.LNK_CNT);
        ASSERT_EFI_ERROR(Status);
    }

    return Status;
}

/**
    This function will Program Link with provided VALUEs
    trained correctly.

        
    @param Device Pointer to PCI Device Private Data structure.
    @param Data Value to put in LNK_CNT register
    @param Reg2 Flag to select LNK_CNT or LNK_CNT2 register.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
EFI_STATUS PcieProgramLink(PCI_DEV_INFO *Device, UINT16 Data, BOOLEAN Reg2){
    EFI_STATUS          Status=0;
    PCI_CFG_ADDR        addr;
//------------------------------------------

    //Get DownStream Port Properties
    addr.ADDR=Device->Address.ADDR;
    if(Reg2)addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_CNT2_OFFSET;
    else addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;

    Status=PciCfg16(Device->RbIo, addr, TRUE, &Data);
    ASSERT_EFI_ERROR(Status);

    return Status;

}

/**
    This function will issue HOT RESET on a LINK referenced by "Device".

    @param Device Pointer to PCI Device Private Data structure.
    @param LnkSpeed check LNK SPEED VECTOR. 

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
EFI_STATUS PcieResetLink(PCI_DEV_INFO *Device, UINT8 LnkSpeed){
    EFI_STATUS          Status=0;
    PCI_CFG_ADDR        addr;
    PCI_BRG_CNT_REG     bcnt;
    BOOLEAN             lnkrst=TRUE;
//--------------------------------
    //For any Root or Switch Downstream Port, setting the Secondary Bus Reset bit of the Bridge
    //Control register associated with the Port must cause a hot reset to be sent (see Section 4.2.4.7).

    DEBUG((DEBUG_VERBOSE," Link RESETING (LnkSpeed=0x%X)... ",LnkSpeed));
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.Register=PCI_BRIDGE_CNTL;


//#if PCI_EXPRESS_GEN3_SUPPORT
    if(LnkSpeed>=PCIE_LINK_SPEED_80G){
        Status=Pcie3EqualizeLink(Device, &lnkrst, LnkSpeed);
        ASSERT_EFI_ERROR(Status);
    }
//#endif

    //Read BRG_CNT_REG
    Status=PciCfg16(Device->RbIo, addr, FALSE, &bcnt.BRG_CNT_REG);
    ASSERT_EFI_ERROR(Status);

    //ASSERT reset Signal
    bcnt.SecBusReset=TRUE;
    Status=PciCfg16(Device->RbIo, addr, TRUE, &bcnt.BRG_CNT_REG);
    ASSERT_EFI_ERROR(Status);
    pBS->Stall(PCI_T_RST);

    //DE-ASSERT reset Signal
    bcnt.SecBusReset=FALSE;
    Status=PciCfg16(Device->RbIo, addr, TRUE, &bcnt.BRG_CNT_REG);
    ASSERT_EFI_ERROR(Status);
    DEBUG((DEBUG_VERBOSE,"Status = %r.\n",Status));
    pBS->Stall(PCI_T_RST_RECOVERY);

    return Status;

}

/**
    This function will initiate LINK training of the
    Down Stream Port referenced as "Device".

        
    @param Device Pointer to PCI Device Private Data structure.
    @param Data Value to put in LNK_CNT register.
    @param LnkSpeed check LNK SPEED VECTOR.
    @param RetryCount count for retry.
    @param Timeout microseconds for stall execution.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
EFI_STATUS PcieRetrainLink(PCI_DEV_INFO *Device, UINT16 Data, UINT8 LnkSpeed, UINTN RetryCount, UINTN TimeOut){
    EFI_STATUS          Status;
    PCI_CFG_ADDR        staa, cnta;
    PCIE_LNK_STA_REG    sta;
    PCIE_LNK_CNT_REG    cnt;
    UINTN               i=0, j=0;
    BOOLEAN             ltok, lnkrt=TRUE;
//-------------------------------------

    DEBUG((DEBUG_VERBOSE," Link RETRAINING...(LnkSpeed=0x%X); # of retry %d\n ",LnkSpeed, RetryCount));

    staa.ADDR=Device->Address.ADDR;
    cnta.ADDR=Device->Address.ADDR;
    cnta.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
    staa.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_STA_OFFSET;

    Status=PciCfg16(Device->RbIo, cnta, FALSE, &cnt.LNK_CNT);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

//#if PCI_EXPRESS_GEN3_SUPPORT
    if(LnkSpeed>=PCIE_LINK_SPEED_80G){
        Status=Pcie3EqualizeLink(Device, &lnkrt, LnkSpeed);
        ASSERT_EFI_ERROR(Status);
    }
//#endif

    //The above algorithm guarantees that Link training will be based on the Link control parameter
    //settings that software intends.
    for(i=0, ltok=FALSE; i<=RetryCount; i++){
    //for(i=0, ltok=FALSE; i<=gPciSetupData->LnkTrRetry; i++){
        //1. Software sets the relevant Link control parameters to the desired settings without writing a 1b to
        //   the Retrain Link bit. (Was done before in PcieProgramLink routine)...

        //2. Software polls the Link Training bit in the Link Status register until the value returned is 0b.
        Status=PciCfg16(Device->RbIo, staa, FALSE, &sta.LNK_STA);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) goto rEXIT;

        if(sta.LnkTraining!=0){
            DEBUG((DEBUG_VERBOSE,"PciE: LinkTraining Loop 1:\n"));
            for(j=0; j<PCIE_LINK_TRAINING_POLLING_COUNT; j++){
                Status=PciCfg16(Device->RbIo, staa, FALSE, &sta.LNK_STA);
                ASSERT_EFI_ERROR(Status);
                if(sta.LnkTraining==0) break;
                //pBS->Stall(gPciSetupData->LnkTrTimeout);
                pBS->Stall(TimeOut);
            }
            DEBUG((DEBUG_VERBOSE,"tried %d times of %d; TimeOut=%duS.\n",j+1,PCIE_LINK_TRAINING_POLLING_COUNT,TimeOut));
        }

        //if still link did not come from Recovery State
        if(sta.LnkTraining!=0 && i>=RetryCount){
            DEBUG((DEBUG_ERROR,"PciE: TRAINING FAILURE! Link can't exit Recovery State. Disabling Link!\n"));
            PcieDisableLink(Device, TRUE ); //gPciSetupData->LnkDisable);
            Status=EFI_DEVICE_ERROR;
            ASSERT_EFI_ERROR(Status);
            goto rEXIT;
        } 


        //3. Software writes a 1b to the Retrain Link bit without changing any other fields in the Link
        //   Control register.
        cnt.RetrainLnk=TRUE;

        Status=PciCfg16(Device->RbIo, cnta, TRUE, &cnt.LNK_CNT);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;

        //Workaround for devices which having Link Retrain bit "sticky"
        //Spec says it always reads as "0".
        Status=PciCfg16(Device->RbIo, cnta, FALSE, &cnt.LNK_CNT);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;

        if(cnt.RetrainLnk==TRUE){
            cnt.RetrainLnk=FALSE;
            Status=PciCfg16(Device->RbIo, cnta, TRUE, &cnt.LNK_CNT);
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)) return Status;
        }

        //Pool link Status back again to make sure it trained well.
        Status=PciCfg16(Device->RbIo, staa, FALSE, &sta.LNK_STA);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) goto rEXIT;

        if(sta.LnkTraining!=0){
            DEBUG((DEBUG_VERBOSE,"PciE: LinkTraining Loop 2:\n "));
            for(j=0; j<PCIE_LINK_TRAINING_POLLING_COUNT; j++){
                Status=PciCfg16(Device->RbIo, staa, FALSE, &sta.LNK_STA);
                ASSERT_EFI_ERROR(Status);
                if(sta.LnkTraining==0) {
                    ltok=TRUE;
                    break;
                }
                pBS->Stall(TimeOut);
            }
            DEBUG((DEBUG_VERBOSE,"tried %d times of %d, TimeOut=%duS\n",j+1,PCIE_LINK_TRAINING_POLLING_COUNT,TimeOut));
        } else ltok=TRUE;

        if(ltok) break;
    }

//#if PCI_EXPRESS_GEN3_SUPPORT
    if(LnkSpeed>=PCIE_LINK_SPEED_80G){
        Status=Pcie3GetEqualizationStatus(Device,LnkSpeed);
        if(EFI_ERROR(Status)){
            //if equalization did not went through will possibly retry it...
            DEBUG((DEBUG_VERBOSE," EQ_STS=%r\n; ", Status));
        } else {
            Status=EFI_SUCCESS;
        }
    }
//#endif

rEXIT:
    DEBUG((DEBUG_VERBOSE," %r\n", Status));
    return Status;
}


/**
    This function will Initialize SLT_CNT and SLT_STA registers
    of the "Device".

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/

EFI_STATUS PcieSetSlotProperties(PCI_DEV_INFO *Device){
    EFI_STATUS            Status=EFI_SUCCESS;
    PCI_CFG_ADDR        addr;
    PCIE_SLT_CNT_REG    sltc;
    PCIE_SLT_STA_REG    slts;
//--------------------------------

    DEBUG((DEBUG_VERBOSE," PciE:  PcieSetSlotProperties() SlotImpl=%d; DontTouch=%d -> ",
            Device->PciExpress->PcieCap.SlotImpl, Device->PciExpress->Pcie1Setup.SltCtrDontTouch));
    if(Device->PciExpress->PcieCap.SlotImpl==TRUE && Device->PciExpress->Pcie1Setup.SltCtrDontTouch ){
        addr.ADDR=Device->Address.ADDR;

        addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_SLT_CNT_OFFSET;
        Status=PciCfg16(Device->RbIo,addr,FALSE,&sltc.SLT_CNT);
        ASSERT_EFI_ERROR(Status);

        if(Device->PciExpress->Pcie1Setup.SltCtrDontTouch!=PCI_SETUP_DONT_TOUCH){
            sltc.AttnBtnPress=0;
            sltc.PowerFaul=0;
            sltc.MrlSensor=0;
            sltc.PresenceDet=0;
            sltc.CmdCompleted=0;
            sltc.HpIntEnable=0;
            sltc.DllStatChEn=0;

            //Power Up slot and set all possible indication on...
            if(Device->PciExpress->SlotCap.AttnInd) sltc.AttnIndCnt=PCIE_SLT_INDICATOR_OFF;
            if(Device->PciExpress->SlotCap.PowerInd)sltc.PowerIndCnt=PCIE_SLT_INDICATOR_ON;
            if(Device->PciExpress->SlotCap.PwrCntler)sltc.PowerOff=PCIE_SLT_PWR_ON;

            Status=PciCfg16(Device->RbIo,addr,TRUE,&sltc.SLT_CNT);
            if(EFI_ERROR(Status)) goto eExit;           
        }

        addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_SLT_STA_OFFSET;
        Status=PciCfg16(Device->RbIo,addr,FALSE,&slts.SLT_STA);
        ASSERT_EFI_ERROR(Status);

        //Clear all RW1C bits in status reg...
        slts.AttnBtnPress=1;
        slts.PowerFaul=1;
        slts.MrlSensor=1;
        slts.PresenceDet=1;
        slts.CmdCompleted=1;
        slts.DllStateChg=1;

        Status=PciCfg16(Device->RbIo,addr,TRUE,&slts.SLT_STA);
    }

eExit:    
    DEBUG((DEBUG_VERBOSE,"%r\n", Status));
    ASSERT_EFI_ERROR(Status);

    return Status;
}


/**
    This function will Select values for Link Control2 register on
    both sides of the LINK based on Setup Settings and hardware capabilities.

        
    @param DnStreamPort Pointer to PCI Device Private Data of Downstream Port of the link.
    @param DnLnkCnt Pointer to the LNK_CNT Reg of the Downstream Port of the link.
    @param UpStreamPort Pointer to PCI Device Private Data of Upstream Port of the link.
    @param UpLnkCnt Pointer to the LNK_CNT Reg of the Upstream Port of the link.
    @param LinkTrainingRequired Flag to modify if Link will need RETRAINING after programming.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetLnkProperties( PCI_DEV_INFO           *DnStreamPort,
                                 PCIE_LNK_CNT_REG       *DnLnkCnt,
                                 PCI_DEV_INFO           *UpStreamPort,
                                 PCIE_LNK_CNT_REG       *UpLnkCnt,
                                 BOOLEAN                *LinkTrainingRequired
                                 )
{
    EFI_STATUS          Status;
    PCIE_LNK_STA_REG    dnls, upls; //link status reg content of up and down side of the link
    PCI_CFG_ADDR        addr;
    BOOLEAN             cc=FALSE;
    BOOLEAN                ExtendedSynchOption;
    BOOLEAN                ClockPmOption;
    BOOLEAN                HwAutoWidthOption;
    UINT8               CommonClockOption;
//------------------------------

    DEBUG((DEBUG_VERBOSE,"PciE: PcieSetLnkProperties() ENTRY:\n"));

    DEBUG((DEBUG_VERBOSE," DN STREAM PORT -> [B%X|D%X|F%X] <--> [B%X|D%X|F%X] <- UP STREAM PORT\n",
            DnStreamPort->Address.Addr.Bus,DnStreamPort->Address.Addr.Device, DnStreamPort->Address.Addr.Function,
            UpStreamPort->Address.Addr.Bus,UpStreamPort->Address.Addr.Device, UpStreamPort->Address.Addr.Function));

    //Get DownStream Port Properties
    addr.ADDR=DnStreamPort->Address.ADDR;
    addr.Addr.Register=DnStreamPort->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
    Status=PciCfg16(DnStreamPort->RbIo,addr,FALSE, &DnLnkCnt->LNK_CNT);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    DEBUG((DEBUG_VERBOSE," LNK_CNT -> [BwI=%X|BwMI=%X|AuWi=%X|CkPM=%X|ExS=%X|CCk=%X|Rtr=%X|LDis=%X|RCB=%X|ASPM=%X]\n",
                DnLnkCnt->AutoBandwInt,
                DnLnkCnt->BandwMgmtInt,
                DnLnkCnt->HwAutoWdtDis,
                DnLnkCnt->ClockPm,
                DnLnkCnt->ExtSynch,
                DnLnkCnt->CommonClk,
                DnLnkCnt->RetrainLnk,
                DnLnkCnt->LnkDisable,
                DnLnkCnt->RdComplBound,
                DnLnkCnt->AspmControl));



    addr.Addr.Register=DnStreamPort->PciExpress->PcieOffs+PCIE_LNK_STA_OFFSET;
    Status=PciCfg16(DnStreamPort->RbIo,addr,FALSE, &dnls.LNK_STA);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    //Get UpStream Stream Port Properties
    addr.ADDR=UpStreamPort->Address.ADDR;

    addr.Addr.Register=UpStreamPort->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
    Status=PciCfg16(UpStreamPort->RbIo,addr,FALSE, &UpLnkCnt->LNK_CNT);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    DEBUG((DEBUG_VERBOSE," LNK_CNT -> [BwI=%X|BwMI=%X|AuWi=%X|CkPM=%X|ExS=%X|CCk=%X|Rtr=%X|LDis=%X|RCB=%X|ASPM=%X]\n    ....... UPDATINNG .......\n",
                UpLnkCnt->AutoBandwInt,
                UpLnkCnt->BandwMgmtInt,
                UpLnkCnt->HwAutoWdtDis,
                UpLnkCnt->ClockPm,
                UpLnkCnt->ExtSynch,
                UpLnkCnt->CommonClk,
                UpLnkCnt->RetrainLnk,
                UpLnkCnt->LnkDisable,
                UpLnkCnt->RdComplBound,
                UpLnkCnt->AspmControl));

    //Get Lnk Status Content... from DownSteam and UpStream Ports
    addr.Addr.Register=UpStreamPort->PciExpress->PcieOffs+PCIE_LNK_STA_OFFSET;
    Status=PciCfg16(UpStreamPort->RbIo,addr,FALSE, &upls.LNK_STA);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    //1.Disable ASPM for now we will set it later starting from Endpoints!
    if(UpStreamPort->PciExpress->Pcie1Setup.AspmMode!=PCI_SETUP_DONT_TOUCH){
        DnLnkCnt->AspmControl=0;
        UpLnkCnt->AspmControl=0;
    }

    //2. Check Clock Config\
    //If on any side of the link Porting updated IgnoreCommonClock setup option 
    CommonClockOption = (UpStreamPort->PciExpress->Pcie1Setup.IgnoreCommonClock | DnStreamPort->PciExpress->Pcie1Setup.IgnoreCommonClock);
    //If we asked to not touch CC - ignore it.
    if(!CommonClockOption){
        //Check if Lnk Clk config programmed correctly by CSP Code.
        //see if both sides can use Common Clock (Power up Default == 0)
        if(dnls.CommonClk && upls.CommonClk) cc=TRUE;

        //Verify if ClkConfig is set correctly and detect if link training is needed
        if((DnLnkCnt->CommonClk != cc) || (UpLnkCnt->CommonClk!=cc)) *LinkTrainingRequired=TRUE;
        DnLnkCnt->CommonClk=cc;
        UpLnkCnt->CommonClk=cc;
    }
    
    //3.Extended Synch
    if((DnStreamPort->PciExpress->Pcie1Setup.ExtendedSynch!=PCI_SETUP_DONT_TOUCH) && 
            (UpStreamPort->PciExpress->Pcie1Setup.ExtendedSynch!=PCI_SETUP_DONT_TOUCH))
    {
        ExtendedSynchOption=(DnStreamPort->PciExpress->Pcie1Setup.ExtendedSynch | UpStreamPort->PciExpress->Pcie1Setup.ExtendedSynch );

        DnLnkCnt->ExtSynch=ExtendedSynchOption;
        UpLnkCnt->ExtSynch=ExtendedSynchOption;
    }

    //4. Clock PM
    ClockPmOption=(Pcie2CheckPcie2Compatible(DnStreamPort) && Pcie2CheckPcie2Compatible(UpStreamPort));
    //Play with it if Both sides of the link Supports Pcie2 otherwise - don't touch.
    if(ClockPmOption){
        if((UpStreamPort->PciExpress->Pcie1Setup.ClockPm != PCI_SETUP_DONT_TOUCH) && 
                (DnStreamPort->PciExpress->Pcie1Setup.ClockPm != PCI_SETUP_DONT_TOUCH))
        {
            ClockPmOption=(UpStreamPort->PciExpress->Pcie1Setup.ClockPm |
                    DnStreamPort->PciExpress->Pcie1Setup.ClockPm );

            if( (UpStreamPort->PciExpress->LinkCap.ClockPm) &&     ClockPmOption )
            {
                DnLnkCnt->ClockPm=TRUE;
                UpLnkCnt->ClockPm=TRUE;
            } else {
                DnLnkCnt->ClockPm=FALSE;
                UpLnkCnt->ClockPm=FALSE;
            }
        }
    }

    //5. Clear Some interrupt related bits.
    DnLnkCnt->BandwMgmtInt=FALSE;
    DnLnkCnt->AutoBandwInt=FALSE;
    
    // 6. 
    
    //get Link HwAutoWidth option
        if((Pcie2CheckPcie2Compatible(DnStreamPort) && 
           DnStreamPort->PciExpress->Pcie2->Pcie2Setup.HwAutoWidth!=PCI_SETUP_DONT_TOUCH) && 
           (Pcie2CheckPcie2Compatible(UpStreamPort) && 
            UpStreamPort->PciExpress->Pcie2->Pcie2Setup.HwAutoWidth!=PCI_SETUP_DONT_TOUCH)){
            
            if(DnStreamPort->PciExpress->Pcie2->Pcie2Setup.HwAutoWidth <= UpStreamPort->PciExpress->Pcie2->Pcie2Setup.HwAutoWidth)
             HwAutoWidthOption=DnStreamPort->PciExpress->Pcie2->Pcie2Setup.HwAutoWidth;
            else HwAutoWidthOption=UpStreamPort->PciExpress->Pcie2->Pcie2Setup.HwAutoWidth;
        
            //Check;  HwAutoSpeedDis; HwAutoWidth settings
            DnLnkCnt->HwAutoWdtDis=HwAutoWidthOption;
            UpLnkCnt->HwAutoWdtDis=HwAutoWidthOption;
        }

    DEBUG((DEBUG_VERBOSE," LNK_CNT -> [BwI=%X|BwMI=%X|AuWi=%X|CkPM=%X|ExS=%X|CCk=%X|Rtr=%X|LDis=%X|RCB=%X|ASPM=%X]\n",
                DnLnkCnt->AutoBandwInt,
                DnLnkCnt->BandwMgmtInt,
                DnLnkCnt->HwAutoWdtDis,
                DnLnkCnt->ClockPm,
                DnLnkCnt->ExtSynch,
                DnLnkCnt->CommonClk,
                DnLnkCnt->RetrainLnk,
                DnLnkCnt->LnkDisable,
                DnLnkCnt->RdComplBound,
                DnLnkCnt->AspmControl));

    DEBUG((DEBUG_VERBOSE," LNK_CNT -> [BwI=%X|BwMI=%X|AuWi=%X|CkPM=%X|ExS=%X|CCk=%X|Rtr=%X|LDis=%X|RCB=%X|ASPM=%X]\n\n",
                UpLnkCnt->AutoBandwInt,
                UpLnkCnt->BandwMgmtInt,
                UpLnkCnt->HwAutoWdtDis,
                UpLnkCnt->ClockPm,
                UpLnkCnt->ExtSynch,
                UpLnkCnt->CommonClk,
                UpLnkCnt->RetrainLnk,
                UpLnkCnt->LnkDisable,
                UpLnkCnt->RdComplBound,
                UpLnkCnt->AspmControl));

    return Status;
}


/**
    This function will Collect PCI Express Capabilities Data
    of the "Device".

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
EFI_STATUS PcieGetCaps(PCI_DEV_INFO *Device){
    EFI_STATUS        Status;
    PCI_CFG_ADDR        addr;
//-------------------------------
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=0;

    DEBUG((DEBUG_VERBOSE,"\nPciE: Collecting CAPs for [B%X|D%X|F%X]\n",
            Device->Address.Addr.Bus,Device->Address.Addr.Device,Device->Address.Addr.Function));

    //Get PCI Express CAPs - 16 bit
    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_CAP_OFFSET;
    Status=PciCfg16(Device->RbIo,addr,FALSE,&Device->PciExpress->PcieCap.PCIE_CAP);
    ASSERT_EFI_ERROR(Status);
    
    DEBUG((DEBUG_VERBOSE," PCIE_CAP-> [R=%X|IntMsgNo=%X|SlotImpl=%X|PortType=%X|CapVer=%X]\n",
                Device->PciExpress->PcieCap.Reserved,
                Device->PciExpress->PcieCap.IntMsgNo,
                Device->PciExpress->PcieCap.SlotImpl,
                Device->PciExpress->PcieCap.PortType,
                Device->PciExpress->PcieCap.CapVersion
    ));

    //Get PCI Express Device CAPs - 32 bit after setting clock configuration
    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_DEV_CAP_OFFSET;
    Status=PciCfg32(Device->RbIo,addr,FALSE,&Device->PciExpress->DeviceCap.DEV_CAP);
    ASSERT_EFI_ERROR(Status);


    //Print Content of DEV_CAP and DEV_CNT register..
    DEBUG((DEBUG_VERBOSE," DEV_CAP -> [R2=%X|FR=%X|PwrS=%X|PwrV=%X|R1=%X|RBE=%X|PI=%X|AI=%X|AB=%X|EpL1=%X|EpL0=%X|ExT=%X|PhF=%X|MPL=%X]\n",
                Device->PciExpress->DeviceCap.Reserved2,
                //Added in PCIe Base V2.0
                Device->PciExpress->DeviceCap.FuncResetCap,
                Device->PciExpress->DeviceCap.SlotPwrLimS,
                Device->PciExpress->DeviceCap.SlotPwrLimV,
                Device->PciExpress->DeviceCap.Reserved1,
                //Added in PCIe Base V2.0
                Device->PciExpress->DeviceCap.RoleBasedErr,
                Device->PciExpress->DeviceCap.PowerInd,
                Device->PciExpress->DeviceCap.AttnInd,
                Device->PciExpress->DeviceCap.AttnBtn,
                Device->PciExpress->DeviceCap.EpL1Latency,
                Device->PciExpress->DeviceCap.EpL0Latency,
                Device->PciExpress->DeviceCap.ExtTagFld,
                Device->PciExpress->DeviceCap.PhantomFunc,
                Device->PciExpress->DeviceCap.MaxPayload
    ));

    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_CAP_OFFSET;
    Status=PciCfg32(Device->RbIo,addr,FALSE,&Device->PciExpress->LinkCap.LNK_CAP);
    ASSERT_EFI_ERROR(Status);

    DEBUG((DEBUG_VERBOSE," LNK_CAP -> [P#=%X|R=%X|BwN=%X|DllL=%X|SDE=%X|ClkPM=%X|L1=%X|L0=%X|ASPM=%X|MaxW=%X|MaxS=%X]\n",
                Device->PciExpress->LinkCap.PortNum,
                Device->PciExpress->LinkCap.Reserved,
                //Added in PCIe Base V2.0
                Device->PciExpress->LinkCap.BandwNotify,
                Device->PciExpress->LinkCap.DllLnkActive,
                Device->PciExpress->LinkCap.SurpDownErr,
                Device->PciExpress->LinkCap.ClockPm,
                //---------------------------------
                Device->PciExpress->LinkCap.ExL1Latency,
                Device->PciExpress->LinkCap.ExL0Latency,
                Device->PciExpress->LinkCap.AspmSupport,
                Device->PciExpress->LinkCap.MaxLnkWidth,
                Device->PciExpress->LinkCap.MaxLnkSpeed
    ));

    //Get PCI Express Slot CAPs - 32 bit it is valid only for Ports Switches and Bridges
    if((Device->Type==tPci2PciBrg || Device->Type==tPci2CrdBrg) &&
        Device->PciExpress->PcieCap.SlotImpl )
    {
        addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_SLT_CAP_OFFSET;
        Status=PciCfg32(Device->RbIo,addr,FALSE,&Device->PciExpress->SlotCap.SLT_CAP);
        ASSERT_EFI_ERROR(Status);

        DEBUG((DEBUG_VERBOSE," SLT_CAP -> [S#=%X|NoCmdC=%X|EmLck=%X|PwrS=%X|PwrV=%X|HpC=%X|HpS=%X|PI=%X|AI=%X|MrlS=%X|PwC=%X|AB=%X]\n",
                Device->PciExpress->SlotCap.PhisSlotNum,
                //Added in PCIe Base V2.0
                Device->PciExpress->SlotCap.NoCmdCompl,
                Device->PciExpress->SlotCap.EmInterlock,
                //----------------------------
                Device->PciExpress->SlotCap.PwrLimScale,
                Device->PciExpress->SlotCap.PwrLimVal,
                Device->PciExpress->SlotCap.HpCapable,
                Device->PciExpress->SlotCap.HpSurprise,
                Device->PciExpress->SlotCap.PowerInd,
                Device->PciExpress->SlotCap.AttnInd,
                Device->PciExpress->SlotCap.MrlSensor,
                Device->PciExpress->SlotCap.PwrCntler,
                Device->PciExpress->SlotCap.AttnBtn
        ));

    }
    
    //Get RootPort CAPs needed for CRS support Regs present in Root Complex Root Port Device types only. 
    if(Device->PciExpress->PcieCap.PortType == PCIE_TYPE_ROOTPORT ){
        addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_ROOT_CAP_OFFSET;
        Status=PciCfg16(Device->RbIo,addr,FALSE,&Device->PciExpress->RootCap.ROOT_CAP);
        ASSERT_EFI_ERROR(Status);
        
        DEBUG((DEBUG_VERBOSE," ROOT_CAP-> [Res=%X|CRSSwV=%X]\n",
                Device->PciExpress->RootCap.Reserved,
                Device->PciExpress->RootCap.CrsSwVisib));
    }

    return Status;
}

/**
    This function will check if *PCI_DEV_INFO passed belongs to
    PCI Express DOWNSTREAM PORT:
    - RootPort of PCIe Root Complex;
    - DownStream Port of PCIe Switch;
    - PCI/PCI-X to PCIe Bridge.

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval BOOLEAN
        TRUE         When PCI_DEV_INFO passed belongs to DOWNSTREAM PORT
        FALSE        All other cases...

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
BOOLEAN PcieIsDownStreamPort(PCI_DEV_INFO *Device){

    if((Device->PciExpress->PcieCap.PortType == PCIE_TYPE_ROOTPORT) ||
       (Device->PciExpress->PcieCap.PortType == PCIE_TYPE_DNS_PORT) ||
       (Device->PciExpress->PcieCap.PortType == PCIE_TYPE_PCI_PCIE))
        return TRUE;
    else
        return FALSE;
}


/**
    This function will Initialize Pcie Link on both sides starting
    from DownStream Port.

        
    @param DnPort Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieInitLink(PCI_DEV_INFO     *DnPort )
{
    EFI_STATUS             Status=EFI_SUCCESS;
    EFI_STATUS             TempStatus;
    PCI_DEV_INFO        *dn, *up, *f0;
    PCI_BRG_EXT         *ext;
    PCIE_LNK_CNT_REG    dnlc, uplc;
    BOOLEAN                lr=FALSE,lt=FALSE, pe2=FALSE, mf=FALSE;
    //START: Fix for Coverity Issue : UNINIT - Un-Initialized "l0" and "l1" used
    UINT16              mpl, l0 = 0, l1 = 0;//calculated link speed
    //END: Fix for Coverity Issue : UNINIT - Un-Initialized "l0" and "l1" used
    UINTN               i;
    T_ITEM_LIST         *chldlst=NULL;
    UINT8               lnkspeed=0;
    PCIE_LNK_CNT2_REG   dnlc2, uplc2;
    BOOLEAN             ARI_FAULTY = FALSE;
//-------------------------------
    //START: Fix for Coverity Issue : REVERSE_INULL
    if(!DnPort){
        return EFI_INVALID_PARAMETER;
    }
    //END: Fix for Coverity Issue : REVERSE_INULL

    if(!PcieCheckPcieCompatible(DnPort)) return EFI_SUCCESS;

    //Going from DOWN STREAM PORT to EP
    dn=DnPort;
    ext=(PCI_BRG_EXT*)(dn+1);


    DEBUG((DEBUG_VERBOSE,"\n PciE: InitLink DNSP @ [B%X|D%X|F%X] has 0x%X Childs",
            dn->Address.Addr.Bus,dn->Address.Addr.Device,dn->Address.Addr.Function, ext->ChildCount));
    if(ext->ChildCount==0 || dn->PciExpress->Pcie1Setup.LnkDisable==2){
        if(ext->ChildCount==0) Status=PcieDisableLink(dn, FALSE);//, LnkDisableOption);//gPciSetupData->LnkDisable);
        else Status=PcieDisableLink(dn, TRUE);
        DEBUG((DEBUG_VERBOSE," - Disable Link=%d: %r\n", dn->PciExpress->Pcie1Setup.LnkDisable, Status));
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    //Check if we need to initialize Pcie2 features as well
    pe2=Pcie2CheckPcie2Compatible(dn);

    if(pe2)DEBUG((DEBUG_VERBOSE," ARI=%d; ",dn->PciExpress->Pcie2->AriEnabled));
    else DEBUG((DEBUG_VERBOSE," ARI=0; "));

    Status=CpyItemLst((T_ITEM_LIST*)&ext->InitialCount, &chldlst);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    //Go through Bridge's Children and...
    //START: Fix for Coverity Issue : REVERSE_INULL
    for(i=0,f0=NULL; (chldlst != NULL)&&(i<chldlst->ItemCount); /*i++*/){
    //END: Fix for Coverity Issue : REVERSE_INULL
      ARI_FAULTY = FALSE;
        up=(PCI_DEV_INFO*)chldlst->Items[i];

        if(!PcieCheckPcieCompatible(up))return EFI_SUCCESS;

        if(IsFunc0(up)){
            f0=up;
            mf=IsFunc0OfMfDev(up);
//#if PCI_EXPRESS_GEN2_SUPPORT
            //Set PCIe GEN2 Properties first it is applicable only for Func0
            //if both sides of the link supports it...
            //if((up->PciExpress->Pcie2!=NULL) && (pe2==TRUE)){
            if(Pcie2CheckPcie2Compatible(up) && (pe2==TRUE)){
                DEBUG((DEBUG_VERBOSE,"GEN2=%d.\n",pe2));
                Status=Pcie2SetLnkProperties(dn, &dnlc2, up, &uplc2, &lr, &lt);
                ASSERT_EFI_ERROR(Status);
                if(EFI_ERROR(Status)) return Status;
            } else pe2 = FALSE;
//#endif
            if(pe2==FALSE)DEBUG((DEBUG_VERBOSE,"GEN2=%d.\n",pe2));


            //Now Set PCIe GEN1 properties.
            Status=PcieSetLnkProperties(dn, &dnlc, up, &uplc, &lt);
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)) return Status;

            if( (up->PciExpress->AriData) && ((up->PciExpress->AriData->AriCaps.ARI_CAP >> 8) == 0)) ARI_FAULTY = TRUE;
            //Now remove pointer to the func 0 from DOWN STREAM PORT Child's List
            Status=DeleteItemLst(chldlst, i, FALSE);
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)) return Status;
            if (!ARI_FAULTY)
            break;
        }
        if (!ARI_FAULTY)
          i++;
        else
          i = 0;
    }

    //It must be a Child with Function == 0 there...
    //ASSERT(f0!=NULL);
    if(f0==NULL) {
        Status=EFI_DEVICE_ERROR;
        ASSERT_EFI_ERROR(Status);
        return Status;
    }
    
    //Check some condition which suppose to be true for PCI Express Devices.
    //ASSERT(chldlst->ItemCount == f0->FuncCount); //it must be the same number as FuncCount of Func0 device
    if((chldlst != NULL) && (chldlst->ItemCount != f0->FuncCount)){
        Status=EFI_DEVICE_ERROR;
        ASSERT_EFI_ERROR(Status);
        return Status;
    }
    
    //now we are here and have prepared content of LNK_CNT and LNK_CNT2 for programming.
    //there are some rules for link initialization that differs for ARI devices and regular ones.
    //1. If device is ARI device (has ARI compatibility header) all programming is applicable for Function0 only.
    //   all other Link control Register bits are reserved for the functions other than 0.
    //2. If Device is NON-ARI than:
    //  a) all functions must report same value in capabilities except ClockPM bit -
    //     it is independent but will work only if all Functions of the device reports same capability.
    //  b) all functions must have same values in LNK_CNT for feature to work.
    //     except same old ClockPM - it can be set only if all functions support it.

    //If device is NON-ARI we must program all functions to the same values
    if((f0!=NULL) && (f0->PciExpress->AriData == NULL) && (mf==TRUE)){

        PcieUpdateClockPm(f0,&uplc);
        //Apply Settings for each child if device is MF
        for(i=0; i<f0->FuncCount; i++){
            up=f0->DevFunc[i];

            if(!PcieCheckPcieCompatible(up)) continue;

            Status=PcieProgramLink(up,uplc.LNK_CNT, FALSE);
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)) return Status;

//#if PCI_EXPRESS_GEN2_SUPPORT
            if(pe2){
                Status=PcieProgramLink(up,uplc2.LNK_CNT2,TRUE);
                ASSERT_EFI_ERROR(Status);
                if(EFI_ERROR(Status)) return Status;
            }
//#endif

        }
    }

    //Now Program the link ....
    //Func 0 of Upstream port...
    Status=PcieProgramLink(f0,uplc.LNK_CNT, FALSE);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

//#if PCI_EXPRESS_GEN2_SUPPORT
    if(pe2){
        Status=PcieProgramLink(f0,uplc2.LNK_CNT2, TRUE);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;
    }
//#endif

    //Downstream port...
    Status=PcieProgramLink(dn,dnlc.LNK_CNT, FALSE);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

//#if PCI_EXPRESS_GEN2_SUPPORT
    if(pe2){
        Status=PcieProgramLink(dn,dnlc2.LNK_CNT2, TRUE);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;
    }
    //All soft instances of PCIe registers updated
    //now check update lnkspeed variable
    if(pe2)lnkspeed=(UINT8)(dnlc2.TargetLnkSpeed);
    else lnkspeed=1;

    //And now see do we need to Retrain link or issue a Hot Reset.
    //If Link Speed was changed we need to do a Hot Reset of a Link.
    //When Hot Reset issued with New Link Parameters link will be retrained anyway.

    if(lt){
        Status=PcieRetrainLink(dn,dnlc.LNK_CNT, lnkspeed,
                dn->PciExpress->Pcie1Setup.LnkTrRetry,
            dn->PciExpress->Pcie1Setup.LnkTrTimeout);

            
            if(EFI_ERROR(Status)){
                if(Status==EFI_DEVICE_ERROR)lr=TRUE;
                else {
                  ASSERT_EFI_ERROR(Status);
                  return Status;
                }
            }
    } 

    //Link reset sometimes used to recover from faulty training.
    if(lr){
        while (1) { 
            Status=PcieResetLink(dn, lnkspeed);
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)){
                if(Status==EFI_DEVICE_ERROR && lnkspeed >= PCIE_LINK_SPEED_50G ){
                    lnkspeed--; //Reduce link speed.
                    if(pe2){
                        uplc2.TargetLnkSpeed = lnkspeed;
                        TempStatus=PcieProgramLink(f0,uplc2.LNK_CNT2, TRUE);
                        ASSERT_EFI_ERROR(TempStatus);
                        if(EFI_ERROR(TempStatus)) return TempStatus;
                    }
                    if(pe2){
                        dnlc2.TargetLnkSpeed = lnkspeed;
                        TempStatus=PcieProgramLink(dn,dnlc2.LNK_CNT2, TRUE);
                        ASSERT_EFI_ERROR(TempStatus);
                        if(EFI_ERROR(TempStatus)) return TempStatus;
                    }
                } else return Status;
            }
            if (Status == EFI_SUCCESS)
                break; //exit the while loop
        }

        Status=RestoreBridgeBuses(dn);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;
    } 


    //Update Internal Data now once we have applied all link settings
    //It might affect some capablities of the PCIe Device.
//#if PCI_EXPRESS_GEN2_SUPPORT
    if(pe2){
        dn->PciExpress->Pcie2->LnkSpeed=(UINT8)dnlc2.TargetLnkSpeed;
        f0->PciExpress->Pcie2->LnkSpeed=(UINT8)uplc2.TargetLnkSpeed;
    }
//#endif

    //Set Flag that link was programmed!
    dn->PciExpress->LinkSet=TRUE;
    f0->PciExpress->LinkSet=TRUE;


    //If link was reset or retrained, Update link capabilities
    //since it might change because of clock config and speed change.
    if(lt || lr ){
        DEBUG((DEBUG_VERBOSE,"Pcie: Refresh CAPs for DNSTREAM; UPSTREAM + ALL FUNC\n"));
        Status=PcieGetCaps(dn);
        ASSERT_EFI_ERROR(Status);

        Status=PcieGetCaps(f0);
        ASSERT_EFI_ERROR(Status);
    }

    //For all devices interfacing with DownStream port
    //update MaxPayLoad and L0s L1 exit Latencies once we are already here.
    //We will use it later to Program Pcie Device Chain
    mpl=dn->PciExpress->DeviceCap.MaxPayload;

    //value of 0xFFFE indicates corresponded Lx state is not supported)
    //value of 0xFFFF indicates corresponded Lx latency was not calculated yet.
    if(dn->PciExpress->LinkCap.AspmSupport & PCIE_ASPM_L0_ENABLE) l0=dn->PciExpress->LinkCap.ExL0Latency;
    if(dn->PciExpress->LinkCap.AspmSupport & PCIE_ASPM_L1_ENABLE) l1=dn->PciExpress->LinkCap.ExL1Latency;

    if(mpl > f0->PciExpress->DeviceCap.MaxPayload)mpl=f0->PciExpress->DeviceCap.MaxPayload;
    //It is stated in spec "Multi-Function devices associated with an Upstream Port must
    //report the same value in this field for all Functions"
    if(l0!=0xFFFF && (l0 < f0->PciExpress->LinkCap.ExL0Latency)) l0=f0->PciExpress->LinkCap.ExL0Latency;
    if(l1  < f0->PciExpress->LinkCap.ExL1Latency) l1=f0->PciExpress->LinkCap.ExL1Latency;

    for(i=0; i<f0->FuncCount; i++){
        up=f0->DevFunc[i];

        if(!PcieCheckPcieCompatible(up))continue;

        up->PciExpress->LinkSet=TRUE;
//#if PCI_EXPRESS_GEN2_SUPPORT
        if(pe2) up->PciExpress->Pcie2->LnkSpeed=(UINT8)uplc2.TargetLnkSpeed;
//#endif
        //Link Retraining or Hot Reset might change Latency values...
        if(lt || lr ){
            Status=PcieGetCaps(up);
            ASSERT_EFI_ERROR(Status);
        }

        //Update MaxPL for all other functions,....spec says
        // "The Functions of a multi-Function device are permitted to
        // report different values for this field."
        if(mpl>up->PciExpress->DeviceCap.MaxPayload)mpl=up->PciExpress->DeviceCap.MaxPayload;
        up->PciExpress->MaxL0Lat=l0;
        up->PciExpress->MaxL1Lat=l1;
    }

    //found smallest one and record it in MaxPayload field of DOWNSTREAM PORT and UPSTREAM PORT's F0;
    f0->PciExpress->MaxPayload=mpl;
    f0->PciExpress->MaxL0Lat=l0;
    f0->PciExpress->MaxL1Lat=l1;

    dn->PciExpress->MaxPayload=mpl;
    dn->PciExpress->MaxL0Lat=l0;
    dn->PciExpress->MaxL1Lat=l1;

    //some house keeping;
    ClearItemLst(chldlst, FALSE);
    if(chldlst)pBS->FreePool(chldlst);

    return Status;
}

/**
    This function will Fill out structure of PciE Ext Cap data

    @param Device Pointer to PCI Device Private Data structure.
    @param Dataptr pointer to holding data.
    @param DataSize size of data to allocate memory.
    @param CapHdrOffset offset for Capability Register header.
    @param Cap16 TRUE/FALSE
    @param OffsetFromCapHdr offset from Capability Register header. 

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieCollectDeviceExtCap(PCI_DEV_INFO *Device, VOID **DataPtr, UINTN DataSize, UINT32 CapHdrOffset, BOOLEAN Cap16, UINTN OffsetFromCapHdr){
    EFI_STATUS          Status=EFI_SUCCESS;
    PCI_CFG_ADDR        addr;
    PCIE_EXT_DATA_HDR   *dhdr;
//---------------------------------------
    if(!PcieCheckPcieCompatible(Device)) return EFI_SUCCESS;
    //Pointer must be empty at the beginning.
    if(*DataPtr!=NULL) {
        ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
        return EFI_INVALID_PARAMETER;
    }

    *DataPtr=MallocZ(DataSize);

    if(*DataPtr==NULL) {
        ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
        return EFI_OUT_OF_RESOURCES;
    }

    dhdr=(PCIE_EXT_DATA_HDR*)(*DataPtr);

    dhdr->CapHdrOffset=CapHdrOffset;

    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=CapHdrOffset+(sizeof(PCIE_EXT_CAP_HDR))+(UINT32)OffsetFromCapHdr;

    if(Cap16)Status=PciCfg16(Device->RbIo, addr, FALSE,(UINT16*)&dhdr->CapReg);
    else Status=PciCfg32(Device->RbIo, addr, FALSE,(UINT32*)&dhdr->CapReg);

    ASSERT_EFI_ERROR(Status);
    return Status;
}

/**
    This function will power ON or OFF PCI Express Slot

        
    @param Dev Pointer to PCI Device Private Data structure.
    @param PwrOn Tells to turn slot power ON or OFF

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetSlotPower(PCI_DEV_INFO* Device, BOOLEAN PwrOn){
    PCI_CFG_ADDR        addr;
    PCIE_SLT_CNT_REG    sltc;
    EFI_STATUS            Status=0;
//    UINT16                pwr=PCIE_SLT_PWR_OFF;
//--------------------------------------------
    if(!PcieCheckPcieCompatible(Device)) return EFI_SUCCESS;

    if(Device->PciExpress->SlotCap.PwrCntler){
        addr.ADDR=Device->Address.ADDR;
        addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_SLT_CNT_OFFSET;
        Status=PciCfg16(Device->RbIo,addr,FALSE,&sltc.SLT_CNT);
        ASSERT_EFI_ERROR(Status);
        if(PwrOn){
            if(Device->PciExpress->SlotCap.PowerInd) sltc.PowerIndCnt=PCIE_SLT_INDICATOR_ON;
            sltc.PowerOff=PCIE_SLT_PWR_ON;
        } else {
            if(Device->PciExpress->SlotCap.PowerInd) sltc.PowerIndCnt=PCIE_SLT_INDICATOR_OFF;
            sltc.PowerOff=PCIE_SLT_PWR_OFF;
        }
        Status=PciCfg16(Device->RbIo,addr,TRUE,&sltc.SLT_CNT);
        ASSERT_EFI_ERROR(Status);
    }
    return Status;
}


/**
    This function will remove Device referenced as EndPoint from
    the gPcieEpLst data structure used as a database of available EndPoint devices.

    @param EndPoint Pointer to PCI Device Private Data structure.
    @param AllFunc TRUE/FALSE

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieRemoveEpFromLst(PCI_DEV_INFO *EndPoint, BOOLEAN AllFunc){
    PCI_DEV_INFO    *dev;
    UINTN           i;
//-------------------------
    
    
    for(i=0; i<gPcieEpList.ItemCount; i++){
        dev=(PCI_DEV_INFO*)gPcieEpList.Items[i];

        if(dev==EndPoint){
            DeleteItemLst(&gPcieEpList, i, FALSE);
            DEBUG((DEBUG_INFO,"PciE: Removing Device [B%X|D%X|F%X] from gPcieEpList[%d]; ItemsCount=%d\n",
            EndPoint->Address.Addr.Bus,EndPoint->Address.Addr.Device, EndPoint->Address.Addr.Function, i, gPcieEpList.ItemCount));
            if(AllFunc && IsFunc0OfMfDev(EndPoint)){
                for(i=0; i< EndPoint->FuncCount; i++){
                    dev=EndPoint->DevFunc[i];
                    PcieRemoveEpFromLst(dev, FALSE);
                }
            }
            return EFI_SUCCESS;
        }
    }
    
    
    DEBUG((DEBUG_INFO,"PciE: Can't find device [B%X|D%X|F%X] in gPcieEpList[%d]\n",
    EndPoint->Address.Addr.Bus,EndPoint->Address.Addr.Device, EndPoint->Address.Addr.Function, gPcieEpList.ItemCount));

    return EFI_NOT_FOUND;
}


/**
    This function will calculate MAX Latency needed to exit from
    one of the ASPM States L0s or L1.

        
    @param Device Pointer to PCI Device Private Data structure.
    @param L0s Tells which ASPM state Latency we are calculating

    @retval 
        UINTN Calculated latency

**/
UINTN PcieCalcLatency(PCI_DEV_INFO *EndPoint, BOOLEAN L0s){
    PCI_DEV_INFO    *dn, *up;
    UINT16          *ll;
    UINTN           i=0, tmp=0, lat=0;
//--------------------
    up=EndPoint;
    DEBUG((DEBUG_VERBOSE,"    Calculate "));
    if(L0s)DEBUG((DEBUG_VERBOSE,"L0s:\n"));
    else DEBUG((DEBUG_VERBOSE,"L1 :\n"));
    //Down steam port and F0 holds optimal MPL for all elements on this link
    do{
        if(!IsFunc0(up)) up=up->Func0;
        dn=up->ParentBrg;
        if(L0s)ll=&dn->PciExpress->MaxL0Lat;
        else ll=&dn->PciExpress->MaxL1Lat;

        //if PciExpress->MaxL0Lat==0xFFFF means Link does not support ASPM
        if(*ll!=0xFFFF ) {
            if(L0s) {
                tmp=PcieConvertLatency(dn->PciExpress->MaxL0Lat, L0s, TRUE);
                lat+=tmp;
            } else {
                tmp=PcieConvertLatency(dn->PciExpress->MaxL1Lat, L0s, TRUE);
                if(tmp>lat) lat=tmp;
            }
        }

        DEBUG((DEBUG_VERBOSE,"        Link# %d Lat=%d(nS) dn->[B%X|D%X|F%X]<->[B%X|D%X|F%X]<-up;\n", i+1, tmp,
            dn->Address.Addr.Bus,dn->Address.Addr.Device, dn->Address.Addr.Function,
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function ));
        i++;

        //Advance to the next link.
        up=dn->ParentBrg;
    } while (up!=NULL && up->ParentBrg!=NULL && up->ParentBrg->Type!=tPciRootBrg && PcieCheckPcieCompatible(up->ParentBrg));//(EIP49743)

    //Internal delay on each Upstream Port (per spec.) MUST BE NO MORE than 1uS.
    //so next UPSTREAM Link will start transition from L1 to L0 state in 1uS (worst case)
    // to calculate L1 exit Latency we will use (MAX lat + 1000nS * (i-1)) where i number of links on the packet path.
    if(!L0s && i>0) lat+=1000*(i-1);

    DEBUG((DEBUG_VERBOSE,"    Total Calculated latency: %d (nS)\n", lat ));
    DEBUG((DEBUG_VERBOSE,"    --------------------------------------------------------\n"));

    return lat;
}


/**
    This function will Initialize "Device"'s Virtual Channel properties
    based on Setup Settings and hardware capabilities.

        
    @param Device Pointer to PCI Device Private Data.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieInitVc(PCI_DEV_INFO  *Device){
    EFI_STATUS          Status=EFI_SUCCESS;
    PCI_CFG_ADDR        addr;
    VC_RES_DATA            *vcrd;
    PCIE_VC_RESCNT_REG  vcrc;
    UINTN                c;
//-------------------------
    //Program VC configuration
    //Limit VC TC to TC0 mapped through VC0 and Disable all Extended VC resources on the way of the LINK.
    //This is a device driver responsibility to allow different TCs and enable Extended VCs if link has it.

    addr.ADDR=Device->Address.ADDR;
    //Arbitration Table has to be loaded by the Chipset specific code
    //in NBDxe.c or SBDxe.c or by device specific driver. So don't bother
    for(c=0; c<Device->PciExpress->VcData->VcCount; c++){
        vcrd=Device->PciExpress->VcData->VcResData[c];

        //Locate VC Resource CNT reg
        addr.Addr.ExtendedRegister=vcrd->VcResOffset+4;
        Status=PciCfg32(Device->RbIo,addr,FALSE,&vcrc.VC_RESCNT);
        ASSERT_EFI_ERROR(Status);

        //Disable VC and TC mapping except default mapping TC0 VC0
        if(c){
            vcrc.Tc_VcMap=0;
            vcrc.VcEnable=0;
        } else {
               //for Channel0(Default VC this bits must be RO but who knows...)
            vcrc.Tc_VcMap=1;
            vcrc.VcEnable=1;
        }
        vcrc.VcId=(UINT8)c;

        //write it back
        Status=PciCfg32(Device->RbIo,addr,TRUE,&vcrc.VC_RESCNT);
        ASSERT_EFI_ERROR(Status);
    }

    return Status;
}


/**
    This function will Select values for Link Control2 register on
    both sides of the LINK based on Setup Settings and hardware capabilities.

        
    @param DnPort Pointer to PCI Device Private Data of Downstream Port of the link.
    @param UpPort Pointer to PCI Device Private Data of Upstream Port of the link.
    @param Aspm ASPM value to program.
    @param LinkHotResetRequired Flag to modify if Link will need HOT RESET after programming.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetAspm(PCI_DEV_INFO *DnPort,PCI_DEV_INFO *UpPort,UINT16 Aspm/*, UINT8 AspmModeOption*/ ){
    EFI_STATUS          Status;
    PCIE_LNK_CNT_REG    lnkc;
    PCI_CFG_ADDR        addr;
    UINT16              aspm=0, aspmstore;//, dnaspm, upaspm;
    UINT8                 AspmModeOption=DnPort->PciExpress->Pcie1Setup.AspmMode;
//-------------------------
        
    DEBUG((DEBUG_VERBOSE,"\n Setting ASPM for Link...\n UP STREAM PORT -> [B%X|D%X|F%X] <--> [B%X|D%X|F%X] <- DN STREAM PORT\n",
            UpPort->Address.Addr.Bus,UpPort->Address.Addr.Device, UpPort->Address.Addr.Function,
            DnPort->Address.Addr.Bus,DnPort->Address.Addr.Device, DnPort->Address.Addr.Function ));

    //If we asked through Porting not to touch ASPM just leave...
    if((AspmModeOption==PCI_SETUP_DONT_TOUCH) || (UpPort->PciExpress->Pcie1Setup.AspmMode==PCI_SETUP_DONT_TOUCH)) {
        DEBUG((DEBUG_VERBOSE,"Setup is set PCI_SETUP_DONT_TOUCH for ASPM for this Link...Exitting\n" )); 
        return EFI_SUCCESS; 
    }

    //Spec tells: "ASPM L1 must be enabled by software in the Upstream
    //component on a Link prior to enabling ASPM L1 in the
    //Downstream component on that Link. When disabling ASPM
    //L1, software must disable ASPM L1 in the Downstream
    //component on a Link prior to disabling ASPM L1 in the
    //Upstream component on that Link. ASPM L1 must only be
    //enabled on the Downstream component if both components on
    //a Link support ASPM L1."
    //NOTE: Here is clarification Downstream COMPONENT appears to be an Upstream PORT of the link 

    //We have recorded in MaxLxLat field of the PciExpres Data Structure
    //Link capability to support certain Lx states.
    //value of 0xFFFF indicates corresponded Lx state is not supported.
    //It must be the same on both sides of the link!
    ASSERT(DnPort->PciExpress->MaxL0Lat == UpPort->PciExpress->MaxL0Lat);
    ASSERT(DnPort->PciExpress->MaxL1Lat == UpPort->PciExpress->MaxL1Lat);
    //If some links on the Pcie Packet Path does not support certain Lx states,
    //they will remain in L0 state all the time. Mean while ones that supports,
    //will transition to Lx power saving state.
    if( (UpPort->PciExpress->MaxL0Lat == 0xFFFF) ||
        !(UpPort->PciExpress->LinkCap.AspmSupport & PCIE_ASPM_L0_ENABLE) ||
        !(DnPort->PciExpress->LinkCap.AspmSupport & PCIE_ASPM_L0_ENABLE) ) Aspm &=(~PCIE_ASPM_L0_ENABLE);
    if( (UpPort->PciExpress->MaxL1Lat == 0xFFFF) ||
        !(UpPort->PciExpress->LinkCap.AspmSupport & PCIE_ASPM_L1_ENABLE) ||
        !(DnPort->PciExpress->LinkCap.AspmSupport & PCIE_ASPM_L1_ENABLE) ) Aspm &=(~PCIE_ASPM_L1_ENABLE);

    aspm=Aspm;

    //Check Setup settings "Force to L0s" must clear calculated PCIE_ASPM_L1_ENABLE;
    //But should not affect OVERWRITE capabilities.
    //[Disable]==0\ Auto==PCI_SETUP_AUTO_VALUE \ Force L0s==1
    //if(gPciSetupData->AspmMode == 1) aspm &= (~PCIE_ASPM_L1_ENABLE);
    if(AspmModeOption == 1) aspm &= (~PCIE_ASPM_L1_ENABLE);
    if(AspmModeOption == 0) aspm = 0;
    //Now "Aspm" has calculated MAX supported ASPM value
    //and "aspm" has Updated accordingly to setup

    //check UP and DN STREAM ASPM override settings.
    //and pick smallest one...

    DEBUG((DEBUG_VERBOSE,"Pcie: Aspm for the Link: Calc ASPM=%X; Setup ASPM=%X; ",Aspm, aspm));

    aspmstore=aspm;
    
    //All conditions for launching init routine checked inside  AmiPciLibLaunchInitRoutine() itself
    //so the previous code was inefficient.
    Status=AmiPciLibLaunchInitRoutine(DnPort, isPcieSetAspm, itDevice, DnPort, UpPort, &aspm, NULL);
    if(EFI_ERROR(Status)){
        if(Status==EFI_UNSUPPORTED){
            //just in case if function returned UNSUPPORTED modified ASPM settings - Restore it...
            aspm=aspmstore;
            DEBUG((DEBUG_VERBOSE,"\n  DnStreamPort: Don't Overwrite ASPM = 0x%X: %r\n", aspm, Status));
            
            //try Upstream port option since Dn stream did not changed anything...
            Status=AmiPciLibLaunchInitRoutine(UpPort, isPcieSetAspm, itDevice, DnPort, UpPort, &aspm, NULL);
                
            if(EFI_ERROR(Status)){
                if(Status==EFI_UNSUPPORTED){
                    aspm=aspmstore;
                    DEBUG((DEBUG_VERBOSE,"\n  UpStreamPort: Don't Overwrite ASPM = 0x%X: %r\n", aspm, Status));
                    //just in case if function returned UNSUPPORTED modified ASPM settings - Restore it...
                    Status=EFI_SUCCESS;
                } else {
                    ASSERT_EFI_ERROR(Status);
                    return Status;
                }
            } else {
                //this is SUCCESS status branch..
                DEBUG((DEBUG_VERBOSE,"\n  UpStreamPort: Overwrites ASPM=0x%X; Old ASPM=0x%X: %r\n", aspm, aspmstore, Status));
            }
            
            Status=EFI_SUCCESS;
        } else {
            ASSERT_EFI_ERROR(Status);
            return Status;
        }
    } else {
        //this is SUCCESS status branch..
        DEBUG((DEBUG_VERBOSE,"\n  DnStreamPort: Overwrites ASPM=0x%X; Old ASPM=0x%X: %r\n", aspm, aspmstore, Status));
    }


    //Check if ASPM settings were correctly overwritten
    //It can't be more than calculated... otherwise system might hung!
    if (aspm>Aspm){
        DEBUG((DEBUG_VERBOSE," !![Calc ASPM = %X] LESSER THAN [Ovr  ASPM = %X] - System Might HUNG. Reset to Calculated!!\n", Aspm, aspm));
        aspm=Aspm;
    }

    DEBUG((DEBUG_VERBOSE,"\n"));

    //Check here if Device Setup settings has PCIE Programming disabled...
    if(!PcieCheckPcieCompatible(DnPort)){
        DEBUG((DEBUG_VERBOSE,"\nPcie: Set ASPM Skipped DN STREAM PORT @ [B%X|D%X|F%X] don't support PCIe\n",
                DnPort->Address.Addr.Bus,DnPort->Address.Addr.Device, DnPort->Address.Addr.Function ));
        return EFI_SUCCESS;
    }

    if(!PcieCheckPcieCompatible(UpPort)){
        DEBUG((DEBUG_VERBOSE,"\nPcie: Set ASPM Skipped UP STREAM PORT @ [B%X|D%X|F%X] don't support PCIe\n",
                UpPort->Address.Addr.Bus,UpPort->Address.Addr.Device, UpPort->Address.Addr.Function ));
        return EFI_SUCCESS;
    }

    //ASPM is Disabled at that point.
    if(aspm) {
        //Upstream Component - the one that has Downstream Port!
        //Select lnk Control Register of Down Stream port
        addr.ADDR=DnPort->Address.ADDR;
        addr.Addr.Register=DnPort->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
        //Read what we have there
        Status=PciCfg16(DnPort->RbIo,addr,FALSE,&lnkc.LNK_CNT);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;

        //Set ASPM for Downstream Port
        lnkc.AspmControl=aspm;

        //Write Back
        Status=PciCfg16(DnPort->RbIo,addr,TRUE,&lnkc.LNK_CNT);
        ASSERT_EFI_ERROR(Status);

        
        //We are coming here with UpPort as Function 0 already.
        addr.ADDR=UpPort->Address.ADDR;

        //Select lnk Control Register
        addr.Addr.Register=UpPort->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
        //Read what we have there
        Status=PciCfg16(UpPort->RbIo,addr,FALSE,&lnkc.LNK_CNT);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;
        
        //Updating ASPM settings...
        lnkc.AspmControl=aspm;

        //Set ASPM in UpStream Port First for Function 0
        Status=PciCfg16(UpPort->RbIo,addr,TRUE,&lnkc.LNK_CNT);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;

        //It is recommended to program all functions even in
        //case of ARI device where Func0 is in charge for link behavior.
        if(IsFunc0OfMfDev(UpPort)){
            UINTN           i;
            PCI_DEV_INFO    *dev;
        //--------------------------
            for(i=0; i<UpPort->FuncCount; i++){
                dev=UpPort->DevFunc[i];

                if(!PcieCheckPcieCompatible(dev)) continue;

                addr.ADDR=dev->Address.ADDR;
                addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
                //Read what we have there
                Status=PciCfg16(dev->RbIo,addr,FALSE,&lnkc.LNK_CNT);
                ASSERT_EFI_ERROR(Status);
                if(EFI_ERROR(Status)) return Status;

                lnkc.AspmControl=aspm;

                Status=PciCfg16(dev->RbIo,addr,TRUE,&lnkc.LNK_CNT);
                ASSERT_EFI_ERROR(Status);
                if(EFI_ERROR(Status)) return Status;
            }

        }

    }//if(Aspm)

    return Status;
}


/**
    This function will Select values for DEVICE CONTROL register
    based on Setup Settings and hardware capabilities.

        
    @param Device Pointer to PCI Device Private Data.
    @param MaxPayload Pointer to the MaxPayload value.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetDevProperties(PCI_DEV_INFO *Device, UINT16 MaxPayload)
{
    EFI_STATUS          Status;
    PCI_CFG_ADDR        addr;
    PCI_DEV_INFO        *dev=Device;
    PCIE_DEV_CNT_REG    devc;
    UINT8                MaxPayloadOption=dev->PciExpress->Pcie1Setup.MaxPayload;
    UINT8                ExtTagFieldOption=dev->PciExpress->Pcie1Setup.ExtTagField;
    UINT8                RelaxedOrderingOption=dev->PciExpress->Pcie1Setup.RelaxedOrdering;
    UINT8                MaxReadRequestOption=dev->PciExpress->Pcie1Setup.MaxReadRequest;
    UINT8                NoSnoopOption=dev->PciExpress->Pcie1Setup.NoSnoop;
//-------------------------
    addr.ADDR=Device->Address.ADDR;

    DEBUG((DEBUG_INFO,"PciE: PcieSetDevProperties() Device @ [B%X|D%X|F%X] ENTRY:\n",
            addr.Addr.Bus,addr.Addr.Device, addr.Addr.Function));
    
    DEBUG((DEBUG_VERBOSE,"PciE: MaxPayloadOption=%x ExtTagFieldOption =%x RelaxedOrderingOption=%x,MaxReadRequestOption=%x:NoSnoopOption=%x\n",
            MaxPayloadOption,ExtTagFieldOption,RelaxedOrderingOption,MaxReadRequestOption,NoSnoopOption));

    if (!PcieCheckPcieCompatible(Device))
        return EFI_SUCCESS;

    //Select Device Control Register
    addr.Addr.Register=dev->PciExpress->PcieOffs+PCIE_DEV_CNT_OFFSET;
    //Read what we have there
    Status=PciCfg16(dev->RbIo,addr,FALSE,&devc.DEV_CNT);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    DEBUG((DEBUG_VERBOSE," DEV_CNT -> [FrBr=%X|MRR=%X|NS=%X|APPM=%X|PhF=%X|ETg=%X|MPL=%X|RO=%X|UpRR=%X|FtEr=%X|NfEr=%X|CEr=%X]\n    ....... UPDATINNG .......\n",
                devc.FnRstBrgRtry,
                devc.MaxReadReq,
                devc.NoSnoop,
                devc.AuxPwrPm,
                devc.PhantomFunc,
                devc.ExtTagFld,
                devc.MaxPayload,
                devc.RelaxOrder,
                devc.UsupReqRep,
                devc.FatalErrRep,
                devc.NfErrRep,
                devc.CorrErrRep));

    //Set some bits:
    //1.Get Maximum PayLoad Size
    //Don't allow overwrite if it is more than supported
    if(MaxPayloadOption!=PCI_SETUP_DONT_TOUCH){
        if(MaxPayloadOption!=PCI_SETUP_AUTO_VALUE && MaxPayloadOption < MaxPayload) devc.MaxPayload=MaxPayloadOption;
        else devc.MaxPayload=MaxPayload;
        dev->PciExpress->MaxPayload=MaxPayload;
    } else dev->PciExpress->MaxPayload=devc.MaxPayload;
    
    
    //2.Get Max Read Request Size
    if(MaxReadRequestOption!=PCI_SETUP_DONT_TOUCH){
        if(MaxReadRequestOption!=PCI_SETUP_AUTO_VALUE && MaxReadRequestOption<=PCIE_MAXPL_4096B) devc.MaxReadReq=MaxReadRequestOption;
        //if "AUTO" selected use value already sitting there...
        //    else devc.MaxReadReq=PCIE_MAXPL_512B;
    }
    
    //3.Enable/Disable extended tagfld per Setup Option
    if(ExtTagFieldOption != PCI_SETUP_DONT_TOUCH){
        devc.ExtTagFld=0; //use 5 bit tag.
        if(dev->PciExpress->DeviceCap.ExtTagFld)devc.ExtTagFld=ExtTagFieldOption;
    }
    
    //4. Set No Snoop bit per Setup Option
    if(RelaxedOrderingOption != PCI_SETUP_DONT_TOUCH) devc.RelaxOrder=RelaxedOrderingOption;
    if(NoSnoopOption != PCI_SETUP_DONT_TOUCH) devc.NoSnoop=NoSnoopOption;

    //Write it back
    Status=PciCfg16(dev->RbIo,addr,TRUE,&devc.DEV_CNT);
    ASSERT_EFI_ERROR(Status);

    DEBUG((DEBUG_VERBOSE," DEV_CNT -> [FrBr=%X|MRR=%X|NS=%X|APPM=%X|PhF=%X|ETg=%X|MPL=%X|RO=%X|UpRR=%X|FtEr=%X|NfEr=%X|CEr=%X]<-Buffer\n",
                devc.FnRstBrgRtry,
                devc.MaxReadReq,
                devc.NoSnoop,
                devc.AuxPwrPm,
                devc.PhantomFunc,
                devc.ExtTagFld,
                devc.MaxPayload,
                devc.RelaxOrder,
                devc.UsupReqRep,
                devc.FatalErrRep,
                devc.NfErrRep,
                devc.CorrErrRep));

    //SetFlag DevProgrammed FLAG..
    dev->PciExpress->DevSet=TRUE;

    //Reread it
    Status=PciCfg16(dev->RbIo,addr,FALSE,&devc.DEV_CNT);
    ASSERT_EFI_ERROR(Status);
    
    DEBUG((DEBUG_VERBOSE," DEV_CNT -> [FrBr=%X|MRR=%X|NS=%X|APPM=%X|PhF=%X|ETg=%X|MPL=%X|RO=%X|UpRR=%X|FtEr=%X|NfEr=%X|CEr=%X]<-Rgister\n\n",
                devc.FnRstBrgRtry,
                devc.MaxReadReq,
                devc.NoSnoop,
                devc.AuxPwrPm,
                devc.PhantomFunc,
                devc.ExtTagFld,
                devc.MaxPayload,
                devc.RelaxOrder,
                devc.UsupReqRep,
                devc.FatalErrRep,
                devc.NfErrRep,
                devc.CorrErrRep));
    

#if (PCI_SETUP_USE_APTIO_4_STYLE == 0)    
    Status = AmiPciGetCommonSetupData(&gPciCommonSetupData);
#else
    Status = AmiPciGetSetupData(NULL, &gPciCommonSetupData, NULL);
#endif        
    
    //Init device's VC
    //if we have a virtual channel capability
    if((Device->PciExpress->VcData!=NULL) && (gPciCommonSetupData.DontResetVcMapping==FALSE)) {
        Status=PcieInitVc(Device);
    }

    return Status;

}

/**
    This function will Select values for DEVICE CONTROL register
    based on Setup Settings and hardware capabilities.

        
    @param EndPoint Pointer to PCI Device Private Data.
    @param MaxPayload MaxPayload value.
    @param LatencyL0s Latency L0s value.
    @param LatencyL1 Latency L1 value.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieApplyDevSettings(PCI_DEV_INFO *EndPoint, UINT16 MaxPayLoad, UINT16 LatencyL0s, UINT16 LatencyL1){
    EFI_STATUS          Status=EFI_SUCCESS;
    PCI_DEV_INFO        *dn, *up, *f0=NULL;
    UINTN               i;
    UINT16              aspmc=0, mpl=0;
    BOOLEAN             pe2;
//--------------------
    //if controls comes here means the "EndPoint" in already EpList[]...
//    up=EndPoint;
    //This condition will never happened now 
    //we are checking PCIe compatibility before calling this function and the one that invokes this
//    if(!PcieCheckPcieCompatible(EndPoint)){
        //if PCIe capabilities get overridden by isGetSetupData IntRoutine or PciPort->PciPortCheckBadPcieDevice,
        //we should remove device from endpoint list...
//        Status=PcieRemoveEpFromLst(EndPoint);
//        DEBUG((DEBUG_VERBOSE,"\nPciE: PcieApplyDevSettings() -> \n  Removing [B%X|D%X|F%X] from EpList: NOT PCIe Compatible! %r\n",
//        up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function, Status));

//        return EFI_SUCCESS;
//    }

    up=EndPoint;
    if(!IsFunc0(up)) up=up->Func0;

    if(up->PciExpress->DeviceCap.EpL0Latency >= LatencyL0s) aspmc |= PCIE_ASPM_L0_ENABLE;
    if(up->PciExpress->DeviceCap.EpL1Latency >= LatencyL1) aspmc |= PCIE_ASPM_L1_ENABLE;


    DEBUG((DEBUG_VERBOSE,"\nPciE: PcieApplyDevSettings() -> Starting with Device [B%X|D%X|F%X]\n",
    up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function));


    //Check if we need to initialize Pcie2 features as well
    //it will happened if both sides of the link supports PcieGen2 spec

    pe2=Pcie2CheckPcie2Compatible(up);

    do{
        if(up==NULL || up->ParentBrg==NULL)break;
        f0=NULL;
        dn=up->ParentBrg;
        
        //optimal MPL calculated and passed here;
        mpl=MaxPayLoad;

        //Set features selected in SETUP
        DEBUG((DEBUG_VERBOSE,"#---> EndPoint/UPSTREAM @ [B%X|D%X|F%X] ASPM=%X; MPL=%X;\n",
            up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function, aspmc, mpl));

        if(pe2){
            //If applicable
            //Set Device features for GEN2 devices
            Status=Pcie2SetDevProperties(up);
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)) return Status;
        }

        Status=PcieSetDevProperties(up, mpl);
           ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;

        //Remove processed entry from EndPoint Database...
        Status=PcieRemoveEpFromLst(up, FALSE);


        //If we got an EndPoint and it is a Multi Func device - go and update all it's functions...
        if(up->PciExpress->PcieCap.PortType == PCIE_TYPE_ENDPOINT ||
           up->PciExpress->PcieCap.PortType == PCIE_TYPE_PCIE_PCI ||
           up->PciExpress->PcieCap.PortType == PCIE_TYPE_LEGACYEP ||
           up->PciExpress->PcieCap.PortType == PCIE_TYPE_UPS_PORT )
        {
            f0=up;
            for (i=0; i<f0->FuncCount; i++){
                up=f0->DevFunc[i];

                DEBUG((DEBUG_VERBOSE,"#---> EndPoint/UPSTREAM Function @ [B%X|D%X|F%X] ASPM=%X; MPL=%X;\n",
                        up->Address.Addr.Bus,up->Address.Addr.Device, up->Address.Addr.Function, aspmc, mpl));

                if(!PcieCheckPcieCompatible(up)) continue;

//#if PCI_EXPRESS_GEN2_SUPPORT == 1
                if(pe2){
                    //If applicable
                    //Set Device features for GEN2 devices
                    Status=Pcie2SetDevProperties(up);
                    ASSERT_EFI_ERROR(Status);
                    if(EFI_ERROR(Status)) return Status;
                }
//#endif

                //Set features selected in SETUP
                Status=PcieSetDevProperties(up, mpl);
                ASSERT_EFI_ERROR(Status);
                if(EFI_ERROR(Status)) return Status;

                //SetFlag DevProgrammed FLAG..
                up->PciExpress->DevSet=TRUE;

                //Remove processed entry from EndPoint Database...
                Status=PcieRemoveEpFromLst(up, FALSE);
            } //for
        }//if Mf endpoint

        //Restore "up" var
        if(f0!=NULL) up=f0;

        DEBUG((DEBUG_VERBOSE,"<---# RootComp/DNSTREAM @ [B%X|D%X|F%X] ASPM=%X; MPL=%X;\n",
                dn->Address.Addr.Bus,dn->Address.Addr.Device, dn->Address.Addr.Function, aspmc, mpl));

        //now do the DownStream Port... it always ONLY one device!
//#if PCI_EXPRESS_GEN2_SUPPORT == 1
        pe2=Pcie2CheckPcie2Compatible(dn);
        if(pe2){
            //If applicable
            //Set Device features for GEN2 devices
            Status=Pcie2SetDevProperties(dn);
            ASSERT_EFI_ERROR(Status);
            if(EFI_ERROR(Status)) return Status;
        }
        Status=PcieSetDevProperties(dn,mpl);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;


        //Aspm is off at that point.
        //We should turn it on first in UPSTREAM PORT, then IN DOWNSTREAM port.
        //Since we can overwrite Settings
        Status=PcieSetAspm(dn,up,aspmc);
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) return Status;

        //SetFlag DevProgrammed FLAG..
        dn->PciExpress->DevSet=TRUE;

        //Remove processed entry from EndPoint Database...
        //Status=PcieRemoveEpFromLst(dn);

        //Go to the next link if any...
        up=dn->ParentBrg;
    } while (up!=NULL && up->ParentBrg!=NULL && up->ParentBrg->Type!=tPciRootBrg && PcieCheckPcieCompatible(up->ParentBrg)); //(EIP49743)

    return Status;
}


/**
    This function will detect optimal settings and program -
    MaxPayLoadSize, ReadRequestSize, ASPM settings and Virtual Channels
    for all devices in the PCIE Links Chain.

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  
  RootPortComplex<=>Switch0...<=>...SwitchN<=>EndPoint
**/
EFI_STATUS PcieInitDevChain(PCI_DEV_INFO* Device ){
    EFI_STATUS            Status=0;
    UINT16                mpl;
    UINTN               lt0,lt1;        //Latency in Nano Seconds.
    UINT16                reg_lt0=0,reg_lt1=0;//Latency to be written into Register.
//-----------------------
    //Now we will program clock config when exiting from the bridge
    //Collect Min Supported feature at the length of the PCIE Devices Chain
    //Get MaxPayLoad
    mpl=PcieCalcMaxPayLoad(Device);

    //After PcieCalcMaxPayLoad() Payload has to be adjusted DOWN STREAM already (towards EP Device)...
    //When We come here ASPM already disabled by PcieProbeDevice() function.
    //Since we are going to use Porting Hook to overwrite ASPM settings Calculate Latency ALWAYS
    //and Enable it only if Setup says so...- will be done in PcieSetAspm()

    //Get L0s L1 exit Latencies if needed...
    lt0=PcieCalcLatency(Device,TRUE);
    lt1=PcieCalcLatency(Device,FALSE);

       //now we should add CIPSET internal L0s Exit Latency to the total value
    //For NB if any
    if( (NbDmiAspmSupport & PCIE_ASPM_L0_ENABLE ) && (SbDmiAspmSupport & PCIE_ASPM_L0_ENABLE )){
        if(NbDmiL0ExitLatency > SbDmiL0ExitLatency) lt0 += NbDmiL0ExitLatency;
        else lt0 += SbDmiL0ExitLatency;
    }
    //and SB if any
    reg_lt0+=(UINT16) PcieConvertLatency(lt0, TRUE, FALSE);


       //now we should add CIPSET L1 Exit Latency to the total value
    if( (NbDmiAspmSupport & PCIE_ASPM_L1_ENABLE ) && (SbDmiAspmSupport & PCIE_ASPM_L1_ENABLE )){
        if(NbDmiL1ExitLatency > SbDmiL1ExitLatency) lt1 += NbDmiL1ExitLatency;
        else lt1 += SbDmiL1ExitLatency;
    }
    //and SB
    lt1+=SbInternalDelay;
    reg_lt1+=(UINT16) PcieConvertLatency(lt1, FALSE, FALSE);


    //Apply collected values to the devices
    Status=PcieApplyDevSettings(Device, mpl, reg_lt0, reg_lt1);

    return Status;
}

/**
    This function will Init VC_DATA structure of the "Device".

        
    @param Device Pointer to PCI Device Private Data.
    @param DevAddr Pointer to Device Address value.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieInitDeviceVc(PCI_DEV_INFO *Device, PCI_CFG_ADDR *DevAddr){
    EFI_STATUS      Status;
    VC_DATA            *vcd=MallocZ(sizeof(VC_DATA));
    VC_RES_DATA        *vcrd;
    UINTN            i;
    PCI_CFG_ADDR    addr;
//-----------------
    addr.ADDR=DevAddr->ADDR;
    if(!vcd) return EFI_OUT_OF_RESOURCES;

    Device->PciExpress->VcData=vcd;
    vcd->VcOffset=addr.Addr.ExtendedRegister;
        //Read and store VC Cap 1 reg
    addr.Addr.ExtendedRegister=vcd->VcOffset+PCIE_PORTVC_CAP1_OFFSET;
    Status=PciCfg32(Device->RbIo,addr,FALSE,&vcd->VcCap1.PORTVC_CAP1);
    ASSERT_EFI_ERROR(Status);

    //Read and store VC Cap 2 reg
    addr.Addr.ExtendedRegister=vcd->VcOffset+PCIE_PORTVC_CAP2_OFFSET;
    Status=PciCfg32(Device->RbIo,addr,FALSE,&vcd->VcCap2.PORTVC_CAP2);
    ASSERT_EFI_ERROR(Status);

        //Collect VC resource info...
    for(i=0; i<vcd->VcCap1.ExtVcCnt+vcd->VcCap1.LPExtVcCnt+1; i++){
        vcrd=MallocZ(sizeof(VC_RES_DATA));
        if(!vcrd) return EFI_OUT_OF_RESOURCES;
        addr.Addr.ExtendedRegister=(UINT32)(vcd->VcOffset+PCIE_VC_RES_CAP_OFFSET(i));
        //Record Offset of the VC resource capability register
        vcrd->VcResOffset=addr.Addr.ExtendedRegister;
        Status=PciCfg32(Device->RbIo,addr,FALSE,&vcrd->VcResCap.VC_RESCAP);
        ASSERT_EFI_ERROR(Status);

        Status=AppendItemLst((T_ITEM_LIST*)&vcd->InitCnt, vcrd);
        if(EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
    }
    return Status;
}

/**
    This function will Initialize PCIE Device to it's default settings
        
    @param Device Pointer to PCI Device Private Data structure.
    @param MaxBusFound maximum number of pci Bus.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  The function will do the following steps
        1.disable ASPM if any;
        2.disable PMI Generation if any;
        3.power up PCIE Slot (if capable) if the slot has device behind it;
        4.if nothing connected to slot, power it down to make it HOTPLUG READY;
**/
EFI_STATUS PcieInitDevice(PCI_DEV_INFO *Device, UINT8 *MaxBusFound){
    PCI_CFG_ADDR        addr;
    PCIE_DEV_CNT_REG    devc;
    PCIE_LNK_CNT_REG    lnkc;
    EFI_STATUS          Status=EFI_SUCCESS;
//--------------------------------

    addr.ADDR=Device->Address.ADDR;
    addr.Addr.Register=0; 
    addr.Addr.ExtendedRegister=0;
    if(PcieCheckPcieCompatible(Device)){
        //Get PCI Express Slot it is valid only for Ports Switches and Bridges
        if((Device->Type==tPci2PciBrg) || (Device->Type==tPci2CrdBrg)) PcieSetSlotProperties(Device);
    
        //Disable Device Control Reg functions we'll not need now
        addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_DEV_CNT_OFFSET;
        Status=PciCfg16(Device->RbIo,addr,FALSE,&devc.DEV_CNT);
        ASSERT_EFI_ERROR(Status);
        //Set Default values
        if(!Device->PciExpress->Pcie1Setup.DevCtrDontTouch){
            devc.CorrErrRep=0;  //bit 0
            devc.NfErrRep=0;    //bit 1
            devc.FatalErrRep=0; //bit 2
            devc.UsupReqRep=0;  //bit 3
            devc.RelaxOrder=1;  //bit 4
            devc.MaxPayload=PCIE_MAXPL_128B;//bit 5..7
            devc.ExtTagFld=0;   //bit 8
            devc.PhantomFunc=0; //bit 9
            devc.NoSnoop=1;     //bit 11
            //Don't touch MRR value it might be device dependent and may cause system hang
            //devc.MaxReadReq=PCIE_MAXPL_512B;//bit 12..14
    
            Status=PciCfg16(Device->RbIo,addr,TRUE,&devc.DEV_CNT);
            ASSERT_EFI_ERROR(Status);
        }
    
        //Get what we have in LNK_CNT_REG
        addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_CNT_OFFSET;
        Status=PciCfg16(Device->RbIo,addr,FALSE,&lnkc.LNK_CNT);
        ASSERT_EFI_ERROR(Status);
    
        //Disable ASPM
        if(!Device->PciExpress->Pcie1Setup.LnkCtrDontTouch){
            lnkc.AspmControl=PCIE_ASPM_DISABLE;
            lnkc.ExtSynch=0;
    
            Status=PciCfg16(Device->RbIo,addr,TRUE,&lnkc.LNK_CNT);
            ASSERT_EFI_ERROR(Status);
        }
        //Set CRS option according to SDL token.
        if(Pcie2CheckPcie2Compatible(Device)) Status=Pcie2SetCrs(Device);
        ASSERT_EFI_ERROR(Status);
    }
    return Status;

}


/**
    This function will collect information about PCIE Device

        
    @param Device Pointer to PCI Device Private Data structure.
    @param SriovSupport TRUE/FALSE.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  The function will do the following steps
**/
//EFI_STATUS PcieProbeDevice(PCI_DEV_INFO *Device, UINT8 *MaxBusFound, BOOLEAN SriovSupport){
EFI_STATUS PcieProbeDevice(PCI_DEV_INFO *Device, BOOLEAN SriovSupport){
    PCI_CFG_ADDR        addr;
    //PCIE_DEV_CNT_REG    devc;
    //PCIE_LNK_CNT_REG    lnkc;
    PCIE_EXT_CAP_HDR    extcap;
    PCIE_LNK_STA_REG    lnks;
    EFI_STATUS            Status;
//-------------------------------------

    //init fields that not suppose to be 0
    Device->PciExpress->MaxL0Lat=0xFFFF;
    Device->PciExpress->MaxL1Lat=0xFFFF;

    Status=PcieGetCaps(Device);
    ASSERT_EFI_ERROR(Status);

    //Get common Lnk Properties from LNK_STA reg..
    addr.ADDR=Device->Address.ADDR;
    addr.Addr.ExtendedRegister=0;
    addr.Addr.Register=Device->PciExpress->PcieOffs+PCIE_LNK_STA_OFFSET;
    Status=PciCfg16(Device->RbIo,addr,FALSE,&lnks.LNK_STA);
    ASSERT_EFI_ERROR(Status);

    //Update data
    Device->PciExpress->CurrLnkSpeed=(UINT8)lnks.LnkSpeed;
    Device->PciExpress->CurrLnkWidth=(UINT8)lnks.LnkWidth;

     
    //if PciExpress Capability Reg if it supports PCIe 2.1
    Status=Pcie2AllocateInitPcie2Data(Device);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;
    
    //Check Pci Express Extended Capability header
    addr.Addr.ExtendedRegister=0x100;
    //if something there advance through EXT capabilities list to find VC Cap
    while(addr.Addr.ExtendedRegister){
        Status=PciCfg32(Device->RbIo,addr,FALSE,&extcap.EXT_CAP_HDR);
        ASSERT_EFI_ERROR(Status);
        //Check if Extended PciExpress Config Space is accessible.
        if(extcap.ExtCapId==0xFFFF) break;    //(EIP19144+)

        //we've got VC Cap header
        if(extcap.ExtCapId==PCIE_CAP_ID_VIRTUAL_CH){
            DEBUG((DEBUG_VERBOSE,"PciE2: Found VIRTUAL_CHNL Ext Cap Header @ offset 0x%X\n", addr.Addr.ExtendedRegister));
            Status=PcieInitDeviceVc(Device, &addr);
            ASSERT_EFI_ERROR(Status);
        }

//#if SRIOV_SUPPORT
        if (extcap.ExtCapId==PCIE_CAP_ID_SRIOV && SriovSupport ){
            Status=SriovAllocateSriovData(Device, addr.Addr.ExtendedRegister);
            ASSERT_EFI_ERROR(Status);
        }
//#endif

//#if PCI_EXPRESS_GEN2_SUPPORT
        if(Pcie2CheckPcie2Compatible(Device)){
        //Check and collect capability (read only) data from devices
        //1. Alternative Routing ID capabilities
        if (extcap.ExtCapId==PCIE_CAP_ID_ARI){
            DEBUG((DEBUG_VERBOSE,"PciE2: Found ARI Ext Cap Header @ offset 0x%X\n", addr.Addr.ExtendedRegister));
            Status=PcieCollectDeviceExtCap(Device, (VOID**) &Device->PciExpress->AriData, sizeof(PCIE2_ARI_DATA),
                    addr.Addr.ExtendedRegister, TRUE, 0); // 16 bit cap reg
            ASSERT_EFI_ERROR(Status);
        }

        //2. Access Control Services
        if (extcap.ExtCapId==PCIE_CAP_ID_ACS){
            DEBUG((DEBUG_VERBOSE," PciE2: Found ACS Ext Cap Header @ offset 0x%X\n", addr.Addr.ExtendedRegister));
            Status=PcieCollectDeviceExtCap(Device, (VOID**)&Device->PciExpress->AcsData, sizeof(PCIE2_ACS_DATA),
                    addr.Addr.ExtendedRegister, TRUE, 0); // 16 bit cap reg
            ASSERT_EFI_ERROR(Status);
        }

        //3. Address Translation services
        if (extcap.ExtCapId==PCIE_CAP_ID_ATS){
            DEBUG((DEBUG_VERBOSE,"PciE2: Found ATS Ext Cap Header @ offset 0x%X\n", addr.Addr.ExtendedRegister));
            Status=PcieCollectDeviceExtCap(Device, (VOID**) &Device->PciExpress->AtsData, sizeof(PCIE2_ATS_DATA),
                    addr.Addr.ExtendedRegister, TRUE, 0); // 16 bit cap reg
            ASSERT_EFI_ERROR(Status);
        }

        //4. RCRB Ext header for alternative CRS settings...
        if (extcap.ExtCapId==PCIE_CAP_ID_RCRB){
            DEBUG((DEBUG_VERBOSE,"PciE2: Found RCRB Ext Cap Header @ offset 0x%X\n", addr.Addr.ExtendedRegister));
            Status=PcieCollectDeviceExtCap(Device, (VOID**) &Device->PciExpress->RcrbData, sizeof(PCIE2_RCRB_DATA),
                    addr.Addr.ExtendedRegister, FALSE, 4); // 32 bit cap reg
            ASSERT_EFI_ERROR(Status);
            
            Status=Pcie2SetCrsExt(Device);
            ASSERT_EFI_ERROR(Status);
        }
        
        //check for Secondary PCIe Extended Capability header.
        //must be implemented if device supports GEN 3 speed of 8.0 GT/s
        if (extcap.ExtCapId==PCIE_CAP_ID_SEC_PCIE_CAP){
            //DEBUG((DEBUG_VERBOSE,"PciE3: Found SEC PCIe Ext Cap Header @ offset 0x%X\n", addr.Addr.ExtendedRegister));
            //Status=PcieCollectDeviceExtCap(Device, &Device->PciExpress->Pcie3, sizeof(PCIE3_DATA),
            //        addr.Addr.ExtendedRegister, FALSE, 4); // 32 bit cap reg
            Status=Pcie3AllocateInitPcie3Data(Device,addr.Addr.ExtendedRegister);
            ASSERT_EFI_ERROR(Status);
        }
        
        // check for Physical Layer 16.0 GT/s Extended Capability (GEN 4)
        // must be implemented if device supports GEN 4 speed of 16.0 GT/s
        if (extcap.ExtCapId==PCI_EXPRESS_EXTENDED_CAPABILITY_PHYSICAL_LAYER_16_0_ID){
            Status=Pcie4AllocateInitPcie4Data(Device,addr.Addr.ExtendedRegister);
            ASSERT_EFI_ERROR(Status);
        }
        
        } //if GEN2_SUPPORT
        //4. Internal RC Link
        if (extcap.ExtCapId==PCIE_CAP_ID_RC_INT_LNK){
            DEBUG((DEBUG_VERBOSE,"PciE2: Found INT_LINK Ext Cap Header @ offset 0x%X\n", addr.Addr.ExtendedRegister));
            Status=PcieCollectDeviceExtCap(Device,(VOID**) &Device->PciExpress->RcLnkData, sizeof(PCIE_RCLNK_DATA),
                    addr.Addr.ExtendedRegister, FALSE, 0); // 32 bit cap reg
            ASSERT_EFI_ERROR(Status);
        }

        addr.Addr.ExtendedRegister=extcap.NextItemPtr;
    } //while loop searching for ext caps


    //If device is Upstream Port type (Link Terminating element) 
    //add it to an EndPoint device list.
    if(!PcieIsDownStreamPort(Device)){
        Status=AppendItemLst(&gPcieEpList, Device);
        DEBUG((DEBUG_VERBOSE,"PciE: Adding Device [B%X|D%X|F%X] to gPcieEpList[%d]=%r\n\n",
        Device->Address.Addr.Bus,Device->Address.Addr.Device, Device->Address.Addr.Function, gPcieEpList.ItemCount, Status));
        ASSERT_EFI_ERROR(Status);
    }

    return Status;
}

/**
    Checks and initializes according to the setup settings 
    Integrated PCI Express Device like Integrated End Point and RC 
    Event Collector. This Device/Port type value == PCIE_TYPE_RC_INTEGR_EP
    or PCIE_TYPE_RC_EVT_COLLECT don't have a Link associated with them and 
    from standard PCIe registers have only Device CAP|CTL|STA registers set.
    This devices not supporting ASPM and they can accept only below settings
    - Enable Relaxed Ordering;
    - Extended Tag Field Enable;
    - Enable No Snoop;
    - Max_Payload_Size;
    - Max_Read_Request_Size;

        
    @param IntPcieDev a Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieCheckIntegratedDevice(PCI_DEV_INFO *IntPcieDev){
    EFI_STATUS  Status=EFI_NOT_FOUND;
//-------------------------
    //check if this is an INTEGRATED DEVICE...
    if(IntPcieDev->PciExpress->PcieCap.PortType==PCIE_TYPE_RC_INTEGR_EP || 
        IntPcieDev->PciExpress->PcieCap.PortType==PCIE_TYPE_RC_EVT_COLLECT)
    { 
        DEBUG((DEBUG_INFO,"@@@@@@@@ INTEGRATED PCIe Device @ [B%X|D%X|F%X] @@@@@@@@:\n", 
                IntPcieDev->Address.Addr.Bus,IntPcieDev->Address.Addr.Device, IntPcieDev->Address.Addr.Function));                
        DEBUG((DEBUG_INFO,"------------------------------------------------------------\n"));
        DEBUG((DEBUG_VERBOSE,"PciBus: PcieCheckIntegratedDevice():\n"));

        //If Pcie2 Capable...
        if (Pcie2CheckPcie2Compatible(IntPcieDev)){
            Status=Pcie2SetDevProperties(IntPcieDev);
            if(EFI_ERROR(Status)) return Status;
        }
        
        //Set features selected in SETUP.. for Integrated devices MPL always == Max Supported.
        Status=PcieSetDevProperties(IntPcieDev, IntPcieDev->PciExpress->DeviceCap.MaxPayload);
        if(EFI_ERROR(Status)) return Status;
    
        //SetFlag DevProgrammed FLAG..
        IntPcieDev->PciExpress->DevSet=TRUE;
    }  
        
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
