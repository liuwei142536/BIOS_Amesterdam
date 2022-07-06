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
// $Header: /Alaska/SOURCE/Modules/TcgNext/Common/AmiTcgPlatform/AmiTcgPlatformPeiBeforeMem.c 1     10/08/13 12:04p Fredericko $
//
// $Revision: 1 $
//
// $Date: 10/08/13 12:04p $
//*************************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Modules/TcgNext/Common/AmiTcgPlatform/AmiTcgPlatformPeiBeforeMem.c $
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:    AmiTcgPlatformPeiBeforeMem.c
//
// Description: Function file for AmiTcgPlatformPeiBeforeMem
//
//<AMI_FHDR_END>
//*************************************************************************
#include <Efi.h>
#include <Token.h>
#include <AmiTcg/TcgCommon20.h>
#include "AmiTcg/sha.h"
#include <AmiTcg/TCGMisc.h>
#include <AmiTcg/TpmLib.h>
#include "Ppi/TcgService.h"
#include "Ppi/TpmDevice.h"
#include <Library/DebugLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/Reset2.h>
#include <Ppi/AmiTcgPlatformPpi.h>
#include <AmiTcg/AmiTcgPlatformPei.h>
#include <AmiTcg/Tpm20Pei.h>
#include <Library/BaseMemoryLib.h>
#include "Ppi/TcgPlatformSetupPeiPolicy.h"
#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
#include <Library/AmiRomLayoutLib.h>
#endif
#include <Ppi/EndOfPeiPhase.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <ImageAuthentication.h>
#include <Library/IoLib.h>
#include <ImageAuthentication.h>
#include <Library/UefiLib.h>
#include <AmiTcg/AmiTpmStatusCodes.h>
#include <Guid/AmiTcgGuidIncludes.h>
#include <Ppi/AmiTcgPpis.h>
#if defined (AmiTrustedFv_Support) && (AmiTrustedFv_Support == 1)
#include <Ppi/AmiFvDigest.h>
#endif
#include<Library/TimerLib.h>

// NIST 800-155
#include <Guid/HobList.h>
#include <AmiHobs.h>
#include <RomLayout.h>
// NIST 800-155


extern EFI_GUID gAmiGlobalVariableGuid;
extern EFI_GUID gTcgPeiPolicyGuid;

//GUID Externs
#define TCG2_PROTOCOL_SPEC_TCG_1_2  0x01
#define TCG2_PROTOCOL_SPEC_TCG_2    0x02

BOOLEAN CrbSupported();
UINT8 GetCurrentInterfaceType();
void printbuffer(UINT8 *Buffer, UINTN BufferSize);
EFI_STATUS FindAndMeasureDxeCore(CONST EFI_PEI_SERVICES     **PeiServices);

#pragma pack(push,1)

typedef struct
{
    TPM2_COMMAND_HEADER       Header;
    TPM_CAP                   Capability;
    UINT32                    Property;
    UINT32                    PropertyCount;
} TPM2_GET_CAPABILITY_COMMAND;

typedef struct
{
    TPM2_RESPONSE_HEADER      Header;
    TPMI_YES_NO               MoreData;
    TPMS_CAPABILITY_DATA      CapabilityData;
} TPM2_GET_CAPABILITY_RESPONSE;

typedef struct
{
    TPM2_COMMAND_HEADER     Header;
    UINT16                  bytesRequested;
} TPM2_GetRandom_COMMAND;

typedef struct
{
    TPM2_RESPONSE_HEADER      Header;
    TPM2B_DIGEST              randomBytes;
} TPM2_GetRandom_RESPONSE;

typedef struct
{
    TPM2_COMMAND_HEADER       Header;
    TPMI_RH_HIERARCHY_AUTH    AuthHandle;
    UINT32                    AuthorizationSize;
    TPMS_AUTH_COMMAND         AuthSession;
    TPM2B_AUTH                NewAuth;
} TPM2_HIERARCHY_CHANGE_AUTH_COMMAND;

typedef struct
{
    TPM2_RESPONSE_HEADER       Header;
    UINT32                     ParameterSize;
    TPMS_AUTH_RESPONSE         AuthSession;
} TPM2_HIERARCHY_CHANGE_AUTH_RESPONSE;

#pragma pack(pop)

//*** AMI PORTING BEGIN *****//
typedef struct _ACPI_RESET_STRUCTURE
{
    UINT64          Addess;
    UINT8           Type;
    UINT8           AccessSize;
}ACPI_RESET_STRUCTURE;
//*** AMI PORTING END *****//

EFI_STATUS MeasureTCGPcClientSpecID(
    IN CONST EFI_PEI_SERVICES **PeiServices);

EFI_STATUS MeasureNISTManifest(
    IN CONST EFI_PEI_SERVICES **PeiServices);
UINT8
EFIAPI FirstBootCheck(
    IN EFI_PEI_SERVICES **PeiServices);

EFI_STATUS
Tpm20MeasureCRTMVersion(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    AMI_TREE_PPI *TrEEPeiPpi);

EFI_STATUS 
EFIAPI
TpmPeiReportStatusCode(  IN EFI_STATUS_CODE_TYPE   Type,
                         IN EFI_STATUS_CODE_VALUE  Value);

#if FTpmPlatformProfile == 1
#define AMD_FTPM_PPI_GUID \
            {0x91774185, 0xf72d, 0x467e, {0x93, 0x39, 0xe0, 0x8, 0xdb, 0xae, 0xe, 0x14}}
EFI_GUID giTpmPpiGuid    = AMD_FTPM_PPI_GUID;
EFI_STATUS iTpmGetInfo (
    IN OUT UINTN  *iTPMStatus
);
#endif

extern TPM20_MEASURE_CRTM_VERSION_PEI_FUNC_PTR  TPM20_MEASURE_CRTM_VERSION_PEI_FUNCTION;
TPM20_MEASURE_CRTM_VERSION_PEI_FUNC_PTR *Tpm20MeasureCRTMVersionFuncPtr = TPM20_MEASURE_CRTM_VERSION_PEI_FUNCTION;

EFI_STATUS dTPMCrbSetReqReadyState( VOID* );
BOOLEAN isTpm20CrbPresent();

#if defined (MDE_PKG_VERSION) && (MDE_PKG_VERSION > 9)
#ifndef TPM2_ALG_SHA1
#define TPM2_ALG_SHA1 TPM_ALG_SHA1
#endif
#ifndef TPM2_ALG_SHA256
#define TPM2_ALG_SHA256 TPM_ALG_SHA256
#endif
#ifndef TPM2_ALG_SHA384
#define TPM2_ALG_SHA384 TPM_ALG_SHA384
#endif
#ifndef TPM2_ALG_SHA512
#define TPM2_ALG_SHA512 TPM_ALG_SHA512
#endif
#ifndef TPM2_ALG_SM3_256
#define TPM2_ALG_SM3_256 TPM_ALG_SM3_256
#endif
#ifndef TPM2_ALG_ID
#define TPM2_ALG_ID TPM_ALG_ID
#endif
#endif

#define SHA1_DIGEST_SIZE        20
#define SHA256_DIGEST_SIZE      32
#define SM3_256_DIGEST_SIZE     32
#define SHA384_DIGEST_SIZE      48
#define SHA512_DIGEST_SIZE      64

