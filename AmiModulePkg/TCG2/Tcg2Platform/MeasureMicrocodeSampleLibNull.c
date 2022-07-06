//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
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
//<AMI_FHDR_START>
//
// Name:    PspfTpmLib.c
//
// Description: Null library for use with the PspfTpmLib
//
//<AMI_FHDR_END>
//**********************************************************************
#include <Uefi.h>

/**
  TpmGetMicrocodePtrPortingFunc: Porting Function to Get Microcode for 
  measurement

  @param[in]   UINTN McuLength,
               UINT8 *PtrToMicrocode 

  @return EFI_SUCCESS   on success
  @return Other         
**/
EFI_STATUS TpmGetMicrocodeFunc(UINT64 *MicrocodeAddress, UINTN *McuLength)
{
    return EFI_NOT_FOUND;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
