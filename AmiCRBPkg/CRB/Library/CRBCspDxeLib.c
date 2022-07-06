//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//*************************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//**********************************************************************

//<AMI_FHDR_START>
//---------------------------------------------------------------------------
//
// Name:        CRBCspDxeLib.c
//
// Description:
//  This file contains code for CRB platform
//   initialization in the Library stage
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>

#include <Token.h>
#include <AmiDxeLib.h>
#include <Protocol/ExitPmAuth.h>
#include <Protocol\LegacyBiosPlatform.h>
#include <PCI.h>
#include <Protocol\PciIo.h>
#include <Protocol\IioUds.h>
#include <Guid\SetupVariable.h>
#include <Include\PchRegs\PchRegsEva.h>
#include <Include\PchRegs\PchRegsSata.h>

#include "BootOptions.h"
#include "BootOptioneLinks.h"
#include "Protocol\IdeControllerInit.h"

#include "Protocol\PciIo.h"

//#include "Protocol\PIDEController.h"
//#include "Protocol/PDiskInfo.h"
#include "Protocol\DiskInfo.h"

//#include "Protocol\PIDEBus.h"
//#include "Protocol/PAhciBus.h"

#include "Protocol\LegacyRegion2.h"
//AptioV server overrride start: Dynamic mmcfg base address change
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include "PchAccess.h"
#endif
//AptioV server overrride end: Dynamic mmcfg base address change
#include "Protocol/PchPlatformPolicy.h"
#include <Library/S3BootScriptLib.h>

EFI_GUID  EfiLegacyRegion2ProtocolGuid = EFI_LEGACY_REGION2_PROTOCOL_GUID;

// Controller info Structure
typedef struct _CONTROLLER_INFO_STRUC {
    UINT16      BusDevFun;      // Bus, Device and Function number for a controller
    UINTN       ControllerNo;   // Controler number for a controller
} CONTROLLER_INFO_STRUC;

CONTROLLER_INFO_STRUC ControllerInfo[30];  // need to use macro
UINTN           MaxControllerFound;

#define SATACONTROLLER0 0
#define SATACONTROLLER1 1
#define SSATACONTROLLER 2

#pragma pack(1)
typedef struct {
  UINT16  SubSystemVendorId;      ///< Default Subsystem Vendor ID of the PCH devices.
  UINT16  SubSystemId;            ///< Default Subsystem ID of the PCH devices.
} IIO_DEFAULT_SVID_SID;

typedef struct {
  UINT8 DeviceNumber;
  UINT8 FunctionNumber;
  UINT8 SvidRegOffset;
  IIO_DEFAULT_SVID_SID DefaultSvidSid;
} IIO_SVID_SID_INIT_ENTRY;
#pragma pack()

//
// GUID Definition(s)
//

