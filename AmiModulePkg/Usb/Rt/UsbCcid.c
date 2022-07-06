//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file UsbCcid.c
    AMI USB CCID Device class support driver

**/


#include "AmiUsb.h"
#include "AmiUsbRtCcid.h"
#include "Protocol/AmiUsbCcid.h"
#include <Library/AmiUsbHcdLib.h>

extern  USB_GLOBAL_DATA *gUsbData;
extern  BOOLEAN gCheckUsbApiParameter;
extern  USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;

extern  UINT8   EFIAPI USB_InstallCallBackFunction (CALLBACK_FUNC  pfnCallBackFunction);

//                      Fi   Max  Di
UINT16 FiFmaxDi[] = {   372,   4, 0, 
                        372,   5, 1, 
                        558,   6, 2,
                        744,   8, 4,
                        1116, 12, 8,
                        1488, 16, 16,
                        1860, 20, 32,
                        0,     0, 64, 
                        0,     0, 12,
                        512,   5, 20,
                        768,   7,  0,
                        1024, 10,  0,
                        1536, 15,  0,
                        2048, 20,  0,
                        0,     0,  0,
                        0,     0,  0
                        };

/**
 Type:        Function Dispatch Table

    This is the table of functions used by USB CCID API

**/

API_FUNC aUsbCCIDApiTable[] = {

    USBCCIDAPISmartClassDescriptorSMM,  // USB Mass API Sub-Func 00h
    USBCCIDAPIAtrSMM,                   // USB Mass API Sub-Func 01h
    USBCCIDAPIPowerupSlotSMM,           // USB Mass API Sub-Func 02h
    USBCCIDAPIPowerDownSlotSMM,         // USB Mass API Sub-Func 03h
    USBCCIDAPIGetSlotStatusSMM,         // USB Mass API Sub-Func 04h
    USBCCIDAPIXfrBlockSMM,              // USB Mass API Sub-Func 05h
    USBCCIDAPIGetParametersSMM,         // USB Mass API Sub-Func 06h

    USBSCardReaderAPIConnectSMM,        // USB Mass API Sub-Func 07h
    USBSCardReaderAPIDisConnectSMM,     // USB Mass API Sub-Func 08h
    USBSCardReaderAPIStatusSMM,         // USB Mass API Sub-Func 09h
    USBSCardReaderAPITransmitSMM,       // USB Mass API Sub-Func 0Ah
    USBSCardReaderAPIControlSMM,        // USB Mass API Sub-Func 0Bh
    USBSCardReaderAPIGetAttribSMM,      // USB Mass API Sub-Func 0Ch
};
 
/**
    This function fills DEV_DRIVER structure

    @param  FpDevDriver    Pointer to the DEV driver        
    @retval VOID
**/
VOID
USBCCIDFillDriverEntries (
    IN OUT DEV_DRIVER    *FpDevDriver
)
{

    FpDevDriver->DevType               = BIOS_DEV_TYPE_STORAGE;
    FpDevDriver->BaseClass             = BASE_CLASS_CCID_STORAGE;
    FpDevDriver->SubClass              = SUB_CLASS_CCID;
    FpDevDriver->Protocol              = PROTOCOL_CCID;
    FpDevDriver->FnDeviceInit          = USBCCIDInitialize;
    FpDevDriver->FnCheckDeviceType     = USBCCIDCheckForDevice;
    FpDevDriver->FnConfigureDevice     = USBCCIDConfigureDevice;
    FpDevDriver->FnDisconnectDevice    = USBCCIDDisconnectDevice;

    return;
}

/**
    This function is part of the USB BIOS CCID API inside SMM
    This API returns 36h bytes of SMART Class Descriptor to the caller. 
    Input Buffer of 36h bytes long is provided by the caller.
    
    @param Urp    Pointer to the URP structure
                  Urp->bRetValue USB_SUCESS if data is returned
    @retval None

**/
VOID
USBCCIDAPISmartClassDescriptorSMM(
    IN OUT URP_STRUC *Urp
)
{
   
    DEV_INFO    *DevInfo;
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO   *CcidDevData;


    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.CCIDSmartClassDescriptor.ResponseBuffer), 
                                            (UINT32)sizeof(SMARTCLASS_DESC));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
            Urp->bRetValue = USB_ERROR;
            return;
        }
        gCheckUsbApiParameter = FALSE;
    }


    DevInfo = (DEV_INFO *) (Urp->ApiData.CCIDSmartClassDescriptor.FpDevInfo);

    // Check whether it is a valid CCID Device
    if (!DevInfo) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!CcidDevData) {
        Urp->bRetValue = USB_ERROR;
        return;
    }    
    if (!CcidDevData->CcidDescriptor) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
       
    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_PARAMETER_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    
    CopyMem((UINT8 *)(Urp->ApiData.CCIDSmartClassDescriptor.ResponseBuffer),
            (UINT8 *)CcidDevData->CcidDescriptor, (UINT32)sizeof(SMARTCLASS_DESC));

    Urp->bRetValue = USB_SUCCESS;

    return;

}

/**
    This function is part of the USB CCID API inside SMM.
    This API returns ATR data if present

    @param Urp    Pointer to the URP structure
                  Urp->bRetValue : USB_SUCESS if data is returned
    @retval None 

**/
VOID
USBCCIDAPIAtrSMM(
    IN OUT URP_STRUC *Urp

)
{

    DEV_INFO            *DevInfo;
    ICC_DEVICE          *IccDevice;
    EFI_STATUS          Status = EFI_SUCCESS;
    UINT8               *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO       *CcidDevData;


    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.CCIDAtr.ATRData),
                    MAX_ATR_LENGTH);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
            Urp->bRetValue = USB_ERROR;
            return;
        }
        gCheckUsbApiParameter = FALSE;
    }

    DevInfo = (DEV_INFO *)(Urp->ApiData.CCIDAtr.FpDevInfo);
    //
    // Check whether it is a valid CCID Device
    //
    if (!DevInfo) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      Urp->bRetValue = USB_ERROR;
        return;
    }
    if (!CcidDevData) {
        Urp->bRetValue = USB_ERROR;
        return;
    }    
    if (!CcidDevData->CcidDescriptor) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_PARAMETER_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Urp->bRetValue = USB_ERROR;

    //
    // Locate the ICCDevice 
    //
    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.CCIDAtr.Slot);

    if (IccDevice) {
        if (IccDevice->ConfiguredStatus & ATRDATAPRESENT) {
            CopyMem((UINT8 *)(Urp->ApiData.CCIDAtr.ATRData),
                    (UINT8 *)IccDevice->RawATRData, MAX_ATR_LENGTH);
            Urp->bRetValue = USB_SUCCESS;
        }
    }

    return;

}

/**
    This function is part of the USB BIOS CCID API inside SMM
    This API powers up the particular slot in CCID and returns ATR data if successful
    @param Urp    Pointer to the URP structure
                  Urp->bRetValue : USB_SUCESS if data is returned
    @retval None 



**/
VOID
USBCCIDAPIPowerupSlotSMM (
    IN OUT URP_STRUC *Urp

)
{

    EFI_STATUS  Status;    
    DEV_INFO    *DevInfo;
    ICC_DEVICE  *IccDevice;
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO       *CcidDevData;



    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.CCIDPowerupSlot.ATRData), MAX_ATR_LENGTH);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
            Urp->bRetValue = USB_ERROR;
            return;
        }
        gCheckUsbApiParameter = FALSE;
    }


    DevInfo = (DEV_INFO *) (Urp->ApiData.CCIDPowerupSlot.FpDevInfo);

    //
    // Check whether it is a valid CCID Device
    //
    if (!DevInfo) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;

    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!CcidDevData) {
        Urp->bRetValue = USB_ERROR;
        return;
    }    
    if (!CcidDevData->CcidDescriptor) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_PARAMETER_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    
    //
    // Locate the ICCDevice 
    //
    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.CCIDPowerupSlot.Slot);

    if (IccDevice) {
        //
        // The slot has been already discovered. Check the status.
        //
        if (IccDevice->ConfiguredStatus & VOLTAGEAPPLIED) {
            //
            // Power down the device
            //
            PCtoRDRIccPowerOff (DevInfo, IccDevice);
            RDRToPCSlotStatus(DevInfo, IccDevice);
        }
    }

    Status = ICCInsertEvent(DevInfo, Urp->ApiData.CCIDPowerupSlot.Slot);

    //
    // If the card has been successfully poweredup copy ATR data
    //
    if (!IccDevice) {
        IccDevice = GetICCDevice(DevInfo, Urp->ApiData.CCIDPowerupSlot.Slot);
        if (!IccDevice) {
            Urp->bRetValue = USB_ERROR;
            return;
        }
    }
    Urp->ApiData.CCIDPowerupSlot.bStatus = IccDevice->bStatus;
    Urp->ApiData.CCIDPowerupSlot.bError = IccDevice->bError;

    if (IccDevice->ConfiguredStatus & ATRDATAPRESENT) {
        CopyMem((UINT8 *)(Urp->ApiData.CCIDPowerupSlot.ATRData),
                (UINT8 *)IccDevice->RawATRData, MAX_ATR_LENGTH);      
    }

    Urp->bRetValue = USB_SUCCESS;

    if (Status == EFI_DEVICE_ERROR){
        Urp->bRetValue = USB_ERROR;
    }

    return;

}


/**
    This function is part of the USB BIOS CCID API inside SMM
    This API powers down the particular slot.
    
    @param Urp    Pointer to the URP structure\
                  Urp->bRetValue : USB_SUCESS if data is returned
    @retval None 

**/
VOID
USBCCIDAPIPowerDownSlotSMM(
    IN OUT URP_STRUC *Urp
)
{
    
    EFI_STATUS  Status = EFI_SUCCESS;
    DEV_INFO    *DevInfo;
    ICC_DEVICE  *IccDevice;
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO       *CcidDevData;

    DevInfo = (DEV_INFO *)(Urp->ApiData.CCIDPowerdownSlot.FpDevInfo);

    Urp->bRetValue = USB_ERROR;

    //
    // Check whether it is a valid CCID Device
    //
    if (!DevInfo) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!CcidDevData) {
        Urp->bRetValue = USB_ERROR;
        return;
    }    
    if (!CcidDevData->CcidDescriptor) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_PARAMETER_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    //
    // Locate the ICCDevice 
    //
    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.CCIDPowerdownSlot.Slot);

    if (IccDevice) {
        //
        // The slot has been already discovered. Check the status.
        //
        if (IccDevice->ConfiguredStatus & ICCPRESENT) {
            //
            // Power down the device
            //
            Status = PCtoRDRIccPowerOff (DevInfo, IccDevice);
            RDRToPCSlotStatus(DevInfo, IccDevice);

            IccDevice->ConfiguredStatus &= (~VOLTAGEAPPLIED);

            Urp->ApiData.CCIDPowerdownSlot.bStatus = IccDevice->bStatus;
            Urp->ApiData.CCIDPowerdownSlot.bError = IccDevice->bError;

        }
    }

    Urp->bRetValue = USB_SUCCESS;

    if (Status == EFI_DEVICE_ERROR) {
        Urp->bRetValue = USB_ERROR;
    }

    return;

}

/**
    This function is part of the USB BIOS MASS API inside SMM
    This API returns information from RDR_to_PC_SlotStatus. 

    @param Urp    Pointer to the URP structure.
                  Urp->bRetValue : USB_SUCESS if data is returned
    @retval None 

**/
VOID
USBCCIDAPIGetSlotStatusSMM (
    IN OUT URP_STRUC *Urp
)
{
    
    EFI_STATUS  Status;
    DEV_INFO    *DevInfo;
    ICC_DEVICE  *IccDevice;
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO       *CcidDevData;

    DevInfo = (DEV_INFO *) (Urp->ApiData.CCIDGetSlotStatus.FpDevInfo);

    //
    // Check whether it is a valid CCID Device
    //
    if (!DevInfo) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!CcidDevData) {
        Urp->bRetValue = USB_ERROR;
        return;
    }    
    if (!CcidDevData->CcidDescriptor) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_PARAMETER_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    //
    // Locate the ICCDevice 
    //
    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.CCIDGetSlotStatus.Slot);
    if (!IccDevice || !(IccDevice->ConfiguredStatus & ICCPRESENT)) {
        Urp->ApiData.CCIDGetSlotStatus.bStatus = 0x42; 
        Urp->ApiData.CCIDGetSlotStatus.bError = 0xFE;
        Urp->bRetValue = USB_ERROR;
        return;
    }
    
    //
    // Issue the cmd
    //
    Status = PCToRDRGetSlotStatus(DevInfo, IccDevice);
    
    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_ERROR;
        return;        
    }

    //
    // Get the response
    //
    Status = RDRToPCSlotStatus(DevInfo, IccDevice);

    Urp->ApiData.CCIDGetSlotStatus.bStatus = IccDevice->bStatus;
    Urp->ApiData.CCIDGetSlotStatus.bError = IccDevice->bError;
    Urp->ApiData.CCIDGetSlotStatus.bClockStatus = IccDevice->bClockStatus;

    Urp->bRetValue = USB_SUCCESS;

    if (Status == EFI_DEVICE_ERROR) {
        Urp->bRetValue = USB_ERROR;
    }

    return;

}


/**
    This function is part of the USB BIOS MASS API.
    This API excutes PC_to_RDR_XfrBlock cmd and returns the response from 
    RDR_to_PC_DataBlock to the caller.
           
    @param Urp    Pointer to the URP structure.
                  Urp->bRetValue : USB_SUCESS if data is returned
    @retval None 



**/

VOID
USBCCIDAPIXfrBlockSMM (
    IN OUT URP_STRUC *Urp
)
{

    EFI_STATUS  Status;
    DEV_INFO    *DevInfo;
    ICC_DEVICE  *IccDevice;
    UINT32      CmdLength = (UINT32)Urp->ApiData.CCIDXfrBlock.CmdLength;
    UINT8       *CmdBuffer = (UINT8 *)Urp->ApiData.CCIDXfrBlock.CmdBuffer;
    UINT8       IsBlock = (BOOLEAN)Urp->ApiData.CCIDXfrBlock.ISBlock;
    UINT32      *ResponseLength = (UINT32 *)&(Urp->ApiData.CCIDXfrBlock.ResponseLength);
    UINT8       *ResponseBuffer = (UINT8 *)(Urp->ApiData.CCIDXfrBlock.ResponseBuffer);
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO       *CcidDevData;


    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.CCIDXfrBlock.CmdBuffer),
                                             Urp->ApiData.CCIDXfrBlock.CmdLength);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
            Urp->bRetValue = USB_ERROR;
            return;
        }
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.CCIDXfrBlock.ResponseBuffer),
                                             Urp->ApiData.CCIDXfrBlock.ResponseLength);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
            Urp->bRetValue = USB_ERROR;
            return;
        }
        gCheckUsbApiParameter = FALSE;
    }

    DevInfo = (DEV_INFO *)(Urp->ApiData.CCIDXfrBlock.FpDevInfo);

    //
    // Check whether it is a valid CCID Device
    //
    if (!DevInfo) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;

    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!CcidDevData) {
        Urp->bRetValue = USB_ERROR;
        return;
    }    
    if (!CcidDevData->CcidDescriptor) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_PARAMETER_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    //
    // Locate the ICCDevice 
    //
    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.CCIDXfrBlock.Slot);

    if (!IccDevice || !(IccDevice->ConfiguredStatus & ICCPRESENT)) {
        Urp->ApiData.CCIDXfrBlock.bStatus = 0x42; 
        Urp->ApiData.CCIDXfrBlock.bError = 0xFE;
        Urp->bRetValue = USB_ERROR;
        return;
    }


    //
    // Only T0/T1 are recognized
    //
    if (IccDevice->bProtocolNum > 1) {
        Urp->bRetValue = USB_ERROR;        
        return;
    } 

    //
    // Check for T0/T1
    //
    if (IccDevice->bProtocolNum) {    
        switch (((SMARTCLASS_DESC*)CcidDevData->CcidDescriptor)->dwFeatures & 0x70000) {

            case TDPU_LEVEL_EXCHANGE:

                Status = TxRxT1TDPUChar (DevInfo, IccDevice, CmdLength, CmdBuffer, IsBlock, ResponseLength, ResponseBuffer);
                break;

            case CHARACTER_LEVEL_EXCHANGE:

                Status = TxRxT1TDPUChar (DevInfo, IccDevice, CmdLength, CmdBuffer, IsBlock, ResponseLength, ResponseBuffer);
                break;

            case SHORT_ADPU_LEVEL_EXCHANGE:
            case EXT_ADPU_LEVEL_EXCHANGE:
        
                Status = TxRxT1Adpu(DevInfo, IccDevice, CmdLength, CmdBuffer, ResponseLength, ResponseBuffer);
                break;
        }
    } else {
        // T0 not supported yet
        Urp->bRetValue = USB_ERROR;        
        return;
    }

    Urp->ApiData.CCIDXfrBlock.bStatus = IccDevice->bStatus;
    Urp->ApiData.CCIDXfrBlock.bError = IccDevice->bError;

    Urp->bRetValue = USB_SUCCESS;

    if (Status == EFI_DEVICE_ERROR) {
        Urp->bRetValue = USB_ERROR;
    }

    return;

}

/**
    This function is part of the USB BIOS CCID API.
    This API returns the response to RDR_to_PCParameters cmd
    
    @param Urp    Pointer to the URP structure.
                  Urp->bRetValue : USB_SUCESS if data is returned
    @retval None 

**/
VOID
USBCCIDAPIGetParametersSMM (
    IN OUT URP_STRUC *Urp

)
{
    
    EFI_STATUS  Status;
    DEV_INFO    *DevInfo;
    ICC_DEVICE  *IccDevice;
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO       *CcidDevData;


    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.CCIDGetParameters.ResponseBuffer),
                                                    Urp->ApiData.CCIDGetParameters.ResponseLength);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
            Urp->bRetValue = USB_ERROR;
            return;
        }
        gCheckUsbApiParameter = FALSE;
    }


    DevInfo = (DEV_INFO *)(Urp->ApiData.CCIDGetParameters.FpDevInfo);

    //
    // Check whether it is a valid CCID Device
    //
    if (!DevInfo) {
        Urp->bRetValue = USB_ERROR;
        return;
    }
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!CcidDevData) {
        Urp->bRetValue = USB_ERROR;
        return;
    }    
    if (!CcidDevData->CcidDescriptor) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_PARAMETER_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    //
    // Locate the ICCDevice 
    //
    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.CCIDGetParameters.Slot);
    if (!IccDevice || !(IccDevice->ConfiguredStatus & ICCPRESENT)) {
        Urp->ApiData.CCIDGetParameters.bStatus = 0x42; 
        Urp->ApiData.CCIDGetParameters.bError = 0xFE;
        Urp->bRetValue = USB_ERROR;
        return;
    }

    // Should we check for device presence in data area. The call will find that out anyways.

    //    
    // Issue the cmd
    //
    Status = PCToRDRGetParameters(DevInfo, IccDevice);
    
    if (EFI_ERROR(Status)) {
        Urp->bRetValue = USB_ERROR;
        return;        
    }

    //
    // Get the response
    //
    Status = RDRToPCParameters(DevInfo, IccDevice);
    if (!EFI_ERROR(Status)) {
        Urp->ApiData.CCIDGetParameters.ResponseLength = 6;
        if (IccDevice->bProtocolNum) {
            Urp->ApiData.CCIDGetParameters.ResponseLength = 8;
        }
        //
        // Update the Data
        //
        CopyMem((UINT8 *)(Urp->ApiData.CCIDGetParameters.ResponseBuffer),
                (UINT8 *)&(IccDevice->bProtocolNum),
                (UINT32)(Urp->ApiData.CCIDGetParameters.ResponseLength));
    }

    Urp->ApiData.CCIDGetParameters.bStatus = IccDevice->bStatus;
    Urp->ApiData.CCIDGetParameters.bError = IccDevice->bError;

    Urp->bRetValue = USB_SUCCESS;

    if (Status == EFI_DEVICE_ERROR) {
        Urp->bRetValue = USB_ERROR;
    }

    return;

}

