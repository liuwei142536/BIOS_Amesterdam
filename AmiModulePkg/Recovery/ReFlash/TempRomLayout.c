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
  This is temporary file to describe ROM layout
  It will be removed once ROM layout wizard is upgraded
**/
#include <Token.h>
#include <AmiRomLayout.h>
#include <Library/AmiRomLayoutLib.h>
#include <Protocol/SmmCommunication.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Guid/AmiNvram.h>
#include <Guid/AmiRomLayout.h>

#include "ReFlash.h"
#include "ReFlashSmm.h"

/*
#ifndef FtRecovery_SUPPORT
#define FtRecovery_SUPPORT 0
#endif

#ifndef BUILD_TIME_BACKUP
#define BUILD_TIME_BACKUP 0
#endif

#ifndef EXTRA_FW_VOLUME
#define EXTRA_FW_VOLUME
#endif

#define AMI_ROM_LAYOUT_FV_BB_GUID \
{ 0x61C0F511, 0xA691, 0x4F54, { 0x97, 0x4F, 0xB9, 0xA4, 0x21, 0x72, 0xCE, 0x53 }}

#define AMI_ROM_LAYOUT_FV_RECOVERY_GUID \
{ 0x764D41AC, 0x282D, 0x4777, { 0xBF, 0x9F, 0xDC, 0x16, 0x71, 0xC1, 0x0F, 0x36 }}

#define AMI_ROM_LAYOUT_FV_MAIN_GUID \
{ 0x5C60F367, 0xA505, 0x419A, { 0x85, 0x9E, 0x2A, 0x4F, 0xF6, 0xCA, 0x6F, 0xE5 }}

#define AMI_ROM_LAYOUT_NVRAM_GUID \
{ 0xfa4974fc, 0xaf1d, 0x4e5d, { 0xbd, 0xc5, 0xda, 0xcd, 0x6d, 0x27, 0xba, 0xec }}

static AMI_ROM_AREA TempRomLayout[] = {
#if FtRecovery_SUPPORT
//FV_BB
    { 
        AMI_ROM_LAYOUT_FV_BB_GUID,
        AMI_ROM_LAYOUT_FV_BB_ADDRESS,
        AMI_ROM_LAYOUT_FV_BB_OFFSET,
        AMI_ROM_LAYOUT_FV_BB_SIZE,
        0,								//AmiRomAreaTypeFv
        AMI_ROM_AREA_SIGNED | AMI_ROM_AREA_VITAL | AMI_ROM_AREA_FV_PEI
    },
//FV_RECOVERY
    { 
        AMI_ROM_LAYOUT_FV_RECOVERY_GUID,
        AMI_ROM_LAYOUT_FV_RECOVERY_ADDRESS,
        AMI_ROM_LAYOUT_FV_RECOVERY_OFFSET,
        AMI_ROM_LAYOUT_FV_RECOVERY_SIZE,
        0,								//AmiRomAreaTypeFv
        AMI_ROM_AREA_SIGNED | AMI_ROM_AREA_VITAL | AMI_ROM_AREA_FV_PEI
    },
#if BUILD_TIME_BACKUP
//FV_BB_BACKUP
    { 
        AMI_ROM_LAYOUT_FV_BB_GUID,
        AMI_ROM_LAYOUT_FV_BB_BACKUP_ADDRESS,
        AMI_ROM_LAYOUT_FV_BB_BACKUP_OFFSET,
        AMI_ROM_LAYOUT_FV_BB_SIZE,
        0,								//AmiRomAreaTypeFv
        AMI_ROM_AREA_SIGNED | AMI_ROM_AREA_VITAL
    },
//FV_RECOVERY_BACKUP
    { 
        AMI_ROM_LAYOUT_FV_RECOVERY_GUID,
        AMI_ROM_LAYOUT_FV_RECOVERY_BACKUP_ADDRESS,
        AMI_ROM_LAYOUT_FV_RECOVERY_BACKUP_OFFSET,
        AMI_ROM_LAYOUT_FV_RECOVERY_SIZE,
        0,								//AmiRomAreaTypeFv
        AMI_ROM_AREA_SIGNED | AMI_ROM_AREA_VITAL
    },
#endif  //BUILD_TIME_BACKUP
#else   //FtRecovery_SUPPORT
//FV_BB
    { 
        AMI_ROM_LAYOUT_FV_BB_GUID,
        AMI_ROM_LAYOUT_FV_BB_ADDRESS,
        AMI_ROM_LAYOUT_FV_BB_OFFSET,
        AMI_ROM_LAYOUT_FV_BB_SIZE,
        0,								//AmiRomAreaTypeFv
        AMI_ROM_AREA_SIGNED | AMI_ROM_AREA_VITAL | AMI_ROM_AREA_FV_PEI
    },
#endif  //FtRecovery_SUPPORT
//FV_MAIN
    { 
        AMI_ROM_LAYOUT_FV_MAIN_GUID,
        AMI_ROM_LAYOUT_FV_MAIN_ADDRESS,
        AMI_ROM_LAYOUT_FV_MAIN_OFFSET,
        AMI_ROM_LAYOUT_FV_MAIN_SIZE,
        0,								//AmiRomAreaTypeFv
        AMI_ROM_AREA_SIGNED | AMI_ROM_AREA_FV_DXE
    },
//NVRAM
    { 
        AMI_ROM_LAYOUT_NVRAM_GUID,
        AMI_ROM_LAYOUT_NVRAM_ADDRESS,
        AMI_ROM_LAYOUT_NVRAM_OFFSET,
        AMI_ROM_LAYOUT_NVRAM_SIZE,
        0,								//AmiRomAreaTypeFv
        AMI_ROM_AREA_SIGNED
    }
//NVRAM_BACKUP
#if FAULT_TOLERANT_NVRAM_UPDATE
    ,{ 
        AMI_ROM_LAYOUT_NVRAM_GUID,
        AMI_ROM_LAYOUT_NVRAM_BACKUP_ADDRESS,
        AMI_ROM_LAYOUT_NVRAM_BACKUP_OFFSET,
        AMI_ROM_LAYOUT_NVRAM_SIZE,
        1,								//AmiRomAreaTypeRaw
        AMI_ROM_AREA_SIGNED
    }
#endif
    EXTRA_FW_VOLUME
};

static UINT32 TempLayoutSize = sizeof (TempRomLayout);
static UINT32 TempAreaSize = sizeof (AMI_ROM_AREA);
*/
static AMI_ROM_AREA *FlashRomLayout = NULL;
static UINT32 FlashRomLayoutSize;
static UINT32 FlashAreaSize;

