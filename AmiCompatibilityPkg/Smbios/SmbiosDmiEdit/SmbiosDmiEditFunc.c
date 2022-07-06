//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file SmbiosDmiEditFunc.c
    This file provides worker functions to support DmiEdit feature

**/

#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Protocol/AmiSmbios.h>
#include <Library/AmiBufferValidationLib.h>
#include "SmbiosDmiEdit.h"

#define WRITE_ONCE_ENTRIES  0x10                // Maximum number of WRITE_ONCE_STATUS entries
#define DMI_ARRAY_SIZE (DMI_ARRAY_COUNT * sizeof(DMI_VAR))

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
extern UINT16 UpdateSmbiosTable (
                            IN TABLE_INFO  *TableInfo,
                            IN UINT8       *Data
                            );

VOID                        *gFlashData;
UINT32                      gFlashDataSize;
#else
CHAR16                      *DmiArrayVar = L"DmiArray";
DMI_VAR                     DmiArray[DMI_ARRAY_COUNT] = {0};
UINTN                       DmiArraySize = DMI_ARRAY_SIZE;
UINT8                       *DmiData;
UINTN                       DmiDataSize;
CHAR16                      VarName[17];
CHAR16                      *Var = VarName;
UINT8                       Index;
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

UINT8                       *SmbiosTable;                   // DO NOT change after initialized
UINT8                       *SmbiosTableEnd;                // DO NOT change after initialized
UINTN                       SmbiosTableSize = 0;                // DO NOT change after initialized
UINT8                       *StructurePtr;

#if (SMBIOS_2X_SUPPORT == 1)
SMBIOS_TABLE_ENTRY_POINT    *SmbiosTableEntryPoint = NULL;
SMBIOS_TABLE_ENTRY_POINT    *SmbiosTableEntryPointF000 = NULL;

SMBIOS_TABLE_ENTRY_POINT    AfriEntryPointTable = {
            '_', 'S', 'M', '_',
            0,                                              // EPS Checksum
            0x1f,                                           // Entry Point Length
#if SMBIOS_3X_SUPPORT
            SMBIOS_3X_MAJOR_VERSION,                        // SMBIOS Major Version
            SMBIOS_3X_MINOR_VERSION,                        // SMBIOS Minor Version
#else
            SMBIOS_2X_MAJOR_VERSION,                        // SMBIOS Major Version
            SMBIOS_2X_MINOR_VERSION,                        // SMBIOS Minor Version
#endif
            0x100,                                          // Maximum Structure Size
            0,                                              // Entry Point Revision
            0, 0, 0, 0, 0,                                  // Formatted Area
            '_', 'D', 'M', 'I', '_',                        // Intermediate Anchor String
            0,                                              // Intermediate Checksum
            0,                                              // Structure Table Length
            0,                                              // Structure Table Address
            0x10,                                           // Number of SMBIOS Stuctures
#if SMBIOS_3X_SUPPORT
            (SMBIOS_3X_MAJOR_VERSION << 4)|SMBIOS_3X_MINOR_VERSION // SMBIOS BCD Revision};
#else
            (SMBIOS_2X_MAJOR_VERSION << 4)|SMBIOS_2X_MINOR_VERSION // SMBIOS BCD Revision};
#endif
            };

SMBIOS_TABLE_ENTRY_POINT        *AfriTableEntryPoint = &AfriEntryPointTable;

UINTN                       Smb2xTablePhysAddress  = 0;
#endif                                          // SMBIOS_2X_SUPPORT
#if (SMBIOS_3X_SUPPORT == 1)
SMBIOS_3X_TABLE_ENTRY_POINT *SmbiosV3TableEntryPoint = NULL;

SMBIOS_3X_TABLE_ENTRY_POINT	AfriV3EntryPointTable = {
			'_', 'S', 'M', '3', '_',
			0,												// EPS Checksum
			sizeof(SMBIOS_3X_TABLE_ENTRY_POINT),            // Entry Point Length
			SMBIOS_3X_MAJOR_VERSION,						// SMBIOS Major Version
			SMBIOS_3X_MINOR_VERSION,						// SMBIOS Minor Version
			0x00,										    // SMBIOS Docrev
			0x01,											// Entry Point Revision 3.0
			0,                                              // Reserved
			0x00000000,                                     // Structure table maximum size
			0x0000000000000000,                             // Structure table address
            };

SMBIOS_3X_TABLE_ENTRY_POINT *AfriV3TableEntryPoint = &AfriV3EntryPointTable;

UINTN                       Smb3xTablePhysAddress    = 0;
#endif                                          // SMBIOS_3X_SUPPORT
UINT8                       *ScratchBufferPtr = NULL;
UINT16						MaximumBufferSize;

//
// String type tables
//
STRING_TABLE    StringType_0[] =   {{0x04, 1, 1},
                                    {0x05, 2, 2},
                                    {0x08, 3, 3},
                                    {0xff, 0, 0},
                                   };

STRING_TABLE    StringType_1[] =   {{0x04, 1, 1},
                                    {0x05, 2, 2},
                                    {0x06, 3, 3},
                                    {0x07, 4, 4},
                                    {0x19, 5, 5},
                                    {0x1a, 6, 6},
                                    {0xff, 0, 0},
                                   };

STRING_TABLE    StringType_2[] =   {{0x04, 1, 1},
                                    {0x05, 2, 2},
                                    {0x06, 3, 3},
                                    {0x07, 4, 4},
                                    {0x08, 5, 5},
                                    {0x0a, 6, 6},
                                    {0xff, 0, 0},
                                   };

#if (TYPE3_STRUCTURE == 1)
STRING_TABLE    StringType_3[NUMBER_OF_SYSTEM_CHASSIS][6] =
                                  {{{0x04, 1, 1},
                                    {0x06, 2, 2},
                                    {0x07, 3, 3},
                                    {0x08, 4, 4},
                                    {0x15 + (ELEMENT_COUNT_1 * ELEMENT_LEN_1), 5, 5},
                                    {0xff, 0, 0},
                                   },
#if NUMBER_OF_SYSTEM_CHASSIS > 1
                                   {
                                    {0x04, 1, 1},
                                    {0x06, 2, 2},
                                    {0x07, 3, 3},
                                    {0x08, 4, 4},
                                    {0x15 + (ELEMENT_COUNT_2 * ELEMENT_LEN_2), 5, 5},
                                    {0xff, 0, 0},
                                   },
#endif
#if NUMBER_OF_SYSTEM_CHASSIS > 2
                                   {
                                    {0x04, 1, 1},
                                    {0x06, 2, 2},
                                    {0x07, 3, 3},
                                    {0x08, 4, 4},
                                    {0x15 + (ELEMENT_COUNT_3 * ELEMENT_LEN_3), 5, 5},
                                    {0xff, 0, 0},
                                   },
#endif
#if NUMBER_OF_SYSTEM_CHASSIS > 3
                                   {
                                    {0x04, 1, 1},
                                    {0x06, 2, 2},
                                    {0x07, 3, 3},
                                    {0x08, 4, 4},
                                    {0x15 + (ELEMENT_COUNT_4 * ELEMENT_LEN_4), 5, 5},
                                    {0xff, 0, 0},
                                   },
#endif
#if NUMBER_OF_SYSTEM_CHASSIS > 4
                                   {
                                    {0x04, 1, 1},
                                    {0x06, 2, 2},
                                    {0x07, 3, 3},
                                    {0x08, 4, 4},
                                    {0x15 + (ELEMENT_COUNT_5 * ELEMENT_LEN_5), 5, 5},
                                    {0xff, 0, 0},
                                   },
#endif
                                  };            // StringType_3
#endif                                          // TYPE3_STRUCTURE

STRING_TABLE    StringType_4[] =   {{0x04, 1, 1},
                                    {0x07, 2, 2},
                                    {0x10, 3, 3},
                                    {0x20, 4, 4},
                                    {0x21, 5, 5},
                                    {0x22, 6, 6},
                                    {0xff, 0, 0},
                                   };

STRING_TABLE    StringType_22[] =  {{0x04, 1, 1},
                                    {0x05, 2, 2},
                                    {0x06, 3, 3},
                                    {0x07, 4, 4},
                                    {0x08, 5, 5},
                                    {0x0e, 6, 6},
                                    {0x14, 7, 7},
                                    {0xff, 0, 0},
                                   };

STRING_TABLE    StringType_39[] =  {{0x05, 1, 1},
                                    {0x06, 2, 2},
                                    {0x07, 3, 3},
                                    {0x08, 4, 4},
                                    {0x09, 5, 5},
                                    {0x0a, 6, 6},
                                    {0x0b, 7, 7},
                                    {0xff, 0, 0},
                                   };

//
// String table
//
VOID*    StringTable[] = {&StringType_0,        // 0
                          &StringType_1,        // 1
                          &StringType_2,        // 2
#if (TYPE3_STRUCTURE == 1)
                          &StringType_3,        // 3
#endif
                          &StringType_4,        // 4
                          &StringType_22,       // 5
                          &StringType_39,       // 6
                         };

UINTN   StringTableSize = sizeof(StringTable);

/**
    Table indicating which structure and offset can be written
    only once or multiple times
**/
WRITE_ONCE_TABLE WriteOnceTable[] = {
                                      {1, 4, TRUE},
                                      {2, 4, TRUE},
                                    };

/**
    Initialize NVRAM variable holding WriteOnce statuses

    @param None

    @retval None

**/
VOID
WriteOnceStatusInit(VOID)
{
    EFI_STATUS          Status;
    WRITE_ONCE_STATUS   *Buffer;
    UINTN               BufferSize;

    BufferSize = WRITE_ONCE_ENTRIES * sizeof(WRITE_ONCE_STATUS);
    Status = pBS->AllocatePool(EfiBootServicesData, BufferSize, (void**)&Buffer);
    ASSERT_EFI_ERROR(Status);
    
    // Create "WriteOnceStatus" variable if it does not exist
    Status = pRS->GetVariable(L"WriteOnceStatus",
                                &gAmiSmbiosNvramGuid,
                                NULL,
                                &BufferSize,
                                Buffer);

    if (Status == EFI_NOT_FOUND) {
        // WriteOnceStatus variable does not exist
        // Create one with default value of Type 127
        MemSet(Buffer, BufferSize, 127);

    	pRS->SetVariable(L"WriteOnceStatus",
                        &gAmiSmbiosNvramGuid,
                        EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        BufferSize,
                        Buffer);
    }

    pBS->FreePool(Buffer);
}

