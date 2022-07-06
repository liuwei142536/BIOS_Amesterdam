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

/**
 * @file SetupReflash.c
 * @brief Implementation of firmware update via Setup
 */

#include <Protocol/DevicePath.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FileExplorerLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/AmiCapsuleUpdateLib.h>

#include <Capsule.h>
#include "ReFlash.h"
#include "ReFlashSmm.h"

extern CALLBACK_INFO SetupCallBack[];

static EFI_GUID gAmiFwCapsuleGuid   = APTIO_FW_CAPSULE_GUID;

#ifndef BDS_FW_UPDATE_CAPSULE_LOAD
#define BDS_FW_UPDATE_CAPSULE_LOAD 0
#endif

EFI_DEVICE_PATH_PROTOCOL *ImagePath = NULL;
AMI_REFLASH_FRAMEBUFFER_INFO *FbInfo = NULL;
VOID *SaveScreenBuf = NULL;

VOID *
EFIAPI
GetFileBufferByFilePath (
  IN BOOLEAN                           BootPolicy,
  IN CONST EFI_DEVICE_PATH_PROTOCOL    *FilePath,
  OUT      UINTN                       *FileSize,
  OUT UINT32                           *AuthenticationStatus
  );

/**
 * @brief Check if setup firmware update is enabled
 *
 * @return Status of operation
 */
EFI_STATUS CheckSetupFlashUpdate (VOID)
{
	return (SETUP_FIRMWARE_UPDATE) ? EFI_SUCCESS : EFI_UNSUPPORTED;
}

/**
 * @brief Return filename of firmware image
 *
 * @return Pointer to filename
 */
CHAR16 * GetFileName (VOID)
{
	EFI_DEVICE_PATH_PROTOCOL *dp = ImagePath;
	if (dp == NULL)
		return NULL;
	while (!IsDevicePathEndType (dp)) {
		if (DevicePathType (dp) == MEDIA_DEVICE_PATH &&
			DevicePathSubType (dp) == MEDIA_FILEPATH_DP) {
			return (CHAR16 *)(dp + 1);
		}
		dp = NextDevicePathNode (dp);
	}
	return NULL;
}

/**
 * @brief Set subtitle strings
 *
 * @param str Pointer to image filename
 */
VOID SetSubtitleString (CHAR16 *str)
{
    CHAR16 Subtitle1[256];
    static BOOLEAN FirstRun = TRUE;
    
    if (FirstRun) {
        HiiSetString (ReflashHiiHandle, STRING_TOKEN(STR_RECOVERY), L"Firmware Update", NULL);
        FirstRun = FALSE;
    }
	
    if (str == NULL)
    	UnicodeSPrintAsciiFormat (Subtitle1, 256, "Image file: Not defined");
    else
    	UnicodeSPrintAsciiFormat (Subtitle1, 256, "Image file: %s", str);

    HiiSetString (ReflashHiiHandle, STRING_TOKEN(STR_SETUP_FLASH_SUBTITLE1), Subtitle1, NULL);
}

/**
 * @brief Setup framebuffer descriptor
 */
VOID EFIAPI SetupGop (VOID)
{
	EFI_STATUS st;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	
	st = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gop);
	if (st)
		return;

	FbInfo->FrameBuf = (UINT32 *)(UINTN)gop->Mode->FrameBufferBase;
	FbInfo->FrameBufSz = (UINT32)gop->Mode->FrameBufferSize;
	FbInfo->HorRes = (UINT16)gop->Mode->Info->HorizontalResolution;
	FbInfo->VerRes = (UINT16)gop->Mode->Info->VerticalResolution;
	FbInfo->PxScanLine = (UINT16)gop->Mode->Info->PixelsPerScanLine;
	FbInfo->PxMask = (UINT16)gop->Mode->Info->PixelFormat;
}

/**
 * @brief Callback for capsule firmware update
 *
 * @param Event Callback event
 * @param Context Callback context
 */
