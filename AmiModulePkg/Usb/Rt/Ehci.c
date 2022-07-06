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

/** @file Ehci.c
    AMI USB EHCI support

**/
#include "AmiUsb.h"
#include "UsbKbd.h"
#include <Library/AmiUsbHcdLib.h>
#include <UsbDevDriverElinks.h>

UINT8   EFIAPI EHCI_Start (HC_STRUC*);
UINT8   EFIAPI EHCI_Stop (HC_STRUC*);
UINT8   EFIAPI EHCI_EnumeratePorts (HC_STRUC*);
UINT8   EFIAPI EHCI_DisableInterrupts (HC_STRUC*);
UINT8   EFIAPI EHCI_EnableInterrupts (HC_STRUC*);
UINT8   EFIAPI EHCI_ProcessInterrupt(HC_STRUC*);
UINT32  EFIAPI EHCI_GetRootHubStatus (HC_STRUC*, UINT8, BOOLEAN);
UINT8   EFIAPI EHCI_DisableRootHub (HC_STRUC*,UINT8);
UINT8   EFIAPI EHCI_EnableRootHub (HC_STRUC*,UINT8);
UINT16  EFIAPI EHCI_ControlTransfer (HC_STRUC*,DEV_INFO*,UINT16,UINT16,UINT16,UINT8*,UINT16);
UINT32  EFIAPI EHCI_BulkTransfer (HC_STRUC*,DEV_INFO*,UINT8,UINT8*,UINT32);
UINT32  EFIAPI EHCI_IsocTransfer (HC_STRUC*,DEV_INFO*,UINT8,UINT8*,UINT32, UINT8*);
UINT16  EFIAPI EHCI_InterruptTransfer (HC_STRUC*, DEV_INFO*, UINT8, UINT16, UINT8*, UINT16);
UINT8   EFIAPI EHCI_DeactivatePolling (HC_STRUC*,DEV_INFO*);
UINT8   EFIAPI EHCI_ActivatePolling (HC_STRUC*,DEV_INFO*);
UINT8   EFIAPI EHCI_DisableKeyRepeat (HC_STRUC*);
UINT8   EFIAPI EHCI_EnableKeyRepeat (HC_STRUC*);
UINT8   EFIAPI EHCI_ResetRootHub (HC_STRUC*,UINT8);
UINT8   EFIAPI EHCI_GlobalSuspend (HC_STRUC*); 
UINT8   EFIAPI EhciSmiControl(HC_STRUC*, BOOLEAN);

UINT8   EHCIResetHC(HC_STRUC*);
UINT8   EHCIInitializePeriodicSchedule(HC_STRUC*, UINT32);
UINT8   EHCIProgramLegacyRegisters(HC_STRUC*, UINT8);
UINT8   EHCIStartAsyncSchedule(HC_STRUC*);
UINT8   EHCIStopAsyncSchedule(HC_STRUC*);
UINT8   EHCIStartPeriodicSchedule(HC_STRUC*);
UINT8   EHCIStopPeriodicSchedule(HC_STRUC*);
UINT8   EHCIProcessQH(HC_STRUC*, EHCI_QH*);
VOID    EHCIProcessPeriodicList(HC_STRUC*);
VOID    EHCIInitializeQueueHead (EHCI_QH*);
VOID    EFIAPI EHCISetQTDBufferPointers(EHCI_QTD*, UINT8*, UINT32);
UINT16  EHCIWaitForTransferComplete(HC_STRUC*, EHCI_QH*,DEV_INFO* );
UINT8   EFIAPI EhciAddPeriodicQh(HC_STRUC*,EHCI_QH*);
VOID    EhciAddIsochTDs(HC_STRUC*);
UINT8   EhciRemovePeriodicQh(HC_STRUC*,EHCI_QH*);
VOID    ProcessOwnerShipChangeSMI(HC_STRUC*);
VOID    ProcessSmiChangeToEhcd(HC_STRUC*);
VOID    ProcessSmiChangeToBios(HC_STRUC*);
UINT8   EFIAPI EHCIGetLegacySupportOffset(HC_STRUC*, UINT16);
UINT8   EFIAPI EhciPollingTDCallback(HC_STRUC*, DEV_INFO*, UINT8*, UINT8*, UINT16);
UINT8   EFIAPI EhciRepeatTDCallback(HC_STRUC*, DEV_INFO*, UINT8*, UINT8*, UINT16);
UINT32  EFIAPI DwordReadMem(UINT32, UINT32);
UINT32  EFIAPI HcReadPciReg(HC_STRUC*, UINT32);
VOID    EFIAPI HcWritePciReg(HC_STRUC*, UINT32, UINT32);
UINT32  EFIAPI HcReadHcMem(HC_STRUC*, UINT32);
VOID    EFIAPI HcWriteHcMem(HC_STRUC*, UINT32, UINT32);
UINT32  EFIAPI HcReadOpReg(HC_STRUC*, UINT32);
VOID    EFIAPI HcWriteOpReg(HC_STRUC*, UINT32, UINT32);
VOID    EFIAPI HcClearOpReg(HC_STRUC*, UINT32, UINT32);
VOID    EFIAPI HcSetOpReg(HC_STRUC*, UINT32, UINT32);
UINT32  EhciReadDebugReg(HC_STRUC*, UINT8, UINT32);
VOID*   EhciMemGetPhyAddr(HC_STRUC*, VOID*);
VOID*   EhciMemGetHostAddr(HC_STRUC*, VOID*);
VOID*   EFIAPI EhciMemAlloc(HC_STRUC*, UINT16);
VOID    EhciMemFree(HC_STRUC*, VOID*, UINT16);
UINT8   EFIAPI HcDmaMap(HC_STRUC*, UINT8, UINT8*, UINT32, UINT8**, VOID**);
UINT8   EFIAPI HcDmaUnmap(HC_STRUC*, VOID*);
BOOLEAN EhciIsHalted(HC_STRUC*);
UINT16  EhciTranslateInterval(UINT8, UINT8);
UINT8   USBCheckPortChange (HC_STRUC*, UINT8, UINT8);
UINT8   USBLogError(UINT16);
UINT8   UsbGetDataToggle(DEV_INFO*,UINT8);
VOID    UsbUpdateDataToggle(DEV_INFO*, UINT8, UINT8);
VOID    USB_InitFrameList (HC_STRUC*, UINT32);
VOID    FixedDelay(UINTN);
VOID*   USB_MemAlloc (UINT16);
UINT8   USB_MemFree (VOID _FAR_ *, UINT16);
UINT8   USB_DisconnectDevice (HC_STRUC*, UINT8, UINT8);
DEV_INFO*   EFIAPI USB_GetDeviceInfoStruc(UINT8, DEV_INFO*, UINT8, HC_STRUC*);
UINT8   USB_StopDevice (HC_STRUC*,  UINT8, UINT8);
UINT32  USB_GetHubPortStatus(HC_STRUC*, UINT8, UINT8, BOOLEAN);
UINT8   EFIAPI USB_InstallCallBackFunction (CALLBACK_FUNC);
VOID    EFIAPI USBKeyRepeat(HC_STRUC*, UINT8);
VOID    EFIAPI USBKBDPeriodicInterruptHandler(HC_STRUC*);



extern  USB_GLOBAL_DATA     *gUsbData;
extern  BOOLEAN             gCheckUsbApiParameter;
extern  USB_DATA_LIST       *gUsbDataList;
extern HC_STRUC             **gHcTable;


typedef VOID (EHCI_GET_ROOT_HUB_PORT_SPEED_FUNS)( 
    HC_STRUC    *HcStruc,
    UINT8       PortNum,
    UINT8       *PortSpeed
);

/**
 *  This function calculates Isochronous TD buffer addresses
 */
typedef struct _ITD_PG_ADDRESS ITD_PG_ADDRESS;  
struct _ITD_PG_ADDRESS{
    UINT32 TrBuffer;
    BOOLEAN NewPgNeeded;
};

extern EHCI_GET_ROOT_HUB_PORT_SPEED_FUNS EHCI_GET_ROOT_HUB_PORT_SPEED_ELINK_LIST EndOfGetRootHubPortSpeedList;
EHCI_GET_ROOT_HUB_PORT_SPEED_FUNS*EhciGetRootHubPortSpeedFunctions[] = {
    EHCI_GET_ROOT_HUB_PORT_SPEED_ELINK_LIST NULL};

/**
    This function fills the host controller driver
    routine pointers

    @param HcdHeader       Pointer to the host controller header structure

    @retval USB_SUCCESS    Success
    @retval USB_ERROR      Failure

**/

UINT8
EHCI_FillHCDEntries(
    HCD_HEADER *HcdHeader
)
{
    HcdHeader->FnHcdStart                = EHCI_Start;
    HcdHeader->FnHcdStop                 = EHCI_Stop;
    HcdHeader->FnHcdEnumeratePorts       = EHCI_EnumeratePorts;
    HcdHeader->FnHcdDisableInterrupts    = EHCI_DisableInterrupts;
    HcdHeader->FnHcdEnableInterrupts     = EHCI_EnableInterrupts;
    HcdHeader->FnHcdProcessInterrupt     = EHCI_ProcessInterrupt;
    HcdHeader->FnHcdGetRootHubStatus     = EHCI_GetRootHubStatus;
    HcdHeader->FnHcdDisableRootHub       = EHCI_DisableRootHub;
    HcdHeader->FnHcdEnableRootHub        = EHCI_EnableRootHub;
    HcdHeader->FnHcdControlTransfer      = EHCI_ControlTransfer;
    HcdHeader->FnHcdBulkTransfer         = EHCI_BulkTransfer;
    HcdHeader->FnHcdIsocTransfer         = EHCI_IsocTransfer;
    HcdHeader->FnHcdInterruptTransfer    = EHCI_InterruptTransfer;
    HcdHeader->FnHcdDeactivatePolling    = EHCI_DeactivatePolling;
    HcdHeader->FnHcdActivatePolling      = EHCI_ActivatePolling;
    HcdHeader->FnHcdDisableKeyRepeat     = EHCI_DisableKeyRepeat;
    HcdHeader->FnHcdEnableKeyRepeat      = EHCI_EnableKeyRepeat;
    HcdHeader->FnHcdEnableEndpoints      = UsbHcdEnableEndpointsDummy;
    HcdHeader->FnHcdInitDeviceData       = UsbHcdInitDeviceDataDummy;
    HcdHeader->FnHcdDeinitDeviceData     = UsbHcdDeinitDeviceDataDummy;
    HcdHeader->FnHcdResetRootHub         = EHCI_ResetRootHub;
    HcdHeader->FnHcdClearEndpointState  = UsbHcdClearEndpointStateDummy;
    HcdHeader->FnHcdGlobalSuspend        = EHCI_GlobalSuspend;
    HcdHeader->FnHcdSmiControl           = EhciSmiControl;

    USB_InstallCallBackFunction(EhciRepeatTDCallback);
    USB_InstallCallBackFunction(EhciPollingTDCallback);

    return  USB_SUCCESS;
}


/**
    This routine locates EHCI debug port and determines whether
    or not the debug port is initialized and being used by other
    agents. If so, the global flag will be set to instruct the
    EHCI runtime routines about debug port presence and prevent
    any unwanted reset/reconfiguration of this port.

    @param HcStruc   Pointer to the host controller structure
                     HcStruc->DebugPort is updated if Debug Port is active on
                     this controller; otherwise it will remain 0.
                     
    @retval None

**/

VOID
EhciIsolateDebugPort(
    HC_STRUC *HcStruc
)
{
    UINT32 HcsParams = HcStruc->HcsParams;   // Host Controller Structural Parameters
    UINT8  DebugPortNo;
    UINT32 NextCap;
    UINT8  DebugPortBarIndex;
    UINT16 DebugPortOffset;

    //
    // Locate debug port by looking at the PCI capabilities
    //
    DebugPortNo = (UINT8)((HcsParams & (EHCI_DEBUG_N)) >> 20);

    HcStruc->DebugPort = 0;
    if (DebugPortNo == 0) return;

    ASSERT(DebugPortNo <= (UINT8)(HcsParams & (EHCI_N_PORTS)));    // Invalid debug port number
    if (DebugPortNo > (UINT8)(HcsParams & (EHCI_N_PORTS))) return;

    //
    // Check whether device implements Capability list that starts at register 0x34
    //
    if (!(HcReadPciReg(HcStruc, 4) & BIT20)) {
        //ASSERT(FALSE);  // Capabilities list is not implemented
        return;
    }

    //
    // Find the beginning of Debug Port registers block
    //
    for (NextCap = HcReadPciReg(HcStruc, 0x34);
            (UINT8)NextCap > 0;
    )
    {
        NextCap = HcReadPciReg(HcStruc, (UINT8)NextCap);
        if ((UINT8)NextCap == 0xA) break;   // Debug port capability found
        NextCap >>= 8;
    }
    if ((UINT8)NextCap == 0) {
        //ASSERT(FALSE);  // Debug capabilities not found
        return;
    }
    DebugPortBarIndex = (UINT8)((NextCap >> 29) - 1);
    DebugPortOffset = (UINT16)((NextCap >> 16) & 0x1FFF);
    ASSERT(DebugPortBarIndex <= 5); // Wrong BAR
    if (!(DebugPortBarIndex <= 5)) return;
    //
    // See whether Debug Port is acquired by other software
    //
    if (EhciReadDebugReg(HcStruc, DebugPortBarIndex, DebugPortOffset) & BIT28) {
      HcStruc->DebugPort = DebugPortNo;
      USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "EHCI HC Debug Port #%d enabled.\n", DebugPortNo);
    }
}


/**
    This API function is called to start a EHCI host controller.
    The input to the routine is the pointer to the HC structure
    that defines this host controller

    @param HcStruc   Pointer to the host controller structure

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
EFIAPI
EHCI_Start(
    HC_STRUC* HcStruc
)
{
    UINT32      Temp;
    UINT32      Index;          
    BOOLEAN     SetPortPower = FALSE;
    UINT16      PortReg;
    EFI_STATUS  Status;
    UINT32      UsbHcClassCode;

    Status = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(Status)) {
      return USB_ERROR;
    }

    // The address offset of class code register is in 09-0Bh, we get the value
    // from offset 08h for alignment and shift right 8 bits.
    UsbHcClassCode = HcReadPciReg(HcStruc, USB_REG_REVISION_ID);
    UsbHcClassCode = UsbHcClassCode >> 8;
    if (UsbHcClassCode != EHCI_CLASS_CODE) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "EHCI HC Class Code is wrong: %x\n", UsbHcClassCode);
        return USB_ERROR;
    }

    //
    // Get memory base address of the HC and store it in the HCStruc
    //
    HcStruc->BaseAddress = HcReadPciReg(HcStruc, USB_MEM_BASE_ADDRESS) & 0xFFFFFFF0;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "EHCI HC Mem Addr: %X\n", HcStruc->BaseAddress);
    
    Status = AmiUsbValidateMmioBuffer((VOID*)HcStruc->BaseAddress, HcStruc->BaseAddressSize);
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      USB_DEBUG(DEBUG_ERROR, 3, "Usb Mmio address is invalid, it is in SMRAM\n");
      return USB_ERROR;
    }
    Status = AmiUsbValidateMemoryBuffer((VOID*)HcStruc->FrameList, 0x1000);
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
      return USB_ERROR;
    }

    //
    // Get the number of ports supported by the host controller (Offset 4)
    // and store it in HCStruc
    //
    HcStruc->HcsParams = HcReadHcMem(HcStruc, EHCI_HCSPARAMS);
    HcStruc->NumPorts = (UINT8)(HcStruc->HcsParams & EHCI_N_PORTS);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "EHCI HC Number of ports: %d\n", HcStruc->NumPorts);

    EhciIsolateDebugPort(HcStruc);

    //
    // Read the Capability Registers Length to find the Offset address for the
    // beginning of the operational registers
    //
    HcStruc->OpRegOffset = (UINT8)HcReadHcMem(HcStruc, EHCI_VERCAPLENGTH);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "EHCI HC Operational Registers Offset: %d\n", HcStruc->OpRegOffset);

    //
    // Read and store the HCCPARAMS value
    //
    HcStruc->HccParams = HcReadHcMem(HcStruc, EHCI_HCCPARAMS);

    //
    // Get PCI register offset for the legacy support in EHCI controller
    // and store it in HC_STRUC
    //
    HcStruc->ExtCapPtr = EHCIGetLegacySupportOffset(
                        HcStruc,
                        HcStruc->BusDevFuncNum);


    if ((gUsbData->UsbFeature & USB_EHCI_64_BIT_DATA_STRUCTURE) == 0){
      //
      // 64bit data structures are not enabled. So check whether this host controller
      // needs 64bit data structure or not.
      //
      if (HcStruc->HccParams & EHCI_64BIT_CAP) {
        //
        // Engineer has to enable the 64bit capability. Post an error message
        //
        USBLogError(ERRUSB_EHCI_64BIT_DATA_STRUC);
        ASSERT(FALSE);
        
        //
        // Connect all ports to the classic host controller
        //
        HcClearOpReg(HcStruc, EHCI_CONFIGFLAG, BIT0);
        return  USB_ERROR;
        }
    }

    if ((gUsbData->UsbFeature & USB_HIDE_HISPEED_SETUP_QUESTION) == 0){
      if ((gUsbDataList->UsbSetupData->UsbHiSpeedSupport == 0) && ((HcStruc->HcsParams & EHCI_N_CC) != 0)) {
        HcClearOpReg(HcStruc, EHCI_CONFIGFLAG, BIT0);
        return USB_ERROR;
       }
    }


//----------------------------------------------------------------------------
// Note: after this point any access to the operational registers is through
// the macros EHCI_DWORD_READ_MEM and EHCI_DWORD_WRITE_MEM; access to the
// capability registers is through the macro USBPORT_DWORD_READ_MEM and
// there is no macro to write to the registers
//----------------------------------------------------------------------------

    if ((HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) == 0) {
      // Turn HC off and wait for the Halted bit to get set
      HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_RUNSTOP);

      // The Host Controller must halt within 16 micro-frames after 
      // software clears the Run bit. 
      for (Index = 0; Index < 16; Index++) {
        if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) {
          break;
        }
        FixedDelay(125);   // 125 us delay
      }
    }
    //
    // Reset the host controller (HC must be halted)
    //
    if (EHCIResetHC(HcStruc) == USB_ERROR) {
      return  USB_ERROR;  // HC reset error, error log is updated
    }

    //
    // Get the frame list size from the EHCI command register
    //
    Temp = HcReadOpReg(HcStruc, EHCI_USBCMD);
    Temp = (Temp & (BIT2 + BIT3)) >> 2;

    //
    // Calculate number of elements in the asynchronous list
    // and store the value in the HCStruc
    //
    switch (Temp) {
        case 0: 
            HcStruc->AsyncListSize   = 1024;
            break;
        case 1: 
            HcStruc->AsyncListSize   = 512;
            break;
        case 2: 
            HcStruc->AsyncListSize   = 256;
            break;
        case 3: 
        default:
            return USB_ERROR;

    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "EHCI AsyncListSize: %d\n", HcStruc->AsyncListSize);

    //
    // Set the max bulk data size
    //
    HcStruc->MaxBulkDataSize = MAX_EHCI_DATA_SIZE;

    //
    // Initialize the frame list pointers
    //
    USB_InitFrameList(HcStruc, EHCI_TERMINATE);

    //
    // Write the base address of the Periodic Frame List to the PERIODIC BASE
    // register
    //

    if (HcStruc->FrameListPhyAddr && HcStruc->FrameListMapping) {
        HcWriteOpReg(HcStruc, EHCI_PERIODICLISTBASE, (UINT32)(UINTN)HcStruc->FrameListPhyAddr);
    } else {
        HcWriteOpReg(HcStruc, EHCI_PERIODICLISTBASE, (UINT32)(UINTN)HcStruc->FrameList);
    }


    //
    // Initialize the periodic schedule
    //
    EHCIInitializePeriodicSchedule(HcStruc, (UINT32)HcStruc->BaseAddress);

    EhciEnableLegacyKeyRepeat(HcStruc);
    //
    // Clear status register - all R/WC bits
    //
    HcWriteOpReg(HcStruc, EHCI_USBSTS,
            EHCI_USB_INTERRUPT |        // Interrupt
            EHCI_USB_ERROR_INTERRUPT |  // Error interrupt
            EHCI_PORT_CHANGE_DETECT |   // Port Change Detect
            EHCI_FRAME_LIST_ROLLOVER |  // Frame List Rollover
            EHCI_HOST_SYSTEM_ERROR |    // Host System Error
            EHCI_INT_ASYNC_ADVANCE);    // Interrupt on Async Advance
    //
    // Program the HC BIOS owned bit and return the legacy support register offset
    //
    if (HcStruc->ExtCapPtr) {
      EHCIProgramLegacyRegisters(HcStruc, 1);  // Set HC BIOS owned semaphore

        //
        // Enable USB SMI, SMI on port change and SMI on ownership change
        //
      Temp = EHCI_SMI + EHCI_PORT_CHANGE_SMI + EHCI_OWNERSHIP_CHANGE_SMI;

      HcWritePciReg(HcStruc, HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG, Temp);
    }

    //
    // Turn HC on
    //
    HcSetOpReg(HcStruc, EHCI_USBCMD, \
        (EHCI_RUNSTOP | EHCI_PER_SCHED_ENABLE));

    // Wait for halt bit get cleared
    for (Index = 0; Index < 20; Index++) {
      if (!(HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED)) {
        break;
      }
      FixedDelay(100);   // 100 us delay
    }

    //
    // If the port has the power switch then enable the port. Otherwise 
    // Power for the port is already present. So don't need to enable the power.
    // ( Refer EHCI Spec 2.2.3 HCSPARAMS Structural Parameters Bit 4 (PPC) )
    if (HcStruc->HcsParams & EHCI_PPC) {
        //
        // Enable port power
        //
        for (Index = 1, PortReg = EHCI_PORTSC; Index <= HcStruc->NumPorts; Index++, PortReg += 4) {
            //
            // Skip enabling DebugPort
            //
            if (HcStruc->DebugPort && HcStruc->DebugPort == Index) continue;
    
            if (HcReadOpReg(HcStruc, PortReg) & EHCI_PORTPOWER) {
                continue;
            }
            
            HcSetOpReg(HcStruc, PortReg, EHCI_PORTPOWER);
            SetPortPower = TRUE;
        }
        //
        // Delay till the ports power is stabilised
        //
        if (SetPortPower) {
            FixedDelay(20 * 1000);        // 20 msec delay
        }
    }

    // Set HC flag as running
    HcStruc->HcFlag |= HC_STATE_RUNNING;

    // Set USB_FLAG_DRIVER_STARTED flag when HC is running.
    if (!(gUsbData->UsbStateFlag & USB_FLAG_DRIVER_STARTED)) {
        gUsbData->UsbStateFlag |= USB_FLAG_DRIVER_STARTED;
    }

    //
    // Disconnect all ports from companion HC (if any) and route them to EHCI
    //
    HcSetOpReg(HcStruc, EHCI_CONFIGFLAG, BIT0);  

    if (HcStruc->HcFlag & HC_STATE_SPECIFIC_CONTROLLER) {
        // Wait for port change detect bit set
        for (Index = 0; Index < 50; Index++) {
            if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_PORT_CHANGE_DETECT) {
                break;
            }
            FixedDelay(100);    // 100 us delay
        }
    } else {
        FixedDelay(100);    // 100 us delay
    }

    return  USB_SUCCESS;

}

/**
    This function programs the EHCI legacy registers as per the
    input. Also this routine returns the PCI register offset
    for the legacy support in EHCI controller

    @param HcStruc   Pointer to the host controller structure
    @param SetReset: Set owned semaphore bit.
                       0   Reset HC BIOS owned bit
                       1   Set HC BIOS owned bit

    @retval 0      If the feature is not present
    @retval Others Legacy support capability offset

**/

