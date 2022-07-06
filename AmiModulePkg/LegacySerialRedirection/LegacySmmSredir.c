//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file LegacySmmSredir.c
    Legacy console redirection SMM support
*/

//---------------------------------------------------------------------------

#include "Token.h"
#include "Protocol/AmiLegacySredir.h"
#include "LegacySredir.h"
#include "AmiDxeLib.h"
#include <Protocol/DevicePath.h>
#include <Protocol/LoadedImage.h>
#include <AmiSmm.h>
#include <Protocol/SmmCpu.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Library/AmiBufferValidationLib.h>
#include "AcpiModeEnable.h"

//---------------------------------------------------------------------------

#define RETURN(status)  {return status;}
EFI_SMM_BASE2_PROTOCOL  *gSmmBase2;
EFI_SMM_CPU_PROTOCOL    *gSmmCpu = NULL;
UINT8                   gMmioWidth=0;
BOOLEAN                 gDontServiceSmi = FALSE;
UINTN                   gMmioAddress = 0;
EFI_GUID gEfiAcpiEnDispatchProtocolGuid = EFI_ACPI_EN_DISPATCH_PROTOCOL_GUID;
EFI_GUID gEfiAcpiDisDispatchProtocolGuid = EFI_ACPI_DIS_DISPATCH_PROTOCOL_GUID;
EFI_GUID gAmiLegacySredirCommunicationGuid = AMI_LEGACY_SREDIR_COMMUNICATION_GUID;

#pragma pack(1)
typedef struct {
    UINT32          MMIOAddress;
    UINT8           FuncNo;
    UINT8           ReadWrite;      
    UINT8           Offset;
    UINT8           Value;
    UINT8           Count;
    UINT32          BufferAddress;
    UINT8           MmioWidth;
} SREDIR_INPUT_PARAMETER;
#pragma pack()

EFI_STATUS
EFIAPI
LegacySredirSmmEntryPoint (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
);

EFI_STATUS
EFIAPI
LegacySredirSMIHandler (
    IN  EFI_HANDLE                  DispatchHandle,
    IN  CONST VOID                  *Context OPTIONAL,
    IN  OUT VOID                    *CommBuffer OPTIONAL,
    IN  OUT UINTN                   *CommBufferSize OPTIONAL
);


/**
    @internal
    
    Read the Data from Serial Port

    @param SREDIR_INPUT_PARAMETER SredirParam
    @retval EFI_SUCCESS
    
    @endinternal
*/
EFI_STATUS
ReadSerialPort (
    IN  OUT SREDIR_INPUT_PARAMETER  *SredirParam
)
{

    UINT32  TempValue;
    
    if (gMmioWidth == DWORD_WIDTH) {
        TempValue=*(UINT32*)(gMmioAddress+(SredirParam->Offset*DWORD_WIDTH));
    } else if (gMmioWidth == WORD_WIDTH ) {
        TempValue=*(UINT16*)(gMmioAddress+(SredirParam->Offset*WORD_WIDTH));
    } else {
        TempValue=*(UINT8*)(gMmioAddress+(SredirParam->Offset*BYTE_WIDTH));
    }
     
    SredirParam->Value=(UINT8)TempValue;
    return EFI_SUCCESS;
}

/**
    @internal
    
    Write the Data to Serial Port

    @param SREDIR_INPUT_PARAMETER SredirParam 

    @retval EFI_SUCCESS
    
    @endinternal
*/
EFI_STATUS
WriteSerialPort (
    IN  OUT SREDIR_INPUT_PARAMETER  *SredirParam
)
{
    UINT32  TempValue=(UINT32)SredirParam->Value;

    if (gMmioWidth == DWORD_WIDTH) {
        *(UINT32*)(gMmioAddress+(SredirParam->Offset*DWORD_WIDTH))=TempValue;
    } else if (gMmioWidth == WORD_WIDTH ) {
        *(UINT16*)(gMmioAddress+(SredirParam->Offset*WORD_WIDTH))=(UINT16)TempValue;
    } else {
        *(UINT8*)(gMmioAddress+(SredirParam->Offset*BYTE_WIDTH))=(UINT8)TempValue;
    }
    
    return EFI_SUCCESS;
}

/**
    @internal
    
    Write the buffer of data to Serial port

    @param SREDIR_INPUT_PARAMETER SredirParam 

    @retval EFI_SUCCESS
    
    @endinternal
*/
EFI_STATUS
WriteBufferSerialPort (
    IN  OUT SREDIR_INPUT_PARAMETER    *SredirParam
)
{
    UINT8       i;
    UINT32      TempValue=0;
    UINT8       *DataBuffer=(UINT8*)SredirParam->BufferAddress;
    EFI_STATUS  Status;

    if(SredirParam->Count == 0) {
        return EFI_SUCCESS;
    }

    if( !DataBuffer ) {
        return EFI_INVALID_PARAMETER;
    }

    // Validate Input buffer is valid and not resides in SMRAM region
    Status = AmiValidateMemoryBuffer ( (VOID*)SredirParam->BufferAddress, SredirParam->Count );
    if( Status != EFI_SUCCESS ) {
        RETURN(Status);
    }

    for(i=0;i<SredirParam->Count;i++) {
        TempValue=*DataBuffer;

        if (gMmioWidth == DWORD_WIDTH) {
            *(UINT32*)(gMmioAddress+(SredirParam->Offset*DWORD_WIDTH))=TempValue;
        } else if (gMmioWidth == WORD_WIDTH ) {
            *(UINT16*)(gMmioAddress+(SredirParam->Offset*WORD_WIDTH))=(UINT16)TempValue;
        } else {
            *(UINT8*)(gMmioAddress+(SredirParam->Offset*BYTE_WIDTH))=(UINT8)TempValue;
        }
        
        DataBuffer++;
    }

    return EFI_SUCCESS;
}
 
