//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/** @file
  Code to log processor subclass data with Smbios protocol.

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/

#include <Token.h>
#include <Protocol/SmBios.h>
#include <IndustryStandard/Smbios.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
//MpService needed for CpuConfigLib.h
#include <Protocol/MpService.h>
#include <Library/CpuConfigLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include "MpCommon.h"

#define BDX_DE_SKU 0x5066
#define BDX_EP_SKU 0x406F

EFI_SMBIOS_PROTOCOL *gSmbiosProtocol;

//Array of processor number indexed by socket.
//Contains info needed for each socket before creating SMBIOS tables.
typedef struct {
    UINT32 ProcNum;
    UINT32 PkgNum;
    UINT32 EnabledCoreCount;
} SMBIOS_CPU_INFO;

SMBIOS_CPU_INFO *gSmbiosCpuInfo;      

#define SMBIOS_CACHE_ECC_PARITY 4
#define SMBIOS_CACHE_ECC_SINGLE_BIT 5

#define EFI_CPUID_VERSION_INFO                0x1
#define EFI_CPUID_BRAND_STRING1               0x80000002
#define EFI_CPUID_BRAND_STRING2               0x80000003
#define EFI_CPUID_BRAND_STRING3               0x80000004

#define ___INTERNAL_CONVERT_TO_STRING___(a) #a
#define CONVERT_TO_STRING(a) ___INTERNAL_CONVERT_TO_STRING___(a)

#define SMBIOS_MAX_NUM_SOCKETS 8
CHAR8 *gSocketDesgination[8] = {
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_0),
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_1),
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_2),
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_3),
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_4),
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_5),
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_6),
    CONVERT_TO_STRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_7)
};


/**
  Returns the processor voltage of the processor installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Voltage in mV

**/
UINT16
GetProcessorVoltage (
  IN UINTN  ProcessorNumber
)
{
    UINT16             VoltageInmV;
    EFI_CPUID_REGISTER *Reg;

    Reg = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_VERSION_INFO);
    ASSERT (Reg != NULL);
    
    if ((Reg->RegEax >> 8 & 0x3F) == 0xF) {
        VoltageInmV = 3000;
    } else if((Reg->RegEax >> 4) == BDX_DE_SKU){ //Broadwell DE
        VoltageInmV = 1800;
    } else if((Reg->RegEax >> 4) == BDX_EP_SKU){ //Broadwell EP
        VoltageInmV = 1800;
    } else {
        VoltageInmV = 1600;
    }

    return VoltageInmV;
}


//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  AutodetectFamily
//
// Description: Return family type from Brand String.
//
// Input:  IN CHAR8 *BrandString
//
// Output: UINT8 - Family
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8 AutodetectFamily(IN CHAR8 *BrandString)
{
    CHAR8 *p = BrandString;

    while(*p) {
        if (CompareMem(p, "Xeon", 4) == 0) return 0xb3;
        if (CompareMem(p, "i7", 2) == 0) return 0xc6;
        if (CompareMem(p, "i5", 2) == 0) return 0xcd;
        if (CompareMem(p, "i3", 2) == 0) return 0xce;
        ++p;
    }

    return 0;
}

//Array of associativity. Element contains Smbios table value.
UINT8 gAssociativityTable[] = {2, 6 ,4 ,1 ,5 ,1 ,1 ,1 ,7 ,1 ,1 ,1 ,9 ,1 ,1, 1, 8, 1, 1, 1, 0xe,
    1, 1, 1, 0xa, 1, 1, 1, 1, 1, 1, 1, 0xb
};

typedef struct {
    UINT32 Size;
    UINT8 Type;
    UINT8 Assoc;
} CACHE_INFO;

CACHE_INFO gCacheInfo[3]; //[0] = L1, [1] = L2, [2] = L3

typedef struct {
    UINT32 CacheType:5;
    UINT32 CacheLevel:3;
    UINT32 NotUsed:6;
    UINT32 SharedThreads:12;
    UINT32 NumSupportedCores:6;
} CPUID4_EAX_CACHE_INFO;

typedef struct {
    UINT32  LineSize:12;
    UINT32  Partitions:10;
    UINT32  Ways:10;
} CPUID4_EBX_CACHE_INFO;


#define SMBIOS_BUFFER_SIZE 200

typedef struct {
    //Private Data
    UINT8   *SmbiosTable;
    UINT32  FixedStructureSize;
    UINT8   *StrBuf;
    INT32   StrBufSize;
    INT32   StrBufAvail;
    UINT8   Tok;
} SMBIOS_TABLE_STR_BUFFER;