//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:  CapAllTpm2Pcrs
//
// Description: Caps all TPM PCRS in recovery mde
//
//
// Input:       PeiServices
//
// Output:      
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
CapAllTpm2Pcrs(IN CONST EFI_PEI_SERVICES    **PeiServices,
               IN AMI_TREE_PPI      *TrEEPeiPpi,
               IN UINT32            InputEventData)
{
    UINT32      Index = 0;
    EFI_STATUS  Status;
    UINT32      EventData;
    UINT64           Flags = 0;
    AMI_TREE_EVENT   *Tpm20Event=NULL;
    
    if(PeiServices == NULL || *PeiServices == NULL \
            || TrEEPeiPpi == NULL){
        
        return EFI_INVALID_PARAMETER;
    }
    
    Status = (*PeiServices)->AllocatePool(PeiServices, (sizeof(AMI_TREE_EVENT_HEADER) + \
                                               sizeof(UINT32) + sizeof(UINT32)),  (void **)&Tpm20Event);

    if(Tpm20Event==NULL || EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
  
    for(Index = 0; Index < 8; Index ++){
        
        EventData = InputEventData;
        Tpm20Event->Size  = sizeof(AMI_TREE_EVENT_HEADER) + sizeof(UINT32) + sizeof(EventData);
        Tpm20Event->Header.HeaderSize = sizeof(AMI_TREE_EVENT_HEADER);
        Tpm20Event->Header.HeaderVersion = 1;
        Tpm20Event->Header.PcrIndex    = Index;
        Tpm20Event->Header.EventType   = EV_SEPARATOR;
    
        (*PeiServices)->CopyMem ((UINT32 *)((UINTN)&Tpm20Event->Event[0]),
                      &EventData,
                      sizeof(UINT32));
        
        Status = TrEEPeiPpi->HashLogExtendEvent(TrEEPeiPpi,
                     Flags, (EFI_PHYSICAL_ADDRESS)(UINTN)&EventData, (UINT64)sizeof(EventData),
                     Tpm20Event);

        if(EFI_ERROR (Status)){
            break;
        }
    }
    
    return Status;
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:  AmiTpmPeiCopyAuthSessionCommand
//
// Description:
//
//
// Input:       UINT8  AuthSessionIn
//
// Output:      TPMS_AUTH_RESPONSE* AuthSessionOut
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
UINT32
EFIAPI
AmiTpmPeiCopyAuthSessionCommand (
    IN      TPMS_AUTH_COMMAND         *AuthSessionIn, OPTIONAL
    OUT     UINT8                             *AuthSessionOut
)
{
    UINT8  *Buffer;

    Buffer = (UINT8 *)AuthSessionOut;

    //
    // Add in Auth session
    //
    if (AuthSessionIn != NULL)
    {
        //  sessionHandle
        WriteUnaligned32 ((UINT32 *)Buffer, SwapBytes32(AuthSessionIn->sessionHandle));
        Buffer += sizeof(UINT32);

        // nonce
        WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16 (AuthSessionIn->nonce.size));
        Buffer += sizeof(UINT16);

        CopyMem (Buffer, AuthSessionIn->nonce.buffer, AuthSessionIn->nonce.size);
        Buffer += AuthSessionIn->nonce.size;

        // sessionAttributes
        *(UINT8 *)Buffer = *(UINT8 *)&AuthSessionIn->sessionAttributes;
        Buffer += sizeof(UINT8);

        // hmac
        WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16 (AuthSessionIn->hmac.size));
        Buffer += sizeof(UINT16);

        CopyMem (Buffer, AuthSessionIn->hmac.buffer, AuthSessionIn->hmac.size);
        Buffer += AuthSessionIn->hmac.size;
    }
    else
    {
        //  sessionHandle
        WriteUnaligned32 ((UINT32 *)Buffer, SwapBytes32(TPM_RS_PW));
        Buffer += sizeof(UINT32);

        // nonce = nullNonce
        WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16(0));
        Buffer += sizeof(UINT16);

        // sessionAttributes = 0
        *(UINT8 *)Buffer = 0x00;
        Buffer += sizeof(UINT8);

        // hmac = nullAuth
        WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16(0));
        Buffer += sizeof(UINT16);
    }

    return (UINT32)(UINTN)(Buffer - (UINT8 *)AuthSessionOut);
}


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:  Tpm2HierarchyControl
//
// Description: enables and disables use of a hierarchy
//
//
// Input:       AuthSize
//
// Output:      UINT8* pOutBuf
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI
AmiPlatformTpmPei2HierarchyControl (
    IN CONST EFI_PEI_SERVICES **PeiServices,
    IN AMI_TREE_PPI      *TrEEPeiPpi,
    IN TPMI_RH_HIERARCHY  AuthHandle,
    IN TPMS_AUTH_COMMAND *AuthSession,
    IN TPMI_RH_HIERARCHY  Hierarchy,
    IN TPMI_YES_NO        State
)
{
    EFI_STATUS                       Status;
    AMI_TPM2_HIERARCHY_CONTROL_COMMAND   Cmd;
    AMI_TPM2_HIERARCHY_CONTROL_RESPONSE  Res;
    UINT32                           CmdSize;
    UINT32                           RespSize;
    UINT8                            *Buffer;
    UINT32                           SessionInfoSize;
    UINT8                            *ResultBuf;
    UINT32                           ResultBufSize;

    if(TrEEPeiPpi == NULL){
        return EFI_INVALID_PARAMETER;
    }
    
    //
    // Construct command
    //
    Cmd.Header.tag          = (UINT16)TPM_H2NS(TPM_ST_SESSIONS);
    Cmd.Header.paramSize    = TPM_H2NL(sizeof(Cmd));
    Cmd.Header.commandCode  = TPM_H2NL(TPM_CC_HierarchyControl);
    Cmd.AuthHandle          = TPM_H2NL(AuthHandle);

    //
    // Add in Auth session
    //
    Buffer = (UINT8 *)&Cmd.AuthSession;

    // sessionInfoSize
    SessionInfoSize = AmiTpmPeiCopyAuthSessionCommand (AuthSession, Buffer);
    Buffer += SessionInfoSize;
    Cmd.AuthorizationSize = TPM_H2NL(SessionInfoSize);

    WriteUnaligned32 ((UINT32 *)Buffer, TPM_H2NL(Hierarchy));
    Buffer += sizeof(UINT32);

    *(UINT8 *)Buffer = State;
    Buffer += sizeof(UINT8);

    CmdSize = (UINT32)(Buffer - (UINT8 *)&Cmd);
    Cmd.Header.paramSize = TPM_H2NL(CmdSize);

    ResultBuf     = (UINT8 *) &Res;
    ResultBufSize = sizeof(Res);

    Status = TrEEPeiPpi->SubmitCommand(TrEEPeiPpi, CmdSize, (UINT8 *)&Cmd,  ResultBufSize, ResultBuf );
    if (EFI_ERROR (Status))
    {
        DEBUG(( EFI_D_ERROR, "Tpm2GetRandom TrEEProtocolInstance->SubmitCommand = %r \n", Status));
        return Status;
    }

    if (ResultBufSize > sizeof(Res))
    {
        DEBUG ((EFI_D_ERROR, "HierarchyControl: Failed ExecuteCommand: Buffer Too Small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }

    //
    // Validate response headers
    //
    RespSize = TPM_H2NL(Res.Header.paramSize);
    if (RespSize > sizeof(Res))
    {
        DEBUG ((EFI_D_ERROR, "HierarchyControl: Response size too large! %d\r\n", RespSize));
        return EFI_BUFFER_TOO_SMALL;
    }

    //
    // Fail if command failed
    //
    if (TPM_H2NL(Res.Header.responseCode) != TPM_RC_SUCCESS)
    {
        DEBUG((EFI_D_ERROR,"HierarchyControl: Response Code error! 0x%08x\r\n", TPM_H2NL(Res.Header.responseCode)));
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}




//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   InternalPeiBuildHobGuid
//
// Description: Internal abstracted function to create a Hob
//
// Input:       IN  EFI_PEI_SERVICES  **PeiServices,
//              IN  EFI_GUID          *Guid,
//              IN  UINTN             DataLength,
//              OUT VOID              **Hob
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS InternalPeiBuildHobGuid(
    IN EFI_PEI_SERVICES **PeiServices,
    IN EFI_GUID         *Guid,
    IN UINTN            DataLength,
    OUT VOID            **Hob )
{
    EFI_STATUS Status;

    Status = (*PeiServices)->CreateHob(
                 (CONST EFI_PEI_SERVICES **)PeiServices,
                 EFI_HOB_TYPE_GUID_EXTENSION,
                 (UINT16) ( sizeof (EFI_HOB_GUID_TYPE) + DataLength ),
                 Hob
             );

    if ( EFI_ERROR( Status ))
    {
        DEBUG((DEBUG_INFO, "Failed to create TCG/TPM Hob Status = %r \n", Status));
        return Status;
    }

    DEBUG((DEBUG_INFO, "Hob created \n"));
    ((EFI_HOB_GUID_TYPE*)(*Hob))->Name = *Guid;

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI AmdPspAvailable(
    IN EFI_PEI_SERVICES          **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN VOID                      *Ppi );



#if FTpmPlatformProfile == 1
EFI_PEI_NOTIFY_DESCRIPTOR   AmdMemDiscCallback =
{
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &giTpmPpiGuid,
    AmdPspAvailable
};
#endif


#if defined (AmiTrustedFv_Support) && (AmiTrustedFv_Support == 1)

EFI_STATUS UpdateLogWithVerifiedHash (CONST EFI_PEI_SERVICES  **PeiServices,
                                      AMI_FV_DIGEST_PPI *VerifiedHashDigestPpi)
{
    EFI_STATUS Status = EFI_NOT_FOUND;
    TCG_PLATFORM_SETUP_INTERFACE    *TcgPeiPolicy = NULL;
    TCG_CONFIGURATION               ConfigFlags;
    UINT8                           gTcg2SpecVersion = 0;  
    AMI_TREE_EVENT                  *TrEEEventData;
    EFI_PLATFORM_FIRMWARE_BLOB      EventData;
    UINTN                           TreeEventSize = 0;
    UINT32                          gPcrBanks = 0;
    AMI_TREE_PPI                    *TrEEPeiPpi = NULL;
    UINTN                           i = 0;
    
    
    DEBUG(( DEBUG_INFO, "UpdateLogWithVerifiedHash \n"));
    
    if((PeiServices == NULL) || (VerifiedHashDigestPpi == NULL)){
        return EFI_INVALID_PARAMETER;
    }
    
    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gAmiTreePpiGuid,
                 0, NULL,
                 (void **)&TrEEPeiPpi);

    if(EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }
    
    Status = (*PeiServices)->LocatePpi(
                     PeiServices,
                     &gTcgPeiPolicyGuid,
                     0, NULL,
                     (void **)&TcgPeiPolicy);

    if(TcgPeiPolicy == NULL || EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }
    
    Status = TcgPeiPolicy->getTcgPeiPolicy((EFI_PEI_SERVICES **)PeiServices, &ConfigFlags);
    if(EFI_ERROR(Status))return Status;
    
    gTcg2SpecVersion = (UINTN)ConfigFlags.Tcg2SpecVersion;
    gPcrBanks = ConfigFlags.PcrBanks;
    
    TreeEventSize =  sizeof(AMI_TREE_EVENT_HEADER) +  sizeof(UINT32) + sizeof(EventData);;

    Status = (*PeiServices)->AllocatePool(PeiServices, TreeEventSize,  (void **)&TrEEEventData);
    if( EFI_ERROR(Status)) return Status;
    
    TrEEEventData->Header.PcrIndex = AMI_PCRi_CRTM_AND_POST_BIOS;
    TrEEEventData->Header.HeaderSize =  sizeof(AMI_TREE_EVENT_HEADER);
    TrEEEventData->Header.HeaderVersion = 1;
    TrEEEventData->Header.EventType = EV_POST_CODE;
    TrEEEventData->Size = TreeEventSize;
    
    DEBUG(( DEBUG_INFO, "VerifiedHashDigestPpi->AddrList[0] = %x \n", VerifiedHashDigestPpi->AddrList[0]));
    EventData.BlobBase = (EFI_PHYSICAL_ADDRESS)VerifiedHashDigestPpi->AddrList[0];
    EventData.BlobLength = 0;
    for(i=0; i<VerifiedHashDigestPpi->HashElemCnt; i++){
        DEBUG(( DEBUG_INFO, "VerifiedHashDigestPpi->LenList = %x \n", VerifiedHashDigestPpi->LenList[i]));
        EventData.BlobLength += (UINT64)VerifiedHashDigestPpi->LenList[i];
    }
    
    DEBUG(( DEBUG_INFO, "EventData.BlobLength = %x \n", EventData.BlobLength));
    printbuffer(VerifiedHashDigestPpi->FvHash,VerifiedHashDigestPpi->HashSize);
    
    DEBUG(( DEBUG_INFO, "Hashing Data \n"));
    Status = TrEEPeiPpi->HashLogExtendEvent(TrEEPeiPpi,0, (EFI_PHYSICAL_ADDRESS)(UINTN)VerifiedHashDigestPpi->FvHash, \
                                    VerifiedHashDigestPpi->HashSize, TrEEEventData);

    printbuffer(VerifiedHashDigestPpi->FvHash,VerifiedHashDigestPpi->HashSize);
    DEBUG(( DEBUG_INFO, "UpdateLogWithVerifiedHash - %r\n", Status));
    
    return Status;
}

EFI_STATUS
EFIAPI
Tpm20EndOfPeiSignalPpiNotifyCallback (
        IN EFI_PEI_SERVICES           **PeiServices,
        IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
        IN VOID                       *Ppi
)
{
    EFI_STATUS                      Status = EFI_NOT_FOUND;
    AMI_FV_DIGEST_PPI               *VerifiedHashDigestPpi = NULL;
    EFI_GUID     gAmiFvDigestPpiguid = AMI_FV_DIGEST_PPI_GUID;
    
    
    Status = (*PeiServices)->LocatePpi(PeiServices,
                                    &gAmiFvDigestPpiguid,
                                    0, NULL,
                                    (void **)&VerifiedHashDigestPpi);
    
    if(!EFI_ERROR(Status) && (VerifiedHashDigestPpi != NULL)){
        Status = UpdateLogWithVerifiedHash(PeiServices, VerifiedHashDigestPpi);
        return Status;
    }else{
        
        Status = FindAndMeasureDxeCore((CONST EFI_PEI_SERVICES   **)PeiServices);
        if ( EFI_ERROR( Status ))
        {
            DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
        }
    }

    return Status;
}


EFI_PEI_NOTIFY_DESCRIPTOR mTpm20EndOfPeiSignalPpiNotifyList[] = {
    {
           (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
            &gEfiEndOfPeiSignalPpiGuid,
            Tpm20EndOfPeiSignalPpiNotifyCallback
    }
};
#endif


#define TREE_BOOT_HASH_ALG_SHA1   0x00000001
#define TREE_BOOT_HASH_ALG_SHA256 0x00000002
#define TREE_BOOT_HASH_ALG_SHA384 0x00000004
#define TREE_BOOT_HASH_ALG_SHA512 0x00000008
#define TREE_BOOT_HASH_ALG_SM3    0x00000010

UINT8      Sha1Digest[SHA1_DIGEST_SIZE];
TPM2_HALG  TpmDigest;

EFI_STATUS MeasureLogDxeFwVol(
    IN  CONST EFI_PEI_SERVICES  **PeiServices,
    IN  FwVolHob                *TpmFwVolHob,
    IN  AMI_TREE_PPI            *TrEEPeiPpi,
    IN  AMI_INTERNAL_HLXE_PPI   *HashLogExtendEx
)
{
    EFI_STATUS                        Status = EFI_SUCCESS;
    TCG_PCR_EVENT2_HDR                Tcg20Event;
    EFI_PLATFORM_FIRMWARE_BLOB        EventData;
    SHA1_CTX                          Sha1Ctx;
    SHA2_CTX                          Sha2Ctx;
    SHA384_CTX                        Sha384Ctx;
    SHA512_CTX                        Sha512Ctx;
    SM3_CTX                           Sm3Ctx;
    UINTN                             Count = 0;
    UINT32                            i = 0;
    UINT32                            FwVolHobCount;
    UINT8                             gTcg2SpecVersion = 0;  
    UINT32                            gPcrBanks = 0;

    DEBUG(( DEBUG_INFO, "[%d] Enter MeasureLogDxeFwVol\n", __LINE__));

    FwVolHobCount = TpmFwVolHob->Count;
    gTcg2SpecVersion = TpmFwVolHob->Tcg2SpecVersion;
    gPcrBanks = TpmFwVolHob->PcrBanks;
    
    Tcg20Event.PCRIndex = AMI_PCRi_CRTM_AND_POST_BIOS;
    Tcg20Event.EventType = EV_POST_CODE;
    Tcg20Event.EventSize = sizeof(EventData);

    EventData.BlobBase = TpmFwVolHob[0].baseAddress;
    EventData.BlobLength = (UINT64)TpmFwVolHob[0].Size;

    if(gTcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_1_2 )
    {
        SHA1Init( &Sha1Ctx );
    }
    else if(gTcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_2)
    {
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA1)
        {
            SHA1Init( &Sha1Ctx );
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA256)
        {
            sha256_init(&Sha2Ctx);
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA384)
        {
            sha384_init(&Sha384Ctx);
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA512)
        {
            sha512_init(&Sha512Ctx);
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SM3)
        {
            sm3_init (&Sm3Ctx);
        }
    }
    
    DEBUG ((DEBUG_INFO, "FwVolHobCount = %x \n", FwVolHobCount));
    for(i=0; i< FwVolHobCount; i++)
    {
        DEBUG ((DEBUG_INFO, "TpmFwVolHob[i].Size = %x \n", TpmFwVolHob[i].Size));
        DEBUG ((DEBUG_INFO, "TpmFwVolHob[i].baseAddress = %lx \n", TpmFwVolHob[i].baseAddress));
        if(gTcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_1_2)
        {
            SHA1Update( &Sha1Ctx,  (unsigned char *)(UINTN)TpmFwVolHob[i].baseAddress, (u32)TpmFwVolHob[i].Size );
        }
        else if(gTcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_2)
        {
            if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA256)
            {
                sha256_process( &Sha2Ctx, (unsigned char *)(UINTN)TpmFwVolHob[i].baseAddress, (u32)TpmFwVolHob[i].Size );
            }
            if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA1)
            {
                SHA1Update( &Sha1Ctx, (unsigned char *)(UINTN)TpmFwVolHob[i].baseAddress, (u32)TpmFwVolHob[i].Size );
            }
            if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA384)
            {
                sha512_process(&Sha384Ctx, (unsigned char *)(UINTN)TpmFwVolHob[i].baseAddress, (u32)TpmFwVolHob[i].Size);
            }
            if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA512)
            {
                sha512_process(&Sha512Ctx, (unsigned char *)(UINTN)TpmFwVolHob[i].baseAddress, (u32)TpmFwVolHob[i].Size);
            }
            if( gPcrBanks & TREE_BOOT_HASH_ALG_SM3)
            {
                sm3_update(&Sm3Ctx, (unsigned char *)(UINTN)TpmFwVolHob[i].baseAddress, (u32)TpmFwVolHob[i].Size);
            }
        }
    }
    
    if(gTcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_1_2)
    {
        SHA1Final((unsigned char *)&Sha1Digest, &Sha1Ctx);
        //printbuffer(Sha1Digest,SHA1_DIGEST_SIZE );
        CopyMem(Tcg20Event.Digests.digests[0].digest.sha1, Sha1Digest, SHA1_DIGEST_SIZE);
        Tcg20Event.Digests.digests[0].hashAlg = TPM2_ALG_SHA1;
    }
    else if(gTcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_2)
    {
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA1)
        {
            SHA1Final((unsigned char *)&TpmDigest.sha1, &Sha1Ctx);
            CopyMem(&Tcg20Event.Digests.digests[Count].digest.sha1, &TpmDigest.sha1, SHA1_DIGEST_SIZE);
            Tcg20Event.Digests.digests[Count].hashAlg = TPM2_ALG_SHA1;
            Count+=1;
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA256)
        {
            sha256_done(&Sha2Ctx, (unsigned char *)&TpmDigest.sha256);
            CopyMem(&Tcg20Event.Digests.digests[Count].digest.sha256, &TpmDigest.sha256, SHA256_DIGEST_SIZE);
            Tcg20Event.Digests.digests[Count].hashAlg = TPM2_ALG_SHA256;
            Count+=1;
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA384)
        {
            sha384_done(&Sha384Ctx, (unsigned char *)&TpmDigest.sha384 );
            CopyMem(&Tcg20Event.Digests.digests[Count].digest.sha384,  &TpmDigest.sha384, SHA384_DIGEST_SIZE);
            Tcg20Event.Digests.digests[Count].hashAlg = TPM2_ALG_SHA384;
            Count+=1;
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SHA512)
        {
            sha512_done(&Sha512Ctx, (unsigned char *)&TpmDigest.sha512);
            CopyMem(&Tcg20Event.Digests.digests[Count].digest.sha512,  &TpmDigest.sha512, SHA512_DIGEST_SIZE);
            Tcg20Event.Digests.digests[Count].hashAlg = TPM2_ALG_SHA512;
            Count+=1;
        }
        if( gPcrBanks & TREE_BOOT_HASH_ALG_SM3)
        {
            sm3_final((unsigned char *)&TpmDigest.sm3_256, &Sm3Ctx);
            CopyMem(&Tcg20Event.Digests.digests[Count].digest.sm3_256,  &TpmDigest.sm3_256, SM3_256_DIGEST_SIZE);
            Tcg20Event.Digests.digests[Count].hashAlg = TPM2_ALG_SM3_256;
            Count+=1;
        }
        
        Tcg20Event.Digests.count = (UINT32)Count;
    }
    
    Status = HashLogExtendEx->AmiHashLogExtendEx(PeiServices, TrEEPeiPpi, NULL, 0, 0, &Tcg20Event, (UINT8*)&EventData);
    
    DEBUG(( DEBUG_INFO, "MeasureLogDxeFwVol - %r\n", Status));
    return Status;

}


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   MeasureCRTMVersion
//
// Description: Measures EFI CRTM Version
//              Demo Version[546BFB1E1D0C4055A4AD4EF4BF17B83A]
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
Tpm20MeasureCRTMVersion(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    AMI_TREE_PPI *TrEEPeiPpi)
{
    AMI_TREE_EVENT          *TrEEEventData;
    UINT32              EventSize = 0;
    EFI_GUID            CrtmVersion = CRTM_GUID;
    EFI_STATUS          Status;
    UINT8               *EventDataPtr;
    UINTN               Len;

    EventSize  = sizeof(EFI_GUID);

    Len =  sizeof(AMI_TREE_EVENT_HEADER) +  sizeof(UINT32) + EventSize;

    Status = (*PeiServices)->AllocatePool(PeiServices, Len,  (void **)&TrEEEventData);
    if( EFI_ERROR(Status)) return Status;

    TrEEEventData->Size = Len;
    TrEEEventData->Header.EventType  = EV_S_CRTM_VERSION;
    TrEEEventData->Header.HeaderSize = sizeof(AMI_TREE_EVENT_HEADER);
    TrEEEventData->Header.HeaderVersion = 1;
    TrEEEventData->Header.PcrIndex      = AMI_PCRi_CRTM_AND_POST_BIOS;

    EventDataPtr = (UINT8 *)TrEEEventData;

    EventDataPtr += sizeof(AMI_TREE_EVENT_HEADER) + sizeof(UINT32);

    CopyMem(
        EventDataPtr,
        &CrtmVersion,
        EventSize
    );

    Status = TrEEPeiPpi->HashLogExtendEvent(TrEEPeiPpi,
                                          0, (EFI_PHYSICAL_ADDRESS)(UINTN)EventDataPtr, EventSize, TrEEEventData);

    TpmPeiReportStatusCode(EFI_PROGRESS_CODE, AMI_SPECIFIC_CRTM_VERSION_MEASURED | EFI_SOFTWARE_PEI_MODULE);
    
    return Status;
}


