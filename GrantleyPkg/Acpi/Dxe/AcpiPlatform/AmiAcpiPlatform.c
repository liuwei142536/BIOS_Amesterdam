#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <AcpiRes.h>
#include <Protocol/MpService.h>
#include <Library/CpuConfigLib.h>
#include <Guid/SetupVariable.h>

#define C1_ACPI_LATENCY 1
#define C3_ACPI_LATENCY 15
#define C6_ACPI_LATENCY 41
#define C7_ACPI_LATENCY 41

#define C1_ACPI_POWER 0x3e8 //Broadwell DE
#define C3_ACPI_POWER 0x1f4
#define C6_ACPI_POWER 0x15e
#define C7_ACPI_POWER 0xc8

#define MAX_NUMBER_OF_TSTATES 15
#define MAX_NUMBER_OF_PSTATES 16


static UINT8 gC3Map, gC6Map, gC7Map;
static UINT8 gNumCxStates;

extern SYSTEM_CONFIGURATION mSystemConfiguration;

BOOLEAN IsTurboModeEnabled();

VOID GetCstateMappings()
{
    gC3Map = mSystemConfiguration.C3Enable ? 2 : 0;    //Report ACPI C2

    if (mSystemConfiguration.C6Enable) {
        gC6Map = mSystemConfiguration.OSCx ? 3 : 2;    //Report ACPI C3 or ACPI C2
    } else gC6Map = 0;

    gC7Map = 0; //No C7 on this system.

    gNumCxStates = 1 + !!(gC3Map) + !!(gC6Map) + !!(gC7Map);    //"1 +" to count C1 state.
}

#pragma pack(push, 1)

typedef struct {
    UINT8 BufferOp; //ResourceTemplate(){Regisgeter(...)}
    UINT8 PkgLength;
    //Buffer[0x11] of Resource;
    UINT8 BytePrefix;
    UINT8 BufferLength;     //0x11
    UINT8 GenericRegDesc;   //0x82
    UINT16 Length;          //0x12
    UINT8 AddressSpaceKeyword;
    UINT8 RegisterBitWidth;
    UINT8 RegisterBitOffset;
    UINT8 AddressSize;
    UINT64 RegisterAddress;
    UINT8 EndTag;           //0x79
    UINT8 Checksum;
} RESEOURCE_TEMPLATE;

typedef struct {
    UINT8  PkgOp;
    UINT16 PkgLength;
    UINT8  NumElem;
    UINT8  BytePrefix;      //1st element
    UINT8  NumCStates;      //1st element
} PKG_CSTATE_HDR;

typedef struct {
    UINT8               PkgOp;
    UINT8               PkgLength;
    UINT8               NumElem;
    RESEOURCE_TEMPLATE  RescTemp;
    UINT8               BytePrefix;
    UINT8               Cstate;
    UINT8               BytePrefix2;
    UINT8               Latency;
    UINT8               WordPrefix;
    UINT16              Power;
} PKG_CSTATE;


//ASL _PSS package structure.
typedef struct {
    UINT8   WordPrefix1;        //All word prefixes will be 0xb;
    UINT16  CoreFreq;
    UINT8   DWordPrefix2;
    UINT32  Power;
    UINT8   WordPrefix3;
    UINT16  TransitionLatency;
    UINT8   WordPrefix4;
    UINT16  BusMasterLatency;
    UINT8   WordPrefix5;
    UINT16  Control;
    UINT8   WordPrefix6;
    UINT16  Status;
} PSTATE;

typedef struct {
    UINT8   PkgOp;
    UINT8   PkgLength;
    UINT8   NumElem;
    PSTATE  Pstate;
} PSTATE_PKG;

typedef struct {
    UINT8       PkgOp;
    UINT16      PkgLength;  //This will be resized.
    UINT8       NumElem;    //This will be resized.
} ALL_PSTATES_PKG_HDR;


typedef struct {
    UINT8   PkgOp;
    UINT8   PkgLength;
    UINT8   NumElem;
} PKG_HDR1;

