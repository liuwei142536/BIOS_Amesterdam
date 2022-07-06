//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1987-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//**********************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//**********************************************************************
// Revision History
// ----------------
// $Log: $
//**********************************************************************

//<AMI_FHDR_START>
//---------------------------------------------------------------------------
// Name:        CpuPeiDummy.c
//
// Description:
//  This file is the main CPU PEI component file. This component utilizes
//   CPU I/O & PCI CFG PPI to publish early CPU Init PPI which can be used
//   by NB PEI to load itself.  Also this file contains a CPU init routine
//   to be executed in permanent memory present environment. This is handled
//   by issuing a notifyPPI on permanent memory PPI.
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>

#include "AmiCspLibInc.h"
#include <AmiPeiLib.h>
#include <CpuHobs.h>
#include <token.h>
#include <Ppi\ReadOnlyVariable2.h>
#include "CpuPeiDummy.h"
//#include "cpupei.h"
EFI_GUID gAmiCpuinfoHobGuid             = AMI_CPUINFO_HOB_GUID;
//EFI_GUID gSmmHobGuid                    = SMM_HOB_GUID;

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: CreateCpuHobWithDefaults
//
// Description: Create CPU Hob and fill in default data.
//
// Input:
//      IN EFI_PEI_SERVICES **PeiServices
//      IN UINT8            NumCpus
//
// Output:
//      CPUINFO_HOB *
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