EFI_STATUS
EFIAPI
Tpm2PeiGetCapability (
    IN      AMI_TREE_PPI            *TrEEPeiPpi,
    IN      TPM_CAP                   Capability,
    IN      UINT32                    Property,
    IN      UINT32                    PropertyCount,
    OUT     TPMI_YES_NO               *MoreData,
    OUT     TPMS_CAPABILITY_DATA      *CapabilityData
)
{
    EFI_STATUS                        Status;
    TPM2_GET_CAPABILITY_COMMAND       SendBuffer;
    TPM2_GET_CAPABILITY_RESPONSE      RecvBuffer;
    UINT32                            SendBufferSize;
    UINT32                            RecvBufferSize;
 
    if(TrEEPeiPpi == NULL)return EFI_INVALID_PARAMETER;

    //
    // Construct command
    //
    SendBuffer.Header.tag = SwapBytes16(TPM_ST_NO_SESSIONS);
    SendBuffer.Header.commandCode = SwapBytes32(TPM_CC_GetCapability);

    SendBuffer.Capability = SwapBytes32 (Capability);
    SendBuffer.Property = SwapBytes32 (Property);
    SendBuffer.PropertyCount = SwapBytes32 (PropertyCount);

    SendBufferSize = (UINT32) sizeof (SendBuffer);
    SendBuffer.Header.paramSize = SwapBytes32 (SendBufferSize);

    //
    // send Tpm command
    //
    RecvBufferSize = sizeof (RecvBuffer);
    //Status = TreeSubmitCommand (pTreeProtocol, SendBufferSize, (UINT8 *)&SendBuffer, RecvBufferSize, (UINT8 *)&RecvBuffer );
    Status  = TrEEPeiPpi->SubmitCommand(TrEEPeiPpi, SendBufferSize, (UINT8 *)&SendBuffer, RecvBufferSize, (UINT8 *)&RecvBuffer );
    if (EFI_ERROR (Status))
    {
        return Status;
    }
    
    // Check the device return Tag, if it is belong the TPM20 RSP_TAG
    if( RecvBuffer.Header.tag != (TPMI_ST_COMMAND_TAG)TPM_H2NS(TPM_ST_NO_SESSIONS) )
    {
        return EFI_INCOMPATIBLE_VERSION;
    }
    

    if (RecvBufferSize <= sizeof (TPM2_RESPONSE_HEADER) + sizeof (UINT8))
    {
        return EFI_DEVICE_ERROR;
    }

    //
    if (SwapBytes32(RecvBuffer.Header.responseCode) != TPM_RC_SUCCESS)
    {
        DEBUG((DEBUG_ERROR, "Tpm2GetCapability: Response Code error! 0x%08x\r\n", SwapBytes32(RecvBuffer.Header.responseCode)));
        return EFI_DEVICE_ERROR;
    }
    // Return the response
    //
    *MoreData = RecvBuffer.MoreData;
    //
    // Does not unpack all possiable property here, the caller should unpack it and note the byte order.
    //
    CopyMem (CapabilityData, &RecvBuffer.CapabilityData, RecvBufferSize - sizeof (TPM2_RESPONSE_HEADER) - sizeof (UINT8));

    return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
Tpm2GetPeiCapabilityFwVersion (
    IN      AMI_TREE_PPI            *TrEEPeiPpi,
    OUT     UINT32                  *TpmFwVersion1,
    OUT     UINT32                  *TpmFwVersion2
)
{
    TPMS_CAPABILITY_DATA    TpmCap;
    TPMI_YES_NO             MoreData;
    EFI_STATUS              Status;
    UINT32                  VersionH=0;
    UINT32                  VersionL=0;

    if((TpmFwVersion1 == NULL) || (TpmFwVersion2 == NULL)) return EFI_INVALID_PARAMETER;
    
    *TpmFwVersion1 = 0;
    *TpmFwVersion2 = 0;    
    
    Status = Tpm2PeiGetCapability (
                 TrEEPeiPpi,
                 TPM_CAP_TPM_PROPERTIES,
                 TPM_PT_FIRMWARE_VERSION_1,
                 1,
                 &MoreData,
                 &TpmCap
             );
    
    if (EFI_ERROR (Status))
    {
        return Status;
    }
    
    *TpmFwVersion1 = SwapBytes32 (TpmCap.data.tpmProperties.tpmProperty->value);
    
    DEBUG((DEBUG_INFO, "\n TpmFwVersion1 = %x \n", *TpmFwVersion1));
 
    Status = Tpm2PeiGetCapability (
	       TrEEPeiPpi,		    
               TPM_CAP_TPM_PROPERTIES, 
               TPM_PT_FIRMWARE_VERSION_2, 
               1, 
               &MoreData, 
               &TpmCap
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    *TpmFwVersion2 = SwapBytes32 (TpmCap.data.tpmProperties.tpmProperty->value);
    
    DEBUG((DEBUG_INFO, "\n TpmFwVersion2 = %x \n", *TpmFwVersion2));

    return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
Tpm2PeiGetCapabilityManufactureID (
    IN      AMI_TREE_PPI            *TrEEPeiPpi,
    OUT     UINT32                    *ManufactureId
)
{
    TPMS_CAPABILITY_DATA    TpmCap;
    TPMI_YES_NO             MoreData;
    EFI_STATUS              Status;

    Status = Tpm2PeiGetCapability (
                 TrEEPeiPpi,
                 TPM_CAP_TPM_PROPERTIES,
                 TPM_PT_MANUFACTURER,
                 1,
                 &MoreData,
                 &TpmCap
             );
    if (EFI_ERROR (Status))
    {
        return Status;
    }
    *ManufactureId = SwapBytes32 (TpmCap.data.tpmProperties.tpmProperty->value);
    
    DEBUG((DEBUG_INFO, "\n ManufactureId = %x \n", *ManufactureId));

    return EFI_SUCCESS;
}





//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   Tpm20PeiSelfTest
//
// Description: Send TPM 20 SelfTest Command
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS Tpm20PeiSelfTest(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    AMI_TREE_PPI *TrEEPeiPpi,
    UINT32 *TpmDeviceFirmware1,
    UINT32 *TpmDeviceFirmware2,  
    UINT32 *TpmDeviceManufacturer)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    TPM2_SelfTest           SelfTestCmd;
    TPM2_Common_RespHdr     SelfTestReponse;
    UINT32                  ReturnSize = 0;
    UINT32                  TpmFwVersion1;
    UINT32                  TpmFwVersion2;
    UINT32                  TpmManufacturer;
    
    if(TrEEPeiPpi == NULL) return EFI_INVALID_PARAMETER;
    
    SelfTestCmd.tag = (TPMI_ST_COMMAND_TAG)TPM_H2NS(TPM_ST_NO_SESSIONS);
    SelfTestCmd.CommandSize = TPM_H2NL((sizeof(TPM2_SelfTest)));
    SelfTestCmd.CommandCode = TPM_H2NL(TPM_CC_SelfTest);
    SelfTestCmd.SelfTestType = 0; 
  
    Status = Tpm2GetPeiCapabilityFwVersion(TrEEPeiPpi, &TpmFwVersion1, &TpmFwVersion2);
    if(Status == EFI_INCOMPATIBLE_VERSION){
        return EFI_DEVICE_ERROR;
    }
    
    Status = Tpm2PeiGetCapabilityManufactureID(TrEEPeiPpi, &TpmManufacturer);
    *TpmDeviceFirmware1 = TpmFwVersion1;
    *TpmDeviceFirmware2 = TpmFwVersion2;
    *TpmDeviceManufacturer = TpmManufacturer;
        
    if(!EFI_ERROR(Status) && TpmManufacturer == 0x49465800){
        if(TpmFwVersion1 == 0x07003C || TpmFwVersion1 == 0x05003C){ //workaround for Infineon FW 5.60 and 7.60 failure
            SelfTestCmd.SelfTestType = 1;
        }else{
            SelfTestCmd.SelfTestType = 0;
        }
    }
    
    ReturnSize = (UINT32)sizeof(SelfTestReponse);

    SetMem((UINT8 *)&SelfTestReponse,(UINTN)sizeof(SelfTestReponse), 0);

    DEBUG((DEBUG_INFO, "\nsending TPM20 SelfTest To Tpm \n"));
    Status = TrEEPeiPpi->SubmitCommand(TrEEPeiPpi,
                                       sizeof(SelfTestCmd),
                                       (UINT8*)&SelfTestCmd,
                                       ReturnSize,
                                       (UINT8*)&SelfTestReponse);

    if ( EFI_ERROR( Status ))
    {
        return Status;
    }

    // Check the device return Tag, if it is belong the TPM20 RSP_TAG
    if( SelfTestReponse.tag != (TPMI_ST_COMMAND_TAG)TPM_H2NS(TPM_ST_NO_SESSIONS) )
    {
        Status = EFI_DEVICE_ERROR;
    }
    else if( SelfTestReponse.ResponseCode != TPM_RC_SUCCESS)
    {
        Status = EFI_NOT_READY;
    }

    DEBUG((DEBUG_INFO, "SelfTestReponse.Tag = %x \n", TPM_H2NS(SelfTestReponse.tag)));
    DEBUG((DEBUG_INFO, "SelfTestReponse.ResponseCode = %x \n", TPM_H2NL(SelfTestReponse.ResponseCode)));
    DEBUG((DEBUG_INFO, "SelfTestReponse.Status = %r \n", Status));


    return Status;
}


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:  Tpm2PeiHierarchyChangeAuth
//
// Description: allows the authorization secret for a hierarchy or lockout to be changed using the current
//              authorization value
//
//
// Input:       AuthHandle, AuthSession, NewAuth
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI
Tpm2PeiHierarchyChangeAuth (IN AMI_TREE_PPI *TrEEPeiPpi,
    IN TPMI_RH_HIERARCHY_AUTH     AuthHandle,
    IN TPMS_AUTH_COMMAND *AuthSession,
    IN TPM2B_AUTH                *NewAuth
)
{
    EFI_STATUS                           Status;
    TPM2_HIERARCHY_CHANGE_AUTH_COMMAND   Cmd;
    TPM2_HIERARCHY_CHANGE_AUTH_RESPONSE  Res;
    UINT32                               CmdSize;
    UINT32                               RespSize;
    UINT8                                *Buffer;
    UINT32                               SessionInfoSize;
    UINT8                                *ResultBuf;
    UINT32                               ResultBufSize;

    //
    // Construct command
    //
    Cmd.Header.tag          = (UINT16)TPM_H2NS(TPM_ST_SESSIONS);
    Cmd.Header.paramSize    = TPM_H2NL(sizeof(Cmd));
    Cmd.Header.commandCode  = TPM_H2NL(TPM_CC_HierarchyChangeAuth);
    Cmd.AuthHandle          = TPM_H2NL(AuthHandle);

    //
    // Add in Auth session
    //
    Buffer = (UINT8 *)&Cmd.AuthSession;

    // sessionInfoSize
    SessionInfoSize = AmiTpmPeiCopyAuthSessionCommand (AuthSession, Buffer);
    Buffer += SessionInfoSize;
    Cmd.AuthorizationSize = TPM_H2NL(SessionInfoSize);

    // New Authorization size
    WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16(NewAuth->size));
    Buffer += sizeof(UINT16);

    // New Authorizeation
    CopyMem(Buffer, NewAuth->buffer, NewAuth->size);
    Buffer += NewAuth->size;

    CmdSize = (UINT32)(Buffer - (UINT8 *)&Cmd);
    Cmd.Header.paramSize = TPM_H2NL(CmdSize);

    ResultBuf     = (UINT8 *) &Res;
    ResultBufSize = sizeof(Res);

    Status = TrEEPeiPpi->SubmitCommand(TrEEPeiPpi, CmdSize, (UINT8*)&Cmd,  ResultBufSize, ResultBuf );

    if (EFI_ERROR(Status))return Status;
    DEBUG ((EFI_D_ERROR, "HierarchyChangeAuth: Response code   %x\r\n", Res.Header.responseCode));
    //
    // Validate response headers
    //
    RespSize = TPM_H2NL(Res.Header.paramSize);
    if (RespSize > sizeof(Res))
    {
        DEBUG ((EFI_D_ERROR, "HierarchyChangeAuth: Response size too large! %d\r\n", RespSize));
        return EFI_BUFFER_TOO_SMALL;
    }

    //
    // Fail if command failed
    //
    if (TPM_H2NL(Res.Header.responseCode) != TPM_RC_SUCCESS)
    {
        DEBUG((EFI_D_ERROR,"HierarchyChangeAuth: Response Code error! 0x%08x\r\n", TPM_H2NL(Res.Header.responseCode)));
        return EFI_DEVICE_ERROR;
    }
    
    return EFI_SUCCESS;
}


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:  Tpm2GetRandom
//
// Description: gets random bytes from the TPM
//
//
// Input:       AuthSize
//
// Output:      UINT8* pOutBuf
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS Tpm2PeiGetRandom(IN CONST EFI_PEI_SERVICES **PeiServices,
                            IN AMI_TREE_PPI *TrEEPeiPpi,
                            IN UINTN                   AuthSize,
                            IN UINT8*                  pOutBuf
)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    TPM2_GetRandom_COMMAND  GetRandom_cmd;
    TPM2_GetRandom_RESPONSE GetRandom_ret;
    UINT32                  RetSize;
    UINTN                   i;
    AMI_TREE_BOOT_SERVICE_CAPABILITY ProtocolCapability;

    // Check the Request Size
    if( AuthSize > sizeof(TPM2B_DIGEST) - sizeof(UINT16) )
    {
        DEBUG(( DEBUG_ERROR, "Tpm2GetRandom Error. Request too large\n"));
        return EFI_BUFFER_TOO_SMALL;
    }

    ProtocolCapability.Size = sizeof(AMI_TREE_BOOT_SERVICE_CAPABILITY);       
    Status = TrEEPeiPpi->GetCapability(TrEEPeiPpi, &ProtocolCapability);
     
    if(EFI_ERROR(Status)){
       DEBUG(( DEBUG_ERROR, "TCG get Capability failed. Aborting. ..\n"));
       return Status;
    }

    DEBUG(( DEBUG_INFO, "ManufacturerID = %x\n", ProtocolCapability.ManufacturerID));
        
    if( (ProtocolCapability.ManufacturerID == 0x494E5443) ) // INTC
    {
        DEBUG(( EFI_D_ERROR, "Matched. ..\n"));
        for (i=0; i<(AuthSize/2); i++){
            AsmRdRand16((UINT16 *)&pOutBuf[i] );
        }
        return EFI_SUCCESS; 
    }

    SetMem( &GetRandom_cmd, sizeof(GetRandom_cmd), 0 );
    SetMem( &GetRandom_ret, sizeof(GetRandom_ret), 0 );

    GetRandom_cmd.Header.tag =  (UINT16)TPM_H2NS(TPM_ST_NO_SESSIONS);
    GetRandom_cmd.Header.commandCode = TPM_H2NL(TPM_CC_GetRandom);
    GetRandom_cmd.Header.paramSize = TPM_H2NL( sizeof(GetRandom_cmd) );
    GetRandom_cmd.bytesRequested = TPM_H2NS(AuthSize );

    RetSize = sizeof(GetRandom_ret);

    Status = TrEEPeiPpi->SubmitCommand(TrEEPeiPpi, sizeof(GetRandom_cmd), (UINT8*)&GetRandom_cmd,  RetSize, (UINT8*)&GetRandom_ret );
    if (EFI_ERROR (Status))
    {
        DEBUG(( DEBUG_ERROR, "Tpm2GetRandom TrEEPeiPpi->SubmitCommand = %r \n", Status));
        return Status;
    }

    if (TPM_H2NL(GetRandom_ret.Header.responseCode) != TPM_RC_SUCCESS)
    {
        DEBUG(( DEBUG_ERROR, "Tpm2GetRandom TrEEPeiPpi->SubmitCommand Response Code error! = %x \n", TPM_H2NL(GetRandom_ret.Header.responseCode)));
        return EFI_DEVICE_ERROR;
    }

    CopyMem( pOutBuf, &GetRandom_ret.randomBytes.buffer[0], AuthSize );

    return Status;
}