//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  CreateSmbiosTable
//
// Description: Allocate memory for SMBIOS table, string buffer, and private structure.
//  Initialize double NULL at end of fixed table.
//
// Input:
//  IN UINT32 FixedStructureSize - size of data not including stings or NUL.
//  IN UINT32 StringBufferSize - Size of buffer for string.
//  OUT VOID * Handle - Instance of SmbiosTable.
//
// Output: VOID * - Smbios table created in memory
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID * CreateSmbiosTable(IN UINT32 FixedStructureSize, IN UINT32 StringBufferSize, OUT VOID **Handle)
{
    UINT8 *SmbiosTable;
    SMBIOS_TABLE_STR_BUFFER *StrBuffer;

    //Allocate memory for fixed size, string buffer, and double NULL.
    SmbiosTable = AllocatePool(FixedStructureSize + StringBufferSize + 2);
    StrBuffer = (SMBIOS_TABLE_STR_BUFFER*) AllocatePool(sizeof(SMBIOS_TABLE_STR_BUFFER));

    //Initialize private structure.
    StrBuffer->SmbiosTable = SmbiosTable;
    StrBuffer->FixedStructureSize = FixedStructureSize;
    StrBuffer->StrBuf = SmbiosTable + FixedStructureSize;
    StrBuffer->StrBufSize = StringBufferSize;
    StrBuffer->StrBufAvail = StringBufferSize;
    StrBuffer->Tok = 0;
    
    //Add double null, required at end of structure with no strings.
    *(UINT16*)(SmbiosTable + FixedStructureSize) = 0;

    *Handle = (VOID*)StrBuffer;
    return (VOID*)SmbiosTable;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  AddStrGetTok
//
// Description: Add string to SMBIOS table and return token number.
//
// Input:
//  IN VOID *Handle - Instance of SMBIOS table.
//  IN CHAR8 *Str - SMBIOS String to add.
//
// Output: UINT8 String token returned.
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8 AddStrGetTok(
    IN VOID *Handle,
    IN CHAR8 *Str
)
{
    SMBIOS_TABLE_STR_BUFFER *StrBuffer = (SMBIOS_TABLE_STR_BUFFER*)Handle;
    UINT8  *SmbiosTable = StrBuffer->SmbiosTable;
    UINT32 FixedSructureSize = StrBuffer->FixedStructureSize;
    INT32 Len = (INT32)AsciiStrLen(Str);
    INT32 BufferUsed;

    if (Len == 0) return 0;                 //No string

    //For string buffer, 2 bytes at end reserved for double 0, so that is why
    //gStrBufAvail - 2
    if (Len > (StrBuffer->StrBufAvail - 2)) return 0; //Not enough space left.

    AsciiStrCpy(StrBuffer->StrBuf, Str);
    StrBuffer->StrBuf += Len + 1;
    StrBuffer->StrBufAvail -= Len + 1;
    ++StrBuffer->Tok;

    BufferUsed = StrBuffer->StrBufSize - StrBuffer->StrBufAvail;
    *((UINT8*)SmbiosTable + FixedSructureSize + BufferUsed) = 0; //End structure of NULL.
  
    return StrBuffer->Tok;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  FreeSmbiosTable
//
// Description: Free memory created with CreateSmbiosTable.
//
// Input:
//  IN VOID *Handle - Instance of SMBIOS table.
//
// Output: VOID
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID FreeSmbiosTable(IN VOID *Handle)
{
    FreePool(((SMBIOS_TABLE_STR_BUFFER*)Handle)->SmbiosTable);
    FreePool(Handle);
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:   GetMaxSpeedFromBrandString
//
// Description: Get the max speed from the brand string.
//
// Input:
//  IN CHAR8    *CpuBrandString - Pointer to CPU brand string.
//
// Output:  UINT32 - frequency found in MHz.
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32 GetMaxSpeedFromBrandString(IN CHAR8 *CpuBrandString)
{
    UINT32  i;
    UINT8   CharCount;
    BOOLEAN TransToMHz = FALSE;
    BOOLEAN FreqStringFound = FALSE;
    CHAR8   FrequencyString[] = "0000";

    while (*CpuBrandString != 0) {
        if (*CpuBrandString == 'G' && *(CpuBrandString + 1) == 'H' && *(CpuBrandString + 2) == 'z') {
            FreqStringFound = TRUE;
            TransToMHz = TRUE;
            break;
        } else if (*CpuBrandString == 'M' && *(CpuBrandString+1) == 'H' && *(CpuBrandString + 2) == 'z') {
            FreqStringFound = TRUE;
            break;
        } else ++CpuBrandString;
    }

    --CpuBrandString;   //first numeric char

    //search numeric char
    CharCount = 0;
    for(i = 0 ; i < 4; ++i) {
        if (*CpuBrandString >= '0' && *CpuBrandString <= '9') {
            --CpuBrandString;
            ++CharCount;
        } else if (*CpuBrandString == '.') {
            --CpuBrandString;
            ++CharCount;
        } else break;
    }

    ++CpuBrandString;   //first numeric char

    if (FreqStringFound && CharCount > 0) {
        for(i = 0; i < CharCount; ++i) {
            if (TransToMHz && *CpuBrandString == '.') CpuBrandString++;

            FrequencyString[i] = *CpuBrandString;
            ++CpuBrandString;
        }
        if (TransToMHz) FrequencyString[3] = '0';
    } else FreqStringFound = FALSE;

    return FreqStringFound ? (UINT32)AsciiStrDecimalToUint64(FrequencyString) : 0;
}

CHAR8 *GetBrandString(UINT32 ProcessorNumber)
{
    EFI_CPUID_REGISTER  *CpuidRegister;
    CHAR8 *BrandString = AllocatePool(49);
    CHAR8 *Pos = BrandString;

    //Get Brand string
    CpuidRegister = GetProcessorCpuid(ProcessorNumber, EFI_CPUID_BRAND_STRING1);
    *(UINT32*)Pos = CpuidRegister->RegEax; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEbx; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEcx; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEdx; Pos +=4;

    CpuidRegister = GetProcessorCpuid(ProcessorNumber, EFI_CPUID_BRAND_STRING2);
    *(UINT32*)Pos = CpuidRegister->RegEax; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEbx; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEcx; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEdx; Pos +=4;

    CpuidRegister = GetProcessorCpuid(ProcessorNumber, EFI_CPUID_BRAND_STRING3);
    *(UINT32*)Pos = CpuidRegister->RegEax; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEbx; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEcx; Pos +=4;
    *(UINT32*)Pos = CpuidRegister->RegEdx; Pos +=4;
    *Pos = '\0';
    
    //Remove Initial spaces.
    Pos = BrandString;
    while (*Pos == ' ' && *Pos !=0) ++Pos;
    if (Pos != BrandString) {
        INT32  NumToCopy = 49 - (UINT32)(Pos - BrandString);
        CHAR8 *Pos2 = BrandString;
        while (NumToCopy--) *Pos2++ = *Pos++;
    }

    return BrandString;
}
//Grangeville AptioV override start - Implement according to SMBIOS 3.0
VOID GetSupportedCoreThreadCounts(UINT32 ProcessorNumber, UINT16 *CoreCount, UINT16 *ThreadCount)
{
    EFI_CPUID_REGISTER      *CpuidRegister;
    UINT16 CoreThreadCount;
    UINT16 TotalThreadCount;
    
    
    CpuidRegister = GetExtendedTopologyEnumerationCpuidLeafs (ProcessorNumber, 1);

    TotalThreadCount = (UINT16) CpuidRegister->RegEbx;
    if (ThreadCount) *ThreadCount = TotalThreadCount;
    
    
    CpuidRegister = GetExtendedTopologyEnumerationCpuidLeafs (ProcessorNumber, 0);
    CoreThreadCount = (UINT16)CpuidRegister->RegEbx;
    if (CoreCount) *CoreCount = (UINT16) (TotalThreadCount / CoreThreadCount);
}
//Grangeville AptioV override end - Implement according to SMBIOS 3.0

VOID GetCacheInfo(UINT32 ProcessorNumber)
{
    UINT8 Index;
    UINT8 NumberOfCacheParameters;

    gCacheInfo[0].Size = 0; //Initialize to 0. If cache level not exist, then size = 0.
    gCacheInfo[1].Size = 0;
    gCacheInfo[2].Size = 0;
    
    NumberOfCacheParameters = (UINT8) GetNumberOfCpuidLeafs (ProcessorNumber, DeterministicCacheParametersCpuidLeafs);

    for (Index = 0; Index < NumberOfCacheParameters; ++Index) {
        EFI_CPUID_REGISTER *CpuidRegisters;
        CPUID4_EAX_CACHE_INFO EaxInfo;
        CPUID4_EBX_CACHE_INFO EbxInfo;
        UINT8 CacheLevel;   //0-based
        UINT8 SharedThreads;
        UINT32 CacheSize;
        
        CpuidRegisters = GetDeterministicCacheParametersCpuidLeaf (ProcessorNumber, Index);
        *(UINT32*)&EaxInfo = CpuidRegisters->RegEax;
        *(UINT32*)&EbxInfo = CpuidRegisters->RegEbx;
        
        SharedThreads = EaxInfo.SharedThreads + 1;
        
        if (EaxInfo.CacheLevel == 0) continue;  //SMBIOS doesn't store L0 cache information.
        CacheLevel = EaxInfo.CacheLevel - 1;

        switch (EaxInfo.CacheType) {
        case 1: gCacheInfo[CacheLevel].Type = 4; break; //Data
        case 2: gCacheInfo[CacheLevel].Type = 3; break; //Instruction
        case 3: gCacheInfo[CacheLevel].Type = 5; break; //Unified
        default: gCacheInfo[CacheLevel].Type = 2; break; //Unknown
        }

        //CPUID 4 ECX + 1 = number of sets
        CacheSize =
            (EbxInfo.Ways + 1) * (EbxInfo.Partitions + 1) * (EbxInfo.LineSize + 1) * (CpuidRegisters->RegEcx + 1) /
             1024;
        
        //If cache for each individual core, report total for all cores.
        if (SharedThreads <= 2) {
//Grangeville AptioV override - Implement according to SMBIOS 3.0
            UINT16 SupportedCores;
            GetSupportedCoreThreadCounts(ProcessorNumber, &SupportedCores, NULL);
            CacheSize *= SupportedCores;
        }

        gCacheInfo[CacheLevel].Size += CacheSize;
        gCacheInfo[CacheLevel].Assoc = gAssociativityTable[EbxInfo.Ways + 1];
    }
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  CreateSmbiosTable7
//
// Description: Create SMBIOS Table 7
//
// Input:  IN UINT8 CacheLevel (L1, L2, L3)
//
// Output: UINT16 - Handle for table.
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT16 CreateSmbiosTable7(IN UINT8 CacheLevel)
{
    EFI_SMBIOS_HANDLE Handle = 0xfffe;
    SMBIOS_TABLE_TYPE7 *CacheInfo;
    EFI_STATUS  Status;
    CACHE_SRAM_TYPE_DATA SramTypeData;
    VOID *SmbiosLibHandle;
    
    if (gCacheInfo[CacheLevel-1].Size == 0) return 0xffff;

    CacheInfo = CreateSmbiosTable(sizeof(SMBIOS_TABLE_TYPE7), SMBIOS_BUFFER_SIZE, &SmbiosLibHandle);
    ASSERT(CacheInfo != NULL);

    CacheInfo->Hdr.Type = 7;
    CacheInfo->Hdr.Length = 0x13;
    CacheInfo->Hdr.Handle = Handle;
    CacheInfo->SocketDesignation = 0;

    CacheInfo->MaximumCacheSize = gCacheInfo[CacheLevel-1].Size;
    if ((CacheInfo->MaximumCacheSize & 0x8000) != 0) {
        //If bit 15 = 0, value in 1k. If bit 15 = 1, value in 64k.
        CacheInfo->MaximumCacheSize = (CacheInfo->MaximumCacheSize / 64) | 0x8000;
    }
   
    CacheInfo->InstalledSize = CacheInfo->MaximumCacheSize;
    if (CacheLevel == 1) CacheInfo->SystemCacheType= 1; //Mixed cache with instruction and data.
    else CacheInfo->SystemCacheType = gCacheInfo[CacheLevel-1].Type;
    CacheInfo->Associativity  = gCacheInfo[CacheLevel-1].Assoc;

    switch (CacheLevel) {
    case 1:
        CacheInfo->CacheConfiguration = 0x180;
#ifdef SMBIOS_TYPE_7_L1_SOCKET_DESIGNATION    
        CacheInfo->SocketDesignation = AddStrGetTok(SmbiosLibHandle, CONVERT_TO_STRING(SMBIOS_TYPE_7_L1_SOCKET_DESIGNATION));
#endif         
        break;
    case 2:
        CacheInfo->CacheConfiguration = 0x181;
#ifdef SMBIOS_TYPE_7_L2_SOCKET_DESIGNATION    
        CacheInfo->SocketDesignation = AddStrGetTok(SmbiosLibHandle, CONVERT_TO_STRING(SMBIOS_TYPE_7_L2_SOCKET_DESIGNATION));
#endif                 
        break;
    case 3:
        CacheInfo->CacheConfiguration = 0x182;
#ifdef SMBIOS_TYPE_7_L3_SOCKET_DESIGNATION    
        CacheInfo->SocketDesignation = AddStrGetTok(SmbiosLibHandle, CONVERT_TO_STRING(SMBIOS_TYPE_7_L3_SOCKET_DESIGNATION));
#endif                         
        break;
    }

    *(UINT16*)&SramTypeData = 0x20;
    CacheInfo->SupportedSRAMType = SramTypeData;
    CacheInfo->CurrentSRAMType = SramTypeData;
    CacheInfo->CacheSpeed = 0;

    if (CacheLevel == 1) {
        CacheInfo->ErrorCorrectionType = SMBIOS_CACHE_ECC_PARITY;
    } else {
        CacheInfo->ErrorCorrectionType = SMBIOS_CACHE_ECC_SINGLE_BIT ;
    }
    
    Status = gSmbiosProtocol->Add(
        gSmbiosProtocol,
        NULL,
        &Handle,
        (VOID*)CacheInfo
    );
    ASSERT_EFI_ERROR(Status);

    FreeSmbiosTable(SmbiosLibHandle);

    return !EFI_ERROR(Status) ? Handle : 0xffff;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  CreateSmbiosTable4
//
// Description: Create SMBIOS Table 4
//
// Input:  IN UINT32 PhysSocket
//
// Output: VOID
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID CreateSmbiosTable4(IN UINT32 SocketNumber, IN UINT32 PhysSocket)
{
    EFI_SMBIOS_HANDLE       Handle = 0xfffe;
    SMBIOS_TABLE_TYPE4      *ProcInfo;
    EFI_STATUS              Status;
    UINT32                  MaxFreqBrandStr;
    EFI_CPUID_REGISTER      *CpuidRegister;
    UINT8                   ThreadPerCore;
    UINT32                  ProcessorNumber = gSmbiosCpuInfo[SocketNumber].ProcNum;
    VOID                    *SmbiosLibHandle;
    UINT16                  ProcessorVoltage;
//Grangeville AptioV override - Implement according to SMBIOS 3.0
    UINT16      CoreCount, ThreadCount, EnabledCoreCount;
    CHAR8 *BrandString;

    BrandString = GetBrandString(ProcessorNumber);
    
    ProcInfo = CreateSmbiosTable(sizeof(SMBIOS_TABLE_TYPE4), SMBIOS_BUFFER_SIZE, &SmbiosLibHandle);
    ASSERT(ProcInfo != NULL);
    
    ProcInfo->Hdr.Type = 4;
//Grangeville AptioV override - Implement according to SMBIOS 3.0
    ProcInfo->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE4);
    ProcInfo->Hdr.Handle = 0xffff;    //To be updated by SMBIOS driver.

    if (PhysSocket >= SMBIOS_MAX_NUM_SOCKETS) ProcInfo->Socket = 0; //If socket not in table, don't provide string.
    else ProcInfo->Socket = AddStrGetTok(SmbiosLibHandle, gSocketDesgination[PhysSocket]);

    ProcInfo->ProcessorType = 3;    // Central processor
//Grangeville AptioV override start - Implement ProcessorFamily2 in Type 4 under SMBIOS 2.6
    ProcInfo->ProcessorFamily = 2;  // Unknown
    ProcInfo->ProcessorFamily2 = 2; // Unknown

    if (SMBIOS_TYPE_4_PROC_FAMILY == 0) {
        if((AutodetectFamily(BrandString) >= 0x0) && (AutodetectFamily(BrandString) <= 0xFD)) {
            ProcInfo->ProcessorFamily = AutodetectFamily(BrandString);
            ProcInfo->ProcessorFamily2 = ProcInfo->ProcessorFamily;
        } else if ((AutodetectFamily(BrandString) >= 0x100) && (AutodetectFamily(BrandString) <= 0xFFFD)) {
            ProcInfo->ProcessorFamily = 0xFE;
            ProcInfo->ProcessorFamily2 = AutodetectFamily(BrandString);
        }
    } else if ((SMBIOS_TYPE_4_PROC_FAMILY > 0x0) && (SMBIOS_TYPE_4_PROC_FAMILY <= 0xFD)) {
        ProcInfo->ProcessorFamily = SMBIOS_TYPE_4_PROC_FAMILY;
        ProcInfo->ProcessorFamily2 = SMBIOS_TYPE_4_PROC_FAMILY;
    } else if ((SMBIOS_TYPE_4_PROC_FAMILY >= 0x100) && (SMBIOS_TYPE_4_PROC_FAMILY <= 0xFFFD)) {
        ProcInfo->ProcessorFamily = 0xFE;
        ProcInfo->ProcessorFamily2 = SMBIOS_TYPE_4_PROC_FAMILY;
    } 
//Grangeville AptioV override end - Implement ProceesorFamily2 in Type 4 under SMBIOS 2.6
    ProcInfo->ProcessorManufacture = AddStrGetTok(SmbiosLibHandle, "Intel");

    CpuidRegister = GetProcessorCpuid(ProcessorNumber, EFI_CPUID_VERSION_INFO);
    ASSERT (CpuidRegister != NULL);
    *(UINT32*)&ProcInfo->ProcessorId.Signature = CpuidRegister->RegEax;
    *(UINT32*)&ProcInfo->ProcessorId.FeatureFlags = CpuidRegister->RegEdx;

    if (*BrandString == '\0') ProcInfo->ProcessorVersion = AddStrGetTok(SmbiosLibHandle, "Intel(R) Genuine processor");
    else ProcInfo->ProcessorVersion = AddStrGetTok(SmbiosLibHandle, BrandString);
    
    ProcessorVoltage = GetProcessorVoltage (ProcessorNumber); // mV unit
    ProcessorVoltage = (UINT16) ((ProcessorVoltage * 10) / 1000);
    *(UINT8 *) &ProcInfo->Voltage = (UINT8) ProcessorVoltage;

    ProcInfo->Voltage.ProcessorVoltageIndicateLegacy = 1;

    ProcInfo->MaxSpeed = SMBIOS_TYPE_4_MAX_SPEED;

    MaxFreqBrandStr = GetMaxSpeedFromBrandString(BrandString);
    if (MaxFreqBrandStr) ProcInfo->CurrentSpeed = MaxFreqBrandStr;
    else ProcInfo->CurrentSpeed = ((UINT16)AsmReadMsr64(EFI_MSR_IA32_PERF_CTL) >> 8) * 100;//AptioV server override:EIP#126818 change made to read correct frequency
    
    if (((UINT32)AsmReadMsr64 (EFI_MSR_FLEX_RATIO) & BIT16) == 0x10000) {
      ProcInfo->CurrentSpeed = ((UINT16)AsmReadMsr64 (EFI_MSR_FLEX_RATIO) >> 8) * 100;
    }

    ProcInfo->ExternalClock = SMBIOS_TYPE_4_EXTERNAL_CLOCK;
    ProcInfo->Status = 0x41;    //Populated and enabled.
    ProcInfo->ProcessorUpgrade = SMBIOS_TYPE_4_PROC_UPGRADE;

    ProcInfo->L1CacheHandle = CreateSmbiosTable7(1);
    ProcInfo->L2CacheHandle = CreateSmbiosTable7(2);
    ProcInfo->L3CacheHandle = CreateSmbiosTable7(3);

    ProcInfo->SerialNumber = 0;

#ifdef SMBIOS_TYPE_4_ASSET_TAG
    ProcInfo->AssetTag = AddStrGetTok(SmbiosLibHandle, CONVERT_TO_STRING(SMBIOS_TYPE_4_ASSET_TAG));
#else
    ProcInfo->AssetTag = 0;
#endif
#ifdef SMBIOS_TYPE_4_PART_NUMBER
    ProcInfo->PartNumber = AddStrGetTok(SmbiosLibHandle, CONVERT_TO_STRING(SMBIOS_TYPE_4_PART_NUMBER));
#else
    ProcInfo->PartNumber = 0;
#endif

//Grangeville AptioV override start - Implement according to SMBIOS 3.0
    GetSupportedCoreThreadCounts(ProcessorNumber, &CoreCount, &ThreadCount);
    if ((PcdGet32 (PcdCpuProcessorFeatureUserConfiguration) & PCD_CPU_HT_BIT) == 0) {
      ThreadCount /= 2;
    }
    ThreadPerCore = ThreadCount / CoreCount;

    if ((CoreCount > 0x0) && (CoreCount <= 0xFF)) {
        ProcInfo->CoreCount = (UINT8)CoreCount;
        ProcInfo->CoreCount2 = CoreCount;
    }
    else if ((CoreCount > 0xFF) && (CoreCount <= 0xFFFE)) {
        ProcInfo->CoreCount = 0xFF;
        ProcInfo->CoreCount2 = CoreCount;
    }
    else {
        ProcInfo->CoreCount = 0;
        ProcInfo->CoreCount2 = 0;
    }

    if ((ThreadCount > 0x0) && (ThreadCount <= 0xFF)) {
        ProcInfo->ThreadCount = (UINT8)ThreadCount;
        ProcInfo->ThreadCount2 = ThreadCount;
    }
    else if ((ThreadCount > 0xFF) && (ThreadCount <= 0xFFFE)) {
        ProcInfo->ThreadCount = 0xFF;
        ProcInfo->ThreadCount2 = ThreadCount;
    }
    else {
        ProcInfo->ThreadCount = 0;
        ProcInfo->ThreadCount2 = 0;
    }

    EnabledCoreCount = (UINT16)gSmbiosCpuInfo[SocketNumber].EnabledCoreCount;
    if ((EnabledCoreCount > 0x0) && (EnabledCoreCount <= 0xFF)) {
        ProcInfo->EnabledCoreCount = (UINT8)EnabledCoreCount;
        ProcInfo->EnabledCoreCount2 = EnabledCoreCount;
    }
    else if ((EnabledCoreCount > 0xFF) && (EnabledCoreCount <= 0xFFFE)) {
        ProcInfo->EnabledCoreCount = 0xFF;
        ProcInfo->EnabledCoreCount2 = EnabledCoreCount;
    }
    else {
        ProcInfo->EnabledCoreCount = 0;
        ProcInfo->EnabledCoreCount2 = 0;
    }

    ProcInfo->ProcessorCharacteristics = BIT2 + BIT5 + BIT7; //X64 Support, Execution Disable Support, Power/Performance
    if (CoreCount > 1)  ProcInfo->ProcessorCharacteristics |= BIT3;
//Grangeville AptioV override end - Implement according to SMBIOS 3.0

    if (ThreadPerCore > 1) ProcInfo->ProcessorCharacteristics |= BIT4;

    if (GetProcessorFeatureCapability (ProcessorNumber, Vt, NULL))
        ProcInfo->ProcessorCharacteristics |= BIT6;

    Status = gSmbiosProtocol->Add(
        gSmbiosProtocol,
        NULL,
        &Handle,
        (VOID*)ProcInfo
    );
    ASSERT_EFI_ERROR(Status);
    
    FreePool(BrandString);
    FreeSmbiosTable(SmbiosLibHandle);
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  CreateSmbiosEmptySocketTable4
//
// Description: Create Empty socket SMBIOS Table 4
//
// Input:  IN UINT32 PhysSocket
//
// Output: VOID
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID CreateSmbiosEmptySocketTable4(UINT32 PhysSocket)
{
    EFI_SMBIOS_HANDLE Handle = 0xfffe;
    SMBIOS_TABLE_TYPE4 *ProcInfo;
    EFI_STATUS Status;
    VOID              *SmbiosLibHandle;
    
    ProcInfo = CreateSmbiosTable(sizeof(SMBIOS_TABLE_TYPE4), SMBIOS_BUFFER_SIZE, &SmbiosLibHandle);
    ASSERT(ProcInfo != NULL);
    
    SetMem(ProcInfo, sizeof(SMBIOS_TABLE_TYPE4), 0);

    ProcInfo->Hdr.Type = 4;
//Grangeville AptioV override - Implement according to SMBIOS 3.0
    ProcInfo->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE4);
    ProcInfo->Hdr.Handle = 0xffff;    //To be updated by SMBIOS driver.

    if (PhysSocket >= SMBIOS_MAX_NUM_SOCKETS) ProcInfo->Socket = 0;     //If socket not in table, don't provide string.
    else ProcInfo->Socket = AddStrGetTok(SmbiosLibHandle, gSocketDesgination[PhysSocket]);

    ProcInfo->L1CacheHandle = 0xffff;
    ProcInfo->L2CacheHandle = 0xffff;
    ProcInfo->L3CacheHandle = 0xffff;

    Status = gSmbiosProtocol->Add(
        gSmbiosProtocol,
        NULL,
        &Handle,
        (VOID*)ProcInfo
    );
    ASSERT_EFI_ERROR(Status);

    FreeSmbiosTable(SmbiosLibHandle);
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:   GetBoardSocketNumber
//
// Description: Get socket number from Apic ID.
//
// Input:
//  IN UINT32 ApicId
//
// Output:  UINT32 - Physical Socket Id
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32 GetBoardSocketNumber(IN UINT32 ProcessorNumber, IN UINT32 ApicId)
{
    EFI_CPUID_REGISTER      *CpuidRegister;
    UINT8  MaxThreadsPackage;

    CpuidRegister = GetProcessorCpuid(ProcessorNumber, EFI_CPUID_VERSION_INFO);
    MaxThreadsPackage = (UINT8)(CpuidRegister->RegEbx >> 16);
    
    return ApicId / MaxThreadsPackage;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  AddCpuSmbiosTables
//
// Description: Create CPU SMBIOS Tables. Installs notification on SMBIOS handlers.
//
// Input: VOID
//
// Output: VOID
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID AddCpuSmbiosTables()
{
    EFI_STATUS Status;
    UINT8 i;
    UINT32 SocketPopBitmap = 0;
    EFI_SMBIOS_HANDLE Handle;
    EFI_SMBIOS_TABLE_HEADER *Record;
    EFI_SMBIOS_TYPE Type;
    UINT32 ProcessorNumber;
    UINT32 CurrentPackageNumber;
    UINT32 CurrentCoreNumber;
    UINT32 NumPopulatedSockets;
    
    ASSERT(NUMBER_CPU_SOCKETS <= SMBIOS_MAX_NUM_SOCKETS);

    Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, &gSmbiosProtocol);
    if (EFI_ERROR(Status)) return;  //First call, error, because protocol not installed yet.
   
    gSmbiosCpuInfo = AllocatePool(NUMBER_CPU_SOCKETS * sizeof(SMBIOS_CPU_INFO));

    //Delete extra existing CPU SBIOS structures type 4 and type 7.
     Type = 4;
     while (Handle = 0xfffe, !EFI_ERROR(gSmbiosProtocol->GetNext(gSmbiosProtocol, &Handle, &Type, &Record, NULL))) {
         gSmbiosProtocol->Remove(gSmbiosProtocol, Handle);
     }

     Type = 7;
     while (Handle = 0xfffe, !EFI_ERROR(gSmbiosProtocol->GetNext(gSmbiosProtocol, &Handle, &Type, &Record, NULL))) {
         gSmbiosProtocol->Remove(gSmbiosProtocol, Handle);
     }

     NumPopulatedSockets = 0;
     //Add CPU SMBIOS structures for populated sockets.

     CurrentPackageNumber = (UINT32)-1;
     CurrentCoreNumber = (UINT32)-1;
     for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigContextBuffer.NumberOfProcessors; ++ProcessorNumber) {
        UINT32 NewPackageNumber;
        UINT32 NewCoreNum;
        
        GetProcessorLocation (ProcessorNumber, &NewPackageNumber, &NewCoreNum, NULL);

        if (NewPackageNumber == CurrentPackageNumber) {
            if (NewCoreNum != CurrentCoreNumber) {
                gSmbiosCpuInfo[NumPopulatedSockets-1].EnabledCoreCount++;   //NumPopulatedSockets was incremented.
                CurrentCoreNumber = NewCoreNum;
            }
            continue;
        }
        CurrentPackageNumber = NewPackageNumber;
        CurrentCoreNumber = NewCoreNum;
        
        
        gSmbiosCpuInfo[NumPopulatedSockets].ProcNum = ProcessorNumber;
        gSmbiosCpuInfo[NumPopulatedSockets].PkgNum = CurrentPackageNumber;
        gSmbiosCpuInfo[NumPopulatedSockets].EnabledCoreCount = 1;
        
        ++NumPopulatedSockets;
    }

     //Add CPU SMBIOS structures for populated sockets.
    for (i = 0; i < NumPopulatedSockets; ++i) {
        UINT32 ProcessorNumber = gSmbiosCpuInfo[i].ProcNum;
        
        GetCacheInfo(ProcessorNumber); //For one thread.

        ASSERT(gSmbiosCpuInfo[i].PkgNum < SMBIOS_MAX_NUM_SOCKETS);

        CreateSmbiosTable4(i, gSmbiosCpuInfo[i].PkgNum);
        SocketPopBitmap |= 1 << gSmbiosCpuInfo[i].PkgNum;
    }

    //Add CPU SMBIOS structures for empty sockets.
    for (i = 0; i < NUMBER_CPU_SOCKETS; ++i) {
        if (SocketPopBitmap & (1 << i)) continue;
        CreateSmbiosEmptySocketTable4(i);
    }

    FreePool(gSmbiosCpuInfo);
}
