//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file 
 SecureFlashDxe.c
**/

#include <Token.h>
#include <AmiDxeLib.h>
#include <AmiHobs.h>
#include <Flash.h>
#include <FlashUpd.h>
#include <../SecureFlash.h>
#include <Library/PciLib.h>
#include <Guid/CapsuleVendor.h>
#include <Library/DebugLib.h>
#include <Protocol/DxeSmmReadyToLock.h>
//
// Global variables
//
extern EFI_BOOT_SERVICES    *pBS;

//----------------------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------------------
/**
    Callback on End Of Dxe
    Action - Signal Ready to Lock the Flash event. and clear any pending Flash Update and related Capsule variables

    @param[in]   Event The event that triggered this notification function  
    @param[in]   ParentImageHandle Pointer to the notification functions context

    @retval      none
**/
VOID EFIAPI
DxeSmmReadyToLockCallback (
    IN EFI_EVENT    Event,
    IN VOID         *Context
  )
{
    EFI_STATUS              Status;
    EFI_HANDLE              Handle = NULL;
    AMI_FLASH_UPDATE_BLOCK  FlashUpdDesc;
    UINT32                  Attributes;
    UINTN                   Size;
    CHAR16                  CapsuleNameBuffer[30];
    EFI_PHYSICAL_ADDRESS    IoData;
    UINTN                   Index;
    static EFI_GUID         gFlashUpdGuid = FLASH_UPDATE_GUID;
    static EFI_GUID         gBiosReadyToLockEventGuid = AMI_EVENT_FLASH_WRITE_LOCK;
    
    DEBUG ((SecureMod_DEBUG_LEVEL,"\nSecure Fl Upd:\nFlash_Ready_To_Lock callback\n"));
// Prep the FlashOp variable
    Size = sizeof(AMI_FLASH_UPDATE_BLOCK);
    if(!EFI_ERROR(pRS->GetVariable( FLASH_UPDATE_VAR, &gFlashUpdGuid, &Attributes, &Size, &FlashUpdDesc)))
    {
       // Erase NV Flash Var
       pRS->SetVariable (FLASH_UPDATE_VAR, &gFlashUpdGuid,Attributes,0,NULL);
        
       // Clear pending Capsule Update Var only if FlashOp is pending. We don't want to interfere with other types of Capsule Upd
       // Erase only the Capsule var with a mailbox address matching the one from FlashUpd variable
       Swprintf_s(CapsuleNameBuffer, sizeof(CapsuleNameBuffer)/sizeof(CHAR16), L"%s", EFI_CAPSULE_VARIABLE_NAME);
       Index  = 0;
       IoData = 0;
       Status = EFI_SUCCESS;
       // Clear an NVRAM variable of the format L"CapsuleUpdateData" and L"CapsuleUpdateDataN" where N is an integer
       while(!EFI_ERROR(Status)) {
           if(Index > 0 )
               Swprintf_s(CapsuleNameBuffer, sizeof(CapsuleNameBuffer)/sizeof(CHAR16), L"%s%d", EFI_CAPSULE_VARIABLE_NAME, Index);
           Size = sizeof(EFI_PHYSICAL_ADDRESS);
           Status = pRS->GetVariable(  CapsuleNameBuffer, &gEfiCapsuleVendorGuid, &Attributes, &Size, &IoData);
           DEBUG ((SecureMod_DEBUG_LEVEL,"Find '%s' %r, %lX[=%lx]\n", CapsuleNameBuffer, Status, IoData, FlashUpdDesc.FwImage.CapsuleMailboxPtr[0]));
           if(!EFI_ERROR(Status) && (IoData == FlashUpdDesc.FwImage.CapsuleMailboxPtr[0] )
           ) {
               pRS->SetVariable(CapsuleNameBuffer, &gEfiCapsuleVendorGuid, Attributes,0,NULL);
               break;
           }
           Index++;
       }
    }
#if defined(FLASH_LOCK_EVENT_NOTIFY) && FLASH_LOCK_EVENT_NOTIFY == 1
    // Signal Event.....
    pBS->InstallProtocolInterface (
        &Handle, &gBiosReadyToLockEventGuid, EFI_NATIVE_INTERFACE,NULL
    );
    pBS->UninstallProtocolInterface (
        Handle,&gBiosReadyToLockEventGuid, NULL
    );
#endif
    //
    //Kill the Event
    //
    pBS->CloseEvent(Event);
}