VOID PeiChangePlatformAuth(IN CONST EFI_PEI_SERVICES **PeiServices,
                           IN AMI_TREE_PPI *TrEEPeiPpi)
{
    TPM2B_AUTH                        NewAuth;
    UINT32                     HashPolicysize = 0x0;
    TCG_CONFIGURATION       ConfigFlags;
    TCG_PLATFORM_SETUP_INTERFACE *TcgPeiPolicy;
    EFI_GUID                        gTcgPeiPolicyGuid =\
            TCG_PLATFORM_SETUP_PEI_POLICY_GUID;
    EFI_STATUS                        Status;

    DEBUG((DEBUG_INFO, "Setting PhRandomization\n"));

    Status = (*PeiServices)->LocatePpi(
                    (CONST EFI_PEI_SERVICES **)PeiServices,
                     &gTcgPeiPolicyGuid,
                     0, NULL,
                     (void **) &TcgPeiPolicy);

    if(EFI_ERROR(Status) || TcgPeiPolicy == NULL )return;;

    Status = TcgPeiPolicy->getTcgPeiPolicy((EFI_PEI_SERVICES **)PeiServices, &ConfigFlags);
    
    if (EFI_ERROR (Status))
    {
        //use default SHA1 size
        HashPolicysize = SHA1_DIGEST_SIZE;
    }
    else
    {
        ASSERT_EFI_ERROR(Status);
        if(ConfigFlags.Tcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_1_2)
        {
            HashPolicysize = SHA1_DIGEST_SIZE;
        }
        else if(ConfigFlags.Tcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_2)
        {
            HashPolicysize = SHA256_DIGEST_SIZE;
        }
    }

    DEBUG((EFI_D_ERROR, "HashPolicysize = %x \n", HashPolicysize));
    
    NewAuth.size = HashPolicysize;

    Tpm2PeiGetRandom(PeiServices,
        TrEEPeiPpi,
        NewAuth.size,
        &NewAuth.buffer[0]
    );

    Tpm2PeiHierarchyChangeAuth (TrEEPeiPpi, TPM_RH_PLATFORM, NULL, &NewAuth);
    return;
}