/**
    This function is part of the USB BIOS CCID API. It refers to 
    SCardConnect API of EFI SMART CARD READER PROTOCOL.

    @param Urp    Pointer to the URP structure.
    @retval None 

**/

VOID
USBSCardReaderAPIConnectSMM(
    IN OUT URP_STRUC *Urp
) 
{
    EFI_STATUS      Status;
    DEV_INFO        *DevInfo;
    ICC_DEVICE      *IccDevice;
    UINT8           Slot;
    UINT32          AccessMode;
    UINT32          CardAction;
    UINT32          PreferredProtocols;
    UINT32          *ActiveProtocol;
    EFI_STATUS      *ReturnStatus = &(Urp->ApiData.SmartCardReaderConnect.EfiStatus);
    UINT8           *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO       *CcidDevData;


    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderConnect.ActiveProtocol),
                                             sizeof(UINT32));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
            *ReturnStatus = EFI_INVALID_PARAMETER;
            return;
        }
        gCheckUsbApiParameter = FALSE;
    }


    // Get the Input Parameter passed to the EFI Smart Card reader Protocol API
    Slot = Urp->ApiData.SmartCardReaderConnect.Slot;
    DevInfo = (DEV_INFO *)(Urp->ApiData.SmartCardReaderConnect.FpDevInfo);
    AccessMode = Urp->ApiData.SmartCardReaderConnect.AccessMode;
    CardAction = Urp->ApiData.SmartCardReaderConnect.CardAction;
    PreferredProtocols = Urp->ApiData.SmartCardReaderConnect.PreferredProtocols;
    ActiveProtocol = (UINT32*)Urp->ApiData.SmartCardReaderConnect.ActiveProtocol;

    *ReturnStatus = EFI_DEVICE_ERROR;
    
    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }
    // Get the respective ICC_DEVICE structure for the SCardReader Slot
    IccDevice = GetICCDevice(DevInfo, Slot);
    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;

    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      Urp->bRetValue = USB_ERROR;
        return;
    }
  
    if (!IccDevice || !CcidDevData || !CcidDevData->CcidDescriptor) {
        *ReturnStatus = EFI_NOT_FOUND;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }

    // Already card is connected
    if (IccDevice->SlotConnectStatus) {
        *ReturnStatus = EFI_ACCESS_DENIED;
        return;
    }

    if (AccessMode == SCARD_AM_READER) {

        if (!((IccDevice->ConfiguredStatus & (ICCPRESENT | VOLTAGEAPPLIED | ATRDATAPRESENT))
                == (ICCPRESENT | VOLTAGEAPPLIED | ATRDATAPRESENT))) {
            // Call to configure the SMART CARD READER if the slot is not active
            Status = ConfigureCCID(DevInfo, IccDevice);
            // Voltage applied but ICC ATR is not retrieved As ICC card is not available
            // it is not an error for SCARD_AM_READER connect
            if (Status == EFI_NOT_FOUND) {
                return;
            }
        }
        *ReturnStatus = EFI_SUCCESS;
        IccDevice->SlotConnectStatus = SCARD_AM_READER;
        return;
    } else {

        Status = EFI_DEVICE_ERROR;

        switch (CardAction) {
            case SCARD_CA_NORESET:
                // Check if SMART card reader slot is already configured, If configured already return Active Protocol
                if (((IccDevice->ConfiguredStatus & (ICCPRESENT | VOLTAGEAPPLIED | ATRDATAPRESENT))
                        == (ICCPRESENT | VOLTAGEAPPLIED | ATRDATAPRESENT))) {
                    // Check Preferred Protocol mask contain an Active Protocol, return success if supported
                    if (((UINTN)(1 << IccDevice->bProtocolNum) & PreferredProtocols) == (UINTN)(1 << IccDevice->bProtocolNum)) {
                         Status = EFI_SUCCESS;
                     } else {
                         Status = EFI_INVALID_PARAMETER;
                     }
                } else {
                    // Power up and Configure Slot 
                    Status = ConfigureCCID(DevInfo, IccDevice);
                }
                break;

            case SCARD_CA_COLDRESET:
                // Power Off the ICC device if it is already Power on and Active
                // Cold Reset starts with Power Off, Power On Device at below (SCARD_CA_WARMRESET)
                PCtoRDRIccPowerOff (DevInfo, IccDevice);

                Status = RDRToPCSlotStatus(DevInfo, IccDevice);
                if (EFI_ERROR(Status)) {
                   break;
                }
            case SCARD_CA_WARMRESET:
                // Power on and configure the ICC device
                Status = ConfigureCCID(DevInfo, IccDevice);
                break;

            case SCARD_CA_UNPOWER:
                // Send Power Off command to the ICC device
                PCtoRDRIccPowerOff (DevInfo, IccDevice);
                Status = RDRToPCSlotStatus(DevInfo, IccDevice);
                break;
            case SCARD_CA_EJECT:
                Status = EFI_UNSUPPORTED;
                break;
            default:
                Status = EFI_SUCCESS;
                break;
        }

        *(UINT32*)Urp->ApiData.SmartCardReaderConnect.ActiveProtocol = (1 << IccDevice->bProtocolNum);

        *ReturnStatus = Status;

        if (!EFI_ERROR(Status)) {
            IccDevice->SlotConnectStatus = SCARD_AM_CARD;
        } 
    }
    return;
}

/**
    This function is part of the USB BIOS MASS API. It refers to 
    SCardDisConnect API of EFI SMART CARD READER PROTOCOL.

    @param Urp    Pointer to the URP structure.
    @retval None 

**/
VOID
USBSCardReaderAPIDisConnectSMM(
    IN OUT URP_STRUC *Urp
)
{
    DEV_INFO        *DevInfo;
    ICC_DEVICE      *IccDevice;
    UINT32          CardAction;
    EFI_STATUS      *ReturnStatus = &(Urp->ApiData.SmartCardReaderDisconnect.EfiStatus);
    EFI_STATUS      Status;
    UINT8           *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO   *CcidDevData;

    DevInfo = (DEV_INFO *)(Urp->ApiData.SmartCardReaderDisconnect.FpDevInfo);
    CardAction = Urp->ApiData.SmartCardReaderDisconnect.CardAction;
    
    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }
    
    // Get the respective ICC_DEVICE structure for the CCID Slot
    IccDevice = GetICCDevice(DevInfo, 
                             Urp->ApiData.SmartCardReaderDisconnect.Slot);

    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;

    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) {
      Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!IccDevice || !CcidDevData || !CcidDevData->CcidDescriptor) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }

    switch (CardAction) {
        case SCARD_CA_EJECT:
            if (IccDevice->SlotConnectStatus == SCARD_AM_READER) {
                // Reader SCardConnect call is made, return Unsupported
                Status = EFI_UNSUPPORTED;
                return;
            }
            break;
        case SCARD_CA_COLDRESET:
        case SCARD_CA_WARMRESET:
        case SCARD_CA_UNPOWER:
            PCtoRDRIccPowerOff (DevInfo, IccDevice);
            Status = RDRToPCSlotStatus(DevInfo, IccDevice);
            break;
        case SCARD_CA_NORESET:
            Status = EFI_SUCCESS;
            break;
    }

    IccDevice->SlotConnectStatus = 0;
    
    *ReturnStatus = Status;
    return;
}

/**
    This function updates the input parameter(State) with Smart card status

    @param CcidCmdStatus      Ccid command status
    @param State              Smart card state

    @retval  None

**/

void
UpdateICCState(
    UINT8  CcidCmdStatus,
    UINT32 *State
)
{
    switch (CcidCmdStatus & 0x07)
    {
    case 0: *State = SCARD_ACTIVE;
            break;
    case 1: *State = SCARD_INACTIVE;
            break;
    case 2: *State = SCARD_ABSENT;
            break;
    default:
        *State = SCARD_UNKNOWN;
    }
    return;
}

/**
    This function updates Smart card Atr information

    @param IccDevice         Pointer to Icc(Integrated Circuit(s) Card)device data structure
    @param Atr               Atr data
    @param AtrLength         Length of Atr data

    @retval  EFI_SUCCESS      On success
    @retval  Others           error

**/
EFI_STATUS
UpdateSCardReaderAtrData(
    ICC_DEVICE    *IccDevice,
    UINT8         *Atr,
    UINTN         *AtrLength
)
{
    // Return error if input is not valid
    if (!Atr || !AtrLength) {
        return EFI_INVALID_PARAMETER;
    }

    if (*AtrLength < sizeof(ATR_DATA) ) {
        *AtrLength = sizeof(ATR_DATA);
        return EFI_BUFFER_TOO_SMALL;
    }

    if (IccDevice) {
       if (IccDevice->ConfiguredStatus & ATRDATAPRESENT) {
           CopyMem(Atr, (UINT8 *)IccDevice->RawATRData, MAX_ATR_LENGTH);
       } else {
           return EFI_DEVICE_ERROR;
       }
    }

    return EFI_SUCCESS;
}

/**
    This function is part of the USB BIOS CCID API. It refers to 
    SCardStatus API of EFI SMART CARD READER PROTOCOL

    @param Urp    Pointer to the URP structure.
    @retval None 

**/

VOID
USBSCardReaderAPIStatusSMM(
   IN OUT URP_STRUC *Urp
)
{
    EFI_STATUS   Status;
    DEV_INFO     *DevInfo;
    ICC_DEVICE   *IccDevice;
    UINT32       *State;
    UINT32       *CardProtocol;
    UINT8        *Atr;
    UINTN        *AtrLength;
    EFI_STATUS   *ReturnStatus;
    UINT8        *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO   *CcidDevData;

    ReturnStatus = (EFI_STATUS*)(VOID*)&(Urp->ApiData.SmartCardReaderStatus.EfiStatus);

    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderStatus.ReaderNameLength), sizeof(UINTN));
        if (Status != EFI_ABORTED){
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderStatus.ReaderName),
                        *(UINTN*)Urp->ApiData.SmartCardReaderStatus.ReaderNameLength);
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderStatus.State), sizeof(UINT32));
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderStatus.CardProtocol), sizeof(UINT32));
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderStatus.AtrLength),
                        sizeof(UINTN));
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderStatus.Atr),
                        *(UINTN*)(Urp->ApiData.SmartCardReaderStatus.AtrLength));
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
        }
        gCheckUsbApiParameter = FALSE;
    }
    
    DevInfo = (DEV_INFO *)(Urp->ApiData.SmartCardReaderStatus.FpDevInfo);
    State = (UINT32*)Urp->ApiData.SmartCardReaderStatus.State;
    CardProtocol = (UINT32*)Urp->ApiData.SmartCardReaderStatus.CardProtocol;
    Atr = (UINT8*)Urp->ApiData.SmartCardReaderStatus.Atr;
    AtrLength = (UINTN*)Urp->ApiData.SmartCardReaderStatus.AtrLength;

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
       *ReturnStatus = EFI_INVALID_PARAMETER;
       return;
    }

    // Get ICC device Interface
    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.SmartCardReaderStatus.Slot);

    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;

    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!IccDevice || !CcidDevData || !CcidDevData->CcidDescriptor) {
        *ReturnStatus = EFI_DEVICE_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }
    
    if (IccDevice->SlotConnectStatus == 0) {
       *ReturnStatus = EFI_INVALID_PARAMETER;
       return;
    }

    if (State != NULL) {
         // Send command to get status of the Scard Reader slot
         PCToRDRGetSlotStatus(DevInfo, IccDevice);
         Status = RDRToPCSlotStatus(DevInfo, IccDevice);
         if (EFI_ERROR(Status)) {
             *ReturnStatus = Status;
             return;
         }   
         UpdateICCState(IccDevice->bStatus, State);
    }

    if (CardProtocol) {
        *CardProtocol = (1 << IccDevice->bProtocolNum);
    }

    Status = UpdateSCardReaderAtrData(IccDevice, Atr, AtrLength);

    *ReturnStatus = Status;
    
    return;
}

/**
    This function is part of the USB BIOS CCID API. Used to transfer command to smard card.

    @param Urp    Pointer to the URP structure.
    @retval None 

**/

VOID
USBSCardReaderAPITransmitSMM(
    IN OUT URP_STRUC *Urp
)
{
    ICC_DEVICE   *IccDevice;
    URP_STRUC    XfrBlockUrp;
    DEV_INFO     *DevInfo;
    EFI_STATUS   *ReturnStatus;
    EFI_STATUS   Status = EFI_SUCCESS;
    UINT8        *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO   *CcidDevData;

    DevInfo = (DEV_INFO*)Urp->ApiData.SmartCardReaderTransmit.FpDevInfo;
    ReturnStatus = (EFI_STATUS*)(VOID*)&(Urp->ApiData.SmartCardReaderTransmit.EfiStatus);

    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderTransmit.CAPDU),
                    Urp->ApiData.SmartCardReaderTransmit.CAPDULength);
        if (Status != EFI_ABORTED){
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderTransmit.RAPDULength), sizeof(UINTN));
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)(Urp->ApiData.SmartCardReaderTransmit.RAPDU),
                        *(UINTN *)(Urp->ApiData.SmartCardReaderTransmit.RAPDULength));
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_ERROR, 3, "UsbCcid Invalid Pointer, Buffer is in SMRAM.\n");
                *ReturnStatus = EFI_INVALID_PARAMETER;
                return;
            }
        }
        gCheckUsbApiParameter = FALSE;
    }


    IccDevice = GetICCDevice(DevInfo, Urp->ApiData.SmartCardReaderTransmit.Slot);

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }

    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;

    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    if (!IccDevice || !CcidDevData ||!CcidDevData->CcidDescriptor) {
        *ReturnStatus = EFI_DEVICE_ERROR;
        return;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }

    if (IccDevice->SlotConnectStatus == 0) {
        *ReturnStatus = EFI_INVALID_PARAMETER;
        return;
    }

    ZeroMem(&XfrBlockUrp, sizeof (URP_STRUC));

    XfrBlockUrp.ApiData.CCIDXfrBlock.CmdLength = Urp->ApiData.SmartCardReaderTransmit.CAPDULength;
    XfrBlockUrp.ApiData.CCIDXfrBlock.CmdBuffer = (UINTN)Urp->ApiData.SmartCardReaderTransmit.CAPDU;
    XfrBlockUrp.ApiData.CCIDXfrBlock.ISBlock = I_BLOCK;
    XfrBlockUrp.ApiData.CCIDXfrBlock.ResponseLength = *(UINTN *)(Urp->ApiData.SmartCardReaderTransmit.RAPDULength);
    XfrBlockUrp.ApiData.CCIDXfrBlock.ResponseBuffer = (UINTN)Urp->ApiData.SmartCardReaderTransmit.RAPDU;

    XfrBlockUrp.ApiData.CCIDXfrBlock.FpDevInfo = (UINTN)DevInfo;
    XfrBlockUrp.ApiData.CCIDXfrBlock.Slot = Urp->ApiData.SmartCardReaderTransmit.Slot;

    USBCCIDAPIXfrBlockSMM(&XfrBlockUrp);

    *(UINTN *)(Urp->ApiData.SmartCardReaderTransmit.RAPDULength) = XfrBlockUrp.ApiData.CCIDXfrBlock.ResponseLength;

    if (Urp->bRetValue) {
        *ReturnStatus = EFI_DEVICE_ERROR;
    } else {
        *ReturnStatus = EFI_SUCCESS;
    }
    return;
}

/**
    This function is part of the USB BIOS CCID API.

    @param Urp    Pointer to the URP structure.
    @retval None 

**/
VOID
USBSCardReaderAPIControlSMM (
    IN OUT URP_STRUC *Urp
)
{
    return;
}

/**
    This function is part of the USB BIOS CCID API.
    The function gets the card reader attrubute
    
    @param Urp    Pointer to the URP structure.
    @retval None 

**/
VOID
USBSCardReaderAPIGetAttribSMM (
    IN OUT URP_STRUC *Urp
)
{
    return;
}

/**
    PC_TO_RDR_XFRBLOCK cmd is issued to the device
    This function sends PC_TO_RDR_XFRBLOCK to the device. 
    See section 6.1.4 of CCID spec 1.1 for the details.

    Input  
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure
    @param CmdLength          Command length
    @param CmdBuffer          Command data buffer
    @param BlockWaitingTime   Block waiting time
    @param LevelParameter     Level parameter
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error
          
**/
EFI_STATUS
PCToRDRXfrBlock (
    IN DEV_INFO             *FpDevInfo,
    IN ICC_DEVICE           *FpICCDevice,
    IN UINT32               CmdLength,
    IN UINT8                *CmdBuffer,
    IN UINT8                BlockWaitingTime,
    IN UINT16               LevelParameter    

)
{

    EFI_STATUS                      Status = EFI_SUCCESS;
    PC_TO_RDR_XFRBLOCK_STRUC        *CcidCmdBuffer;
    UINT32                          Data;
    UINT32                          Index;  

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "PCToRDRXfrBlock ....");

    CcidCmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_XFRBLOCK_STRUC) + CmdLength));
    ASSERT(CcidCmdBuffer);
    if (!CcidCmdBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)CcidCmdBuffer, sizeof(PC_TO_RDR_XFRBLOCK_STRUC) + CmdLength);

    //
    // Prepare  the cmd buffer
    //
    CcidCmdBuffer->bMessageType = PC_TO_RDR_XFRBLOCK;
    CcidCmdBuffer->dwLength = CmdLength;
    CcidCmdBuffer->bSlot = FpICCDevice->Slot;
    CcidCmdBuffer->bSeq = gUsbData->CcidSequence;
    CcidCmdBuffer->bBWI = BlockWaitingTime;
    CcidCmdBuffer->wLevelParameter = LevelParameter;

    //
    // Copy the cmd
    //
    if (CmdLength) {
        CopyMem((UINT8 *)CcidCmdBuffer + sizeof(PC_TO_RDR_XFRBLOCK_STRUC),
                CmdBuffer, CmdLength);
    }


    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\n");
    for (Index = 0; Index < sizeof(PC_TO_RDR_XFRBLOCK_STRUC) + CmdLength; Index++) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%02X ", ((UINT8 *)CcidCmdBuffer)[Index]);
    }
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\n");

    Data = USBCCIDIssueBulkTransfer(FpDevInfo, 0, 
                                    (UINT8 *)CcidCmdBuffer, 
                                    sizeof(PC_TO_RDR_XFRBLOCK_STRUC) + CmdLength
                                    );

    //
    // Handle Error if any. This error is due to blk transfer
    //
    if (!Data) {
        Status = EFI_DEVICE_ERROR;
    }

    USB_MemFree(CcidCmdBuffer, (UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_XFRBLOCK_STRUC) + CmdLength));

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r ....", Status);

    return Status;
    
}

