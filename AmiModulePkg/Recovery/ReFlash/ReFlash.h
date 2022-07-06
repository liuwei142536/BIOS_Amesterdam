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

/** @file
  This file contains Reflash driver structures definitions

**/
#ifndef __REFLASH__H__
#define __REFLASH__H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <ReFlashStrDefs.h>
#include <Token.h>
#ifndef VFRCOMPILE
#include <AmiDxeLib.h>
#include "ReFlashProgress.h"
#endif

#define RECOVERY_FORM_SET_CLASS             0x40
// {80E1202E-2697-4264-9CC9-80762C3E5863}
#define RECOVERY_FORM_SET_GUID \
    { 0x80e1202e, 0x2697, 0x4264, { 0x9c, 0xc9, 0x80, 0x76, 0x2c, 0x3e, 0x58, 0x63 }}

extern EFI_GUID gAmiRecoveryFormsetGuid;

#if REFLASH_INTERACTIVE
#define RECOVERY_MAIN 1
#define RECOVERY_FLASH 2
#define RECOVERY_USER 3
#else
#define RECOVERY_FLASH 1
#endif

#define FLASH_PROGRESS_KEY  	100
#define FLASH_START_KEY 		101
#define FLASH_ENABLE_KEY 		102
#define FLASH_SELECT_FILE_KEY	103
#define FLASH_VERIFY_IMAGE_KEY	104
#define FLASH_UPDATE_IMAGE_KEY	105

#pragma pack(push, 1)
typedef struct {
        UINT8 UserOverride;                 //!< User override flag
        UINT8 VerificationStatus;           //!< Recovery capsule verification status
        UINT8 VerificationError;            //!< Recovery capsule verification error flag
        UINT8 UpdateNv;                     //!< Update NVRAM flag
        UINT8 UpdateBb;                     //!< Update boot block flag
        UINT8 UpdateMain;                   //!< Update main block flag
        UINT8 PartialUpdateAllowed;         //!< Enable/Disable partial update
} AUTOFLASH;

typedef struct {
	UINT8 SetupFlash;
} AUTOFLASH2;
#pragma pack(pop)
    
#include <Setup.h>

#ifndef VFRCOMPILE

#define FW_VERSION_VARIABLE L"AmiFwVersion"

typedef enum {
    FvTypeMain,
    FvTypeBootBlock,
    FvTypeNvRam,
    FvTypeBootBlockBackup,
    FvTypeCustom,
    FvTypeMax
} FLASH_FV_TYPE;

typedef struct _FLASH_AREA_EX FLASH_AREA_EX;

typedef EFI_STATUS (REFLASH_FUNCTION_EX)(
    IN FLASH_AREA_EX *Block,
    IN UINTN BlockNumber
);

struct _FLASH_AREA_EX
{
    UINT8 *BlockAddress;                //!< Address of the area in flash
    UINT8 *BackUpAddress;               //!< Backup address of the area in flash
    UINT32 RomFileOffset;               //!< Offset of the area in ROM file
    UINTN Size;                         //!< Size of the area
    UINT32 BlockSize;                   //!< Size of the area block
    FLASH_FV_TYPE Type;                 //!< Type of the area
    BOOLEAN Update;                     //!< Update area flag
    BOOLEAN TopSwapTrigger;             //!< TOP_SWAP trigger flag
    REFLASH_FUNCTION_EX *BackUp;        //!< Area backup function
    REFLASH_FUNCTION_EX *Program;       //!< Area flash programm function
    EFI_STRING_ID BackUpStringId;       //!< Area backup message ID
    EFI_STRING_ID ProgramStringId;      //!< Area flash message ID
};

extern FLASH_AREA_EX *BlocksToUpdate;
extern EFI_HII_HANDLE ReflashHiiHandle;
extern UINT8 *RecoveryBuffer;
extern UINT32 RecoveryBufferSize;


typedef struct {
    UINT32 Version;
    UINT32 Status;
} AMI_FW_UPDATE_STATUS;

EFI_STATUS GetHiiString(
    IN     EFI_HII_HANDLE HiiHandle,
    IN     STRING_REF Token,
    IN OUT UINTN *DataSize, 
    OUT    EFI_STRING *Data
);

EFI_STATUS AmiFlashBackUp(
    IN FLASH_AREA_EX *Block,
    IN UINTN BlockNumber
);
EFI_STATUS AmiFlashProgram(
    IN FLASH_AREA_EX *Block,
    IN UINTN BlockNumber
);
EFI_STATUS AmiFlashProgramTopSwap(
    IN FLASH_AREA_EX *Block,
    IN UINTN BlockNumber
);

VOID ApplyUserSelection(
    IN BOOLEAN Interactive
);

VOID OemBeforeFlashCallback(
    VOID
);

VOID OemAfterFlashCallback(
    VOID
);

EFI_STATUS EFIAPI FlashProgressEx(
    IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
    IN EFI_BROWSER_ACTION Action,
    IN EFI_QUESTION_ID KeyValue,
    IN UINT8 Type,
    IN EFI_IFR_TYPE_VALUE *Value,
    OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest
);

BOOLEAN IsWin8Update(
    EFI_STATUS ValidationStatus
);

EFI_STATUS FlashWalker(
    IN BOOLEAN BackUp,
    IN FLASH_PROGRESS *Fp
);

EFI_STATUS Epilogue(
    VOID
);

EFI_STATUS Prologue(
    IN BOOLEAN Interactive,
    IN BOOLEAN Win8StyleUpdate
);

EFI_STATUS InstallEsrtTable(
    VOID
);

EFI_STATUS CreateFlashUpdateArea(
    VOID
);

EFI_STATUS FlashControlFlow(
    IN FLASH_PROGRESS *Fp
);

UINT32 GetFirmwareVersion(
    VOID *FvImage OPTIONAL
);

BOOLEAN CheckRomLayout (UINT32 *State);
VOID UpdateNvram (UINT32 Action);
BOOLEAN IsNvramChanged (VOID);
VOID PreserveFfs (VOID);
EFI_STATUS ValidateImage (VOID);
EFI_STATUS CheckSetupFlashUpdate (VOID);
EFI_STATUS SetupFlashUpdateFlow (VOID);
VOID SelectImageFile (VOID);
EFI_STATUS UpdateImage (VOID *Image);
VOID InitHiiHandle (VOID);
EFI_STATUS ExecuteSmmCall (UINT32 Action);
VOID SetBdsCapsuleLoadCallback (VOID);

#endif
    /****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
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
