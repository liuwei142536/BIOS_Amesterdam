//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file CsmLib.c
  Functions and variables used in different CSM modules
*/

#include "Csm.h"
#include "AmiDxeLib.h"
#include "AcpiRes.h"
#include <Protocol/SimplePointer.h>
#include <Protocol/BlockIo.h>
#include "Token.h"
#include "Pci.h"
#include <Protocol/SerialIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SuperIo.h>
#include <Protocol/ConsoleControl.h>

#define PS2KBD_PnpID    0x303
#define PS2MS_PnpID1    0xf03
#define PS2MS_PnpID2    0xf12
#define PS2MS_PnpID3    0xf13
#define UART_PnpID      0x501
#define LPT_PnpID       0x400
#define LPT_ECP_PnpID   0x401
#define FDD_PnpID       0x604

extern BIOS_INFO *gCoreBiosInfo;
extern UINTN gMaxOpRomAddress;
extern EFI_GUID gAmiLegacyBootProtocolGuid;

VOID        *gEmbeddedRom;
UINTN       gEmbeddedRomSize;
EFI_CONSOLE_CONTROL_PROTOCOL *gConsoleControl = NULL;
BOOLEAN     gConsoleLocked = FALSE;
extern BOOLEAN gDoNotUpdateBbsTable;
extern BOOLEAN gBbsUpdateInProgress;

UINT16 PS2PnpIDs[] = { PS2KBD_PnpID, 0 };
UINT16 PS2MSPnpIDs[] = { PS2MS_PnpID1, PS2MS_PnpID2, PS2MS_PnpID3, PS2KBD_PnpID, 0 };
UINT16 UARTPnpIDs[] = { UART_PnpID, 0};
UINT16 LPTPnpIDs[] = { LPT_PnpID, 0 };
UINT16 FDDPnpIDs[] = { FDD_PnpID, 0 };

static CSM_SIO_DEVICE gCsmSioDevice[5] = {
    { CsmPS2KeyboardDevice,   PS2PnpIDs,    0xa, IsConnectedDummy },
    { CsmPS2MouseDevice,      PS2MSPnpIDs,  0xe, IsConnectedPS2MS },
    { CsmComPortDevice,       UARTPnpIDs,   0x0, IsConnectedDummy },
    { CsmLptPortDevice,       LPTPnpIDs,    0x2, IsConnectedDummy },
    { CsmFddDevice,           FDDPnpIDs,    0x3, IsConnectedFloppy},
};

typedef struct _ACPI_PNPID {
    UINT32  Signature   :16;
    UINT32  PnpID       :16;
} ACPI_PNPID;

EFI_HANDLE  *gSerialIoHandleBuffer=NULL;
UINTN       gSerialIoHandleCount=0;
UINT32      gSaveIrq1 = 0;
extern UINT8 gSetTxtMode;

/**
    Locates the FFS in the Firmware Volume and loads it into the input Buffer

    @param Guid File GUID to read
    @param Buffer 
    @param BufferSize 

    @retval EFI_STATUS
        VOID  **Buffer - Contains the content of the Guid file
        UINTN *BufferSize - Size of the output buffer

**/
EFI_STATUS
LoadFileSection(
    IN      EFI_GUID  				*Guid,
    IN OUT  VOID      				**Buffer,
    IN OUT  UINTN     				*BufferSize
)
{
    EFI_STATUS                    Status;
    EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
    UINT32                        Authentication;
    UINTN                         NumHandles;
    EFI_HANDLE                    *HandleBuffer;
    UINTN                         i;

    Status = pBS->LocateHandleBuffer(ByProtocol,&gEfiFirmwareVolume2ProtocolGuid,NULL,&NumHandles,&HandleBuffer);
    if (EFI_ERROR(Status)) return Status;

    for (i = 0; i< NumHandles; ++i) {
        Status = pBS->HandleProtocol(HandleBuffer[i],&guidFV,(VOID**)&Fv);
        if (EFI_ERROR(Status)) continue;

        Status = Fv->ReadSection(Fv,
                                Guid,
                                EFI_SECTION_RAW,
                                0,        //Instance
                                Buffer,
                                BufferSize,
                                &Authentication);

        if (Status == EFI_SUCCESS) break;
    }

    pBS->FreePool(HandleBuffer);
    return Status;
}

/**
    This function searches for ROM within the data previously loaded
    from the main FV image and returns its pointer and size.

    @retval EFI_SUCCESS or EFI_NOT_FOUND

**/

