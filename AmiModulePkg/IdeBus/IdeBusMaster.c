//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file IdeBusMaster.c
    Ide BusMaster Services

**/

//---------------------------------------------------------------------------

#include "IdeBus.h"

//---------------------------------------------------------------------------

extern PLATFORM_IDE_PROTOCOL    *gPlatformIdeProtocol;
extern VOID *gDescriptorBuffer;

/**
    Issues ATA/ATAPI Command using Bus master

    @param IdeBusInterface 
    @param VOID     *Buffer,
    @param UINT32   ByteCount,
    @param UINT8    Features,
    @param UINT32   SectorCountIn,
    @param UINT8    LbaLow,
    @param UINT8    LbaLowExp,
    @param UINT8    LbaMid,
    @param UINT8    LbaMidExp,
    @param UINT8    LbaHigh,
    @param UINT8    LbaHighExp,
    @param UINT8    Device,
    @param UINT8    Command,
    @param BOOLEAN  ReadWrite

    @retval *Buffer

**/
EFI_STATUS 
AtaAtapiDmaDataCommand(
    IN AMI_IDE_BUS_PROTOCOL *IdeBusInterface,
    IN OUT VOID         *Buffer,
    IN UINTN            ByteCount,
    IN UINT8            Features,
    IN UINT32           SectorCountIn,
    IN UINT8            LbaLow,
    IN UINT8            LbaLowExp,
    IN UINT8            LbaMid,
    IN UINT8            LbaMidExp,
    IN UINT8            LbaHigh,
    IN UINT8            LbaHighExp,
    IN UINT8            Device,
    IN UINT8            Command,
    IN BOOLEAN          ReadWrite
)
{
    IDE_DEVICE_INTERFACE *IdeDevice  = &(IdeBusInterface->IdeDevice);
    UINT64               Lba = 0;
    UINT64               LbaHighDword = 0;
    EFI_STATUS           Status;

    if ( !gPlatformIdeProtocol->IdeBusMasterSupport ) {
        return EFI_UNSUPPORTED;
    }

    if ( ! (DMACapable( IdeBusInterface ))) {
        return EFI_UNSUPPORTED;
    }

    if ( Check48BitCommand( Command )) {
        //
        //	if 48 Bit LBA form Upper Dword
        //
        LbaHighDword |= LbaHighExp;
        LbaHighDword = ( Shl64(( Shl64( LbaHighDword, 8)| LbaMidExp), 8)| LbaLowExp);
    }

    //
    //	Complete LBA	 
    //
    Lba |= LbaHigh;
    Lba = (( Shl64(( Shl64( Lba, 8) | LbaMid ), 8)| LbaLow)| Shl64( LbaHighDword, 24 ));

    if(IdeDevice->DeviceType == ATA ) {

        Status = AtaReadWriteBusMaster( IdeBusInterface,
                                        Buffer,
                                        ByteCount,
                                        Lba,
                                        Command,
                                        ReadWrite 
                                        );
    } else {

        Status = AtapiReadWriteBusMaster(
                                        IdeBusInterface,
                                        Buffer,
                                        ByteCount,
                                        Lba,
                                        Command,
                                        ReadWrite 
                                        );
    }

    return Status;
}