/**
    Legacy Serial Redirection Smm handler function

    @param DispatchHandle  - EFI Handle
    @param DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
    @param CommBuffer
    @param CommBufferSize

    @retval EFI_STATUS
*/

EFI_STATUS
EFIAPI
LegacySredirSMIHandler (
    IN  EFI_HANDLE  DispatchHandle,
    IN  CONST VOID  *Context OPTIONAL,
    IN  OUT VOID    *CommBuffer OPTIONAL,
    IN  OUT UINTN   *CommBufferSize OPTIONAL
)

{
    EFI_STATUS    Status = EFI_SUCCESS;
    UINT64        pCommBuff;
    UINT32        HighBufferAddress = 0;
    UINT32        LowBufferAddress = 0;
    UINTN         Cpu = (UINTN)-1;
    SREDIR_INPUT_PARAMETER  *SredirParam;

    // return if servicing this SMI is out of scope, i.e after acpi enable/disable call
    // or if Mmio address is not valid.
    if ( gDontServiceSmi || !gMmioAddress ) {
        return EFI_SUCCESS;
    }
    
    if (CommBuffer != NULL && CommBufferSize != NULL) {
        Cpu = ((EFI_SMM_SW_CONTEXT*)CommBuffer)->SwSmiCpuIndex;
    }

    //
    // Found Invalid CPU number, return
    //
    if(Cpu == (UINTN)-1) RETURN(Status);

    gSmmCpu->ReadSaveState ( gSmmCpu, \
                                      4, \
                                      EFI_SMM_SAVE_STATE_REGISTER_RBX, \
                                      Cpu, \
                                      &LowBufferAddress );
    gSmmCpu->ReadSaveState ( gSmmCpu, \
                                      4, \
                                      EFI_SMM_SAVE_STATE_REGISTER_RCX, \
                                      Cpu, \
                                      &HighBufferAddress );

    pCommBuff            = HighBufferAddress;
    pCommBuff            = Shl64(pCommBuff, 32);
    pCommBuff            += LowBufferAddress;
    
    // Validating the pCommBuff address not resides in SMRAM region
    Status = AmiValidateMemoryBuffer((UINT8*)pCommBuff, sizeof(SREDIR_INPUT_PARAMETER));
    if(EFI_ERROR(Status)) RETURN(Status);

    SredirParam          = (SREDIR_INPUT_PARAMETER *)pCommBuff; 
    
    if( SredirParam->Offset > 7) {
        return EFI_INVALID_PARAMETER;
    }
    
    // MMIO Width is decoded based on the AMI_COM_PARAMETERS
    gMmioWidth = SredirParam->MmioWidth;

    // Validate the MmioWidth Input argument.
    if( (gMmioWidth != BYTE_WIDTH) && (gMmioWidth != WORD_WIDTH) && (gMmioWidth != DWORD_WIDTH) ) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Validate COM port register's MMIO address space are valid and not reside in SMRAM region
    Status = AmiValidateMmioBuffer( (VOID*)gMmioAddress, 8*gMmioWidth );
    if( Status != EFI_SUCCESS ) {
        SredirParam->Value=0;
        RETURN(Status);
    }


    switch(SredirParam->FuncNo)    {

        case 0x1:
                ReadSerialPort(SredirParam);
                break;

        case 0x2:
                WriteSerialPort(SredirParam);
                break;

        case 0x3:
                Status = WriteBufferSerialPort(SredirParam);
                break;

        default: 
                return EFI_INVALID_PARAMETER;

    }

    RETURN(Status);
}


/**
    @internal
    
    ACPI Enable/Disable callback to set gDontServiceSmi variable
    so that we don't service SMI after that which is not valid.

    @param DispatchHandle  - EFI Handle

    @retval VOID

    @endinternal
**/

VOID
EFIAPI
AcpiSredirEnableDisableCallBack(
    IN EFI_HANDLE   DispatchHandle
)
{
    gDontServiceSmi = TRUE;
}

/**
    Register for ACPI Enable/Disable protocol to register for ACPI
    enable/disable call.

    @param This  - Pointer to EFI_ACPI_DISPATCH_PROTOCOL
    @param Function  - EFI_ACPI_DISPATCH function
    @param Handle  - EFI Handle

    @retval Status
**/