/**
    Determines if a given structure type and offset can only
    be written once or multiple times.

    @param Type
    @param Offset

    @return BOOLEAN TRUE/FALSE for Write Once/Multiple

**/
BOOLEAN
isWriteOnce(
    IN UINT8    Type,
    IN UINT8    Offset,
    IN UINT16   Handle
)
{
    EFI_STATUS          Status;
    BOOLEAN             WriteOnce = FALSE;
    UINT8               TableEntries = sizeof(WriteOnceTable)/sizeof(WRITE_ONCE_TABLE);
    WRITE_ONCE_STATUS   Buffer[WRITE_ONCE_ENTRIES];
    UINTN               BufferSize;
    UINT8               i;
    UINT8               j = 0;

    BufferSize = WRITE_ONCE_ENTRIES * sizeof(WRITE_ONCE_STATUS);
    Status = pRS->GetVariable(L"WriteOnceStatus",
                                &gAmiSmbiosNvramGuid,
                                NULL,
                                &BufferSize,
                                Buffer);

    for (i = 0; i < TableEntries; ++i) {
        // Check for WriteOnce condition in WriteOnce table
        if (WriteOnceTable[i].Type == Type \
            && WriteOnceTable[i].Offset == Offset \
            && WriteOnceTable[i].WriteOnce) {
            // WriteOnce is set for input Type and Offset,
            // Check if WriteOnce was set already in WriteOnceStatus table
            // If "WriteOnceStatus" variable was not found then assume
            // WriteOnce was not set for this data field
            if (Status == EFI_SUCCESS) {
                for (j = 0; j < WRITE_ONCE_ENTRIES; ++j) {
                    if (Buffer[j].Type == 127) {
                        break;
                    }
                    if (Buffer[j].Type == Type && Buffer[j].Offset == Offset && Buffer[j].Handle == Handle) {
                        // WriteOnce was already set for input Type and Offset
                        WriteOnce = TRUE;
                        break;
                    }
                }
            }

            if (j < WRITE_ONCE_ENTRIES) {           // Make sure we are still within the WRITE_ONCE_ENTRIES
                // Create new WriteOnce entry if it did not exist for input Type and Offset
                if (WriteOnce == FALSE) {
                    Buffer[j].Type = Type;
                    Buffer[j].Offset = Offset;
                    Buffer[j].Handle = Handle;

                	pRS->SetVariable(L"WriteOnceStatus",
                                    &gAmiSmbiosNvramGuid,
                                    EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                    BufferSize,
                                    Buffer);
                }
            }
        }
    }

    return WriteOnce;
}

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
/**
    Searches the Flash Data Table for a record of Type and
    Offset. If found, returns the location found, the data size,
    and the end of data.

    @param RecordInfo

    @return FLASH_DATA_INFO structure contains location of flash data
            record, its data size, and end of data

**/
FLASH_DATA_INFO
GetFlashDataInfo(
    IN TABLE_INFO   *RecordInfo
)
{
    TABLE_INFO        *FlashDataPtr = gFlashData;
    FLASH_DATA_INFO   FlashDataInfo = {0, 0, 0};

    while (FlashDataPtr->Handle != 0xffff) {
        if (FlashDataPtr->Type == RecordInfo->Type &&
		    FlashDataPtr->Handle == RecordInfo->Handle &&
		    FlashDataPtr->Offset == RecordInfo->Offset &&
			FlashDataPtr->Flags == RecordInfo->Flags) {
			FlashDataInfo.Location = (UINT8*)FlashDataPtr;
			FlashDataInfo.Size = FlashDataPtr->Size;
		}

        FlashDataPtr = (TABLE_INFO*)((UINT8*)(FlashDataPtr + 1) + FlashDataPtr->Size);
	}
	FlashDataInfo.EndOfData = (UINT8*)FlashDataPtr;
	return FlashDataInfo;
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

#if (SMBIOS_2X_SUPPORT == 1)
/**
    Returns the SMBIOS 2.x information

    @param Pointer to SMI function parameters

    @return UINT16 Status (0 for success)

**/
UINT16
GetSmbiosV2Info(
    IN OUT  GET_SMBIOS_INFO   *p
)
{
    if (SmbiosTable == NULL) return DMI_FUNCTION_NOT_SUPPORTED;

#if (((!defined(AFRI_PASS_TO_AFRI_PROTOCOL) && SMM_SUPPORT) || (AFRI_PASS_TO_SMM_COMMUNICATION && SMM_SUPPORT)) && !defined(_DMIEDIT_PROTOCOL_))
    if (AmiValidateMemoryBuffer((VOID*)p, sizeof(GET_SMBIOS_INFO)) == EFI_SUCCESS) {
        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->DmiBiosRevision32BitAddr, 1) == EFI_SUCCESS) {
            *(UINT8*)(UINTN)p->DmiBiosRevision32BitAddr = (SMBIOS_2X_MAJOR_VERSION << 4) | SMBIOS_2X_MINOR_VERSION;
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->NumStructures32BitAddr, 2) == EFI_SUCCESS) {
            *(UINT16*)(UINTN)p->NumStructures32BitAddr = GetNumberOfStructures(SmbiosTable);
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->StructureSize32BitAddr, 2) == EFI_SUCCESS) {
            *(UINT16*)(UINTN)p->StructureSize32BitAddr = GetLargestStructureSize(SmbiosTable);
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->DmiStorageBase32BitAddr, 4) == EFI_SUCCESS) {
            if (Smb2xTablePhysAddress != 0) {
                *(UINT32*)(UINTN)p->DmiStorageBase32BitAddr = (UINT32)Smb2xTablePhysAddress;
            }
            else {
                *(UINT32*)(UINTN)p->DmiStorageBase32BitAddr = (UINT32)SmbiosTable;
            }
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->DmiStorageSize32BitAddr, 2) == EFI_SUCCESS) {
            *(UINT16*)(UINTN)p->DmiStorageSize32BitAddr = MaximumBufferSize;
        }
        else {
            return DMI_BAD_PARAMETER;
        }
    }
    else {
        return DMI_BAD_PARAMETER;
    }
#else
    *(UINT8*)(UINTN)p->DmiBiosRevision32BitAddr = (SMBIOS_2X_MAJOR_VERSION << 4) | SMBIOS_2X_MINOR_VERSION;
    *(UINT16*)(UINTN)p->NumStructures32BitAddr = GetNumberOfStructures(SmbiosTable);
    *(UINT16*)(UINTN)p->StructureSize32BitAddr = GetLargestStructureSize(SmbiosTable);
    if (Smb2xTablePhysAddress != 0) {
        *(UINT32*)(UINTN)p->DmiStorageBase32BitAddr = (UINT32)Smb2xTablePhysAddress;
    }
    else {
        *(UINT32*)(UINTN)p->DmiStorageBase32BitAddr = (UINT32)SmbiosTable;
    }
    *(UINT16*)(UINTN)p->DmiStorageSize32BitAddr = MaximumBufferSize;
#endif

    return 0;
}
#endif                                          // SMBIOS_2X_SUPPORT

#if (SMBIOS_3X_SUPPORT == 1)
/**
    Returns the SMBIOS 3.x information

    @param Pointer to SMI function parameters

    @return UINT16 Status (0 for success)

**/
UINT16
GetSmbiosV3Info(
    IN OUT  GET_SMBIOS_V3_INFO   *p
)
{
    if (SmbiosTable == NULL) return DMI_FUNCTION_NOT_SUPPORTED;

#if (((!defined(AFRI_PASS_TO_AFRI_PROTOCOL) && SMM_SUPPORT) || (AFRI_PASS_TO_SMM_COMMUNICATION && SMM_SUPPORT)) && !defined(_DMIEDIT_PROTOCOL_))
    if (AmiValidateMemoryBuffer((VOID*)p, sizeof(GET_SMBIOS_V3_INFO)) == EFI_SUCCESS) {
        if (AmiValidateMemoryBuffer((VOID*)p->DmiBiosRevision64BitAddr, 1) == EFI_SUCCESS) {
            *(UINT8*)p->DmiBiosRevision64BitAddr = (SMBIOS_3X_MAJOR_VERSION << 4) | SMBIOS_3X_MINOR_VERSION;
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)p->NumStructures64BitAddr, 2) == EFI_SUCCESS) {
            *(UINT16*)p->NumStructures64BitAddr = GetNumberOfStructures(SmbiosTable);
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)p->StructureSize64BitAddr, 2) == EFI_SUCCESS) {
            *(UINT16*)p->StructureSize64BitAddr = GetLargestStructureSize(SmbiosTable);
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)p->DmiStorageBase64BitAddr, 8) == EFI_SUCCESS) {
            if (Smb3xTablePhysAddress != 0) {
                *(UINT64*)p->DmiStorageBase64BitAddr = Smb3xTablePhysAddress;
            }
            else {
                *(UINT64*)p->DmiStorageBase64BitAddr = (UINT64)SmbiosTable;
            }
        }
        else {
            return DMI_BAD_PARAMETER;
        }
        if (AmiValidateMemoryBuffer((VOID*)p->DmiStorageSize64BitAddr, 2) == EFI_SUCCESS) {
            *(UINT16*)p->DmiStorageSize64BitAddr = MaximumBufferSize;
        }
        else {
            return DMI_BAD_PARAMETER;
        }
    }
    else {
        return DMI_BAD_PARAMETER;
    }
#else
    *(UINT8*)p->DmiBiosRevision64BitAddr = (SMBIOS_3X_MAJOR_VERSION << 4) | SMBIOS_3X_MINOR_VERSION;
    *(UINT16*)p->NumStructures64BitAddr = GetNumberOfStructures(SmbiosTable);
    *(UINT16*)p->StructureSize64BitAddr = GetLargestStructureSize(SmbiosTable);
    if (Smb3xTablePhysAddress != 0) {
        *(UINT64*)p->DmiStorageBase64BitAddr = Smb3xTablePhysAddress;
    }
    else {
        *(UINT64*)p->DmiStorageBase64BitAddr = (UINT64)SmbiosTable;
    }
    *(UINT16*)p->DmiStorageSize64BitAddr = MaximumBufferSize;
#endif

    return 0;
}
#endif                                          // SMBIOS_3X_SUPPORT

/**
    Searches the structure table for a record with its handle
    equal to the input Handle.
    Returns the pointer to the structure if found.
    Returns NULL if not found

    @param Handle

    @return UINT8 Pointer to structure found

**/
UINT8*
GetStructureByHandle(
    IN UINT16    *Handle
)
{
    UINT8   *SmbiosTableNext;
    UINT16  Size;

    if (SmbiosTable == NULL) {
        return NULL;
    }

    StructurePtr = SmbiosTable;

    while(StructurePtr < SmbiosTableEnd && ((SMBIOS_STRUCTURE_HEADER*)StructurePtr)->Type != 127) {
        Size = GetStructureLength(StructurePtr, IN_SMBIOS_TABLE);
        if (Size == 0 || Size == 0xffff) {
            return NULL;
        }

        SmbiosTableNext = StructurePtr + Size;
        if (((SMBIOS_STRUCTURE_HEADER*)StructurePtr)->Handle == *Handle) {
            return StructurePtr;
        }
        StructurePtr = SmbiosTableNext;
    }
    return NULL;
}

/**
    Searches the structure table for a record with its handle
    equal to the input Handle.
    Returns the pointer to the structure if found.
    Returns NULL if not found

    @param Handle

    @return UINT8 Pointer to structure found.
            Sets Handle to the handle of the next structure

**/
UINT8*
GetStructureByHandleThenUpdateHandle(
    IN OUT  UINT16    *Handle
)
{
    UINT8   *SmbiosTableNext;
    UINT16  Size;

    if (SmbiosTable == NULL) {
        return NULL;
    }

    StructurePtr = SmbiosTable;

    while(StructurePtr < SmbiosTableEnd) {
        Size = GetStructureLength(StructurePtr, IN_SMBIOS_TABLE);
        if (Size == 0 || Size == 0xffff) {
            return NULL;
        }

        SmbiosTableNext = StructurePtr + Size;
        if (((DMI_STRUC*)StructurePtr)->Handle == *Handle) {
            if (SmbiosTableNext >= SmbiosTableEnd || ((DMI_STRUC*)StructurePtr)->Type == 127 ) *Handle = 0xffff;  //Last handle?
            else *Handle = ((DMI_STRUC*)SmbiosTableNext)->Handle;   //Return next handle
            return StructurePtr;
        }

        StructurePtr = SmbiosTableNext;
    }

    return NULL;
}

#if (SMBIOS_2X_SUPPORT == 1)
/**
    Searches the structure table for a record with its handle
    equal to the input Handle and copies its content into
    the provided buffer.

    @param Pointer to SMI function parameters

    @return GET_SMBIOS_STRUCTURE Input pointer "p" is loaded with
            structure data.

**/
UINT16
GetSmbiosV2Structure(
    IN OUT  GET_SMBIOS_STRUCTURE		*p
)
{


    UINT32      Size;

    if (SmbiosTable == NULL) return DMI_FUNCTION_NOT_SUPPORTED;

#if (((!defined(AFRI_PASS_TO_AFRI_PROTOCOL) && SMM_SUPPORT) || (AFRI_PASS_TO_SMM_COMMUNICATION && SMM_SUPPORT)) && !defined(_DMIEDIT_PROTOCOL_))
    if (AmiValidateMemoryBuffer((VOID*)p, sizeof(GET_SMBIOS_STRUCTURE)) == EFI_SUCCESS) {
        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->Handle32BitAddr, 2) == EFI_SUCCESS) {
            StructurePtr = GetStructureByHandleThenUpdateHandle((UINT16*)(UINTN)p->Handle32BitAddr);
            if (!StructurePtr) return DMI_INVALID_HANDLE;
        }
        else {
            return DMI_BAD_PARAMETER;
        }

        Size = GetStructureLength(StructurePtr, IN_SMBIOS_TABLE);
        if (Size == 0 || Size == 0xffff) {
            return DMI_BAD_PARAMETER;
        }

        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->Buffer32BitAddr, Size) == EFI_SUCCESS) {
            UINT8       *dest;

            dest = (UINT8*)(UINTN)p->Buffer32BitAddr;
            while(Size--) *dest++ = *StructurePtr++;        // Copy Table
            return 0;
        }
        else {
            return DMI_BAD_PARAMETER;
        }
    }
    else {
        return DMI_BAD_PARAMETER;
    }
#else
    StructurePtr = GetStructureByHandleThenUpdateHandle((UINT16*)(UINTN)p->Handle32BitAddr);
    if (!StructurePtr) return DMI_INVALID_HANDLE;

    Size = GetStructureLength(StructurePtr, IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_BAD_PARAMETER;
    }
    else {
        UINT8       *dest;

        dest = (UINT8*)(UINTN)p->Buffer32BitAddr;
        while(Size--) *dest++ = *StructurePtr++;        // Copy Table

        return 0;
    }