//<AMI_PHDR_START>
//-------------------------------------------------------------------------
//
// Procedure:   SignalExitPmAuthProtocolEvent
//
// Description: The elink will signal gExitPmAuthProtocolGuid Event.
//
// Parameters:  VOID
//
//
// Returns:     None
//
//-------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID SignalExitPmAuthProtocolEvent (
  VOID
)
{
  EFI_HANDLE  Handle = NULL;

  //
  // Signaling gExitPmAuthProtocolGuid Event
	//
  pBS->InstallProtocolInterface (
        &Handle,
        &gExitPmAuthProtocolGuid,
        EFI_NATIVE_INTERFACE,
        NULL
        );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Name:    OEMProgramIioSvidSid
//
// Description: Program OEM Sub system ID and Sub system Vendor ID for 
//				IIO devices.
//
// Input:
//  VOID
//
// Output:
//  VOID
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
OEMProgramIioSvidSid (
  IN UINT8  IioNumber,
  IN UINT8  CoreBusNumber )
{
    UINTN                       Index;
    UINTN                       PciEAddressBase;
    UINTN                       SizeofTable;

    IIO_SVID_SID_INIT_ENTRY     OemIioSvidSidInitList[] = {
            //
            // Add Entries here.
            //
            { 0xFF, 0xFF, 0xFF, {0xFFFF, 0xFFFF} }
    };

    //
    // Calculate the Size of Table
    //
    SizeofTable = ( sizeof(OemIioSvidSidInitList)/sizeof(IIO_SVID_SID_INIT_ENTRY) );

    for ( Index = 0; Index<(SizeofTable-1); Index++ ) {

        //
        // Read PCIE Base Address
        //
        PciEAddressBase = MmPciAddress (
                            0,
                            CoreBusNumber,
                            OemIioSvidSidInitList[Index].DeviceNumber,
                            OemIioSvidSidInitList[Index].FunctionNumber,
                            0 );

        //
        // Check if the Device and Function is available by reading VID and DID.
        //
        if ( 0xFFFFFFFF == MmioRead32 (PciEAddressBase) ) {
            continue;
        }

        //
        // Program device Subsystem Vendor Identifier (SVID)
        //
        MmioWrite16 (
          (UINTN) (PciEAddressBase + OemIioSvidSidInitList[Index].SvidRegOffset),
          OemIioSvidSidInitList[Index].DefaultSvidSid.SubSystemVendorId
          );
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + OemIioSvidSidInitList[Index].SvidRegOffset),
          1,
          (VOID *) (UINTN) (PciEAddressBase + OemIioSvidSidInitList[Index].SvidRegOffset)
          );

        //
        // Program device Subsystem Identifier (SID)
        //
        MmioWrite16 (
          (UINTN) (PciEAddressBase + OemIioSvidSidInitList[Index].SvidRegOffset + 2),
          OemIioSvidSidInitList[Index].DefaultSvidSid.SubSystemId
          );  
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + OemIioSvidSidInitList[Index].SvidRegOffset + 2),
          1,
          (VOID *) (PciEAddressBase + OemIioSvidSidInitList[Index].SvidRegOffset + 2)
          );
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Name:    OEMProgramPchSvidSid
//
// Description: Program OEM Sub system ID and Sub system Vendor ID for 
//				PCH devices.
//
// Input:
//  VOID
//
// Output:
//  VOID
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
OEMProgramPchSvidSid (
  VOID )
{
    UINTN                       Index;
    UINTN                       PciEAddressBase;
    UINTN                       SizeofTable;

    PCH_SVID_SID_INIT_ENTRY     OemPchSvidSidInitList[] = {
            //
            // Add Entries here.
            //
            { 0xFF, 0xFF, 0xFF, {0xFFFF, 0xFFFF} }
    };

    //
    // Calculate the Size of Table
    //
    SizeofTable = ( sizeof(OemPchSvidSidInitList)/sizeof(PCH_SVID_SID_INIT_ENTRY) );

    for ( Index = 0; Index<(SizeofTable-1); Index++ ) {

        //
        // Read PCIE Base Address
        //
        PciEAddressBase = MmPciAddress (
                            0,
                            0,
                            OemPchSvidSidInitList[Index].DeviceNumber,
                            OemPchSvidSidInitList[Index].FunctionNumber,
                            0
                            );

        //
        // Check if the Device and Function is available by reading VID and DID.
        //
        if ( 0xFFFFFFFF == MmioRead32 ( PciEAddressBase ) ) {
            continue;
        }

        //
        // Program device Subsystem Vendor Identifier (SVID)
        //
        MmioWrite16 (
          (UINTN) (PciEAddressBase + OemPchSvidSidInitList[Index].SvidRegOffset),
          OemPchSvidSidInitList[Index].DefaultSvidSid.SubSystemVendorId
          );
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + OemPchSvidSidInitList[Index].SvidRegOffset),
          1,
          (VOID *) (UINTN) (PciEAddressBase + OemPchSvidSidInitList[Index].SvidRegOffset)
          );

        //
        // Program device Subsystem Identifier (SID)
        //
        MmioWrite16 (
          (UINTN) (PciEAddressBase + OemPchSvidSidInitList[Index].SvidRegOffset + 2),
          OemPchSvidSidInitList[Index].DefaultSvidSid.SubSystemId
          );  
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + OemPchSvidSidInitList[Index].SvidRegOffset + 2),
          1,
          (VOID *) (PciEAddressBase + OemPchSvidSidInitList[Index].SvidRegOffset + 2)
          );
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Name:    CRBGetPlatformVgaHandle
//
// Description: Returns Vga handle for the Grantley CRB. Onboard/Off-board Vga
//              is decided based upon user setting for Vga priority.
// Input:
//  This            Indicates the EFI_LEGACY_BIOS_PLATFORM_PROTOCOL instance.
//  Mode            Specifies what handle to return.
//                      GetVgaHandle
//  Type            Handle Modifier - Mode specific
//  HandleBuffer    Pointer to buffer containing all Handles matching the
//                  specified criteria. Handles are sorted in priority order.
//                  Type EFI_HANDLE is defined in InstallProtocolInterface()
//                  in the EFI 1.10 Specification.
//  HandleCount     Number of handles in HandleBuffer.
//  AdditionalData  Pointer to additional data returned - mode specific..
//
// Output:
//  EFI_SUCCESS The handle is valid.
//  EFI_UNSUPPORTED Mode is not supported on this platform.
//  EFI_NOT_FOUND   The handle is not known.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
CRBGetPlatformVgaHandle (
  IN  EFI_LEGACY_BIOS_PLATFORM_PROTOCOL     *This,
  IN  EFI_GET_PLATFORM_HANDLE_MODE          Mode,
  IN  UINT16                                Type,
  OUT EFI_HANDLE                            **HandleBuffer,
  OUT UINTN                                 *HandleCount,
  OUT VOID                                  OPTIONAL **AdditionalData
  )
{
    EFI_HANDLE              *hBuffer = NULL;
    EFI_HANDLE              hVga = NULL;
    EFI_HANDLE              hOffboardVideo = NULL;
    EFI_STATUS              Status;
    UINT8                   dData[4];
    EFI_PCI_IO_PROTOCOL     *PciIo;
    EFI_STATUS              VgaStatus = EFI_NOT_FOUND;
    UINTN                   VariableSize = sizeof(SYSTEM_CONFIGURATION);
    UINTN                   n, Count, Seg, Bus, Dev, Fun, SelectedVgaBus;
    UINT8                   SocketNum;
    UINT64                  PciAttributes;
    EFI_IIO_UDS_PROTOCOL    *mIohUds;
    SYSTEM_CONFIGURATION    SystemConfiguration;
    UINT32                  VariableAttr; //AptioV Server Override: Remove NVRAM RT Attribute

    if (Mode == EfiGetPlatformVgaHandle) {
        Status = pRS->GetVariable(  L"IntelSetup",
                        &gEfiSetupVariableGuid,
                        &VariableAttr, //AptioV Server Override: Remove NVRAM RT Attribute
                        &VariableSize,
                        &SystemConfiguration
                        );
        if (EFI_ERROR (Status)) {
            return Status;
        }
    
        //
        // Locate all PciIo handles
        //
        Status = pBS->LocateHandleBuffer (
                        ByProtocol,
                        &gEfiPciIoProtocolGuid,
                        NULL,
                        &Count,
                        &hBuffer);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR (Status)) {
            return Status;
        }
        
        for (n = 0; n < Count; n++) {
            Status = pBS->HandleProtocol (
                            hBuffer[n],
                            &gEfiPciIoProtocolGuid,
                            &PciIo);           // Get PciIo protocol
            ASSERT_EFI_ERROR (Status);
            if (EFI_ERROR(Status)) break;
            Status = PciIo->Pci.Read(
                                PciIo,
                                EfiPciIoWidthUint32,
                                8,  // offset
                                1,  // width
                                dData);
            
            ASSERT_EFI_ERROR(Status);
            if (EFI_ERROR(Status)) break;
            //
            // Check for VGA-compatible Display controller
            // Base Class: 03h (Display Controller), Sub-Class: 00h (VGA-compatible controller)
            //
            if (dData[3] == PCI_CL_DISPLAY && dData[2] == PCI_CL_DISPLAY_SCL_VGA) {
                Status = PciIo->GetLocation(PciIo, &Seg, &Bus, &Dev, &Fun);
                Status = PciIo->Attributes(PciIo, EfiPciIoAttributeOperationGet, 0, &PciAttributes);
                VgaStatus = EFI_SUCCESS;
                if (EFI_ERROR(Status))
                    return Status;

                SelectedVgaBus = Bus; 
                
                //
                // Check for on-board device(EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE is set).
                // If no onboard video is installed, and offboard video detected, then offboard VGA handle is returned.
                //
                if ((PciAttributes & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE)) {
                    hVga = hBuffer[n];               	
                    if (SystemConfiguration.VideoSelect == 1) {// If user priority set to Onboard video, break the loop
                        break;
                    }
                }
                //
                // Handle only the 1st Offboard Video device identified.
                // If no offboard video is installed, then onboard VGA handle is returned when present.
                //
                if(hOffboardVideo == NULL && !(PciAttributes & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE)) { //Offboard Video
                    hOffboardVideo = hBuffer[n];
                    if(SystemConfiguration.VideoSelect == 2) {
                        hVga = hOffboardVideo;
                        break; // If user priority set to Offboard Video, break the loop
                    }
                }
                
            } // if PCI_CL_DISPLAY
        } // end of for loop
        
        pBS->FreePool(hBuffer);
        
        if(VgaStatus != EFI_SUCCESS){
            **HandleBuffer = 0;
            *HandleCount = 0;
            return VgaStatus;
        }
        
        Bus = SelectedVgaBus; // Get bus of VGA device
        
        //
        // Check for offboard VGA handle when there is no onboard video and user sets priority to onboard video
        //
        if(hOffboardVideo != NULL && hVga == NULL) {
            hVga = hOffboardVideo;
        }
        
        //
        // Get the Cpu No. of the assigned Vga controller
        //
        Status = pBS->LocateProtocol(
                        &gEfiIioUdsProtocolGuid,
                        NULL,
                        &mIohUds
                        );
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status))
            return Status;
        
        for (SocketNum = 0; SocketNum < mIohUds->IioUdsPtr->SystemStatus.numCpus; SocketNum++) {
            if(Bus >= mIohUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].BusBase && Bus <= mIohUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].BusLimit) {
                break;
            }
        }
        
        if(SocketNum == mIohUds->IioUdsPtr->SystemStatus.numCpus) {
            return EFI_NOT_FOUND;
        }
        
        **HandleBuffer = hVga;
        *HandleCount = 1;        
        
        //
        // Issue Soft Reset if the Cpu No. for Vga changes from previous boot
        //
        if(SystemConfiguration.LegacyVgaSoc != SocketNum) {
            VariableSize = sizeof(SYSTEM_CONFIGURATION);
            SystemConfiguration.LegacyVgaSoc = SocketNum;
            Status = pRS->SetVariable (
                            L"IntelSetup",
                            &gEfiSetupVariableGuid,
                            VariableAttr, //AptioV Server Override: Remove NVRAM RT Attribute
                            VariableSize,
                            &SystemConfiguration
                            );
            ASSERT_EFI_ERROR(Status);
            TRACE((TRACE_ALWAYS, "Issue Soft Reset for change in VGA range target \n"));
            pRS->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
        }

        return VgaStatus;
   }
    else {
        return EFI_UNSUPPORTED;
    }
	
}