/**
    PC_TO_RDR_ICCPOWERON cmd is issued to the CCID
    See section 6.1.1 of CCID spec Rev 1.1 for more details
        
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure
    @param PowerLevel         00:Automatic  Voltage selection, 01:5.0v, 02:3.0v, 03:1.8v

    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/
EFI_STATUS
PCtoRDRIccPowerOn(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice,
    IN UINT8            PowerLevel
)
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    PC_TO_RDR_ICCPOWERON_STRUC     *CmdBuffer;
    UINT32                        Data;

    USB_DEBUG (DEBUG_INFO, DEBUG_LEVEL_3, "PCtoRDRIccPowerOn .... PowerLevel : %x...", PowerLevel);

    CmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_ICCPOWERON_STRUC)));
    ASSERT(CmdBuffer);
    if (!CmdBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)CmdBuffer, sizeof(PC_TO_RDR_ICCPOWERON_STRUC));

    //
    // Prepare  the cmd buffer
    //
    CmdBuffer->bMessageType = PC_TO_RDR_ICCPOWERON;
    CmdBuffer->dwLength = 0;
    CmdBuffer->bSlot = FpICCDevice->Slot;
    CmdBuffer->bSeq = gUsbData->CcidSequence;
    CmdBuffer->bPowerSlot = PowerLevel;
    CmdBuffer->abRFU = 0;

    Data = USBCCIDIssueBulkTransfer(FpDevInfo, 0, 
                                    (UINT8 *)CmdBuffer, 
                                    sizeof(PC_TO_RDR_ICCPOWERON_STRUC)
                                    );

    //
    // Handle Error if any. This error is due to blk transfer
    //
    if (!Data) {
        Status = EFI_DEVICE_ERROR;
    }

    USB_MemFree(CmdBuffer, (UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_ICCPOWERON_STRUC)));

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r ....", Status);

    return Status;

}


/**
    PC_TO_RDR_ICCPOWEROFF cmd is issued to the CCID
    See section 6.1.2 of CCID spec Rev 1.1 for more details
    
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure


    @retval EFI_SUCCESS       Success
    @retval Others            Error


**/
EFI_STATUS
PCtoRDRIccPowerOff(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice
)
{

    EFI_STATUS                  Status = EFI_SUCCESS;
    PC_TO_RDR_ICCPOWEROFF_STRUC *CmdBuffer;
    UINT32                      Data;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "PCtoRDRIccPowerOff ....");

    CmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_ICCPOWEROFF_STRUC)));
    ASSERT(CmdBuffer);
    if (!CmdBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)CmdBuffer, sizeof(PC_TO_RDR_ICCPOWEROFF_STRUC));

    //
    // Prepare the buffer
    //
    CmdBuffer->bMessageType = PC_TO_RDR_ICCPOWEROFF;
    CmdBuffer->dwLength = 0;
    CmdBuffer->bSlot = FpICCDevice->Slot;
    CmdBuffer->bSeq = gUsbData->CcidSequence;

    Data = USBCCIDIssueBulkTransfer(FpDevInfo, 0, 
                                    (UINT8 *)CmdBuffer, 
                                    sizeof(PC_TO_RDR_ICCPOWEROFF_STRUC)
                                    );

    //
    // Handle Error if any. This error is due to blk transfer
    //
    if (!Data) {
        Status = EFI_DEVICE_ERROR;
    }
    else {
        FpICCDevice->ConfiguredStatus = 0;
    }

    USB_MemFree(CmdBuffer, (UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_ICCPOWEROFF_STRUC)));

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r ....", Status);

    return Status;

}

/**
    PC_TO_RDR_GETSLOTSTATUS cmd is issued to CCID
    See section 6.1.3 of CCID spec Rev 1.1 for more details
    
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure


    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/
EFI_STATUS
PCToRDRGetSlotStatus(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice
)
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    PC_TO_RDR_GETSLOT_STATUS_STRUC *CmdBuffer;
    UINT32                        Data;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "PCToRDRGetSlotStatus ....");

    CmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_GETSLOT_STATUS_STRUC)));
    ASSERT(CmdBuffer);
    if (!CmdBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)CmdBuffer, sizeof(PC_TO_RDR_GETPARAMETERS_STRUC));
    
    //
    // Prepare cmd buffer
    //
    CmdBuffer->bMessageType = PC_TO_RDR_GETSLOTSTATUS;
    CmdBuffer->bSlot = FpICCDevice->Slot;
    CmdBuffer->bSeq = gUsbData->CcidSequence;

    Data = USBCCIDIssueBulkTransfer(FpDevInfo, 0, 
                                    (UINT8 *)CmdBuffer, 
                                    sizeof(PC_TO_RDR_GETSLOT_STATUS_STRUC)
                                    );

    //
    // Handle Error if any. This error is due to blk transfer
    //
    if (!Data) {
        Status = EFI_DEVICE_ERROR;
    }

    USB_MemFree(CmdBuffer, (UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_GETSLOT_STATUS_STRUC)));

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r ....", Status);

    return Status;

}


/**
    PC_TO_RDR_GETPARAMETERS cmd is issued to CCID
    See section 6.1.5 of CCID spec Rev 1.1 for more details
    
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure


    @retval EFI_SUCCESS       Success
    @retval Others            Error
            
**/
EFI_STATUS
PCToRDRGetParameters(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice
)
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    PC_TO_RDR_GETPARAMETERS_STRUC *CmdBuffer;
    UINT32                        Data;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "PCToRDRGetParameters ....");

    CmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_GETPARAMETERS_STRUC)));
    ASSERT(CmdBuffer);
    if (!CmdBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)CmdBuffer, sizeof(PC_TO_RDR_GETPARAMETERS_STRUC));

    //
    // Prepare cmd buffer
    //
    CmdBuffer->bMessageType = PC_TO_RDR_GETPARAMETERS;
    CmdBuffer->bSlot = FpICCDevice->Slot;
    CmdBuffer->bSeq = gUsbData->CcidSequence;

    Data = USBCCIDIssueBulkTransfer(FpDevInfo, 0, 
                                    (UINT8 *)CmdBuffer, 
                                    sizeof(PC_TO_RDR_GETPARAMETERS_STRUC)
                                    );

    //
    // Handle Error if any. This error is due to blk transfer
    //
    if (!Data) {
        Status = EFI_DEVICE_ERROR;
    }

    USB_MemFree(CmdBuffer, (UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_GETPARAMETERS_STRUC)));

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r ....", Status);

    return Status;

}

/**
    PC_TO_RDR_SETPARAMETERS cmd is issued to CCID
    See section 6.1.7 of CCID spec Rev 1.1 for more details
    
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure
    @param ProtocolNum        0 : T=0, 1 : T=1
    @param Data               Points to data from abProtocolDataStructure
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/
EFI_STATUS
PCToRDRSetParameters(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice,
    IN UINT8            ProtocolNum,
    IN VOID             *Data
)
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    PC_TO_RDR_SETPARAMETERS_T0_STRUC *CmdBuffer;
    UINT32                        DwData;
    UINT8                        Length = ProtocolNum == 0 ? sizeof(PROTOCOL_DATA_T0) : sizeof(PROTOCOL_DATA_T1);

    USB_DEBUG (DEBUG_INFO, DEBUG_LEVEL_3, "PCToRDRSetParameters .... ProtocolNum : %x ", ProtocolNum);

    CmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(Length + sizeof(RDR_HEADER)));
    ASSERT(CmdBuffer);
    if (!CmdBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)CmdBuffer, Length + sizeof(RDR_HEADER));

    //
    // Prepare 
    //
    CmdBuffer->bMessageType = PC_TO_RDR_SETPARAMETERS;
    CmdBuffer->dwLength = Length;
    CmdBuffer->bSlot = FpICCDevice->Slot;
    CmdBuffer->bSeq = gUsbData->CcidSequence;
    CmdBuffer->bProtocolNum = ProtocolNum;

    CopyMem((UINT8 *)CmdBuffer + sizeof(RDR_HEADER), (UINT8 *)Data, Length);

    DwData = USBCCIDIssueBulkTransfer(FpDevInfo, 0, 
                                    (UINT8 *)CmdBuffer, 
                                    Length + sizeof(RDR_HEADER)
                                    );

    //
    // Handle Error if any. This error is due to blk transfer
    //
    if (!DwData) {
        Status = EFI_DEVICE_ERROR;
    }

    USB_MemFree(CmdBuffer, (UINT8)GET_MEM_BLK_COUNT(Length + sizeof(RDR_HEADER)));

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r ....", Status);

    return Status;

}

/**
    PC_TO_RDR_SETDATARATEANDCLOCK cmd is issued. 
    Response for this cmd is from RDR_TO_PC_DATARATEANDCLOCK
    See section 6.1.14 of CCID spec Rev 1.1 for more detail
         
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure 
    @param ClockFrequency     ICC Clock Frequency in KHz
    @param DataRate           ICC data rate in bpd

    @retval EFI_SUCCESS       Success
    @retval Others            Error
    
**/
EFI_STATUS
PCToRDRSetDataRateAndClockFrequency(
    IN DEV_INFO          *FpDevInfo,
    IN ICC_DEVICE        *FpICCDevice,
    IN UINT32            ClockFrequency, 
    IN UINT32            DataRate
)
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY_STRUC *CmdBuffer;
    UINT32                        Data;

    USB_DEBUG (DEBUG_INFO, DEBUG_LEVEL_3, "PCToRDRSetDataRateAndClockFrequency ....");

    CmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY_STRUC)));
    ASSERT(CmdBuffer);
    if (!CmdBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)CmdBuffer, sizeof(PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY_STRUC));

    //
    // Prepare cmd buffer
    //
    CmdBuffer->bMessageType = PC_TO_RDR_SETDATARATEANDCLOCK;
    CmdBuffer->dwLength = 8;
    CmdBuffer->bSlot = FpICCDevice->Slot;
    CmdBuffer->bSeq = gUsbData->CcidSequence;
    CmdBuffer->dwCloclFrequency = ClockFrequency;
    CmdBuffer->dwDataRate = DataRate;

    Data = USBCCIDIssueBulkTransfer(FpDevInfo, 0, 
                                    (UINT8 *)CmdBuffer, 
                                    sizeof(PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY_STRUC)
                                    );

    //
    // Handle Error if any. This error is due to blk transfer
    //
    if (!Data) {
        Status = EFI_DEVICE_ERROR;
    }

    USB_MemFree(CmdBuffer, (UINT8)GET_MEM_BLK_COUNT(sizeof(PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY_STRUC)));

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r ....", Status);

    return Status;

}

/**
    RDR_TO_PC_DATABLOCK cmd is issued to the CCID. 
    This is on response to PCI_to_RDR_XfrBlock
    See section 6.2.1 of CCID spec Rev 1.1 for more details  
         
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure 
    @param Length             Length of bytes in Buffer
    @param Buffer             Points to abData in RDR_TO_PC_DATABLOCK

    @retval EFI_SUCCESS       Success
    @retval Others            Error
     
**/
EFI_STATUS
RDRToPCDataBlock(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice,
    IN OUT UINT32       *Length,
    OUT UINT8           *Buffer

)
{    

    EFI_STATUS      Status = EFI_SUCCESS;
    RDR_TO_PC_DATABLOCK_STRUC*  FpReceiveBuffer;
    UINT32          Data;
    UINT8           Iterations = 3;
    UINT32          InputLength = *Length;
    UINT32          Index;   

    //
    // Allocate memory for receiving data
    //
    FpReceiveBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_DATABLOCK_STRUC) + *Length));
    ASSERT(FpReceiveBuffer);
    if (!FpReceiveBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)FpReceiveBuffer, sizeof(RDR_TO_PC_DATABLOCK_STRUC) + *Length);

    do {
        //
        // Get the response 
        //
        FpReceiveBuffer->bMessageType = RDR_TO_PC_DATABLOCK;
        FpReceiveBuffer->dwLength = *Length;
        FpReceiveBuffer->bSlot = FpICCDevice->Slot;
        FpReceiveBuffer->bSeq = gUsbData->CcidSequence;
        FpReceiveBuffer->bStatus = 0;
        FpReceiveBuffer->bError = 0;
        FpReceiveBuffer->bChainParameter = 0;

        Data = USBCCIDIssueBulkTransfer(FpDevInfo, BIT7, 
                                        (UINT8 *)FpReceiveBuffer, 
                                        sizeof(RDR_TO_PC_DATABLOCK_STRUC) + *Length
                                        );

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\n");

        for (Index =0; Index < sizeof(RDR_TO_PC_DATABLOCK_STRUC) + FpReceiveBuffer->dwLength; Index++) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%02X ", ((UINT8 *)FpReceiveBuffer)[Index]);
        }

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\n");

        //    
        // Handle Error if any. This error is due to blk transfer
        //
        if (!Data) {
            Status = EFI_DEVICE_ERROR;
            goto exit_RDRToPCDataBlock;
        }    

        //
        // Check for time extension 
        //
        if ((FpReceiveBuffer->bStatus & 0xC0) == 0x80) {
            FixedDelay(FpReceiveBuffer->bError * FpICCDevice->WaitTime * 1000);  
        } else {
            break;
        }

        Iterations--;
    } while (Iterations);    

    // Should the cmd be aborted if the response isn't received???

    //
    // Processed without error if Zero
    //
    if (FpReceiveBuffer->bStatus & 0xC0) {
        Status = EFI_DEVICE_ERROR;

        if ((FpReceiveBuffer->bStatus & 0x3) == 0x2) {        
            Status = EFI_NOT_FOUND;
        }
    }

    if (Iterations && !EFI_ERROR(Status)) {
    
        FpICCDevice->bChainParameter = FpReceiveBuffer->bChainParameter;

        //
        // If response is successful get the data
        //
        if (FpReceiveBuffer->dwLength && FpReceiveBuffer->dwLength <= *Length) {
    
            // Copy data 
            CopyMem(Buffer,
                    (UINT8 *)FpReceiveBuffer + sizeof(RDR_TO_PC_DATABLOCK_STRUC),
                    FpReceiveBuffer->dwLength);

        }

        if  (FpReceiveBuffer->dwLength > *Length) {
            Status = EFI_BUFFER_TOO_SMALL;
        }

        //
        // Update the o/p buffer length
        //
        *Length = FpReceiveBuffer->dwLength;

    } else {

        Status = EFI_DEVICE_ERROR;
        *Length = 0;
    }

    //
    // Save the last cmd status 
    //
    FpICCDevice->bStatus = FpReceiveBuffer->bStatus;
    FpICCDevice->bError = FpReceiveBuffer->bError;


    //    
    // if success exit
    //
    if (!EFI_ERROR(Status) && !FpICCDevice->bStatus) {
        Status =  EFI_SUCCESS;
        goto exit_RDRToPCDataBlock;
    }

    // Card not present?
    Status = EFI_NOT_FOUND;
    if ((FpReceiveBuffer->bStatus & 7) == 2) goto exit_RDRToPCDataBlock;

    //
    // Other errors
    //
    Status = EFI_DEVICE_ERROR;

exit_RDRToPCDataBlock:

    gUsbData->CcidSequence++;

    USB_MemFree(FpReceiveBuffer, 
                (UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_DATABLOCK_STRUC) + InputLength)
                );

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " Status : %r bStatus : %02X bError : %02X\n", Status, FpICCDevice->bStatus, FpICCDevice->bError);
    
    return Status;

}

/**
    RDR_TO_PC_SLOTSTATUS cmd is issued to CCID.
    See section 6.2.2 of CCID spec Rev 1.1 for more details.
        
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure 

    @retval EFI_SUCCESS       Success
    @retval Others            Error

    
**/
EFI_STATUS
RDRToPCSlotStatus(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice
)
{

    EFI_STATUS                  Status = EFI_SUCCESS;
    RDR_TO_PC_SLOTSTATUS_STRUC  *FpReceiveBuffer;
    UINT32                      Data;
    UINT8                       Iterations = 3;    

    FpReceiveBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_SLOTSTATUS_STRUC)));
    ASSERT(FpReceiveBuffer);
    if (!FpReceiveBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)FpReceiveBuffer, sizeof(RDR_TO_PC_SLOTSTATUS_STRUC));
    do {
        //
        // Read the PCSlot Status
        //
        FpReceiveBuffer->bMessageType = RDR_TO_PC_SLOTSTATUS;
        FpReceiveBuffer->dwLength = 0;
        FpReceiveBuffer->bSlot = FpICCDevice->Slot;
        FpReceiveBuffer->bSeq = gUsbData->CcidSequence;

        Data = USBCCIDIssueBulkTransfer(FpDevInfo, BIT7, 
                                        (UINT8 *)FpReceiveBuffer, 
                                        sizeof(RDR_TO_PC_SLOTSTATUS_STRUC)
                                        );

        //
        // Handle Error if any. This error is due to blk transfer
        //
        if (!Data) {
            Status = EFI_DEVICE_ERROR;
            goto exit_RDRToPCSlotStatus;
        }

        //
        // Check for time extension 
        //
        if ((FpReceiveBuffer->bStatus & 0xC0) == 0x80) {
            FixedDelay(FpReceiveBuffer->bError * FpICCDevice->WaitTime * 1000);  
        } else {
            break;
        }

        Iterations--;
    } while (Iterations);


    //
    // Save the last cmd status 
    //
    FpICCDevice->bStatus = FpReceiveBuffer->bStatus;
    FpICCDevice->bError = FpReceiveBuffer->bError;

    // Processed without error if Zero
    if (FpReceiveBuffer->bStatus & 0xC0) {
        Status = EFI_DEVICE_ERROR;

        if ((FpReceiveBuffer->bStatus & 0x3) == 0x2) {        
            Status = EFI_NOT_FOUND;
        }

    }

    if (Iterations && !EFI_ERROR(Status)) {
        //
        // Update the last ClockStatus
        //
        FpICCDevice->bClockStatus = FpReceiveBuffer->bClockStatus; 
    } else {
        Status = EFI_DEVICE_ERROR;
    }

exit_RDRToPCSlotStatus:

    gUsbData->CcidSequence++;

    USB_MemFree(FpReceiveBuffer, 
                (UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_SLOTSTATUS_STRUC))
                );

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " Status :  %r bStatus : %02X bError : %02X\n", Status, FpICCDevice->bStatus, FpICCDevice->bError);

    return Status;
}

/**
    RDR_TO_PC_SLOTSTATUS cmd is issued
    See section 6.2.3 of CCID spec Rev 1.1 for more details
         
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure 

    @retval EFI_SUCCESS       Success
    @retval Others            Error
       
**/
EFI_STATUS
RDRToPCParameters(
    IN DEV_INFO           *FpDevInfo,
    IN ICC_DEVICE        *FpICCDevice
)
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    RDR_TO_PC_PARAMETERS_T1_STRUC *FpReceiveBuffer;
    UINT32                        Data;
    UINT8                         Iterations = 3;

    FpReceiveBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_PARAMETERS_T1_STRUC)));
    ASSERT(FpReceiveBuffer);
    if (!FpReceiveBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)FpReceiveBuffer, sizeof(RDR_TO_PC_PARAMETERS_T1_STRUC));

    do {

        //
        // Read the PCSlot Status
        //
        FpReceiveBuffer->Header.bMessageType = RDR_TO_PC_PARAMETERS;
        FpReceiveBuffer->Header.dwLength = 0;
        FpReceiveBuffer->Header.bSlot = FpICCDevice->Slot;
        FpReceiveBuffer->Header.bSeq = gUsbData->CcidSequence;

        Data = USBCCIDIssueBulkTransfer(FpDevInfo, BIT7, 
                                        (UINT8 *)FpReceiveBuffer, 
                                        sizeof(RDR_TO_PC_PARAMETERS_T1_STRUC)
                                        );

        //
        // Handle Error if any. This error is due to blk transfer
        //
        if (!Data) {
            Status = EFI_DEVICE_ERROR;
            goto exit_RDRToPCParameters;
        }

        //
        // Check for time extension 
        //
        if ((FpReceiveBuffer->Header.bStatus & 0xC0) == 0x80) {
           FixedDelay(FpReceiveBuffer->Header.bError * FpICCDevice->WaitTime * FpICCDevice->etu);  
        } else {
           break;
        }

        Iterations--;

    } while (Iterations);

    //
    // Save the last cmd status 
    //
    FpICCDevice->bStatus = FpReceiveBuffer->Header.bStatus;
    FpICCDevice->bError = FpReceiveBuffer->Header.bError;

    //
    // Processed without error if Zero
    //
    if (FpReceiveBuffer->Header.bStatus & 0xC0) {
        Status = EFI_DEVICE_ERROR;

        if ((FpReceiveBuffer->Header.bStatus & 0x3) == 0x2) {        
            Status = EFI_NOT_FOUND;
        }

    }

    if (Iterations && !EFI_ERROR(Status)) {

        //
        // Update the Data
        //
        CopyMem((UINT8 *)&(FpICCDevice->bProtocolNum),
                (UINT8 *)&(FpReceiveBuffer->Header.Data),
                sizeof(RDR_TO_PC_PARAMETERS_T1_STRUC) - 9);

    } else {
        Status = EFI_DEVICE_ERROR;
    }

