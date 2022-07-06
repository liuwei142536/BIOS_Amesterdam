//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <Token.h>
#include <AmiDxeLib.h>
#include <AmiHobs.h>
#include <Protocol/DevicePath.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SmmSxDispatch.h>
#include <Protocol/SecSmiFlash.h>
#include <Protocol/AmiFlash.h>
#include <Library/AmiBufferValidationLib.h>
#include <Library/DebugLib.h>
#include <Guid/AmiGlobalVariable.h>
#include "AmiCertificate.h"
#include <AmiRomLayout.h>

#if FWCAPSULE_RECOVERY_SUPPORT == 1
#include <Capsule.h>
#include <Guid/CapsuleVendor.h>
#endif

#if INSTALL_SECURE_FLASH_SW_SMI_HNDL == 1
#include <Protocol/SmmCpu.h>
#include <Protocol/SmmSwDispatch2.h>

static EFI_SMM_CPU_PROTOCOL  *gSmmCpu;
#define RETURN(status) {return status;}
#endif
//----------------------------------------------------------------------
// Module defined global variables
//----------------------------------------------------------------------

static EFI_GUID FlashUpdGuid    = FLASH_UPDATE_GUID;

EFI_GUID gFwCapFfsGuid          = AMI_FW_CAPSULE_FFS_GUID;
EFI_GUID gFwCapSectionGuid      = AMI_FW_CAPSULE_SECTION_GUID;

static FLASH_UPD_POLICY FlUpdatePolicy = {FlashUpdatePolicy, BBUpdatePolicy};
static EFI_BOOT_MODE    BootMode;

EFI_SHA256_HASH  *gHashTbl = NULL;
UINT8             gHashDB[SHA512_DIGEST_SIZE];
CRYPT_HANDLE      gpPubKeyHndl;

AMI_DIGITAL_SIGNATURE_PROTOCOL *gAmiSig;

#if FWCAPSULE_RECOVERY_SUPPORT == 1
static EFI_PHYSICAL_ADDRESS  gpFwCapsuleMailbox     = 0;
static UINTN                 gpFwCapsuleMailboxSize = 0;
static CHAR16                gCapsuleNameBuffer[30];
#endif

// Allocate the space in OS Reserved mem for new FW image to be uploaded by a Flash tool
// Alternatively the buffer may be reserved within the SMM depend on NEW_BIOS_MEM_ALLOC Token setting
// AFU would have to execute a sequence of SW SMI calls to load new BIOS image to a reserved mem
static EFI_PHYSICAL_ADDRESS gpFwCapsuleBuffer = 0;
#if FWCAPSULE_IMAGE_SIZE < FLASH_SIZE
#error FwCapsule buffer is too small
#endif
UINTN gFwCapMaxSize = FWCAPSULE_IMAGE_SIZE;

//----------------------------------------------------------------------
// Flash Upd Protocol defines
//----------------------------------------------------------------------
AMI_FLASH_WRITE     pFlashWrite; // Original Ptr inside FlashAPI
AMI_FLASH_UPDATE    pFlashUpdate;
AMI_FLASH_ERASE     pFlashErase;

// Global variable used to store the address of the start of the BIOS region in the flash part
//#ifdef FLASH_BASE
//UINTN Bios4GBMapStart = FLASH_BASE;
//#else
//UINTN Bios4GBMapStart = (0xFFFFFFFF - FLASH_SIZE + 1);
//#endif
static ROM_AREA *OrgRomLayout = NULL;
static ROM_AREA *NewRomLayout = NULL;

//----------------------------------------------------------------------
// External function prototypes
//----------------------------------------------------------------------
EFI_STATUS FindCapHdrFFS(
    IN  VOID    *pCapsule,
    IN  UINT32   Size,
    OUT APTIO_FW_CAPSULE_HEADER  **pFwCapHdr
);
EFI_STATUS EFIAPI CapsuleValidate (
    IN OUT UINT8     **pFwCapsule,
    IN OUT VOID      **pFwCapsuleHdr
);

//---------------------------------------------------
// Local functions
//----------------------------------------------------
BOOLEAN SupportUpdateCapsuleReset (VOID);

VOID SmiSxCapsuleCallback ( IN EFI_HANDLE                    DispatchHandle,
                            IN EFI_SMM_SX_DISPATCH_CONTEXT   *DispatchContext);

BOOLEAN IsFwCapHdrValid   ( IN APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr);

//EFI_STATUS GetFwRomLayout ( IN OUT ROM_AREA **RomLayout,
//                            IN OUT UINTN     *Size);

EFI_STATUS SetSmmRomLayout( IN OUT ROM_AREA **NewRomLayout,
                            IN ROM_AREA      *RomLayout,
                            IN UINTN          RomLayoutSize);

//----------------------------------------------------------------------
// UpFront API Function definitions
//----------------------------------------------------------------------
EFI_STATUS EFIAPI LoadFwImage(
    IN OUT FUNC_BLOCK  *pFuncBlock
);
EFI_STATUS EFIAPI GetFlUpdPolicy(
    IN OUT FLASH_POLICY_INFO_BLOCK  *InfoBlock
);
EFI_STATUS EFIAPI SetFlUpdMethod(
    IN OUT FUNC_FLASH_SESSION_BLOCK  *pSessionBlock
);
EFI_STATUS EFIAPI SecureFlashWrite (
    VOID* FlashAddress, UINTN Size, VOID* DataBuffer
);
EFI_STATUS EFIAPI SecureFlashUpdate (
    VOID* FlashAddress, UINTN Size, VOID* DataBuffer
);
EFI_STATUS EFIAPI SecureFlashErase (
    VOID* FlashAddress, UINTN Size
);
//----------------------------------------------------

EFI_SEC_SMI_FLASH_PROTOCOL SecSmiFlash = {
    LoadFwImage,
    GetFlUpdPolicy,
    SetFlUpdMethod,
    CapsuleValidate,                // Validate FwCapsule's signature with BIOS version control
    0, //UINT32   *pFwCapsule;      // Local buffer for FwCapsule
    0, //ROM_AREA *RomLayout;       // Local copy of Rom Layout table 
    0, //EFI_SHA256_HASH *HashTbl;  // ptr to Fw image Hash table
    0  //UINT32   FSHandle;         // SetFlash Session handle 
};

#if FWCAPSULE_RECOVERY_SUPPORT == 1

#if CSLIB_WARM_RESET_SUPPORTED == 0
VOID SBLib_ResetSystem( IN EFI_RESET_TYPE ResetType );

VOID AsmWbinvd ();

UINT8 ReadRtcIndexedRegister(IN UINT8 Index);
VOID WriteRtcIndexedRegister(IN UINT8 Index, IN UINT8 Value);

//----------------------------------------------------------------------------
// Function Definitions

/**
    Used to read RTC register indexed by the argument

    @param[in]   Index Rtc Index of the register to read

    @retval      Byte Current value of the register
**/
UINT8 ReadRtcIndexedRegister(IN UINT8 Index){

    UINT8 Byte = IoRead8(0x70) & 0x80;   // preserve bit 7
    IoWrite8(0x70, Index | Byte);
    Byte = IoRead8(0x71);
    return Byte;
}

/**
    Used to write to RTC register indexed by the argument

    @param[in]   Index of the register to write to 
    @param[in]   Value to write to the RTC register

    @retval      none
**/

VOID WriteRtcIndexedRegister(IN UINT8 Index, IN UINT8 Value){
 
    IoWrite8(0x70,Index | (IoRead8(0x70) & 0x80));
    IoWrite8(0x71,Value);
}

