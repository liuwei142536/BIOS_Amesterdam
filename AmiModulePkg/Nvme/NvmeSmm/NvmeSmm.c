//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file NvmeSmm.c
    Nvme SMM driver to handle the Nvme device access

**/

//---------------------------------------------------------------------------

#include "NvmeSmm.h"
#include <Library/BaseMemoryLib.h>
#include <Guid/SmmVariableCommon.h>
#include <Library/AmiBufferValidationLib.h>
#include <Protocol/IoMmu.h>
#include <Protocol/SmmReadyToBoot.h>
//---------------------------------------------------------------------------

EFI_SMM_BASE2_PROTOCOL          *gSmmBase2;
EFI_SMM_SYSTEM_TABLE2           *pSmst2;
NVME_GLOBAL_DATA                *gNvmeData;
AMI_NVME_CONTROLLER_PROTOCOL    *gNvmeController[NVMEDEVS_MAX_ENTRIES] = {0};
BOOLEAN                         gFirstAPICall = FALSE;
AMI_NVME_PASS_THRU_PROTOCOL     gAmiSmmNvmePassThru;
VOID                            *gRegistrationSmmReadyToBoot = NULL;
BOOLEAN                         gCheckSmmNvmePassThruInstalled = FALSE;

/**
     NvmeMassApiTable - NVMe API Function Dispatch Table
        
**/


API_FUNC NvmeMassApiTable[] = {
    NvmeMassAPIGetDeviceInformation,    // Nvme Mass API Sub-Func 00h
    NvmeMassAPIGetDeviceGeometry,       // Nvme Mass API Sub-Func 01h
    NvmeMassAPIResetDevice,             // Nvme Mass API Sub-Func 02h
    NvmeMassAPIReadDevice,              // Nvme Mass API Sub-Func 03h
    NvmeMassAPIWriteDevice,             // Nvme Mass API Sub-Func 04h
    NvmeMassAPIPass,                    // Nvme Mass API Sub-Func 05h VerifyDevice
    NvmeMassAPIPass,                    // Nvme Mass API Sub-Func 06h FormatDevice
    NvmeMassAPINotSupported,            // Nvme Mass API Sub-Func 07h CommandPassThru
    NvmeMassAPINotSupported,            // Nvme BIOS API function 08h AssignDriveNumber
    NvmeMassAPINotSupported,            // Nvme BIOS API function 09h CheckDevStatus
    NvmeMassAPINotSupported,            // Nvme BIOS API function 0Ah GetDevStatus
    NvmeMassAPINotSupported             // Nvme BIOS API function 0Bh GetDeviceParameters
};

/**
    Stalls for the Required Amount of MicroSeconds

    @param 
        UINTN   Usec    // Number of microseconds to delay

    @retval VOID

**/
VOID 
SmmStall (
    UINTN Usec
)
{
    UINTN  Counter, index;
    UINT32 Data32, PrevData;
    UINT32 Remainder;

    Counter = (UINTN)DivU64x32Remainder ((Usec * 10), 3, &Remainder);

    if (Remainder != 0) {
        Counter++;
    }

    // Call WaitForTick for Counter + 1 ticks to try to guarantee Counter tick
    // periods, thus attempting to ensure Microseconds of stall time.
    if (Counter != 0) {

        PrevData = IoRead32 (PM_BASE_ADDRESS + 8); // Timer Base Address
        for (index = 0; index < Counter; ) {
            Data32 = IoRead32 (PM_BASE_ADDRESS + 8);    
            if (Data32 < PrevData) {        // Reset if there is a overlap
                PrevData = Data32;
                continue;
            }
            index += (Data32 - PrevData);        
            PrevData = Data32;
        }
    }
    return;
}

/**
    Get the Index# for the DeviceAddress

    @param  DeviceAddress,
    @param  **ActiveNameSpace

    @retval None

**/

VOID
GetDevEntry (
    UINT8                   DeviceAddress,
    ACTIVE_NAMESPACE_DATA   **ActiveNameSpace
)
{

    UINT8                           Count;
    LIST_ENTRY                      *LinkData;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController;
    
    // Locate a free slot to copy the pointer
     for (Count = 0; Count < NVMEDEVS_MAX_ENTRIES; Count++ ){
         if (gNvmeController[Count]) {
             NvmeController = gNvmeController[Count];
             
             if(IsListEmpty(&NvmeController->ActiveNameSpaceList)) {
                 continue;
             } 
             
             for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; \
                     LinkData != &NvmeController->ActiveNameSpaceList; 
                     LinkData = LinkData->ForwardLink) {
                 
                 *ActiveNameSpace = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
                 if ((*ActiveNameSpace)->Int13DeviceAddress == DeviceAddress) {
                     return;
                 }
             }
         }
     }
    
    *ActiveNameSpace = NULL;
    return ;

}

