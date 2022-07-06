//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
#ifndef __AMI_HII_UPDATE_LIB__H__
#define __AMI_HII_UPDATE_LIB__H__
#include <Efi.h>
#include <UefiHii.h>

#ifndef  AMITSE_SETUP_ENTER_GUID
#define AMITSE_SETUP_ENTER_GUID \
  { 0x71202EEE, 0x5F53, 0x40d9, 0xAB, 0x3D, 0x9E, 0x0C, 0x26, 0xD9, 0x66, 0x57 }
#endif

UINT8* AmiHiiCreateSuppresGrayIdVal(
  IN VOID               *OpCodeHandle,
  IN EFI_QUESTION_ID    QuestionId,
  UINT16                Value,
  BOOLEAN               Suppress    //if TRUE Suppress; False Gray out.
  );

UINT8* AmiHiiTerminateScope(IN VOID *OpCodeHandle);

#endif //__AMI_HII_UPDATE_LIB__H__

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