/**
     This function provides a default implementation of a system reset 
     with preserved memory context
     It setups RTC for 1 second alarm and puts system into ACPI S3 State.

    @param[in]   none

    @retval      none, system will enter ACPI S3 State.
**/
VOID SBLib_ResetSystem( IN EFI_RESET_TYPE ResetType )
{
    UINT32 IoData;
    UINT8 Hour, Minute, Second;
    BOOLEAN     inBCD = TRUE;

    // Only Warm Reset type is supported
    if(ResetType != EfiResetWarm)
        return;

    //
    // Flush cache into memory before we go to sleep. It is necessary for S3 sleep
    // because the changes in gpFwCapsuleMailbox are only in cache now
    //
    AsmWbinvd ();

    // determine if RTC is in BCD mode
    if( ReadRtcIndexedRegister(0xB) & 0x4 ) // bit 2
        inBCD = FALSE;
    // wait for time update to complete before reading the values
    while( ReadRtcIndexedRegister(0xA) & 0x80 ); // while bit 7 is set the 
                                                // time update is in progress
    //read current hour, minute, second
    Hour = ReadRtcIndexedRegister(0x4);
    Minute = ReadRtcIndexedRegister(0x2);
    Second = ReadRtcIndexedRegister(0x0);

    //convert second to decimal from BCD and increment by 1
    if(inBCD)
        Second = (Second >> 4) * 10 + (Second & 0x0F);
    Second += 2;
    
    if(Second > 59){
        Second -= 60;
        if(inBCD)
            Minute = (Minute >> 4) * 10 + (Minute & 0x0F);
        Minute++;
        if(Minute > 59){
            Minute = 0;
            if(inBCD)
                Hour = (Hour >> 4) * 10 + (Hour & 0x0F);
            Hour++;
            // check 24 hour mode/12 hour mode
            if( ReadRtcIndexedRegister(0xB) & 0x2 ) {// bit 1 1=24hour else 12 hour
                if(Hour > 23)
                    Hour = 0;
            } else {
                if(Hour > 11)
                    Hour = 0;
            }

            if(inBCD)
                Hour = Hour % 10 + ( (Hour / 10) << 4 ) ;
        }
        if(inBCD)
            Minute = Minute % 10 + ( (Minute / 10) << 4 ) ;
    }

    //convert from decimal to BCD
    if(inBCD)
        Second = Second % 10 + ( (Second / 10) << 4 ) ;
    
    //set the alarm

    WriteRtcIndexedRegister(0x5, Hour);
    WriteRtcIndexedRegister(0x3, Minute);
    WriteRtcIndexedRegister(0x1, Second);
    //enable the alarm
    WriteRtcIndexedRegister(0xB, ( ReadRtcIndexedRegister(0xB) | ((UINT8)( 1 << 5 )) ));

// ========== PORTING REQUIRED ===========================================================
//  Current implementation to simulate the Warm Reboot may not be sufficient on some platforms. 
//  S3 transition may require additional Chipset/Platform coding.
//  If needed add any necessary OEM hooks to be able to put the system into S3 at the end of this handler
//========================================================================================

    //set RTC_EN bit in PM1_EN to wake up from the alarm
    IoWrite16(PM_BASE_ADDRESS + 0x02, ( IoRead16(PM_BASE_ADDRESS + 0x02) | (1 << 10) ));
    
    //Disable Sleep SMI to avoid SMI re-entrance.
    IoWrite16(PM_BASE_ADDRESS + 0x30, ( IoRead16(PM_BASE_ADDRESS + 0x30) & (~BIT4) ));
    
    //modify power management control register to reflect S3
    IoData = IoRead32(PM_BASE_ADDRESS + 0x04);
    //following code is applicable to Intel PCH only. 
    IoData &= ~(0x1C00);
    IoData |= 0x1400; //Suspend to RAM
/*
    // AMD w/a to enter S3 state
    IoData |= 0x2C00; //Suspend to RAM
    {
        UINT8 Temp8;
        IoWrite8(0xCD6, 0x004);
        Temp8 = IoRead8(0xCD7);
        Temp8 &= ~(BIT7);
        IoWrite8(0xCD6, 0x004);
        IoWrite8(0xCD7, Temp8);
        IoWrite8(0xCD6, 0x007);
        IoWrite8(0xCD7, BIT7);
    }
} 
*/
    IoWrite32(PM_BASE_ADDRESS + 0x04, IoData );

    EFI_DEADLOOP()
}
#else //CSLIB_WARM_RESET_SUPPORTED != 0
extern VOID SBLib_ResetSystem( IN EFI_RESET_TYPE ResetType );
#endif

/**
    Capsule NVRAM variables are owned by either the runtime Capsule Update service 
    or by this driver. Each service must not override previously created instances 
    of the variable and create a new one with n+1 index

    @param[in]   None

    @retval      Variable Name
**/
CHAR16* GetLastFwCapsuleVariable()
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN Index = 0;
    EFI_PHYSICAL_ADDRESS    IoData=0;
    UINTN                   Size;

    Swprintf_s(gCapsuleNameBuffer, sizeof(gCapsuleNameBuffer)/sizeof(CHAR16), L"%s", EFI_CAPSULE_VARIABLE_NAME);
    // get any NVRAM variable of the format L"CapsuleUpdateDataN" where N is an integer
    while(!EFI_ERROR(Status)) {
        if(Index > 0 )
            Swprintf_s(gCapsuleNameBuffer, sizeof(gCapsuleNameBuffer)/sizeof(CHAR16), L"%s%d", EFI_CAPSULE_VARIABLE_NAME, Index);
        Size = sizeof(EFI_PHYSICAL_ADDRESS);
        Status = pRS->GetVariable(  gCapsuleNameBuffer, &gEfiCapsuleVendorGuid,
                                    NULL,
                                    &Size, 
                                    &IoData);
        DEBUG ((SecureMod_DEBUG_LEVEL,"Get '%S' %r, %lX\n", gCapsuleNameBuffer, Status, IoData));
        if(!EFI_ERROR(Status) && 
           (IoData == gpFwCapsuleMailbox )
        ) {
            break;
        }
        Index++;
    }

    return gCapsuleNameBuffer;
}

/**
 DESCRIPTION: SMI handler to perform capsule reset (bounce from S5 to S3)
 ========== PORTING REQUIRED ===========================================================
  Current implementation to simulate the Warm Reboot may not be sufficient on some platforms. 
  S3 transition may require additional Chipset/Platform coding.
  If needed add any necessary OEM hooks to be able to put the system into S3 at the end of this handler
 ========================================================================================

    @param[in]   DispatchHandle    Handle of SMI dispatch protocol
    @param[in]   DispatchContext   Pointer to SMI dispatch context structure

    @retval      none
**/
VOID SmiS4CapsuleCallback ( IN  EFI_HANDLE                    DispatchHandle,
                            IN  EFI_SMM_SX_DISPATCH_CONTEXT   *DispatchContext
){
    DEBUG ((SecureMod_DEBUG_LEVEL,"SecSMI. S4 Trap.\n"));
    SmiSxCapsuleCallback(DispatchHandle, DispatchContext);
}

VOID SmiS5CapsuleCallback ( IN  EFI_HANDLE                    DispatchHandle,
                            IN  EFI_SMM_SX_DISPATCH_CONTEXT   *DispatchContext
){
    DEBUG ((SecureMod_DEBUG_LEVEL,"SecSMI. S5 Trap.\n"));
    SmiSxCapsuleCallback(DispatchHandle, DispatchContext);
}

VOID SmiSxCapsuleCallback ( IN  EFI_HANDLE                    DispatchHandle,
                            IN  EFI_SMM_SX_DISPATCH_CONTEXT   *DispatchContext
){
    EFI_PHYSICAL_ADDRESS    IoData;
    UINTN                   Size;
    EFI_CAPSULE_HEADER     *CapsuleHeader;
    EFI_CAPSULE_BLOCK_DESCRIPTOR *pCapsuleMailboxPtr;
    AMI_FLASH_UPDATE_BLOCK  FlUpdateBlock;

    //
    //Check if the Capsule update is supported by platform policy
    //
    if (!SupportUpdateCapsuleReset()) 
        return;

    Size = sizeof(AMI_FLASH_UPDATE_BLOCK);
    if(EFI_ERROR(pRS->GetVariable(FLASH_UPDATE_VAR, &FlashUpdGuid, NULL, &Size, &FlUpdateBlock)) ||
       FlUpdateBlock.FlashOpType != FlCapsule)
        return;

    // verify the FW capsule mailbox is in SMRAM
    Size = sizeof(EFI_PHYSICAL_ADDRESS);
    if(EFI_ERROR(pRS->GetVariable(GetLastFwCapsuleVariable(), &gEfiCapsuleVendorGuid, NULL, &Size, &IoData)))
        return;

    if(IoData != gpFwCapsuleMailbox)
        return;
    
    pCapsuleMailboxPtr = (EFI_CAPSULE_BLOCK_DESCRIPTOR*)(UINTN)IoData;
    CapsuleHeader = (EFI_CAPSULE_HEADER*)pCapsuleMailboxPtr[0].Union.DataBlock;
    //
    // Compare GUID with APTIO_FW_CAPSULE_GUID
    //
    if (guidcmp (&CapsuleHeader->CapsuleGuid, &gFWCapsuleGuid))
        return;

    DEBUG ((SecureMod_DEBUG_LEVEL,"Enter warm reset...\n"));

    SBLib_ResetSystem(EfiResetWarm);
}

/**
    Is the platform has a way to guarantee the memory integrity across a system reset.
    Return TRUE, else FALSE. 

    @param[in]   none

    @retval      BOOLEAN
**/
BOOLEAN SupportUpdateCapsuleReset (
    VOID
){
    if( (FlUpdatePolicy.FlashUpdate & FlCapsule) || 
        (FlUpdatePolicy.BBUpdate & FlCapsule))
        return TRUE;

    return FALSE;
}

/**
    This code prepares Capsule Update EFI Variable

    @param[in]   EFI_CAPSULE_HEADER **CapsuleHeaderArray - array of pointers to capsule headers passed in

    @retval      EFI_STATUS
                 EFI_SUCCESS - capsule processed successfully
                 EFI_INVALID_PARAMETER - CapsuleCount is less than 1,CapsuleGuid is not supported
                 EFI_DEVICE_ERROR - capsule processing failed
**/
static EFI_STATUS UpdateCapsule (
    IN APTIO_FW_CAPSULE_HEADER  *pFwCapsuleHdr
){
    EFI_CAPSULE_BLOCK_DESCRIPTOR *pCapsuleMailboxPtr;
    UINT32              Attributes;
    CHAR16             *EfiFwCapsuleVarName = NULL;

    //
    //Check if the platform supports update capsule across a system reset
    //
    if (!SupportUpdateCapsuleReset()) {
        return EFI_UNSUPPORTED;
    }
    //
    //Compare GUID with APTIO_FW_CAPSULE_GUID 
    //
    if (!pFwCapsuleHdr || guidcmp (&pFwCapsuleHdr->CapHdr.CapsuleGuid, &gFWCapsuleGuid))
        return EFI_DEVICE_ERROR; 

    pCapsuleMailboxPtr = (EFI_CAPSULE_BLOCK_DESCRIPTOR*)(UINTN)gpFwCapsuleMailbox;
    pCapsuleMailboxPtr[0].Length = pFwCapsuleHdr->CapHdr.HeaderSize;
    pCapsuleMailboxPtr[0].Union.DataBlock = (EFI_PHYSICAL_ADDRESS)(UINTN)pFwCapsuleHdr;

    pCapsuleMailboxPtr[1].Length = pFwCapsuleHdr->CapHdr.CapsuleImageSize-pFwCapsuleHdr->CapHdr.HeaderSize;
    pCapsuleMailboxPtr[1].Union.DataBlock = (EFI_PHYSICAL_ADDRESS)(UINTN)SecSmiFlash.pFwCapsule;

    pCapsuleMailboxPtr[2].Length = 0;
    pCapsuleMailboxPtr[2].Union.DataBlock = 0;

    EfiFwCapsuleVarName = GetLastFwCapsuleVariable();
    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    // Erase prev Var copy
    pRS->SetVariable ( EfiFwCapsuleVarName, &gEfiCapsuleVendorGuid, 0, 0, NULL);
    if(!EFI_ERROR(pRS->SetVariable ( EfiFwCapsuleVarName, &gEfiCapsuleVendorGuid, Attributes, sizeof(EFI_PHYSICAL_ADDRESS),(VOID*)&gpFwCapsuleMailbox)))
        return EFI_SUCCESS;

    return EFI_DEVICE_ERROR;
}