exit_RDRToPCParameters:

    gUsbData->CcidSequence++;

    USB_MemFree(FpReceiveBuffer, 
                (UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_PARAMETERS_T1_STRUC))
                );

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " Status :  %r bStatus : %02X bError : %02X\n", Status, FpICCDevice->bStatus, FpICCDevice->bError);

    PrintPCParameters((UINT8 *)&(FpICCDevice->bProtocolNum));

    return Status;

}


/**
    RDR_TO_PC_DATARATEANDCLOCK cmd is issued. 
    Returns dwClockFrequency and dwDataRate.See section 6.2.5 of CCID spec Rev 1.1 for more details.
         
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure 

    @retval EFI_SUCCESS       Success
    @retval Others            Error
         
**/
EFI_STATUS
RDRToPCDataRateAndClockFrequency(
    IN DEV_INFO          *FpDevInfo,
    IN ICC_DEVICE        *FpICCDevice
)
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    RDR_TO_PC_DATARATEANDCLOCKFREQUENCY_STRUC  *FpReceiveBuffer;
    UINT32                        Data;
    UINT8                        Iterations = 3;    

    FpReceiveBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_DATARATEANDCLOCKFREQUENCY_STRUC)));
    ASSERT(FpReceiveBuffer);
    if (!FpReceiveBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)FpReceiveBuffer, sizeof(RDR_TO_PC_DATARATEANDCLOCKFREQUENCY_STRUC));

    do {

        //
        // Read the PCSlot Status
        //
        FpReceiveBuffer->bMessageType = RDR_TO_PC_DATARATEANDCLOCK;
        FpReceiveBuffer->dwLength = 8;
        FpReceiveBuffer->bSlot = FpICCDevice->Slot;
        FpReceiveBuffer->bSeq = gUsbData->CcidSequence;

        Data = USBCCIDIssueBulkTransfer(FpDevInfo, BIT7, 
                                        (UINT8 *)FpReceiveBuffer, 
                                        sizeof(RDR_TO_PC_DATARATEANDCLOCKFREQUENCY_STRUC));

        //
        // Handle Error if any. This error is due to blk transfer
        //
        if (!Data) {
            Status = EFI_DEVICE_ERROR;
            goto exit_RDRToPCDataRateAndClockFrequency;
        }

        //
        // Check for time extension 
        //
        if ((FpReceiveBuffer->bStatus & 0xC0) == 0x80) {
            FixedDelay(FpReceiveBuffer->bError * FpICCDevice->WaitTime * 1000);  
        } else {
            break;
        }

        Iterations--;

    } while (Iterations);


    //
    // Processed without error if Zero
    //
    if (FpReceiveBuffer->bStatus & 0xC0) {
        Status = EFI_DEVICE_ERROR;

        if ((FpReceiveBuffer->bStatus & 0x3) == 0x2) {        
            Status = EFI_NOT_FOUND;
        }
    }

    if (Iterations && !EFI_ERROR(Status)) {

         FpICCDevice->dwClockFrequency = FpReceiveBuffer->dwClockFrequency; 
         FpICCDevice->dwDataRate = FpReceiveBuffer->dwDataRate;

    } else {

        Status = EFI_DEVICE_ERROR;

    }

    //
    // Save the last cmd status 
    //
    FpICCDevice->bStatus = FpReceiveBuffer->bStatus;
    FpICCDevice->bError = FpReceiveBuffer->bError;

exit_RDRToPCDataRateAndClockFrequency:

    gUsbData->CcidSequence++;

    USB_MemFree(FpReceiveBuffer, 
                (UINT8)GET_MEM_BLK_COUNT(sizeof(RDR_TO_PC_DATARATEANDCLOCKFREQUENCY_STRUC))
                );

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " Status :  %r bStatus : %02X bError : %02X\n", 
                Status, FpICCDevice->bStatus, FpICCDevice->bError);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " dwClockFrequency :  %4x dwDataRate : %4x\n", 
                FpICCDevice->dwClockFrequency, FpICCDevice->dwDataRate);

    return Status;

}

/**
    Transmit/Receive T1 ADPU
          
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure 
    @param CmdLength          Command length
    @param CmdBuffer          Command buffer
    @param ResponseLength     Response length
    @param ResponseLength     Response length

    @retval EFI_SUCCESS       Success
    @retval Others            Error


**/
EFI_STATUS
TxRxT1Adpu (
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice,
    IN UINT32           CmdLength,
    IN UINT8            *CmdBuffer,
    OUT UINT32          *ResponseLength,
    OUT UINT8           *ResponseBuffer
)
{

    EFI_STATUS  Status;

    //
    // Issue the cmd
    //
    Status = PCToRDRXfrBlock(FpDevInfo, FpICCDevice, CmdLength, CmdBuffer, 0, 0);
    
    if (EFI_ERROR(Status)){
        return Status;        
    }

    //
    // Get the response
    //
    Status = RDRToPCDataBlock(FpDevInfo, FpICCDevice, ResponseLength, ResponseBuffer);

    return Status;
}

/**
    Transmit/Receive T1 TDPU/Character         
           
    @param FpDevInfo          Pointer to devInfo structure
    @param FpICCDevice        Pointer to ICC_DEVICE structure 
    @param CmdLength          Command length
    @param CmdBuffer          Command buffer
    @param ResponseLength     Response length
    @param ResponseLength     Response length

    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/
EFI_STATUS
TxRxT1TDPUChar (
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice,
    IN UINT32           CmdLength,
    IN UINT8            *CmdBuffer,
    IN UINT8            ISBlock,
    IN OUT UINT32       *ResponseLength,
    OUT UINT8           *ResponseBuffer
)
{

    EFI_STATUS  Status;
    UINT8       Pcb = ISBlock;


    UINT32      IBlockFrameLength = 0;      // Used for I-Block
    UINT8       *IBlockFrame = NULL;

    UINT32      SendBlockFrameLength = 0;   // Place holder for the block currently sent
    UINT8       *SendBlockFrame = NULL;

    UINT32      RBlockFrameLength = 0;      // Used for R-Block
    UINT8       *RBlockFrame = NULL;

    UINT32      SBlockFrameLength = 0;      // Used for S-Block
    UINT8       *SBlockFrame = NULL;

    UINT32      lResponseLength = 0;        // Response buffer for all the blocks I/S/R
    UINT32      OrglResponseLength = 0;     
    UINT8       *lResponseBuffer;

    UINT8       wLevelParameter = 0;

    UINT8       ReceiveStatus;
    UINT8       bBWIByte = 0;    

    UINT32      UserBufferLength = *ResponseLength;
    UINT32      lResponseBufferAddDataPtr = 0;
    CCID_DEV_INFO   *CcidDevData;

    BOOLEAN     T1Char;
    UINT8     *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return EFI_DEVICE_ERROR;

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return EFI_DEVICE_ERROR;
    }
    
    T1Char = ((((SMARTCLASS_DESC*)CcidDevData->CcidDescriptor)->dwFeatures) & TDPU_LEVEL_EXCHANGE) ? FALSE : TRUE;
    
    // Initialize Chaining is off
    FpICCDevice->Chaining = FALSE;
    *ResponseLength  = 0;

    // Update Pcb with Nas only for IBlocks
    if (!ISBlock) {
        Pcb = ((FpICCDevice->NaSInterface & 1) << 6);
    }

    Status = ConstructBlockFrame(FpDevInfo, FpICCDevice, 
                                FpICCDevice->NAD, Pcb, 
                                CmdLength, CmdBuffer, 
                                &wLevelParameter, &IBlockFrameLength, 
                                &IBlockFrame
                                );        


    if (EFI_ERROR(Status)) { 
        return Status;
    }

    SendBlockFrameLength = IBlockFrameLength;
    SendBlockFrame = IBlockFrame;

    if (UserBufferLength < 2)  lResponseLength = 2;

    lResponseLength += (UserBufferLength + 3 + (FpICCDevice->EpilogueFields == 0 ? 1 :  2));

    lResponseBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(lResponseLength));
    ASSERT(lResponseBuffer);
    if (!lResponseBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem(lResponseBuffer, lResponseLength);

    OrglResponseLength = lResponseLength;

    FpICCDevice->T1CharCmdDataPhase = TRUE; // Always Cmd Phase first

    do {

        Status = PCToRDRXfrBlock(FpDevInfo, FpICCDevice, 
                                SendBlockFrameLength, SendBlockFrame, 
                                bBWIByte, wLevelParameter
                                );

        if (EFI_ERROR(Status)){
            break;        
        }

        //
        // Get the response
        //
        lResponseLength = OrglResponseLength - lResponseBufferAddDataPtr;
        Status = RDRToPCDataBlock(FpDevInfo, FpICCDevice, &lResponseLength, lResponseBuffer + lResponseBufferAddDataPtr);

        if (EFI_ERROR(Status)){
            break;        
        }

        // Check for errors
        ReceiveStatus = HandleReceivedBlock(FpDevInfo, FpICCDevice, 
                                            IBlockFrameLength, IBlockFrame, 
                                            SendBlockFrameLength, SendBlockFrame, 
                                            lResponseBuffer
                                            );        

        bBWIByte = 0;

        switch (ReceiveStatus) {

            case BLOCK_TRANSMISION_SUCCESS:
                break;
    
            case RESEND_BLOCK:
                break;

            case SEND_R_BLOCK_1:
            case SEND_R_BLOCK_0:

                // Check if Chaining is in progress
                if (FpICCDevice->Chaining) {

                    // Copy the data from lResponseBuffer to the user buffer
                    //
                    // If success copy the data to Response buffer
                    //
                    if (lResponseBuffer[2] && ((UserBufferLength - *ResponseLength) < lResponseBuffer[2])) {
                        Status = EFI_BUFFER_TOO_SMALL;        
                    }
                    CopyMem(ResponseBuffer + *ResponseLength, lResponseBuffer + 3, lResponseBuffer[2]);
                    *ResponseLength +=  lResponseBuffer[2];
                    lResponseBufferAddDataPtr = 0;          // Reset to use the lResponseBuffer from the beginning
                    
                    // Clear out the PCB/length feild so that by mistake the buffer is interpreted as valid data
                    lResponseBuffer[1] = 0;
                    lResponseBuffer[2] = 0;
                    lResponseLength = OrglResponseLength;
                    
                }
                Status = ConstructBlockFrame(FpDevInfo, FpICCDevice,    
                                            FpICCDevice->NAD, ReceiveStatus == SEND_R_BLOCK_1 ? 0x80 | 0x10 : 0x80, 
                                            0, NULL, &wLevelParameter, 
                                            &RBlockFrameLength, &RBlockFrame
                                            );        

                if (EFI_ERROR(Status)) {
                    ReceiveStatus = BLOCK_TRANSMISSION_TERMINATE;
                }
                SendBlockFrameLength = RBlockFrameLength;
                SendBlockFrame = RBlockFrame;
                FpICCDevice->T1CharCmdDataPhase = TRUE;
                break;

            case I_BLOCK_RESEND:

                Status = ConstructBlockFrame(FpDevInfo, FpICCDevice, 
                                            FpICCDevice->NAD, Pcb, CmdLength, 
                                            CmdBuffer, &wLevelParameter, 
                                            &IBlockFrameLength, &IBlockFrame
                                            );        

                if (EFI_ERROR(Status)) {
                    ReceiveStatus = BLOCK_TRANSMISSION_TERMINATE;
                }
                SendBlockFrameLength = IBlockFrameLength;
                SendBlockFrame = IBlockFrame;
                FpICCDevice->T1CharCmdDataPhase = TRUE;
                break;

            case WTX_RESPONSE:

                bBWIByte = lResponseBuffer[3];

                Status = ConstructBlockFrame(FpDevInfo, FpICCDevice, 
                                            FpICCDevice->NAD, WTX_RESPONSE, 
                                            lResponseBuffer[2], lResponseBuffer + 3, 
                                            &wLevelParameter, &SBlockFrameLength, 
                                            &SBlockFrame
                                            );        

                if (EFI_ERROR(Status)) {
                    ReceiveStatus = BLOCK_TRANSMISSION_TERMINATE;
                }

                SendBlockFrameLength = SBlockFrameLength;
                SendBlockFrame = SBlockFrame;
                FpICCDevice->T1CharCmdDataPhase = TRUE;
                break;

            case GET_DATA_T1_CHAR:
                
                //
                // Issue a PCToRDRXfrBlock with dwLength to zero. 
                // Check Page 68 of CCID spec Rev 1.1, Apr 22, 2005
                //

                SendBlockFrameLength = 0;
                // Assumption : only LRC is supported
                wLevelParameter = lResponseBuffer[2] + 1; 

                //
                // Since the prologue is received in the first three bytes increment 
                // the address so that data is recived after that  
                //
                lResponseBufferAddDataPtr += 3;   

                //
                // Indicate it is data phase now                  
                //
                FpICCDevice->T1CharCmdDataPhase = FALSE;  
                break;

            case IFS_RESPONSE:
                Status = ConstructBlockFrame(FpDevInfo, FpICCDevice, 
                                            FpICCDevice->NAD, IFS_RESPONSE, 
                                            lResponseBuffer[2], lResponseBuffer + 3, 
                                            &wLevelParameter, &SBlockFrameLength, 
                                            &SBlockFrame
                                            );        

                if (EFI_ERROR(Status)) {
                    ReceiveStatus = BLOCK_TRANSMISSION_TERMINATE;
                }

                SendBlockFrameLength = SBlockFrameLength;
                SendBlockFrame = SBlockFrame;
                FpICCDevice->T1CharCmdDataPhase = TRUE;
                break;

            case ABORT_RESPONSE:
                 Status = ConstructBlockFrame(FpDevInfo, FpICCDevice, 
                                            FpICCDevice->NAD, ABORT_RESPONSE, 
                                            lResponseBuffer[2], lResponseBuffer + 3, 
                                            &wLevelParameter, &SBlockFrameLength, 
                                            &SBlockFrame
                                            );        

                if (EFI_ERROR(Status)) {
                    ReceiveStatus = BLOCK_TRANSMISSION_TERMINATE;
                }

                SendBlockFrameLength = SBlockFrameLength;
                SendBlockFrame = SBlockFrame;
                FpICCDevice->T1CharCmdDataPhase = TRUE;
                break;

            default:
                break;
                        
        }

        if (ReceiveStatus == BLOCK_TRANSMISION_SUCCESS) {
            break;
        }       
    }while (1);

    //
    // If success copy the data to Response buffer for the last I-Block that was received.
    //
    if (lResponseBuffer[2] && ((UserBufferLength - *ResponseLength) < lResponseBuffer[2])) {
        Status = EFI_BUFFER_TOO_SMALL;        
    }

    if (lResponseBuffer[2] && ((UserBufferLength - *ResponseLength) >= lResponseBuffer[2])) {
        CopyMem(ResponseBuffer + *ResponseLength, lResponseBuffer + 3, lResponseBuffer[2]);
        *ResponseLength +=  lResponseBuffer[2];
    }

    //
    // Free up memory I-Block allocated here
    //
    if (IBlockFrame && IBlockFrameLength) {
        USB_MemFree(IBlockFrame, (UINT8)GET_MEM_BLK_COUNT(IBlockFrameLength));
    }

    //
    // Free up S-Block memory allocated here
    //
    if (SBlockFrame && SBlockFrameLength) {
        USB_MemFree(SBlockFrame, (UINT8)GET_MEM_BLK_COUNT(SBlockFrameLength));
    }

    if (lResponseBuffer && OrglResponseLength) {
        USB_MemFree(lResponseBuffer, (UINT8)GET_MEM_BLK_COUNT(OrglResponseLength));
    }



    return Status;
}


/**
    Construct the Block Frame for the CCID
        
    @param FpDevInfo         
    @param FpICCDevice        
    @param Nad          
    @param PCB          
    @param InfLength     
    @param InfBuffer    
    @param LevelParameter          
    @param BlockFrameLength     
    @param BlockFrame
    
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/
EFI_STATUS
ConstructBlockFrame(
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice,
    IN UINT8            Nad,
    IN UINT8            PCB,
    IN UINT32           InfLength,
    IN UINT8            *InfBuffer,
    OUT UINT8           *LevelParameter,
    OUT UINT32          *BlockFrameLength,
    OUT UINT8           **BlockFrame
)
{

    UINT32  BufLengthRequired = InfLength + 3 + (FpICCDevice->EpilogueFields == 0 ? 1 :  2);
    UINT8   *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO   *CcidDevData;


    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return EFI_DEVICE_ERROR;

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return EFI_DEVICE_ERROR;
    }
    //
    // Check if the input buffer if already allocated is enough for the current case. 
    // If not free it up and allocate again.
    //
    
    if (BufLengthRequired > *BlockFrameLength) {
        if (*BlockFrame) {
            USB_MemFree(*BlockFrame, (UINT8)GET_MEM_BLK_COUNT(*BlockFrameLength));
            *BlockFrame = NULL;

        }
    }

    *BlockFrameLength = InfLength + 3 + (FpICCDevice->EpilogueFields == 0 ? 1 :  2);

    //
    // if BlockFrame is NULL only then allocate memory. Assumption is if Memory 
    // has been allocated before then it is sufficent enough for the length needed.
    //
    if (!*BlockFrame) {
        //
        // Allocate length needed to contruct the Block Frame
        //
        *BlockFrame = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(*BlockFrameLength));

        if (!*BlockFrame) {
            return EFI_OUT_OF_RESOURCES;
        }
    }

    ZeroMem(*BlockFrame, *BlockFrameLength);

    (*BlockFrame)[0] = Nad;
    (*BlockFrame)[1] = PCB;
    (*BlockFrame)[2] = (UINT8)InfLength;

    if (InfLength) {
        CopyMem((UINT8 *)(*BlockFrame + 3), (UINT8 *)InfBuffer, InfLength);
    }

    //
    // Update Checksum
    //
    (*BlockFrame)[*BlockFrameLength - 1] = 0;

    if (FpICCDevice->EpilogueFields == 0) {
        CalculateLRCChecksum(*BlockFrame, *BlockFrameLength);
    } else {
        return EFI_UNSUPPORTED;
    }

    //
    // For Character transfer update wLevelParameter also
    //
    if (!(((SMARTCLASS_DESC*)CcidDevData->CcidDescriptor)->dwFeatures & 0x70000)) {
        *LevelParameter = 3;
    }

    return EFI_SUCCESS;    

}

/**
    Process the Recevied data from CCID device

        
    @param FpDevInfo 
    @param FpICCDevice 
    @param OriginalBlockFrameLength 
    @param OriginalBlockFrame 
    @param SentBlockFrameLength 
    @param SentBlockFrame 
    @param ReceivedBlockFrame 

    @retval EFI_SUCCESS       Success
    @retval Others            Error


    @note  
      For Character exchange control will come twice for S(Response), I-Block with M bit set. So while counting
      number of exchnages this needs to be taken care of.
      Refer to ISO/IEC 7816-1 First edition 1998-10-15 for different scenarios mentioned in this function.

**/
UINT8   
HandleReceivedBlock (
    IN DEV_INFO         *FpDevInfo,
    IN ICC_DEVICE       *FpICCDevice,
    IN UINT32           OriginalBlockFrameLength,
    IN UINT8            *OriginalBlockFrame,
    IN UINT32           SentBlockFrameLength,
    IN UINT8            *SentBlockFrame,
    IN UINT8            *ReceivedBlockFrame
)
{

    UINT8        ReturnParameter = BLOCK_TRANSMISION_SUCCESS;
    BOOLEAN      T1Char;
    UINT8        *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO   *CcidDevData;
    EFI_STATUS Status = EFI_SUCCESS;


    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return BLOCK_TRANSMISSION_TERMINATE;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) {
            return BLOCK_TRANSMISSION_TERMINATE;
    }


    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return BLOCK_TRANSMISSION_TERMINATE;
    }

    T1Char = (((SMARTCLASS_DESC*)CcidDevData->CcidDescriptor)->dwFeatures & 0x70000) ? FALSE : TRUE;

    // It is easy to support T1 TDPU & CHAR as they are almost same except that 
    // prologue and data are received separatly in T1 Char.
    // The trick here will be that when data is received we can combine the 
    // previously received prologue and the INF/Epilogue received 
    // later so that it will be similar to T1 TDPU. Then all the processing will be same.

    if (!ReceivedBlockFrame) {    // if no response

        if (FpICCDevice->RBlockCounter == 2) {
            return BLOCK_TRANSMISSION_TERMINATE;
        }


        
        // If I-Block sent before and no response, send R-Block with the expected I Block(N(R). Rule 7.1/Rule 7.6
        if (!(SentBlockFrame[1] & 0x80)) { 
            
            FpICCDevice->RBlockCounter++;

            if (FpICCDevice->NaSCard) {
             return SEND_R_BLOCK_0;
            } else {
                return SEND_R_BLOCK_1;
            }
        }
        
    }

    // Reset the RBlock Counter if the response we received isn't a R-Block.
    if ((ReceivedBlockFrame[1] & 0xC0) !=  RBLOCK) {
            FpICCDevice->RBlockCounter = 0;
    }

    //
    // Is the block received an I-Block?
    //
    if (!(ReceivedBlockFrame[1] & 0x80)) {

        //
        // It is T1 Char and also if in cmd phase handle it.
        //
        if (T1Char && FpICCDevice->T1CharCmdDataPhase)  { 

            // Save the N(s) from the card for later use.
            FpICCDevice->NaSCard = (ReceivedBlockFrame[1] & NSBIT) >> 6;

            // If data needs to be received
            if (ReceivedBlockFrame[2]){
                return GET_DATA_T1_CHAR;
            }  else {
                return BLOCK_TRANSMISION_SUCCESS;
            }            

        }

        // It is T1TDPU or it is T1 Char but in data phase

       
        // Is Mbit set, then nothing more to do
        if (!(ReceivedBlockFrame[1] & 0x20)) {
            //
            // Toggle N(S) bit only after a successful I Block Transmission
            //
            FpICCDevice->Chaining = FALSE;
            FpICCDevice->NaSInterface = !(FpICCDevice->NaSInterface);

            return BLOCK_TRANSMISION_SUCCESS;
        }
        else {
            // Since Mbit is set, Send R-Block with the next N(s) expected from card. Section 5, Rules 2.2 and 5
            
            FpICCDevice->Chaining = TRUE;

            if (FpICCDevice->NaSCard) {
                return SEND_R_BLOCK_0;
            }
            else {
                return SEND_R_BLOCK_1;
            }
        }
    }

    // No difference between T1 Char and T1 Tdpu in R-phase

    //
    // Is the Block received is a R block?
    //
    if ((ReceivedBlockFrame[1] & 0xC0) ==  RBLOCK) {
        

        // Is there an error?
        if (ReceivedBlockFrame[1] & 0x03) {
            //Re-transmit it
            if ((SentBlockFrame[1] & 0xc0) == 0x00) {
                return  I_BLOCK_RESEND;
            }
            else {
                return RESEND_BLOCK;
            }
        }

        if (T1Char && FpICCDevice->T1CharCmdDataPhase) {
            return  GET_DATA_T1_CHAR;
        }

        if (FpICCDevice->RBlockCounter == 3) {
            FpICCDevice->RBlockCounter = 0;
            return BLOCK_TRANSMISSION_TERMINATE;
        }
        FpICCDevice->RBlockCounter++;

        if (FpICCDevice->Chaining == FALSE) {

            //
            // if the received  R-Block is same as the last sent I-Block AND Chaining is not in progress, resend I-Block. Scenario 8
            //
            if ((ReceivedBlockFrame[1] & 0x10) >> 4 == (FpICCDevice->NaSInterface & 1) << 6) {
                return I_BLOCK_RESEND;
            } 
            else {
                //
                // Scenario 11/12
                //
                if (FpICCDevice->NaSInterface & 1) {
                    return SEND_R_BLOCK_1;
                } else {
                    return SEND_R_BLOCK_0;
                }
            }
        }
        else { 
            //
            // Chaining is in progress...
            //
            //   
            // Scenario 5
            //
            if ((ReceivedBlockFrame[1] & 0x10) >> 4 != (FpICCDevice->NaSInterface & 1) << 6) {
                // return I_BLOCK;
            }
            //
            // Scenario 23
            //
            if (ReceivedBlockFrame[1]  == SentBlockFrame[1]) {
                if (ReceivedBlockFrame[1] & 0x10) {
                    return SEND_R_BLOCK_1;
                } else {
                    return SEND_R_BLOCK_0;
                }
            }
           
        }

        // We can try giving S-Synch also if it doesn't respond to R-Block. 
        // S-Synch can be done only for 2nd Iblock on-wards.
    }    

    //
    // Is the Block Received is a S block? 
    //
    if ((ReceivedBlockFrame[1] & 0xC0) == 0xC0) {

        switch (ReceivedBlockFrame[1]) {

            case IFS_REQUEST:

               if (T1Char && FpICCDevice->T1CharCmdDataPhase) {
                    ReturnParameter = GET_DATA_T1_CHAR;
                    break;
                }
                // Save the new IFSD data
                FpICCDevice->IFSD = ReceivedBlockFrame[3];
                ReturnParameter = IFS_RESPONSE;
                break;

            case IFS_RESPONSE:
                //
                // It is T1 Char and also if in cmd phase handle it.
                //
                if (T1Char && FpICCDevice->T1CharCmdDataPhase)  { 

                    // If data needs to be received
                    if (ReceivedBlockFrame[2]){
                        return GET_DATA_T1_CHAR;
                    }  else {
                        return BLOCK_TRANSMISION_SUCCESS;
                    }            
                }  
                break;

            case ABORT_REQUEST:
                    FpICCDevice->Chaining = FALSE;
                    ReturnParameter = ABORT_RESPONSE;
                    break;

            case ABORT_RESPONSE:
                break;

            case WTX_REQUEST:

                if (T1Char && FpICCDevice->T1CharCmdDataPhase) {
                    ReturnParameter = GET_DATA_T1_CHAR;
                    break;
                }

                ReturnParameter = WTX_RESPONSE;
                break;                

            case RESYNCH_RESPONSE:
                break;

            case WTX_RESPONSE:              // Won't be received from card. Card will only generate WTX request.
                break;        
            case RESYNCH_REQUEST:           // Card won't issue ReSynch
                break;
            default:
                break;

        }  
  
    }                

   return ReturnParameter;

}

