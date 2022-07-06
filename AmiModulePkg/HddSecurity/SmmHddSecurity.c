//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file SmmHddSecurity.c
    Provide functions to unlock HDD password during S3 resume

**/

//---------------------------------------------------------------------------

#include "SmmHddSecurity.h"
#include <Protocol/IoMmu.h>
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
#include "AhciController.h"
#define     COMMAND_LIST_SIZE_PORT              0x800
#endif
#include <Library/SmmConfidentialMemLib.h>
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
#include <Include/NvmeRstPwdIncludes.h>
#endif
#ifdef  SW_SMI_IO_ADDRESS
UINT8   SwSmiPort = SW_SMI_IO_ADDRESS;
#else
// Default Sw Smi port 
UINT8   SwSmiPort=0xB2;
#endif

//---------------------------------------------------------------------------

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
AMI_AHCI_BUS_SMM_PROTOCOL *mAhciSmm = NULL;
#endif

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
AMI_NVME_PASS_THRU_PROTOCOL     *gSmmNvmePassThru=NULL;
#endif

UINT8    *gSecurityUnlockCommandBuffer = NULL;

EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch;
EFI_SMM_SYSTEM_TABLE2           *pSmst2;

UINT8                 *SecurityBuffer     = NULL;
BOOLEAN               AhciInit            = FALSE;
UINT64                PciExpressBaseAddress = 0;

#define PCI_CFG_ADDR( bus, dev, func, reg ) \
    ((VOID*)(UINTN) (PciExpressBaseAddress\
                     + ((bus) << 20) + ((dev) << 15) + ((func) << 12) + reg))

#define AHCI_BAR    0x24
#define PCI_SCC     0x000A        // Sub Class Code Register


#if defined(EFI64) || defined(EFIx64)         // Check Flag to Select the Function for
                                              // the corresponding Execution Platform
/** @internal
    Dividend input is divided by the Divisor input
    parameter and Stored the Remainder value in Remainder
    argument and Returns Quotient.This Function is selected in
    64 bit environment architecture. 

    @param Dividend             Input value to be divided by divisor
    @param Divisor              Input value that divides the Dividend(Can only be 31 bits)
    @param Remainder OPTIONAL   The result of division operation.

    @retval UINT64          Quotient value.

    @endinternal
**/

static
UINT64 DivU64x32Local (
    IN  UINT64              Dividend,
    IN  UINTN               Divisor,
    OUT UINTN*Remainder     OPTIONAL
)
{
    UINT64 Result = Dividend / Divisor;

    if ( Remainder ) {
        *Remainder = Dividend % Divisor;
    }
    return Result;
}

#else

/** @internal
    Dividend input is divided by the Divisor input
    parameter and Stored the Remainder value in Remainder
    argument and Returns Quotient. This function is selected in
    32 bit environment architecture.

    @param Dividend             Input value to be divided by divisor
    @param Divisor              Input value that divides the Dividend(Can only be 31 bits)
    @param Remainder OPTIONAL   The result of division operation.

    @return UINT64          Quotient value.

    @endinternal
**/

static
UINT64 DivU64x32Local (
    IN  UINT64              Dividend,
    IN  UINTN               Divisor,   //Can only be 31 bits.
    OUT UINTN*Remainder     OPTIONAL
)
{
    UINT64 Result;
    UINT32 Rem;

    _asm
    {
        mov eax, dword ptr Dividend[0]
        mov edx, dword ptr Dividend[4]
        mov esi, Divisor
        xor     edi, edi                    ; Remainder
        mov     ecx, 64                     ; 64 bits
Div64_loop:
        shl     eax, 1                      ;Shift dividend left. This clears bit 0.
        rcl     edx, 1
        rcl     edi, 1                      ;Shift remainder left. Bit 0 = previous dividend bit 63.

        cmp     edi, esi                    ; IF Rem >= Divisor, dont adjust
        cmc                                 ; ELSE adjust dividend and subtract divisor.
        sbb     ebx, ebx                    ; IF Rem >= Divisor, ebx = 0, ELSE ebx = -1.
        sub     eax, ebx                    ; IF adjust, bit 0 of dividend = 1
        and     ebx, esi                    ; IF adjust, ebx = Divisor, ELSE ebx = 0. 
        sub     edi, ebx                    ; IF adjust, subtract divisor from remainder.
        loop    Div64_loop

        mov     dword ptr Result[0], eax
        mov     dword ptr Result[4], edx
        mov     Rem, edi
    }

        if (Remainder) *Remainder = Rem;

        return Result;
}
#endif


/** @internal
    Stalls for the Required Amount of MicroSeconds

    @param Usec      Number of microseconds to delay

    @return VOID

    @endinternal
**/
VOID 
Stall (
    UINTN           Usec
)
{
    UINTN   Counter, i;
    UINT32  Data32, PrevData;
    UINTN   Remainder;

  Counter = (UINTN)DivU64x32Local ((Usec * 10), 3, &Remainder);

  if (Remainder != 0) {
    Counter++;
  }

  //
  // Call WaitForTick for Counter + 1 ticks to try to guarantee Counter tick
  // periods, thus attempting to ensure Microseconds of stall time.
  //
  if (Counter != 0) {

    PrevData = IoRead32(PM_BASE_ADDRESS + 8);
    for (i=0; i < Counter; ) {
       Data32 = IoRead32(PM_BASE_ADDRESS + 8);    
        if (Data32 < PrevData) {        // Reset if there is a overlap
            PrevData=Data32;
            continue;
        }
        i += (Data32 - PrevData);        
        PrevData = Data32;
    }
  }
}

/** @internal
    Clears the buffer

    @param *Buffer          Pointer to the buffer to be cleared.
    @param Size             Size of the buffer.

    @return VOID

    @endinternal
**/
VOID
ZeroMemorySmm (
    VOID            *Buffer,
    UINTN           Size
 )
{
    UINT8   *Ptr;
    Ptr = Buffer;
    while (Size--) {
        *(Ptr++) = 0;
    }
}

/** @internal
    Waits for the given bit to be cleared

    @param *pHddPassword        Pointer to HDD_PASSWORD
    @param BitClear             The bit to be cleared within time-out
    @param Timeout              Time limit before which the bit must be cleared.

    @retval EFI_SUCCESS         The bit is cleared before Time-out.
    @retval EFI_TIMEOUT         Time-out before the bit is cleared.

    @note
         1. In this function the wait time is 10 microseconds.
         
    @endinternal
**/
EFI_STATUS
SmmWaitforBitClear (
    HDD_PASSWORD        *pHddPassword,
    UINT8               BitClear,
    UINT32              Timeout
)
{
    UINT8           Delay;
    UINT8           Data8;

    for ( ; Timeout > 0; Timeout--) {
        for ( Delay = 100; Delay > 0; Delay--) {
            Data8 = IoRead8(pHddPassword->DeviceControlReg);
            if (!(Data8 & BitClear)) return EFI_SUCCESS;
            Stall(10);                      // 10 Usec
        }
    }
    return EFI_TIMEOUT;
}

/** @internal
    Checks for a particular Bit to be set for a given amount of time 

    @param *pHddPassword        Pointer to HDD_PASSWORD
    @param BitSet               The bit to be set within the time-out
    @param Timeout              Time limit before which the bit must be set

    @retval EFI_SUCCESS         The bit is set before Time-out.
    @retval EFI_TIMEOUT         Time-out before the bit is set.

    @endinternal
**/

EFI_STATUS
SmmWaitforBitSet (
    HDD_PASSWORD                    *pHddPassword,
    UINT8                           BitSet,
    UINT32                          TimeOut
)
{
    UINT8           Delay;
    UINT8           Data8;

    for ( ; TimeOut > 0; TimeOut--) {
        for ( Delay = 100; Delay > 0; Delay--) {
            Data8 = IoRead8(pHddPassword->DeviceControlReg);
            if (Data8 & BitSet) {
                return EFI_SUCCESS;
            }
            Stall(10);                      // 10 Usec
        }
    }
    return EFI_TIMEOUT;
}

/** @internal
    Waits for BSY bit to get clear

    @param *pHddPassword        Pointer to HDD_PASSWORD

    @retval EFI_SUCCESS         BSY bit is cleared before time-out.
    @retval EFI_DEVICE_ERROR    Device error while waiting for clearing BSY bit

    @note  Wait for BSY bit to get clear. Check for any errors.

    @endinternal
**/
EFI_STATUS
SmmWaitForCmdCompletion (
    HDD_PASSWORD                    *pHddPassword
)
{
    UINT8           Data8;
    EFI_STATUS      Status;

    //  Read ATL_STATUS and ignore the result. Just a delay
    IoRead8(pHddPassword->DeviceControlReg);
    
    //  Check for BSY bit to be clear
    Status = SmmWaitforBitClear (   pHddPassword, 
                                    IDE_BSY, 
                                    DMA_ATAPI_COMMAND_COMPLETE_TIMEOUT);

    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    //  Check for errors. 
    Data8 = IoRead8(pHddPassword->BaseAddress + 7);

    if (Data8 & (IDE_ERR | IDE_DF)) {
        return EFI_DEVICE_ERROR;  
    }

    return EFI_SUCCESS;
}

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )

