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

/** @file Uhcd.h
    AMI USB Host Controller Driver header file

**/

#ifndef _AMIUSB_UHCD_H
#define _AMIUSB_UHCD_H
#include <AmiDef.h>
#include <UsbDef.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Protocol/SimplePointer.h>
#include <Protocol/ComponentName.h>
#include <Protocol/AmiUsbController.h>
#include <Protocol/AmiUsbCcid.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/UsbIo.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <AmiStatusCodes.h>

/* PCI Configuration Registers for USB */
// Class Code Register offset
#define CLASSC                      0x09
// USB IO Space Base Address Register offset
#define USBBASE                     0x20

//
// USB Base Class Code,Sub-Class Code and Programming Interface.
//
#define PCI_CLASSC_BASE_CLASS_SERIAL        0x0c
#define PCI_CLASSC_SUBCLASS_SERIAL_USB      0x03
#define PCI_CLASSC_IFT_USB_DEVICE           0xFE    // Interface type for USB Device
#define PCI_CLASSC_PI_UHCI                  0x00
#define PCI_CLASSC_PI_OHCI                  0x10
#define PCI_CLASSC_PI_EHCI                  0x20
#define PCI_CLASSC_PI_XHCI                  0x30

#define USB_TOKEN_NAME_SIZE                 30
#pragma pack(push, 1)

//
// USB Class Code structure
//
typedef struct
{
  UINT8    PI;
  UINT8    SubClassCode;
  UINT8    BaseCode;
} USB_CLASSC;

#pragma pack(pop)

UINT8 UsbSmiReConfigDevice(HC_STRUC* hc, DEV_INFO* dev);

#define DEBUG_LEVEL_USBBUS          DEBUG_LEVEL_3
#define DEBUG_USBHC_LEVEL           DEBUG_LEVEL_3
#define DEBUG_USBHC_LEVEL8          DEBUG_LEVEL_8

EFI_STATUS usbhc_init(EFI_HANDLE  ImageHandle,EFI_HANDLE  ServiceHandle);

#define ATOMIC(a) {\
        EFI_TPL savetpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);\
        {a;}\
        gBS->RestoreTPL(savetpl);   \
}\

#define CRITICAL_CODE(level, a) {\
        EFI_TPL savetpl;\
        EFI_TPL currenttpl = gBS->RaiseTPL (TPL_HIGH_LEVEL); \
        gBS->RestoreTPL(currenttpl);\
        if(currenttpl<=level)\
            currenttpl=level;\
        savetpl = gBS->RaiseTPL (currenttpl);\
        {a;}\
        gBS->RestoreTPL(savetpl);   \
}\

EFI_STATUS  UsbMsInit(EFI_HANDLE  ImageHandle, EFI_HANDLE  ServiceHandle);
HC_STRUC*   FindHcStruc(EFI_HANDLE Controller);

EFI_STATUS
EFIAPI
AmiUsbDriverEntryPoint(
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable );

EFI_STATUS
EFIAPI
AmiUsbDriverBindingSupported (
    IN EFI_DRIVER_BINDING_PROTOCOL  *This,
    IN EFI_HANDLE                   Controller,
    IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath );

EFI_STATUS
EFIAPI
AmiUsbDriverBindingStart (
    IN EFI_DRIVER_BINDING_PROTOCOL  *This,
    IN EFI_HANDLE                   Controller,
    IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath );

EFI_STATUS
EFIAPI
AmiUsbDriverBindingStop (
    IN EFI_DRIVER_BINDING_PROTOCOL  *This,
    IN EFI_HANDLE                   Controller,
    IN UINTN                        NumberOfChildren,
    IN EFI_HANDLE                   *ChildHandleBuffer );

HC_STRUC*
AddHC (
    EFI_HANDLE                   Controller,
    UINTN                        PciBus,
    UINTN                        PciDev,
    UINTN                        PciFunc,
    UINT8                        HcType,
    UINT8                        Irq,
    EFI_DEVICE_PATH_PROTOCOL     *DevicePatch,
    EFI_PCI_IO_PROTOCOL          *PciIo
);