static AMI_ROM_AREA *ImageRomLayout = NULL;
static UINT32 ImageRomLayoutSize;
static UINT32 ImageAreaSize;

typedef struct {
    VOID *FvAddres;
    UINT32 FfsCount;
    VOID **FfsList;
} AMI_FFS_ARRAY;

typedef struct {
    UINT32 Count;
    AMI_FFS_ARRAY *Array;
} AMI_FFS_LAYOUT;

static AMI_FFS_LAYOUT FlashFfsLayout;
static AMI_FFS_LAYOUT ImageFfsLayout;
extern EFI_GUID* FfsToPreserveGuidList;
extern UINT32 FfsToPreserveGuidListSize;

static BOOLEAN PartialUpdateAllowed = FALSE;


/**
  This function returns current ROM layout
  
  @param Layout     Pointer where to store pointer to ROM layout
  @param AreaSize   Pointer where to store area size
  @param LayoutSize Pointer where to store layout size

  @retval EFI_SUCCESS Layout returned successfully
  @retval other       error occured during execution

**/
EFI_STATUS ImageGetRomLayout (
    AMI_ROM_AREA **Layout,
    UINT32 *AreaSize,
    UINT32 *LayoutSize
    )
{
    EFI_STATUS st = EFI_SUCCESS;
    if (ImageRomLayout == NULL)
        st = AmiGetImageRomLayout (RecoveryBuffer, FLASH_SIZE, &ImageRomLayout, NULL, &ImageAreaSize, &ImageRomLayoutSize);
    *Layout = ImageRomLayout;
    *AreaSize = ImageAreaSize;
    *LayoutSize = ImageRomLayoutSize;
    return st;
}

/**
 * @brief Return area by guid from new firmware image
 *
 * @param guid Pointer to area guid
 *
 * @return Pointer to area
 */
AMI_ROM_AREA * GetImageArea (EFI_GUID *guid)
{
    UINT32 i, c;
    EFI_STATUS st = EFI_SUCCESS;
    AMI_ROM_AREA *r = ImageRomLayout;
    
    if (ImageRomLayout == NULL)
        st = AmiGetImageRomLayout (RecoveryBuffer, FLASH_SIZE, &ImageRomLayout, NULL, &ImageAreaSize, &ImageRomLayoutSize);
    if (EFI_ERROR (st))
        return NULL;
    else
        r = ImageRomLayout;
    
    c = ImageRomLayoutSize / ImageAreaSize;
    for (i = 0; i < c; i++) {
        if (CompareGuid (&r->Guid, guid))
            return r;
        r = (AMI_ROM_AREA *)((UINT8 *)r + ImageAreaSize);
    }
    return NULL;
}