VOID EFIAPI ProcessCapsuleImage (IN EFI_EVENT Event, IN VOID *Context)
{
    EFI_STATUS st;
    VOID *Image;
    st = gBS->LocateProtocol (&gAmiFwCapsuleGuid, NULL, &Image);
    if (!st)
        st = AmiPreprocessCapsule (Image, FLASH_SIZE, &Image, NULL);
    if (!st) {
        UpdateImage (Image);
        gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
}

/**
 * @brief Install capsule loaded callback
 */
VOID SetBdsCapsuleLoadCallback (VOID)
{
	VOID *Dummy;
	
    //
    // Register notify function on AmiFwImage protocol to initiate flash update
    //
    if (BDS_FW_UPDATE_CAPSULE_LOAD)
        EfiCreateProtocolNotifyEvent (
            &gAmiFwCapsuleGuid,
            TPL_CALLBACK,
            ProcessCapsuleImage,
            NULL,
            &Dummy
        );
}

/**
 * @brief Setup firmware update entrypoint
 *
 * @return Status of the execution
 */
EFI_STATUS SetupFlashUpdateFlow (VOID)
{
	UINTN Size;
	EFI_STATUS Status;
	UINT32 Attributes;
	
    AUTOFLASH2 AutoFlash = {
    	1
    };
    
//Update Reflash parameters
    Size = sizeof(AUTOFLASH2);
    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    Status = gRT->SetVariable(L"Setup2", &gAmiRecoveryFormsetGuid, Attributes, Size, &AutoFlash);
    InitHiiHandle ();
    SetSubtitleString (GetFileName ());
    FbInfo = AllocateRuntimePool (sizeof (*FbInfo));
    FbInfo->FrameBuf = NULL;		//flag, that FbInfo is not valid
    
    return Status;
}

/**
 * @brief Firmware update image file selection dialog
 */
VOID SelectImageFile (VOID)
{
    EFI_STATUS st;
    
    st = ChooseFile (NULL, NULL, NULL, &ImagePath);
    if (!st)
        SetSubtitleString (GetFileName ());
    return;
}

/**
 * @brief Save framebuffer content
 */
VOID GopSaveScreen (VOID)
{
    if (FbInfo->FrameBuf != NULL) {
        SaveScreenBuf = AllocatePool (FbInfo->FrameBufSz);
        CopyMem (SaveScreenBuf, FbInfo->FrameBuf, FbInfo->FrameBufSz);
        SetMem (FbInfo->FrameBuf, FbInfo->FrameBufSz, 0);
    }
}

/**
 * @brief Restore framebuffer content
 */
VOID GopRestoreScreen (VOID)
{
    if (FbInfo->FrameBuf != NULL) {
        CopyMem (FbInfo->FrameBuf, SaveScreenBuf, FbInfo->FrameBufSz);
        FreePool (SaveScreenBuf);
    }
}

/**
 * @brief Flash new firmware image
 *
 * @param Image Pointer to image
 *
 * @return Status of operation
 */
EFI_STATUS UpdateImage (VOID *Image)
{
    VOID *Buffer;
    UINTN Size;
    UINT32 Auth;
    EFI_STATUS st;
    
    if (Image != NULL) {
        Buffer = Image;
        Size = FLASH_SIZE;
    } else {
        if (ImagePath == NULL)
            return EFI_INVALID_PARAMETER;
        Buffer = GetFileBufferByFilePath (FALSE, ImagePath, &Size, &Auth);
        if (Buffer == NULL || Size != FLASH_SIZE) {
            DEBUG ((-1, "can't find file or file is of wrong size\n"));
            return EFI_NOT_FOUND;
        }
    }

    SetupGop ();
    SmmCommBuffer->Header.Data[0] = (UINT64)(UINTN)Buffer;
    SmmCommBuffer->Header.Data[1] = (UINT64)Size;
    SmmCommBuffer->Header.Data[2] = (UINT64)(UINTN)FbInfo;
    GopSaveScreen ();
    st = ExecuteSmmCall (AMI_REFLASH_SMM_UPDATE_IMAGE);
    GopRestoreScreen ();
    return st;
}

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