typedef struct {
    PKG_HDR1    PkgHdr1;
    PKG_HDR1    PkgHdr2;
    UINT8       BytePrefix1;
    UINT8       NumEntries;
    UINT8       BytePrefix2;
    UINT8       Revision;
    UINT8       BytePrefix3;
    UINT8       Domain;
    UINT8       BytePrefix4;
    UINT8       CoordType;
    UINT8       BytePrefix5;
    UINT8       NumCpus;
} PSD_PKG;


//ASL _PSS package structure.
typedef struct {
    UINT8   PkgOp;
    UINT8   PkgLength;
    UINT8   NumElem;
    UINT8   BytePrefix1;
    UINT8   Percent;
    UINT8   DwordPrefix2;
    UINT32  Power;
    UINT8   BytePrefix3;
    UINT8   Latency;
    UINT8   BytePrefix5;
    UINT8   Control;
    UINT8   BytePrefix6;
    UINT8   Status;
} TSTATE_PKG;

typedef struct {
    UINT8       PkgOp;
    UINT16      PkgLength;
    UINT8       NumElem;
} ALL_TSTATES_PKG_HDR;

typedef struct {
    UINT8       PkgOp;
    UINT8       PkgLength;
    UINT8       NumElem;
} ALL_TSSA_PKG_HDR;
#pragma pack(pop)

PSTATE      gPstates[MAX_NUMBER_OF_PSTATES][MAX_SOCKET];
UINT8       gNumPstates[MAX_SOCKET] = {0};

#define AML_PKG_LEN_WORD_ENCODING  0x40
#define AML_PKG_LEN_3BYTE_ENCODING 0x80
#define AML_PKG_LEN_DWORD_ENCODING 0xc0

EFI_STATUS UpdatePkgLen(UINT8 *ptr, UINT32 Len) {
    UINT8 NumOfBytes = (*ptr) >> 6;
    switch (NumOfBytes) {
    case 0:
        if (Len > 63) return EFI_INVALID_PARAMETER;
        *ptr = Len;
        break;
    case 1:
        if (Len > 4095) return EFI_INVALID_PARAMETER;
        *ptr = (Len & 0xf) | AML_PKG_LEN_WORD_ENCODING;
        *(ptr + 1) = (UINT8)(Len >> 4);
        break;
    case 2:
        if (Len > 0x1fffff) return EFI_INVALID_PARAMETER;
        *ptr = (Len & 0xf) | AML_PKG_LEN_3BYTE_ENCODING;
        *(UINT16*)(ptr + 1) = (UINT16)(Len >> 4);
        break;
    case 3:
        if (Len > 0x0fffffff) return EFI_INVALID_PARAMETER;
        *ptr = (Len & 0xf) | AML_PKG_LEN_DWORD_ENCODING;
        *(UINT32*)(ptr + 1) &= 0xff000000;
        *(UINT32*)(ptr + 1) |= (UINT32)(Len >> 4);
        break;
    }
    return EFI_SUCCESS;
}

