#include "AmiCpuInfo.h"

EFI_GUID gAmiCpuInfoProtocolGuid = AMI_CPU_INFO_PROTOCOL_GUID;
EFI_GUID gAmiCpuInfo2ProtocolGuid = AMI_CPU_INFO_2_PROTOCOL_GUID;

EFI_MP_SERVICES_PROTOCOL *gMpServices;

UINT32 *gApics;
UINT32 gNumCpus;

UINT32 GetCpuIndex(UINT32 ApicId) {
	UINT32 i;
	for (i = 0; i < gNumCpus; ++i) {
		if (gApics[i] == ApicId) return i; 
	}
	
	ASSERT(FALSE);
	return 0;
}

EFI_STATUS EntryPoint(
        IN EFI_HANDLE           ImageHandle,
        IN EFI_SYSTEM_TABLE   *SystemTable
)
{
    EFI_STATUS Status;
    UINTN NumberOfEnabledProcessors;
    UINTN NumberOfProcessors;
    EFI_PROCESSOR_INFORMATION EfiProcessorInformation;
    UINT32 i;
    
    Status = gBS->LocateProtocol(&gEfiMpServiceProtocolGuid, NULL, &gMpServices);
    ASSERT_EFI_ERROR(Status);
    
    gMpServices->GetNumberOfProcessors(gMpServices, &NumberOfProcessors, &NumberOfEnabledProcessors);
    gNumCpus = (UINT32)NumberOfProcessors;
    
    gCpuDxeIntThreadData = AllocatePool(gNumCpus * sizeof(CPU_DXE_INT_THREAD_DATA));
    ZeroMem(gCpuDxeIntThreadData, gNumCpus * sizeof(CPU_DXE_INT_THREAD_DATA));
    
    gAmiCpuInfoData = AllocatePool(sizeof(AMI_CPU_INFO) * gNumCpus);

    gApics = AllocatePool(sizeof(UINT32) * gNumCpus);

    gMpServices->GetNumberOfProcessors(gMpServices, &NumberOfProcessors, &NumberOfEnabledProcessors);
    
    for (i = 0; i < NumberOfProcessors; ++i) {
        Status = gMpServices->GetProcessorInfo(gMpServices,
                i,
                &EfiProcessorInformation);
        ASSERT_EFI_ERROR(Status);
        
        gApics[i] = (UINT32)EfiProcessorInformation.ProcessorId;
        ASSERT(i == 0 || gApics[i] > gApics[i-1]);	//It assumed that the APICs are from lowest to highest.
        
    }

    CreateAmiAcpiInfoProtocolData(); //This must be before CreateAmiAcpiInfo2ProtocolData.
    
    CreateAmiAcpiInfo2ProtocolData();

	Status = gBS->InstallMultipleProtocolInterfaces(
		&ImageHandle,
		&gAmiCpuInfoProtocolGuid, &gAmiCpuInfo,
		&gAmiCpuInfo2ProtocolGuid, &gAmiCpuInfo2,
		NULL);
	ASSERT_EFI_ERROR(Status);
    
	return EFI_SUCCESS;
    
}

