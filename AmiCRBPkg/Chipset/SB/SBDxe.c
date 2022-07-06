
//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//*****************************************************************************


//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:  SBDXE.C
//
// Description: This file contains code for Template Southbridge initialization
//    in the DXE stage
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>


// Module specific Includes
#include <Efi.h>
#include <token.h>
#include <Setup.h>
#include <Dxe.h>
#include <AmiLib.h>
#include <AmiCspLib.h>
#include <AmiDxeLib.h>

// Consumed Protocols
#include <Protocol\PciIo.h>
#include <Protocol\Cpu.h>
#include <Protocol\PciRootBridgeIo.h>
#include "Protocol\DevicePath.h"
#include "protocol\DriverBinding.h"
#include <Protocol\ComponentName.h>

#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
#include <Protocol/AmiUsbController.h>
#endif

// AptioV: USB Per-Port/Controller Disable support. >>>
#if defined OEM_USB_PER_PORT_DISABLE_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1
#include <Protocol\UsbIo.h>
#endif
// AptioV: USB Per-Port/Controller Disable support. <<<
#include <Guid/SetupVariable.h>
#include <PchRegs.h>
//#include <PchRegs/PchregsRcrb.h>
//#include <PchRegs/PchregsUsb.h>
#include <Library/S3BootScriptLib.h>
#include <Library\PciLib.h>

// AptioV: USB Per-Port/Controller Disable support. >>>
#include <Protocol/PchPlatformPolicy.h>
#include <Library/PchPlatformLib.h>
// AptioV: USB Per-Port/Controller Disable support. <<<

#include <SB.h> //keep this as last include to avoid re-definition errors

#define ICH_RCRB_FD2        0x3428 // Function Disable Register 2
#define PCILIB_TO_COMMON_ADDRESS(Address) \
        ((UINT64) ((((UINTN) ((Address>>20) & 0xff)) << 24) + (((UINTN) ((Address>>15) & 0x1f)) << 16) + (((UINTN) ((Address>>12) & 0x07)) << 8) + ((UINTN) (Address & 0xfff ))))
// AptioV: USB Per-Port/Controller Disable support. <<<
#define XHCI_BUS                   0x00
#define XHCI_DEV                   0x14
#define XHCI_FUN                   0x00
#define R_PCH_H_XHCI_PORTSC1USB3   0x570
#define B_PCH_XHCI_PORTSCXUSB3_PP  BIT9   ///< Port Power
#define D29_DIS                    BIT0
#define D26_DIS                    BIT1
#define D20_DIS                    BIT2
#define ALL_USB_DISABLED           (D29_DIS | D26_DIS | D20_DIS)
#ifndef BASE_CLASS_MASS_STORAGE
#define BASE_CLASS_MASS_STORAGE    0x08
#endif
// AptioV: USB Per-Port/Controller Disable support. <<<
//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#define ONBOARD_RAID_GUID \
    { 0x5d206dd3, 0x516a, 0x47dc, 0xa1, 0xbc, 0x6d, 0xa2, 0x4, 0xaa, 0xbe, 0x8};

#define SB_SIGNATURE '__SB'

//----------------------------------------------------------------------------
//   Variable Prototypes
//----------------------------------------------------------------------------
typedef struct {
    UINT8 Bus;
    UINT8 Dev;
    UINT8 Fun;
    VOID *Process;
} DEVICES_AFTER_PCIIO;

DEVICES_AFTER_PCIIO gDevicesTable[] = {
    { 0x00, 0x1F, 0x02, NULL },
    { 0x00, 0x11, 0x04, NULL },//AptioV Server Override: In RAID mode, the HDD info cannot be shown in SATA/sSATA Configuration page.
    { 0xFF, 0xFF, 0xFF, NULL }
};

//----------------------------------------------------------------------------
//   Variable Declaration
//----------------------------------------------------------------------------
EFI_SYSTEM_TABLE *mST = 0;
EFI_HANDLE gImageHandle;

// GUID Definitions
EFI_GUID gOnboardRaidGuid = ONBOARD_RAID_GUID;
// AptioV: USB Per-Port Disable support. >>>
EFI_GUID gAmiBeforeBootProtocolGuid        = AMITSE_EVENT_BEFORE_BOOT_GUID;
// AptioV: USB Per-Port Disable support. <<<

UINTN gEventCount = sizeof(gDevicesTable) / sizeof(DEVICES_AFTER_PCIIO);
VOID *gSbInitNotifyReg = NULL;

EFI_RUNTIME_SERVICES *gRT;
EFI_BOOT_SERVICES *gBS;
EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *gPciRootBridgeIo;
EFI_EVENT gEvtBootScript;
EFI_EVENT gEvtLegacyBoot;

// AptioV: USB Per-Port/Controller Disable support. >>>
BOOLEAN                         gDisableAllUsbControllers = FALSE;
UINT8                           gDisabledUsbBitsMap = 0;
#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
EFI_EVENT                       gEvtUsbProtocol  = NULL;
VOID                            *gRegUsbProtocol = NULL;
EFI_USB_PROTOCOL                *gUsbProtocol = NULL;
#endif
// AptioV: USB Per-Port/Controller Disable support. <<<

//----------------------------------------------------------------------------
// Function Prototypes
//----------------------------------------------------------------------------
VOID SbCallbackReadyToBoot(IN EFI_EVENT Event, IN VOID *Context);
VOID SbCallbackLegacyBoot(IN EFI_EVENT Event, IN VOID *Context);

VOID SbInitAfterDeviceInstall (
    IN EFI_EVENT        Event,
    IN VOID             *Context
);

EFI_STATUS
SBSupported(

    IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
    IN  EFI_HANDLE                      Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath
);

EFI_STATUS
SBStart(

    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
);

EFI_STATUS
SBStop(

    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  UINTN                          NumberOfChildren,
    IN  EFI_HANDLE                     *ChildHandleBuffer
);

// AptioV: USB Per-Port/Controller Disable support. >>>
#if defined OEM_USB_PER_PORT_DISABLE_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1
VOID USBPerPortDisableCallback (
    IN EFI_EVENT    Event,
    IN VOID *Context
);
#endif

#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
VOID SbUsbProtocolCallback (
    IN EFI_EVENT        Event,
    IN VOID             *Context
);
#endif
// AptioV: USB Per-Port/Controller Disable support. <<<

CHAR16  *gSBDriverName         = L"AMI SB Driver";

CHAR16  *gSBControllerName     = L"SB Controller";

EFI_DRIVER_BINDING_PROTOCOL     gSBDriverBindingProtocol =
{

    SBSupported,
    SBStart,
    SBStop,
    1,                      // Version
    0,                   // Image handle
    0                    // Driver Binding Handle
};
//Aptio V Override save the SB date for S3 function start.
#define PCI_DEVICE(Bus, Dev, Func)  \
          Bus, Dev, Func

#define PCI_REG_MASK(Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7) \
          Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7

#define PCI_DEVICE_END    0xFF

