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
// $Header: /Alaska/SOURCE/Modules/TCG/EMpTcmDxe.c 4     5/09/12 6:40p Fredericko $
//
// $Revision: 4 $
//
// $Date: 5/09/12 6:40p $
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  EMpTpmDxe.c
//
// Description:
//        Contains function that help with making legacy TCG calls
//
//<AMI_FHDR_END>
//*************************************************************************
#include <Efi.h>
#include "AmiTcg/TcgTpm12.h"
#include <AmiTcg/TpmLib.h>
#include "Protocol/TpmDevice.h"
#include "AmiTcg/TCGMisc.h"
#include<Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <AmiTcg/Tcmdxe.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/AmiTcgGuidIncludes.h>
#include "Token.h"

extern EFI_GUID gEfiTcgMpDriverHobGuid;
TCM_PC_REGISTERS_PTR      TcmBaseReg = (TCM_PC_REGISTERS_PTR)(UINTN)PORT_TPM_IOMEMBASE;

#define _CR( Record, TYPE,\
     Field )((TYPE*) ((CHAR8*) (Record)- (CHAR8*) &(((TYPE*) 0)->Field)))

#define TPM_DXE_PRIVATE_DATA_FROM_THIS( this )\
    _CR( this, TPM_DXE_PRIVATE_DATA, TpmInterface )

typedef struct _TPM_DXE_PRIVATE_DATA
{
    EFI_TPM_DEVICE_PROTOCOL TpmInterface;
} TPM_DXE_PRIVATE_DATA;

EFI_TCMSDRV_PROTOCOL    *TcmDrvProtocol = NULL;

//**********************************************************************
//<AMI_PHDR_START>
//
// Name: EMpTcmDxeInit
//
// Description: Call to OEM driver to initialize TPM.
//
// Input:       IN  EFI_TPM_DEVICE_PROTOCOL   *This
//
// Returns:     EFI_STATUS
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
EFIAPI EMpTcmDxeInit(
    IN EFI_TPM_DEVICE_PROTOCOL *This )
{
    if(TcmDrvProtocol != NULL)
        return EFI_SUCCESS;
    else
        return EFI_NOT_FOUND;
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Name:  EMpTcmDxeClose
//
// Description: Close TPM connection for locality 0
//
// Input:       IN  PEI_TPM_PPI               *This,
//              IN  EFI_PEI_SERVICES          **PeiServices
//
// Output:      EFI STATUS
///
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
EFIAPI EMpTcmDxeClose(
    IN EFI_TPM_DEVICE_PROTOCOL *This )
{
    if(TcmDrvProtocol != NULL)
        return EFI_SUCCESS;
    else
        return EFI_NOT_FOUND;
}




//**********************************************************************
//<AMI_PHDR_START>
//
//
// Name:  EMpTcmDxeGetStatusInfo
//
// Description: GetStatus Info from MP driver
//
// Input:       IN  PEI_TPM_PPI               *This,
//              IN  EFI_PEI_SERVICES          **PeiServices
//
// Output:      EFI STATUS
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
EFIAPI  EMpTcmDxeGetStatusInfo(
    IN EFI_TPM_DEVICE_PROTOCOL   * This
)
{
    if(TcmDrvProtocol != NULL)
       return EFI_SUCCESS;
   else
       return EFI_NOT_FOUND;
}


//**********************************************************************
//<AMI_PHDR_START>
//
//
// Name:  EMpTcmDxeTransmit
//
// Description: Dxe Transmit Tcg Data
//
// Input:    IN      EFI_TPM_DEVICE_PROTOCOL   *This,
//           IN      UINTN                     NoInBuffers,
//           IN      TPM_TRANSMIT_BUFFER       *InBuffers,
//           IN      UINTN                     NoOutBuffers,
//           IN OUT  TPM_TRANSMIT_BUFFER       *OutBuffers
//
// Output:     EFI STATUS
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
EFIAPI EMpTcmDxeTransmit(
    IN EFI_TPM_DEVICE_PROTOCOL *This,
    IN UINTN                   NoInBuffers,
    IN TPM_TRANSMIT_BUFFER     *InBuffers,
    IN UINTN                   NoOutBuffers,
    IN OUT TPM_TRANSMIT_BUFFER *OutBuffers )
{
    EFI_STATUS      Status;
    UINTN           InbufferSize=0, OutbufferSize=0;
    UINT8           *CmdBuffer, *RespBuff, *BufferPtr;
    UINTN           i=0;
    
    if(TcmDrvProtocol == NULL)
        return EFI_NOT_FOUND;
    
    for(i=0; i< NoInBuffers; i++)
    {
        InbufferSize += InBuffers[i].Size;
    }
    
    for(i=0; i< NoOutBuffers; i++)
    {
        OutbufferSize += OutBuffers[i].Size;
    }
    
    Status = gBS->AllocatePool (EfiBootServicesData, InbufferSize, (void **) &CmdBuffer );
    if(EFI_ERROR(Status))return Status;
      
    Status = gBS->AllocatePool (EfiBootServicesData, OutbufferSize, (void **) &RespBuff );
    if(EFI_ERROR(Status))return Status;
    
    BufferPtr = CmdBuffer;
    
    for (i=0; i < NoInBuffers; i++ )
    {
        CopyMem(BufferPtr, InBuffers[i].Buffer,\
                                    InBuffers[i].Size);
        BufferPtr+=InBuffers[i].Size;
    }

    Status=  TcmDrvProtocol->TcmLibPassThrough(TcmBaseReg, CmdBuffer, (UINT32)InbufferSize,\
                                                    RespBuff, (UINT32)OutbufferSize);
    if(EFI_ERROR(Status))return Status;
    
    for (i=0; i < NoOutBuffers; i++ )
    {
        CopyMem(OutBuffers[i].Buffer, BufferPtr, \
                                    OutBuffers[i].Size);
        BufferPtr+=OutBuffers[i].Size;
    }
        
    return EFI_SUCCESS;
}




static TPM_DXE_PRIVATE_DATA mTpmPrivate =
{
    {
        EMpTcmDxeInit,
        EMpTcmDxeClose,
        EMpTcmDxeGetStatusInfo,
        EMpTcmDxeTransmit
    }
};



//**********************************************************************
//<AMI_PHDR_START>
//
//
// Name:  TcmDxeEntry
//
// Description: TpmDxeEntry, Init TpmDxeDriver and install protocol
//
// Input:      EFI_HANDLE                ImageHandle
//             EFI_SYSTEM_TABLE          *SystemTable
//
// Output:     EFI STATUS
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
EFIAPI TcmDxeEntry(
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS           Status = EFI_NOT_FOUND;

    Status = gBS->LocateProtocol (&gEfiTcmMPProtocolGuid,  NULL, (void **)&TcmDrvProtocol);
    if(EFI_ERROR(Status))return Status;
    
    Status = gBS->InstallMultipleProtocolInterfaces(
                         &ImageHandle,
                         &gEfiTpmDeviceProtocolGuid,
                         &mTpmPrivate.TpmInterface,
                         NULL);
    return Status;
}
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
//**********************************************************************
//**********************************************************************