#endif //#if FWCAPSULE_RECOVERY_SUPPORT == 1

/**
    Set FlashUpd variable as a mailbox for PEI resume code to process Fw capsule after re-boot.

    @param[in]   FUNC_FLASH_SESSION_BLOCK    *pSessionBlock

    @retval      EFI_STATUS
**/
EFI_STATUS SetFlashUpdateVar (
    IN FUNC_FLASH_SESSION_BLOCK    *pSessionBlock
){
    UINTN               Size;
    UINT32              CounterHi;

    if(pSessionBlock->FlUpdBlock.FlashOpType == FlRecovery &&
        pSessionBlock->FlUpdBlock.FwImage.AmiRomFileName[0] == 0
    )
        return EFI_DEVICE_ERROR;

    CounterHi = 0;
    Size = sizeof(UINT32);
// MonotonicCounter is a boot time service, hence the variable may have restricted access in runtime
    if(EFI_ERROR(pRS->GetVariable(L"MonotonicCounter", &gAmiGlobalVariableGuid,
                  NULL, &Size, &CounterHi))
    )
//        return Status;//EFI_DEVICE_ERROR;
//SetMode should set FlashUpd even if no MC var detected.
        CounterHi=0xffffffff;

    pSessionBlock->FlUpdBlock.MonotonicCounter = CounterHi;
    CounterHi = (EFI_VARIABLE_NON_VOLATILE |
        EFI_VARIABLE_RUNTIME_ACCESS |
        EFI_VARIABLE_BOOTSERVICE_ACCESS);
    // Erase prev copy
    pRS->SetVariable ( FLASH_UPDATE_VAR, &FlashUpdGuid,0,0,NULL); 
    if(!EFI_ERROR(pRS->SetVariable ( FLASH_UPDATE_VAR, &FlashUpdGuid, CounterHi,
                  sizeof(AMI_FLASH_UPDATE_BLOCK), (VOID*) &pSessionBlock->FlUpdBlock )))
        return EFI_SUCCESS;

    return EFI_DEVICE_ERROR;
}

//-----------------------------------------------------------------------------------------
// Secured Flash Update API:
// GetFlUpdPolicy
// SetFlUpdMethod
// LoadFwImage
//-----------------------------------------------------------------------------------------