/**
 * @brief Return area by guid from current flash image
 *
 * @param guid Pointer to guid
 *
 * @return Pointer to area
 */
AMI_ROM_AREA * GetFlashArea (EFI_GUID *guid)
{
    UINT32 i, c;
    AMI_ROM_AREA *r = FlashRomLayout;
    
    if (r == NULL)
        return NULL;
    
    c = FlashRomLayoutSize / FlashAreaSize;
    for (i = 0; i < c; i++) {
        if (CompareGuid (&r->Guid, guid))
            return r;
        r = (AMI_ROM_AREA *)((UINT8 *)r + FlashAreaSize);
    }
    return NULL;
}

/**
 * @brief Perform SMM operation
 *
 * @param Action Operation ID
 *
 * @return Operation execution status
 */
EFI_STATUS ExecuteSmmCall (UINT32 Action)
{
    static EFI_GUID HandlerGuid = AMI_REFLASH_SMM_HANDLER_GUID;
    UINTN sz = sizeof (*SmmCommBuffer);
    EFI_STATUS st;
    EFI_SMM_COMMUNICATION_PROTOCOL *cp;
    
    SmmCommBuffer->Handler = HandlerGuid;
    SmmCommBuffer->Size = sizeof (AMI_REFLASH_SMM_COMMUNICATE_HEADER);
    SmmCommBuffer->Header.Action = (UINT64)Action;
    SmmCommBuffer->Header.Status = 0;
    
    st = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **)&cp);
    if (!EFI_ERROR (st)) {
        cp->Communicate (cp, SmmCommBuffer, &sz);
        st = (EFI_STATUS)SmmCommBuffer->Header.Status;
    }
    return st;
}

/**
 * @brief Check firmware image signature and version
 *
 * @return Status of operation
 */
EFI_STATUS ValidateImage (VOID)
{
    SmmCommBuffer->Header.Data[0] = (UINT64)(UINTN)RecoveryBuffer;
    return ExecuteSmmCall (AMI_REFLASH_SMM_VALIDATE_IMAGE);
}

/**
 * @brief Perform NVRAM update
 *
 * @param Action Update type (in place or migrate)
 */
VOID UpdateNvram (UINT32 Action)
{
    AMI_ROM_AREA *nv;
    if (Action > 2)		//Smm Nvram calls supports only 0-2 actions
    	return;
    
    nv = GetImageArea (&gAmiNvramMainRomAreaGuid);
    if (nv == NULL)
        return;
    
    if (Action < 2) {
    	SmmCommBuffer->Header.Data[0] = (UINT64)((UINTN)RecoveryBuffer + nv->Offset);
    	SmmCommBuffer->Header.Data[1] = nv->Size;
    } else {
    	SmmCommBuffer->Header.Data[0] = nv->Address;            //pass new address
    	SmmCommBuffer->Header.Data[2] = nv->Size;
        nv = GetImageArea (&gAmiNvramBackupRomAreaGuid);
        SmmCommBuffer->Header.Data[1] = nv->Address;            //pass new backup address
    }
    ExecuteSmmCall (Action);
}

/**
 * @brief Check if NVRAM is changed
 *
 * @return TRUE if changed, FALSE otherwise
 */
BOOLEAN IsNvramChanged (VOID)
{
    AMI_ROM_AREA *fm, *fb;
    AMI_ROM_AREA *im, *ib;
    BOOLEAN NvramChanged;
    
    fm = GetFlashArea (&gAmiNvramMainRomAreaGuid);
    fb = GetFlashArea (&gAmiNvramBackupRomAreaGuid);
    im = GetImageArea (&gAmiNvramMainRomAreaGuid);
    ib = GetImageArea (&gAmiNvramBackupRomAreaGuid);
    
    if (fm != NULL && fb != NULL &&
        fm->Address == im->Address && 
        fb->Address == ib->Address)
    	NvramChanged = FALSE;       //NVRAM placement is the same, use in-place update
    else
    	NvramChanged = TRUE;        //NVRAM placement is different
    
    if (NvramChanged) {      // We need to update image buffer to preserve NVRAM variables            
        UpdateNvram (AMI_REFLASH_SMM_UPDATE_NVRAM_IN_BUFFER);
    } else {                 //We update NVRAM in place via protocol, so exclude from flash update via FlashWalker
        im->Type = 1;
        im->Attributes &= ~AMI_ROM_AREA_SIGNED;
        ib->Type = 1;
        ib->Attributes &= ~AMI_ROM_AREA_SIGNED;
    }
    return NvramChanged;
}

