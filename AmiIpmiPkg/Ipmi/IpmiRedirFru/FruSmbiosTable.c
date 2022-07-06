//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file FruSmbiosTable.c
    Update SMBIOS type 1,2,3 structure

**/

//----------------------------------------------------------------------

#include "IpmiRedirFru.h"
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

//----------------------------------------------------------------------

//
// Macro definitions
//
#define MAX_SMBIOS_STRING_SIZE  0x40

//
// Macro represent size of SmBios structure end.
// Every SmBios type ends with 0x0000.
//
#define SIZE_OF_SMBIOS_STRUCTURE_END_MACRO sizeof(UINT16)

/**
    Convert the Unicode string into Ascii string.

    @param UnicodeStr Unicode string pointer.
    @param AsciiStr Ascii string pointer

    @retval CHAR8* Ascii string pointer.

**/

UINT8
Unicode2AsciiConversion (
  OUT CHAR8    *AsciiStr,
  IN  CHAR16   *UnicodeStr )
{
    UINT8   Length = 0;

    while (TRUE) {
        *AsciiStr = (CHAR8) *(UnicodeStr++);
        Length++;
        if (*(AsciiStr++) == '\0') {
            return Length;
        }
    }
}

/**
    Convert the Valid  Unicode string into Ascii string. If strinig is invalid
    truncate to maximum size and converted.

    @param AsciiStr Ascii string pointer
    @param UnicodeStr Unicode string pointer.

    @retval CHAR8* AsciiStr - Ascii string pointer.

**/

VOID
GetValidSMBIOSString(
  OUT  CHAR8     *AsciiStr,
  IN   CHAR16    *UnicodeStr)
{
    CHAR16  TempUnicodeString[MAX_SMBIOS_STRING_SIZE];
    UINTN   Length;

    Length = StrSize (UnicodeStr);

    ZeroMem (&TempUnicodeString[0], Length);

    //
    // Validating string size
    //
    if ( Length >= MAX_SMBIOS_STRING_SIZE ) {

        //
        // Truncate size to maximum allowable
        //
        Length = MAX_SMBIOS_STRING_SIZE - 1;
    }
    StrnCpy (TempUnicodeString, UnicodeStr, Length);
    Unicode2AsciiConversion (AsciiStr, TempUnicodeString);
}

/**
    Notification function for SMBIOS protocol.
    Update SMBIOS type 1,2,3 structure.

    @param Event Event which caused this handler
    @param Context Context passed during Event Handler registration

    @retval VOID

**/