/**
    Return Flash Update Policy descriptor.

    @param[in]    FLASH_POLICY_INFO_BLOCK  *InfoBlock
    @param[out]   FLASH_POLICY_INFO_BLOCK  *InfoBlock
    
    @retval      EFI_STATUS
**/
EFI_STATUS EFIAPI GetFlUpdPolicy(
    IN OUT FLASH_POLICY_INFO_BLOCK  *InfoBlock
)
{
    UINTN   KeySize = DEFAULT_RSA_KEY_MODULUS_LEN;

//TRACE((TRACE_ALWAYS,"SecSMI. GetPolicy. %X_%X\n",FlUpdatePolicy.FlashUpdate, FlUpdatePolicy.BBUpdate));
//APTIOV_SERVER_OVERRIDE_START: SA50081 Security Advisory update
// InfoBlock->ErrorCode = 1; //SA50090 Security Advisory update

    if(EFI_ERROR(AmiValidateMemoryBuffer(InfoBlock, sizeof(FLASH_POLICY_INFO_BLOCK))) ||
       EFI_ERROR(AmiValidateMemoryBuffer(&(InfoBlock->FlUpdPolicy), sizeof(FLASH_UPD_POLICY))) ||
       EFI_ERROR(AmiValidateMemoryBuffer(&(InfoBlock->PKpub), KeySize))
       )
//APTIOV_SERVER_OVERRIDE_END: SA50081 Security Advisory update
        return EFI_DEVICE_ERROR;

    MemCpy(&InfoBlock->FlUpdPolicy, &FlUpdatePolicy, sizeof(FLASH_UPD_POLICY));
    MemSet(&InfoBlock->PKpub, KeySize, 0xFF);
    if(gpPubKeyHndl.BlobSize < KeySize)
        KeySize = gpPubKeyHndl.BlobSize;
    MemCpy(&InfoBlock->PKpub, gpPubKeyHndl.Blob, KeySize);

    InfoBlock->ErrorCode = 0;

    return EFI_SUCCESS;
}
/**
    Set Flash Update Method. Main Secure Flash routine. Performs FwCapsule image validation, 
    prepares the Capsule mailbox for deferred Flash update (capsule or hard reset)
    and creates the flash block Hash table for the runtime update

    @param[in]    FUNC_FLASH_SESSION_BLOCK    *pSessionBlock
    @param[out]   FUNC_FLASH_SESSION_BLOCK    *pSessionBlock
    
    @retval      EFI_STATUS
**/
EFI_STATUS EFIAPI SetFlUpdMethod(
    IN OUT FUNC_FLASH_SESSION_BLOCK    *pSessionBlock
)
{
    EFI_STATUS          Status;
    UINTN               HashBlock;
    UINTN               BlockSize;
    UINTN               BlockCount;
    UINT8              *BlockAddr;
    APTIO_FW_CAPSULE_HEADER    *pFwCapsuleHdr;

    ROM_AREA *pRomLayout;
    UINTN     RomLayoutSize;

    Status = EFI_DEVICE_ERROR;
//    pSessionBlock->ErrorCode = 1; //SA50090 Security Advisory update

    if(EFI_ERROR(AmiValidateMemoryBuffer(pSessionBlock, sizeof(FUNC_FLASH_SESSION_BLOCK))))
        return Status;

DEBUG ((SecureMod_DEBUG_LEVEL,"SecSMI. SetFlash\npSessionBlock %X\nSize     : %X\n",pSessionBlock, pSessionBlock->FlUpdBlock.ImageSize));
DEBUG_CODE (
if(pSessionBlock->FlUpdBlock.FlashOpType == FlRecovery)
DEBUG ((SecureMod_DEBUG_LEVEL,"File Name: %a\n",pSessionBlock->FlUpdBlock.FwImage.AmiRomFileName));
else
DEBUG ((SecureMod_DEBUG_LEVEL,"Image Adr: %X\n",pSessionBlock->FlUpdBlock.FwImage.CapsuleMailboxPtr[0]));
);
DEBUG ((SecureMod_DEBUG_LEVEL,"ROMmap   : %X\n",pSessionBlock->FlUpdBlock.ROMSection));
DEBUG ((SecureMod_DEBUG_LEVEL,"FlOpType : %X\n",pSessionBlock->FlUpdBlock.FlashOpType));

// Verify if chosen Flash method is compatible with FlUpd Policy
    if(((pSessionBlock->FlUpdBlock.ROMSection & (1<<BOOT_BLOCK)) && (pSessionBlock->FlUpdBlock.FlashOpType & FlUpdatePolicy.BBUpdate)) || 
      (!(pSessionBlock->FlUpdBlock.ROMSection & (1<<BOOT_BLOCK)) && (pSessionBlock->FlUpdBlock.FlashOpType & FlUpdatePolicy.FlashUpdate))
    ){

DEBUG ((SecureMod_DEBUG_LEVEL,"Buff Adr : %lX\nBuff Size: %X\n",gpFwCapsuleBuffer, gFwCapMaxSize));
        // !!! make sure Flash blocks BOOT_BLOCK, MAIN_, NV_ and EC_ are matching enum types in FlashUpd.h
        // Get Flash Update mode   
        switch(pSessionBlock->FlUpdBlock.FlashOpType)
        {
#if FWCAPSULE_RECOVERY_SUPPORT == 1
            case FlCapsule:
#endif
            case FlRuntime:
               //  common for FlRuntime or Capsule

                SecSmiFlash.pFwCapsule = (UINT32*)gpFwCapsuleBuffer;
                // AFU updates the address in CapsuleMailboxPtr if it's capable of 
                // allocating large buffer to accommodate the entire FW Capsule image
                if(pSessionBlock->FlUpdBlock.FwImage.CapsuleMailboxPtr[0] != 0 )
                {
                    if(EFI_ERROR(AmiValidateMemoryBuffer((VOID*)(pSessionBlock->FlUpdBlock.FwImage.CapsuleMailboxPtr[0]),
                                                          pSessionBlock->FlUpdBlock.ImageSize)))
                        break;

                    if(SecSmiFlash.pFwCapsule != NULL) {

                        if(pSessionBlock->FlUpdBlock.ImageSize > gFwCapMaxSize)
                           break; // suspecting buffer overrun

                        MemCpy((VOID*)SecSmiFlash.pFwCapsule, 
                               (VOID*)pSessionBlock->FlUpdBlock.FwImage.CapsuleMailboxPtr[0],
                                pSessionBlock->FlUpdBlock.ImageSize);

                    } else {
                        SecSmiFlash.pFwCapsule = (UINT32*)pSessionBlock->FlUpdBlock.FwImage.CapsuleMailboxPtr[0];
                    }
                }
                // else AFU must've uploaded the image to designated SMM space using LoadFw command
DEBUG ((SecureMod_DEBUG_LEVEL,"SecSmiFlash.pFwCapsule : %X\n",SecSmiFlash.pFwCapsule));
                if(SecSmiFlash.pFwCapsule == NULL)
                    break;
                // 
                // Verify there is a FwCapsule at gSecSmiFlash.pFwCapsule, 
                // Validate FwCapsule's signature and the compatible BIOS revision
                // update the pointers to FwCap Header and the capsule's Payload
                // 
                pFwCapsuleHdr = NULL;
                Status = CapsuleValidate((UINT8**)&(SecSmiFlash.pFwCapsule), (VOID**)&pFwCapsuleHdr);
                if(EFI_ERROR(Status)) { 
                // In case of a Capsule update, a physically present user may override a
                // roll-back protection block on the Recovery Setup page after the FW reset on Capsule update flow
                    if (Status == EFI_INCOMPATIBLE_VERSION && 
                        pSessionBlock->FlUpdBlock.FlashOpType == FlCapsule)
                        Status = EFI_SUCCESS;
                    else
                        break;
                }
                if(pFwCapsuleHdr == NULL)
                    break;
                // Capture RomLayout from new Secure Image if it's loaded in memory and validated
                //SecSmiFlash.RomLayout = (ROM_AREA *)(UINTN)((UINTN)pFwCapsuleHdr+pFwCapsuleHdr->RomLayoutOffset);
                if(pSessionBlock->FlUpdBlock.FlashOpType == FlRuntime) {
                    // Fill in gShaHashTbl Hash Table
                    BlockSize = FLASH_BLOCK_SIZE;
                    BlockAddr = (UINT8*)SecSmiFlash.pFwCapsule;
                    // BlockCount added to account for signed Small data blocks, e.g. partial BIOS blocks update
                    BlockCount = (pFwCapsuleHdr->CapHdr.CapsuleImageSize-pFwCapsuleHdr->RomImageOffset)/FLASH_BLOCK_SIZE;
//                    DEBUG ((SecureMod_DEBUG_LEVEL,"BlockCount %X <= %X\n",BlockCount, SEC_FLASH_HASH_TBL_BLOCK_COUNT));
                    for(HashBlock = 0; HashBlock < BlockCount && HashBlock < SEC_FLASH_HASH_TBL_BLOCK_COUNT; HashBlock++)
                    {
                        Status = gAmiSig->Hash(gAmiSig, &gEfiHashAlgorithmSha256Guid, 
                            1, (const UINT8**)(UINTN)&BlockAddr, (const UINTN*)&BlockSize, (UINT8*)gHashTbl[HashBlock]);
                        if (EFI_ERROR(Status)) break;
                        BlockAddr = (UINT8*)((UINTN)BlockAddr + BlockSize);
                    }
                    /*
                     * Copy new Trusted RomMap to pre-allocated SMM buffer
                     * Use it along with original map to verify flashed data against the Hash table
                    */
                    RomLayoutSize = pFwCapsuleHdr->RomImageOffset - pFwCapsuleHdr->RomLayoutOffset;
                    pRomLayout = (ROM_AREA *)((UINTN)pFwCapsuleHdr+pFwCapsuleHdr->RomLayoutOffset);
                    Status = SetSmmRomLayout(&NewRomLayout, pRomLayout, RomLayoutSize);
                    if(EFI_ERROR(Status)) break;
                    SecSmiFlash.RomLayout = NewRomLayout;
                    // done for Runtime Upd
                    break;
                }
                // continue for a Capsule update flow
#if FWCAPSULE_RECOVERY_SUPPORT == 1
                // Set CapsuleUpd EFI Var. Capsule Mailbox to point to FW_CAPSULE header
                pSessionBlock->FlUpdBlock.ImageSize = pFwCapsuleHdr->CapHdr.CapsuleImageSize;
                pSessionBlock->FlUpdBlock.FwImage.CapsuleMailboxPtr[0] = gpFwCapsuleMailbox;
                Status = UpdateCapsule(pFwCapsuleHdr);
DEBUG ((SecureMod_DEBUG_LEVEL,"SetFlash.UpdateCapsuleVar %r\n",Status));
                if(EFI_ERROR(Status)) break;
#endif
            //  common for Recovery or Capsule
            case FlRecovery:
                //  Set FlUpd EFI Var (Get MC, verify RecFileName)
                Status = SetFlashUpdateVar (pSessionBlock);
DEBUG ((SecureMod_DEBUG_LEVEL,"SetFlash.SetFlashUpdVar %r\n",Status));
                break;

            default:
                Status = EFI_DEVICE_ERROR;
        }
    }
    if (!EFI_ERROR(Status)) {
        // FSHandle is updated if Capsule validation passed.
        // The value should be unique for each new FwCapsule image.
        SecSmiFlash.FSHandle = *(UINT32*)(UINTN)gHashTbl;
        pSessionBlock->FSHandle = SecSmiFlash.FSHandle;
        pSessionBlock->ErrorCode = 0;

        return Status;
    }

    // Set Error Status
    SecSmiFlash.FSHandle = 0;
    SecSmiFlash.pFwCapsule = NULL;
    SecSmiFlash.RomLayout = NULL;
    pSessionBlock->FSHandle  = 0;
    pSessionBlock->ErrorCode = 1;

    return EFI_DEVICE_ERROR;
}

