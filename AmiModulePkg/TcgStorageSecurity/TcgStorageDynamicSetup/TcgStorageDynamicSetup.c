//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file TcgStorageDynamicSetup.c
    TCG Security Setup Routines
    Use this hook to dynamically change TCG Security VFR data.

**/

#include <Token.h>
#include <AmiDxeLib.h>
// Setup eModule Header
#include <Setup.h>
// For variable access
#include <Guid/GlobalVariable.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
// For HII operations
#include <Library/HiiLib.h>
#include <Protocol/HiiString.h>
#include <Protocol/AMIPostMgr.h>
#include <Guid/MdeModuleHii.h>
#include <Library/AmiHiiUpdateLib.h>
// For Device data
#include <Protocol/PciIo.h>
#include <Protocol/BlockIo.h>
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
#include <Protocol/AmiNvmeController.h>
#endif
#include <TcgStorageSecurity/TcgStorageSecurityCommon.h>
#include <TcgStorageSecurity/TcgStorageSecOPAL.h>
// For this Module
#include "TcgStorageDynamicSetup.h"

#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
EFI_STRING_ID gTcgSecSscTypeStrIds[TCG_SECURITY_MAX_SD_COUNT];
EFI_STRING_ID gTcgSecSupportedStrIds[TCG_SECURITY_MAX_SD_COUNT];
EFI_STRING_ID gTcgSecEnabledStrIds[TCG_SECURITY_MAX_SD_COUNT];
EFI_STRING_ID gTcgSecLockedStrIds[TCG_SECURITY_MAX_SD_COUNT];
EFI_STRING_ID gTcgSecFrozenStrIds[TCG_SECURITY_MAX_SD_COUNT];
EFI_STRING_ID gTcgSecUserPwdStsStrIds[TCG_SECURITY_MAX_SD_COUNT];
EFI_STRING_ID gTcgSecMasterPwdStsStrIds[TCG_SECURITY_MAX_SD_COUNT];
#else
EFI_STRING_ID gTcgSecSscTypeStrIds[1];
EFI_STRING_ID gTcgSecSupportedStrIds[1];
EFI_STRING_ID gTcgSecEnabledStrIds[1];
EFI_STRING_ID gTcgSecLockedStrIds[1];
EFI_STRING_ID gTcgSecFrozenStrIds[1];
EFI_STRING_ID gTcgSecUserPwdStsStrIds[1];
EFI_STRING_ID gTcgSecMasterPwdStsStrIds[1];
#endif

UINTN gTcgSecFrozenBaseKey          = TCG_SECURITY_DYNAMIC_SETUP_SEC_FROZEN_00;
UINTN gTcgSecShowMasterBaseKey      = TCG_SECURITY_DYNAMIC_SETUP_SEC_SHOW_MP_00;
UINTN gTcgSecEnableBaseKey          = TCG_SECURITY_DYNAMIC_SETUP_SEC_ENABLE_00;
UINTN gTcgSecUserPasswordBaseKey    = TCG_SECURITY_DYNAMIC_SETUP_UP_00;
UINTN gTcgSecMasterPasswordBaseKey  = TCG_SECURITY_DYNAMIC_SETUP_MP_00;
#if ENABLE_DEVICE_RESET_THRU_PSID
UINTN gTcgSecPsidBaseKey            = TCG_SECURITY_DYNAMIC_SETUP_PSID_00;
#endif
UINTN gTcgSecGotoBaseKey            = TCG_SECURITY_DYNAMIC_SETUP_GOTO_00;
EFI_VARSTORE_ID     gTcgStorageSecVarstoreId     = TCG_SECURITY_DYNAMIC_VARIABLE_ID;

static EFI_GUID     gTcgSecurityConfigurationGuid = 
                                        TCG_SECURITY_CONFIGURATION_VARIABLE_GUID;
static EFI_GUID     gTcgSecurityLoadSetupDefaultsGuid = 
                                        AMI_TCG_STORAGE_SEC_LOAD_SETUP_DEFAULTS_GUID;
static EFI_GUID     gSecurityFormsetGuid = SECURITY_FORM_SET_GUID;
static EFI_EVENT    gTcgSecurityLoadSetupDefaultsEvent = NULL;
static TCG_STORAGE_SECURITY_INTERNAL_DATA     *gTcgSecurityInternalData = NULL;
static TCG_SECURITY_CONFIGURATION     *gTcgSecurityConfig = NULL;
static EFI_HII_STRING_PROTOCOL        *HiiString = NULL;
static EFI_HII_HANDLE                 gHiiHandle = NULL;
static AMI_POST_MANAGER_PROTOCOL      *mPostMgr = NULL;

static BOOLEAN  AdminPwdFlagToSetUserPwd = FALSE;
static BOOLEAN  gIsEscPressed = FALSE;
static UINT16   gTcgSecurityCount   = 0;
static UINTN    gCurrentDeviceIndex = 0xFFFF;
static CHAR8    *SupportedLanguages = NULL;
VOID            *gEscNotifyRegistration;
EFI_EVENT       gEscNotifyEvent;

// Remove below code when gAmiTcgStorageSecurityProtocolGuid 
// definition is added in AmiModulePkg
#ifndef AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID 
#define AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID \
        { 0xb396da3a, 0x52b2, 0x4cd6, { 0xa8, 0x9a, 0x13, 0xe7, 0xc4, 0xae, 0x97, 0x90 } }
#endif

EFI_GUID gAmiTcgStorageSecurityProtocolGuid = AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID;
EFI_GUID gTseEscOnPwdPromptGuid = AMITSE_ESC_ON_PWD_PROMPT_GUID;
static BOOLEAN  gReadyToBootEventTriggered=FALSE;

#if !(defined (MDE_CPU_IA32) || defined (MDE_CPU_X64))

/**
  Local function defined for the Build except X64, IA32 Architecture
  i.e. GetRandomNumber64() function defined in "RngLib" will be used for X64 and IA32 supported projects.

  if Rand is NULL, then ASSERT().

  @param[out] Rand     Buffer pointer to store the 64-bit random value.

  @retval TRUE         Random number generated successfully.
  @retval FALSE        Failed to generate the random number.

**/
static
BOOLEAN
EFIAPI
GetRandomNumber64 (
  OUT     UINT64                    *Rand
)
{
    if(!Rand) {
       ASSERT(FALSE);
       return FALSE;
    }
    
    *Rand = 0;
    return TRUE;
}

#else

BOOLEAN
EFIAPI
GetRandomNumber64 (
  OUT     UINT64    *Rand
);

#endif

/** @internal
    Function returns TRUE if "AmiTseTcgFreezeStateNotifyGuid" protocol is installed
    at gReadyToBootGuidEvent

    @param VOID

    @retval BOOLEAN
    
    @endinternal

**/
BOOLEAN
IsTcgBlockSidCmdSent()
{
    EFI_STATUS  Status;
    EFI_GUID    AmiTseTcgFreezeStateNotifyGuid = AMI_TSE_TCG_FREEZE_STATE_NOTIFY_GUID;
    VOID        *Protocol=NULL;
    
    if(gReadyToBootEventTriggered) {
        return TRUE;
    }
    
    Status = pBS->LocateProtocol ( &AmiTseTcgFreezeStateNotifyGuid,
                                   NULL,
                                   &Protocol);

    if(!EFI_ERROR(Status)) {
        gReadyToBootEventTriggered = TRUE;
    }

    return gReadyToBootEventTriggered;
}

/** @internal
    Function returns OpalConfig Data of the OPAL device based on the input 
    "DeviceIndex"

    @param DeviceIndex

    @retval Pointer to the OPAL_DEVICE Strcuture
    
    @endinternal

**/
OPAL_DEVICE * 
GetOpalConfigFromDeviceIndex (
    UINTN        DeviceIndex
)
{
    VOID                                  *BusInterface = NULL;
    AMI_STORAGE_BUS_INTERFACE             BusInterfaceType;
    OPAL_DEVICE                           *OpalConfig = NULL;
    TCG_STORAGE_SECURITY_INTERNAL_DATA    *DataPtr             = NULL;
    BOOLEAN                               RstControlled = FALSE;
    
    if ( gTcgSecurityInternalData == NULL ) {
        return 0;
    }

    DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)&gTcgSecurityInternalData[DeviceIndex];
    if ( DataPtr == NULL ) {
        return 0;
    }

    BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )DataPtr->TcgStorageSecProtocol)->BusInterface;
    BusInterfaceType =  ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )DataPtr->TcgStorageSecProtocol)->BusInterfaceType;    

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalConfig = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )
        if( ((NVME_RST_INTERFACE*)BusInterface)->Signature == RST_PWD_SIGNATURE) {
            RstControlled = TRUE;
            OpalConfig = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalConfig = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    return OpalConfig;
    
}

/** @internal
    Locate the Security Protocols and return the information

    @param VOID

    @retval VOID
    
    @endinternal

**/
static
UINT16
InitTcgSecurityInternalDataPtr(
    VOID
)
{
    EFI_STATUS                      Status;
    AMI_TCG_STORAGE_SECURITY_INTERFACE      *Security = NULL;

    Status = pBS->LocateProtocol(
                    &gAmiTcgStorageSecurityProtocolGuid,
                    NULL,
                    (VOID**) &Security);

    if ( !EFI_ERROR( Status )) {
        gTcgSecurityCount = *((UINT16*)Security->PostHddCount);
        gTcgSecurityInternalData = *((TCG_STORAGE_SECURITY_INTERNAL_DATA**)Security->PostHddData);
        return gTcgSecurityCount;
    }

    return 0;
}

/** @internal
    Combines Password1 and Password2 into a Single buffer and returns the Buffer

    @param Password1
    @param Password2

    @retval UINT8*
    
    note: Caller has to Free the Returned Combined Password buffer
    
    @endinternal
**/

UINT8*
CombinePasswordBuffer(
    IN UINT8 *Password1,
    IN UINT8 *Password2
) 
{
    EFI_STATUS Status;
    UINT8      *NewPwdBuffer = NULL;
    UINT8      i,ii;
    UINTN      CombinePwdLen = (OPAL_PASSWORD_LENGTH+1)*2;
    
    //Caller has to clear this buffer
    Status = pBS->AllocatePool( EfiBootServicesData,
                                CombinePwdLen, //allocate double size
                                (VOID**)&NewPwdBuffer );

    pBS->SetMem( NewPwdBuffer, CombinePwdLen , 0 );

    if (!EFI_ERROR(Status)) {
        for ( ii = 0; ii < OPAL_PASSWORD_LENGTH+1; ii++ ) {
            NewPwdBuffer[ii] = Password1[ii];
            if ( !Password1[ii] ) {
                break;
            }
        }
        for ( i=0, ii = OPAL_PASSWORD_LENGTH+1 ; ii < CombinePwdLen; ii++,i++ ) {
            NewPwdBuffer[ii] = Password2[i];
            if ( !Password2[i] ) {
                break;
            }
        }
    }
    return NewPwdBuffer;
}

