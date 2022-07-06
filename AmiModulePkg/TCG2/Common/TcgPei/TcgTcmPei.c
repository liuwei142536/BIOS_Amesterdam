//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TcgNext/Common/TcgPei/TcgTcmPei.c 1     10/08/13 12:02p Fredericko $
//
// $Revision: 1 $
//
// $Date: 10/08/13 12:02p $
//*************************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Modules/TcgNext/Common/TcgPei/TcgTcmPei.c $
//*************************************************************************
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  TcgPei.c
//
// Description:
//  Functions for early initialization of TPM are executed here
//
//<AMI_FHDR_END>
//*************************************************************************
#include <Efi.h>
#include <AmiTcg/Tcm.h>
#include "AmiTcg/TcgCommon12.h"
#include <AmiTcg/sha1.h>
#include <AmiTcg/TCGMisc.h>
#include <Token.h>
#include <AmiTcg/tcg.h>
#include <AmiTcg/TcgPc.h>
#include <Ppi/TcgTcmService.h>
#include <Ppi/TcgService.h>
#include <Ppi/TpmDevice.h>
#include "Ppi/CpuIo.h"
#include "Ppi/LoadFile.h"
#include <Ffs.h>
#include <Library/DebugLib.h>
#include <Guid/AmiTcgGuidIncludes.h>

//*********************************************************************
//                      GLOBAL DEFINITIONS
//*********************************************************************

TCM_PC_REGISTERS_PTR      TcmBaseReg = (TCM_PC_REGISTERS_PTR)(UINTN)PORT_TPM_IOMEMBASE;
    
#pragma pack(1)
typedef struct _TCG_PEI_CALLBACK_CONTEXT
{
    PEI_TPM_PPI      *TpmDevice;
    EFI_PEI_SERVICES **PeiServices;
} TCG_PEI_CALLBACK_CONTEXT;
#pragma pack()


EFI_STATUS TcgGetNextGuidHob(
    IN OUT VOID          **HobStart,
    IN EFI_GUID          * Guid,
    OUT VOID             **Buffer,
    OUT UINTN            *BufferSize OPTIONAL );

EFI_STATUS
EFIAPI TcmPeiLogEvent(
    IN AMI_PEI_TCM_INTERFACE      *This,
    IN EFI_PEI_SERVICES **PeiServices,
    IN TCM_PCR_EVENT    *Event,
    OUT UINT32          *EventNum );

EFI_STATUS
EFIAPI TcgPeiHashLogExtendEventTcm(
    IN AMI_PEI_TCM_INTERFACE       *This,
    IN EFI_PEI_SERVICES  **PeiServices,
    IN UINT8             *HashData,
    IN UINT32            HashDataLen,
    IN OUT TCM_PCR_EVENT *NewEvent,
    OUT UINT32           *EventNum );


static
EFI_STATUS
EFIAPI FillCallbackContext(
    IN EFI_PEI_SERVICES          **PeiService,
    OUT TCG_PEI_CALLBACK_CONTEXT *CallbackContext )
{
    CallbackContext->PeiServices = PeiService;
    return (*PeiService)->LocatePpi(
               (CONST EFI_PEI_SERVICES    **)PeiService,
               &gPeiTpmPpiGuid,
               0,
               NULL,
               (void **)&CallbackContext->TpmDevice
           );
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   TcmPeiGetEventLog
//
// Description: Helper function for logging TCM events
//
// Input:        IN EFI_PEI_SERVICES **PeiServices,
//               OUT TCG_LOG_HOB     **EventLog
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
EFIAPI TcmPeiGetEventLog(
    IN EFI_PEI_SERVICES **PeiServices,
    OUT TCG_LOG_HOB     **EventLog )
{
    return EFI_UNSUPPORTED;
}


//**********************************************************************
//<AMI_PHDR_START>
//
// Name:  TcgPeiPassThroughToTcm
//
// Description: TCGPEI common function to abstract passing commands to the TPM
//                FIFO
//
// Input:       IN   *This
//              IN   **PeiServices
//              IN   TpmInputParameterBlockSize
//              IN   *TpmInputParameterBlock
//              IN   TpmOutputParameterBlockSize
//              IN   *TpmOutputParameterBlock
//
// Output:      EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI TcgPeiPassThroughToTcm(
    IN AMI_PEI_TCM_INTERFACE   *This,
    IN EFI_PEI_SERVICES **PeiServices,
    IN UINT32           TpmInputParameterBlockSize,
    IN UINT8            *TpmInputParameterBlock,
    IN UINT32           TpmOutputParameterBlockSize,
    IN UINT8            *TpmOutputParameterBlock )
{
    EFI_STATUS              Status;
    PEI_TCM_PPI             *TcmPpi;

    Status = (*PeiServices)->LocatePpi ((CONST EFI_PEI_SERVICES**)PeiServices,\
                                                &gPeiTcmPpiGuid, 0, NULL, (void **)&TcmPpi );
    
    if ( EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Locate TcgPeiPassThroughToTcm Status == %r\n",Status));
      return Status;
    }

    return TcmPpi->TcmLibPassThrough(TcmBaseReg, TpmInputParameterBlock,\
                                     TpmInputParameterBlockSize, TpmOutputParameterBlock,
                                     TpmOutputParameterBlockSize);
}