EFI_STATUS
EFIAPI
RegisterSredirAcpiEnableCallBack(
    IN EFI_ACPI_DISPATCH_PROTOCOL   *This,
    IN EFI_ACPI_DISPATCH            Function,
    OUT EFI_HANDLE                  *Handle
)
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      RegisterHandle;
    EFI_ACPI_DISPATCH_PROTOCOL      *AcpiEnDispatch;
    EFI_ACPI_DISPATCH_PROTOCOL      *AcpiDisDispatch;

    Status = pSmst->SmmLocateProtocol(&gEfiAcpiEnDispatchProtocolGuid, NULL, &AcpiEnDispatch);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = AcpiEnDispatch->Register(AcpiEnDispatch, AcpiSredirEnableDisableCallBack, &RegisterHandle);
    if(EFI_ERROR(Status)) {
        return Status;
    }
    
    Status = pSmst->SmmLocateProtocol(&gEfiAcpiDisDispatchProtocolGuid, NULL, &AcpiDisDispatch);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = AcpiDisDispatch->Register(AcpiDisDispatch, AcpiSredirEnableDisableCallBack, &RegisterHandle);
    return Status;
}


/** 
    SMI Handler to get Mmio Address from DXE driver.

    @param   DispatchHandle,
    @param   *Context         OPTIONAL,
    @param   *CommBuffer      OPTIONAL,
    @param   *CommBufferSize  OPTIONAL

    @retval  EFI_STATUS
**/

EFI_STATUS
EFIAPI
InitializeMmioAddress (
    IN  EFI_HANDLE  DispatchHandle,
    IN  CONST   VOID    *Context        OPTIONAL,
    IN  OUT     VOID    *CommBuffer     OPTIONAL,
    IN  OUT     UINTN   *CommBufferSize OPTIONAL
  )
{
    
    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL) {
        return EFI_SUCCESS;
    }

    gMmioAddress = *(UINTN *)CommBuffer;
    
	// UnRegister smi handler as we have got MMIO Address
    return pSmst->SmiHandlerUnRegister (DispatchHandle);

}

/**
    This function is called from SMM during SMM registration.
   
    @param ImageHandle 
    @param SystemTable 

    @retval EFI_STATUS
*/
EFI_STATUS
LegacySredirInSmmFunction (
    EFI_HANDLE          ImageHandle,
    EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_STATUS  Status;
    EFI_HANDLE                      SwHandle = NULL;

    EFI_SMM_SW_DISPATCH2_PROTOCOL    *pSwDispatch;

    EFI_SMM_SW_REGISTER_CONTEXT     SwContext;
    EFI_ACPI_DISPATCH_PROTOCOL      *AcpiEnDispatch;
    VOID                            *Reg;
    EFI_HANDLE                      DispatchHandle;    

    Status = InitAmiSmmLib( ImageHandle, SystemTable );
    if (EFI_ERROR(Status)) return Status;

    Status = pBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, &gSmmBase2);

    if (EFI_ERROR(Status)) return Status;

    Status = pSmmBase->GetSmstLocation (gSmmBase2, &pSmst);
    if (EFI_ERROR(Status)) return Status;

    Status = pSmst->SmmLocateProtocol( \
                        &gEfiSmmSwDispatch2ProtocolGuid, NULL, &pSwDispatch);
    if (EFI_ERROR(Status)) return Status;

    Status = pSmst->SmmLocateProtocol(&gEfiSmmCpuProtocolGuid, NULL, &gSmmCpu);
    if (EFI_ERROR(Status)) return Status;

    SwContext.SwSmiInputValue = LEGACY_SREDIR_SWSMI;
    Status = pSwDispatch->Register (pSwDispatch, LegacySredirSMIHandler, &SwContext, &SwHandle);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // Register for ACPI callback
    Status = pSmst->SmmLocateProtocol(&gEfiAcpiEnDispatchProtocolGuid, NULL, &AcpiEnDispatch);
    if (!EFI_ERROR(Status)) {
        RegisterSredirAcpiEnableCallBack(NULL, NULL, NULL);
    } else {
        Status = pSmst->SmmRegisterProtocolNotify(
                        &gEfiAcpiEnDispatchProtocolGuid,
                        RegisterSredirAcpiEnableCallBack,
                        &Reg);                
    }
    
    // Register handler to transfer data from DXE driver to SMM
    Status = pSmst->SmiHandlerRegister (
                      InitializeMmioAddress,
                      &gAmiLegacySredirCommunicationGuid,
                      &DispatchHandle
                      );

    return Status;
}

/**
    @internal
     
    Legacy Serial Redirection  Smm entry point

    @param Standard EFI Image entry - EFI_IMAGE_ENTRY_POINT
    @param EFI System Table - Pointer to System Table

    @retval EFI_STATUS OR EFI_NOT_FOUND

    @endinternal
*/

EFI_STATUS
EFIAPI
LegacySredirSmmEntryPoint (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
)
{
    InitAmiLib(ImageHandle, SystemTable);
    return InitSmmHandler(ImageHandle, SystemTable, LegacySredirInSmmFunction, NULL);
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