/**
    Routine is called in a loop by the Flash tool.
    Depending on the OS environment, Flash tool passes either an entire 
    Flash Image into SMM buffer or block by block. 
    E.g AFUDOS could allocate a contiguous buffer for the entire ROM buffer,
    while certain OSes (Linux skus) may only allocate limited size buffers

    @param[in]    FUNC_BLOCK   *pFuncBlock
    
    @retval      EFI_STATUS
**/
EFI_STATUS EFIAPI LoadFwImage(
    IN OUT FUNC_BLOCK   *pFuncBlock
)
{
    if(gpFwCapsuleBuffer == 0 || pFuncBlock == NULL)
        return EFI_DEVICE_ERROR;

//DEBUG ((SecureMod_DEBUG_LEVEL,"SecSMI. LoadImage at %lX\n",(UINTN)gpFwCapsuleBuffer + pFuncBlock->BlockAddr));
    if(EFI_ERROR(AmiValidateMemoryBuffer(pFuncBlock, sizeof(FUNC_BLOCK))))
        return EFI_DEVICE_ERROR;

// prevent leaking of the SMM code to the external buffer
    if(EFI_ERROR(AmiValidateMemoryBuffer((VOID*)(pFuncBlock->BufAddr), pFuncBlock->BlockSize)))
        return EFI_DEVICE_ERROR;

// assuming the address in 0 based offset in new ROM image
    if(((UINT64)pFuncBlock->BlockAddr + pFuncBlock->BlockSize) > (UINT64)gFwCapMaxSize)
        return EFI_DEVICE_ERROR;

    SecSmiFlash.FSHandle = 0;  // clear out Hndl. Will be set to valid number in SetFlashMethod
    SecSmiFlash.pFwCapsule = NULL;
    SecSmiFlash.RomLayout = NULL;

    MemCpy((VOID*)((UINTN)gpFwCapsuleBuffer+pFuncBlock->BlockAddr),
            (VOID*)pFuncBlock->BufAddr, pFuncBlock->BlockSize);

    pFuncBlock->ErrorCode = (UINT8)MemCmp(
        (VOID*)((UINTN)gpFwCapsuleBuffer+pFuncBlock->BlockAddr), 
        (VOID*)pFuncBlock->BufAddr, pFuncBlock->BlockSize)==0?0:1;

    return (pFuncBlock->ErrorCode==0)?EFI_SUCCESS:EFI_DEVICE_ERROR;
}
//-----------------------------------------------------------------------------------------
// End Secured Flash Update API
//-----------------------------------------------------------------------------------------
/**
    Use RomLayout map from new verified image to match the data offsets to flash addresses.
    Verify the integrity of the data block to be flashed by comparing its hash with 
    pre-calculated digest in the Hash tbl in smm:  gHashTbl[n]==Hash(NewData)[n]
    Note, SecSmiFlash.pFwCapsule provides an address of a trusted DataBuffer

    @param[in]   FlashAddress    Pointer to address of a flash 
    @param[in]   DataBuffer      Ptr to a Data to be written to Flash
    @param[in]   Area1           Rom Area from Original Rom Image RomMap

    @retval      Status          EFI_SUCCESS if the flash region is inside unSigned RomArea
                                 or verified to be authentic, 
                                 else return EFI_WRITE_PROTECTED
**/
EFI_STATUS NewImageIntegrityCheck(
    VOID* FlashAddress, UINTN Size, UINT8 **DataBuffer, ROM_AREA *Area1
)
{
    EFI_STATUS  Status;
    ROM_AREA    *Area;
    UINTN       BuffOffs;
    UINT8       *BuffAddr;
    UINT8       *PageAddr;
    UINTN       PageSize;
    UINTN       PageNum;
    UINTN       PageCnt;

//    DEBUG ((SecureMod_DEBUG_LEVEL, "New.Flash Range %8lX - %8lX(sz %X)\n", FlashAddress, (UINTN)FlashAddress+Size, Size));

    // Check that new flash image has been validated in "SetFlUpdMethod" prior to flash attempt
    if(SecSmiFlash.pFwCapsule == NULL ||
       SecSmiFlash.FSHandle == 0 || 
       SecSmiFlash.FSHandle != *(UINT32*)gHashTbl ||
       *(UINT32*)gHashTbl == 0xdbdbdbdb // no prior LoadFwImage or SetFlUpdMethod got called
       )
        return EFI_WRITE_PROTECTED;

    for (Area = SecSmiFlash.RomLayout; Area && Area->Size!=0; Area++) {

//        DEBUG ((SecureMod_DEBUG_LEVEL, "New RomArea %8lX(offs %8X) - %8lX, Attr %X\n",Area->Address, Area->Offset, Area->Address+Area->Size, Area->Size, Area->Attributes));
        if( ((UINTN)FlashAddress >= Area->Address && 
             (UINTN)FlashAddress  < (Area->Address+Area->Size))
             ||
             (Area->Address >= (UINTN)FlashAddress && 
              Area->Address  < ((UINT64)(UINTN)FlashAddress + Size)) )
        {
//            if( (Area->Address+Area->Size) < ((UINT64)(UINTN)FlashAddress + Size) ) {
//                DEBUG ((SecureMod_DEBUG_LEVEL, "Range overflow\nFlash Range %8X - %8X\n", 
//                        ((UINTN)Area->Address+Area->Size), ((UINTN)FlashAddress + Size) - ((UINTN)Area->Address+Area->Size)));
            //------------------------------------------------------------------------------------------
            //Debug: check if the flash range overflows the current RomArea
            //       call this func recursively for the overlapped section. 
            //       Is only relevant if next range on the Map has different Flash Addr-to-File Offs mapping
            //------------------------------------------------------------------------------------------
            //    Status = NewImageIntegrityCheck((VOID*)((UINTN)Area->Address+Area->Size), ((UINTN)FlashAddress + Size) - ((UINTN)Area->Address+Area->Size), Area1);
            //    if(EFI_ERROR(Status))
            //        return Status;
//            }
            // Runtime update is allowed for a selected region and by a Flash Update policy
            if ( Area->Attributes & ROM_AREA_FV_SIGNED ||
                (Area1 && Area1->Attributes & ROM_AREA_FV_SIGNED) )
                break;

//            DEBUG ((SecureMod_DEBUG_LEVEL, "Rom Hole 2\n\n"));

            // Rom Hole ->
            return EFI_SUCCESS;
        }
    }
    if(Area == NULL || Area->Size == 0) {
//      DEBUG ((SecureMod_DEBUG_LEVEL, "Range not mapped on a RomArea"));
        return EFI_WRITE_PROTECTED;
    }
//-------------------------------------------------------------------------------------------
//  Follow the RomMap mapping to locate the data block to be 
//  flashed inside the new image by matching FlashAddress with the Image offset within the RomMap table
//------------------------------------------------------------------------------------------
    PageSize = FLASH_BLOCK_SIZE;
    BuffOffs = Area->Offset + (UINTN)FlashAddress - (UINTN)Area->Address;
    PageNum  = BuffOffs / PageSize;
    PageCnt  = Size/PageSize + (Size%PageSize == 0?0:1); // Align to the next page
    // Flash Write -> Update ptr to internal Acpi NVS or SMM Buffer
    BuffAddr = (UINT8*)(UINTN)SecSmiFlash.pFwCapsule;
    PageAddr = (UINT8*)((UINTN)BuffAddr + (PageSize*PageNum));
    BuffAddr = (UINT8*)((UINTN)BuffAddr + BuffOffs);

//   DEBUG ((SecureMod_DEBUG_LEVEL, "Page#0%2d(%2d), PageAddr=%X(%8X...), BuffAddr=%X, RomAddr=%X(%8X...), FlashAddress=%X, Size=%x\n", 
//             PageNum, PageCnt, PageAddr, *((UINT32*)PageAddr), BuffAddr, ((UINTN)FlashAddress - Bios4GBMapStart), 0xFF, FlashAddress, Size));
    while( PageCnt-- ) {

//        DEBUG ((SecureMod_DEBUG_LEVEL, "Hash Page#%02d, FlashAddr=%8X, PageAddr=%8X\n", PageNum, (UINTN)FlashAddress+(PageNum * PageSize), PageAddr));
        if( (PageNum  >= SEC_FLASH_HASH_TBL_BLOCK_COUNT) ||
            (PageAddr >= (BuffAddr + Size))
        ){
            ASSERT (FALSE);
            return EFI_WRITE_PROTECTED;
        }
        // compare new data hash value with the hash of a good image: 
        // skip Flash update Operation if there is no match
        Status = gAmiSig->Hash(gAmiSig, &gEfiHashAlgorithmSha256Guid, 1,
                              (const UINT8**)&PageAddr, (const UINTN*)&PageSize, gHashDB);
        if(EFI_ERROR(Status) || MemCmp(gHashDB, gHashTbl[PageNum], SHA256_DIGEST_SIZE) )
        {
            DEBUG ((SecureMod_DEBUG_LEVEL, "Hash compare Error!\nPage %2d, PageAddr=%x, BuffAddr=%X, Size=%x\n",
                    PageNum, PageAddr, BuffAddr, Size));

            return EFI_WRITE_PROTECTED;
        }
        // repeat Hash check on next Flash Block if Write Block overlaps the Flash Block boundary
        PageNum++;
        PageAddr = (UINT8*)((UINTN)PageAddr + PageSize);
    }

    // not Erase ?
    if(DataBuffer != NULL)
        *DataBuffer = BuffAddr;

    return EFI_SUCCESS;
}
/**
      Protected runtime flash Write/Erase/Update operations

      Verifies if the updated flash address range is protected by a signature
      1. Return Success if flash region mapped onto un-Signed RomArea:
        -ROM Map table must be available and,
        -the flash update region is mapped to un-signed Area(rom hole or NC) and,
        -the ROM area is enabled to be updated in Runtime by a FlUpdatePolicy(SDL settings)
      2. Call NewImageIntegrityCheck() to verify the integrity of the data block to be flashed 
         by comparing its hash with pre-calculated digest in the Hash tbl in smm:
         gHashTbl[n]==Hash(NewData)[n]

      Rules for secure AFU Rom Hole update in Runtime
      1. AFU:Read full BIOS image from flash to a ROM buffer
      2. AFU:Merge the Rom Hole from input file into a ROM buffer
      3. AFU:Send updated ROM image to "LoadFwImage"
      4. Verify the ROM image in "SetFlUpdMethod"
         Note. In order to pass ROM image signature verification the NVRAM section of the ROM should be
               re-constructed in memory using the Std defaults to match the build time ROM image
      5. Calls to erase/write the Rom hole should pass

    @param[in]   FlashAddress    Pointer to address of a flash 
    @param[in]   Size            Number of bytes to be written to Flash
    @param[in]   DataBuffer      Ptr to a Data to be written to Flash

    @retval      Status          EFI_SUCCESS if the flash region is inside unSigned RomArea
                                 or verified to be authentic, 
                                 else return EFI_WRITE_PROTECTED
**/
EFI_STATUS BeforeSecureUpdate (
    VOID* FlashAddress, UINTN Size, UINT8 **DataBuffer
)
{
    EFI_STATUS  Status;
    ROM_AREA    *Area;

//    DEBUG ((SecureMod_DEBUG_LEVEL, "Flash Addr %8lX - %8lX(sz %X)\n", FlashAddress, (UINTN)FlashAddress+Size, Size));

    if(OrgRomLayout == NULL) {
        /*
         * Unlikely scenario: Missing Rom Layout data -> Process with RomMap from new Trusted FW if loaded
         */
        return NewImageIntegrityCheck(FlashAddress, Size, DataBuffer, NULL);
    }
    // Use local image RomLayout to enforce static Rom Hole locations in new ROM image
    for (Area = OrgRomLayout; Area && Area->Size!=0; Area++) {
//        DEBUG ((SecureMod_DEBUG_LEVEL, "Org RomArea %8lX(offs %8X) - %8X(sz %X), Attr %X\n",Area->Address, Area->Offset, Area->Address+Area->Size, Area->Size, Area->Attributes));
        if( ((UINTN)FlashAddress >= Area->Address && 
             (UINTN)FlashAddress  < (Area->Address+Area->Size))
             ||
             (Area->Address >= (UINTN)FlashAddress && 
              Area->Address  < ((UINT64)(UINTN)FlashAddress + Size)) )
        {
//            DEBUG ((SecureMod_DEBUG_LEVEL, "Mapped range: %8lX - %8lX(sz %X), Attr %X(Signed %X)\n",Area->Address, Area->Address+Area->Size, Size, Area->Attributes, (Area->Attributes & ROM_AREA_FV_SIGNED)));
            if(// !(Area->Attributes & AMI_ROM_AREA_RUN_TIME_UPDATABLE) ||
               ( BootMode != BOOT_ON_FLASH_UPDATE && BootMode != BOOT_IN_RECOVERY_MODE) && // exclude a recovery boot flow
               (
                ( (Area->Attributes & AMI_ROM_AREA_VITAL) && !(FlUpdatePolicy.BBUpdate & FlRuntime)) ||
                (!(Area->Attributes & AMI_ROM_AREA_VITAL) && !(FlUpdatePolicy.FlashUpdate & FlRuntime))
               )
            )
                return EFI_WRITE_PROTECTED;

            // Runtime update is allowed for a selected region and by a Flash Update policy
            if (Area->Attributes & ROM_AREA_FV_SIGNED)
                break;

//DEBUG ((SecureMod_DEBUG_LEVEL, "Rom Hole 1\n"));
            
            // Address is within un-protected range for the original BIOS image
            // Until new RomImage is loaded and validated - this range is treated as un-protected Rom Hole.
            // Once new image is set - the check for the rom hole will be extended to a new Rom Map
            if(NewRomLayout /*SecSmiFlash.RomLayout */ == NULL)
                return EFI_SUCCESS;

            break;
        }
    }
    // only mapped regions can be updated
    if(Area == NULL || Area->Size == 0) {
//DEBUG ((SecureMod_DEBUG_LEVEL, "Range not mapped on a RomArea"));
        return EFI_WRITE_PROTECTED;
    }

    Status = NewImageIntegrityCheck(FlashAddress, Size, DataBuffer, Area);
//    DEBUG ((SecureMod_DEBUG_LEVEL, "Block integrity check: %r\n\n", Status));
    return Status;
}