/**
    Calculates LRC checksum

    @param BlockFrame
    @param BlockFrameLength

    @retval None
**/
VOID
CalculateLRCChecksum (
    UINT8       *BlockFrame, 
    UINT32      BlockFrameLength
)
{
    UINT32  Index = 0;

    for (; Index < BlockFrameLength - 1; Index++ ){
        BlockFrame[BlockFrameLength-1] ^= BlockFrame[Index];
    }
    
    return;
}

/**
    Search the linked list to find the ICC_DEVICE for the given slot number.

    @param FpDevInfo      Pointer to the DevInfo structure
    @param Slot           Slot number

    @retval FpICCDevice   Pointer to the Icc device structure
    @retval NULL          Can not find any ICC_DEVICE

**/
ICC_DEVICE*
GetICCDevice(
    DEV_INFO        *FpDevInfo, 
    UINT8            Slot
)
{
    ICC_DEVICE    *FpICCDevice;
    LIST_ENTRY    *Link;
    UINT8         *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO *CcidDevData;
    EFI_STATUS Status = EFI_SUCCESS;

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if(!CcidDevData) return NULL;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) {
        return NULL;
    }
    for (Link = CcidDevData->IccDeviceList.ForwardLink;
        Link != &CcidDevData->IccDeviceList; 
        Link = Link->ForwardLink ) {
        FpICCDevice = BASE_CR(Link, ICC_DEVICE, Link);

       
        //
        // Slot # matches
        //
        if ((FpICCDevice->Signature == ICC_DEVICE_SIG) && (FpICCDevice->Slot == Slot)) {
            if (((UINTN)FpICCDevice < (UINTN)gUsbDataList->MemBlockStart) ||
                (((UINTN)FpICCDevice + sizeof(ICC_DEVICE)) > (UINTN)MemBlockEnd)) {
                return NULL;
            }
            return FpICCDevice;
        }      
          
    }

    return NULL;
}

/**
    The routine update the Transmision protocol supported and other 
    timing related data
 
    @param FpDevInfo      Pointer to the DevInfo structure
    @param FpICCDevice    Pointer to the Icc device structure

          
    @retval None

    @note  This function looks into ATR data and updates CLASS A/B/C information, 
              calculates ETU, WaitTime etc
        
**/
VOID
UpdateATRDataInfo(
    DEV_INFO            *FpDevInfo,
    ICC_DEVICE          *FpICCDevice
)
{
    UINT8    Data;
    UINT8    Index =1;

    //
    // T0 is mandatory
    //
    FpICCDevice->AtrData.T0 = FpICCDevice->RawATRData[Index];
    FpICCDevice->AtrData.NumberofHystoricalBytes = FpICCDevice->RawATRData[Index] & 0xF;
    Index++;

    //
    // Update TA1
    //
    if (FpICCDevice->AtrData.T0 & 0x10) {
        FpICCDevice->AtrData.TA1 = FpICCDevice->RawATRData[Index];
        FpICCDevice->AtrData.TA1Present = TRUE;
        Index++;
    } else {
        //
        // Default value if TA1 is not present
        //
        FpICCDevice->AtrData.TA1 = 0x11;
    }

    Data = FpICCDevice->AtrData.TA1;
    FpICCDevice->GlobalFi = FiFmaxDi[(Data >> 4) * 3];
    FpICCDevice->GlobalFmax = (UINT8)FiFmaxDi[(Data >> 4) * 3 + 1];
    FpICCDevice->GlobalDi = (UINT8)FiFmaxDi[(Data& 0xF) * 3 + 2];


    //
    // Update TB1
    //
    if (FpICCDevice->AtrData.T0 & 0x20) {
        FpICCDevice->AtrData.TB1 = FpICCDevice->RawATRData[Index];
        FpICCDevice->AtrData.TB1Present = TRUE;
        Index++;
    }

    //
    // Update TC1
    //
    if (FpICCDevice->AtrData.T0 & 0x40) {
        FpICCDevice->AtrData.TC1 = FpICCDevice->RawATRData[Index];
        FpICCDevice->AtrData.TC1Present = TRUE;
        Index++;
    }

    //
    // Update TD1
    //
    if (FpICCDevice->AtrData.T0 & 0x80) {
        FpICCDevice->AtrData.TD1 = FpICCDevice->RawATRData[Index];
        FpICCDevice->AtrData.TD1Present = TRUE;
        Index++;
    }

    if (FpICCDevice->AtrData.TD1) {

        //
        // Update TA2
        //
        if (FpICCDevice->AtrData.TD1 & 0x10) {
            FpICCDevice->AtrData.TA2 = FpICCDevice->RawATRData[Index];
            FpICCDevice->AtrData.TA2Present = TRUE;
            FpICCDevice->SpecificMode = (FpICCDevice->AtrData.TA2 & BIT7) ? TRUE : FALSE;
            Index++;
        }

        //
        // Update TB2
        //
        if (FpICCDevice->AtrData.TD1 & 0x20) {
            FpICCDevice->AtrData.TB2 = FpICCDevice->RawATRData[Index];
            FpICCDevice->AtrData.TB2Present = TRUE;
            Index++;
        }

        //
        // Update TC2
        //
        if (FpICCDevice->AtrData.TD1 & 0x40) {
            FpICCDevice->AtrData.TC2 = FpICCDevice->RawATRData[Index];
            FpICCDevice->AtrData.TC2Present = TRUE;
            Index++;
        }

        //
        // Update TD2
        //
        if (FpICCDevice->AtrData.TD1 & 0x80) {
            FpICCDevice->AtrData.TD2 = FpICCDevice->RawATRData[Index];
            FpICCDevice->AtrData.TD2Present = TRUE;
            Index++;
        }
    }

    //
    // Check if T15 is present else only CLASS A is supported. 
    // By default CLASS A is supported
    //
    FpICCDevice->ClassABC = 1;            

    for (Data = 1;  Data < MAX_ATR_LENGTH ;){

        //
        // Is it T15?
        //
        if ((FpICCDevice->RawATRData[Data] & 0xF) == 0xF){
            if ((Data + 1) < MAX_ATR_LENGTH) { 
                FpICCDevice->ClassABC = FpICCDevice->RawATRData[Data + 1] & 0x3F;
                FpICCDevice->StopClockSupport = FpICCDevice->RawATRData[Data + 1] >> 5;
                FpICCDevice->AtrData.TD15 = FpICCDevice->RawATRData[Data];
                FpICCDevice->AtrData.TD15Present = TRUE;
                FpICCDevice->AtrData.TA15 = FpICCDevice->RawATRData[Data + 1];
                FpICCDevice->AtrData.TA15Present = TRUE;
                break;
            }
        } else {
            // We need info on how many Transmission Protocols are supported by the 
            // card and what are those. Use these loop to do that.
            if (Data > 1) {    // Skip T0
                Index = FpICCDevice->TransmissionProtocolSupported;
                FpICCDevice->TransmissionProtocolSupported |=  ( 1 << (FpICCDevice->RawATRData[Data] & 0x0F));
                if (Index != FpICCDevice->TransmissionProtocolSupported) { 
                    FpICCDevice->NumofTransmissionProtocolSupported++;
                }
            }

            // No more valid TDx?
            if (!(FpICCDevice->RawATRData[Data] & 0x80)) break;
            Data += FindNumberOfTs(FpICCDevice->RawATRData[Data]);
        }
    }

    return;
}

/**
    Find the First offerred Transmission protocol.
    Section 8.2.3 ISO 7816-3 2006-11-01: TD1 encodes first offered protocol. 
    If TD1 not present assume T0.

    @param FpICCDevice    Pointer to the Icc device structure

    @retval TRANSMISSION_PROTOCOL        

**/
TRANSMISSION_PROTOCOL GetDefaultProtocol (
    ICC_DEVICE        *FpICCDevice
)
{

    if (FpICCDevice->AtrData.TD1Present) {
        return FpICCDevice->AtrData.TD1 & 0xf;
    }

    return T0_PROTOCOL;

}

/**
    CCID which doesn't perform "Automatic parameter config. based on ATR

    @param FpDevInfo      Pointer to the DevInfo structure
    @param FpICCDevice    Pointer to the Icc device structure

    @retval TA1           Best TA1 value

    @note  
  1. Calculate the Baud rate using TA1 value

  2. If in CCID bNumDataRatesSupported = 0 then any value between dwDatRate 
     and dwMaxDataRate is supported

  3. Check if ICC baud rate is less tha dwMaxDataRate. If yes use that.

  4. If  bNumDataRatesSupported is not zero get all possible values and try to 
     match it and use that value.

**/
UINT8 
FindBestTA1Value (
    DEV_INFO        *FpDevInfo,
    ICC_DEVICE      *FpICCDevice
)
{

    UINT32              ICCBaudrate;
    UINT8               Di = FpICCDevice->GlobalDi;
    SMARTCLASS_DESC    *CCIDDescriptor;
    UINT8              *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    EFI_STATUS Status = EFI_SUCCESS;
    CCID_DEV_INFO   *CcidDevData;


    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return 0;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return 0;
    }


    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return 0;
    }
    
    CCIDDescriptor = (SMARTCLASS_DESC*)CcidDevData->CcidDescriptor;
    //
    // If Automatic parameter conf. based on ATR data is 
    //
    if (CCIDDescriptor->dwFeatures & AUTO_PARAMETER_CONFIG) {
        return FpICCDevice->AtrData.TA1;
    }

    ICCBaudrate =   (FpICCDevice->GlobalFmax * 1000 * FpICCDevice->GlobalDi)/FpICCDevice->GlobalFi;      

    if (CcidDevData->DataRates && CcidDevData->ClockFrequencies) {
    } else {
        if (ICCBaudrate <= CCIDDescriptor->dwMaxDataRate) {
            return FpICCDevice->AtrData.TA1;
        } else {
            //
            // Can we decrement the Di value and try to match it
            //
            for ( ; Di ; --Di){
                ICCBaudrate =   (FpICCDevice->GlobalFmax * 1000 * Di)/FpICCDevice->GlobalFi;                
                if (ICCBaudrate <= CCIDDescriptor->dwMaxDataRate) {
                    return ((FpICCDevice->AtrData.TA1 & 0xF0) | Di);
                }
            }
        }
    }

    //
    // Worst case return the default value. 
    // Actuall we should fail saying this CCID/ICC combination isn't supported.
    //
    return FpICCDevice->AtrData.TA1;        

}


/**
    Based on the agreed upon TA1 value and Transmission protocol 
    calculate the timing values

    @param FpDevInfo      Pointer to the DevInfo structure
    @param FpICCDevice    Pointer to the Icc device structure
      
    @retval None

**/
VOID
CalculateTimingValues (
    DEV_INFO          *FpDevInfo,
    ICC_DEVICE        *FpICCDevice
)
{

    UINT8   NValue;
    UINT8   Data;
    UINT8    TDCount = 0;

    FpICCDevice->bmFindIndex = FpICCDevice->AtrData.TA1;

    //
    // NValue defaults to zero if TC1 not present
    //
    NValue = FpICCDevice->AtrData.TC1Present == TRUE ? FpICCDevice->AtrData.TC1 : 0;

    //
    // Calculate 1 etu in micro sec
    //
    FpICCDevice->etu = FpICCDevice->GlobalFi / (FpICCDevice->GlobalDi * FpICCDevice->GlobalFmax); 

    //
    // Extra Gaurd Time GT in etu units (section 8.3)
    //
    if (FpICCDevice->AtrData.TA15Present) {
        FpICCDevice->ExtraGuardTime = (UINT8)(12 +  (NValue / FpICCDevice->GlobalFmax  * FpICCDevice->GlobalFi/ FpICCDevice->GlobalDi));
    } else {
        FpICCDevice->ExtraGuardTime = 12 + (NValue / FpICCDevice->GlobalFmax) ;          
    }

    // Update Wait Time  (see section 10.2)
    // WT = WI * 960 * Fi /f where WI is TC2
    // Default if TC2 is not present
    Data = 10;         

    if (FpICCDevice->AtrData.TC2Present) {
        Data = FpICCDevice->AtrData.TC2;
    }    

    //
    // Calculate WT (wait time between two characters) in ETU units
    //
    FpICCDevice->WTwaittime = 960 * FpICCDevice->GlobalFi/(FpICCDevice->GlobalFmax);


    // update Block Width time and Epilogue bit
    // BWT = 11etu + 2 ^ BWI * 960 * Fd /f  (Section 11.4.3)
    // Default BWI is 4. Bit 7:4 in first TB for T1 encodes BWI
    // Fd = 372 (sec section 8.1)

    // Default values (11.4.3)
    FpICCDevice->BWI  = 4;
    FpICCDevice->CWI =  13;
    FpICCDevice->IFSC = 32;
    FpICCDevice->IFSD = 32;
    FpICCDevice->NAD = 0;

    for (Data = 1;  Data < MAX_ATR_LENGTH; ){

        // Look for the First TD for T= 1. It should from TD2
        if (TDCount < 2) {
            if (FpICCDevice->RawATRData[Data] & 0x80) {
                TDCount++;
                Data += FindNumberOfTs(FpICCDevice->RawATRData[Data]);
                continue;
            } else {
                break;
            }
        }

        // Is it T1?
        if ((FpICCDevice->RawATRData[Data] & 0xF) == 0x1){

            if (FpICCDevice->RawATRData[Data] & 0x10) {
                if ((Data + 1) < MAX_ATR_LENGTH) { 
                    FpICCDevice->IFSC = FpICCDevice->RawATRData[Data + 1];
                }
            }

            if (FpICCDevice->RawATRData[Data] & 0x20) {
                if ((Data + 2) < MAX_ATR_LENGTH) { 
                    FpICCDevice->BWI = (FpICCDevice->RawATRData[Data + 2] & 0xF0) >> 4;
                    FpICCDevice->CWI = FpICCDevice->RawATRData[Data + 2] & 0xF;
                }
                
            }

            // Section 11.4.4
            if (FpICCDevice->RawATRData[Data] & 0x40) {
                if ((Data + 3) < MAX_ATR_LENGTH) { 
                    FpICCDevice->EpilogueFields = (FpICCDevice->RawATRData[Data + 3] & 0x1);
                }
            }

            break;
        }

        //
        // No more valid TDx?
        //
        if (!(FpICCDevice->RawATRData[Data] & 0x80)) break;

        Data += FindNumberOfTs(FpICCDevice->RawATRData[Data]);

    }    

    //
    // Block Widthtime in ETU units
    //
    FpICCDevice->BWT = (UINT8)((1 << (FpICCDevice->BWI - 1)) * 960 * 372 /(FpICCDevice->GlobalFmax)) + 11;

    PrintTimingInfo(FpICCDevice);

    return;
}