/** @internal
    This function is wrapper function of pRS->GetVariable() with
    error check.

    @param CHAR16* - Variable name
    @param EFI_GUID - Variable GUID
    @param UINT32* - Variable attribute
    @param UINTN* - Variable size
    @param VOID** - pointer to variable's memory location

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
TcgSecGetEfiVariable(
    IN CHAR16 *sName,
    IN EFI_GUID *pGuid,
    OUT UINT32 *pAttributes OPTIONAL,
    IN OUT UINTN *pDataSize,
    OUT VOID **ppData
)
{
    EFI_STATUS Status;
    if (!*ppData) *pDataSize=0;
    Status = pRS->GetVariable(sName, pGuid, pAttributes, pDataSize, *ppData);
    if (!EFI_ERROR(Status)) return Status;
    if (Status==EFI_BUFFER_TOO_SMALL)
    {
        if (*ppData) pBS->FreePool(*ppData);
        *ppData=Malloc(*pDataSize);
        if (!*ppData) return EFI_OUT_OF_RESOURCES;
        Status = pRS->GetVariable(sName, pGuid, pAttributes, pDataSize, *ppData);
    }
    return Status;
}

/** @internal
    return the String based on StringId

    @param HiiHandle
    @param StringId

    @retval EFI_STRING
    
    @endinternal
**/
EFI_STRING
TcgSecHiiGetString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  EFI_STRING_ID   StringId
)
{
    EFI_STRING String = NULL;
    UINTN StringSize = 0;
    EFI_STATUS Status;
    CHAR8* PlatformLang = NULL;
    UINTN Size = 0;

    if(HiiString == NULL) {
        Status = pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
        if(EFI_ERROR(Status))
            return NULL;
    }

    //language not provided - try platform language
    Status = TcgSecGetEfiVariable(L"PlatformLang", &gEfiGlobalVariableGuid, NULL, &Size, (VOID**)&PlatformLang);
    if(!EFI_ERROR(Status)) {
        Status = HiiString->GetString(HiiString, PlatformLang, HiiHandle, StringId, String, &StringSize, NULL);
        if(Status == EFI_BUFFER_TOO_SMALL) {
            Status = pBS->AllocatePool(EfiBootServicesData, StringSize, (VOID**)&String);
            if(!EFI_ERROR(Status))
                Status = HiiString->GetString(HiiString, PlatformLang, HiiHandle, StringId, String, &StringSize, NULL);
        }
        pBS->FreePool(PlatformLang);
    }
    return (EFI_ERROR(Status)) ? NULL : String;
}

/** @internal
    Add/Set the String to HII Database using HiiString Protocol

    @param HiiHandle
    @param String
    @param StringId

    @retval VOID
    
    @endinternal
**/
EFI_STRING_ID
TcgSecHiiAddStringInternal (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  CHAR16 *        String,
    IN  EFI_STRING_ID   StringId
)
{
    EFI_STATUS      Status;
    CHAR8*          Languages = NULL;
    UINTN           LangSize = 0;
    CHAR8*          CurrentLanguage;
    BOOLEAN         LastLanguage = FALSE;

    if(HiiString == NULL) {
        Status = pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
        if(EFI_ERROR(Status)) {
            return 0;
        }
    }

    if(SupportedLanguages == NULL) {
        Status = HiiString->GetLanguages(HiiString, HiiHandle, Languages, &LangSize);
        if(Status == EFI_BUFFER_TOO_SMALL) {
            Status = pBS->AllocatePool(EfiBootServicesData, LangSize, (VOID**)&Languages);
            if(EFI_ERROR(Status)) {
                //
                //not enough resources to allocate string
                //
                return 0;
            }
            Status = HiiString->GetLanguages(HiiString, HiiHandle, Languages, &LangSize);
            // Return if failed to get list of supported Languages
            if(EFI_ERROR(Status)) {
                return 0;
            }
        }
        
        if(Languages == NULL) {
            return 0;
        }
        
        SupportedLanguages=Languages;
    } else {
        Languages=SupportedLanguages;
    }

    while(!LastLanguage) {
        //
        //point CurrentLanguage to start of new language
        //
        CurrentLanguage = Languages;
        while(*Languages != ';' && *Languages != 0)
            Languages++;

        //
        //Last language in language list
        //
        if(*Languages == 0) {
            LastLanguage = TRUE;
            if(StringId == 0) {
                Status = HiiString->NewString(HiiString, HiiHandle, &StringId, CurrentLanguage, NULL, String, NULL);
            } else {
                Status = HiiString->SetString(HiiString, HiiHandle, StringId, CurrentLanguage, String, NULL);
            }
            if(EFI_ERROR(Status)) {
                return 0;
            }
        } else {
            //
            //put null-terminator
            //
            *Languages = 0;
            if(StringId == 0) {
                Status = HiiString->NewString(HiiString, HiiHandle, &StringId, CurrentLanguage, NULL, String, NULL);
            } else {
                Status = HiiString->SetString(HiiString, HiiHandle, StringId, CurrentLanguage, String, NULL);
            }
            *Languages = ';';       //restore original character
            Languages++;
            if(EFI_ERROR(Status)) {
                return 0;
            }
        }
    }
    return StringId;
}

/** @internal
    Add the String to HII Database using HiiString Protocol

    @param HiiHandle
    @param String

    @retval VOID
    
    @endinternal
**/
EFI_STRING_ID
TcgSecHiiAddString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  CHAR16 *        String
)
{
    return TcgSecHiiAddStringInternal(HiiHandle, String, 0);
}

/** @internal
    Set the String to HII Database using HiiString Protocol

    @param HiiHandle
    @param String
    @param StringId

    @retval VOID

    @endinternal
**/
EFI_STRING_ID
TcgSecHiiSetString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  CHAR16 *        String,
    IN  EFI_STRING_ID   StringId
)
{
    return TcgSecHiiAddStringInternal(HiiHandle, String, StringId);
}


/**
  Create EFI_IFR_PASSWORD_OP opcode.

  If OpCodeHandle is NULL, then ASSERT().
  If any reserved bits are set in QuestionFlags, then ASSERT().

  @param[in]  OpCodeHandle          Handle to the buffer of opcodes.
  @param[in]  QuestionId            Question ID
  @param[in]  VarStoreId            Storage ID
  @param[in]  VarOffset             Offset in Storage or String ID of the name (VarName)
                                    for this name/value pair.
  @param[in]  Prompt                String ID for Prompt
  @param[in]  Help                  String ID for Help
  @param[in]  QuestionFlags         Flags in Question Header
  @param[in]  PasswordFlag          Flags for password opcode
  @param[in]  MinSize               String minimum length
  @param[in]  MaxSize               String maximum length
  @param[in]  DefaultsOpCodeHandle  Handle for a buffer of DEFAULT opcodes.  This
                                    is an optional parameter that may be NULL.

  @retval NULL   There is not enough space left in Buffer to add the opcode.
  @retval Other  A pointer to the created opcode.

**/
static
UINT8 *
EFIAPI
HiiCreatePasswordOpCode (
  IN VOID             *OpCodeHandle,
  IN EFI_QUESTION_ID  QuestionId,
  IN EFI_VARSTORE_ID  VarStoreId,
  IN UINT16           VarOffset,
  IN EFI_STRING_ID    Prompt,
  IN EFI_STRING_ID    Help,
  IN UINT8            QuestionFlags,
  //IN UINT8            PasswordFlag,
  IN UINT8            MinSize,
  IN UINT8            MaxSize,
  IN VOID             *DefaultsOpCodeHandle  OPTIONAL
  )
{
  EFI_IFR_PASSWORD  OpCode;
  UINTN           Position;

  ASSERT ((QuestionFlags & (~(EFI_IFR_FLAG_READ_ONLY | EFI_IFR_FLAG_CALLBACK | EFI_IFR_FLAG_RESET_REQUIRED))) == 0);

  pBS->SetMem (&OpCode, sizeof (OpCode), 0);
  OpCode.Question.Header.Prompt          = Prompt;
  OpCode.Question.Header.Help            = Help;
  OpCode.Question.QuestionId             = QuestionId;
  OpCode.Question.VarStoreId             = VarStoreId;
  OpCode.Question.VarStoreInfo.VarOffset = VarOffset;
  OpCode.Question.Flags                  = QuestionFlags;
  OpCode.MinSize                         = MinSize;
  OpCode.MaxSize                         = MaxSize;
  //OpCode.Flags                           = PasswordFlag;

  if (DefaultsOpCodeHandle == NULL) {
    return InternalHiiCreateOpCodeExtended (OpCodeHandle, &OpCode, EFI_IFR_PASSWORD_OP, sizeof (OpCode), 0, 0);
  }

  Position = InternalHiiOpCodeHandlePosition (OpCodeHandle);
  InternalHiiCreateOpCodeExtended (OpCodeHandle, &OpCode, EFI_IFR_PASSWORD_OP, sizeof (OpCode), 0, 1);
  InternalHiiAppendOpCodes (OpCodeHandle, DefaultsOpCodeHandle);
  HiiCreateEndOpCode (OpCodeHandle);
  return InternalHiiOpCodeHandleBuffer (OpCodeHandle) + Position;
}

/**
  Create EFI_IFR_SUPPRESS_IF_OP/EFI_IFR_GRAY_OUT_IF_OP opcode with
  TRUE/FALSE condition.

  If OpCodeHandle is NULL, then ASSERT().

  @param[in]  OpCodeHandle          Handle to the buffer of opcodes.
  @param[in]  Value                 Condition value - TRUE/FALSE
  @param[in]  Suppress              1: EFI_IFR_SUPPRESS_IF_OP, 0: EFI_IFR_GRAY_OUT_IF_OP

  @retval NULL   There is not enough space left in Buffer to add the opcode.
  @retval Other  A pointer to the created opcode.

**/
static
UINT8 *
EFIAPI
AmiHiiCreateSuppresGrayVal(
  IN VOID               *OpCodeHandle,
  BOOLEAN               Value,
  BOOLEAN               Suppress    //if TRUE Suppress; False Gray out.
  )
{
    EFI_IFR_OP_HEADER   *Condition;
    EFI_IFR_OP_HEADER   *Header;
    UINT8               *Buffer=NULL;
    UINT8               *OpCodeBuffer=NULL;
    UINTN               sz;
//-------------------------------------
    //Allocate buffer for SUPPRESS_IF/GRAY_OUT_IF opcode + EFI_IFR_EQ_ID_VAL for suppress;
    sz=sizeof(EFI_IFR_OP_HEADER)+sizeof(EFI_IFR_OP_HEADER);
    Buffer=AllocateZeroPool(sz);
    if(Buffer==NULL) {
        ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
        return Buffer;
    }

    //Init Pointers;
    Header=(EFI_IFR_OP_HEADER*)Buffer;
    Condition=(EFI_IFR_OP_HEADER*)(Header+1);

    //Update OpCodes...
    //EFI_IFR_SUPPRESS_IF_OP/EFI_IFR_GRAY_OUT_IF_OP first;
    if(Suppress) Header->OpCode=EFI_IFR_SUPPRESS_IF_OP;
    else Header->OpCode=EFI_IFR_GRAY_OUT_IF_OP;
    Header->Length=sizeof(EFI_IFR_OP_HEADER);
    Header->Scope=1;

    //then goes conditions.
    //First update OpCode Header
    if(Value) Condition->OpCode=EFI_IFR_TRUE_OP;
    else Condition->OpCode=EFI_IFR_FALSE_OP;
    Condition->Scope=0;
    Condition->Length=sizeof(EFI_IFR_OP_HEADER);

    //Here we go...
    OpCodeBuffer = HiiCreateRawOpCodes(OpCodeHandle,Buffer,sz);
    FreePool(Buffer);
    return OpCodeBuffer;
}