/**
    Issues Read/Write Command and Read/Write the data from/to the ATA device
    using BusMaster

    @param IdeBusInterface 
    @param VOID *Buffer,
    @param UINT32   ByteCount,
    @param UINT64   LBA

    @retval *Buffer

    @note  
        1. Create Descriptor Table
        2. Issue ATA Read/Write command. Enable BusMastering
        3. Wait for Data Transfer
        4. Check for errors
        5. If success, check if any more data need to transferred, if yes, goto step 1

**/
EFI_STATUS
AtaReadWriteBusMaster (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT64                  LBA,
    IN  UINT8                   ReadWriteCommand,
    IN  BOOLEAN                 READWRITE
)
{
    EFI_STATUS  Status;
    EFI_STATUS  DMAStatus;
    UINTN       RemaingByteCount;
    UINTN       Total_Number_Of_Sectors;
    UINTN       MaxSectorCount;
    UINTN       CurrentSectorCount;
    UINTN       CurrentByteCount;
    UINT8       *TempBuffer;
    IO_REGS     Regs    =   IdeBusInterface->IdeDevice.Regs;
    UINTN       DMATimeout;
    UINTN       DescriptorBuffer;
    UINT8       Data8;
    UINT32      SectorSize  =   ATA_SECTOR_BYTES;
    BOOLEAN     SectorGTBytes   =   FALSE;
    UINT32      Timeout = 0;
    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.DeviceReg, IdeBusInterface->IdeDevice.Device << 4 );

    if ( Check48BitCommand( ReadWriteCommand )) {
        MaxSectorCount = MAX_SECTOR_COUNT_PIO_48BIT;
    }
    else {
        MaxSectorCount = MAX_SECTOR_COUNT_PIO;
    }

    //
    // Calculate Sector Size
    //
    if((IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT14) && // WORD 106 valid? - BIT 14 - 1
       (!(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT15)) && // WORD 106 valid? - BIT 15 - 0
       (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT12)) { // WORD 106 bit 12 - Sectorsize > 256 words
        // The sector size is in words 117-118.
        SectorSize = (UINT32)(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[13] + \
                              (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[14] << 16)) * 2;
    }

    Total_Number_Of_Sectors = ByteCount / SectorSize;         //512

    // If the caller is requesting less bytes than one sector, we need to
    // allocate space for one sector.
    if ((ByteCount < SectorSize) && (ByteCount > 0)) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    SectorSize,
                                    (VOID**)&TempBuffer );
        if ( EFI_ERROR( Status )) {
            return EFI_OUT_OF_RESOURCES;
        }
        SectorGTBytes = TRUE;
        Total_Number_Of_Sectors = 1;
    } else {
        TempBuffer = Buffer;
    }

    do
    {
        if ( Total_Number_Of_Sectors > MaxSectorCount ) {
            CurrentSectorCount = 0;
        }
        else {
            CurrentSectorCount = Total_Number_Of_Sectors;
        }
        CurrentByteCount = (CurrentSectorCount == 0 ? MaxSectorCount : CurrentSectorCount) * SectorSize;

        DescriptorBuffer = (UINTN) gDescriptorBuffer;
        Status           = CreateDescriptorTable( &DescriptorBuffer, TempBuffer, CurrentByteCount, &RemaingByteCount );

        if ( EFI_ERROR( Status )) {
            return Status;
        }

        Status=InitBusMasterRegisters( IdeBusInterface, DescriptorBuffer, READWRITE );		
		ASSERT_EFI_ERROR(Status);

        MaskandSaveInterrupt( IdeBusInterface );
        //
        //Enable Interrupt
        //
        IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 0 );

        Status = IssueAtaReadWriteCommand( IdeBusInterface, LBA, (UINT32)CurrentSectorCount, ReadWriteCommand, 0 );

        if ( EFI_ERROR( Status )) {
            return EFI_DEVICE_ERROR;
        }

        StartStopBusMastering( IdeBusInterface, TRUE );

        if(gPlatformIdeProtocol->CommandTimeout == 0) {
            Timeout = gPlatformIdeProtocol->DmaAtaCompleteCommandTimeout;
        } else {
            Timeout = (UINT32)gPlatformIdeProtocol->CommandTimeout;
        }
        
        //Timeout = 5000msec + Number of Sectors * 2
        DMATimeout = ((CurrentSectorCount == 0 ? MaxSectorCount : CurrentSectorCount) << 1) + Timeout;

        DMAStatus = WaitforDMAtoCompletion( IdeBusInterface, DMATimeout );

        Status = HandleATABMErrors( IdeBusInterface );

        StartStopBusMastering( IdeBusInterface, FALSE );
        //
        //Disable Interrupt
        //
        IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );
        //
        //Clear interrupt status
        //
        IdeReadByte( IdeBusInterface->PciIO,
                     IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMStatusRegister,
                     &Data8 );

        if ( Data8 & 4 ) {
            IdeWriteByte( IdeBusInterface->PciIO,
                          IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMStatusRegister,
                          Data8 );
        }

        RestoreInterrupt( IdeBusInterface );

        //
        //Check the status from WaitforDMAtoCompletion. More Descriptors are never formed. So EFI_SUCCESS_ACTIVE_SET is an error.
        //
        if ( EFI_ERROR( DMAStatus )) {
            return EFI_DEVICE_ERROR;
        }

        if ( EFI_ERROR( Status )) {
            return EFI_DEVICE_ERROR;
        }

        TempBuffer              += CurrentByteCount;
        Total_Number_Of_Sectors -= (CurrentSectorCount == 0 ? MaxSectorCount : CurrentSectorCount);
        LBA                     += (CurrentSectorCount == 0 ? MaxSectorCount : CurrentSectorCount);
    } while ( Total_Number_Of_Sectors );

    if (SectorGTBytes) {
        pBS->CopyMem( Buffer, TempBuffer, ByteCount);
        pBS->FreePool( TempBuffer );
    }

    return EFI_SUCCESS;
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
AtapiReadWriteBusMaster (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT64                  LBA,
    IN  UINT8                   ReadWriteCommand,
    IN  BOOLEAN                 READWRITE
)
{
    EFI_STATUS      Status;
    EFI_STATUS      DMAStatus;
    INTN            TotalNumberofBlocks;
    INTN            TransferLength;
    UINTN           BytesRemainingTobeRead;
    UINTN           RemaingByteCount;
    ATAPI_DEVICE    *AtapiDevice    =   IdeBusInterface->IdeDevice.AtapiDevice;
    VOID            *TempBuffer     =   Buffer;
    IO_REGS         Regs            =   IdeBusInterface->IdeDevice.Regs;
    UINT8           Data8;
    UINTN           DescriptorBuffer    =   (UINTN)gDescriptorBuffer;

    //
    //Check for CHK bit in status register before proceeding, if set give ATAPI reset
    //
    Status = CheckCHKonEntry( IdeBusInterface );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    TotalNumberofBlocks = ByteCount / AtapiDevice->BlockSize;

    for (; TotalNumberofBlocks >  0; TotalNumberofBlocks -= TransferLength )
    {
        //
        //Clear the buffer
        //
        ZeroMemory( AtapiDevice->PacketBuffer, AtapiDevice->PacketSize );

        //
        //Calculate # of blocks to be transferred
        //
        if ( TotalNumberofBlocks > 0xffff ) {
            TransferLength = 0xffff;
        }
        else {
            TransferLength = TotalNumberofBlocks;
        }

        //Update the buffer
        AtapiDevice->PacketBuffer[0] = ReadWriteCommand;
        AtapiDevice->PacketBuffer[1] = AtapiDevice->Lun << 5;
        AtapiDevice->PacketBuffer[2] = ((UINT32) LBA) >>  24;
        AtapiDevice->PacketBuffer[3] = ((UINT32) LBA) >> 16;
        AtapiDevice->PacketBuffer[4] = ((UINT16) LBA) >> 8;
        AtapiDevice->PacketBuffer[5] = ((UINT8) LBA) & 0xff;

        AtapiDevice->PacketBuffer[7] = (UINT8) ( TransferLength >> 8 );           // MSB
        AtapiDevice->PacketBuffer[8] = (UINT8) ( TransferLength & 0xff );     // LSB

        BytesRemainingTobeRead = TransferLength * AtapiDevice->BlockSize;

        Status = CreateDescriptorTable( &DescriptorBuffer, TempBuffer, BytesRemainingTobeRead, &RemaingByteCount );

        if ( EFI_ERROR( Status )) {
            return Status;
        }

        Status=InitBusMasterRegisters( IdeBusInterface, DescriptorBuffer, READWRITE );
        ASSERT_EFI_ERROR(Status);

        MaskandSaveInterrupt( IdeBusInterface );

        //Enable Interrupt
        IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 0 );

        Status = IssueAtapiPacketCommand( IdeBusInterface, 
                                        (UINT16*) AtapiDevice->PacketBuffer, 
                                        BUSMASTER_DMA, 
                                        0xffff );

        if ( EFI_ERROR( Status )) {
            return Status;
        }

        StartStopBusMastering( IdeBusInterface, TRUE );

        DMAStatus=WaitforDMAtoCompletion( IdeBusInterface, gPlatformIdeProtocol->DmaAtaPiCompleteCommandTimeout );

        ASSERT_EFI_ERROR(DMAStatus);



        //Check for errors
        Status = HandleAtapiError( IdeBusInterface );

        StartStopBusMastering( IdeBusInterface, FALSE );

        //Disable Interrupt
        IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );

        //Clear interrupt status
        IdeReadByte( IdeBusInterface->PciIO,
                     IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMStatusRegister,
                     &Data8 );

        if ( Data8 & 4 ) {
            IdeWriteByte( IdeBusInterface->PciIO,
                          IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMStatusRegister,
                          Data8 );
        }

        RestoreInterrupt( IdeBusInterface );

        if ( Status != EFI_SUCCESS ) {
            //Some error has occurred
            //Check if Device is getting ready. If yes, wait till it gets ready
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

        //
        //If WaitforDMAtoCompletion returns a error, read the status register again to make sure it is indeed a failure.
        //
        IdeReadByte( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMStatusRegister, &Data8 );

        if ( Data8 & (BM_ERROR | BM_ACTIVE)) {
            return EFI_DEVICE_ERROR;
        }

        //Update pointer
       TempBuffer = (UINT8*)(TempBuffer) + BytesRemainingTobeRead;   
        LBA += TransferLength;
    }

    return EFI_SUCCESS;
}

