//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiIoDecodeLib.h

    Chipset Specific LPC IO Decode library.
**/

#ifndef __AMI_IO_DECODE_LIB_H__
#define __AMI_IO_DECODE_LIB_H__

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Uefi.h>
#include <Protocol/AmiSio.h>

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//Variable, Prototype, and External Declarations
//-------------------------------------------------------------------------

EFI_STATUS EFIAPI AmiSioLibSetLpcDeviceDecoding (
    IN VOID      				*LpcPciIo,
    IN UINT16                   Base,
    IN UINT8                    DevUid,
    IN UINT32                   Type );
EFI_STATUS EFIAPI AmiSioLibSetLpcGenericDecoding (
    IN VOID      				*LpcPciIo,
    IN UINT16                   Base,
    IN UINT16                   Length,
    IN BOOLEAN                  Enable );


#endif
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