EFI_STATUS FindEmbeddedRom(
    UINT16 ModuleId, UINT16 VendorId, UINT16 DeviceId,
    VOID **ImageStart, UINTN *ImageSize
)
{
    OPROM_HEADER *Header;
    EFI_STATUS Status;
    PARTIAL_OPROM_HEADER *PartialHeader = NULL;
    UINT8 NumAdditionalDevicesWithSameOpRom = 0;
    UINT32 SizeOfPartialHeaders = 0;
    UINT32 i = 0;
    PCI_DATA_STRUCTURE *pcir;
    UINT16 *DeviceIDList;

    Status = GetPlatformEmbeddedRom(ModuleId, VendorId, DeviceId, ImageStart, ImageSize);
    if (!EFI_ERROR(Status)) return Status;

    for(  Header = (OPROM_HEADER*)gEmbeddedRom
        ; (UINT8*)Header<(UINT8*)gEmbeddedRom+gEmbeddedRomSize
        ; Header = (OPROM_HEADER*)((UINT8*)(Header+1)+
                                    SizeOfPartialHeaders+
                                    Header->Size)
    )
    {
        if (   Header->ModuleId==ModuleId
            && Header->VendorId==VendorId
            && Header->DeviceId==DeviceId
        )
        {
            *ImageStart = Header+1;
            *ImageSize=Header->Size;
            return EFI_SUCCESS;
        }
        else
        {
            // try LinkDid/LinkVid for OpROMs
            //If there is more than 1 device using an option rom,
            //the header of the first device has a module id with the follow format:
            //  0xA0XX
            //where the least significant byte is the additional number of devices using the option ROM.
            // e.g. If there are 2 devices using the option rom, then the header of the
            //first device has module id 0xA001.
            //The first device device has a complete header, and the remaining devices have partial headers.
            //A partial header is the complete header without the 'Size' field.
            if ((Header->ModuleId & 0xFF00) == OPROM_MULTIPLE_MODULEID)
            {
                NumAdditionalDevicesWithSameOpRom = (UINT8) (Header->ModuleId & 0xFF);
                SizeOfPartialHeaders = sizeof(PARTIAL_OPROM_HEADER) *
                                        NumAdditionalDevicesWithSameOpRom;
                //check the first header
                //we do not check the module id
                if ((Header->VendorId == VendorId) &&
                    (Header->DeviceId==DeviceId)) {
                    *ImageStart = (UINT8 *)(Header+1)+SizeOfPartialHeaders;
                    *ImageSize=Header->Size;
                    return EFI_SUCCESS;
                }
                //Check the partial headers of the remaining devices using this option rom.
                //The first partial header directly follows the regular header.
                PartialHeader = (PARTIAL_OPROM_HEADER *)(Header+1);
                for (i = 0; i < NumAdditionalDevicesWithSameOpRom; i++) {
                    if ((PartialHeader->ModuleId == ModuleId) &&
                        (PartialHeader->VendorId == VendorId) &&
                        (PartialHeader->DeviceId == DeviceId)) {
                        *ImageStart = (UINT8 *)(Header+1)+SizeOfPartialHeaders;
                        *ImageSize=Header->Size;
                        return EFI_SUCCESS;
                    }
                    PartialHeader++;
                }
            } else {
                SizeOfPartialHeaders = 0;
            }
        }
        *ImageStart = 0;
        *ImageSize = 0;
    }

    // Linked List failed. So check if it is a PCI 3.0 Option and has a valid Device List pointer
    for(  Header = (OPROM_HEADER*)gEmbeddedRom
        ; (UINT8*)Header<(UINT8*)gEmbeddedRom+gEmbeddedRomSize
        ; Header = (OPROM_HEADER*)((UINT8*)(Header+1)+
                                    SizeOfPartialHeaders+
                                    Header->Size)
    )
    {
        UINTN DummyImageSize;

        if ((Header->ModuleId & 0xFF00) == OPROM_MULTIPLE_MODULEID)
        {
            NumAdditionalDevicesWithSameOpRom = (UINT8) (Header->ModuleId & 0xFF);
            SizeOfPartialHeaders = sizeof(PARTIAL_OPROM_HEADER) * NumAdditionalDevicesWithSameOpRom;
            PartialHeader = (PARTIAL_OPROM_HEADER *)(Header+1);
            for (i = 0; i < NumAdditionalDevicesWithSameOpRom; i++) {
                PartialHeader++;
            }
        } else {
            SizeOfPartialHeaders = 0;
        }

        *ImageStart = (UINT8 *)(Header+1)+SizeOfPartialHeaders;
        *ImageSize = Header->Size;
        if (Check30ROM(*ImageStart, &DummyImageSize, NULL)){
            // Check for valid Device List Pointer
            pcir = (PCI_DATA_STRUCTURE *)((UINT8*)*ImageStart + *(UINT16*)((UINT8*)*ImageStart + 0x18));
            if ((pcir->VendorId == VendorId) && (pcir->Reserved0)) { //(EIP42523)
                DeviceIDList = (UINT16 *)((UINT8 *)pcir +  pcir->Reserved0);
                for (;*DeviceIDList; DeviceIDList++ ){
                    if (*DeviceIDList == DeviceId) {
                        return EFI_SUCCESS;
                    }
                }
            }
        }
        *ImageStart = 0;
        *ImageSize = 0;
    }

    return EFI_NOT_FOUND;

}


EFI_STATUS IsConnectedDummy(EFI_HANDLE handle)
{
    return EFI_UNSUPPORTED;
}

EFI_STATUS IsConnectedPS2MS(EFI_HANDLE handle)
{
    EFI_STATUS Status;
    EFI_SIMPLE_POINTER_PROTOCOL *spp;
    EFI_SIMPLE_POINTER_STATE state;

    Status = pBS->HandleProtocol(handle,&gEfiSimplePointerProtocolGuid,(VOID**)&spp);
    if (!EFI_ERROR(Status)) {
        Status = spp->GetState(spp, &state);
        // ask A. to fix this if - should return DEVICE_ERROR, not NO_READY
        //if (Status != EFI_DEVICE_ERROR) {
        if (Status != EFI_NOT_READY) {
            Status = EFI_SUCCESS;
        }
    }
    return Status;
}


EFI_STATUS IsConnectedFloppy(EFI_HANDLE handle)
{
    EFI_STATUS Status;
    EFI_BLOCK_IO_PROTOCOL *blkiop;

    Status = pBS->HandleProtocol(handle,&gEfiBlockIoProtocolGuid,(VOID**)&blkiop);

    return Status;
}


/**
    Returns the presence of certain SIO device. If present, also returns device handle.

**/

