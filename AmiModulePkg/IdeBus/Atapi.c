//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file Atapi.c
    Supports ATAPI devices

**/

//---------------------------------------------------------------------------

#include "IdeBus.h"

//---------------------------------------------------------------------------

//
//External variables
//
extern PLATFORM_IDE_PROTOCOL    *gPlatformIdeProtocol;

/**
    Issues ATAPI Reset command.

    @param This 
    @param ExtendedVerification 

    @retval EFI_STATUS

**/

EFI_STATUS
AtapiReset (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  BOOLEAN                 ExtendedVerification
)
{
    return EFI_SUCCESS;
}

/**
    Read from the Atapi Device

    @param This 
    @param MediaId 
    @param EFI_LBA  LBA,
    @param BufferSize 
    @param Buffer 

    @retval EFI_STATUS

**/

EFI_STATUS
AtapiBlkRead (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
)
{
    UINTN              DataN;
    EFI_STATUS         Status;
    AMI_IDE_BUS_PROTOCOL   *IdeBusInterface = ((IDE_BLOCK_IO*)This)->IdeBusInterface;
    EFI_BLOCK_IO_MEDIA *BlkMedia = This->Media;
    UINTN              BufferAddress;

    //
    //Check if Media Present
    //
    if ( BlkMedia->MediaPresent == FALSE ) {
        Status = DetectAtapiMedia( IdeBusInterface );

        if ( Status == EFI_SUCCESS ) {
            return EFI_MEDIA_CHANGED;
        }
        return Status;
    }

    //
    //Check if Media ID matches
    //
    if ( BlkMedia->MediaId != MediaId ) {
        return EFI_MEDIA_CHANGED;
    }

    if ( BufferSize == 0 ) {
        return EFI_SUCCESS;
    }
    
    //
    // If IoAlign values is 0 or 1, means that the buffer can be placed 
    // anywhere in memory or else IoAlign value should be power of 2. To be
    // properly aligned the buffer address should be divisible by IoAlign  
    // with no remainder. 
    // 
    BufferAddress =(UINTN) Buffer;
    if((BlkMedia->IoAlign > 1 ) && (BufferAddress % BlkMedia->IoAlign)) {
        return EFI_INVALID_PARAMETER;
    }

    //
    //Check whether the block size is multiple of BlkMedia->BlockSize
    //
    DataN = BufferSize % BlkMedia->BlockSize;

    if ( DataN ) {
        return EFI_BAD_BUFFER_SIZE;
    }

    //	Check for Valid start LBA #
    if ( LBA > BlkMedia->LastBlock ) {
        return EFI_INVALID_PARAMETER;
    }

    //
    //Check for Valid End LBA #
    //
    DataN = BufferSize / BlkMedia->BlockSize;

    if ( LBA + DataN > BlkMedia->LastBlock + 1 ) {
        return EFI_INVALID_PARAMETER;
    }

    if ( ( gPlatformIdeProtocol->IdeBusMasterSupport ) && ( gPlatformIdeProtocol->AtapiBusMasterSupport ) ) {
        if ( DMACapable( IdeBusInterface )) {
            Status = AtapiReadWriteBusMaster(
                        IdeBusInterface,
                        Buffer,
                        BufferSize,
                        LBA,
                        IdeBusInterface->IdeDevice.ReadCommand,
                        FALSE );
            return Status;
        }
    }


    Status = AtapiReadWritePio( IdeBusInterface,
                                Buffer,
                                BufferSize,
                                LBA,
                                IdeBusInterface->IdeDevice.ReadCommand,
                                FALSE );
    return Status;
}

/**
    Write to the Atapi Device

        
    @param This 
    @param MediaId 
    @param EFI_LBA  LBA,
    @param BufferSize 
    @param Buffer 

    @retval EFI_STATUS

**/

EFI_STATUS
AtapiBlkWrite (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    IN  VOID                    *Buffer
)
{
    UINTN              DataN;
    EFI_STATUS         Status;
    AMI_IDE_BUS_PROTOCOL   *IdeBusInterface = ((IDE_BLOCK_IO*)This)->IdeBusInterface;
    EFI_BLOCK_IO_MEDIA *BlkMedia = This->Media;
    UINTN              BufferAddress;

    //
    //	Check if Media Present
    //
    if ( BlkMedia->MediaPresent == FALSE ) {
        Status = DetectAtapiMedia( IdeBusInterface );

        if ( Status == EFI_SUCCESS ) {
            return EFI_MEDIA_CHANGED;
        }
        return Status;
    }

    //
    //Check if Media ID matches
    //
    if ( BlkMedia->MediaId != MediaId ) {
        return EFI_MEDIA_CHANGED;
    }

    if ( BufferSize == 0 ) {
        return EFI_SUCCESS;
    }

    //
    // If IoAlign values is 0 or 1, means that the buffer can be placed 
    // anywhere in memory or else IoAlign value should be power of 2. To be
    // properly aligned the buffer address should be divisible by IoAlign  
    // with no remainder. 
    // 
    BufferAddress =(UINTN) Buffer;
    if((BlkMedia->IoAlign > 1 ) && (BufferAddress % BlkMedia->IoAlign)) {
        return EFI_INVALID_PARAMETER;
    }

    //
    //Check whether the block size is multiple of BlkMedia->BlockSize
    //
    DataN = BufferSize % BlkMedia->BlockSize;

    if ( DataN ) {
        return EFI_BAD_BUFFER_SIZE;
    }

    //
    //Check for Valid start LBA #
    //
    if ( LBA > BlkMedia->LastBlock ) {
        return EFI_INVALID_PARAMETER;
    }

    //
    //Check for Valid End LBA #
    //
    DataN = BufferSize / BlkMedia->BlockSize;

    if ( LBA + DataN > BlkMedia->LastBlock + 1 ) {
        return EFI_INVALID_PARAMETER;
    }

    if ( ( gPlatformIdeProtocol->IdeBusMasterSupport ) && ( gPlatformIdeProtocol->AtapiBusMasterSupport ) ) {
        if ( DMACapable( IdeBusInterface )) {
            Status = AtapiReadWriteBusMaster(
                                        IdeBusInterface,
                                        Buffer,
                                        BufferSize,
                                        LBA,
                                        IdeBusInterface->IdeDevice.WriteCommand,
                                        TRUE );
            return Status;
        }
    }


    Status = AtapiReadWritePio( IdeBusInterface,
                                Buffer,
                                BufferSize,
                                LBA,
                                IdeBusInterface->IdeDevice.WriteCommand,
                                TRUE );
    return Status;
}