/**
    Creates a IDE BUS master Descriptor Table

        
    @param DescriptorBuffer // 128KB buffer
    @param StartAddress 
    @param ByteCount 
    @param FinalByteCount 

    @retval DescriptorBuffer, EFI_STATUS

    @note  
        1. Create a Physical Region Descriptor for IDE  Bus Master.
        2. FinalByteCount will be the total # of Bytes that can be transferred
        using this Descriptor. Most of the time FinalByteCount will be equal to ByteCount.
        If the Descriptor Buffer exceeds 64K, then partial transfer will take place.


**/

EFI_STATUS
CreateDescriptorTable (
    IN  OUT UINTN   *DescriptorBuffer,
    IN  UINT8       *StartAddress,
    IN  UINTN       ByteCount,
    OUT UINTN       *RemainingByteCount
)
{
    UINT16                      Index;
    UINTN                       Address = (UINTN)StartAddress;
    BUS_MASTER_DESCRIPTOR_TABLE *DescriptorTable;

    //64KB alignment
    *DescriptorBuffer &= 0xffff0000;
    *DescriptorBuffer += 0x10000;
    DescriptorTable    =        (BUS_MASTER_DESCRIPTOR_TABLE*)(*DescriptorBuffer);


    //Address should be DWORD aligned
    if ( Address & 0x3 ) {
        return EFI_INVALID_PARAMETER;
    }

    //Restore Address
    Address = (UINTN)StartAddress;

    for ( Index = 0; Index <  0x10000 / 8; Index++ )
    {
        DescriptorTable[Index].BaseAddress = (UINT32) Address;
        DescriptorTable[Index].Flag        = 0;
        //
        // if Start address is not 64KB aligned, then byte count cannot be 64KB (Data transfer cannot cross 64KB boundary
        //
        if ( Address & 0xffff ) {
            DescriptorTable[Index].ByteCount = 0x10000 - (UINT16)( Address & 0xffff );

            if ( DescriptorTable[Index].ByteCount > ByteCount ) {
                DescriptorTable[Index].ByteCount = (UINT16)ByteCount;
            }
            ByteCount -= DescriptorTable[Index].ByteCount;
            Address   += DescriptorTable[Index].ByteCount;

            if ( ByteCount == 0 ) {
                break;
            }
        }
        else {                                    // address is 64KB aligned
            if ( ByteCount >= 0x10000 ) {
                DescriptorTable[Index].ByteCount = 0;
                ByteCount                       -= 0x10000;
                Address                         += 0x10000;

                if ( ByteCount == 0 ) {
                    break;
                }
            }
            else {
                DescriptorTable[Index].ByteCount = (UINT16) ByteCount;
                ByteCount                        = 0;
                break;
            }
        }
    }

    //
    //Update EOT
    //
    DescriptorTable[Index].Flag = 0x8000;

    *RemainingByteCount = ByteCount;

    return EFI_SUCCESS;
}