STATIC UINT8  mPciCfgRegTable[] = {
  //
  // Logic to decode the table masks to arrive at the registers saved
  // Dword Registers are saved. For a given mask, the Base+offset register
  // will be saved as in the table below.
  // (example) To save register 0x24, 0x28 the mask at the Base 0x20 will be 0x06
  //     Base      0x00   0x20   0x40  0x60  0x80  0xA0  0xC0  0xE0
  // Mask  offset   
  // 0x01   0x00
  // 0x02   0x04
  // 0x04   0x08
  // 0x08   0x0C
  // 0x10   0x10
  // 0x20   0x14
  // 0x40   0x18
  // 0x80   0x1C
  //

  //
  // Bus,   Dev,  Func,
  // 00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
  // Only Bus 0 device is supported now
  //

  PCI_DEVICE   (0, 0, 0),					//DMI
  PCI_REG_MASK (0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 1, 0),					//PCIE 1
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 1, 1),					//PCIE 2
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 0),					//PCIE 3
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 1),					//PCIE 4
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 2),					//PCIE 5
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 3),					//PCIE 6
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 0),					//PCIE 7
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 1),					//PCIE 8
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 2),					//PCIE 9
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 3),					//PCIE A
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 4, 0),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 1),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 2),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 3),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 4),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 5),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 6),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 7),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 5, 0),					//VT
  PCI_REG_MASK (0x08, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00),

  PCI_DEVICE   (0, 5, 1),					//Mem Hot
  PCI_REG_MASK (0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 5, 4),					//IOAPIC
  PCI_REG_MASK (0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 17, 4),					
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 17, 0),					
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 17, 5),					
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  // Save Memory base register of D25F0
  PCI_DEVICE   (0, 25, 0),
  PCI_REG_MASK (0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  // Save PIRQ register of D31F0
  PCI_DEVICE   (0, 31, 0),
  PCI_REG_MASK (0x00, 0x00, 0x00, 0x05, 0x1f, 0x00, 0x00, 0x00),

  // Save CMD register of D31F2
  PCI_DEVICE   (0, 31, 2),
  PCI_REG_MASK (0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),
  // Save CMD register of D31F5
  PCI_DEVICE   (0, 31, 5),
  PCI_REG_MASK (0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  // Save CMD, MEM register of D31F3
  PCI_DEVICE   (0, 31, 3),
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE_END
};

                                        // <(P040109A)

EFI_STATUS
SaveRuntimeScriptTable (
   )
{
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress;
  UINT32                Data32;
  UINT16                Data16;
  UINT8                 Data8;
  UINT8                 Mask;
  UINTN                 Index;
  UINTN                 Offset;


  //
  // Save PCI-Host bridge settings (0, 0, 0). 0x90, 94 and 9c are changed by CSM
  // and vital to S3 resume. That's why we put save code here
  //
  Index = 0;
  while (mPciCfgRegTable[Index] != PCI_DEVICE_END) {

    PciAddress.Bus              = mPciCfgRegTable[Index++];
    PciAddress.Device           = mPciCfgRegTable[Index++];
    PciAddress.Function         = mPciCfgRegTable[Index++];
    PciAddress.Register         = 0;
    PciAddress.ExtendedRegister = 0;

    Data16 = PciRead16 (PCI_LIB_ADDRESS(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PciAddress.Register));
    if (Data16 == 0xFFFF) {
      Index += 8;
      continue;
    }

    for (Offset = 0, Mask = 0x01; Offset < 256; Offset += 4, Mask <<= 1) {

      if (Mask == 0x00) {
        Mask = 0x01;
      }

      if (mPciCfgRegTable[Index + Offset / 32] & Mask) {

        PciAddress.Register = (UINT8) Offset;
        Data32 = PciRead32 (PCI_LIB_ADDRESS(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PciAddress.Register));

        //
        // Save latest settings to runtime script table
        //
        S3BootScriptSavePciCfgWrite (
            S3BootScriptWidthUint32,
            PCILIB_TO_COMMON_ADDRESS (PCI_LIB_ADDRESS(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PciAddress.Register)),
            1,
          &Data32
          );
      }
    }

    Index += 8;
  }

  //
  // Save I/O ports to S3 script table
  //

  //
  // Selftest KBC
  //
  Data8 = 0xAA;
  S3BootScriptSaveIoWrite(S3BootScriptWidthUint8, 0x64, 1, &Data8);

  //
  // BXW, WA, Cleare possible pendding SMIs
  //
  Data32 = 0;
  S3BootScriptSaveIoWrite(S3BootScriptWidthUint16, (PM_BASE_ADDRESS + R_PCH_LPTH_ALT_GP_SMI_EN), 1, &Data32);
  //
  // Important to trap Sx for SMM
  //
  Data32 = IoRead32 (PM_BASE_ADDRESS + R_PCH_SMI_EN);
  S3BootScriptSaveIoWrite(S3BootScriptWidthUint32, (PM_BASE_ADDRESS + R_PCH_SMI_EN), 1, &Data32);

  //
  // Save B_TCO_LOCK so it will be done on S3 resume path.
  //
  /* AptioV override
  Data16 = IoRead16 (PM_BASE_ADDRESS + PCH_TCO_BASE + R_PCH_TCO1_CNT);

  S3BootScriptSaveIoWrite(S3BootScriptWidthUint16, (PM_BASE_ADDRESS + PCH_TCO_BASE + R_PCH_TCO1_CNT), 1, &Data16);
  */
  return EFI_SUCCESS;
}
//Aptio V Override save the SB date for S3 function end.

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SBDXE_Init
//
// Description: This function is the entry point for this DXE. This function
//              initializes the chipset SB
//
// Input: ImageHandle Image handle
//        SystemTable Pointer to the system table
//
// Output: Return Status based on errors that occurred while waiting for
//         time to expire.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SBDXE_Init(
    IN EFI_HANDLE  ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{

    EFI_STATUS Status;
    EFI_EVENT Event;
    EFI_GCD_MEMORY_SPACE_DESCRIPTOR         GcdDescriptor;
    DXE_SERVICES                    *DS = NULL;
// AptioV: USB Per-Port/Controller Disable support. >>>
    VOID                    *Registration = NULL;
#if CSM_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1
    EFI_EVENT               LegacyBootEvent;
#endif
// AptioV: USB Per-Port/Controller Disable support. <<<
    
  
    UINT64  PcieBaseAddr;
    InitAmiLib(ImageHandle, SystemTable);
  
    PcieBaseAddr = (UINT64)PcdGet64(PcdPciExpressBaseAddress); 

    mST = SystemTable;
    gImageHandle = ImageHandle;
     
    PROGRESS_CODE(DXE_SB_INIT);

    Status = pBS->LocateProtocol(&gEfiPciRootBridgeIoProtocolGuid, 0, &gPciRootBridgeIo);
    ASSERT_EFI_ERROR(Status);
    
    Status=LibGetDxeSvcTbl(&DS);
    TRACE ((-1, "LibGetDxeSvcTbl(&DS)%r.\n", Status));
    // Only mark the runtime attribute to this region. This region is already added into GCD.
       Status = DS->GetMemorySpaceDescriptor (PcieBaseAddr, &GcdDescriptor);
       TRACE ((-1, " GcdDescriptor.Attributes%x.\n",  GcdDescriptor.Attributes));
          			if (EFI_ERROR (Status)) {
      			   		TRACE ((-1, "Fail to get PCIEx GcdDescriptor at %x.\n", PcieBaseAddr));
      			   	}
      			   	else {
      			   	Status = DS->SetMemorySpaceAttributes (
      			        		PcieBaseAddr,
      			        		0x10000000,
      			        		GcdDescriptor.Attributes | EFI_MEMORY_RUNTIME
      			                        );
      			        if (EFI_ERROR (Status)) {
      			            TRACE ((-1, "Unable to add EFI_MEMORY_RUNTIME attribute to PCIEx region %r.\n",Status));
      			        }
      			   	}
    
    Status = RegisterProtocolCallback( &gEfiPciIoProtocolGuid,
                                       SbInitAfterDeviceInstall,
                                       NULL,
                                       &Event,
                                       &gSbInitNotifyReg );
    ASSERT_EFI_ERROR(Status);

    Status = CreateReadyToBootEvent(TPL_NOTIFY, SbCallbackReadyToBoot, 0, &gEvtBootScript );
    ASSERT_EFI_ERROR(Status);

    Status = CreateLegacyBootEvent(TPL_NOTIFY, SbCallbackLegacyBoot, 0, &gEvtLegacyBoot );
    ASSERT_EFI_ERROR(Status);

// AptioV: USB Per-Port/Controller Disable support. >>>
#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
    Status = RegisterProtocolCallback( &gEfiUsbProtocolGuid,\
                                       SbUsbProtocolCallback,\
                                       NULL,\
                                       &gEvtUsbProtocol,\
                                       &gRegUsbProtocol );
    
    pBS->SignalEvent(gEvtUsbProtocol);
#endif

#if defined OEM_USB_PER_PORT_DISABLE_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1 
    RegisterProtocolCallback(
        &gAmiBeforeBootProtocolGuid,
        USBPerPortDisableCallback,
        NULL, &Event, &Registration
    );
	
#if CSM_SUPPORT
    CreateLegacyBootEvent(TPL_CALLBACK, &USBPerPortDisableCallback, NULL, &LegacyBootEvent);
#endif
#endif
// AptioV: USB Per-Port/Controller Disable support. <<<

    gSBDriverBindingProtocol.ImageHandle         = ImageHandle;
    gSBDriverBindingProtocol.DriverBindingHandle = ImageHandle;

    Status = pBS->InstallMultipleProtocolInterfaces(&ImageHandle,
             &gEfiDriverBindingProtocolGuid, &gSBDriverBindingProtocol,
             0);

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   SbInitAfterDeviceInstall
//
// Description: This function installs the the timer protocol on its handle,
//              and initializes the timer.
//
// Input:       Event   - Event of callback
//              Context - Context of callback.
//
// Output:      EFI_SUCCESS
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID SbInitAfterDeviceInstall (
    IN EFI_EVENT        Event,
    IN VOID             *Context )
{
    EFI_STATUS              Status;
    UINTN                   BufferSize = 20 * sizeof(EFI_HANDLE);
    EFI_HANDLE              Handle;
    EFI_PCI_IO_PROTOCOL     *PciIo;
    UINTN                   i = 0;
    UINTN                   PciSeg;
    UINTN                   PciBus;
    UINTN                   PciDev;
    UINTN                   PciFun;
    UINT16                  CapPtr16 = 0;
    SYSTEM_CONFIGURATION      SystemConfiguration; 
    UINTN                             VarSize;    

    Status = pBS->LocateHandle( ByRegisterNotify,
                                NULL,
                                gSbInitNotifyReg,
                                &BufferSize,
                                &Handle );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return;
    //
    // Locate PciIo protocol installed on Handle
    //
    Status = pBS->HandleProtocol( Handle, &gEfiPciIoProtocolGuid, &PciIo );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return;

    // Get PCI Device Bus/Device/Function Numbers
    Status = PciIo->GetLocation( PciIo, &PciSeg, &PciBus, &PciDev, &PciFun );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return;

    if (((PciBus == 0x00) && (PciDev == 0x1F) && (PciFun == 0x02)) || ((PciBus == 0x00) && (PciDev == 0x11) && (PciFun == 0x04)))
    {    	
      VarSize = sizeof (SystemConfiguration);

      Status = mST->RuntimeServices->GetVariable (
                      L"IntelSetup",
                      &gEfiSetupVariableGuid,
                      NULL,
                      &VarSize,
                      &SystemConfiguration
                      );
      
#ifdef PCH_SERVER_BIOS_FLAG
     if((((PciBus == 0x00) && (PciDev == 0x1F) && (PciFun == 0x02))&& SystemConfiguration.SataInterfaceMode == 2) ||                       
     (((PciBus == 0x00) && (PciDev == 0x11) && (PciFun == 0x04))&& SystemConfiguration.sSataInterfaceMode == 2)){                       
   	
    	TRACE((-1,"Found onboard RAID controller during PCI enumeration \n"));
        if (MmPci8(PciBus, PciDev, PciFun, 0x0A) == SCC_RAID_DEVICE)
        {
            Status = pBS->InstallProtocolInterface(
                            &Handle,
                            &gOnboardRaidGuid,
                            EFI_NATIVE_INTERFACE,
                            NULL);
            ASSERT_EFI_ERROR(Status);
        }

        gEventCount -= 1;
     }
#endif
    }

    // Kill the Event
    if (gEventCount == 1)
        pBS->CloseEvent(Event);
}

// AptioV: USB Per-Port/Controller Disable support. >>>
#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   DisconnectUsbController
//
// Description: Disconnect USB controller in ready to boot phase.
//
// Input:       IN UINT8 DisabledUsbBitsMap
//
// Output:      None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID DisconnectUsbController
(
  IN UINT8 DisabledUsbBitsMap
)
{
    EFI_STATUS                  Status;
    UINTN                       HandleCount;
    EFI_HANDLE                  *HandleBuffer;
    UINTN                       Index;
    EFI_PCI_IO_PROTOCOL         *PciIo;
    UINTN                       Segment;
    UINTN                       Bus;
    UINTN                       Device;
    UINTN                       Function;
  
    TRACE((-1, "DisconnectUsbController() Starts.\n"));

    Status = pBS->LocateHandleBuffer (
		    ByProtocol,
		    &gEfiPciIoProtocolGuid,
		    NULL,
		    &HandleCount,
		    &HandleBuffer);
    for (Index = 0; Index < HandleCount; Index ++) {
      Status = pBS->HandleProtocol (
		      HandleBuffer[Index],
		      &gEfiPciIoProtocolGuid,
		      &PciIo);
      if (EFI_ERROR(Status)) 
	      continue;
      PciIo->GetLocation (
		      PciIo,
		      &Segment,
		      &Bus,
		      &Device,
		      &Function);
      if (((DisabledUsbBitsMap & D20_DIS) && (Device == PCI_DEVICE_NUMBER_PCH_XHCI) && (Function == PCI_FUNCTION_NUMBER_PCH_XHCI)) ||
          ((DisabledUsbBitsMap & D29_DIS) && (Device == PCI_DEVICE_NUMBER_PCH_USB) && (Function == PCI_FUNCTION_NUMBER_PCH_EHCI)) ||
          ((DisabledUsbBitsMap & D26_DIS) && (Device == PCI_DEVICE_NUMBER_PCH_USB_EXT) && (Function == PCI_FUNCTION_NUMBER_PCH_EHCI2)))
      {
        TRACE((-1, "Disconnect USB Controller Dev:%x Func:%x\n", Device, Function));
        pBS->DisconnectController(HandleBuffer[Index], NULL, NULL);      
      }
    }

    TRACE((-1, "DisconnectUsbController() Ends.\n"));
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckDisableUsbControllers
//
// Description:
//
// Input:      IN SB_CONFIG_DATA *SbConfig - SB configuration data
//
// Output:     None
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID CheckDisableUsbControllers(VOID)
{
  UINT32 FunctionDisable = 0;
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_GUID   EfiGlobalVariableGuid = EFI_GLOBAL_VARIABLE;
  UINTN      BootOrderSize = 0;
  UINT16     *BootOrder = NULL;

  TRACE((-1, "CheckDisableUsbControllers() Starts.\n"));
  
  Status = pBS->LocateProtocol( &gEfiUsbProtocolGuid,
                                NULL,
                                &gUsbProtocol );
  if (EFI_ERROR(Status)) return;

  DisconnectUsbController(gDisabledUsbBitsMap);

  FunctionDisable = READ_MEM32_RCRB(R_PCH_RCRB_FUNC_DIS);

  if (gDisableAllUsbControllers) {

    Status = pRS->GetVariable( L"BootOrder", \
                               &EfiGlobalVariableGuid, \
                               NULL, \
                               &BootOrderSize, \
                               &BootOrder );
    if (Status == EFI_NOT_FOUND) return;

    MmPci16( \
        DEFAULT_PCI_BUS_NUMBER_PCH, \
        PCI_DEVICE_NUMBER_PCH_USB, \
        PCI_FUNCTION_NUMBER_PCH_EHCI, \
        R_PCH_EHCI_COMMAND_REGISTER) = 0;
    FunctionDisable |= B_PCH_RCRB_FUNC_DIS_EHCI1;
    
    MmPci16( \
        DEFAULT_PCI_BUS_NUMBER_PCH, \
        PCI_DEVICE_NUMBER_PCH_USB_EXT, \
        DEFAULT_PCI_BUS_NUMBER_PCH, \
        R_PCH_EHCI_COMMAND_REGISTER) = 0;
    FunctionDisable |= B_PCH_RCRB_FUNC_DIS_EHCI2;
    
    MmPci16( \
        DEFAULT_PCI_BUS_NUMBER_PCH, \
        PCI_DEVICE_NUMBER_PCH_XHCI, \
        PCI_FUNCTION_NUMBER_PCH_XHCI, \
        R_PCH_XHCI_COMMAND_REGISTER) = 0;
    FunctionDisable |= B_PCH_RCRB_FUNC_DIS_XHCI;

  } else {
	  
#if defined OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT && OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT == 1

    if (gDisabledUsbBitsMap & D29_DIS) {
      MmPci16( \
          DEFAULT_PCI_BUS_NUMBER_PCH, \
          PCI_DEVICE_NUMBER_PCH_USB, \
          PCI_FUNCTION_NUMBER_PCH_EHCI, \
          R_PCH_EHCI_COMMAND_REGISTER) = 0;
      FunctionDisable |= B_PCH_RCRB_FUNC_DIS_EHCI1;
    }

    if (gDisabledUsbBitsMap & D26_DIS) {
      MmPci16( \
          DEFAULT_PCI_BUS_NUMBER_PCH, \
          PCI_DEVICE_NUMBER_PCH_USB_EXT, \
          DEFAULT_PCI_BUS_NUMBER_PCH, \
          R_PCH_EHCI_COMMAND_REGISTER) = 0;
      FunctionDisable |= B_PCH_RCRB_FUNC_DIS_EHCI2;
    }

    if (gDisabledUsbBitsMap & D20_DIS) {
      MmPci16( \
          DEFAULT_PCI_BUS_NUMBER_PCH, \
          PCI_DEVICE_NUMBER_PCH_XHCI, \
          PCI_FUNCTION_NUMBER_PCH_XHCI, \
          R_PCH_XHCI_COMMAND_REGISTER) = 0;
      FunctionDisable |= B_PCH_RCRB_FUNC_DIS_XHCI;
    }
#endif // OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT

  }

  WRITE_MEM32_RCRB (R_PCH_RCRB_FUNC_DIS, FunctionDisable);
  S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (SB_RCBA + R_PCH_RCRB_FUNC_DIS),
      1,
      (VOID *) (UINTN) (SB_RCBA + R_PCH_RCRB_FUNC_DIS)
      );

  TRACE((-1, "CheckDisableUsbControllers() Ends.\n"));

}
#endif // AMIUSB_SUPPORT


#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   SbUsbProtocolCallback
//
// Description: This callback function is called after USB Protocol is 
//              installed.
//
// Input:       Event   - Event of callback
//              Context - Context of callback.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID SbUsbProtocolCallback (
    IN EFI_EVENT                    Event,
    IN VOID                         *Context )
{
    EFI_STATUS    Status = EFI_SUCCESS;
    SYSTEM_CONFIGURATION      SystemConfiguration;
    UINTN                     VarSize;    
    UINT8           IndexPort = 0;
    USB_SKIP_LIST   *SkipOemMassTable;
    USB_SKIP_LIST SkipMassTable[] = { {SKIP_FOR_ALLCONTROLLER, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xff, 0, 0, BASE_CLASS_MASS_STORAGE},
                                      {0, 0, 0,    0, 0, 0  }
                                    };
#if defined OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT && OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT == 1
    USB_SKIP_LIST SkipEhci1MassTable[] = { {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 0, 0, BASE_CLASS_MASS_STORAGE} };
    USB_SKIP_LIST SkipEhci2MassTable[] = { {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xD0, 0, 0, BASE_CLASS_MASS_STORAGE} };
    USB_SKIP_LIST SkipXhciMassTable[]  = { {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 0, 0, BASE_CLASS_MASS_STORAGE} };
#endif
#if USB_DRIVER_MAJOR_VER >= 22  // EIP426082
    UINT8          SkipTableSize;
#endif  //USB_DRIVER_MAJOR_VER >= 22
#if defined OEM_USB_PER_PORT_DISABLE_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1	
    UINT8           Port = 0;
    
    ///
    ///  Table: USB2 Pins Mapping between XHCI/EHCI Port
    ///   -------------------------------------------
    ///  | USB2 Pin     | EHCI Port      | XHCI Port |
    ///  |--------------+----------------+-----------|
    ///  | USB[P,N][0]  | EHCI 1 Port 0  | Port 0    |
    ///  | USB[P,N][1]  | EHCI 1 Port 1  | Port 1    |
    ///  | USB[P,N][2]  | EHCI 1 Port 2  | Port 2    |
    ///  | USB[P,N][3]  | EHCI 1 Port 3  | Port 3    |
    ///  | USB[P,N][4]  | EHCI 1 Port 4  | Port 8    |
    ///  | USB[P,N][5]  | EHCI 1 Port 5  | Port 9    |
    ///  | USB[P,N][6]  | EHCI 1 Port 6  | Port 12   |
    ///  | USB[P,N][7]  | EHCI 1 Port 7  | Port 13   |
    ///  | USB[P,N][8]  | EHCI 2 Port 8  | Port 4    |
    ///  | USB[P,N][9]  | EHCI 2 Port 9  | Port 5    |
    ///  | USB[P,N][10] | EHCI 2 Port 10 | Port 6    |
    ///  | USB[P,N][11] | EHCI 2 Port 11 | Port 7    |
    ///  | USB[P,N][12] | EHCI 2 Port 12 | Port 10   |
    ///  | USB[P,N][13] | EHCI 2 Port 13 | Port 11   |
    ///   -------------------------------------------
    ///
    USB_SKIP_LIST SkipUSBEhciMassTable[] = { 
             // EHCI1(D29:F0) for Port 0 to 7		    
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 1, BASE_CLASS_MASS_STORAGE},  // Port 0                          
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 2, BASE_CLASS_MASS_STORAGE},  // Port 1          
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 3, BASE_CLASS_MASS_STORAGE},  // Port 2
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 4, BASE_CLASS_MASS_STORAGE},  // Port 3
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 5, BASE_CLASS_MASS_STORAGE},  // Port 4
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 6, BASE_CLASS_MASS_STORAGE},  // Port 5
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 7, BASE_CLASS_MASS_STORAGE},  // Port 6     
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xE8, 1, 8, BASE_CLASS_MASS_STORAGE},  // Port 7           
             // EHCI2(D26:F0 for Port 8 to 13
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xD0, 1, 1, BASE_CLASS_MASS_STORAGE},  // Port 8
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xD0, 1, 2, BASE_CLASS_MASS_STORAGE},  // Port 9
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xD0, 1, 3, BASE_CLASS_MASS_STORAGE},  // Port 10
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xD0, 1, 4, BASE_CLASS_MASS_STORAGE},  // Port 11
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xD0, 1, 5, BASE_CLASS_MASS_STORAGE},  // Port 12
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xD0, 1, 6, BASE_CLASS_MASS_STORAGE}   // Port 13
    };

    USB_SKIP_LIST SkipUSBXhciMassTable[] = {
             // XHCI(D20:F0) for Port 0 to 20
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 1,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 0
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 2,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 1
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 3,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 2
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 4,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 3
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 9,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 8	
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 10, 0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 9
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 13, 0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 12
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 14, 0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 13	            
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 5,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 4
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 6,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 5					  
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 7,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 6
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 8,  0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 7		
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 11, 0, BASE_CLASS_MASS_STORAGE},  // XHCI Port 10	
             {0, SKIP_FLAG_SKIP_MULTI_LEVEL, 0xA0, 12, 0, BASE_CLASS_MASS_STORAGE}   // XHCI Port 11              
    };    