/**
    Return Device information

    @param  *NvmeURP

    @retval None

**/

VOID
NvmeMassAPIGetDeviceInformation (
    NVME_STRUC  *NvmeURP
)
{

    NvmeURP->bRetValue = NVME_NOT_SUPPORTED;
    return;
    
}

/**
    Return Device Geometry

    @param *NvmeURP

    @retval None

**/

VOID
NvmeMassAPIGetDeviceGeometry (
    NVME_STRUC  *NvmeURP
)
{
    NvmeURP->bRetValue = NVME_NOT_SUPPORTED;
    return;
}

/**
    Reset device

    @param  *NvmeURP

    @retval None

**/

VOID
NvmeMassAPIResetDevice (
    NVME_STRUC  *NvmeURP
)
{
    NvmeURP->bRetValue = NVME_NOT_SUPPORTED;
    return;
}

/**
    Read data from the device

    @param *NvmeURP

    @retval None

**/

VOID
NvmeMassAPIReadDevice (
    NVME_STRUC  *NvmeURP
)
{
    
    ACTIVE_NAMESPACE_DATA               *ActiveNameSpace = NULL;
    EFI_STATUS                          Status;
    EFI_LBA                             Lba;
    UINT16                              NumBlks;
    UINT16                              BlksPerTransfer;
    VOID                                *Buffer = NULL;
    VOID                                *ReadBuffer;
    UINTN                               BufferSize;
    BOOLEAN                             UnalignedTransfer = FALSE;
    AMI_NVME_CONTROLLER_PROTOCOL        *NvmeController;
    
    if(!gCheckSmmNvmePassThruInstalled){
        return;
    }
    GetDevEntry(NvmeURP->ApiData.Read.DeviceAddress, &ActiveNameSpace);
    
    if(ActiveNameSpace == NULL) {
        NvmeURP->bRetValue = NVME_PARAMETER_FAILED;
        return;
    }
    
    // Validate if input Buffer address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers.
    // NOTE: As DMA transfer isn't supported inside SMM, Buffer validation is not needed for 
    // NVMe. But below validation code is added to avoid repeated Security False Threat reports.

    Status = AmiValidateMemoryBuffer( (VOID*)(UINTN)NvmeURP->ApiData.Read.BufferAddress, 
                                       NvmeURP->ApiData.Read.NumBlks * 
                                       ActiveNameSpace->NvmeBlockIO.Media->BlockSize );
    if (EFI_ERROR(Status)) {
        NvmeURP->bRetValue = NVME_PARAMETER_FAILED;
        return;
    }
    
    NvmeController = ActiveNameSpace->NvmeController;

    Lba=NvmeURP->ApiData.Read.LBA;
    NumBlks=NvmeURP->ApiData.Read.NumBlks;
   
    Buffer= (VOID *)(UINTN)NvmeURP->ApiData.Read.BufferAddress;

    BlksPerTransfer =  NumBlks;
    ReadBuffer = Buffer;
    
    //If Buffer isn't aligned use internal buffer
    if ((UINTN)NvmeURP->ApiData.Read.BufferAddress & ((1 << ActiveNameSpace->NvmeBlockIO.Media->IoAlign)-1)) {
        BlksPerTransfer = 1;
        ReadBuffer = NvmeController->LegacyNvmeBuffer;
        UnalignedTransfer = TRUE;
    }

    BufferSize = BlksPerTransfer * ActiveNameSpace->NvmeBlockIO.Media->BlockSize;
    
    for (  ;  NumBlks; NumBlks -= BlksPerTransfer){
        Status = NvmeReadWriteBlocks (ActiveNameSpace, ActiveNameSpace->NvmeBlockIO.Media->MediaId, Lba, BufferSize, ReadBuffer,NULL,NVME_READ);
        if (EFI_ERROR(Status)) {
            break;
        }
        if  (UnalignedTransfer) {
            CopyMem (Buffer, ReadBuffer, BufferSize); 
        }
        Buffer = (VOID *)((UINTN)Buffer + BufferSize);
        Lba += BlksPerTransfer;

    }
   
    if (EFI_ERROR(Status)) {
        NvmeURP->bRetValue = NVME_READ_ERR;
    } else {
        NvmeURP->bRetValue = NVME_SUCCESS;
    }
    
    return;
    
}

/**
    Write data to the device

    @param  *NvmeURP

    @retval None

**/