/**
    Flush the cache

    @param This 

    @retval EFI_STATUS

**/

EFI_STATUS
AtapiBlkFlush (
    IN  EFI_BLOCK_IO_PROTOCOL   *This
)
{
    EFI_STATUS             Status = EFI_SUCCESS;
    AMI_IDE_BUS_PROTOCOL   *IdeBusInterface = NULL;
    
    IdeBusInterface = ((IDE_BLOCK_IO *)This)->IdeBusInterface;
    
    if (IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x1000) {
        // Flush Cache command supported
        Status = IdeNonDataCommand( IdeBusInterface, 
                                    0, 0, 0, 0, 0,
                                    IdeBusInterface->IdeDevice.Device << 4,
                                    FLUSH_CACHE );
    }

    return Status;

}

/**
    Detects whether a Media is present in the ATAPI Removable device or not.

    @param IDE_BUS_INIT_PROTOCOL			*IdeBusInitInterface

    @retval EFI_STATUS

    @note  
	1. Issue Read Capacity command for CDROM or Read Format command for other ATAPI devices.
	2. If step 1  is successful, update last LBA, Block Size, Read/Write capable, Media ID

**/

EFI_STATUS
DetectAtapiMedia (
    IN  OUT AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    UINT8               *InputData, *PacketBuffer, LoopCount;
    ATAPI_DEVICE        *AtapiDevice    =   IdeBusInterface->IdeDevice.AtapiDevice;
    EFI_BLOCK_IO_MEDIA  *BlkMedia       =   IdeBusInterface->IdeBlkIo->BlkIo.Media;
    EFI_STATUS          Status;
    UINT16              ByteCount       =   256, Data16;
    BOOLEAN             ReadCapacity    =   FALSE;

    PROGRESS_CODE( DXE_REMOVABLE_MEDIA_DETECT );

    //
    //Default Values
    //
    BlkMedia->MediaPresent                  = FALSE;
    BlkMedia->LastBlock                     = 0x100;                            // Dummy value
    IdeBusInterface->IdeDevice.ReadCommand  = ATAPI_READ_10;
    IdeBusInterface->IdeDevice.WriteCommand = ATAPI_WRITE_10;

    Status = TestUnitReady( IdeBusInterface );

    if ((Status != EFI_MEDIA_CHANGED) && (Status != EFI_SUCCESS)) {
        return Status;
    }

    //
    //Issue Read Capacity command
    //
    Status = pBS->AllocatePool( EfiBootServicesData,
                                ByteCount,
                                (VOID**)&InputData );

    if ( EFI_ERROR( Status )) {
        return Status;
    }
    Status = pBS->AllocatePool( EfiBootServicesData, 16, (VOID**)&PacketBuffer );

    if ( EFI_ERROR( Status )) {
        pBS->FreePool( InputData );
        return Status;
    }

    ZeroMemory( PacketBuffer, 16 );

    //
    //For CDROM use Read Capacity command else use Read Format Command
    //
    if ( AtapiDevice->DeviceType == CDROM_DEVICE ) {
        BlkMedia->BlockSize    = CDROM_BLOCK_SIZE;         
        AtapiDevice->BlockSize = BlkMedia->BlockSize;
        PacketBuffer[0]        = ATAPI_READ_CAPACITY;
        PacketBuffer[1]        = AtapiDevice->Lun << 5;
        Data16                 = 8;
    }
    else {
        BlkMedia->BlockSize    = LS120_BLOCK_SIZE;         
        AtapiDevice->BlockSize = BlkMedia->BlockSize;
        PacketBuffer[0]        = ATAPI_READ_FORMAT_CAPACITIES;
        PacketBuffer[1]        = AtapiDevice->Lun << 5;
        PacketBuffer[7]        = ByteCount >> 8;
        PacketBuffer[8]        = ByteCount & 0xff;
        Data16                 = ByteCount;
    }

    for ( LoopCount  = 0; LoopCount < 5; LoopCount++ ) {
        ByteCount = Data16;
        ZeroMemory( InputData, ByteCount );
        Status = GeneralAtapiCommandAndData( IdeBusInterface,
                                             PacketBuffer,
                                             InputData,
                                             &ByteCount );

        if ( PacketBuffer[0] == ATAPI_READ_FORMAT_CAPACITIES
             && AtapiDevice->Atapi_Status == ILLEGAL_REQUEST ) {
            //
            //If the Read Format Capacities not supported by device, try
            //ReadCapacity command
            //
            ZeroMemory( PacketBuffer, 16 );
            BlkMedia->BlockSize    = CDROM_BLOCK_SIZE;         // Default size
            AtapiDevice->BlockSize = BlkMedia->BlockSize;
            PacketBuffer[0]        = ATAPI_READ_CAPACITY;
            PacketBuffer[1]        = AtapiDevice->Lun << 5;
            Data16                 = 8;
            ReadCapacity           = TRUE;
        }

        if ( AtapiDevice->Atapi_Status == EFI_SUCCESS ) {
            break;
        }

        if ( AtapiDevice->Atapi_Status == MEDIUM_NOT_PRESENT ) {
            break;
        }
    }

    if ( Status == EFI_SUCCESS ) {
        if ( ReadCapacity == TRUE ) {
            BlkMedia->LastBlock = InputData[0] << 24 | InputData[1] << 16
                                  | InputData[2] << 8 | InputData[3];

            BlkMedia->LastBlock--;
            BlkMedia->MediaPresent = TRUE;
            BlkMedia->MediaId++;
            BlkMedia->BlockSize = InputData[4] << 24 | InputData[5] << 16
                                  | InputData[6] << 8 | InputData[7];

            AtapiDevice->BlockSize = BlkMedia->BlockSize;
            BlkMedia->ReadOnly     = FALSE;
        }
        else if ( AtapiDevice->DeviceType == CDROM_DEVICE )
        {
            BlkMedia->LastBlock = InputData[0] << 24 | InputData[1] << 16
                                  | InputData[2] << 8 | InputData[3];
            BlkMedia->LastBlock--;
            BlkMedia->BlockSize    = CDROM_BLOCK_SIZE;
            AtapiDevice->BlockSize = BlkMedia->BlockSize;
            BlkMedia->MediaPresent = TRUE;
            BlkMedia->MediaId++;
            BlkMedia->ReadOnly = TRUE;
        }
        else if ( InputData[8] != 3 )           // No media present
        {
            BlkMedia->LastBlock = InputData[4] << 24 | InputData[5] << 16
                                  | InputData[6] << 8 | InputData[7];
            BlkMedia->LastBlock--;
            BlkMedia->MediaPresent = TRUE;
            BlkMedia->MediaId++;
            BlkMedia->BlockSize = InputData[9] << 16 | InputData[10] << 8
                                  | InputData[11];
            BlkMedia->ReadOnly     = FALSE;
            AtapiDevice->BlockSize = BlkMedia->BlockSize;
        }

        //
        //		Update ReadOnly Status
        //
        if ( AtapiDevice->DeviceType != CDROM_DEVICE ) {
            ByteCount = 256;
            ZeroMemory( PacketBuffer,     16 );
            ZeroMemory( InputData, ByteCount );
            PacketBuffer[0] = ATAPI_MODE_SENSE;
            PacketBuffer[2] = RETURN_ALL_PAGES;
            PacketBuffer[7] = ByteCount >> 8;
            PacketBuffer[8] = ByteCount & 0xff;
            Status          = GeneralAtapiCommandAndData( IdeBusInterface,
                                                          PacketBuffer,
                                                          InputData,
                                                          &ByteCount );

            if ((Status == EFI_SUCCESS) && (ByteCount > 8)) {
                BlkMedia->ReadOnly = (InputData[3] & 0x80) != 0 ? TRUE : FALSE;
            }
        }

        if(pST->Hdr.Revision >= 0x0002001F) {
            //
            // For Atapi Devices, Default set the 1 for logical blocks per PhysicalBlock
            //
            BlkMedia->LogicalBlocksPerPhysicalBlock=1;
    
            //
            // For Atapi Devices, Default value set to 0 for Lowest Aligned LBA
            //
            BlkMedia->LowestAlignedLba=0;
    
            BlkMedia->OptimalTransferLengthGranularity=BlkMedia->BlockSize;
        }
    }

    pBS->FreePool( InputData );
    pBS->FreePool( PacketBuffer );
    return Status;
}