static AMI_PEI_TCM_INTERFACE            mAmiTcmPpiInterface =
{
    TcgPeiHashLogExtendEventTcm,
    TcmPeiLogEvent,
    TcgPeiPassThroughToTcm
};


static EFI_PEI_PPI_DESCRIPTOR mTcmPpiList[] =
{
    {
        EFI_PEI_PPI_DESCRIPTOR_PPI
        | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gPeiTcgPpiGuid,
        &mAmiTcmPpiInterface
    }
};


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:  IntTcmTcgPeiGetCapabilities
//
// Description: Executes TCM operation to read capabilities
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      TCM_Capabilities_PermanentFlag
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
TCM_Capabilities_PermanentFlag IntTcmTcgPeiGetCapabilities(
    IN EFI_PEI_SERVICES **PeiServices )
{
    TCM_Capabilities_PermanentFlag * cap = NULL;
    EFI_STATUS                     Status;
    TPM_GetCapabilities_Input      cmdGetCap;
    UINT8                          result[0x100];
    PEI_TCM_PPI                   *TcmPpi;

    Status = (*PeiServices)->LocatePpi ((CONST EFI_PEI_SERVICES**)PeiServices,
                                            &gPeiTcmPpiGuid, 0, NULL, (void **)&TcmPpi );
    
    if(EFI_ERROR(Status)){
        (*PeiServices)->SetMem(result, sizeof(TCM_Capabilities_PermanentFlag),0);
        cap = (TCM_Capabilities_PermanentFlag*)result;
        return *cap;
    }

    cmdGetCap.Tag         = TPM_H2NS( TCM_TAG_RQU_COMMAND );
    cmdGetCap.ParamSize   = TPM_H2NL( sizeof (cmdGetCap));
    cmdGetCap.CommandCode = TPM_H2NL( TCM_ORD_GetCapability );
    cmdGetCap.caparea     = TPM_H2NL( TCM_CAP_FLAG );
    cmdGetCap.subCapSize  = TPM_H2NL( 4 ); // subCap is always 32bit long
    cmdGetCap.subCap      = TPM_H2NL( TCM_CAP_FLAG_PERMANENT );

    Status = TcmPpi->TcmLibPassThrough(
                 TcmBaseReg,
                 (UINT8*)&cmdGetCap,
                 sizeof(cmdGetCap),
                 result, 
                 0x100);
    
    if(EFI_ERROR(Status)){
        (*PeiServices)->SetMem(result, sizeof(TCM_Capabilities_PermanentFlag),0);
        cap = (TCM_Capabilities_PermanentFlag*)result;
        return *cap;
    }
    
    cap = (TCM_Capabilities_PermanentFlag*)result;

    DEBUG((DEBUG_INFO,
           "GetCapability: %r; size: %x; retCode:%x; tag:%x; bytes %08x\n",
           Status,TPM_H2NL( cap->ParamSize ), TPM_H2NL(cap->RetCode ),
           (UINT32)TPM_H2NS(cap->tag ), TPM_H2NL( *(UINT32*)&cap->disabled )));

    return *cap;
}


//**********************************************************************
//<AMI_PHDR_START>
// Name:  TcmPeiLogEvent
//
// Description: TCM common function to log PEI events
//
// Input:       IN   *This
//              IN   **PeiServices
//              IN   *Event,
//              IN   *EventNum
//
// Output:     EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//*********************************************************************
EFI_STATUS
EFIAPI TcmPeiLogEvent(
    IN AMI_PEI_TCM_INTERFACE      *This,
    IN EFI_PEI_SERVICES **PeiServices,
    IN TCM_PCR_EVENT    *Event,
    OUT UINT32          *EventNum )
{
    return EFI_UNSUPPORTED;
}



//**********************************************************************
//<AMI_PHDR_START>
// Name:  TcgPeiHashLogExtendEventTcm
//
// Description: Tcm common function to Hash, Log and Extend data
//
// Input:       IN  *This
//              IN  **PeiServices
//              IN  *HashData
//              IN  HashDataLen
//              IN  *NewEvent,
//              IN  *EventNum
//
// Output:     EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//*********************************************************************
EFI_STATUS
EFIAPI TcgPeiHashLogExtendEventTcm(
    IN AMI_PEI_TCM_INTERFACE       *This,
    IN EFI_PEI_SERVICES  **PeiServices,
    IN UINT8             *HashData,
    IN UINT32            HashDataLen,
    IN OUT TCM_PCR_EVENT *NewEvent,
    OUT UINT32           *EventNum )
{
    return EFI_UNSUPPORTED;
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   TcgTcmPeiEntry
//
// Description: Entry point for TCM device initialization
//
//
// Input:       IN      EFI_FFS_FILE_HEADER       *FfsHeader
//              IN      EFI_PEI_SERVICES          **PeiServices,
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
EFIAPI TcgTcmPeiEntry(
    IN EFI_PEI_FILE_HANDLE  FileHandle,
    IN CONST EFI_PEI_SERVICES    **PeiServices)
{
    EFI_STATUS Status;

    Status = (*PeiServices)->InstallPpi( PeiServices, mTcmPpiList );
    return Status;
}
