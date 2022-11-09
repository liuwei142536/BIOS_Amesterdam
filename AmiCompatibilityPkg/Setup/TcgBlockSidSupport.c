//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/**
 * @file
 * This file contains functions to handle TCG Disable Block SID setup option. 
 */

#include <Token.h>
#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Library/BaseLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/HiiLib.h>
#include <Library/DxeServicesLib.h>
#include <Setup.h>
#include <Protocol/StorageSecurityCommand.h>
#include <IndustryStandard/TcgStorageCore.h>
#include <Guid/MdeModuleHii.h>

#define TCG2_INF_FILE_GUID \
  { 0x0718AD81, 0xF26A, 0x4850, {0xA6, 0xEC, 0xF2, 0x68, 0xE3, 0x09, 0xD7, 0x07}}

static BOOLEAN gPublishBlockSidSetupOption = FALSE;
  
/** @internal
 * Function converts Big endian dword to Little Endian Dword and Vice versa.
 * 
 * @param [in]  EndianDword
 * 
 * @retval UINT16
 * @endinternal
**/

UINT32
ToBigLittleEndianDword (
    UINT32 EndianDword)
{
    return (((EndianDword & 0xFF) << 24) + ((EndianDword & 0xFF00) << 8) + \
            ((EndianDword & 0xFF0000) >>8) + ((EndianDword & 0xFF000000) >>24));
}

/** @internal
 * Function converts Big endian word to Little Endian and Vice versa.
 * 
 * @param [in]  EndianWord
 * 
 * @retval UINT16
 * 
 * @endinternal
**/

UINT16
ToBigLittleEndianWord (
    UINT16 EndianWord)
{
    return (((EndianWord >> 8) & 0xFF) + (EndianWord << 8));
}

/** @internal
    This function will dynamically add VFR contents to Security BIOS setup page
    using HII library functions.

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
CreateTcgBlockSidSetupOption (
  EFI_HANDLE      HiiHandle
  )
{
    VOID                    *StartOpCodeHandle;
    VOID                    *EndOpCodeHandle;
    VOID                    *YesNoOneOfOptionOpCodeHandle;
    EFI_IFR_GUID_LABEL      *StartLabel;
    EFI_IFR_GUID_LABEL      *EndLabel;
    static EFI_GUID         SecurityFormsetGuid = SECURITY_FORM_SET_GUID;

    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    EndOpCodeHandle = HiiAllocateOpCodeHandle ();
    YesNoOneOfOptionOpCodeHandle = HiiAllocateOpCodeHandle ();

    // Return if failed to allocate memory for new Opcode handle
    if (!StartOpCodeHandle || !EndOpCodeHandle || !YesNoOneOfOptionOpCodeHandle ) {
        DEBUG((DEBUG_ERROR, "%a(): Failed to allocate new opcode handle.\n"));
        return;
    }

    // Create Hii Extended Label OpCode as the start and end opcode
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, 
                                                             &gEfiIfrTianoGuid, 
                                                             NULL, 
                                                             sizeof (EFI_IFR_GUID_LABEL));

    // Return if failed to create Hii extended Label Opcode
    if (!StartLabel ) {
        DEBUG((DEBUG_ERROR, "%a(): Failed to create StartLabel Hii extended label opcode.\n"));
        return;
    }

    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, 
                                                           &gEfiIfrTianoGuid, 
                                                           NULL, 
                                                           sizeof (EFI_IFR_GUID_LABEL));

    // Return if failed to create Hii extended Label Opcode
    if (!EndLabel ) {
        DEBUG((DEBUG_ERROR, "%a(): Failed to create EndLabel Hii extended label opcode.\n"));
        return;
    }
    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    StartLabel->Number = SETUP_DISABLE_TCG_BLOCK_SID_DYNAMIC_ITEM_START;
    EndLabel->Number = SETUP_DISABLE_TCG_BLOCK_SID_DYNAMIC_ITEM_END;
    
    HiiCreateOneOfOptionOpCode (YesNoOneOfOptionOpCodeHandle,
                                STRING_TOKEN(STR_DISABLED),
                                (EFI_IFR_OPTION_DEFAULT | EFI_IFR_OPTION_DEFAULT_MFG),
                                EFI_IFR_NUMERIC_SIZE_1,
                                0 );

    HiiCreateOneOfOptionOpCode (YesNoOneOfOptionOpCodeHandle,
                                STRING_TOKEN(STR_ENABLED),
                                0,
                                EFI_IFR_NUMERIC_SIZE_1,
                                1 );
    // HiiCreateOneOfOpCode (StartOpCodeHandle,
    //                       0,
    //                       (EFI_VARSTORE_ID)SETUP_DATA_KEY_ID,
    //                       (UINT16)OFFSET_OF(SETUP_DATA, SetupDisableTcgBlockSid),
    //                       STRING_TOKEN(STR_TCG_BLOCK_SID_PROMPT),
    //                       STRING_TOKEN(STR_TCG_BLOCK_SID_PROMPT_HELP),
    //                       EFI_IFR_FLAG_RESET_REQUIRED,
    //                       EFI_IFR_NUMERIC_SIZE_1,
    //                       YesNoOneOfOptionOpCodeHandle,
    //                       NULL);
    
    //Done!! Update form.
    HiiUpdateForm ( HiiHandle,
                    &SecurityFormsetGuid,
                    SECURITY_MAIN,
                    StartOpCodeHandle,
                    EndOpCodeHandle);
    
    DEBUG((DEBUG_INFO, "%a(): Security from Updated with OneOf opcode.\n", __FUNCTION__));
    
    HiiFreeOpCodeHandle (YesNoOneOfOptionOpCodeHandle);
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    
    return;
}

/**
 * Function that will publish check for the presence of TCG storage drives
 * with Block SID support. If any devices found, L"TcgBlockSidDevicePresent"
 * NVRAM variable will be set based on which "Disable Block SID" setup option
 * will be shown or suppressed.
 * 
 * @param  HiiHandle    HII handle
   @param  Class        Class value of Setup Form Set
 * 
 * @return
 */
