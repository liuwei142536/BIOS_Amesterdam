//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <Token.h>
#include <AmiDxeLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesLib.h>
#include <SecureBoot.h>
#include <AmiSecureBootLib.h>
#include <AmiSecureBootLibHooks.h>
#include <AmiDeviceGuardApi.h>
#include <Library/DebugLib.h>

static SDL_X509_SIG_ENTRY gMsUefiCertList[] = {
    {"Microsoft Corporation UEFI CA 2011", { 0x77FA9ABD, 0x0359, 0x4D32, 0xBD, 0x60, 0x28, 0xF4, 0xE7, 0x8F, 0x78, 0x4B} },
    {NULL, {0,0,0,0,0,0,0,0,0,0,0}}
};

//----------------------------------------------------------------------------
// Function forward declaration
//----------------------------------------------------------------------------

// Reset the content of Authorized Secure Boot key database DB to its default value
// This property can be changed only in Setup mode or in custom mode with physical user present
EFI_STATUS EFIAPI ResetDbToDefault (
    IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
);
// Check if the UEFI CA in DB
EFI_STATUS EFIAPI IsUefiCaInDb (
    IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
);
// Delete the UEFI CA from DB
// This property can be changed only in Setup mode or in custom mode with physical user present
EFI_STATUS EFIAPI DeleteUefiCaFromDb (
    IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
);

//----------------------------------------------------------------------------
// Protocol Identifiers
//----------------------------------------------------------------------------

AMI_DEVICEGUARD_SECBOOT_PROTOCOL mDeviceGuardSecbootAPI = {
  0x00000001,
  ResetDbToDefault,
  IsUefiCaInDb,
  DeleteUefiCaFromDb
};
//----------------------------------------------------------------------------
/**
  Reset the content of all UEFI Secure Boot key databases (PK, KEK, DB, DBX) to their default values

  @retval     Status
**/
EFI_STATUS EFIAPI
ResetDbToDefault (
    IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
    )
{
    EFI_STATUS Status;
    VOID *Data = NULL;
    UINTN  DataSize = 0;
//    UINT32    Attributes;

    Status = GetSectionFromAnyFv (&gAmiDbFileGuid, EFI_SECTION_RAW, 0, &Data, (UINTN*)&DataSize);
//    DEBUG((SecureBoot_DEBUG_LEVEL,"Get Raw image %s(%r), sz = %x\n",EFI_IMAGE_SECURITY_DATABASE, Status, DataSize));
    if (!EFI_ERROR(Status)) {
        Status = AmiSetSecureBootVariable((SET_SECURE_VARIABLE_DEL | SET_SECURE_VARIABLE_SET), EFI_IMAGE_SECURITY_DATABASE, NULL, &gEfiImageSecurityDatabaseGuid, (UINT8 *)Data, DataSize );
//        Attributes = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);
//        Status = pRS->SetVariable(EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, Attributes, DataSize, Data);
    }
    if(Data!=NULL)
        pBS->FreePool(Data);
    return Status;
}

/**
  Locate Microsoft UEFI CA Certificate in Authorized Signature Database db.

  @retval     Status
**/
EFI_STATUS EFIAPI IsUefiCaInDb (
    IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
){
    EFI_STATUS Status;
    UINT8    *VarBuf;
    UINTN    VarSize;

    VarSize = 0;
    Status = pRS->GetVariable( EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, NULL, &VarSize, NULL);
//    DEBUG((SecureBoot_DEBUG_LEVEL,"Find Uefi CA in Db\nGet %s(%r), sz = %x\n",EFI_IMAGE_SECURITY_DATABASE, Status, VarSize));
    if(Status != EFI_BUFFER_TOO_SMALL)
        return Status;

    VarBuf = AllocatePool(VarSize);
    if(VarBuf == NULL)
        return EFI_OUT_OF_RESOURCES;

    Status = pRS->GetVariable( EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, NULL, &VarSize, VarBuf);
//    DEBUG((SecureBoot_DEBUG_LEVEL,"Read (%r), sz = %x\n", Status, VarSize));
    if (!EFI_ERROR(Status)) {
        Status = AmiFindCertificateInSignatureList( gMsUefiCertList, VarBuf, VarSize, NULL);
//        DEBUG((SecureBoot_DEBUG_LEVEL, "Find (%r)\n", Status));
    }

    if(VarBuf!=NULL)
        pBS->FreePool(VarBuf);

    return Status;
}