#endif

    VarSize = sizeof (SystemConfiguration);
    Status = pRS->GetVariable (
        L"IntelSetup",
        &gEfiSetupVariableGuid,
        NULL,
        &VarSize,
        &SystemConfiguration
        );
    ASSERT_EFI_ERROR(Status);
    
    if ((SystemConfiguration.PchUsb20[0] == 0) || (SystemConfiguration.PchUsb30Mode == 1))
      gDisabledUsbBitsMap |= D29_DIS;
    if ((SystemConfiguration.PchUsb20[1] == 0) || (SystemConfiguration.PchUsb30Mode == 1))
      gDisabledUsbBitsMap |= D26_DIS;
    if (SystemConfiguration.PchUsb30Mode == 0)
      gDisabledUsbBitsMap |= D20_DIS;
    
    if (gDisabledUsbBitsMap == ALL_USB_DISABLED)
      gDisableAllUsbControllers = TRUE;
    
    Status = pBS->LocateProtocol( &gEfiUsbProtocolGuid,
                                  NULL,
                                  &gUsbProtocol );
    if (EFI_ERROR(Status)) {
      pBS->CloseEvent(Event);
      return;
    }

    SkipOemMassTable = MallocZ( 
#if defined OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT && OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT == 1
                        sizeof(SkipEhci1MassTable)   + 
                        sizeof(SkipEhci2MassTable)   + 
                        sizeof(SkipXhciMassTable)    + 
#endif
#if defined OEM_USB_PER_PORT_DISABLE_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1
                        sizeof(SkipUSBEhciMassTable) + 
                        sizeof(SkipUSBXhciMassTable) + 
#endif
                        sizeof (USB_SKIP_LIST));
    ASSERT(SkipOemMassTable);
    
    if ( gDisableAllUsbControllers ) {
#if USB_DRIVER_MAJOR_VER >= 22  // EIP426082
        SkipTableSize = sizeof(SkipMassTable)/sizeof (USB_SKIP_LIST);
        Status = gUsbProtocol->UsbCopySkipTable(SkipMassTable, \
                                                &SkipTableSize, \
                                                FALSE);
#else
        gUsbProtocol->UsbCopySkipTable(SkipMassTable, \
                                       sizeof(SkipMassTable)/sizeof (USB_SKIP_LIST));
#endif  //USB_DRIVER_MAJOR_VER >= 22
      pBS->CloseEvent(Event);
      return;
#if defined OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT && OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT == 1
    } else {
      if (gDisabledUsbBitsMap & D29_DIS) {
        SkipOemMassTable[IndexPort++] = SkipEhci1MassTable[0];
        TRACE((-1, "Skip all mass storage devices under EHCI1 Controller\n"));
       }
      if (gDisabledUsbBitsMap & D26_DIS) {
        SkipOemMassTable[IndexPort++] = SkipEhci2MassTable[0];
        TRACE((-1, "Skip all mass storage devices under EHCI2 Controller\n"));
      }
      if (gDisabledUsbBitsMap & D20_DIS) {
        SkipOemMassTable[IndexPort++] = SkipXhciMassTable[0];
        TRACE((-1, "Skip all mass storage devices under XHCI Controller\n"));
      }
#endif
    }

