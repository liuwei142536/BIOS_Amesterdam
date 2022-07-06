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
#ifndef _AMI_USB_HC_MEM_RECORD_PROTOCOL_H_
#define _AMI_USB_HC_MEM_RECORD_PROTOCOL_H_

#define AMI_USB_HC_MEM_RECORD_PROTOCOL_GUID \
{0x3279a703, 0xba93, 0x4cbc, {0xad, 0x11, 0x5c, 0x2a, 0x82, 0xc9, 0x4a, 0x80}}

typedef struct _AMI_USB_HC_MEM_RECORD_PROTOCOL {

    EFI_PHYSICAL_ADDRESS        HcMemStart;
    UINTN                       MemTotalPages;

} AMI_USB_HC_MEM_RECORD_PROTOCOL;

#endif
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