/**
    Issue PPS cmd to select T0/T1
    
    @param FpDevInfo      Pointer to the DevInfo structure
    @param FpICCDevice    Pointer to the Icc device structure
    @param Data           Points to the buffer which is sent to CCID. 
    @param DataLength     Data length

    @retval EFI_SUCCESS       Success
    @retval Others            Error

    @note  
              This command is issued to CCID which doesn't support AUTO_PARAMETER_CONFIG 
              or when default values or not acceptable

**/
EFI_STATUS
IssuePPSCmd(
    DEV_INFO            *FpDevInfo,
    ICC_DEVICE          *FpICCDevice,
    UINT8               *Data,
    UINT8               DataLength
)
{

    EFI_STATUS    Status = EFI_SUCCESS;
    UINT8        *ResponseBuffer;
    UINT32        ResponseLength = DataLength;
    SMARTCLASS_DESC *CCIDDescriptor;
    UINT8        *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    CCID_DEV_INFO   *CcidDevData;


    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return EFI_DEVICE_ERROR;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return EFI_DEVICE_ERROR;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return EFI_DEVICE_ERROR;
    }

    CCIDDescriptor = (SMARTCLASS_DESC*)CcidDevData->CcidDescriptor;

    //
    // Allocate memory for receiving data
    //
    ResponseBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(DataLength));
    ASSERT(ResponseBuffer);
    if (!ResponseBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem((UINT8 *)ResponseBuffer, DataLength);


    //
    //Check what level of Transmission Protocol is supported
    //
    ResponseLength = 0;
    if (!(CCIDDescriptor->dwFeatures & 0x70000)) {
        ResponseLength = 2;                                 // For Character exchange only 2 bytes expected.
    }  


    PCToRDRXfrBlock(FpDevInfo, FpICCDevice, DataLength, Data, 0, (UINT16)ResponseLength);
    if (CCIDDescriptor->dwFeatures & 0x70000) {
        ResponseLength = 4;                                 // For TDPU expected data is 4
    }
    Status = RDRToPCDataBlock(FpDevInfo, FpICCDevice, &ResponseLength, ResponseBuffer);

    // If length is not same and only Character level Transmission is supported, 
    // issue another XfrBlock cmd to get the rest of the data
    if ((ResponseLength != DataLength) && !(CCIDDescriptor->dwFeatures & 0x70000)) {

        DataLength = (UINT8)ResponseLength;
        ResponseLength = 2;
        PCToRDRXfrBlock(FpDevInfo, FpICCDevice, 0, Data, 0, (UINT16)ResponseLength);
        Status = RDRToPCDataBlock(FpDevInfo, FpICCDevice, &ResponseLength, ResponseBuffer + DataLength);
    
    }

    //
    // I/P and O/P should be identical for success
    //
    USB_MemFree(ResponseBuffer, (UINT8)GET_MEM_BLK_COUNT(DataLength));

    return Status;
}


/**
    Based on the dwFeatures register setting, power up CCID/ICC

    @param FpDevInfo      Pointer to the DevInfo structure
    @param FpICCDevice    Pointer to the Icc device structure

    @retval EFI_SUCCESS       Success
    @retval Others            Error 

    @note  Based on dwFeatures value from SMART Class Descriptor either 
              do an automatic Power-on or go through a manual
              power up sequence and then callect the ATR data.
    
    
**/
EFI_STATUS
VoltageSelection(
    DEV_INFO          *FpDevInfo,
    ICC_DEVICE        *FpICCDevice
)
{

    EFI_STATUS   Status = EFI_DEVICE_ERROR;
    EFI_STATUS   ATRStatus = EFI_DEVICE_ERROR;
    SMARTCLASS_DESC *CCIDDescriptor;
    //
    // Get all voltage level supported by CCID
    //
    UINT8        VoltageLevelCCID;        
    //
    // Select the lowest voltage
    //
    UINT8        VoltageMask = VOLT_18;                                                
    //
    // Successful poweron will result in ATR data
    //
    UINT32       BufferLength = MAX_ATR_LENGTH;                                        
    UINT8        *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    CCID_DEV_INFO   *CcidDevData;

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return EFI_DEVICE_ERROR;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return EFI_DEVICE_ERROR;
    }


    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return EFI_DEVICE_ERROR;
    }
    
    CCIDDescriptor = (SMARTCLASS_DESC*)CcidDevData->CcidDescriptor;
    VoltageLevelCCID = CCIDDescriptor->bVoltageSupport; 
    //
    // Make sure the first selection is valid
    //
    do {

        if (VoltageLevelCCID & VoltageMask) {
            break;
        }

        VoltageMask = VoltageMask >> 1;

    }while (VoltageMask);

    //
    // If Automatic Voltage selection is supported go for it. 
    // Discard the initialization done above
    if (CCIDDescriptor->dwFeatures & AUTO_ACTIVATION_VOLT_SELECTION){
        //
        // Automatic Voltage selection is supported            
        //
        VoltageLevelCCID = AUTO_VOLT;
        VoltageMask = 0;
    }

    do {

        //
        // Issue the cmd to Power it up
        //
        Status = PCtoRDRIccPowerOn (FpDevInfo, 
                                    FpICCDevice, 
                                    ((VoltageLevelCCID & VoltageMask) == 4) ?  3 : VoltageMask);

        if(EFI_ERROR(Status)) { 
            break;
        }

        //
        // Get the response to IccPoweron
        //
        BufferLength = MAX_ATR_LENGTH;
        Status = RDRToPCDataBlock ( FpDevInfo, 
                                    FpICCDevice, 
                                    &BufferLength, 
                                    FpICCDevice->RawATRData
                                    );

        //
        // if successfully powered up, ATR data should be available
        //
        if (!EFI_ERROR(Status) && BufferLength) {

            FpICCDevice->ConfiguredStatus = (ICCPRESENT | VOLTAGEAPPLIED | ATRDATAPRESENT);

            PrintATRData(FpICCDevice->RawATRData);
        
            // From the ATR data, get the required information
            UpdateATRDataInfo(FpDevInfo, FpICCDevice);

            // ATR data got successfully and configured successfully. 
            ATRStatus = EFI_SUCCESS;
            break;

        }

        //
        // if Card not present    
        //
        if ((FpICCDevice->bStatus & 7) == 2) {
            Status = EFI_NOT_FOUND;
            break;
        }

        //
        // ICC is present but some error
        //
        FpICCDevice->ConfiguredStatus = ICCPRESENT;

        //
        // Card present but voltage selection is not OK. Power it off and select next voltage
        //
        Status =  PCtoRDRIccPowerOff (FpDevInfo,  FpICCDevice);
        if (EFI_ERROR(Status)) break;

        Status = RDRToPCSlotStatus(FpDevInfo, FpICCDevice);
        if (EFI_ERROR(Status)) break;

        VoltageMask = VoltageMask >> 1;

        //
        // 10 msec delay before applying the next power class Spec 6.2.3
        //
        FixedDelay (10 * 1000);

    } while (VoltageMask);

    // Return the status of the ATR data read and configuration
    return ATRStatus;

}

/**
    Based on the ATR data and the dwFeature register contend 
    do the Rate and Protocol programming


    @param FpDevInfo      Pointer to the DevInfo structure
    @param FpICCDevice    Pointer to the Icc device structure
    @param Data           Points to the buffer which is sent to CCID. 
    @param DataLength     Data length

    @retval EFI_SUCCESS       Success
    @retval Others            Error

    @note  Based on data received from Power-on sequence (ATR data) and dwFetaures value, 
              Speed of communicatin is established.

**/
EFI_STATUS
RateAndProtocolManagement(
    DEV_INFO          *FpDevInfo,
    ICC_DEVICE        *FpICCDevice
)
{

    EFI_STATUS              Status = EFI_SUCCESS;
    PROTOCOL_DATA_T1        Data = {0};
    UINT8                   PPSData[] = {0xFF, 0x10, 0x11, 0x00};
    UINT8                   Counter;
    SMARTCLASS_DESC         *CCIDDescriptor;
    UINT32                  ClockFrequency;
    UINT32                  DataRate;
    TRANSMISSION_PROTOCOL   FirstOfferredProtocol;
    UINT8                   SetIFS[] = {0xFC};
    UINT32                  ResponseLength;
    UINT8                   ResponseBuffer[20];
    UINT32                  ExchangeLevel;
    BOOLEAN                 ForceSetParams = FALSE;
    UINT8                   *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO           *CcidDevData;

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return EFI_DEVICE_ERROR;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return EFI_DEVICE_ERROR;
    }


    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return EFI_DEVICE_ERROR;
    }
    
    CCIDDescriptor = (SMARTCLASS_DESC*)CcidDevData->CcidDescriptor;
    ClockFrequency = CCIDDescriptor->dwMaximumClock;
    DataRate = CCIDDescriptor->dwMaxDataRate;
    ExchangeLevel = (CCIDDescriptor->dwFeatures & 0x70000);
    
    FirstOfferredProtocol = GetDefaultProtocol(FpICCDevice);

    FpICCDevice->bProtocolNum = (UINT8)FirstOfferredProtocol;

    //
    // Check whether TA1 value is good enough for the reader. If not get the right value
    //
    FpICCDevice->AtrData.TA1 = FindBestTA1Value(FpDevInfo, FpICCDevice);


    //
    // Check if more than one transmission protocol is supported. 
    // If yes then there may be a need for PPSCmd (ISO 7816-3:2006(E) Sec: 6.3.1)
    // Check if Automatic PPS negotiation done by CCID or not. If not issue one.
    // If TA2 is present Card is in Specific mode. So no need for PPS (7816-3:2006 see sec 6.3 fig 4)
    //

    // When PPS exchange must be made? (Page 19 CCID Rev 1.1) 
    // 1. If both AUTO_PPS_NEGOTIATION_CCID AND AUTO_PPS_NEGOTIATION_ACTIVE are not set PPS must be given in case of TDPU or Character
    //                          OR
    // 2. if AUTO_PPS_NEGOTIATION_ACTIVE is present AND TA2 not present AND the preferred protocol isn't USE_T0_T1_PROTOCOL

    if (((CCIDDescriptor->dwFeatures & (AUTO_PPS_NEGOTIATION_CCID | AUTO_PPS_NEGOTIATION_ACTIVE)) == 0 &&
        (ExchangeLevel <= 0x10000 ) && !FpICCDevice->AtrData.TA2Present) ||
        ((CCIDDescriptor->dwFeatures & AUTO_PPS_NEGOTIATION_ACTIVE) && !FpICCDevice->AtrData.TA2Present && 
         FpICCDevice->NumofTransmissionProtocolSupported > 1 && FirstOfferredProtocol != gUsbData->UseT0T1Protocol)) {

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "IssuePPSCmd ...");

        //
        // Update PPS data if in case PPSCmd needs to be issued
        //
        PPSData[1] |= FirstOfferredProtocol;

        //
        // Update PPS2
        //
        PPSData[2] = FpICCDevice->AtrData.TA1;
    
        //
        // Update checksum
        //
        for (Counter = 0; Counter < sizeof (PPSData) - 1; Counter++) {
            PPSData[sizeof (PPSData) - 1] ^= PPSData[Counter];
        }
    
        Status = IssuePPSCmd(FpDevInfo, FpICCDevice, PPSData, sizeof (PPSData));

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%r\n", Status);

        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "CCIDDescriptor.... dwFeatures: %x \n",CCIDDescriptor->dwFeatures);

    if (CCIDDescriptor->dwFeatures & AUTO_PARAMETER_CONFIG) {   

        //
        // Issue GetParameters to get the Transmission Protocol and other parameters
        //
        Status = PCToRDRGetParameters(FpDevInfo, FpICCDevice);
        if (EFI_ERROR(Status)) return Status;
    
        Status = RDRToPCParameters(FpDevInfo, FpICCDevice);
        if (EFI_ERROR(Status)) return Status;        

        //if returned parameters is all zero, use the default values.
        // Workaround for Broadcom CCID. GetParametrs always return zero.
        if (!FpICCDevice->bProtocolNum && !FpICCDevice->bmFindIndex && !FpICCDevice->bmTCCKST && !FpICCDevice->bGuardTime \
            && !FpICCDevice->bWaitingInteger && !FpICCDevice->bIFSC && !FpICCDevice->bClockStop && !FpICCDevice->nNadValue){
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Get Parameters is all zero..\n");
            CalculateTimingValues (FpDevInfo, FpICCDevice);
            ForceSetParams = TRUE;
            FpICCDevice->bProtocolNum = FirstOfferredProtocol;
        }
        else {
            FpICCDevice->ExtraGuardTime = FpICCDevice->bGuardTime;
            FpICCDevice->WTwaittime = FpICCDevice->bWaitingInteger;
            FpICCDevice->IFSC =  FpICCDevice->bIFSC;
            FpICCDevice->NAD = FpICCDevice->nNadValue;
        }

    } else {

        //
        // Now that the TA1 value and the protocol has been finalized, 
        // It is time to calculate the different timing parameters.
        //
        CalculateTimingValues (FpDevInfo, FpICCDevice);
    }

    // Issue SET Params if below two conditions are satisfied
    // Condition 1: If CCID get parameters are all Zero (i.e. Workaround for Broadcom devices) OR
    // Condition 2: Not in specific mode (TA2 is zero) AND
    //              BIT1 not set OR (BIT1 Set but Getparams return non prefered protocol AND
    //              if BIT7 is set OR if BIT 6 and 7 are both not set
    if (
        ForceSetParams == TRUE || \
        (!FpICCDevice->AtrData.TA2Present && \
            (!(CCIDDescriptor->dwFeatures & AUTO_PARAMETER_CONFIG) ||
                // BIT1 set and Protocol we received in Getparams doesn't match the first offered protocol
                ((CCIDDescriptor->dwFeatures & AUTO_PARAMETER_CONFIG) && FpICCDevice->bProtocolNum != FirstOfferredProtocol)) && \
            // If BIT7 is set and not in Specific mode (TA2 non-zero)
            ((CCIDDescriptor->dwFeatures & AUTO_PPS_NEGOTIATION_ACTIVE ) || \
            // if BIT 6 & 7 are both zero, also not in Specific mode
            (!(CCIDDescriptor->dwFeatures & (AUTO_PPS_NEGOTIATION_ACTIVE | AUTO_PPS_NEGOTIATION_CCID) ) ) ) \
        )){ 

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Set Parameters required..%x\n", FpICCDevice->bProtocolNum);

        //
        // Use the superset of the T0/T1 structure (ie T1 structure) even if it is T0. It should work.
        //
        Data.bmFindDindex = FpICCDevice->bmFindIndex;
        Data.bmTCCKST1 = FpICCDevice->bProtocolNum == 0  ? 0 : (FpICCDevice->EpilogueFields | 0x10);
        Data.bGuardTimeT1 = FpICCDevice->ExtraGuardTime;

        Data.bWaitingIntergersT1 = FpICCDevice->bProtocolNum == 0  ? 
                                (UINT8)FpICCDevice->WTwaittime : (FpICCDevice->BWI << 4 | FpICCDevice->CWI);

        Data.bClockStop = FpICCDevice->bClockStop;
        Data.bIFSC = FpICCDevice->IFSC;
        Data.bNadValue = FpICCDevice->NAD;
            
        Status = PCToRDRSetParameters(FpDevInfo, FpICCDevice, FpICCDevice->bProtocolNum, (VOID *)&Data);

        if (!EFI_ERROR(Status)){
            Status = RDRToPCParameters(FpDevInfo, FpICCDevice);
        } else {
            return Status; 
        }

        // Work around for Broadcom CCID
        if (FpICCDevice->bProtocolNum != FirstOfferredProtocol) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bProtocolNum %x doesn't match with  FirstOfferredProtocol%x\n", FpICCDevice->bProtocolNum, FirstOfferredProtocol);
            FpICCDevice->bProtocolNum =  FirstOfferredProtocol;
        }
    }

    //
    // Based on T0 or T1 update Waittime. For T0 use WTWaittime, for T1 use BWT. 
    //
    if (FpICCDevice->bProtocolNum) {
        FpICCDevice->WaitTime = FpICCDevice->BWT;
    } else {
        FpICCDevice->WaitTime = FpICCDevice->WTwaittime;            
    }

    //
    // If Automatic ICC Clock Freq AND Automatic Buad Rate selection 
    // isn't supported issue SetDataRateAndClock cmd
    //
    if (!(CCIDDescriptor->dwFeatures & (AUTO_BAUD_RATE_SELECTION |AUTO_ICC_CLOCK_FREQ))){

    }

    //
    // Check if IFSC/IFSD needs to be increased. Default value is 0x20. T1 and TDPU/Char needs this cmd.
    //
    if (FpICCDevice->bProtocolNum){    
        switch(CCIDDescriptor->dwFeatures & 0x70000) { 
            case CHARACTER_LEVEL_EXCHANGE:
                // Both SUZCR90 and O2Micro oz77c6l1 didn't respond to SBlock call below without this delay
                FixedDelay(10 * 1000);      // 10msec delay. No break. Let the flow continue below.
            case TDPU_LEVEL_EXCHANGE:
              ResponseLength = 1;
                SetIFS[0] = (UINT8)CCIDDescriptor->dwMaxIFSD;

                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "SetIFS[0] %x\n", SetIFS[0]);

                Status = TxRxT1TDPUChar (FpDevInfo, FpICCDevice, sizeof (SetIFS), SetIFS, IFS_REQUEST, &ResponseLength, ResponseBuffer);
                // Update the received IFSD
                if (!EFI_ERROR(Status) && ResponseLength == 1){
                    FpICCDevice->IFSD = ResponseBuffer[0];
                }
                break;
            default:
                break;
        }
    } 
    return Status;

}