#if defined OEM_USB_PER_PORT_DISABLE_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1
    //Report the SKIP USB port table for USB per-port disable
    
    if( SystemConfiguration.PchUsbPerPortCtl == PCH_DEVICE_ENABLE ) {
      
      // For USB Ehci per-port disable
      if ( SystemConfiguration.PchUsb30Mode != 1 ) {
        for ( Port = 0; Port < GetPchUsbMaxPhysicalPortNum(); Port++ ) {
          if ( SystemConfiguration.PchUsbPort[Port] == PCH_DEVICE_DISABLE ) {
            SkipOemMassTable[IndexPort++] = SkipUSBEhciMassTable[Port];
            TRACE((-1, "Skip mass storage device under USB EHCI Port = %x\n", Port));
          }
        }
      }

      // For USB Xhci per-port disable
      if (SystemConfiguration.PchUsb30Mode != PCH_DEVICE_DISABLE) {
        for (Port = 0; Port < GetPchUsbMaxPhysicalPortNum(); Port++) {
          if (SystemConfiguration.PchUsbPort[Port] == PCH_DEVICE_DISABLE) {
            SkipOemMassTable[IndexPort++] = SkipUSBXhciMassTable[Port];
            TRACE((-1, "Skip mass storage device under USB XHCI Port = %x\n", Port));
          }
        }
      }
    }