//*** AMI PORTING BEGIN *****//
EFI_STATUS S3ResetPrep(IN CONST EFI_PEI_SERVICES **PeiServices)
{
    ACPI_RESET_STRUCTURE    AcpiResetVar = {0,0xff,0};
    EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices = NULL;
    EFI_GUID gAmiGlobalVariableGuid = { 0x1368881, 0xc4ad, 0x4b1d, { 0xb6, 0x31, 0xd5, 0x7a, 0x8e, 0xc8, 0xdb, 0x6b } };
    UINTN VarSize = sizeof(ACPI_RESET_STRUCTURE);          
    EFI_STATUS Status;
    
    //Getting the information, saved on ready_to_boot, about PM1 Control Register to clear SLP_TYPx bits before reseting the system
    Status = PeiServicesLocatePpi (
                 &gEfiPeiReadOnlyVariable2PpiGuid,
                 0,
                 NULL,
                 (VOID **) &VariableServices
                 );
    
    ASSERT_EFI_ERROR (Status);
    
    DEBUG ((EFI_D_ERROR,"VarSize = %x \n", VarSize));
    //Getting the information, saved on ready_to_boot, about PM1 Control Register to clear SLP_TYPx bits before reseting the system
    if (VariableServices != NULL)
        Status = VariableServices->GetVariable (
                                     VariableServices,
                                     L"AcpiResetVar",
                                     &gAmiGlobalVariableGuid,
                                     NULL,
                                     &VarSize,
                                     &AcpiResetVar
                                     );
    
    DEBUG ((EFI_D_ERROR,"VarSize = %x \n", VarSize));
    
    ASSERT_EFI_ERROR (Status);
        DEBUG ((EFI_D_ERROR,"Getting AcpiResetVar: Status=%r Addess=%lx AccessSize=%x Type=%x \n", Status, AcpiResetVar.Addess, AcpiResetVar.AccessSize, AcpiResetVar.Type));   
  
    if ((AcpiResetVar.AccessSize == 2) && (AcpiResetVar.Type < 2) && (AcpiResetVar.Addess < 0xffffffff))
    {
        if (AcpiResetVar.Type == 1)
        {  
            //Clearing SLP_TYPx bits in PM1 Control Register in I/O Space
            IoWrite32((UINTN)AcpiResetVar.Addess, IoRead32((UINTN)AcpiResetVar.Addess) & ~(0x1c00));
        }
        else 
        {
             //Clearing SLP_TYPx bits in memory mapped PM1 Control Register
            UINT32 MemoryMappedReset = *(UINT32*)(UINTN)AcpiResetVar.Addess;
            *(UINT32*)(UINTN)AcpiResetVar.Addess = MemoryMappedReset & ~(0x1c00);
          
      }
    }
    else
    {
        //We can not clear SLP_TYPx bits in PM1 Control Register at this stage. 
        DEBUG ((EFI_D_ERROR,"Performing reset without clearing SLP_TYPx bits in PM1 Control Register! \n"));
        DEBUG ((EFI_D_ERROR,"After system will resets, it may still be on S3 resume path and can asserts or hang somewhere in memory initialization or elsewhere! \n"));
    }
    return Status;
}
//*** AMI PORTING END *****//

//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   HandleTpmResumeFailure
//
// Description: If Startup(State) fails, Cap PCRS and disable all hierarchis
//
//
// Input:       IN    EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS HandleTpmResumeFailure(IN CONST EFI_PEI_SERVICES **PeiServices,
                               AMI_TREE_PPI *TrEEPeiPpi)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_PEI_RESET2_PPI *ResetPpi;
    
    DEBUG((DEBUG_INFO, "Handling TPM resume failure \n"));
    
    Status = CapAllTpm2Pcrs(PeiServices, TrEEPeiPpi, 1);
  
    if(EFI_ERROR (Status)){
        //failed to cap;  reboot system-failsafe 
        Status = S3ResetPrep(PeiServices);
        if(EFI_ERROR(Status)){
            Status = PeiServicesLocatePpi ( &gEfiPeiReset2PpiGuid, 0, NULL, (void **)&ResetPpi);
            if(!EFI_ERROR(Status))
            {
                (*PeiServices)->ResetSystem2 (EfiResetShutdown, EFI_SECURITY_VIOLATION, 0, NULL);
            }else{
                (*PeiServices)->ResetSystem(PeiServices);
            }
        }else{
            Status = PeiServicesLocatePpi ( &gEfiPeiReset2PpiGuid, 0, NULL, (void **)&ResetPpi);
            if(!EFI_ERROR(Status))
            {
                (*PeiServices)->ResetSystem2 (EfiResetCold, EFI_SECURITY_VIOLATION, 0, NULL);
            }else{
                (*PeiServices)->ResetSystem(PeiServices);
            }
        }
        CpuDeadLoop ();
    }
    
    Status = AmiPlatformTpmPei2HierarchyControl(PeiServices, TrEEPeiPpi, TPM_RH_PLATFORM,\
                                                  NULL, TPM_RH_ENDORSEMENT,  0);
    if(EFI_ERROR(Status)){
      DEBUG ((EFI_D_ERROR, "Tcg2 Boot in recovery Disable EH Status =%r \n", Status));
    }
    Status = AmiPlatformTpmPei2HierarchyControl(PeiServices, TrEEPeiPpi, TPM_RH_PLATFORM,\
                                                  NULL, TPM_RH_OWNER,  0);
    if(EFI_ERROR(Status)){
       DEBUG ((EFI_D_ERROR, "Tcg2 Boot in recovery Disable SH Status =%r \n", Status));
    }
    
    PeiChangePlatformAuth(PeiServices, TrEEPeiPpi);
    
    return Status;
}