UINT8
EHCIProgramLegacyRegisters(
    HC_STRUC*   HcStruc,
    UINT8       SetReset
)
{
    UINT32  Temp;

    //
    // Check whether EHCI extended capabilities  pointer is present
    //
    if (!HcStruc->ExtCapPtr) {
        return 0;  // No extended capabilities are implemented.
    }

    //
    // Program 'HC BIOS owned semaphore bit'
    //
    Temp = HcReadPciReg(HcStruc, HcStruc->ExtCapPtr);
    Temp &= ~BIT16;

    if (SetReset) {
        Temp |= BIT16;
    }

                                        // (USB_S4_RESUME_ISSUE, EIP#20084)>
    if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI) {
        Temp &= ~BIT24;
    }
                                        // <(USB_S4_RESUME_ISSUE, EIP#20084)

    HcWritePciReg(HcStruc, HcStruc->ExtCapPtr, Temp);

    //
    // Reset all enable bits and clear the status
    //
    Temp = 0xE0000000 | EHCI_OWNERSHIP_CHANGE_SMI;

    HcWritePciReg(HcStruc, HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG, Temp);

    return HcStruc->ExtCapPtr;

}


/**
    This procedure clear EHCI legacy support status.

    @param HcStruc   Pointer to the host controller structure
    @param Status    Legacy status to clear

    @retval None

**/
VOID
ClearEECPstatus(
    HC_STRUC*   HcStruc,
    UINT16      Status
)
{
    UINT32  Temp;

    if (!HcStruc->ExtCapPtr) {
        return; // No extended capabilities are implemented.
    }

    //
    // Read control and status register
    //
    Temp = HcReadPciReg(HcStruc,
                HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG);

    //
    // Keep enable bits and set clear status bit
    //
    Temp = (Temp & 0xFFFF) | ((UINT32)Status << 16);
    HcWritePciReg(HcStruc, HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG, Temp);
    
}


/**
    This routine return USBLEGSUP register content. It could be
    used to check EHCI semaphore owened by BIOS or OS.

    @param HcStruc   Pointer to the host controller structure

    @retval Temp         Legacy support extended capability register content.
            0xFFFFFFFF   If no extended capabilities are implemented.

**/

UINT32
GetEhciUSBLEGSUP(
    HC_STRUC* HcStruc
)
{
    UINT32  Temp;

    //
    // Check whether EHCI extended capabilities  pointer is present
    //
    if (!HcStruc->ExtCapPtr) {
        return 0xFFFFFFFF;  // No extended capabilities are implemented.
    }

    //
    // Read Legacy support register
    //
    Temp = HcReadPciReg(HcStruc, HcStruc->ExtCapPtr);

    return Temp;
}


/**
    This function enumerates the HC ports for devices

    @param HcStruc   Pointer to the host controller structure

    @retval USB_SUCCESS  Finish to enumerates the HC ports.
    @retval Others       Fail to enumerates these ports.
**/

UINT8
EFIAPI
EHCI_EnumeratePorts(
    HC_STRUC* HcStruc
)
{
    UINT8   HcNumber;
    UINT8   PortNum;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return USB_ERROR;
    }

    if (EhciIsHalted(HcStruc)) {
      return USB_ERROR;
    }

    HcNumber = (UINT8)(HcStruc->HcNumber | BIT7);
    

    //
    // Enable port power so that new devices can be detected.
    //
    // Check whether enumeration flag is set by us or by somebody else by checking
    // local enum flag.
    //
    if (gUsbData->EnumFlag == FALSE) {
        gUsbData->IgnoreConnectStsChng    = TRUE;
        gUsbData->EnumFlag                = TRUE;
                                        //(EIP122174+)>
        do {
            //
            // Clear the EHCI_PCD bit of the interrupt status register EHCI_USBSTS
            //
            HcWriteOpReg(HcStruc, EHCI_USBSTS, EHCI_PORT_CHANGE_DETECT);

            //
            // Check the root hub ports to see if a device is connected.  If so, then
            // call USBCheckPortChange to handle the attachment of a new device.
            //
            for (PortNum = 1; PortNum <= HcStruc->NumPorts; PortNum++) {
                //
                // Skip DebugPort enumeration
                //
                if (HcStruc->DebugPort && HcStruc->DebugPort == PortNum) {
                    continue;
                }

                //
                // Process device connect/disconnect
                //
                USBCheckPortChange(HcStruc, HcNumber, PortNum);
            }
        } while ((HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_PORT_CHANGE_DETECT));
                                        //<(EIP122174+)
        gUsbData->IgnoreConnectStsChng = FALSE;

        //
        // Reset enumeration flag and enable hub enumeration
        //
        gUsbData->EnumFlag = FALSE;
    }

    //
    // Enable appropriate interrupts
    //
    HcWriteOpReg(HcStruc, EHCI_USBINTR, EHCI_USBINT_EN | EHCI_PCDINT_EN);
    return  USB_SUCCESS;
}


/**
    This function checks whether the host controller is still
    under BIOS

    @param HcStruc  Pointer to the host controller structure

    @retval USB_SUCCESS    If the control is with the BIOS
    @retval USB_ERROR      If the control is not with the BIOS

**/

UINT8
EHCICheckHCStatus(
    HC_STRUC* HcStruc
)
{
    UINT32  Cmd;
    UINT32  Sts;

    Cmd = HcReadOpReg(HcStruc, EHCI_USBCMD);
    Sts = HcReadOpReg(HcStruc, EHCI_USBSTS);

    // Don't read Periodic Frame List Base Address Register if the controller 
    // doesn't support periodic schedule.
    if (Cmd & EHCI_PER_SCHED_ENABLE) {
        if (!(Sts & EHCI_PER_SCHED_STATUS)) {
            return USB_SUCCESS;
        }
    }
    //
    // Check whether the controller is still under BIOS control
    // Read the base address of the Periodic Frame List to the PERIODIC BASE
    // register and compare with stored value
    //
    if (HcStruc->FrameListPhyAddr && HcStruc->FrameListMapping) {
        if ((UINTN)HcStruc->FrameListPhyAddr ==
            (HcReadOpReg(HcStruc, EHCI_PERIODICLISTBASE) & 0xFFFFF000)) {
            return USB_SUCCESS;    // Control is with BIOS
        }
    } else {
        if ((UINTN)HcStruc->FrameList ==
            (HcReadOpReg(HcStruc, EHCI_PERIODICLISTBASE) & 0xFFFFF000)) {
            return USB_SUCCESS;    // Control is with BIOS
        }
    }

    return USB_ERROR;  // HC is controlled by someone else
}


/**
    This function stops the EHCI controller.

    @param HcStruc        Pointer to the host controller structure

    @retval USB_SUCCESS   Success to stop EHCI controller.
    @retval USB_ERROR     Fail to stop EHCI controller.

**/

UINT8
EFIAPI 
EHCI_Stop(
    HC_STRUC* HcStruc
)
{
    UINT8   PortNum;
    UINT8   Status;
    UINT8   Index;
    EHCI_DESC_PTRS  *DescPtr;
    EFI_STATUS  EfiStatus;
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    EfiStatus = UsbHcStrucValidation(HcStruc);    
    if (EFI_ERROR(EfiStatus)) {
      return USB_ERROR;
    }


    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
      return USB_ERROR;
    }

    DescPtr = HcStruc->DescPtrs.EhciDescPtrs;

    if (((UINTN)DescPtr < (UINTN)gUsbDataList->MemBlockStart) ||
        (((UINTN)DescPtr + sizeof(EHCI_DESC_PTRS)) > (UINTN)MemBlockEnd)) {
      return USB_ERROR;
    }

    
    //
    // Check whether the control is with BIOS or not
    //
    if (EHCICheckHCStatus(HcStruc) == USB_SUCCESS) {    // Controlled by BIOS
      if (((gUsbData->UsbFeature & USB_SPECIFIC_EHCI_OWNERSHIP_CHANGE_MECHANISM) == 
            USB_SPECIFIC_EHCI_OWNERSHIP_CHANGE_MECHANISM) && 
            ((HcStruc->HcFlag & HC_STATE_OWNERSHIP_CHANGE_IN_PROGRESS) == 
            HC_STATE_OWNERSHIP_CHANGE_IN_PROGRESS)){
         UINT16  PortReg;
         UINT32  PortSts;
         UINT32  Data32;
                
         // Disconnect all the devices connected to its ports
         for (PortNum = 1; PortNum <= HcStruc->NumPorts; PortNum++) {
           USB_StopDevice(HcStruc, (UINT8)(HcStruc->HcNumber | BIT7), PortNum);
         }
                
         // Stop the asynchronous schedule
         EHCIStopAsyncSchedule(HcStruc);
                
         // Stop the periodic schedule
         EHCIStopPeriodicSchedule(HcStruc);
                
         for (PortNum = 1; PortNum <= HcStruc->NumPorts; PortNum++) {
           PortReg = (UINT16)((PortNum - 1) * 4 + EHCI_PORTSC);
           PortSts = HcReadOpReg(HcStruc, PortReg);               
           if (!(PortSts & EHCI_PORTENABLE)) {
             continue;
             }
           HcWriteOpReg(HcStruc, PortReg, PortSts | EHCI_SUSPEND);
          }
          FixedDelay(250);      // 250 us delay
                
          // Stop the host controller (Reset bit 0)
          HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_RUNSTOP);
                
         // The Host Controller must halt within 16 micro-frames after 
         // software clears the Run bit. 
         for (Index = 0; Index < 16; Index++) {
           if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) {
             break;
            }
           FixedDelay(125);          // 125 us delay
         }
                
         // Clear the SMI enable bits
         if (HcStruc->ExtCapPtr) {
           Data32 = HcReadPciReg(HcStruc, 
                                 HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG
                                 );
                
           HcWritePciReg(HcStruc,
                         HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG, Data32 & ~(0x3F));
                }
                
           // Clear the USBSTS register bits
           HcWriteOpReg(HcStruc, EHCI_USBSTS, HcReadOpReg(HcStruc, EHCI_USBSTS));
                
           // Clear the Configure Flag bit
           HcClearOpReg(HcStruc, EHCI_CONFIGFLAG, BIT0);
         } else {                                                                                       
             //
             // Disconnect all the devices connected to its ports
             //
             for (PortNum = 1; PortNum <= HcStruc->NumPorts; PortNum++) {
               USB_DisconnectDevice(HcStruc,
               (UINT8)(HcStruc->HcNumber | BIT7), PortNum);
             }
             if (HcStruc->DebugPort == 0) {
               //
               // Stop the host controller (Reset bit 0)
               //
                HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_RUNSTOP);                                                                                              
                 // The Host Controller must halt within 16 micro-frames after 
                 // software clears the Run bit. 
                 for (Index = 0; Index < 16; Index++) {
                   if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) {
                     break;
                     }
                   FixedDelay(125);      // 125 us delay
                 }
                                                                                                
                 //
                 // Reset the host controller
                 //
                 Status = EHCIResetHC(HcStruc);
                 ASSERT(Status == USB_SUCCESS);
              }
        }
        //
        // Program the HC BIOS owned bit and return the legacy
        // support register offset
        //
        EHCIProgramLegacyRegisters(HcStruc, 0);   // Reset HC BIOS owned semaphore
                                                    // ERROR CONDITION IS NOT HANDLED

        //
        // Clear the frame list pointers
        //
        USB_InitFrameList(HcStruc, EHCI_TERMINATE);

        //
        // Disable TD schedule and free the data structures
        //
        if (DescPtr->fpQHRepeat) {
            EhciMemFree(HcStruc, DescPtr->fpQHRepeat,
                GET_MEM_BLK_COUNT(sizeof(EHCI_QH) + sizeof(EHCI_QTD) ));
        }

        //
        // Free the scheduling QHs
        //
        EhciMemFree(HcStruc, DescPtr->PeriodicQh,        
                        GET_MEM_BLK_COUNT(1 * sizeof(EHCI_QH)));

        //
        // Free descriptor structure
        //
        EhciMemFree(HcStruc, DescPtr,
                        GET_MEM_BLK_COUNT_STRUC(EHCI_DESC_PTRS));

        USBKeyRepeat(HcStruc, 3);
    }
    else    // not controlled by BIOS
    {
        //
        // Program the HC BIOS owned bit and return the legacy
        // support register offset
        //
        EHCIProgramLegacyRegisters(HcStruc, 0);   // Reset HC BIOS owned semaphore
    }

    //
    // Set the HC state to stopped
    //
    HcStruc->HcFlag  &= ~(HC_STATE_RUNNING);

    CheckBiosOwnedHc();

    return USB_SUCCESS;
}


/**
    This function disables the HC interrupts

    @param HcStruc        Pointer to the host controller structure
    
    @retval USB_SUCCESS   Success to disable HC interrupts.
    @retval Others        Fail to disable HC interrupts.

**/

UINT8
EFIAPI
EHCI_DisableInterrupts (
    HC_STRUC*   HcStruc
)
{
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }
    //
    // Disable interrupt generation
    //
    HcClearOpReg(HcStruc, EHCI_USBINTR, EHCI_USBINT_EN | EHCI_PCDINT_EN);

    //
    // Stop periodic and asynchoronous schedule
    //
    EHCIStopAsyncSchedule(HcStruc);
    EHCIStopPeriodicSchedule(HcStruc);

    return  USB_SUCCESS;
}


/**
    This function enables the HC interrupts

    @param HcStruc        Pointer to the host controller structure

    @retval USB_SUCCESS   Success to enable HC interrupt.
    @retval Others        Fail to enable HC interrupt.

**/

UINT8
EFIAPI
EHCI_EnableInterrupts (
    HC_STRUC* HcStruc
)
{
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }
    //
    // Start periodic and asynchoronous schedule
    //
    EHCIStartAsyncSchedule(HcStruc);
    EHCIStartPeriodicSchedule(HcStruc);

    //
    // Enable interrupt generation
    //
    HcSetOpReg(HcStruc, EHCI_USBINTR, EHCI_USBINT_EN | EHCI_PCDINT_EN);

    return USB_SUCCESS;
}


/**
    Root hub change processing code

    @param HcStruc        Pointer to the host controller structure
    
    @retval USB_SUCCESS   Success to check port changes(Enumeration).
    @retval Others        Fail to check port changes(Enumeration).

**/

UINT8
ProcessRootHubChanges(
    HC_STRUC* HcStruc
)
{
    UINT8   PortNum;

    //
    // Check EnumFlag before enumerating devices behind root hub
    //
    if ((gUsbData->EnumFlag) == TRUE) {
        return USB_ERROR;
    }

    // The interval with a minimum duration of 100 ms when 
    // the USB System Software is notified of a connection detection.
    FixedDelay(USB_PORT_CONNECT_STABLE_DELAY_MS * 1000);

    //
    // Clear the port change bit of the interrupt status register EHCI_USBSTS
    //
    HcWriteOpReg(HcStruc, EHCI_USBSTS, EHCI_PORT_CHANGE_DETECT);

    //
    // Check all the ports on the root hub for any change in connect status.
    // If the connect status has been changed on either or both of these ports,
    // then call the  routine UsbHubPortChange for each changed port.
    //
    // Set enumeration flag and avoid hub port enumeration
    //
    gUsbData->EnumFlag = TRUE;

    for (PortNum = 1; PortNum <= HcStruc->NumPorts; PortNum++) {
        if (HcStruc->DebugPort && HcStruc->DebugPort == PortNum) {
            continue;
        }
        //
        // Process device connect/disconnect
        // Note: port connect status is cleared while processing
        // connect/disconnect (EHCIGetRootHubStatus)
        //
        USBCheckPortChange(HcStruc, (UINT8)(HcStruc->HcNumber | BIT7), PortNum);
    }

    //
    // Reset enumeration flag and enable hub enumeration
    //
    gUsbData->EnumFlag = FALSE;
    return USB_SUCCESS;
}

/**
    This function is called when the USB interrupt bit is
    set. This function will parse through the TDs and QHs to
    find out completed TDs and call their respective call
    back functions

    @param HcStruc       Pointer to the host controller structure

    @retval USB_ERROR    Need more Interrupt processing
    @retval USB_SUCCESS  No interrupts pending

**/

