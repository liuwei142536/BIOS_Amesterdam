/*++

   Copyright (c) 2005 Intel Corporation. All rights reserved
   This software and associated documentation (if any) is furnished
   under a license and may only be used or copied in accordance
   with the terms of the license. Except as permitted by such
   license, no part of this software or documentation may be
   reproduced, stored in a retrieval system, or transmitted in any
   form or by any means without the express written consent of
   Intel Corporation.


   Module Name:

   TpmDxe.h

   Abstract:

   DXE driver for TPM devices

   --*/

//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TCG/TpmDxe.c 13    11/14/11 1:27p Fredericko $
//
// $Revision: 13 $
//
// $Date: 11/14/11 1:27p $
//*************************************************************************
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  TpmDxe.c
//
// Description:
//  Abstracted functions for Tpm protocol are defined here
//
//<AMI_FHDR_END>
//*************************************************************************
#include <Uefi.h>
#include <AmiTcg/TcgTpm12.h>
#include <AmiTcg/TpmLib.h>
#include <Protocol/TpmDevice.h>
#include <Library/DebugLib.h>
#include <Token.h>
#include <Library/IoLib.h>
#include<Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/TcgPlatformSetupPolicy.h>

extern EFI_GUID TcgPlatformSetupPolicyGuid;

#define _CR( Record, TYPE,\
        Field )((TYPE*) ((CHAR8*) (Record) - (CHAR8*) &(((TYPE*) 0)->Field)))

VOID* LocateATcgHob (
    UINTN                   NoTableEntries,
    EFI_CONFIGURATION_TABLE *ConfigTable,
    EFI_GUID                *HOB_guid );

#define TPM_DXE_PRIVATE_DATA_FROM_THIS( this )  \
    _CR( this, TPM_DXE_PRIVATE_DATA, TpmInterface )

typedef struct _TPM_DXE_PRIVATE_DATA
{
    EFI_TPM_DEVICE_PROTOCOL TpmInterface;
    EFI_PHYSICAL_ADDRESS    BaseAddr;
} TPM_DXE_PRIVATE_DATA;


void FixedDelay(UINT32 dCount);

static UINTN                  TpmFuncID;
static TPMTransmitEntryStruct TpmEmptyBuf;
static UINT32                 TpmRet;
static UINTN                  ReadytoBootVar = 0;


EFI_STATUS
EFIAPI TpmDxeInit(
    IN EFI_TPM_DEVICE_PROTOCOL *This )
{
    EFI_STATUS Status;
    volatile TPM_1_2_REGISTERS_PTR TpmReg;
    TpmReg  = (TPM_1_2_REGISTERS_PTR)(( UINTN ) (PORT_TPM_IOMEMBASE));
    Status = TisRequestLocality(TpmReg);
    return Status;
}

EFI_STATUS
EFIAPI TpmDxeClose(
    IN EFI_TPM_DEVICE_PROTOCOL *This )
{
    EFI_STATUS Status;
    volatile TPM_1_2_REGISTERS_PTR TpmReg;
    TpmReg  = (TPM_1_2_REGISTERS_PTR)(( UINTN ) (PORT_TPM_IOMEMBASE));
    Status = TisReleaseLocality(TpmReg);
    return Status;
}

EFI_STATUS
EFIAPI TpmDxeGetStatusInfo(
    IN EFI_TPM_DEVICE_PROTOCOL *This )
{
    return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI TpmDxeTransmit(
    IN EFI_TPM_DEVICE_PROTOCOL *This,
    IN UINTN                   NoInBuffers,
    IN TPM_TRANSMIT_BUFFER     *InBuffers,
    IN UINTN                   NoOutBuffers,
    IN OUT TPM_TRANSMIT_BUFFER *OutBuffers )
{
    volatile TPM_1_2_REGISTERS_PTR TpmReg;
    TpmReg  = (TPM_1_2_REGISTERS_PTR)(( UINTN ) (PORT_TPM_IOMEMBASE));

    return TpmLibPassThrough(
               TpmReg,
               NoInBuffers,
               InBuffers,
               NoOutBuffers,
               OutBuffers
           );
}


static TPM_DXE_PRIVATE_DATA mTpmPrivate =
{
    {
        TpmDxeInit,
        TpmDxeClose,
        TpmDxeGetStatusInfo,
        TpmDxeTransmit
    },
    TPM_BASE_ADDRESS
};




EFI_STATUS
EFIAPI TpmDxeEntry(
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
    EFI_STATUS Status;
#if TCG_LEGACY == 1
    FAR32LOCALS          *TempLoc   = NULL;
#endif
    TCG_PLATFORM_SETUP_PROTOCOL     *ProtocolInstance;

    Status = gBS->LocateProtocol (&gTcgPlatformSetupPolicyGuid,  NULL, (void **)&ProtocolInstance);
    if (EFI_ERROR (Status))
    {
        return Status;
    }

#if TCG_LEGACY == 0
    if(ProtocolInstance->ConfigFlags.DeviceType != 0)
    {
        if(isTpm20CrbPresent())return EFI_NOT_FOUND;
    }

    Status = IsTpmPresent((TPM_1_2_REGISTERS_PTR)( UINTN ) mTpmPrivate.BaseAddr );

    if ( EFI_ERROR( Status ))
    {
        return EFI_REQUEST_UNLOAD_IMAGE;
    }
#else
    TempLoc = (FAR32LOCALS*)LocateATcgHob( pST->NumberOfTableEntries,
                                           pST->ConfigurationTable,
                                           &gEfiTcgMpDriverHobGuid );


    if(TempLoc == NULL || TempLoc->Offset == NULL  ) return EFI_NOT_FOUND;
    Prepare2Thunkproc( TempLoc->Offset, TempLoc->Selector );
    Status = EfiOSReadyToBoot();
#endif

    Status = gBS->InstallMultipleProtocolInterfaces(
                 &ImageHandle,
                 &gEfiTpmDeviceProtocolGuid,
                 &mTpmPrivate.TpmInterface,
                 NULL
             );

    return Status;
}