/**
    Issues Atapi Inquiry Command and returns the DATA.

    @param IDE_BUS_INIT_PROTOCOL    *IdeBusInitInterface
    @param UINT8    *InquiryData,
    @param UINT32   *InquiryDataSize

    @retval EFI_STATUS

**/

EFI_STATUS
AtapiInquiryData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    OUT UINT8                   *InquiryData,
    IN  OUT UINT16              *InquiryDataSize
)
{
    EFI_STATUS      Status;
    UINT8           *PacketBuffer;
    ATAPI_DEVICE    *AtapiDevice = IdeBusInterface->IdeDevice.AtapiDevice;


    Status = pBS->AllocatePool( EfiBootServicesData, 16, (VOID**)&PacketBuffer );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    ZeroMemory( PacketBuffer, 16 );
    PacketBuffer[0] = ATAPI_INQUIRY;
    PacketBuffer[1] = AtapiDevice->Lun << 5;
    PacketBuffer[4] = (UINT8) *InquiryDataSize;
    Status = GeneralAtapiCommandAndData( IdeBusInterface,
                                                  PacketBuffer,
                                                  InquiryData,
                                                  InquiryDataSize );
    pBS->FreePool( PacketBuffer );
    return Status;
}

/**
    Read/Write data from/to the ATAPI device

        
    @param IdeBusInterface 
    @param VOID *Buffer,
    @param UINTN    ByteCount,
    @param UINT64   LBA,
    @param UINT8    ReadWriteCommand,
    @param BOOLEAN  READWRITE

    @retval EFI_STATUS

    @note  
	1. Prepare ATAPI Command Packet
	2. Check for errors. If Media_Change, detect the new atapi media if present and return status accordingly.
	3. Read/write data if the command packet is issues successfully.
	4. Repeat from step 1 until all data has been read/written.

**/