UINT8
EFIAPI
EHCI_ProcessInterrupt(
    HC_STRUC* HcStruc
)
{
    UINT32  Sts;
    UINT32  Temp;
    UINT16  Status;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }
    //
    // If EHCI extended capabilities  pointer is present,
    // then service OwnerShipChange SMI
    //
    if (HcStruc->ExtCapPtr) {
        //
        // Read control and status register
        //
        Temp = HcReadPciReg(
                HcStruc,
                HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG);
        Status = (UINT16)Temp;
        Status &= (UINT16)(Temp >> 16);   // "And" enable and status bits
        if (Status & EHCI_OWNERSHIP_CHANGE_SMI_STS) {
            ClearEECPstatus(HcStruc, Status);
            ProcessOwnerShipChangeSMI(HcStruc);
            return USB_SUCCESS; // Break from Interrupt process loop
        }
    }

    //
    // Check whether the controller is still under BIOS control
    // Read the base address of the Periodic Frame List to the PERIODIC BASE
    // register and compare with stored value
    //
    if (EHCICheckHCStatus(HcStruc) == USB_ERROR) {
        //
        // Control is not with us anymore, we should disable SMI generation
        // and come out.
        //
        if (HcStruc->ExtCapPtr) {
            //
            // Read control and status register
            //
            Temp = HcReadPciReg(
                        HcStruc,
                        HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG);
        
            //
            // Leave only Ownership SMI active.
            //
            Temp &= 0xE0002000; 
            HcWritePciReg(HcStruc, HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG,
                            Temp);
        }
        return USB_SUCCESS;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
      return USB_SUCCESS;
    }

    while (TRUE) {
        //
        // Get the interrupt status
        //
        Sts = HcReadOpReg(HcStruc, EHCI_USBSTS);

        //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "-->> %x <<--\n", Sts);

        if (Sts & EHCI_HOST_SYSTEM_ERROR) {
          gUsbData->UsbStateFlag &= (~USB_FLAG_ENABLE_BEEP_MESSAGE);
          EHCI_Start(HcStruc);
          EHCI_EnumeratePorts(HcStruc);
          gUsbData->UsbStateFlag |= USB_FLAG_ENABLE_BEEP_MESSAGE;
          continue;
         }

        if (Sts & EHCI_HCHALTED) {
          // Clear the USBSTS register bits
          HcWriteOpReg(HcStruc, EHCI_USBSTS, HcReadOpReg(HcStruc, EHCI_USBSTS));
          break;
        }
        
        //
        // Check for transaction complete
        //
        
        if ((gUsbData->ProcessingPeriodicList == TRUE) && (Sts & EHCI_USB_INTERRUPT)) {

            //
            // Clear the interrupt status
            //
            HcWriteOpReg(HcStruc, EHCI_USBSTS, EHCI_USB_INTERRUPT);

            //Section 4.4 Schedule traversal rules.
            //if the periodic schedule is enabled (see Section 4.6) then the host controller must 
            //execute from the periodic schedule before executing from the asynchronous schedule. 
            //It will only execute from the asynchronous schedule after it encounters the end of
            //the periodic schedule.

            //
            // Check and process periodic schedule
            //
            if (Sts & EHCI_PER_SCHED_STATUS) {
                //
                // Check the command register for Async status
                //
                Temp = HcReadOpReg(HcStruc, EHCI_USBCMD);

                if (Temp & EHCI_PER_SCHED_ENABLE) {
                    EHCIProcessPeriodicList(HcStruc);
                }
            }
            continue;
        }

        //
        // Check PORT_CHANGE_DETECT bit
        //
        if ((Sts & EHCI_PORT_CHANGE_DETECT )) {
            if (ProcessRootHubChanges(HcStruc) == USB_SUCCESS) {
                continue;
            }
        }
        break;  // No more statuses to process
    }
    return  USB_SUCCESS;    // Set as interrupt processed
}


/**
    This procedure process EHCI OwnerShipChange SMI.

    @param HcStruc   Pointer to the host controller structure

    @retval None

**/
VOID
ProcessOwnerShipChangeSMI(
    HC_STRUC* HcStruc
)
{
    UINT32 Temp = GetEhciUSBLEGSUP(HcStruc);
    
    HcStruc->HcFlag |= HC_STATE_OWNERSHIP_CHANGE_IN_PROGRESS;
    if (Temp & EHCI_HC_OS) {
        gUsbData->UsbStateFlag &= (~USB_FLAG_ENABLE_BEEP_MESSAGE);
        ProcessSmiChangeToEhcd(HcStruc);
    }
    else {
        gUsbData->UsbStateFlag |= USB_FLAG_ENABLE_BEEP_MESSAGE;
        ProcessSmiChangeToBios(HcStruc);
    }
   HcStruc->HcFlag &= ~(HC_STATE_OWNERSHIP_CHANGE_IN_PROGRESS);
}


/**
    This procedure process OwnerShipChange for BIOS -> EHCD.

    @param HcStruc   Pointer to the host controller structure

    @retval None

**/
VOID
ProcessSmiChangeToEhcd(
    HC_STRUC* HcStruc
)
{
    EHCI_Stop(HcStruc);    // Stop EHCI legacy
}


/**
    This procedure process OwnerShipChange for EHCD -> BIOS.

    @param  HcStruc   HCStruc pointer

    @retval None

**/

VOID
ProcessSmiChangeToBios (
    HC_STRUC    *HcStruc
)
{
    HC_STRUC    *Hc;
    UINT8       Count;
    DEV_INFO    *Device;

    // Stop UHCI devices connected to the companions
    for (Count = 1; Count < gUsbData->MaxDevCount; Count++) {
        Device = &gUsbDataList->DevInfoTable[Count];
        
        if ((Device->Flag & DEV_INFO_VALIDPRESENT) != DEV_INFO_VALIDPRESENT) {
          continue;
        }
        
        Hc = gHcTable[Device->HcNumber - 1];

        if (Hc->HcType != USB_HC_UHCI) {
            continue;   // Not UHCI
        }

        if ((Device->Flag & DEV_INFO_VALID_STRUC)==0) {
            continue;    // Not valid
        }

        if ((Hc->BusDevFuncNum & 0xfff8) !=
            (HcStruc->BusDevFuncNum & 0xfff8)) {
            continue; // Not a companion
        }

        // Found a device connected to UHCI companion controller. Stop it.
        USB_StopDevice(Hc, Device->HubDeviceNumber, Device->HubPortNumber);
    }

    EHCI_Start(HcStruc);  // Reinitialize EHCI host controller
}


/**

    @param HcStruc           Pointer to the host controller structure
    @param PortNum           Port in the HC whose status is requested

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure 

**/

UINT8
EHCI_ReleasePortOwner(
    HC_STRUC* HcStruc,
    UINT8  PortNum
)
{
    UINT16  PortReg = (UINT16)((PortNum - 1) * 4 + EHCI_PORTSC);
    UINT16  Index;

    if ((HcStruc->HcsParams & EHCI_N_CC) == 0) {
      return USB_SUCCESS;
    }

    if (!(HcReadOpReg(HcStruc, PortReg) & EHCI_CURRENTCONNECTSTATUS)) {
        return USB_ERROR;
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Release EHCI port %d\n", PortNum);
    HcSetOpReg(HcStruc, PortReg, EHCI_PORTOWNER);

    // Loop until Full speed device disconnect event process done.
    // This change is done in sync with Core8 except the extra 400mS delay
    for (Index = 0; Index < 200; Index++) {
        if (HcReadOpReg(HcStruc, PortReg) & EHCI_CONNECTSTATUSCHANGE) {
            break;
        }
        FixedDelay(100); 
    }

    HcSetOpReg(HcStruc, PortReg, EHCI_CONNECTSTATUSCHANGE);

    return USB_SUCCESS; 
}

/**
    This function returns the port connect status for the root hub port.

    @param HcStruc          Pointer to the host controller structure
    @param PortNum          Port in the HC whose status is requested
    @param ClearChangeBits  If true, set connect status change.
    
    @retval Status          Port status flags (see USB_PORT_STAT_XX equates)

**/

UINT32
EFIAPI
EHCI_GetRootHubStatus(
    HC_STRUC*   HcStruc,
    UINT8       PortNum,
    BOOLEAN     ClearChangeBits
)
{
    UINT32  Tmp;
    UINT32  Status = USB_PORT_STAT_DEV_OWNER;
    UINT16  PortReg = (UINT16)((PortNum - 1) * 4 + EHCI_PORTSC);
    EFI_STATUS  EfiStatus;
    UINT8   Index;
    UINT8   PortSpeed;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (HcStruc->DebugPort && HcStruc->DebugPort == PortNum) {
        return 0;
    }

    //
    // Read the status of the port
    //
    Tmp = HcReadOpReg(HcStruc, PortReg);
    USB_DEBUG(DEBUG_INFO, 3, "Ehci port[%d] status: %08x\n", PortNum, Tmp);

    // Detect the high-speed device.
    // In case of low-speed or full-speed change the ownership to a
    // companion 1.1 controller (if any)
    if (Tmp & EHCI_CURRENTCONNECTSTATUS) {
      // Analyze Line Status
      if ((Tmp & EHCI_LINE_STATUS) == EHCI_DMINUSBIT) { // Low speed device connected
        EHCI_ReleasePortOwner(HcStruc, PortNum);
        Tmp = HcReadOpReg(HcStruc, PortReg);
      }
    }

    //
    // Check the connect status change bit
    //
    if (Tmp & EHCI_CONNECTSTATUSCHANGE) {
        //
        // Set connect status change flag
        //
        Status |= USB_PORT_STAT_DEV_CONNECT_CHANGED;

        //
        // Wait 20ms for host controller could report accurate port status properly.
        //
        //FixedDelay(gUsbDataList->UsbTimingPolicy->EhciPortConnect * 1000);   // 20ms delay

        //
        // Read the status of the port
        //
        //dTmp = HcReadOpReg(HcStruc, wPortReg);

        // Clear connect status change
        if (ClearChangeBits == TRUE) {
          HcSetOpReg(HcStruc, PortReg, EHCI_CONNECTSTATUSCHANGE);  //(EIP61030+)
        }
    }

    if (Tmp & EHCI_CURRENTCONNECTSTATUS) {
        Status |= USB_PORT_STAT_DEV_CONNECTED;

        if (Tmp & EHCI_PORTENABLE) {
            PortSpeed = AMI_USB_PORT_STAT_HIGH_SPEED;
            for (Index = 0; EhciGetRootHubPortSpeedFunctions[Index]; Index++) {
                EhciGetRootHubPortSpeedFunctions[Index](HcStruc, PortNum, &PortSpeed);
            }
            switch (PortSpeed) {
                case AMI_USB_PORT_STAT_LOW_SPEED:
                    Status |= USB_PORT_STAT_DEV_LOWSPEED;
                    break;
                case AMI_USB_PORT_STAT_FULL_SPEED:
                    Status |= USB_PORT_STAT_DEV_FULLSPEED;
                    break;
                case AMI_USB_PORT_STAT_HIGH_SPEED:
                    Status |= USB_PORT_STAT_DEV_HISPEED;
                    break;
                default:
                    USB_DEBUG(DEBUG_ERROR, 3, "Get the undefined speed of the root hub port %x\n", PortSpeed);
                    break;
            }
            Status |= USB_PORT_STAT_DEV_ENABLED;
        } else {
            if (gUsbData->IgnoreConnectStsChng == TRUE) {
              if (!(Tmp & EHCI_CONNECTSTATUSCHANGE)) {
                Status |= USB_PORT_STAT_DEV_CONNECT_CHANGED;
              }
            }
        }
    }

    if (Tmp & EHCI_PORTOWNER) {
        Status &= ~USB_PORT_STAT_DEV_OWNER;
    }
    if (Tmp & EHCI_PORTPOWER) {
        Status |= USB_PORT_STAT_DEV_POWER;
    }
    if (Tmp & EHCI_OVERCURRENTACTIVE) {
        Status |= USB_PORT_STAT_DEV_OVERCURRENT;
    }
    if (Tmp & EHCI_SUSPEND) {
        Status |= USB_PORT_STAT_DEV_SUSPEND;
    }
    if (Tmp & EHCI_PORTRESET) {
        Status |= USB_PORT_STAT_DEV_RESET;
    }
    if (Tmp & EHCI_PORTENABLESTATUSCHANGE) {
        Status |= USB_PORT_STAT_DEV_ENABLE_CHANGED;
    }
    if (Tmp & EHCI_OVERCURRENTCAHGE) {
        Status |= USB_PORT_STAT_DEV_OVERCURRENT_CHANGED;
    }

    return Status;
}


/**
    This function disables the EHCI HC Ruoot hub port.

    @param HcStruc    Pointer to the host controller structure
    @param PortNum    Port in the HC to disable

    @retval USB_SUCCESS   Success to disable the root hub port.
    @retval Others        Fail to disable the root hub port.

**/

UINT8
EFIAPI
EHCI_DisableRootHub(
    HC_STRUC*   HcStruc,
    UINT8       PortNum
)
{
    UINT16  PortReg = (UINT16)((PortNum - 1) * 4 + EHCI_PORTSC);
    UINT32 Count;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (HcStruc->DebugPort && HcStruc->DebugPort == PortNum) {
        return USB_SUCCESS;
    }

    if (!(HcReadOpReg(HcStruc, PortReg) & EHCI_PORTENABLE)) {
      return USB_SUCCESS;
    }
    HcClearOpReg(HcStruc, PortReg, EHCI_PORTENABLE);

    for (Count = 0; Count < 100; Count++) {
       if ((HcReadOpReg(HcStruc, PortReg) & EHCI_PORTENABLE) == 0) {
         break;
    }
     FixedDelay(100);
 }
    return USB_SUCCESS;
}


/**
    This function enables the EHCI HC Root hub port.

    @param HcStruc     Pointer to the host controller structure
    @param PortNum     Port in the HC to enable.

    @retval USB_SUCCESS Success to enable the root hub port.
    @retval Others      Fail to enable the root hub port.

**/

UINT8
EFIAPI
EHCI_EnableRootHub(
    HC_STRUC*   HcStruc,
    UINT8       PortNum
)
{
//
// Software can only enable the EHCI root hub ports by port RESET.  HC will
// enable the port only if it is a high speed device
//
    return USB_SUCCESS;
}

/**
    This function resets the EHCI HC Root hub port.

    @param HcStruc      Pointer to the host controller structure
    @param PortNum      Port in the HC to enable

    @retval USB_SUCCESS Success to reset the root hub port.
    @retval Others      Fail to reset the root hub port.

**/

UINT8
EFIAPI
EHCI_ResetRootHub(
    HC_STRUC* HcStruc,
    UINT8  PortNum
)
{
    UINT16  PortReg = (UINT16)((PortNum - 1) * 4 + EHCI_PORTSC);
    UINT32 Count;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    // Disable the port if it is enabled
    if (HcReadOpReg(HcStruc, PortReg) & EHCI_PORTENABLE) {
        HcClearOpReg(HcStruc, PortReg, EHCI_PORTENABLE);
 
        // There may be a delay in disabling or enabling a port due to other 
        // host controller and bus events.
        for (Count = 0; Count < 100; Count++) {
            if ((HcReadOpReg(HcStruc, PortReg) & EHCI_PORTENABLE) == 0) {
                break;
            }
            FixedDelay(100);    // 100 us delay
        }
    }

    // Reset the port
    HcSetOpReg(HcStruc, PortReg, EHCI_PORTRESET);

    if ((HcStruc->HcFlag & HC_STATE_SPECIFIC_CONTROLLER) && (PortNum == 1)) {
        FixedDelay(3 * 1000);  // 3 ms delay
    } else {
        // Wait til port disable is complete (Tdrstr=50ms Ref 7.1.7.5 of USB Spec 2.0)
        FixedDelay(50 * 1000);  // 50 ms delay        
    }

     HcClearOpReg(HcStruc, PortReg, EHCI_PORTRESET); // Terminate reset

    if (!(HcReadOpReg(HcStruc, PortReg) & EHCI_CURRENTCONNECTSTATUS)) {
        return USB_ERROR;
    }

    // if the port detects that the attached device is high-speed during reset, 
    // then the host controller must have the port in the enabled state within 2ms 
    // of software writing this bit to a zero.
    for (Count = 0; Count < 20; Count++) {
        if ((HcReadOpReg(HcStruc, PortReg) & (EHCI_PORTRESET | 
            EHCI_PORTENABLE)) == EHCI_PORTENABLE) {
            break;
        }
        FixedDelay(100);  // 100 us delay
    }

    if (HcReadOpReg(HcStruc, PortReg) & EHCI_PORTRESET) {  // Reset failed
        USBLogError(USB_ERR_PORT_RESET_FAILED);
        return USB_ERROR;
    }

    if (!(HcReadOpReg(HcStruc, PortReg) & EHCI_PORTENABLE)) {
        if (!(HcReadOpReg(HcStruc, PortReg) & EHCI_CURRENTCONNECTSTATUS)) {
          return USB_ERROR;
        } else {
           EHCI_ReleasePortOwner(HcStruc, PortNum);
           return USB_ERROR;
        }
    }

    FixedDelay(1 * 1000);  // 1 ms delay

    return USB_SUCCESS;
}

/**
    This function is call to control SMI enable/disable for EHCI.

    @param HcStruc      Pointer to the host controller structure
    @param Enable       If true, enable SMI(EHCI_SMI|EHCI_PORT_CHANGE_SMI|EHCI_OWNERSHIP_CHANGE_SMI).
                        If false, reset all enable bits and clear the status.
                        
    @retval USB_SUCCESS Success to set SMI enable/disable.
    @retval Others      Fail to set SMI enable/disable.

**/
UINT8
EFIAPI
EhciSmiControl(
    HC_STRUC* HcStruc,
    BOOLEAN Enable
)
{
    UINT32 LegCtlStsReg;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return USB_ERROR;
    }

    if (HcStruc->HwSmiHandle == NULL) {
        return USB_SUCCESS;
    }

    if (Enable == TRUE) {
        LegCtlStsReg = EHCI_SMI + EHCI_PORT_CHANGE_SMI + EHCI_OWNERSHIP_CHANGE_SMI;

        HcWritePciReg(HcStruc, HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG, LegCtlStsReg);
    } else {
        //
        // Reset all enable bits and clear the status
        //
        LegCtlStsReg = 0xE0000000;

        HcWritePciReg(HcStruc, HcStruc->ExtCapPtr + EHCI_LEGACY_CTRL_STS_REG, LegCtlStsReg);
    }

    return USB_SUCCESS;
}

/**
    This function suspend the EHCI HC.
    @param HcStruc      Pointer to the host controller structure
    
    @retval USB_SUCCESS Success to suspend the EHCI HC.
    @retval Others      Fail to suspend the EHCI HC.
**/

UINT8
EFIAPI
EHCI_GlobalSuspend(
    HC_STRUC* HcStruc
)
{
    UINT16  PortReg;
    UINT32 PortSts;
    UINT8   PortNum;
    UINT8   Count;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return USB_ERROR;
    }
    
    if (EhciIsHalted(HcStruc)) {
      return USB_ERROR;
    }

    for (PortNum = 1; PortNum <= HcStruc->NumPorts; PortNum++) {
        PortReg = (UINT16)(EHCI_PORTSC + (PortNum - 1) * 4 );
        PortSts = HcReadOpReg(HcStruc, PortReg );
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3,"EHCI PortSts[%x] %x \n",
                        PortNum, PortSts); 
        // Check if port is disabled or suspended.
        if ((PortSts & EHCI_PORTENABLE) && (!(PortSts & EHCI_SUSPEND))) {
            // Suspend if necessary.
            HcClearOpReg(HcStruc, PortReg,
                            EHCI_WKOC_E | EHCI_WKDSCNNT_E | EHCI_WKCNNT_E);
            HcSetOpReg(HcStruc, PortReg, EHCI_SUSPEND);
            // Read PortSc until port shows suspended. 
            for (Count = 0; Count < 100; Count++) {
                if (HcReadOpReg(HcStruc, PortReg) & EHCI_SUSPEND) {
                    break;
                }
                FixedDelay(100);      // 100 us delay
            } 
        }
    }

    // Turn HC off and wait for the Halted bit to get set
    HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_RUNSTOP);
    // The Host Controller must halt within 16 micro-frames after 
    // software clears the Run bit. 
    for (Count = 0; Count < 16; Count++) {
        if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) {
            break;
        }
        FixedDelay(125);      // 125 us delay
    }

    EHCIProgramLegacyRegisters(HcStruc, 0); // Reset HC BIOS owned semaphore

    HcStruc->HcFlag &= ~(HC_STATE_RUNNING);
    HcStruc->HcFlag |= HC_STATE_SUSPEND;

    return USB_SUCCESS;
}