EFI_STATUS
GetSioDeviceHandle(
    IN CSM_SIO_DEVICE_TYPE Device,
    OUT EFI_HANDLE    *hDevice
)
{
    UINTN               i;
    UINTN               cnt;
    EFI_STATUS          Status;
    ACPI_HID_DEVICE_PATH    *siodp;
    ACPI_PNPID          *pnpid;
    EFI_HANDLE          *hbuff;
    EFI_DEVICE_PATH_PROTOCOL    *dp;
    BOOLEAN             IsConnected;
    UINT16              *DevPnpId;

    hbuff = NULL;
    Status = pBS->LocateHandleBuffer(ByProtocol, &gEfiSioProtocolGuid, NULL, &cnt, &hbuff);
    if (EFI_ERROR(Status)) return EFI_NOT_FOUND;

    for (i = 0, IsConnected = FALSE; i < cnt; i++) {
        Status = pBS->HandleProtocol(hbuff[i], &gEfiDevicePathProtocolGuid, (VOID**)&dp);
        if (EFI_ERROR(Status)) continue;

        siodp = DPGetLastNode(dp);
        if (siodp->Header.Type != ACPI_DEVICE_PATH
            || siodp->Header.SubType != ACPI_DP
            || NODE_LENGTH(&siodp->Header) != ACPI_DEVICE_PATH_LENGTH) continue;

        pnpid = (ACPI_PNPID*)&siodp->HID;
        for (DevPnpId = gCsmSioDevice[Device].PnpIDs; *DevPnpId; DevPnpId++) {
            if (pnpid->PnpID == *DevPnpId) {
                IsConnected = !EFI_ERROR(gCsmSioDevice[Device].IsConnectedFunction(hbuff[i]));
                if (IsConnected) *hDevice = hbuff[i];
                break;
            }
        }
        if (*DevPnpId) break;
    }
    pBS->FreePool(hbuff);
    
    return IsConnected? EFI_SUCCESS : EFI_NOT_FOUND;
}


/**
    Returns present/absent status of SIO device.

**/

BOOLEAN
IsSioDevicePresent(
    IN CSM_SIO_DEVICE_TYPE Device
)
{
    EFI_STATUS  Status;
    BOOLEAN     IsConnected;
    UINT16      SioDevStatus;
    UINTN       SioDevStatusSize;
    EFI_HANDLE  Handle;

    static EFI_GUID  SioDevStatusVarGuid = {0x5820de98, 0xfc8e, 0x4b0b, {0xa4, 0xb9, 0xa, 0x94, 0xd, 0x16, 0x2a, 0x7e}};

    // Try to get device presence from NVRAM; if variable is not found, try to locate device from ACPI DP node
    //
    SioDevStatusSize = sizeof(UINT16);

    Status = pRS->GetVariable(
                L"SIO_DEV_STATUS_VAR",
                &SioDevStatusVarGuid,
                NULL,
                &SioDevStatusSize,
                &SioDevStatus);

    if (!EFI_ERROR(Status)) {
        IsConnected = SioDevStatus & (1 << gCsmSioDevice[Device].SioDevStatusBit);
    } else {
        Status = GetSioDeviceHandle(Device, &Handle);
        IsConnected = !EFI_ERROR(Status);
    }
    
    return IsConnected;
}


/**
    Returns Address and Irq of the device

        
    @param PnpId     Device ACPI PnP ID
    @param PortNo    Device ACPI UID
    @retval Address  Device IO Address
    @retval Irq      Device IRQ

    @retval 
        EFI_STATUS Status

**/

