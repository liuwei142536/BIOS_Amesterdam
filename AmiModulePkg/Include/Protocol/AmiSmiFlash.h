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
/** @file
  Header file that defines the AMI SmiFlash protocol interfaces.
*/

#ifndef __AMI_SMI_FLASH_PROTOCOL__H__
#define __AMI_SMI_FLASH_PROTOCOL__H__

#define AMI_SMI_FLASH_PROTOCOL_GUID \
    { 0x6f2c06a8, 0xd4b4, 0x4a93, { 0x9f, 0x78, 0x2b, 0x48, 0x0b, 0xe8, 0x0e, 0x0f } }

/// Software SMI numbers used by AmiSmiFlash interface
///@{
#define AMI_SMI_FLASH_ENABLE_FLASH_SMI      0x20
#define AMI_SMI_FLASH_READ_FLASH_SMI        0x21
#define AMI_SMI_FLASH_ERASE_FLASH_SMI       0x22
#define AMI_SMI_FLASH_WRITE_FLASH_SMI       0x23
#define AMI_SMI_FLASH_DISABLE_FLASH_SMI     0x24
#define AMI_SMI_FLASH_GET_FLASH_INFO_SMI    0x25
///@}


/// Version of the AMI_SMI_FLASH_FLASH_BLOCK_TYPE data structure defined by this header.
#define AMI_SMI_FLASH_INFO_BLOCK_VERSION    14

/// Definition of the data buffer for flash read, write, and erase commands
typedef struct {
    UINT64      BufAddr;
    UINT32      BlockAddr;      ///< Block address in ROM file
    UINT32      BlockSize;
    UINT8       ErrorCode;
} AMI_SMI_FLASH_FUNC_BLOCK;

#pragma pack(1)

/// ROM block descriptor.
typedef struct {
    /// Starting offset of the block in the ROM image
    UINT32      StartAddress;
    /// Size of the block
    UINT32      BlockSize;
    /// Block type defined by AMI_SMI_FLASH_FLASH_BLOCK_TYPE enumeration values
    UINT8       Type;
} AMI_SMI_FLASH_BLOCK_DESC;

/// Flash block type definitions
typedef enum {
     AMI_SMI_FLASH_BOOT_BLOCK
    ,AMI_SMI_FLASH_MAIN_BLOCK
    ,AMI_SMI_FLASH_NV_BLOCK
    ,AMI_SMI_FLASH_EC_BLOCK
    ,AMI_SMI_FLASH_OA3_FLASH_BLOCK_DESC_TYPE = 0x4A
    ,AMI_SMI_FLASH_OA3_BLOCK = 0x4A // define an alias
    /// Types from AMI_SMI_FLASH_OEM_BLOCK to AMI_SMI_FLASH_OEM_BLOCK_END
    /// are reserved for OEM blocks.
    ,AMI_SMI_FLASH_OEM_BLOCK = 0x70
    ,AMI_SMI_FLASH_OEM_BLOCK_END = 0x7F
    /// Types from AMI_SMI_FLASH_NC_BLOCK to AMI_SMI_FLASH_NC_BLOCK_END
    /// are reserved for non critical blocks.
    ,AMI_SMI_FLASH_NC_BLOCK = 0x80
    ,AMI_SMI_FLASH_NC_BLOCK_END = 0xFF
} AMI_SMI_FLASH_FLASH_BLOCK_TYPE;

/// ROM image descriptor.
///
/// The data structure starts with the header that describes generic ROM image properties followed
/// by the descriptors for individual ROM image blocks.
typedef struct  {
    /// Size of the buffer allocated for this data structure. Initialized by the caller.
    UINT32      Length;
    /// Flag field that is used for communication between protocol produced and consumer.
    UINT8       Implemented;
    /// Version of the data structure
    UINT8       Version;
    /// Number of block descriptors in this data structure
    UINT16      TotalBlocks;
    /// Offset of the Embedded Controller(EC) firmware version in the ROM image.
    /// Only present if Version is greater than or equal to 10.
    /// If EC firmware is not part of the image, this field is zero.
    UINT32      ECVersionOffset;
    /// A bit mask that has to be applied to a value at offset ECVersionOffset in the ROM image to extract EC firmware version.
    /// Only present if Version is greater than or equal to 10.
    /// If EC firmware is not part of the image, this field is zero.
    UINT32      ECVersionMask;
    /// On-board firmware image size
    /// Only present if Version is greater than or equal to 14.
    UINT32      BiosRomSize;
    /// Block size of the flash device
    /// Only present if Version is greater than or equal to 14.
    UINT32      BaseBlockSize;
    /// Reserved for future use.
    /// Only present if Version is greater than or equal to 14.
    UINT8       Reserved[2];
    /// ROM block descriptors. Array with TotalBlocks elements.
    AMI_SMI_FLASH_BLOCK_DESC  Blocks[];
} AMI_SMI_FLASH_INFO_BLOCK;
#pragma pack()

