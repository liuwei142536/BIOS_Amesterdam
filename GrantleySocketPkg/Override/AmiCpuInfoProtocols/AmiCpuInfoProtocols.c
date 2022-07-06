#include <Library/LocalApicLib.h>
#include "AmiCpuInfo.h"


#define MAX_NUM_CACHE_DESC  8


CPU_DXE_INT_THREAD_DATA *gCpuDxeIntThreadData;

static CACHE_DESCRIPTOR_INFO gZeroCacheDesc = {0, 0, 0, 0, 0};

typedef struct {
    UINT32 CacheType:5;
    UINT32 CacheLevel:3;
    UINT32 NotUsed:24;
} CPUID4_EAX_CACHE_INFO;

typedef struct {
    UINT32  LineSize:12;
    UINT32  Partitions:10;
    UINT32  Ways:10;
} CPUID4_EBX_CACHE_INFO;


typedef CHAR8 BRAND_STRING[49];
typedef CACHE_DESCRIPTOR_INFO CPU_CACHE_INFO[MAX_NUM_CACHE_DESC];

AMI_CPU_INFO *gAmiCpuInfoData;
UINT32 gBsp;

VOID FillCacheDesc(CACHE_DESCRIPTOR_INFO * CacheInfo)
{
    CPUID4_EAX_CACHE_INFO EaxInfo;
    CPUID4_EBX_CACHE_INFO EbxInfo;
    UINT32 RegEcx;
    UINT32 RegEdx;
    UINT32 CacheCount;

    for (CacheCount  = 0; CacheCount < (MAX_NUM_CACHE_DESC - 1); ++CacheCount) {
        AsmCpuidEx(4, CacheCount, (UINT32*)&EaxInfo, (UINT32*)&EbxInfo, &RegEcx, &RegEdx);
        if (EaxInfo.CacheType == 0) break; //No more cache.

        CacheInfo[CacheCount].Desc = 0xff;  //Unused.
        CacheInfo[CacheCount].Level = EaxInfo.CacheLevel;
        switch (EaxInfo.CacheType) {
        case 1: CacheInfo[CacheCount].Type = 0; break;
        case 2: CacheInfo[CacheCount].Type = 1; break;
        case 3: CacheInfo[CacheCount].Type = 3; break;
        }

        CacheInfo[CacheCount].Size =
            (EbxInfo.Ways + 1) * (EbxInfo.Partitions + 1) * (EbxInfo.LineSize + 1) * (RegEcx + 1) /
             1024;
        CacheInfo[CacheCount].Associativity = EbxInfo.Ways + 1;
    }
    CacheInfo[CacheCount] = gZeroCacheDesc;
}


