//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
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
  Common file for the DXE driver. Contains the main code for FlashSmiDxe module.
  
  FlashSmi module will hook FLASH_PROTOCOL functions to genrate SMI to use
  functions in SMM.
  
**/

//----------------------------------------------------------------------------
// Includes
// Statements that include other files
#include <PiDxe.h>
#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiCriticalSectionLib.h>

#include <Protocol/SmmCommunication.h>
#include <Protocol/AmiFlash.h>

#include "FlashSmi.h"
#include <Token.h>

EFI_GUID    gFlashSmiGuid = FLASH_SMI_GUID;
EFI_GUID    gBdsConnectDriversProtocolGuid = BDS_CONNECT_DRIVERS_PROTOCOL_GUID;

EFI_SMM_COMMUNICATE_HEADER      *gCommunicateHeader = NULL;
FLASH_SMI_UPDATE                *gFlashSmiBuffer = NULL;
AMI_FLASH_PROTOCOL              *gFlash = NULL;
EFI_SMM_COMMUNICATION_PROTOCOL  *gSmmCommunicate = NULL;
VOID *gFlSmiRtBuff = NULL;

AMI_FLASH_ERASE         gSavedFlashErase;
AMI_FLASH_WRITE         gSavedFlashWrite;
AMI_FLASH_UPDATE        gSavedFlashUpdate;
AMI_FLASH_WRITE_ENABLE  gSavedWriteEnable;
AMI_FLASH_WRITE_DISABLE gSavedWriteDisable;

CRITICAL_SECTION                gFlashSmiCs;