EFI_STATUS
AtapiReadWritePio (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT64                  LBA,
    IN  UINT8                   ReadWriteCommand,
    IN  BOOLEAN                 READWRITE
)
{
    EFI_STATUS      Status;
    INTN            TotalNumberofBlocks;
    INTN            TransferLength;
    UINT16          BytesRead;
    UINTN           BytesRemainingTobeRead;
    ATAPI_DEVICE    *AtapiDevice    =   IdeBusInterface->IdeDevice.AtapiDevice;
    VOID            *TempBuffer =   Buffer;
    IO_REGS         Regs        =   IdeBusInterface->IdeDevice.Regs;

    //
    //Disable Interrupt
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );

    //
    //Check for CHK bit in status register before proceeding, if set give ATAPI reset
    //
    Status = CheckCHKonEntry( IdeBusInterface );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    TotalNumberofBlocks = ByteCount / AtapiDevice->BlockSize;

    for (; TotalNumberofBlocks >  0; TotalNumberofBlocks -= TransferLength ) {
        //
        //Clear the buffer
        //
        ZeroMemory( AtapiDevice->PacketBuffer, AtapiDevice->PacketSize );

        //
        //Calculate # of blocks to be transferred
        //
        if ( TotalNumberofBlocks > 0xffff ) {
            TransferLength = 0xffff;
        } else {
            TransferLength = TotalNumberofBlocks;
        }

        //		If the ATAPI device is old, below "ATAPI-3" standard, use one block
        //		per transfer. Some of the old devices don't update the ByteCount value
        //		before setting the DRQ bit. Instead of adding delay, we will transfer 1 block at a time.
        //		Even If this enabled, do it only for CDROM. LS120 write will be very slow if 1 block at time is written.
        //		if (IdeBusInterface->IdeDevice.IdentifyData.Major_Revision_80 < 3) TransferLength = 1;

        //		Update the buffer
        AtapiDevice->PacketBuffer[0] = ReadWriteCommand;
        AtapiDevice->PacketBuffer[1] = AtapiDevice->Lun << 5;
        AtapiDevice->PacketBuffer[2] = ((UINT32) LBA) >>  24;
        AtapiDevice->PacketBuffer[3] = ((UINT32) LBA) >> 16;
        AtapiDevice->PacketBuffer[4] = ((UINT16) LBA) >> 8;
        AtapiDevice->PacketBuffer[5] = ((UINT8) LBA) & 0xff;

        AtapiDevice->PacketBuffer[7] = (UINT8)( TransferLength >> 8 );            // MSB
        AtapiDevice->PacketBuffer[8] = (UINT8)( TransferLength & 0xff );      // LSB

        BytesRemainingTobeRead = TransferLength * AtapiDevice->BlockSize;
        Status = IssueAtapiPacketCommand( IdeBusInterface,
                                       (UINT16*) AtapiDevice->PacketBuffer,
                                       0,
                                       0xffff );

        if ( EFI_ERROR( Status )) {
            return Status;
        }

        do
        {
            //
            //Check for errors
            //
            Status = HandleAtapiError( IdeBusInterface );

            if ( Status != EFI_SUCCESS ) {
                //
                //Check if Device is getting ready. If yes, wait till it gets ready
                //    
                if ( AtapiDevice->Atapi_Status == BECOMING_READY ) {
                    Status = TestUnitReady( IdeBusInterface );
                }

                if ( Status == EFI_MEDIA_CHANGED ) {
                    Status = DetectAtapiMedia( IdeBusInterface );

                    // This may happen during initial power-up also. If ReinstallProtocol  needs to be done,
                    // then differentiate between power-up and other cases.
                    if ( Status == EFI_SUCCESS ) {
                        return EFI_MEDIA_CHANGED;    // Return Media Change
                    }
                }
                return Status;
            }

            //Check if DRQ asserted, else DEVICE error. Since BSY is cleared (HandleAtapiError) when control comes to this
            //place, a small delay is enough.
            Status = WaitforBitSet( IdeBusInterface->PciIO,
                                    Regs.CommandBlock.StatusReg,
                                    IDE_DRQ,
                                    DRQ_SET_TIMEOUT );

            if ( EFI_ERROR( Status )) {
                return EFI_DEVICE_ERROR;
            }

            if ( READWRITE ) {
                Status = WriteAtapiData( IdeBusInterface,
                                         TempBuffer,
                                         &BytesRead );
            }
            else {
                Status = ReadAtapiData( IdeBusInterface, TempBuffer, &BytesRead );
            }

            //
            //Update pointer
            //
            TempBuffer = (UINT8*)(TempBuffer) + BytesRead;       
            BytesRemainingTobeRead -= BytesRead;
        }   while ( BytesRemainingTobeRead );

        LBA += TransferLength;
        //
        //Check for errors
        //
        Status = HandleAtapiError( IdeBusInterface );

        if ( EFI_ERROR( Status )) {
            return Status;
        }
    }

    return EFI_SUCCESS;
}