#if   TCG_STORAGE_SEC_ALL_HDD_SAME_PW == 1
/** @internal
    This function will dynamically add VFR contents to Security BIOS setup page
    using HII library functions.

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
TcgSecInitDynamicSecondChildFormContents(
        VOID
)
{
    VOID                *StartOpCodeHandle;
    VOID                *EndOpCodeHandle;
    VOID                *YesNoOneOfOptionOpCodeHandle;
    EFI_IFR_GUID_LABEL  *StartLabel;
    EFI_IFR_GUID_LABEL  *EndLabel;
    UINTN               DeviceIndex = 0;

    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    EndOpCodeHandle = HiiAllocateOpCodeHandle ();
    YesNoOneOfOptionOpCodeHandle = HiiAllocateOpCodeHandle ();

    // Create Hii Extended Label OpCode as the start and end opcode
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
            StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
            EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    StartLabel->Number = TCG_SECURITY_DYNAMIC_STATUS_START;
    EndLabel->Number = TCG_SECURITY_DYNAMIC_STATUS_END;

 
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_PASSWORD_CONFIGURATION),
            STRING_TOKEN(STR_EMPTY),
            0,0);

    //SEPARATOR
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_EMPTY),
            STRING_TOKEN(STR_EMPTY),
            0,0);
    
    UpdateSSCFeatureCode (DeviceIndex);
	
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_SSC_TYPE),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecSscTypeStrIds[DeviceIndex]
    );
    
    gTcgSecSupportedStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_SUPPORTED),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecSupportedStrIds[DeviceIndex]
    );

    gTcgSecEnabledStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_ENABLED),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecEnabledStrIds[DeviceIndex]
    );

    gTcgSecLockedStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_LOCKED),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecLockedStrIds[DeviceIndex]
    );

    // Suppress by default
    AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, TRUE);
    //Now create OneOf OpCode...
    HiiCreateOneOfOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(gTcgSecFrozenBaseKey + DeviceIndex),
        gTcgStorageSecVarstoreId,
        (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, Frozen[DeviceIndex]),
        STRING_TOKEN(STR_TCGSEC_FROZEN),
        STRING_TOKEN(STR_EMPTY),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        YesNoOneOfOptionOpCodeHandle,
        NULL
    );
    //close scope
    AmiHiiTerminateScope(StartOpCodeHandle);

    // Suppress by default
    AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, TRUE);
    //Now create OneOf OpCode...
    HiiCreateOneOfOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(gTcgSecEnableBaseKey + DeviceIndex),
        gTcgStorageSecVarstoreId,
        (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, Enabled[DeviceIndex]),
        STRING_TOKEN(STR_TCGSEC_ENABLED),
        STRING_TOKEN(STR_EMPTY),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        YesNoOneOfOptionOpCodeHandle,
        NULL
    );
    //close scope
    AmiHiiTerminateScope(StartOpCodeHandle);

    gTcgSecFrozenStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_FROZEN),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecFrozenStrIds[DeviceIndex]
    );

    gTcgSecUserPwdStsStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"NOT INSTALLED");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_USER_PASSWORD_STS),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecUserPwdStsStrIds[DeviceIndex]
    );

    gTcgSecMasterPwdStsStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"NOT INSTALLED");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_ADMIN_PASSWORD_STS),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecMasterPwdStsStrIds[DeviceIndex]
    );

    // Suppress by default
    AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, TRUE);
    //Now create OneOf OpCode...
    HiiCreateOneOfOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(gTcgSecShowMasterBaseKey + DeviceIndex),
        gTcgStorageSecVarstoreId,
        (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, ShowMaster[DeviceIndex]),
        STRING_TOKEN(STR_EMPTY),
        STRING_TOKEN(STR_EMPTY),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        YesNoOneOfOptionOpCodeHandle,
        NULL
    );
    //close scope
    AmiHiiTerminateScope(StartOpCodeHandle);
        
    //We are done!!
    HiiUpdateForm (
      gHiiHandle,
      &gSecurityFormsetGuid,
      TCG_SECURITY_DYNAMIC_STATUS_FORM,
      StartOpCodeHandle,
      EndOpCodeHandle
    );

    
    if (StartOpCodeHandle != NULL)
    {
        HiiFreeOpCodeHandle (StartOpCodeHandle);
        HiiFreeOpCodeHandle (EndOpCodeHandle);
        HiiFreeOpCodeHandle (YesNoOneOfOptionOpCodeHandle);
    }

}
#endif

/** @internal
    This function will dynamically add VFR contents to Security BIOS setup page
    using HII library functions.

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
TcgSecInitDynamicChildFormContents(
        VOID
)
{
    VOID                *StartOpCodeHandle;
    VOID                *EndOpCodeHandle;
    VOID                *YesNoOneOfOptionOpCodeHandle;
    EFI_IFR_GUID_LABEL  *StartLabel;
    EFI_IFR_GUID_LABEL  *EndLabel;
    UINTN               DeviceIndex = 0;
    OPAL_DEVICE         *OpalConfig = NULL;
#if (defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE)  ||   TCG_STORAGE_SEC_ALL_HDD_SAME_PW
    EFI_STRING          HddName = NULL;
    EFI_STRING_ID       HddNameToken = 0;
#endif

    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    EndOpCodeHandle = HiiAllocateOpCodeHandle ();
    YesNoOneOfOptionOpCodeHandle = HiiAllocateOpCodeHandle ();
    OpalConfig = GetOpalConfigFromDeviceIndex(DeviceIndex);

    // Return if failed to allocate memory for new Opcode handle
    if (!StartOpCodeHandle || !EndOpCodeHandle || !YesNoOneOfOptionOpCodeHandle ) {
        return;
    }

    // Create Hii Extended Label OpCode as the start and end opcode
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
            StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

    // Return if failed to create Hii extended Label Opcode
    if (!StartLabel ) {
        return;
    }

    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
            EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

    // Return if failed to create Hii extended Label Opcode
    if (!EndLabel ) {
        return;
    }
    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    StartLabel->Number = TCG_SECURITY_DYNAMIC_ITEM_START;
    EndLabel->Number = TCG_SECURITY_DYNAMIC_ITEM_END;

    HiiCreateOneOfOptionOpCode (
            YesNoOneOfOptionOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_YES),
            (EFI_IFR_OPTION_DEFAULT | EFI_IFR_OPTION_DEFAULT_MFG),
            EFI_IFR_NUMERIC_SIZE_1,
            1 );

    HiiCreateOneOfOptionOpCode (
            YesNoOneOfOptionOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_NO),
            0,
            EFI_IFR_NUMERIC_SIZE_1,
            0 );

    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_PASSWORD_DESCRIPTION),
            STRING_TOKEN(STR_EMPTY),
            0,0);

    // Password description
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_EMPTY),
            STRING_TOKEN(STR_EMPTY),
            0,0);
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_BANNER_ONE),
            STRING_TOKEN(STR_EMPTY),
            0,0);
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_BANNER_TWO),
            STRING_TOKEN(STR_EMPTY),
            0,0);
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_BANNER_THREE),
            STRING_TOKEN(STR_EMPTY),
            0,0);
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_BANNER_FOUR),
            STRING_TOKEN(STR_EMPTY),
            0,0);

    //SEPARATOR
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_EMPTY),
            STRING_TOKEN(STR_EMPTY),
            0,0);
    
#if TCG_STORAGE_SEC_ALL_HDD_SAME_PW == 1
        // Set Admin Password control
        AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecShowMasterBaseKey), 0,FALSE);
        AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecFrozenBaseKey), 1,FALSE);

        HiiCreatePasswordOpCode(
            StartOpCodeHandle,
            (EFI_QUESTION_ID)(gTcgSecMasterPasswordBaseKey),
            gTcgStorageSecVarstoreId,
            (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, OpalPassword[1]),
            STRING_TOKEN(STR_TCGSEC_ADMIN_PASSWORD),
            STRING_TOKEN(STR_TCGSEC_ADMIN_PASSWORD_HELP),
            EFI_IFR_FLAG_CALLBACK | EFI_IFR_FLAG_RESET_REQUIRED,
            0,
            32,
            NULL
        );
        //close scope
        AmiHiiTerminateScope(StartOpCodeHandle);
        AmiHiiTerminateScope(StartOpCodeHandle);

        if(!IS_ENTERPRISE_DEVICE(OpalConfig)) {
            // Set User Password control
            AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecShowMasterBaseKey), 0,FALSE);
            AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecFrozenBaseKey), 1,FALSE);
            AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecEnableBaseKey), 0,FALSE);
        
            HiiCreatePasswordOpCode(
                StartOpCodeHandle,
                (EFI_QUESTION_ID)(gTcgSecUserPasswordBaseKey),
                gTcgStorageSecVarstoreId,
                (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, OpalPassword[0]),
                STRING_TOKEN(STR_TCGSEC_USER_PASSWORD),
                STRING_TOKEN(STR_TCGSEC_USER_PASSWORD_HELP),
                EFI_IFR_FLAG_CALLBACK | EFI_IFR_FLAG_RESET_REQUIRED,
                0,
                32,
                NULL
                );

            //close scope
            AmiHiiTerminateScope(StartOpCodeHandle);
            AmiHiiTerminateScope(StartOpCodeHandle);
            AmiHiiTerminateScope(StartOpCodeHandle);
        }

    //SEPARATOR
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_EMPTY),
            STRING_TOKEN(STR_EMPTY),
            0,0);

    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_SECURITY_CONFIGURATION),
            STRING_TOKEN(STR_EMPTY),
            0,0);

    for(DeviceIndex = 0; DeviceIndex < gTcgSecurityCount && DeviceIndex < TCG_SECURITY_MAX_SD_COUNT; DeviceIndex++) {
        if(gHiiHandle == gTcgSecurityInternalData[DeviceIndex].PostHiiHandle) {
            HddNameToken = gTcgSecurityInternalData[DeviceIndex].PromptToken;
        } else {
            HddName = TcgSecHiiGetString(gTcgSecurityInternalData[DeviceIndex].PostHiiHandle, gTcgSecurityInternalData[DeviceIndex].PromptToken);
            HddNameToken = TcgSecHiiAddString(gHiiHandle, HddName);
            pBS->FreePool(HddName);
        }

        // Add goto control
        HiiCreateGotoOpCode (
            StartOpCodeHandle,
            TCG_SECURITY_DYNAMIC_STATUS_FORM,
            HddNameToken,
            STRING_TOKEN(STR_TCGSEC_SECURITY_ALL_HELP),
            EFI_IFR_FLAG_CALLBACK,
            (EFI_QUESTION_ID)(gTcgSecGotoBaseKey + DeviceIndex)
        );
    }
#else

    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_PASSWORD_CONFIGURATION),
            STRING_TOKEN(STR_EMPTY),
            0,0);

    //SEPARATOR
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_EMPTY),
            STRING_TOKEN(STR_EMPTY),
            0,0);

#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
    for(DeviceIndex = 0; DeviceIndex < gTcgSecurityCount && DeviceIndex < TCG_SECURITY_MAX_SD_COUNT; DeviceIndex++) {
        if(gHiiHandle == gTcgSecurityInternalData[DeviceIndex].PostHiiHandle) {
            HddNameToken = gTcgSecurityInternalData[DeviceIndex].PromptToken;
        } else {
            HddName = TcgSecHiiGetString(gTcgSecurityInternalData[DeviceIndex].PostHiiHandle, gTcgSecurityInternalData[DeviceIndex].PromptToken);
            HddNameToken = TcgSecHiiAddString(gHiiHandle, HddName);
            pBS->FreePool(HddName);
        }

        HiiCreateSubTitleOpCode(StartOpCodeHandle,
                HddNameToken,
                STRING_TOKEN(STR_EMPTY),
                0,0);
#else
        DeviceIndex = 0;
#endif

    gTcgSecSscTypeStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"Unknown");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_SSC_TYPE),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecSscTypeStrIds[DeviceIndex]
    );
#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
    UpdateSSCFeatureCode (DeviceIndex);
#endif
    
    gTcgSecSupportedStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_SUPPORTED),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecSupportedStrIds[DeviceIndex]
    );

    gTcgSecEnabledStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_ENABLED),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecEnabledStrIds[DeviceIndex]
    );

    gTcgSecLockedStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_LOCKED),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecLockedStrIds[DeviceIndex]
    );

    // Suppress by default
    AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, TRUE);
    //Now create OneOf OpCode...
    HiiCreateOneOfOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(gTcgSecFrozenBaseKey + DeviceIndex),
        gTcgStorageSecVarstoreId,
        (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, Frozen[DeviceIndex]),
        STRING_TOKEN(STR_TCGSEC_FROZEN),
        STRING_TOKEN(STR_EMPTY),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        YesNoOneOfOptionOpCodeHandle,
        NULL
    );
    //close scope
    AmiHiiTerminateScope(StartOpCodeHandle);

    // Suppress by default
    AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, TRUE);
    //Now create OneOf OpCode...
    HiiCreateOneOfOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(gTcgSecEnableBaseKey + DeviceIndex),
        gTcgStorageSecVarstoreId,
        (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, Enabled[DeviceIndex]),
        STRING_TOKEN(STR_TCGSEC_ENABLED),
        STRING_TOKEN(STR_EMPTY),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        YesNoOneOfOptionOpCodeHandle,
        NULL
    );
    //close scope
    AmiHiiTerminateScope(StartOpCodeHandle);

    gTcgSecFrozenStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"No");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_FROZEN),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecFrozenStrIds[DeviceIndex]
    );

    if(!IS_ENTERPRISE_DEVICE(OpalConfig)) {
        gTcgSecUserPwdStsStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"NOT INSTALLED");
        HiiCreateTextOpCode(StartOpCodeHandle,
                STRING_TOKEN(STR_TCGSEC_USER_PASSWORD_STS),
                STRING_TOKEN(STR_EMPTY),
                gTcgSecUserPwdStsStrIds[DeviceIndex]
        );
    }
    
    gTcgSecMasterPwdStsStrIds[DeviceIndex] = TcgSecHiiAddString(gHiiHandle, L"NOT INSTALLED");
    HiiCreateTextOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_ADMIN_PASSWORD_STS),
            STRING_TOKEN(STR_EMPTY),
            gTcgSecMasterPwdStsStrIds[DeviceIndex]
    );

    // Suppress by default
    AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, TRUE);
    //Now create OneOf OpCode...
    HiiCreateOneOfOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(gTcgSecShowMasterBaseKey + DeviceIndex),
        gTcgStorageSecVarstoreId,
        (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, ShowMaster[DeviceIndex]),
        STRING_TOKEN(STR_EMPTY),
        STRING_TOKEN(STR_EMPTY),
        0,
        EFI_IFR_NUMERIC_SIZE_1,
        YesNoOneOfOptionOpCodeHandle,
        NULL
    );
    //close scope
    AmiHiiTerminateScope(StartOpCodeHandle);

    //SEPARATOR
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_EMPTY),
            STRING_TOKEN(STR_EMPTY),
            0,0);

    // Set Admin Password control
    AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecShowMasterBaseKey + DeviceIndex), 0,FALSE);
    AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecFrozenBaseKey + DeviceIndex), 1,FALSE);
    HiiCreatePasswordOpCode(
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(gTcgSecMasterPasswordBaseKey + DeviceIndex),
        gTcgStorageSecVarstoreId,
        (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, OpalPassword[1]),
        STRING_TOKEN(STR_TCGSEC_ADMIN_PASSWORD),
        STRING_TOKEN(STR_TCGSEC_ADMIN_PASSWORD_HELP),
        EFI_IFR_FLAG_CALLBACK | EFI_IFR_FLAG_RESET_REQUIRED,
        0,
        OPAL_PASSWORD_LENGTH,
        NULL
    );
    //close scope
    AmiHiiTerminateScope(StartOpCodeHandle); // scope gTcgSecFrozenBaseKey 
    AmiHiiTerminateScope(StartOpCodeHandle); // scope gTcgSecShowMasterBaseKey 
         
    if(!IS_ENTERPRISE_DEVICE(OpalConfig)) {
        // Set User Password control
        AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecShowMasterBaseKey + DeviceIndex), 0,FALSE);
        AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecFrozenBaseKey + DeviceIndex), 1,FALSE);
        AmiHiiCreateSuppresGrayIdVal(StartOpCodeHandle, (EFI_QUESTION_ID)(gTcgSecEnableBaseKey + DeviceIndex), 0,FALSE);
         
        HiiCreatePasswordOpCode(
                 StartOpCodeHandle,
                 (EFI_QUESTION_ID)(gTcgSecUserPasswordBaseKey + DeviceIndex),
                 gTcgStorageSecVarstoreId,
                 (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, OpalPassword[0]),
                 STRING_TOKEN(STR_TCGSEC_USER_PASSWORD),
                 STRING_TOKEN(STR_TCGSEC_USER_PASSWORD_HELP),
                 EFI_IFR_FLAG_CALLBACK | EFI_IFR_FLAG_RESET_REQUIRED,
                 0,
                 OPAL_PASSWORD_LENGTH,
                 NULL
                 );
         
         //close scope
         AmiHiiTerminateScope(StartOpCodeHandle); // scope gTcgSecurityInternalData[DeviceIndex].Enabled
         AmiHiiTerminateScope(StartOpCodeHandle); // scope gTcgSecFrozenBaseKey
         AmiHiiTerminateScope(StartOpCodeHandle); // scope gTcgSecShowMasterBaseKey
    }

#if ENABLE_DEVICE_RESET_THRU_PSID
    // Device Reset string control
    HiiCreateStringOpCode (
            StartOpCodeHandle,
            (EFI_QUESTION_ID)(gTcgSecPsidBaseKey + DeviceIndex),
            gTcgStorageSecVarstoreId,
            (UINT16)EFI_FIELD_OFFSET(TCG_SECURITY_CONFIGURATION, PSID[0]),
            STRING_TOKEN(STR_TCGSEC_PSID_REVERT),
            STRING_TOKEN(STR_TCGSEC_PSID_REVERT_HELP),
            EFI_IFR_FLAG_CALLBACK | EFI_IFR_FLAG_RESET_REQUIRED,
            0,
            0,
            32,
            NULL
        );
#endif


#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
        //SEPARATOR
        HiiCreateSubTitleOpCode(StartOpCodeHandle,
                STRING_TOKEN(STR_EMPTY),
                STRING_TOKEN(STR_EMPTY),
                0,0);
        //SEPARATOR
        HiiCreateSubTitleOpCode(StartOpCodeHandle,
                STRING_TOKEN(STR_EMPTY),
                STRING_TOKEN(STR_EMPTY),
                0,0);
    } // end of FOR loop
#endif
#endif
    //We are done!!
    HiiUpdateForm (
      gHiiHandle,
      &gSecurityFormsetGuid,
      TCG_SECURITY_DYNAMIC_SETUP_FORM,
      StartOpCodeHandle,
      EndOpCodeHandle
    );

    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    HiiFreeOpCodeHandle (YesNoOneOfOptionOpCodeHandle);
}

/** @internal
    This function will dynamically add VFR contents to Security BIOS setup page
    using HII library functions.

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
TcgSecInitDynamicMainFormContents(
        VOID
)
{
    VOID                *StartOpCodeHandle;
    VOID                *EndOpCodeHandle;
    EFI_IFR_GUID_LABEL  *StartLabel;
    EFI_IFR_GUID_LABEL  *EndLabel;
    UINTN               DeviceIndex;
#if !(defined TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) || TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE == 0
    EFI_STRING          HddName = NULL;
    EFI_STRING_ID       HddNameToken = 0;
#endif

    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    EndOpCodeHandle = HiiAllocateOpCodeHandle ();

    // Return if failed to allocate memory for new Opcode handle
    if (!StartOpCodeHandle || !EndOpCodeHandle ) {
        return;
    }

    // Create Hii Extended Label OpCode as the start and end opcode
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
            StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

    // Return if failed to create Hii extended Label Opcode
    if (!StartLabel ) {
        return;
    }
    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
            EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
    
	// Return if failed to create Hii extended Label Opcode
    if (!EndLabel ) {
        return;
    }

    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    StartLabel->Number = TCG_SECURITY_DYNAMIC_GOTO_START;
    EndLabel->Number = TCG_SECURITY_DYNAMIC_GOTO_END;

    //SEPARATOR for Title
//    HiiCreateSubTitleOpCode(StartOpCodeHandle,
//            STRING_TOKEN(STR_EMPTY),
//            STRING_TOKEN(STR_EMPTY),
//            0,0);

#if (defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) || TCG_STORAGE_SEC_ALL_HDD_SAME_PW
    DeviceIndex = 0;
    // Add goto control
    HiiCreateGotoOpCode (
        StartOpCodeHandle,
        TCG_SECURITY_DYNAMIC_SETUP_FORM,
        STRING_TOKEN(STR_TCGSEC_SECURITY_ALL_HELP),
        STRING_TOKEN(STR_TCGSEC_SECURITY_ALL_HELP),
        EFI_IFR_FLAG_CALLBACK,
        (EFI_QUESTION_ID)(gTcgSecGotoBaseKey + DeviceIndex)
    );
#else

    HiiCreateSubTitleOpCode(StartOpCodeHandle,
            STRING_TOKEN(STR_TCGSEC_SECURITY_CONFIGURATION),
            STRING_TOKEN(STR_EMPTY),
            0,0);
    
    for(DeviceIndex = 0; DeviceIndex < gTcgSecurityCount && DeviceIndex < TCG_SECURITY_MAX_SD_COUNT; DeviceIndex++) {
        if(gHiiHandle == gTcgSecurityInternalData[DeviceIndex].PostHiiHandle) {
            HddNameToken = gTcgSecurityInternalData[DeviceIndex].PromptToken;
        } else {
            HddName = TcgSecHiiGetString(gTcgSecurityInternalData[DeviceIndex].PostHiiHandle, gTcgSecurityInternalData[DeviceIndex].PromptToken);
            HddNameToken = TcgSecHiiAddString(gHiiHandle, HddName);
            gBS->FreePool(HddName);
        }

        // Add goto control
        HiiCreateGotoOpCode (
            StartOpCodeHandle,
            TCG_SECURITY_DYNAMIC_SETUP_FORM,
            HddNameToken,
            STRING_TOKEN(STR_TCGSEC_SECURITY_ALL_HELP),
            EFI_IFR_FLAG_CALLBACK,
            (EFI_QUESTION_ID)(gTcgSecGotoBaseKey + DeviceIndex)
        );
    }
#endif

    //We are done!!
    HiiUpdateForm (
      gHiiHandle,
      &gSecurityFormsetGuid,
      SECURITY_MAIN,
      StartOpCodeHandle,
      EndOpCodeHandle
    );
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
    Notification function for "Load Default" action in BIOS setup. This function
    will re-initialize the HDD security setup data.

    @param EFI_EVENT Event - Event to signal
    @param VOID* Context - Event specific context

    @retval VOID

**/
static
VOID
EFIAPI
TcgSecurityLoadSetupDefaults (
        IN EFI_EVENT Event,
        IN VOID *Context
)
{
    TcgSecurityRefreshSetupData(gCurrentDeviceIndex);
}