/** @internal
    This Function unlocks HDD password during S3 resume in 
    Ahci Mode Using Int 13.

    @param *pHddPassword        Pointer to HDD_PASSWORD

    @retval EFI_SUCCESS         HDD password is unlocked in Ahci mode
    @retval EFI_DEVICE_ERROR    Device error in unlocking the HDD password.
    
    @endinternal
**/
EFI_STATUS
SmmAhciSecurityUnlockCommand (
    HDD_PASSWORD                    *pHddPassword
)
{
    EFI_STATUS         Status;
    UINTN              i;
    UINT32             AhciBar = 0;
    UINT8              SccReg = 0 ;
    BOOLEAN            ControllerinAhciMode = FALSE;
    UINT32             GlobalControl;
    COMMAND_STRUCTURE  CommandStructure;
    UINT32             NoOfPorts=0;
    UINT64             FisBaseAddress=0;
    UINT64             OrgFisBaseAddress=0;
    UINT64             PortCommandListBaseAddr=0;
    EFI_PHYSICAL_ADDRESS   SecurityUnlockCommandBufferMapped=0;
    VOID               *Mapping = NULL;
    UINTN              Size = 512;
    EDKII_IOMMU_OPERATION  Operation;
    EDKII_IOMMU_PROTOCOL  *EdkiiIoMmu = NULL;
    if(mAhciSmm == NULL) {
        return EFI_NOT_FOUND;
    }

    //    1. Check if Controller is in AHCI Mode.
    //    2. Read ABAR Offset and Get HbaAddress.
    //    3. Disable AHCI_ENABLE. 
    //    4. Issue Controller Reset. 
    //    5. Wait for HBA Reset to Complete.
    //    6. Enable AHCI_ENABLE.

    AhciBar = *(UINT32*)PCI_CFG_ADDR(pHddPassword->BusNo,
                               pHddPassword->DeviceNo,
                               pHddPassword->FunctionNo,
                               AHCI_BAR);
    AhciBar &= 0xFFFFFFF0;
    
    mAhciSmm->AhciBaseAddress = AhciBar;

    // Find Number of Ports from BIT0:BIT4 of the HBA_CAP register
    NoOfPorts = ((MmioRead32(AhciBar + HBA_CAP)) & HBA_CAP_NP_MASK) + 1;

    // Check if AHCI MMIO address space resides in SMRAM region. If yes, don't proceed.
    if( AmiValidateMmioBuffer((VOID*)(UINTN)mAhciSmm->AhciBaseAddress, HBA_PORTS_START + (NoOfPorts * HBA_PORTS_REG_WIDTH) ) ) {
        return EFI_NOT_FOUND;
    }

    if(!AhciInit) {
        SccReg = *(UINT8*)PCI_CFG_ADDR(pHddPassword->BusNo,
                                       pHddPassword->DeviceNo,
                                       pHddPassword->FunctionNo,
                                       PCI_SCC);    // Get Scc Register;

        if((SccReg & 0x06) || (SccReg & 0x04)) { 
            ControllerinAhciMode = TRUE;
        }

        if(ControllerinAhciMode) {
            GlobalControl = MmioRead32(AhciBar + 0x04);

            GlobalControl &= 0x7FFFFFFF;
            MmioWrite32(AhciBar + 0x04, GlobalControl);

            GlobalControl = 0x01;
            MmioWrite32(AhciBar + 0x04, GlobalControl);

            Stall(5000);   // 5 milliSec Delay

            GlobalControl = 0x80000000;
            MmioWrite32(AhciBar + 0x04, GlobalControl);

            Stall(1000000);   // 1 Second Delay
        }
        AhciInit=TRUE;
    }

    // If unlock buffer not available, return error
    if(gSecurityUnlockCommandBuffer == NULL) {
        return EFI_UNSUPPORTED;
    }
    
    //  Clear the Buffer
    ZeroMemorySmm (gSecurityUnlockCommandBuffer, 512);
    gSecurityUnlockCommandBuffer[0] = pHddPassword->Control & 1;;
    if(gSecurityUnlockCommandBuffer[0]) {
    //Copy 32 Bytes of Password
        for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
            ((UINT8 *)gSecurityUnlockCommandBuffer)[i + 2] = pHddPassword->MasterPassword[i];
        }
    } else {
    //Copy 32 Bytes of Password
        for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
                ((UINT8 *)gSecurityUnlockCommandBuffer)[i + 2] = pHddPassword->UserPassword[i];
        }
    }
    //Locate the Edk2IommuProtocol
    Status = pSmst2->SmmLocateProtocol(
                        &gEdkiiIoMmuProtocolGuid, 
                        NULL, 
                        &EdkiiIoMmu);
    
    if (!EFI_ERROR(Status) && EdkiiIoMmu) { 
        Status = EdkiiIoMmu->AllocateBuffer(
                                       EdkiiIoMmu,
                                       0,
                                       0,
                                       EFI_SIZE_TO_PAGES(RECEIVED_FIS_SIZE + 0x100), 
                                       (VOID**)&FisBaseAddress,
                                       0);
        ASSERT_EFI_ERROR (Status);
        
        OrgFisBaseAddress = mAhciSmm->PortFISBaseAddr;
        mAhciSmm->PortFISBaseAddr =((FisBaseAddress & (~0xFF))+ 0x100);
        ZeroMemorySmm ((VOID *)mAhciSmm->PortFISBaseAddr, RECEIVED_FIS_SIZE);
        
        Status = EdkiiIoMmu->AllocateBuffer(
                                    EdkiiIoMmu,
                                    0,
                                    0,
                                    EFI_SIZE_TO_PAGES(COMMAND_LIST_SIZE_PORT * 2),
                                    (VOID**)&PortCommandListBaseAddr,
                                    0);
        ASSERT_EFI_ERROR (Status);
        mAhciSmm->PortCommandListBaseAddr = PortCommandListBaseAddr;
        mAhciSmm->PortCommandTableBaseAddr = mAhciSmm->PortCommandListBaseAddr + 0x80;
        
        ZeroMemorySmm ((VOID *)mAhciSmm->PortCommandListBaseAddr, COMMAND_LIST_SIZE_PORT);
        
        Operation = EdkiiIoMmuOperationBusMasterRead;
        Status = EdkiiIoMmu->Map(
                                EdkiiIoMmu, 
                                Operation,
                                gSecurityUnlockCommandBuffer,
                                &Size,
                                &SecurityUnlockCommandBufferMapped,
                                &Mapping);
        ASSERT_EFI_ERROR (Status);
    }
    //
    //Resuming from S3. So bring back the AHCI controller to working state
    //
    mAhciSmm->AhciSmmInitPortOnS3Resume(mAhciSmm,pHddPassword->PortNumber);
    
    //
    //Setup the Unlock command 
    //
    ZeroMemorySmm(&CommandStructure, sizeof(COMMAND_STRUCTURE));
    CommandStructure.SectorCount = 1;
    CommandStructure.LBALow = 0;
    CommandStructure.LBAMid = 0;
    CommandStructure.LBAHigh = 0;
    CommandStructure.Device = 0x40;
    CommandStructure.Command = SECURITY_UNLOCK;
    CommandStructure.Buffer = SecurityUnlockCommandBufferMapped ? (VOID *)SecurityUnlockCommandBufferMapped : gSecurityUnlockCommandBuffer;
    CommandStructure.ByteCount = 512;
    //
    //Issue the unlock command
    //
    Status = mAhciSmm->AhciSmmExecutePioDataCommand( mAhciSmm,
                                                    &CommandStructure,
                                                    pHddPassword->PortNumber,
                                                    0xFF,
                                                    ATA,
                                                    TRUE);
    DEBUG ((DEBUG_VERBOSE, "SmmHddSec: AHCI Security Unlock Status = %r", Status));
 
#if DISABLE_SOFT_SET_PREV 
    ZeroMemorySmm (&CommandStructure, sizeof(COMMAND_STRUCTURE));
    CommandStructure.Features = DISABLE_SATA2_SOFTPREV;         // Disable Software Preservation
    CommandStructure.SectorCount = 6;
    CommandStructure.Command = SET_FEATURE_COMMAND;

    mAhciSmm->AhciSmmExecuteNonDataCommand( mAhciSmm,
                                            CommandStructure,
                                            pHddPassword->PortNumber,
                                            0xFF,
                                            ATA
                                            );
#endif

    //
    //Issue the Security Freeze lock command
    //
    ZeroMemorySmm(&CommandStructure, sizeof(COMMAND_STRUCTURE)); 
    CommandStructure.Command = SECURITY_FREEZE_LOCK;
    mAhciSmm->AhciSmmExecuteNonDataCommand( mAhciSmm,
                                            CommandStructure,
                                            pHddPassword->PortNumber,
                                            0xFF,
                                            ATA
                                            );

    //
    //Issue the Device configure Freeze lock command 
    //
    ZeroMemorySmm (&CommandStructure, sizeof(COMMAND_STRUCTURE));
    CommandStructure.Command = DEV_CONFIG_FREEZE_LOCK;
    CommandStructure.Features = DEV_CONFIG_FREEZE_LOCK_FEATURES;
    mAhciSmm->AhciSmmExecuteNonDataCommand( mAhciSmm,
                                            CommandStructure,
                                            pHddPassword->PortNumber,
                                            0xFF,
                                            ATA
                                            );
    //clear the password buffer
    if (SecurityUnlockCommandBufferMapped) {
        ZeroMemorySmm ((VOID *)SecurityUnlockCommandBufferMapped, 512);
    }
    ZeroMemorySmm (gSecurityUnlockCommandBuffer, 512);

    if (EdkiiIoMmu) {
    
        if (Mapping){
            EdkiiIoMmu->Unmap(EdkiiIoMmu, Mapping);    
        }
        
        Status = EdkiiIoMmu->FreeBuffer(
                                    EdkiiIoMmu,
                                    EFI_SIZE_TO_PAGES(COMMAND_LIST_SIZE_PORT * 2),
                                    (void *)PortCommandListBaseAddr);
        ASSERT_EFI_ERROR (Status);
        
        Status = EdkiiIoMmu->FreeBuffer(
                                    EdkiiIoMmu,
                                    EFI_SIZE_TO_PAGES(RECEIVED_FIS_SIZE + 0x100),
                                    (void *)FisBaseAddress);
        ASSERT_EFI_ERROR (Status);
        
        mAhciSmm->PortFISBaseAddr = OrgFisBaseAddress;
    }
    return EFI_SUCCESS;

}
#endif