/**
    This function gets the hi-speed hub's device and port number
    to which this low speed device is connected.  It parses
    through its parents until it finds the correct device. This
    information is used for split transaction

    @param DevInfo    Device info pointer of the device

    @retval RetCode   Device/port number of the hi-speed hub

    @note  This low/full speed device may be behind different hubs as
          shown below. In any case this routine will get the device
          address of the hub number HISP_A :
      Notations used:
          MBPortX     Motherboard USB port
          HISP_X      Hi-speed hub number X
          FUSP_X      Full-speed hub number X
          Device      Low/Full speed device
      Config 1:
        MBPortX --> HISP_A --> Device
      Config 2:
        MBPortX --> HISP_A --> FUSP_1 --> Device
      Config 3:
        MBPortX --> HISP_B --> HISP_A --> Device
      Config 4:
        MBPortX --> HISP_A --> FUSP_1 --> HISP_B --> Device
        In the above configuration the HISP_B will be operated in
      full speed rather than hi-speed since it is connected to a
      full speed hub
**/

UINT16
EHCI_GetHiSpeedHubPortNumber(
    DEV_INFO* DevInfo
)
{
    DEV_INFO*   HubDev = DevInfo;
    DEV_INFO*   ParentHubDev;
    UINT16      RetCode;

    if (!VALID_DEVINFO(DevInfo)) {
        return 0;
    }

//
// Get the device info structure for the matching device address
//
    //
    // Get the device number of the immediate hub, then get the device
    // info structure for this device number
    //
    for(;;) {
        ParentHubDev = USB_GetDeviceInfoStruc(USB_SRCH_DEV_ADDR,
                                0, HubDev->HubDeviceNumber, 0);
        if (!ParentHubDev) {
            return 0; // Error. Exit !
        }
        if (((ParentHubDev->EndpointSpeed << USB_PORT_STAT_DEV_SPEED_MASK_SHIFT)
             & USB_PORT_STAT_DEV_SPEED_MASK) == 0) {
             break;
        }
        HubDev = ParentHubDev;
    }
    //
    // The first USB 2.0 hub found as fpHubDev to which the low/full speed
    // device is connected
    //
    RetCode = (UINT16)((HubDev->HubPortNumber << 7) |
                                HubDev->HubDeviceNumber);

    return RetCode;
}

/**
    This function insert the requested QH to asynchronous schedule
    and waits until the QH completes or the transaction time-out.

    @param HcStruc    Pointer to the host controller structure
    @param XferQh     Pointer to the QH which has to be completed

    @retval USB_SUCCESS On success.
    @retval Others      On fail.

**/

UINT16
EhciExexuteAsyncSchedule(
    HC_STRUC *HcStruc,
    EHCI_QH  *XferQh
)
{
    UINT16  Status = USB_SUCCESS;
    UINT32  Count;
    UINT32  TimeOut = gUsbData->UsbReqTimeOutValue * 100; // in 10 microsecond unit
    EHCI_QH *XferQhPhyAddr;
    //
    // Set the ASYNCLISTADDR register to point to the Control/Bulk QH
    //
    XferQhPhyAddr = EhciMemGetPhyAddr(HcStruc, XferQh);
    HcWriteOpReg(HcStruc, EHCI_ASYNCLISTADDR, (UINT32)(UINTN)XferQhPhyAddr);

    //
    // Set next QH pointer to itself (circular link)
    //
    XferQh->dLinkPointer = (UINT32)((UINTN)XferQhPhyAddr | EHCI_QUEUE_HEAD);
    XferQh->bActive = TRUE;
    //
    // Now put the Control/Bulk QH into the HC's schedule by
    // setting the Async. schedule enabled field of USBCMD register
    // This will cause the HC to execute the transaction in the next active frame.
    //
    Status = EHCIStartAsyncSchedule(HcStruc);
    
    if (Status == USB_ERROR) {
        return Status;
    }

    // Wait for tansfer complete
    for (Count = 0; !TimeOut || Count < TimeOut; Count++) {
        EHCIProcessQH(HcStruc, XferQh);
        if (XferQh->bActive == FALSE) {
            break;
        }
        FixedDelay(10);  // 10 microsec
    }

    //
    // Stop the Async transfer
    //
    EHCIStopAsyncSchedule(HcStruc);

    if (XferQh->bActive == TRUE) {
        XferQh->bActive = FALSE;
        Status = USB_ERROR;
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "EHCI Time-Out\n");
    }
 
    // Service all interrupts
    EHCI_ProcessInterrupt(HcStruc);

    return Status;
}

/**
    This function executes a device request command transaction
    on the USB. One setup packet is generated containing the
    device request parameters supplied by the caller.  The setup
    packet may be followed by data in or data out packets
    containing data sent from the host to the device
    or vice-versa. This function will not return until the
    request either completes successfully or completes in error
    (due to time out, etc.)

    @param HcStruc   Pointer to the host controller structure
    @param DevInfo    DeviceInfo structure (if available else 0)
    @param Request    Request type (low byte)
           Bit 7   : Data direction
                     0 = Host sending data to device
                     1 = Device sending data to host
           Bit 6-5 : Type
                     00 = Standard USB request
                     01 = Class specific
                     10 = Vendor specific
                     11 = Reserved
           Bit 4-0 : Recipient
                     00000 = Device
                     00001 = Interface
                     00010 = Endpoint
                     00100 - 11111 = Reserved
    @param Request   A one byte code describing the actual device request to be executed.
                     (ex: Get Configuration, Set Address etc)
    @param Index     wIndex request parameter (meaning varies)
    @param Value     wValue request parameter (meaning varies)
    @param Buffer    Buffer containing data to be sent to the device or buffer to be used to receive data.
    @param Length    wLength request parameter, number of bytes of data to be transferred in or out of the host controller.

    @retval Length   Number of bytes actually transferred

**/
UINT16
EFIAPI
EHCI_ControlTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT16      Request,
    UINT16      Index,
    UINT16      Value,
    UINT8       *Buffer,
    UINT16      Length
)
{
    UINT32      Tmp;
    UINT32      Tmp1;
    UINT16      RetCode = 0;   // Initialize with error
    EHCI_QH     *QhCtl;
    EHCI_QTD    *QtdCtlSetup;
    EHCI_QTD    *QtdCtlData;
    EHCI_QTD    *QtdCtlStatus;
    DEV_REQ     *DevRequest = NULL;
    UINT8       EndpointSpeed;
    UINT8       *BufPhyAddr = NULL;
    VOID        *BufferMapping = NULL;
    UINT8       *DevRequestPhyAddr = NULL;
    UINT8       *QtdCtlSetupPhyAddr = NULL;
    UINT8       *QtdCtlDataPhyAddr = NULL;
    UINT8       *QtdCtlStatusPhyAddr = NULL;
    EFI_STATUS  Status;
    
    Status = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(Status)) {
        return 0;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        return 0;
    }

    if (gCheckUsbApiParameter) {
        if (Length != 0) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)Buffer, Length);
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                USB_DEBUG(DEBUG_ERROR, 3, "Ehci ControlTransfer Invalid Pointer, Buffer is in SMRAM.\n");
                return 0;
            }
        }
        gCheckUsbApiParameter = FALSE;
    }
    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return 0;
    }

    if (EhciIsHalted(HcStruc)) {
        return 0;
    }

    if (!VALID_DEVINFO(DevInfo)) {
        return 0;
    }
    
    gUsbData->UsbLastCommandStatusExtended = 0; 

    //
    // Build the device request in the data area of the control setup qTD
    //
    DevRequest = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT(sizeof(DEV_REQ)));
    
    ASSERT(DevRequest);
    
    if (DevRequest == NULL) {
        return 0;
    }

    DevRequestPhyAddr = EhciMemGetPhyAddr(HcStruc, DevRequest);

    DevRequest->RequestType = Request;
    DevRequest->Index       = Index;
    DevRequest->Value       = Value;
    DevRequest->DataLength  = Length;

//
// The QH endpoint characteristic field will be set so
//   Function address & Endpoint number = From DeviceInfo structure,
//   Direction = From TD,
//   Speed = DeviceInfo.EndpointSpeed,
//   Skip = 1, Format = 0,
//   Max packet size  = DeviceInfo.Endp0MaxPacket
// The dNextqTDPtr field will be set to qTDControlSetup
// The dAltNextqTDPtr field will be set to EHCI_TERMINATE
// The dCurrentqTDPtr field will be set to 0
//
    //
    // Intialize the queue head with null pointers
    //
    QhCtl = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT((1 * sizeof(EHCI_QH))+
                (3 * sizeof(EHCI_QTD))));

    if (!QhCtl) {
        return 0;
    }

    QtdCtlSetup = (EHCI_QTD*)((UINTN)QhCtl + sizeof(EHCI_QH));
    QtdCtlData = (EHCI_QTD*)((UINTN)QtdCtlSetup + sizeof(EHCI_QTD));
    QtdCtlStatus = (EHCI_QTD*)((UINTN)QtdCtlData + sizeof(EHCI_QTD));
    QtdCtlSetupPhyAddr = EhciMemGetPhyAddr(HcStruc, QtdCtlSetup);
    QtdCtlDataPhyAddr = EhciMemGetPhyAddr(HcStruc, QtdCtlData);
    QtdCtlStatusPhyAddr = EhciMemGetPhyAddr(HcStruc, QtdCtlStatus);

    EHCIInitializeQueueHead(QhCtl);

    EndpointSpeed = DevInfo->EndpointSpeed; // 00/01/10 for HI/LO/FULL

    //
    // Assume as a high speed device
    //
    Tmp = QH_HIGH_SPEED;   // 10b - High speed

    //
    // Check for high speed
    //
    if (EndpointSpeed) { // Low/Full speed device
        Tmp = ((UINT32)EndpointSpeed & 1) << 12;  // Bit 12 = full/low speed flag
        Tmp |= QH_CONTROL_ENDPOINT;
        //
        // Set the hub address and port number
        // Get the Hispeed hub port number & device number
        //
        Tmp1 = (UINT32)EHCI_GetHiSpeedHubPortNumber(DevInfo);
        Tmp1 = (Tmp1 << 16);  // Split complete Xaction
        QhCtl->dEndPntCap |= Tmp1;
    }

    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Tmp1..%x\n", dTmp);

    //
    // Use data toggle from qTD and this QH is the head of the queue
    //
    Tmp |= QH_USE_QTD_DT;
    // Do not set QH_HEAD_OF_LIST bit on VIA controller
    if (HcStruc->Vid != 0x1106) {
        Tmp |= QH_HEAD_OF_LIST;
    }
    Tmp |= (UINT32)DevInfo->DeviceAddress;
    //
    // dTmp[Bits 6:0] = Dev. Addr
    // dTmp[Bit7] = I bit(0)
    // dTmp[Bits11:8] = Endpoint (0)
    //

    Tmp1 = (UINT32)DevInfo->Endp0MaxPacket;
    Tmp |= (Tmp1 << 16);  // Tmp[Bits26:16] = device's packet size
    QhCtl->dEndPntCharac  = Tmp;

    //
    // Fill in various fields in the qTDControlSetup.
    //
    //fpQTDCtlSetup = HcStruc->DescPtrs.EhciDescPtrs->fpqTDControlSetup; //(EIP71067-)

    //
    // The token field will be set so
    //   Direction PID = QTD_SETUP_TOKEN,
    //   Size = size of the data,
    //   Data Toggle = QTD_SETUP_TOGGLE,
    //   Error Count = QTD_THREE_ERRORS,
    //   Status code = QTD_DO_OUT + QTD_ACTIVE
    // The buffer pointers field will point to the aControlSetupData buffer
    //   which was before initialized to contain a DeviceRequest struc.
    // The dNextqTDPtr field will point to the qTDControlData if data will
    //   be sent/received or to the qTDControlStatus if no data is expected.
    // The dAltNextqTDPtr field will be set to EHCI_TERMINATE
    //
    QtdCtlSetup->dToken = QTD_SETUP_TOKEN |
                          QTD_SETUP_TOGGLE | QTD_IOC_BIT |
                          QTD_THREE_ERRORS | QTD_DO_OUT | QTD_ACTIVE |
                          (8 << 16);  // Data size

    //
    // Update buffer pointers
    //
    EHCISetQTDBufferPointers(QtdCtlSetup, (UINT8*)DevRequestPhyAddr, 8);
    //fpQTDCtlData = HcStruc->DescPtrs.EhciDescPtrs->fpqTDControlData; //(EIP71067-)

    if (Length) {      // br if no data to transfer
        //
        // Fill in various fields in the qTDControlData
        //
        // The token field will be set so
        //   Direction PID = QTD_OUT_TOKEN/QTD_IN_TOKEN,
        //   Size = size of the data,
        //   Data Toggle = QTD_DATA1_TOGGLE,
        //   Error Count = QTD_THREE_ERRORS,
        //   Status code = QTD_DO_OUT(if it is out) + QTD_ACTIVE
        // The buffer pointers field will point to the FpBuffer buffer
        //   which was before initialized to contain a DeviceRequest struc.
        // The dNextqTDPtr field will point to the qTDControlSetup
        // The dAltNextqTDPtr field will be set to EHCI_TERMINATE
        //
        QtdCtlData->dToken = QTD_IN_TOKEN |
                QTD_DATA1_TOGGLE | QTD_IOC_BIT |
                QTD_THREE_ERRORS | QTD_ACTIVE;
        if (!(Request & BIT7)) // Br if host sending data to device (OUT)
        {
            QtdCtlData->dToken    = QTD_OUT_TOKEN |
                QTD_DATA1_TOGGLE | QTD_IOC_BIT |
                QTD_THREE_ERRORS | QTD_DO_OUT | QTD_ACTIVE;
        }

        //
        // Set length
        //
        QtdCtlData->dToken |= ((UINT32)Length << 16);

        HcDmaMap(HcStruc, (UINT8)(Request & BIT7), Buffer, Length, 
                 &BufPhyAddr, &BufferMapping);
        
        //
        // Update buffer pointers
        //
        EHCISetQTDBufferPointers(QtdCtlData,
                (UINT8*)BufPhyAddr,
                (UINT32)Length);
    }

    //
    // Fill in various fields in the qTDControlStatus
    //
    //fpQTDCtlStatus = HcStruc->DescPtrs.EhciDescPtrs->fpqTDControlStatus; 

    //
    // The token field will be set so
    //   Direction PID = QTD_OUT_TOKEN/QTD_IN_TOKEN,
    //   Size = 0,
    //   Data Toggle = QTD_DATA1_TOGGLE,
    //   Error Count = QTD_THREE_ERRORS,
    //   Status code = QTD_DO_OUT(if it is out) + QTD_ACTIVE
    // The buffer pointers field will be 0
    // The dNextqTDPtr field will set to EHCI_TERMINATE
    // The dAltNextqTDPtr field will be set to EHCI_TERMINATE
    //
    // For OUT control transfer status should be IN and
    // for IN cotrol transfer, status should be OUT
    //
    QtdCtlStatus->dToken = QTD_IN_TOKEN |
                           QTD_DATA1_TOGGLE | QTD_IOC_BIT |
                           QTD_THREE_ERRORS | QTD_ACTIVE;
    if (Request & BIT7) {
        QtdCtlStatus->dToken  = QTD_OUT_TOKEN |
                                QTD_DATA1_TOGGLE | QTD_IOC_BIT |
                                QTD_THREE_ERRORS | QTD_DO_OUT | QTD_ACTIVE;
    }

    EHCISetQTDBufferPointers(QtdCtlStatus, NULL, 0);

    //
    // Link the qTD formed now and connect them with the control queue head
    //
    QhCtl->fpFirstqTD     = QtdCtlSetup;
    QhCtl->dNextqTDPtr    = (UINT32)(UINTN)QtdCtlSetupPhyAddr;

    if(Length) {
        QtdCtlSetup->dNextqTDPtr  = (UINT32)(UINTN)QtdCtlDataPhyAddr;
        QtdCtlData->dNextqTDPtr   = (UINT32)(UINTN)QtdCtlStatusPhyAddr;
    } else {
        QtdCtlSetup->dNextqTDPtr  = (UINT32)(UINTN)QtdCtlStatusPhyAddr;
    }

    QtdCtlStatus->dNextqTDPtr = EHCI_TERMINATE;

    RetCode = EhciExexuteAsyncSchedule(HcStruc, QhCtl);
    QhCtl->fpFirstqTD     = 0;
    QhCtl->dNextqTDPtr    = EHCI_TERMINATE;

    if (Length) {
        HcDmaUnmap(HcStruc, BufferMapping);
        Length = Length - (UINT16)((QtdCtlData->dToken & ~(QTD_DATA_TOGGLE)) >> 16);
    }

    //
    // Clear the stalled condition flag
    //
    gUsbData->UsbLastCommandStatus &= ~USB_CONTROL_STALLED;

    //
    // Check whether the QH stopped or timed out
    //
    if (RetCode != USB_SUCCESS) {
        gUsbData->UsbLastCommandStatusExtended |= USB_TRNSFR_TIMEOUT; 
        Length = 0;                    
    }

    if (QhCtl->bErrorStatus & QTD_HALTED) {
        //
        // Command stalled set the error bit appropriately
        //
        gUsbData->UsbLastCommandStatus   |= USB_CONTROL_STALLED;
        gUsbData->UsbLastCommandStatusExtended  |= USB_CONTROL_STALLED;
        Length = 0;                    
    }
                                       
    EhciMemFree(HcStruc, QhCtl, GET_MEM_BLK_COUNT((1 * sizeof(EHCI_QH))+
                                           (3 * sizeof(EHCI_QTD))));
                                       
    EhciMemFree(HcStruc, DevRequest, GET_MEM_BLK_COUNT(sizeof(DEV_REQ)));

    return Length;
}


/**
    This function executes a bulk transaction on the USB. The
    transfer may be either DATA_IN or DATA_OUT packets containing
    data sent from the host to the device or vice-versa. This
    function will not return until the request either completes
    successfully or completes with error (due to time out, etc.)
    @note  Make sure that amount of bytes to transfer should not
              exceed MAX_EHCI_DATA_SIZE

    @param HcStruc    Pointer to the host controller structure
    @param DevInfo    DeviceInfo structure (if available else 0)
    @param XferDir    Transfer direction
                      Bit 7   : Data direction
                                 0 Host sending data to device
                                 1 Device sending data to host
                      Bit 6-0 : Reserved
    @param Buffer     Buffer containing data to be sent to the
                      device or buffer to be used to receive data
                      value in Segment:Offset format
    @param Length     dwLength request parameter, number of bytes
                      of data to be transferred in or out
                      of the host controller

    @retval BytesTransferred  Amount of data transferred

**/

