#pragma warning(disable: 4001)
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file UsbPoint.c
    AMI USB Absolute Device support implementation

**/

#include "AmiUsb.h"
#include "UsbKbd.h"
#include <Protocol/AbsolutePointer.h>
#include <Protocol/AmiUsbHid.h>

extern  USB_GLOBAL_DATA     *gUsbData;

UINT8       USBAbsProcessMouseData (HC_STRUC*, DEV_INFO*, UINT8*, UINT8*, UINT16);
VOID        SetABSData (UINT8*,UINT16*,UINT8,UINT8,UINT16);       

/**
    This routine checks an interface descriptor of the USB device
    detected to see if it describes a HID/Boot/Mouse device.
    If the device matches the above criteria, then the device is
    configured and initialized

    @param HcStruc   HCStruc pointer
    @param DevInfo   Device information structure pointer
    @param Desc      Pointer to the descriptor structure
    @param Start     Offset within interface descriptor
                     supported by the device
    @param End       End offset of the device descriptor

    @retval DEV_INFO  New device info structure, 0 on error

**/

DEV_INFO*
USBAbsConfigureDevice (
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8*      Desc,
    UINT16      Start,
    UINT16      End
)
{
    return NULL;
}

/**
    Process absolute Mouse Data
    @param HcStruc      Pointer to HCStruc
    @param DevInfo      Pointer to device information structure
    @param Td           Pointer to the polling TD
    @param Buffer       Pointer to the data buffer
    @param DataLength   Data length

    @retval USB_SUCCESS  Success
    @retval Others       Error

**/