VOID
EFIAPI
FruSmbiosTableUpdate (
  IN  EFI_EVENT                     Event,
  IN  VOID                          *Context )
{
    EFI_STATUS                      Status;
    EFI_SMBIOS_HANDLE               SmbiosHandle;
    EFI_SMBIOS_PROTOCOL             *SmbiosProtocol;
    EFI_SMBIOS_TYPE                 Type;
    EFI_SMBIOS_TABLE_HEADER         *Record;
    SMBIOS_TABLE_TYPE1              *Type1Structure;
    SMBIOS_TABLE_TYPE2              *Type2Structure;
    SMBIOS_TABLE_TYPE3              *Type3Structure;
    UINTN                           StringNumber;
    CHAR16                          *UnicodeString;
    CHAR16                          *DefaultString = L"To be filled by O.E.M.                                            ";
    CHAR8                           AsciiString[MAX_SMBIOS_STRING_SIZE];
    EFI_GUID                        *Uuid = NULL;
    EFI_GUID                        DefaultUuid = {0};

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));
    Status = gBS->LocateProtocol (
                &gEfiSmbiosProtocolGuid,
                NULL,
                (VOID **) &SmbiosProtocol);
    DEBUG ((DEBUG_INFO, "Locate SmbiosProtocol Status:%r \n", Status));
    if (EFI_ERROR (Status)) {
        return;
    }
    Status = gBS->CloseEvent (Event);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "CloseEvent Status:%r \n", Status));
  
    //
    // Smbios Type 1 structure process start
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Type = EFI_SMBIOS_TYPE_SYSTEM_INFORMATION;
    Status = SmbiosProtocol->GetNext (SmbiosProtocol,
                                      &SmbiosHandle,
                                      &Type,
                                      &Record,
                                      NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext Status:%r \n", Status));
    if (EFI_ERROR(Status)) {
        //
        // Type 1 structure is not available, needs to be created.
        //
        Type1Structure = AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE1) + SIZE_OF_SMBIOS_STRUCTURE_END_MACRO);
        if (Type1Structure == NULL) {
            return;
        }
        Type1Structure->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_INFORMATION;
        Type1Structure->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE1);
        Type1Structure->Hdr.Handle = 0xFFFF;//To be updated by SMBIOS driver.
        //
        // Creating the Smbios Type 1 Structure.
        //
        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        Status = SmbiosProtocol->Add (SmbiosProtocol,
                                      NULL,
                                      &SmbiosHandle,
                                      (EFI_SMBIOS_TABLE_HEADER*) Type1Structure);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->Add Status:%r \n", Status));
        if (EFI_ERROR (Status)) {
            return;
        }
        
        //Free the memory allocated for Type 1 structure
        FreePool(Type1Structure);
        Status = SmbiosProtocol->GetNext (SmbiosProtocol,
                                          &SmbiosHandle,
                                          &Type,
                                          &Record,
                                          NULL);
        if (EFI_ERROR (Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext EFI_SMBIOS_TYPE_SYSTEM_INFORMATION Status:%r \n", Status));
            return;
        }
    }
    Type1Structure = (SMBIOS_TABLE_TYPE1 *)Record;
    //
    //Initialize with null string index
    //
    Type1Structure->Manufacturer = 0x00;
    Type1Structure->ProductName = 0x00;
    Type1Structure->Version = 0x00;
    Type1Structure->SerialNumber = 0x00;

    //
    // Copy the Uuid
    //
    Uuid = (EFI_GUID *)PcdGetPtr (PcdSystemUuid);
    if(CompareGuid ((CONST EFI_GUID*)Uuid, (CONST EFI_GUID*)&DefaultUuid)== FALSE) {
        CopyMem (&(Type1Structure->Uuid), Uuid, sizeof(EFI_GUID));
    }
    //
    // Type 1 Manufacturer String
    //
    SmbiosHandle = Type1Structure->Hdr.Handle;
    StringNumber = 0x01;
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdSystemManufacturer);
    if ( StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00){
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
        Type1Structure->Manufacturer = (SMBIOS_TABLE_STRING)StringNumber;
        StringNumber ++;
        }
    }
    //
    // Type 1 Product Name String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdSystemProductName);
    if ( StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00){
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
        Type1Structure->ProductName = (SMBIOS_TABLE_STRING)StringNumber;
        StringNumber ++;
        }
    }
    //
    // Type 1 Version String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdSystemVersion);
    if ( StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    } 
    if (StrCmp (UnicodeString, L"") != 0x00){
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type1Structure->Version = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
        }
    }
    //
    // Type 1 Serial Number String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdSystemSerialNumber);
    if ( StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }  
    if (StrCmp (UnicodeString, L"") != 0x00){
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type1Structure->SerialNumber = (SMBIOS_TABLE_STRING)StringNumber;
        }
    }
    //
    // Smbios Type 2 structure process start
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Type = EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION;
    Status = SmbiosProtocol->GetNext (SmbiosProtocol,
                                      &SmbiosHandle,
                                      &Type,
                                      &Record,
                                      NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext Status:%r \n", Status));
    if (EFI_ERROR (Status)) {
        //
        // Type 2 structure is not available, needs to be created.
        //
        Type2Structure = AllocateZeroPool (sizeof(SMBIOS_TABLE_TYPE2) + SIZE_OF_SMBIOS_STRUCTURE_END_MACRO);
        if (Type2Structure == NULL) {
            return;
        }
        Type2Structure->Hdr.Type = EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION;
        Type2Structure->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE2);
        Type2Structure->Hdr.Handle = 0xFFFF;//To be updated by SMBIOS driver.
        //
        // Creating the Smbios Type 2 Structure.
        //
        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        Status = SmbiosProtocol->Add (SmbiosProtocol,
                                      NULL,
                                      &SmbiosHandle,
                                      (EFI_SMBIOS_TABLE_HEADER*) Type2Structure);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->Add Status:%r \n", Status));
        if (EFI_ERROR (Status)) {
                    return;
        }
        //Free the memory allocated for Type 2 structure
        FreePool(Type2Structure);
        Status = SmbiosProtocol->GetNext (SmbiosProtocol,
                                          &SmbiosHandle,
                                          &Type,
                                          &Record,
                                          NULL);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION Status:%r \n", Status));
        if (EFI_ERROR (Status)) {
            return;
        }
    } 
    Type2Structure = (SMBIOS_TABLE_TYPE2 *)Record;
    //
    //Initialize with null string index
    //
    Type2Structure->Manufacturer = 0x00;
    Type2Structure->ProductName = 0x00;
    Type2Structure->Version = 0x00;
    Type2Structure->SerialNumber = 0x00;
    Type2Structure->AssetTag = 0x00;
    //
    // Type 2 Manufacturer String
    //
    SmbiosHandle = Type2Structure->Hdr.Handle;
    StringNumber = 0x01;
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdBaseBoardManufacturer);
    if ( StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type2Structure->Manufacturer = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }
    //
    // Type 2 Product Name String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdBaseBoardProductName);
    if (StrCmp (UnicodeString, DefaultString) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type2Structure->ProductName = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }
    //
    // Type 2 Version String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdBaseBoardVersion);
    if (StrCmp (UnicodeString, DefaultString) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    } 
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type2Structure->Version = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }
    //
    // Type 2 Serial Number String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdBaseBoardSerialNumber);
    if (StrCmp (UnicodeString, DefaultString) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type2Structure->SerialNumber = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }
    //
    // Type 2 AssetTag String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdBaseBoardAssetTag);
    if ( StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type2Structure->AssetTag = (SMBIOS_TABLE_STRING)StringNumber;
         }
    }
  
    //
    // Smbios Type 3 structure process start
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Type = EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE;
    Status = SmbiosProtocol->GetNext (SmbiosProtocol,
                                      &SmbiosHandle,
                                      &Type,
                                      &Record,
                                      NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext Status:%r \n", Status));
    if (EFI_ERROR (Status)) {
        //
        // Type 3 structure is not available, needs to be created.
        Type3Structure = AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE3) + SIZE_OF_SMBIOS_STRUCTURE_END_MACRO);
        if (Type3Structure == NULL) {
            return;
        }
        Type3Structure->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE;
        Type3Structure->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE3);
        Type3Structure->Hdr.Handle = 0xFFFF;//To be updated by SMBIOS driver.
        //
        // Creating the Smbios Type 3 Structure.
        //
            SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
            Status = SmbiosProtocol->Add (
                        SmbiosProtocol,
                        NULL,
                        &SmbiosHandle,
                        (EFI_SMBIOS_TABLE_HEADER*) Type3Structure);
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->Add Status:%r \n", Status));
            if (EFI_ERROR (Status)) {
                return;
            }
            //Free the memory allocated for Type 3 structure
            FreePool(Type3Structure);
            Status = SmbiosProtocol->GetNext (SmbiosProtocol,
                                              &SmbiosHandle,
                                              &Type,
                                              &Record,
                                              NULL);
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE Status:%r \n", Status));
            if (EFI_ERROR (Status)) {
                return;
            }
        } 
    Type3Structure = (SMBIOS_TABLE_TYPE3 *)Record;
    //
    //Initialize with null string index
    //
    Type3Structure->Manufacturer = 0x00;
    Type3Structure->Version = 0x00;
    Type3Structure->SerialNumber = 0x00;
    Type3Structure->AssetTag = 0x00;
    Type3Structure->Type = PcdGet8 (PcdChassisType);
    //
    // Type 3 Manufacturer String
    //
    SmbiosHandle = Type3Structure->Hdr.Handle;
    StringNumber = 0x01;
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdChassisManufacturer);
    if (StrCmp (UnicodeString, DefaultString) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type3Structure->Manufacturer = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }
    //
    // Type 3 Version String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdChassisVersion);
    if (StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type3Structure->Version = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }
    //
    // Type 3 SerialNumber String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdChassisSerialNumber);
    if (StrCmp (UnicodeString, DefaultString) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type3Structure->SerialNumber = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }
    //
    // Type 3 AssetTag String
    //
    UnicodeString = (CHAR16 *) PcdGetPtr (PcdChassisAssetTag);
    if ( StrCmp (UnicodeString, DefaultString) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (PcdStrDefault);
    }
    if (StrCmp (UnicodeString, L"") != 0x00) {
        GetValidSMBIOSString (AsciiString, UnicodeString);
        Status = SmbiosProtocol->UpdateString (SmbiosProtocol,
                                               &SmbiosHandle,
                                               &StringNumber,
                                               AsciiString);
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Status:%r \n", Status));
            Type3Structure->AssetTag = (SMBIOS_TABLE_STRING)StringNumber;
         }
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Exiting from %a\n", __FUNCTION__ ));
    return;
}

/**
    Register notification callback on SMBIOS protocol to update FRU Smbios
    structure.

    @param VOID

    @retval VOID

**/

EFI_STATUS
UpdateFruSmbiosTables (
  VOID )
{
    VOID    *SmbiosProtocolRegistration;

    //
    // Create Notification event for SmbiosProtocol GUID
    //
    EfiCreateProtocolNotifyEvent (
        &gEfiSmbiosProtocolGuid,
        TPL_CALLBACK,
        FruSmbiosTableUpdate,
        NULL,
        &SmbiosProtocolRegistration );

    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
