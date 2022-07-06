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

/** @file SmmEhci.c
   AMI USB EHCI SMM driver
**/


#include <AmiUsb.h>
#include <UsbKbd.h>

UINT32  EFIAPI HcReadPciReg(HC_STRUC*, UINT32);
VOID*   EFIAPI EhciMemAlloc(HC_STRUC*, UINT16);
VOID    EFIAPI EHCISetQTDBufferPointers(EHCI_QTD*, UINT8*, UINT32);
UINT8   EFIAPI EhciAddPeriodicQh(HC_STRUC*, EHCI_QH*);
UINT8   EFIAPI USB_InstallCallBackFunction (CALLBACK_FUNC);
VOID    EFIAPI USBKeyRepeat(HC_STRUC*, UINT8);
UINT8   EFIAPI EhciRepeatTDCallback(HC_STRUC*, DEV_INFO*, UINT8*, UINT8*, UINT16);
extern  USB_GLOBAL_DATA     *gUsbData;


/**
    This function is call to read debug register

    @param HcStruc   Ptr to the host controller structure
    @param BarIndex  Bar Index
    @param Offset    Offset for read.

    @retval Value    Return the value.

**/
UINT32
EFIAPI
EhciReadDebugReg(
    HC_STRUC    *HcStruc,
    UINT8       BarIndex,
    UINT32      Offset
)
{
    UINT8  BarOffset[6] = {0x10, 0x14, 0x18, 0x1C, 0x20, 0x24};
    UINT32 DebugPortsMem;

    DebugPortsMem = HcReadPciReg(HcStruc, BarOffset[BarIndex]);
    return DwordReadMem(DebugPortsMem, Offset);
}


/**
    This function is call to enable legacy key repeat.

    @param HcStruc   Ptr to the host controller structure

    @retval EFI_SUCCESS  Success to enable legacy key repeat.

**/
EFI_STATUS
EFIAPI
EhciEnableLegacyKeyRepeat(
    HC_STRUC    *HcStruc
)
{
    EHCI_QH            *QhRepeat = NULL;
    EHCI_DESC_PTRS     *DescPtr = NULL;
    EHCI_QTD           *QtdRepeat = NULL;

    // Check whether no companion host controllers
    if (!(HcStruc->HcFlag & HC_STATE_EXTERNAL) &&
        (HcStruc->HcsParams & EHCI_N_CC) == 0) {
        //
        // Allocate a QH/qTD for QHRepeat/qTDRepeat
        //
        QhRepeat = EhciMemAlloc(HcStruc,
                       GET_MEM_BLK_COUNT(sizeof(EHCI_QH) + sizeof(EHCI_QTD)));
    
        if (!QhRepeat) {
            return  USB_ERROR;      // Memory allocation error
        }
        DescPtr = HcStruc->DescPtrs.EhciDescPtrs;
        DescPtr->fpQHRepeat = QhRepeat;
        QtdRepeat = (EHCI_QTD*)((UINT32)QhRepeat + sizeof(EHCI_QH));
        DescPtr->fpqTDRepeat = QtdRepeat;
//
// Setup QHRepeat and qTDRepeat.  It will run a interrupt transaction to a
// nonexistant dummy device.  This will have the effect of generating
// a periodic interrupt used to generate keyboard repeat.  This QH/qTD
// is normally inactive,  and is only activated when a key is pressed.
//
        //
        // Set the first qTD pointer
        //
        QhRepeat->fpFirstqTD = QtdRepeat;

        QhRepeat->dNextqTDPtr = (UINT32)QtdRepeat;

        //
        // Intialize the queue head
        //
        QhRepeat->dAltNextqTDPtr = EHCI_TERMINATE;
        QhRepeat->dLinkPointer = EHCI_TERMINATE;

        //
        // Set max packet size, address, endpoint and high speed
        // Update the AH's endpoint characteristcs field with the data formed
        //
        QhRepeat->dEndPntCharac |= ((0x40 << 16) | (UINT32)(gUsbData->MaxDevCount+1) |
                                                                QH_HIGH_SPEED);

        //
        // Set a bit in interrupt mask
        //
        QhRepeat->dEndPntCap    = (BIT0 | QH_ONE_XFER);
        //QhRepeat->Interval = REPEAT_INTERVAL;
        if (gUsbData->RepeatIntervalMode){
            QhRepeat->Interval = REPEAT_INTERVAL_16MS;
        } else {
            QhRepeat->Interval = REPEAT_INTERVAL_8MS;
        }
//
// Fill the repeat qTD with relevant information
// The token field will be set so
//       Direction PID = QTD_IN_TOKEN,
//       Size = size of the data,
//       Data Toggle = QTD_DATA0_TOGGLE,
//       Error Count = QTD_NO_ERRORS,
//       Status code = QTD_ACTIVE
// The buffer pointers field will point to the fpBuffer buffer
//       which was before initialized to contain a DeviceRequest struc.
// The dNextqTDPtr field will point to the qTDControlSetup
// The dAltNextqTDPtr field will be set to EHCI_TERMINATE
//
        QhRepeat->dTokenReload = ((UINT32)8 << 16) | QTD_IN_TOKEN | QTD_ONE_ERROR;
        QtdRepeat->dToken = ((UINT32)8 << 16) | QTD_IN_TOKEN | QTD_ONE_ERROR;

        EHCISetQTDBufferPointers(QtdRepeat, &QhRepeat->aDataBuffer[0], 8);

        //
        // Update next & alternate next qTD pointers
        //
        QtdRepeat->dNextqTDPtr = EHCI_TERMINATE;
        QtdRepeat->dAltNextqTDPtr = EHCI_TERMINATE;

        //
        // Schedule the QHRepeat to 8ms schedule
        //
        EhciAddPeriodicQh(HcStruc,QhRepeat);

        QhRepeat->CallBackIndex = USB_InstallCallBackFunction(EhciRepeatTDCallback);
        QhRepeat->bActive = FALSE;

        USBKeyRepeat(HcStruc, 0);
    }

    return EFI_SUCCESS;
}

/**
    This function returns the PCI register offset for the legacy
    support in EHCI controller

    @param HcStruc   Ptr to the host controller structure
    @param PciAddr   PCI address of the EHCI host controller

    @retval 0        If the feature is not present
    @retval <>0       Legacy support capability offset

**/

UINT8
EFIAPI
EHCIGetLegacySupportOffset (
    HC_STRUC    *HcStruc,
    UINT16      PciAddr
)
{
    UINT8   Ptr = 0;
    UINT32  Data = 0;

    if (HcStruc->HcFlag & HC_STATE_IRQ) {
        return 0;
    }

    if (HcStruc->HcFlag & HC_STATE_EXTERNAL) {
        return 0;
    }

    //
    // Get EHCI Extended Capabilities Pointer
    //
    Ptr = (UINT8)((HcStruc->HccParams >> 8) & 0xFF);

    if (!Ptr) {
        // No extended capabilities are implemented.
        return 0;
    }

    Data = HcReadPciReg(HcStruc, Ptr);
    if (!((UINT8)Data & 1)) {
        return 0;
    }

    return Ptr;

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