/**
    Issues the ATAPI command and reads the data

    @param  IdeBusInterface 
    @param  UINT8   *PacketBuffer,
    @param  UINT8   *Buffer,
    @param  ByteCount 

    @retval EFI_STATUS

**/
EFI_STATUS
GeneralAtapiCommandAndData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT8                       *PacketBuffer,
    UINT8                       *Buffer,
    IN  OUT     UINT16          *ByteCount
)
{
    EFI_STATUS  Status;
    IO_REGS     Regs    =   IdeBusInterface->IdeDevice.Regs;

    Status = IssueAtapiPacketCommand( IdeBusInterface,
                                      (UINT16*)PacketBuffer,
                                      0,
                                      *ByteCount );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    //    
    //Check for errors
    //    
    Status = HandleAtapiError( IdeBusInterface );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    if ( *ByteCount ) {
        //
        //Check if DRQ asserted, else DEVICE error
        //
        Status = WaitforBitSet( IdeBusInterface->PciIO,
                                Regs.CommandBlock.StatusReg,
                                IDE_DRQ,
                                DRQ_TIMEOUT );

        if ( EFI_ERROR( Status )) {
            return EFI_DEVICE_ERROR;
        }

        //
        //Read the data
        //
        ReadAtapiData( IdeBusInterface, Buffer, ByteCount );
		

        //
        //Check for errors
        //
        Status = HandleAtapiError( IdeBusInterface );
        return Status;
    } else {
        *ByteCount = 0;
        return Status;
    }
}

/**
    Issues ATAPI Packet command and send the packet.

    @param  IdeBusInterface 
    @param  UINT16  *PacketBuffer,		// Pointer to packet
    @param  UINT16  ByteCount		    // Byte count Limit

    @retval EFI_STATUS

    @note   The Packet Command and the packet are sent. Error is not analyzed in this
            Routine. Once the packet is sent successfully, EFI_SUCCESS is returned.
            It is the callers responsibility to check the status.

**/
EFI_STATUS
IssueAtapiPacketCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINT16                  *PacketBuffer,
    IN  UINT8                   Features,
    IN  UINT16                  ByteCount
)
{
    IDE_DEVICE_INTERFACE    *IdeDevice  =   &(IdeBusInterface->IdeDevice);
    IO_REGS                 Regs        =   IdeBusInterface->IdeDevice.Regs;
    UINT8                   Data8;
    UINT16                  Data16;
    EFI_STATUS              Status;
    ATAPI_DEVICE            *AtapiDevice    =   IdeBusInterface->IdeDevice.AtapiDevice;

    //
    //	Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  IdeDevice->Device << 4 );
    // 
    //	Before Writing to any register check for BSY and DRQ bit. Should be zero
    //
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.ControlBlock.AlternateStatusReg,
                              IDE_BSY | IDE_DRQ,
                              gPlatformIdeProtocol->AtaPiBusyClearTimeout );

    if ( EFI_ERROR( Status )) {
        Status = IssueAtapiReset( IdeBusInterface, TRUE );

        if ( EFI_ERROR( Status )) {
            return EFI_DEVICE_ERROR;
        }
    }

    //
    //	Write Feature register. No OVL and DMA
    //    
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.FeatureReg,
                  Features );

    //
    //Write Byte Count
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,
                  (ByteCount & 0xff));

    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg,
                  (ByteCount >> 8));

    //
    //Write Command Register
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.CommandReg,
                  PACKET_COMMAND );

    //	Wait for 400nsec before reading the status. To accomplish it, read ATL_STATUS and ignore the result.
    //	Assumption is, this call will take at least 400nsec to complete.
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );
    //
    //	Check for BSY bit to be clear 30msec
    //
    Data16 = 30;
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.ControlBlock.AlternateStatusReg,
                              IDE_BSY,
                              (UINT32) Data16 );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }
    //
    //	Check for DRQ set
    //
    Status = WaitforBitSet( IdeBusInterface->PciIO,
                            Regs.CommandBlock.StatusReg,
                            IDE_DRQ,
                            (UINT32) Data16 );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //	Write the Data
    Status = IdeWriteMultipleWord( IdeBusInterface->PciIO,
                                   Regs.CommandBlock.DataReg,
                                   AtapiDevice->PacketSize / 2,
                                   PacketBuffer );

    return Status;
}