/** @internal
    This Function unlocks HDD password during S3 resume.

    @param *pHddPassword      Pointer to HDD_PASSWORD

    @retval EFI_SUCCESS         HDD password is unlocked
    @retval EFI_DEVICE_ERROR    Device error in unlocking the HDD password.

    @endinternal
**/
EFI_STATUS
SmmSecurityUnlockCommand (
    HDD_PASSWORD                    *pHddPassword
)
{
    EFI_STATUS                      Status;
    UINT8                           Data8;
    UINT8                           i;
    UINT16                          Reg;

    //  Disable Interrupt
    IoWrite8(pHddPassword->DeviceControlReg, 2);

    //  Select the drive
    IoWrite8(pHddPassword->BaseAddress + 6, pHddPassword->Device << 4);

    //  Wait for BSY to go low
    Status = SmmWaitforBitClear (pHddPassword, IDE_BSY, S3_BUSY_CLEAR_TIMEOUT);
    if (EFI_ERROR(Status)) return EFI_DEVICE_ERROR;

    //  Clear the Buffer
    ZeroMemorySmm (SecurityBuffer, 512);

    SecurityBuffer[0] = pHddPassword->Control & 1;

    if(SecurityBuffer[0]) {

        //  Copy 32 Bytes of Password
        for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
            ((UINT8 *)SecurityBuffer)[i + 2] = pHddPassword->MasterPassword[i];
        }
    } else {

        // Copy 32 Bytes of Password
        for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
                ((UINT8 *)SecurityBuffer)[i + 2] = pHddPassword->UserPassword[i];
        }
    }

    Status = SmmIdeNonDataCommand (pHddPassword, 
                                    0,
                                    0,
                                    0,
                                    0,
                                    0, 
                                    SECURITY_UNLOCK); 
    DEBUG ((DEBUG_VERBOSE, "SmmHddSec: IDE Security Unlock Status = %r", Status));
    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    //  Wait for Command completion
    Status = SmmWaitForCmdCompletion (pHddPassword);
    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    //  Check for DRQ
    Status = SmmWaitforBitSet(pHddPassword, IDE_DRQ, DRQ_TIMEOUT);
    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    Reg = pHddPassword->BaseAddress;

//  Status = IdeWriteMultipleWord (pHddPassword->BaseAddress, 256, &SecurityBuffer);
    IoWrite(
        CpuIoWidthFifoUint16,
        Reg,
        256,
        SecurityBuffer
    );

    //  Wait for Command completion
    Status = SmmWaitForCmdCompletion (pHddPassword);
    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    //  Check for errors. 
    Data8 = IoRead8 ( pHddPassword->BaseAddress + 7);
    if(Data8 & 0x21) {          // ERR OR DF bit set ?
        return EFI_DEVICE_ERROR;  
    }

#if DISABLE_SOFT_SET_PREV 
    Status = SmmIdeNonDataCommand (pHddPassword, 
                                    DISABLE_SATA2_SOFTPREV,
                                    6,
                                    0,
                                    0,
                                    0, 
                                    SET_FEATURE_COMMAND);

    if (EFI_ERROR(Status)) {
        return Status;
    }
    //  Check for errors
    Status = SmmWaitForCmdCompletion (pHddPassword);
    if (EFI_ERROR(Status)) {
        return Status;
    }
#endif

    Status = SmmIdeNonDataCommand (pHddPassword, 
                              DEV_CONFIG_FREEZE_LOCK_FEATURES,
                              0,
                              0,
                              0,
                              0, 
                              DEV_CONFIG_FREEZE_LOCK);
	ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
        return Status;
    }

     // Wait for Command completion
    Status = SmmWaitForCmdCompletion (pHddPassword);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    return EFI_SUCCESS;
}

/** @internal
    Issues Set Feature command (Non Data)

    @param *pHddPassword       Pointer to HDD_PASSWORD
    @param Features            Hdd commands written to the given port.
    @param SectorCount         Command structure written to the given port.
    @param LBALow              Location of Block of data written to the given port.
    @param LBAMid              Location of Block of data written to the given port.
    @param LBAHigh             Location of Block of data written to the given port.
    @param Command             Security commands written to the given port.

    @retval EFI_SUCCESS             The command issued successfully.
    @retval EFI_DEVICE_ERROR        Device error when checking for controller presence.
    @retval EFI_TIMEOUT             Time to wait for BSY and DRQ bit has crossed the required time.

    @endinternal
**/
EFI_STATUS 
SmmIdeNonDataCommand (
    IN  HDD_PASSWORD                     *pHddPassword,
    IN  UINT8                            Features,
    IN  UINT8                            SectorCount,
    IN  UINT8                            LBALow,
    IN  UINT8                            LBAMid,
    IN  UINT8                            LBAHigh,
    IN  UINT8                            Command
) 
{

    EFI_STATUS                      Status;
    UINT8                           Data8;

    //  Select the drive
    IoWrite8(pHddPassword->BaseAddress + 6, pHddPassword->Device << 4);

    //  Check for Controller presence
    Data8 = IoRead8(pHddPassword->DeviceControlReg);
    if (Data8 == 0xFF) {
        ASSERT(Data8 != 0xFF);
        return EFI_DEVICE_ERROR;
    }

    //  Before Writing to Sector Count Register, BSY and DRQ bit should be zero
    Status = SmmWaitforBitClear(pHddPassword, IDE_BSY, S3_BUSY_CLEAR_TIMEOUT);
    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    //  Check for DRDY
    Status = SmmWaitforBitSet(pHddPassword, IDE_DRDY, DRDY_TIMEOUT);
    if (EFI_ERROR(Status)) {
        return EFI_TIMEOUT;
    }

    //  Issue command
    IoWrite8 ( pHddPassword->BaseAddress + 1, Features);
    IoWrite8 ( pHddPassword->BaseAddress + 2, SectorCount);
    IoWrite8 ( pHddPassword->BaseAddress + 3, LBALow);
    IoWrite8 ( pHddPassword->BaseAddress + 4, LBAMid);
    IoWrite8 ( pHddPassword->BaseAddress + 5, LBAHigh);
    IoWrite8 ( pHddPassword->BaseAddress + 7, Command);

    return EFI_SUCCESS;

}
/** @internal
    This function Locates the AhciSmm protocol from the Smm

    @param *VariableGuid         Pointer to GUID value that uniquely identifies 
                                    the system configuration table.
    @param **VariablePointer     Pointer to the table associated with VariableGuid.

    @retval EFI_SUCCESS          AhciSmm protocol is located.
    @retval EFI_NOT_FOUND        AhciSmm protocol is not found.

    @endinternal
**/
EFI_STATUS
LocateSmmServices (
    IN  EFI_GUID    *VariableGuid,
    IN  VOID        **VariablePointer
)
{
    UINTN           Index;


    for (Index = 0; Index < pSmst2->NumberOfTableEntries; ++Index) {
        if (guidcmp(&pSmst2->SmmConfigurationTable[Index].VendorGuid,VariableGuid) == 0) {
            break;
        }
    }

    if (Index != pSmst2->NumberOfTableEntries) {
        *VariablePointer = pSmst2->SmmConfigurationTable[Index].VendorTable;
        return EFI_SUCCESS;
    }

    return EFI_NOT_FOUND;
}

/** @internal
    This function Locates the protocols and saves in global pointer

    @param VOID

    @return VOID

    @endinternal
**/
VOID 
LocateIdeSmmProtocols()
{
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if(mAhciSmm == NULL) {
        LocateSmmServices(&gAmiAhciSmmProtocolGuid,(VOID **)&mAhciSmm);
    }
#endif
 
    return ;  
}

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
/** @internal
    This function Locates the protocols to access Nvme controller and saves in global pointer

    @param NvmeControllerHandle       Nvme Controller Handle
    @param **SmmNvmeController        Pointer to AMI_NVME_CONTROLLER_PROTOCOL

    @retval EFI_SUCCESS              The required protocol is located.
    @retval EFI_NOT_FOUND            The required protocol is not found.
    @retval EFI_BUFFER_TOO_SMALL     The buffer is not sufficient to hold the data.

    @endinternal
**/
EFI_STATUS 
LocateNvmeSmmProtocols(
    EFI_HANDLE                    NvmeControllerHandle,
    AMI_NVME_CONTROLLER_PROTOCOL  **SmmNvmeController
)
{
    EFI_STATUS  Status=EFI_NOT_FOUND;
    EFI_HANDLE  *HandleBuffer = NULL;
    UINTN       Index, HandleBufferSize = 0;

    if(gSmmNvmePassThru == NULL) {
        Status = pSmst2->SmmLocateProtocol(&gAmiSmmNvmePassThruProtocolGuid, NULL, (VOID **)&gSmmNvmePassThru);
        if(EFI_ERROR(Status)) {
            return Status;
        }
    }

    Status = pSmst2->SmmLocateHandle( ByProtocol, 
                                      &gAmiSmmNvmeControllerProtocolGuid,
                                      NULL,
                                      &HandleBufferSize, 
                                      HandleBuffer );
        
    if( EFI_ERROR(Status) ) {
        if( Status == EFI_BUFFER_TOO_SMALL ) {
            Status = pSmst2->SmmAllocatePool( EfiRuntimeServicesCode,
                                              HandleBufferSize,
                                              (VOID**)&HandleBuffer );
            if( EFI_ERROR(Status) ) {
                ASSERT(FALSE);
                return Status;
            }

            Status = pSmst2->SmmLocateHandle( ByProtocol, 
                                              &gAmiSmmNvmeControllerProtocolGuid,
                                              NULL,
                                              &HandleBufferSize, 
                                              HandleBuffer );
        }
    }
    
    if(HandleBuffer == NULL) {
        return EFI_NOT_FOUND;
    }

    if( !EFI_ERROR(Status) ) {
        for(Index=0; Index<HandleBufferSize/sizeof(EFI_HANDLE); Index++) {
            Status = pSmst2->SmmHandleProtocol( HandleBuffer[Index], 
                                                &gAmiSmmNvmeControllerProtocolGuid, 
                                                (VOID **)SmmNvmeController );
                
            if( !EFI_ERROR(Status) && (*SmmNvmeController)->ControllerHandle == NvmeControllerHandle ) {
                Status = EFI_SUCCESS;
                break;
            }
        }

        if( Index == HandleBufferSize/sizeof(EFI_HANDLE) ) {
            Status = EFI_NOT_FOUND;
        }
    }

    // Free HandleBuffer memory
    if( HandleBuffer ) {
        pSmst2->SmmFreePool(HandleBuffer);
    }

    return Status;
}
#endif