VOID
NvmeMassAPIWriteDevice (
    NVME_STRUC  *NvmeURP
)
{
    
    ACTIVE_NAMESPACE_DATA               *ActiveNameSpace = NULL;
    EFI_STATUS                          Status;
    EFI_LBA                             Lba;
    UINT16                              NumBlks;
    UINT16                              BlksPerTransfer;
    VOID                                *Buffer = NULL;
    VOID                                *ReadBuffer;
    UINTN                               BufferSize;
    BOOLEAN                             UnalignedTransfer = FALSE;
    AMI_NVME_CONTROLLER_PROTOCOL        *NvmeController;
    
    if(!gCheckSmmNvmePassThruInstalled){
        return;
    }
    
    GetDevEntry(NvmeURP->ApiData.Read.DeviceAddress, &ActiveNameSpace);
    
    if(ActiveNameSpace == NULL) {
        NvmeURP->bRetValue = NVME_PARAMETER_FAILED;
        return;
    }
    
    // Validate if input Buffer address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers.
    // NOTE: As DMA transfer isn't supported inside SMM, Buffer validation is not needed for 
    // NVMe. But below validation code is added to avoid repeated Security False Threat reports.

    Status = AmiValidateMemoryBuffer( (VOID*)(UINTN)NvmeURP->ApiData.Read.BufferAddress, 
                                      NvmeURP->ApiData.Read.NumBlks * 
                                      ActiveNameSpace->NvmeBlockIO.Media->BlockSize );

    if (EFI_ERROR(Status)) {
        NvmeURP->bRetValue = NVME_PARAMETER_FAILED;
        return;
    }

    NvmeController = ActiveNameSpace->NvmeController;

    Lba=NvmeURP->ApiData.Read.LBA;
    NumBlks=NvmeURP->ApiData.Read.NumBlks;
   
    Buffer= (VOID *) (UINTN) NvmeURP->ApiData.Read.BufferAddress;

    BlksPerTransfer =  NumBlks;
    ReadBuffer = Buffer;
    
    //If Buffer isn't aligned use internal buffer
    if ((UINTN)NvmeURP->ApiData.Read.BufferAddress & ((1 << ActiveNameSpace->NvmeBlockIO.Media->IoAlign)-1)) {
        BlksPerTransfer = 1;
        ReadBuffer = NvmeController->LegacyNvmeBuffer;
        UnalignedTransfer = TRUE;
    }

    BufferSize = BlksPerTransfer * ActiveNameSpace->NvmeBlockIO.Media->BlockSize;
    
    for (  ;  NumBlks; NumBlks -= BlksPerTransfer){
        
        if  (UnalignedTransfer) {
            CopyMem (ReadBuffer, Buffer, BufferSize); 
        }
        
        Status = NvmeReadWriteBlocks (ActiveNameSpace, ActiveNameSpace->NvmeBlockIO.Media->MediaId, Lba, BufferSize, ReadBuffer,NULL, NVME_WRITE);
        if (EFI_ERROR(Status)) {
            break;
        }

        Buffer = (VOID *)((UINTN)Buffer + BufferSize);
        Lba += BlksPerTransfer;

    }
   
    if (EFI_ERROR(Status)) {
        NvmeURP->bRetValue = NVME_WRITE_ERR;
    }
    else {
        NvmeURP->bRetValue = NVME_SUCCESS;
    }
    
    return;
    
}



/**
    Dummy handler to return NVME_SUCCESS

        
    @param *NvmeURP
    @retval None

**/

VOID
NvmeMassAPIPass(
    NVME_STRUC *NvmeURP
)
{

    NvmeURP->bRetValue = NVME_SUCCESS;
    return;

}


/**
    Dummy handler to return NVME_NOT_SUPPORTED

    @param *NvmeURP

    @retval None

**/

VOID
NvmeMassAPINotSupported (
    NVME_STRUC  *NvmeURP
)
{

    NvmeURP->bRetValue = NVME_NOT_SUPPORTED;
    return;

}

/**
    Stop/release resources in S3 boot path

    @param   NvmeController  Instance of AMI_NVME_CONTROLLER_PROTOCOL

    @retval  EFI_SUCCESS     Resources are freed successfully.

**/

EFI_STATUS
EFIAPI
NvmeStopController (
    IN  AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController 
  )
{
 
   EDKII_IOMMU_PROTOCOL        *EdkiiIoMmu = NULL;
   EFI_STATUS                   Status;
    
   Status = pSmst2->SmmLocateProtocol(
                       &gEdkiiIoMmuProtocolGuid, 
                       NULL, 
                       &EdkiiIoMmu);              
        
    if (!EFI_ERROR(Status) && EdkiiIoMmu){
        Status = EdkiiIoMmu->FreeBuffer(
                                    EdkiiIoMmu,
                                    NvmeController->AdminCompletionUnAlignedSize,
                                    (void *)NvmeController->AdminCompletionUnAligned);
        ASSERT_EFI_ERROR(Status);
        
        Status = EdkiiIoMmu->FreeBuffer(
                                    EdkiiIoMmu,
                                    NvmeController->AdminSubmissionUnAlignedSize,
                                    (void *)NvmeController->AdminSubmissionUnAligned);
        ASSERT_EFI_ERROR(Status);
    }
    return EFI_SUCCESS;
}