VOID UpdateCStateAsl(
    EFI_ACPI_DESCRIPTION_HEADER *Table,
    IN OUT ASL_OBJ_INFO *SbScopeObjInfo
)
{
    UINT8       *pSbScope = SbScopeObjInfo->DataStart;
    ASL_OBJ_INFO ObjInfo;
    UINT32       PkgLen;
    PKG_CSTATE_HDR *PkgCstHdr;
    PKG_CSTATE  *PkgCst;
    UINT8       *ptr;
    EFI_STATUS  Status;
    UINT32      BytesToRemove;


    UINT8   NumOrigAslCxStates;
   
    UINT8   C1Latency = C1_ACPI_LATENCY;
    UINT8   C3Latency = C3_ACPI_LATENCY;
    UINT8   C6Latency = C6_ACPI_LATENCY;
    UINT8   C7Latency = C7_ACPI_LATENCY;
    
    UINT16  C1Power = C1_ACPI_POWER;
    UINT16  C3Power = C3_ACPI_POWER;
    UINT16  C6Power = C6_ACPI_POWER;
    UINT16  C7Power = C7_ACPI_POWER;

    GetCstateMappings();
    
    //Mwait
    Status = GetAslObj(pSbScope, SbScopeObjInfo->Length, "CMST", otName, &ObjInfo);
    if (EFI_ERROR(Status)) return;
    ptr = (UINT8*)ObjInfo.DataStart;

    //This points to package to update number of elements;
    PkgCstHdr = (PKG_CSTATE_HDR *)ptr;

    NumOrigAslCxStates = PkgCstHdr->NumCStates;
    PkgCstHdr->NumCStates = gNumCxStates;
    PkgCstHdr->NumElem = gNumCxStates + 1;
    ASSERT(PkgCstHdr->BytePrefix == AML_BYTE_PREFIX);

    PkgCst = (PKG_CSTATE*)(PkgCstHdr + 1);  //This points to fixed entry.

    //Always C1;
    PkgCst->Cstate = 1;
    PkgCst->Latency = C1Latency;
    PkgCst->Power = C1Power;
    ASSERT(PkgCst->BytePrefix2 == AML_BYTE_PREFIX);
    ++PkgCst;

    if (gC3Map) {
        PkgCst->RescTemp.RegisterAddress = 0x10;
        PkgCst->Cstate = gC3Map;
        PkgCst->Latency = C3Latency;
        PkgCst->Power = C3Power;
        ++PkgCst;
    }
        
    if (gC6Map) {
        PkgCst->RescTemp.RegisterAddress = 0x20;
        PkgCst->Cstate = gC6Map;
        PkgCst->Latency = C6Latency;
        PkgCst->Power = C6Power;
        ASSERT(PkgCst->BytePrefix2 == AML_BYTE_PREFIX);
        ++PkgCst;
    }
    
    if (gC7Map) {
        PkgCst->RescTemp.RegisterAddress = 0x30;
        PkgCst->Cstate = gC7Map;
        PkgCst->Latency = C7Latency;
        PkgCst->Power = C7Power;
        ASSERT(PkgCst->BytePrefix2 == AML_BYTE_PREFIX);
        ++PkgCst;
    }

    //
    //Remove Extra entries.
    //
    
    ASSERT(NumOrigAslCxStates >= gNumCxStates);
    if (NumOrigAslCxStates > gNumCxStates) {
        BytesToRemove = (NumOrigAslCxStates - gNumCxStates) * sizeof(PKG_CSTATE);

        //Update C-state package header.
        PkgLen = (UINT32) GetPackageLen((AML_PACKAGE_LBYTE*)&PkgCstHdr->PkgLength, NULL);
        PkgLen -= BytesToRemove;
        Status = UpdatePkgLen((UINT8*)&PkgCstHdr->PkgLength, PkgLen);
        ASSERT_EFI_ERROR(Status);
    
        //Update the _SB Scope size.
        PkgLen = (UINT32)GetPackageLen((AML_PACKAGE_LBYTE*)((UINT8*)SbScopeObjInfo->Object + 1), NULL);
        PkgLen -= BytesToRemove;
        Status = UpdatePkgLen((UINT8*)SbScopeObjInfo->Object + 1, PkgLen); 
        ASSERT_EFI_ERROR(Status);
        SbScopeObjInfo->Length -= BytesToRemove;

        MemCpy(PkgCst, (UINT8*)PkgCst + BytesToRemove, Table->Length - ((UINT8*)PkgCst + BytesToRemove - (UINT8*)Table));
        Table->Length -= BytesToRemove;
    }
    
    //IO C-states
    Status = GetAslObj(pSbScope, SbScopeObjInfo->Length, "CIST", otName, &ObjInfo);
    if (EFI_ERROR(Status)) return;
    ptr = (UINT8*)ObjInfo.DataStart;

    //This points to package to update number of elements;
    PkgCstHdr = (PKG_CSTATE_HDR *)ptr;

    NumOrigAslCxStates = PkgCstHdr->NumCStates;
    PkgCstHdr->NumCStates = gNumCxStates;
    PkgCstHdr->NumElem = gNumCxStates + 1;
    ASSERT(PkgCstHdr->BytePrefix == AML_BYTE_PREFIX);

    PkgCst = (PKG_CSTATE*)(PkgCstHdr + 1);  //This points to fixed entry.
    //Always C1;
    PkgCst->Cstate = 1;
    PkgCst->Latency = C1Latency;
    PkgCst->Power = C1Power;
    ++PkgCst;
    ASSERT(PkgCst->BytePrefix2 == AML_BYTE_PREFIX);
   
    if (gC3Map) {
        PkgCst->RescTemp.RegisterAddress = PM_BASE_ADDRESS + 0x14;
        PkgCst->Cstate = gC3Map;
        PkgCst->Latency = C3Latency;
        PkgCst->Power = C3Power;
        ASSERT(PkgCst->BytePrefix2 == AML_BYTE_PREFIX);
        ++PkgCst;
    }
        
    if (gC6Map) {
        PkgCst->RescTemp.RegisterAddress = PM_BASE_ADDRESS + 0x15;
        PkgCst->Cstate = gC6Map;
        PkgCst->Latency = C6Latency;
        PkgCst->Power = C6Power;
        ASSERT(PkgCst->BytePrefix2 == AML_BYTE_PREFIX);
        ++PkgCst;
    }
    
    if (gC7Map) {
        PkgCst->RescTemp.RegisterAddress = PM_BASE_ADDRESS + 0x16;
        PkgCst->Cstate = gC7Map;
        PkgCst->Latency = C7Latency;
        PkgCst->Power = C7Power;
        ASSERT(PkgCst->BytePrefix2 == AML_BYTE_PREFIX);
        ++PkgCst;
    }

    //
    //Remove Extra entries.
    //
    ASSERT(NumOrigAslCxStates >= gNumCxStates);
    if (NumOrigAslCxStates > gNumCxStates) {
        BytesToRemove = (NumOrigAslCxStates - gNumCxStates) * sizeof(PKG_CSTATE);
    
        //Update C-state package header.
        PkgLen = (UINT32)GetPackageLen((AML_PACKAGE_LBYTE*)&PkgCstHdr->PkgLength, NULL);
        PkgLen -= BytesToRemove;
        Status = UpdatePkgLen((UINT8*)&PkgCstHdr->PkgLength, PkgLen);
        ASSERT_EFI_ERROR(Status);
    
    
        //Update the _SB Scope size.
        PkgLen = (UINT32)GetPackageLen((AML_PACKAGE_LBYTE*)((UINT8*)SbScopeObjInfo->Object + 1), NULL);
        PkgLen -= BytesToRemove;
        Status = UpdatePkgLen((UINT8*)SbScopeObjInfo->Object + 1, PkgLen); 
        ASSERT_EFI_ERROR(Status);
        SbScopeObjInfo->Length -= BytesToRemove;
    
        MemCpy(PkgCst, (UINT8*)PkgCst + BytesToRemove, Table->Length - ((UINT8*)PkgCst + BytesToRemove - (UINT8*)Table));
        Table->Length -= (UINT32)BytesToRemove;
    }
}