/** 
    This function saves the HDD password and other information 
    necessary to unlock HDD password during S3 Resume.

    @param DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
    @param Context         Points to an optional handler context which was specified when the
                           handler was registered.
    @param CommBuffer      A pointer to a collection of data in memory that will
                           be conveyed from a non-SMM environment into an SMM environment.
    @param CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS      Command is handled successfully.

**/
EFI_STATUS 
EFIAPI
SaveHddPassword (
    IN  EFI_HANDLE      DispatchHandle,
    IN  CONST   VOID    *DispatchContext OPTIONAL,
    IN  OUT VOID        *CommBuffer      OPTIONAL,
    IN  OUT UINTN       *CommBufferSize  OPTIONAL
)
{

    HDD_PASSWORD    *pHddPassword, *StoredHddList;
    DLINK           *dlink;
    UINT8           i;
    BOOLEAN         UserOrMaster = FALSE;
    BOOLEAN         CheckFlag;
    EFI_STATUS      Status=EFI_SUCCESS;
    UINT64          HddSecSmiUnRegSignature =  0;

    LocateIdeSmmProtocols();
    
    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL) {
        return EFI_SUCCESS;
    }
    // Validate if CommBuffer address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)CommBuffer, sizeof(UINT64));
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\n  SaveHddPassword() CommBuffer overlaps with SMRAM region" ));
        return EFI_SUCCESS;
    }
	
    // Check whether this Communicate call is initiated to UnRegister this SMI handler
    HddSecSmiUnRegSignature = *(UINT64  *)CommBuffer;
        
    if (HddSecSmiUnRegSignature == HDD_SEC_SMI_UNREGISTER_SIGNATURE) {
        Status = pSmst2->SmiHandlerUnRegister(DispatchHandle);
        ASSERT_EFI_ERROR(Status);
        return EFI_SUCCESS;
    }
    
    //  This Communicate call is initiated to Save password data to SMRAM
    //  CommBuffer contains HDD_PASSWORD buffer and password buffer
    if(*CommBufferSize != (sizeof(HDD_PASSWORD)+ ( 2 * IDE_PASSWORD_LENGTH ))) {
        return EFI_SUCCESS;
    }

    pHddPassword = (HDD_PASSWORD *)CommBuffer;

    //  Check if signature is present or not
    if ((UINT32) pHddPassword->Signature != HDD_SEC_SIGNATURE) {
        return Status;
    }
    
    // Validate if pHddPassword address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)pHddPassword, sizeof(HDD_PASSWORD));
    if (EFI_ERROR(Status))  {
         DEBUG((DEBUG_VERBOSE,"\n SaveHddPassword() CommBuffer overlaps with SMRAM region" ));
         return EFI_SUCCESS;
    }
    //  Check whether info about the HDD is already present
    dlink = gPasswordList.pHead;
    for ( ; dlink; dlink = dlink->pNext) {
        StoredHddList = OUTTER(dlink, LinkList, HDD_PASSWORD);
        CheckFlag = FALSE;

        // Check if drive is connected to same controller
        if ((StoredHddList->BusNo      != pHddPassword->BusNo   ) ||
            (StoredHddList->DeviceNo   != pHddPassword->DeviceNo) ||
            (StoredHddList->FunctionNo != pHddPassword->FunctionNo)) {
            continue;
        }

        if(StoredHddList->BusInterfaceType == AmiStorageInterfaceAhci) {
            if ((StoredHddList->PortNumber   == pHddPassword->PortNumber) && 
                (StoredHddList->PMPortNumber == pHddPassword->PMPortNumber)) {
                CheckFlag = TRUE;
            }
        } else if(StoredHddList->BusInterfaceType == AmiStorageInterfaceIde ) {
            if ((StoredHddList->BaseAddress == pHddPassword->BaseAddress) && 
                (StoredHddList->Device      == pHddPassword->Device)) {
                CheckFlag = TRUE;
            }
        } else { // i.e. StoredHddList->BusInterfaceType == AmiStorageInterfaceNvme
            if ( StoredHddList->DevHandle == pHddPassword->DevHandle ) {
                CheckFlag = TRUE;
            }
        }

        if (CheckFlag) {
            UserOrMaster = (BOOLEAN)((pHddPassword->Control) & 0x01);
            // Validate Password buffer to avoid SMRAM corruption
            if(UserOrMaster){
                Status = AmiValidateMemoryBuffer((VOID*)pHddPassword->MasterPassword, IDE_PASSWORD_LENGTH);
                if (EFI_ERROR(Status))  {
                    DEBUG((DEBUG_VERBOSE,"\n MasterPassword Buffer overlaps with SMRAM region" ));
                    return EFI_SUCCESS;
                }
                // Match has been found. Just copy the Password buffer
                for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
                    StoredHddList->MasterPassword[i] = pHddPassword->MasterPassword[i];
                }
         
            } else {
                Status = AmiValidateMemoryBuffer((VOID*)pHddPassword->UserPassword, IDE_PASSWORD_LENGTH);
                if (EFI_ERROR(Status))  {
                    DEBUG((DEBUG_VERBOSE,"\n UserPassword Buffer overlaps with SMRAM region" ));
                    return EFI_SUCCESS;
                }
                // Match has been found. Just copy the Password buffer
                for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
                    StoredHddList->UserPassword[i] = pHddPassword->UserPassword[i];
                }
            }
            return Status;
        }
    }

    //  Allocate memory needed while unlocking the Password. Done only once. 
    //  Same buffer will be reused.
    if (SecurityBuffer == NULL) { 
        pSmst2->SmmAllocatePool(EfiRuntimeServicesData, 512, (VOID**)&SecurityBuffer);
    }

    //  Match has not been found. Allocate memory and copy the buffer.
    if (pSmst2->SmmAllocatePool(EfiRuntimeServicesData, sizeof(HDD_PASSWORD), (VOID**)&StoredHddList) != EFI_SUCCESS) { 
        return Status;
    }
    
    for (i = 0; i < sizeof (HDD_PASSWORD); i++) {
        ((UINT8 *)StoredHddList)[i] = ((UINT8 *)pHddPassword)[i];
    }
    // Validate password buffer and save the password to allocated Confidential memory
    if ( (StoredHddList->Control) & 0x01) {
        Status = AmiValidateMemoryBuffer((VOID*)pHddPassword->MasterPassword, IDE_PASSWORD_LENGTH);
        if (EFI_ERROR(Status))  {
            DEBUG((DEBUG_VERBOSE,"\n MasterPassword Buffer overlaps with SMRAM region" ));
            return EFI_SUCCESS;
        }
        Status = SmmAllocateConfidentialPool(IDE_PASSWORD_LENGTH, (void**)(&StoredHddList->MasterPassword));
        if (EFI_ERROR(Status) || (StoredHddList->MasterPassword == NULL)) {
            return Status;
        }
        CopyMem (StoredHddList->MasterPassword, pHddPassword->MasterPassword, IDE_PASSWORD_LENGTH);
    } else {
        Status = AmiValidateMemoryBuffer((VOID*)pHddPassword->UserPassword, IDE_PASSWORD_LENGTH);
        if (EFI_ERROR(Status))  {
            DEBUG((DEBUG_VERBOSE,"\n UserPassword Buffer overlaps with SMRAM region" ));
            return EFI_SUCCESS;
        }
        Status = SmmAllocateConfidentialPool(IDE_PASSWORD_LENGTH, (void**)(&StoredHddList->UserPassword));
        if (EFI_ERROR(Status) || (StoredHddList->UserPassword == NULL)) {
            return Status;
        }
        CopyMem (StoredHddList->UserPassword, pHddPassword->UserPassword, IDE_PASSWORD_LENGTH);
    }
    DListAdd(&gPasswordList, &StoredHddList->LinkList); 

    return Status;
}