EFI_STATUS
LocateEhciController(
    IN EFI_DRIVER_BINDING_PROTOCOL  *This,
    IN EFI_HANDLE                   Controller,
    IN EFI_DEVICE_PATH_PROTOCOL     *CompanionDevicePath
);

VOID
EFIAPI
OnLegacyBoot(
    EFI_EVENT   Event,
    VOID        *Context
);

VOID
EFIAPI
ReadyToBootNotify(
    EFI_EVENT   Event, 
    VOID        *Context
);

VOID
EFIAPI
EndOfDxeEventCallback(
    EFI_EVENT   Event, 
    VOID        *Context
);

EFI_STATUS
EFIAPI
UsbGetRuntimeRegion(
    EFI_PHYSICAL_ADDRESS *Start,
    EFI_PHYSICAL_ADDRESS *End
);

EFI_STATUS  EFIAPI Dummy1(USB_MASS_DEV*   Device);
EFI_STATUS  EFIAPI Dummy2(VOID);

VOID        EFIAPI ReportDevices(CONNECTED_USB_DEVICES_NUM*);
UINT8       EFIAPI GetNextMassDeviceName(UINT8*, UINT8, UINT8);
VOID        EFIAPI UsbChangeEfiToLegacy(UINT8);
VOID*       EFIAPI AllocAlignedMemory(UINT32, UINT16);
VOID        EFIAPI USBGenerateSWSMI(UINT8);
VOID        EFIAPI UsbPrepareForLegacyOS();
EFI_STATUS  EFIAPI UpdateHcPciInfo();
EFI_STATUS  EFIAPI OemGetAssignUsbBootPort(UINT8*, UINT8*);
EFI_STATUS  EFIAPI UsbGetSkipList(USB_SKIP_LIST*, UINT8*, BOOLEAN); //(EIP51653+) 
VOID        FreeMemory(UINT32);
VOID        EFIAPI InvokeUsbApi(URP_STRUC*);

EFI_STATUS
EFIAPI
InstallHcProtocols(
    IN EFI_DRIVER_BINDING_PROTOCOL  *This,
    IN EFI_HANDLE                   Controller,
    IN EFI_PCI_IO_PROTOCOL          *PciIo,
    IN HC_STRUC                     *HcData
);

EFI_STATUS
EFIAPI
AmiUsb2HcGetState(
  IN  EFI_USB2_HC_PROTOCOL    *This,
  OUT EFI_USB_HC_STATE        *State);

EFI_STATUS
EFIAPI
AmiUsb2HcSetState(
  IN EFI_USB2_HC_PROTOCOL    *This,
  IN EFI_USB_HC_STATE        State);

EFI_STATUS
EFIAPI
AmiUsb2HcSyncInterruptTransfer(
  IN     EFI_USB2_HC_PROTOCOL               *HcProtocol,
  IN     UINT8                              DeviceAddress,
  IN     UINT8                              Endpointaddress,
  IN     UINT8                              DeviceSpeed,
  IN     UINTN                              MaximumPacketLength,
  IN OUT VOID                               *Data,
  IN OUT UINTN                              *DataLength,
  IN OUT UINT8                              *DataToggle,
  IN     UINTN                              Timeout,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  OUT    UINT32                             *TransferResult
  );

EFI_STATUS
EFIAPI
AmiUsb2HcControlTransfer(
  IN     EFI_USB2_HC_PROTOCOL               *HcProtocol,
  IN     UINT8                              DeviceAddress,
  IN     UINT8                              DeviceSpeed,
  IN     UINTN                              MaximumPacketLength,
  IN     EFI_USB_DEVICE_REQUEST             *Request,
  IN     EFI_USB_DATA_DIRECTION             TransferDirection,
  IN OUT VOID                               *Data,
  IN OUT UINTN                              *DataLength,
  IN     UINTN                              Timeout,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  OUT    UINT32                             *TransferResult
  );