/**
    This function initializes HDD Security HiiHandle and IFR contents.

    @param  HiiHandle   Handle to HII database
    @param  Class       Indicates the setup class

    @retval VOID

**/

VOID
EFIAPI
InitTcgSecurityDynamicIfrContents (
        EFI_HII_HANDLE  HiiHandle,
        UINT16          Class
)
{
    EFI_STATUS                 Status;
    UINTN         BufferSize = sizeof(UINTN);
    VOID          *NotifyRegistration;
    EFI_EVENT     NotifyEvent;
    EFI_HANDLE    TseTcgNotify;
    EFI_GUID      AmiTseTcgNotifyGuid = AMI_TSE_TCG_NOTIFY_GUID;
       
    gHiiHandle = HiiHandle;
    
    Status = pBS->LocateHandle ( ByProtocol,
                                 &AmiTseTcgNotifyGuid,
                                 NULL,
                                 &BufferSize,
                                 &TseTcgNotify );
       
       if (Status == EFI_SUCCESS) {
           //If HDD post data already initialized then Init Setup Strings here.
           AmiTseTcgNotifyInitString (NULL, NULL);        
       } else if (Status == EFI_NOT_FOUND) {
           //Else register notify for AmiTseTcgNotifyGuid
           Status = pBS->CreateEvent( EFI_EVENT_NOTIFY_SIGNAL,
                                      TPL_CALLBACK,
                                      AmiTseTcgNotifyInitString,
                                      &NotifyRegistration,
                                      &NotifyEvent );
           if(EFI_ERROR(Status)) {
              return;
           }
           
           pBS->RegisterProtocolNotify( &AmiTseTcgNotifyGuid,
                                        NotifyEvent,
                                        &NotifyRegistration );
       }
       
       return;
}