CPUINFO_HOB * CreateCpuHobWithDefaults(EFI_PEI_SERVICES **PeiServices, UINT8 NumCpus)
{
    CPUINFO_HOB *CpuinfoHob;
    EFI_STATUS  Status;
    UINT32      RegEax;
    UINT32      RegEbx;
    UINT32      RegEcx;
    UINT32      RegEdx;
    UINT8       i;

    //Create hob for storing Cpu Data
    Status = (**PeiServices).CreateHob(PeiServices,
        EFI_HOB_TYPE_GUID_EXTENSION,
        //NOTE: sizeof(CPUINFO_HOB) already includes size of one CPUINFO structure
        sizeof(CPUINFO_HOB) + (NumCpus - 1) * sizeof(CPUINFO),
        &CpuinfoHob
    );
    ASSERT_PEI_ERROR(PeiServices, Status);

    CpuinfoHob->EfiHobGuidType.Name = gAmiCpuinfoHobGuid;
    CpuinfoHob->CpuCount = NumCpus;
    CpuinfoHob->NodeCount = NUMBER_CPU_SOCKETS;
    CpuinfoHob->CacheLineSize = 64;

    for(i = 0; i < NumCpus; ++i) {
        CpuinfoHob->Cpuinfo[i].Valid    = FALSE;
        CpuinfoHob->Cpuinfo[i].Disabled = FALSE;
    }
//Save BSP features to CpuinfoHob
    CPULib_CpuID(0x01, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    CpuinfoHob->CpuFeatures.FeatureEcx = RegEcx;
    CpuinfoHob->CpuFeatures.FeatureEdx = RegEdx;

    CPULib_CpuID(0x80000001, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    CpuinfoHob->CpuFeatures.ExtFeatureEax = RegEax;
    CpuinfoHob->CpuFeatures.ExtFeatureEbx = RegEbx;
    CpuinfoHob->CpuFeatures.ExtFeatureEcx = RegEcx;
    CpuinfoHob->CpuFeatures.ExtFeatureEdx = RegEdx;
    
    return CpuinfoHob;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure: UpdateOrCreateCpuHob
//
// Description: Finds or Create Cpu Hob and initialize it.
//
// Input:
//      IN EFI_PEI_SERVICES **PeiServices
//
// Output:
//      VOID
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID UpdateOrCreateCpuHob(EFI_PEI_SERVICES **PeiServices) 
{
    VOID        *FirstHob;
    EFI_HOB_CPU *CpuHob;
    UINT32      RegEax;
    UINT32      RegEbx;
    UINT32      RegEcx;
    UINT32      RegEdx;
    EFI_STATUS  Status;

    (*PeiServices)->GetHobList(PeiServices, &FirstHob);
    if (!FirstHob) ASSERT_PEI_ERROR(PeiServices, EFI_NOT_FOUND);

    CpuHob = (EFI_HOB_CPU*) FirstHob;
    Status = FindNextHobByType(EFI_HOB_TYPE_CPU, &CpuHob);
    if (EFI_ERROR(Status)) {
        Status = (**PeiServices).CreateHob(PeiServices,
            EFI_HOB_TYPE_CPU,
            sizeof(CpuHob),
            &CpuHob
        );
        ASSERT_PEI_ERROR(PeiServices, Status);

        CpuHob->SizeOfMemorySpace = 0xff;
        MemSet(CpuHob->Reserved, 6, 0);
    }

    CPULib_CpuID(0x80000008, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    RegEax &= 0xff;
    if (RegEax < CpuHob->SizeOfMemorySpace) CpuHob->SizeOfMemorySpace = RegEax;
    CpuHob->SizeOfIoSpace = 16; 
}
   
//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure: CpuPeiEntry
//
// Description: AMI CPU PEI driver entry
//
// Input:
//      IN EFI_FFS_FILE_HEADER      *FfsHeader
//      IN EFI_PEI_SERVICES         **PeiServices
//
// Output:
//      EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CpuPeiDummyEntry(
    IN       EFI_PEI_FILE_HANDLE   FileHandle,
    IN CONST EFI_PEI_SERVICES **PeiServices
)
{
    EFI_PEI_READ_ONLY_VARIABLE2_PPI  *ReadOnlyVariable;
//    EFI_BOOT_MODE       BootMode;
    EFI_STATUS          Status;
    CPUINFO_HOB         *CpuinfoHob;
    UINT32              NumCpus;
//    UINT32      Tseg = (UINT32)NBGetTsegBase();
//    SMM_HOB     *SmmHob;
    
    PEI_PROGRESS_CODE(PeiServices, PEI_CPU_INIT);

    Status = (*PeiServices)->LocatePpi(
        PeiServices, &gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, &ReadOnlyVariable       
    );
    ASSERT_PEI_ERROR(PeiServices, Status);

//    Status = (*PeiServices)->GetBootMode(PeiServices, &BootMode);
//    ASSERT_PEI_ERROR(PeiServices, Status);

    UpdateOrCreateCpuHob(PeiServices);

    //After initialized, APs are in holding loop until halted.
    NumCpus = (UINT32)((UINT8)ReadMsr(MSR_CORE_THREAD_COUNT));
	
    PEI_TRACE((-1, PeiServices, "Cpu Pei - number of Cpus %x\n",NumCpus ));
    CpuinfoHob = CreateCpuHobWithDefaults(PeiServices, NumCpus);

    CpuinfoHob->BspNo = 0;
    CpuinfoHob->Cpuinfo->Valid = 1;
    CpuinfoHob->Cpuinfo->ApicId = 0;
    CpuinfoHob->Cpuinfo->ApicEId = 0;
    CpuinfoHob->Cpuinfo->ApicVer = 0;
    CpuinfoHob->Cpuinfo->EistEnable = 0;

// Commented for now as we are not using SMM_HOB
/*    if (BootMode != BOOT_ON_S3_RESUME){
        //Create Smm Hob
        Status = (*PeiServices)->CreateHob(
            PeiServices,
            EFI_HOB_TYPE_GUID_EXTENSION,
            sizeof(SMM_HOB)+ (NumCpus - 1) * sizeof(VOID*),
            &SmmHob
        );
        ASSERT_PEI_ERROR(PeiServices, Status);
        MemCpy(&SmmHob->EfiHobGuidType.Name, &gSmmHobGuid, sizeof(EFI_GUID));

        SmmHob->Tseg = Tseg;
        SmmHob->TsegLength = TSEG_SIZE;
        PEI_ASSERT(PeiServices, (SmmHob->Tseg & 0x7fffff)==0);      //Must be 8MB boundary.
        PEI_ASSERT(PeiServices, TSEG_SIZE >= 0x800000);             //Must be >= 8MB.        

        SmmHob->IedStart = Tseg + TSEG_SIZE - IED_SIZE;
        SmmHob->IedSize = IED_SIZE;

        PEI_ASSERT(PeiServices, (SmmHob->IedStart & 0x3fffff)==0);  //Must be 4MB boundary.
      
        SmmHob->NumCpus = NumCpus;
        SmmHob->Bsp = 0;
    }*/
    
    return EFI_SUCCESS;

}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1987-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