/**
    Allows to write to flash device if Secure Capsule is loaded into memory
    Function replacing Flash->Write API call

    @param[in]    VOID* FlashAddress
    @param[in]    UINTN Size
    @param[in]    VOID* DataBuffer

    @retval      EFI_STATUS
**/
EFI_STATUS EFIAPI SecureFlashWrite (
    VOID* FlashAddress, UINTN Size, VOID* DataBuffer
)
{
    EFI_STATUS  Status;
    UINT8       *CurrBuff = (UINT8*)DataBuffer;

    Status = BeforeSecureUpdate(FlashAddress, Size, &CurrBuff);
//DEBUG ((SecureMod_DEBUG_LEVEL, "SecSMIFlash: Write %r, FlshAddr=%X(%X), BuffAddr=%X, OrgBuff=%X\n", Status, FlashAddress, Size, CurrBuff, DataBuffer));
    if(!EFI_ERROR(Status))
        return pFlashWrite(FlashAddress, Size, CurrBuff);

    return Status;
}

/**
    Allows to write to flash device if Secure Capsule is loaded into memory
    Function replacing Flash->Update API call

    @param[in]    VOID* FlashAddress
    @param[in]    UINTN Size
    @param[in]    VOID* DataBuffer

    @retval      EFI_STATUS
**/
EFI_STATUS EFIAPI SecureFlashUpdate (
    VOID* FlashAddress, UINTN Size, VOID* DataBuffer
)
{
    EFI_STATUS  Status;
    UINT8       *CurrBuff = (UINT8*)DataBuffer;

    Status = BeforeSecureUpdate(FlashAddress, Size, &CurrBuff);
//DEBUG ((SecureMod_DEBUG_LEVEL, "SecSMIFlash: Update %r, FlshAddr=%X(%X), BuffAddr=%X, OrgBuff=%X\n", Status, FlashAddress, Size, CurrBuff, DataBuffer));
    if(!EFI_ERROR(Status))
        return pFlashUpdate(FlashAddress, Size, CurrBuff);

    return Status;
}

/**
    Allows erase of flash device is Secure Capsule is loaded into memory
    Function replacing Flash->Erase API call

    @param[in]    VOID* FlashAddress
    @param[in]    UINTN Size

    @retval      EFI_STATUS
**/
EFI_STATUS EFIAPI SecureFlashErase (
    VOID* FlashAddress, UINTN Size
)
{
    EFI_STATUS  Status;

    Status = BeforeSecureUpdate(FlashAddress, Size, NULL);
//DEBUG ((SecureMod_DEBUG_LEVEL, "SecSMIFlash Erase %r, FlshAddr=%X(%X)\n", Status, FlashAddress, Size));
    if(!EFI_ERROR(Status))
        return pFlashErase(FlashAddress, Size);

    return Status;
}
//-------------------------------------------------------
// Auxiliary functions
//-------------------------------------------------------
/**
   Loads a FwCapsule from GUIDed section of ffs

    @param[out]   Buffer    returns a pointer to allocated memory. Caller must free it when done.
    @param[out]   Size      returns the size of the binary loaded into the buffer.

    @retval      EFI_STATUS
                 EFI_NOT_FOUND  - Can't find the binary.
                 EFI_LOAD_ERROR - Load fail.
                 EFI_SUCCESS    - Load success.
**/
EFI_STATUS
GetFwCapFfs (
  IN OUT  VOID           **Buffer,
  IN OUT  UINTN          *Size
  )
{
  EFI_STATUS                    Status;
  UINTN                         HandleCount;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME_PROTOCOL  *Fv;
  EFI_HANDLE                    *HandleBuff;
  UINT32                        AuthenticationStatus;

 *Buffer=0;
 *Size=0;
  Status = pBS->LocateHandleBuffer (ByProtocol,&gEfiFirmwareVolume2ProtocolGuid,NULL,&HandleCount,&HandleBuff);
  if (EFI_ERROR (Status) || HandleCount == 0) {
    return EFI_NOT_FOUND;
  }
  //
  // Find desired image in all Fvs
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = pBS->HandleProtocol (HandleBuff[Index],&gEfiFirmwareVolume2ProtocolGuid,(void**)&Fv);

    if (EFI_ERROR (Status)) {
       continue;//return EFI_LOAD_ERROR;
    }
    //
    // Try a raw file
    //
    Status = Fv->ReadSection (
                  Fv,
                  &gFwCapFfsGuid,
                  EFI_SECTION_FREEFORM_SUBTYPE_GUID,//EFI_SECTION_RAW
                  0,    //Instance
                  Buffer,
                  Size,
                  &AuthenticationStatus
                  );

    if (Status == EFI_SUCCESS &&
        !guidcmp(&gFwCapSectionGuid, (EFI_GUID*)*Buffer))
        break;
  }

  pBS->FreePool(HandleBuff);

  if (Index >= HandleCount) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
   Returns a pointer to current BIOS Rom Layout map
   Default location for RomLayout is inside the FwCapsule's Header.
   If not FwCapHder ffs present - use primary RomLayout reported by Rom Layout HOB

    @param[in]    SystemTable Pointer to System Table
    @param[out]   RomLayout

    @retval      EFI_STATUS
**/
EFI_STATUS GetRomLayout(
    IN EFI_SYSTEM_TABLE    *SystemTable,
    OUT ROM_AREA **RomLayout
)
{
    EFI_STATUS Status;
    static EFI_GUID HobListGuid = HOB_LIST_GUID;
    static EFI_GUID AmiRomLayoutHobGuid = AMI_ROM_LAYOUT_HOB_GUID;
    ROM_LAYOUT_HOB *RomLayoutHob;
    UINTN RomLayoutSize=0;
    APTIO_FW_CAPSULE_HEADER*  pFwCapHdr = NULL;
    ROM_AREA *OrgRomLayout = NULL;
    VOID *pFfsSec;

// 1. Try to locate RomLayout from embedded CapHdr Ffs
    Status = GetFwCapFfs(&pFfsSec, &RomLayoutSize);
    if (!EFI_ERROR(Status)) {
        // Skip over Section GUID
        pFwCapHdr = (APTIO_FW_CAPSULE_HEADER*)((UINTN)pFfsSec + sizeof (EFI_GUID));
        if(!guidcmp((EFI_GUID*)&pFwCapHdr->CapHdr.CapsuleGuid, &gFWCapsuleGuid) &&
           IsFwCapHdrValid(pFwCapHdr)) {

            OrgRomLayout = (ROM_AREA*)((UINTN)pFwCapHdr+pFwCapHdr->RomLayoutOffset);
            RomLayoutSize = pFwCapHdr->RomImageOffset - pFwCapHdr->RomLayoutOffset;
        }
    }
    DEBUG ((SecureMod_DEBUG_LEVEL, "RomLayout in FwCapHdr FFS at %X(size 0x%X), map offs %X\n", pFwCapHdr, pFwCapHdr->RomLayoutOffset, RomLayoutSize));

    if(OrgRomLayout==NULL || RomLayoutSize==0) {
// 2. Backup: Use primary RomLayout from Rom Layout HOB.
//  This one does not yet report the Rom Hole regions
    //----- Get HobList -------------------------------------
        RomLayoutHob = GetEfiConfigurationTable(SystemTable, &HobListGuid);
        if (RomLayoutHob!=NULL) {
    // -------- Get RomLayoutHob ----------------------
            Status = FindNextHobByGuid((EFI_GUID *)&AmiRomLayoutHobGuid, (VOID **)&RomLayoutHob);
            if (!EFI_ERROR(Status)) {

                RomLayoutSize =   RomLayoutHob->Header.Header.HobLength
                                  - sizeof(ROM_LAYOUT_HOB);

                OrgRomLayout=(ROM_AREA*)(RomLayoutHob+1);
                DEBUG ((SecureMod_DEBUG_LEVEL, "Legacy RomLayout from Hob at %X(size 0x%X)\n", OrgRomLayout, RomLayoutSize));
            }
        }
    }
    if(RomLayoutSize) {
        //---Allocate memory in SMRAM for RomLayout---
        Status = SetSmmRomLayout(RomLayout, OrgRomLayout, RomLayoutSize);
        if(pFfsSec)
            pBS->FreePool(pFfsSec);
        return Status;
    }
    return EFI_NOT_FOUND;
}
/**
   Allocate a copy of RomMap in SMM buffer

    @param[in]    RomLayout
    @param[in]    RomLayoutSize
    @param[out]   New RomLayout

    @retval      EFI_STATUS
**/
EFI_STATUS
SetSmmRomLayout(
    ROM_AREA     **NewRomLayout,
    ROM_AREA      *RomLayout,
    UINTN          RomLayoutSize
) {
    EFI_STATUS Status;

    if(!NewRomLayout || !RomLayout || !RomLayoutSize )
        return EFI_INVALID_PARAMETER;

    if(*NewRomLayout) { 
        pSmst->SmmFreePool(*NewRomLayout);
        *NewRomLayout = NULL;
    }
    Status = pSmst->SmmAllocatePool(EfiRuntimeServicesData, RomLayoutSize, (void **)NewRomLayout);
    if(EFI_ERROR(Status)) {
      return Status;
    }
    MemSet(*NewRomLayout, RomLayoutSize, 0);
    MemCpy(*NewRomLayout, RomLayout, RomLayoutSize);
    DEBUG ((SecureMod_DEBUG_LEVEL,"Copy RomLayout %08X -> SMM:%08X, size 0x%X\n", RomLayout, *NewRomLayout, RomLayoutSize));
    DEBUG_CODE (
            INTN i;
            UINTN Size;
            ROM_AREA *Area;
            
            Size = 0;
            Area = *NewRomLayout;
            for (i=0; (Area[i]).Size!=0 && Size < RomLayoutSize; i++) {
                Size += sizeof(ROM_AREA);
                DEBUG ((SecureMod_DEBUG_LEVEL, "%08X...%08lX, offs %08X, size 0x%08X, attr %X\n",
                        Area[i].Address, (Area[i].Address+Area[i].Size-1), Area[i].Offset, Area[i].Size, Area[i].Attributes));
            }
    );

    return Status;
}