EFI_STATUS
GetResource (
    UINT16    PnpId,
    UINT8     PortNo,
    UINT16    *Address,
    UINT8     *Irq
)
{
    EFI_HANDLE                  *Handle = NULL;
    UINTN                       Count;
    UINTN                       i;
    EFI_STATUS                  Status;
    UINT8                       PortCount = (UINT8)-1;
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath = NULL;
    ACPI_HID_DEVICE_PATH        *AcpiPrevDPNodePtr = NULL;
    ACPI_RESOURCE_HEADER_PTR    ResourceList;
    EFI_SIO_PROTOCOL            *EfiSio=NULL;
    ACPI_SMALL_RESOURCE_HEADER  *Hdr=NULL;

    *Address=0;
    *Irq=0;

    Status = pBS->LocateHandleBuffer(ByProtocol,
               &gEfiSioProtocolGuid, 
               NULL,
               &Count, 
               &Handle);

    if(EFI_ERROR(Status)) 
        return Status;

    for (i = 0; i < Count; i++) {

        Status = pBS->HandleProtocol(Handle[i],
                   &gEfiDevicePathProtocolGuid,
                   (VOID**)&DevicePath);

        if(!EFI_ERROR(Status) && (DevicePath != NULL)) {

            for ( ;!isEndNode(DevicePath); DevicePath = NEXT_NODE(DevicePath)) {
                AcpiPrevDPNodePtr = (ACPI_HID_DEVICE_PATH *)(DevicePath);

                if ((AcpiPrevDPNodePtr->Header.Type == ACPI_DEVICE_PATH) && 
                           (AcpiPrevDPNodePtr->Header.SubType == ACPI_DP) && 
                           (AcpiPrevDPNodePtr->HID == EISA_PNP_ID(PnpId))) {

                    if(PnpId == UART_PnpID) {
                        PortCount++;
                        //
                        // Check if we have reached "ComNo" th instance of UART devices
                        //
                        if(PortCount != PortNo)
                            continue;
                    }

                    Status = pBS->HandleProtocol(Handle[i],
                               &gEfiSioProtocolGuid,
                               (VOID**)&EfiSio);

                    if(!(EFI_ERROR( Status ))) {

                        ResourceList.SmallHeader = NULL;
                        Status = EfiSio->GetResources(EfiSio,
                                                        &ResourceList);

                        if(!(EFI_ERROR( Status ))) {

                            if (ResourceList.SmallHeader){
                                for(Hdr=ResourceList.SmallHeader; (Hdr->Byte)!=ASLV_END_TAG_HDR; Hdr+=((sizeof(ACPI_SMALL_RESOURCE_HEADER))+Hdr->Bits.Length)) {
                                    switch(Hdr->Bits.Name) {
                                        case ASLV_RT_FixedIO:
                                            if (*Address == 0) *Address = ((ASLR_FixedIO*)Hdr)->_BAS;
                                            break;
                                        case ASLV_RT_IO:
                                            if (*Address == 0) *Address = ((ASLR_IO*)Hdr)->_MIN;
                                            break;
                                        case ASLV_RT_IRQ:
                                            if (*Irq == 0) *Irq = (UINT8)((ASLR_IRQNoFlags*)Hdr)->_INT;
                                            break;
                                    }

                                    if(*Address!=0 && *Irq!=0) {
                                        pBS->FreePool(Handle);
                                        return EFI_SUCCESS;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    pBS->FreePool(Handle);
    return EFI_NOT_FOUND;

}

/**
    Returns COM port base address and interrupt

    @param ComNo - COM port number, 0-based.

    @retval EFI_NOT_FOUND COM port is not found.
    @retval EFI_SUCCESS COM port found, the output variables are updated:
    @retval Address IO base address
    @retval Irq IRQ number

**/

EFI_STATUS
GetComPortResource(
    IN UINT8    ComNo,
    OUT UINT16  *Address,
    OUT UINT8   *Irq
)
{
    EFI_STATUS          Status;

    // Hide COM1 from legacy OS if we are using Trace in Runtime
    if (ComNo == 0) {
#if RUNTIME_TRACE_SUPPORT
        return EFI_NOT_FOUND;
#endif
    }

    Status = GetResource (UART_PnpID, ComNo, Address, Irq);
    return Status;
}


/**
    Returns LPT base address and interrupt

**/

EFI_STATUS
GetLptResource(
    OUT UINT16  *Address,
    OUT UINT8   *Irq
)
{
    EFI_STATUS Status;

    Status = GetResource(LPT_PnpID, 0, Address, Irq);
    if (EFI_ERROR(Status)) {
		Status = GetResource(LPT_ECP_PnpID, 0, Address, Irq);
    }

    return Status;
}


/**
    This function attempts to allocate EBDA.

    @param EBDA size in kilo Bytes

    @retval 32 bit pointer of the allocated memory block inside EBDA
        The allocated address offset off the beginning of EBDA
        EFI_OUT_OF_RESOURCES if EBDA can not be allocated
        EFI_SUCCESS if memory is successfully allocated

    @note  - This routine can be called multiple times and every time EBDA
              data is copied to a different location. It is the caller
              responsibility to obtain the current EBDA segment and its size
              and adjust the offsets before accessing the data.
              - NewEbdaAddress could be used immediately after allocation. If
              the allocated block is not used immediately, - any successive
              call moves the existing data - then the caller should use
              EbdaStartOffset to calculate the correct address of the previously
              allocated data.

**/

EFI_STATUS
EFIAPI
AllocateEbda(
    IN EFI_LEGACY_BIOS_EXT_PROTOCOL *This,
    IN UINT8    RequestedSizeKB,
    OUT UINT32  *NewEbdaAddress,
    OUT UINT32  *EbdaStartOffset
)
{
    UINT16 CurEbdaSeg = *(UINT16*)(UINTN)0x40E;
    UINTN  CurEbdaLocation, NewEbdaLocation;
    UINT8  CurEbdaSizeKB;
    UINTN  CurEbdaSize, EbdaSize;
    UINTN  MinEbdaAddress = 0xA0000-PMM_EBDA_LOMEM_SIZE;

    //
    // Check the current EBDA allocation and see if there is still room in it
    //
    CurEbdaLocation = (UINTN)CurEbdaSeg<<4;
    ASSERT(CurEbdaLocation > MinEbdaAddress);   // either not allocated or already full

    EbdaSize = (UINTN)RequestedSizeKB << 10;
    NewEbdaLocation = CurEbdaLocation - EbdaSize;

    if (NewEbdaLocation < MinEbdaAddress) {
        return EFI_OUT_OF_RESOURCES;
    }

    //
    // Do the allocation:
    // - copy old EBDA
    // - clear the allocated EBDA
    // - update EBDA pointer and EBDA size
    //
    CurEbdaSizeKB = *(UINT8*)CurEbdaLocation;
    CurEbdaSize = (UINTN)CurEbdaSizeKB<<10;
    
    ACCESS_PAGE0_CODE(
        pBS->CopyMem((VOID*)NewEbdaLocation, (VOID*)CurEbdaLocation, CurEbdaSize);
    
        pBS->SetMem((UINT8*)(NewEbdaLocation+CurEbdaSize), EbdaSize, 0);
    
        *(UINT16*)(UINTN)0x40E = (UINT16)(NewEbdaLocation>>4);
        *(UINT8*)NewEbdaLocation = CurEbdaSizeKB + RequestedSizeKB;
    
        //
        // Update 40:13 with the new size of available base memory
        //
        *(UINT16*)(UINTN)0x413 = (*(UINT16*)(UINTN)0x413) - RequestedSizeKB;
    );
    //
    // Update the EBDA information for the saved OpROMs
    //
    UpdateEbdaMap((UINT32)EbdaSize);

    *NewEbdaAddress = (UINT32)(NewEbdaLocation+CurEbdaSize);
    *EbdaStartOffset = (UINT32)CurEbdaSizeKB << 10;

    return EFI_SUCCESS;
}


/**
    Opens the shadow region(s) for writing.

    @param Address - starting address
        Size    - size of the region

    @retval LockUnlockAddr address of the first unlocked block
    @retval LockUnlockSize unlocked address space
        Function returns EFI_OUT_OF_RESOURCES if there is not enough room
        in the shadow.
**/

EFI_STATUS
EFIAPI
UnlockShadow(
    IN UINT8    *Address,
    IN UINTN    Size,
    OUT OPTIONAL UINT32  *LockUnlockAddr,
    OUT OPTIONAL UINT32  *LockUnlockSize
)
{
    UINT32 Granularity;
    UINT32 addr, size;
    UINT32 addr_adjustment;
    UINT32 size_adjustment = 0;
    EFI_STATUS Status;

    addr = (UINT32)(UINTN)Address;
    size = (UINT32)Size;
    if (size) {
        Status = gCoreBiosInfo->iRegion->UnLock (gCoreBiosInfo->iRegion,
                    addr, size, &Granularity);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return Status;

        addr_adjustment = addr % Granularity;

        if (((addr+size) % Granularity) != 0) {
            size_adjustment = Granularity - ((addr+size) % Granularity);
        }

        // Adjust the address to align with the granularity
        addr -= addr_adjustment;
        // Adjust the size to align with the granularity
        size += addr_adjustment;    // Align left
        size += size_adjustment;

    }

    // moved these outside the if (size) for case when size = 0
    if (LockUnlockAddr != NULL) *LockUnlockAddr = addr;
    if (LockUnlockSize != NULL) *LockUnlockSize = size;

    //
    // Finally make CSM16 area writable for the cases where
    // CSM16 area needs to be updated during DispatchOptionRom
    // call, e.g. for non-BBS OpROMs.
    //
    Status = gCoreBiosInfo->iRegion->UnLock (
        gCoreBiosInfo->iRegion,
        (UINT32)gMaxOpRomAddress+1,
        0xFFFFF-(UINT32)gMaxOpRomAddress,
        &Granularity
    );
    ASSERT_EFI_ERROR(Status);

    return EFI_SUCCESS;
}


/**
    Locks the shadow region(s).

    @param LockAddr    - Starting address of the area to be locked
        LockSize    - Size of the area to be locked

    @retval Status of the lock operation.

    @note  This function will also clear the R/W attribute for the region
          from LockAddr+LockSize to gMaxOpRomAddress; this is needed for
          some OS drivers to be loaded properly (Win PCMCIA driver)
**/

EFI_STATUS
EFIAPI
LockShadow(
    IN UINT32  LockAddr,
    IN UINT32  LockSize
)
{
    UINT32      Granularity;
    EFI_STATUS  Status = EFI_SUCCESS;

    if (LockSize && ((LockAddr+LockSize) < gMaxOpRomAddress)) {
        Status = gCoreBiosInfo->iRegion->Lock (gCoreBiosInfo->iRegion, LockAddr, LockSize, &Granularity);
        ASSERT_EFI_ERROR(Status);
        //
        // Disable Shadow W/R for the unused shadow regions
        // Take care of the boundary conditions: LockAddr+LockSize is not nesessarily
        // within Granularity boundaries. For example, if LockAddr is 0xC0000, LockSize
        // is 0x1400 and Granularity is 0x1000, then area from 0xC0000 to 0xC2000 is
        // locked. The next call must not use 0xC0000+LockSize, otherwise part of the
        // OpROM becomes unreadable.
        //
        if (LockSize % Granularity) LockSize += Granularity;
        LockSize = (LockSize/Granularity)*Granularity;
    }
/* This block affects EDKII implementation of LegacyRegion where Lock/Unlock functions
   manipulate W bit of PAM, and Decode function manipulate R bit of PAM. Consequently,
   when Decode is called with OFF, any further Lock/Unlock call will not make the region
   readable.
	if( LockAddr!=0 && LockSize!=0 ) {
		BOOLEAN On=FALSE;
		if ((LockAddr+LockSize) < gMaxOpRomAddress) {
			Status = gCoreBiosInfo->iRegion->Decode (gCoreBiosInfo->iRegion,
													LockAddr+LockSize,
													(UINT32)gMaxOpRomAddress-LockSize-LockAddr+1,
													&Granularity,
													&On);
			ASSERT_EFI_ERROR(Status);
		}
	}
*/
    gCoreBiosInfo->iRegion->Lock (
        gCoreBiosInfo->iRegion,
        (UINT32)gMaxOpRomAddress+1,
        0xFFFFF-(UINT32)gMaxOpRomAddress,
        &Granularity
    );

    return Status;
}

/**
    This function will clear the R/W attribute for the region
    from DecodeAddr to gMaxOpRomAddress; this is needed for
    some OS drivers to be loaded properly (Win PCMCIA driver).

    @param DecodeAddr    - Starting address of the area to remove the decoding

    @retval Status of the decoding operation.

**/

EFI_STATUS
RemoveDecodingForShadow(
    IN UINT32  DecodeAddr
)
{
    BOOLEAN     On=FALSE;
    EFI_STATUS  Status=EFI_SUCCESS;
    UINT32      Granularity;
    UINT32      addr, size=1;
    UINT32      addr_adjustment, size_adjustment;

    addr = DecodeAddr;

    //
    //Find the nearest starting address
    //
    if (size) {
        DEBUG((DEBUG_INFO, "RemoveDecoding: addr %x\n", addr));
        Status = gCoreBiosInfo->iRegion->UnLock (gCoreBiosInfo->iRegion,
                    addr, size, &Granularity);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return Status;

        addr_adjustment = addr % Granularity;
        size_adjustment = Granularity - ((addr+size) % Granularity);
        // Adjust the address to align with the granularity
        addr -= addr_adjustment;
        // Adjust the size to align with the granularity
        size += addr_adjustment;    // Align left
        size += size_adjustment;
    }

    Status = gCoreBiosInfo->iRegion->Lock (gCoreBiosInfo->iRegion, addr, size, &Granularity);

    if(addr == DecodeAddr) {
        DecodeAddr=addr;
    } else {
        DecodeAddr=addr+size;
    }

    if (DecodeAddr < gMaxOpRomAddress) {

        Status = gCoreBiosInfo->iRegion->Decode (gCoreBiosInfo->iRegion,
                     DecodeAddr,
                    (UINT32)((gMaxOpRomAddress+1)-DecodeAddr),
					&Granularity,
                    &On);
        ASSERT_EFI_ERROR(Status);
    }

    return Status;
}


/**
    This function verifies whether or not the memory is writable. Useful to
    verify if the shadow memory area can be made writable or read-only.

    @param Pointer to the BYTE to be verified

    @retval TRUE if area is writable, otherwise FALSE
**/

BOOLEAN
MemoryIsWritable (
    UINTN   Address
)
{
    UINT8 Data;
    BOOLEAN IsWritable;
    
    Data = *(volatile UINT8*)Address;
    *(volatile UINT8*)Address = ~Data;
    
    IsWritable = (Data != *(volatile UINT8*)Address);
    if (IsWritable)
        *(volatile UINT8*)Address = Data;  // restore original data
    
    return IsWritable;
}


/**
    This function returns a pointer to the BBS table and the
    number of entries.

    @retval BbsTable Pointer to BBS table.
    @retval BbsCount Number of entries in table.

**/

EFI_STATUS
EFIAPI
GetBbsTable (
    IN OUT BBS_TABLE **BbsTable,
    IN OUT UINT8 *BbsCount
)
{
    if ((BbsTable == NULL) || (BbsCount == NULL)) 
        return EFI_INVALID_PARAMETER;

    if (gCoreBiosInfo == NULL) 
        return EFI_NOT_READY;

    if (gCoreBiosInfo->BbsTable == NULL)
        return EFI_NOT_READY;

    *BbsTable = gCoreBiosInfo->BbsTable;
    *BbsCount = gCoreBiosInfo->BbsEntriesNo;

    return EFI_SUCCESS;
}

/**
    This function adds given BBS entry to a BBS table.

    @param BbsEntry - BBS table entry to be added to a table.

**/

EFI_STATUS
EFIAPI
AddBbsEntry (
    IN BBS_TABLE *BbsEntry
)
{
    if (gDoNotUpdateBbsTable) return EFI_ACCESS_DENIED;
    if (gBbsUpdateInProgress) return EFI_NOT_READY;

    gBbsUpdateInProgress = TRUE;

    gCoreBiosInfo->BbsTable[gCoreBiosInfo->BbsEntriesNo] = *BbsEntry;
    gCoreBiosInfo->BbsTable[gCoreBiosInfo->BbsEntriesNo].BootPriority = BBS_UNPRIORITIZED_ENTRY;
    gCoreBiosInfo->BbsEntriesNo++;

    gBbsUpdateInProgress = FALSE;

    return EFI_SUCCESS;
}


/**
    This function is an extension of AddBbsEntry function; it
    allows to add a BBS_ENTRY at a particular location or add
    a new entry and return its new location.

    @param This        - Instance of LegacyBiosExt protocol.
	@param EntryNumber - Pointer to 0-based index in BBS table. The
        value of 0xFF will generate a new entry.

    @retval EFI_INVALID_PARAMETER EntryNumber is NULL.
    @retval EFI_SUCCESS EntryNumber is updated with the index of updated
        BBS entry.

**/

EFI_STATUS
EFIAPI
InsertBbsEntryAt (
    IN EFI_LEGACY_BIOS_EXT_PROTOCOL *This,
    IN BBS_TABLE *BbsEntry,
    IN OUT UINT8 *EntryNumber
)
{
    UINT8 EntryNo;

    if (gDoNotUpdateBbsTable) return EFI_ACCESS_DENIED;
    if (gBbsUpdateInProgress) return EFI_NOT_READY;

    if (EntryNumber == NULL) return EFI_INVALID_PARAMETER;

    gBbsUpdateInProgress = TRUE;

    if (*EntryNumber != 0xff) {
        EntryNo = *EntryNumber;
    } else {
        //
        // Find an empty entry within MAX_IDE_CONTROLLER*2+1..BbsEntriesNo-1
        //
        for (EntryNo = MAX_IDE_CONTROLLER*2+1; EntryNo < gCoreBiosInfo->BbsEntriesNo; EntryNo++)
        {
            if (gCoreBiosInfo->BbsTable[EntryNo].BootPriority == BBS_IGNORE_ENTRY)
            {
                break;
            }
        }
        //
        // If not found, EntryNo will be equal BbsEntriesNo
        //
    }
    gCoreBiosInfo->BbsTable[EntryNo] = *BbsEntry;
    gCoreBiosInfo->BbsTable[EntryNo].BootPriority = BBS_UNPRIORITIZED_ENTRY;

    //
    // BbsEntriesNo should not be incremented for the BBS entries
    // with indexes from 0 to CoreBiosInfo->BbsEntriesNo-1
    //
    if (EntryNo >= gCoreBiosInfo->BbsEntriesNo) {
        gCoreBiosInfo->BbsEntriesNo++;
    }
    *EntryNumber = EntryNo;

    gBbsUpdateInProgress = FALSE;

    return EFI_SUCCESS;
}


/**
    This function removes BBS entry from BBS table.

    @param This        - Instance of LegacyBiosExt protocol.
    @param EntryNumber - Entry index to be removed.

    @retval EFI_INVALID_PARAMETER EntryNumber exceeds the current table.
    @retval EFI_NOT_FOUND Entry does not exist.
    @retval EFI_SUCCESS Entry has been successfully removed.

**/

EFI_STATUS
EFIAPI
RemoveBbsEntryAt (
    IN EFI_LEGACY_BIOS_EXT_PROTOCOL *This,
    IN UINT8 EntryNumber
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (gDoNotUpdateBbsTable) return EFI_ACCESS_DENIED;
    if (gBbsUpdateInProgress) return EFI_NOT_READY;

    gBbsUpdateInProgress = TRUE;

    //
    // Can only remove a particular entry within 0..CoreBiosInfo->BbsEntriesNo-1 range;
    //
    if (EntryNumber >= gCoreBiosInfo->BbsEntriesNo) {
        Status = EFI_INVALID_PARAMETER;
    }

    if (!EFI_ERROR(Status)) {
        if (gCoreBiosInfo->BbsTable[EntryNumber].BootPriority == BBS_IGNORE_ENTRY) {
            Status = EFI_NOT_FOUND;
        }
    }

    if (!EFI_ERROR(Status)) {
        //
        // Decrement CoreBiosInfo->BbsEntriesNo only if requested index equals
        // gCoreBiosInfo->BbsEntriesNo-1
        //
        if (EntryNumber == (gCoreBiosInfo->BbsEntriesNo-1)) {
            gCoreBiosInfo->BbsEntriesNo--;
        }
    
        pBS->SetMem(&gCoreBiosInfo->BbsTable[EntryNumber], sizeof(BBS_TABLE), 0);
        gCoreBiosInfo->BbsTable[EntryNumber].BootPriority = BBS_IGNORE_ENTRY;
    }

    gBbsUpdateInProgress = FALSE;

    return Status;
}

/**
    Internal function that installs/uninstall protocol with a specified GUID.
    Such protocols can be used as event signaling mechanism.

               
    @param ProtocolGuid Pointer to the protocol GUID
    @param Interface Pointer to the interface that allows to pass data
        pointer to the event consumer.

               
    @retval VOID

**/

VOID SignalProtocolEvent(IN EFI_GUID *ProtocolGuid, IN VOID *Interface)
{
    EFI_HANDLE  Handle = NULL;
    pBS->InstallProtocolInterface (
        &Handle, ProtocolGuid, EFI_NATIVE_INTERFACE, Interface
    );
    pBS->UninstallProtocolInterface (
        Handle, ProtocolGuid, Interface
    );
}

VOID SignalAmiLegacyBootEvent()
{
    DEBUG((DEBUG_INFO,"Signaling AMI legacy boot event.\n"));
	SignalProtocolEvent(&gAmiLegacyBootProtocolGuid, NULL);
}


/**
    Connect all the serial IO that was disconnected before.

**/

VOID ConnectSerialIO()
{
    UINTN       n; 

    for (n = 0; n < gSerialIoHandleCount; n++) {
        pBS->ConnectController(gSerialIoHandleBuffer[n],NULL,NULL,TRUE);
    }

    return;

}

EFI_STATUS LockConsole()
{
    EFI_STATUS Status;
    
    if (gConsoleControl == NULL)
    {
        Status = pBS->LocateProtocol(&gEfiConsoleControlProtocolGuid, NULL, (VOID**)&gConsoleControl);
        if (EFI_ERROR(Status)) return Status;
    }
    if (gConsoleLocked) return EFI_NO_RESPONSE;
    
    Status = gConsoleControl->LockStdIn(gConsoleControl, L"");
    if (!EFI_ERROR(Status))
    {
        if (gSetTxtMode == 2)
        {
            UINT32      *volatile Ivt = (UINT32*)0;
            ACCESS_PAGE0_CODE(
                gSaveIrq1 = Ivt[9];
                Ivt[9] = 0xf000eef3;
            );
        }
        gConsoleLocked = TRUE;
    }
    return Status;
}

EFI_STATUS UnlockConsole()
{
    EFI_STATUS Status;
    
    if (gConsoleControl == NULL) return EFI_NOT_FOUND;
    if (gConsoleLocked == FALSE) return EFI_NO_RESPONSE;
    Status = gConsoleControl->LockStdIn(gConsoleControl, L"");
    if (!EFI_ERROR(Status))
    {
        if (gSetTxtMode == 2 && gSaveIrq1 != 0)
        {
            UINT32      *volatile Ivt = (UINT32*)0;
            ACCESS_PAGE0_CODE(
                Ivt[9] = gSaveIrq1;
            );
        }
        gConsoleLocked = FALSE;
    }
    return Status;
}

EFI_STATUS GetConsoleMode(
    BOOLEAN *IsText
)
{
    EFI_STATUS Status;
    EFI_CONSOLE_CONTROL_SCREEN_MODE Mode;
    
    if (gConsoleControl == NULL)
    {
        Status = pBS->LocateProtocol(&gEfiConsoleControlProtocolGuid, NULL, (VOID**)&gConsoleControl);
        if (EFI_ERROR(Status)) return Status;
    }
    
    Status = gConsoleControl->GetMode(gConsoleControl, &Mode, NULL, NULL);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;

    *IsText = (Mode == EfiConsoleControlScreenText);
    return EFI_SUCCESS;
}


/**
    Disconnect the All the Serial io Handles

**/

VOID DisconnectSerialIO()
{
    UINTN       n; 
    EFI_STATUS  Status;
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath;
  
    //
    // Locate all Serial IO handles
    //
    Status = pBS->LocateHandleBuffer (
        ByProtocol,
        &gEfiSerialIoProtocolGuid,
        NULL,
        &gSerialIoHandleCount,
        &gSerialIoHandleBuffer);

    if (EFI_ERROR (Status)) {
        return ;
    }

    for (n = 0; n < gSerialIoHandleCount; n++) {
        //
        // Check if we have DevicePath Protocol installed on Controller handle.
        // If not, do not disconnect the controller because it may not be handled
        // by the Serial IO drvier. 
        // 
        Status = pBS->HandleProtocol (
                        gSerialIoHandleBuffer[n],
                        &gEfiDevicePathProtocolGuid,
                        (VOID**)&DevicePath);

        if (!EFI_ERROR (Status)) {
            pBS->DisconnectController(gSerialIoHandleBuffer[n],NULL,NULL);
        }
    }

    return;
}


/**
    This function checks whether the CSM16 image is made by AMI.

**/

BOOLEAN
IsAMICSM16(
    EFI_COMPATIBILITY16_TABLE* Csm16Header
)
{
    // Check for "AMIBIOS(" at F000:F500
    return (*(UINT32*)0xff500 == 0x42494d41 &&
            *(UINT32*)0xff504 == 0x28534f49);
}


/**
    Calculates and updates a checksum field of $EFI header.

    @param 
        Csm16Header Pointer to the EFI_COMPATIBILITY16_TABLE structure

    @retval 
        Checksum byte

    @note  
  Shadow region must be open for writing

**/

UINT8
ChecksumCSM16Header (
    EFI_COMPATIBILITY16_TABLE   *Csm16Header
)
{
    UINT8   i;
    UINT8   chksum;
    UINT8   *p;

    Csm16Header->TableChecksum = 0;
    p = (UINT8*)gCoreBiosInfo->Csm16Header;
    for (chksum = 0, i = 0; i < Csm16Header->TableLength; i++) {
        chksum += *p++;
    }
    Csm16Header->TableChecksum = ~(--chksum);

    return Csm16Header->TableChecksum;
}

typedef struct {
    UINT32 Feature;
    UINT32 Bit;
    UINT8 InfoByteOfs;
} CSM16_FEATURE_LOOKUP;

EFI_STATUS
Csm16Config(
	IN CSM16_FEATURE_LOOKUP *Csm16Feature,
	IN UINT8 FeatureCount,
    IN CSM16_CONFIGURATION_ACTION ConfigAction,
    IN UINT32 Param,
    OUT OPTIONAL UINT32 *Data
)
{
    LEGACY16_TO_EFI_DATA_TABLE_STRUC *Csm16Data;
    UINT8 *d8;
    UINT32 Granularity;
    EFI_STATUS Status;
    BOOLEAN F000WasOpen;
    UINT32 i;

    // Find parameter in the lookup table
    DEBUG((DEBUG_ERROR, "Csm16Config: count %d\n", FeatureCount));
    for (i = 0; i < FeatureCount; i++)
    {
        if ((UINT32)Csm16Feature[i].Feature == Param) break;
    }
    
    if (i == FeatureCount)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    DEBUG((DEBUG_ERROR, "Csm16Config: index %d\n", i));    
    
    Csm16Data = (LEGACY16_TO_EFI_DATA_TABLE_STRUC*)(UINTN)(0xf0000 + *(UINT16*)0xfff4c);
    
    switch (Csm16Feature[i].InfoByteOfs) {
    	case 0: d8 = (UINT8*)(UINTN)Csm16Data->MiscInfoAddress; break;
    	case 1: d8 = (UINT8*)(UINTN)Csm16Data->RtCmosByteAddress; break;
    	case 2: d8 = (UINT8*)(UINTN)0xff674; break;	// csm_misc_info
    	default:
			DEBUG((DEBUG_ERROR, "Csm16Config ERROR: invalid parameter\n"));
			ASSERT(FALSE);
			return EFI_INVALID_PARAMETER;
    }
    
    // see if F000 needs to be made writable
    F000WasOpen = MemoryIsWritable (0xf0000);

    if (F000WasOpen == FALSE)
    {
        Status = gCoreBiosInfo->iRegion->UnLock (gCoreBiosInfo->iRegion, 0xf0000, 0x10000, &Granularity);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return EFI_ACCESS_DENIED;
        if (MemoryIsWritable (0xf0000) == FALSE) return EFI_ACCESS_DENIED;  // PAMs could be locked permanently
    }

    Status = EFI_SUCCESS;

    switch (ConfigAction) {
        case Csm16FeatureGet:
            if (Data == NULL)
            {
                Status = EFI_INVALID_PARAMETER; break;
            }
            *Data = (*d8 & Csm16Feature[i].Bit)? 1 : 0;
            break;

        case Csm16FeatureSet:
        	*d8 |= Csm16Feature[i].Bit;
            break;

        case Csm16FeatureReset:
            *d8 &= ~Csm16Feature[i].Bit;
            break;
        default: ASSERT(FALSE); Status = EFI_INVALID_PARAMETER;
    }

    if (!F000WasOpen)
    {
        gCoreBiosInfo->iRegion->Lock (gCoreBiosInfo->iRegion, 0xf0000, 0x10000, &Granularity);
    }

    return Status;
}


/**
    This is LegacyBiosExt interface function that manages CSM16 configuration
    settings.

**/

EFI_STATUS
Csm16ConfigurationEx(
    IN CSM16_CONFIGURATION_ACTION ConfigAction,
    IN UINT32 Param,
    OUT OPTIONAL UINT32 *Data
)
{
    static CSM16_FEATURE_LOOKUP Csm16Feature[] = {
        { Csm16HddKeepOriginalOrder, CSM_HDD_ORIGINAL_ORDER_BIT, 2 }
    };

    return Csm16Config(Csm16Feature, 1, ConfigAction, Param, Data);
}


/**
    This is LegacyBiosExt interface function that manages CSM16 configuration
    settings.

**/

EFI_STATUS
EFIAPI
Csm16Configuration(
    IN CSM16_CONFIGURATION_ACTION ConfigAction,
    IN CSM16_FEATURE Param,
    OUT OPTIONAL UINT32 *Data
)
{
	UINT8 Count;
	
    static CSM16_FEATURE_LOOKUP Csm16Feature[] = {
        { Csm16HeadlessI19Retry, CSM16_HEADLESS_INT19_RETRY_BIT, 0 },
        { Csm16I13MbrWriteProtect, CSM16_I13_HDD_MBR_WP_BIT, 0 },
        { Csm16LegacyFree, CSM16_NO_KBC_PRESENT_BIT, 0 },
        { Csm16ZipHddEmulation, CSM16_ZIP_HDD_EMUL_BIT, 0 },
        { Csm16NoUsbBoot, CSM16_NO_USB_BOOT_BIT, 0 },
        { Csm16I19Trap, CSM16_I19_TRAP_BIT, 0 },
        { Csm16Flexboot, CSM16_FLEXBOOT_ENABLE_BIT, 0 },
        { Csm16GA20, CSM16_FAST_GATE_A20_BIT, 0 },
        { Csm16SkipGA20Deactivation, CSM_RT_CMOS_SKIP_GA20_DEACTIVATION_BIT, 1 },
        { Csm16Nmi, CSM_RT_CMOS_PARITY_BIT, 1 },
        { Csm16CpuRealModeOnBoot, CSM_RT_CPU_RM_ONBOOT_BIT, 1 },
        { Csm16PS2, CSM_RT_CMOS_PS2_BIT, 1 },
        { Csm16LegacyToEfi, CSM_RT_CMOS_LTE_BIT, 1 },
        { Csm16LegacyToEfiPonr, CSM_RT_CMOS_LTE_PONR_BIT, 1 }
    };

    Count = sizeof(Csm16Feature)/sizeof(CSM16_FEATURE_LOOKUP);
    return Csm16Config(Csm16Feature, Count, ConfigAction, (UINT32)Param, Data);
}