#endif

    if ( IndexPort != 0 ) {
#if USB_DRIVER_MAJOR_VER >= 22  // EIP426082
            SkipTableSize = IndexPort +1;
            gUsbProtocol->UsbCopySkipTable(SkipOemMassTable, \
                                           &SkipTableSize, \
                                           FALSE);
#else            
      gUsbProtocol->UsbCopySkipTable(SkipOemMassTable, IndexPort + 1);
#endif	  
    }
    pBS->FreePool( SkipOemMassTable );

    pBS->CloseEvent(Event);
}
#endif


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   DisableRmhDownPort
//
// Description: Disable RMH Down Port             
//
// Input:       UsbIo   - USB IO protocol
//              Port    - USB Port#.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
#if defined OEM_USB_PER_PORT_DISABLE_SUPPORT && OEM_USB_PER_PORT_DISABLE_SUPPORT == 1
EFI_STATUS  DisableRmhDownPort (
    IN EFI_USB_IO_PROTOCOL      *UsbIo,
    IN UINT8                    Port
)
{
    EFI_STATUS  Status;
    EFI_USB_DEVICE_REQUEST DevReq;
    UINT32 Timeout;
    UINT32 UsbStatus;
    
    DevReq.RequestType = 0x23;
    DevReq.Request     = 0x01;
    DevReq.Value       = 0x1;
    DevReq.Index       = Port;
    DevReq.Length      = 0;
    Timeout            = 3000;
    Status = UsbIo->UsbControlTransfer(
        UsbIo, 
        &DevReq, 
        EfiUsbNoData, 
        Timeout, 
        NULL, 
        0, 
        &UsbStatus
        );
  
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   USBPerPortDisableCallback
//
// Description: This function can disable USB PerPort before OS booting.             
//
// Input:       Event   - Event of callback
//              Context - Context of callback.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID USBPerPortDisableCallback (
    IN EFI_EVENT    Event,
    IN VOID *Context
)
{
    EFI_STATUS                  Status;
    EFI_HANDLE                  *HandleBuffer;
    EFI_GUID                    EfiUsbIoProtocolGuid = EFI_USB_IO_PROTOCOL_GUID;
    UINTN                       HandleCount = 0;
    EFI_USB_IO_PROTOCOL         *UsbIo;
    EFI_USB_IO_PROTOCOL         *Ehci1RmhUsbIo = NULL;
    EFI_USB_IO_PROTOCOL         *Ehci2RmhUsbIo = NULL;
    EFI_USB_DEVICE_DESCRIPTOR   DevDesc   = {0};
    UINT32                      Index;
    UINT8                       EHCIPort1 = 0;
    UINT8                       EHCIPort2 = 0;
    UINT8                       XhciPortDisableFlage[21];
    UINT32                      XhciUsb2Pdo = 0;
    UINT32                      XhciUsb3Pdo = 0;
    UINT32                      XhciIndex;
    UINT16                      RegData16;
    UINT32                      XhciUsb2InternalPortNumberLookUpTable[] = {0,1,2,3,8,9,12,13,4,5,6,7,10,11,12,13};
    static BOOLEAN              USBPerPortDisableDone = FALSE;
    SYSTEM_CONFIGURATION        SystemConfiguration; 
    UINTN                       VarSize;    

    ///
    ///  Table: USB2 Pins Mapping between XHCI/EHCI Port
    ///   -------------------------------------------
    ///  | USB2 Pin     | EHCI Port      | XHCI Port |
    ///  |--------------+----------------+-----------|
    ///  | USB[P,N][0]  | EHCI 1 Port 0  | Port 0    |
    ///  | USB[P,N][1]  | EHCI 1 Port 1  | Port 1    |
    ///  | USB[P,N][2]  | EHCI 1 Port 2  | Port 2    |
    ///  | USB[P,N][3]  | EHCI 1 Port 3  | Port 3    |
    ///  | USB[P,N][4]  | EHCI 1 Port 4  | Port 8    |
    ///  | USB[P,N][5]  | EHCI 1 Port 5  | Port 9    |
    ///  | USB[P,N][6]  | EHCI 1 Port 6  | Port 12   |
    ///  | USB[P,N][7]  | EHCI 1 Port 7  | Port 13   |
    ///  | USB[P,N][8]  | EHCI 2 Port 8  | Port 4    |
    ///  | USB[P,N][9]  | EHCI 2 Port 9  | Port 5    |
    ///  | USB[P,N][10] | EHCI 2 Port 10 | Port 6    |
    ///  | USB[P,N][11] | EHCI 2 Port 11 | Port 7    |
    ///  | USB[P,N][12] | EHCI 2 Port 12 | Port 10   |
    ///  | USB[P,N][13] | EHCI 2 Port 13 | Port 11   |
    ///   -------------------------------------------
    ///

    //Make sure the processing is performed only once. 
    if (USBPerPortDisableDone) {
        pBS->CloseEvent(Event);
        return;
    }

    TRACE((-1, "OEM_USB_PER_PORT_DISABLE_SUPPORT - Start\n"));

    VarSize = sizeof (SystemConfiguration);
    Status = pRS->GetVariable (
                    L"IntelSetup",
                    &gEfiSetupVariableGuid,
                    NULL,
                    &VarSize,
                    &SystemConfiguration
                    );

    // Locate handle buffer for USB Io Protocol
    Status = pBS->LocateHandleBuffer( ByProtocol,
                                      &EfiUsbIoProtocolGuid,
                                      NULL,
                                      &HandleCount,
                                      &HandleBuffer);
    if (EFI_ERROR(Status)) {
        TRACE((-1, "ERROR: Cannot Locate handle buffer for USB Io Protocol !!!\n"));
    } else {
        TRACE((-1, "USB Io Protocol user are %d Handles!!!\n", HandleCount));
    }

    // Initial Xhci Port Disable Flage
    // SS/HS: HS HS HS HS HS HS HS HS HS HS HS HS HS HS xx SS SS SS SS SS SS
    for (Index=0;Index<21;Index++) XhciPortDisableFlage[Index] = 0;

    for ( Index=0; Index < HandleCount; Index++ ) {
      pBS->HandleProtocol(HandleBuffer[Index], &gEfiUsbIoProtocolGuid, &UsbIo);
      Status = UsbIo->UsbGetDeviceDescriptor(UsbIo, &DevDesc);
      if (EFI_ERROR(Status)) {
        continue;
      }
      if (DevDesc.IdVendor != 0x8087) {
        continue;
      }
      switch (DevDesc.IdProduct) {
        case 0x8002:
          Ehci1RmhUsbIo = UsbIo;
          break;
        case 0x800A:
          Ehci2RmhUsbIo = UsbIo;
          break;
        default:
          break;
      } // switch
    } // for loop

// Intel_RC >>>
    if (SystemConfiguration.PchUsbPerPortCtl == PCH_DEVICE_ENABLE) {
      ///
      /// Open the Per-Port Disable Control Override
      ///

      RegData16 = IoRead16 ((UINTN) ((UINT64) (PM_BASE_ADDRESS + R_PCH_UPRWC)));
      RegData16 |= B_PCH_UPRWC_WR_EN;
      IoWrite16 ((UINTN) ((UINT64) (PM_BASE_ADDRESS + R_PCH_UPRWC)), RegData16);

      ///
      /// To support RapidStart resume from G3 state, all resume well registers need to be saved
      /// into S3 Script table.
      ///
      S3BootScriptSaveIoWrite(S3BootScriptWidthUint16, (PM_BASE_ADDRESS + R_PCH_UPRWC), 1, &RegData16);

      for (Index = 0; Index < GetPchUsbMaxPhysicalPortNum (); Index++) {
        if ((Index < 8) && (SystemConfiguration.PchUsb20[0] == PCH_DEVICE_ENABLE) && ( SystemConfiguration.PchUsb30Mode != 1)) {
          ///
          /// EHCI1 PDO for Port 0 to 7
          ///
          if (SystemConfiguration.PchUsbPort[Index] == PCH_DEVICE_DISABLE) {
            EHCIPort1 |= B_PCH_EHCI_PDO_DIS_PORT0 << Index;
            if (Ehci1RmhUsbIo != NULL) {
              DisableRmhDownPort(Ehci1RmhUsbIo, (Index + 1));
            }
          } else {
            EHCIPort1 &= ~(B_PCH_EHCI_PDO_DIS_PORT0 << Index);
          }
        } // EHCI1 PDO
        if ((Index >= 8) && (Index < 14) && (SystemConfiguration.PchUsb20[1] == PCH_DEVICE_ENABLE) && (SystemConfiguration.PchUsb30Mode != 1)) {
          ///
          /// EHCI2 PDO for Port 8 to 13
          ///
          if (SystemConfiguration.PchUsbPort[Index] == PCH_DEVICE_DISABLE) {
            EHCIPort2 |= B_PCH_EHCI_PDO_DIS_PORT0 << (Index - 8);
            if (Ehci2RmhUsbIo != NULL) {
              DisableRmhDownPort(Ehci2RmhUsbIo, (Index - 7));
            }
          } else {
            EHCIPort2 &= ~(B_PCH_EHCI_PDO_DIS_PORT0 << (Index - 8));
          }
        } // EHCI2 PDO
       } // for loop

      if((SystemConfiguration.PchUsb20[0] == PCH_DEVICE_ENABLE) && (SystemConfiguration.PchUsb30Mode != 1)) {
        ///
        /// To support RapidStart resume from G3 state, all resume well registers need to be saved
        /// into S3 Script table.
        ///
        TRACE((-1, "Write back Ehci1 PDO value: %x to PDO register\n", EHCIPort1));
        WRITE_PCI8 (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_USB, PCI_FUNCTION_NUMBER_PCH_EHCI, R_PCH_EHCI_PDO, EHCIPort1);
        S3BootScriptSavePciCfgWrite (
            S3BootScriptWidthUint32,
            PCILIB_TO_COMMON_ADDRESS (PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_USB, PCI_FUNCTION_NUMBER_PCH_EHCI, R_PCH_EHCI_PDO)),
            1,
            &EHCIPort1
            );

        if (SystemConfiguration.PchUsb20[1] == PCH_DEVICE_ENABLE) {
          TRACE((-1, "Write back Ehci2 PDO value: %x to PDO register\n", EHCIPort2));
          WRITE_PCI8 (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_USB_EXT, PCI_FUNCTION_NUMBER_PCH_EHCI2, R_PCH_EHCI_PDO, EHCIPort2);
          S3BootScriptSavePciCfgWrite (
              S3BootScriptWidthUint32,
              PCILIB_TO_COMMON_ADDRESS (PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_USB_EXT, PCI_FUNCTION_NUMBER_PCH_EHCI2, R_PCH_EHCI_PDO)),
              1,
              &EHCIPort2
              );
        } // PchSeries == PchH && Echi2 enable
      } // Echi1 enable

      if (SystemConfiguration.PchUsb30Mode != 0) {
        for (Index = 0; Index < GetPchUsbMaxPhysicalPortNum (); Index++) {
          XhciIndex = Index;
          ///
          /// Translate physical pins to internal ports numbering
          ///
          XhciIndex = XhciUsb2InternalPortNumberLookUpTable[Index];
          if (SystemConfiguration.PchUsbPort[Index] == PCH_DEVICE_DISABLE) {
            XhciUsb2Pdo |= (UINT32) (B_PCH_XHCI_USB2PDO_DIS_PORT0 << XhciIndex);
            XhciPortDisableFlage[XhciIndex] |= 1;
          } else {
            XhciUsb2Pdo &= (UINT32)~(B_PCH_XHCI_USB2PDO_DIS_PORT0 << XhciIndex);
          } // XCHI PDO
        } // for loop

        ///
        /// XHCI PDO for SS
        ///
        for (Index = 0; Index < GetPchXhciMaxUsb3PortNum (); Index++) {
          if (SystemConfiguration.PchUsb30Port[Index] == PCH_DEVICE_DISABLE) {
            XhciUsb3Pdo |= (UINT32) (B_PCH_XHCI_USB3PDO_DIS_PORT0 << Index);
            XhciPortDisableFlage[Index + 15] |= 1;
          } else {
            XhciUsb3Pdo &= (UINT32)~(B_PCH_XHCI_USB3PDO_DIS_PORT0 << Index);
          } // XHCI PDO
        } // for loop
        
        ///
        /// USB2PDO and USB3PDO are Write-Once registers and bits in them are in the SUS Well.
        ///
        TRACE((-1, "Write back Xhci HS PDO value: %x to HS PDO register\n", XhciUsb2Pdo));
        WRITE_PCI32(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI, R_PCH_XHCI_USB2PDO, XhciUsb2Pdo);
        S3BootScriptSavePciCfgWrite (
            S3BootScriptWidthUint32,
            PCILIB_TO_COMMON_ADDRESS (PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI, R_PCH_XHCI_USB2PDO)),
            1,
            &XhciUsb2Pdo
            );

        TRACE((-1, "Write back Xhci SS PDO value: %x to SS PDO register\n", XhciUsb3Pdo));
        WRITE_PCI32(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI, R_PCH_XHCI_USB3PDO, XhciUsb3Pdo);
        S3BootScriptSavePciCfgWrite (
            S3BootScriptWidthUint32,
            PCILIB_TO_COMMON_ADDRESS (PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI, R_PCH_XHCI_USB3PDO)),
            1,
            &XhciUsb3Pdo
            );

        ///
        /// Close the Per-Port Disable Control Override
        ///
        RegData16 &= (~B_PCH_UPRWC_WR_EN);
        IoWrite16 ((UINTN) ((UINT64) (PM_BASE_ADDRESS + R_PCH_UPRWC)), RegData16);
        ///
        /// To support RapidStart resume from G3 state, all resume well registers need to be saved
        /// into S3 Script table.
        ///
        S3BootScriptSaveIoWrite(S3BootScriptWidthUint16, (PM_BASE_ADDRESS + R_PCH_UPRWC), 1, &RegData16);
      } // SystemConfiguration.PchUsb30Mode != 0
    } // SystemConfiguration.PchUsbPerPortCtl == PCH_DEVICE_ENABLE