// !!! do not install if OFBD SecFlash is installed
#if INSTALL_SECURE_FLASH_SW_SMI_HNDL == 1
/**
   SW SMI SecSMIFlash Handler

    @param[in]    EFI_HANDLE  DispatchHandle,
    @param[in]    CONST VOID  *Context OPTIONAL,
    @param[out]   VOID        *CommBuffer OPTIONAL,
    @param[out]   UINTN       *CommBufferSize OPTIONAL

    @retval      EFI_STATUS
**/
EFI_STATUS
SecSMIFlashSMIHandler (
    IN  EFI_HANDLE                  DispatchHandle,
        IN CONST VOID               *Context OPTIONAL,
        IN OUT VOID                 *CommBuffer OPTIONAL,
        IN OUT UINTN                *CommBufferSize OPTIONAL
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT8        Data;
    UINT64       pCommBuff;
    UINT32       HighBufferAddress = 0;
    UINT32       LowBufferAddress = 0;
    UINTN       Cpu;

//APTIOV_SERVER_OVERRIDE_START: SA50081 Security Advisory update
    //
    // If input is invalid, stop processing this SMI
    //
    if (CommBuffer == NULL || CommBufferSize == NULL) {
      return Status;
    }
    // Sanity check
    //
    if (*CommBufferSize < sizeof(EFI_SMM_SW_CONTEXT)) {
      TRACE ((TRACE_ALWAYS, "Command Buffer Size invalid!\n"));
      return Status;
    }
    if(EFI_ERROR(AmiValidateSmramBuffer(CommBuffer, sizeof(EFI_SMM_SW_CONTEXT)))) {
      TRACE ((TRACE_ALWAYS, "Command Buffer outside of SMRAM!\n"));
      return Status;
    }
//APTIOV_SERVER_OVERRIDE_END: SA50081 Security Advisory update

    Cpu = ((EFI_SMM_SW_CONTEXT*)CommBuffer)->SwSmiCpuIndex;
    //
    // Found Invalid CPU number, return
    //
    if(Cpu == (UINTN)-1) RETURN(Status);

    gSmmCpu->ReadSaveState ( gSmmCpu, \
                                      4, \
                                      EFI_SMM_SAVE_STATE_REGISTER_RBX, \
                                      Cpu, \
                                      &LowBufferAddress );
    gSmmCpu->ReadSaveState ( gSmmCpu, \
                                      4, \

                                      EFI_SMM_SAVE_STATE_REGISTER_RCX, \
                                      Cpu, \
                                      &HighBufferAddress );

    Data = ((EFI_SMM_SW_CONTEXT*)CommBuffer)->CommandPort;

    pCommBuff            = HighBufferAddress;
    pCommBuff            = Shl64(pCommBuff, 32);
    pCommBuff           += LowBufferAddress;

//TRACE((TRACE_ALWAYS, "Sec SW SMI Flash Hook == 0x%x (%r)\n", Data, Status));
//APTIOV_SERVER_OVERRIDE_START: SA50081 Security Advisory update
    if(EFI_ERROR(AmiValidateMemoryBuffer((CONST VOID* )((UINTN)pCommBuff), sizeof(UINTN)))) {
        TRACE ((TRACE_ALWAYS, "Buffer in SMRAM or overflow!\n"));
        return Status;
    }
//APTIOV_SERVER_OVERRIDE_END: SA50081 Security Advisory update
    switch(Data)
    {
        case SecSMIflash_Load:             // 0x1d Send Flash Block to memory
            Status = LoadFwImage((FUNC_BLOCK *)pCommBuff);
            break;

        case SecSMIflash_GetPolicy:        // 0x1e Get Fl Upd Policy 
            Status = GetFlUpdPolicy((FLASH_POLICY_INFO_BLOCK *)pCommBuff);
            break;
        
        case SecSMIflash_SetFlash:        // 0x1f Set Flash method
            Status = SetFlUpdMethod((FUNC_FLASH_SESSION_BLOCK *)pCommBuff);
            break;
    }
//DEBUG ((SecureMod_DEBUG_LEVEL,"Exit SW SMI with %r\n", Status));
    RETURN(Status);
}
#endif