VOID 
InitTcgBlockSidVar (
    IN EFI_HII_HANDLE    HiiHandle, 
    IN UINT16            Class)
{
    EFI_STATUS                            Status;
    EFI_HANDLE                            *HandleBuffer;
    UINTN                                 Index;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL *StorageSecurityProtocol = NULL;
    UINTN                                 NumOfHandles = 0;
    BOOLEAN                               IsBlockSidDevicePresent = FALSE;
    UINTN                                 PayloadTransferSize = 0;
    UINT8                                 QueryBuffer[0x200];
    TCG_SUPPORTED_SECURITY_PROTOCOLS      *SupportedProtocolList;
    TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER  *FeatureDescriptor;
    UINT16                                FeatureCode;
    UINTN                                 SpByte;
    UINT16                                ListLength;
    UINTN                                 TotalLength;
    
    if (gPublishBlockSidSetupOption == FALSE) {
        DEBUG ((DEBUG_ERROR, "%a(): Skip publishing Disable Block SID setup option.\n", __FUNCTION__));
        return;
    }
    Status = gBS->LocateHandleBuffer (
                                ByProtocol,
                                &gEfiStorageSecurityCommandProtocolGuid,
                                NULL,
                                &NumOfHandles,
                                &HandleBuffer);
    
    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "%a():Failed to get handles of gEfiStorageSecurityCommandProtocolGuid, Status:%r.\n", __FUNCTION__, Status));
        return;   
    }
    
    for (Index = 0; Index < NumOfHandles; Index++) {

        Status = gBS->HandleProtocol (
                                HandleBuffer[Index],
                                &gEfiStorageSecurityCommandProtocolGuid,
                                (VOID **) &StorageSecurityProtocol);
                   
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "%a():Failed to get the instance: %d of gEfiStorageSecurityCommandProtocolGuid, Status: %r.\n", 
                    __FUNCTION__, Index, Status));
            continue;
        }
       
        ZeroMem (QueryBuffer, 0x200);
       
        // SPC Spec Section 7.7.1 has details on decoding the data
        Status = StorageSecurityProtocol->ReceiveData( 
                                                StorageSecurityProtocol,
                                                0,
                                                100000000,    // 10 seconds timeout ( in terms of 100ns)
                                                TCG_SECURITY_PROTOCOL_INFO,      // Security Protocol ID
                                                ToBigLittleEndianWord(TCG_SP_SPECIFIC_PROTOCOL_LIST), // SP specific id
                                                0x200,                           // Transfer Length
                                                &QueryBuffer,
                                                &PayloadTransferSize);             // Transferred Length
        
        if (EFI_ERROR(Status)) {
            DEBUG((DEBUG_ERROR, "%a(): Failed to get Security protocol information, Status: %r\n", __FUNCTION__, Status));
            continue;
        }
       
        SupportedProtocolList = (TCG_SUPPORTED_SECURITY_PROTOCOLS *)QueryBuffer;
        // The List Length field indicates the total length, in bytes, of the supported security 
        // protocol list.
        ListLength = ToBigLittleEndianWord (SupportedProtocolList->ListLength_BE);
        
        if (ListLength == 0) {
            continue;
        }
        
        DEBUG((DEBUG_INFO, "%a():ListLength:%d\n",__FUNCTION__, ListLength));
        
        // Check Security supported Protocol List for Protocol ID 1
        for (SpByte = 0; SpByte < ListLength; SpByte++) {
            DEBUG ((DEBUG_INFO, "%a(): SpByte =: %x, SupportedProtocolList = %x\n",
                               __FUNCTION__, SpByte, SupportedProtocolList->List[SpByte]));  
            if (SupportedProtocolList->List[SpByte] == TCG_OPAL_SECURITY_PROTOCOL_1) {    
               break;
           }
        }
        
        if (SpByte == ListLength) {
           DEBUG((DEBUG_INFO, "%a(): TCG_OPAL_SECURITY_PROTOCOL_1 is not supported.\n", __FUNCTION__));
           continue;
        }
        // Check whether device supports TCG Block SID feature from the Level 0 
        // discovery data.
        ZeroMem(QueryBuffer,0x200);
        
        Status = StorageSecurityProtocol->ReceiveData (
                                               StorageSecurityProtocol,
                                               0,
                                               100000000,    // 10 seconds timeout ( in terms of 100ns)
                                               TCG_OPAL_SECURITY_PROTOCOL_1,                                    // Security ID
                                               ToBigLittleEndianWord(TCG_SP_SPECIFIC_PROTOCOL_LEVEL0_DISCOVERY),// SP specific id - COM ID
                                               0x200,                                                           // TranferLength
                                               QueryBuffer,
                                               &PayloadTransferSize);
        
        if (EFI_ERROR(Status)) {
           DEBUG ((DEBUG_ERROR, "%a(): Error getting Level0 discovery Data, Command status = %r.\n", __FUNCTION__, Status));
           continue;
        }
        
        TotalLength =  ToBigLittleEndianDword(((TCG_LEVEL0_DISCOVERY_HEADER*)QueryBuffer)->LengthBE) + 4;
        FeatureDescriptor = (TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER  *)((UINT8 *)QueryBuffer + 
                                                       sizeof(TCG_LEVEL0_DISCOVERY_HEADER));
        DEBUG((DEBUG_INFO, "%a():TotalLength: %d\n", __FUNCTION__, TotalLength));
        
        do {
           FeatureCode = ToBigLittleEndianWord(FeatureDescriptor->FeatureCode_BE);
           // Check for the Block SID feature and Locking feature in the supported feature Code
           if (FeatureCode == TCG_FEATURE_BLOCK_SID) {
               IsBlockSidDevicePresent = TRUE;
               DEBUG((DEBUG_INFO, "%a(): TCG storage drive with Block SID feature support is found.\n", __FUNCTION__ ));
               break;
           }
           // Get the Next Feature Descriptor
          FeatureDescriptor = (TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER *)((UINT8 *)FeatureDescriptor + 
                                                              FeatureDescriptor->Length + 4);
        } while ((UINT8 *)FeatureDescriptor < (UINT8 *)((UINT8 *)QueryBuffer + TotalLength));
        
        // At least one drive found with Block SID feature support is enough to show 
        // "Disable Block SID" setup option.
        if (IsBlockSidDevicePresent) {
           break;
        }
    }
    
    // Found drive with Block SID support and "Disable Block Sid" setup option will be displayed.
    if (IsBlockSidDevicePresent) {
        // CreateTcgBlockSidSetupOption (HiiHandle);
    } else {
        DEBUG((DEBUG_INFO, "%a(): Storage device with TCG Block SID feature support is not found.\n", __FUNCTION__));
    }
    
    if (NumOfHandles) {
        FreePool (HandleBuffer);
    }
    return;
}