//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

#include <Uefi.h>

#pragma pack(1)
typedef struct
{
    UINT32      Signature;
    UINT32      FlashAddress;
    UINTN       Size;
    EFI_STATUS  Status;
    UINT16      Subfunction;
    UINT32      DataBuffer;
}FLASH_SMI_UPDATE;
#pragma pack()

#define FSMI_SIGNATURE SIGNATURE_32('F','S','M','I')

#define FLASH_SMI_COMMUNICATE_SIZE (OFFSET_OF(EFI_SMM_COMMUNICATE_HEADER, Data) + sizeof(FLASH_SMI_UPDATE))

#define FLASH_SMI_GUID \
    { \
        0x4052ACA8, 0x8D90, 0x4F5A, { 0xBF, 0xE8, 0xB8, 0x95, 0xB1, 0x64, 0xE4, 0x82 } \
    }

#define BDS_CONNECT_DRIVERS_PROTOCOL_GUID \
    { \
        0x3AA83745, 0x9454, 0x4F7A, { 0xA7, 0xC0, 0x90, 0xDB, 0xD0, 0x2F, 0xAB, 0x8E } \
    }

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************