/**
    Initialize Bus Master registers

        
    @param IdeBusInterface 
    @param DescriptorBuffer 
    @param ReadWrite 

    @retval DescriptorBuffer, EFI_STATUS

    @note  
        1. Create a Physical Region Descriptor for IDE  Bus Master.
        2. FinalByteCount will be the total # of Bytes that can be transferred
        using this Descriptor. Most of the time FinalByteCount will be equal to ByteCount.
        If the Descriptor Buffer exceeds 64K, then partial transfer will take place.

**/

EFI_STATUS
InitBusMasterRegisters (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT UINTN               DescriptorBuffer,
    IN  BOOLEAN                 ReadWrite
)

{
    IO_REGS Regs = IdeBusInterface->IdeDevice.Regs;
    UINT8   Data8;

    //
    //Update Read/Write Control and clear Start/Stop bit
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMCommandRegister, (ReadWrite == FALSE ? 1 : 0) << 3 );

    //
    //Clear the status
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMStatusRegister, &Data8 );
    Data8 |= BM_INTERRUPT | BM_ERROR;
    IdeWriteByte( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMStatusRegister, Data8 );

    //
    //Update Descriptor Table Pointer
    //
    IdeWriteDword( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMDescriptorTablePointerReg, (UINT32) DescriptorBuffer );

    return EFI_SUCCESS;
}

