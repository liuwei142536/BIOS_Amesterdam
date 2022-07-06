//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AmiTcgStorageDeviceReset.h
    @brief AMI TCG Storage Device Reset Header File.

  The file contains definitions of the AMI Specific TCG Storage 
  Device reset related structures and constants.
**/

#ifndef __AMI_TCG_STORAGE_DEVICE_RESET__H__
#define __AMI_TCG_STORAGE_DEVICE_RESET__H__

#ifdef __cplusplus
extern "C" {
#endif
 
typedef struct _AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL;


#define AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL_GUID  \
  { 0x0C29BFB7, 0xB6DB, 0x47E8, { 0xB6, 0xE2, 0x42, 0x2D, 0x0C, 0x09, 0xA1, 0x60 }}

/**
   Resets the whole device using the 32 Byte PSID (PhysicalSecurityIdentification)
   value. PSID revert function is used to unlock the drive, when the access code is lost.
   It will reset the device to Original Factory State.

    @param This     AMI_TCG_DEVICE_RESET_PROTOCOL
    @param Buffer   (32 Bytes)    Pointer to Psid buffer which will be 
                                used to reset the drive.

          
    @retval EFI_SUCCESS            Device Reset Success
    @retval EFI_INVALID_PARAMETER  Invalid PSID value.

**/ 
typedef
EFI_STATUS 
(EFIAPI *AMI_TCG_STORAGE_DEVICE_RESET_PSIDREVERT) (
  IN AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL      *This,
  UINT8                                         *PsidBuffer
);



struct _AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL {
  AMI_TCG_STORAGE_DEVICE_RESET_PSIDREVERT           PsidRevert;
};

#ifdef __cplusplus
}
#endif

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