#if defined(ENABLE_SECURE_FLASH_INFO_PAGE) && ENABLE_SECURE_FLASH_INFO_PAGE == 1
#ifndef ICH_REG_LPC_BIOS_CNTL
// LPC Bridge Bus Device Function Definitions
#define ICH_REG_LPC_VID 0
#define ICH_REG_LPC_BIOS_CNTL 0xDC
#define LPC_BUS     0
#define LPC_DEVICE  31
#define LPC_FUNC    0    
#endif    
#define PCI_LIB_ADDRESS(Bus,Device,Function,Register)   \
  (((Register) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

/**
    Check for a BIOS flash region Lock

    @param[in]   Event The event that triggered this notification function  
    @param[in]   ParentImageHandle Pointer to the notification functions context

    @retval      UINT8 - BIOS Flash lock status
                 0 - Flash Lock information N/A
                 1 - Flash Lock Disabled
                 2 - Flash Lock Enabled
**/
UINT8 CheckBiosLockStatus()
{
    UINT8  Byte;
    UINT16 Word;
// Sample implementation works only for Intel PCH's with SPI controller programming of the BIOS flash region lock
    Word = PciRead16(PCI_LIB_ADDRESS(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_VID));
    Byte = PciRead8 (PCI_LIB_ADDRESS(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_BIOS_CNTL));
    DEBUG ((SecureMod_DEBUG_LEVEL,"Vendor %x, reg 0xDC[0,1,5]  = 0x%X\n", Word, Byte));
    if(Word != 0x8086 || Byte == 0xFF)
        return 0; // lock info not available
    else
        if((Byte & (BIT00 | BIT01 | BIT05)) ==(BIT01 | BIT05))
            return 2; // enabled
        else
            return 1; // disabled
}
/**
    Callback on Flash Lock Complete event.
    Update SecureFlash setup variable with state of BIOS region Lock

    @param[in]   Event The event that triggered this notification function  
    @param[in]   ParentImageHandle Pointer to the notification functions context

    @retval      none
**/
VOID EFIAPI
AfterLockCallback (
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
  )
{
    EFI_STATUS Status;
    UINTN Size;

    SECURE_FLASH_SETUP_VAR SecureFlashSetupVar = {0,0,0,0};
    static EFI_GUID gSecureFlashSetupVarGuid = AMI_SECURE_FLASH_SETUP_VAR_GUID;

    Size = sizeof(SECURE_FLASH_SETUP_VAR);
    Status = pRS->GetVariable (AMI_SECURE_FLASH_SETUP_VAR,
            &gSecureFlashSetupVarGuid,  
            NULL,
            &Size,
            &SecureFlashSetupVar);
    DEBUG ((SecureMod_DEBUG_LEVEL,"\nSecure Fl Upd:\nAfter_Flash_Lock callback\nGet %s: %r (%s.Lock=%d)\n", AMI_SECURE_FLASH_SETUP_VAR, Status, AMI_SECURE_FLASH_SETUP_VAR, SecureFlashSetupVar.Lock));
    if(SecureFlashSetupVar.Lock == 0) {
//    
// test only. will remove the call when actual BiosLockSet event is supported
//
        SecureFlashSetupVar.Lock = CheckBiosLockStatus();
// end
        if(SecureFlashSetupVar.Lock != 0) {
            Size = sizeof(SECURE_FLASH_SETUP_VAR);
            Status = pRS->SetVariable (AMI_SECURE_FLASH_SETUP_VAR,
                    &gSecureFlashSetupVarGuid,                      EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    Size,
                    &SecureFlashSetupVar);
        }
        DEBUG ((SecureMod_DEBUG_LEVEL,"Set %s: %r (%s.Lock %a)\n\n", AMI_SECURE_FLASH_SETUP_VAR, Status, AMI_SECURE_FLASH_SETUP_VAR, SecureFlashSetupVar.Lock==2?"Enabled":"Disabled"));
    }
    //
    //Kill the Event
    //
    pBS->CloseEvent(Event);
}
#endif // #if defined(ENABLE_SECURE_FLASH_INFO_PAGE) && ENABLE_SECURE_FLASH_INFO_PAGE == 1

/**
    Entry point of Flash Update Policy driver

    @param[in]   ImageHandle
    @param[in]   SystemTable

    @retval      Status
**/
EFI_STATUS EFIAPI
SecFlashUpdDxe_Init (
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE   *SystemTable
)
{
    EFI_STATUS              Status;
    EFI_EVENT               Event;
    VOID                   *Registration;

    InitAmiLib(ImageHandle, SystemTable);

    //
    // Installing a callback on Dxe Smmm ready to lock
    //
    Status = RegisterProtocolCallback( &gEfiDxeSmmReadyToLockProtocolGuid,
                                        DxeSmmReadyToLockCallback,
                                        NULL, &Event, &Registration);
    ASSERT (Event != NULL);
    if(EFI_ERROR(Status)) {
        return Status;
    }

#if defined(ENABLE_SECURE_FLASH_INFO_PAGE) && ENABLE_SECURE_FLASH_INFO_PAGE == 1
    // Installing a callback on BIOS Lock set event
    Status = RegisterProtocolCallback ( &gBdsAllDriversConnectedProtocolGuid,
                                        AfterLockCallback,
                                        NULL, &Event, &Registration);
    ASSERT (Event != NULL);
#endif

    return Status;
}