/**
  Delete 'Microsoft Corporation UEFI CA 2011' Certificate from Authorized Signature Database db.

  @retval     Status
**/
EFI_STATUS EFIAPI
DeleteUefiCaFromDb( 
    IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
    )
{
    EFI_STATUS Status;
    UINT8     *Data, *VarBuf;
    UINTN      size, VarSize;
//    UINT32     Attributes;

    Data = NULL;
    VarSize = 0;
    Status = pRS->GetVariable( EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, NULL, &VarSize, NULL);
//    DEBUG((SecureBoot_DEBUG_LEVEL,"Delete Uefi CA rom Db\nGet %s(%r), sz = %x\n",EFI_IMAGE_SECURITY_DATABASE, Status, VarSize));
    if(Status != EFI_BUFFER_TOO_SMALL)
        return Status;

    VarBuf = AllocatePool(VarSize);
    if(VarBuf == NULL)
        return EFI_OUT_OF_RESOURCES;

    Status = pRS->GetVariable( EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, NULL, &VarSize, VarBuf);
//    DEBUG((SecureBoot_DEBUG_LEVEL,"Read (%r), sz = %x\n", Status, VarSize));
    if (!EFI_ERROR(Status)) {
//        Status = AmiRemoveBadCertificates(NULL, VarBuf, &VarSize);
        Status = AmiRemoveBadCertificates(gMsUefiCertList, VarBuf, &VarSize);
//        DEBUG((SecureBoot_DEBUG_LEVEL,"Remove UEFI (%r), new sz = %x\n", Status, VarSize));
        if (!EFI_ERROR(Status)) {
            size = VarSize+sizeof(AMI_EFI_VARIABLE_AUTHENTICATION_2);
            Data = AllocatePool(size);
            Status = EFI_OUT_OF_RESOURCES;
            if(Data == NULL)
                goto exit;
            AmiFillAuthenticatedHdr2(Data, SET_SECURE_VARIABLE_SET);
            MemCpy ((VOID*)((UINTN)Data+sizeof(AMI_EFI_VARIABLE_AUTHENTICATION_2)), VarBuf, VarSize);
            Status = AmiSetSecureBootVariable((SET_SECURE_VARIABLE_DEL | SET_SECURE_VARIABLE_SET), EFI_IMAGE_SECURITY_DATABASE, NULL, &gEfiImageSecurityDatabaseGuid, Data, size );
//            Attributes = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);
//            Status = pRS->SetVariable(EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, Attributes, size, Data);
            DEBUG((SecureBoot_DEBUG_LEVEL,"Update Db (%r)\n", Status));
        }
    }
exit: 
    if(VarBuf!=NULL)
        pBS->FreePool(VarBuf);
    if(Data!=NULL)
        pBS->FreePool(Data);

    return Status;
}
/**
  Entry point of Secure Module DXE driver
  @param[in]  EFI_HANDLE 
  @param[in]  EFI_SYSTEM_TABLE 
  
  @retval     Status
**/
EFI_STATUS EFIAPI 
DeviceGuardApi_Init (
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE   *SystemTable
    )
{
    EFI_HANDLE Handle;

    InitAmiLib(ImageHandle, SystemTable);

    Handle = NULL;
    // 0-15 - Protocol version
    //      0- 7  Min version
    //      8-15  Major
    // 16-31 - reserved
    mDeviceGuardSecbootAPI.Version = 0x00000001;
    return pBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    &gAmiDeviceGuardSecBootApiGuid,
                    &mDeviceGuardSecbootAPI,
                    NULL
                    );
}
