#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/MpService.h>
#include <Protocol/AmiCpuInfo.h>
#include <Protocol/AmiCpuInfo2.h>

#define MSR_CPU_INFO_IA32_APIC_BASE 	  0x1b
#define MSR_CPU_INFO_CORE_THREAD_COUNT 	  0x35
#define MSR_CPU_INFO_IA32_BIOS_SIGN_ID 	  0x8b
#define MSR_CPU_INFO_IA32_PERF_CTRL 	  0x199

typedef struct {
    UINT32 ApicId;
    UINT32 ApicVer;
    UINT32 MaxSupportedCores;
    UINT32 MaxSupportedThreads;
} CPU_DXE_INT_THREAD_DATA;

extern EFI_MP_SERVICES_PROTOCOL *gMpServices;
extern CPU_DXE_INT_THREAD_DATA *gCpuDxeIntThreadData;
extern UINT32 *Apics;
extern AMI_CPU_INFO *gAmiCpuInfoData;
extern UINT32       gNumCpus;
extern UINT32       gBsp;
extern AMI_CPU_INFO_PROTOCOL gAmiCpuInfo;
extern AMI_CPU_INFO_2_PROTOCOL gAmiCpuInfo2;


UINT32 GetCpuIndex(UINT32 ApicId);
VOID CreateAmiAcpiInfoProtocolData();
VOID FillAmiCpuInfoProtocolData(IN OUT VOID *);
VOID CreateAmiAcpiInfo2ProtocolData();
UINT32  NumberOfCpuSocketsPopulated();
UINT32  NumberOfCpuSocketsPopulated();
UINT64 CalculateTimerPeriod();
BOOLEAN IsHt0();
UINT8 NumSupportedCpuCores();
UINT8 NumSupportedThreadsPerCore();
BOOLEAN IsHtEnabled();