/**
    Initialize NVMe Controller in S3 boot path

    @param   NvmeController,

    @retval  EFI_SUCCESS NvmeController is initialized successfully

**/

EFI_STATUS
EFIAPI
NvmeInitController (
    IN  AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController 
  )
{
    UINT32                  Delay;
    UINT32                  ProgramCC = 0;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    EDKII_IOMMU_PROTOCOL        *EdkiiIoMmu = NULL;
    EFI_PHYSICAL_ADDRESS        AdminQueueAddress = 0;
    UINTN                       AllocatePageSize;
    UINTN                       AdminQueueSize;
    EFI_STATUS                  Status;
    if(NvmeController->NvmeBarOffset == 0 ) {
        return EFI_NOT_FOUND;
    }

    if(CONTROLLER_REG32(NvmeController, Offset_Version) == 0xFFFFFFF) {
        ASSERT(FALSE);
        return EFI_NOT_FOUND;
    }

    // Initialize Nvme controller only once in S3Resume 
    if( NvmeController->NvmeInitControllerFlag ) {
        if( CONTROLLER_REG32(NvmeController, Offset_Aqa) ==\
                     ((NvmeController->AdminCompletionQueueSize - 1) << 16) + (NvmeController->AdminSubmissionQueueSize - 1) ) {
            return EFI_SUCCESS;
        }
        NvmeController->NvmeInitControllerFlag = FALSE;
    }

    NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr = 0;
    NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr = 0;
    NvmeDxeSmmDataArea->AdminCompletionQueueHeadPtr = 0;
    NvmeDxeSmmDataArea->AdminCompletionQueueTailPtr = 0;
    NvmeDxeSmmDataArea->CommandIdentifierAdmin = 0;
    NvmeController->DoorBellStride = 0;
    NvmeDxeSmmDataArea->AdminPhaseTag = FALSE;
    NvmeDxeSmmDataArea->Queue1PhaseTag = FALSE;

    Status = pSmst2->SmmLocateProtocol(
                            &gEdkiiIoMmuProtocolGuid, 
                            NULL, 
                            &EdkiiIoMmu);
    
    if (!EFI_ERROR(Status) && EdkiiIoMmu){
        
        // Allocate memory for Admin Queue. Each entry is 64bytes long and queue should be aligned on MemoryPageSize
        AdminQueueSize = (NvmeController->MaxQueueEntrySupported >= ADMIN_QUEUE_SIZE ? 
                          ADMIN_QUEUE_SIZE : NvmeController->MaxQueueEntrySupported) * sizeof (NVME_ADMIN_COMMAND);

        AllocatePageSize = NvmeController->MemoryPageSize + AdminQueueSize; 

        Status = EdkiiIoMmu->AllocateBuffer(
                                    EdkiiIoMmu,
                                    0,
                                    0,
                                    EFI_SIZE_TO_PAGES(AllocatePageSize),
                                    (VOID**)&AdminQueueAddress,
                                    0);
        if( EFI_ERROR(Status) ) {
            ASSERT_EFI_ERROR(Status);
            return Status;
        }
        NvmeController->AdminSubmissionUnAligned = AdminQueueAddress;
        NvmeController->AdminSubmissionUnAlignedSize = EFI_SIZE_TO_PAGES(AllocatePageSize);
        NvmeController->AdminSubmissionQueue = (AdminQueueAddress & ~((UINT64)NvmeController->MemoryPageSize - 1)) + 
                                                NvmeController->MemoryPageSize;
        
        Status = EdkiiIoMmu->AllocateBuffer(
                                     EdkiiIoMmu,
                                     0,
                                     0,
                                     EFI_SIZE_TO_PAGES(AllocatePageSize),
                                     (VOID**)&AdminQueueAddress,
                                     0);
        if( EFI_ERROR(Status) ) {
            ASSERT_EFI_ERROR(Status);
            return Status;
        }
        NvmeController->AdminCompletionUnAligned = AdminQueueAddress;
        NvmeController->AdminCompletionUnAlignedSize = EFI_SIZE_TO_PAGES(AllocatePageSize);
        NvmeController->AdminCompletionQueue = (AdminQueueAddress & ~((UINT64)NvmeController->MemoryPageSize - 1)) + 
                                                NvmeController->MemoryPageSize;
        
    }
    ZeroMem((VOID *)NvmeController->AdminSubmissionQueue, NvmeController->AdminSubmissionQueueSize);
    ZeroMem((VOID *)NvmeController->AdminCompletionQueue, NvmeController->AdminCompletionQueueSize);

    Delay = NvmeController->TimeOut * 500;

    // Check if the controller is still in shutdown process occurring state
    do {
        if ((CONTROLLER_REG32(NvmeController, Offset_CSTS) & 0xC) != 4) {
            break;
        }
        SmmStall (1000);  //  1Msec delay
        
    } while (--Delay);

    if (!Delay) {
        return EFI_DEVICE_ERROR;
    }

    // CC.EN bit needs to be disabled before set ASQ,ACQ values
    Delay = NvmeController->TimeOut * 500;
    if (CONTROLLER_REG32(NvmeController, Offset_CC) & 0x1) {
        //  Disable Enable bit
        CONTROLLER_REG32_AND (NvmeController, Offset_CC, ~0x01);
        do {
            if (!(CONTROLLER_REG32(NvmeController, Offset_CSTS) & 0x1)) {
                break;
            }
            SmmStall (1000);  //  1Msec delay
        }while (--Delay);
    }

    if (!Delay) {
        return EFI_DEVICE_ERROR;
    }

    // Initialize Controller configuration register. 
    // Select Round Robin and NVM Command Set (both values are zero)
    // Max Page Size
    NvmeController->MemoryPageSize = NvmeController->MemoryPageSizeMin;
    ProgramCC |= (UINT32) LShiftU64((UINTN)RShiftU64(NvmeController->MemoryPageSize, 13), 7);
    
    // Initialize with default value. Later it can be modified
    ProgramCC |= (6 << 16);     // I/O Submission Queue Entry Size
    ProgramCC |= (4 << 20);     // I/O Completion Queue Entry Size
    
    CONTROLLER_WRITE_REG32( NvmeController, Offset_CC, ProgramCC );

    // Program Admin Queue Size and Base Address
    CONTROLLER_WRITE_REG32( NvmeController, Offset_Aqa, ((NvmeController->AdminCompletionQueueSize - 1) << 16) + 
                               (NvmeController->AdminSubmissionQueueSize - 1) );

    CONTROLLER_WRITE_REG32( NvmeController, Offset_Asq, 
                               (UINT32) NvmeController->AdminSubmissionQueue );

    CONTROLLER_WRITE_REG32( NvmeController, Offset_Asq + 4,  
                             (UINT32) RShiftU64(NvmeController->AdminSubmissionQueue, 32) );
    
    CONTROLLER_WRITE_REG32( NvmeController, Offset_Acq, 
                               (UINT32)NvmeController->AdminCompletionQueue );
    
    CONTROLLER_WRITE_REG32( NvmeController, Offset_Acq + 4, 
                               (UINT32) RShiftU64(NvmeController->AdminCompletionQueue, 32) );

    // Enable Controller
    CONTROLLER_REG32_OR( NvmeController, Offset_CC, 0x1 );

    // Wait for the controller to get ready
    // Check if the controller is already running. If yes stop it.
    Delay = NvmeController->TimeOut * 500;
    do {
        if ((CONTROLLER_REG32(NvmeController, Offset_CSTS) & 0x1)) {
            break;
        }
        
        MicroSecondDelay (1000);
    } while (--Delay);
    
    if (!Delay) {
        return EFI_DEVICE_ERROR;
    }

    NvmeController->NvmeInitControllerFlag = TRUE;
    return EFI_SUCCESS;
}