EFI_STATUS CalcPstates(UINT32 Socket)
{
    UINT32 Index;
    UINT8  MaxApicId;
    UINT32 Eax, Ebx, Ecx, Edx;
    UINT32 ApicId;
    UINT32 ApicIdNextSocket;
    CPU_MISC_DATA *CpuMiscData = NULL;
    static CPU_MISC_DATA *Socket0MiscData;  //Always Socket 0.
    FVID_ENTRY    *PssState;
            
    AsmCpuid (1, &Eax, &Ebx, &Ecx, &Edx);
    MaxApicId = (UINT8)(Ebx >> 16);
    ApicId = Socket * MaxApicId;
    ApicIdNextSocket = (Socket + 1) * MaxApicId;
   
    for (Index = 0; Index < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; ++Index) {
        UINT32 ApicIdIndex = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[Index].CpuMiscData.ApicID;
        //Core0 could be disabled, so get first from socket.
        if (ApicIdIndex >= ApicId && ApicIdIndex < ApicIdNextSocket) {
            CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[Index].CpuMiscData;
            break;
        }
    }

    //If socket unpopulated, use Socket 0 data because of hotplug.
    if (Socket == 0) Socket0MiscData = CpuMiscData;
    else if (CpuMiscData == NULL) CpuMiscData = Socket0MiscData;
    
    ASSERT(CpuMiscData != NULL);
    if (CpuMiscData == NULL) return EFI_UNSUPPORTED;
    
    PssState = CpuMiscData->FvidTable;
    
    //Initialize prefixes, latency
    for(Index = 0; Index < MAX_NUMBER_OF_PSTATES; ++Index) {
        PSTATE *Pstate = &gPstates[Index][Socket];
        
        if (Index == CpuMiscData->NumberOfPStates) break;
        
        Pstate->WordPrefix1 = AML_WORD_PREFIX;
        Pstate->DWordPrefix2 = AML_DWORD_PREFIX;
        Pstate->WordPrefix3 = AML_WORD_PREFIX;
        Pstate->WordPrefix4 = AML_WORD_PREFIX;
        Pstate->WordPrefix5 = AML_WORD_PREFIX;
        Pstate->WordPrefix6 = AML_WORD_PREFIX;
        Pstate->TransitionLatency = 10;
        Pstate->BusMasterLatency  = 10;

        Pstate->CoreFreq = (UINT32)(CpuMiscData->IntendedFsbFrequency * PssState[Index].Ratio);

        if (Index == 0 && IsTurboModeEnabled()) {
            Pstate->CoreFreq = (UINT32)(CpuMiscData->IntendedFsbFrequency * PssState[Index + 1].Ratio) + 1;
        }
        Pstate->Power    =  (UINT32)(PssState[Index].Power);        // when calculate Tdp already make it mW;

        Pstate->Control  = (UINT32)(PssState[Index].Ratio << 8);
        Pstate->Status   = (UINT32)(PssState[Index].Ratio << 8);
    }

    gNumPstates[Socket] = Index;
    
    return EFI_SUCCESS;
}