/**
    This Event notifies String initialization for TcgStorageSecurity for can be processed
    
    @param  Event      Event whose notification function is being invoked.
    @param  Context    The pointer to the notification function's context,
                       which is implementation-dependent.

    @retval VOID

**/
VOID 
AmiTseTcgNotifyInitString(
    IN EFI_EVENT Event, 
    IN VOID *Context
)
{
    EFI_STATUS                 Status;
    EFI_STRING                 vname = L"TcgStorageDynamicSetupVar";
    EFI_GUID                   vguid = TCG_SECURITY_CONFIGURATION_VARIABLE_GUID;
    UINTN                      vsize =  sizeof(TCG_SECURITY_CONFIGURATION);
    TCG_SECURITY_CONFIGURATION vdata;

    Status = pRS->SetVariable(vname, &vguid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  vsize, &vdata);
    
    ASSERT_EFI_ERROR(Status);

    // Initialize HDD security internal data structure and gTcgSecurityCount
    gTcgSecurityCount = InitTcgSecurityInternalDataPtr( );
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)
    DEBUG((EFI_D_INFO,"\n InitTcgSecurityDynamicIfrContents gTcgSecurityCount=%d", gTcgSecurityCount));
#endif    
    if( 0 == gTcgSecurityCount) { // No OPAL device detected
        return;
    }

#if TCG_STORAGE_SEC_ALL_HDD_SAME_PW == 1
    TcgSecInitDynamicSecondChildFormContents();
#endif
    TcgSecInitDynamicChildFormContents();
    TcgSecInitDynamicMainFormContents();

    if(gTcgSecurityLoadSetupDefaultsEvent == NULL) {
      Status = pBS->CreateEventEx(
                     EVT_NOTIFY_SIGNAL,
                     TPL_CALLBACK,
                     TcgSecurityLoadSetupDefaults,
                     NULL,
                     &gTcgSecurityLoadSetupDefaultsGuid,
                     &gTcgSecurityLoadSetupDefaultsEvent);
      ASSERT_EFI_ERROR(Status);
    }
}