/**
    Initialize NVMe SMM data area

    @param   DispatchHandle,
    @param   *Context         OPTIONAL,
    @param   *CommBuffer      OPTIONAL,
    @param   *CommBufferSize  OPTIONAL

    @retval  EFI_STATUS

**/

EFI_STATUS
EFIAPI
NvmeInitSmmData (
    IN  EFI_HANDLE  DispatchHandle,
    IN  CONST   VOID    *Context        OPTIONAL,
    IN  OUT     VOID    *CommBuffer     OPTIONAL,
    IN  OUT     UINTN   *CommBufferSize OPTIONAL
  )
{

    EFI_STATUS                    Status = EFI_SUCCESS;
    ACTIVE_NAMESPACE_DATA         *OrgActiveNameSpace;
    ACTIVE_NAMESPACE_DATA         *ActiveNameSpace;
    AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController;
    AMI_NVME_CONTROLLER_PROTOCOL  *OrgNvmeController;
    EFI_BLOCK_IO_MEDIA            *Media = NULL;
    EFI_HANDLE                    AmiSmmNvmeControllerHandle=NULL;
    EFI_HANDLE                    SwHandle = NULL;
    EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch = NULL;
    EFI_SMM_SW_REGISTER_CONTEXT   SwContext;
    UINT64                        *NvmeSwSmiSignature = NULL;
    NVME_CONTROLLER_DATA_TO_SMM   *NvmeSmmData = NULL;
    
    
    // After the first API call is invoked, don't initialize SMM data area. This is an additional
    // Security check so that data won't get corrupted.
    if (gFirstAPICall) {
        return EFI_SUCCESS;
    }
    
    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL) {
        return EFI_SUCCESS;
    }
    
    // Validate CommBuffer with sizeof(UINT64) to validate NvmeDxe driver's Signature
    Status = AmiValidateMemoryBuffer((VOID*)CommBuffer, sizeof(UINT64) );
    if (EFI_ERROR(Status)) {
        return EFI_SUCCESS;
    }

    NvmeSwSmiSignature = (UINT64  *)CommBuffer;
    
    // Check if the SMI triggerd for the SW SMI register 
    if (*NvmeSwSmiSignature == SIGNATURE_64 ('S', 'W', 'S', 'M', 'I', 0, 0, 0)) {
       
	    Status = pSmst2->SmmLocateProtocol(&gEfiSmmSwDispatch2ProtocolGuid,
                                            NULL,
                                            (VOID**)&SwDispatch);
        if (EFI_ERROR(Status)) {
            return Status;
        }

        // Register the SW SMI for the Legacy Function
        SwContext.SwSmiInputValue = NVME_SWSMI;
        Status = SwDispatch->Register (SwDispatch,
                                        NvmeSwSmiHandler,
                                        &SwContext,
                                        &SwHandle);
        ASSERT_EFI_ERROR(Status);
        
        return Status;
    }
    
    // Check if the SMI triggerd for DXE to SMM data transfer
    if (*NvmeSwSmiSignature != SIGNATURE_64 ('D', 'X', 'E', 'T', 'O', 'S', 'M', 'M')) {
    	return EFI_UNSUPPORTED;
    }
    
    NvmeSmmData = (NVME_CONTROLLER_DATA_TO_SMM *)CommBuffer;
    
    // Validate NvmeSmmData with "sizeof(NVME_CONTROLLER_DATA_TO_SMM)" size should not overlap with SMRAM
    Status = AmiValidateMemoryBuffer((VOID *)NvmeSmmData, sizeof(NVME_CONTROLLER_DATA_TO_SMM));
    if (EFI_ERROR(Status)) {
        return EFI_SUCCESS;
    }

    if(NvmeSmmData->ControllerNumber >= NVMEDEVS_MAX_ENTRIES) {
    	return EFI_OUT_OF_RESOURCES;
    }
    
    // If NameSpace data passed before controller memory allcoation, then return error
    if((!gNvmeController[NvmeSmmData->ControllerNumber]) && (NvmeSmmData->NameSpaceId != 0)) {
    	return EFI_UNSUPPORTED;
    }
    
    //
    // Validate CommBuffer Passed from Nvme Dxe driver
    //
    // gNvmeController[NvmeSmmData->ControllerNumber] is 0, CommBuffer contains "Nvme Controller" data. Validate the data
    // not overlaps with SMRAM region
    //
    // gNvmeController[NvmeSmmData->ControllerNumber] is not 0, CommBuffer contains "Nvme Namespace" data. Validate the data
    // not overlaps with SMRAM region.
    //
    if(!gNvmeController[NvmeSmmData->ControllerNumber]) {
        
        OrgNvmeController = (AMI_NVME_CONTROLLER_PROTOCOL *)NvmeSmmData->Data;

        // Validate "OrgNvmeController" with size "sizeof(AMI_NVME_CONTROLLER_PROTOCOL)" not overlaps with SMRAM region
        Status = AmiValidateMemoryBuffer((VOID *)OrgNvmeController, sizeof(AMI_NVME_CONTROLLER_PROTOCOL));
        if (EFI_ERROR(Status)) {
            return EFI_SUCCESS;
        }
                
        // Validate LegacyBuffer
        Status = AmiValidateMemoryBuffer((VOID *)OrgNvmeController->LegacyNvmeBuffer, EFI_SIZE_TO_PAGES(512)*EFI_PAGE_SIZE);
        if (EFI_ERROR(Status)) {
            return EFI_SUCCESS;
        }
        
        // Validate IdentifyControllerData buffer
        Status = AmiValidateMemoryBuffer((VOID *)OrgNvmeController->IdentifyControllerData, sizeof(IDENTIFY_CONTROLLER_DATA));
        if (EFI_ERROR(Status)) {
            return EFI_SUCCESS;
        }

    } else {
        OrgActiveNameSpace = (ACTIVE_NAMESPACE_DATA *)NvmeSmmData->Data;

        // Validate "OrgActiveNameSpace" with size "sizeof(ACTIVE_NAMESPACE_DATA)" not overlaps with SMRAM region
        Status = AmiValidateMemoryBuffer((VOID *)OrgActiveNameSpace, sizeof(ACTIVE_NAMESPACE_DATA));
        if (EFI_ERROR(Status)) {
            return EFI_SUCCESS;
        }
    
        // Validate Media buffer
        Status = AmiValidateMemoryBuffer((VOID *)OrgActiveNameSpace->NvmeBlockIO.Media, sizeof(EFI_BLOCK_IO_MEDIA));
        if (EFI_ERROR(Status)) {
            return EFI_SUCCESS;
        }
    }

    // Check whether memory allocated for controller or not
    if(!gNvmeController[NvmeSmmData->ControllerNumber]) {
        
        Status = pSmst2->SmmAllocatePool ( 
                                    EfiRuntimeServicesData, 
                                    sizeof (AMI_NVME_CONTROLLER_PROTOCOL),
                                    (VOID**)&NvmeController);
        if( EFI_ERROR(Status) ) {
            ASSERT_EFI_ERROR(Status);
            return Status;
        }
        
        // Store the allocated memory pointer
        gNvmeController[NvmeSmmData->ControllerNumber] = NvmeController;
        
        // Copy controller data to SMRAM
        CopyMem ((VOID *)NvmeController, OrgNvmeController, sizeof (AMI_NVME_CONTROLLER_PROTOCOL));
        
        Status = pSmst2->SmmAllocatePool ( 
                                    EfiRuntimeServicesData, 
                                    sizeof (IDENTIFY_CONTROLLER_DATA),
                                    (VOID**)&(NvmeController->IdentifyControllerData));
        if( EFI_ERROR(Status) ) {
            ASSERT_EFI_ERROR(Status);
            return Status;
        }
        
        // Copy IDENTIFY_CONTROLLER_DATA to SMRAM
        CopyMem (NvmeController->IdentifyControllerData, OrgNvmeController->IdentifyControllerData, sizeof(IDENTIFY_CONTROLLER_DATA));
        
        // Initialize some of the pointers to NULL which aren't applicable during runtime
        NvmeController->PciIO = NULL;
        NvmeController->NvmeInSmm = TRUE;

        //Initialize the Controller Init API for SMM.
        NvmeController->NvmeInitController = NvmeInitController;
        NvmeController->NvmeStopController = NvmeStopController;

        // Flag to denote NvmeInitController API is called or not
        NvmeController->NvmeInitControllerFlag = FALSE;

        InitializeListHead (&NvmeController->ActiveNameSpaceList);
        
        pSmst2->SmmAllocatePool ( EfiRuntimeServicesData, 
                                  sizeof (NVME_COMMAND_WRAPPER),
                                  (VOID**)&(NvmeController->NvmeCmdWrapper));
        
        // Controller Data alone passed. So return
        return EFI_SUCCESS;
        
    } else {
        // Memory already allocated and controller data stored. Retrieve pointer
    	NvmeController = gNvmeController[NvmeSmmData->ControllerNumber];
    }
    
    
    Status = pSmst2->SmmAllocatePool ( EfiRuntimeServicesData, 
    								   sizeof (ACTIVE_NAMESPACE_DATA),
    								   (VOID**)&(ActiveNameSpace));
    if( EFI_ERROR(Status) ) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }
    
    // Copy NameSpace data to SMRAM
    CopyMem (ActiveNameSpace, OrgActiveNameSpace, sizeof(ACTIVE_NAMESPACE_DATA));
    
    ActiveNameSpace->NvmeController = NvmeController;
    ActiveNameSpace->EfiDevicePath = NULL;
    
    Status = pSmst2->SmmAllocatePool ( EfiRuntimeServicesData, 
                              sizeof (EFI_BLOCK_IO_MEDIA),
                              (VOID**)&(Media) );
    if( EFI_ERROR(Status) ) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    // Copy EFI_BLOCK_IO_MEDIA to SMRAM
    CopyMem ( Media, OrgActiveNameSpace->NvmeBlockIO.Media, sizeof(EFI_BLOCK_IO_MEDIA) );
    
    ActiveNameSpace->NvmeBlockIO.Media = Media;
    
    ActiveNameSpace->Link.ForwardLink = &(ActiveNameSpace->Link);
    ActiveNameSpace->Link.BackLink = &(ActiveNameSpace->Link);

    InsertTailList (&NvmeController->ActiveNameSpaceList, &ActiveNameSpace->Link);
    
    // If the last NameSpace data transferred, then install AmiSmmNvmeControllerProtocol
    if(NvmeSmmData->LastNameSpace) {
		Status = pSmst2->SmmInstallProtocolInterface(
							&AmiSmmNvmeControllerHandle,
							&gAmiSmmNvmeControllerProtocolGuid,
							EFI_NATIVE_INTERFACE,
							NvmeController
							);
    }
    
    return Status;
}