typedef struct _AMI_SMI_FLASH_PROTOCOL AMI_SMI_FLASH_PROTOCOL;

/**
  Procedure to report the Flash Blocks Information.

  @param pInfoBlock     specifies the where the blocks infomation to be stored.

  @retval EFI_SUCCESS             The information of flash blocks are reported
  @retval EFI_BUFFER_TOO_SMALL    The given buffer is too small for flash block information
**/
typedef EFI_STATUS (EFIAPI *AMI_SMI_FLASH_GET_FLASH_INFO) (
    IN OUT AMI_SMI_FLASH_INFO_BLOCK           *InfoBlock
);

/**
  Procedure to enable the ability to write to the flash

  @param FuncBlock    Contain the block information for flash write enable

  @retval EFI_SUCCESS    Enable the ability to write to the flash successfully
**/
typedef EFI_STATUS (EFIAPI *AMI_SMI_FLASH_ENABLE_FLASH) (
    IN OUT AMI_SMI_FLASH_FUNC_BLOCK           *FuncBlock
);

/**
  Procedure to disable the ability to write to the flash

  @param FuncBlock    Contain the block information for flash write disable

  @retval EFI_SUCCESS    Disable the ability to write to the flash successfully
**/
typedef EFI_STATUS (EFIAPI *AMI_SMI_FLASH_DISABLE_FLASH) (
    IN OUT AMI_SMI_FLASH_FUNC_BLOCK           *FuncBlock
);

/**
  Procedure to read Flash block.

  @param FuncBlock    Contain the block information for flash read

  @retval EFI_SUCCESS    Flash block read successfully
**/
typedef EFI_STATUS (EFIAPI *AMI_SMI_FLASH_READ_FLASH) (
    IN OUT AMI_SMI_FLASH_FUNC_BLOCK           *FuncBlock
);

/**
  Procedure to write Flash block.

  @param FuncBlock    Contain the block information for flash write

  @retval EFI_SUCCESS    Flash block write successfully
**/
typedef EFI_STATUS (EFIAPI *AMI_SMI_FLASH_WRITE_FLASH) (
    IN OUT AMI_SMI_FLASH_FUNC_BLOCK           *FuncBlock
);

/**
  Procedure to erase Flash block.

  @param FuncBlock      Contain the block information for flash erase

  @retval EFI_SUCCESS    Flash block erase successfully
**/
typedef EFI_STATUS (EFIAPI *AMI_SMI_FLASH_ERASE_FLASH) (
    IN OUT AMI_SMI_FLASH_FUNC_BLOCK           *FuncBlock
);

///  The AMI_SMI_FLASH_PROTOCOL protocol provides flash device update functions.
///  These include Read, Write, Erase, as well as function to get system firmware layout.
struct _AMI_SMI_FLASH_PROTOCOL {
    AMI_SMI_FLASH_GET_FLASH_INFO  GetFlashInfo;
    AMI_SMI_FLASH_ENABLE_FLASH    EnableFlashWrite;
    AMI_SMI_FLASH_DISABLE_FLASH   DisableFlashWrite;
    AMI_SMI_FLASH_READ_FLASH      ReadFlash;
    AMI_SMI_FLASH_WRITE_FLASH     WriteFlash;
    AMI_SMI_FLASH_ERASE_FLASH     EraseFlash;
    UINT32                        FlashCapacity;
};

extern EFI_GUID gAmiSmiFlashProtocolGuid;
#endif

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