/** @internal
    Updates the password for the current HDD alone.

    @param UINT32 Index,
    @param CHAR16 *Password,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
BOOLEAN
TcgStorageSecurityUpdateHdd (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    AMI_HDD_SECURITY_PROTOCOL   *TcgStorageSecProtocol = NULL;
    UINT16                      Control              = bCheckUser ? 0 : 1;
    UINT8                       RevisionCode         = 0;
    BOOLEAN                     Locked, Enabled;
    EFI_STATUS                  Status = EFI_UNSUPPORTED;
    UINT8                       Buffer[OPAL_PASSWORD_LENGTH + 1];
    UINT8                       *AdminUserPwdBuffer = NULL;
    #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    UINTN                       ii;
    #endif
    TCG_STORAGE_SECURITY_INTERNAL_DATA  *DataPtr;
    EFI_STRING          StrTitle = NULL;
    EFI_STRING          StrMessage = NULL;
    UINT16              SecurityStatus = 0;
  
    if ( gTcgSecurityInternalData == NULL ) {
        return FALSE;
    }
    
    DataPtr = &gTcgSecurityInternalData[Index];

    TcgStorageSecProtocol = DataPtr->TcgStorageSecProtocol;

    //Get the security status of the device
    Status = TcgStorageSecProtocol->ReturnSecurityStatus( TcgStorageSecProtocol, &SecurityStatus );
    if ( EFI_ERROR( Status )) {
        // Unable to get security status
	    return FALSE; 
    }
	
    // Mask security status based on mask data
    Locked  = ((SecurityStatus & SecurityLockedMask) ? TRUE : FALSE );
    Enabled = ((SecurityStatus & SecurityEnabledMask) ? TRUE : FALSE );
    
    if (!Locked ) {
        if ( Password[0] == 0 ) {
            
            // Security is already disabled, No need to clear again
            if(!Enabled) {
                if (mPostMgr != NULL) {
                    StrTitle = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_WARNING_TITLE));
                    StrMessage = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_PWD_CLEAR_MESSAGE));
                    mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK ,NULL);
                    gBS->FreePool(StrTitle);
                    gBS->FreePool(StrMessage);
                }
                return EFI_SUCCESS;
            }
            
            //empty string is entered -> disable password
            if (mPostMgr == NULL) {
                pBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, (void**)&mPostMgr);
            }
            
            Status = TcgStorageSecProtocol->SecurityDisablePassword(
                TcgStorageSecProtocol,
                Control,
                gTcgSecurityInternalData[Index].PWD );

            // If disable Admin password then OPAL device requires power cycle to set it again.
            // Display warning !!!
            if ( !EFI_ERROR( Status ) && !bCheckUser) {
                if (mPostMgr != NULL) {
                    StrTitle = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_WARNING_TITLE));
                    StrMessage = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_AFTER_RESET_WARNING_MESSAGE));
                    mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK ,NULL);
                    gBS->FreePool(StrTitle);
                    gBS->FreePool(StrMessage);
                }
            }
        } else {

            //set new password
            MemSet( &Buffer, OPAL_PASSWORD_LENGTH + 1, 0 );
            #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW

            for ( ii = 0; ii < OPAL_PASSWORD_LENGTH + 1; ii++ ) {
                Buffer[ii] = (UINT8)Password[ii];

                if ( Password[ii] == L'\0' ) {
                    break;
                }
            }// end of for
            #else
            pBS->CopyMem( Buffer, Password, OPAL_PASSWORD_LENGTH + 1 );
            #endif

            // If Security is enabled, Combine Old Admin(Master) password and New input Password
            if ( Enabled ) {
                // For OPAL - Send saved password + new password
                AdminUserPwdBuffer = CombinePasswordBuffer((UINT8*)&(DataPtr->PWD), (UINT8*)Buffer);
                if (AdminUserPwdBuffer == NULL) {
                    return FALSE;
                }
                Control |= BIT1; // BIT1 indicates password buffer has both old and new password.    
            }
            // End For OPAL

            Status = TcgStorageSecProtocol->SecuritySetPassword(
                TcgStorageSecProtocol,
                Control,
                (Control&BIT1) ? AdminUserPwdBuffer : Buffer,
                RevisionCode );
            
            if ( EFI_ERROR(Status) && (mPostMgr != NULL)) {
                  StrTitle = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_ERROR_WARNING_TITLE));
                  StrMessage = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_SET_PWD_FAIL_ERROR));
                  mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK ,NULL);
                  gBS->FreePool(StrTitle);
                  gBS->FreePool(StrMessage);
            }
            
            if (AdminUserPwdBuffer) {
                pBS->FreePool(AdminUserPwdBuffer);
            }
        }
    }// end if(!Locked)

    if ( EFI_ERROR( Status )) {
        return FALSE;
    }

    return TRUE;
}

/** @internal
    Updates the HDD password for all the HDDs present.

    @param UINT32 Index,
    @param CHAR16 *Password,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
BOOLEAN
TcgStorageSecurityUpdateAllHdd (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    UINTN       i;
    BOOLEAN     Status = FALSE;
        for ( i = 0; i < gTcgSecurityCount; i++ ) {
            Status = TcgStorageSecurityUpdateHdd( (UINT32)i, Password, bCheckUser);
        }
    return Status;
}

/** @internal
    Hook function to update the password for the HDDs based
    on the token TCG_STORAGE_SEC_ALL_HDD_SAME_PW.

    @param  UINT32 Index,
    @param  CHAR16 *Password,
    @param  BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
BOOLEAN
TcgStorageSecurityUpdate (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    #if TCG_STORAGE_SEC_ALL_HDD_SAME_PW
     return TcgStorageSecurityUpdateAllHdd( Index, Password, bCheckUser);
    #else
     return TcgStorageSecurityUpdateHdd( Index, Password, bCheckUser);
    #endif
}

/** @internal
    Validates the password for the current HDD alone.

    @param CHAR16 *Password,
    @param VOID* Ptr,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
TcgStorageSecurityAuthenticateHdd (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    UINT16            Control = 0;
    EFI_STATUS        Status;
    UINT8             Buffer[OPAL_PASSWORD_LENGTH + 1];

    #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    UINTN             i;
    #endif

    TCG_STORAGE_SECURITY_INTERNAL_DATA * DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)Ptr;

    MemSet( &Buffer, OPAL_PASSWORD_LENGTH + 1, 0 );

    #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW

    for ( i = 0; i < OPAL_PASSWORD_LENGTH + 1; i++ ) {
        Buffer[i] = (UINT8)Password[i];

        if ( Password[i] == L'\0' ) {
            break;
        }
    }
    #else
    pBS->CopyMem( Buffer, Password, OPAL_PASSWORD_LENGTH + 1 );
    #endif

    // OpalHdd Authentication is always done with Admin(Master) password
    Control = 1; 

    Status = (DataPtr->TcgStorageSecProtocol)->SecurityUnlockPassword(
        DataPtr->TcgStorageSecProtocol,
        Control,
        Buffer );

    if ( EFI_ERROR( Status )) {
        return EFI_ACCESS_DENIED;
    }

    //save password in case we need to disable it during the setup
    pBS->CopyMem( &(DataPtr->PWD), &Buffer, OPAL_PASSWORD_LENGTH + 1 );
    DataPtr->Locked = FALSE;

    DataPtr->LoggedInAsMaster = TRUE;        

    return EFI_SUCCESS;
}

/** @internal
    Validates the password for all the HDDs Present.

    @param CHAR16 *Password,
    @param VOID* Ptr,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
TcgStorageSecurityAuthenticateAllHdd (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    TCG_STORAGE_SECURITY_INTERNAL_DATA *DataPtr;
    UINTN i;
    EFI_STATUS        Status=EFI_NOT_FOUND;

    //For all drives
    DataPtr = gTcgSecurityInternalData;

    if(DataPtr == NULL) {
        return EFI_NOT_FOUND;
    }

    for ( i = 0; i < gTcgSecurityCount; i++ ) {

        Status = TcgStorageSecurityAuthenticateHdd( Password,
                                             DataPtr,
                                             bCheckUser );
        if ( EFI_SUCCESS != Status ) {
            // Unlock failed.
            LibReportStatusCode( EFI_ERROR_CODE | EFI_ERROR_MAJOR,
                                    DXE_INVALID_IDE_PASSWORD,
                                    0,
                                    NULL,
                                    NULL );
        }
        DataPtr++;
    }
    return Status;
}

/** @internal
    Hook function to validate Password for the HDDs based on
    the token TCG_STORAGE_SEC_ALL_HDD_SAME_PW

    @param CHAR16 *Password,
    @param VOID* Ptr,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
TcgStorageSecurityAuthenticate (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    #if TCG_STORAGE_SEC_ALL_HDD_SAME_PW
     return TcgStorageSecurityAuthenticateAllHdd( Password, Ptr, bCheckUser);
    #else
     return TcgStorageSecurityAuthenticateHdd( Password, Ptr, bCheckUser);
    #endif
}

#if ENABLE_DEVICE_RESET_THRU_PSID
/** @internal
    Reset the device to the Original Factory State using PSID value.

    @param CHAR16 *Password,
    @param VOID* Ptr,

    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
TcgStorageSecurityRevert (
    CHAR16  *Password,
    VOID    *Ptr
)
{
    EFI_STATUS        Status = EFI_INVALID_PARAMETER;
    UINT8             Index = 0;
    UINT8             *Buffer = NULL;
    TCG_STORAGE_SECURITY_INTERNAL_DATA * DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)Ptr;

    EFI_STRING         StrTitle = NULL;
    EFI_STRING         StrMessage = NULL;
    UINTN              Size = 0; 
    
	// Calculating the size of Password
    while(Password[Index] != L'\0'){
        Index++;
    }
    Size = Index;
    if (Size ) {
		Status = pBS->AllocatePool( EfiBootServicesData,
									Size+1,
									(VOID**)&Buffer);
		MemSet(Buffer, Size + 1, 0 );
		
		for (Index = 0; Index < Size; Index++){
            Buffer[Index] = (CHAR8)Password[Index]; 
		}
		
		Status = DataPtr->TcgDeviceResetProtocol->PsidRevert (
													DataPtr->TcgDeviceResetProtocol,
													Buffer); 
		
		//Clear the buffer holding PSID value.
		MemSet( Buffer, Size + 1, 0 );
		
		pBS->FreePool(Buffer);
    }
    if (mPostMgr == NULL) {
        pBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, &mPostMgr);
    }
    
    if ( EFI_ERROR( Status )) {
        //Display the error message
        if (mPostMgr!=NULL) {
            StrTitle = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_ERROR_WARNING_TITLE));
            StrMessage = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_PSID_REVET_ERROR_WARNING_MESSAGE));
            mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK ,NULL);
            gBS->FreePool(StrTitle);
            gBS->FreePool(StrMessage);
        }
        return Status;
    }
    //Display the success message
    if (mPostMgr!=NULL) {
        StrTitle = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_PSID_REVET_SUCCESS_WARNING_TITLE));
        StrMessage = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_PSID_REVET_SUCCESS_WARNING_MESSAGE));
        mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK ,NULL);
        gBS->FreePool(StrTitle);
        gBS->FreePool(StrMessage);
    }
    
    return EFI_SUCCESS;
}
#endif
/** @internal
    Initializes the structure TCG_SECURITY_CONFIGURATION for the current
    HDD if the data pointer to the structure TCG_STORAGE_SECURITY_INTERNAL_DATA is
    initialized already.

    @param TCG_SECURITY_CONFIGURATION *TcgSecurityConfig
    @param  UINTN value

    @retval VOID
    
    @endinternal
**/
VOID
TcgStorageSecUpdateSetupDataByDeviceIndex(
    VOID    *TempTcgSecurityConfig,
    UINTN   value
)
{
    TCG_STORAGE_SECURITY_INTERNAL_DATA  *DataPtr             = NULL;
    AMI_HDD_SECURITY_PROTOCOL           *TcgStorageSecProtocol = NULL;
    UINT32                              IdePasswordFlags = 0;
    EFI_STATUS                          ReturnStatus;
    UINT16                              SecurityStatus = 0;
    
    TCG_SECURITY_CONFIGURATION           *TcgSecurityConfig
        = (TCG_SECURITY_CONFIGURATION*)TempTcgSecurityConfig;

    //Set current HDD security page
    DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)&gTcgSecurityInternalData[value];
#if !defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) || TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE == 0
    value = 0;
#endif

    if ( DataPtr ) {
        TcgStorageSecProtocol = DataPtr->TcgStorageSecProtocol;

        //Get the security status of the device
        ReturnStatus = TcgStorageSecProtocol->ReturnSecurityStatus( TcgStorageSecProtocol, &SecurityStatus );
        if ( EFI_ERROR( ReturnStatus ))
                return ; // Unable to get security status
        
        // Mask security status based on mask data
        TcgSecurityConfig->Supported[value] = ((SecurityStatus & SecuritySupportedMask) ? 1 : 0 );
        TcgSecurityConfig->Enabled[value]   = ((SecurityStatus & SecurityEnabledMask) ? 1 : 0 );
        TcgSecurityConfig->Locked[value]    = ((SecurityStatus & SecurityLockedMask) ? 1 : 0 );
        TcgSecurityConfig->Frozen[value]    = ((SecurityStatus & SecurityFrozenMask) ? 1 : 0 );


        ReturnStatus  = TcgStorageSecProtocol->ReturnIdePasswordFlags(
                                     TcgStorageSecProtocol,
                                     &IdePasswordFlags );

        if ( EFI_ERROR( ReturnStatus )) {
            return;
        }
		
        if (IdePasswordFlags & 0x00000010) {
            TcgSecurityConfig->UserPasswordStatus[value] = 2;
        } else {
            TcgSecurityConfig->UserPasswordStatus[value]
                              = (IdePasswordFlags & 0x00020000) ? 1 : 0;
        }
        TcgSecurityConfig->MasterPasswordStatus[value]
            = (IdePasswordFlags & 0x00010000) ? 1 : 0;

        // Setting "Admin and User Password" is allowed based on ShowMaster field
        // It is disabled if the System booted to Setup after ReadyToBootEvent
        if(IsTcgBlockSidCmdSent()) {
            TcgSecurityConfig->ShowMaster[value] = 0;
        } else {
            TcgSecurityConfig->ShowMaster[value] = 1;
        }
        
        TcgSecHiiSetString(gHiiHandle, TcgSecurityConfig->Supported[value]  ? L"Yes" : L"No", gTcgSecSupportedStrIds[value]);
        TcgSecHiiSetString(gHiiHandle, TcgSecurityConfig->Enabled[value]  ? L"Yes" : L"No", gTcgSecEnabledStrIds[value]);
        TcgSecHiiSetString(gHiiHandle, TcgSecurityConfig->Locked[value]  ? L"Yes" : L"No", gTcgSecLockedStrIds[value]);
        TcgSecHiiSetString(gHiiHandle, TcgSecurityConfig->Frozen[value]  ? L"Yes" : L"No", gTcgSecFrozenStrIds[value]);
        TcgSecHiiSetString(gHiiHandle, (TcgSecurityConfig->UserPasswordStatus[value]  == 0) ? L"NOT INSTALLED" : \
                (TcgSecurityConfig->UserPasswordStatus[value]  == 1) ? L"INSTALLED" : L"UNKNOWN", gTcgSecUserPwdStsStrIds[value]);
        TcgSecHiiSetString(gHiiHandle, TcgSecurityConfig->MasterPasswordStatus[value]  ? L"INSTALLED" : \
                L"NOT INSTALLED", gTcgSecMasterPwdStsStrIds[value]);
    }// end if

    return;
}

/** @internal
    Initializes the structure TCG_SECURITY_CONFIGURATION for all the
    HDDs present if the data pointer to the structure
    TCG_STORAGE_SECURITY_INTERNAL_DATA is initialized already.

    @param TCG_SECURITY_CONFIGURATION *TcgSecurityConfig
    @param  UINTN value

    @retval VOID
    
    @endinternal
**/
VOID
TcgStorageSecUpdateSetupDataForAllHdd(
    VOID    *TcgSecurityConfig,
    UINTN   value
)
{
    UINTN i;

    for ( i = 0; i < gTcgSecurityCount; i++ ) {
        TcgStorageSecUpdateSetupDataByDeviceIndex( TcgSecurityConfig, i);
    }
    return;
}

/** @internal
    Hook function to Initialize the structure TCG_SECURITY_CONFIG
    for the HDDs based on the token TCG_STORAGE_SEC_ALL_HDD_SAME_PW.

    @param TCG_SECURITY_CONFIGURATION *TcgSecurityConfig
    @param  UINTN value

    @retval VOID
    
    @endinternal
**/
VOID
TcgStorageSecUpdateSetupData(
    VOID    *TcgSecurityConfig,
    UINTN   value
)
{

    TcgStorageSecUpdateSetupDataByDeviceIndex( TcgSecurityConfig, value);


}