VOID UpdatePStateAsl(
    EFI_ACPI_DESCRIPTION_HEADER *Table,
    IN OUT ASL_OBJ_INFO *SbScopeObjInfo
)
{
    UINT8 SocketNum;
    
    EFI_STATUS  Status;
    ASL_OBJ_INFO ObjInfo;
    UINT8   *ptr;
    UINTN   OrigPkgLen, NewPkgLen;
    UINT8   PStateNum;
    ALL_PSTATES_PKG_HDR *Pstates;
    UINT8 *pStart = SbScopeObjInfo->DataStart;
    CHAR8 Name[] = "PSS0";
    UINT8 Domain;
    UINT8 HWALValue;
  
    //Initialize HWAL
    Status = GetAslObj(pStart, SbScopeObjInfo->Length, "HWAL", otName, &ObjInfo);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return;

    HWALValue = (UINT8) mSystemConfiguration.ProcessorEistPsdFunc;
    *((UINT8*)ObjInfo.DataStart + 1) = HWALValue;
    
    for (SocketNum = 0; SocketNum < MAX_SOCKET; ++SocketNum) {
        Name[3] = SocketNum + '0';

        gNumPstates[SocketNum] = 0;
        Status = CalcPstates(SocketNum);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return;
        
        Status = GetAslObj(pStart, SbScopeObjInfo->Length, Name, otName, &ObjInfo);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return;

        //Outer package which are all the PSTATES
        Pstates = (ALL_PSTATES_PKG_HDR*)ObjInfo.DataStart;
        OrigPkgLen = GetPackageLen((VOID*)&Pstates->PkgLength, NULL) + 1;

        ASSERT(Pstates->NumElem  == MAX_NUMBER_OF_PSTATES); //Check if number of PSTATES in ASL file matches.
        ASSERT(OrigPkgLen == (sizeof(ALL_PSTATES_PKG_HDR) + MAX_NUMBER_OF_PSTATES * sizeof(PSTATE_PKG)));
        
        //Adjust package elements.
        NewPkgLen = sizeof(ALL_PSTATES_PKG_HDR) + gNumPstates[SocketNum] * sizeof(PSTATE_PKG);
        Status = UpdatePkgLen((UINT8*)&Pstates->PkgLength, (UINT32)NewPkgLen-1);  
        ASSERT_EFI_ERROR(Status);

        //Inner packages which are the individual PSTATE.
        for (PStateNum = 0; PStateNum < gNumPstates[SocketNum]; ++PStateNum) { //gNumPstates
            ((PSTATE_PKG*)(Pstates + 1))[PStateNum].Pstate = gPstates[PStateNum][SocketNum];
        }

        //Remove extra table entries.
        if (gNumPstates[SocketNum] < MAX_NUMBER_OF_PSTATES) {
            UINTN LenDif = (OrigPkgLen - NewPkgLen);

            Pstates->NumElem = gNumPstates[SocketNum];
            
            ptr = (UINT8*)&((PSTATE_PKG*)(Pstates + 1))[PStateNum];

            //Remove extra entries by moving the rest of DSDT up.
            MemCpy(ptr, ptr + LenDif, Table->Length - (ptr + LenDif - (UINT8*)Table));
            //Update DSDT table length.
            Table->Length -= (UINT32) LenDif;

            //Update the _SB Scope size.
            SbScopeObjInfo->Length -= LenDif;
            Status = UpdatePkgLen((UINT8*)SbScopeObjInfo->Object + 1, (UINT32)SbScopeObjInfo->Length); 
            ASSERT_EFI_ERROR(Status);
        }
    }

    Domain = 0;
    while (TRUE) {
        ASL_OBJ_INFO PsdObjInfo;
        PSD_PKG      *PsdPkg;
    
        if (Domain == (MAX_SOCKET * MAX_CORE * MAX_THREAD)) break;
        
        // Set Domain;
        Status = GetAslObj(pStart, SbScopeObjInfo->Length, "PSDC", otName, &PsdObjInfo);
        if (EFI_ERROR(Status)) break;

        PsdPkg = (PSD_PKG*) PsdObjInfo.DataStart;
        ASSERT (PsdPkg->BytePrefix3 == AML_BYTE_PREFIX);
        PsdPkg->Domain = Domain;
        
        Status = GetAslObj(pStart, SbScopeObjInfo->Length, "PSDD", otName, &PsdObjInfo);
        if (EFI_ERROR(Status)) break;

        PsdPkg = (PSD_PKG*) PsdObjInfo.DataStart;
        ASSERT (PsdPkg->BytePrefix3 == AML_BYTE_PREFIX);
        PsdPkg->Domain = Domain;

        Status = GetAslObj(pStart, SbScopeObjInfo->Length, "PSDE", otName, &PsdObjInfo);
        if (EFI_ERROR(Status)) break;
        
        PsdPkg = (PSD_PKG*) PsdObjInfo.DataStart;
        ASSERT (PsdPkg->BytePrefix3 == AML_BYTE_PREFIX);
        PsdPkg->Domain = Domain;

        ++Domain;
        pStart = PsdObjInfo.DataStart;  //Start after last PSDE to find next PSDC.
    }
}