// Intel_RC <<<
// Disable usb port under Xhci controller >>>
{
    UINT8                     counter;
    UINT32                    RegVal;
    UINT64                    XhciBar;
    
    // Read back Xhci MMIO addrss
    if ((READ_PCI32(XHCI_BUS, XHCI_DEV, XHCI_FUN, R_PCH_XHCI_MEM_BASE) & 0x6) == 0x4) {
      XhciBar = (((UINT64) READ_PCI32(XHCI_BUS, XHCI_DEV, XHCI_FUN, R_PCH_XHCI_MEM_BASE + 4) << 32) | \
                 ((UINT64) READ_PCI32(XHCI_BUS, XHCI_DEV, XHCI_FUN, R_PCH_XHCI_MEM_BASE) & (~0x0F)));
    } else {
      XhciBar = READ_PCI32(XHCI_BUS, XHCI_DEV, XHCI_FUN, R_PCH_XHCI_MEM_BASE) & (~0x0F);
    }
    TRACE((-1, "Xhci Bar = %x\n", XhciBar));

    // Disable Xhci port which are disconnected
    for(Index=0;Index<21;Index++) {
      if (XhciPortDisableFlage[Index] != 0) {
        TRACE((-1, "Disable port%d under Xhci controller(start number:1)\n", (Index + 1)));
        // Index: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20
        // SS/HS: HS HS HS HS HS HS HS HS HS HS HS HS HS HS xx SS SS SS SS SS SS
        if (Index < 14) {
          //HS port
          RegVal = MmioRead32(XhciBar + R_PCH_XHCI_PORTSC01USB2 + 0x10 * Index);
          if (RegVal & B_PCH_XHCI_PORTSCXUSB2_PED) {
            RegVal = (B_PCH_XHCI_PORTSCXUSB2_PED | B_PCH_XHCI_PORTSCXUSB2_PP);
            MmioWrite32((XhciBar + R_PCH_XHCI_PORTSC01USB2 + 0x10 * Index), RegVal);
            
            for(counter=0;counter<200;counter++) {
              RegVal = MmioRead32(XhciBar + R_PCH_XHCI_PORTSC01USB2 + 0x10 * Index);
              if(!(RegVal & B_PCH_XHCI_PORTSCXUSB2_PED)) break;
              pBS->Stall(100);
            } // for loop
          } // PED bit is enable
        } else {
          //SS port
          RegVal = MmioRead32(XhciBar + R_PCH_H_XHCI_PORTSC1USB3 + 0x10 * Index);
          if (RegVal & B_PCH_XHCI_PORTSCXUSB3_PED) {
            RegVal = (B_PCH_XHCI_PORTSCXUSB3_PR | B_PCH_XHCI_PORTSCXUSB3_PP);
            MmioWrite32((XhciBar + R_PCH_H_XHCI_PORTSC1USB3 + 0x10 * Index), RegVal);
            for(counter=0;counter<3000;counter++) {
              RegVal = MmioRead32(XhciBar + R_PCH_H_XHCI_PORTSC1USB3 + 0x10 * Index);
              if(RegVal & B_PCH_XHCI_PORTSCXUSB3_PRC) break;
              pBS->Stall(100);
            } // for loop
            
            // Clear Warm Port Reset Change and Port Reset Change bits
            //RegVal = (B_PCH_XHCI_PORTSCXUSB3_WRC | B_PCH_XHCI_PORTSCXUSB3_PRC | B_PCH_XHCI_PORTSCXUSB3_PP);
            //MmioWrite32((XhciBar + R_PCH_H_XHCI_PORTSC1USB3 + 0x10 * Index), RegVal);
          } // PED bit is enable
        } // SS/HS port
      } // XhciPortDisableFlage[counter] != 0
    } // for loop

    pBS->FreePool(HandleBuffer);
}
// Disable usb port under Xhci controller <<<

    USBPerPortDisableDone = TRUE;
    TRACE((-1, "OEM_USB_PER_PORT_DISABLE_SUPPORT - End\n"));
    pBS->CloseEvent(Event);
}
#endif
// AptioV: USB Per-Port/Controller Disable support. <<<

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbCallbackLegacyBoot
//
// Description: SB call back function before legacy boot.
//
// Input: IN EFI_EVENT Event
//        IN VOID  *Context
//
// Output: None
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SbCallbackLegacyBoot(IN EFI_EVENT Event, IN VOID *Context)
{

    pBS->CloseEvent(Event);           
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbCallbackReadyToBoot
//
// Description: SB call back function before boot.
//
// Input: IN EFI_EVENT Event
//        IN VOID   *Context
// Output: None
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SbCallbackReadyToBoot(IN EFI_EVENT Event, IN VOID *Context)
{	
#if A_S3	
	EFI_STATUS  Status;
#endif	

#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
	CheckDisableUsbControllers();
#endif	

    pBS->CloseEvent(Event); 
#if A_S3	
    Status=SaveRuntimeScriptTable(); //Aptio V Override save the SB date for S3 function.
#endif	
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBSupported
//
// Description: Checks whether the device/controller is a SB device (other than IDE)
//
// Input:       *This           Pointer to the EFI_DRIVER_BINDING_PROTOCOL instance
//              Controller      The handle of a controller that the driver specified by
//                              This is managing.  This handle specifies the controller
//                              that needs a driver
//              *RemainingDevicePath    Device path of the controller
//
// Output:      EFI_STATUS
//              EFI_SUCCES              The Unicode string for the Driver specified by This
//                                      and the language specified by Language was returned
//                                      in DriverName
//              EFI_INVALID_PARAMETER   Language is NULL
//              EFI_INVALID_PARAMETER   DriverName is NULL
//              EFI_UNSUPPORTED         The driver specified by This does not support the
//                                      language specified by Language
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
SBSupported(
    IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
    IN  EFI_HANDLE                      Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath
)
{
//####  UINT8                       PciConfig[0x40];
    EFI_STATUS                  Status;
    EFI_PCI_IO_PROTOCOL         *PciIO;
    UINTN         seg, bus, dev, fun;

    // Check whether PCI Protocol has been installed on this controller
    Status = pBS->OpenProtocol(Controller, &gEfiPciIoProtocolGuid, (VOID **) & PciIO,
                               This->DriverBindingHandle, Controller, EFI_OPEN_PROTOCOL_BY_DRIVER);
    if (EFI_ERROR(Status))
        return Status;

//####    // Read the PCI Config Space
//####    Status = PciIO->Pci.Read (PciIO, EfiPciIoWidthUint8, 0, sizeof (PciConfig), PciConfig );
//####    if (EFI_ERROR(Status)) {
//####        pBS->CloseProtocol (Controller, &gEfiPciIoProtocolGuid, This->DriverBindingHandle, Controller);
//####        return EFI_UNSUPPORTED;
//####    }
//####
//####   // Porting require for chipset SMBUS device
//####    // Check whether the PCI Controller is IDE or NOT?
//####    if ( PciConfig[0x2] != 0x30                 ||
//####         PciConfig[0x3] != 0x3a                 ||
//####        PciConfig[0x0] != (0x86  )     ||
//####         PciConfig[0x1] != (0x80  )    ) {
//####        pBS->CloseProtocol (Controller, &gEfiPciIoProtocolGuid, This->DriverBindingHandle, Controller);
//####        return EFI_UNSUPPORTED;
//####    }

    Status = PciIO->GetLocation(PciIO, &seg, &bus, &dev, &fun);

    //  Close PciIoProtocol and return SUCCESS
    pBS->CloseProtocol(Controller, &gEfiPciIoProtocolGuid, This->DriverBindingHandle, Controller);

    if (EFI_ERROR(Status))
        return EFI_UNSUPPORTED;

        return EFI_UNSUPPORTED;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBStart
//
// Description: This function is called to start this controller driver
//
// Input:       *This           Pointer to the EFI_DRIVER_BINDING_PROTOCOL instance
//              Controller      The handle of a controller that the driver specified by
//                              This is managing.  This handle specifies the controller
//                              whose driver has to be started
//              *RemainingDevicePath    Device path of the controller
//
// Output:      EFI_SUCCESS     Always
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
SBStart(
    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
)
{
    EFI_STATUS                  Status;
    EFI_PCI_IO_PROTOCOL         *PciIO;
//  EFI_DEVICE_PATH_PROTOCOL    *ParentDevicePath;
    UINT8                       PciConfig[0x40];

    //  Check whether PCI Protocol has been installed on this controller
    Status = pBS->OpenProtocol(Controller, &gEfiPciIoProtocolGuid, (VOID **) & PciIO,
                               This->DriverBindingHandle, Controller, EFI_OPEN_PROTOCOL_BY_DRIVER);
    if (EFI_ERROR(Status))
        return Status;

    //  Read the PCI Config Space
    Status = PciIO->Pci.Read(PciIO, EfiPciIoWidthUint8, 0, sizeof(PciConfig), PciConfig);
    if (EFI_ERROR(Status))
    {
        pBS->CloseProtocol(Controller, &gEfiPciIoProtocolGuid, This->DriverBindingHandle, Controller);
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBStop
//
// Description: This function is called to stop this controller driver
//
// Input:       *This           Pointer to the EFI_DRIVER_BINDING_PROTOCOL instance
//              Controller      The handle of a controller that the driver specified by
//                              This is managing.  This handle specifies the controller
//                              whose driver has to be stopped
//              NumberOfChildren    Number of children associated with this driver
//              *ChildHandleBuffer  Pointers of all the children handles
//
// Output:      EFI_SUCCESS     Always
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
SBStop(
    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  UINTN                          NumberOfChildren,
    IN  EFI_HANDLE                     *ChildHandleBuffer
)
{
    return      EFI_SUCCESS;
}