#endif
}
#endif                                          // SMBIOS_2X_SUPPORT

#if (SMBIOS_3X_SUPPORT == 1)
/**
    Searches the structure table for a record with its handle
    equal to the input Handle and copies its content into
    the provided buffer.

    @param Pointer to SMI function parameters

    @return GET_SMBIOS_STRUCTURE Input pointer "p" is loaded with
            structure data.

**/
UINT16
GetSmbiosV3Structure(
    IN OUT  GET_SMBIOS_V3_STRUCTURE    *p
)
{
    UINT32      Size;

    if (SmbiosTable == NULL) return DMI_FUNCTION_NOT_SUPPORTED;

#if (((!defined(AFRI_PASS_TO_AFRI_PROTOCOL) && SMM_SUPPORT) || (AFRI_PASS_TO_SMM_COMMUNICATION && SMM_SUPPORT)) && !defined(_DMIEDIT_PROTOCOL_))
    if (AmiValidateMemoryBuffer((VOID*)p, sizeof(GET_SMBIOS_V3_STRUCTURE)) == EFI_SUCCESS) {
        if (AmiValidateMemoryBuffer((VOID*)p->Handle64BitAddr, 2) == EFI_SUCCESS) {
            StructurePtr = GetStructureByHandleThenUpdateHandle((UINT16*)(UINTN)p->Handle64BitAddr);
            if (!StructurePtr) return DMI_INVALID_HANDLE;
        }
        else {
            return DMI_BAD_PARAMETER;
        }

        Size = GetStructureLength(StructurePtr, IN_SMBIOS_TABLE);
        if (Size == 0 || Size == 0xffff) {
            return DMI_BAD_PARAMETER;
        }

        if (AmiValidateMemoryBuffer((VOID*)p->Buffer64BitAddr, Size) == EFI_SUCCESS) {
            UINT8       *dest;

            dest = (UINT8*)(UINTN)p->Buffer64BitAddr;
            while(Size--) *dest++ = *StructurePtr++;        // Copy Table
        }
        else {
            return DMI_BAD_PARAMETER;
        }

        return 0;
    }
    else {
        return DMI_BAD_PARAMETER;
    }
#else
    StructurePtr = GetStructureByHandleThenUpdateHandle((UINT16*)(UINTN)p->Handle64BitAddr);
    if (!StructurePtr) return DMI_INVALID_HANDLE;

    Size = GetStructureLength(StructurePtr, IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_BAD_PARAMETER;
    }
    else {
        UINT8       *dest;

        dest = (UINT8*)(UINTN)p->Buffer64BitAddr;
        while(Size--) *dest++ = *StructurePtr++;        // Copy Table

        return 0;
    }
#endif
}
#endif                                          // SMBIOS_3X_SUPPORT

/**
    Returns the length of the structure pointed by BufferStart
    in bytes

    @param BufferStart
    @param SmbiosTableStructure

    @return UINT16 Size of the structure

    @notes === All structures in Smbios Table have been checked for
           === not crossing table end boundary at the beginning of the
           === SMI handler. In case a crossing condition was detected, 
           === the SMI handler would terminate so control will not come here
           ===
           === Each structure's length field and its double zero terminator
           === were checked so we don't need to validate them here.
**/
UINT16
GetStructureLength(
    IN UINT8    *BufferStart,
    IN BOOLEAN  SmbiosTableStructure
)
{
    UINT8       *BufferEnd = BufferStart;

    BufferEnd += ((SMBIOS_STRUCTURE_HEADER*)BufferStart)->Length;

    while (*(UINT16*)BufferEnd != 0) {
        BufferEnd++;
        if (SmbiosTableStructure) {
            if (BufferEnd >= SmbiosTableEnd) {
                // Crossing the end of Smbios Table, return 0 as error
                return 0;
            }
        }
        else {
            // Assuming structure length is less than 0xffff to avoid infinit loop
            if ((BufferEnd + 2 - BufferStart) == 0xffff) {
                return 0xffff;
            }
        }
    }

    return (UINT16)(BufferEnd + 2 - BufferStart);   // +2 for double zero terminator
}

/**
    Returns the instance of the input structure type and its handle

    @param Type
    @param Handle

    @return UINT8 Instance number (1-based) if found, or 0 if not found

**/
UINT8
GetInstanceByTypeHandle(
    IN UINT8    Type,
    IN UINT16   Handle
)
{
    UINT8   Instance = 0;                       // 1-based
    UINT16  Size;

    if (SmbiosTable == NULL) {
        return 0;
    }

    StructurePtr = SmbiosTable;

    while ((StructurePtr < SmbiosTableEnd) &&
           ((SMBIOS_STRUCTURE_HEADER*)StructurePtr)->Type != 127) {
        if (((SMBIOS_STRUCTURE_HEADER*)StructurePtr)->Type == Type) {
        	Instance ++;
        }

        if (((SMBIOS_STRUCTURE_HEADER*)StructurePtr)->Handle == Handle) {
            return Instance;
        }

        Size = GetStructureLength(StructurePtr, IN_SMBIOS_TABLE);
        if (Size == 0 || Size == 0xffff) {
            return 0;
        }

        StructurePtr = StructurePtr + Size;
    }

    return 0;					                // Not found
}

/**
    Find structure type starting from memory location pointed by
    Buffer

    @param Buffer
    @param StructureFoundPtr
    @param SearchType
    @param Instance


    @retval TRUE Structure found
    @retval FALSE Structure not found

        If SearchType is found:
        UINT8   **Buffer - Points to the next structure
        UINT8   **StructureFoundPtr - Points to the structure
        that was found
        If SearchType is not found:
        UINT8   **Buffer - No change
        UINT8   **StructureFoundPtr = NULL

**/
BOOLEAN
FindStructureType(
    IN OUT UINT8    **Buffer,
    IN OUT UINT8    **StructureFoundPtr,
	IN     UINT8    SearchType,
    IN     UINT8    Instance,       // 1-based
    IN     BOOLEAN  SmbiosTableStructure
)
{
    UINT8       *BufferPtr = *Buffer;
    UINT16      Size;
    BOOLEAN     FindStatus = FALSE;

    *StructureFoundPtr = NULL;
    while ((BufferPtr < SmbiosTableEnd) &&
           ((SMBIOS_STRUCTURE_HEADER*)BufferPtr)->Type != 127) {
        if (((SMBIOS_STRUCTURE_HEADER*)BufferPtr)->Type == SearchType) {
            // If this instance, set the find status flag and update the Buffer pointer
            if (--Instance == 0) {
                FindStatus = TRUE;
                *StructureFoundPtr = BufferPtr;
                Size = GetStructureLength(BufferPtr, SmbiosTableStructure);
                if (Size == 0 || Size == 0xffff) {
                    return FALSE;
                }
                *Buffer = BufferPtr + Size;
                break;
            }
        }
        Size = GetStructureLength(BufferPtr, SmbiosTableStructure);
        if (Size == 0 || Size == 0xffff) {
            return FALSE;
        }
        BufferPtr += Size;
    }
    if ((FindStatus == FALSE) && (SearchType == 127)) {
        FindStatus = TRUE;
        *StructureFoundPtr = BufferPtr;
        Size = GetStructureLength(BufferPtr, SmbiosTableStructure);
        if (Size == 0 || Size == 0xffff) {
            return FALSE;
        }
        *Buffer = BufferPtr + Size;
    }
    return FindStatus;
}

/**
    Returns the string array index for a given Offset in
    structure pointed by input StringTablePtr

    @param *StringTablePtr
    @param Offset

    @return UINT8 String array index

**/
UINT8
GetStringTableIndex(
    STRING_TABLE    *StringTablePtr,
    IN UINT8        Offset
)
{
    UINT8       i;

    for (i = 0; StringTablePtr->Offset != 0xff; i++) {
        if (StringTablePtr->Offset == Offset) break;
        StringTablePtr++;
    }

    return i;
}

/**
    Return the size from the Pointer Buffer to the last
    structure 127.

    @param Buffer

    @return UINT16 Size of remaining structure

**/
UINT16
GetRemainingStructuresSize(
    IN UINT8    *Buffer,
    IN BOOLEAN  SmbiosTableStructure
)
{
    UINT8   *EndPtr;
    UINT16  Length = 0;
    UINT16  BlockSize;

    if (SmbiosTableStructure) {
        EndPtr = SmbiosTableEnd;
    } else {
        EndPtr = Buffer + 0x10000;
    }
    
    while ((Buffer < EndPtr) &&
           ((SMBIOS_STRUCTURE_HEADER*)Buffer)->Type != 127) {
        BlockSize = GetStructureLength(Buffer, SmbiosTableStructure);
        if (BlockSize == 0 || BlockSize == 0xffff) {
            return BlockSize;
        }
        Length += BlockSize;
        Buffer += BlockSize;
    }

    BlockSize = GetStructureLength(Buffer, SmbiosTableStructure);
    if (BlockSize == 0 || BlockSize == 0xffff) {
        return BlockSize;
    }
    Length += BlockSize;
    return Length;
}

/**
    Returns the checksum of "length" bytes starting from the
    "*ChecksumSrc"

    @param ChecksumSrc
    @param length

    @return UINT8 Checksum value

**/
UINT8
SmbiosCheckSum(
    IN UINT8    *ChecksumSrc,
    IN UINT8    length
)
{
    UINT8     Checksum = 0;
    UINT8     i;

    for (i= 0; i < length; i++) {
        Checksum += *ChecksumSrc++;
    }
    return (0 - Checksum);
}

/**
    Returns the number of structures starting from Buffer til
    (and including) type 127 structure.

    @param  *Buffer

    @return Number of structures

**/
UINT16
GetNumberOfStructures(
    IN UINT8    *Buffer
)
{
    UINT8       *BufferPtr = Buffer;
    UINT16      SmbiosStrucCount = 1;
    UINT16      Size;

    while ((BufferPtr < SmbiosTableEnd) &&
           ((SMBIOS_STRUCTURE_HEADER*)BufferPtr)->Type != 127) {
        ++SmbiosStrucCount;
        Size = GetStructureLength(BufferPtr, IN_SMBIOS_TABLE);
        if (Size == 0 || Size == 0xffff) {
            return 0;
        }
        BufferPtr += Size;
    }
    return SmbiosStrucCount;
}

/**
    Returns the largest structure size

    @param Buffer

    @return UINT16 Largest structure size

**/
UINT16
GetLargestStructureSize(
    IN UINT8     *Buffer
)
{
    UINT8     *BufferPtr = Buffer;
    UINT16    LargestStructureSize = 0;
    UINT16    CurrentStructureSize;

    while ((BufferPtr < SmbiosTableEnd) &&
           ((SMBIOS_STRUCTURE_HEADER*)BufferPtr)->Type != 127) {
        UINT8     *LastBufferPtr;

        LastBufferPtr = BufferPtr;
        BufferPtr += ((SMBIOS_STRUCTURE_HEADER*)BufferPtr)->Length;
        while(TRUE) {
            if ((*(UINT16*)BufferPtr) == 0) {
                BufferPtr += 2;
                break;
            }
            BufferPtr++;
        }
        CurrentStructureSize = (UINT16)(BufferPtr - LastBufferPtr);
        if (CurrentStructureSize > LargestStructureSize) {
            LargestStructureSize = CurrentStructureSize;
        }
    }
    return LargestStructureSize;
}