/**
    Issues ATAPI Reset command

        
    @param IdeBusInterface 
    @param IN BOOLEAN   TESTUNITREADY

    @retval EFI_STATUS

    @note  
    If TESTUNITREADY is TRUE, after ATAPI Reset, TestUnitReady command is issued.
    Generally after DEVICE RESET command, ATAPI device respond to and command with
    POWER ON RESET OCCURRED and followed by MEDIA CHANGE.

**/
EFI_STATUS
IssueAtapiReset (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  BOOLEAN                 TESTUNITREADY
)

{
    IDE_DEVICE_INTERFACE    *IdeDevice  =   &(IdeBusInterface->IdeDevice);
    IO_REGS                 Regs        =   IdeBusInterface->IdeDevice.Regs;
    UINT8                   Data8;
    EFI_STATUS              Status;

    PROGRESS_CODE( DXE_REMOVABLE_MEDIA_RESET );

    //
    // Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  IdeDevice->Device << 4 );

    //
    //Disable Interrupt
    //    
    IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );

    //
    //Issue Device reset Command
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.CommandReg,
                  DEVICE_RESET );

    //
    //Wait for 400nsec before reading the status. To accomplish it, read ATL_STATUS and ignore the result
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    //
    //Check for BSY bit to be clear
    //
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.CommandBlock.StatusReg,
                              IDE_BSY,
                              gPlatformIdeProtocol->AtaPiResetCommandTimeout );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

   IdeReadByte( IdeBusInterface->PciIO,
                     Regs.CommandBlock.LBAMidReg,
                     &Data8 );

    if ( Data8 == (ATAPI_SIGNATURE & 0xff)) {
        IdeReadByte( IdeBusInterface->PciIO,
                     Regs.CommandBlock.LBAHighReg,
                     &Data8 );

        if ( Data8 == (ATAPI_SIGNATURE >> 8)) {
            if ( TESTUNITREADY ) {
                return TestUnitReady( IdeBusInterface );
            }
        }
    }
    return EFI_SUCCESS;
}

/**
    Handle Atapi error if any.

    @param IdeBusInterface 

    @retval EFI_STATUS

    @note   If DF and CHK bits are not set, return EFI_SUCCESS. If either
            one of the bits are set, analyze the error and return appropriate
            error message.

**/
EFI_STATUS
HandleAtapiError (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    IDE_DEVICE_INTERFACE    *IdeDevice  =   &(IdeBusInterface->IdeDevice);
    IO_REGS                 Regs        =   IdeBusInterface->IdeDevice.Regs;
    ATAPI_DEVICE            *AtapiDevice    =   IdeBusInterface->IdeDevice.AtapiDevice;
    UINT8                   Data8;
    EFI_STATUS              Status;
    UINT8                   *SenseData;
    UINT8                   *SensePacket;
    UINT16                  BytesRead;

    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  IdeDevice->Device << 4 );

    //
    //Read the status register
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    //
    //Check if BSY clear else DEVICE error
    //
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.CommandBlock.StatusReg,
                              IDE_BSY,
                              gPlatformIdeProtocol->AtaPiBusyClearTimeout );

    IdeBusInterface->AtapiSenseDataLength = 0;

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Read the status register
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    //
    //Check for DF
    //
    if ( Data8 & IDE_DF ) {
        AtapiDevice->Atapi_Status = DEVICE_ERROR;
        return EFI_DEVICE_ERROR;
    }

    //
    //Check for CHK
    //
    if ( Data8 & IDE_CHK ) {
        AtapiDevice->Atapi_Status = DEVICE_ERROR;
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    256,
                                   (VOID**)&SenseData
                                   );

        if ( EFI_ERROR( Status )) {
            return Status;
        }

        Status = pBS->AllocatePool( EfiBootServicesData,
                                    16,
                                    (VOID**)&SensePacket
                                    );

        if ( EFI_ERROR( Status )) {
            pBS->FreePool( SenseData );
            return Status;
        }

        //
        //Update the buffer
        //
        ZeroMemory( SenseData,  256 );
        ZeroMemory( SensePacket, 16 );

        SensePacket[0] = ATAPI_REQUEST_SENSE;
        SensePacket[4] = 0xff;

        Status = IssueAtapiPacketCommand( IdeBusInterface,
                                          (UINT16*) SensePacket,
                                          0,
                                          256 );

        if ( EFI_ERROR( Status )) {
            goto exit_HandleAtapiError;
        }


        //
        //Wait for BSY to get cleared
        //
        Status = WaitforBitClear( IdeBusInterface->PciIO,
                                  Regs.CommandBlock.StatusReg,
                                  IDE_BSY,
                                  gPlatformIdeProtocol->AtaPiBusyClearTimeout );

        if ( EFI_ERROR( Status )) {
            goto exit_HandleAtapiError;
        }

        //    
        //read the status register
        //
        IdeReadByte( IdeBusInterface->PciIO,
                     Regs.ControlBlock.AlternateStatusReg,
                     &Data8 );

        Status = EFI_DEVICE_ERROR;

        //
        //Check for DF and CHK
        //
        if ( Data8 & (IDE_DF | IDE_CHK)) {
            goto exit_HandleAtapiError_with_Reset;
        }

        //
        //Check if DRQ asserted, else DEVICE error
        //
        if ( !(Data8 & IDE_DRQ)) {
            goto exit_HandleAtapiError_with_Reset;
        }

        //
        //Read the data
        //
        ReadAtapiData( IdeBusInterface, (UINT16*)SenseData, &BytesRead );
		


        Status = EFI_DEVICE_ERROR;              // Default Value
        AtapiDevice->Atapi_Status = DEVICE_ERROR;

        //
        // Store the SenseData which would be used by ScsiPassThruAtapi PassThru Interface.
        //
        pBS->CopyMem( IdeBusInterface->AtapiSenseData, SenseData, BytesRead);
        IdeBusInterface->AtapiSenseDataLength = (UINT8)BytesRead;

        if (((SenseData[2] & 0xf) == 2) && (SenseData[12] == 0x3a)) {
            Status = EFI_NO_MEDIA;
            AtapiDevice->Atapi_Status = MEDIUM_NOT_PRESENT;
        }

        if (((SenseData[2] & 0xf) == 2) && (SenseData[12] == 0x04)
            && (SenseData[13] == 0x01)) {
            Status = EFI_MEDIA_CHANGED;
            AtapiDevice->Atapi_Status = BECOMING_READY;
        }

        if (((SenseData[2] & 0xf) == 6) && (SenseData[12] == 0x28)) {
            Status = EFI_MEDIA_CHANGED;
            AtapiDevice->Atapi_Status = MEDIA_CHANGED;
        }

        if (((SenseData[2] & 0xf) == 7) && (SenseData[12] == 0x27)) {
            Status = EFI_WRITE_PROTECTED;
            AtapiDevice->Atapi_Status = WRITE_PROTECTED_MEDIA;
        }

        if (((SenseData[2] & 0xf) == 6) && (SenseData[12] == 0x29)) {
            AtapiDevice->Atapi_Status = POWER_ON_OR_DEVICE_RESET;
        }

        if (((SenseData[2] & 0xf) == 5) && (SenseData[0] == 0x70)) {
            AtapiDevice->Atapi_Status = ILLEGAL_REQUEST;
        }

