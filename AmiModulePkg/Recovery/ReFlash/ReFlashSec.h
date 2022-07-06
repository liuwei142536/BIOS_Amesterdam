//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**   (C)Copyright 1985 -2017, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
/** @file
  Header file that defines the Secure Flash protocol interface
**/

#ifndef __REFLASH_SECURE_FLASH_PROTOCOL__H__
#define __REFLASH_SECURE_FLASH_PROTOCOL__H__

#ifdef __cplusplus
extern "C"
{
#endif

static EFI_GUID ReflashSecureFlashProtocolGuid = 
	{ 0x3bf4af16, 0xab7c, 0x4b43, { 0x89, 0x8d, 0xab, 0x26, 0xac, 0x5d, 0xdc, 0x6c } };

#pragma pack(push, 1)
/// A Flash Update Block type represents a communication buffer between caller
/// and Secure Flash interface
typedef struct {
    UINT8               FlashOpType;
    UINT32              ROMSection;
    union {
      CHAR8             AmiRomFileName[16]; // can be a file path
      EFI_PHYSICAL_ADDRESS   CapsuleMailboxPtr[2];
    } FwImage;
    UINT32              ImageSize;
    UINT32              MonotonicCounter;
} AMI_FLASH_UPDATE_BLOCK;
#pragma pack(pop)

/// A Secure Flash Session type represents a wrapper to handle communication
/// identity and errors
typedef struct {
    AMI_FLASH_UPDATE_BLOCK FlUpdBlock;
    UINT8                  ErrorCode;
    UINT32                 FSHandle;
} AMI_SECURE_FLASH_SESSION;

typedef struct _AMI_SECURE_FLASH_PROTOCOL AMI_SECURE_FLASH_PROTOCOL;

typedef EFI_STATUS (EFIAPI *AMI_LOAD_FLASH_IMAGE) (
    IN OUT VOID *FuncBlock
);

typedef EFI_STATUS (EFIAPI *AMI_GET_FLASH_UPDATE_POLICY) (
    IN OUT VOID *InfoBlock
);

typedef EFI_STATUS (EFIAPI *AMI_SET_FLASH_UPDATE_METHOD) (
    IN OUT AMI_SECURE_FLASH_SESSION *InfoBlock
);

typedef EFI_STATUS (EFIAPI *AMI_VALIDATE_CAPSULE) (
    IN OUT UINT8    **FwCapsule,
    IN OUT VOID     **FWCapsuleHdr
);

struct _AMI_SECURE_FLASH_PROTOCOL {
	AMI_LOAD_FLASH_IMAGE        LoadImage;
	AMI_GET_FLASH_UPDATE_POLICY GetFlashUpdatePolicy;
	AMI_SET_FLASH_UPDATE_METHOD SetFlashUpdateMethod;
	AMI_VALIDATE_CAPSULE        ValidateCapsule;
	VOID						*ReservedBuffer;
};


    /****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif

//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**   (C)Copyright 1985 - 2017, American Megatrends, Inc.       **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