/**
    This function powers up, sets the clock/rate etc 
    (configure CCID based on device capability)

    @param FpDevInfo      Pointer to the DevInfo structure
    @param FpICCDevice    Pointer to the Icc device structure
    @param Data           Points to the buffer which is sent to CCID. 
    @param DataLength     Data length

    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/
EFI_STATUS
ConfigureCCID(
    DEV_INFO          *FpDevInfo,
    ICC_DEVICE        *FpICCDevice
)
{
    EFI_STATUS  Status;
    UINT8       RetryCount = 3;

    //
    // Power up the device
    //
    do {
        Status = VoltageSelection(FpDevInfo, FpICCDevice);
        RetryCount--;

        //    
        // check for errors and do try to recover
        //
        if(EFI_ERROR(Status) || FpICCDevice->bStatus) {
            //
            // If card present but not powered up retry it. 
            // If card not present the exit immediatly
            //
            if (FpICCDevice->bStatus ==  2) {
                break;
            }            
        } else {
            break;
        }

    }while (RetryCount);

    //    
    //Configure the data Rate and select the Protocol
    //
    if (!EFI_ERROR(Status)){
        Status = RateAndProtocolManagement (FpDevInfo, FpICCDevice);
    }
    
    if (EFI_ERROR(Status)) {
        FpICCDevice->ConfiguredStatus = CONFIGFAILED;
    }

    return Status;
}

/**
    This function executes a bulk transaction on the USB. The
    transfer may be either DATA_IN or DATA_OUT packets containing
    data sent from the host to the device or vice-versa. This
    function wil not return until the request either completes
    successfully or completes with error (due to time out, etc.)
    Size of data can be upto 64K

    @param DeviceInfo    Pointer to the DevInfo structure (if available else 0)
    @param XferDir       Transfer direction
                         Bit 7   : Data direction
                                    0 Host sending data to device
                                    1 Device sending data to host
                         Bit 6-0 : Reserved
    @param CmdBuffer     Buffer containing data to be sent to the device or
                         buffer to be used to receive data. Value in
    @param Size         Length request parameter, number of bytes of data
                         to be transferred in or out of the host controller

    @retval data         Amount of data transferred

**/

UINT32
USBCCIDIssueBulkTransfer (
    DEV_INFO*   FpDevInfo, 
    UINT8       XferDir,
    UINT8*      CmdBuffer, 
    UINT32      Size
)
{


    return AmiUsbBulkTransfer(
               gHcTable[FpDevInfo->HcNumber -1],
               FpDevInfo, XferDir,
               CmdBuffer, Size);

    // Handle Bulk Transfer error here

}

/**
    Issues Control Pipe request to default pipe

    @param FpDevInfo   DeviceInfo structure (if available else 0)
   @param  Request     Request type (low byte)
                       Bit 7   : Data direction
                                 0 = Host sending data to device
                                 1 = Device sending data to host
                       Bit 6-5 : Type
                                 00 = Standard USB request
                                 01 = Class specific
                                 10 = Vendor specific
                                 11 = Reserved
                       Bit 4-0 : Recipient
                                 00000 = Device
                                 00001 = Interface
                                 00010 = Endpoint
                                 00100 - 11111 = Reserved
                      Request code, a one byte code describing
                     the actual device request to be executed
                     (ex: 1 : ABORT, 2 : GET_CLOCK_FREQUENCIES, 3: GET_DATA_RATES)
   @param Index      wIndex request parameter (meaning varies)
   @param Value      wValue request parameter (meaning varies)
   @param FpBuffer    Buffer containing data to be sent to the
                      device or buffer to be used to receive data
   @param Length     wLength request parameter, number of bytes
                      of data to be transferred in or out
                      of the host controller

   @retval Data       Number of bytes actually transferred

**/

UINT32
USBCCIDIssueControlTransfer(
    DEV_INFO*   FpDevInfo,     
    UINT16      Request,
    UINT16      Index,
    UINT16      Value,
    UINT8       *FpBuffer,
    UINT16      Length
)
{
   return AmiUsbControlTransfer(
              gHcTable[FpDevInfo->HcNumber - 1],
              FpDevInfo,
              Request,
              Index,
              Value,
              FpBuffer,
              Length);

}

/**
    Returns the # of Ts present in TDx

    @param Data      
    @retval Count  Returns number of TDx present in ATR data

**/
UINT8
FindNumberOfTs(
    UINT8    Data
)
{
    UINT8    Count = 0;
    UINT8    Mask = 0x10;

    for ( ;Mask; Mask = Mask << 1){
        if (Data & Mask) { 
            Count++;
        }
    }

    return Count;    
}

/**
    This function prints the information gathered from GetPCParameters

    @param Data       
    @retval VOID

**/
VOID 
PrintPCParameters(
    UINT8 * Data
)
{

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bProtocolNum     : %02X\n", Data[0]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bmFindexDIndex   : %02X\n", Data[1]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bmTCCKST0        : %02X\n", Data[2]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bGaurdTime       : %02X\n", Data[3]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bWaitingInterger : %02X\n", Data[4]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bClockStop       : %02X\n", Data[5]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bIFSC            : %02X\n", Data[6]); // Valid only for T1    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bNadValue        : %02X\n", Data[7]); // Valid only for T1

    return;
} 


/**
    This function prints the information gathered from ATR data

    @param FpDevInfo      Pointer to the DevInfo structure       
    @retval None

**/
VOID
PrintTimingInfo(
    ICC_DEVICE    *FpICCDevice
)
{

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "etu              : %02X  \n", FpICCDevice->etu); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "GlobalFi         : %04x  \n", FpICCDevice->GlobalFi); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "GlobalFmax       : %02X  \n", FpICCDevice->GlobalFmax); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "GlobalDi         : %02X  \n", FpICCDevice->GlobalDi); 

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "SpecificMode     : %02X  \n", FpICCDevice->SpecificMode); 

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "ClassABC         : %02X  \n", FpICCDevice->ClassABC); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "StopClockSupport : %02X  \n", FpICCDevice->StopClockSupport); 

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "ExtraGuardTime   : %02X  \n", FpICCDevice->ExtraGuardTime); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "WTwaittime       : %08x  \n", FpICCDevice->WTwaittime); 

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "BWI              : %02X  \n", FpICCDevice->BWI); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "CWI              : %02X  \n", FpICCDevice->CWI); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "IFSC             : %02X  \n", FpICCDevice->IFSC); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "NAD              : %02X  \n", FpICCDevice->NAD); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "EpilogueFields   : %02X  \n", FpICCDevice->EpilogueFields); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "BWT              : %02X  \n", FpICCDevice->BWT); 

    return;
}

/**
    This function Prints the RAW ATR Data

    @param ATRData   the RAW ATR Data  
    @retval None

**/
VOID
PrintATRData(
    UINT8            *ATRData
)
{

    UINT8        TDx = 2;
    UINT8        Index;


    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "    ATR Data \n");

    for (Index=0; Index < 32; Index++) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%02X ", ATRData[Index]);
    }

    Index = 0;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\nTS  : %02X  \n", ATRData[Index++]); 

    TDx = ATRData[Index];
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "T0  : %02X  \n", ATRData[Index++]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TA1 : %02X  \n", (TDx & 0x10) ? ATRData[Index++] : 0); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TB1 : %02X  \n", (TDx & 0x20) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TC1 : %02X  \n", (TDx & 0x40) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TD1 : %02X  \n", (TDx & 0x80) ? ATRData[Index++] : 0);

    if (!(TDx & 0x80)) return;
    TDx = ATRData[Index-1];

 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TA2 : %02X \n", (TDx & 0x10) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TB2 : %02X \n", (TDx & 0x20) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TC2 : %02X \n", (TDx & 0x40) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TD2 : %02X \n", (TDx & 0x80) ? ATRData[Index++] : 0);

    if (!(TDx & 0x80)) return;
    TDx = ATRData[Index-1];

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TA3 : %02X \n", (TDx & 0x10) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TB3 : %02X \n", (TDx & 0x20) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TC3 : %02X \n", (TDx & 0x40) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TD3 : %02X \n", (TDx & 0x80) ? ATRData[Index++] : 0);

    if (!(TDx & 0x80)) return;
    TDx = ATRData[Index-1];

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TA4 : %02X \n", (TDx & 0x10) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TB4 : %02X \n", (TDx & 0x20) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TC4 : %02X \n", (TDx & 0x40) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TD4 : %02X \n", (TDx & 0x80) ? ATRData[Index++] : 0);

    if (!(TDx & 0x80)) return;
    TDx = ATRData[Index-1];

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TA5 : %02X \n", (TDx & 0x10) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TB5 : %02X \n", (TDx & 0x20) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TC5 : %02X \n", (TDx & 0x40) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TD5 : %02X \n", (TDx & 0x80) ? ATRData[Index++] : 0);

    if (!(TDx & 0x80)) return;
    TDx = ATRData[Index-1];

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TA6 : %02X \n", (TDx & 0x10) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TB6 : %02X \n", (TDx & 0x20) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TC6 : %02X \n", (TDx & 0x40) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TD6 : %02X \n", (TDx & 0x80) ? ATRData[Index++] : 0);

    if (!(TDx & 0x80)) return;
    TDx = ATRData[Index-1];

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TA7 : %02X \n", (TDx & 0x10) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TB7 : %02X \n", (TDx & 0x20) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TC7 : %02X \n", (TDx & 0x40) ? ATRData[Index++] : 0);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TD7 : %02X \n", (TDx & 0x80) ? ATRData[Index++] : 0);

    return;
}

/**
    Prints SMART class Descriptor data

    @param FpCCIDDesc  SMART class Descriptor data        
    @retval VOID

**/
VOID
PrintDescriptorInformation (
    SMARTCLASS_DESC *FpCCIDDesc
)
{

    CHAR8    *Strings[] = {"CHARACTER", "TDPU", "Short ADPU", "Extended ADPU"};
    UINT8    Exchange;


    Exchange = (UINT8)((FpCCIDDesc->dwFeatures & 0x70000) >> 16);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Sizeof SMART Class Descriptor :  %X\n", sizeof (SMARTCLASS_DESC));
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "DescLength            :  %04X\n", FpCCIDDesc->DescLength);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "DescType              :  %04X\n", FpCCIDDesc->DescType);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bcdCCID                :  %04X\n", FpCCIDDesc->bcdCCID);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bMaxSlotIndex          :  %04X\n", FpCCIDDesc->bMaxSlotIndex);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bVoltageSupport        :  %04X\n", FpCCIDDesc->bVoltageSupport);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwProtocols            :  %04X\n", FpCCIDDesc->dwProtocols);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwDefaultClock         :  %04X\n", FpCCIDDesc->dwDefaultClock);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwMaximumClock         :  %04X\n", FpCCIDDesc->dwMaximumClock);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bNumClockSupported     :  %04X\n", FpCCIDDesc->bNumClockSupported);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwDataRate             :  %04X\n", FpCCIDDesc->dwDataRate);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwMaxDataRate          :  %04X\n", FpCCIDDesc->dwMaxDataRate);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bNumDataRatesSupported :  %04X\n", FpCCIDDesc->bNumDataRatesSupported);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwMaxIFSD              :  %04X\n", FpCCIDDesc->dwMaxIFSD);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwSynchProtocols       :  %04X\n", FpCCIDDesc->dwSynchProtocols);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwMechanical           :  %04X\n", FpCCIDDesc->dwMechanical);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "dwFeatures             :  %04X\n", FpCCIDDesc->dwFeatures);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bClassGetResponse      :  %04X\n", FpCCIDDesc->dwMaxCCIDMessageLength);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bClassGetResponse      :  %04X\n", FpCCIDDesc->bClassGetResponse);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bClassEnvelope         :  %04X\n", FpCCIDDesc->bClassEnvelope);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "wLcdLayout             :  %04X\n", FpCCIDDesc->wLcdLayout);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bPINSupport            :  %04X\n", FpCCIDDesc->bPINSupport);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "bMaxCCIDBusySlots      :  %04X\n", FpCCIDDesc->bMaxCCIDBusySlots);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "*************************************\n"); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " Device is in:"); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%a Exchange mode\n", Strings[Exchange]); 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "*************************************\n"); 

}

/**
    This routine is called when InterruptIN messages is generated
      
    @param pHCStruc      Pointer to HCStruc
    @param DevInfo       Pointer to device information structure
    @param Td            Pointer to the polling TD
    @param Buffer        Pointer to the data buffer
    @param DataLength    Data length
    
    @retval USB_SUCCESS       Success
    @retval UEB_ERROR         Error

    @note  When an ICC card is inserted or removed Interrupt message is generated.   

**/

UINT8
EFIAPI
USBCCID_ProcessInterruptData (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Td,
    UINT8       *Buffer,
    UINT16      DataLength
)
{

    EFI_STATUS      Status;
    UINT8           Data;
    UINT8           Slot = 0;
    UINT8           bmSlotICCByte = 0;
    UINT32          SlotICCStatus = *(UINT32 *)(Buffer + 1);
    SMARTCLASS_DESC *CCIDDescriptor;
    UINT8           *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    CCID_DEV_INFO   *CcidDevData;

    CcidDevData = (CCID_DEV_INFO*)DevInfo->SpecificDevData;
    if (!CcidDevData) return USB_ERROR;


    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return USB_ERROR;
    }


    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return USB_ERROR;
    }

    CCIDDescriptor = (SMARTCLASS_DESC*)CcidDevData->CcidDescriptor;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBCCID_ProcessInterruptData.... %X %X %X %X\n", 
                *Buffer, *(Buffer +1), *(Buffer + 2), *(Buffer + 3));


    switch (*Buffer) {

        //
        // ICC Card either inserted or Removed
        //
        case RDR_TO_PC_NOTIFYSLOTCHANGE:        

            //
            // Find the # of bytes in this notification
            //
            Slot = CCIDDescriptor->bMaxSlotIndex + 1; // Make it 1 based
            bmSlotICCByte = (CCIDDescriptor->bMaxSlotIndex + 1) >> 2;

            if (Slot & 0x3) { 
                bmSlotICCByte++;
            }

            Slot = 0;
            do {
                Data = (SlotICCStatus >> Slot) & 0x3;
                //
                // Is there a change in status
                //
                if ((Data & 0x3) == 3) {                
                    Status = ICCInsertEvent (DevInfo, Slot);
                }
                if ((Data & 0x3) == 2) {                
                    Status = ICCRemovalEvent (DevInfo, Slot);
                }
                  Slot++;
            } while (Slot < (CCIDDescriptor->bMaxSlotIndex + 1));                        

            break;

        case RDR_TO_PC_HARDWAREERROR:        

            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "RDR To PC Hardware Error Slot %X Sequence %X Error Code %X \n", 
                            *Buffer, *(Buffer +1), *(Buffer + 2));
            break;

        default:
            break;
    }

    return USB_SUCCESS;
}

/**
    In response to Device removal, Interrupt-in message is received. 
    Icc Device is removed from the linked list.

    @param FpDevInfo      Pointer to the DevInfo structure
    @param Slot           Slot number
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error    

**/
EFI_STATUS
ICCRemovalEvent(
        DEV_INFO    *FpDevInfo,
        UINT8        Slot
)
{

    ICC_DEVICE        *FpICCDevice;
    
    FpICCDevice = GetICCDevice(FpDevInfo, Slot);
   
    if (FpICCDevice) {

        // Don't free up the memory. EFI driver (EfiUsbCCID) makes use of this data area to 
        // find whether ICC has been removed or added.
        // Before freeing up, clear the bytes

//      MemFill((UINT8 *)FpICCDevice, sizeof(ICC_DEVICE), 0);
        ZeroMem((UINT8 *)(FpICCDevice->RawATRData), sizeof(FpICCDevice->RawATRData));
        ZeroMem((UINT8 *)&(FpICCDevice->AtrData), sizeof(ATR_DATA));

        //
        //Free up the memory and remove it from linked list
        //

        if (FpICCDevice->ConfiguredStatus & ICCPRESENT) {
            FpICCDevice->ConfiguredStatus = CARDREMOVED;
        } else {
            //
            // Handle if IccRemovalEven is called multiple times
            //
            return EFI_SUCCESS;
        }

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "ICC device removed - Slot : %X\n", Slot);

        if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI) {
            ICC_SmiQueuePut((void *)FpICCDevice);
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Removal: FpDevInfo %X FpICCDevice %X\n", FpDevInfo, FpICCDevice);

    return EFI_SUCCESS;
}


/**
    In response to Device Insertion, Interrupt-in message is received. 
    Icc Device is added to the linked list and configured.

    @param FpDevInfo      Pointer to the DevInfo structure
    @param Slot           Slot number
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error   

**/
EFI_STATUS
ICCInsertEvent(
    DEV_INFO    *FpDevInfo,
    UINT8       Slot
)
{

    EFI_STATUS        Status;
    ICC_DEVICE        *FpICCDevice;
    BOOLEAN         NewDeviceAdded = FALSE;
    CCID_DEV_INFO   *CcidDevData;

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return EFI_DEVICE_ERROR;

    Status = AmiUsbValidateMemoryBuffer((VOID*)CcidDevData, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return EFI_DEVICE_ERROR;
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "ICCInsertEvent Slot %X \n", Slot);
    //
    // Check if the device already exist. if so use it.
    //
    FpICCDevice = GetICCDevice(FpDevInfo, Slot);

    if (!FpICCDevice) {
        //
        // Alocate memory for ICC_DEVICE and attach it to the linked list
        //
        FpICCDevice = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(ICC_DEVICE)));
        ASSERT(FpICCDevice);
        if (!FpICCDevice) {
            return EFI_OUT_OF_RESOURCES;
        }    
        ZeroMem((UINT8 *)FpICCDevice, sizeof(ICC_DEVICE));

        //
        // Add to the slot list
        //

        if (CcidDevData->IccDeviceList.ForwardLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.ForwardLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return EFI_DEVICE_ERROR;
            }
        }
        if (CcidDevData->IccDeviceList.BackLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.BackLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return EFI_DEVICE_ERROR;
            }
        }


        InsertTailList(&(CcidDevData->IccDeviceList), &(FpICCDevice->Link));
        NewDeviceAdded = TRUE;

    }


    if ((gUsbData->UsbFeature & USB_CCID_USE_INT_INS_REMOVAL) == USB_CCID_USE_INT_INS_REMOVAL){
    // Handle Multiple ICCInsertEvent calls. Some cards generate 
    // Interrupt in Interrupt-IN endpoint and some don't.
    // For card which don't generate the intterupt, CCID API should be used to power up the device.
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "FpICCDevice->ConfiguredStatus %X \n", FpICCDevice->ConfiguredStatus);
    if ((FpICCDevice->ConfiguredStatus != CARDREMOVED) && (FpICCDevice->ConfiguredStatus)) {

        if (FpICCDevice->ConfiguredStatus == CONFIGFAILED) {
            return EFI_DEVICE_ERROR;
        }
        return EFI_SUCCESS;

    }
    }

    
    FpICCDevice->Signature = ICC_DEVICE_SIG;
    FpICCDevice->Slot = Slot;
    FpICCDevice->WaitTime = INITWAITTIME;
    FpICCDevice->ConfiguredStatus = 0;

    Status = ConfigureCCID(FpDevInfo, FpICCDevice);


    if ((gUsbData->UsbFeature & USB_CCID_USE_INT_INS_REMOVAL) == USB_CCID_USE_INT_INS_REMOVAL){
    if(EFI_ERROR(Status)){

        //
        //Free up the memory and remove it from linked list
        //
        if (CcidDevData->IccDeviceList.ForwardLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.ForwardLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return EFI_DEVICE_ERROR;
            }
        }
        if (CcidDevData->IccDeviceList.BackLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.BackLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return EFI_DEVICE_ERROR;
            }
        }

        //DListDelete (&(CcidDevData->IccDeviceList), &(FpICCDevice->ICCDeviceLink));
        RemoveEntryList(&FpICCDevice->Link);
        USB_MemFree(FpICCDevice, (UINT8)GET_MEM_BLK_COUNT(sizeof(ICC_DEVICE)));

    } else {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "ICC device added - Slot : %X\n", Slot);
    
        if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI && (FpICCDevice->ChildHandle == NULL)) {
            ICC_SmiQueuePut((void *)FpICCDevice);
        }
    }

    } else {
    //
    // Even if configuration failed install the protocol in polling mode.         
    //
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "ICC device added - Slot : %X\n", Slot);
    
    if ((gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI) && NewDeviceAdded && (FpICCDevice->ChildHandle == NULL)) {
        ICC_SmiQueuePut((void *)FpICCDevice);
    }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Insert : FpDevInfo %X FpICCDevice %X\n", FpDevInfo, FpICCDevice);

    return Status;
}