//----------------------------------------------------------------------------
// Function Definitions
/**
    Event to locate EFI_SMM_COMMUNICATION_PROTOCOL.

    @param Event
    @param Context

    @return VOID

**/
VOID
EFIAPI
SmmCommunicationReady(
    IN  EFI_EVENT   Event, 
    IN  VOID        *Context
)
{
    gBS->CloseEvent( Event );

    gBS->LocateProtocol(
            &gEfiSmmCommunicationProtocolGuid,
            NULL,
            &gSmmCommunicate );
}
/**
    Use EFI_SMM_COMMUNICATION_PROTOCOL to generate SMI.
  
    @param VOID

    @return EFI_STATUS
    @retval EFI_SUCCESS Generate SMI successful

**/
EFI_STATUS
GenerateFlashSmi(
    IN  VOID
)
{
    EFI_STATUS  Status;
    UINTN       SmmCommunicateSize;

    if( (gSmmCommunicate == NULL) || (gCommunicateHeader == NULL) )
        return EFI_UNSUPPORTED;

    SmmCommunicateSize = FLASH_SMI_COMMUNICATE_SIZE;

    Status = gSmmCommunicate->Communicate(
                                gSmmCommunicate,
                                (VOID*)gCommunicateHeader,
                                &SmmCommunicateSize );

    return Status;
}
/**
    Update the region of the flash part with the provided data.
  
    @param FlashAddress Pointer to address of a flash to update
    @param Size Size to update
    @param DataBuffer Pointer to the data to write into the flash part
  
    @return EFI_STATUS
    @retval EFI_SUCCESS The data was written successfully
    @retval EFI_DEVICE_ERROR An error was encountered while writing the data

**/
EFI_STATUS
EFIAPI
FlashDriverUpdateSmi(
    IN  VOID    *FlashAddress,
    IN  UINTN   Size,
    IN  VOID    *DataBuffer
)
{
    EFI_STATUS  Status;
	
	if (Size > FLASH_SIZE) 
		return EFI_INVALID_PARAMETER;

    Status = EFI_NO_RESPONSE;
    if( (gFlashSmiBuffer != NULL) && (gFlash != NULL) && (gFlSmiRtBuff != NULL))
    {
        if( EFI_ERROR(BeginCriticalSection( gFlashSmiCs )) ) 
            return EFI_DEVICE_ERROR;

        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );		
		gBS->CopyMem (gFlSmiRtBuff, DataBuffer, Size);		
        gFlashSmiBuffer->Signature = FSMI_SIGNATURE;
        gFlashSmiBuffer->FlashAddress = (UINT32)FlashAddress;
        gFlashSmiBuffer->Size = Size;
        gFlashSmiBuffer->Status = EFI_NO_RESPONSE;
        gFlashSmiBuffer->Subfunction = 'Fu';
        gFlashSmiBuffer->DataBuffer = (UINT32)gFlSmiRtBuff;
        GenerateFlashSmi();
        Status = gFlashSmiBuffer->Status;
        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        EndCriticalSection( gFlashSmiCs );
    }

    if( Status == EFI_NO_RESPONSE )
        Status = gSavedFlashUpdate( FlashAddress, Size, DataBuffer );

    if( EFI_ERROR(Status) )
        Status = EFI_DEVICE_ERROR;

    return Status;
}
/**
    Write the passed data from DataBuffer into the flash part at FlashAddress

    @param FlashAddress The address in the flash part to write the data
    @param Size The size of the data to write
    @param DataBuffer Pointer to the buffer of data to write into the flash part

    @return EFI_STATUS
    @retval EFI_SUCCESS The data was written successfully
    @retval EFI_DEVICE_ERROR An error was encountered while writing the data

**/
EFI_STATUS
EFIAPI
FlashDriverWriteSmi(
    IN  VOID    *FlashAddress,
    IN  UINTN   Size,
    IN  VOID    *DataBuffer
)
{
    EFI_STATUS  Status;

	if (Size > FLASH_SIZE) 
		return EFI_INVALID_PARAMETER;
	
    Status = EFI_NO_RESPONSE;	
	
    if( (gFlashSmiBuffer != NULL) && (gFlash != NULL) && (gFlSmiRtBuff != NULL))
    {
        if( EFI_ERROR(BeginCriticalSection( gFlashSmiCs )) ) 
            return EFI_DEVICE_ERROR;

        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
		gBS->CopyMem (gFlSmiRtBuff, DataBuffer, Size);
        gFlashSmiBuffer->Signature = FSMI_SIGNATURE;
        gFlashSmiBuffer->FlashAddress = (UINT32)FlashAddress;
        gFlashSmiBuffer->Size = Size;
        gFlashSmiBuffer->Status = EFI_NO_RESPONSE;
        gFlashSmiBuffer->Subfunction = 'Fw';
        gFlashSmiBuffer->DataBuffer = (UINT32)gFlSmiRtBuff;
        GenerateFlashSmi();
        Status = gFlashSmiBuffer->Status;
        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        EndCriticalSection( gFlashSmiCs );
    }

    if( Status == EFI_NO_RESPONSE )
        Status = gSavedFlashWrite( FlashAddress, Size, DataBuffer );

    if( EFI_ERROR(Status) )
        Status = EFI_DEVICE_ERROR;

    return Status;
}
/**
    Erase the flash part starting at the passed Address.

    @param FlashAddress Pointer to the address of the flash to erase
    @param Size Size, in bytes, of the flash to erase

    @return EFI_STATUS
    @retval EFI_SUCCESS The desired portion of the flash was erased
    @retval EFI_DEVICE_ERROR An error was encountered while erasing the flash

**/
EFI_STATUS
EFIAPI
FlashDriverEraseSmi(
    IN  VOID    *FlashAddress,
    IN  UINTN   Size
)
{
    EFI_STATUS  Status;

    Status = EFI_NO_RESPONSE;	
    if( (gFlashSmiBuffer != NULL) && (gFlash != NULL) )
    {
        if( EFI_ERROR(BeginCriticalSection( gFlashSmiCs )) ) 
            return EFI_DEVICE_ERROR;

        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        gFlashSmiBuffer->Signature = FSMI_SIGNATURE;
        gFlashSmiBuffer->FlashAddress = (UINT32)FlashAddress;
        gFlashSmiBuffer->Size = Size;
        gFlashSmiBuffer->Status = EFI_NO_RESPONSE;
        gFlashSmiBuffer->Subfunction = 'Fe';
        GenerateFlashSmi();
        Status = gFlashSmiBuffer->Status;
        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        EndCriticalSection( gFlashSmiCs );
    }

    if( Status == EFI_NO_RESPONSE )
        gSavedFlashErase( FlashAddress, Size );

    if( EFI_ERROR(Status) )
        Status = EFI_DEVICE_ERROR;

    return Status;
}
/**
    Enable writing to the flash device.

    @return EFI_STATUS
    @retval EFI_SUCCESS Writing to the flash part has been enabled

**/
EFI_STATUS
EFIAPI
FlashDriverWriteEnableSmi(
    IN  VOID
)
{
    EFI_STATUS          Status;

    Status = EFI_NO_RESPONSE;
    if( (gFlashSmiBuffer != NULL) && (gFlash != NULL) )
    {
        if( EFI_ERROR(BeginCriticalSection( gFlashSmiCs )) ) 
            return EFI_DEVICE_ERROR;

        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        gFlashSmiBuffer->Signature = FSMI_SIGNATURE;
        gFlashSmiBuffer->Status = EFI_NO_RESPONSE;
        gFlashSmiBuffer->Subfunction = 'We';
        GenerateFlashSmi();
        Status = gFlashSmiBuffer->Status;
        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        EndCriticalSection( gFlashSmiCs );
    }

    if( Status == EFI_NO_RESPONSE )
        gSavedWriteEnable ();

    if( EFI_ERROR(Status) )
        Status = EFI_DEVICE_ERROR;

    return Status;
}
/**
    Disable writing to the flash part

    @return EFI_STATUS
    @retval EFI_SUCCESS Writing to the flash part was disabled
    @retval EFI_DEVICE_ERROR There was an error restoring a firmware volume header signature

**/
EFI_STATUS
EFIAPI
FlashDriverWriteDisableSmi(
    IN  VOID
)
{
    EFI_STATUS          Status;

    Status = EFI_NO_RESPONSE;
    if( (gFlashSmiBuffer != NULL) && (gFlash != NULL) )
    {
        if( EFI_ERROR(BeginCriticalSection( gFlashSmiCs )) ) 
            return EFI_DEVICE_ERROR;

        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        gFlashSmiBuffer->Signature = FSMI_SIGNATURE;
        gFlashSmiBuffer->Status = EFI_NO_RESPONSE;
        gFlashSmiBuffer->Subfunction = 'Wd';
        GenerateFlashSmi();
        Status = gFlashSmiBuffer->Status;
        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        EndCriticalSection( gFlashSmiCs );
    }

    if( Status == EFI_NO_RESPONSE )
        gSavedWriteDisable ();

    if( EFI_ERROR(Status) )
        Status = EFI_DEVICE_ERROR;

    return Status;
}
/**
    Unregister the FlashSmi handler

    @return VOID

**/
VOID
EFIAPI
UnregisterFlashSmiHandlerSmi(
    IN  VOID
)
{
    if( gFlashSmiBuffer != NULL )
    {
        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );
        gFlashSmiBuffer->Signature = FSMI_SIGNATURE;
        gFlashSmiBuffer->Status = EFI_NO_RESPONSE;
        gFlashSmiBuffer->Subfunction = 'Un';
        GenerateFlashSmi();
        SetMem( gFlashSmiBuffer, sizeof(FLASH_SMI_UPDATE), 0 );        
    }

    return;
}
/**
    This function will be called when Connect Drivers Protocol is installed
    and will update FlashProtocol function in RunTime.

    @param Event signalled event
    @param Context calling context

    @return VOID

**/
VOID
EFIAPI
FlashSmiConnectDrivers(
    IN  EFI_EVENT   Event, 
    IN  VOID        *Context
)
{
    gBS->CloseEvent( Event );

    if( gFlashSmiBuffer == NULL || gFlSmiRtBuff == NULL || gFlash == NULL)
        return;

    gSavedFlashUpdate = gFlash->Update;
    gSavedFlashErase = gFlash->Erase;
    gSavedFlashWrite = gFlash->Write;
    gSavedWriteEnable = gFlash->DeviceWriteEnable;
    gSavedWriteDisable = gFlash->DeviceWriteDisable;

    gFlash->Update = FlashDriverUpdateSmi;
    gFlash->Erase = FlashDriverEraseSmi;
    gFlash->Write = FlashDriverWriteSmi;
    gFlash->DeviceWriteEnable = FlashDriverWriteEnableSmi;
    gFlash->DeviceWriteDisable = FlashDriverWriteDisableSmi;

}
/**
  This function will be called when Ready To Boot and will restore oringinal
  FlashProtocol function.

  @param Event signalled event
  @param Handle handle

  @return VOID
**/
VOID
EFIAPI
FlashSmiReadyToBoot(
    EFI_EVENT   Event,
    VOID        *Handle
)
{
    gBS->CloseEvent( Event );
    gFlash->Update = gSavedFlashUpdate;
    gFlash->Erase = gSavedFlashErase;
    gFlash->Write = gSavedFlashWrite;
    gFlash->DeviceWriteEnable = gSavedWriteEnable;
    gFlash->DeviceWriteDisable = gSavedWriteDisable;
    UnregisterFlashSmiHandlerSmi();	
}
/**
  This function will convert pointer.

  @param Event signalled event
  @param Context calling context

  @return VOID
**/
VOID
EFIAPI
FlashSmiVirtualFixup(
    IN  EFI_EVENT   Event, 
    IN  VOID        *Context
)
{
    gRT->ConvertPointer( 0, &gCommunicateHeader );
    gRT->ConvertPointer( 0, &gFlashSmiBuffer );
}
/**
  Main function in boot time.

  @param ImageHandle image handle
  @param SystemTable pointer to EFI_SYSTEM_TABLE

  @return VOID
**/
EFI_STATUS
FlashSmiDxeFunction(
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_STATUS  Status;
    UINTN       BufferSize;
    VOID        *Buffer;
    VOID        *RegSmmCommunicationReady;
    EFI_EVENT   EvtVirtualFixup;
    VOID        *RegConnectDrivers;
    EFI_EVENT   EvtReadyToBoot;

    Status = CreateCriticalSection( &gFlashSmiCs );
    if( EFI_ERROR(Status) )
        return Status;

	BufferSize = FLASH_SIZE;
	Status = gBS->AllocatePool(
                    EfiRuntimeServicesData,
                    BufferSize,
                    &gFlSmiRtBuff );
    if( EFI_ERROR(Status) )
        return EFI_SUCCESS;
	
    Status = gBS->LocateProtocol(
                    &gEfiSmmCommunicationProtocolGuid,
                    NULL,
                    &gSmmCommunicate );
    if( EFI_ERROR(Status) )
    {
        RegSmmCommunicationReady = NULL;
        Status = EfiNamedEventListen(
                    &gEfiSmmCommunicationProtocolGuid,
                    TPL_CALLBACK,
                    SmmCommunicationReady,
                    NULL,
                    RegSmmCommunicationReady );
    }

    // Locate FlashProtocol.
    Status = gBS->LocateProtocol( &gAmiFlashProtocolGuid, NULL, &gFlash );
    if( EFI_ERROR(Status) )
        return Status;

    //Allocate buffer for commnicate prorocol
    BufferSize = FLASH_SMI_COMMUNICATE_SIZE;
    Buffer = NULL;
    Status = gBS->AllocatePool(
                    EfiRuntimeServicesData,
                    BufferSize,
                    &Buffer );
    if( EFI_ERROR(Status) )
        return EFI_SUCCESS;

    SetMem( Buffer, BufferSize, 0 );
    gCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER*)Buffer;
    gFlashSmiBuffer = (FLASH_SMI_UPDATE*)gCommunicateHeader->Data;

    //Prepare Smm communicate header
    CopyMem( &gCommunicateHeader->HeaderGuid, &gFlashSmiGuid, sizeof(EFI_GUID) );
    gCommunicateHeader->MessageLength = sizeof(FLASH_SMI_UPDATE);

    Status = gBS->CreateEvent(
                    EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
                    TPL_CALLBACK,
                    FlashSmiVirtualFixup,
                    NULL,
                    &EvtVirtualFixup );
    if( EFI_ERROR(Status) )
        return EFI_SUCCESS;

    RegConnectDrivers = NULL;
    Status = EfiNamedEventListen(
                &gBdsConnectDriversProtocolGuid,
                TPL_CALLBACK,
                FlashSmiConnectDrivers,
                NULL,
                RegConnectDrivers );
    if( EFI_ERROR(Status) )
        return EFI_SUCCESS;

    Status = gBS->CreateEventEx (
                EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK,
                FlashSmiReadyToBoot,
                NULL,
                &gEfiEventReadyToBootGuid,
                &EvtReadyToBoot );

    return EFI_SUCCESS;
}
/**
  Module entry point.

  @param ImageHandle image handle
  @param SystemTable pointer to EFI_SYSTEM_TABLE

  @return VOID
**/
EFI_STATUS
EFIAPI
FlashSmiDxeEntry(
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_BOOT_MODE   Bootmode;

    Bootmode = GetBootModeHob();
    if( (Bootmode == BOOT_ON_FLASH_UPDATE) ||
        (Bootmode == BOOT_IN_RECOVERY_MODE) )
    {
        return FlashSmiDxeFunction( ImageHandle, SystemTable );
    }

    return EFI_UNSUPPORTED;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************