//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file bbs.h
    Header file for bbs/legacy boot management.

**/
//**********************************************************************

#ifndef _BBS_H_
#define _BBS_H_

#ifndef MAX_DRIVE_NAME
#define MAX_DRIVE_NAME TSE_MAX_DRIVE_NAME 
#endif //MAX_DRIVE_NAME

typedef struct
{
	UINT16	HardDiskCount;
	UINT16	CDROMCount;
	UINT16	FloppyCount;
	UINT16	NetworkCount;
	UINT16	BevCount;
}
BBS_DEVICE_COUNT;

typedef struct
{
	UINT32	Type;
	UINT16	Length;
	UINT16	Data[1];
}
BBS_ORDER_TABLE;

#define BBS_ORDER_DISABLE_MASK  0xFF00

typedef struct _BBS_ORDER_LIST
{
    UINT16	Index;
    CHAR16	Name[MAX_DRIVE_NAME];
}
BBS_ORDER_LIST;

//exported variables
extern EFI_GUID gLegacyDevGuid;

//exported functions
VOID BBSGetDeviceList( VOID );
VOID BBSSetBootPriorities( BOOT_DATA *pBootData, UINT16 *pOrder, UINTN u16OrderCount);
EFI_STATUS BBSSetBootNowPriority( BOOT_DATA *BootData,UINTN uiPrefferedDevice,BOOLEAN ShowAllBbsDev);
BOOLEAN BBSValidDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevicePath );
EFI_STATUS BBSLaunchDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevicePath );

VOID BBSUpdateBootData(UINT16 GroupNo);
CHAR16 *BBSGetOptionName( UINT16 *Option, UINT16 Pos  );
//VOID BBSUpdateOrder(UINT16 newOption,UINT32 *offset,UINTN *size, VOID **buffer);
VOID BBSSetDisabled(UINT16 Index, VOID **DisDPs, UINTN *DPSize);

#endif /* _BBS_H_ */