UINT32
EFIAPI
EHCI_BulkTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length
)
{
    UINT16      MaxPkt;
    UINT8       Endp;
    UINT8       DatToggle;
    EHCI_QH     *QhBulk;
    EHCI_QTD    *QtdBulkData;
    UINT32      Tmp;
    UINT32      Tmp1;
    UINT16      EhciCmdStatus;
    UINT32      BytesToTransfer;
    UINT32      BytesRemaining;
    UINT32      BytesTransferred;
    UINT8       *BufPhyAddr = NULL;
    VOID        *BufferMapping = NULL;
    UINT8       *TempBuffer = NULL;
    UINT8       *QhBulkPhyAddr;
    UINT8       *QtdBulkDataPhyAddr;
    EFI_STATUS  Status;

    Status = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(Status)) {
        return 0;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        return 0;
    }

    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)Buffer, Length);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "Ehci BulkTransfer Invalid Pointer, Buffer is in SMRAM.\n");
            return 0;
        }
        gCheckUsbApiParameter = FALSE;
    }


    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return 0;
    }

    if (EhciIsHalted(HcStruc)) {
      return 0;
      }

    if (!VALID_DEVINFO(DevInfo)) {
        return 0;
    }

    // Realtek 8111EP EHCI controller workaround
    // The controller doesn't work if the buffer address isn't DWORD alignment 
    // (current offset of qTD). Provide the workaround to locate DWORD alignment buffer.
    
    if ((HcStruc->Vid == 0x10EC) && (HcStruc->Did == 0x816D)) {
        if ((UINTN)Buffer & (BIT0 | BIT1)) {
            if (Length < gUsbData->HighSpeedMaxBulkDataSize) {
                TempBuffer = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT((UINT16)Length));
            } else {
                TempBuffer = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT(gUsbData->HighSpeedMaxBulkDataSize));  
            }
        }
    }

    //clear HW source of error
    gUsbData->UsbLastCommandStatusExtended = 0;


    BytesRemaining     = Length;
    BytesTransferred   = 0;

    //
    // Get Bulk IN/OUT enpoint number, data sync value & max packet size
    //
    if (XferDir & BIT7) {
        MaxPkt = DevInfo->BulkInMaxPkt;
        Endp   = DevInfo->BulkInEndpoint;
    } else {
        MaxPkt = DevInfo->BulkOutMaxPkt;
        Endp   = DevInfo->BulkOutEndpoint;
    }
    if (MaxPkt == 0) {
        return 0;
    }

    QhBulk = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT((1 * sizeof(EHCI_QH))+
                                                     (1 * sizeof(EHCI_QTD))));

    if (!QhBulk) {
        return 0;
    }

    QtdBulkData = (EHCI_QTD*)((UINTN)QhBulk + sizeof(EHCI_QH));

    QhBulkPhyAddr = EhciMemGetPhyAddr(HcStruc, QhBulk);
    QtdBulkDataPhyAddr = EhciMemGetPhyAddr(HcStruc, QtdBulkData);

    HcDmaMap(HcStruc, XferDir, Buffer, Length, &BufPhyAddr, &BufferMapping);

    while (BytesRemaining) {
        BytesToTransfer = 
             (BytesRemaining < gUsbData->HighSpeedMaxBulkDataSize)?
                    BytesRemaining : gUsbData->HighSpeedMaxBulkDataSize;
  
        //
        // Get data toggle value
        //
        DatToggle = UsbGetDataToggle(DevInfo, Endp | XferDir);
        //
        // Intialize the queue head
        //
        EHCIInitializeQueueHead(QhBulk);
    
        //
        // Set the first qTD pointer
        //
        QhBulk->fpFirstqTD    = QtdBulkData;
        QhBulk->dNextqTDPtr   = (UINT32)(UINTN)QtdBulkDataPhyAddr;
        QhBulk->dLinkPointer  = (UINT32)((UINTN)QhBulkPhyAddr | EHCI_QUEUE_HEAD);
    
        //
        // Device address & Endpoint
        //
        Tmp = (UINT32)(DevInfo->DeviceAddress | (Endp << 8));
    
        //
        // Set max packet size
        //
        Tmp = Tmp | ((UINT32)MaxPkt << 16);
    
        //
        // Set the data toggle control
        //

        Tmp |= QH_USE_QTD_DT;
        // Do not set QH_HEAD_OF_LIST bit on VIA controller
        if (HcStruc->Vid != 0x1106) {
            Tmp |= QH_HEAD_OF_LIST;
        }
    
        //
        // Set the device speed
        // Reset the device speed bits
        //
        Tmp1 = (UINT32)DevInfo->EndpointSpeed;    // 00/01/10 for HI/LO/FULL
    
        //
        // Assume as a high speed device
        //
        Tmp |= QH_HIGH_SPEED;   // 10b - High speed
    
        //
        // Check for high speed
        //
        if (Tmp1) {
            Tmp1   = (Tmp1 & 1) << 12;    // Bit 12 = full/low speed flag
            Tmp    &= ~(QH_ENDPOINT_SPEED);
            Tmp    |= Tmp1;
            //
            // Set the hub address and port number
            //
            Tmp1   = (UINT32)EHCI_GetHiSpeedHubPortNumber(DevInfo);
            Tmp1   = (Tmp1 << 16);     // Hispeed hub port number & device number
            QhBulk->dEndPntCap    |= Tmp1;   // Split complete Xaction
        }
    
        //
        // Update the endpoint characteristics field with the data formed
        //
        QhBulk->dEndPntCharac = Tmp;
    
        //
        // Fill the bulk data qTD with relevant information
        // The token field will be set so
        //   Direction PID = QTD_OUT_TOKEN/QTD_IN_TOKEN,
        //   Size = size of the data,
        //   Data Toggle = bDatToggle,
        //   Error Count = QTD_THREE_ERRORS,
        //   Status code = QTD_DO_OUT(if it is out) + QTD_ACTIVE
        // The buffer pointers field will point to the FpBuffer buffer
        //   which was before initialized to contain a DeviceRequest struc.
        // The dNextqTDPtr field will point to the qTDControlSetup
        // The dAltNextqTDPtr field will be set to EHCI_TERMINATE
        //
        if (XferDir & BIT7) {
            QtdBulkData->dToken   = QTD_IN_TOKEN |
                    QTD_IOC_BIT |
                    QTD_THREE_ERRORS | QTD_ACTIVE;
        } else {
            QtdBulkData->dToken   = QTD_OUT_TOKEN |
                    QTD_IOC_BIT |
                    QTD_THREE_ERRORS | QTD_DO_OUT | QTD_ACTIVE;
            if (TempBuffer != NULL) {
                CopyMem(TempBuffer, BufPhyAddr, BytesToTransfer);
            }            
        }
    
        //
        // Set the data toggle depending on the bDatToggle value
        //
        QtdBulkData->dToken   |= (UINT32)DatToggle << 31;
    
        //
        // Set length
        //
        QtdBulkData->dToken   |= (BytesToTransfer << 16);
    
        //
        // Update buffer pointers
        //
        if (TempBuffer != NULL) {
            EHCISetQTDBufferPointers(QtdBulkData, TempBuffer, BytesToTransfer);
        } else {
            EHCISetQTDBufferPointers(QtdBulkData, BufPhyAddr, BytesToTransfer);
        }
    
        //
        // Update next & alternate next qTD pointers
        //
        QtdBulkData->dNextqTDPtr      = EHCI_TERMINATE;
        QtdBulkData->dAltNextqTDPtr   = EHCI_TERMINATE;
    
        QhBulk->bActive   = TRUE;
    
    
        //
        // Set bulk condition as not stalled
        //
        gUsbData->UsbLastCommandStatus &= ~(USB_BULK_STALLED + USB_BULK_TIMEDOUT);
    
        //
        // Now wait for bulk transaction to be complete
        // the EHCIProcessInterrupt will set its active flag to FALSE.
        // Now wait for the bulk transfer to complete
        //
        EhciCmdStatus = EhciExexuteAsyncSchedule(HcStruc, QhBulk);

        QhBulk->fpFirstqTD = 0;
        QhBulk->dNextqTDPtr = EHCI_TERMINATE;

        if (EhciCmdStatus != USB_SUCCESS) {
            //
            // Set time out status
            //
            gUsbData->UsbLastCommandStatus |= USB_BULK_TIMEDOUT;
            gUsbData->UsbLastCommandStatusExtended  |= USB_TRNSFR_TIMEOUT;
            break;
        }

        if (QhBulk->bErrorStatus & QTD_HALTED) {
           //
           // Stall condition
           //
            gUsbData->UsbLastCommandStatus   &= ~(USB_BULK_TIMEDOUT);
            gUsbData->UsbLastCommandStatus   |= USB_BULK_STALLED;
            gUsbData->UsbLastCommandStatusExtended  |= USB_TRSFR_STALLED;
          break;
        }

        //
        // Update the data toggle value into the mass info structure
        //
        DatToggle =
            (UINT8)(((QhBulk->dToken & QH_DATA_TOGGLE) >> 31) & 1);
        UsbUpdateDataToggle(DevInfo, Endp | XferDir, DatToggle);

        //
        // Get the size of data transferred
        //
        Tmp = (QtdBulkData->dToken & ~(QTD_DATA_TOGGLE)) >> 16;
        Tmp = (Tmp)? BytesToTransfer - Tmp : BytesToTransfer;

        if (!Tmp) {
            break;
        }

        if (TempBuffer != NULL) {
            if (XferDir & BIT7) {
                CopyMem(BufPhyAddr, TempBuffer, Tmp);
            }
        }

        //
        // Adjust loop variables
        //
        BytesRemaining = BytesRemaining - Tmp;
        BytesTransferred += Tmp;

        if (Tmp < BytesToTransfer) {
            break;
        }

        BufPhyAddr += Tmp;
    }

    HcDmaUnmap(HcStruc, BufferMapping);
    EhciMemFree(HcStruc, QhBulk, GET_MEM_BLK_COUNT( (1 * sizeof(EHCI_QH)) +
                                                    (1 * sizeof(EHCI_QTD))));

    if (TempBuffer != NULL) {
        if (Length < gUsbData->HighSpeedMaxBulkDataSize) {
            EhciMemFree(HcStruc, TempBuffer, GET_MEM_BLK_COUNT((UINT16)Length));     
        } else {
            EhciMemFree(HcStruc, TempBuffer, GET_MEM_BLK_COUNT(gUsbData->HighSpeedMaxBulkDataSize));
        }
    }
    
    return  BytesTransferred;

}


/**
    This function calculates Isochronous TD buffer addresses
    
    @param ItdAddress    Pointer to Isochronous TD buffer addresses
    @param Address       Address

    @retval EFI_SUCCESS  On success

**/
EFI_STATUS
CalculateItdBuffers (
    ITD_PG_ADDRESS  *ItdAddress,
    UINT32          *Address
)
{
    UINT8 Index;
    UINT32 ItdTrOffset;
    BOOLEAN NewPgNeeded;

    for (Index = 0; Index < 8; Index++)
    {
        ItdAddress[Index].TrBuffer = *Address;
        ItdTrOffset = *Address & 0xfff;
        NewPgNeeded = ItdTrOffset >= EHCI_ITD_PAGE_MAX_OFFSET;
        ItdAddress[Index].NewPgNeeded = NewPgNeeded;
        *Address += USB_ISOC_XFER_MEM_LENGTH;
    }
    // can't have a new page on 7th TR record; however if the record can hold the data we also return success
    return (NewPgNeeded && ItdTrOffset != EHCI_ITD_PAGE_MAX_OFFSET)? EFI_INVALID_PARAMETER : EFI_SUCCESS;
}


// Note: this function uses current frame index as input. Any delays in this
// function (debug messages, stalls, etc.) affect async transfer since "current"
// frame will not be current.
/**
    Create Isochronous TD buffer 
    
    @param Hc            Pointer to the host controller structure
    @param Device        DeviceInfo structure (if available else 0)
    @param Index         The index in FrameList.
    @param Buffer        Itd buffer
    @param Length        Length request parameter, number of bytes
                         of data to be transferred in or out
                         of the host controller                     
    @param XferDir       Transfer direction
    @param AsyncStatus   asynchronous indicator 
                         
    @retval FrameCount   Frame Count

**/
UINT16 CreateITDs(
    HC_STRUC    *Hc,
    DEV_INFO    *Device,
    UINT16      Index,
    UINT8       *Buffer,
    UINT32      Length,
    UINT8       XferDir,
    UINT8       *AsyncStatus
)
{
    UINT16      Count;
    UINT8       Frame;
    UINT16      FrameCount = 1;
    volatile EHCI_ITD    *Itd;
    UINT32      Address = (UINT32)(UINTN)Buffer;
    UINT32      Page;
    ITD_PG_ADDRESS ItdAddress[8];
    UINT16      CurrentIndex = Index;
    UINT8       Endpoint = Device->IsocDetails.Endpoint & 0xf;
    UINT32      XferRecLength = Device->IsocDetails.EpMaxPkt * Device->IsocDetails.EpMult;
    UINT32      ScheduledDataLength = 0;
    EFI_STATUS  Status;

    for (Count = 0; Count < Hc->AsyncListSize; Count++)
    {
        Status = CalculateItdBuffers(ItdAddress, &Address);
        if (EFI_ERROR(Status))
        {
            // can't have a new page on 7th TR record, adjust address and redo this iTD
            Address = (ItdAddress[0].TrBuffer & 0xfffff000) + 0x1000;
            Status = CalculateItdBuffers(ItdAddress, &Address);
            // second call must be successful
            if (EFI_ERROR (Status)) {
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Ehci Calculate Itd Buffers Staus %r\n", Status);
            }
        }

        // Fill the iTD
        Page = 0;
        Itd = (EHCI_ITD*)(UINTN)Hc->FrameList[CurrentIndex++];
        if (CurrentIndex == Hc->AsyncListSize) CurrentIndex = 0;

        ASSERT_EFI_ERROR(Status);
        Itd->BufferPointer[0] = ItdAddress[0].TrBuffer & 0xfffff000;

        for (Frame = 0; Frame < 8; Frame++, FrameCount++)
        {
            Itd->ControlStatus[Frame] = (EHCI_ISOC_MAX_REC_DATA_SIZE << 16);    // transaction length
            Itd->ControlStatus[Frame] |= ItdAddress[Frame].TrBuffer & 0xfff;   // transaction offset
            Itd->ControlStatus[Frame] |= (Page << 12);        // page
            if(AsyncStatus) {
                Itd->ControlStatus[Frame] |= BIT15;   // Interrupt on completion
            }
            if (ItdAddress[Frame].NewPgNeeded && (Frame != 7))
            {
                Page++;
                Itd->BufferPointer[Page] = ItdAddress[Frame+1].TrBuffer & 0xfffff000;
            }
            ScheduledDataLength += XferRecLength;
            if (ScheduledDataLength >= Length) break;
        }

        Itd->BufferPointer[0] |= (Device->DeviceAddress | ((UINT16)Endpoint << 8));
        Itd->BufferPointer[1] |= Device->IsocDetails.EpMaxPkt;
        Itd->BufferPointer[2] |= Device->IsocDetails.EpMult;    // # of transaction per microframe
        if (XferDir != 0)
        {
            Itd->BufferPointer[1] |= BIT11;             // BIT11 0 - out, 1 - in
        }

        if (ScheduledDataLength >= Length) break;
    }

    Device->IsocDetails.AsyncStatus = AsyncStatus;
    if (AsyncStatus)
    {
        Device->IsocDetails.XferStart = (UINTN)Index;
        Device->IsocDetails.XferKey = (UINTN)Itd;
    }    
    return FrameCount;
}

/**
    This function activates or deactivates the given range of ITDs

    @param HcStruc    Pointer to the host controller structure
    @param FrmNum     Frame number
    @param FrameCount Frame Count
    @param Frame      The target frame
    @param Activate   If TRUE,  activate the given range of ITDs.
                      If FALSE, deactivate the given range of ITDs.
    @retval Itd       Return the ITD pointer.

**/
EHCI_ITD* ActivateDeactivateITDs(
    HC_STRUC    *HcStruc,
    UINT16      FrmNum,
    UINT16      FrameCount,
    UINT8       *Frame,
    BOOLEAN     Activate
)
{
    UINT16  FrameTotal = 0;
    UINT16  FrameNum = FrmNum;
    EHCI_ITD *Itd;
    UINT8   Index;

    while (1)
    {
        Itd = (EHCI_ITD*)(UINTN)HcStruc->FrameList[FrameNum++];
        if (FrameNum == HcStruc->AsyncListSize) FrameNum = 0;

        for (Index = 0; Index < 8; Index++)
        {
            if (Activate)
                Itd->ControlStatus[Index] |= 0x80000000;
            else
                Itd->ControlStatus[Index] &= 0x7fffffff;
            
            FrameTotal++;
            if (FrameTotal == FrameCount)
            {
                if (Frame != NULL)
                    *Frame = Index;
                return Itd;
            }
        }
    }
}

/**
 * This function returns the current frame index
 * 
 * @param HcStruc    Pointer to the host controller structure
 * @retval           The current frame index.
 */
UINT16 GetCurrentFIndex(HC_STRUC* HcStruc)
{
    UINT32  Data = HcReadOpReg(HcStruc, EHCI_FRINDEX);
    return (UINT16)((Data >> 3) & (HcStruc->AsyncListSize - 1));
}


/**
    For a given range of micro-frames update the map of transfer size with
    the individual micro-frame transfer length. Return the total transfer
    data amount.
    
    @param HcStruc    Pointer to the host controller structure
    @param Index      Frame index in FrameList
    @param FrameCount Frame Count
    @param FrameData  The total transfer data
    
    @retval TotalAmount    Total transfer data amount
**/
UINT32 FinalizeIsocXfer(
    HC_STRUC    *HcStruc,
    UINT16      Index,
    UINT16      FrameCount,
    UINT32      *FrameData
)
{
    UINT16  FrameTotal = 0;
    UINT16  Frame = Index;
    volatile EHCI_ITD *Itd;
    UINT8   ControlStatusIndex;
    UINT32  TotalAmount = 0;
    UINT32  FrameXferBytes;
    UINT16  Count;

    for (Count = 0; Count < HcStruc->AsyncListSize; Count++) {
        Itd = (EHCI_ITD*)(UINTN)HcStruc->FrameList[Frame++];
        if (Frame == HcStruc->AsyncListSize) Frame = 0;

        for (ControlStatusIndex = 0; ControlStatusIndex < 8; ControlStatusIndex++) {
            FrameXferBytes = 0;
            if ((Itd->ControlStatus[ControlStatusIndex] & 0x80000000) == 0) {
                FrameXferBytes = ((Itd->ControlStatus[ControlStatusIndex] >> 16) & 0xfff);
                TotalAmount += FrameXferBytes;
            } else {
                Itd->ControlStatus[ControlStatusIndex] &= 0x7fffffff;  // deactivate microframe that has not been processed
            }

            FrameTotal++;
            if (FrameData != NULL) {
                *FrameData++ = FrameXferBytes;
            }

            if (FrameTotal == FrameCount) {
                return TotalAmount;
            }
        }
    }
    ASSERT(FALSE);
    return 0;
}

/**
    This function executes a isochronous transaction on the USB. The transfer may be
    either DATA_IN or DATA_OUT packets containing data sent from the host to the
    device or vice-versa. Depending on the value passed in AsyncIndicator this transfer
    would either be a blocking (synchronous) or non-blocking (asynchronous).

    @param[in]  HcStruc         Pointer to HCStruc of the host controller
    @param[in]  DevInfo         DeviceInfo structure (if available else 0)
    @param[in]  XferDir         Transfer direction
                                 Bit 7: Data direction
                                   0 Host sending data to device
                                   1 Device sending data to host
                                  Bit 6-0 : Reserved
    @param[in]  Buffer          32-bit buffer containing data to be sent to the device or buffer
                                to be used to receive data
    @param[in]  Length          Number of bytes of data to be transferred in or out
    @param[in]  AsyncIndicator  If NULL, the call is not returned until all data is transferred
                                if not NULL, it points to the UINT8 data which will be updated
                                when transfer is complete.
    @retval TotalAmount         Total transfer data amount
    
    @note       Isochronous transfer implies no checking for the data transmission
          errors, i.e. transfer completes successfully when the last iTD becomes inactive.
**/
UINT32
EFIAPI
EHCI_IsocTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length,
    UINT8       *AsyncIndicator
)
{
    UINT16      CurrentFIndex;
    UINT32      MaxXferLength;
    UINT16      FrameCount;
    UINT16      TransferTimeout15us;
    UINT8       Frame;
    volatile    EHCI_ITD    *Itd;
    UINT16      Count;
    UINT32      TotalAmount;
    EFI_STATUS  Status;

    Status = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(Status)) {
        return 0;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        return 0;
    }


    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)Buffer, Length);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "Ehci IsocTransfer Invalid Pointer, Buffer is in SMRAM.\n");
            return 0;
        }
        if (AsyncIndicator) {
            Status = AmiUsbValidateMemoryBuffer((VOID*)AsyncIndicator, sizeof(UINT8));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return 0;
            }
        }
        gCheckUsbApiParameter = FALSE;
    }


    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return 0;
    }

    if (EhciIsHalted(HcStruc)) {
      return 0;
    }

    if (HcStruc->IsocTds == NULL) {
        return 0;
    }

    // see if the requested length is acceptable
    MaxXferLength = DevInfo->IsocDetails.EpMaxPkt * DevInfo->IsocDetails.EpMult * 8 * HcStruc->AsyncListSize;
   
    if (Length > MaxXferLength)
    {
        ASSERT(FALSE);
        return 0;   // too much to ask
    }

    gUsbData->UsbLastCommandStatusExtended = 0;

    // initialize and activate the necessary number of iTDs

    CurrentFIndex = GetCurrentFIndex(HcStruc);
    // advance by, say, one 1 ms frame to avoid losing first TD
    CurrentFIndex += 1;
    if (CurrentFIndex == HcStruc->AsyncListSize)
    {
        CurrentFIndex = 0;
    }

    FrameCount = CreateITDs(HcStruc, DevInfo, CurrentFIndex, Buffer, Length, XferDir, AsyncIndicator);
    Itd = ActivateDeactivateITDs(HcStruc, CurrentFIndex, FrameCount, &Frame, TRUE);

    if (AsyncIndicator != NULL)
    {
        *AsyncIndicator = 0;    // clear indicator, to be set by ProcessInterrupt upon transfer completion
        return 0;
    }
    
    // calculate the timeout
    TransferTimeout15us = (FrameCount << 3) + 1;  // 125 us / 8 = 15 us

    // wait for transfer completion
    for (Count = 0; Count < TransferTimeout15us; Count++)
    {
        if ((Itd->ControlStatus[Frame] & 0x80000000) == 0) break;
        FixedDelay(15);  // 15 microsec
    }

    Status = AmiUsbValidateMemoryBuffer((VOID*)DevInfo->IsocDetails.XferDetails, FrameCount * sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return 0;
    }

    TotalAmount = FinalizeIsocXfer(HcStruc, CurrentFIndex, FrameCount, DevInfo->IsocDetails.XferDetails);
    if (Count == TransferTimeout15us)
    {
        gUsbData->UsbLastCommandStatusExtended |= USB_TRNSFR_TIMEOUT;
    }

    return TotalAmount;
}