/** @internal
    This Function unlocks HDD password during S3 resume.

    @param DispatchHandle  Handle to the Dispatcher
    @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT.
    @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
    @param CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS Command is handled successfully.
  
  @endinternal

**/
EFI_STATUS 
EFIAPI
UnlockHddPassword (
    IN  EFI_HANDLE       DispatchHandle,
    IN  CONST VOID       *DispatchContext OPTIONAL,
    IN  OUT VOID         *CommBuffer OPTIONAL,
    IN  OUT UINTN        *CommBufferSize OPTIONAL
)
{
    HDD_PASSWORD    *StoredHddList;
    DLINK           *dlink;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    HDD_PASSWORD    *StoredHddList2;
    DLINK           *dlink2;
#endif    
    EFI_STATUS      Status=EFI_SUCCESS;

    dlink = gPasswordList.pHead;
    for ( ; dlink; dlink = dlink->pNext) {
        StoredHddList = OUTTER(dlink, LinkList, HDD_PASSWORD);
        if (StoredHddList->Signature == HDD_SEC_SIGNATURE) {
            if(StoredHddList->BusInterfaceType == AmiStorageInterfaceIde ) {
                Status = SmmSecurityUnlockCommand (StoredHddList);
                DEBUG ((DEBUG_VERBOSE, "SmmHddSec: HDD (IDE Mode) Unlock Status = %r", Status));
            } else if (StoredHddList->BusInterfaceType == AmiStorageInterfaceAhci ){
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
                // Check if this is the first entry for this controller.
                // If yes, we have to make sure the controller will be fully re-initialized
                AhciInit = FALSE;
                for (dlink2 = dlink->pPrev; dlink2; dlink2 = dlink2->pPrev) {
                    StoredHddList2 = OUTTER(dlink2, LinkList, HDD_PASSWORD);
                    if ((StoredHddList2->BusNo      == StoredHddList->BusNo   ) &&
                        (StoredHddList2->DeviceNo   == StoredHddList->DeviceNo) &&
                        (StoredHddList2->FunctionNo == StoredHddList->FunctionNo)) {
                        // there was another one previously, so initialization is already done
                        AhciInit = TRUE;
                        break;
                    }
                }
                Status = SmmAhciSecurityUnlockCommand (StoredHddList);
                DEBUG ((DEBUG_VERBOSE, "SmmHddSec: HDD (AHCI Mode) Unlock Status = %r", Status));
#endif
            } else if(StoredHddList->BusInterfaceType == AmiStorageInterfaceNvme) {
                if(StoredHddList->RstControlled == TRUE){
#if NvmeRstPassword_SUPPORT                
                  Status = RstSmmUnlockNvmeDevice(StoredHddList);                    
#endif               
                }else{
                  Status = SmmUnlockNvmeDevice(StoredHddList);
                  
                }
                
                DEBUG ((DEBUG_VERBOSE, "SmmHddSec: NVMe device Unlock Status = %r", Status));
            }
        }
    }

    return Status;
}

/** @internal
    This Function removes HDD password from the internal database

    @param DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
    @param Context         Points to an optional handler context which was specified when the
                            handler was registered.
    @param CommBuffer      A pointer to a collection of data in memory that will
                            be conveyed from a non-SMM environment into an SMM environment.
    @param CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS Command is handled successfully.
  
  @endinternal

**/
EFI_STATUS
EFIAPI
RemoveHddPassword (
    IN  EFI_HANDLE      DispatchHandle,
    IN  CONST VOID      *DispatchContext OPTIONAL,
    IN  OUT VOID        *CommBuffer      OPTIONAL,
    IN  OUT UINTN       *CommBufferSize  OPTIONAL
)
{
    HDD_PASSWORD    *pHddPassword, *StoredHddList;
    DLINK           *dlink;
    EFI_STATUS      Status=EFI_SUCCESS;
    UINT64          HddSecSmiUnRegSignature =  0;

    LocateIdeSmmProtocols();
    
    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL) {
        return EFI_SUCCESS;
    }
    // Validate if CommBuffer address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)CommBuffer, sizeof(UINT64));
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\n RemoveHddPassword() CommBuffer overlaps with SMRAM region" ));
        return EFI_SUCCESS;
    }
	
    // Check whether this Communicate call is initiated to UnRegister this SMI handler
    HddSecSmiUnRegSignature = *(UINT64  *)CommBuffer;
        
    if (HddSecSmiUnRegSignature == HDD_SEC_SMI_UNREGISTER_SIGNATURE) {
        Status = pSmst2->SmiHandlerUnRegister(DispatchHandle);
        ASSERT_EFI_ERROR(Status);
        return EFI_SUCCESS;
    }
    
    //  This Communicate call is initiated to Save password data to SMRAM
    if(*CommBufferSize != (sizeof(HDD_PASSWORD)+( 2 * IDE_PASSWORD_LENGTH ))) {
        return EFI_SUCCESS;
    }
   
    pHddPassword = (HDD_PASSWORD *)CommBuffer;

    //  Check if signature is present or not
    if ((UINT32) pHddPassword->Signature != HDD_SEC_SIGNATURE) {
        return Status;
    }
    
    // Validate if pHddPassword address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)pHddPassword, sizeof(HDD_PASSWORD));
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\n RemoveHDDPassword() CommBuffer overlaps with SMRAM region" ));
        return EFI_SUCCESS;
    }
    
    //  Check whether info about the HDD is already present
    dlink = gPasswordList.pHead;
    for ( ; dlink; dlink = dlink->pNext) {
        StoredHddList = OUTTER(dlink, LinkList, HDD_PASSWORD);

        // Check if drive is connected to same controller
        if ((StoredHddList->BusNo      != pHddPassword->BusNo   ) ||
            (StoredHddList->DeviceNo   != pHddPassword->DeviceNo) ||
            (StoredHddList->FunctionNo != pHddPassword->FunctionNo)) {
            continue;
        }
        // Freepool the allocated Confidential memory if match found
        if(StoredHddList->BusInterfaceType == AmiStorageInterfaceAhci) {
            if ((StoredHddList->PortNumber   == pHddPassword->PortNumber) && 
                (StoredHddList->PMPortNumber == pHddPassword->PMPortNumber)) {
                if(pHddPassword->Control & 0x01){
                    if(StoredHddList->MasterPassword)SmmFreeConfidentialPool((void**)&StoredHddList->MasterPassword);
                } else {
                    if(StoredHddList->UserPassword)SmmFreeConfidentialPool((void**)&StoredHddList->UserPassword);
                }
                DListDelete(&gPasswordList, &StoredHddList->LinkList);
            }
        } else if(StoredHddList->BusInterfaceType == AmiStorageInterfaceIde) {
            if ((StoredHddList->BaseAddress == pHddPassword->BaseAddress) && 
                (StoredHddList->Device      == pHddPassword->Device)) {
                if(pHddPassword->Control & 0x01){
                    if(StoredHddList->MasterPassword)SmmFreeConfidentialPool((void**)&StoredHddList->MasterPassword);
                } else {
                    if(StoredHddList->UserPassword)SmmFreeConfidentialPool((void**)&StoredHddList->UserPassword);
                }
                DListDelete(&gPasswordList, &StoredHddList->LinkList);
            }
        } else {
            if ( StoredHddList->DevHandle == pHddPassword->DevHandle ) {
                if(pHddPassword->Control & 0x01){
                    if(StoredHddList->MasterPassword)SmmFreeConfidentialPool((void**)&StoredHddList->MasterPassword);
                } else {
                    if(StoredHddList->UserPassword)SmmFreeConfidentialPool((void**)&StoredHddList->UserPassword);
                }
                DListDelete(&gPasswordList, &StoredHddList->LinkList);
            }
        }
    }
    return Status;
}