/**
    Handle SWSMI generated from NVMe CSM16 module 

    @param  DispatchHandle,
    @param  *DispatchContext OPTIONAL,
    @param  *CommBuffer OPTIONAL,
    @param  *CommBufferSize OPTIONAL
  
    @retval  EFI_STATUS

**/

EFI_STATUS
EFIAPI
NvmeSwSmiHandler (
    IN  EFI_HANDLE      DispatchHandle,
    IN  CONST   VOID    *DispatchContext OPTIONAL,
    IN  OUT     VOID    *CommBuffer OPTIONAL,
    IN  OUT     UINTN   *CommBufferSize OPTIONAL
)
{
    EFI_STATUS  Status;
    NVME_STRUC  *NvmeURP=NULL;
    UINT8       bFuncIndex;
    UINT8       bNumberOfFunctions;
    UINT16      EbdaSeg;

    gFirstAPICall = TRUE;
    
    // Get the fpURP pointer from EBDA
        
    EbdaSeg = *((UINT16*)0x40E);
    NvmeURP = *(NVME_STRUC**)(UINTN)(((UINT32)EbdaSeg << 4) + NVME_DATA_EBDA_OFFSET);
    NvmeURP = (NVME_STRUC*)((UINTN)NvmeURP & 0xFFFFFFFF);
    

    // Validate if URP address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)NvmeURP, sizeof(NVME_STRUC));
    if (EFI_ERROR(Status)) {
    	// return without modifying the NvmeUrp buffer as it's not belongs to the normal Memory Region.
        return Status;
    }
    
    if  (NvmeURP->bFuncNumber != NVME_API_MASS_DEVICE_REQUEST) {
        NvmeURP->bRetValue = NVME_PARAMETER_FAILED;
        return Status;    
    }
    
    bFuncIndex = NvmeURP->bSubFunc;
    bNumberOfFunctions = sizeof NvmeMassApiTable / sizeof (API_FUNC *);

    //
    // Make sure function number is valid; if function number is not zero
    // check for valid extended SDIO API function
    //
    if (bFuncIndex >= bNumberOfFunctions ) {
        NvmeURP->bRetValue = NVME_PARAMETER_FAILED;
        return Status;
    }

    //
    // Call the appropriate function
    //

    NvmeMassApiTable[bFuncIndex](NvmeURP);

    return Status;
}

