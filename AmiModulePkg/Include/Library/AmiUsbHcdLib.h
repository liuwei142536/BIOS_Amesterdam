//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AmiUsbHcdLib.h
    Head file for HCD library
**/

#ifndef __AMI_USB_HCD_LIB_H__
#define __AMI_USB_HCD_LIB_H__
#include <Protocol/AmiUsbController.h>
#include <UsbDef.h>
#include <Library/BaseMemoryLib.h>

VOID 
EFIAPI
InitHcdTable(
  IN    HCD_HEADER          *UsbHcdTable,
  IN    VOID                *UsbGlobdaDataAddr,
  IN    VOID                *HcTableAddr,
  IN    HCD_MEM_HEADER      *UsbHcdMemTable
);

UINT8
EFIAPI
AmiUsbControllerStart(
    HC_STRUC* HcStruc
);

UINT8
EFIAPI
AmiUsbControllerStop(
    HC_STRUC*    HcStruc
);

UINT8
EFIAPI
AmiUsbEnumeratePorts(
    HC_STRUC*    HcStruc
);

UINT8
EFIAPI
UsbDisableInterrupts (
    HC_STRUC*   HcStruc
);

UINT8
EFIAPI
AmiUsbEnableInterrupts (
    HC_STRUC* HcStruc
);

UINT8
EFIAPI
AmiUsbProcessInterrupt(
    HC_STRUC*    HcStruc
);

UINT32
EFIAPI
AmiUsbGetRootHubStatus(
    HC_STRUC*   HcStruc,
    UINT8       PortNum,
    BOOLEAN     ClearChangeBits
);

UINT8
EFIAPI
AmiUsbDisableRootHub(
    HC_STRUC*   HcStruc,
    UINT8       PortNum
);

UINT8
EFIAPI
AmiUsbEnableRootHub(
    HC_STRUC*   HcStruc,
    UINT8       PortNum
);

UINT16
EFIAPI
AmiUsbControlTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT16      Request,
    UINT16      Index,
    UINT16      Value,
    UINT8       *Buffer,
    UINT16      Length
);

UINT32
EFIAPI
AmiUsbBulkTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length
);
UINT32
EFIAPI
AmiUsbIsocTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length,
    UINT8       *AsyncIndicator
);

UINT16
EFIAPI
AmiUsbInterruptTransfer (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       EndpointAddress,
    UINT16      MaxPktSize,
    UINT8       *Buffer,
    UINT16      Length
);

UINT8
EFIAPI
AmiUsbDisableKeyRepeat (
    HC_STRUC    *HcStruc
);

UINT8
EFIAPI
AmiUsbEnableKeyRepeat (
    HC_STRUC*    HcStruc
);

UINT8
EFIAPI
AmiUsbEnableEndpoints (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Desc
);

UINT8
EFIAPI
AmiUsbActivatePolling(
    HC_STRUC* HcStruc,
    DEV_INFO* DevInfo
);

UINT8
EFIAPI
AmiUsbDeactivatePolling(
    HC_STRUC*       HcStruc,
    DEV_INFO*       DevInfo
);

UINT8
EFIAPI
AmiUsbInitDeviceData(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT32      PortStatus,
    UINT8       **DeviceData
);

UINT8
EFIAPI
AmiUsbDeinitDeviceData(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo
);

UINT8
EFIAPI
AmiUsbResetRootHub(
    HC_STRUC*    HcStruc,
    UINT8        PortNum
);

UINT8
EFIAPI
AmiUsbClearEndpointState(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       Endpoint
);

UINT8
EFIAPI
AmiUsbGlobalSuspend(
    HC_STRUC*	HcStruc
);

UINT8
EFIAPI
AmiUsbSmiControl(
    HC_STRUC*     HcStruc,
    BOOLEAN       Enable
);

EFI_STATUS
EFIAPI
AmiUsbGlobalDataSwitch(
    USB_GLOBAL_DATA    **UsbGlobdaDataAddr,
    USB_DATA_LIST      **UsbDataListAddr,
    HC_STRUC           **HcTableAddr,
    UINT8              *DataInSmm,
    UINT8              Direction
);

EFI_STATUS
EFIAPI
InitUsbHcdDriver(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
);

EFI_STATUS
EFIAPI
AmiUsbGlobalDataValidation(
    USB_GLOBAL_DATA *UsbData
);

EFI_STATUS 
EFIAPI 
AmiUsbValidateMemoryBuffer(CONST VOID* Buffer, CONST UINTN BufferSize);

EFI_STATUS 
EFIAPI 
AmiUsbValidateMmioBuffer(CONST VOID* Buffer, CONST UINTN BufferSize);

EFI_STATUS
EFIAPI
InterruptCallBack(
    VOID       *IntDevInfo,
    UINT8      *Buffer,
    UINT16     DataLength
);

EFI_STATUS
EFIAPI
InstallSmmHandle(
    HC_STRUC*    HcStruc
);

EFI_STATUS 
EFIAPI 
AmiUsbResetSystem(
    VOID
);

EFI_STATUS
EFIAPI
UhciEnableLegacyReg(
    HC_STRUC*   HcStruc
);

UINT8
EFIAPI
UhciCheckLegacyReg(
    HC_STRUC*   HcStruc
);

EFI_STATUS
EFIAPI
UhciDisableLegacyReg(
    HC_STRUC*   HcStruc
);

EFI_STATUS
EFIAPI
EhciEnableLegacyKeyRepeat(
    HC_STRUC    *HcStruc
);

EFI_STATUS
EFIAPI
OhciEnableControlReg(
    HC_STRUC    *HcStruc
);

EFI_STATUS
EFIAPI
XhciEnableLegacyReg(
    HC_STRUC*            HcStruc
);

EFI_STATUS
EFIAPI
XhciDisableLegacyReg(
    HC_STRUC*            HcStruc
);

EFI_STATUS
EFIAPI
XhciUninstallKeyRepeat(
    HC_STRUC*   HcStruc
);

EFI_STATUS
EFIAPI
GetBosDevCapDesc(
    IN   UINT8   *BosDesc,
    IN   UINT8   CapDescType,
    OUT  UINT8   *DescLength,
    IN   VOID    *Buffer,
    IN   UINT32  BufLength
);

EFI_STATUS
EFIAPI
SettingUsbSmi(
  BOOLEAN    Enable
);

EFI_STATUS
EFIAPI
AmiUsbPerfStart(
    IN VOID   *Handle,
    IN CHAR8  *Token,
    IN CHAR8  *Module,
    IN UINT64 TimeStamp
);

EFI_STATUS
EFIAPI
AmiUsbPerfEnd(
    IN VOID   *Handle,
    IN CHAR8  *Token,
    IN CHAR8  *Module,
    IN UINT64 TimeStamp
);
#endif //__AMI_USB_HCD_LIB_H__

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