/**
    This Function Saves SMI value and SW SMI Port Address 
    in Boot Script for S3 resume during ReadytoBoot

    @param DispatchHandle  Handle to the Dispatcher
    @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT.
    @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
    @param CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS Command is handled successfully.

**/
EFI_STATUS
EFIAPI
BootScriptSaveHddSecSmmEventForS3 (
    IN  EFI_HANDLE       DispatchHandle,
    IN  CONST VOID       *DispatchContext OPTIONAL,
    IN  OUT VOID         *CommBuffer OPTIONAL,
    IN  OUT UINTN        *CommBufferSize OPTIONAL
)
{
    EFI_STATUS  Status;
    UINT8       Value = SW_SMI_HDD_UNLOCK_PASSWORD;

    S3BootScriptSaveIoWrite (
        S3BootScriptWidthUint8,
        (UINTN)SwSmiPort,
        1,
        &Value
        );
    
    // UnRegister SMI handler as we have saved SW SMI port & value to the boot script
    Status = pSmst->SmiHandlerUnRegister (DispatchHandle);
    ASSERT_EFI_ERROR(Status);

    return EFI_SUCCESS;
}

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
/** @internal
    This function registers Boot script for Nvme devices to initialize Nvme 
    controller along with PCI root bridge BAR,CMD, BUS PCI Configuration 
    values in S3 resume path

    @param DispatchHandle  Handle to the Dispatcher
    @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT.
    @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
    @param CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS Command is handled successfully.
  
  @endinternal

**/
EFI_STATUS 
EFIAPI
BootScriptSaveNvmeSecSmmEventForS3 (
    IN EFI_HANDLE       DispatchHandle,
    IN CONST VOID       *DispatchContext OPTIONAL,
    IN OUT VOID         *CommBuffer OPTIONAL,
    IN OUT UINTN        *CommBufferSize OPTIONAL
 )
{
    EFI_STATUS                           Status;
    UINT32                               SmmNvmeSaveEntryCount = 0;
    UINTN                               NvmeBootScriptSaveBufferSize = 0;
    UINT32                               EntryIndex = 0;
    UINTN                                PciAddress = 0;
    EFI_BOOT_SCRIPT_WIDTH                Width  = 0;
    UINTN                                Value = 0;
    BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY *NvmeBootScriptSaveBuffer = NULL;
    
    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL) {
        return EFI_SUCCESS;
    }
    
    NvmeBootScriptSaveBufferSize = *CommBufferSize;
	
    // Confirm that communication buffer contains required data
    NvmeBootScriptSaveBuffer = (BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY *)CommBuffer;
    
    // Make sure that "NvmeBootScriptSaveBufferSize" should be greater than "2*sizeof(UINT32)" size
    if( (NvmeBootScriptSaveBufferSize <= 2*sizeof(UINT32)) ) {
        DEBUG((DEBUG_VERBOSE,"\n BootScriptSaveNvmeSecSmmEventForS3() CommBufferSize is invalid" ));
        return EFI_SUCCESS;
    }
    
    Status = AmiValidateMemoryBuffer((VOID*)NvmeBootScriptSaveBuffer, NvmeBootScriptSaveBufferSize);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\n BootScriptSaveNvmeSecSmmEventForS3() CommBuffer overlaps with SMRAM region" ));
       return EFI_SUCCESS;
    }
    
    SmmNvmeSaveEntryCount = *(UINT32*)(((UINT8*)NvmeBootScriptSaveBuffer + NvmeBootScriptSaveBufferSize) - sizeof(UINT32));

    // "SmmNvmeSaveEntryCount" value should be greater than 1 as it is known that 
    // more than one Bootscript entry for NVMe device must have been added by HddSecurity DXE driver
    if(SmmNvmeSaveEntryCount==0) {
        DEBUG((DEBUG_VERBOSE,"\n Bootscript entry is 0 for NVMe device" ));
        return EFI_SUCCESS;
    }
    
    Status = AmiValidateMemoryBuffer((VOID*)NvmeBootScriptSaveBuffer,\
            SmmNvmeSaveEntryCount * sizeof(BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY) + 2*sizeof(UINT32) );
    if (EFI_ERROR(Status)) {
    	DEBUG((DEBUG_VERBOSE,"\n CommBuffer passed with NVMe BootScript overlaps with SMRAM space. Either CommBuffer or CommBufferSize invalid. " ));
        return EFI_SUCCESS;
    }

    // Check for Signature at the End of "NvmeBootScriptSaveBuffer" CommBuffer
    if( *(UINT32*)(&NvmeBootScriptSaveBuffer[SmmNvmeSaveEntryCount]) != NVME_SECURITY_SAVE_SIGNATURE ) {
        DEBUG((DEBUG_VERBOSE,"\n CommBuffer passed with NVMe BootScript doesn't have valid signature" ));
        return EFI_SUCCESS; 
    }

    // Parse the NvmeBootScriptSaveBuffer for the Entry List and save the values to the BootScript.
    for( EntryIndex = 0; EntryIndex < SmmNvmeSaveEntryCount; EntryIndex++ ) {
        PciAddress = NvmeBootScriptSaveBuffer[EntryIndex].PciAddress;
        Width = NvmeBootScriptSaveBuffer[EntryIndex].Width;
        Value = NvmeBootScriptSaveBuffer[EntryIndex].Value;

        DEBUG((DEBUG_VERBOSE, "SmmHddSec: BootScriptSaveNvmeSecSmmEventForS3... EntryIndex:%x PciAddress:%x Width:%x Value:%x\n",
                              EntryIndex, PciAddress, Width, Value ));

        S3BootScriptSavePciCfgWrite (
                  Width,
                  PciAddress,
                  1,
                  &Value
                  );
    }
    
    // UnRegister Smi handler as we have saved the values to boot script
    Status = pSmst->SmiHandlerUnRegister (DispatchHandle);
    ASSERT_EFI_ERROR(Status);

    return EFI_SUCCESS;
}
#endif

/** @internal
    This function Locates the AhciSMM driver and save pointer globally and stores the
    buffer which will be used for unlocking the HDD

    @param DispatchHandle  Handle to the Dispatcher
    @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT. 
    @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
    @param CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS              Command is handled successfully.
    @retval EFI_INVALID_PARAMETER    The given parameter is invalid.
	
    @endinternal

**/
EFI_STATUS 
EFIAPI
LocateAhciSmmProtocol (
    IN EFI_HANDLE       DispatchHandle,
    IN CONST VOID       *DispatchContext OPTIONAL,
    IN OUT VOID         *CommBuffer OPTIONAL,
    IN OUT UINTN        *CommBufferSize OPTIONAL
 )
{
    
    EFI_STATUS Status;
    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    
    if(gSecurityUnlockCommandBuffer == NULL) {
        
        // Unlock command Buffer
        gSecurityUnlockCommandBuffer = (UINT8 *)(*(UINTN *)CommBuffer);
    
        // Validate the gSecurityUnlockCommandBuffer not resides in SMRAM region
        Status = AmiValidateMemoryBuffer((VOID*)gSecurityUnlockCommandBuffer, 512);
        if(EFI_ERROR(Status)) {
            gSecurityUnlockCommandBuffer = NULL;
            DEBUG((DEBUG_VERBOSE,"\n gSecurityUnlockCommandBuffer overlaps with SMRAM region" ));
            return EFI_SUCCESS;
        }
    }   
    
#endif
    
    LocateIdeSmmProtocols();
    
    // UnRegister Smi handler as we have located the protocol
    Status = pSmst->SmiHandlerUnRegister (DispatchHandle);
    ASSERT_EFI_ERROR(Status);

    return EFI_SUCCESS;
}


/** @internal
    Called from InstallSmiHandler

    @param ImageHandle          Image handle
    @param *SystemTable         Pointer to EFI_SYSTEM_TABLE

    @retval EFI_SUCCESS            SMI handler added successfully.
    @retval EFI_INVALID_PARAMETER  Handler is NULL or DispatchHandle is NULL. 

    @endinternal
**/
EFI_STATUS
InSmmFunction (
    IN  EFI_HANDLE       ImageHandle,
    IN  EFI_SYSTEM_TABLE *SystemTable
 )
{
    EFI_SMM_SW_REGISTER_CONTEXT Unlock_Hdd_Password = {SW_SMI_HDD_UNLOCK_PASSWORD};
	
    EFI_STATUS  Status;
    EFI_HANDLE  Handle;

    Status = pSmst2->SmiHandlerRegister(
                        SaveHddPassword,
                        &gSaveHddPasswordGuid,
                        &Handle
                        );
	
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;	
    }

    Status = pSmst2->SmiHandlerRegister(
                        RemoveHddPassword,
                        &gRemoveHddPasswordGuid,
                        &Handle
                        );

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;	
    }

    Status = SwDispatch->Register(
                        SwDispatch,
                        UnlockHddPassword,
                        &Unlock_Hdd_Password,
                        &Handle
                        );
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;	
    }
 
    Status = pSmst2->SmiHandlerRegister(
                                BootScriptSaveHddSecSmmEventForS3,
                                &gHddSecurityBootScriptSaveTriggerGuid,
                                &Handle
                                );
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    
    Status = pSmst2->SmiHandlerRegister(
                           BootScriptSaveNvmeSecSmmEventForS3,
                          &gNvmeSecurityBootScriptSaveTriggerGuid,
                          &Handle
                          );
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }
#endif

    Status = pSmst2->SmiHandlerRegister(
                    	LocateAhciSmmProtocol,
                    	&gLocateIdeAhciSmmProtocolGuid,
                    	&Handle
                    	);
    
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    return EFI_SUCCESS;
}


/** 
    Initializes HDD Password SMM Drivers.

    @param ImageHandle      Image handle
    @param *SystemTable     Pointer to EFI_SYSTEM_TABLE

    @retval EFI_STATUS     

    @note Here is the control flow of this function

**/

EFI_STATUS
EFIAPI
HddPasswordSmmInit (
    IN  EFI_HANDLE       ImageHandle,
    IN  EFI_SYSTEM_TABLE *SystemTable
 )
{
    EFI_STATUS              Status;
    EFI_SMM_BASE2_PROTOCOL  *gSmmBase2;

    InitAmiSmmLib(ImageHandle,SystemTable);
    
    Status = pBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&gSmmBase2);
    
    if (EFI_ERROR(Status)) { 
        return Status;
    }

    Status = gSmmBase2->GetSmstLocation( gSmmBase2, &pSmst2);
    
    if (EFI_ERROR(Status)) {  
        return EFI_UNSUPPORTED;
    }

    Status  = pSmst2->SmmLocateProtocol( &gEfiSmmSwDispatch2ProtocolGuid, \
                                          NULL, \
                                          (VOID**)&SwDispatch );
    
    if (EFI_ERROR(Status)) {  
        return EFI_UNSUPPORTED;
    }
    

    DListInit(&gPasswordList);
    
    PciExpressBaseAddress = PcdGet64 (PcdPciExpressBaseAddress);
    
    return (InitSmmHandler (ImageHandle, SystemTable, InSmmFunction, NULL));
}

