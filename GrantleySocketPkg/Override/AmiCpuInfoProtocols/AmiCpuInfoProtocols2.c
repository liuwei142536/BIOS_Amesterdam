//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//<AMI_FHDR_START>
//---------------------------------------------------------------------------
// Name: AmiCpuInfo2.c
//
// Description: AmiCpuInfo2 Protocol functions
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>

#include "AmiCpuInfo.h"
#include "Token.h"

UINT32 GetCpuNumByPkgCoreThrd(
    IN UINT32 Package,
    IN UINT32 Core,
    IN UINT32 Thread
);

BOOLEAN GetCpuPkgCoreThrdByNum(
    IN UINT32 CpuNum,
    OUT UINT32 *Package,
    OUT UINT32 *Core,
    OUT UINT32 *Thread
);

typedef struct {
    UINT32 NumCores;    //Number of cores in Package.
    UINT32 NumEntries;  //Each cache descriptor should have same number of entries, max of 4.
    AMI_CPU_INFO_2_CACHE_DESCR PkgCacheDesc[4];   //Total of the cores
    AMI_CPU_INFO_2_CACHE_DESCR CoreCacheDesc[4];  //Internal implementation assumes symmetry among cores.
                                                  //If this every changes, internal implementation will be updated.
} PKG_CACHE_DESCR;

//The following definitions are usally defined in Asl.sdl
#ifndef LAPIC_1_TRIGGER_MODE
#define LAPIC_1_TRIGGER_MODE 1
#endif
#ifndef LAPIC_1_POLARITY
#define LAPIC_1_POLARITY 1
#endif
#ifndef LAPIC_1_DEST_LINTIN
#define LAPIC_1_DEST_LINTIN 1
#endif