EFI_STATUS 
EFIAPI 
AmiUsb2HcBulkTransfer(
  IN     EFI_USB2_HC_PROTOCOL               *HcProtocol,
  IN     UINT8                              DeviceAddress,
  IN     UINT8                              Endpointaddress,
  IN     UINT8                              DeviceSpeed,
  IN     UINTN                              MaximumPacketLength,
  IN     UINT8                              DataBuffersNumber,
  IN OUT VOID                               *Data[EFI_USB_MAX_BULK_BUFFER_NUM],
  IN OUT UINTN                              *DataLength,
  IN OUT UINT8                              *DataToggle,
  IN     UINTN                              Timeout,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  OUT    UINT32                             *TransferResult
  );

EFI_STATUS
EFIAPI
AmiUsb2HcAsyncInterruptTransfer(
    IN EFI_USB2_HC_PROTOCOL               *HcProtocol,
    IN UINT8                              DeviceAddress,
    IN UINT8                              EndpointAddress,
    IN UINT8                              DeviceSpeed,
    IN UINTN                              MaxPacket,
    IN BOOLEAN                            IsNewTransfer,
    IN OUT UINT8                          *DataToggle,
    IN UINTN                              PollingInterval,
    IN UINTN                              DataLength,
    IN EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
    IN EFI_ASYNC_USB_TRANSFER_CALLBACK    CallbackFunction ,
    IN VOID                               *Context
    );

EFI_STATUS
EFIAPI
DiskInfoInquiry (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    IN  OUT VOID                *InquiryData,
    IN  OUT UINT32              *InquiryDataSize
);

EFI_STATUS
EFIAPI
DiskInfoIdentify (
    EFI_DISK_INFO_PROTOCOL  *This,
    IN  OUT VOID            *IdentifyData,
    IN  OUT UINT32          *IdentifyDataSize
);

EFI_STATUS
EFIAPI
DiskInfoSenseData (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    OUT VOID                    *SenseData,
    OUT UINT32                  *SenseDataSize,
    OUT UINT8                   *SenseDataNumber
);

EFI_STATUS
EFIAPI
DiskInfoWhichIDE (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    OUT UINT32                  *IdeChannel,
    OUT UINT32                  *IdeDevice
);

EFI_STATUS
EFIAPI
AmiUsbBlkIoReset (
  IN  EFI_BLOCK_IO_PROTOCOL  *This,
  IN  BOOLEAN                ExtendedVerification
);

EFI_STATUS
EFIAPI
AmiUsbBlkIoFlushBlocks (
  IN  EFI_BLOCK_IO_PROTOCOL  *This
);

EFI_STATUS
EFIAPI
AmiUsbBlkIoReadBlocks (
  IN  EFI_BLOCK_IO_PROTOCOL  *This,
  IN  UINT32                 MediaId,
  IN  EFI_LBA                LBA,
  IN  UINTN                  BufferSize,
  OUT VOID                   *Buffer
);

EFI_STATUS
EFIAPI
AmiUsbBlkIoWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL  *This,
  IN UINT32                 MediaId,
  IN EFI_LBA                LBA,
  IN UINTN                  BufferSize,
  IN VOID                   *Buffer
);

EFI_STATUS
EFIAPI
UsbHcMemoryRecord(
    EFI_HANDLE              Handle,
    EFI_PHYSICAL_ADDRESS    MemoryStartAddress,
    UINTN                   MemoryTotalPages
);

EFI_STATUS EFIAPI UpdateMassDevicesForSetup();

VOID EFIAPI OnExitBootServices(EFI_EVENT, VOID*);
EFI_STATUS EFIAPI InitUsbSetupVars(USB_GLOBAL_DATA*);
UINT8 EFIAPI UsbSetupGetLegacySupport();

typedef EFI_STATUS (*USB_HC_PREINIT_FUNC) (
  IN EFI_HANDLE Handle,
    HC_STRUC    *HcStruc
);

typedef EFI_STATUS (*USB_HC_POSTSTOP_FUNC) (
  IN EFI_HANDLE Handle,
    HC_STRUC    *HcStruc
);