/** @internal
    Routine to Unlock Nvme controller

    @param pHddPassword       Pointer to HDD_PASSWORD

    @retval EFI_SUCCESS           Nvme Controller is unlocked successfully
    @retval EFI_DEVICE_ERROR      Failed to Unlock the Nvme Controller.

    @note 
        1. Locate the protocols to access Nvme controller.
        2. Initialize Nvme in S3 path.
        3. Check if the device is locked. if yes unlock the device.
    @endinternal
**/
EFI_STATUS
SmmUnlockNvmeDevice (
    HDD_PASSWORD    *pHddPassword
)
{
    EFI_STATUS                    Status=EFI_SUCCESS;
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    UINT16                        SecurityStatus;
    AMI_NVME_CONTROLLER_PROTOCOL  *SmmNvmeController=NULL;
    UINT64                        NvmeBarOffset = 0;

    if( EFI_ERROR( LocateNvmeSmmProtocols( pHddPassword->DevHandle, &SmmNvmeController ) )) {
        return EFI_DEVICE_ERROR;
    }
    
    // Check if the controller is already running. If yes check the Admin Completion Queue base address.
    NvmeBarOffset = SmmNvmeController->NvmeBarOffset;
    if (MMIO_READ32(NvmeBarOffset + Offset_CC) & 0x1) {
        if (MMIO_READ32(NvmeBarOffset + Offset_CSTS) & 0x1) {
            // If Admin Completion Queue base address is not the same as the one AMI NVME bus driver loaded 
            // we should not unlock the device.
            // This is to prevent device unlocking, if SMI is invoked from a source other than during S3 resume.
            if( MMIO_READ64(NvmeBarOffset + Offset_Acq) != 
                    SmmNvmeController->AdminCompletionQueueMappedAddr) {
                DEBUG((DEBUG_VERBOSE,"\n Unlock Password can't be proceeded !! Admin Completion Queue"));
                DEBUG((DEBUG_VERBOSE," Address is not matched with Aptio NVMe driver assigned value"));
                return EFI_UNSUPPORTED;
            }
        }
        
    }

    Status = SmmNvmeController->NvmeInitController(SmmNvmeController);

    if(EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    Status = GetNvmeDeviceSecurityStatus(
                SmmNvmeController,
                &SecurityStatus );

    // if Nvme device is locked, send Unlock command
    if( !EFI_ERROR(Status) && ( SecurityStatus & NVME_SECURITY_LOCK_MASK ) ) {
        Status = SendNvmeSecurityUnlockCommand (
                 SmmNvmeController,
                 pHddPassword
                 );
        
    }
    DEBUG ((DEBUG_VERBOSE, "SmmHddSec: Nvme Security Unlock Status = %r", Status));
    // Function used to stop/release resources in S3 path
    Status = SmmNvmeController->NvmeStopController(SmmNvmeController);
#endif
    return Status;
}
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
/** @internal
    Function SmmNvmePassThruReturnSecurityStatus

    @param SecurityStatus

    @retval EFI_STATUS
    @endinternal
**/
EFI_STATUS
EFIAPI
SmmNvmePassThruReturnSecurityStatus (
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *StorageSecurityProtocol,
  UINT16                                  *SecurityStatus    
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;        
    NVME_RST_SECURITY_RECEIVE_BUFFER    SecReceiveBuffer;    
    UINT16                          Data16 = 0;
    UINTN                           SecReceiveBufferSize = sizeof(NVME_RST_SECURITY_RECEIVE_BUFFER);

    if(StorageSecurityProtocol == NULL) return EFI_UNSUPPORTED;
    
    ZeroMemorySmm (&SecReceiveBuffer, NVME_SECURITY_RECEIVE_BUFFER_SIZE);

    Status = StorageSecurityProtocol->ReceiveData( StorageSecurityProtocol,
                                                   0,                   // Media ID
                                                   NVME_COMMAND_TIMEOUT * 100000000,      // 10sec timeout
                                                   NVME_SECUIRTY_SECP,                    // Security Protocol ID
                                                   0x00,                                  // SP specific id
                                                   sizeof(NVME_RST_SECURITY_RECEIVE_BUFFER),  // Transfer Length
                                                   &SecReceiveBuffer,
                                                   &SecReceiveBufferSize                  // Transferred Length
                                                  );    
    
    Data16 = SecReceiveBuffer.NvmeSecStatus >> 8;        
    Data16 |= SecReceiveBuffer.NvmeSecStatus << 8;
    
    *SecurityStatus = Data16;
                    
    return Status;
}
#endif
/**
    Function converts Big endian Qword to Little Endian Qword and Vice versa.

    @param EndianDword
    
    @retval UINT64
**/
UINT16
ToBigLittleEndianWord (
    UINT16 EndianWord
)
{
    return (((EndianWord >> 8) & 0xFF) + (EndianWord << 8));
}

/** @internal
    Builds Nvme Passwords buffer from user entered password in order
    to unlock Master or User password

    @param SecurityCommand     Security command for NVMe
    @param Control             Bit0 - 0/1 Indicates User/Master password    
                               Bit8 - 0/1 High/Maximum Security level
    @param *PasswrdBuffer      Pointer to the password buffer
    @param *Buffer             Pointer to the buffer
    @param RevisionCode(Valid when Control Bit 0 = 1 of Master Password)
           
    @return VOID

    @endinternal
**/
VOID
BuildNvmeCommandBuffer (
    UINT8            SecurityCommand,
    HDD_PASSWORD     *HddPassword,
    UINT8            *Buffer
)
{
    UINT8   i;
    UINT16  Control = 0;
    UINT16  RevisionCode = 0;
    
    // Bit0 - 0/1 Indicates User/Master password    
    // Bit8 - 0/1 High/Maximum Security level

    Control = HddPassword->Control & 0x103;
    RevisionCode = HddPassword->RevisionCode;

    Buffer[0] |= Control>>8;
    Buffer[1] |= (UINT8)Control&BIT0;
    
    
    // Security Erase command
    // BIT1 - 0/1 Normal Erase/ Enhanced Erase
    if( SecurityCommand==NVME_ATA_SECURITY_ERASE_UNIT ) {
        Buffer[0] |= Control>>1;
    }

    if( (SecurityCommand!=NVME_ATA_SECURITY_DISABLE_PASSWORD) &&\
            (Control&0x1) && (RevisionCode != 0) && (RevisionCode != 0xFFFF) ) {
        Buffer[34] = RevisionCode>>8;
        Buffer[35] = (UINT8)RevisionCode;
    }

    // Buffer[1] will 0/1 i.e. User/Master password
    if( Buffer[1] ){
        // Match has been found. Just copy the Password buffer
        for (i = 0; i < sizeof (HddPassword->MasterPassword); i++) {
            ((UINT8 *)Buffer)[i + 2] = HddPassword->MasterPassword[i];
        }
    } else {
        // Match has been found. Just copy the Password buffer
        for (i = 0; i < sizeof (HddPassword->UserPassword); i++) {
            ((UINT8 *)Buffer)[i + 2] = HddPassword->UserPassword[i];
        }
    }
    
    return;
}
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
/**
    Function SmmNvmePassThruSecurityUnlockPassword
             Send unlock password by NvmePassthough format

    @param pHddPassword
    
    @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
SmmNvmePassThruSecurityUnlockPassword (
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *StorageSecurityProtocol,        
    HDD_PASSWORD                            *pHddPassword        
)
{
    
    EFI_STATUS                      Status = EFI_SUCCESS;
    UINT8                           NvmeSecurityCmdBuffer[NVME_SECURITY_SEND_BUFFER_SIZE];
    UINT8                           SecurityCommand = NVME_ATA_SECURITY_UNLOCK;
    UINT32                          CommandLength = 0;

       
    if(StorageSecurityProtocol == NULL) return EFI_UNSUPPORTED;
    
    ZeroMemorySmm (&NvmeSecurityCmdBuffer, NVME_SECURITY_SEND_BUFFER_SIZE);  
    
    BuildNvmeCommandBuffer(
                SecurityCommand,
                pHddPassword,
                NvmeSecurityCmdBuffer
                );
    
    CommandLength = NVME_SECURITY_SEND_BUFFER_SIZE;
    
    Status = StorageSecurityProtocol->SendData( StorageSecurityProtocol,
                                                0,                                      // Media ID
                                                NVME_COMMAND_TIMEOUT * 100000000,       // 10sec timeout
                                                NVME_SECUIRTY_SECP,
                                                ToBigLittleEndianWord(NVME_ATA_SECURITY_UNLOCK),
                                                CommandLength,
                                                NvmeSecurityCmdBuffer
                                               );

    return Status;
}
/**
    Function RstSmmUnlockNvmeDevice
             locate all of RstSmm protocol and unlock each of drive

    @param pHddPassword
    
    @retval EFI_STATUS
**/
EFI_STATUS
RstSmmUnlockNvmeDevice (
    HDD_PASSWORD    *HddPassword
)
{            
    EFI_STATUS                    Status = EFI_SUCCESS;
    UINT16                        SecurityStatus;    
    EFI_HANDLE                    *HandleBuffer = NULL;
    UINTN                         Index, HandleBufferSize = 0;
    EFI_GUID  RaidSmmSSCPProtocolGuid = RST_SMM_STORAGE_PROTOCOL_GUID;    
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *StorageSecurityProtocol = NULL;


    Status = pSmst2->SmmLocateHandle( ByProtocol, 
                                      &RaidSmmSSCPProtocolGuid,
                                      NULL,
                                      &HandleBufferSize, 
                                      HandleBuffer );
                             
    if( EFI_ERROR(Status) ) {
        
        if( Status == EFI_BUFFER_TOO_SMALL ) {
            Status = pSmst2->SmmAllocatePool( EfiRuntimeServicesCode,
                                              HandleBufferSize,
                                              (VOID**)&HandleBuffer );
            
            if( EFI_ERROR(Status) ) {
                ASSERT(FALSE);
                return Status;
            }

            Status = pSmst2->SmmLocateHandle( ByProtocol, 
                                              &RaidSmmSSCPProtocolGuid,
                                              NULL,
                                              &HandleBufferSize, 
                                              HandleBuffer );
            
        }
    }
    
    //Using every StorageSecurity protocol installed by Rst driver to unlock this drive's password during S3
    //If there are drives set as the same password , we are able to unlock it at once.
    for(Index=0; Index<(HandleBufferSize/sizeof(EFI_HANDLE)); Index++) {
        
        DEBUG ((DEBUG_INFO, "HandleBuffer[%d] = %x\n", Index,HandleBuffer[Index]));

        StorageSecurityProtocol = NULL;
        Status = pSmst2->SmmHandleProtocol( HandleBuffer[Index], 
                                            &RaidSmmSSCPProtocolGuid, 
                                            (VOID **)&StorageSecurityProtocol );
                     
        if( EFI_ERROR(Status) ) continue;

        Status = SmmNvmePassThruReturnSecurityStatus(StorageSecurityProtocol,&SecurityStatus);
          
        // if Nvme device is locked, send Unlock command
        if( !EFI_ERROR(Status) && ( SecurityStatus & NVME_SECURITY_LOCK_MASK ) ) {
            
            Status = SmmNvmePassThruSecurityUnlockPassword (StorageSecurityProtocol,HddPassword);
            
        }        
    }

    // Free HandleBuffer memory
    if( HandleBuffer ) {
        pSmst2->SmmFreePool(HandleBuffer);
    }
        
    return EFI_SUCCESS;
}
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
/** @internal
    Routine to get Nvme controller security data

    @param *SmmNvmeController       Pointer to AMI_NVME_CONTROLLER_PROTOCOL
    @param *SecurityStatus          Pointer to security status of Nvme device.

    @retval EFI_SUCCESS      Nvme device security status returned successfully.
    @return Other            Return error status.      

    @endinternal
**/
EFI_STATUS
GetNvmeDeviceSecurityStatus(
    AMI_NVME_CONTROLLER_PROTOCOL  *SmmNvmeController,
    UINT16                        *SecurityStatus 
)
{
    EFI_STATUS                    Status;
    NVME_COMMAND_WRAPPER          *NvmeCmdWrapper = SmmNvmeController->NvmeCmdWrapper;
    NVME_SECURITY_RECEIVE_BUFFER  *SecReceiveBuffer;
    COMPLETION_QUEUE_ENTRY        *NvmeCompletionData;  // Use Global buffer for NVMe completion entry
    EDKII_IOMMU_PROTOCOL          *EdkiiIoMmu = NULL;
    EDKII_IOMMU_OPERATION         Operation;
    VOID                          *Mapping = NULL;
    EFI_PHYSICAL_ADDRESS          DeviceAddress;
    UINTN                         Size = 512; // allocate the  size as 512 
    
    // If unlock buffer not available, return error
    if(gSecurityUnlockCommandBuffer == NULL) {
        return EFI_UNSUPPORTED;
    }
    
    SecReceiveBuffer = (NVME_SECURITY_RECEIVE_BUFFER*)gSecurityUnlockCommandBuffer;
    NvmeCompletionData = (COMPLETION_QUEUE_ENTRY *)(gSecurityUnlockCommandBuffer+100);  // Use Global buffer for NVMe completion entry
     // Locate EdkiiIoMmuProtocol
    Status = pSmst2->SmmLocateProtocol(
                &gEdkiiIoMmuProtocolGuid, 
                NULL, 
                &EdkiiIoMmu);

    if (!EFI_ERROR(Status) && EdkiiIoMmu){
        
        Operation = EdkiiIoMmuOperationBusMasterWrite;
        Status = EdkiiIoMmu->Map(
                          EdkiiIoMmu,
                          Operation,
                          SecReceiveBuffer,
                          &Size,
                          &DeviceAddress,
                          &Mapping);
        
        if (EFI_ERROR(Status)){
            return Status;
        }
        SecReceiveBuffer = (NVME_SECURITY_RECEIVE_BUFFER*)DeviceAddress;
        NvmeCompletionData = (COMPLETION_QUEUE_ENTRY *) (DeviceAddress + 100);
    }

    ZeroMemorySmm (NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER));
    ZeroMemorySmm (SecReceiveBuffer,  sizeof(NVME_SECURITY_RECEIVE_BUFFER));
    ZeroMemorySmm (NvmeCompletionData, sizeof(COMPLETION_QUEUE_ENTRY));
    
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SECURITY_RECEIVE;
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
#if NVME_DRIVER_VERSION > 10
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = SmmNvmeController->NvmeDxeSmmDataArea->CommandIdentifierAdmin;
#else
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = SmmNvmeController->CommandIdentifierAdmin;
#endif
    NvmeCmdWrapper->NvmCmd.NSID = 0;
    NvmeCmdWrapper->NvmCmd.MPTR = 0;
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->SQIdentifier = 0;  // Queue 0 for Admin cmds,1 for I/O command
    NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 0x1000;
    
    NvmeCmdWrapper->NvmCmd.CDW10 = (((UINT32)NVME_SECUIRTY_SECP)<<24);
    NvmeCmdWrapper->NvmCmd.CDW11 = 0x10;
    NvmeCmdWrapper->NvmCmd.PRP1 = (UINT64)SecReceiveBuffer;
    NvmeCmdWrapper->NvmCmd.PRP2 = 0;
    
    Status = gSmmNvmePassThru->ExecuteNvmeCmd (
                  SmmNvmeController, 
                  NvmeCmdWrapper, 
                  NvmeCompletionData );

    if (Mapping){
        EdkiiIoMmu->Unmap(EdkiiIoMmu, Mapping);    
    }
    // Return Nvme device security status
    *SecurityStatus  = (SecReceiveBuffer->NvmeSecStatus >> 8) | (SecReceiveBuffer->NvmeSecStatus << 8);
    DEBUG ((DEBUG_VERBOSE, "SmmHddSec: Nvme Device Security Status = %x", *SecurityStatus));
    return Status;
}