/**
 * @brief Check ROM layout change
 *
 * @param State Pointer where to return check status (0-no change, 1-unknown, 2-changed)
 *
 * @return TRUE if partial update is allowed, FALSE otherwise
 */
BOOLEAN CheckRomLayout (UINT32 *State)
{
    EFI_STATUS st;
    AMI_ROM_AREA *TmpRomLayout;
    UINT32 RomLayoutState;
    AMI_ROM_LAYOUT_HOB *rlh;
    	
    rlh = (AMI_ROM_LAYOUT_HOB *)GetFirstGuidHob (&gAmiRomLayoutGuid);
    if (rlh->Reserved != 0) { //rom layout was updated from recovery image, do not process it
    	RomLayoutState = 1;
    } else {
        st = AmiGetRomLayout (&FlashRomLayout, NULL, &FlashAreaSize, &FlashRomLayoutSize);
        st = AmiGetImageRomLayout (RecoveryBuffer, FLASH_SIZE, &TmpRomLayout, NULL, &ImageAreaSize, &ImageRomLayoutSize);

        // make a copy of image rom layout to avoid corruption
        ImageRomLayout = (AMI_ROM_AREA *) AllocatePool (ImageRomLayoutSize);
        CopyMem (ImageRomLayout, TmpRomLayout, ImageRomLayoutSize);
        if (FlashRomLayoutSize != ImageRomLayoutSize || 
            CompareMem (FlashRomLayout, ImageRomLayout, ImageRomLayoutSize)) //rom layouts don't match - disable partial update
            RomLayoutState = 2;
        else
            RomLayoutState = 0;
    }
    
    if (State != NULL)
    	*State = RomLayoutState;
    
    PartialUpdateAllowed = (RomLayoutState == 0);
    return PartialUpdateAllowed;
}

/**
 * @brief Check if FFS is valid
 *
 * @param ffs Pointer to FFS
 *
 * @return TRUE - FFS is valid, FALSE otherwise
 */
BOOLEAN IsValidFfs (EFI_FFS_FILE_HEADER *ffs)
{
    UINT64 *p, t;
    
    t = (FLASH_ERASE_POLARITY) ? 0xffffffffffffffff : 0;
    p = (UINT64 *)ffs;
    return (p[0] == t && p[1] == t && p[2] == t) ? FALSE : TRUE;
}

/**
 * @brief Return FFS list from firmware volume
 *
 * @param p Pointer to firmware volume
 * @param  Pointer to store list
 *
 * @return Number of FFS in volume
 */
UINT32 GetFfsList (void *p, void ***out)
{
    static const UINT32 fvs = 0x4856465f;   //_FVH
    void **r;
    UINT32 c = 0;
    EFI_FIRMWARE_VOLUME_EXT_HEADER *fveh;
    EFI_FFS_FILE_HEADER *ffs;
    EFI_FIRMWARE_VOLUME_HEADER *fvh = (EFI_FIRMWARE_VOLUME_HEADER *)p;
    UINT32 fvhs, sfvhs;
    
    if (fvh->Signature != fvs)
        return 0;
    
    fvhs = fvh->HeaderLength;
    if (fvh->ExtHeaderOffset != 0) {
        fveh = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)((UINT8 *)p + fvh->ExtHeaderOffset);
        fvhs = fvh->ExtHeaderOffset + fveh->ExtHeaderSize;
    }
    
    if (fvhs % 8)
        fvhs += 8 - (fvhs % 8);
    sfvhs = fvhs;
    
    while (fvhs < fvh->FvLength) {
        ffs = (EFI_FFS_FILE_HEADER *)((UINT8 *)fvh + fvhs);
        if (!IsValidFfs (ffs))
            break;
        c++;
        fvhs += IS_FFS_FILE2(ffs) ? FFS_FILE2_SIZE(ffs) : FFS_FILE_SIZE(ffs);
        if (fvhs % 8)
            fvhs += 8 - (fvhs % 8);
    }
    r = AllocatePool (sizeof (void *) * c);
    if (r == NULL)
        return 0;
    
    fvhs = sfvhs;
    *out = r;
    while (fvhs < fvh->FvLength) {
        ffs = (EFI_FFS_FILE_HEADER *)((UINT8 *)fvh + fvhs);
        if (!IsValidFfs (ffs))
            break;
        *r++ = (void *)ffs;
        fvhs += IS_FFS_FILE2(ffs) ? FFS_FILE2_SIZE(ffs) : FFS_FILE_SIZE(ffs);
        if (fvhs % 8)
            fvhs += 8 - (fvhs % 8);
    }
    return c;
}