typedef struct {
    UINT64  PCICommand;
    UINT16  FrameListSize;
    UINT16  FrameListAlignment;
    USB_HC_PREINIT_FUNC HcPreInit;
    USB_HC_POSTSTOP_FUNC HcPostStop;
} HC_SPECIFIC_INFO;

enum {
    opHC_Start,
    opHC_Stop,
    opHC_EnumeratePorts,
    opHC_DisableInterrupts,
    opHC_EnableInterrupts,
    opHC_ProcessInterrupt,
    opHC_GetRootHubStatus,
    opHC_DisableRootHub,
    opHC_EnableRootHub,
    opHC_ControlTransfer,
    opHC_BulkTransfer,
    opHC_IsocTransfer,
    opHC_InterruptTransfer,
    opHC_DeactivatePolling,
    opHC_ActivatePolling,
    opHC_EnableEndpoints,
    opHC_DisableKeyRepeat,
    opHC_EnableKeyRepeat,
    opHC_InitDeviceData,
    opHC_DeinitDeviceData,
    opHC_ResetRootHub,
    opHC_ClearEndpointState,
    opHC_GlobalSuspend,
    opHC_SmiControl,
};


#define USB_PCD_TYPE_BOOLEAN    0x01
#define USB_PCD_TYPE_UINT8      0x02
#define USB_PCD_TYPE_UINT16     0x03
#define USB_PCD_TYPE_UINT32     0x04
#define USB_PCD_TYPE_UINT64     0x05
#define USB_PCD_TYPE_VOID       0x06

#pragma pack(push, 1)
typedef struct _PCD_INFO_INIT_TABLE PCD_INFO_INIT_TABLE;
struct _PCD_INFO_INIT_TABLE{
    UINTN   TokenName;
    UINT32  *MemAddr;
    UINTN   Value;
    UINT8   Type;
};
#pragma pack(pop)

EFI_STATUS  DummyHcFunc(EFI_HANDLE, HC_STRUC*);
EFI_STATUS  PreInitXhci(EFI_HANDLE, HC_STRUC*);
EFI_STATUS  PostStopXhci(EFI_HANDLE, HC_STRUC*);
EFI_STATUS  PreInitEhci(EFI_HANDLE, HC_STRUC*);
EFI_STATUS  PostStopEhci(EFI_HANDLE, HC_STRUC*);
EFI_STATUS  EFIAPI Usb3OemGetMaxDeviceSlots(HC_STRUC*, UINT8*);
VOID*       EFIAPI AllocateHcMemory (IN EFI_PCI_IO_PROTOCOL*, UINTN, UINTN);
VOID        EFIAPI FreeHcMemory(IN EFI_PCI_IO_PROTOCOL*, IN UINTN, IN VOID*);
EFI_STATUS  EFIAPI ReallocateMemory(UINTN, UINTN, VOID**);
VOID        UsbSmiPeriodicEvent(VOID);
VOID        EFIAPI UhcdPciIoNotifyCallback(EFI_EVENT, VOID*);
VOID        EFIAPI UhcdPciIrqPgmNotifyCallback(EFI_EVENT, VOID*);
VOID        EFIAPI LegacyBiosProtocolNotifyCallback(EFI_EVENT, VOID*);
VOID        EFIAPI UsbLegcyToEfiNotify(EFI_EVENT, VOID*);
UINTN       EFIAPI UsbSmiHc(UINT8,UINT8, ...);
VOID        EFIAPI Emul6064NotifyCallback(EFI_EVENT, VOID*);
VOID        EFIAPI UsbRtShutDownLegacy(VOID);
VOID        EFIAPI UsbRtStopController(UINT16);
VOID        EFIAPI UsbHcOnTimer(EFI_EVENT, VOID*);
UINT8       UsbSmiEnableEndpoints(HC_STRUC*, DEV_INFO*, UINT8*);
EFI_STATUS  EFIAPI GetRuntimeRegion(EFI_PHYSICAL_ADDRESS*, EFI_PHYSICAL_ADDRESS*);
BOOLEAN     EFIAPI IsExternalController(EFI_DEVICE_PATH_PROTOCOL*);
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