/**
    Start/Stop Bus mAstering

        
    @param IdeBusInterface 
    @param StartStop //	Start = TRUE

    @retval EFI_STATUS

    @note Don't destroy Read/Write Control bit

**/

EFI_STATUS
StartStopBusMastering (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  BOOLEAN                 StartStop
)
{
    IO_REGS Regs = IdeBusInterface->IdeDevice.Regs;
    UINT8   Data8;

    //
    //Update  Start/Stop bit in Command Register
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMCommandRegister, &Data8 );
    Data8 &= 0xFE;
    Data8 |= (StartStop == TRUE ? 1 : 0);
    IdeWriteByte( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMCommandRegister, Data8 );
    return EFI_SUCCESS;
}

/**
    Start/Stop Bus mAstering

        
    @param IdeBusInterface 
    @param TimeDelay (msec)

    @retval EFI_STATUS   Success : If DMA completes with or with out Error.
    @retval EFI_DEVICE_ERROR	 : If timed out.

    @note  
        1. Check for Interrupt bit set. If yes, command completed.
        2. Check for Active bit to go Zero. If yes, command completed.
        3. Waits for a max. of TimeDelay/10,000 second for the command to get completed.

**/
EFI_STATUS
WaitforDMAtoCompletion (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINTN                   TimeDelay
)
{
    UINT8   Data8;
    UINT8   Index2;
    UINT32  Index;
    IO_REGS Regs = IdeBusInterface->IdeDevice.Regs;

    for ( Index = 0; Index < TimeDelay; Index++ )
    {
        for ( Index2 = 0; Index2 < 10; Index2++ )
        {
            IdeReadByte( IdeBusInterface->PciIO, Regs.BusMasterBlock.BMStatusRegister, &Data8 );

            if ((Data8 & (BM_INTERRUPT | BM_ACTIVE)) == 4 ) {
                return EFI_SUCCESS;
            }

            if ((Data8 & (BM_INTERRUPT | BM_ACTIVE)) == 5 ) {
                return EFI_SUCCESS_ACTIVE_SET;
            }

            if ((Data8 & BM_ERROR) == 2 ) {
                return EFI_DEVICE_ERROR;
            }
            pBS->Stall( 100 );                           // 100 usec
        }
    }

    return EFI_DEVICE_ERROR;
}

/**
    Check for any errors

    @param IdeBusInterface 

    @retval EFI_STATUS

**/
EFI_STATUS
HandleATABMErrors (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_STATUS  Status;

    Status = WaitForCmdCompletion( IdeBusInterface );
    return Status;
}

/**
    Mask IDE interrupt

    @param IdeBusInterface 

    @retval 

**/
EFI_STATUS
MaskandSaveInterrupt (
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    return EFI_SUCCESS;
}

/**
    Restore the Interrupt mask

    @param IdeBusInterface 

    @retval 

**/
EFI_STATUS
RestoreInterrupt (
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    return EFI_SUCCESS;
}

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