static PKG_CACHE_DESCR *gPkgCacheDesc; //Array of Package Cache Descriptions.

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:  GetCacheSharedThreads
//
// Description: Return number of shared threads for a Information.
//
// Input:  IN UINT8 Level - Cache level
//
// Output: UINT8 - Number of shared threads.
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8 GetCacheSharedThreads(IN UINT8 Level)
{
    UINT32 RegEax;
    UINT32 i = 0;

    for(;;) {
        AsmCpuidEx(4, i, &RegEax, NULL, NULL, NULL);
        if ((RegEax & 0x1f) == 0) break;
        if (((RegEax >> 5) & 7) == Level) return 1 + ((RegEax >> 14)& 0xfff);
        ++i;
    }
    return 0;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:   GetNumPackages
//
// Description: Get the number of packages populated and sockets.
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL  *This
//  OUT UINT32                  *NumPopulatedPackages
//  OUT UINT32                  *NumBoardSockets OPTIONAL
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetNumPackages(
    IN AMI_CPU_INFO_2_PROTOCOL  *This,
    OUT UINT32                  *NumPopulatedPackages,
    OUT UINT32                  *NumBoardSockets OPTIONAL
)
{
    UINT32 i = 0;

    *NumPopulatedPackages = 0;
    while(i < gNumCpus) {
        i += gAmiCpuInfoData[i].NumCores * (gAmiCpuInfoData[i].NumHts ? 2 : 1);
        ++*NumPopulatedPackages;
    }

    //NUMBER_CPU_SOCKETS must be ported if more than 1 socket.
    ASSERT(*NumPopulatedPackages <= NUMBER_CPU_SOCKETS);
    if (NumBoardSockets) {
        *NumBoardSockets = NUMBER_CPU_SOCKETS;

        //In case of porting error, Board sockets can never be less than Populated packages.
        if (*NumPopulatedPackages > *NumBoardSockets) *NumBoardSockets = *NumPopulatedPackages;
    }
    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:   GetNumCoresThreads
//
// Description: Get the number of Cores
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL  *This,
//  IN UINT32                   Package
//  OUT UINT32                  *NumEnabledCores
//  OUT UINT32                  *NumEnabledThreads - This value is total for package.
//  OUT UINT32                  *NumDisabledCores OPTIONAL -- Flag must be set in Protocol.
//  OUT UINT32                  *NumEnabledThreads OPTIONAL  -- Flag must be set in Protocol.
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetNumCoresThreads(
    IN AMI_CPU_INFO_2_PROTOCOL  *This,
    IN UINT32                   Package,
    OUT UINT32                  *NumEnabledCores,
    OUT UINT32                  *NumEnabledThreads,
    OUT UINT32                  *NumDisabledCores OPTIONAL,
    OUT UINT32                  *NumDisabledThreads OPTIONAL
)
{
    UINT32 SupportedCores;
    UINT32 SupportedThreads;
    UINT32 CpuNum = GetCpuNumByPkgCoreThrd(Package, 0, 0); //Core = 0 and Thread = 0.
    if (CpuNum == (UINT32) -1) return EFI_INVALID_PARAMETER;

    SupportedCores = gCpuDxeIntThreadData[CpuNum].MaxSupportedCores;
    SupportedThreads = gCpuDxeIntThreadData[CpuNum].MaxSupportedThreads;

    *NumEnabledCores = gAmiCpuInfoData[CpuNum].NumCores;
    *NumEnabledThreads = *NumEnabledCores * (gAmiCpuInfoData[CpuNum].NumHts == 2 ? 2 : 1);

    if (NumDisabledCores) *NumDisabledCores = SupportedCores - *NumEnabledCores;
    if (NumDisabledThreads) *NumDisabledThreads = SupportedThreads - *NumEnabledThreads;

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure:   GetNumThreads
//
// Description: Get the number of Threads
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL  *This
//  IN UINT32                   Package
//  IN UINT32                   Core
//  OUT UINT32                  *NumEnabledThreads
//  OUT UINT32                  *NumDisabledThreads OPTIONAL  -- Flag must be set if valid
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetNumThreads(
    IN AMI_CPU_INFO_2_PROTOCOL  *This,
    IN UINT32                   Package,
    IN UINT32                   Core,
    OUT UINT32                  *NumEnabledThreads,
    OUT UINT32                  *NumDisabledThreads OPTIONAL
)
{
    UINT32 CpuNum = GetCpuNumByPkgCoreThrd(Package, Core, 0);
    if (CpuNum == (UINT32) -1) return EFI_INVALID_PARAMETER;

    *NumEnabledThreads = gAmiCpuInfoData[CpuNum].NumHts ? 2 : 1;
    if (NumDisabledThreads) {
        BOOLEAN HtSupport = (gCpuDxeIntThreadData[CpuNum].MaxSupportedThreads / gCpuDxeIntThreadData[CpuNum].MaxSupportedCores)
             > 1;
        *NumDisabledThreads = (HtSupport && *NumEnabledThreads == 1) ? 1 : 0;
    }
    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: GetSbsp
//
// Description: Get SBSP
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL  *This
//  OUT UINT32                  *Package
//  OUT UINT32                  *Core
//  OUT UINT32                  *Thread
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetSbsp(
    IN AMI_CPU_INFO_2_PROTOCOL  *This,
    OUT UINT32                  *Package,
    OUT UINT32                  *Core,
    OUT UINT32                  *Thread
)
{
    //Internal structures are ordered by APIC ID.
    //MP functions are also ordered by APIC ID.

    if (GetCpuPkgCoreThrdByNum(gBsp, Package, Core, Thread)) return EFI_SUCCESS;
    return EFI_INVALID_PARAMETER;
}


//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: GetApicInfo
//
// Description: Get Apic Number and Version
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL  *This
//  IN UINT32                   Package
//  IN UINT32                   Core
//  IN UINT32                   Thread
//  OUT UINT32                  *ApicId
//  OUT UINT32                  *ApicVer OPTIONAL
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetApicInfo(
    IN AMI_CPU_INFO_2_PROTOCOL  *This,
    IN UINT32                   Package,
    IN UINT32                   Core,
    IN UINT32                   Thread,
    OUT UINT32                  *ApicId,
    OUT UINT32                  *ApicVer OPTIONAL
)
{
    UINT32 CpuNum = GetCpuNumByPkgCoreThrd(Package, Core, Thread);

    if (CpuNum == (UINT32) -1) return EFI_INVALID_PARAMETER;

    *ApicId = gCpuDxeIntThreadData[CpuNum].ApicId;

    if (ApicVer) {
        *ApicVer = gCpuDxeIntThreadData[CpuNum].ApicVer;
    }

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: GetAcpiInfo
//
// Description: Get Cpu ACPI information.
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL  *This
//  OUT VOID                    **AcpiData  - ACPI Data
//  OUT UINT32                  *NumEntries - Number of Entries in data.
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetAcpiInfo(
    IN AMI_CPU_INFO_2_PROTOCOL  *This,
    OUT VOID                    **AcpiData,
    OUT UINT32                  *NumEntries
)
{
    return EFI_UNSUPPORTED;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: GetPackageCacheDescr
//
// Description: Get the package Cache Information
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL      *This
//  IN UINT32                       Package - Socket number. Intenal socket number (continous)
//  OUT AMI_CPU_INFO_2_CACHE_DESCR  **Description - Updates pointer to pointer with pointer to Cache information. 
//  OUT UINT32                      *NumEntries - Number of AMI_CPU_INFO_2_CACHE_DESCR Entries.
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetPackageCacheDescr(
    IN AMI_CPU_INFO_2_PROTOCOL      *This,
    IN UINT32                       Package,
    OUT AMI_CPU_INFO_2_CACHE_DESCR  **Description,
    OUT UINT32                      *NumEntries
)
{
    PKG_CACHE_DESCR *PkgCacheDesc;

    if (Package >= NumberOfCpuSocketsPopulated()) return EFI_INVALID_PARAMETER;

    PkgCacheDesc = &gPkgCacheDesc[Package];
    *Description = &PkgCacheDesc->PkgCacheDesc[0];
    *NumEntries = PkgCacheDesc->NumEntries;
    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: GetCoreCacheDescr
//
// Description: Get the Core Cache Information
//
// Input:
//  IN AMI_CPU_INFO_2_PROTOCOL      *This
//  IN UINT32                       Package - Socket number. Internal socket number (continous)
//  IN UINT32                       Core - Core number. Internal core number (continous)
//  OUT AMI_CPU_INFO_2_CACHE_DESCR  **Description - Updates pointer to pointer with pointer to Cache information. 
//  OUT UINT32                      *NumEntries - Number of AMI_CPU_INFO_2_CACHE_DESCR Entries.
//
// Output:  EFI_STATUS
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetCoreCacheDescr(
    IN AMI_CPU_INFO_2_PROTOCOL      *This,
    IN UINT32                       Package,
    IN UINT32                       Core,
    OUT AMI_CPU_INFO_2_CACHE_DESCR  **Description,
    OUT UINT32                      *NumEntries
)
{
    PKG_CACHE_DESCR *PkgCacheDesc;

    if (Package >= NumberOfCpuSocketsPopulated()) return EFI_INVALID_PARAMETER;

    PkgCacheDesc = &gPkgCacheDesc[Package];

    if (Core >= PkgCacheDesc->NumCores) return EFI_INVALID_PARAMETER;

    *Description = &PkgCacheDesc->CoreCacheDesc[0];
    *NumEntries = PkgCacheDesc->NumEntries;
    return EFI_SUCCESS;
}


AMI_CPU_INFO_2_PROTOCOL gAmiCpuInfo2 = {
    3,          //ProtocolVer
    AMI_CPU_INFO_2_FLAGS_NUM_DIS_CORES_VALID | AMI_CPU_INFO_2_FLAGS_NUM_DIS_THRDS_VALID,          //Flags
    GetNumPackages,
    GetNumCoresThreads,
    GetNumThreads,
    GetSbsp,
    GetApicInfo,
    GetAcpiInfo,
    GetPackageCacheDescr,
    GetCoreCacheDescr
};

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: GetPtrToPrivateAmiCpuInfo2Entry
//
// Description: Get CPU Number
//
// Input:
//  IN UINT32 Package
//  IN UINT32 Core
//  IN UINT32 Thread
//
// Output:  UINT32 - Cpu Number or -1
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32 GetCpuNumByPkgCoreThrd(
    IN UINT32 Package,
    IN UINT32 Core,
    IN UINT32 Thread
)
{
    UINT32 Cpu = 0;
    UINT32 Pkg = 0;
    UINT32 MaxThread;

    while (Pkg < Package) {
        Cpu += gAmiCpuInfoData[Cpu].NumCores * (gAmiCpuInfoData[Cpu].NumHts ? 2 : 1);
        if (Cpu >= gNumCpus) return (UINT32)-1;
        ++Pkg;
    }

    MaxThread = gAmiCpuInfoData[Cpu].NumHts ? 2 : 1;
    if (Thread >= MaxThread) return (UINT32)-1;

    Cpu += Core * MaxThread + Thread;
    if (Cpu >= gNumCpus) return (UINT32)-1;

    return Cpu;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: GetCpuPkgCoreThrdByNum
//
// Description: Get CPU Package/Core/Thread by CPU Number. Number sequencial to APIC ID.
//
// Input:
//  IN UINT32 CpuNum
//  OUT UINT32 *Package
//  OUT UINT32 *Core
//  OUT UINT32 *Thread
//
// Output:  BOOLEAN -- If found, return TRUE.
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

BOOLEAN GetCpuPkgCoreThrdByNum(
    IN UINT32 CpuNum,
    OUT UINT32 *Package,
    OUT UINT32 *Core,
    OUT UINT32 *Thread
)
{
    UINT32 MaxThreadsPkg;
    UINT32 i = 0;


    if (CpuNum >= gNumCpus) return FALSE;

    //Calculate package Num.
    *Package = 0;
    while(TRUE) {
        MaxThreadsPkg = gAmiCpuInfoData[i].NumCores * (gAmiCpuInfoData[i].NumHts ? 2 : 1);
        if ((i + MaxThreadsPkg) > CpuNum) break;
        i += MaxThreadsPkg;
        CpuNum -= MaxThreadsPkg;
        ++*Package;        
    }

    if (gAmiCpuInfoData[i].NumHts) {
        if (CpuNum & 1) {
        *Core = (CpuNum - 1) / 2;
        *Thread = 1;
        } else {
        *Core = CpuNum / 2;
        *Thread = 0;
        }
    } else {
        *Core = CpuNum;
        *Thread = 0;
    }

    return TRUE;
}



//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: CreateAmiAcpiInfo2ProtocolData
//
// Description: Create AmiCpuInfoProtocol2 Data
//
// Input: VOID
//
// Output:  VOID
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID CreateAmiAcpiInfo2ProtocolData()
{
    UINT32  NumPackages;
    UINT32  Package;
    UINT32  CpuNum;
    EFI_STATUS Status;

    BOOLEAN IsLxSharedByPackage[4];

                                                                                                                                                //Assume between CPUs on the board, similar cache sharing. Only like CPUs can power the board.
    IsLxSharedByPackage[1] = GetCacheSharedThreads(1) > 2 ? TRUE: FALSE;
    IsLxSharedByPackage[2] = GetCacheSharedThreads(2) > 2 ? TRUE: FALSE;
    IsLxSharedByPackage[3] = GetCacheSharedThreads(3) > 2 ? TRUE: FALSE;

    Status = gAmiCpuInfo2.GetNumPackages(&gAmiCpuInfo2, &NumPackages, NULL);
    ASSERT_EFI_ERROR(Status);

    gPkgCacheDesc = AllocatePool(sizeof(PKG_CACHE_DESCR) * NumPackages);

    ZeroMem(gPkgCacheDesc, sizeof(PKG_CACHE_DESCR) * NumPackages);

    //Assume symmetry between the cores in a package. This never likely to change.
    for(Package = 0, CpuNum = 0; CpuNum < gNumCpus; ++Package) {
        AMI_CPU_INFO *AmiCpuInfo    = &gAmiCpuInfoData[CpuNum];
        PKG_CACHE_DESCR *PkgDesc    = &gPkgCacheDesc[Package];
        CACHE_DESCRIPTOR_INFO *CacheInfo    = AmiCpuInfo->CacheInfo;
        UINT32 NumCores             = AmiCpuInfo->NumCores;
        UINT32 Entry = 0;

        ASSERT (Package < NumPackages);

        PkgDesc->NumCores = NumCores;

        while(CacheInfo->Desc != 0) {
            AMI_CPU_INFO_2_CACHE_DESCR *PkgCacheDesc = &PkgDesc->PkgCacheDesc[Entry];
            AMI_CPU_INFO_2_CACHE_DESCR *CoreCacheDesc = &PkgDesc->CoreCacheDesc[Entry];

            ASSERT(Entry < 4);
            ASSERT(CacheInfo->Level < 4);

            CoreCacheDesc->LengthDesc = sizeof(AMI_CPU_INFO_2_CACHE_DESCR);
            CoreCacheDesc->Level = CacheInfo->Level;
            CoreCacheDesc->Type = CacheInfo->Type;
            CoreCacheDesc->Size = CacheInfo->Size;
            CoreCacheDesc->Associativity = CacheInfo->Associativity;
            CoreCacheDesc->Shared = IsLxSharedByPackage[CacheInfo->Level] + 1;

            if (CoreCacheDesc->Type == 3) {
                CoreCacheDesc->Type = 2;   //Translate type from AMI CPU INFO 1 to AMI CPU INFO 2.
            }

            CopyMem(PkgCacheDesc, CoreCacheDesc, sizeof(AMI_CPU_INFO_2_CACHE_DESCR));

            PkgCacheDesc->Size *= !IsLxSharedByPackage[CacheInfo->Level] ? NumCores : 1;

            ++Entry;
            ++CacheInfo;
        }
        CpuNum += NumCores * (AmiCpuInfo->NumHts ? 2 : 1)                             ;
        PkgDesc->NumEntries = Entry;
    }
}


//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