/**
    This function executes an interrupt transaction on the USB.
    The data transfer direction is always DATA_IN. This
    function will not return until the request either completes
    successfully or completes in error (due to time out, etc.)

    @param HcStruc         Pointer to the host controller structure
    @param DevInfo         DeviceInfo structure (if available else 0)
    @param EndpointAddress The destination USB device endpoint to which the device request is being sent.
    @param MaxPktSize      Indicates the maximum packet size the target endpoint is capable 
                           of sending or receiving.
    @param Buffer          Buffer containing data to be sent to the
                           device or buffer to be used to receive data
    @param Length          wLength request parameter, number of bytes of data to be transferred in

    @retval BytesTransferred  Number of bytes transferred


    @note  DO NOT TOUCH THE LINK POINTER OF THE TDInterruptData. It is
              statically allocated and linked with other items in the
              1ms schedule

**/

UINT16
EFIAPI
EHCI_InterruptTransfer (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       EndpointAddress,
    UINT16      MaxPktSize,
    UINT8       *Buffer,
    UINT16      Length
)
{

    UINT8       DatToggle;
    EHCI_QH     *QhInt;
    UINT32      Tmp;
    UINT32      Tmp1;
    EHCI_QTD    *QtdIntData;
    UINT32      Count;
    UINT32      Timeout;
    UINT32      BytesTransferred;
    UINT8       *BufPhyAddr = NULL;
    VOID        *BufferMapping = NULL;
    EFI_STATUS  Status;

    Status = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(Status)) {
        return 0;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        return 0;
    }
    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)Buffer, Length);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "Ehci InterruptTransfer Invalid Pointer, Buffer is in SMRAM.\n");
            return 0;
        }
        gCheckUsbApiParameter = FALSE;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return 0;
    }

    if (EhciIsHalted(HcStruc)) {
      return 0;
    }

    gUsbData->UsbLastCommandStatusExtended = 0;

    DatToggle = UsbGetDataToggle(DevInfo, EndpointAddress);

    //
    // Get the QHInterrupt pointer
    //
    QhInt = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT((1 * sizeof(EHCI_QH))+
                                               (1 * sizeof(EHCI_QTD))));
    if (QhInt == NULL) {
      return 0;
     }

    QtdIntData = (EHCI_QTD*)((UINTN)QhInt + sizeof(EHCI_QH));

    //
    // Intialize the queue head
    //
    EHCIInitializeQueueHead(QhInt);

    //
    // Set the first qTD pointer
    //
    QhInt->fpFirstqTD = QtdIntData;
    QhInt->dNextqTDPtr = (UINT32)(UINTN)QtdIntData;
    QhInt->dLinkPointer = EHCI_TERMINATE;

    //
    // Get Device address & Endpoint
    //
    Tmp = (UINT32)DevInfo->DeviceAddress;
    Tmp |= (UINT32)(EndpointAddress & 0xF) << 8;

    //
    // Set max packet size
    //
    Tmp |= (UINT32)(MaxPktSize) << 16;

    //
    // Set the device speed, reset the device speed bits
    //
    Tmp1 = (UINT32)DevInfo->EndpointSpeed;  // 00/01/10 for HI/LO/FULL

    //
    // Assume as a high speed device
    //
    Tmp |= QH_HIGH_SPEED;  // 10b - High speed

    //
    // Check for high speed
    //
    if (Tmp1) {
        Tmp1 = (Tmp1 & 1) << 12;  // Bit 12 = full/low speed flag
        Tmp &= ~(QH_ENDPOINT_SPEED);
        Tmp |= Tmp1;
        //
        // Set the hub address and port number
        //
        Tmp1 = (UINT32)EHCI_GetHiSpeedHubPortNumber(DevInfo);
        Tmp1 = (Tmp1 << 16) | BIT10 | BIT11 | BIT12;
        QhInt->dEndPntCap |= Tmp1;   // Split complete Xaction
    }
    //
    // Update the endpoint characteristcs field with the data formed
    //
    QhInt->dEndPntCharac = Tmp;
    QhInt->dEndPntCap |= (BIT0 | QH_ONE_XFER);    // Interrupt schedule mask
    QhInt->Interval = EhciTranslateInterval(DevInfo->EndpointSpeed, 
                            DevInfo->PollInterval);

    //
    // Set the data toggle depending on the bDatToggle value
    //
    QhInt->dToken |= (UINT32)DatToggle << 31;

//
// Fill the interrupt data qTD with relevant information
// The token field will be set so
//   Direction PID = QTD_IN_TOKEN,
//   Size = size of the data,
//   Data Toggle = bDatToggle,
//   Error Count = QTD_THREE_ERRORS,
//   Status code = QTD_ACTIVE
// The buffer pointers field will point to the EDX
//   which was before initialized to contain a DeviceRequest struc.
// The dNextqTDPtr field will point to the qTDControlSetup
// The dAltNextqTDPtr field will be set to EHCI_TERMINATE
//
    QtdIntData->dToken = QTD_IOC_BIT | QTD_THREE_ERRORS | QTD_ACTIVE;
    if (EndpointAddress & BIT7) {
     QtdIntData->dToken |= QTD_IN_TOKEN;
    } else {
     QtdIntData->dToken |= QTD_OUT_TOKEN;
    }

    //
    // Set length
    //
    QtdIntData->dToken |= (UINT32)Length << 16;
 
    HcDmaMap(HcStruc, EndpointAddress & BIT7, Buffer, Length, 
             &BufPhyAddr, &BufferMapping);
    //
    // Update buffer pointers
    //
    EHCISetQTDBufferPointers(QtdIntData, BufPhyAddr, (UINT32)Length);

    //
    // Update next & alternate next qTD pointers
    //
    QtdIntData->dNextqTDPtr = EHCI_TERMINATE;
    QtdIntData->dAltNextqTDPtr = EHCI_TERMINATE;

    //
    // Schedule the QHInterrupt to 1msec schedule
    //
    EhciAddPeriodicQh(HcStruc, QhInt);

    // Set the QH as active
    QhInt->bActive = TRUE;

    //
    // Now wait for interrupt transaction to be complete;
    // the EHCIProcessInterrupt will set its active flag to FALSE.
    //
    Timeout = gUsbData->UsbReqTimeOutValue * 100; // makes it number of 10 microsecond units

    for (Count = 0; Timeout == 0 || Count < Timeout; Count++) {
      if (!(QtdIntData->dToken & QTD_ACTIVE)) {
        break;
      }
      FixedDelay(10);  // 60 microsec
    }
    //Status = EHCIWaitForTransferComplete(HcStruc, fpQHInt, FpDevInfo);

    // Remove the QH from periodic schedule
    EhciRemovePeriodicQh(HcStruc,(EHCI_QH*)QhInt);

    //
    // Check whether the QH stopped or timed out
    //
    BytesTransferred = 0;
    if (QtdIntData->dToken & QTD_ACTIVE) {
       USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "EHCI Time-Out\n");
       gUsbData->UsbLastCommandStatusExtended |= USB_TRNSFR_TIMEOUT;
    } else if (QtdIntData->dToken & QTD_HALTED) {
     gUsbData->UsbLastCommandStatusExtended  |= USB_TRSFR_STALLED;
    } else {
     BytesTransferred = (UINT16)(Length - ((QtdIntData->dToken &
        ~(QTD_DATA_TOGGLE)) >> 16));
     //
     // Update the data toggle value into the mass info structure
     //
     DatToggle = (UINT8)(((QhInt->dToken & QH_DATA_TOGGLE) >> 31) & 1);
     UsbUpdateDataToggle(DevInfo, EndpointAddress, DatToggle);
    }

    HcDmaUnmap(HcStruc, BufferMapping);

    // Free the allocated QH and qTD
    EhciMemFree(HcStruc, QhInt, GET_MEM_BLK_COUNT((1 * sizeof(EHCI_QH)) +
                                                  (1 * sizeof(EHCI_QTD))));

    // Service all interrupts
    EHCI_ProcessInterrupt(HcStruc);

    return (UINT16)BytesTransferred;
}


/**
    This function deactivates the polling QH for the requested
    device. The device may be a USB keyboard or USB hub

    @param HcStruc   Pointer to the host controller structure
    @param DevInfo   Pointer to the device information structure

    @retval USB_SUCCESS   On success
    @retval Others        On fail
**/
UINT8
EFIAPI
EHCI_DeactivatePolling(
    HC_STRUC*       HcStruc,
    DEV_INFO*       DevInfo
)
{
    UINT8   *PollEd;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    EfiStatus = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return USB_ERROR;
    }

    if (EhciIsHalted(HcStruc)) {
        return USB_ERROR;
    }

    // Deactivate iTDs
    if ((DevInfo->IsocDetails.Endpoint) && (HcStruc->IsocTds != NULL)) {
        ActivateDeactivateITDs(HcStruc, 0, HcStruc->AsyncListSize * 8, NULL, FALSE);
    }

    //
    // Get a pointer to the device's QH from the poll QH pointer and remove
    // the polling ED from the schedule
    //
    PollEd = DevInfo->PollEdPtr;
    if (!PollEd) {
        return USB_ERROR;
    }

    ((EHCI_QH*)PollEd)->bActive = FALSE;

    EhciRemovePeriodicQh(HcStruc,(EHCI_QH*)PollEd); 

    UsbUpdateDataToggle(DevInfo, DevInfo->IntInEndpoint, 
    (UINT8)((((EHCI_QH*)PollEd)->dToken & QH_DATA_TOGGLE) >> 31));

    EhciMemFree(HcStruc, PollEd, GET_MEM_BLK_COUNT(sizeof(EHCI_QH) + sizeof(EHCI_QTD)));
    DevInfo->PollEdPtr  = NULL;
    DevInfo->PollTdPtr  = NULL;

    if (DevInfo->PollDataBuffer) {
        EhciMemFree(HcStruc, DevInfo->PollDataBuffer, 
                    GET_MEM_BLK_COUNT(DevInfo->PollingLength)); 
        DevInfo->PollDataBuffer = 0;
    }

    return USB_SUCCESS;
}


/**
    This function activates the polling QH for the requested
    device. The device may be a USB keyboard or USB hub

    @param HcStruc   Pointer to the host controller structure
    @param DevInfo   Pointer to the device information structure

    @retval USB_SUCCESS Success to activate the polling.
    @retval Others      Fail to activates the polling.

    @note  For the keyboard device this routine allocates TDRepeat
              also, if it is not already allocated. This routine allocate
              a polling TD and schedule it to 8ms schedule for keyboards
              and to 1024ms schedule for hubs.

**/
UINT8
EFIAPI
EHCI_ActivatePolling(
    HC_STRUC* HcStruc,
    DEV_INFO* DevInfo
)
{
    EHCI_QH     *PollEd; 
    EHCI_QTD    *PollTd;
    UINT32      Tmp;
    UINT32      Tmp1;
    EFI_STATUS  EfiStatus;
    UINT8       *PollTdPhyAddr;
    UINT8       *PollDataBufferPhyAddr;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "EHCI_AP dev type %d, adr %d, ep %x, maxp %x, speed %x, interval %d\n",
        DevInfo->DeviceType, DevInfo->DeviceAddress, DevInfo->IntInEndpoint, DevInfo->IntInMaxPkt, DevInfo->EndpointSpeed, DevInfo->PollInterval);

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    EfiStatus = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return USB_ERROR;
    }
    
    if (EhciIsHalted(HcStruc)) {
      return USB_ERROR;
    }

    //
    // Allocate a QH/qTD for polling QH & qTD
    //
    PollEd = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT(
                                  sizeof(EHCI_QH)+sizeof(EHCI_QTD)));
    if (!PollEd)
    {
        return  USB_ERROR;  // Memory allocation error
    }

    //
    // Save the pointers in DeviceInfo structure
    //
    DevInfo->PollEdPtr  = (UINT8*)PollEd;
    PollTd    = (EHCI_QTD*)((UINTN)PollEd + sizeof(EHCI_QH));
    DevInfo->PollTdPtr  = (UINT8*)PollTd;

    PollTdPhyAddr = EhciMemGetPhyAddr(HcStruc, PollTd);

//
// Setup the polling QH
// Set the QH's dNextqTDPtr field to polling qTD and dAltqTDPtr field to
// EHCI_TERMINATE
//

    PollEd->fpFirstqTD    = PollTd;
    PollEd->dNextqTDPtr   = (UINT32)(UINTN)PollTdPhyAddr;

    //
    // Intialize the queue head
    //
    PollEd->dAltNextqTDPtr    = EHCI_TERMINATE;
    PollEd->dLinkPointer      = EHCI_TERMINATE;
  
    PollEd->DevInfoIndex = ((UINTN)DevInfo - (UINTN)&gUsbDataList->DevInfoTable[0])/sizeof(DEV_INFO);

    //
    // Get Device address & Endpoint
    //
    Tmp = ((UINT32)DevInfo->DeviceAddress)  |
                    ((UINT32)(DevInfo->IntInEndpoint & 0xF) << 8);

    Tmp |= ((UINT32)DevInfo->IntInMaxPkt) << 16; // Set max packet size 

    Tmp1   = (UINT32)DevInfo->EndpointSpeed;    // 00/01/10 for HI/LO/FULL

    //
    // Assume as a high speed device
    //
    Tmp |= QH_HIGH_SPEED;  // 10b - High speed

    //
    // Check for high speed
    //
    if (Tmp1)
    {
        Tmp1 = (Tmp1 & 1) << 12;  // Bit 12 = full/low speed flag
        Tmp &= ~(QH_ENDPOINT_SPEED);
        Tmp |= Tmp1;
        //
        // Set the hub address and port number
        //
        Tmp1 = (UINT32)EHCI_GetHiSpeedHubPortNumber(DevInfo);
        Tmp1 = (Tmp1 << 16) | (BIT10 + BIT11 + BIT12);    // Split complete Xaction
        PollEd->dEndPntCap    |= Tmp1;
    }

    //
    // Update the endpoint characteristcs field with the data formed
    //
    PollEd->dEndPntCharac = Tmp;

    //
    // Set a bit in interrupt mask
    //
    PollEd->dEndPntCap    |= (BIT0 + QH_ONE_XFER);
    PollEd->Interval = EhciTranslateInterval(DevInfo->EndpointSpeed, 
                                DevInfo->PollInterval);

    //
    // Set the data toggle
    //
    PollEd->dToken |= (UINT32)(UsbGetDataToggle(DevInfo, DevInfo->IntInEndpoint) << 31);

//
// Fill the polling qTD with relevant information
// The token field will be set so
//   Direction PID = QTD_IN_TOKEN,
//   Size = size of the data,
//   Data Toggle = QTD_DATA0_TOGGLE,
//   Error Count = QTD_THREE_ERRORS,
//   Status code = QTD_ACTIVE
// The buffer pointers field will point to the FpBuffer buffer
//   which was before initialized to contain a DeviceRequest struc.
// The dNextqTDPtr field will point to the qTDControlSetup
// The dAltNextqTDPtr field will be set to EHCI_TERMINATE
//
    PollTd->dToken = QTD_IN_TOKEN |
                    QTD_IOC_BIT |
                    QTD_THREE_ERRORS |
                    QTD_ACTIVE;
    //
    // Set length
    //
    PollTd->dToken |= (UINT32)DevInfo->PollingLength << 16;
    DevInfo->PollDataBuffer = EhciMemAlloc(HcStruc, 
                                             GET_MEM_BLK_COUNT(DevInfo->PollingLength)); 
    ASSERT(DevInfo->PollDataBuffer);

    PollDataBufferPhyAddr = EhciMemGetPhyAddr(HcStruc, DevInfo->PollDataBuffer);

    //
    // Update buffer pointers
    //
    EHCISetQTDBufferPointers(PollTd,
                PollDataBufferPhyAddr, DevInfo->PollingLength);

    //
    // Update next & alternate next qTD pointers
    //
    PollTd->dNextqTDPtr = EHCI_TERMINATE;
    PollTd->dAltNextqTDPtr = EHCI_TERMINATE;

    EhciAddPeriodicQh(HcStruc,PollEd); 

    PollEd->CallBackIndex = USB_InstallCallBackFunction(EhciPollingTDCallback);
    PollEd->bActive = TRUE;

    return  USB_SUCCESS;
}


/**
    This function disables the keyboard repeat.

    @param HcStruc        Pointer to the host controller structure

    @retval USB_SUCCESS   Success to disable keyboard repeat.
    @retval Others        On fail.
**/
UINT8
EFIAPI
EHCI_DisableKeyRepeat (
   HC_STRUC *HcStruc
)
{
    EHCI_DESC_PTRS  *DescPtr;
    EHCI_QH         *RepeatQh;
    EFI_STATUS      EfiStatus;
    UINT8           *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return USB_ERROR;
    }
    
   if (EhciIsHalted(HcStruc)) {
     return USB_ERROR;
   }

    DescPtr = HcStruc->DescPtrs.EhciDescPtrs;

    if (DescPtr == NULL) {
        return USB_ERROR;
    }


    if (((UINTN)DescPtr < (UINTN)gUsbDataList->MemBlockStart) ||
        (((UINTN)DescPtr + sizeof(EHCI_DESC_PTRS)) > (UINTN)MemBlockEnd)) {
        return USB_ERROR;
    }
    RepeatQh = DescPtr->fpQHRepeat;

    if (RepeatQh == NULL) {
        return USB_ERROR;
    }
    if (((UINTN)RepeatQh < (UINTN)gUsbDataList->MemBlockStart) ||
        (((UINTN)RepeatQh + sizeof(EHCI_QH)) > (UINTN)MemBlockEnd)) {
        return USB_ERROR;
    }

    RepeatQh->dTokenReload = ((UINT32)8 << 16) | QTD_IN_TOKEN | QTD_ONE_ERROR;
    RepeatQh->bActive = FALSE;

    return  USB_SUCCESS;
}

/**
   This function enables the keyboard repeat.
   
    @param HcStruc        Pointer to the host controller structure

    @retval USB_SUCCESS   Success to disable keyboard repeat.
    @retval Others        On fail.

**/