UINT8
USBAbsProcessMouseData (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Td,
    UINT8       *Buffer,
    UINT16      DataLength
)
{
    USBABS_DATA         AbsBuffer;
    HID_REPORT_FIELD    *Field = NULL;
    UINT8               OffsetTmp = 0;
    UINT8               XStart = 0;
    UINT8               XEnd;
    UINT8               YStart = 0;
    UINT8               YEnd;
    UINT8               ButtonStart = 0;
    UINT8               ButtonEnd = 0;
    UINT8               Index;
    UINT16              Count;
    UINT16              MaxX;
    UINT16              MaxY;
    BOOLEAN             SetButtonData = FALSE;
    BOOLEAN             SetXData = FALSE;
    BOOLEAN             SetYData = FALSE;

    AbsBuffer.X =0;
    AbsBuffer.Y =0;
    AbsBuffer.Button =0;
    
    for (Index = 0; Index < DevInfo->HidReport.FieldCount; Index++) {
        Field = DevInfo->HidReport.Fields[Index];

        //Check is input?
        if (!(Field->Flag & HID_REPORT_FIELD_FLAG_INPUT)) {
          continue;
        }

       // Check if report id is matched
       if (Field->ReportId != 0 && Field->ReportId != Buffer[0]) {
         continue;
       }

        // Check if the field is contant.
        if (Field->Flag & HID_REPORT_FIELD_FLAG_CONSTANT) {
            OffsetTmp += (UINT8)(Field->ReportCount * Field->ReportSize);
            continue;
        }

        if ((Field->UsagePage == HID_UP_DIGITIZER) && (Field->UsageCount != 0)) {
            for (Count = 0; Count < Field->UsageCount; Count++) {
                //Check Tip switch
                if (Field->Usages[Count] == HID_DIGITIZER_TIP_SWITCH) {
                    if (SetButtonData == TRUE) {
                        break;
                    }
                    ButtonStart = OffsetTmp;
                    if (Field->ReportId != 0) {
                      ButtonStart += 8; 
                    }
                    ButtonEnd = ButtonStart + (UINT8)(Field->ReportSize * Field->ReportCount);    
                    SetABSData(Buffer, (UINT16*)(&AbsBuffer.Button), ButtonStart, ButtonEnd, 0xffff);
                    SetButtonData = TRUE;
                    //In UEFI spec, the definitions of bits within ActiveButtons are EFI_ABSP_TouchActive
                    // and EFI_ABS_AltActive.
                    AbsBuffer.Button &= (EFI_ABSP_TouchActive | EFI_ABS_AltActive);
                }
             }
          }

          //Check Button
        if ((Field->UsagePage == HID_UP_BUTTON) && 
            (Field->UsageCount != 0) &&
            (Field->Usages[0] == HID_BUTTON_BUTTON1)) {
            if (SetButtonData == TRUE) {
                break;
            }
            ButtonStart = OffsetTmp;
            if (Field->ReportId != 0) {
              ButtonStart += 8;
            }
            ButtonEnd = ButtonStart + (UINT8)(Field->ReportSize * Field->ReportCount);    
            SetABSData(Buffer, (UINT16*)(&AbsBuffer.Button), ButtonStart, ButtonEnd, 0xffff);  
            SetButtonData = TRUE;
        }

        //Check X,Y
      if ((Field->UsagePage == HID_UP_GENDESK) && (Field->UsageCount != 0)) {
        for (Count = 0; Count < Field->UsageCount; Count++) {
            //find X
            if (Field->Usages[Count] == HID_GENDESK_X) {
              if (SetXData == TRUE) {
                break;
              }
              XStart = OffsetTmp + (UINT8)(Count * Field->ReportSize);
              if (Field->ReportId != 0) {
                XStart += 8;
              }
              XEnd = XStart + Field->ReportSize;
              SetABSData(Buffer, &AbsBuffer.X, XStart, XEnd, (UINT16)Field->LogicalMax);      
              SetXData = TRUE;
              MaxX = (UINT16)Field->LogicalMax;
        }

        //find Y
        if (Field->Usages[Count] == HID_GENDESK_Y) {
           if (SetYData == TRUE) {
             break;
             }
          YStart = OffsetTmp + (UINT8)(Count * Field->ReportSize);
          if (Field->ReportId != 0) {
            YStart += 8;
          }
          YEnd = YStart + Field->ReportSize;
          MaxY = (UINT16)Field->LogicalMax; 
          SetABSData(Buffer, &AbsBuffer.Y, YStart, YEnd, (UINT16)Field->LogicalMax);        
          SetYData = TRUE;
        }
      }
    }
    OffsetTmp += (UINT8)(Field->ReportCount * Field->ReportSize);
  }
  
    if (AbsBuffer.Button == 0 && AbsBuffer.X == 0 && AbsBuffer.Y == 0) {
        return USB_SUCCESS;
    }
    
    if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI) {
        DevInfo->AbsMouseData.ButtonStauts = AbsBuffer.Button;
        DevInfo->AbsMouseData.Xcoordinate = AbsBuffer.X;
        DevInfo->AbsMouseData.Ycoordinate = AbsBuffer.Y;
        DevInfo->AbsMouseData.Pressure = 0;
        DevInfo->AbsMouseData.Max_X = MaxX;
        DevInfo->AbsMouseData.Max_Y = MaxY;
    }
    return USB_SUCCESS;
}

/**
    This routine sets absolute mouse data

    @param Buffer       Data Buffer
    @param ReportData   Report data
    @param Start        Start offset
    @param End          End offset
    @param Maxvalue     Max Value
    
    @retval None
**/

VOID
SetABSData (
    UINT8       *Buffer,
    UINT16      *ReportData,
    UINT8       Start,
    UINT8       End,
    UINT16      Maxvalue
)
{
    UINT8   ReportSize;
    UINT8   Size;
    UINT8   PreSkip;
    UINT8   PostSkip;
    UINT32  TempData = 0;
    
    ReportSize = End - Start;
    Size = ReportSize / 8;
    
    if ((ReportSize % 8) !=0) {
        Size++;
    }

    ASSERT(Size > 0 && Size <= sizeof(TempData));
    if ((Size == 0) || (Size > sizeof(TempData))) {
        return;
    }
    
    CopyMem(&TempData, Buffer + Start / 8, Size);
    
    PreSkip = Start % 8;
    PostSkip = End % 8;

    if (PreSkip != 0) {
      TempData = TempData >> PreSkip;
    }
    
    if (PostSkip != 0) {
      TempData = TempData << PostSkip;
      TempData = TempData >> PostSkip;
    }
    
    if (TempData > Maxvalue) {
      TempData &= Maxvalue;
    }
    
    *ReportData = (UINT16)TempData;
     USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "out data %x\n",*ReportData);
    
    return;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