/**
 * @brief Fill FFS layout structure
 *
 * @param Flash Source flag: 1 - existing image, 0 - new image
 */
VOID GetFfsLayout (BOOLEAN Flash)
{
    UINT32 i, c, s;
    AMI_ROM_AREA *r;
    AMI_FFS_LAYOUT *l;
    
    if (Flash) {
        r = FlashRomLayout;
        l = &FlashFfsLayout;
        s = FlashAreaSize;
        c = FlashRomLayoutSize / FlashAreaSize;
    } else {
        r = ImageRomLayout;
        l = &ImageFfsLayout;
        s = ImageAreaSize;
        c = ImageRomLayoutSize / ImageAreaSize;
    }
    
    l->Count = c;
    l->Array = AllocatePool (sizeof (AMI_FFS_ARRAY) * c);
    
    for (i = 0; i < c; i++) {
        l->Array[i].FvAddres = (Flash) ? (VOID *)(r->Address) : (VOID *)(RecoveryBuffer + r->Offset);
        l->Array[i].FfsCount = GetFfsList (l->Array[i].FvAddres, &(l->Array[i].FfsList));
        r = (AMI_ROM_AREA *)((UINT8 *)r + s);
    }
}

/**
 * @brief Return FFS file
 *
 * @param Name FFS guid
 * @param Flash Source flag: 1 - existing image, 0 - new image
 *
 * @return Pointer to FFS file or NULL if not found
 */
VOID * GetFfsFile (EFI_GUID *Name, BOOLEAN Flash)
{
    UINT32 i, j;
    AMI_FFS_LAYOUT *l;
    VOID **v;
    EFI_FFS_FILE_HEADER *f;
    static BOOLEAN FirstRun = TRUE;
    
    if (FirstRun) {
        GetFfsLayout (TRUE);
        GetFfsLayout (FALSE);
        FirstRun = FALSE;
    }
    
    l = (Flash) ? &FlashFfsLayout : &ImageFfsLayout;
    for (i = 0; i < l->Count; i++) {
        for (j = 0, v = l->Array[i].FfsList; j < l->Array[i].FfsCount; j++, v++) {
            f = (EFI_FFS_FILE_HEADER *)*v;
            if (CompareGuid (Name, &f->Name))
                return (VOID *)f;
        }
    }
    return NULL;
}

/**
 * @brief Copy data from existing image to new image for particular FFS file
 *
 * @param Name FFS guid
 */
VOID UpdateImageFfs (EFI_GUID *Name)
{
    EFI_FFS_FILE_HEADER *ff, *fi;
    UINT32 fsz, isz;
    VOID *fd, *id;
    
    fi = GetFfsFile (Name, FALSE);      //get pointer to ffs in image
    ff = GetFfsFile (Name, TRUE);       //get pointer to ffs in flash
    
    if (!ff || !fi)     //one or both not found, nothing to update
        return;
    
    //calculate data size and data pointers
    if (IS_FFS_FILE2(ff)) {
        fsz = FFS_FILE2_SIZE(ff) - sizeof (EFI_FFS_FILE_HEADER2);
        fd = (VOID *)((EFI_FFS_FILE_HEADER2 *)ff + 1);
    } else {
        fsz = FFS_FILE_SIZE(ff) - sizeof (EFI_FFS_FILE_HEADER);
        fd = (VOID *)(ff + 1);
    }
    
    if (IS_FFS_FILE2(fi)) {
        isz = FFS_FILE2_SIZE(fi) - sizeof (EFI_FFS_FILE_HEADER2);
        id = (VOID *)((EFI_FFS_FILE_HEADER2 *)fi + 1);
    } else {
        isz = FFS_FILE_SIZE(fi) - sizeof (EFI_FFS_FILE_HEADER);
        id = (VOID *)(fi + 1);
    }
    
    if (fsz > isz)      //new ffs size is too small to accomodate preservation
        return;
    
    CopyMem (id, fd, fsz);
}

/**
 * @brief Save data from existing image to new image for the list of FFS files
 */
VOID PreserveFfs (VOID)
{
    UINT32 i;
    
    if (!FfsToPreserveGuidListSize || !PartialUpdateAllowed)
        return;
    
    for (i = 0; i < FfsToPreserveGuidListSize; i++)
        UpdateImageFfs (FfsToPreserveGuidList + i);
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