UINT8
EFIAPI
EHCI_EnableKeyRepeat (
   HC_STRUC* HcStruc
)
{
    EHCI_DESC_PTRS      *DescPtr;
    EHCI_QH             *RepeatQh;
    EHCI_QTD            *RepeatQtd;
    EFI_STATUS          EfiStatus;
    UINT8               *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    EfiStatus = UsbHcStrucValidation(HcStruc);
    
    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    if (!(HcStruc->HcFlag & HC_STATE_RUNNING)) {
        return USB_ERROR;
    }

    if (EhciIsHalted(HcStruc)) {
        return USB_ERROR;
    }

    DescPtr = HcStruc->DescPtrs.EhciDescPtrs;

    if (DescPtr == NULL) {
        return USB_ERROR;
    }


    if (((UINTN)DescPtr < (UINTN)gUsbDataList->MemBlockStart) ||
        (((UINTN)DescPtr + sizeof(EHCI_DESC_PTRS)) > (UINTN)MemBlockEnd)) {
        return USB_ERROR;
    }


    RepeatQh = DescPtr->fpQHRepeat;

    if (RepeatQh == NULL) {
        return USB_ERROR;
    }

    if (((UINTN)RepeatQh < (UINTN)gUsbDataList->MemBlockStart) ||
        (((UINTN)RepeatQh + sizeof(EHCI_QH)) > (UINTN)MemBlockEnd)) {
        return USB_ERROR;
    }


    if ((RepeatQh->dTokenReload & QTD_ACTIVE) == 0) {
        RepeatQtd = DescPtr->fpqTDRepeat;
        if (RepeatQtd == NULL) {
            return USB_ERROR;
        }

        if (((UINTN)RepeatQtd < (UINTN)gUsbDataList->MemBlockStart) ||
            (((UINTN)RepeatQtd + sizeof(EHCI_QTD)) > (UINTN)MemBlockEnd)) {
            return USB_ERROR;
        }

        RepeatQh->dTokenReload = ((UINT32)8 << 16) | QTD_IN_TOKEN | 
                                  QTD_ONE_ERROR | QTD_IOC_BIT | QTD_ACTIVE;

        // Update buffer pointers
        EHCISetQTDBufferPointers(RepeatQtd,
                                 &RepeatQh->aDataBuffer[0], 8);
  
        // Re-init the QH pointers
        RepeatQh->dCurqTDPtr = 0;
        RepeatQh->dAltNextqTDPtr = EHCI_TERMINATE;
        RepeatQh->dNextqTDPtr = (UINT32)(UINTN)RepeatQtd;
  
        //
        // Restart the qTD
        //
        RepeatQh->dToken = 0;
        RepeatQtd->dToken = RepeatQh->dTokenReload;

        RepeatQh->bActive = TRUE;
    }
    return USB_SUCCESS;
}


/**
    This function resets the EHCI controller

    @param HcStruc        Pointer to the host controller structure

    @retval USB_SUCCESS   Success to reset the controller.
    @retval Others        On fail.

**/
UINT8
EHCIResetHC(
    HC_STRUC* HcStruc
)
{
    UINT16   Count;

    if (HcStruc->DebugPort) {
        return USB_SUCCESS;
    }
    //
    // Check HC is halted: attempting to reset an actively running HC will
    // result in undefined behavior.
    //
    if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) {
        //
        // Issue reset
        //
        HcSetOpReg(HcStruc, EHCI_USBCMD, EHCI_HCRESET);

        //
        // EHCI_HCRESET bit is set to zero by the Host Controller when the reset
        // process is complete.
        //
        for (Count = 0; Count < 500; Count++) {
            if (!(HcReadOpReg(HcStruc, EHCI_USBCMD) & EHCI_HCRESET)) {
                return  USB_SUCCESS;
            }
            FixedDelay(100);     // 100 us delay
        }
    }

    //
    // Error - HC reset failed
    //
    USBLogError(USB_ERR_HC_RESET_FAILED);

    return  USB_ERROR;
}

/**
    This function initializes the periodic schedules for the
    EHCI host controller

    @param HcStruc        Pointer to the host controller structure
    @param MemBase        Memory base address

    @retval USB_SUCCESS   Success to initialize the periodic schedules.
    @retval Others        On fail.

    @note  This routine creates 8ms and 32ms schedules

**/
UINT8
EHCIInitializePeriodicSchedule(
    HC_STRUC*   HcStruc,
    UINT32      MemBase
)
{
    UINT8           *Ptr;
    EHCI_DESC_PTRS  *DescPtr;
    //
    // Allocate descriptor structure and fill it in HCStruc
    //
    DescPtr = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT_STRUC(EHCI_DESC_PTRS));
    if (DescPtr == NULL) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_4, "EHCI Descriptor struc alloc failed. %d \n",
                                    GET_MEM_BLK_COUNT_STRUC(EHCI_DESC_PTRS));
        return USB_ERROR;
    }

    //
    // Save the value in the HC struc
    //
    HcStruc->DescPtrs.EhciDescPtrs = DescPtr;

    EhciAddIsochTDs(HcStruc);    // This must be called before any other periodic scheduling

    // Allocate QH/qTD for PeriodicQh
    Ptr = EhciMemAlloc(HcStruc, GET_MEM_BLK_COUNT((1 * sizeof(EHCI_QH))+
                                                  (0 * sizeof(EHCI_QTD))));
    if (Ptr == NULL) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_4, "Schedule of EHCI QH alloc failed.\n");
        return USB_ERROR;
    }
 
    // Save the 1 QH in appropriate location
    DescPtr->PeriodicQh = (EHCI_QH*)Ptr;

    EHCIInitializeQueueHead(DescPtr->PeriodicQh);
    DescPtr->PeriodicQh->dNextqTDPtr = EHCI_TERMINATE;
    DescPtr->PeriodicQh->dAltNextqTDPtr = EHCI_TERMINATE;
    DescPtr->PeriodicQh->dEndPntCharac = ((UINT32)0x40 << 16) + QH_HIGH_SPEED;
    DescPtr->PeriodicQh->dLinkPointer      = EHCI_TERMINATE;
    DescPtr->PeriodicQh->dToken = QTD_HALTED;
    DescPtr->PeriodicQh->Interval = 1;

    EhciAddPeriodicQh(HcStruc, DescPtr->PeriodicQh);

    return  USB_SUCCESS;
}



/**
    This function initializes the queue head with default values

    @param Qh    Pointer to queue head
    @retval None

**/
VOID
EHCIInitializeQueueHead(
    EHCI_QH *Qh
)
{
    Qh->dNextqTDPtr       = 1;
    Qh->dAltNextqTDPtr    = 1;
    Qh->dCurqTDPtr        = 0;

    Qh->dEndPntCap        = QH_ONE_XFER;
    Qh->dToken            = 0;
    Qh->dEndPntCharac     = 0;
    Qh->dBufferPtr0       = 0;
    Qh->dBufferPtr1       = 0;
    Qh->dBufferPtr2       = 0;
    Qh->dBufferPtr3       = 0;
    Qh->dBufferPtr4       = 0;
    Qh->bErrorStatus      = 0;
}


/**
    This function starts the periodic schedule for the
    EHCI host controller

    @param HcStruc         Pointer to the host controller structure

    @retval USB_SUCCESS    Success to start the periodic schedule
    @retval Others         On fail

**/
UINT8
EHCIStartPeriodicSchedule(
    HC_STRUC* HcStruc
)
{
    UINT16  Count;
    //
    // Start periodic schedule
    //
    HcSetOpReg(HcStruc, EHCI_USBCMD, EHCI_PER_SCHED_ENABLE);

    //
    // Make sure the HC started the schedules
    //
    for (Count = 0; Count < 1000; Count++) {
        if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_PER_SCHED_STATUS) {
            break;
        }
        FixedDelay(10);
    }
    if (!(HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_PER_SCHED_STATUS)) {
        return USB_ERROR;
    }
    return USB_SUCCESS;
}


/**
    This function stops the periodic schedule for the
    EHCI USB host controller

    @param HcStruc        Pointer to the host controller structure

    @retval USB_SUCCESS   Success to stop the periodic schedule
    @retval Others        On fail

**/

UINT8
EHCIStopPeriodicSchedule(
    HC_STRUC* HcStruc
)
{
    UINT16  Count;
    //
    // Stop periodic schedule
    //
    HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_PER_SCHED_ENABLE);

    //
    // Make sure the HC stopped the schedules
    //
    for (Count = 0; Count < 1000; Count++) {
        if (!(HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_PER_SCHED_STATUS)) {
            break;
        }
        FixedDelay(10);
    }
    if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_PER_SCHED_STATUS) {
        return USB_ERROR;
    }
    return USB_SUCCESS;
}


/**
    This function starts the asynchronous schedule


    @param HcStruc         Pointer to the host controller structure

    @retval USB_SUCCESS    Success to start the asynchronous schedule
    @retval Others         On fail

**/

UINT8
EHCIStartAsyncSchedule(
    HC_STRUC* HcStruc
)
{
    UINT16  Count;
    //
    // Start the Async schedule
    //
    HcSetOpReg(HcStruc, EHCI_USBCMD, EHCI_ASYNC_SCHED_ENABLE);

    //
    // Make sure the HC started the async. execution
    //
    for (Count = 0; Count < 1000; Count++) {
        if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_ASYNC_SCHED_STATUS) {
            break;
        }
        FixedDelay(10);
    }
    if (!(HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_ASYNC_SCHED_STATUS)) {
        //
        // Stop the host controller (Reset bit 0)
        //
        HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_RUNSTOP);
        // The Host Controller must halt within 16 micro-frames after 
        // software clears the Run bit. 
        for (Count = 0; Count < 16; Count++) {
            if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) {
                break;
            }
            FixedDelay(125);      // 125 us delay
        }
        return USB_ERROR;
    }
    
    return USB_SUCCESS;
}


/**
    This function stops the asynchronous transfer and sets the
    asynchronous pointer to null

    @param HcStruc        Pointer to the host controller structure

    @retval USB_SUCCESS   Success to stop the asynchronous schedule
    @retval Others        On fail

**/

UINT8
EHCIStopAsyncSchedule(
    HC_STRUC* HcStruc
)
{
    UINT16  Count;
    
    if (!(HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_ASYNC_SCHED_STATUS)) {
      return USB_SUCCESS;
    }

    //
    // Stop periodic schedule
    //
    HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_ASYNC_SCHED_ENABLE);

    //
    // Make sure the HC stopped the async. execution
    //
    for (Count = 0; Count < 1000; Count++) {
        if (!(HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_ASYNC_SCHED_STATUS)) {
            break;
        }
        FixedDelay(10);
    }
    if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_ASYNC_SCHED_STATUS) {
        //
        // Stop the host controller (Reset bit 0)
        //
        HcClearOpReg(HcStruc, EHCI_USBCMD, EHCI_RUNSTOP);
        // The Host Controller must halt within 16 micro-frames after 
        // software clears the Run bit. 
        for (Count = 0; Count < 16; Count++) {
            if (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) {
                break;
            }
            FixedDelay(125);      // 125 us delay
        }
        return USB_ERROR;
    }
    
    return USB_SUCCESS;
}

/**
    This function adds Isochronous TDs to the frame list

    @param HcStruc   Pointer to the host controller structure

    @retval None
**/
VOID EhciAddIsochTDs(
    HC_STRUC *HcStruc
)
{
    UINT16  Index;
    UINT8   *Ptr;
    EHCI_ITD *Itd;

    if (HcStruc->IsocTds == NULL) {
        return;
    }

    Itd = (EHCI_ITD*)HcStruc->IsocTds;

    USB_DEBUG(DEBUG_INFO, 3, "EHCI add ISOC TDs: ITD %x\n", Itd);

    for (Index = 0; Index < HcStruc->AsyncListSize; Index++)
    {
        Ptr = (UINT8*)&HcStruc->FrameList[Index];
        Itd->NextLinkPointer = *((UINT32*)(UINTN)Ptr);
        *((UINT32*)(UINTN)Ptr) = (UINT32)(UINTN)Itd;
        Itd++;
    }
}

/**
    This function adds a QH to the frame list
    @param HcStruc        Pointer to the host controller structure
    @param NewQh          Pointer to the QH to be added        

    @retval USB_SUCCESS   Success to add a QH to the frame list.
    @retval Others        Fail to add a QH to the frame list.
**/
UINT8
EFIAPI
EhciAddPeriodicQh(
    HC_STRUC    *HcStruc,
    EHCI_QH     *NewQh
)
{
    UINT16      Index;
    UINT32      *PrevPtr;
    UINT32      LinkPtr;
    EHCI_QH     *Qh;
    EHCI_QH     *NewQhPhyAddr;
    EHCI_QH     *QhHostAddr;
    EFI_STATUS  Status = EFI_SUCCESS;

    if (NewQh == NULL || NewQh->Interval == 0) {
        return USB_ERROR;
    }

    NewQhPhyAddr = EhciMemGetPhyAddr(HcStruc, NewQh);
 
    for (Index = HcStruc->SplitPeriodicIndex; Index < HcStruc->AsyncListSize; Index += NewQh->Interval) {
        PrevPtr = &HcStruc->FrameList[Index];
        if (HcStruc->IsocTds != NULL) {
            PrevPtr = (UINT32*)(UINTN)*PrevPtr; // leave ITD on the top of the list
        }
        LinkPtr = *PrevPtr; 
        while (!(LinkPtr & EHCI_TERMINATE)){
            Qh = (EHCI_QH*)(LinkPtr & EHCI_POINTER_MASK);

            Status = AmiUsbValidateMemoryBuffer((VOID*)Qh, sizeof(EHCI_QH));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return USB_ERROR;
            }
            QhHostAddr = EhciMemGetHostAddr(HcStruc, Qh);
            if (QhHostAddr->Interval <= NewQh->Interval) {
                break;
            }
            PrevPtr = &QhHostAddr->dLinkPointer;
            LinkPtr = *PrevPtr; 
        }

        if (!(LinkPtr & EHCI_TERMINATE) && (QhHostAddr == NewQh)) {
            continue;
        }
        Status = AmiUsbValidateMemoryBuffer((VOID*)PrevPtr, sizeof(UINT32));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return USB_ERROR;
        }
        NewQh->dLinkPointer = *PrevPtr;
        *PrevPtr = (UINT32)((UINTN)NewQhPhyAddr | EHCI_QUEUE_HEAD);
    }

    if (HcStruc->SplitPeriodicIndex == MAX_SPLIT_PERIODIC_NUMBER) {
        HcStruc->SplitPeriodicIndex = 0;
    } else {
        HcStruc->SplitPeriodicIndex++;
    }

    return USB_SUCCESS;
} 


/**
    This function removes a QH from the frame list
    
    @param HcStruc        Pointer to the host controller structure
    @param RetiredQh      Pointer to the QH to be removed
    
    @retval USB_SUCCESS   Success to remove the QH from the frame list.
    @retval Others        Fail to remove the QH from the frame list.
**/

UINT8
EhciRemovePeriodicQh (
    HC_STRUC    *HcStruc,
    EHCI_QH     *RetiredQh
)
{
    UINT16      Index;
    UINT32      *PrevPtr;
    UINT32      LinkPtr;
    EHCI_QH     *Qh;
    EFI_STATUS  Status = EFI_SUCCESS;
    EHCI_QH     *QhHostAddr;

    if (RetiredQh == NULL || RetiredQh->Interval == 0) {
        return USB_ERROR;
    }

    EHCIStopPeriodicSchedule(HcStruc);

    for (Index = 0; Index < HcStruc->AsyncListSize; Index++) {
        PrevPtr = &HcStruc->FrameList[Index];
        LinkPtr = *PrevPtr; 
 
        while (!(LinkPtr & EHCI_TERMINATE)){
            Qh = (EHCI_QH*)(LinkPtr & EHCI_POINTER_MASK);
            Status = AmiUsbValidateMemoryBuffer((VOID*)Qh, sizeof(EHCI_QH));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return USB_ERROR;
            }
            QhHostAddr = EhciMemGetHostAddr(HcStruc, Qh);
            if (QhHostAddr == RetiredQh) {
                break;
            }
            PrevPtr = &QhHostAddr->dLinkPointer;
            LinkPtr = *PrevPtr; 
        }

        if (LinkPtr & EHCI_TERMINATE) {
            continue;
        }
        Status = AmiUsbValidateMemoryBuffer((VOID*)PrevPtr, sizeof(UINT32));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return USB_ERROR;
        }
        *PrevPtr = RetiredQh->dLinkPointer;
    }

    RetiredQh->dLinkPointer = EHCI_TERMINATE;

    EHCIStartPeriodicSchedule(HcStruc);

    return USB_SUCCESS;
}


/**
    This function will set the 5 buffer pointer in the qTD
    appropriately depending upon the input size

    @param Qtd     Pointer to the qTD
    @param Buf     32bit absolute buffer pointer
    @param Size    Amount of data to be transferred

    @retval None
**/

VOID
EFIAPI
EHCISetQTDBufferPointers(
    EHCI_QTD    *Qtd,
    UINT8       *Buf,
    UINT32      Size
)
{
    UINT16      BufSize;
    UINT8       *Buffer   = Buf;
    UINT32      *BufferPtr;
    UINT16      W4KRemainder;

    //
    // Fill the buffer pointers with 0s
    //
    Qtd->dBufferPtr0      = 0;
    Qtd->dBufferPtr1      = 0;
    Qtd->dBufferPtr2      = 0;
    Qtd->dBufferPtr3      = 0;
    Qtd->dBufferPtr4      = 0;
    Qtd->dAltNextqTDPtr   = 1;

    //
    // If size to transfer is 0 skip updating pointers
    //
    if (!Size)
    {
        return;
    }

    //
    // Make sure the amount of data to be xferred is 16K or less
    //
    BufSize = (UINT16)((Size > MAX_EHCI_DATA_SIZE) ? MAX_EHCI_DATA_SIZE : Size);

    BufferPtr = &Qtd->dBufferPtr0;

    for (;;)
    {
        *BufferPtr = (UINT32)(UINTN)Buffer;
        //
        // Calculate the number of bytes that can be transferred using current
        // buffer pointer
        //
        W4KRemainder = (UINT16)((((UINT32)((UINTN)Buffer+0x1000)) & 0xFFFFF000) -
                                        (UINT32)(UINTN)Buffer);

        //
        // Check whether all the bytes can be accomadated in the current buffer
        //
        if (W4KRemainder >= BufSize)
        {
            break;  // Yes. Current Buffer is sufficient for the rest of data
        }

        //
        // We have more data to transfer - adjust data and store it in the next pointer
        //
        BufSize = (UINT16)(BufSize - W4KRemainder);   // Amount of data remaining
        Buffer = Buffer + W4KRemainder;     // Adjust buffer (4K bound)
        BufferPtr++;                          // Next buffer pointer
    }
}


/**
    This function waits until the requested QH completes or
    the transaction time-out

    @param HcStruc        Pointer to the host controller structure
    @param Qh             Pointer to the QH which has to be completed
    @param DevInfo        Pointer to the DevInfo structure
    
    @retval USB_SUCCESS   Success
    @retval Others        On fail

**/

UINT16
EHCIWaitForTransferComplete(
    HC_STRUC    *HcStruc,
    EHCI_QH     *Qh,
    DEV_INFO*   DevInfo

)
{
    UINT32 Count;
    UINT32 CountLimit = gUsbData->UsbReqTimeOutValue * 100; // makes it number of microsecond units
    UINT16 Status = USB_ERROR;

    //
    // Check status change loop iteration
    //
    for (Count = 0; !CountLimit || Count < CountLimit; Count++) {
        EHCIProcessQH(HcStruc, Qh);
        if (Qh->bActive == FALSE) {
            Status = USB_SUCCESS;
            break;
        }
        FixedDelay(10);  // 60 microsec
    }

    if (Qh->bActive == TRUE) {
       // Set the QH as in-active
       Qh->bActive = FALSE;
       Status = USB_ERROR;
       USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "EHCI Time-Out\n");
     }

    // Service all interrupts
    EHCI_ProcessInterrupt(HcStruc);
 
    return Status;
}


