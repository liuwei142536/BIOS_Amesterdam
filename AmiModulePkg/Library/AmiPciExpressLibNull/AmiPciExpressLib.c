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
    Library Class for AMI Pci Express Support Functions. Dummy Instance


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
#include <Library/AmiPciExpressGen2Lib.h>
#include <Library/AmiPciExpressGen3Lib.h>

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
T_ITEM_LIST  gPcieEpList = {0,0,NULL};

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
    Checks if PCIe features supported by the Device
    
    @param Device pointer to device internal data. 

    @retval BOOLEAN
        TRUE    Device is Function 0 of Multifunctional device.
        FALSE   Device is not Function 0 of Multifunctional device.

**/
BOOLEAN PcieCheckPcieCompatible(PCI_DEV_INFO	*Device){
	return FALSE;
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
    return EFI_SUCCESS;
}

/**
    This function will create S3 Resume Boot Script
    for Device's Virtual Channels feature initialization if device
    supports Virtual Channels.

        
    @param Brg Pointer to PCI Device Private Data structure.
    @param S3SaveStateProtocol Pointer to EFI_S3_SAVE_STATE_PROTOCOL instance

    @retval VOID

**/
VOID PcieRecordBootScript(PCI_DEV_INFO *Brg, EFI_S3_SAVE_STATE_PROTOCOL *S3SaveStateProtocol, BOOLEAN RecordScript){
    return;
}


/**
    This function will reprogram Max Payload and Read Request Size
    in upstream link if initialization code founds that one of Nodes down stream
    has lesser Max Pay load size than currently programmed

        
    @param Device Pointer to PCI Device Private Data structure.
    @param MaxPayload Reduced Max Payload Value.
    @param MaxReadReq Reduced Read Request Value.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  
  RootPortComplex<=>Switch0...<=>...SwitchN<=>EndPoint
**/
EFI_STATUS PcieProgramPayloadUp(PCI_DEV_INFO *Device, UINT16 MaxPayload, UINT16 MaxReadReq){
    return EFI_SUCCESS;
}

/**
    This function will calculate MAX PayLoad Size needed to work
    correctly.

        
    @param EndPoint Pointer to PCI Device Private Data structure.

    @retval 
        UINT16 MaxPayLoad


**/
UINT16 PcieCalcMaxPayLoad(PCI_DEV_INFO *EndPoint){
    return EFI_SUCCESS;
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
        UINTN               Converted Value (-1) - means Invalid Value passed;

**/
//Return value in nS of Max L0s, L1 Exit latency .
UINTN PcieConvertLatency(UINTN Latency, BOOLEAN L0s, BOOLEAN ToNs){
        return 0;
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
	return EFI_SUCCESS;
}

/**
    This function will update Device's referenced as "Func0"
    Clock Power Management based on Setup Settings and hardware capabilities.

        
    @param Func0 Pointer to PCI Device Private Data structure.
    @param LnkCnt Pointer to the Device's LNK_CNT register data.

    @retval NOTHING

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
VOID PcieUpdateClockPm(PCI_DEV_INFO *Func0, PCIE_LNK_CNT_REG *LnkCnt){
    return;
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
    return EFI_SUCCESS;
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
    return EFI_SUCCESS;
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
    return EFI_SUCCESS;

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
    return EFI_SUCCESS;
}


/**
    This function will Initialize SLT_CNT and SLT_STA registers
    of the "Device".

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/

EFI_STATUS PcieSetSlotProperties(PCI_DEV_INFO *Device){
    return EFI_SUCCESS;
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
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetLnkProperties( PCI_DEV_INFO           *DnStreamPort,
                                 PCIE_LNK_CNT_REG       *DnLnkCnt,
                                 PCI_DEV_INFO           *UpStreamPort,
                                 PCIE_LNK_CNT_REG       *UpLnkCnt,
                                 BOOLEAN                *LinkTrainingRequired
                                 )
{
    return EFI_SUCCESS;
}


/**
    This function will Collect PCI Express Capabilities Data
    of the "Device".

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  Must be called only for PCI Express devices (Device->PciExpress!=NULL)

**/
EFI_STATUS PcieGetCaps(PCI_DEV_INFO *Device){
    return EFI_SUCCESS;
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

        return FALSE;
}


/**
    This function will Initialize Pcie Link on both sides starting
    from DownStream Port.

        
    @param DnPort Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieInitLink(PCI_DEV_INFO 	*DnPort )
{
    return EFI_SUCCESS;
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
    return EFI_SUCCESS;
}


/**
    This function will power ON or OFF PCI Express Slot

        
    @param Dev Pointer to PCI Device Private Data structure.
    @param PwrOn Tells to turn slot power ON or OFF

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetSlotPower(PCI_DEV_INFO* Device, BOOLEAN PwrOn){
    return EFI_SUCCESS;
}


/**
    This function will remove Device referenced as EndPoint from
    the gPcieEpLst data structure used as a database of available EndPoint devices.

        
    @param EndPoint Pointer to PCI Device Private Data structure.
    @param AllFunc TRUE/FALSE

    @retval EFI_STATUS
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           EndPoint not in the gPcieEpLst.

**/
EFI_STATUS PcieRemoveEpFromLst(PCI_DEV_INFO *EndPoint, BOOLEAN AllFunc){

    return EFI_NOT_FOUND;
}


/**
    This function will Initialize "Device"'s Virtual Channel properties
    based on Setup Settings and hardware capabilities.

        
    @param Device Pointer to PCI Device Private Data.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieInitVc(PCI_DEV_INFO  *Device){
    return EFI_SUCCESS;

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
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetAspm(PCI_DEV_INFO *DnPort,PCI_DEV_INFO *UpPort,UINT16 Aspm/*, UINT8 AspmModeOption*/ ){


    return EFI_SUCCESS;
}


/**
    This function will Select values for DEVICE CONTROL register
    based on Setup Settings and hardware capabilities.

        
    @param Device Pointer to PCI Device Private Data.
    @param MaxPayload Pointer to the MaxPayload value.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS PcieSetDevProperties(PCI_DEV_INFO *Device, UINT16 MaxPayload)
{
    return EFI_SUCCESS;
}
/**
    This function will detect optimal settings and program -
    MaxPayLoadSize, ReadRequestSize, ASPM settings and Virtual Channels
    for all devices in the PCIE Links Chain.

        
    @param Device Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_OUT_OF_RESOURCES    When system runs out of resources.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

    @note  
  RootPortComplex<=>Switch0...<=>...SwitchN<=>EndPoint
**/
EFI_STATUS PcieInitDevChain(PCI_DEV_INFO* Device ){
	return EFI_SUCCESS;
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

    return EFI_SUCCESS;
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
EFI_STATUS PcieProbeDevice(PCI_DEV_INFO *Device, BOOLEAN SriovSupport){

	return EFI_SUCCESS;
}

/**
    Checks and initializes according to the setup settings 
    Integrated PCI Express Device like Integrated End Point and RC 
    Event Collector. This Device/Port type value == PCIE_TYPE_RC_INTEGR_EP
    or PCIE_TYPE_RC_EVT_COLLECT don't have a Link associated with them and 
    from standard PCIe registers have only Device CAP|CTL|STA registers set.
    This devices not supporting ASPM and they can accept only below settings-  
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
	
	return EFI_NOT_FOUND;
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