// Fucntion to get Controller Number
#ifdef CSM_SUPPORT
/*
VOID 
GetControllerNumber (
    IN OUT UINTN                *gControllerNoAddr,
    IN UINT16                   wBusDevFunc
)
{
    UINT8       i;
    UINT32      Addr16=0;
    UINT16     Func;

    Func = wBusDevFunc & 0x07;

    for(i=0 ; i<MaxControllerFound ; i++) {
        if(ControllerInfo[i].BusDevFun == wBusDevFunc) {

            // Updatng the respective controller number.
            *gControllerNoAddr = ControllerInfo[i].ControllerNo;
            return;
        }
    }

    //
    // A new controller is found so BusDevFunc and ControllerNo is added
    // into the array of structure.
    //
    ControllerInfo[MaxControllerFound].BusDevFun    = wBusDevFunc;
    switch( Func ) {
        case PCI_FUNCTION_NUMBER_PCH_SATA:
            ControllerInfo[MaxControllerFound].ControllerNo = SATACONTROLLER0;
            break;
        case PCI_FUNCTION_NUMBER_PCH_SATA2:
            ControllerInfo[MaxControllerFound].ControllerNo = SATACONTROLLER1;
            break;
        case PCI_FUNCTION_NUMBER_EVA_SSATA:
            ControllerInfo[MaxControllerFound].ControllerNo = SSATACONTROLLER;
            break;
    }
    *gControllerNoAddr= ControllerInfo[MaxControllerFound].ControllerNo;
    MaxControllerFound++;
    return;
}

//  Function to Create AHCI drive Boot Option
EFI_STATUS
CreateBootOptionNameForAhciDrive ( 
    UINT8  ControllerNo,
    UINT8  PortNumber,
    UINT8  *SerialNumber,
    UINT8  *DriveString
)
{
    EFI_STATUS Status;
    UINT8   s[32] = {0};
    UINT8   TempStr[12] = "xSATAx Px: ";   //"[Controller] P[PortNo]: "
    UINTN   Index;
    EFI_LEGACY_REGION2_PROTOCOL *LegacyRegion2Protocol;
    UINT32  Granularity;
    
    TempStr[5] = ControllerNo + 0x30;
    TempStr[8] = PortNumber + 0x30;
    TempStr[0] = 0x20;

    if( ControllerNo == SSATACONTROLLER ) {
        TempStr[0] = 's';
        TempStr[5] = 0x20;
        pBS->CopyMem(s,TempStr,11);
        Index = 11;
    } else {
        pBS->CopyMem(s,&TempStr[1],10);
        Index = 10;
    }
    pBS->CopyMem(&s[Index],SerialNumber,20);

    Status = pBS->LocateProtocol(&EfiLegacyRegion2ProtocolGuid,NULL,&LegacyRegion2Protocol);
          
    if(EFI_ERROR(Status)) {
        return Status;
    }
          
    LegacyRegion2Protocol->UnLock(LegacyRegion2Protocol,(UINT32)DriveString,32,&Granularity);

    pBS->CopyMem(DriveString,s,32);

    LegacyRegion2Protocol->Lock(LegacyRegion2Protocol,(UINT32)DriveString,32,&Granularity);

   return EFI_SUCCESS;
    
}

// Create String for Ide Drive
EFI_STATUS
CreateBootOptionNameForIdeDrive (
    UINT8  ControllerNo,
    UINT16 DriveType,
    UINT8  Channel,
    UINT8  Device,
    UINT8  *SerialNumber,
    UINT8  *DriveString
)
{
    EFI_STATUS Status;
    UINT8   s[32] = {0};
    UINT8   TempStr[12] = "xPATAx XX: "; // "[ControllerNo] [DeviceType][Channel]: "
    EFI_LEGACY_REGION2_PROTOCOL *LegacyRegion2Protocol;
    UINT32  Granularity;
    UINT8    Index;

    TempStr[5] = ControllerNo + 0x30;

    if( (DriveType != 0xFFFF) &&  (DriveType & 6) ) {
        TempStr[1] = 'S';
        if ( ControllerNo == SSATACONTROLLER ) {
            TempStr[0] = 's';
            TempStr[5] = 0x20;
        }
    } else {
        TempStr[0] = 0x20;
    }
    TempStr[7]  = (Device == 0) ? 'P': 'S';
    TempStr[8] = (Channel == 0)? 'M': 'S';
    if(TempStr[0] == 's') {
         pBS->CopyMem(s,&TempStr[0],11);
         Index = 11;
     } else {
         pBS->CopyMem(s,&TempStr[1],10);
         Index = 10;
     }

    pBS->CopyMem(&s[Index],SerialNumber,20);

    Status = pBS->LocateProtocol(&EfiLegacyRegion2ProtocolGuid,NULL,&LegacyRegion2Protocol);
              
    if(EFI_ERROR(Status)) {
        return Status;
    }
              
    LegacyRegion2Protocol->UnLock(LegacyRegion2Protocol,(UINT32)DriveString,32,&Granularity);
    
    if(ControllerNo >= 0) {
       pBS->CopyMem(DriveString,s,32);
    }
    else {
        pBS->CopyMem(DriveString,&s[4],32);
    }

    LegacyRegion2Protocol->Lock(LegacyRegion2Protocol,(UINT32)DriveString,32,&Granularity);

    return Status;

}

///  Fucntion to create boot option string to all drives
EFI_STATUS CreateStringForAllDrives(BBS_TABLE *BbsTable)
{
    EFI_STATUS  Status;
    static EFI_GUID DiskInfoGuid      = EFI_DISK_INFO_PROTOCOL_GUID;
    UINT32                            Bus = BbsTable->Bus;
    UINT32                            Device = BbsTable->Device;
    UINT32                            Function = BbsTable->Function;    
    UINT8                             Class = BbsTable->Class;
    UINT8                             SubClass = BbsTable->SubClass;
    UINTN                             gControllerNo=0,i;
    VOID                              *Dev;
    EFI_HANDLE                        DeviceHandle = (EFI_HANDLE)*(UINTN*)&BbsTable->IBV1;

    EFI_DISK_INFO_PROTOCOL            *DiskInfo;
    IDE_BUS_PROTOCOL                  *IdeBusProtocol;
    UINT16                            IdentifyData[256];
    UINT8                             SerialNumber[20];
    UINT8                             data8;
    UINT32                            Size;

    // Call to get Controller Number of the Drive
    GetControllerNumber(&gControllerNo,((UINT16)Bus<<8) + ((UINT16)Device<<3) + ((UINT16)Function) );

    Status = pBS->HandleProtocol( DeviceHandle, &DiskInfoGuid, &DiskInfo );

    if (EFI_ERROR(Status)) return 0;

    Size = sizeof(IdentifyData);
    
    Status = DiskInfo->Identify ( DiskInfo, IdentifyData, &Size );

    pBS->CopyMem(SerialNumber,&IdentifyData[27],20);
      
    for (i=0; i< 20; i+=2) {
        data8 = SerialNumber[i];
        SerialNumber[i] = SerialNumber[i+1];
        SerialNumber[i+1] = data8;
    }

    // BAsed on SubClass Value calling AHCI or IDE drive Option
    if(SubClass == 6) {
        Dev = (SATA_DEVICE_INTERFACE *)((SATA_DISK_INFO*)DiskInfo)->SataDevInterface;

        CreateBootOptionNameForAhciDrive((UINT8)gControllerNo,
                                         ((SATA_DEVICE_INTERFACE *)Dev)->PortNumber,
                                         SerialNumber,
                                         (UINT8*)(((UINT32)BbsTable->DescStringSegment << 4)+((UINT32)BbsTable->DescStringOffset & 0x0FFFF)) );
    }
    else if(SubClass == 1) {
        IdeBusProtocol = (IDE_BUS_PROTOCOL *)((IDE_DISK_INFO*)DiskInfo)->IdeBusInterface;
        Dev = (IDE_DEVICE_INTERFACE*) &IdeBusProtocol->IdeDevice;

        CreateBootOptionNameForIdeDrive(
                  (UINT8)gControllerNo,
                  IdentifyData[76],
                  ((IDE_DEVICE_INTERFACE*)Dev)->Channel,  // Channel (Master or Slave)
                  ((IDE_DEVICE_INTERFACE*)Dev)->Device,  // Primary or Secondary device
                  SerialNumber,
                  (UINT8*)(((UINT32)BbsTable->DescStringSegment << 4)+((UINT32)BbsTable->DescStringOffset & 0x0FFFF)) );
    }
    return Status;
}



/// OEM hook function 
UINTN OEMConstructBootOptionNameByBbsDescription (
        BOOT_OPTION *Option, CHAR16 *Name, UINTN NameSize
)
{
    if (Option->BbsEntry == NULL) return 0;

    // Function to Change the Boot Option Name of the Drive
    CreateStringForAllDrives(Option->BbsEntry);

    return 0;
  
}
*/

#endif


//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