/**
    Main initialization function, called from Smi Entry Point

    @param[in]    EFI_HANDLE ImageHandle, 
    @param[in]    EFI_SYSTEM_TABLE *SystemTable
    
    @retval      EFI_STATUS
**/
EFI_STATUS InSmmFunction(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
#if INSTALL_SECURE_FLASH_SW_SMI_HNDL == 1
    EFI_SMM_SW_DISPATCH2_PROTOCOL    *pSwDispatch = NULL;
    EFI_SMM_SW_REGISTER_CONTEXT      SwContext;
    UINTN                            Index;
    UINT8   MinSMIPort = SecSMIflash_Load;    //0x1d
    //UINT8   MinSMIPort = SecSMIflash_GetPolicy; //0x1e;
    UINT8   MaxSMIPort = SecSMIflash_SetFlash; //0x1f;
#endif
#if FWCAPSULE_RECOVERY_SUPPORT == 1
const EFI_SMM_SX_REGISTER_CONTEXT      SxS5RegisterContext = {SxS5, SxEntry};
#if FWCAPSULE_S4_SHUTDOWN_SUPPORT == 1
const EFI_SMM_SX_REGISTER_CONTEXT      SxS4RegisterContext = {SxS4, SxEntry};
#endif
      EFI_SMM_SX_DISPATCH2_PROTOCOL    *SxDispatchProtocol;
#endif
    EFI_GUID guidHob = HOB_LIST_GUID;
    EFI_HOB_HANDOFF_INFO_TABLE *pHit;

    EFI_STATUS              Status;
    EFI_HANDLE              Handle = NULL;
    EFI_HANDLE              DummyHandle = NULL;
    UINTN                   Size;
#if NEW_BIOS_MEM_ALLOC == 0
    VOID                   *Memory;
#endif    
    AMI_FLASH_PROTOCOL     *Flash;

    InitAmiSmmLib( ImageHandle, SystemTable );

    pHit = GetEfiConfigurationTable(pST, &guidHob);
    if (pHit == NULL)
        return EFI_LOAD_ERROR;

    //Get Boot Mode
    BootMode = pHit->BootMode;

#if CSLIB_WARM_RESET_SUPPORTED == 0 && FWCAPSULE_RECOVERY_SUPPORT == 1
    if (BootMode == BOOT_ON_FLASH_UPDATE) {
        if( (ReadRtcIndexedRegister(0xB) & ( 1 << 5 )) == ( 1 << 5 )) {
            // ========== INTEL CHIPSET PORTING ========================
            //clear RTC_STS bit in PM1_STS if resuming from RTC alarm on flash update
            IoWrite16(PM_BASE_ADDRESS, ( IoRead16(PM_BASE_ADDRESS) | (1 << 10) ));
            //disable the RTC alarm
            WriteRtcIndexedRegister(0xB, ( ReadRtcIndexedRegister(0xB) & ~( 1 << 5 ) ));
        }
    }
#endif

    Status = pSmst->SmmLocateProtocol(&gAmiSmmDigitalSignatureProtocolGuid, NULL, (void**)&gAmiSig);
    if (EFI_ERROR(Status) || gAmiSig == NULL) return Status;

// Test if Root Platform Key is available,else - don't install Flash Upd security measures.
    gpPubKeyHndl.Blob = NULL;
    gpPubKeyHndl.BlobSize = 0;
    Status = gAmiSig->GetKey(gAmiSig, &gpPubKeyHndl, &gPRKeyGuid, gpPubKeyHndl.BlobSize, 0);
DEBUG ((SecureMod_DEBUG_LEVEL,"GetKey %r (%X, 0x%lx bytes)\n",Status, gpPubKeyHndl.Blob, gpPubKeyHndl.BlobSize));
    if (EFI_ERROR(Status) || gpPubKeyHndl.Blob == NULL) {
        if(Status == EFI_BUFFER_TOO_SMALL) 
            return EFI_SUCCESS;
        return Status;
    }
    //
    // Allocate scratch buffer to hold entire Signed BIOS image for Secure Capsule and Runtime Flash Updates
    // AFU would have to execute a sequence of SW SMI calls to push entire BIOS image to SMM
    //
    //NEW_BIOS_MEM_ALLOC == 2 AFU will allocate a buffer and provide pointer via SET_FLASH_METHOD API call. 
    //
#if NEW_BIOS_MEM_ALLOC < 2
#if NEW_BIOS_MEM_ALLOC == 0
    //
    // Alternatively the buffer may be reserved within the SMM memory 
    //
    Status = pSmst->SmmAllocatePool(EfiRuntimeServicesData, gFwCapMaxSize, &Memory);
    gpFwCapsuleBuffer = (EFI_PHYSICAL_ADDRESS)(UINTN)Memory;
#else//  NEW_BIOS_MEM_ALLOC == 1
    //
    // The buffer is allocated anywhere within reserved system memory
    //
    Status = pST->BootServices->AllocatePages(AllocateAnyPages, EfiReservedMemoryType, EFI_SIZE_TO_PAGES(gFwCapMaxSize), &gpFwCapsuleBuffer);
#endif
    DEBUG ((SecureMod_DEBUG_LEVEL,"Allocate FwCapsuleBuffer %lX,0x%X %r\n",gpFwCapsuleBuffer, gFwCapMaxSize, Status));
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    MemSet((void*)gpFwCapsuleBuffer, gFwCapMaxSize, 0 );
#endif
    //
    // Allocate space to hold a Hash table for all Flash blocks in SMM
    //
    Size = SEC_FLASH_HASH_TBL_SIZE;
    Status = pSmst->SmmAllocatePool(EfiRuntimeServicesData, Size, (void**)&gHashTbl);
    DEBUG ((SecureMod_DEBUG_LEVEL,"Allocate HashTbl Pool %lx(0x%x) %r\n",gHashTbl, Size, Status));
    if (EFI_ERROR(Status)) return Status;
    MemSet((void*)gHashTbl, Size, 0xdb );

#if FWCAPSULE_RECOVERY_SUPPORT == 0
    FlUpdatePolicy.FlashUpdate &=~FlCapsule;
    FlUpdatePolicy.BBUpdate &=~FlCapsule;
#else
    gpFwCapsuleMailboxSize = 4*sizeof(EFI_CAPSULE_BLOCK_DESCRIPTOR) + sizeof(EFI_CAPSULE_HEADER); // (4*16)+28
    //
    // Reserve memory pool for capsule's mailbox list
    //
#if NEW_BIOS_MEM_ALLOC == 0
    // in smm
    Status = pSmst->SmmAllocatePool(EfiRuntimeServicesData, gpFwCapsuleMailboxSize, &Memory);
    gpFwCapsuleMailbox = (EFI_PHYSICAL_ADDRESS)(UINTN)Memory;
#else
    // in generic reserved memory
    Status = pST->BootServices->AllocatePages(AllocateAnyPages, EfiReservedMemoryType, EFI_SIZE_TO_PAGES(gpFwCapsuleMailboxSize), &gpFwCapsuleMailbox);
#endif
    DEBUG ((SecureMod_DEBUG_LEVEL,"Allocate FwCapsuleMailbox %lx(0x%x) %r\n",gpFwCapsuleMailbox, gpFwCapsuleMailboxSize, Status));
    if (EFI_ERROR(Status)) return Status;
    MemSet((void*)gpFwCapsuleMailbox, gpFwCapsuleMailboxSize, 0 );

    //
    // Install callback on S5 Sleep Type SMI. Needed to transition to S3 if Capsule's mailbox is pending
    // Locate the Sx Dispatch Protocol
    //
    // gEfiSmmSxDispatch2ProtocolGuid
    Status = pSmst->SmmLocateProtocol( &gEfiSmmSxDispatch2ProtocolGuid, NULL, (void**)&SxDispatchProtocol);
    ASSERT_EFI_ERROR (Status);  
    if (EFI_ERROR(Status)) return Status;
    //
    // Register the callback for S5 entry
    //
    if (SxDispatchProtocol && SupportUpdateCapsuleReset()) {
      Status = SxDispatchProtocol->Register (
                    SxDispatchProtocol,
                    (EFI_SMM_HANDLER_ENTRY_POINT2)SmiS5CapsuleCallback,
                    &SxS5RegisterContext,
                    &Handle
                    );
#if FWCAPSULE_S4_SHUTDOWN_SUPPORT == 1
      if (!EFI_ERROR(Status)) { 
          Status = SxDispatchProtocol->Register (
                    SxDispatchProtocol,
                    (EFI_SMM_HANDLER_ENTRY_POINT2)SmiS4CapsuleCallback,
                    &SxS4RegisterContext,
                    &Handle
                    );
      }
#endif    
      if (EFI_ERROR(Status)) goto Done;
    }
#endif

    Status = GetRomLayout(SystemTable, &OrgRomLayout);
DEBUG ((SecureMod_DEBUG_LEVEL,"Rom Layout(SMM) %X %r\n",OrgRomLayout, Status));
    // Rom Layout HOB may not be found in Recovery mode and if FW does not include built in FwCapsule Hdr file
//    if (EFI_ERROR(Status)) goto Done;
    //
    // Trap the original Flash Driver API calls to enforce 
    // Flash Write protection in SMM at the driver API level
    //
    Flash = NULL;
    Status = pSmst->SmmLocateProtocol(&gAmiSmmFlashProtocolGuid, NULL, (void**)&Flash);
    if (EFI_ERROR(Status)) 
        Status = pBS->LocateProtocol(&gAmiSmmFlashProtocolGuid, NULL, (void**)&Flash);
DEBUG ((SecureMod_DEBUG_LEVEL,"Find SMM Flash API %r\nFlash->Write API fixup %X->%X to ->%X\n", Status,Flash,(Flash==NULL)?0:Flash->Write,SecureFlashWrite));
    if (EFI_ERROR(Status) || Flash == NULL) goto Done;

    // preserve org Flash API
    pFlashWrite = Flash->Write; 
    pFlashUpdate = Flash->Update;
    pFlashErase = Flash->Erase;

    // replace with local functions 
    Flash->Write = SecureFlashWrite;
    Flash->Update= SecureFlashUpdate;
    Flash->Erase = SecureFlashErase;
    //
    // Install Secure SMI Flash Protocol 
    //
    SecSmiFlash.pFwCapsule = (UINT32*)gpFwCapsuleBuffer;
    SecSmiFlash.HashTbl = gHashTbl;
    SecSmiFlash.RomLayout = NULL;
    SecSmiFlash.FSHandle = 0;
    Status = pSmst->SmmInstallProtocolInterface(
                 &DummyHandle,
                 &gSecureSMIFlashProtocolGuid,
                 EFI_NATIVE_INTERFACE,
                 &SecSmiFlash
             );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) goto Done;

    //
    // Install SW SMI callbacks for 3 SecSMI Flash functions
    // !!! do not install if OFBD SecFlash is installed 
    //
#if INSTALL_SECURE_FLASH_SW_SMI_HNDL == 1

    Status = pSmst->SmmLocateProtocol( \
                        &gEfiSmmSwDispatch2ProtocolGuid, NULL, &pSwDispatch);
    if (EFI_ERROR(Status)) return EFI_SUCCESS;


    Status = pSmst->SmmLocateProtocol(&gEfiSmmCpuProtocolGuid, NULL, &gSmmCpu);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return EFI_SUCCESS;

    for(Index=MinSMIPort;Index<=MaxSMIPort;Index++)
    {
        SwContext.SwSmiInputValue    = Index;
        Status    = pSwDispatch->Register(pSwDispatch, SecSMIFlashSMIHandler, &SwContext, &Handle);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) break;
        //If any errors,unregister any registered SwSMI by this driver.
        //If error, and driver is unloaded, then a serious problem would exist.
    }
#endif
Done:
    return EFI_SUCCESS;
}

/**
    Smi driver Entry Point

    @param[in]    EFI_HANDLE ImageHandle, 
    @param[in]    EFI_SYSTEM_TABLE *SystemTable
    
    @retval      EFI_STATUS
**/
EFI_STATUS EFIAPI SecSMIFlashDriverEntryPoint(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE    *SystemTable
)
{
    InitAmiLib(ImageHandle, SystemTable);
    return InitSmmHandlerEx(ImageHandle, SystemTable, InSmmFunction, NULL);
}

