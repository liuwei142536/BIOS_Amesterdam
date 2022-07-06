//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**        5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093         **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TcgNext/Common/TcgSetup/TcgSetup.c 1     10/08/13 12:05p Fredericko $
//
// $Revision: 1 $
//
// $Date: 10/08/13 12:05p $
//*************************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Modules/TcgNext/Common/TcgSetup/TcgSetup.c $
//
// 1     10/08/13 12:05p Fredericko
// Initial Check-In for Tpm-Next module
//
// 1     7/10/13 5:57p Fredericko
// [TAG]        EIP120969
// [Category]   New Feature
// [Description]    TCG (TPM20)
//
// 2     3/29/11 1:24p Fredericko
//
// [TAG]        EIP 54642
// [Category] Improvement
// [Description] 1. Checkin Files related to TCG function override
// 2. Include TCM and TPM auto detection
// [Files] Affects all TCG files
//
//
//
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  TcgSetup.c
//
// Description:
//
//
//<AMI_FHDR_END>
//**********************************************************************
#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Protocol/AMIPostMgr.h>
#include <AmiTcg/TCGMisc.h>
#include <Setup.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiPackageList.h>
#include <Protocol/HiiString.h>
#include <Library/HiiLib.h>


static EFI_HII_STRING_PROTOCOL *HiiString=NULL;
/**
    This function is eLink'ed with the chain executed right before
    the Setup.

**/
extern EFI_GUID gTcgInternalflagGuid;

static CHAR8* GetSupportedLanguages (IN EFI_HII_HANDLE   HiiHandle);

static CHAR8* InternalTcgGetSupportedLanguages (IN EFI_HII_HANDLE   HiiHandle) {
    EFI_STATUS  Status;
    CHAR8 *SupportedLanguages = NULL;
    UINTN Size = 0;
    
    // Retrieve list of supported languages
    if (HiiHandle == NULL) return NULL;
    if (   !HiiString
        && EFI_ERROR(pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (void **)&HiiString))
    ) return NULL;
    
    Status = HiiString->GetLanguages(HiiString, HiiHandle, SupportedLanguages, &Size);
    if (Status != EFI_BUFFER_TOO_SMALL) return NULL;
    SupportedLanguages = Malloc(Size);
    Status = HiiString->GetLanguages(HiiString, HiiHandle, SupportedLanguages, &Size);
    if (EFI_ERROR(Status)){
        pBS->FreePool(SupportedLanguages);
        return NULL;
    }
    return SupportedLanguages;
}

//updates a string value in the Hii database based by getting the input string and 
// setting the updated string to the input string for all languages
EFI_STATUS InternalUpdateSetupString(EFI_HII_HANDLE HiiHandle, IN STRING_REF InputStringId, 
        IN STRING_REF OutStringId, CHAR8 *SupportedLangs)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN          StingSize = 0x80;
    CHAR16         String[0x80];
    CHAR8          *Supported;
    CHAR8          *Language;
    CHAR8          *AllocatedLanguages;
    
    if(SupportedLangs == NULL)return EFI_INVALID_PARAMETER;
    //AllocatedLanguages = AllocateCopyPool (AsciiStrSize (SupportedLanguages), SupportedLanguages);
    Status = pBS->AllocatePool(EfiBootServicesData, AsciiStrSize (SupportedLangs), (void **)&AllocatedLanguages);
    if(EFI_ERROR(Status))return Status;
    
    pBS->CopyMem(AllocatedLanguages, SupportedLangs, AsciiStrSize (SupportedLangs));
        
    for (Supported = AllocatedLanguages; *Supported != '\0'; ) {
        //
        // Cache a pointer to the beginning of the current language in the list of languages
        //
        Language = Supported;

        //
        // Search for the next language seperator and replace it with a Null-terminator
        //
        for (; *Supported != 0 && *Supported != ';'; Supported++);
        if (*Supported != 0) {
          *(Supported++) = '\0';
        }
        
        if (AsciiStrnCmp (Language, UEFI_CONFIG_LANG, AsciiStrLen (UEFI_CONFIG_LANG)) == 0) {
          //
          // Skip string package used for keyword protocol.
          //
          continue;
        }

        //
        // If StringId is 0, then call NewString().  Otherwise, call SetString()
        //
        Status = HiiString->GetString(HiiString, Language, HiiHandle,InputStringId, String, &StingSize, NULL);
        if(!EFI_ERROR(Status)){
            HiiString->SetString(HiiString, HiiHandle, OutStringId, Language, String, NULL);
          //                Status = HiiString->SetString(HiiString, HiiHandle, *StringId, CurrentLanguage, String, NULL);
            //InitString(HiiHandle,OutStringId, L"%S", String);
        }
        
        if (EFI_ERROR (Status)) {
          break;
        }
    }
    
    pBS->FreePool(AllocatedLanguages);
    return Status;
}