/**
    Puts the pointer  into the queue for processing.
    updates queue head and tail. This data is read from EfiUSBCCID.C 
    which installs AMI_CCID_IO_PROTOCOL

    @param  d         (void *)FpICCDevice        
    @retval None

**/

VOID
ICC_SmiQueuePut(
    VOID * d
)
{
    QUEUE_T* q = &gUsbDataList->ICCQueueCnnctDisc;

    while (q->head >= q->maxsize) {
        q->head -= q->maxsize;
    }

    q->data[q->head++] = d;
    if (q->head == q->maxsize) {
        q->head -= q->maxsize;
    }
    if (q->head == q->tail) {
        //Drop data from queue
        q->tail++;
        while (q->tail >= q->maxsize) {
            q->tail -= q->maxsize;
        }
    }
    return;
}

/**
    Do some USB device info data initialization 

    @param FpDevInfo          Pointer to devInfo structure
    @param FpDesc             Pointer to the descriptor structure
    @param StartOffset        Start offset of the device descriptor
    @param  EndOffset         End offset of the device descriptor

    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/
EFI_STATUS
DoDevInfoInitialization (
    DEV_INFO    *FpDevInfo,
    UINT8       *FpDesc,
    UINT16      StartOffset,
    UINT16      EndOffset
)
{

    UINT8           Temp;
    ENDP_DESC       *FpEndpDesc;
    INTRF_DESC      *FpIntrfDesc;
    SMARTCLASS_DESC *FpCCIDDesc = NULL;
    CCID_DEV_INFO   *CcidDevData;

    FpDevInfo->DeviceType      = BIOS_DEV_TYPE_CCID;
    FpDevInfo->PollTdPtr      = 0;

    FpDevInfo->CallBackIndex = USB_InstallCallBackFunction(USBCCID_ProcessInterruptData);

    FpDevInfo->SpecificDevData = (VOID*)USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(CCID_DEV_INFO)));
    ASSERT(FpDevInfo->SpecificDevData);
    if (!FpDevInfo->SpecificDevData) {
        return EFI_OUT_OF_RESOURCES;
    }

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;

    //
    // Initialize the Initlist to hold data for each Slot 
    //
    //DListInit(&(CcidDevData->IccDeviceList));
    InitializeListHead(&CcidDevData->IccDeviceList); 
    FpIntrfDesc = (INTRF_DESC*)(FpDesc + StartOffset);

    //
    // Calculate the end of descriptor block
    //
    FpDesc+=((CNFG_DESC*)FpDesc)->TotalLength; 
    FpEndpDesc = (ENDP_DESC*)((char*)FpIntrfDesc + FpIntrfDesc->DescLength);

    do {
        if (FpIntrfDesc->DescType == DESC_TYPE_SMART_CARD) {
            FpCCIDDesc = (SMARTCLASS_DESC *)FpIntrfDesc;
            break;
        }
        FpIntrfDesc = (INTRF_DESC*) ((UINT8 *)FpIntrfDesc + FpIntrfDesc->DescLength);
    } while ((UINT8 *)FpIntrfDesc < FpDesc);

    if (!FpCCIDDesc) { 
        return EFI_DEVICE_ERROR;
    }

    CcidDevData->CcidDescriptor = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(SMARTCLASS_DESC)));
    ASSERT(CcidDevData->CcidDescriptor);
    if (!CcidDevData->CcidDescriptor) {
        return EFI_OUT_OF_RESOURCES;
    }   
    CopyMem((UINT8 *)(CcidDevData->CcidDescriptor), (UINT8 *)FpCCIDDesc, sizeof(SMARTCLASS_DESC));
  FpCCIDDesc = (SMARTCLASS_DESC*)CcidDevData->CcidDescriptor;

    if (FpCCIDDesc->bNumDataRatesSupported) {

        CcidDevData->DataRates = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(
                                            FpCCIDDesc->bNumDataRatesSupported * sizeof(UINT32)));
        ASSERT(CcidDevData->DataRates);
        if (!CcidDevData->DataRates) {
            return EFI_OUT_OF_RESOURCES;
        }      
        //
        // Issue GET_DATA_RATES cmd. Should interface number be zero?
        //
        USBCCIDIssueControlTransfer(FpDevInfo, 
                                    CCID_CLASS_SPECIFIC_GET_DATA_RATES, 
                                    0x0, 0, (UINT8 *)CcidDevData->DataRates, 
                                    FpCCIDDesc->bNumDataRatesSupported * sizeof(UINT32)
                                    );
        
    } else {
        CcidDevData->DataRates = 0;
    }

    if (FpCCIDDesc->bNumClockSupported) {

        CcidDevData->ClockFrequencies = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(
                                        FpCCIDDesc->bNumClockSupported * sizeof(UINT32)));
        ASSERT(CcidDevData->ClockFrequencies);
        if (!CcidDevData->ClockFrequencies) {
            return EFI_OUT_OF_RESOURCES;
        }
        //            
        // Issue GET_CLOCK_FREQUENCIES. Should interface number be zero?
        //
        USBCCIDIssueControlTransfer(FpDevInfo,  
                                    CCID_CLASS_SPECIFIC_GET_CLOCK_FREQUENCIES, 
                                    0x0, 0, (UINT8 *)CcidDevData->ClockFrequencies,
                                    FpCCIDDesc->bNumClockSupported * sizeof(UINT32));
    } else {
        CcidDevData->ClockFrequencies = 0;
    }

    PrintDescriptorInformation(CcidDevData->CcidDescriptor);

    Temp = 0x03;       // bit 1 = Bulk In, bit 0 = Bulk Out

    for( ;(FpEndpDesc->DescType != DESC_TYPE_INTERFACE) && ((UINT8*)FpEndpDesc < FpDesc);
        FpEndpDesc = (ENDP_DESC*)((UINT8 *)FpEndpDesc + FpEndpDesc->DescLength)){

    if(!(FpEndpDesc->DescLength)) {  
        // Br if 0 length desc (should never happen, but...)
        break;  
    }

        if( FpEndpDesc->DescType != DESC_TYPE_ENDPOINT ) {
            continue;
        }

        if ((FpEndpDesc->EndpointFlags & EP_DESC_FLAG_TYPE_BITS) ==
                EP_DESC_FLAG_TYPE_BULK) {   // Bit 1-0: 10 = Endpoint does bulk transfers
            if(!(FpEndpDesc->EndpointAddr & EP_DESC_ADDR_DIR_BIT)) {
                //
                // Bit 7: Dir. of the endpoint: 1/0 = In/Out
                // If Bulk-Out endpoint already found then skip subsequent ones
                // on the interface.
                //
                if (Temp & 1) {
                    FpDevInfo->BulkOutEndpoint = (UINT8)(FpEndpDesc->EndpointAddr
                                                        & EP_DESC_ADDR_EP_NUM);
                    FpDevInfo->BulkOutMaxPkt = FpEndpDesc->MaxPacketSize;
                    Temp &= 0xFE;
                }
            } else {
                //
                // If Bulk-In endpoint already found then skip subsequent ones
                // on the interface
                //
                if (Temp & 2) {
                    FpDevInfo->BulkInEndpoint  = (UINT8)(FpEndpDesc->EndpointAddr
                                                        & EP_DESC_ADDR_EP_NUM);
                    FpDevInfo->BulkInMaxPkt    = FpEndpDesc->MaxPacketSize;
                    Temp   &= 0xFD;
                }
            }
        }

        //
        // Check for and configure Interrupt endpoint if present
        //
        if ((FpEndpDesc->EndpointFlags & EP_DESC_FLAG_TYPE_BITS) !=
                EP_DESC_FLAG_TYPE_INT) {    // Bit 1-0: 10 = Endpoint does interrupt transfers
          continue;
        }

    if (FpEndpDesc->EndpointAddr & EP_DESC_ADDR_DIR_BIT) {
      FpDevInfo->IntInEndpoint = FpEndpDesc->EndpointAddr;
      FpDevInfo->IntInMaxPkt = FpEndpDesc->MaxPacketSize;
      FpDevInfo->PollInterval = FpEndpDesc->PollInterval;  
    }
    }

    return EFI_SUCCESS;
}

/**
    This function initializes CCID device related data
  
    @param None              
    @retval None

**/

VOID
EFIAPI
USBCCIDInitialize ()
{
    gUsbData->CcidSequence = 0;
    USB_InstallCallBackFunction(USBCCID_ProcessInterruptData);
    return;
}

/**
    This routine checks for CCID type device from the
    interface data provided

    @param FpDevInfo    Pointer to device information structure
    @param BaseClass    USB base class code
    @param SubClass     USB sub-class code
    @param Protocol     USB protocol code

    @retval BIOS_DEV_TYPE_CCID     USB CCID type 
    @retval Others                 Not USB CCID type 

**/

UINT8
EFIAPI
USBCCIDCheckForDevice (
    DEV_INFO    *FpDevInfo,
    UINT8       BaseClass,
    UINT8       SubClass,
    UINT8       Protocol
)
{

    if(BaseClass == BASE_CLASS_CCID_STORAGE  && Protocol == PROTOCOL_CCID) {
        return BIOS_DEV_TYPE_CCID;
    }

    return USB_ERROR;
}

/**
    This routine initializes each slot of Smartcard reader

    @param FpDevInfo    Pointer to device information structure

    @retval EFI_SUCCESS       Success
    @retval Others            Error
**/
EFI_STATUS
UpdateSCardSlotInfo (
    DEV_INFO    *FpDevInfo
)
{
    UINT8          i=0;
    ICC_DEVICE     *FpICCDevice = NULL;    
    UINT8          *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    EFI_STATUS     Status = EFI_SUCCESS;
    CCID_DEV_INFO   *CcidDevData;

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if(!CcidDevData) return EFI_INVALID_PARAMETER;

    if(!FpDevInfo || !((SMARTCLASS_DESC*)(CcidDevData->CcidDescriptor)) ) {
        return EFI_INVALID_PARAMETER;
    }

    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return EFI_DEVICE_ERROR;
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "((SMARTCLASS_DESC*)(CcidDevData->CcidDescriptor))->bMaxSlotIndex %X \n", ((SMARTCLASS_DESC*)(CcidDevData->CcidDescriptor))->bMaxSlotIndex );

    for( i=0; i<=((SMARTCLASS_DESC*)(CcidDevData->CcidDescriptor))->bMaxSlotIndex ; i++ ) {

        // Code to initialize Each slot of the CCID irrespective of ICC card 
        // is present in the Slot or not
        FpICCDevice = GetICCDevice(FpDevInfo, i);

        if (FpICCDevice) {
            FpICCDevice->ChildHandle = 0;
            FpICCDevice->SCardChildHandle = 0;
            FpICCDevice->WaitTime = INITWAITTIME;
            FpICCDevice->SlotConnectStatus = 0;
            FpICCDevice->ConfiguredStatus = 0;
            continue;
        }

        FpICCDevice = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(sizeof(ICC_DEVICE)));
        ASSERT(FpICCDevice);
        if (!FpICCDevice) {
            return EFI_OUT_OF_RESOURCES;
        }
        ZeroMem((UINT8 *)FpICCDevice, sizeof(ICC_DEVICE));

        FpICCDevice->Signature = ICC_DEVICE_SIG;
        FpICCDevice->ChildHandle = 0;
        FpICCDevice->SCardChildHandle = 0;

        // Slot Number
        FpICCDevice->Slot = i;
        FpICCDevice->WaitTime = INITWAITTIME;
        FpICCDevice->SlotConnectStatus = 0;
        FpICCDevice->ConfiguredStatus = 0;


        if (CcidDevData->IccDeviceList.ForwardLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.ForwardLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return EFI_DEVICE_ERROR;
            }
        }
        if (CcidDevData->IccDeviceList.BackLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.BackLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return EFI_DEVICE_ERROR;
            }
        }


        // Add to the slot list
        InsertTailList(&(CcidDevData->IccDeviceList), &(FpICCDevice->Link));
        
        if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI && (FpICCDevice->ChildHandle == NULL)) {
            ICC_SmiQueuePut((void *)FpICCDevice);
        }
    }

    return EFI_SUCCESS;
}

/**
    This function checks an interface descriptor of a device
    to see if it describes a USB CCID device.  If the device
    is a CCID device,  then it is configured
    and initialized.

    @param  FpHCStruc          Pointer to Host controller structure
    @param  FpDevInfo          Pointer to devInfo structure
    @param  FpDesc             Pointer to the descriptor structure
    @param  StartOffset        Start offset of the device descriptor
    @param  EndOffset          End offset of the device descriptor

    @retval EFI_SUCCESS       Success
    @retval Others            Error

    @retval  FpDevInfo         New device info structure
    @retval  NULL              On error

**/
DEV_INFO*
EFIAPI
USBCCIDConfigureDevice (
    HC_STRUC        *FpHCStruc,
    DEV_INFO        *FpDevInfo,
    UINT8           *FpDesc,
    UINT16          StartOffset,
    UINT16          EndOffset
)
{

    EFI_STATUS        Status;
    INTRF_DESC      *FpIntrfDesc = (INTRF_DESC*)(FpDesc + StartOffset);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBCCIDConfigureDevice ....\n");

    //
    // Do some house keeping related DEV_INFO structure. No H/W access
    //
    Status = DoDevInfoInitialization(FpDevInfo, FpDesc, StartOffset, EndOffset);

    if (EFI_ERROR(Status)) {
        return NULL;
    }

    // Add number if ICC_DEVICE structure(based in the Slot present in the Descriptor) in DEV_INFO structure
    UpdateSCardSlotInfo(FpDevInfo);


    if ((gUsbData->UsbFeature & USB_CCID_USE_INT_INS_REMOVAL) == USB_CCID_USE_INT_INS_REMOVAL){
    //
    // if Interrupt EndPoint is supported
    //
    if (FpIntrfDesc->NumEndpoints == 3) {
    FpDevInfo->PollingLength = FpDevInfo->IntInMaxPkt;
        AmiUsbActivatePolling(FpHCStruc, FpDevInfo);
    }

    }else{
    Status = ICCInsertEvent(FpDevInfo, 0);
    if (EFI_ERROR(Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3,"ICCInsertEvent Status = %r\n", Status);
    }
    }

    //
    // Should we support CCID which doesn't support interrupt-IN Message.
    // Maybe not for now.
    //
    return  FpDevInfo;

}


/**
    This function disconnects the CCID device.

    @param  FpDevInfo          Pointer to devInfo structure   
         
    @retval USB_SUCCESS        Success
    @retval Others             Error

    @note  Free up all memory allocated to the device. 
              Remove ICC device from the device list.

**/

UINT8
EFIAPI
USBCCIDDisconnectDevice (
    DEV_INFO    *FpDevInfo
)
{

    ICC_DEVICE *FpICCDevice;
    LIST_ENTRY *Link;
    HC_STRUC   *fpHCStruc = gHcTable[FpDevInfo->HcNumber - 1];
    SMARTCLASS_DESC *CCIDDescriptor;
    UINT8      *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    EFI_STATUS  Status;
    CCID_DEV_INFO   *CcidDevData;

    CcidDevData = (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if(!CcidDevData) return USB_ERROR;


    if (((UINTN)CcidDevData->CcidDescriptor < (UINTN)gUsbDataList->MemBlockStart) ||
        ((UINTN)CcidDevData->CcidDescriptor > (UINTN)MemBlockEnd)) {
        return USB_ERROR;
    }
    Link = CcidDevData->IccDeviceList.ForwardLink;
    CCIDDescriptor = (SMARTCLASS_DESC*)CcidDevData->CcidDescriptor;


    if (Link != NULL) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)Link, (UINT32)sizeof(LIST_ENTRY));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return USB_ERROR;
        }
    }



    if ((gUsbData->UsbFeature & USB_CCID_USE_INT_INS_REMOVAL) == USB_CCID_USE_INT_INS_REMOVAL){
        // Stop polling the endpoint
        AmiUsbDeactivatePolling(fpHCStruc,FpDevInfo);
        FpDevInfo->IntInEndpoint = 0;
    }
    //
    // Free up all the memory allocated for each ICC device
    //
    for (Link = CcidDevData->IccDeviceList.ForwardLink;
        Link != &CcidDevData->IccDeviceList; 
        Link = Link->ForwardLink ) {
        FpICCDevice = BASE_CR(Link, ICC_DEVICE, Link);
    

        if (((UINTN)FpICCDevice < (UINTN)gUsbDataList->MemBlockStart) ||
            (((UINTN)FpICCDevice + sizeof(ICC_DEVICE)) > (UINTN)MemBlockEnd)) {
            return USB_ERROR;
        }

        if (CcidDevData->IccDeviceList.ForwardLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.ForwardLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return USB_ERROR;
            }
        }
        if (CcidDevData->IccDeviceList.BackLink != NULL) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)(CcidDevData->IccDeviceList.BackLink), 
                            (UINT32)sizeof(LIST_ENTRY));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return USB_ERROR;
            }
        }

        RemoveEntryList(&FpICCDevice->Link);
        USB_MemFree(FpICCDevice, (UINT8)GET_MEM_BLK_COUNT(sizeof(ICC_DEVICE)));    
    }

    if (CcidDevData->DataRates) {
        USB_MemFree(CcidDevData->DataRates, 
                    (UINT8)GET_MEM_BLK_COUNT(CCIDDescriptor->bNumDataRatesSupported * sizeof(UINT32))
                    );
    }

    if (CcidDevData->ClockFrequencies) {
        USB_MemFree(CcidDevData->ClockFrequencies, 
                    (UINT8)GET_MEM_BLK_COUNT(CCIDDescriptor->bNumClockSupported * sizeof(UINT32))
                    );
    }

    //
    // Free up all the memory allocated for CCID Descriptor
    //
    USB_MemFree(CCIDDescriptor, 
                (UINT8)GET_MEM_BLK_COUNT(sizeof(SMARTCLASS_DESC))
                );

    CcidDevData->CcidDescriptor = 0;

    USB_MemFree(CcidDevData, 
                (UINT8)GET_MEM_BLK_COUNT(sizeof(CCID_DEV_INFO))
                );

    return USB_SUCCESS;
}



/**
    This routine services the USB API function number 30h.  It
    handles all the CCID related calls from the higher
    layer. Different sub-functions are invoked depending on
    the sub-function number

    @param UsbUrp     Pointer to the URP structure
                      URP structure is updated with the relevant information

    @retval None
**/

VOID
EFIAPI
USBAPI_CCIDRequest(
    URP_STRUC *UsbUrp
)
{

    
    UINT8 CcidFuncIndex = UsbUrp->bSubFunc;
    UINT8 NumberOfCcidFunctions = sizeof(aUsbCCIDApiTable) / sizeof(API_FUNC *);

    if ((gUsbData->UsbDevSupport & USB_CCID_DEV_SUPPORT) == USB_CCID_DEV_SUPPORT){
    //
    // Make sure function number is valid
    //
    if (CcidFuncIndex >= NumberOfCcidFunctions) {
        //UsbUrp->bRetValue = USBAPI_INVALID_FUNCTION;
        USB_DEBUG(DEBUG_ERROR, 3, "UsbApi CCIDRequest Invalid function#%x\n", CcidFuncIndex);
        return;
    }
    //
    // Function number is valid - call it
    //
    aUsbCCIDApiTable[CcidFuncIndex](UsbUrp);
    }


}
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
