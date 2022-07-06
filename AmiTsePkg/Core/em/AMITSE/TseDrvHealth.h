//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2015, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Archive: /Alaska/BIN/Modules/AMITSE2_0/AMITSE/TseDrvHealth.h $
//
// $Author: Arunsb $
//
// $Revision: 4 $
//
// $Date: 5/29/12 3:19a $
//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
/** @file TseDrvHealth.h
    Header file for driver health only contains TSE specific.

**/
         
#include "Protocol/DriverHealth.h"
#include "Protocol/DevicePathToText.h"



typedef struct DRV_HEALTH_HNDLS{
	EFI_HANDLE 	ControllerHandle;
	EFI_HANDLE 	ChildHandle;
	EFI_DRIVER_HEALTH_STATUS	HealthStatus;
	struct DRV_HEALTH_HNDLS 	*Next;
}DRV_HEALTH_HNDLS;

typedef struct {
	EFI_HII_HANDLE HiiHandle;
	EFI_STRING_ID StringId;
	UINT64 MessageCode;
} TSE_EFI_DRIVER_HEALTH_HII_MESSAGE;

CHAR16 *GetDrvHlthCtrlName (DRV_HEALTH_HNDLS *);
VOID AddHandleIntoList (EFI_HANDLE, EFI_HANDLE, EFI_DRIVER_HEALTH_STATUS);
VOID DisplayMsgListMessageBox (TSE_EFI_DRIVER_HEALTH_HII_MESSAGE *MessageList);
BOOLEAN AdvancedRepairSupported (VOID);

//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2015, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