/** @internal
    function to update the setup configuration page after password update

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
TcgSecurityRefreshSetupData (
        UINTN               DeviceIndex
)
{
    EFI_STATUS          Status;

    // Allocate memory for setup configuration data
    if(NULL == gTcgSecurityConfig) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    sizeof(TCG_SECURITY_CONFIGURATION),
                                    (VOID**)&gTcgSecurityConfig);
        if(EFI_ERROR(Status) || NULL == gTcgSecurityConfig) {
            return;
        }
        pBS->SetMem(gTcgSecurityConfig,sizeof(TCG_SECURITY_CONFIGURATION),0);
    }

    if(DeviceIndex == 0xFFFF) {
        // Update setup data for all OPAL devices
        TcgStorageSecUpdateSetupDataForAllHdd( (VOID*)gTcgSecurityConfig, DeviceIndex );
    } else {
        TcgStorageSecUpdateSetupData( (VOID*)gTcgSecurityConfig, DeviceIndex );
    }

    // Set the new setup configuration data
    pRS->SetVariable(L"TcgStorageDynamicSetupVar",
                    &gTcgSecurityConfigurationGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(TCG_SECURITY_CONFIGURATION),
                    gTcgSecurityConfig);

    // Refresh browser data
    HiiLibSetBrowserData( sizeof(TCG_SECURITY_CONFIGURATION),
                        gTcgSecurityConfig,
                        &gTcgSecurityConfigurationGuid,
                        L"TcgStorageDynamicSetupVar");

}

/**
    Notification function for "Esc pressed" action in BIOS setup.

    @param EFI_EVENT Event - Event to signal
    @param VOID* Context - Event specific context

    @retval VOID

**/
STATIC
VOID 
EFIAPI
NotifyEscPressed(
    IN EFI_EVENT Event, 
    IN VOID *Context
)
{
    gIsEscPressed = TRUE;
    return;
}

/** @internal
    Fills the password buffer with the random number
    generated using GetRandomNumber64()

    @param  Buffer       Pointer to Password buffer.
    @param  size         Size of the buffer.

    @return VOID
    
    @endinternal
**/
STATIC
VOID
FillRandNoInPwdBuffer(
    UINT64 *Buffer, 
    UINT8  size
)
{
    UINT64     RandNo;
    UINT8      i;
    UINT8      *TempBuffer;
    UINT8      Remainder = size % 8;
	
	if(Buffer == NULL || size == 0)
        return;
    // Fill the buffer in multiples of eight as
	// we are getting eight byte random number
    for(i= 0; i < (size/8); i++) {
        GetRandomNumber64(&RandNo);
        *Buffer = RandNo;
        Buffer++;
    }
	// Fill the remaining buffer byte by byte
    if(Remainder) {
        TempBuffer = (UINT8*)Buffer;
        GetRandomNumber64(&RandNo);
        
        while(Remainder) {
          *TempBuffer = (UINT8)RandNo;
          TempBuffer++;
          RandNo = RandNo >> 8;
          Remainder--;
        }
    }
    return;
}

/**
    Prompt User for the confirmation to enable Password for the TCG enterprise device

    @param  DeviceIndex - Index of to be searched in "gTcgSecurityInternalData"
                          through which TCG Enterprise Config details can be known

    @retval BOOLEAN
**/
BOOLEAN
ConfirmToSetPswForEnterpriseDev (
    UINTN       DeviceIndex
)
{
    OPAL_DEVICE     *OpalConfig = NULL;
    CHAR16          *WarnMsg;
    CHAR16          *WarnMsgTitle;
    UINT8           MsgBoxSel=MAX_UINT8;
    
    OpalConfig = GetOpalConfigFromDeviceIndex(DeviceIndex);
    
    if(IS_ENTERPRISE_DEVICE(OpalConfig)) {
        if (mPostMgr == NULL) {
            pBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, (VOID**)&mPostMgr);
        }
        
        if(mPostMgr) {
            // Show warning popup box when Security is not enabled
            WarnMsgTitle = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_WARNING_TITLE));
            
            WarnMsg = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_ENTERPRICE_DEV_SET_PSW_HELP));
                                
            mPostMgr->DisplayMsgBox(WarnMsgTitle,
                                    WarnMsg,
                                    MSGBOX_TYPE_YESNO,
                                    &MsgBoxSel );
            
            gBS->FreePool(WarnMsgTitle);
            gBS->FreePool(WarnMsg);

            // If user selects yes, then procced to Secure Erase
            if(MsgBoxSel != MSGBOX_YES) {
                return FALSE;
            }
        }
    }
    
    return TRUE;
}

/**
    This function handles password operations (check/validate/change/clear)
    for all OPAL password fields

    @param  EFI_HII_HANDLE HiiHandle - HII handle of formset
    @param  UINT16 Class             - class of formset
    @param  UINT16 SubClass          - subclass of formset
    @param  UINT16 Key               - Id of setup control

    @retval EFI_STATUS

    @note
    Case1: Check password status
        EFI_SUCCESS    - No password
        EFI_NOT_FOUND  - Password exists
    Case2: Validate password
        EFI_SUCCESS    - Password is correct
        EFI_NOT_READY  - Password is wrong
    Case3: Set/Clear password
        EFI_SUCCESS    - Operation successful
        EFI_NOT_READY  - Operation failed
**/

EFI_STATUS
TcgSecurityDynamicSetupPasswordCallback(
        EFI_HII_HANDLE HiiHandle,
        UINT16         Class,
        UINT16         SubClass,
        UINT16         Key
)
{
    EFI_STATUS          Status;
    CALLBACK_PARAMETERS *CallbackParameters;
    CHAR16              PasswordBuffer[OPAL_PASSWORD_LENGTH+1];  
    static CHAR16       OldPasswordBuffer[OPAL_PASSWORD_LENGTH+1];
    BOOLEAN             bCheckUser = FALSE;
    BOOLEAN             IsSecurityEnabled = FALSE;
    static BOOLEAN      IsPasswordValidated = FALSE;
    static BOOLEAN      NoPasswordExistedOnFirstCall = FALSE;
    static UINTN        LastHardDiskNumber = 0xFFFF;
    UINTN               CurrentHardDiskNumber;
    UINTN               size;
    VOID*               DataPtr;
    CHAR16              PsidPasswordBuffer[PSID_PASSWORD_LENGTH+1];
    BOOLEAN             PsidRevert = FALSE;
#if ENABLE_DEVICE_RESET_THRU_PSID
    UINT8               MsgBoxSel=MAX_UINT8;
    EFI_STRING          StrTitle = NULL;
    EFI_STRING          StrMessage = NULL;
#endif
    
    // Get actual parameter passed by browser
    CallbackParameters = GetCallbackParameters();

    // Check for supported browser actions
    if (!CallbackParameters || ((CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) 
            && (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) 
            && (CallbackParameters->Action != EFI_BROWSER_ACTION_DEFAULT_STANDARD))) {
        return EFI_UNSUPPORTED;
    }

    // Always return success here
    if ((CallbackParameters->Action == EFI_BROWSER_ACTION_CHANGED) || 
            (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD )) {
                return EFI_SUCCESS;
    }

    // Check which password field was selected
#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
        if(gTcgSecUserPasswordBaseKey <= Key && Key < gTcgSecUserPasswordBaseKey + TCG_SECURITY_MAX_SD_COUNT) {
            bCheckUser = TRUE;
            CurrentHardDiskNumber = Key - gTcgSecUserPasswordBaseKey;
        } else if (gTcgSecMasterPasswordBaseKey <= Key && Key < gTcgSecMasterPasswordBaseKey + TCG_SECURITY_MAX_SD_COUNT) {
            bCheckUser = FALSE;
            CurrentHardDiskNumber = Key - gTcgSecMasterPasswordBaseKey; 
#if ENABLE_DEVICE_RESET_THRU_PSID
        } else if (gTcgSecPsidBaseKey <= Key && Key < gTcgSecPsidBaseKey + TCG_SECURITY_MAX_SD_COUNT) {
            CurrentHardDiskNumber = Key - gTcgSecPsidBaseKey;
            PsidRevert = TRUE;
#endif
        } else {
            CurrentHardDiskNumber = 0xFFFF;
            LastHardDiskNumber = 0xFFFF;
        }
#else
        if(gTcgSecUserPasswordBaseKey == Key) {
            bCheckUser = TRUE;
            CurrentHardDiskNumber = gCurrentDeviceIndex;
        } else if (gTcgSecMasterPasswordBaseKey == Key) {
            bCheckUser = FALSE;
            CurrentHardDiskNumber = gCurrentDeviceIndex;
#if ENABLE_DEVICE_RESET_THRU_PSID
        } else if (gTcgSecPsidBaseKey == Key) {
            CurrentHardDiskNumber = gCurrentDeviceIndex;
            PsidRevert = TRUE;
#endif
        } else {
            CurrentHardDiskNumber = 0xFFFF;
            LastHardDiskNumber = 0xFFFF;
        }
#endif

    // Not a valid OPAL device number
    if ( CurrentHardDiskNumber == 0xFFFF ) {
        return EFI_NOT_FOUND;
    }

    // Get Instance of AmiPostManagerProtocol
    if (mPostMgr == NULL) {
        pBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, (VOID**)&mPostMgr);
    }
    
    // Allocate memory for setup data
    if (NULL == gTcgSecurityConfig) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    sizeof(TCG_SECURITY_CONFIGURATION),
                                    (VOID**)&gTcgSecurityConfig);
        if(EFI_ERROR(Status) || NULL == gTcgSecurityConfig) {
            return EFI_NOT_FOUND;
        }
        pBS->SetMem(gTcgSecurityConfig,sizeof(TCG_SECURITY_CONFIGURATION),0);
    }

    // Get setup/browser data
    size = sizeof(TCG_SECURITY_CONFIGURATION);
    Status = HiiLibGetBrowserData(  &size,
                                    gTcgSecurityConfig,
                                    &gTcgSecurityConfigurationGuid,
                                    L"TcgStorageDynamicSetupVar");
    if (EFI_ERROR(Status)) {
        return EFI_NOT_FOUND;
    }
    
    if(gEscNotifyEvent == NULL) {
        //register notify for gTseEscOnPwdPromptGuid
        Status = pBS->CreateEvent( EFI_EVENT_NOTIFY_SIGNAL,
                                   TPL_CALLBACK,
                                   NotifyEscPressed,
                                   &gEscNotifyRegistration,
                                   &gEscNotifyEvent );
        if(EFI_ERROR(Status)) {
            return Status;
        }
          
        pBS->RegisterProtocolNotify( &gTseEscOnPwdPromptGuid,
                                     gEscNotifyEvent,
                                     &gEscNotifyRegistration );
    }

#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
    IsSecurityEnabled = gTcgSecurityConfig->Enabled[CurrentHardDiskNumber] ? TRUE : FALSE;
#else
    IsSecurityEnabled = gTcgSecurityConfig->Enabled[0] ? TRUE : FALSE;
#endif

    // Get string from HII database using input string token
    if (CallbackParameters->Value->string != 0) {
        if(PsidRevert){
            size = sizeof(PsidPasswordBuffer);  
        } else {
            size = sizeof(PasswordBuffer);
        }    
        Status = HiiLibGetString(   HiiHandle,
                                    CallbackParameters->Value->string,
                                    &size,
                                    PsidRevert ?(EFI_STRING)PsidPasswordBuffer:(EFI_STRING)PasswordBuffer);
        if (EFI_ERROR(Status)) {
            return EFI_NOT_FOUND;
        }
    } else {
        return EFI_NOT_FOUND;
    }