//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   Tpm20PeiSendStartup
//
// Description: Send TPM 20 Startup Command
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS Tpm20PeiSendStartup(IN CONST EFI_PEI_SERVICES **PeiServices,
                               AMI_TREE_PPI *TrEEPeiPpi, EFI_BOOT_MODE BootMode)
{
    EFI_STATUS          Status = EFI_SUCCESS;
    EFI_STATUS          Status2;
    TPM2_Startup_Cmd    StartupCmd;
    TPM2_Common_RespHdr StartupReponse;
    UINT32              ReturnSize = 0;
    AMI_TCG_PEI_FUNCTION_OVERRIDE_PPI       *PpiOverride;
    AMI_TCG_PEI_FUNCTION_OVERRIDE_PPI       *TpmS3ResumePpiOverride;
    BOOLEAN             SkipTpmStartup = FALSE;
    EFI_HOB_GUID_TYPE       *Hob;
    Tpm20DeviceHob          *TrEEDeviceHob;
    TCG_CONFIGURATION       ConfigFlags;
    TCG_PLATFORM_SETUP_INTERFACE *TcgPeiPolicy;
    EFI_GUID                        gTcgPeiPolicyGuid =\
            TCG_PLATFORM_SETUP_PEI_POLICY_GUID;

    UINT32              TpmFirmwareVerion1=0;
    UINT32              TpmFirmwareVerion2=0;
    UINT32              TpmManufacturer=0;
#if ( defined(TPM2_S3_STARTUP_FAILURE_REBOOT_FLOW) && (TPM2_S3_STARTUP_FAILURE_REBOOT_FLOW != 0) )
    EFI_PEI_RESET2_PPI *ResetPpi;
#endif

    //A previous call to TpmStartupSave state on S3 resume was called and failed 
    //so a subsequent call to TpmStartClear was called and now TPM PCRs need to be
    //capped
    if(BootMode == BOOT_ON_S3_RESUME)
    {
        Status = (*PeiServices)->LocatePpi(
                     PeiServices,
                     &gHandleTpmS3ResumeFailure,
                     0, NULL,
                     (void **)&TpmS3ResumePpiOverride);
     
        if(!EFI_ERROR(Status))
        {
            Status = HandleTpmResumeFailure(PeiServices, TrEEPeiPpi);
            return Status;
        }
    }
    
    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gSkipTpmStartupGuid,
                 0, NULL,
                 (void **)&PpiOverride);

    if(!EFI_ERROR(Status))
    {
        SkipTpmStartup = TRUE;
    }

    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gTcgPeiPolicyGuid,
                 0, NULL,
                 (void **)&TcgPeiPolicy);

    if(EFI_ERROR(Status) || TcgPeiPolicy == NULL )return Status;

    Status = TcgPeiPolicy->getTcgPeiPolicy((EFI_PEI_SERVICES **)PeiServices, &ConfigFlags);

    if(EFI_ERROR(Status))return Status;

    if(TrEEPeiPpi == NULL)return EFI_INVALID_PARAMETER;

    StartupCmd.tag = (TPMI_ST_COMMAND_TAG)TPM_H2NS(TPM_ST_NO_SESSIONS);
    StartupCmd.CommandSize = TPM_H2NL((sizeof(TPM2_Startup_Cmd)));
    StartupCmd.CommandCode = TPM_H2NL(TPM_CC_Startup);



    if(BootMode == BOOT_ON_S3_RESUME)
    {
        StartupCmd.StartupType = TPM_H2NS(TPM_SU_STATE);   
    }
    else
    {
        StartupCmd.StartupType = TPM_H2NS(TPM_SU_CLEAR);
    }

    ReturnSize = (UINT32)sizeof(StartupReponse);

    SetMem((UINT8 *)&StartupReponse,(UINTN)sizeof(StartupReponse), 0);
    if( !SkipTpmStartup )
    {                
        if(*(UINT32*)(0xFED40000 + 0xC)  !=  0xFFFFFFFF)
        {

            if( isTpm20CrbPresent() == TRUE )
            {
                *(UINT8*)(0xfed40000 + 0x08) = 0x01;
                MicroSecondDelay ( 0x05 * 0x1000 ); // Delay 5 ms
                *(UINT8*)(0xfed40000 + 0x40) = 0x01;
                MicroSecondDelay ( 0x50 * 0x1000 ); // Delay 50 ms
            }
        }
        DEBUG((DEBUG_INFO, "sending TPM20 b4 TCGPassThroughToTpm \n"));
        Status = TrEEPeiPpi->SubmitCommand(TrEEPeiPpi,
                                           sizeof(TPM2_Startup_Cmd),
                                           (UINT8*)&StartupCmd,
                                           ReturnSize,
                                           (UINT8*)&StartupReponse);

        if(EFI_ERROR(Status))return Status;

        if((StartupReponse.ResponseCode) != TPM_RC_SUCCESS 
                    && ((TPM_H2NL(StartupReponse.ResponseCode))!= TPM_RC_INITIALIZE))
        {
            DEBUG((DEBUG_INFO, "StartupReponse.ResponseCode = %x \n", TPM_H2NL(StartupReponse.ResponseCode)));
            DEBUG((DEBUG_INFO, "StartupReponse.Status = %r \n", Status));
            TpmPeiReportStatusCode(EFI_ERROR_CODE|EFI_ERROR_MAJOR, AMI_SPECIFIC_TPM_2_0_STARTUP_ERROR | EFI_SOFTWARE_PEI_MODULE);   
            Status = EFI_DEVICE_ERROR;
        }else{
            TpmPeiReportStatusCode(EFI_PROGRESS_CODE, AMI_SPECIFIC_TPM_2_0_STARTUP_CMD_SENT | EFI_SOFTWARE_PEI_MODULE);
        }
        
    }
    
    if(BootMode == BOOT_ON_S3_RESUME && ConfigFlags.Tpm20Device == 1)
    {
#if defined (SetReadyStateOnExitBootServices) && (SetReadyStateOnExitBootServices == 1)
        // When customer have enable the TOKEN,
        // We still need to sync on S3 resume

        // For Intel PTT Sighting, we avoid the TPM_BASE addr.
        if(*(UINT32*)(0xFED40000 + 0xC) !=  0xFFFFFFFF )
        {
            // The requirement is suitable for Intel PTT or D-TPM
            if( TRUE == isTpm20CrbPresent() )
            {
                dTPMCrbSetReqReadyState( (VOID*)0xFED40000 );
            }
        }
#endif
        if((StartupReponse.ResponseCode) != TPM_RC_SUCCESS)
        {
#if ( defined(TPM2_S3_STARTUP_FAILURE_REBOOT_FLOW) && (TPM2_S3_STARTUP_FAILURE_REBOOT_FLOW != 0) )
            Status = S3ResetPrep(PeiServices);
            if(EFI_ERROR(Status)){
                Status = PeiServicesLocatePpi ( &gEfiPeiReset2PpiGuid, 0, NULL, (void **)&ResetPpi);
                if(!EFI_ERROR(Status))
                {
                    (*PeiServices)->ResetSystem2 (EfiResetShutdown, EFI_SECURITY_VIOLATION, 0, NULL);
                }else{
                    (*PeiServices)->ResetSystem(PeiServices);  //do system reset
                }
            }else{
                Status = PeiServicesLocatePpi ( &gEfiPeiReset2PpiGuid, 0, NULL, (void **) &ResetPpi);
                if(!EFI_ERROR(Status))
                {
                    (*PeiServices)->ResetSystem2 (EfiResetCold, EFI_SECURITY_VIOLATION, 0, NULL);
                }else{
                    (*PeiServices)->ResetSystem(PeiServices);  //do system reset
                }
            }
            CpuDeadLoop ();
#else            
            StartupCmd.StartupType = TPM_H2NS(TPM_SU_CLEAR);
            Status = TrEEPeiPpi->SubmitCommand(TrEEPeiPpi,
                                               sizeof(TPM2_Startup_Cmd),
                                               (UINT8*)&StartupCmd,
                                               ReturnSize,
                                               (UINT8*)&StartupReponse);

             if(EFI_ERROR(Status))return Status;

             if((StartupReponse.ResponseCode) != TPM_RC_SUCCESS)
             {
                 DEBUG((DEBUG_INFO, "StartupReponse.ResponseCode = %x \n", TPM_H2NL(StartupReponse.ResponseCode)));
                 DEBUG((DEBUG_INFO, "StartupReponse.Status = %r \n", Status));
                 TpmPeiReportStatusCode(EFI_ERROR_CODE|EFI_ERROR_MAJOR, AMI_SPECIFIC_TPM_2_0_STARTUP_ERROR | EFI_SOFTWARE_PEI_MODULE);   
                 Status = EFI_DEVICE_ERROR;
                 return Status;
             }
                    
             Status = HandleTpmResumeFailure(PeiServices, TrEEPeiPpi);
#endif
        }
        return Status;
    }
    // Since the BootGuard will skip startup command, Check here if the TPM 2.0 is present
    if( !EFI_ERROR(Status) )
    {
        Status = Tpm20PeiSelfTest( PeiServices, TrEEPeiPpi, &TpmFirmwareVerion1, &TpmFirmwareVerion2,  &TpmManufacturer);
        if( Status == EFI_NOT_READY )
        {
            Status = EFI_SUCCESS;
        }
        if(!EFI_ERROR(Status)){
            TpmPeiReportStatusCode(EFI_PROGRESS_CODE, AMI_SPECIFIC_TPM_2_0_SELFTEST_CMD_SENT | EFI_SOFTWARE_PEI_MODULE);
        }
        
    }

    Status2 = InternalPeiBuildHobGuid((EFI_PEI_SERVICES **)PeiServices, &gTpm20HobGuid,
                                     (sizeof(Tpm20DeviceHob)),  (void **)&Hob);
    ASSERT_EFI_ERROR( Status2 );
    
    if(Status != EFI_DEVICE_ERROR && !EFI_ERROR(Status))
    {
        TrEEDeviceHob = (Tpm20DeviceHob*)(Hob + 1);
        TrEEDeviceHob->Tpm20DeviceState = 1;
        if(!GetCurrentInterfaceType())
        {
            TrEEDeviceHob->InterfaceType = 1;
        }
        else
        {
            TrEEDeviceHob->InterfaceType = 0;
        }
        TrEEDeviceHob->Tpm2FWersion1 = TpmFirmwareVerion1;
        TrEEDeviceHob->Tpm2FWersion2 = TpmFirmwareVerion2;        
        
        TrEEDeviceHob->Tpm2manufacturer = TpmManufacturer;
    }
    else
    {
        TrEEDeviceHob = (Tpm20DeviceHob*)(Hob + 1);
        TrEEDeviceHob->Tpm20DeviceState = 0;
        TrEEDeviceHob->InterfaceType = ConfigFlags.InterfaceSel;
    }
    

    TrEEDeviceHob->Reserved = FirstBootCheck((EFI_PEI_SERVICES **)PeiServices);

    if( TrEEDeviceHob->Tpm20DeviceState )
    {
        DEBUG((DEBUG_INFO, "StartupReponse.Tag = %x \n", StartupReponse.tag));
        DEBUG((DEBUG_INFO, "StartupReponse.Size = %x \n", StartupReponse.ResponsSize));
        DEBUG((DEBUG_INFO, "StartupReponse.ResponseCode = %x \n", StartupReponse.ResponseCode));
    }
    
    return Status;
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   FirstBootCheck
//
// Description: Check for first boot
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
UINT8
EFIAPI FirstBootCheck(
    IN EFI_PEI_SERVICES **PeiServices)
{

    EFI_STATUS  Status;
    CHAR16      Monotonic[] = L"MonotonicCounter";
#if defined(CORE_COMBINED_VERSION) && (CORE_COMBINED_VERSION > 262797)
    EFI_GUID    Guid        = gAmiGlobalVariableGuid;
#else
    EFI_GUID    Guid        = gEfiGlobalVariableGuid;
#endif
    UINTN                          Size        = sizeof(UINT32);
    UINT32                         Counter;
    EFI_PEI_READ_ONLY_VARIABLE2_PPI  *ReadOnlyVariable;

    Status = (*PeiServices)->LocatePpi(
                 (CONST EFI_PEI_SERVICES    **)PeiServices,
                 &gEfiPeiReadOnlyVariable2PpiGuid,
                 0, NULL,
                 (void **)&ReadOnlyVariable
             );

    if(EFI_ERROR(Status) || ReadOnlyVariable == NULL )return Status;

    Status = ReadOnlyVariable->GetVariable(ReadOnlyVariable, Monotonic, &Guid,
                                           NULL, &Size, &Counter );


    if ( EFI_ERROR( Status ))
    {
        DEBUG((DEBUG_INFO, "TPM20::First boot Scenario determined \n"));
        return 01;
    }

    DEBUG((DEBUG_INFO, "TPM20::Not First boot Scenario\n"));
    return 0xFF;
}




