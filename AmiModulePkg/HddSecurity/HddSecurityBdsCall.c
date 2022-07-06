//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file HddSecurityBdsCall.c
    This function will connect the handle's of RAID Controllers and
    the IDE/AHCI Devices( the handle whose DiskIo was not opened by any one.)


**/

//---------------------------------------------------------------------------

#include <IndustryStandard/Pci22.h>
#include "Protocol/DiskIo.h"
#include <Protocol/IdeControllerInit.h>
#include <AmiDxeLib.h>
#include <Protocol/DevicePath.h>

//---------------------------------------------------------------------------

extern EFI_GUID   gAmiGlobalVariableGuid;
extern EFI_STATUS GetPciHandlesByClass(
    UINT8           Class,
    UINT8           SubClass,
    UINTN           *NumberOfHandles,
    EFI_HANDLE      **HandleBuffer
);

/** @internal
    This function will connect the handle's of RAID Controllers and the IDE/AHCI
    Devices( the handle whose DiskIo was not opened by any one.)

    @param VOID

    @return VOID

    @note  
     If Conin Devices are not Present, RAID Option ROM will not be launched 
     by CSM BlockIo as RegisterHddNotification function would get control after
     connectEverything() is called from BDS. Rather it would be launched by
     ShadowAllLegacyOproms() and UEFI Boot option for RAID will not be formed as
     blockIo will not be installed by CSM BlockIo driver. So Connect the RAID handle
     so that CSM BlockIo starts. And also if Password is installed the above said case
     will happen when no conin device is present.
    
     Check for any device in IDE/AHCI mode not unlocked when no ConIn Device is not present.
     If found Connecting that Device using ConnectController() after the Device is unlocked.
     as read would fail and FileSystem would not be formedOpen gEfiIdeControllerInitProtocolGuid
     which will be installed on each controller. This will be opened as
     EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER by each device handle.Connect that Handle where
     DiskIo will be installed and not opened by another driver.

    @endinternal
**/

VOID
IdeConnectControllerAfterConnectEverthing (
) 
{

    UINT8                                   Index;
    UINT8                                   Index1;
    UINTN                                   Count;
    EFI_HANDLE                              *HandleBuffer = NULL;
    UINTN                                   NumHandles;
    EFI_STATUS                              Status;
    EFI_OPEN_PROTOCOL_INFORMATION_ENTRY     *Entries = NULL;
    BOOLEAN                                 RaidDriverBlocked=FALSE;
    UINTN                                   VarSize=sizeof(RaidDriverBlocked);

    

    // If Conin Devices are not Present, RAID Option ROM will not be launched 
    // by CSM BlockIo as RegisterHddNotification function would get control after
    // connectEverything() is called from BDS. Rather it would be launched by
    // ShadowAllLegacyOproms() and UEFI Boot option for RAID will not be formed as
    // blockIo will not be installed by CSM BlockIo driver. So Connect the RAID handle
    // so that CSM BlockIo starts. And also if Password is installed the above said case
    // will happen when no conin device is present.

    Status = GetPciHandlesByClass(PCI_CLASS_MASS_STORAGE,
                                  PCI_CLASS_MASS_STORAGE_RAID,
                                  &NumHandles,
                                  &HandleBuffer);

    // Connect the handle so that CSM BlockIo Launches RAID Option Rom.
    if(!EFI_ERROR(Status)){

        Status = pRS->GetVariable(L"RaidDriverBlockingStatus",
                                  &gAmiGlobalVariableGuid,
                                  NULL,
                                  &VarSize,
                                  &RaidDriverBlocked );


        if((EFI_ERROR(Status)) || (RaidDriverBlocked == FALSE)) {
            return;
        }
        
        for(Index=0; Index<NumHandles; Index++){

            // We have blocked the Raid driver until password verification is done. 
            // Now we need to un block and connect the Raid driver after HddPassword verification is done 
            Status = pBS->OpenProtocolInformation(HandleBuffer[Index], 
                                                  &gEfiDevicePathProtocolGuid, 
                                                  &Entries, 
                                                  &Count);

            if(!EFI_ERROR(Status)) {
                for(Index1=0; Index1 < Count; Index1++) { 
                    if (Entries[Index1].Attributes!=EFI_OPEN_PROTOCOL_BY_DRIVER) {
                        continue;
                    }

                    pBS->CloseProtocol (HandleBuffer[Index],
                                        &gEfiDevicePathProtocolGuid,
                                        Entries[Index1].AgentHandle,
                                        Entries[Index1].ControllerHandle);
                }
            }

            pBS->ConnectController(HandleBuffer[Index],NULL,NULL,TRUE);
        }

    }

    if(HandleBuffer) {
        pBS->FreePool(HandleBuffer);
    }

    return;
}