#if ENABLE_DEVICE_RESET_THRU_PSID
    if (PsidRevert) {
        
        // Exit if User entered NULL Password or ESC key
        if(PsidPasswordBuffer[0] == L'\0') {
            return EFI_UNSUPPORTED;
        }

        //Display the error message
        if (mPostMgr!=NULL) {
            StrTitle = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_TCGSEC_WARNING_TITLE));
            StrMessage = TcgSecHiiGetString(gHiiHandle, STRING_TOKEN(STR_PSID_REVERT_WARNING_MESSAGE));
            mPostMgr->DisplayMsgBox(StrTitle,
                                    StrMessage,
                                    MSGBOX_TYPE_YESNO,
                                    &MsgBoxSel );
            
            gBS->FreePool(StrMessage);
            gBS->FreePool(StrTitle);

            // Procced "PSID Revert" only if User selects Yes
            if(MsgBoxSel != MSGBOX_YES) {
                // Avoid displaying PSID value in Setup, by clearing the string
                MemSet( &PsidPasswordBuffer, PSID_PASSWORD_LENGTH + 1, 0 );
                HiiLibSetString(HiiHandle,
                                CallbackParameters->Value->string,
                                (EFI_STRING)PsidPasswordBuffer
                                );
                return EFI_NOT_FOUND;
            }
        }
        
        DataPtr = &gTcgSecurityInternalData[CurrentHardDiskNumber];
        Status = TcgStorageSecurityRevert (PsidPasswordBuffer, DataPtr );
        
        // Avoid displaying PSID value in Setup, by clearing the string
        MemSet( &PsidPasswordBuffer, PSID_PASSWORD_LENGTH + 1, 0 );
        HiiLibSetString(HiiHandle,
                        CallbackParameters->Value->string,
                        (EFI_STRING)PsidPasswordBuffer
                        );
        //Update NVRAM and browser data
#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE && TCG_STORAGE_SEC_ALL_HDD_SAME_PW
        TcgSecurityRefreshSetupData(0xFFFF);
#else
        TcgSecurityRefreshSetupData(CurrentHardDiskNumber);
#endif
        return Status;
    }
#endif
 
    // Just to make sure that two subsequent calls are for same OPAL device
    if (LastHardDiskNumber != 0xFFFF && LastHardDiskNumber != CurrentHardDiskNumber) {
        IsPasswordValidated = FALSE;
        LastHardDiskNumber = 0xFFFF;
        NoPasswordExistedOnFirstCall = FALSE;
        ZeroMem((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
        AdminPwdFlagToSetUserPwd = FALSE;
    }
    
    // Return password status
    if (PasswordBuffer[0] == L'\0' && (IsPasswordValidated == FALSE) && (NoPasswordExistedOnFirstCall == FALSE)) {
        LastHardDiskNumber = CurrentHardDiskNumber;
        // Initializing the OldPasswordBuffer to zeros for handling the callback 
        // when Esc is pressed
        ZeroMem((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
        // If Esc is pressed reset the flag
        gIsEscPressed = FALSE;
        if (IsSecurityEnabled == TRUE) {
            NoPasswordExistedOnFirstCall = FALSE;            
            AdminPwdFlagToSetUserPwd = TRUE;
            return EFI_NOT_FOUND;   // Pre-existing password
        } else {        
            // If TCG Enterprise device, Prompt to confirm setting Password for the Device
            if( bCheckUser==FALSE && !ConfirmToSetPswForEnterpriseDev(CurrentHardDiskNumber) ) {
                return EFI_UNSUPPORTED;
            }
            NoPasswordExistedOnFirstCall = TRUE;
            return EFI_SUCCESS;    // No pre-existing password.
        }
    } else if (IsSecurityEnabled == TRUE && IsPasswordValidated == FALSE) { // Validate password
        DataPtr = &gTcgSecurityInternalData[CurrentHardDiskNumber];

        Status = TcgStorageSecurityAuthenticate( PasswordBuffer, DataPtr, bCheckUser );

        if (EFI_ERROR( Status )) {
            // To make sure OldPasswordBuffer is filled with random number before exiting the function.
            FillRandNoInPwdBuffer((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
            return EFI_NOT_READY;   // Password is wrong.
        } else {
            IsPasswordValidated = TRUE; // Set it so that password could be changed/cleared on next call           
            Wcscpy(OldPasswordBuffer,PasswordBuffer); // Save existing password
            return EFI_SUCCESS;    // Password is correct.
        }
    } else { // Set new password or clear old one
        // Reset static variables
        IsPasswordValidated = FALSE;
        NoPasswordExistedOnFirstCall = FALSE;
        DataPtr = &gTcgSecurityInternalData[CurrentHardDiskNumber];
        
        // If existing password is equals to new password then do nothing
        if((Wcscmp(OldPasswordBuffer,PasswordBuffer)==0) && gIsEscPressed) {
            // Make sure Buffer is filled with random number before exiting the function.
            FillRandNoInPwdBuffer((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
            FillRandNoInPwdBuffer((VOID*)((TCG_STORAGE_SECURITY_INTERNAL_DATA*)DataPtr)->PWD, \
                    sizeof(((TCG_STORAGE_SECURITY_INTERNAL_DATA*)DataPtr)->PWD));
            // If Esc is pressed reset the flag
            gIsEscPressed = FALSE;
            return EFI_SUCCESS;
        }

        FillRandNoInPwdBuffer((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
        // Set/Clear OPAL password
        if(!TcgStorageSecurityUpdate( (UINT32)CurrentHardDiskNumber, PasswordBuffer, bCheckUser )) {
            if(bCheckUser) {
                return EFI_NOT_READY; // Not updated
            }
        }
        // After setting pwd fill the buffer with random number
        FillRandNoInPwdBuffer((VOID*)((TCG_STORAGE_SECURITY_INTERNAL_DATA*)DataPtr)->PWD, \
                sizeof(((TCG_STORAGE_SECURITY_INTERNAL_DATA*)DataPtr)->PWD));
        
        //Update NVRAM and browser data
#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE && TCG_STORAGE_SEC_ALL_HDD_SAME_PW
        TcgSecurityRefreshSetupData(0xFFFF);
#else
        TcgSecurityRefreshSetupData(CurrentHardDiskNumber);
#endif
    }
    return EFI_SUCCESS;
}

/**
    This function will update security status of storage device

    @param  EFI_HII_HANDLE HiiHandle - HII handle of formset
    @param  UINT16 Class             - class of formset
    @param  UINT16 SubClass          - subclass of formset
    @param  UINT16 Key               - Id of setup control

    @retval EFI_SUCCESS      - Operation finished
    @retval EFI_UNSUPPORTED  - Operation not supported on current event

**/
EFI_STATUS
TcgSecurityDynamicSetupUpdateFormCallback(
        EFI_HII_HANDLE HiiHandle,
        UINT16         Class,
        UINT16         SubClass,
        UINT16         Key
)
{
    CALLBACK_PARAMETERS *CallbackParameters;

    // Get actual parameter passed by browser
    CallbackParameters = GetCallbackParameters();
   
    // Check for supported browser actions
    if(!CallbackParameters || ((CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) && \
            (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) && (CallbackParameters->Action != EFI_BROWSER_ACTION_RETRIEVE)) )
        return EFI_UNSUPPORTED;

    // Always return success here
    if((CallbackParameters->Action == EFI_BROWSER_ACTION_CHANGED)) {
        return EFI_SUCCESS;
    }
    // Initialize HDD security internal data structure and gTcgSecurityCount
    gTcgSecurityCount = InitTcgSecurityInternalDataPtr( );
    
    // Check which OPAL device was selected
#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
    gCurrentDeviceIndex = 0xFFFF;
#else
    if(gTcgSecGotoBaseKey <= Key && Key < gTcgSecGotoBaseKey + TCG_SECURITY_MAX_SD_COUNT) {
        gCurrentDeviceIndex = Key - gTcgSecGotoBaseKey;
        UpdateSSCFeatureCode(gCurrentDeviceIndex);
    } else {
        gCurrentDeviceIndex = 0xFFFF;
    }
#endif

    //Update NVRAM and browser data
    TcgSecurityRefreshSetupData(gCurrentDeviceIndex);

    return EFI_SUCCESS;
}

/**@internal
    Function to update the SSC feature code in setup configuration page.

    @param UINTN       DeviceIndex

    @retval VOID
   
   @endinternal
**/
VOID 
UpdateSSCFeatureCode (
  UINTN        DeviceIndex
) {
	
    VOID                                  *BusInterface = NULL;
    AMI_STORAGE_BUS_INTERFACE             BusInterfaceType;
    OPAL_DEVICE                           *OpalConfig = NULL;
    TCG_STORAGE_SECURITY_INTERNAL_DATA    *DataPtr             = NULL;
    BOOLEAN                               RstControlled = FALSE;
    
    if ( gTcgSecurityInternalData == NULL ) {
        ASSERT(FALSE);
        return;
    }

    DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)&gTcgSecurityInternalData[DeviceIndex];
    
    BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )DataPtr->TcgStorageSecProtocol)->BusInterface;
    BusInterfaceType =  ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )DataPtr->TcgStorageSecProtocol)->BusInterfaceType;    

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalConfig = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif
    
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )
        if( ((NVME_RST_INTERFACE*)BusInterface)->Signature == RST_PWD_SIGNATURE) {
            RstControlled = TRUE;
            OpalConfig = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalConfig = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
#if !defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) || TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE == 0
    DeviceIndex = 0;
#endif

    if(NULL == OpalConfig) {
        ASSERT(FALSE);
        return;
    }
    
    if (OpalConfig->OpalFeature.FeatureCode == OPAL_SSC_V200_Feature) {
        TcgSecHiiSetString(gHiiHandle, L"Opal", gTcgSecSscTypeStrIds[DeviceIndex]);   
    } else if(OpalConfig->OpalFeature.FeatureCode == OPALITE_SSC_Feature) {
        TcgSecHiiSetString(gHiiHandle, L"Opalite", gTcgSecSscTypeStrIds[DeviceIndex]);   
    } else if(OpalConfig->OpalFeature.FeatureCode == PYRITE_SSC_Feature) {
        TcgSecHiiSetString(gHiiHandle, L"Pyrite 1.0", gTcgSecSscTypeStrIds[DeviceIndex]);  
    } else if(OpalConfig->OpalFeature.FeatureCode == PYRITE_SSC_Feature_v2_0) {
        TcgSecHiiSetString(gHiiHandle, L"Pyrite 2.0", gTcgSecSscTypeStrIds[DeviceIndex]);
    } else if(OpalConfig->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature) {
        TcgSecHiiSetString(gHiiHandle, L"Enterprise", gTcgSecSscTypeStrIds[DeviceIndex]);  
    } else if(OpalConfig->OpalFeature.FeatureCode == RUBY_SSC_Feature) {
        TcgSecHiiSetString(gHiiHandle, L"Ruby", gTcgSecSscTypeStrIds[DeviceIndex]);  
    }

    return;
}