//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   AmdPspAvailable
//
// Description: Call back for PSP fTPM
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI AmdPspAvailable(
    IN EFI_PEI_SERVICES          **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN VOID                      *Ppi )
{

    EFI_STATUS Status;
    AMI_TREE_PPI *TrEEPeiPpi = NULL;
    TCG_PLATFORM_SETUP_INTERFACE    *TcgPeiPolicy;
    TCG_CONFIGURATION               ConfigFlags;
    EFI_GUID                        gTcgPeiPolicyGuid =\
            TCG_PLATFORM_SETUP_PEI_POLICY_GUID;
    EFI_BOOT_MODE       BootMode;

    Status = (*PeiServices)->LocatePpi(
                  (CONST EFI_PEI_SERVICES    **)PeiServices,
                 &gAmiTreePpiGuid,
                 0, NULL,
                 (void **)&TrEEPeiPpi);

    if(EFI_ERROR(Status) || TrEEPeiPpi == NULL )return Status;

    Status = (*PeiServices)->GetBootMode ((CONST EFI_PEI_SERVICES    **)PeiServices, &BootMode);
    if(EFI_ERROR(Status))return Status;

    Status = (*PeiServices)->LocatePpi(
                 (CONST EFI_PEI_SERVICES    **)PeiServices,
                 &gTcgPeiPolicyGuid,
                 0, NULL,
                 (void **)&TcgPeiPolicy);

    if(TcgPeiPolicy == NULL || EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    Status = TcgPeiPolicy->getTcgPeiPolicy((EFI_PEI_SERVICES **)PeiServices, &ConfigFlags);
    if(EFI_ERROR(Status))return Status;


    //send TPM 2.0 Startup
    Status = Tpm20PeiSendStartup((CONST EFI_PEI_SERVICES **)PeiServices, TrEEPeiPpi, BootMode);
    if( EFI_ERROR(Status) )
    {
        return EFI_SUCCESS;
    }

    if(BootMode == BOOT_ON_S3_RESUME)
    {
        return EFI_SUCCESS;
    }

    if(ConfigFlags.Tcg2SpecVersion ==  TCG2_PROTOCOL_SPEC_TCG_2)
    {
        Status =  MeasureTCGPcClientSpecID((CONST EFI_PEI_SERVICES **)PeiServices);
        if ( EFI_ERROR( Status ))
        {
            DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
        }
    }
    
#if defined(TcgNistSp800_155_SUPPORT) && (TcgNistSp800_155_SUPPORT == 1)
    Status = MeasureNISTManifest(PeiServices);
    if ( EFI_ERROR( Status ))
    {
        DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
    }
#endif
    Status = Tpm20MeasureCRTMVersionFuncPtr((CONST EFI_PEI_SERVICES **)PeiServices, TrEEPeiPpi);
    if ( EFI_ERROR( Status ))
    {
        DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
    }
    
#if defined (AmiTrustedFv_Support) && (AmiTrustedFv_Support == 1)
    Status = PeiServicesNotifyPpi (&mTpm20EndOfPeiSignalPpiNotifyList[0]);
#else
    Status = FindAndMeasureDxeCore((CONST EFI_PEI_SERVICES   **)PeiServices);
    if ( EFI_ERROR( Status ))
    {
        DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
    }
#endif
    return Status;
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   printbuffer
//
// Description: Printbuffer for debug purposes
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
void printbuffer(UINT8 *Buffer, UINTN BufferSize)
{
    UINTN i=0;
    UINTN j=0;

    DEBUG((DEBUG_INFO,"\n**********PrintBuffer Entry********"));
    for(i=0; i<BufferSize; i++)
    {

        if(i%16 == 0)
        {
            DEBUG((DEBUG_INFO,"\n"));
            DEBUG((DEBUG_INFO,"%04x :", j));
            j+=1;
        }
        DEBUG((DEBUG_INFO,"%02x ", Buffer[i]));
    }
    DEBUG((DEBUG_INFO,"\n"));
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   MeasureNISTManifest
//
// Description: Measures NIST spec
//
//
// Input:       IN  PEI_TCG_PPI* tcg,
//              IN  EFI_PEI_SERVICES      **ps
//
// Output:
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
#if defined(TcgNistSp800_155_SUPPORT) && (TcgNistSp800_155_SUPPORT == 1)
EFI_STATUS MeasureNISTManifest(
    IN CONST EFI_PEI_SERVICES **PeiServices)
{
    AMI_TREE_EVENT                    *TrEEEventData;
    EFI_STATUS                    Status;
    EFI_GUID                      ManifestGuid = ReferenceManifestGuid_TcgLog;
    AMI_TREE_PPI                    *TrEEPeiPpi = NULL;
    UINTN                           Size=sizeof(EFI_GUID) + sizeof(UINT32) + sizeof(AMI_TREE_EVENT_HEADER) + sizeof(UINT32);
    UINT8                           *EventDataPtr;

    DEBUG((DEBUG_INFO,  "TCG Pei: MeasureNISTManifest\n"));
    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gAmiTreePpiGuid,
                 0, NULL,
                 (void **)&TrEEPeiPpi);

    if(TrEEPeiPpi == NULL || EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    Status = (*PeiServices)->AllocatePool(PeiServices, Size, (void **)&TrEEEventData);
    if(EFI_ERROR(Status))return Status;

    TrEEEventData->Header.EventType = EV_NO_ACTION;
    TrEEEventData->Header.HeaderSize =  sizeof(AMI_TREE_EVENT_HEADER);
    TrEEEventData->Header.HeaderVersion = 1;
    TrEEEventData->Header.PcrIndex = PCRi_CRTM_AND_POST_BIOS;
    TrEEEventData->Size = Size;

    EventDataPtr = (UINT8 *)TrEEEventData;
    EventDataPtr += sizeof(AMI_TREE_EVENT_HEADER) + sizeof(UINT32);

    *(UINT32 *)EventDataPtr = TcgOemVenderID;
    EventDataPtr += sizeof(UINT32);

    CopyMem(EventDataPtr,
            &ManifestGuid,
            sizeof(EFI_GUID));

    Status = TrEEPeiPpi->HashLogExtendEvent(TrEEPeiPpi,
                                            0,
                                            (EFI_PHYSICAL_ADDRESS)EventDataPtr,  //not used
                                            0,
                                            TrEEEventData);

    return Status;
}
#endif

// ->>>>>>>>>>>>>>>> NIST 800-155


#define STRUCT_FIELD_OFFSET( type, field )  \
    ((UINTN)&(((type*)0)->field))
//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   MeasureTCGPcClientSpecID
//
// Description: Includes a measurement of the TcgSpecID into PCR[0]
//
//
// Input:       IN  PEI_TCG_PPI* tcg,
//              IN  EFI_PEI_SERVICES      **ps
//
// Output:
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS MeasureTCGPcClientSpecID(
    IN CONST EFI_PEI_SERVICES **PeiServices)
{
    AMI_TREE_EVENT                    *TrEEEventData;
    EFI_STATUS                    Status;
    CHAR8                         Signature[] = "Spec ID Event03";
    UINT32                        NumOfAlg=0;
    AMI_TREE_PPI                    *TrEEPeiPpi = NULL;
    UINTN                           Size=sizeof(TCG_PCClientSpecIDEventStructEx) + sizeof(AMI_TREE_EVENT_HEADER) + sizeof(UINT32);
    UINT8                           *EventDataPtr;
    TCG_PLATFORM_SETUP_INTERFACE    *TcgPeiPolicy;
    TCG_CONFIGURATION               ConfigFlags;
    EFI_GUID                        gTcgPeiPolicyGuid =\
            TCG_PLATFORM_SETUP_PEI_POLICY_GUID;
    UINT32                          PcrBanks;


    DEBUG((DEBUG_INFO,  "TCG Pei: MeasureTCGPcClientSpecID\n"));
    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gAmiTreePpiGuid,
                 0, NULL,
                 (void **)&TrEEPeiPpi);

    if(TrEEPeiPpi == NULL || EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gTcgPeiPolicyGuid,
                 0, NULL,
                 (void **)&TcgPeiPolicy);

    if(TcgPeiPolicy == NULL || EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    Status = TcgPeiPolicy->getTcgPeiPolicy((EFI_PEI_SERVICES **)PeiServices, &ConfigFlags);
    if(EFI_ERROR(Status))return Status;

    PcrBanks = ConfigFlags.PcrBanks;   

    DEBUG((DEBUG_INFO, "TCG Pei: TCG_PcClientSpecID\n"));

    Status = (*PeiServices)->AllocatePool(PeiServices,Size, (void **)&TrEEEventData);
    if(EFI_ERROR(Status))return Status;

    EventDataPtr = (UINT8 *)TrEEEventData;
    SetMem(EventDataPtr, Size, 0);

    TrEEEventData->Header.PcrIndex = AMI_PCRi_CRTM_AND_POST_BIOS;
    TrEEEventData->Header.EventType = EV_NO_ACTION;
    TrEEEventData->Header.HeaderSize = sizeof(AMI_TREE_EVENT_HEADER);
    TrEEEventData->Header.HeaderVersion = 1;


    EventDataPtr +=  sizeof(AMI_TREE_EVENT_HEADER) + sizeof(UINT32);

    DEBUG((DEBUG_INFO, "TrEEEventData->Size = %x \n", TrEEEventData->Size ));
    DEBUG((DEBUG_INFO, "TPML_DIGEST_VALUES Size = %x \n",sizeof(TPML_DIGEST_VALUES)));

    CopyMem(
        EventDataPtr,
        Signature,
        AsciiStrLen(Signature));

    ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->PlatformClass = TCG_PLATFORM_CLASS;
    ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->SpecVersionMinor = 0x00;
    ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->SpecVersionMajor = 0x02;
    ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->SpecErrata = 0x00;
#if defined(x64_TCG) && (x64_TCG == 1)
    ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->uintnSize = 2;
#else
    ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->uintnSize = 1;
#endif

    if(ConfigFlags.Tcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_2)
    {
        if( PcrBanks & AMI_TREE_BOOT_HASH_ALG_SHA1 )
        {
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].algortihgmId = TPM2_ALG_SHA1;
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].digestSize = SHA1_DIGEST_SIZE;
            ++NumOfAlg;
        }
        if( PcrBanks & AMI_TREE_BOOT_HASH_ALG_SHA256 )
        {
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].algortihgmId = TPM2_ALG_SHA256;
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].digestSize = SHA256_DIGEST_SIZE;
            ++NumOfAlg;
        }
        if( PcrBanks & AMI_TREE_BOOT_HASH_ALG_SHA384 )
        {
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].algortihgmId = TPM2_ALG_SHA384;
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].digestSize = SHA384_DIGEST_SIZE;
            ++NumOfAlg;
        }
        if( PcrBanks & AMI_TREE_BOOT_HASH_ALG_SHA512)
        {
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].algortihgmId = TPM2_ALG_SHA512;
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].digestSize = SHA512_DIGEST_SIZE;
            ++NumOfAlg;
        }

        if( PcrBanks & AMI_TREE_BOOT_HASH_ALG_SM3 )
        {
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].algortihgmId = TPM2_ALG_SM3_256;
            ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[NumOfAlg].digestSize = SM3_256_DIGEST_SIZE;
            ++NumOfAlg;
        }
        ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->numberOfAlgorithms = NumOfAlg;
    }
    else
    {
        ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[0].algortihgmId = TPM2_ALG_SHA1;
        ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->digestSizes[0].digestSize = SHA1_DIGEST_SIZE;
        ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->numberOfAlgorithms = 1;
    }

    Size=STRUCT_FIELD_OFFSET(TCG_PCClientSpecIDEventStructEx, digestSizes)
         + ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->numberOfAlgorithms * sizeof(TCG_EFISpecIdEventAlgorithmSize) + sizeof(UINT8)
         + sizeof(AMI_TREE_EVENT_HEADER) + sizeof(UINT32);

    TrEEEventData->Size = Size;


    Size = STRUCT_FIELD_OFFSET(TCG_PCClientSpecIDEventStructEx, digestSizes);
    Size += ((TCG_PCClientSpecIDEventStructEx *)EventDataPtr)->numberOfAlgorithms * sizeof(TCG_EFISpecIdEventAlgorithmSize);
    *(UINT8*)(EventDataPtr + Size) = 0;

    EventDataPtr +=  sizeof(TCG_PCClientSpecIDEventStructEx);

    Status = TrEEPeiPpi->HashLogExtendEvent(TrEEPeiPpi,
                                            0,
                                            (EFI_PHYSICAL_ADDRESS)(UINTN)EventDataPtr,  //not used
                                            0,
                                            TrEEEventData);

    return Status;
}