VOID InitTcgStrings(EFI_HII_HANDLE HiiHandle, UINT16 Class)
{
    UINTN          Size = sizeof(AMITCGSETUPINFOFLAGS);
    AMITCGSETUPINFOFLAGS    Info;
    EFI_STATUS     Status;
    UINT32         VariableAttributes=0;
    SETUP_DATA              SetupDataBuffer;
    UINTN                   SetupVariableSize = sizeof(SETUP_DATA);
    UINT32                  SetupVariableAttributes;
    EFI_GUID                gSetupGuid = SETUP_GUID;
    CHAR8          *StringBegin;
    UINTN          len=0;
    BOOLEAN        Comma=FALSE;
    CHAR16         myPtr;
    UINT32         ManufacturerVal;
    UINT32         iter=0;
    CHAR8          *SupportedLangs = NULL;
    UINTN          StingSize2 = 0x80;
    CHAR16         String2[0x80];

    Status = pRS->GetVariable (L"Setup",
                               &gSetupGuid,
                               &SetupVariableAttributes,
                               &SetupVariableSize,
                               &SetupDataBuffer);
    if(EFI_ERROR(Status))return;
    
    if (   !HiiString 
           && EFI_ERROR(pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (void **)&HiiString))
       ) return;

    
    SupportedLangs = InternalTcgGetSupportedLanguages(HiiHandle);
    
    if(SupportedLangs == NULL)return;
    if(SetupDataBuffer.Tpm20Device == 0 && SetupDataBuffer.TpmHrdW == 0)
    {
        //TPM 1.2 or TCM Devices
        if(SetupDataBuffer.TpmEnaDisable == 0)
        {
            InternalUpdateSetupString(HiiHandle, STRING_TOKEN(STR_TPM_ENABLE), 
                    STRING_TOKEN(STR_ENABLED_PROMPT_VALUE), SupportedLangs);
        }
        else
        {
            InternalUpdateSetupString(HiiHandle, STRING_TOKEN(STR_TPM_DISABLE), 
                    STRING_TOKEN(STR_ENABLED_PROMPT_VALUE), SupportedLangs);
        }


        if(SetupDataBuffer.TpmActDeact == 1)
        {
            InternalUpdateSetupString(HiiHandle, STRING_TOKEN(STR_TPMDEACTIVATED), 
                    STRING_TOKEN(STR_ACTIVATED_PROMPT_VALUE), SupportedLangs);
        }
        else
        {
            InternalUpdateSetupString(HiiHandle, STRING_TOKEN(STR_TPMACTIVATED), 
                    STRING_TOKEN(STR_ACTIVATED_PROMPT_VALUE), SupportedLangs);
        }


        if(SetupDataBuffer.TpmOwnedUnowned == 1)
        {
            InternalUpdateSetupString(HiiHandle, STRING_TOKEN(STR_OWNED), 
                    STRING_TOKEN(STR_OWNED_PROMPT_VALUE), SupportedLangs);
        }
        else
        {
            InternalUpdateSetupString(HiiHandle, STRING_TOKEN(STR_UNOWNED), 
                    STRING_TOKEN(STR_OWNED_PROMPT_VALUE), SupportedLangs);
        }
    }

    Status = pRS->GetVariable( L"PCRBitmap", \
                               &gTcgInternalflagGuid, \
                               &VariableAttributes, \
                               &Size, \
                               &Info );
    if(EFI_ERROR(Status))return;
    pBS->SetMem(String2, 0x80, 0);

    StringBegin = (UINT8 *)&String2[0];
    if(Info.SupportedPcrBitMap & 1)
    {
        len = Wcslen(L"SHA-1");
        pBS->CopyMem(StringBegin, L"SHA-1", len*2);
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.SupportedPcrBitMap & 2)
    {
        if(Comma)
        {
            len = Wcslen(L",SHA256");
            pBS->CopyMem(StringBegin, L",SHA256", len*2);
        }
        else
        {
            len = Wcslen(L"SHA256");
            pBS->CopyMem(StringBegin, L"SHA256", len*2);
        }
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.SupportedPcrBitMap & 4)
    {
        if(Comma)
        {
            len = Wcslen(L",SHA384");
            pBS->CopyMem(StringBegin, L",SHA384", len*2);
        }
        else
        {
            len = Wcslen(L"SHA384");
            pBS->CopyMem(StringBegin, L"SHA384", len*2);
        }
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.SupportedPcrBitMap & 0x8)
    {
        if(Comma)
        {
            len = Wcslen(L",SHA512");
            pBS->CopyMem(StringBegin,  L",SHA512", len*2);
        }
        else
        {
            len = Wcslen(L"SHA512");
            pBS->CopyMem(StringBegin,  L"SHA512", len*2);
        }
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.SupportedPcrBitMap & 0x10)
    {
        if(Comma)
        {
            len = Wcslen(L",SM3");
            pBS->CopyMem(StringBegin,  L",SM3", len*2);
        }
        else
        {
            len = Wcslen(L"SM3");
            pBS->CopyMem(StringBegin,  L"SM3", len*2);
        }
        StringBegin += len*2;
    }

    InitString(HiiHandle, STRING_TOKEN(STR_TPM_SUPPORTED_PCR_BANKS_VALUE),
               L"%s", String2);

    pBS->SetMem(String2, 0x80, 0);

    StringBegin = (UINT8 *)&String2[0];
    Comma = FALSE;

    if(Info.ActivePcrBitMap & 1)
    {
        len = Wcslen(L"SHA-1");
        pBS->CopyMem(StringBegin, L"SHA-1", len*2);
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.ActivePcrBitMap & 2)
    {
        if(Comma)
        {
            len = Wcslen(L",SHA256");
            pBS->CopyMem(StringBegin, L",SHA256", len*2);
        }
        else
        {
            len = Wcslen(L"SHA256");
            pBS->CopyMem(StringBegin, L"SHA256", len*2);
        }
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.ActivePcrBitMap & 4)
    {
        if(Comma)
        {
            len = Wcslen(L",SHA384");
            pBS->CopyMem(StringBegin, L",SHA384", len*2);
        }
        else
        {
            len = Wcslen(L"SHA384");
            pBS->CopyMem(StringBegin, L"SHA384", len*2);
        }
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.ActivePcrBitMap & 0x8)
    {
        if(Comma)
        {
            len = Wcslen(L",SHA512");
            pBS->CopyMem(StringBegin, L",SHA512", len*2);
        }
        else
        {
            len = Wcslen(L"SHA512");
            pBS->CopyMem(StringBegin, L"SHA512", len*2);
        }
        StringBegin += len*2;
        Comma = TRUE;
    }

    if(Info.ActivePcrBitMap & 0x10)
    {
        if(Comma)
        {
            len = Wcslen(L",SM3");
            pBS->CopyMem(StringBegin, L",SM3", len*2);
        }
        else
        {
            len = Wcslen(L"SM3");
            pBS->CopyMem(StringBegin, L"SM3", len*2);
        }
    }

    InitString(HiiHandle, STRING_TOKEN(STR_TPM_ACTIVE_PCR_BANKS_VALUE),
               L"%s", String2);
    
    pBS->SetMem(String2, 0x80, 0);
    StringBegin = (CHAR8 *)&String2[0];
        
    InitString(HiiHandle, STRING_TOKEN(STR_TPM_FW_VERSION),L"%d.%d", Info.TpmFwVersion >> 16, Info.TpmFwVersion & 0xFFFF);
    
    pBS->SetMem(String2, 0x80, '\0');
    StringBegin = (CHAR8 *)&String2[0];
    
    ManufacturerVal = Info.TpmManufacturer;
    StringBegin+=6;
        
    while(iter < 4){
        myPtr = (UINT16) ManufacturerVal & 0xFF;
        pBS->CopyMem((CHAR16 *)StringBegin, &myPtr, sizeof(CHAR16)); 
        StringBegin-=2;
        iter+=1;
        ManufacturerVal = ManufacturerVal >> 8;
    } 
    
    InitString(HiiHandle, STRING_TOKEN(STR_TPM_MANUFACTURER),L"%s", String2);
    
    pBS->FreePool (SupportedLangs);

}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093            **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
