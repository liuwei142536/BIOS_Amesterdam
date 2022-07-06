//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file
  Common file for elinks to ReFlash nodule..
  
  FlashSmiElinks will hook GetVariable() before recovery process to avoid 
  garbage collection while flash nvram.
  
**/

//----------------------------------------------------------------------------
// Includes
// Statements that include other files
#include <AmiDxeLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

//----------------------------------------------------------------------------
// Function Externs

//----------------------------------------------------------------------------
// Local prototypes

//----------------------------------------------------------------------------
// Local Variables
EFI_GET_VARIABLE gSavedGetVar = NULL;

UINT32  gLangAttr = 0;
UINTN   gLangSize = 0;
VOID    *gLangBuff = NULL;

/**
  Dummy GetVariable function, will return preserved PlatformLang variable.

  @param VariableName
  @param VendorGuid
  @param Attributes
  @param DataSize
  @param Data

  @return EFI_STATUS
**/
EFI_STATUS
EFIAPI
DummyGetVariable(
    IN      CHAR16      *VariableName,
    IN      EFI_GUID    *VendorGuid,
    OUT     UINT32      *Attributes OPTIONAL,
    IN  OUT UINTN       *DataSize,
    OUT     VOID        *Data
)
{
    if( (StrCmp( VariableName, L"PlatformLang" ) == 0) &&
        CompareGuid( VendorGuid, &gEfiGlobalVariableGuid ) )
    {
        if( (DataSize != NULL) && (*DataSize < gLangSize) )
            return EFI_BUFFER_TOO_SMALL;

        if( gLangBuff != NULL )
        {
            if( Attributes != NULL )
                *Attributes = gLangAttr;
            if( DataSize != NULL )
                *DataSize = gLangSize;
            if( Data != NULL )
                CopyMem( Data, gLangBuff, *DataSize );

            return EFI_SUCCESS;
        }

        return EFI_NOT_FOUND;
    }

    return EFI_DEVICE_ERROR;
}
/**
  This function will be called in Prologue() which is before starting
  recovery process.
  Will preserve PlatformLang variable and hook GetVaraible().

  @param VOID

  @return VOID
**/
VOID
FlashSmiPrologueHook(
    VOID
)
{
    EFI_STATUS  Status;
    UINTN       VarSize;
    UINT32      VarAttr;
    VOID        *VarBuff;

    Status = pRS->GetVariable(
                    L"PlatformLang",
                    &gEfiGlobalVariableGuid,
                    &VarAttr,
                    &VarSize,
                    NULL );
    if( Status == EFI_BUFFER_TOO_SMALL )
    {
        Status = pBS->AllocatePool(
                        EfiBootServicesData,
                        VarSize,
                        &VarBuff );
        if( !EFI_ERROR(Status) )
        {
            Status = pRS->GetVariable(
                            L"PlatformLang",
                            &gEfiGlobalVariableGuid,
                            &VarAttr,
                            &VarSize,
                            VarBuff );
            if( !EFI_ERROR(Status) )
            {
                gLangAttr = VarAttr;
                gLangSize = VarSize;
                gLangBuff = VarBuff;
            }
        }
    }

    gSavedGetVar = pRS->GetVariable;
    pRS->GetVariable = DummyGetVariable;
}
/**
  This function will be called in Epilogue() which is after recovery process.
  Will retrun original GetVaraible().

  @param VOID

  @return VOID
**/
VOID
FlashSmiEpilogueHook(
    VOID
)
{
    if( gLangBuff != NULL )
    {
        pBS->FreePool( gLangBuff );
        gLangBuff = NULL;
    }

    pRS->GetVariable = gSavedGetVar;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