exit_HandleAtapiError:
        pBS->FreePool( SenseData );
        pBS->FreePool( SensePacket );
        return Status;

exit_HandleAtapiError_with_Reset:
        IssueAtapiReset( IdeBusInterface, TRUE );
        goto    exit_HandleAtapiError;
    }

    AtapiDevice->Atapi_Status = EFI_SUCCESS;
    return EFI_SUCCESS;
}

/**
    Issues Start/Stop unit Command

    @param  IdeBusInterface 

    @retval EFI_STATUS  EFI_SUCCESS : If Media is accessible
    @retval EFI_NO_MEDIA
    @retval EFI_MEDIA_CHANGED
    @retval EFI_DEVICE_ERROR

**/
EFI_STATUS
TestUnitReady (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_STATUS      Status;
    UINT8           *Packet;
    ATAPI_DEVICE    *AtapiDevice    =   IdeBusInterface->IdeDevice.AtapiDevice;
    UINT16          ByteCount   =   0;
    UINT16          LoopCount;


    Status = pBS->AllocatePool( EfiBootServicesData, 16, (VOID**)&Packet );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    ZeroMemory( Packet, 16 );
    Packet[0] = ATAPI_TEST_UNIT_READY;
    Packet[1] = AtapiDevice->Lun << 5;

    for ( LoopCount  = 0; LoopCount < 1000; LoopCount++ )             
    {
        Status = GeneralAtapiCommandAndData( IdeBusInterface,
                                             Packet,
                                             NULL,
                                             &ByteCount );

        if ( Status == EFI_SUCCESS ) {
            break;
        }

        if ( AtapiDevice->Atapi_Status == MEDIUM_NOT_PRESENT ) {
            break;
        }

        if ( AtapiDevice->Atapi_Status == MEDIA_CHANGED ) {
            break;
        }
        pBS->Stall( 10000 ); // 10msec
    }

    pBS->FreePool( Packet );
    return Status;
}

/**
    Find the ODD device Type and return it

    @param IdeBusInterface 

    @retval OUT UINT16  *OddType

**/
EFI_STATUS
GetOddType (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT UINT16              *OddType
)
{
    EFI_STATUS  Status;
    UINT8       *PacketBuffer;
    UINT8       *ProfileData;
    UINT16      ProfileDataSize = 16;

    Status = pBS->AllocatePool( EfiBootServicesData, 16, (VOID**)&PacketBuffer );

    if ( EFI_ERROR( Status )) {
        return Status;
    }
    Status = pBS->AllocatePool( EfiBootServicesData, 16, (VOID**)&ProfileData );

    if ( EFI_ERROR( Status )) {
        return Status;
    }
    ZeroMemory( ProfileData,  16 );
    ZeroMemory( PacketBuffer, 16 );
    PacketBuffer[0] = ATAPI_GET_CONFIGURATION;
    //
    // Get the Feature Descriptor.
    //
    PacketBuffer[1] = FEATURE_DISCRIPTOR;
    //
    // Get the Profile list
    //
    PacketBuffer[3] = GET_PROFILE_LIST;
    //
    // Response Data Size
    //
    PacketBuffer[8] = 0x10;

    Status = GeneralAtapiCommandAndData( IdeBusInterface,
                                         PacketBuffer,
                                         ProfileData,
                                         &ProfileDataSize );

    if ( !EFI_ERROR( Status )) {
        //
        // Get the Profile Number
        //
        *OddType = (UINT16 )(((ProfileData[sizeof(GET_CONFIGURATION_HEADER) + 4]) << 8) 
                                + ProfileData[sizeof(GET_CONFIGURATION_HEADER) + 5] );
    }

    pBS->FreePool( PacketBuffer );
    pBS->FreePool( ProfileData );

    return Status;
}