#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
// <AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Name: GetRomLayout
//
// Description:
//
// Input:
//  OUT UINT8 **pRomLayout
//  OUT UINTN *Size
//
// Output:      EFI_STATUS
//
//--------------------------------------------------------------------------
// <AMI_PHDR_END>
EFI_STATUS GetRomArea(
    IN OUT AMI_ROM_AREA **pRomLayout,
    IN OUT UINTN *Size,
    IN  AMI_ROM_AREA *First
)
{
    if(pRomLayout == NULL)return EFI_INVALID_PARAMETER;
    if(First == NULL)
    {
        *pRomLayout = AmiGetFirstRomArea();
        if(*pRomLayout == NULL)return EFI_NOT_FOUND;
    }
    else
    {
        *pRomLayout = AmiGetNextRomArea(First);
        if(*pRomLayout == NULL)return EFI_NOT_FOUND;
    }

    *Size = (*pRomLayout)->Size;
    return EFI_SUCCESS;
}
#endif

#ifndef ROM_AREA_TCG_MEASURED
// Platform Firmware Volume that either contains or
// measures the EFI Boot Services and EFI Run Time Services; (code measured into TPM PCR[0])
#define ROM_AREA_TCG_MEASURED     0x000008000
#endif


//*******************************************************************************
//<AMI_PHDR_START>
//
// Procedure:   FindAndMeasureDxeCore
//
// Description:
//
// Input:
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//******************************************************************************
EFI_STATUS FindAndMeasureDxeCore(CONST EFI_PEI_SERVICES  **PeiServices)
{

    EFI_STATUS              Status;
    AMI_TREE_PPI                    *TrEEPeiPpi = NULL;
    AMI_INTERNAL_HLXE_PPI           *HashLogExtendEx = NULL;
    TCG_PLATFORM_SETUP_INTERFACE    *TcgPeiPolicy;
    TCG_CONFIGURATION               ConfigFlags;
#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
    AMI_ROM_AREA                    *RomArea = NULL;
    UINTN                           RomArea_size;
#endif
    FwVolHob                        *TpmFwVolHob;
    FwVolHob                        FwVolHobArray[10];
    UINTN                           i=0;
    UINTN                            Count=0;
    EFI_HOB_GUID_TYPE               *VolHob;

    //locate the PeiTree Protocol
    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gAmiTreePpiGuid,
                 0, NULL,
                 (void **)&TrEEPeiPpi);

    if(EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gAmiHashLogExtendExGuid,
                 0, NULL,
                 (void **)&HashLogExtendEx);

    if(EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    //create RomAreaOHob for Measurement of FV in Dxemain
#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
    Status = GetRomArea(&RomArea,&RomArea_size, NULL);
    if( (!EFI_ERROR(Status))&&(RomArea != NULL) )
    {
        for(i=0; RomArea!=NULL; i++)
        {
            
            if((RomArea->Attributes & ROM_AREA_FV_SIGNED) && (RomArea->Attributes & ROM_AREA_TCG_MEASURED) ||
                    ((RomArea->Attributes & (ROM_AREA_FV_PEI+ROM_AREA_FV_DXE)) &&
                     !(RomArea->Attributes & ROM_AREA_VITAL) ) )  // Attributes)
            {
                
                FwVolHobArray[Count].baseAddress = RomArea->Address;
                FwVolHobArray[Count].Size        = RomArea->Size;
                DEBUG ((DEBUG_INFO, "RomArea->Address = %x \n", RomArea->Address));
                DEBUG ((DEBUG_INFO, "RomArea->Size = %x \n",RomArea->Size));
                Count +=1;
            }
            
            GetRomArea(&RomArea,&RomArea_size, RomArea);
        }
    }
    else
    {
        FwVolHobArray[Count].baseAddress = FV_MAIN_BASE;
        FwVolHobArray[Count].Size = FV_MAIN_SIZE;
        Count+=1;
    }
#else
    FwVolHobArray[Count].baseAddress = FV_MAIN_BASE;
    FwVolHobArray[Count].Size = FV_MAIN_SIZE;
    Count+=1;
#endif

    Status = InternalPeiBuildHobGuid((EFI_PEI_SERVICES **)PeiServices, &amiFVhoblistguid,
                                     (sizeof(FwVolHob)*Count),   (void **)&VolHob);
    if (EFI_ERROR(Status))
    {
        return Status;
    }

    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gTcgPeiPolicyGuid,
                 0, NULL,
                 (void **)&TcgPeiPolicy);

    if(TcgPeiPolicy == NULL || EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    Status = TcgPeiPolicy->getTcgPeiPolicy((EFI_PEI_SERVICES **)PeiServices, &ConfigFlags);
    if(EFI_ERROR(Status))return Status;

    TpmFwVolHob = (FwVolHob*)(VolHob + 1);
    for(i=0; i<Count; i++)
    {

        if(i==0)
        {
            TpmFwVolHob->Size = FwVolHobArray[i].Size;
            TpmFwVolHob->baseAddress = FwVolHobArray[i].baseAddress;
            TpmFwVolHob->Count = Count;
            TpmFwVolHob->Tcg2SpecVersion = (UINTN)ConfigFlags.Tcg2SpecVersion;
            TpmFwVolHob->PcrBanks = ConfigFlags.PcrBanks;
        }
        else
        {
            TpmFwVolHob->Size = FwVolHobArray[i].Size;
            TpmFwVolHob->baseAddress = FwVolHobArray[i].baseAddress;
            TpmFwVolHob->Count = 0;
            TpmFwVolHob->Tcg2SpecVersion = 0;
            TpmFwVolHob->PcrBanks = 0;
        }

        DEBUG ((DEBUG_INFO, "TpmFwVolHob->Size = %x \n", TpmFwVolHob->Size));
        DEBUG ((DEBUG_INFO, "TpmFwVolHob->baseAddress = %x \n",TpmFwVolHob->baseAddress));
        DEBUG ((DEBUG_INFO, "TpmFwVolHob->Tcg2SpecVersion = %x \n",TpmFwVolHob->Tcg2SpecVersion));
        DEBUG ((DEBUG_INFO, "TpmFwVolHob address = %x \n", TpmFwVolHob));
        
        TpmFwVolHob+=1;
    }
    
    //For meet NIST golden measurement verification
#if defined(SAVE_ENTIRE_FV_IN_MEM) && (SAVE_ENTIRE_FV_IN_MEM == 0)
    MeasureLogDxeFwVol(
            PeiServices,
            (TpmFwVolHob-Count),
            TrEEPeiPpi,
            HashLogExtendEx
            );
#endif

    return Status;
}






//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   AmiTcgPlatformPEI_EntryBeforeMem
//
// Description: Installs AMIplatform PPI for initialization in PEI before
//              memory is installed
//
// Input:        IN EFI_FFS_FILE_HEADER *FfsHeader,
//               IN EFI_PEI_SERVICES    **PeiServices
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI AmiTpm20PlatformPeiEntry(
    IN EFI_PEI_FILE_HANDLE  FileHandle,
    IN CONST EFI_PEI_SERVICES    **PeiServices
)
{
    EFI_STATUS Status;
    AMI_TREE_PPI *TrEEPeiPpi = NULL;
#if FTpmPlatformProfile == 1
    UINTN  Info;
#endif
    TCG_PLATFORM_SETUP_INTERFACE    *TcgPeiPolicy;
    TCG_CONFIGURATION               ConfigFlags;
    EFI_BOOT_MODE       BootMode;

#if FTpmPlatformProfile == 1
    Status = EFI_SUCCESS;
    if(!EFI_ERROR(iTpmGetInfo(&Info)))
    {
        Status = (**PeiServices).NotifyPpi (PeiServices, &AmdMemDiscCallback);
        return EFI_SUCCESS;
    }

#endif

    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gAmiTreePpiGuid,
                 0, NULL,
                 (void **)&TrEEPeiPpi);

    if(EFI_ERROR(Status) || TrEEPeiPpi == NULL )return Status;

    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gTcgPeiPolicyGuid,
                 0, NULL,
                 (void **)&TcgPeiPolicy);

    if(TcgPeiPolicy == NULL || EFI_ERROR(Status))
    {
        return EFI_NOT_FOUND;
    }

    Status = TcgPeiPolicy->getTcgPeiPolicy((EFI_PEI_SERVICES **)PeiServices, &ConfigFlags);
    if(EFI_ERROR(Status))return Status;

    Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
    if(EFI_ERROR(Status))return Status;

    //send TPM 2.0 Startup
    Status = Tpm20PeiSendStartup(PeiServices, TrEEPeiPpi, BootMode);
    if( EFI_ERROR(Status) )
    {
        return EFI_SUCCESS;
    }

    if(BootMode == BOOT_ON_S3_RESUME)
    {
        return EFI_SUCCESS;
    }
       
    if(BootMode == BOOT_IN_RECOVERY_MODE){
        
        Status = CapAllTpm2Pcrs(PeiServices, TrEEPeiPpi, 0);
        DEBUG ((DEBUG_INFO, "Tcg2 Boot in recovery CapPcrs Status =%r \n", Status));
        
        Status = AmiPlatformTpmPei2HierarchyControl(PeiServices, TrEEPeiPpi, TPM_RH_PLATFORM,\
                                                        NULL, TPM_RH_ENDORSEMENT,  0);
        if(EFI_ERROR(Status)){
            DEBUG ((DEBUG_INFO, "Tcg2 Boot in recovery Disable EH Status =%r \n", Status));
        }
        Status = AmiPlatformTpmPei2HierarchyControl(PeiServices, TrEEPeiPpi, TPM_RH_PLATFORM,\
                                                        NULL, TPM_RH_OWNER,  0);
        if(EFI_ERROR(Status)){
            DEBUG ((DEBUG_INFO, "Tcg2 Boot in recovery Disable SH Status =%r \n", Status));
        }
    }

    if(ConfigFlags.Tcg2SpecVersion == TCG2_PROTOCOL_SPEC_TCG_2)
    {
        Status =  MeasureTCGPcClientSpecID(PeiServices);
        if ( EFI_ERROR( Status ))
        {
            DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
        }
    }
    
#if defined(TcgNistSp800_155_SUPPORT) && (TcgNistSp800_155_SUPPORT == 1)
    Status =  MeasureNISTManifest(PeiServices);
#endif
    
    Status = Tpm20MeasureCRTMVersionFuncPtr( PeiServices, TrEEPeiPpi);

    if ( !EFI_ERROR( Status ))
    {
#if defined (AmiTrustedFv_Support) && (AmiTrustedFv_Support == 1)
        Status = PeiServicesNotifyPpi (&mTpm20EndOfPeiSignalPpiNotifyList[0]);
#else
        Status = FindAndMeasureDxeCore(PeiServices);
#endif
    }
    return Status;

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
