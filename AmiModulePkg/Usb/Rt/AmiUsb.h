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

/** @file AmiUsb.h
    AMI UsbRt driver definitions

**/

#ifndef _EFI_USB_H
#define _EFI_USB_H


#include "AmiDef.h"
#include "UsbDef.h"
#include <Protocol/DevicePath.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/AmiUsbController.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/BlockIo.h>
#include <Protocol/PciIo.h>
#include <Protocol/SimplePointer.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/ComponentName.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/UsbPolicy.h>
#include <Protocol/Emul6064KbdInput.h>
#include <Protocol/Emul6064MsInput.h>
#include <Protocol/Emul6064Trap.h>
#include <Library/DevicePathLib.h>


EFI_STATUS
EFIAPI
USBDriverEntryPoint(
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

VOID            EFIAPI UsbApiHandler(VOID*);
EFI_STATUS      EFIAPI InstallUsbProtocols(VOID);
EFI_STATUS      EFIAPI UsbInstallHwSmiHandler(HC_STRUC*);

UINT8   EFIAPI ByteReadIO (UINT16);
VOID    EFIAPI ByteWriteIO (UINT16, UINT8);
UINT32  EFIAPI ReadPCIConfig(UINT16, UINT8);
VOID    EFIAPI WordWritePCIConfig(UINT16, UINT8, UINT16);
UINT32  EFIAPI DwordReadMem(UINT32, UINT32);

//
// There are the prototypes of functions used by USB API
//
VOID EFIAPI USBAPI_CheckPresence(URP_STRUC*);                     // API 00h
VOID EFIAPI USBAPI_Start(URP_STRUC*);                             // API 20h
VOID EFIAPI USBAPI_Stop(URP_STRUC*);                              // API 21h
VOID EFIAPI USBAPI_DisableInterrupts(URP_STRUC*);                 // API 22h
VOID EFIAPI USBAPI_EnableInterrupts(URP_STRUC*);                  // API 23h
VOID EFIAPI USBAPI_MoveDataArea(URP_STRUC*);                      // API 24h
VOID EFIAPI USBAPI_GetDeviceInfo(URP_STRUC*);                     // API 25h
VOID EFIAPI USBAPI_CheckDevicePresence(URP_STRUC*);               // API 26h
VOID EFIAPI USBAPI_MassDeviceRequest(URP_STRUC*);                 // API 27h
VOID EFIAPI USBAPI_PowerManageUSB(URP_STRUC*);                    // API 28h
VOID EFIAPI USBAPI_PrepareForOS(URP_STRUC*);                      // API 29h
VOID EFIAPI USBAPI_SecureInterface(URP_STRUC*);                   // API 2Ah
VOID EFIAPI USBAPI_LightenKeyboardLEDs(URP_STRUC*);               // API 2Bh
VOID EFIAPI USBAPI_ChangeOwner(URP_STRUC*);                       // API 2Ch
VOID EFIAPI USBAPI_HC_Proc(URP_STRUC*);                           // API 2Dh
VOID EFIAPI USBAPI_Core_Proc(URP_STRUC*);                         // API 2eh
VOID EFIAPI USBAPI_LightenKeyboardLEDs_Compatible(URP_STRUC*);    // API 2Fh
VOID EFIAPI USBAPI_KbcAccessControl(URP_STRUC*);                  // API 30h
VOID EFIAPI USBAPI_LegacyControl(URP_STRUC*);                     // API 31h
VOID EFIAPI USBAPI_GetDeviceAddress(URP_STRUC*);                  // API 32h
VOID EFIAPI USBAPI_ExtDriverRequest(URP_STRUC*);                  // API 33h
VOID EFIAPI USBAPI_CCIDRequest(URP_STRUC*);                       // API 34h      
VOID EFIAPI USBAPI_UsbStopController(URP_STRUC*);                 // API 35h
VOID EFIAPI USBAPI_HcStartStop(URP_STRUC *Urp);                   // API 36h
VOID EFIAPI UsbApiLockUsbSwSmi(URP_STRUC *Urp);                   // API 37h


//
// There are the prototypes of functions used by USB Mass Storage API
//
VOID EFIAPI USBMassAPIGetDeviceInformation(URP_STRUC*);    // USB Mass API Sub-Func 00h
VOID EFIAPI USBMassAPIGetDeviceGeometry(URP_STRUC*);       // USB Mass API Sub-Func 01h
VOID EFIAPI USBMassAPIResetDevice(URP_STRUC*);             // USB Mass API Sub-Func 02h
VOID EFIAPI USBMassAPIReadDevice(URP_STRUC*);              // USB Mass API Sub-Func 03h
VOID EFIAPI USBMassAPIWriteDevice(URP_STRUC*);             // USB Mass API Sub-Func 04h
VOID EFIAPI USBMassAPIVerifyDevice(URP_STRUC*);            // USB Mass API Sub-Func 05h
VOID EFIAPI USBMassAPIFormatDevice(URP_STRUC*);            // USB Mass API Sub-Func 06h
VOID EFIAPI USBMassAPICommandPassThru(URP_STRUC*);         // USB Mass API Sub-Func 07h
VOID EFIAPI USBMassAPIAssignDriveNumber(URP_STRUC*);       // USB BIOS API function 08h
VOID EFIAPI USBMassAPICheckDevStatus(URP_STRUC*);          // USB BIOS API function 09h
VOID EFIAPI USBMassAPIGetDevStatus(URP_STRUC*);            // USB BIOS API function 0Ah
VOID EFIAPI USBMassAPIGetDeviceParameters(URP_STRUC*);     // USB BIOS API function 0Bh
VOID EFIAPI USBMassAPIEfiReadDevice(URP_STRUC*);           // USB Mass API Sub-Func 0Ch
VOID EFIAPI USBMassAPIEfiWriteDevice(URP_STRUC*);          // USB Mass API Sub-Func 0Dh
VOID EFIAPI USBMassAPIEfiVerifyDevice(URP_STRUC*);         // USB Mass API Sub-Func 0Eh
VOID EFIAPI USBMassAPIGetIdentifyData(URP_STRUC*);         // USB Mass API Sub-Func 0Fh


DEV_INFO*   USBWrap_GetnthDeviceInfoStructure(UINT8);
VOID        USBWrap_GetDeviceCount(URP_STRUC*);
UINT8       USBWrapGetATAErrorCode(UINT32);
UINT16      USBMassRWVCommand(DEV_INFO*, UINT8, VOID*);
UINT8*      USB_GetDescriptor(HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT8, UINT8);
UINT8       UsbSetInterface(HC_STRUC*, DEV_INFO*, UINT8);

UINT32      USB_ReConfigDevice(HC_STRUC*, DEV_INFO*);
UINT32      USB_ReConfigDevice2(HC_STRUC*, DEV_INFO*, CNFG_DESC*, INTRF_DESC*);
DEV_INFO*   UsbAllocDevInfo();
VOID        PrepareForLegacyOs(VOID);
UINT32      USB_ResetAndReconfigDev(HC_STRUC*, DEV_INFO*);
UINT32      USB_DevDriverDisconnect(HC_STRUC*, DEV_INFO*);
VOID        USBKB_LEDOn();

UINT8        USB_StartHostControllers(VOID);
UINT8        USB_StopHostControllers(VOID);
EFI_STATUS   EFIAPI InitializeUsbGlobalData(VOID);
UINT8        UsbHcStart(HC_STRUC*);
UINT8        UsbHcRegisterSmi(VOID); 
UINT8        UsbHcStop(HC_STRUC*);
VOID         EFIAPI FixedDelay(UINTN);
DEV_INFO*    EFIAPI USB_GetDeviceInfoStruc(UINT8, DEV_INFO*, UINT8, HC_STRUC*);
UINT8        USBMassGetDeviceInfo(MASS_GET_DEV_INFO*);
UINT8        USBMassGetDeviceGeometry(MASS_GET_DEV_GEO*);

UINT16       USBMassStartUnitCommand (DEV_INFO*);
UINT8        USBMassCmdPassThru (MASS_CMD_PASS_THRU*);
UINT8        USBMassGetDeviceStatus (MASS_GET_DEV_STATUS*);
UINT32       USBMassCheckDeviceReady(DEV_INFO*);
MASS_INQUIRY *USBMassGetDeviceParameters(DEV_INFO*);
ATA_IDENTIFY_DATA* USBMassGetIdentifyData(DEV_INFO*);
VOID         EFIAPI InitSysKbc(EFI_EMUL6064KBDINPUT_PROTOCOL**, EFI_EMUL6064MSINPUT_PROTOCOL**);
VOID         UsbPeriodicEvent();
DEV_INFO*    USB_DetectNewDevice(HC_STRUC*, UINT8, UINT8, UINT32);
UINT8        EFIAPI UsbHcdStartDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdStopDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdEnumeratePortsDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdDisableInterruptsDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdEnableInterruptsDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdProcessInterruptDummy(HC_STRUC*);
UINT32       EFIAPI UsbHcdGetRootHubStatusDummy(HC_STRUC*, UINT8, BOOLEAN);
UINT8        EFIAPI UsbHcdDisableRootHubDummy(HC_STRUC*, UINT8);
UINT8        EFIAPI UsbHcdEnableRootHubDummy(HC_STRUC*, UINT8);
UINT16       EFIAPI UsbHcdControlTransferDummy(HC_STRUC*, DEV_INFO*, UINT16, UINT16, UINT16, UINT8*, UINT16);
UINT32       EFIAPI UsbHcdBulkTransferDummy(HC_STRUC*, DEV_INFO*, UINT8, UINT8*, UINT32);
UINT32       EFIAPI UsbHcdIsocTransferDummy(HC_STRUC*, DEV_INFO*, UINT8, UINT8*, UINT32, UINT8*);
UINT16       EFIAPI UsbHcdInterruptTransferDummy(HC_STRUC*, DEV_INFO*, UINT8, UINT16, UINT8*, UINT16);
UINT8        EFIAPI UsbHcdDeactivatePollingDummy(HC_STRUC*, DEV_INFO*);
UINT8        EFIAPI UsbHcdActivatePollingDummy(HC_STRUC*, DEV_INFO*);
UINT8        EFIAPI UsbHcdDisableKeyRepeatDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdEnableKeyRepeatDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdEnableEndpointsDummy(HC_STRUC*, DEV_INFO*, UINT8*);
UINT8        EFIAPI UsbHcdInitDeviceDataDummy(HC_STRUC*, DEV_INFO*, UINT32, UINT8**);
UINT8        EFIAPI UsbHcdDeinitDeviceDataDummy(HC_STRUC*, DEV_INFO*);
UINT8        EFIAPI UsbHcdResetRootHubDummy(HC_STRUC*, UINT8);
UINT8        EFIAPI UsbHcdClearEndpointStateDummy(HC_STRUC*, DEV_INFO*, UINT8);
UINT8        EFIAPI UsbHcdGlobalSuspendDummy(HC_STRUC*);
UINT8        EFIAPI UsbHcdSmiControlDummy(HC_STRUC*, BOOLEAN);     
VOID*        USB_MemAlloc(UINT16);
UINT8        USB_MemFree(VOID*, UINT16);
UINT8        USBCheckPortChange (HC_STRUC*, UINT8, UINT8);
VOID         EFIAPI UsbKbcAccessControl(UINT8);
EFI_STATUS   EFIAPI USBRT_LegacyControl (VOID *FpURP);
VOID         EFIAPI USB_StopUnsupportedHc();
UINT8        UsbControlTransfer(HC_STRUC*, DEV_INFO*, DEV_REQ, UINT16, VOID*);
UINT8        UsbInterruptTransfer(HC_STRUC*, DEV_INFO*, UINT8, UINT16, VOID*, UINT16, UINT16);
VOID         CheckBiosOwnedHc(VOID);
DEV_DRIVER*  UsbFindDeviceDriverEntry(UINTN DevDriverIndex);

#endif

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
