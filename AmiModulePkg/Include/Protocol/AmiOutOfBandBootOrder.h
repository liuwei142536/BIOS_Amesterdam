//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
#ifndef __AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL__H__
#define __AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL__H__

#define AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL_GUID \
    { 0x9aa85482, 0x6743, 0x4bfa, { 0xa7, 0x6c, 0xf3, 0x77, 0x88, 0x10, 0x40, 0x44 } }


typedef struct _AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL;

typedef struct {
    UINT16 Length; ///< Length of the array of boot options
    UINT16 Order[]; ///< This is the first value in an array with size of Length
} AMI_OUT_OF_BAND_BOOT_ORDER_STRUCT;

typedef EFI_STATUS ( EFIAPI *AMI_OUT_OF_BAND_GET_BOOT_ORDER) (
    IN  AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL  *This,
    OUT AMI_OUT_OF_BAND_BOOT_ORDER_STRUCT **BootOrder
);

typedef EFI_STATUS ( EFIAPI *AMI_OUT_OF_BAND_SET_BOOT_ORDER) (
    IN AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL *This,
    IN AMI_OUT_OF_BAND_BOOT_ORDER_STRUCT *NewBootOrder
);

struct _AMI_OUT_OF_BAND_BOOT_ORDER_PROTOCOL {
    AMI_OUT_OF_BAND_GET_BOOT_ORDER GetBootOrder;
    AMI_OUT_OF_BAND_SET_BOOT_ORDER SetBootOrder;
};

extern EFI_GUID gAmiOutOfBandBootOrderProtcolGuid;

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
