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
/** @file
  Definition of the AmiCriticalSectionLib library class.
*/
#ifndef __AMI_CRITICAL_SECTION_LIB__H__
#define __AMI_CRITICAL_SECTION_LIB__H__

typedef VOID* CRITICAL_SECTION;

EFI_STATUS CreateCriticalSection(OUT CRITICAL_SECTION *);
EFI_STATUS BeginCriticalSection(IN CRITICAL_SECTION);
EFI_STATUS EndCriticalSection(IN CRITICAL_SECTION);
EFI_STATUS DestroyCriticalSection(IN CRITICAL_SECTION);

#define AMI_BEGIN_CRITICAL_SECTION(Cs) {\
    EFI_STATUS __Status__ = BeginCriticalSection(Cs);\
    ASSERT(__Status__==EFI_SUCCESS || __Status__==EFI_ACCESS_DENIED);\
    if (EFI_ERROR(__Status__)) return __Status__;\
}

#define AMI_END_CRITICAL_SECTION(Cs) {\
    EFI_STATUS __Status__ = EndCriticalSection(Cs);\
    ASSERT_EFI_ERROR(__Status__);\
}

#endif
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