/** @internal
    Routine sends SecurityUnlock command to Nvme controller

    @param *SmmNvmeController        Pointer to AMI_NVME_CONTROLLER_PROTOCOL
    @param *pHddPassword             Pointer to HDD_PASSWORD

    @retval EFI_SUCCESS       SecurityUnlock command is sent to Nvme controller
    @return Other             Return error status.
    
    @note
         1. Clear the NvmeCmdWrapper and buffer.
         2. Check for user/master password and copy them to Buffer.
         3. Clear NameSpaceId and MetaDataPointer and set Command set as Admin.
    @endinternal
**/
EFI_STATUS
SendNvmeSecurityUnlockCommand (
    AMI_NVME_CONTROLLER_PROTOCOL    *SmmNvmeController,
    HDD_PASSWORD                    *pHddPassword
)
{
    NVME_COMMAND_WRAPPER    *NvmeCmdWrapper = SmmNvmeController->NvmeCmdWrapper;
    UINT8                   *Buffer;
    COMPLETION_QUEUE_ENTRY   NvmeCompletionData;
    UINTN                    i;
    EFI_STATUS               Status;
	EDKII_IOMMU_PROTOCOL     *EdkiiIoMmu = NULL;
    EDKII_IOMMU_OPERATION    Operation;
    VOID                     *Mapping = NULL;
    EFI_PHYSICAL_ADDRESS     DeviceAddress;
    UINTN                    Size=512;
    
    // If unlock buffer not available, return error
    if(gSecurityUnlockCommandBuffer == NULL) {
        return EFI_UNSUPPORTED;
    }
    
    Buffer = gSecurityUnlockCommandBuffer;
	 //SMM IOMMU
    Status = pSmst2->SmmLocateProtocol(
                        &gEdkiiIoMmuProtocolGuid, 
                        NULL, 
                        &EdkiiIoMmu);
    
    if (!EFI_ERROR(Status) && EdkiiIoMmu){
     
        Operation = EdkiiIoMmuOperationBusMasterRead;
        Status = EdkiiIoMmu->Map(
                                EdkiiIoMmu,
                                Operation,
                                gSecurityUnlockCommandBuffer,
                                &Size,
                                &DeviceAddress,
                                &Mapping);
        if (EFI_ERROR(Status)){
            return Status;
        }
        Buffer = (UINT8 *)DeviceAddress;
    }
    
    //  Clear the Buffer
    ZeroMemorySmm (NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER));
    ZeroMemorySmm (Buffer, 512);

    Buffer[1] = (UINT8)(pHddPassword->Control & 0x1);

    // Buffer[1] will 0/1 i.e. User/Master password
    if( Buffer[1] ){
        // Match has been found. Just copy the Password buffer
        for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
            Buffer[i+2] = pHddPassword->MasterPassword[i];
        }
    } else {
        // Match has been found. Just copy the Password buffer
        for (i = 0; i < IDE_PASSWORD_LENGTH; i++) {
            Buffer[i+2] = pHddPassword->UserPassword[i];
        }
    }

    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SECURITY_SEND;
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;

#if NVME_DRIVER_VERSION > 10
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = SmmNvmeController->NvmeDxeSmmDataArea->CommandIdentifierAdmin;
#else
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = SmmNvmeController->CommandIdentifierAdmin;
#endif

    NvmeCmdWrapper->NvmCmd.NSID = 0;
    NvmeCmdWrapper->NvmCmd.MPTR = 0;
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->SQIdentifier = 0;  // Queue 0 for Admin cmds,1 for I/O command
    NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 0x1000;
    
    NvmeCmdWrapper->NvmCmd.CDW10 = (((UINT32)NVME_SECUIRTY_SECP)<<24) + (((UINT32)NVME_ATA_SECURITY_UNLOCK)<<8);
    NvmeCmdWrapper->NvmCmd.CDW11 = NVME_SECURITY_SEND_BUFFER_SIZE;
    NvmeCmdWrapper->NvmCmd.PRP1 = (UINT64)Buffer;
    NvmeCmdWrapper->NvmCmd.PRP2 = 0;
    
    Status = gSmmNvmePassThru->ExecuteNvmeCmd (
                  SmmNvmeController, 
                  NvmeCmdWrapper, 
                  &NvmeCompletionData );
    // clear password
    ZeroMemorySmm (Buffer, 512);

    if (Mapping){
        EdkiiIoMmu->Unmap(EdkiiIoMmu, Mapping);    
    }
    return Status;
}
#endif