VOID UpdateTStateAsl(
    EFI_ACPI_DESCRIPTION_HEADER *Table,
    IN OUT ASL_OBJ_INFO *SbScopeObjInfo
)
{
    EFI_STATUS Status;
    UINT32 Index;
    UINT32 SocketNum;
    UINT32 Lpf;
    UINT32 TstatePwr;
    CHAR8 Name[] = "TSS0";
    ALL_TSTATES_PKG_HDR *Tstates;
    ALL_TSSA_PKG_HDR *TssaPkgHdr;
    TSTATE_PKG *TstatePkg;
    ASL_OBJ_INFO ObjInfo;
    
    for (SocketNum = 0; SocketNum < MAX_SOCKET; ++SocketNum) {
        if (gNumPstates[SocketNum] == 0) continue;
        Lpf = gPstates[gNumPstates[SocketNum] - 1][SocketNum].Power;    //Power of lowest P-state.

        Status = GetAslObj(SbScopeObjInfo->DataStart, SbScopeObjInfo->Length, "TSSA", otName, &ObjInfo);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return;

        TssaPkgHdr = (ALL_TSSA_PKG_HDR*)ObjInfo.DataStart;
        ASSERT(TssaPkgHdr->NumElem  == 1); //Check if number of TSTATES in ASL file matches.
        TstatePkg =(TSTATE_PKG*)(TssaPkgHdr + 1);
        TstatePwr = (62 * 14 + 125) * Lpf / 1000;   //To match formula below for all P-states.
        
        ASSERT(TstatePkg[0].DwordPrefix2 == AML_DWORD_PREFIX);
        TstatePkg[0].Power = TstatePwr;

        Name[3] = SocketNum + '0';
        Status = GetAslObj(SbScopeObjInfo->DataStart, SbScopeObjInfo->Length, Name, otName, &ObjInfo);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return;

        //Outer package which are all the TSTATES
        Tstates = (ALL_TSTATES_PKG_HDR*)ObjInfo.DataStart;
        ASSERT(Tstates->NumElem  == MAX_NUMBER_OF_TSTATES); //Check if number of TSTATES in ASL file matches.
        
        TstatePkg =(TSTATE_PKG*)(Tstates + 1);

        for (Index = 0; Index < MAX_NUMBER_OF_TSTATES; ++Index) {
            //T-state power is ratio of LPF, each T-state is 6.25% less. 
            UINT32 TstatePwr = (62 * (14 - Index) + 125) * Lpf / 1000;
            
            ASSERT(TstatePkg[Index].DwordPrefix2 == AML_DWORD_PREFIX);
            TstatePkg[Index].Power = TstatePwr;
        }
    }
}