VOID FillAmiCpuInfoProtocolData(IN OUT VOID *Buffer)
{
	UINT32 ApicId;
	UINT32 CpuIndex;
    AMI_CPU_INFO    *AmiCpuInfo;
    UINT32      CpuSignature;
    UINT32      FeatureEcx, FeatureEdx;
    UINT32      RegEax, RegEbx, RegEcx, RegEdx;
    UINT32      ExtFeatureEdx;
    CHAR8       *BrandString;
    UINT32      MicroCodeVersion;
    UINT8       CpuCores = (UINT8)((UINT32)AsmReadMsr64(MSR_CPU_INFO_CORE_THREAD_COUNT) >> 16);
    BOOLEAN     Ht0 = IsHt0();   //True, if not hyper-threaded CPU.
    UINT8       *pos1;
    
	ApicId = GetApicId();
	CpuIndex =  GetCpuIndex(ApicId);
    AmiCpuInfo = &gAmiCpuInfoData[CpuIndex];
    BrandString = AmiCpuInfo->BrandString;

    
    AsmCpuid(1, &CpuSignature, NULL,  &FeatureEcx, &FeatureEdx);
    AsmCpuid(0x80000001, NULL, NULL, NULL, &ExtFeatureEdx);

    FillCacheDesc(AmiCpuInfo->CacheInfo);   //Get Cache Information.

    //Get Brand string
    AsmCpuid(0x80000002, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    *(UINT32*)BrandString = RegEax; BrandString +=4;
    *(UINT32*)BrandString = RegEbx; BrandString +=4;
    *(UINT32*)BrandString = RegEcx; BrandString +=4;
    *(UINT32*)BrandString = RegEdx; BrandString +=4;

    AsmCpuid(0x80000003, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    *(UINT32*)BrandString = RegEax; BrandString +=4;
    *(UINT32*)BrandString = RegEbx; BrandString +=4;
    *(UINT32*)BrandString = RegEcx; BrandString +=4;
    *(UINT32*)BrandString = RegEdx; BrandString +=4;

    AsmCpuid(0x80000004, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    *(UINT32*)BrandString = RegEax; BrandString +=4;
    *(UINT32*)BrandString = RegEbx; BrandString +=4;
    *(UINT32*)BrandString = RegEcx; BrandString +=4;
    *(UINT32*)BrandString = RegEdx; BrandString +=4;
    *BrandString = '\0';

    //Remove leading spaces. After removing leading spaces, the Brand String can not be
    //freed. However, it should never be freed.
    while (*AmiCpuInfo->BrandString == ' ') ++AmiCpuInfo->BrandString;

    //Remove extra spaces in middle.
    pos1 = AmiCpuInfo->BrandString;
    for(;;) {
        UINT8 *pos2;
        UINT8 *pos3;
        while (*pos1 != ' ' && *pos1 != '\0') ++pos1;   //Find next space.
        if (*pos1 == '\0') break;                       //If found terminator, break.
        if (*++pos1 != ' ') continue;                   //If not second space, continue scanning.
        pos2 = pos1;                                    //Found 2 spaces.
        while(*++pos2 == ' ');                          //Skip spaces.
        pos3 = pos1;
        while(*pos2 != '\0') *pos3++ = *pos2++;         //copy string
        *pos3++ = '\0';                                 //Add terminator.
    }

    AmiCpuInfo->Version      = CpuSignature;
    AmiCpuInfo->X64Supported = (ExtFeatureEdx >> 29) & 1;
    AmiCpuInfo->Ht0          = Ht0;
    AmiCpuInfo->Features     = LShiftU64(FeatureEcx, 32) + FeatureEdx;
    AmiCpuInfo->NumCores     = CpuCores;
    AmiCpuInfo->NumHts       = IsHtEnabled() * 2;   //Either 2 or 0.

    gCpuDxeIntThreadData[CpuIndex].ApicId = ApicId;
    gCpuDxeIntThreadData[CpuIndex].ApicVer = (UINT8)GetApicVersion();
    
    gCpuDxeIntThreadData[CpuIndex].MaxSupportedCores = NumSupportedCpuCores();
    gCpuDxeIntThreadData[CpuIndex].MaxSupportedThreads =
        gCpuDxeIntThreadData[CpuIndex].MaxSupportedCores * NumSupportedThreadsPerCore();

    
    AmiCpuInfo->FSBFreq = 100;
    //IntendedFreq and ActualFreq are no longer useful because the CPU freq will change
    //late in post after the protocol is created.
    AmiCpuInfo->IntendedFreq = (UINT8)((UINT16)AsmReadMsr64(MSR_CPU_INFO_IA32_PERF_CTRL) >> 8) * AmiCpuInfo->FSBFreq; 
    AmiCpuInfo->ActualFreq      = AmiCpuInfo->IntendedFreq;
    AmiCpuInfo->Voltage         = 0;    //Voltage is variable, and no information os available.
    MicroCodeVersion = (UINT32)RShiftU64(AsmReadMsr64(MSR_CPU_INFO_IA32_BIOS_SIGN_ID), 32);
    AmiCpuInfo->MicroCodeVers   = MicroCodeVersion;

    if ((UINT32)AsmReadMsr64(MSR_CPU_INFO_IA32_APIC_BASE) & BIT8)  gBsp = CpuIndex;
}


VOID CreateAmiAcpiInfoProtocolData()
{
	BRAND_STRING *BrandStrings;
	CPU_CACHE_INFO *CacheDescriptors;
	EFI_STATUS Status;
	UINT32 i;
	
	
	gAmiCpuInfoData = AllocatePool(sizeof(AMI_CPU_INFO) * gNumCpus);

	BrandStrings = AllocatePool(sizeof(BRAND_STRING) * gNumCpus);
	CacheDescriptors = AllocatePool(sizeof(CPU_CACHE_INFO) * gNumCpus);
	
	for (i = 0; i < gNumCpus; ++i) {
		gAmiCpuInfoData[i].BrandString = (CHAR8*)&BrandStrings[i];
		gAmiCpuInfoData[i].CacheInfo = (CACHE_DESCRIPTOR_INFO*)&CacheDescriptors[i];
	}
	
    //Collect Cpu Data for AMI CPU INFO Protocol.
    Status = gMpServices->StartupAllAPs(
    		gMpServices,
    		FillAmiCpuInfoProtocolData,
    		FALSE,
    		NULL,
    		0,
    		NULL,
    		NULL
    );
    ASSERT_EFI_ERROR(Status);

    FillAmiCpuInfoProtocolData(NULL);
    
}


EFI_STATUS GetCpuInfo(
	IN AMI_CPU_INFO_PROTOCOL	*This,
	IN UINTN					Cpu,
	OUT AMI_CPU_INFO			**Info
)
{
	if (Cpu >= gNumCpus) return EFI_INVALID_PARAMETER;
	*Info = &gAmiCpuInfoData[Cpu];
	return EFI_SUCCESS;
}

AMI_CPU_INFO_PROTOCOL gAmiCpuInfo = {GetCpuInfo};