/**
    Installs AmiSmmNvmePassThruProtocolGuid at ReadyToBootEvent.

    @param ImageHandle,
    @param *SystemTable

    @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
InstallSmmNvmePassThruCallback (
    IN CONST EFI_GUID     *Protocol,
    IN VOID               *Interface,
    IN EFI_HANDLE         Handle
)
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      AmiSmmNvmePassThruHandle=NULL;
    
    if(gCheckSmmNvmePassThruInstalled) {
        return EFI_SUCCESS;
    }
    gAmiSmmNvmePassThru.ExecuteNvmeCmd = ExecuteNvmeCmd;
    Status = pSmst2->SmmInstallProtocolInterface(
                    &AmiSmmNvmePassThruHandle,
                    &gAmiSmmNvmePassThruProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &gAmiSmmNvmePassThru
                    );
    ASSERT_EFI_ERROR (Status);
    
    gCheckSmmNvmePassThruInstalled = TRUE;
    return EFI_SUCCESS;
}

/**
    Loads NVMe SMM module into SMM and registers SMI handler

    @param ImageHandle,
    @param *SystemTable

    @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
NvmeSmmDriverEntryPoint (
    IN  EFI_HANDLE       ImageHandle,
    IN  EFI_SYSTEM_TABLE *SystemTable
    )
{
    
    EFI_STATUS                      Status;
    EFI_HANDLE                      DispatchHandle;
    
    Status = gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&gSmmBase2);
    if (EFI_ERROR(Status)) {
        return EFI_SUCCESS;
    }

    Status = gSmmBase2->GetSmstLocation (gSmmBase2, &pSmst2);
    if (EFI_ERROR(Status)) {
        return EFI_SUCCESS;
    }

    //
    //Allocate Memory for NVMe global Data.
    //
    Status = pSmst2->SmmAllocatePool(EfiRuntimeServicesData,sizeof(NVME_GLOBAL_DATA), (VOID**)&gNvmeData);
    ASSERT_EFI_ERROR(Status);
    //
    //  Clear the Buffer
    //
    ZeroMem((VOID*)gNvmeData, sizeof(NVME_GLOBAL_DATA));
    
    
    //
    // Register Nvme handler to transfer data from DXE driver to SMM
    //
    Status = pSmst2->SmiHandlerRegister (
                      NvmeInitSmmData,
                      &gAmiSmmNvmeCommunicationGuid,
                      &DispatchHandle
                      );
    
    ASSERT_EFI_ERROR (Status);
    
    Status = pSmst2->SmmRegisterProtocolNotify (
                                &gEdkiiSmmReadyToBootProtocolGuid,
                                InstallSmmNvmePassThruCallback,
                                &gRegistrationSmmReadyToBoot
                                );
    ASSERT_EFI_ERROR (Status);
    
    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