/**
    Find the ODD Loading Type information and return it

    @param IdeBusInterface 

    @retval OUT UINT16  *OddLoadingType

**/
EFI_STATUS
GetOddLoadingType (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT UINT8               *OddLoadingType
)
{
    EFI_STATUS   Status;
    UINT8        *PacketBuffer;
    UINT8        *ProfileData;
    UINT16       ProfileDataSize = 16;

    Status = pBS->AllocatePool( EfiBootServicesData, 16, (VOID**)&PacketBuffer );

    if ( EFI_ERROR( Status )) {
        return Status;
    }
    Status = pBS->AllocatePool( EfiBootServicesData, 16, (VOID**)&ProfileData );

    if ( EFI_ERROR( Status )) {
        return Status;
    }
    ZeroMemory( ProfileData,  16 );
    ZeroMemory( PacketBuffer, 16 );
    PacketBuffer[0] = ATAPI_GET_CONFIGURATION;
    //
    // Get the Feature Descriptor.
    //
    PacketBuffer[1] = FEATURE_DISCRIPTOR;
    //
    // Get the Removable Medium feature 
    //
    PacketBuffer[3] = GET_REMOVEABLE_MEDIUM_FEATURE;
    //
    // Response Data Size
    //
    PacketBuffer[8] = 0x10;

    Status = GeneralAtapiCommandAndData( IdeBusInterface,
                                         PacketBuffer,
                                         ProfileData,
                                         &ProfileDataSize );

    if ( !EFI_ERROR( Status )) {
        //
        // Get the ODD Loading Type
        //
        *OddLoadingType=(UINT8 )(((ProfileData[sizeof(GET_CONFIGURATION_HEADER)+4]) & 0xE0) >> 5);
    }

    pBS->FreePool( PacketBuffer );
    pBS->FreePool( ProfileData );

    return Status;
}


/**
    Check for CHK bit. If set Issue ATAPI reset.

    @param IdeBusInterface 

    @retval EFI_STATUS

**/
EFI_STATUS
CheckCHKonEntry (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    IO_REGS                 Regs    =   IdeBusInterface->IdeDevice.Regs;
    IDE_DEVICE_INTERFACE    *IdeDevice  =   &(IdeBusInterface->IdeDevice);
    UINT8                   Data8;

    //    
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  IdeDevice ->Device << 4 );

    //
    //Disable Interrupt
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );

    //
    //Read Status
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    if ( Data8 & IDE_CHK ) {
        return (IssueAtapiReset( IdeBusInterface, TRUE ));
    }

    return EFI_SUCCESS;
}

/**
    Read data from the data port based on Byte count value

    @param IdeBusInterface 
    @param AtapiCommand 
    @param BytesRead 

    @retval EFI_STATUS

**/
EFI_STATUS
ReadAtapiData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    OUT void                    *Data,
    OUT UINT16                  *BytesRead
)
{
    IO_REGS    Regs = IdeBusInterface->IdeDevice.Regs;
    UINT8      Data8;
    UINT16     ByteCount = 0;
    EFI_STATUS Status;

    //
    //Get the number of Bytes to read
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg, &Data8 );
    ByteCount = Data8 << 8;
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,  &Data8 );
    ByteCount |= Data8;

    *BytesRead = ByteCount;
    Status     = IdeReadMultipleWord( IdeBusInterface->PciIO,
                                      Regs.CommandBlock.DataReg,
                                      ByteCount / 2,
                                      Data );

    return Status;
}

/**
    Write data to the data port based on Byte count value

        
    @param IdeBusInterface 
    @param AtapiCommand 
    @param BytesRead 

    @retval EFI_STATUS

**/
EFI_STATUS
WriteAtapiData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    OUT void                    *Data,
    OUT UINT16                  *BytesRead
)
{
    IO_REGS    Regs = IdeBusInterface->IdeDevice.Regs;
    UINT8      Data8;
    UINT16     ByteCount = 0;
    EFI_STATUS Status;

    //
    //Get the number of Bytes to Write
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg, &Data8 );
    ByteCount = Data8 << 8;
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,  &Data8 );
    ByteCount |= Data8;

    *BytesRead = ByteCount;
    Status     = IdeWriteMultipleWord( IdeBusInterface->PciIO,
                                       Regs.CommandBlock.DataReg,
                                       ByteCount / 2,
                                       Data );

    return Status;
}

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