/**
    Updates SMBIOS Entry Point Header

    @param None

    @retval EFI_SUCCESS

**/
EFI_STATUS
UpdateSmbiosTableHeader(
)
{
    UINT16      Size;

#if (SMBIOS_2X_SUPPORT == 1)
    Size = GetRemainingStructuresSize(SmbiosTable, NOT_IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    SmbiosTableEntryPoint->TableLength = Size;
    SmbiosTableEntryPoint->IntermediateChecksum = 0;
    SmbiosTableEntryPoint->IntermediateChecksum = SmbiosCheckSum((UINT8*)SmbiosTableEntryPoint + 0x10, 15);
    SmbiosTableEntryPoint->MaxStructureSize = GetLargestStructureSize(SmbiosTable);
    SmbiosTableEntryPoint->EntryPointStructureChecksum = 0;
    SmbiosTableEntryPoint->EntryPointStructureChecksum = SmbiosCheckSum((UINT8*)SmbiosTableEntryPoint,
                                                                        SmbiosTableEntryPoint->EntryPointLength);
    if (SmbiosTableEntryPointF000) {
        // Copy to second EPS 2.x Header in F000
        MemCpy(SmbiosTableEntryPointF000, SmbiosTableEntryPoint, sizeof(SMBIOS_TABLE_ENTRY_POINT));
    }
#endif                                          // SMBIOS_2X_SUPPORT

#if (SMBIOS_3X_SUPPORT == 1)
    Size = GetRemainingStructuresSize(SmbiosTable, NOT_IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    SmbiosV3TableEntryPoint->TableMaximumSize = Size;
    // Update Checksums in EPS Header
    SmbiosV3TableEntryPoint->EntryPointStructureChecksum = 0;
    SmbiosV3TableEntryPoint->EntryPointStructureChecksum = SmbiosCheckSum((UINT8*)SmbiosV3TableEntryPoint,
                                                                          SmbiosV3TableEntryPoint->EntryPointLength);
#endif                                          // SMBIOS_3X_SUPPORT

    return  EFI_SUCCESS;
}

/**
    Return pointer to the input type string table

    @param Structure Type

    @return VOID Pointer to the input type string table
            (or NULL if not found)

**/
VOID*
GetTypeTable(
    IN UINT8    StructType
)
{
    UINT8       Index;

    switch (StructType) {
        case    0:
        case    1:
        case    2:
        case    3:
        case    4:  Index = StructType;
                    break;
        case   22:  Index = 5;
                    break;
        case   39:  Index = 6;
                    break;
        default:    Index = 0xff;
    }

    if (Index != 0xff) {
        return StringTable[Index];
    }
    else {
        return NULL;
    }
}

/**
    Returns the string offset for StringNumber from input string
    buffer BufferStart

    @param BufferStart
    @param StringNumber (1-based)

    @return UINT16 Offset from BufferStart

**/
UINT16
GetStringOffset(
    IN UINT8    *BufferStart,
    IN UINT8    StringNumber                    // 1-based
)
{
    UINT8       *BufferEnd = BufferStart;

    while (--StringNumber) {
        while(*BufferEnd != 0) {
            BufferEnd++;
        }
        BufferEnd++;
    }

    return (UINT16)(BufferEnd - BufferStart);
}

/**
    Returns pointer to the string number in structure BufferPtr

    @param BufferPtr
    @param StringNumber

    @return UINT8 BufferPtr = Pointer to the #StringNumber string

**/
BOOLEAN
FindString(
    IN OUT UINT8    **BufferPtr,
    IN     UINT8    StringNumber                // 1-based
)
{
    *BufferPtr += ((SMBIOS_STRUCTURE_HEADER*)*BufferPtr)->Length;
    *BufferPtr += GetStringOffset(*BufferPtr, StringNumber);
    return TRUE;
}

/**
    Return the largest string number in a structure

    @param StructPtr
    @param StrTablePtr

    @return UINT8 String number (1-based)
            (or 0 if not found)

**/
UINT8
FindLargestStrNumber (
    IN UINT8            *StructPtr,
    IN STRING_TABLE     *StrTablePtr
)
{
    UINT8       Number;
    UINT8       StrNumber = 0;

    // Find largest string number from structure
    while (StrTablePtr->Offset != 0xff) {
        Number = *(StructPtr + StrTablePtr->Offset);
        if (Number > StrNumber) {
            StrNumber = Number;
        }
        StrTablePtr++;
    }

    return StrNumber;                           // 1-based
}

/**
    Zero out the string number in StructPtr

    @param StructPtr
    @param StrNumber

    @retval None

**/
VOID
DeleteStringNumber (
    IN UINT8    *StructPtr,
    IN UINT8    StrNumber
)
{
    UINT8           Number;
    STRING_TABLE    *StrTablePtr;

    StrTablePtr = GetTypeTable(((SMBIOS_STRUCTURE_HEADER*)StructPtr)->Type);

    while (StrTablePtr->Offset != 0xff) {
        Number = *(StructPtr + StrTablePtr->Offset);
        if (Number > StrNumber) {
            *(StructPtr + StrTablePtr->Offset) = Number - 1;
        }
        if (Number == StrNumber) {
            *(StructPtr + StrTablePtr->Offset) = 0;
        }
        StrTablePtr++;
    }
}

/**
    Delete string at Offset

    @param StructPtr
    @param Offset

    @retval None

**/
VOID
DeleteString (
    IN UINT8            *StructPtr,
    IN UINT8            Offset
)
{
    UINT8       StrNumber;
    UINT8       *TempPtr;
    UINT8       *StructEndPtr;
    UINTN       RemainingSize;

    StrNumber = *(StructPtr + Offset);

    // Delete string number
    DeleteStringNumber(StructPtr, StrNumber);

    FindString(&StructPtr, StrNumber);                      // StructPtr = StrNumber string
    TempPtr = StructPtr + Strlen((char*)StructPtr) + 1;     // Move pointer to next string

    // Find end of structure
    StructEndPtr = TempPtr;
    while(*(UINT16*)StructEndPtr != 0) {
        StructEndPtr++;
    }

    // Copy remaining strings
    RemainingSize = StructEndPtr + 2 - TempPtr;             // Including double NULL characters
    MemCpy(StructPtr, TempPtr, RemainingSize);
}

/**
    Replace the #StringNum in the input buffer *DestStructPtr
    with StringData

    @param DestStructPtr Pointer to structure to be updated
    @param StringNum String number (1 based)
    @param StringData String with NULL terminated character

    @return EFI_STATUS

**/
EFI_STATUS
ReplaceString(
    IN UINT8    *DestStructPtr,
    IN UINT8    StringNum,
    IN UINT8    *StringData
)
{
    UINTN       StringSize = 0;
    UINT8       *TempPtr;
    UINT8       *NextStrPtr;
    UINT8       *StructEndPtr;
    UINTN       RemainingSize;

    FindString(&DestStructPtr, StringNum);
    NextStrPtr = DestStructPtr;
    StructEndPtr = DestStructPtr;

    while(*NextStrPtr != 0) {
        NextStrPtr++;
    }

    // NextStrPtr = Pointer to the next string
    NextStrPtr++;

    while(*(UINT16*)StructEndPtr != 0) {
        StructEndPtr++;
    }

    RemainingSize = StructEndPtr + 2 - NextStrPtr;  // Including double NULL characters

    TempPtr = StringData;
    while (*(TempPtr++) != 0) {
        StringSize++;
    }
    StringSize++;                                   // Including NULL character

    // Copy remaining strings
    MemCpy(DestStructPtr + StringSize, NextStrPtr, RemainingSize);

    // Copy the string
    MemCpy(DestStructPtr, StringData, StringSize);

    return EFI_SUCCESS;
}

/**
    Add new string number for a structure "Type" at "Offset".
    Return the string number, assuming all strings exist in the
    structure according to the Smbios specification

    @param Pointer to SmbiosTable or Structure
    @param Type
    @param Offset

    @return UINT8 String number
            (0xff if not found)

**/
UINT8
AddStringNumber(
    IN  UINT8       *StructurePtr,
    IN  UINT8       Type,
    UINT8           Offset
)
{
    STRING_TABLE    *StrTablePtr;
    UINT8           StrNumber = 0;
    UINT8           Number;

    StrTablePtr = GetTypeTable(Type);
    if (StrTablePtr != NULL) {
        // Find largest string number from structure
        while (StrTablePtr->Offset != 0xff) {
            Number = *(StructurePtr + StrTablePtr->Offset);
            if (Number > StrNumber) {
                StrNumber = Number;
            }
            StrTablePtr++;
        }

        ++StrNumber;

        // Assign next string number to structure at input Offset
        *(StructurePtr + Offset) = StrNumber;

        return StrNumber;
    }

    return 0xff;
}

/**
    Add NULL terminator to the end of the structure

    @param StructPtr
    @param StrTablePtr

    @retval None

**/
VOID
AddNullTerminator (
    IN UINT8            *StructPtr,
    IN STRING_TABLE     *StrTablePtr
)
{
    UINT8       StrNumber;
    UINT8       i;

    // Find largest string number
    StrNumber = FindLargestStrNumber(StructPtr, StrTablePtr);

    // Skip to string section
    StructPtr += ((SMBIOS_STRUCTURE_HEADER*)StructPtr)->Length;

    // Move pointer to end of last string
    for (i = 0; i < StrNumber; i++) {
        while (*StructPtr != 0) StructPtr++;
        StructPtr++;
    }

    // Add NULL terminator
    *StructPtr = 0;
}

/**
    Updates strings in SMBIOS Structure with input Handle
    in Runtime with DMI data

    @param Handle
    @param TableInfo
    @param Data

    @return UINT16 Status

**/
UINT16
UpdateStrings(
    IN UINT16           Handle,
    IN TABLE_INFO       TableInfo,
    IN UINT8            *Data
)
{
    UINT8               *TablePtr;
    UINT8               *TempBuffer;
    UINT8               *StructPtr;
    UINT8               i;
    UINT16              BlockSize;
    UINT16              AvailableBlkSize = 0;
    STRING_TABLE        *StrTablePtr;
    UINT8               StrNumber;
    UINT8               Instance;
    UINT16              Size;

    // Check if enough space
#if (SMBIOS_2X_SUPPORT == 1)
    AvailableBlkSize = MaximumBufferSize - (UINT16)SmbiosTableSize;
#else
#if (SMBIOS_3X_SUPPORT == 1)
    Size = GetRemainingStructuresSize(SmbiosTable, IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    AvailableBlkSize = MaximumBufferSize - Size;
#endif                                          // SMBIOS_3X_SUPPORT
#endif                                          // SMBIOS_2X_SUPPORT
    if (AvailableBlkSize < (Strlen((char*)Data) + 1)) {
        return DMI_BAD_PARAMETER;               // Not enough space
    }

    // Get pointer to structure to be updated
    StructPtr = GetStructureByHandle(&Handle);
    if (StructPtr == NULL) {
        return DMI_INVALID_HANDLE;
    }

    // Get pointer to the StringTable
    StrTablePtr = GetTypeTable(((SMBIOS_STRUCTURE_HEADER*)StructPtr)->Type);
    if (((SMBIOS_STRUCTURE_HEADER*)StructPtr)->Type == 3) {
        Instance = GetInstanceByTypeHandle(3, Handle);
        StrTablePtr += 6 * (Instance - 1);
    }

    if (StrTablePtr == NULL) return DMI_BAD_PARAMETER;

    // Copy structure data
    TempBuffer = ScratchBufferPtr;
    BlockSize = GetStructureLength(StructPtr, IN_SMBIOS_TABLE);
    if (BlockSize == 0 || BlockSize == 0xffff) {
        return DMI_BAD_PARAMETER;
    }
    MemCpy(TempBuffer, StructPtr, BlockSize);

    // Set TablePtr to next structure
    TablePtr = StructPtr + BlockSize;

    // Update String fields
    for (i = 0; StrTablePtr[i].Offset != 0xff; i++) {
        // Update string at input Offset
        if (StrTablePtr[i].Offset == TableInfo.Offset) {
            // Update string if input data not empty, else delete it
            if (Strlen((char*)Data)) {
                // Add string if does not exist, else replace it
                StrNumber = *(TempBuffer + TableInfo.Offset);
                if (StrNumber == 0) {
                    StrNumber = AddStringNumber(TempBuffer, TableInfo.Type, TableInfo.Offset);
                }
                ReplaceString(TempBuffer, StrNumber, Data);
            }
            else {
                DeleteString(TempBuffer, TableInfo.Offset);
            }
        }
    }

    // Add structure terminator Null byte
    AddNullTerminator(TempBuffer, StrTablePtr);

    BlockSize = GetRemainingStructuresSize(TablePtr, IN_SMBIOS_TABLE);
    if (BlockSize == 0 || BlockSize == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    Size = GetStructureLength(TempBuffer, NOT_IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    MemCpy(TempBuffer + Size, TablePtr, BlockSize);

    // Replace all DMI data with TempBuffer
    Size = GetStructureLength(TempBuffer, NOT_IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    BlockSize += Size;
    MemCpy(StructPtr, TempBuffer, BlockSize);

    // Update SMBIOS Structure Table Entry Point - Structure Table Length, Intermediate checksum
    UpdateSmbiosTableHeader();

    return DMI_SUCCESS;
}

#if OEM_STRING_INFO
/**
    Updates SMBIOS Type 11 Structure in Runtime with DMI data

    @param Handle
    @param TableInfo
    @param Data

    @return UINT8 Status

**/
UINT8
DynamicUpdateType11(
    IN UINT16      Handle,
    IN TABLE_INFO  TableInfo,
    IN UINT8       *Data
)
{
    UINT8               *TablePtr;
    UINT8               *TempBuffer;
    UINT8               *StructPtr;
    UINT16              BlockSize;
    UINT16              StringSize;
    UINT16              Size;
    UINT8               i;
    UINT16              AvailableBlkSize = 0;
    UINT8               Count;

    StructPtr = GetStructureByHandle(&Handle);
    if (StructPtr == NULL) {
        return DMI_INVALID_HANDLE;
    }

    TablePtr = StructPtr;
    TempBuffer = ScratchBufferPtr;

#if (SMBIOS_2X_SUPPORT == 1)
    AvailableBlkSize = MaximumBufferSize - (UINT16)SmbiosTableSize;
#else
    BlockSize = GetRemainingStructuresSize(SmbiosTable, IN_SMBIOS_TABLE);
    if (BlockSize == 0 || BlockSize == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    AvailableBlkSize = MaximumBufferSize - BlockSize;
#endif                                          // SMBIOS_2X_SUPPORT

    if (AvailableBlkSize < (Strlen((CHAR8*)Data) + 1)) {
        return DMI_BAD_PARAMETER;               // Not enough space
    }

    // Copy structure data (without string data)
    BlockSize = ((SMBIOS_STRUCTURE_HEADER*)TablePtr)->Length;
    MemCpy(TempBuffer, TablePtr, BlockSize);
    Count = ((SMBIOS_OEM_STRINGS_INFO*)TempBuffer)->Count;

    TablePtr += BlockSize;
    TempBuffer += BlockSize;

    // string fields
    for (i = 1; i < (Count + 1); i++) {
        StringSize = (UINT16)Strlen((CHAR8*)TablePtr) + 1;  // Size including string NULL terminator
        if (TableInfo.Offset == i) {
            BlockSize = (UINT16)Strlen((CHAR8*)Data) + 1;
            MemCpy(TempBuffer, Data, BlockSize);
            TempBuffer += BlockSize;
        }
        else {
            MemCpy(TempBuffer, TablePtr, StringSize);
            TempBuffer += StringSize;
        }
        TablePtr += StringSize;
    }

    // Add NULL byte for end of string-set
    *TempBuffer = 0;
    TempBuffer++;
    TablePtr++;

    BlockSize = GetRemainingStructuresSize(TablePtr, IN_SMBIOS_TABLE);
    if (BlockSize == 0 || BlockSize == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    MemCpy(TempBuffer, TablePtr, BlockSize);

    // Replace all DMI data with TempBuffer
    TempBuffer = ScratchBufferPtr;
    Size = GetStructureLength(TempBuffer, NOT_IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    BlockSize += Size;
    MemCpy(StructPtr, TempBuffer, BlockSize);

    // Update SMBIOS Structure Table Entry Point - Structure Table Length, Intermediate checksum
    UpdateSmbiosTableHeader();

    return DMI_SUCCESS;
}
#endif                                          // OEM_STRING_INFO

#if SYSTEM_CONFIG_OPTION_INFO
/**
    Updates SMBIOS Type 12 Structure in Runtime with DMI data

    @param Handle
    @param TableInfo
    @param Data

    @return UINT8 Status

**/
UINT8
DynamicUpdateType12(
    IN UINT16      Handle,
    IN TABLE_INFO  TableInfo,
    IN UINT8       *Data
)
{
    UINT8               *TablePtr;
    UINT8               *TempBuffer;
    UINT8               *StructPtr;
    UINT16              BlockSize;
    UINT16              StringSize;
    UINT16              Size;
    UINT8               i;
    UINT16              AvailableBlkSize = 0;
    UINT8               Count;

    StructPtr = GetStructureByHandle(&Handle);
    if (StructPtr == NULL) {
        return DMI_INVALID_HANDLE;
    }

    TablePtr = StructPtr;
    TempBuffer = ScratchBufferPtr;

#if (SMBIOS_2X_SUPPORT == 1)
    AvailableBlkSize = MaximumBufferSize - (UINT16)SmbiosTableSize;
#else
    BlockSize = GetRemainingStructuresSize(SmbiosTable, IN_SMBIOS_TABLE);
    if (BlockSize == 0 || BlockSize == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    AvailableBlkSize = MaximumBufferSize - BlockSize;
#endif                                          // SMBIOS_2X_SUPPORT

    if (AvailableBlkSize < (Strlen((CHAR8*)Data) + 1)) {
        return DMI_BAD_PARAMETER;               // Not enough space
    }

    // Copy structure data (without string data)
    BlockSize = ((SMBIOS_STRUCTURE_HEADER*)TablePtr)->Length;
    MemCpy(TempBuffer, TablePtr, BlockSize);
    Count = ((SMBIOS_SYSTEM_CONFIG_INFO*)TempBuffer)->Count;

    TablePtr += BlockSize;
    TempBuffer += BlockSize;

    // string fields
    for (i = 1; i < (Count + 1); i++) {
        StringSize = (UINT16)Strlen((CHAR8*)TablePtr) + 1;  // Size including string NULL terminator
        if (TableInfo.Offset == i) {
            BlockSize = (UINT16)Strlen((CHAR8*)Data) + 1;
            MemCpy(TempBuffer, Data, BlockSize);
            TempBuffer += BlockSize;
        }
        else {
            MemCpy(TempBuffer, TablePtr, StringSize);
            TempBuffer += StringSize;
        }
        TablePtr += StringSize;
    }

    // Add NULL byte for end of string-set
    *TempBuffer = 0;
    TempBuffer++;
    TablePtr++;

    BlockSize = GetRemainingStructuresSize(TablePtr, IN_SMBIOS_TABLE);
    if (BlockSize == 0 || BlockSize == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    MemCpy(TempBuffer, TablePtr, BlockSize);

    // Replace all DMI data with TempBuffer
    TempBuffer = ScratchBufferPtr;
    Size = GetStructureLength(TempBuffer, NOT_IN_SMBIOS_TABLE);
    if (Size == 0 || Size == 0xffff) {
        return DMI_INVALID_ADDRESS;
    }
    BlockSize += Size;
    MemCpy(StructPtr, TempBuffer, BlockSize);

    // Update SMBIOS Structure Table Entry Point - Structure Table Length, Intermediate checksum
    UpdateSmbiosTableHeader();

    return DMI_SUCCESS;
}
#endif                                          // SYSTEM_CONFIG_OPTION_INFO

/////////////////////////////////////////////
// Worker function for setting structures. //
/////////////////////////////////////////////

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC == 2
/**
    Store DMIEdit data into global variables "DmiArray" and "DmiArraySize"

    @param Var
    @param Data
    @param DataSize

    @return EFI_STATUS

**/
EFI_STATUS
StoreNvramData(
    IN  CHAR16  *Var,
    IN  VOID    *Data,
    IN  UINTN   DataSize,
    IN  UINTN   Index
)
{
    EFI_STATUS  Status;
    UINTN       Size;

	// Check if variable already exists
    //
    // Size of zero is used to detect if the variable exists.
    // If the variable exists, an error code of EFI_BUFFER_TOO_SMALL
    // would be returned
    Size = 0;
    Status = pRS->GetVariable(
                        Var,
                        &gAmiSmbiosNvramGuid,
                        NULL,
                        &Size,
                        NULL);

    if (Status == EFI_NOT_FOUND || Index == 1) {
		// Record not present or DmiArray not present (when Index = 1), increment record count
        DmiArray[0].Type += 1;

        Size = DMI_ARRAY_SIZE;
        Status = pRS->SetVariable(
                            DmiArrayVar,
                            &gAmiSmbiosNvramGuid,
                            EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                            Size,
                            &DmiArray);
        if (EFI_ERROR(Status)) {
#if DMIEDIT_DEBUG_TRACE
            DEBUG((DEBUG_ERROR | DEBUG_WARN, "Failed to store DmiArrayVar\n"));
#endif
            return EFI_INVALID_PARAMETER;
        }
    }

	// Update DMI data record if already exists,
	// or store new record if total record count in DmiArray was successfully
	// updated
    if (Status == EFI_BUFFER_TOO_SMALL || Status == EFI_SUCCESS) {
	    Status = pRS->SetVariable(
	                        Var,
	                        &gAmiSmbiosNvramGuid,
	                        EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                            DataSize,
	                        Data);
        if (EFI_ERROR(Status)) {
#if DMIEDIT_DEBUG_TRACE
            DEBUG((DEBUG_ERROR | DEBUG_WARN, "Failed to store DMI data record\n"));
#endif
        }
	}

    return Status;
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

/**
    Returns the data size for DMI Function 0x52

    @param Data

    @return UINT16 Data Size

**/
UINT16
GetDmiDataSize(
    IN SET_SMBIOS_STRUCTURE_DATA   *Data
)
{
    switch(Data->Command) {
        case 0:
                return 1;
        case 1:
                return 2;
        case 2:
                return 4;
        case 4:
                return 0;                       // Delete command, size does not matter
        default:
                return Data->DataLength;        // Add, String, or Block change command
    }
}

/**
    Fills "TableInfo" with data from DMI Function 0x52

    @param Handle
    @param Data
    @param TableInfo

    @return UINT16 Data Size

**/
VOID
GetInputDataInfo(
    IN      UINT16                      Handle,
    IN      SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN OUT  TABLE_INFO                  *TableInfo
)
{
    TableInfo->Type = Data->StructureHeader.Type;
    TableInfo->Offset = Data->FieldOffset;
    TableInfo->Reserved = 0;
    TableInfo->Flags = DMIEDIT_EXTENDED_HDR;
    TableInfo->HdrLength = sizeof(TABLE_INFO);
    TableInfo->Size = GetDmiDataSize(Data);
    TableInfo->Handle = Handle;
}

#if (SMBIOS_DMIEDIT_DATA_LOC == 2)
/**
    Clear DmiArray Nvram variable

    @param None

    @return None
**/
VOID
ClearDmiArray(VOID) {
    MemSet(DmiArray, DMI_ARRAY_SIZE, 0);

    pRS->SetVariable(
                    DmiArrayVar,
                    &gAmiSmbiosNvramGuid,
                    EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    DmiArraySize,
                    &DmiArray);
}

/**
    Get DmiArray Nvram variable

    @param None

    @return EFI_STATUS Status
                  EFI_INVALID_PARAMETER in case DmiArray was compromised
                  EFI_NOT_FOUND if DmiArray is not found
**/
EFI_STATUS
GetDmiArray(VOID) {
    EFI_STATUS      Status;
    UINTN           Size;

    Size = DMI_ARRAY_SIZE;
    Status = pRS->GetVariable(
                        DmiArrayVar,
                        &gAmiSmbiosNvramGuid,
                        NULL,
                        &Size,
                        &DmiArray);

    if (Status == EFI_SUCCESS) {
        if (Size != DMI_ARRAY_SIZE) {
        // When DmiArraySize is not equal to DMI_ARRAY_SIZE,
        // then DmiArray was compromised, clear it
#if DMIEDIT_DEBUG_TRACE
        DEBUG((DEBUG_INFO, "DmiArray was either corrupted or compromised, return error"));
#endif
        Status = EFI_INVALID_PARAMETER;
        }
    }
    return Status;
}

/**
    Prepare DmiArray and create variable named DmiVarXXXXXXXX for update

    @param Type
    @param Handle
    @param Offset

    @return EFI_STATUS

    @note

**/
EFI_STATUS
PrepareDmiArrayDmiVar(
    UINT8   Type,
    UINT16  Handle,
    UINT8   Offset,
    UINT8   Flags
)
{
    EFI_STATUS  Status;

    //
    // Get number of DMI data records in NVRAM
    //
    // Note: DMI data record actually starts with record #1,
    //       first record #0 holds total number of DMI data records
    //       instead of TABLE_INFO
    //       ===> DmiArray[0].Type = count
    //
    Status = GetDmiArray();

    if (EFI_ERROR(Status)) {
        if (Status == EFI_NOT_FOUND) {
            ClearDmiArray();
        }
        else {
#if DMIEDIT_DEBUG_TRACE
            DEBUG((DEBUG_INFO, "Array is bad, exiting with no change ..."));
#endif
            return EFI_INVALID_PARAMETER;
        }
    }

    Index = DmiArray[0].Type;               // Note: DmiArray[0] has count # instead of Type/Offset
    if (Index == (DMI_ARRAY_COUNT - 1)) {   // Checking for last possible index of 0xFF
        // DmiArray is full. New entry can not be added
#if DMIEDIT_DEBUG_TRACE
        DEBUG((DEBUG_INFO, "Array is full, exiting with no change ..."));
#endif
        return EFI_INVALID_PARAMETER;
    }

    ++Index;

    DmiArray[Index].Type = Type;
    DmiArray[Index].Handle = Handle;
    DmiArray[Index].Offset = Offset;
    DmiArray[Index].Flags = Flags;

    Swprintf(Var, L"DmiVar%02x%04x%02x%02x",
               DmiArray[Index].Type,
               DmiArray[Index].Handle,
               DmiArray[Index].Offset,
               DmiArray[Index].Flags);

    return  EFI_SUCCESS;
}
#endif											// SMBIOS_DMIEDIT_DATA_LOC

/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 0 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT16 Status

    @note  Type 0 Offset 8 (Release Date) is a fixed form string. This
           function only checks for proper length. It is up to the DMI
           editing utility to check for the proper format.
**/
UINT16
SetType0(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS  Status;
    TABLE_INFO  TableInfo;

    if (Data->Command != 5) return DMI_BAD_PARAMETER;

    if ( Data->FieldOffset != 4
      && Data->FieldOffset != 5
      && Data->FieldOffset != 8
    ) return DMI_BAD_PARAMETER;

    if ((Data->FieldOffset == 8) && (Data->DataLength != 11)) {
        return DMI_BAD_PARAMETER;               // Date string is fixed size
    }

    if (Set == FALSE) return DMI_SUCCESS;

    if (isWriteOnce(0, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2

    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);

#else
{
	//
	// Get number of DMI data records in NVRAM
	//
	// Note: DMI data record actually starts with record #1,
	//		 first record #0 holds total number of DMI data records
	//       instead of TABLE_INFO
	//       ===> DmiArray[0].Type = count
	//
    Status = PrepareDmiArrayDmiVar(0, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    // Dynamically update strings in Smbios table
    return UpdateStrings(Handle, TableInfo, Data->StructureData);
}

/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 1 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType1(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS          Status;
    TABLE_INFO          TableInfo;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
    switch (Data->FieldOffset) {
        case 0x04 :
        case 0x05 :
        case 0x06 :
        case 0x07 :
        case 0x19 :
        case 0x1a : if (Data->Command != 5) return DMI_BAD_PARAMETER;

                    if (Set == FALSE) return DMI_SUCCESS;

				    if (isWriteOnce(1, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

                    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);
                    break;

        default:    if ((Data->FieldOffset > 0x07) && (Data->FieldOffset < 0x18)) {
                        UINT8       *Ptr;
                        UINT8       *UuidPtr;
                        UINT8       i;

                        Ptr = GetStructureByHandle(&Handle);
                        UuidPtr = (UINT8*)&((SMBIOS_SYSTEM_INFO*)Ptr)->Uuid;
                        Ptr = UuidPtr + Data->FieldOffset - 8;

                        if (Data->Command < 3) {
                            if (Data->Command == 0) {
                                *Ptr &= (UINT8)Data->ChangeMask;
                                *Ptr |= (UINT8)Data->ChangeValue;
                            }
                            if (Data->Command == 1) {
                                *(UINT16*)Ptr &= (UINT16)Data->ChangeMask;
                                *(UINT16*)Ptr |= (UINT16)Data->ChangeValue;
                            }
                            if (Data->Command == 2) {
                                *(UINT32*)Ptr &= Data->ChangeMask;
                                *(UINT32*)Ptr |= Data->ChangeValue;
                            }
                        }
                        else if (Data->Command == 6) {
                            for (i = 0; i < (UINT8)TableInfo.Size; i++) {
                                Ptr[i] = Data->StructureData[i];
                            }
                        }
                        else {
                            return DMI_BAD_PARAMETER;
                        }

                        if (Set == FALSE) return DMI_SUCCESS;

					    if (isWriteOnce(1, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

                        TableInfo.Offset = 8;
                        TableInfo.Size = sizeof(EFI_GUID);

                        Status = UpdateSmbiosTable(&TableInfo, UuidPtr);
                    }
                    else {
                        return DMI_BAD_PARAMETER;
                    }
    }
#else
{
    VOID    *NvramData;

    NvramData = &Data->StructureData;

    Status = PrepareDmiArrayDmiVar(1, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    switch (Data->FieldOffset) {
        case 0x04 :
        case 0x05 :
        case 0x06 :
        case 0x07 :
        case 0x19 :
        case 0x1a : if (Data->Command != 5) return DMI_BAD_PARAMETER;
                    break;

        default:    if ((Data->FieldOffset > 0x07) && (Data->FieldOffset < 0x18)) {
                        UINT8       *Ptr;
                        UINT8       *UuidPtr;
                        UINT8       i;

                        Ptr = GetStructureByHandle(&Handle);
                        UuidPtr = (UINT8*)&((SMBIOS_SYSTEM_INFO*)Ptr)->Uuid;
                        Ptr = UuidPtr + Data->FieldOffset - 8;

                        if (Data->Command < 3) {
                            if (Data->Command == 0) {
                                *Ptr &= (UINT8)Data->ChangeMask;
                                *Ptr |= (UINT8)Data->ChangeValue;
                            }
                            if (Data->Command == 1) {
                                *(UINT16*)Ptr &= (UINT16)Data->ChangeMask;
                                *(UINT16*)Ptr |= (UINT16)Data->ChangeValue;
                            }
                            if (Data->Command == 2) {
                                *(UINT32*)Ptr &= Data->ChangeMask;
                                *(UINT32*)Ptr |= Data->ChangeValue;
                            }
                        }
                        else if (Data->Command == 6) {
                            for (i = 0; i < (UINT8)TableInfo.Size; i++) {
                                Ptr[i] = Data->StructureData[i];
                            }
                        }
                        else {
                            return DMI_BAD_PARAMETER;
                        }

                        DmiArray[Index].Offset = 0x08;
                        NvramData = UuidPtr;
                        TableInfo.Offset = 8;
                        TableInfo.Size = sizeof(EFI_GUID);
                    }
                    else {
                        return DMI_BAD_PARAMETER;
                    }
    }

    if (Set == FALSE) return DMI_SUCCESS;

    if (isWriteOnce(1, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    Status = StoreNvramData(Var, NvramData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

#if DMIEDIT_DEBUG_TRACE
    DEBUG((DEBUG_INFO, "Change structure. Type = %x, Handle = %x, Offset = %x\n",\
                                  TableInfo.Type,\
                                  TableInfo.Handle,\
                                  TableInfo.Offset));
#endif

    if (Status) {
        return DMI_NO_CHANGE;
    }

    if ((Data->FieldOffset > 0x07) && (Data->FieldOffset < 0x18)) {
        return DMI_SUCCESS;
    }
    else {
        // Dynamically update strings in Smbios table
        return UpdateStrings(Handle, TableInfo, Data->StructureData);
    }
}

#if BASE_BOARD_INFO
/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 2 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType2(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS          Status;
    TABLE_INFO          TableInfo;

    if (Data->Command != 5) return DMI_BAD_PARAMETER;
    if ( Data->FieldOffset != 4
      && Data->FieldOffset != 5
      && Data->FieldOffset != 6
      && Data->FieldOffset != 7
      && Data->FieldOffset != 8
      && Data->FieldOffset != 0x0a
    ) return DMI_BAD_PARAMETER;

    if (Set == FALSE) return DMI_SUCCESS;

    if (isWriteOnce(2, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2

    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);

#else
{
    Status = PrepareDmiArrayDmiVar(2, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (Status) {
        return DMI_NO_CHANGE;
    }

    // Dynamically update strings in Smbios table
    return UpdateStrings(Handle, TableInfo, Data->StructureData);
}
#endif                                          // BASE_BOARD_INFO

#if SYS_CHASSIS_INFO
/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 3 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType3(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS          Status;
    TABLE_INFO          TableInfo;
    UINT8               *StructPtr;
    UINT8               Instance;
    STRING_TABLE        *StringTablePtr;

    switch (Data->FieldOffset) {
        case 4:
        case 6:
        case 7:
        case 8:     if (Data->Command != 5) return DMI_BAD_PARAMETER;
                    break;
        case 5:
        case 0x11:
        case 0x12:  if (Data->Command != 0) return DMI_BAD_PARAMETER;
                    break;
        case 0x0d:  if (Data->Command != 2) return DMI_BAD_PARAMETER;
                    break;
        default:    {
                        // Get instance number
                        Instance = GetInstanceByTypeHandle(3, Handle);
                        StringTablePtr = &StringType_3[0][0];
                        StringTablePtr += 6 * (Instance - 1);

                        while (StringTablePtr->Offset != 0xff) {
                            if (StringTablePtr->Offset == Data->FieldOffset) {
                                break;
                            }

                            StringTablePtr++;
                        };

                        if (StringTablePtr->Offset != 0xff) {
                            if (Data->Command != 0x5) {
                                return DMI_BAD_PARAMETER;
                            }
                        }
                        else {
                            return DMI_BAD_PARAMETER;
                        }
                    }
    }

    if (Set == FALSE) return DMI_SUCCESS;

    if (isWriteOnce(3, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
    if (Data->Command == 0 || Data->Command == 0x2)
        *(UINT32*)Data->StructureData = Data->ChangeValue;

    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);

#else
{
    Status = PrepareDmiArrayDmiVar(3, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    if (Data->Command == 0 || Data->Command == 0x2)
        *(UINT32*)Data->StructureData = Data->ChangeValue;

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (Status) {
        return DMI_NO_CHANGE;
    }

    // Dynamically update the structure in the Smbios table
    StructPtr = GetStructureByHandle(&Handle);
    if (StructPtr != NULL) {
        switch (Data->FieldOffset) {
            case 0x05:  ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->Type &= (UINT8)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->Type |= (UINT8)Data->ChangeValue;
                        break;
            case 0x0d:  ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->OemDefined &= (UINT32)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->OemDefined |= (UINT32)Data->ChangeValue;
                        break;
            case 0x11:  ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->Height &= (UINT8)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->Height |= (UINT8)Data->ChangeValue;
                        break;
            case 0x12:  ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->NumberOfPowerCord &= (UINT8)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_ENCLOSURE_INFO*)StructPtr)->NumberOfPowerCord |= (UINT8)Data->ChangeValue;
        }
    }

    return UpdateStrings(Handle, TableInfo, Data->StructureData);
}
#endif                                          // SYS_CHASSIS_INFO

#if PROCESSOR_DMIEDIT_SUPPORT
/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 4 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType4(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS          Status;
    TABLE_INFO          TableInfo;

    switch (Data->FieldOffset) {
        case 0x20:
        case 0x21:
        case 0x22:  if (Data->Command != 0x5) return DMI_BAD_PARAMETER;
                    break;
        default:    return DMI_BAD_PARAMETER;
    }

    if (Set == FALSE) return DMI_SUCCESS;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);
#else
{
    Status = PrepareDmiArrayDmiVar(4, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (Status) {
        return DMI_NO_CHANGE;
    }

    // Dynamically update the structure in the Smbios table
    return UpdateStrings(Handle, TableInfo, Data->StructureData);
}
#endif                                          // PROCESSOR_DMIEDIT_SUPPORT

#if OEM_STRING_INFO
/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 11 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType11(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS          Status;
    TABLE_INFO          TableInfo;
    static UINT8        StringNumber = 0;

    if (isWriteOnce(11, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

    if (Data->Command == 0) {
        if (Data->FieldOffset != 4) return DMI_BAD_PARAMETER;
        if (Set == FALSE) return DMI_SUCCESS;

        StringNumber = (UINT8) Data->ChangeValue;
#if DMIEDIT_DEBUG_TRACE
        DEBUG((DEBUG_INFO, "String number = %d\n", StringNumber));
#endif
        return DMI_SUCCESS;
    }

    if (Data->Command != 5) return DMI_BAD_PARAMETER;
    if (Data->FieldOffset != 4) return DMI_BAD_PARAMETER;
    if (!StringNumber)  return DMI_BAD_PARAMETER;
    if (Set == FALSE) return DMI_SUCCESS;

    TableInfo.Offset = StringNumber;

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2

    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);

#else
{
    Status = PrepareDmiArrayDmiVar(11, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (Status) {
        return DMI_NO_CHANGE;
    }
    return DynamicUpdateType11(Handle, TableInfo, Data->StructureData);
}
#endif                                          // OEM_STRING_INFO

#if SYSTEM_CONFIG_OPTION_INFO
/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 12 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType12(
    UINT16                      Handle,
    SET_SMBIOS_STRUCTURE_DATA   *Data,
    BOOLEAN                     Set
)
{
    EFI_STATUS  	Status;
    TABLE_INFO  	TableInfo;
    static UINT8	StringNumber = 0;

    if (Data->Command == 0) {
        if (Data->FieldOffset != 4) return DMI_BAD_PARAMETER;
        if (Set == FALSE) return DMI_SUCCESS;

        StringNumber = (UINT8) Data->ChangeValue;
        return DMI_SUCCESS;
    }

    if (Data->Command != 5) return DMI_BAD_PARAMETER;
    if (Data->FieldOffset != 4) return DMI_BAD_PARAMETER;
    if (!StringNumber)  return DMI_BAD_PARAMETER;
    if (Set == FALSE) return DMI_SUCCESS;

    if (isWriteOnce(12, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

    TableInfo.Offset = StringNumber;

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);
    if (Status != 0) {
        return DMI_NO_CHANGE;
    }
#else
{
    Status = PrepareDmiArrayDmiVar(12, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (Status) {
        return DMI_NO_CHANGE;
    }
    return DynamicUpdateType12(Handle, TableInfo, Data->StructureData);
}
#endif                                          // SYSTEM_CONFIG_OPTION_INFO

#if PORTABLE_BATTERY_INFO
/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 22 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType22(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS          Status;
    TABLE_INFO          TableInfo;
    UINT8               *StructPtr;

    switch (Data->FieldOffset) {
        case 0x09:
        case 0x0f:
        case 0x15:  if (Data->Command != 0) return DMI_BAD_PARAMETER;
                    break;
        case 0x0a:
        case 0x0c:
        case 0x10:
        case 0x12:  if (Data->Command != 1) return DMI_BAD_PARAMETER;
                    break;
        case 0x16:  if (Data->Command != 2) return DMI_BAD_PARAMETER;
                    break;
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
        case 0x0e:
        case 0x14:  if (Data->Command != 5) return DMI_BAD_PARAMETER;
                    break;
        default:    return DMI_BAD_PARAMETER;
    }

    if (Set == FALSE) return DMI_SUCCESS;

    if (isWriteOnce(22, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

    if (Data->Command == 0 || Data->Command == 0x1 || Data->Command == 0x2)
        *(UINT32*)Data->StructureData = Data->ChangeValue;

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2

    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);

#else
    Status = PrepareDmiArrayDmiVar(22, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (Status) {
        return DMI_NO_CHANGE;
    }

    // Dynamically update the structure in the Smbios table
    StructPtr = GetStructureByHandle(&Handle);
    if (StructPtr != NULL) {
        switch (Data->FieldOffset) {
            case 0x09:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DeviceChemistry &= (UINT8)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DeviceChemistry |= (UINT8)Data->ChangeValue;
                        break;
            case 0x0a:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DesignCapacity &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DesignCapacity |= (UINT16)Data->ChangeValue;
                        break;
            case 0x0c:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DesignVoltage &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DesignVoltage |= (UINT16)Data->ChangeValue;
                        break;
            case 0x0f:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->MaxErrorInBatteryData &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->MaxErrorInBatteryData |= (UINT16)Data->ChangeValue;
                        break;
            case 0x10:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->SBDSSerialNumber &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->SBDSSerialNumber |= (UINT16)Data->ChangeValue;
                        break;
            case 0x12:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->SBDSManufacturerDate &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->SBDSManufacturerDate |= (UINT16)Data->ChangeValue;
                        break;
            case 0x15:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DesignCapabilityMult &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->DesignCapabilityMult |= (UINT16)Data->ChangeValue;
                        break;
            case 0x16:  ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->OEMSpecific &= (UINT32)Data->ChangeMask;
                        ((SMBIOS_PORTABLE_BATTERY_INFO*)StructPtr)->OEMSpecific |= (UINT32)Data->ChangeValue;
                        break;
        }
    }

    return UpdateStrings(Handle, TableInfo, Data->StructureData);
}
#endif                                          // PORTABLE_BATTERY_INFO

#if SYSTEM_POWER_SUPPLY_INFO
/**
    Updates Flash Data record with input DMI data
    Updates SMBIOS Type 39 Structure in Runtime with DMI data

    @param Handle
    @param Data
    @param Set

    @return UINT8 Status

**/
UINT16
SetType39(
    IN UINT16                      Handle,
    IN SET_SMBIOS_STRUCTURE_DATA   *Data,
    IN BOOLEAN                     Set
)
{
    EFI_STATUS          Status;
    TABLE_INFO          TableInfo;
    UINT8               *StructPtr;

    switch (Data->FieldOffset) {
        case 0x04:  if (Data->Command != 0) return DMI_BAD_PARAMETER;
                    break;
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
        case 0x09:
        case 0x0a:
        case 0x0b:  if (Data->Command != 5) return DMI_BAD_PARAMETER;
                    break;
        case 0x0c:
        case 0x0e:
        case 0x10:
        case 0x12:
        case 0x14:  if (Data->Command != 1) return DMI_BAD_PARAMETER;
                    break;
        default:    return DMI_BAD_PARAMETER;
    }

    if (Set == FALSE) return DMI_SUCCESS;

    if (isWriteOnce(39, Data->FieldOffset, Handle)) return DMI_READ_ONLY;

    // Fill TableInfo with input data
    GetInputDataInfo(Handle, Data, &TableInfo);

    if (Data->Command == 0 || Data->Command == 0x1)
        *(UINT32*)Data->StructureData = Data->ChangeValue;

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2

    Status = UpdateSmbiosTable(&TableInfo, Data->StructureData);

#else
{
    Status = PrepareDmiArrayDmiVar(39, Handle, Data->FieldOffset, 0);

    if (EFI_ERROR(Status)) {
        return DMI_NO_CHANGE;
    }

    Status = StoreNvramData(Var, &Data->StructureData, (UINTN)TableInfo.Size, Index);
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

    if (Status) {
        return DMI_NO_CHANGE;
    }

    // Dynamically update the structure in the Smbios table
    StructPtr = GetStructureByHandle(&Handle);
    if (StructPtr != NULL) {
        switch (Data->FieldOffset) {
            case 0x04:  ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->PwrUnitGroup &= (UINT8)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->PwrUnitGroup |= (UINT8)Data->ChangeValue;
                        break;
            case 0x0c:  ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->MaxPwrCapacity &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->MaxPwrCapacity |= (UINT16)Data->ChangeValue;
                        break;
            case 0x0e:  ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->PwrSupplyChar &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->PwrSupplyChar |= (UINT16)Data->ChangeValue;
                        break;
            case 0x10:  ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->InputVoltProbeHandle &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->InputVoltProbeHandle |= (UINT16)Data->ChangeValue;
                        break;
            case 0x12:  ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->CoolingDevHandle &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->CoolingDevHandle |= (UINT16)Data->ChangeValue;
                        break;
            case 0x14:  ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->InputCurrentProbeHandle &= (UINT16)Data->ChangeMask;
                        ((SMBIOS_SYSTEM_PWR_SUPPY_INFO*)StructPtr)->InputCurrentProbeHandle |= (UINT16)Data->ChangeValue;
                        break;
        }
    }

    return UpdateStrings(Handle, TableInfo, Data->StructureData);
}
#endif                                          // SYSTEM_POWER_SUPPLY_INFO

/**
    PnP function 52 Command 03: Add structure

    @param dmiDataBuffer
    @param Set

    @return UINT8 Status

**/
UINT16
PnPFn52AddStructure (
    IN SET_SMBIOS_STRUCTURE_DATA    *dmiDataBuffer,
    IN UINT8						Control
)
{
    UINT16  Status;
    UINT8   *SrcPtr;
    UINT8   *DestPtr;
    UINT8   Type127Buffer[4];
    UINT16  Size;
    TABLE_INFO  TableInfo;

    DestPtr = GetStructureByHandle(&dmiDataBuffer->StructureHeader.Handle);

    if (DestPtr) {
        Status = DMI_INVALID_HANDLE;
    }
    else {
        SrcPtr = SmbiosTable;
        if (FindStructureType(&SrcPtr, &DestPtr, 127, 1, IN_SMBIOS_TABLE)) {
#if (SMBIOS_2X_SUPPORT == 1)
            if ((MaximumBufferSize - SmbiosTableSize) >= dmiDataBuffer->DataLength) {
#else
            Size = GetRemainingStructuresSize(SmbiosTable, IN_SMBIOS_TABLE);
            if (Size == 0 || Size == 0xffff) {
                return DMI_INVALID_ADDRESS;
            }
            if ((MaximumBufferSize - Size) >= dmiDataBuffer->DataLength) {
#endif                                          // SMBIOS_2X_SUPPORT
                if (Control) {
                    TableInfo.Type = dmiDataBuffer->StructureHeader.Type;
                    TableInfo.Offset = dmiDataBuffer->FieldOffset;
                    TableInfo.Reserved = 0;
                    TableInfo.Flags = DMIEDIT_ADD_STRUC | DMIEDIT_EXTENDED_HDR;
                    TableInfo.HdrLength = sizeof(TABLE_INFO);
                    TableInfo.Size = dmiDataBuffer->DataLength;
                    TableInfo.Handle = dmiDataBuffer->StructureHeader.Handle;

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
                    Status = UpdateSmbiosTable(&TableInfo, (UINT8*)&dmiDataBuffer->StructureHeader);
                    if (Status != 0) {
                        return Status;
                    }
#else
{
					EFI_STATUS	Status;

					Status = PrepareDmiArrayDmiVar(TableInfo.Type, TableInfo.Handle, TableInfo.Offset, TableInfo.Flags);

                    if (EFI_ERROR(Status)) {
                        return DMI_NO_CHANGE;
                    }

					// Check if record already exists
                    //
                    // DmiDataSize can be anything since the purpose of this GetVariable
                    // call is to detect if the variable already exists or not. Its
                    // content is not used.
                    DmiDataSize = 0;                        // Dummy value
				    Status = pRS->GetVariable(
				                        Var,
				                        &gAmiSmbiosNvramGuid,
				                        NULL,
				                        &DmiDataSize,
				                        &DmiData);

				    if (Status == EFI_NOT_FOUND) {
						// Record not present, increment record count

				        DmiArray[0].Type += 1;          // Increment # variable counter

				        Status = pRS->SetVariable(
				                            DmiArrayVar,
				                            &gAmiSmbiosNvramGuid,
				                            EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
				                            DmiArraySize,
				                            &DmiArray);
				        if (EFI_ERROR(Status)) {
#if DMIEDIT_DEBUG_TRACE
				            DEBUG((DEBUG_ERROR | DEBUG_WARN, "PnPFn52AddStructure - Failed to store DmiArrayVar\n"));
#endif
                            return DMI_ADD_STRUCTURE_FAILED;
                        }
				    }

					// Update DMI data record if already exists,
					// or store new record if total record count in DmiArray was successfully
					// updated
                    if (Status == EFI_BUFFER_TOO_SMALL || Status == EFI_SUCCESS) {
                        DmiDataSize = (UINTN)dmiDataBuffer->DataLength;
					    Status = pRS->SetVariable(
					                        Var,
					                        &gAmiSmbiosNvramGuid,
					                        EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                            DmiDataSize,
					                        (VOID*)&dmiDataBuffer->StructureHeader);
				        if (EFI_ERROR(Status)) {
#if DMIEDIT_DEBUG_TRACE
				            DEBUG((DEBUG_ERROR | DEBUG_WARN, "PnPFn52AddStructure - Failed to store DMI data record\n"));
#endif
                        }
					}

                    if (Status != 0) {
                        return DMI_NO_CHANGE;
                    }
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

                    // Copy Type 127
                    MemCpy(&Type127Buffer, DestPtr, 4);
                    MemCpy(DestPtr, (UINT8*)&dmiDataBuffer->StructureHeader, dmiDataBuffer->DataLength);
                    Size = GetStructureLength(DestPtr, IN_SMBIOS_TABLE);
                    if (Size == 0 || Size == 0xffff) {
                        return DMI_INVALID_ADDRESS;
                    }
                    DestPtr = DestPtr + Size;
                    MemCpy(DestPtr, &Type127Buffer, 4);

                    // Update SMBIOS Structure Table Entry Point - Structure Table Length, Intermediate checksum
                    UpdateSmbiosTableHeader();
                }

                Status = DMI_SUCCESS;
            }
            else {
                Status = DMI_ADD_STRUCTURE_FAILED;
            }
        }
        else {
            Status = DMI_ADD_STRUCTURE_FAILED;
        }
    }

    return Status;
}

/**
    PnP function 52 Command 04: Delete structure

    @param dmiDataBuffer
    @param Set

    @return Status

**/
UINT16
PnPFn52DeleteStructure (
	IN SET_SMBIOS_STRUCTURE_DATA    *dmiDataBuffer,
	IN UINT8						Control
)
{
    UINT16  DmiStatus;
    UINT8   *DestPtr;
    UINT16  i;
    UINT16  Size;
    UINT16  RemainingSize;
    TABLE_INFO  TableInfo;

    DestPtr = GetStructureByHandle(&((SET_SMBIOS_STRUCTURE_DATA*)dmiDataBuffer)->StructureHeader.Handle);
    if (DestPtr) {
        if (Control) {
            UINT8   *SrcPtr;

            TableInfo.Type = dmiDataBuffer->StructureHeader.Type;
            TableInfo.Offset = 0xff;
            TableInfo.Reserved = 0;
            TableInfo.Flags = DMIEDIT_DELETE_STRUC | DMIEDIT_EXTENDED_HDR;
            TableInfo.HdrLength = sizeof(TABLE_INFO);
            TableInfo.Size = 0;
            TableInfo.Handle = dmiDataBuffer->StructureHeader.Handle;

#if !defined(SMBIOS_DMIEDIT_DATA_LOC) || SMBIOS_DMIEDIT_DATA_LOC != 2
            DmiStatus = UpdateSmbiosTable(&TableInfo, (UINT8*)&dmiDataBuffer->StructureHeader);
            if (DmiStatus != 0) {
                return DmiStatus;
            }
#else
{
			EFI_STATUS	Status;

            Status = PrepareDmiArrayDmiVar(TableInfo.Type, TableInfo.Handle, TableInfo.Offset, TableInfo.Flags);

            if (EFI_ERROR(Status)) {
                return DMI_NO_CHANGE;
            }
            
            // Delete the variable
            Status = pRS->SetVariable (Var,
                                       &gAmiSmbiosNvramGuid,
                                       EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                       0,
                                       NULL);

            if (EFI_ERROR(Status)) {
                return DMI_NO_CHANGE;
            }

            // Searching for DMI record in DmiArray
            for (i = 1; i < DmiArray[0].Type; i++) {
                if (DmiArray[i].Type == TableInfo.Type &&
                    DmiArray[i].Handle == TableInfo.Handle &&
                    DmiArray[i].Offset == TableInfo.Offset &&
                    DmiArray[i].Flags == TableInfo.Flags) {
                    break;
                }
            }

        if (i < DmiArray[0].Type) {
            // Delete DMI record from DmiArray
            for (; i < DmiArray[0].Type; i++) {
                if (i == (DmiArray[0].Type - 1)) {
                    // Zero out last entry
                    DmiArray[i].Type = 0;
                    DmiArray[i].Handle = 0;
                    DmiArray[i].Offset = 0;
                    DmiArray[i].Flags = 0;
                }
                else {
                    DmiArray[i].Type = DmiArray[i + 1].Type;
                    DmiArray[i].Handle = DmiArray[i + 1].Handle;
                    DmiArray[i].Offset = DmiArray[i + 1].Offset;
                    DmiArray[i].Flags = DmiArray[i + 1].Flags;
                }
            }

            DmiArray[0].Type -= 1;          // Decrement # variable counter

            Status = pRS->SetVariable(
                                DmiArrayVar,
                                &gAmiSmbiosNvramGuid,
                                EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                DmiArraySize,
                                &DmiArray);
            if (EFI_ERROR(Status)) {
#if DMIEDIT_DEBUG_TRACE
            DEBUG((DEBUG_ERROR | DEBUG_WARN, "PnPFn52DeleteStructure - Failed to store DmiArrayVar\n"));
#endif
            return DMI_NO_CHANGE;
            }

        }
}
#endif                                          // SMBIOS_DMIEDIT_DATA_LOC

			// Copy / update the remaining structures in the Smbios Table
            Size = GetStructureLength(DestPtr, IN_SMBIOS_TABLE);
            if (Size == 0 || Size == 0xffff) {
                return DMI_INVALID_ADDRESS;
            }
            SrcPtr = DestPtr + Size;
            RemainingSize = (UINT16)(SmbiosTableEnd - SrcPtr);

            for (i = 0; i < RemainingSize; i++) {
                *DestPtr = *SrcPtr;
                SrcPtr++;
                DestPtr++;
            }

            // Update SMBIOS Structure Table Entry Point - Structure Table Length, Intermediate checksum
            UpdateSmbiosTableHeader();
        }

        DmiStatus = DMI_SUCCESS;
    }
    else {
        DmiStatus = DMI_INVALID_HANDLE;
    }

    return DmiStatus;
}

/**
    DMIEdit function to update the structures and saves the
    DMI data in the Flash Part for subsequent boot.

    @param Pointer to SMI function parameters

    @return Status

**/
UINT16
SetSmbiosStructure(
    IN SET_SMBIOS_STRUCTURE_DATA	*Data,
    IN UINT8						Control
)
{
    UINT8       *SmbTable;
    UINT16		Handle = Data->StructureHeader.Handle;

    if (Data->Command == 3) {                   // Add structure
        return PnPFn52AddStructure(Data, Control&1);
    }

    if (Data->Command == 4) {                   // Delete structure
        return PnPFn52DeleteStructure(Data, Control&1);
    }

    SmbTable = GetStructureByHandle(&Handle);
    if (!SmbTable) return DMI_INVALID_HANDLE;

    // Verify header
    if (*(UINT16*)&Data->StructureHeader != *(UINT16*)SmbTable) return DMI_BAD_PARAMETER;

    // Currently only accept certain table types;
    switch (Data->StructureHeader.Type) {
        case 0:
                return SetType0(Handle, Data, Control&1);
        case 1:
                return SetType1(Handle, Data, Control&1);
#if BASE_BOARD_INFO
        case 2:
                return SetType2(Handle, Data, Control&1);
#endif                                          // BASE_BOARD_INFO
#if SYS_CHASSIS_INFO
        case 3:
                return SetType3(Handle, Data, Control&1);
#endif                                          // SYS_CHASSIS_INFO
#if PROCESSOR_DMIEDIT_SUPPORT
        case 4:
                return SetType4(Handle, Data, Control&1);
#endif                                          // PROCESSOR_DMIEDIT_SUPPORT
#if OEM_STRING_INFO
        case 11:
                return SetType11(Handle, Data, Control&1);
#endif                                          // OEM_STRING_INFO
#if SYSTEM_CONFIG_OPTION_INFO
        case 12:
                return SetType12(Handle, Data, Control&1);
#endif                                          // SYSTEM_CONFIG_OPTION_INFO
#if PORTABLE_BATTERY_INFO
        case 22:
                return SetType22(Handle, Data, Control&1);
#endif                                          // PORTABLE_BATTERY_INFO
#if SYSTEM_POWER_SUPPLY_INFO
        case 39:
                return SetType39(Handle, Data, Control&1);
#endif                                          // SYSTEM_POWER_SUPPLY_INFO
    }
    return DMI_BAD_PARAMETER;
}

#if (SMBIOS_2X_SUPPORT == 1)
/**
    DMIEdit function to update Smbios 2.x structures and saves the
    DMI data in the Flash Part for subsequent boot.

    @param Pointer to SMI function parameters

    @return Status

**/
UINT16
SetSmbiosV2Structure(
    IN SET_SMBIOS_STRUCTURE    *p
)
{
    SET_SMBIOS_STRUCTURE_DATA	*dmiDataBuffer;
    UINT8						Control;

    if (SmbiosTable == NULL) return DMI_FUNCTION_NOT_SUPPORTED;

#if (((!defined(AFRI_PASS_TO_AFRI_PROTOCOL) && SMM_SUPPORT) || (AFRI_PASS_TO_SMM_COMMUNICATION && SMM_SUPPORT)) && !defined(_DMIEDIT_PROTOCOL_))
{
    UINTN                       BufferSize;

    if (AmiValidateMemoryBuffer((VOID*)p, sizeof(SET_SMBIOS_STRUCTURE)) == EFI_SUCCESS) {
        // First check if fixed format portion of dmiDataBuffer is valid
        BufferSize = 0x10;
        if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->Buffer32BitAddr, BufferSize) == EFI_SUCCESS) {
            if (((SET_SMBIOS_STRUCTURE_DATA*)(UINTN)p->Buffer32BitAddr)->Command > 6) {
                return DMI_BAD_PARAMETER;
            }

            // Adjust buffersize based on "Command" type
            if (((SET_SMBIOS_STRUCTURE_DATA*)(UINTN)p->Buffer32BitAddr)->Command == 3 ||
                ((SET_SMBIOS_STRUCTURE_DATA*)(UINTN)p->Buffer32BitAddr)->Command > 4) {
                BufferSize += ((SET_SMBIOS_STRUCTURE_DATA*)(UINTN)p->Buffer32BitAddr)->DataLength;
            }

            if (AmiValidateMemoryBuffer((VOID*)(UINTN)p->Buffer32BitAddr, BufferSize) == EFI_SUCCESS) {
                dmiDataBuffer = (SET_SMBIOS_STRUCTURE_DATA *)(UINTN)p->Buffer32BitAddr;
                Control = p->Control;

                return SetSmbiosStructure(dmiDataBuffer, Control);
            }
            else {
                return DMI_BAD_PARAMETER;
            }
        }
        else {
            return DMI_BAD_PARAMETER;
        }
    }
    else {
        return DMI_BAD_PARAMETER;
    }
}
#else
    dmiDataBuffer = (SET_SMBIOS_STRUCTURE_DATA *)(UINTN)p->Buffer32BitAddr;
    Control = p->Control;

    return SetSmbiosStructure(dmiDataBuffer, Control);
#endif
}
#endif                                          // SMBIOS_2X_SUPPORT

#if (SMBIOS_3X_SUPPORT == 1)
/**
    DMIEdit function to update Smbios 3.x structures and saves the
    DMI data in the Flash Part for subsequent boot.

    @param Pointer to SMI function parameters

    @return Status

**/
UINT16
SetSmbiosV3Structure(
    IN SET_SMBIOS_V3_STRUCTURE	*p
)
{
    SET_SMBIOS_STRUCTURE_DATA	*dmiDataBuffer;
    UINT8						Control;

    if (SmbiosTable == NULL) return DMI_FUNCTION_NOT_SUPPORTED;

#if (((!defined(AFRI_PASS_TO_AFRI_PROTOCOL) && SMM_SUPPORT) || (AFRI_PASS_TO_SMM_COMMUNICATION && SMM_SUPPORT)) && !defined(_DMIEDIT_PROTOCOL_))
{
    UINTN                       BufferSize;

    if (AmiValidateMemoryBuffer((VOID*)p, sizeof(SET_SMBIOS_V3_STRUCTURE)) == EFI_SUCCESS) {
        // First check if fixed format portion of dmiDataBuffer is valid
        BufferSize = 0x10;
        if (AmiValidateMemoryBuffer((VOID*)p->Buffer64BitAddr, BufferSize) == EFI_SUCCESS) {
            if (((SET_SMBIOS_STRUCTURE_DATA*)p->Buffer64BitAddr)->Command > 6) {
                return DMI_BAD_PARAMETER;
            }

            // Adjust buffersize based on "Command" type
            if (((SET_SMBIOS_STRUCTURE_DATA*)p->Buffer64BitAddr)->Command == 3 ||
                ((SET_SMBIOS_STRUCTURE_DATA*)p->Buffer64BitAddr)->Command > 4) {
                BufferSize += ((SET_SMBIOS_STRUCTURE_DATA*)p->Buffer64BitAddr)->DataLength;
            }

            if (AmiValidateMemoryBuffer((VOID*)p->Buffer64BitAddr, BufferSize) == EFI_SUCCESS) {
                dmiDataBuffer = (SET_SMBIOS_STRUCTURE_DATA *)(UINTN)p->Buffer64BitAddr;
                Control = p->Control;

                return SetSmbiosStructure(dmiDataBuffer, Control);
            }
            else {
                return DMI_BAD_PARAMETER;
            }
        }
        else {
            return DMI_BAD_PARAMETER;
        }
    }
    else {
        return DMI_BAD_PARAMETER;
    }
}
#else
    dmiDataBuffer = (SET_SMBIOS_STRUCTURE_DATA *)(UINTN)p->Buffer64BitAddr;
    Control = p->Control;

    return SetSmbiosStructure(dmiDataBuffer, Control);
#endif
}
#endif                                          // SMBIOS_3X_SUPPORT

#if (SMBIOS_3X_SUPPORT == 1)
/**
    Fills input buffer with contect of SMBIOS 3.x Table

    @param Pointer to SMI function parameters

    @return UINT16 Status (0 for success)

**/

UINT16
GetSmbiosV3Table(
    IN OUT  GET_SMBIOS_V3_TABLE   *p
)
{
	UINT8		*BufferPtr;

	if (!SmbiosV3TableEntryPoint) return DMI_FUNCTION_NOT_SUPPORTED;

    if (p->SmbiosTableBufferSize == 0) return DMI_BUFFER_TOO_SMALL;

	if (p->SmbiosTableBufferAddr == 0) return DMI_BUFFER_NOT_ALLOCATED;

    BufferPtr = p->SmbiosTableBufferAddr;
    MemCpy(BufferPtr, (UINT8*)SmbiosV3TableEntryPoint->TableAddress, p->SmbiosTableBufferSize);

    return 0;
}

#endif                                          // SMBIOS_3X_SUPPORT
