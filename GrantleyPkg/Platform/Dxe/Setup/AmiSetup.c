//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

#include <SetupPlatform.h>

VOID AmiPowerMgtCallback(IN OUT EFI_IFR_TYPE_VALUE *Value)
{
    BOOLEAN Status;
    SYSTEM_CONFIGURATION SystemConfiguration;
    UINTN SysConfSize = sizeof(SYSTEM_CONFIGURATION);
    
    if(Value->u8 > 1) return;    //Custom -- don't change values

    Status = HiiGetBrowserData (&gEfiSetupVariableGuid, L"IntelSetup", SysConfSize, (UINT8*)&SystemConfiguration);
    ASSERT(Status);
    if (!Status) return;
    
    if(Value->u8 == 0) {  //Disabled
        SystemConfiguration.ProcessorEistEnable = 0;
        SystemConfiguration.TurboMode = 0;
        SystemConfiguration.ProcessorEistPsdFunc = 0;
        //?EnergyEfficientPState
        SystemConfiguration.BootPState = 0;     //Set to max boot -- Question is suppressed.
        SystemConfiguration.PackageCState = 0;
        SystemConfiguration.C3Enable = 0;
        SystemConfiguration.C6Enable = 0;
        SystemConfiguration.TStateEnable = 0;
    } else {    //Enabled
        SystemConfiguration.ProcessorEistEnable = 1;
        SystemConfiguration.TurboMode = 1;
        SystemConfiguration.ProcessorEistPsdFunc = 0;
        //?EnergyEfficientPState
        SystemConfiguration.BootPState = 0;     //Set to max boot -- Question is suppressed.
        SystemConfiguration.PackageCState = 3;  //C6 retention
        SystemConfiguration.C3Enable = 0;
        SystemConfiguration.C6Enable = 1;
        SystemConfiguration.TStateEnable = 1;
    }

    Status = HiiSetBrowserData(&gEfiSetupVariableGuid, L"IntelSetup", SysConfSize, (UINT8*)&SystemConfiguration, NULL);
    ASSERT(Status);
}

VOID UpdateNumCoresSetupQuestion(EFI_HII_PACKAGE_HEADER *IfrPackagePtr)
{
    UINT8 *EndPtr = (UINT8*)IfrPackagePtr + IfrPackagePtr->Length;
    EFI_IFR_OP_HEADER *IfrOpHdr = (EFI_IFR_OP_HEADER *)(IfrPackagePtr + 1);

    UINT32 Eax, Ebx, Ecx, Edx;
    UINT8  CoreThreadCount;
    UINT8  TotalThreadCount;
    UINT8  CoreCount;
    
    ASSERT(IfrOpHdr->OpCode == EFI_IFR_FORM_SET_OP);
    ASSERT(CompareGuid((GUID *)&((EFI_IFR_FORM_SET *) IfrOpHdr)->Guid, &mSystemConfigurationGuid) == TRUE);
    
    //Platform requirement is all physical CPUs must have the same max core count.
    AsmCpuidEx(0xb, 1, &Eax, &Ebx, &Ecx, &Edx);
    TotalThreadCount = (UINT8) Ebx;
        
    AsmCpuidEx(0xb, 0, &Eax, &Ebx, &Ecx, &Edx);
    CoreThreadCount = (UINT8)Ebx;
    CoreCount = (UINT8) (TotalThreadCount / CoreThreadCount);
    
    IfrOpHdr = (EFI_IFR_OP_HEADER *)((UINT8*)IfrOpHdr + IfrOpHdr->Length);
    
    for ( ;(UINT8*)IfrOpHdr < EndPtr; IfrOpHdr = (EFI_IFR_OP_HEADER *)((UINT8*)IfrOpHdr + IfrOpHdr->Length)) {
        EFI_IFR_NUMERIC *NumericOpHdr;
        if (IfrOpHdr->OpCode != EFI_IFR_NUMERIC_OP) continue;

        NumericOpHdr = (EFI_IFR_NUMERIC*)IfrOpHdr;

        //0x2850 is for socket 0 to 0x2851 for socket 1.
        if (NumericOpHdr->Question.QuestionId < 0x2850 || NumericOpHdr->Question.QuestionId > (0x2850 + NUMBER_CPU_SOCKETS - 1)) continue;
        
        ASSERT((NumericOpHdr->Flags & EFI_IFR_NUMERIC_SIZE) == EFI_IFR_NUMERIC_SIZE_1);  //Code assumes byte.
        NumericOpHdr->data.u8.MaxValue = CoreCount;

        if (NumericOpHdr->Question.QuestionId - 0x2850 == (NUMBER_CPU_SOCKETS - 1)) break;  //All questions fixed up?
    }

    ASSERT((UINT8*)IfrOpHdr < EndPtr);  //Assert if question not found.
}

#define MAX_STR_STR_CORE_ENABLE_HELP_SIZE 70

VOID SetNumCoresSetupQuestionHelpStr()
{
    UINT32 Eax, Ebx, Ecx, Edx;
    UINT8  CoreThreadCount;
    UINT8  TotalThreadCount;
    UINT8  CoreCount;
    CHAR16 NewString[MAX_STR_STR_CORE_ENABLE_HELP_SIZE];
    
    //Platform requirement is all physical CPUs must have the same max core count.
    AsmCpuidEx(0xb, 1, &Eax, &Ebx, &Ecx, &Edx);
    TotalThreadCount = (UINT8) Ebx;
        
    AsmCpuidEx(0xb, 0, &Eax, &Ebx, &Ecx, &Edx);
    CoreThreadCount = (UINT8)Ebx;
    CoreCount = (UINT8) (TotalThreadCount / CoreThreadCount);
            
    UnicodeSPrint(NewString, sizeof(CHAR16) * MAX_STR_STR_CORE_ENABLE_HELP_SIZE, L"Number of Cores to Enable. 0 means all cores. %d Cores available.",CoreCount);
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_CORE_ENABLE_HELP), NewString, NULL);
}


//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
