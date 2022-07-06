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
//**********************************************************************

/** @file AmiSioDxeLib.h

    Definitions of the AMI SIO DXE Library.
**/

#ifndef __AMI_SIO_DXE_LIB_H__
#define __AMI_SIO_DXE_LIB_H__

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Efi.h>
#include <AmiGenericSio.h>
#include <AcpiRes.h>

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
typedef struct _IO_DECODE_DATA{
    UINT16          BaseAdd;
    //!!!Attention!!!If type is 0xFF, UID is a IO legth
    UINT8           UID;
    SIO_DEV_TYPE    Type;
} IO_DECODE_DATA;

typedef struct _SIO_DEVICE_INIT_DATA{
    UINT16      Reg16;
    UINT8       AndData8;   // 0x00 means register don't need AndMask
                            // only write OrData8 to regisrer.
    UINT8       OrData8;
} SIO_DEVICE_INIT_DATA;

typedef struct _DXE_DEVICE_INIT_DATA{
    UINT8       Reg8;
    UINT8       AndData8;   // 0x00 means register don't need AndMask
                            // only write OrData8 to regisrer.
    UINT8       OrData8;
} DXE_DEVICE_INIT_DATA;

typedef struct _SIO_DATA{
    UINT16           Addr;
    //AND mask value, 0xFF means register don't need AndMask and 
    //only write OrData8 to regisrer.
    UINT8           DataMask; 
    //OR mask value.  
    UINT8           DataValue;
} SIO_DATA;

// SIO DECODE list creation code must be in this order
typedef EFI_STATUS (IO_RANGE_DECODE)(
    IN  VOID            *LpcPciIo,
    IN  UINT16          DevBase, 
    IN  UINT8           UID, 
    IN  SIO_DEV_TYPE    Type
); 

//-------------------------------------------------------------------------
//Variable, Prototype, and External Declarations
//-------------------------------------------------------------------------

EFI_STATUS EFIAPI GetPrsFromTable(SIO_DEV2* Dev, UINTN Index);
EFI_STRING EFIAPI GetSioLdVarName(SIO_DEV2* SioLd, BOOLEAN Volatile);
EFI_STATUS EFIAPI SioDxeLibSetFdcPrs(SIO_DEV2 *Dev);
EFI_STATUS EFIAPI SioDxeLibSetPs2kPrs(SIO_DEV2 *Dev);
EFI_STATUS EFIAPI SioDxeLibSetPs2mPrs(SIO_DEV2 *Dev);
EFI_STATUS EFIAPI SioDxeLibSetUartPrs(SIO_DEV2 *Dev);
EFI_STATUS EFIAPI SioDxeLibSetLptPrs(SIO_DEV2 *Dev, BOOLEAN UseDma);
EFI_STATUS EFIAPI SioDxeLibSetGamePrs(SIO_DEV2 *Dev);
EFI_STATUS EFIAPI SioDxeLibSetMpu401Prs(SIO_DEV2 *Dev);
EFI_STATUS EFIAPI SioDxeLibGetPrsFromAml(SIO_DEV2* Dev, UINT8 *PrsName ,UINTN Index);
EFI_STATUS EFIAPI AmiSioLibSetLpcDeviceDecoding (
    IN VOID      				*LpcPciIo,
    IN UINT16                   Base,
    IN UINT8                    DevUid,
    IN SIO_DEV_TYPE             Type );
EFI_STATUS EFIAPI AmiSioLibSetLpcGenericDecoding (
    IN VOID      				*LpcPciIo,
    IN UINT16                   Base,
    IN UINT16                   Length,
    IN BOOLEAN                  Enable );
EFI_STATUS EFIAPI SioLibLaunchInitRoutine(SIO_DEV2 *Device, SIO_INIT_STEP InitStep);
EFI_STATUS EFIAPI SioLibLaunchResourceRoutine(SIO_DEV2 *Device, SIO_RESOURCE_REG_TYPE RegType);
EFI_STATUS EFIAPI SioLibLaunchCompRoutine(SIO_DEV2 *Device, SIO_INIT_STEP InitStep);

void EFIAPI GetValueWithIOx (
	IN UINT16 IndexP,
	IN UINT16 DataP,
	IN UINT8  BankReg,
    IN UINT8  BankNo,
    IN UINT8  LpcReg,
    OUT UINTN *Value
);

void EFIAPI ProgramIsaRegisterTable(
    IN  UINT16  Index,
    IN  UINT16  Data,
    IN  SIO_DEVICE_INIT_DATA *Table,
    IN  UINT8   Count
);

void EFIAPI ProgramRtRegisterTable(
    IN  UINT16  Base,
    IN  SIO_DEVICE_INIT_DATA  *Table,
    IN  UINT8   Count
);

VOID EFIAPI SioLib_BootScriptIO(
		UINT16 Reg16,
		UINT8 Data8,
		EFI_S3_SAVE_STATE_PROTOCOL *SaveState
		);

VOID EFIAPI SioLib_BootscriptLdnSel(
		UINT16 idxReg,
		UINT16 dataReg,
		UINT8  LdnSel,
		UINT8  Ldn,
		EFI_S3_SAVE_STATE_PROTOCOL *SaveState
		);

VOID EFIAPI SioLib_BootScriptSioS3SaveTable(
		UINT16 idxReg,
		UINT16 dataReg,
		SIO_DEVICE_INIT_DATA  *Table,
		UINT8 Count,
		EFI_S3_SAVE_STATE_PROTOCOL *SaveState
		);

VOID EFIAPI SioLib_BootScriptRTS3SaveTable(
		UINT16 Base,
		SIO_DEVICE_INIT_DATA  *Table,
		UINT8 Count,
		EFI_S3_SAVE_STATE_PROTOCOL *SaveState
		);


#endif //__AMI_SIO_DXE_LIB_H__
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