/**
    This function whether all the TD's in the QH is completed
    
    @param HcStruc       Pointer to the host controller structure
    @param Qh            Pointer to the QH which has to be completed

    @retval USB_SUCCESS  On success
    @retval Others       On fail

**/
UINT8
EHCIProcessQH(
    HC_STRUC *HcStruc,
    EHCI_QH  *Qh
)
{
    EHCI_QTD    *Qtd = Qh->fpFirstqTD;
    EFI_STATUS  Status;
    EHCI_QTD    *QtdHostAddr;
    DEV_INFO    *DevInfoInQh = NULL;

    for (;;) {
        if (Qtd == NULL) {
            return USB_ERROR;
        }
        Status = AmiUsbValidateMemoryBuffer((VOID*)Qtd, sizeof(EHCI_QTD));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return USB_ERROR;
        }
        //
        // Check whether the qTD is active, if so. Exit!
        //
        if (Qtd->dToken & QTD_ACTIVE) {
            return  USB_ERROR;
        }

        //
        // Check for halt condition, if halted - exit
        //
        if (Qtd->dToken & QTD_HALTED) {
            //
            // Set the QH halted status
            //
            Qh->bErrorStatus = QTD_HALTED;
            //
            // Set the QH as in-active
            //
            Qh->bActive = FALSE;
            break;
        }
        //
        // qTD is not active and not halted. That is it is completed successfully
        // Check whether this qTD is the last one in the list
        //
        if (Qtd->dNextqTDPtr & EHCI_TERMINATE) {
            //
            // Set the QH as in-active
            //
            Qh->bActive = FALSE;
            break;
        }

        //
        // More qTDs are in the list. Process next qTD
        //
        Qtd = (EHCI_QTD*)(UINTN)Qtd->dNextqTDPtr;
        QtdHostAddr = EhciMemGetHostAddr(HcStruc, Qtd);
        Qtd = QtdHostAddr;
    }

    if ((Qh->CallBackIndex) && (Qh->CallBackIndex <= MAX_CALLBACK_FUNCTION)) {
        if (gAmiUsbCallBackFunTbl[Qh->CallBackIndex - 1 + CALLBACK_FUNCTION_START]) {
            if ((Qh->DevInfoIndex>0)&&(Qh->DevInfoIndex<gUsbData->MaxDevCount))
                DevInfoInQh = &gUsbDataList->DevInfoTable[Qh->DevInfoIndex];
            (*gAmiUsbCallBackFunTbl[Qh->CallBackIndex - 1 + CALLBACK_FUNCTION_START])
                (HcStruc, DevInfoInQh, (UINT8*)Qh, NULL, 0);
        }
    }

    return  USB_SUCCESS;
}


/**
    Find specific Isochronous transfer descriptor
    
    @param  Itd     Pointer to the specific Isochronous transfer descriptor
    @retval Index   Return the index number.

**/
UINT8
EHCI_FindIOCuFrame (
    EHCI_ITD  *Itd
)
{
    UINT8 Index;

    for (Index = 0; Index < 8; Index++)
    {
        if (Itd->ControlStatus[Index] & BIT15)
        {
            break;
        }
    }
    return Index;
}


/**
    Processing current Itd.
    
    @param  HcStruc   Pointer to the host controller structure
    @retval None

**/
VOID
EHCIProcessCurrentItd (
    HC_STRUC *HcStruc
)
{
    UINT32      LinkPtr;
    UINT8       Index;
    UINT32      Count;
    UINT16      CurrentFIndex;
    EHCI_ITD    *Itd;
    UINT16      FrameCount;
    DEV_INFO    *DevInfo;
    EFI_STATUS  Status = EFI_SUCCESS;

    // Check current and previous iTD for IOC
    CurrentFIndex = GetCurrentFIndex(HcStruc);
    LinkPtr = HcStruc->FrameList[CurrentFIndex];
    Itd = (EHCI_ITD*)(LinkPtr & EHCI_POINTER_MASK);
    Index = EHCI_FindIOCuFrame(Itd);
    if (Index == 8)
    {
        if (CurrentFIndex == 0) CurrentFIndex = HcStruc->AsyncListSize;
        CurrentFIndex--;
        LinkPtr = HcStruc->FrameList[CurrentFIndex];
        Itd = (EHCI_ITD*)(LinkPtr & EHCI_POINTER_MASK);
        Index = EHCI_FindIOCuFrame(Itd);
        if (Index == 8) return; // iTD was not the reason we are here
    }

    Status = AmiUsbValidateMemoryBuffer((VOID*)Itd, sizeof(EHCI_ITD));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return;
    }
    
    // Locate the device by matching iTD with DevInfo->IsocDetails.XferKey
    for (Count = 1; Count < gUsbData->MaxDevCount; Count++) {
        DevInfo = &gUsbDataList->DevInfoTable[Count];
        if ((DevInfo->Flag & DEV_INFO_VALIDPRESENT) != DEV_INFO_VALIDPRESENT) {
            continue;
        }
        if (DevInfo->IsocDetails.XferKey == (UINTN)Itd) break;
    }
    //ASSERT(j < gUsbData->MaxDevCount);
    if (Count == gUsbData->MaxDevCount) return;   // device not found
    
    Itd->ControlStatus[Index] &= ~BIT15;    // Clear IOC
    // Count uFrames
    if (CurrentFIndex < (UINT16)DevInfo->IsocDetails.XferStart)
        CurrentFIndex += HcStruc->AsyncListSize;
    ASSERT(CurrentFIndex >= (UINT16)DevInfo->IsocDetails.XferStart);
    FrameCount = ((CurrentFIndex - (UINT16)DevInfo->IsocDetails.XferStart) << 3) + Index + 1; // i is 0-based
    Status = AmiUsbValidateMemoryBuffer((VOID*)DevInfo->IsocDetails.XferDetails, FrameCount * sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return ;
    }   
    FinalizeIsocXfer(HcStruc, (UINT16)DevInfo->IsocDetails.XferStart,
                     FrameCount, DevInfo->IsocDetails.XferDetails);

    Status = AmiUsbValidateMemoryBuffer((VOID*)DevInfo->IsocDetails.AsyncStatus, sizeof(UINT8));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return;
    }
    *DevInfo->IsocDetails.AsyncStatus = 1;
}

/**
    This function parses through the periodic list to find
    completed qTD.  If a qTD is done it will call its associated
    call back function (in device info structure) and restarts
    the polling qTD

    @param  HcStruc   Pointer to the host controller structure

    @retval None

**/
VOID
EHCIProcessPeriodicList(
    HC_STRUC *HcStruc
)
{
    UINT16      Index;
    UINT32      *PrevPtr;
    UINT32      LinkPtr;
    EHCI_QH     *Qh;
    UINT32      OrgQhLinkPointer;
    EHCI_QH     *QhHostAddr;
    EFI_STATUS  Status;


    if (HcStruc->IsocTds != NULL) {
        EHCIProcessCurrentItd(HcStruc);
    }
    

    //
    // Get the first entry in the periodic list. This QH list will link to all
    // the periodic QH's
    //

    for (Index = 0; Index <= MAX_SPLIT_PERIODIC_NUMBER; Index ++) {
        PrevPtr = &HcStruc->FrameList[Index];
        LinkPtr = *PrevPtr; 

     while (!(LinkPtr & EHCI_TERMINATE)) {
         Qh = (EHCI_QH*)(LinkPtr & EHCI_POINTER_MASK);
            Status = AmiUsbValidateMemoryBuffer((VOID*)Qh, sizeof(EHCI_QH));
            if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
                return;
            }
            QhHostAddr = EhciMemGetHostAddr(HcStruc, Qh);

            // Process only QHeads, skip the other types
            if ((QhHostAddr->dLinkPointer & 6) == EHCI_QUEUE_HEAD) {

                // Check whether this QH is actived
                if (QhHostAddr->bActive == TRUE) {
                    OrgQhLinkPointer = QhHostAddr->dLinkPointer;
                    EHCIProcessQH(HcStruc, QhHostAddr);
                    // Qh is changed, we re-parses through the list.
                    if (QhHostAddr->dLinkPointer != OrgQhLinkPointer) {
                        LinkPtr = *PrevPtr;
                        continue;
                    }
                }
             }

      LinkPtr = QhHostAddr->dLinkPointer;
      }

    }
}

/**
    This function is called when a polling TD from the TD pool 
    completes an interrupt transaction to its assigned device.
    This routine should process any data in the TD's data buffer, 
    handle any errors, and then copy the TD's CSReloadValue field 
    into its control status field to put the TD back into service.

    @param HcStruc       Pointer to the host controller structure
    @param DevInfo       NULL (DevInfo is not valid)
    @param QueueHead     Pointer to the QH that completed
    @param Buffer        Not used
    @param DataLength    Not used
    @retval None

**/

UINT8
EFIAPI
EhciPollingTDCallback(
    HC_STRUC *HcStruc,
    DEV_INFO *DevInfo,
    UINT8  *QueueHead,
    UINT8  *Buffer,
    UINT16      DataLength
)
{
    EHCI_QH*    PollQh = (EHCI_QH*)QueueHead;
    EHCI_QTD    *PollQtd = PollQh->fpFirstqTD;
    UINT16      BytesTransferred;
    UINT32      PortStatus;
    UINT8       *PollDataBufferPhyAddr;
    UINT8       *PollQtdPhyAddr;
    EFI_STATUS  Status;


    Status = AmiUsbValidateMemoryBuffer((VOID*)PollQtd, sizeof(EHCI_QTD));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return USB_ERROR;
    }


    // Update datat toggle value
    UsbUpdateDataToggle(DevInfo, DevInfo->IntInEndpoint, 
    (UINT8)((PollQh->dToken & QH_DATA_TOGGLE) >> 31));

    if (!(PollQh->dToken & QTD_STATUS_FIELD)) {
        if ((DevInfo->CallBackIndex) && (DevInfo->CallBackIndex <= MAX_CALLBACK_FUNCTION)) {
            if (gAmiUsbCallBackFunTbl[DevInfo->CallBackIndex - 1 + CALLBACK_FUNCTION_START]) {
                //
                // Get the size of data transferred
                //
                BytesTransferred = DevInfo->PollingLength - (UINT16)((PollQtd->dToken & ~(QTD_DATA_TOGGLE)) >> 16);
                (*gAmiUsbCallBackFunTbl[DevInfo->CallBackIndex - 1 + CALLBACK_FUNCTION_START])
                        (HcStruc, DevInfo, (UINT8*)PollQtd, DevInfo->PollDataBuffer, BytesTransferred);
            }
      }
     } else {
        if (PollQh->dToken & QTD_XACT_ERROR) {
            // When the device is disconnecting, the transaction may be error, 
            // check if the port status is enabled.
            PortStatus = USB_GetHubPortStatus(HcStruc, DevInfo->HubDeviceNumber, DevInfo->HubPortNumber, FALSE);
            if ((PortStatus == USB_ERROR) || (!(PortStatus & USB_PORT_STAT_DEV_ENABLED))) {
                return USB_SUCCESS;
            }
        }
    }

    // Keep the PID code bit in the qTD
    PollQtd->dToken &= QTD_DIRECTION_PID;
    
    //
    // Set length
    //
    PollQtd->dToken |= (UINT32)DevInfo->PollingLength << 16;

    //
    // Update buffer pointers
    //
    PollDataBufferPhyAddr = EhciMemGetPhyAddr(HcStruc, DevInfo->PollDataBuffer);
    
    EHCISetQTDBufferPointers(PollQtd,
                             PollDataBufferPhyAddr, DevInfo->PollingLength);

    //
    // Re-init the QH pointers
    //
    PollQh->dToken &= QH_DATA_TOGGLE; 
    PollQh->dCurqTDPtr = 0;
    PollQh->dAltNextqTDPtr = EHCI_TERMINATE;
    PollQh->dBufferPtr0 = 0;
    PollQh->dBufferPtr1 = 0;
    PollQh->dBufferPtr2 = 0;
    PollQh->dBufferPtr3 = 0;
    PollQh->dBufferPtr4 = 0;
    //
    // Update next & alternate next qTD pointers
    //
    PollQtd->dNextqTDPtr = EHCI_TERMINATE;
    PollQtd->dAltNextqTDPtr = EHCI_TERMINATE;
    PollQtdPhyAddr = EhciMemGetPhyAddr(HcStruc, PollQtd);
    PollQh->dNextqTDPtr = (UINT32)(UINTN)PollQtdPhyAddr;

    //
    // Restart the qTD
    //
    PollQtd->dToken |= (QTD_IOC_BIT | QTD_THREE_ERRORS | QTD_ACTIVE);
    PollQh->bActive = TRUE;
 
    return  USB_SUCCESS;
}

/**
    This function is called when qTdRepeat completes
    a transaction.  This qTD runs a dummy interrupt transaction
    to a non-existant device address for the purpose of
    generating a periodic timeout interrupt which in turn
    is used to generate keyboard repeat or update LED status.

    @param HcStruc   Pointer to the HCStruc structure
    @param DevInfo   NULL (DevInfo is not valid)
    @param QueueHead Pointer to the QH that completed
    @param Buffer    Not used
    @param DataLength    Not used
    
    @retval USB_SUCCESS On success
    @retval Others      On fail

**/

UINT8
EFIAPI
EhciRepeatTDCallback(
     HC_STRUC    *HcStruc,
     DEV_INFO    *DevInfo,
     UINT8       *QueueHead,
     UINT8       *Buffer,
     UINT16      DataLength
)
{
    EHCI_QH     *RepeatQh = (EHCI_QH*)QueueHead;
    EHCI_QTD    *RepeatQtd = RepeatQh->fpFirstqTD;
    EFI_STATUS  Status;


    Status = AmiUsbValidateMemoryBuffer((VOID*)RepeatQtd, sizeof(EHCI_QTD));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
        return USB_ERROR;
    }

    if ((gUsbData->UsbDevSupport & USB_KB_DEV_SUPPORT) == USB_KB_DEV_SUPPORT)
    USBKBDPeriodicInterruptHandler(HcStruc);

    if (RepeatQh->dTokenReload & QTD_ACTIVE) {
        //
        // Update buffer pointers
        //
        EHCISetQTDBufferPointers(RepeatQtd,
                                 &RepeatQh->aDataBuffer[0], 8);

        //
        // Re-init the QH pointers
        //
        RepeatQh->dToken &= QH_DATA_TOGGLE;
        RepeatQh->dCurqTDPtr = 0;
        RepeatQh->dAltNextqTDPtr = EHCI_TERMINATE;
        RepeatQh->dBufferPtr0 = 0;
        RepeatQh->dBufferPtr1 = 0;
        RepeatQh->dBufferPtr2 = 0;
        RepeatQh->dBufferPtr3 = 0;
        RepeatQh->dBufferPtr4 = 0;
        RepeatQh->dNextqTDPtr = (UINT32)(UINTN)RepeatQtd;

        //
        // Restart the qTD
        //
        RepeatQh->dToken = 0;
        RepeatQtd->dToken = RepeatQh->dTokenReload;
        
        RepeatQh->bActive = TRUE;
    }

    return  USB_SUCCESS;
}

/**
    This function check whether HC is halted.
    @param HcStruc  Pointer to the host controller structure
    @retval TRUE    Ehci controller is halted
    @retval FALSE   Ehci controller is not halted
**/
BOOLEAN
EhciIsHalted (
    HC_STRUC *HcStruc
)
{
    return (HcReadOpReg(HcStruc, EHCI_USBSTS) & EHCI_HCHALTED) == EHCI_HCHALTED;
}

/**
    This function calculates the polling rate in frames unit.
    @param Speed          Link speed
    @param Interval       Polling interval value
    @retval PollingRate   Polling rate
**/
UINT16
EhciTranslateInterval(
    UINT8   Speed,
    UINT8   Interval
)
{
    UINT8   BitCount = 0;
    UINT16  PollingRate;

    if (Speed == USB_PORT_STAT_DEV_HISPEED) {
        // For high-speed interrupt endpoints, the Interval value must be 
        // from 1 to 16
        ASSERT(Interval >= 1 && Interval <= 16);
 
        PollingRate = (1 << (Interval - 1)) >> 3;
        return PollingRate != 0 ? PollingRate : 1;
    }

    // For full-/low-speed interrupt endpoints, the Interval value should 
    // be from 1 to 255
    ASSERT(Interval >= 1 && Interval <= 255);

    for (BitCount = 0; Interval != 0; BitCount++) {
        Interval >>= 1;
    }
    return (1 << (BitCount - 1));
}


/**
    Get physical memory address

    @param HcStruc Pointer to the host controller structure
    @param MemPtr  Memory address   
    @retval physical memory address
**/
VOID*
EhciMemGetPhyAddr(
    HC_STRUC  *HcStruc,
    VOID      *MemPtr
)
{

    if (gUsbData->UsbRuntimeDriverInSmm) {
        return MemPtr;
    } else {
        return (VOID*)(HcStruc->MemPoolPhyAddr + ((UINT8*)MemPtr - HcStruc->MemPool));
    }

}

/**
    Get host controller address

    @param HcStruc  Pointer to the host controller structure
    @param  MemPtr  Memory address   
    @retval MemPtr  Memory address  
**/
VOID*
EhciMemGetHostAddr(
    HC_STRUC  *HcStruc,
    VOID      *MemPtr
)
{

    if (gUsbData->UsbRuntimeDriverInSmm) {
        return MemPtr;
    } else {
        return (VOID*)(HcStruc->MemPool + ((UINT8*)MemPtr - HcStruc->MemPoolPhyAddr));
    }
}

/**
    Allocate memory

    @param HcStruc  Pointer to the host controller structure  
    @param NumBlks  Block number
    @retval None
**/
VOID*
EFIAPI
EhciMemAlloc(
     HC_STRUC *HcStruc,
     UINT16  NumBlks
)
{

    if (gUsbData->UsbRuntimeDriverInSmm){
      return USB_MemAlloc(NumBlks);

    } else {
    UINT32 SavedMemPages = gUsbData->MemPages;
    UINT8 *SavedMemPool = gUsbDataList->MemBlockStart;
    UINT32 SavedMemBlkStsBytes = gUsbData->MemBlkStsBytes;
    UINT32 *SavedMemBlsSts = gUsbDataList->MemBlkSts;
    VOID* MemPtr;

    gUsbData->MemPages = HcStruc->MemPoolPages;
    gUsbDataList->MemBlockStart = HcStruc->MemPool;
    gUsbData->MemBlkStsBytes = HcStruc->MemBlkStsBytes;
    gUsbDataList->MemBlkSts = HcStruc->MemBlkSts;

    MemPtr = USB_MemAlloc(NumBlks);

    gUsbData->MemPages = SavedMemPages;
    gUsbDataList->MemBlockStart = SavedMemPool;
    gUsbData->MemBlkStsBytes = SavedMemBlkStsBytes;
    gUsbDataList->MemBlkSts = SavedMemBlsSts;

    return MemPtr;
    }

}

/**
    Free memory

    @param HcStruc  Pointer to the host controller structure
    @param MemPtr   Memory address   
    @param NumBlks  Block number
    @retval None
**/
VOID
EhciMemFree(
    HC_STRUC *HcStruc,
    VOID*  MemPtr,
    UINT16  NumBlks
)
{

    if (gUsbData->UsbRuntimeDriverInSmm){
      USB_MemFree(MemPtr, NumBlks);
    } else {
    UINT32 SavedMemPages = gUsbData->MemPages;
    UINT8 *SavedMemPool = gUsbDataList->MemBlockStart;
    UINT32 SavedMemBlkStsBytes = gUsbData->MemBlkStsBytes;
    UINT32 *SavedMemBlsSts = gUsbDataList->MemBlkSts;

    gUsbData->MemPages = HcStruc->MemPoolPages;
    gUsbDataList->MemBlockStart = HcStruc->MemPool;
    gUsbData->MemBlkStsBytes = HcStruc->MemBlkStsBytes;
    gUsbDataList->MemBlkSts = HcStruc->MemBlkSts;

    USB_MemFree(MemPtr, NumBlks);

    gUsbData->MemPages = SavedMemPages;
    gUsbDataList->MemBlockStart = SavedMemPool;
    gUsbData->MemBlkStsBytes = SavedMemBlkStsBytes;
    gUsbDataList->MemBlkSts = SavedMemBlsSts;

    return;
    }

}


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