VOID NoEistRenamePstateMethods(IN OUT ASL_OBJ_INFO *SbScopeObjInfo)
{
    UINT32  CpuNo = 0;
    UINT8   *pStart = SbScopeObjInfo->DataStart;
    EFI_STATUS Status;
    
    //P-states not supported, so change system names not to report any P-states.
    
    while (CpuNo < (MAX_SOCKET * MAX_CORE * MAX_THREAD)) {
        ASL_OBJ_INFO ObjInfo;
    
        Status = GetAslObj(pStart, SbScopeObjInfo->Length, "_PPC", otMethod, &ObjInfo);
        if (EFI_ERROR(Status)) break;

        *(UINT32*)ObjInfo.ObjName = 'CPPX';  //XPPC
        pStart = ObjInfo.DataStart;

        Status = GetAslObj(pStart, SbScopeObjInfo->Length, "_PSD", otMethod, &ObjInfo);
        if (EFI_ERROR(Status)) break;

        *(UINT32*)ObjInfo.ObjName = 'DSPX';  //XPSD
        pStart = ObjInfo.DataStart;

        Status = GetAslObj(pStart, SbScopeObjInfo->Length, "_PSS", otMethod, &ObjInfo);
        if (EFI_ERROR(Status)) break;

        
        *(UINT32*)ObjInfo.ObjName = 'SSPX';  //XPSS
        pStart = ObjInfo.DataStart;

        Status = GetAslObj(pStart, SbScopeObjInfo->Length, "_PCT", otName, &ObjInfo);
        if (EFI_ERROR(Status)) break;

        *(UINT32*)ObjInfo.ObjName = 'TCPX';  //XPCT
        pStart = ObjInfo.DataStart;

        ++CpuNo;
    }
}
    
    




VOID AmiUpdatePowerManagementSsdt(EFI_ACPI_DESCRIPTION_HEADER *Table)
{
    //Update SSDT based on setup questions and processor support.

    EFI_STATUS Status;
    ASL_OBJ_INFO SbScopeObjInfo;
    UINT8       *ptr;
    UINT32      Length;
    
    ptr = (UINT8*)(Table + 1);  //Set ptr to find scope after table.
    Length = Table->Length - sizeof(EFI_ACPI_DESCRIPTION_HEADER);
    
    Status =  GetAslObj(ptr, Length, "_SB", otScope, &SbScopeObjInfo);
    if (EFI_ERROR(Status)) return;

    UpdateCStateAsl(Table, &SbScopeObjInfo);

    if (mSystemConfiguration.ProcessorEistEnable) {
        UpdatePStateAsl(Table, &SbScopeObjInfo);
   
        //UpdateTStateAsl must be called after UpdatePStateAsl because of Pstate table must be created.
        UpdateTStateAsl(Table, &SbScopeObjInfo);
    } else NoEistRenamePstateMethods(&SbScopeObjInfo);
}
