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

/** @file Ata.c
    ATA Services

**/

//---------------------------------------------------------------------------

#include "IdeBus.h"

//---------------------------------------------------------------------------

#if ( defined(BOOT_SECTOR_WRITE_PROTECT) && (BOOT_SECTOR_WRITE_PROTECT != 0) )
#include <Protocol/AmiBlockIoWriteProtection.h>
extern AMI_BLOCKIO_WRITE_PROTECTION_PROTOCOL *AmiBlkWriteProtection;
#endif

extern PLATFORM_IDE_PROTOCOL    *gPlatformIdeProtocol;

/**
    Detects the ATA/ATAPI device

    @param  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface;

    @retval EFI_STATUS

    @note  
    Here is the control flow of this function:
     1. If controller not present return EFI_NOT_FOUND
     2. If IDE_BSY bit not clear and IDE_DRDY not set, return EFI_DEVICE_ERROR
     3. If Identify command fails, return EFI_NOT_FOUND
     4. Else return Success

**/
EFI_STATUS
DetectIdeDevice (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_STATUS  Status;
    UINT16      Index;
    IO_REGS     Regs = IdeBusInterface->IdeDevice.Regs;
    UINT8       Data8;

    //
    // Check if the controller is present
    //
    Status = ControllerPresence( IdeBusInterface );

    if ( Status == EFI_NOT_FOUND ) {
        //
        // Status Register is 0xff
        //
        return EFI_NOT_FOUND;  
    }

    if ( Status == EFI_DEVICE_ERROR ) {
        IdeSoftReset( IdeBusInterface );

        //
        //3 Second loop
        //    
        for ( Index = 0; Index < 300; Index++ ) {
            //
            //check for IDE_BSY bit to be clear
            //
            Status = WaitforBitClear( IdeBusInterface->PciIO,
                                      Regs.CommandBlock.StatusReg,
                                      IDE_BSY,
                                      1 );          // 1 msec

            if ( Status == EFI_SUCCESS ) {
                break;
            }
            pBS->Stall( 10000 );                    // 10Msec
        }

        Status = ControllerPresence( IdeBusInterface );

        if ( EFI_ERROR( Status )) {
            //
            // Device couldn't be detected
            //
            return EFI_NOT_FOUND;   
        }
    }

    //    
    //Disable Interrupt
    //    
    DisableIdeInterrupt( IdeBusInterface );

    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  IdeBusInterface->IdeDevice.Device << 4 );

    //
    //Check for IDE_BSY bit to be clear
    //
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.CommandBlock.StatusReg,
                              IDE_BSY,
                              gPlatformIdeProtocol->PoweonBusyClearTimeout ); // 10 second
	if(EFI_ERROR(Status)){
		return Status;
	}

    //
    //Check Drive ready. ATAPI devices will not set IDE_DRDY bit after reset
    //
    Status = CheckDriveReady( IdeBusInterface );

    if ( EFI_ERROR( Status )) {
        Status = AtapiIdentifyCommand( IdeBusInterface,
                                    &(IdeBusInterface->IdeDevice.IdentifyData));
        IdeBusInterface->IdeDevice.DeviceType = ATAPI;
        return Status;
    }

    //
    //Detect ATA device
    //
    Status = AtaIdentifyCommand( IdeBusInterface,
                              &(IdeBusInterface->IdeDevice.IdentifyData));
    IdeBusInterface->IdeDevice.DeviceType = ATA;

    if ( Status == EFI_SUCCESS ) {
        return EFI_SUCCESS;
    }

    //
    //Detect ATAPI Device. After failing the ATA Identify command, ATAPI device if present, 
    //should have the signature EB14h
    //
    Status = EFI_NOT_FOUND;
    IdeReadByte( IdeBusInterface->PciIO,
                     Regs.CommandBlock.LBAMidReg,
                     &Data8 );

    if ( Data8 == (ATAPI_SIGNATURE & 0xff)) {
        IdeReadByte( IdeBusInterface->PciIO,
                     Regs.CommandBlock.LBAHighReg,
                     &Data8 );

        if ( Data8 == (ATAPI_SIGNATURE >> 8)) {
            IdeBusInterface->IdeDevice.DeviceType = ATAPI;
            Status = AtapiIdentifyCommand( IdeBusInterface,
                                            &(IdeBusInterface->IdeDevice.
                                            IdentifyData));
        }
    }

    if ( EFI_ERROR( Status )) {
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}

/**
    Detects the ATA/ATAPI Controller

    @param AMI_IDE_BUS_PROTOCOL *IdeBusInterface;

    @retval EFI_STATUS

    @note  
  Here is the control flow of this function:
     1. Select the drive
     2. Read status Register
     3. If Status_Reg = 0xff, return Not_Found
     4. Check if Busy bit is clear, return Found
     5. If the BUS is not floating return EFI_DEVICE_ERROR

**/
EFI_STATUS
ControllerPresence (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    UINT8   Device = IdeBusInterface->IdeDevice.Device;
    IO_REGS Regs   = IdeBusInterface->IdeDevice.Regs;
    UINT8   Data8;
    UINT8   Temp;
    UINT16  Index;

    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  Device << 4 );

    //
    //Read the status Register
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.StatusReg, &Data8 );

    // When the Controller is not present , Status register returns 0xFF
    // With Controller Present and Port is disabled in the Controller 
    // status register returns 0x7F. On both the cases return with ERROR.

    if ( (Data8 == 0xff) || (Data8 == 0x7f) ) {
        return EFI_NOT_FOUND;
    }

    Index = 0;

    do
    {
        //
        // Status Register is not 0xff.
        //
        Temp = Data8;

        //60 usec delay
        pBS->Stall( 60 );

        //
        //Read the status Register
        //
        IdeReadByte( IdeBusInterface->PciIO,
                     Regs.CommandBlock.StatusReg,
                     &Data8 );

        //
        //Return Success if controller present
        //
        if ( !(Data8 & 0x80)) {
            return EFI_SUCCESS;
        }

        Data8 |= Temp;
        //
        //Keep bits 7,6,3 and 0
        //    
        Data8 &= 0xc9;

        if ( Data8 == 0xc9 ) {
            //Check for Bus Floating
            //Select the drive
            Data8 = (Device & 1) ? 0xA0 : 0xB0;
            IdeWriteByte( IdeBusInterface->PciIO,
                          Regs.CommandBlock.DeviceReg,
                          Data8 );
            IdeWriteByte( IdeBusInterface->PciIO,
                          Regs.CommandBlock.SectorCountReg,
                          0xff );
            IdeReadByte( IdeBusInterface->PciIO,
                         Regs.CommandBlock.DeviceReg,
                         &Temp );
            //Decide Controller Present or Not
            return (Temp == Data8) ? EFI_SUCCESS : EFI_DEVICE_ERROR;
        }
        Index++;
    } while ( Index < 350 );

    return EFI_SUCCESS;
}

/**
    Reset ATA device

        
    @param IdeBusInterface 

    @retval 
        EFI_STATUS

**/
EFI_STATUS
AtaReset (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  BOOLEAN                 ExtendedVerification
)
{
    return EFI_SUCCESS;
}

/**
    Read from ATA device
        
    @param This 
    @param MediaId 
    @param LBA,
    @param BufferSize 
    @param Buffer 

    @retval EFI_STATUS

    @note  
    1. Check for ERROR conditions.
    2. Call AtaReadWritePio.

**/

EFI_STATUS
AtaBlkRead (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
)
{
    UINTN                   DataN;
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = ((IDE_BLOCK_IO*)This)->IdeBusInterface;
    EFI_BLOCK_IO_MEDIA      *BlkMedia = This->Media;
    UINTN                   BufferAddress;

    //    
    //Check if Media ID matches
    //
    if ( BlkMedia->MediaId != MediaId ) {
        return EFI_MEDIA_CHANGED;
    }

    if ( BlkMedia->MediaPresent == FALSE ) {
        return EFI_NO_MEDIA;
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
    BufferAddress = (UINTN)Buffer;
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

    if ( gPlatformIdeProtocol->IdeBusMasterSupport ) {
        if ( DMACapable( IdeBusInterface )) {
            Status = AtaReadWriteBusMaster( IdeBusInterface,
                                        Buffer,
                                        BufferSize,
                                        LBA,
                                        IdeBusInterface->IdeDevice.ReadCommand,
                                        0 );
            return Status;
        }
    }


    Status = AtaReadWritePio( IdeBusInterface,
                              Buffer,
                              BufferSize,
                              LBA,
                              IdeBusInterface->IdeDevice.ReadCommand,
                              0 );
    return Status;
}

/**
    Write to ATA device
        
    @param This 
    @param MediaId 
    @param LBA,
    @param BufferSize 
    @param Buffer 

    @retval EFI_STATUS

    @note  
    1. Check for error conditions.
    2. Call AtaReadWritePio.

**/
EFI_STATUS
AtaBlkWrite (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    IN  VOID                    *Buffer
)
{
    UINTN                   DataN;
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = ((IDE_BLOCK_IO*)This)->IdeBusInterface;
    EFI_BLOCK_IO_MEDIA      *BlkMedia = This->Media;
    UINTN                   BufferAddress;

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
    
#if ( defined(BOOT_SECTOR_WRITE_PROTECT) && (BOOT_SECTOR_WRITE_PROTECT != 0) )
    if(AmiBlkWriteProtection != NULL) {
        // Get user input
        Status = AmiBlkWriteProtection->BlockIoWriteProtectionCheck ( 
                                                    AmiBlkWriteProtection,
                                                    This,
                                                    LBA,
                                                    BufferSize
                                                    );
        // Abort operation if denied
        if(Status == EFI_ACCESS_DENIED) {
            return Status;
        }
    } 
#endif

    if ( gPlatformIdeProtocol->IdeBusMasterSupport ) {
        if ( DMACapable( IdeBusInterface )) {
            Status = AtaReadWriteBusMaster( IdeBusInterface,
                                        Buffer,
                                        BufferSize,
                                        LBA,
                                        IdeBusInterface->IdeDevice.WriteCommand,
                                        1 );
             return Status;
         }
    }

    Status = AtaReadWritePio( IdeBusInterface,
                              Buffer,
                              BufferSize,
                              LBA,
                              IdeBusInterface->IdeDevice.WriteCommand,
                              1 );
    return Status;
}

/**
    Flush the cache
        
    @param This 

    @retval EFI_STATUS

**/
EFI_STATUS
AtaBlkFlush (
    IN  EFI_BLOCK_IO_PROTOCOL   *This
)
{
    EFI_STATUS             Status;
    AMI_IDE_BUS_PROTOCOL   *IdeBusInterface = NULL;
    UINT8                  Command;
    
    IdeBusInterface = ((IDE_BLOCK_IO *)This)->IdeBusInterface;
    
    if (IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400) {
        // 48Bit LBA Supported
        Command = FLUSH_CACHE_EXT;
    } else {
        Command = FLUSH_CACHE;
    }

    Status = IdeNonDataCommand( IdeBusInterface, 
                                0, 0, 0, 0, 0,
                                IdeBusInterface->IdeDevice.Device << 4,
                                Command );
    
    return Status;

}

/**
    Issues Read/Write Command and Read/Write the data from/to the ATA device


    @param  IdeBusInterface
    @param  *Buffer,
    @param  ByteCount,
    @param  LBA
    @param  ReadWriteCommand 
    @param  ReadWrite Read/Write = 0/1

    @retval *Buffer

    @note  
    1. Check if Multiple sectors can be read/written to the ATA device.
    2. Check for 48-bit LBA support.
    3. Issue the command based on step 1 and step 2 results.
    4. check for errors.
    5. If success read/write data.
    6. Based on step 1 results, complete the read/write sequence
    7. If all sectors are not completed, goto step 3.

**/
EFI_STATUS
AtaReadWritePio (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT64                  LBA,
    IN  UINT8                   ReadWriteCommand,
    IN  BOOLEAN                 ReadWrite
)
{
    EFI_STATUS              Status;
    INT32                   WordCount;
    UINT32                  SectorCount;
    UINTN                   Remainder;
    UINT8                   Data8;
    IDE_DEVICE_INTERFACE    *IdeDevice  = &(IdeBusInterface->IdeDevice);
    UINT8                   BlockSize   = 1;    // 1 sector Default
    VOID                    *TempBuffer;
    IO_REGS                 Regs        = IdeBusInterface->IdeDevice.Regs;
    INT64                   LoopCount;
    INT64                   MaxSectorCount;
    INT64                   Total_Number_Of_Sectors;
    UINT32                  EraseCommandTimeout = 0;
    UINT32                  SectorSize = ATA_SECTOR_BYTES;
    BOOLEAN                 SectorGTBytes = FALSE;

    //
    //Check if the device supports Multiple sector Read/Write
    //
    if  ( IdeDevice->IdentifyData.Valid_Bits_59 & 0x100 ) {
        BlockSize = (UINT8) IdeDevice->IdentifyData.Valid_Bits_59;
    }

    //
    //	Error Checking
    //
    if ( BlockSize == 0 ) {
        BlockSize = 1;
    }

    if((IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT14) && // WORD 106 valid? - BIT 14 - 1
       (!(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT15)) && // WORD 106 valid? - BIT 15 - 0
       (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT12)) { // WORD 106 bit 12 - Sectorsize > 256 words
        // The sector size is in words 117-118.
        SectorSize = (UINT32)(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[13] + \
                              (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[14] << 16)) * 2;
    }
    //
    //	Disable Interrupt
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );

    if ( Check48BitCommand( ReadWriteCommand )) {
        MaxSectorCount = MAX_SECTOR_COUNT_PIO_48BIT;
    }
    else {
        MaxSectorCount = MAX_SECTOR_COUNT_PIO;
    }

    //
    //Calculate the total number of Sectors to be transferred
    //
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

    for (;
         Total_Number_Of_Sectors > 0;
         Total_Number_Of_Sectors -= MaxSectorCount )
    {
        if ( Total_Number_Of_Sectors > MaxSectorCount ) {
            SectorCount = 0;
        } else {
            SectorCount = (UINT32) Total_Number_Of_Sectors;
        }

        Status = IssueAtaReadWriteCommand( IdeBusInterface,
                                           LBA,
                                           SectorCount,
                                           ReadWriteCommand,
                                           0 );

        if ( EFI_ERROR( Status )) {
            return EFI_INVALID_PARAMETER;
        }

        //
        //Update LBA for next loop
        //
        if ( SectorCount ) {
            LBA       += SectorCount;                                 
            LoopCount  = (SectorCount / BlockSize);
            LoopCount += ((SectorCount % BlockSize) > 0 ? 1 : 0);
        }
        else {
            LBA       += MaxSectorCount;
            LoopCount  = Div64( MaxSectorCount, BlockSize, &Remainder );
            LoopCount += (Remainder > 0 ? 1 : 0);
        }

        //
        //For Security Erase command the time out value comes from Identify Data.
        //
        if ( ReadWriteCommand == SECURITY_ERASE_UNIT ) {
            UINT8   *BufferData = (UINT8*) Buffer;

            // BIT:1 of the BufferData[0] refers Enhanced Security Erase mode.
            // If this Bit is set, Identify Data Word 90 will be used for Erase command timeout
            // else Word 89 will be used for the timeout.
            if( BufferData[0] & 2 ) {
                EraseCommandTimeout= (UINT32)(IdeBusInterface->IdeDevice.IdentifyData.Time_Esecurity_Earse_90);
            } else {
                EraseCommandTimeout = (UINT32)( IdeBusInterface->IdeDevice.IdentifyData.Time_security_Earse_89 );
            }

            if ( EraseCommandTimeout <= 254 ) {
                EraseCommandTimeout = EraseCommandTimeout * 2 * 1000 * 60; //Value * 2Minitues
            }
            else {
                EraseCommandTimeout = 0;                                 // No Timeout Value
            }
        }

        //
        //Read Data
        //
        for (; LoopCount > 0; LoopCount -= 1 ) {
            //
            //Wait for Command completion
            //
            if ( ReadWriteCommand == SECURITY_ERASE_UNIT ) {
                Status = WaitForCmdCompletionWithTimeOutValue( IdeBusInterface,
                                                                EraseCommandTimeout );
            }
            else {
                Status = WaitForCmdCompletion( IdeBusInterface );
            }

            if ( EFI_ERROR( Status )) {
                return EFI_DEVICE_ERROR;
            }

            //
            //Check for IDE_DRQ
            //
            Status = WaitforBitSet( IdeBusInterface->PciIO,
                                    Regs.ControlBlock.AlternateStatusReg,
                                    IDE_DRQ,
                                    DRQ_TIMEOUT );

            if ( EFI_ERROR( Status )) {
                return EFI_DEVICE_ERROR;
            }

            //
            //Calculate # of Words to be read/written
            //
            if ( SectorCount ) {
                if ( SectorCount >= BlockSize ) {
                    WordCount = (BlockSize * SectorSize) / 2;
                }
                else {
                    WordCount = (SectorCount * SectorSize) / 2;   // Partial Block will be transferred
                }
            }
            else {
                WordCount = (BlockSize * SectorSize) / 2;
            }

            if ( BlockSize == 1 ) {
                WordCount = (SectorSize) / 2;
            }

            if ( ReadWrite ) {
                Status = IdeWriteMultipleWord( IdeBusInterface->PciIO,
                                               Regs.CommandBlock.DataReg,
                                               WordCount,
                                               TempBuffer );
            } else {
                Status = IdeReadMultipleWord( IdeBusInterface->PciIO,
                                              Regs.CommandBlock.DataReg,
                                              WordCount,
                                              TempBuffer );
            }

            if ( EFI_ERROR( Status )) {
                return EFI_DEVICE_ERROR;
            }

            //
            //Check for errors.
            //
            IdeReadByte( IdeBusInterface->PciIO,
                         Regs.CommandBlock.StatusReg,
                         &Data8 );

            if ( Data8 & 0x21 ) {          // IDE_ERR OR IDE_DF bit set ?
                return EFI_DEVICE_ERROR;
            }

            TempBuffer = (UINT8*)(TempBuffer) + (WordCount * 2);

            if ( SectorCount ) {
                SectorCount -= (WordCount * 2) / SectorSize;
            }
            else {
                SectorCount = (UINT32) ( MaxSectorCount - ((WordCount * 2) / SectorSize));
            }
        }                                                                   
    }                                                                       
    if (SectorGTBytes) {
        pBS->CopyMem( Buffer, TempBuffer, ByteCount);
        pBS->FreePool( TempBuffer );
    }

    //
    //	Check for errors
    //    
    if ( ReadWriteCommand == SECURITY_ERASE_UNIT ) {
        Status = WaitForCmdCompletionWithTimeOutValue( IdeBusInterface,
                                                       EraseCommandTimeout );
    } else {
        Status = WaitForCmdCompletion( IdeBusInterface );
    }

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
    Issues command which require data to be read

    @param  IdeBusInterface 
    @param  VOID    *Buffer,
    @param  UINT32  ByteCount,
    @param  UINT8   SectorCount,
    @param  UINT8   LBALow,
    @param  UINT8   LBAHigh,
    @param  UINT8   Device,
    @param  UINT8   Command,
    @param  BOOLEAN     Multiple  // to determine the block size

    @retval *Buffer

    @note  
    Used to get Identify command data etc.
    1. Issue the command
    2. Check for errors.
    3. Check if Data is ready. If yes, read it else return error.

**/
EFI_STATUS
AtaPioDataIn (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    OUT VOID                    *Buffer,
    IN  UINT32                  ByteCount,
    IN  UINT8                   Features,
    IN  UINT8                   SectorCount,
    IN  UINT8                   LBALow,
    IN  UINT8                   LBAMid,
    IN  UINT8                   LBAHigh,
    IN  UINT8                   Device,
    IN  UINT8                   Command,
    IN  BOOLEAN                 Multiple
)
{
    IO_REGS     Regs = IdeBusInterface->IdeDevice.Regs;
    EFI_STATUS  Status;
    UINT8       Data8;
    UINT32      BlockSize;
    VOID        *TempBuffer = Buffer;
    UINT32      SectorSize = ATA_SECTOR_BYTES;
    INT64       TempByteCount;


    if((IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT14) && // WORD 106 valid? - BIT 14 - 1
       (!(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT15)) && // WORD 106 valid? - BIT 15 - 0
       (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT12)) { // WORD 106 bit 12 - Sectorsize > 256 words
        // The sector size is in words 117-118.
        SectorSize = (UINT32)(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[13] + \
                              (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[14] << 16)) * 2;
    }

    if ( Multiple ) {
        BlockSize = SectorSize * (IdeBusInterface->IdeDevice.IdentifyData.Valid_Bits_59 & 0x0f);
    }
    else {                  
        //
        // Only one block of data to read
        //
        BlockSize = SectorSize;
    }

    if (ByteCount < SectorSize) BlockSize = ByteCount;

    // Make the byte count a signed number.
    TempByteCount = (INT64)ByteCount;

    //
    //Select the drive
    //
    IdeWriteByte(   IdeBusInterface->PciIO,
                    Regs.CommandBlock.DeviceReg,
                    Device );

    //
    //	Issue the Command
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.FeatureReg,
                  Features );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.SectorCountReg,
                  SectorCount );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBALowReg,
                  LBALow );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBAMidReg,
                  LBAMid );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBAHighReg,
                  LBAHigh );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.CommandReg,
                  Command );

    for (; TempByteCount > 0; TempByteCount -= BlockSize ) {

        //
        //Wait for 400nsec before reading the status. To accomplish it, read ATL_STATUS and ignore the result
        //
        IdeReadByte( IdeBusInterface->PciIO,
                     Regs.ControlBlock.AlternateStatusReg,
                     &Data8 );

        //
        //Check for IDE_BSY bit to be clear
        //
        Status = WaitforBitClear( IdeBusInterface->PciIO,
                                  Regs.ControlBlock.AlternateStatusReg,
                                  IDE_BSY,
                                  COMMAND_COMPLETE_TIMEOUT );

        if ( EFI_ERROR( Status )) {
            return EFI_DEVICE_ERROR;
        }

        //
        //Check if IDE_DRQ is set else it is an ERROR
        //
        IdeReadByte( IdeBusInterface->PciIO,
                     Regs.CommandBlock.StatusReg,
                     &Data8 );

        //
        // IDE_DRQ bit set ?
        //
        if ( !(Data8 & 0x08)) {       
            //
            // Handle Error condition		
            //
            return EFI_DEVICE_ERROR;
        }

        if ( Data8 & 0x21 ) {          
            return EFI_DEVICE_ERROR;
        }

        Status = IdeReadMultipleWord( IdeBusInterface->PciIO,
                                      Regs.CommandBlock.DataReg,
                                      BlockSize / 2,
                                      TempBuffer );
        //
        //		Wait for IDE_DRQ to go low
        //
        WaitforBitClear( IdeBusInterface->PciIO,
                         Regs.CommandBlock.StatusReg,
                         IDE_DRQ,
                         DRQ_CLEAR_TIMEOUT );

        if ( EFI_ERROR( Status )) {
            return EFI_DEVICE_ERROR;
        }
        TempBuffer = (UINT8*)(TempBuffer) + BlockSize;

    }

    return EFI_SUCCESS;
}

/**
    Issues Read/Write Command and Read/Write the data from/to the ATA device
    with SubCommand Support.

    @param  IdeBusInterface 
    @param  VOID    *Buffer,
    @param  UINTN   ByteCount,
    @param  UINT8   Features,
    @param  UINT32  SectorCountIn,
    @param  UINT8   LBALow,
    @param  UINT8   LBALowExp,
    @param  UINT8   LBAMid,
    @param  UINT8   LBAMidExp,
    @param  UINT8   LBAHigh,
    @param  UINT8   LBAHighExp,
    @param  UINT8   Device,
    @param  UINT8   Command,
    @param  BOOLEAN ReadWrite, // Read/Write = 0/1
    @param  BOOLEAN Multiple   // to determine the block size

    @retval *Buffer

    @note  
    1. Check if Multiple sectors can be read/written to the ATA device.
    2. Check for 48-bit LBA support.
    3. Issue the command based on step 1 and step 2 results.
    4. check for errors.
    5. If success read/write data.
    6. Based on step 1 results, complete the read/write sequence
    7. If all sectors are not completed, goto step 3.

**/

EFI_STATUS
AtaPioDataOut   (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT8                   Features,
    IN  UINT32                  SectorCountIn,
    IN  UINT8                   LBALow,
    IN  UINT8                   LBALowExp,
    IN  UINT8                   LBAMid,
    IN   UINT8                  LBAMidExp,
    IN  UINT8                   LBAHigh,
    IN  UINT8                   LBAHighExp,
    IN  UINT8                   Device,
    IN  UINT8                   Command,
    IN  BOOLEAN                 ReadWrite,
    IN  BOOLEAN                 Multiple
)
{
    EFI_STATUS           Status;
    INT32                WordCount;
    UINT32               SectorCount;
    UINTN                Remainder;
    UINT8                Data8;
    IDE_DEVICE_INTERFACE *IdeDevice  = &(IdeBusInterface->IdeDevice);
    UINT8                BlockSize   = 1;                         // 1 sector Default
    VOID                 *TempBuffer;
    VOID                 *TempBufferGT;
    IO_REGS              Regs        = IdeBusInterface->IdeDevice.Regs;
    INT64                LoopCount;
    INT64                MaxSectorCount;
    INT64                Total_Number_Of_Sectors;
    UINT32               SectorSize = ATA_SECTOR_BYTES;
    BOOLEAN              SectorGTBytes = FALSE;
    UINT64               LBA = 0;
    UINT64               LBAHighDword = 0;
    UINT32               Timeout = 0;

    //
    // Check if the device supports Multiple sector Read/Write
    //    
    if  ( IdeDevice->IdentifyData.Valid_Bits_59 & 0x100 ) {
        BlockSize = (UINT8) IdeDevice->IdentifyData.Valid_Bits_59;
    }

    //
    // Error Checking
    //
    if ( BlockSize == 0 ) {
       BlockSize = 1;
    }

    if((IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT14) && // WORD 106 valid? - BIT 14 - 1
       (!(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT15)) && // WORD 106 valid? - BIT 15 - 0
       (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT12)) { // WORD 106 bit 12 - Sectorsize > 256 words
        // The sector size is in words 117-118.
        SectorSize = (UINT32)(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[13] + \
                              (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[14] << 16)) * 2;
    }
    //
    //	Disable Interrupt
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );

    if ( Check48BitCommand( Command )) {
        MaxSectorCount = MAX_SECTOR_COUNT_PIO_48BIT;
        //
        //	if 48 Bit LBA form Upper Dword
        //
        LBAHighDword |= LBAHighExp;
        LBAHighDword = ( Shl64(( Shl64( LBAHighDword, 8)| LBAMidExp), 8)| LBALowExp);
    }
    else {
        MaxSectorCount = MAX_SECTOR_COUNT_PIO;
    }
    //
    //	Complete LBA	 
    //
    LBA |= LBAHigh;
    LBA = (( Shl64(( Shl64( LBA, 8) | LBAMid ), 8)| LBALow)| Shl64( LBAHighDword, 24 ));

    //
    //Calculate the total number of Sectors to be transferred
    //
    Total_Number_Of_Sectors = ByteCount / SectorSize;         //512
    // If the caller is requesting less bytes than one sector, we need to
    // allocate space for one sector.
    if (!ReadWrite && ((ByteCount < SectorSize) && (ByteCount > 0))) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    SectorSize,
                                    (VOID**)&TempBuffer );
        if ( EFI_ERROR( Status )) {
            return EFI_OUT_OF_RESOURCES;
        }
        SectorGTBytes = TRUE;
        TempBufferGT = TempBuffer;
        Total_Number_Of_Sectors = 1;
    } else {
        TempBuffer = Buffer;
    }

    for (;
         Total_Number_Of_Sectors > 0;
         Total_Number_Of_Sectors -= MaxSectorCount )
    {
        if ( Total_Number_Of_Sectors > MaxSectorCount ) {
            SectorCount = 0;
        } else {
            SectorCount = (UINT32) Total_Number_Of_Sectors;
        }

        Status = IssueAtaReadWriteCommand( IdeBusInterface,
                                           LBA,
                                           SectorCount,
                                           Command,
                                           Features );

        if ( EFI_ERROR( Status )) {
            return EFI_INVALID_PARAMETER;
        }

        //
        //Update LBA for next loop
        //
        if ( SectorCount ) {
            LBA       += SectorCount;
            LoopCount  = (SectorCount / BlockSize);
            LoopCount += ((SectorCount % BlockSize) > 0 ? 1 : 0);
        }
        else {
            LBA       += MaxSectorCount;
            LoopCount  = Div64( MaxSectorCount, BlockSize, &Remainder );
            LoopCount += (Remainder > 0 ? 1 : 0);
        }

        //
        //Read Data
        //
        for (; LoopCount > 0; LoopCount -= 1 ) {
            //
            //Wait for Command completion
            //
            Status = WaitForCmdCompletion( IdeBusInterface );

            if ( EFI_ERROR( Status )) {
                return EFI_DEVICE_ERROR;
            }
            //
            //Check for IDE_DRQ
            //
            
            if(gPlatformIdeProtocol->CommandTimeout == 0) {
                Timeout = DRQ_TIMEOUT;
            } else {
                Timeout = (UINT32)gPlatformIdeProtocol->CommandTimeout;
            }
            
            Status = WaitforBitSet( IdeBusInterface->PciIO,
                                    Regs.ControlBlock.AlternateStatusReg,
                                    IDE_DRQ,
                                    Timeout );

            if ( EFI_ERROR( Status )) {
                return EFI_DEVICE_ERROR;
            }

            //
            //Calculate # of Words to be read/written
            //
            if ( SectorCount ) {
                if ( SectorCount >= BlockSize ) {

                    // BlockSize will be transferred
                    WordCount = (BlockSize * SectorSize) / 2;
                } else {

                    // Partial Block will be transferred
                    WordCount = (SectorCount * SectorSize) / 2;   
                }
            } else {
                WordCount = (BlockSize * SectorSize) / 2;
            }

            if ( BlockSize == 1 ) {
                WordCount = (SectorSize) / 2;
            }

            if ( ReadWrite ) {
                Status = IdeWriteMultipleWord( IdeBusInterface->PciIO,
                                               Regs.CommandBlock.DataReg,
                                               WordCount,
                                               TempBuffer );
            } else {
                Status = IdeReadMultipleWord( IdeBusInterface->PciIO,
                                              Regs.CommandBlock.DataReg,
                                              WordCount,
                                              TempBuffer );
            }

            if ( EFI_ERROR( Status )) {
                return EFI_DEVICE_ERROR;
            }

            //
            //Check for errors.
            //
            IdeReadByte( IdeBusInterface->PciIO,
                         Regs.CommandBlock.StatusReg,
                         &Data8 );

            if ( Data8 & 0x21 ) {          // IDE_ERR OR IDE_DF bit set ?
                return EFI_DEVICE_ERROR;
            }

            TempBuffer = (UINT8*)(TempBuffer) + (WordCount * 2);
                

            if ( SectorCount ) {
                SectorCount -= (WordCount * 2) / SectorSize;
            }
            else {
                SectorCount = (UINT32) ( MaxSectorCount - ((WordCount * 2) / SectorSize));
            }
        }
    }

    if (SectorGTBytes) {
        pBS->CopyMem( Buffer, TempBufferGT, ByteCount);
        pBS->FreePool( TempBufferGT );
    }

    //
    //	Check for errors
    //    
    Status = WaitForCmdCompletion( IdeBusInterface );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;

}


/**
    Issues ATA Read/Write Command

    @param  IdeBusInterface 
    @param  UINT64  LBA,
    @param  INT32   SectorCount,
    @param  UINT8   Command
    @param  UINT8   Features

    @retval EFI_STATUS

    @note  
    1. Select the drive.
    2. check if BSY and DRQ bits are zero.
    3. Issue the command.

**/
EFI_STATUS
IssueAtaReadWriteCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINT64                  LBA,
    IN  INT32                   SectorCount,
    IN  UINT8                   Command,
    IN  UINT8                   Features
)
{
    EFI_STATUS              Status;
    IDE_DEVICE_INTERFACE    *IdeDevice  = &(IdeBusInterface->IdeDevice);
    IO_REGS                 Regs    = IdeBusInterface->IdeDevice.Regs;
    UINT8                   Device  = (IdeDevice->Device << 4);
    UINT32                  Timeout = 0;

    //
    // Select the drive
    //    
    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.DeviceReg, Device );

    if(gPlatformIdeProtocol->CommandTimeout == 0) {
        Timeout = DRQ_TIMEOUT;
    } else {
        Timeout = (UINT32)gPlatformIdeProtocol->CommandTimeout;
    }
    //
    //Before Writing to Sector Count Register, IDE_BSY and IDE_DRQ bit should be zero
    //
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.ControlBlock.AlternateStatusReg,
                              IDE_BSY | IDE_DRQ,
                              Timeout );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    if(gPlatformIdeProtocol->CommandTimeout == 0) {
        Timeout = DRDY_TIMEOUT;
    } else {
        Timeout = (UINT32)gPlatformIdeProtocol->CommandTimeout;
    }
    //
    //Check for IDE_DRDY
    //
    Status = WaitforBitSet( IdeBusInterface->PciIO,
                            Regs.ControlBlock.AlternateStatusReg,
                            IDE_DRDY,
                            Timeout );

    if ( EFI_ERROR( Status )) {
        return EFI_TIMEOUT;
    }

    if ( Check48BitCommand( Command )) {
        //		48 Bit LBA
        //		Write the Upper LBA DWORD and Upper byte of Sector Count
        IdeWriteByte(   IdeBusInterface->PciIO,
                         Regs.CommandBlock.SectorCountReg,
                         (UINT8)( SectorCount >> 8 ));

        IdeWriteByte(   IdeBusInterface->PciIO,
                        Regs.CommandBlock.LBALowReg,
                        (UINT8)Shr64( LBA, 24 ));

        IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,
                      (UINT8) Shr64( LBA, 32 ));

        IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg,
                      (UINT8) Shr64( LBA, 40 ));

        Device = (IdeDevice->Device << 4) | 0x40;       // 48Bit LBA
    }
    else {                                              // 28 Bit LBA
        Device = ((UINT8) ((UINT32) LBA >> 24 ) & 0x0f) | (IdeDevice->Device << 4) | 0x40;
    }

    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  Device );

    //
    //Issue command
    //
    if ( Features ) {
        IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.FeatureReg,  //SubCommand
                      Features );
    }
    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.SectorCountReg,
                  (UINT8) SectorCount );

    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBALowReg,
                  (UINT8)LBA );

    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,
                  (UINT8) (((UINT32)LBA >> 8) & 0xff ));

    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg,
                  (UINT8) (((UINT32)LBA >> 16) & 0xff ));

    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.CommandReg,
                  Command );

    //
    //Wait for 400nsec for status to be available
    //
    pBS->Stall( 1 );
    return EFI_SUCCESS;
}

/**
    Issues command where no data transfer takes place

    @param  IdeBusInterface 
    @param  VOID    *Buffer,
    @param  UINT32  ByteCount,
    @param  UINT8   SectorCount,
    @param  UINT8   LBALow,
    @param  UINT8   LBAMid,
    @param  UINT8   LBAHigh,
    @param  UINT8   Device,
    @param  UINT8   Command,

    @retval *Buffer

**/
EFI_STATUS
IdeNonDataCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINT8                   Features,
    IN  UINT8                   SectorCount,
    IN  UINT8                   LBALow,
    IN  UINT8                   LBAMid,
    IN  UINT8                   LBAHigh,
    IN  UINT8                   Device,
    IN  UINT8                   Command
)
{
    IO_REGS    Regs = IdeBusInterface->IdeDevice.Regs;
    EFI_STATUS Status;
    UINT8      Data8;
    UINT32     Timeout = 0;
    
    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.DeviceReg, Device );

    
    if(gPlatformIdeProtocol->CommandTimeout == 0) {
        Timeout = DRDY_TIMEOUT;
    } else {
        Timeout = (UINT32)gPlatformIdeProtocol->CommandTimeout;
    }
    //
    //Wait for IDE_DRDY to be set
    //
    Status = WaitforBitSet( IdeBusInterface->PciIO,
                            Regs.ControlBlock.AlternateStatusReg,
                            IDE_DRDY,
                            Timeout);

    if ( EFI_ERROR( Status )) {
        return EFI_TIMEOUT;
    }

    //
    //Issue the Command
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.FeatureReg,
                  Features );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.SectorCountReg,
                  SectorCount );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBALowReg,
                  LBALow );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBAMidReg,
                  LBAMid );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBAHighReg,
                  LBAHigh );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.CommandReg,
                  Command );

    //
    //Wait for 400nsec before reading the status. To accomplish it, read ATL_STATUS and ignore the result
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    if(gPlatformIdeProtocol->CommandTimeout == 0) {
        Timeout = gPlatformIdeProtocol->PoweonBusyClearTimeout;
    }  else {
        Timeout = (UINT32)gPlatformIdeProtocol->CommandTimeout;
    }
    
    //
    //Check for IDE_BSY bit to be clear
    //
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.ControlBlock.AlternateStatusReg,
                              IDE_BSY | IDE_DRQ,
                              Timeout);

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Check for any ERRORs
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.StatusReg, &Data8 );

    if ( Data8 & (IDE_DF | IDE_ERR)) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
    Issues command where no data transfer takes place

        
    @param  IdeBusInterface 
    @param  VOID    *Buffer,
    @param  UINT32  ByteCount,
    @param  UINT8   SectorCount,
    @param  UINT8   SectorCountExp,
    @param  UINT8   LBALow,
    @param  UINT8   LBALowExp,
    @param  UINT8   LBAMid,
    @param  UINT8   LBAMidExp,
    @param  UINT8   LBAHigh,
    @param  UINT8   LBAHighExp,
    @param  UINT8   Device,
    @param  UINT8   Command,

    @retval *Buffer

**/
EFI_STATUS
IdeNonDataCommandExp (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINT8                   Features,
    IN  UINT8                   SectorCount,
    IN  UINT8                   SectorCountExp,
    IN  UINT8                   LBALow,
    IN  UINT8                   LBALowExp,
    IN  UINT8                   LBAMid,
    IN  UINT8                   LBAMidExp,
    IN  UINT8                   LBAHigh,
    IN  UINT8                   LBAHighExp,
    IN  UINT8                   Device,
    IN  UINT8                   Command
)
{
    IO_REGS     Regs = IdeBusInterface->IdeDevice.Regs;
    EFI_STATUS  Status;
    UINT8       Data8;

    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.DeviceReg, Device );

    //
    //Wait for IDE_DRDY to be set
    //
    Status = WaitforBitSet( IdeBusInterface->PciIO,
                            Regs.ControlBlock.AlternateStatusReg,
                            IDE_DRDY,
                            DRDY_TIMEOUT );

    if ( EFI_ERROR( Status )) {
        return EFI_TIMEOUT;
    }

    //		48 Bit LBA
    //		Write the Upper LBA DWORD and Upper byte of Sector Count
    IdeWriteByte(   IdeBusInterface->PciIO,
                    Regs.CommandBlock.SectorCountReg,
                    SectorCountExp);

    IdeWriteByte(   IdeBusInterface->PciIO,
                    Regs.CommandBlock.LBALowReg,
                    LBALowExp);

    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAMidReg,
                  LBAMidExp);

    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.LBAHighReg,
                  LBAHighExp);

    //
    //Issue the Command
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.FeatureReg,
                  Features );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.SectorCountReg,
                  SectorCount );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBALowReg,
                  LBALow );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBAMidReg,
                  LBAMid );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.LBAHighReg,
                  LBAHigh );
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.CommandReg,
                  Command );

    //
    //Wait for 400nsec before reading the status. To accomplish it, read ATL_STATUS and ignore the result
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    //
    //Check for IDE_BSY bit to be clear
    //
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.ControlBlock.AlternateStatusReg,
                              IDE_BSY | IDE_DRQ,
                              gPlatformIdeProtocol->PoweonBusyClearTimeout );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Check for any errors
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.StatusReg, &Data8 );

    if ( Data8 & (IDE_DF | IDE_ERR)) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
    Issues IDENTIFY DATA command (0xEC)

    @param IdeBusInterface 
    @param Buffer 

    @retval *Buffer

**/
EFI_STATUS
AtaIdentifyCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer

)
{
    EFI_STATUS Status = AtaPioDataIn(
        IdeBusInterface,
        Buffer,
        512,
        0,
        0,
        0,
        0,
        0,
        IdeBusInterface->IdeDevice.Device << 4,
        IDENTIFY_COMMAND,
        FALSE );

    return Status;
}

/**
    Issues IDENTIFY PACKET DATA command (0xA1)

    @param IdeBusInterface 
    @param Buffer 

    @retval *Buffer

**/
EFI_STATUS
AtapiIdentifyCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer
)
{
    return AtaPioDataIn(
               IdeBusInterface,
               Buffer,
               512,                // Byte Count
               0,
               0,
               0,
               0,
               0,
               IdeBusInterface->IdeDevice.Device << 4,
               IDENTIFY_PACKET_COMMAND,
               FALSE );
}

/**
    Gets Identify command data.

        
    @param IdeBusInterface 
    @retval 
        EFI_STATUS

    @note  ATA/ATAPI device type should have been known already.

**/
EFI_STATUS
GetIdentifyData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer
)
{
    //
    //check whether it is a ATA or ATAPI device
    //
    if ( IdeBusInterface->IdeDevice.DeviceType == ATAPI ) {
        return AtaPioDataIn(
                   IdeBusInterface,
                   Buffer,
                   512,                // Byte Count
                   0,
                   0,
                   0,
                   0,
                   0,
                   IdeBusInterface->IdeDevice.Device << 4,
                   IDENTIFY_PACKET_COMMAND,
                   FALSE );
    }
    else {
        EFI_STATUS Status = AtaPioDataIn(
            IdeBusInterface,
            Buffer,
            512,
            0,
            0,
            0,
            0,
            0,
            IdeBusInterface->IdeDevice.Device << 4,
            IDENTIFY_COMMAND,
            FALSE );

        return Status;
    }
}

/**
    Issue a ATA Non-Data Command

    @param IdeBusInterface 

    @retval EFI_STATUS

**/
EFI_STATUS
IdeSetFeatureCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT8                       SubCommand,
    UINT8                       Mode
)
{
    return IdeNonDataCommand(
               IdeBusInterface,
               SubCommand,
               Mode,
               0,
               0,
               0,
               IdeBusInterface->IdeDevice.Device << 4,
               SET_FEATURE_COMMAND );
}

/**
    Issue a Soft Reset

        
    @param IdeBusInterface 
    @retval 
        EFI_STATUS

**/
EFI_STATUS
IdeSoftReset (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    UINT8   Device = IdeBusInterface->IdeDevice.Device;
    IO_REGS Regs   = IdeBusInterface->IdeDevice.Regs;
    UINT8   Data8;

    PROGRESS_CODE( DXE_IDE_RESET );

    //
    //Select the drive. Not needed.
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  Device << 4 );

    //
    //Read the status Register
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.StatusReg, &Data8 );

    //
    //If IDE_BSY bit set, don't issue Soft reset
    //
    if ( Data8 & 0x80 ) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Assert IDE_SRST, disable IDE_NIEN
    //
    Data8 = IDE_SRST | IDE_NIEN;
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.ControlBlock.DeviceControlReg,
                  Data8 );
    pBS->Stall( 100 );               // 100 usec

    //
    //DeAssert IDE_SRST
    //
    Data8 = IDE_NIEN;
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.ControlBlock.DeviceControlReg,
                  Data8 );
    pBS->Stall( 10000 );             // 10 msec

    return EFI_SUCCESS;
}

/**
    Waits for IDE_BSY bit to get clear

    @param IdeBusInterface 
    @param TimeOutValue 

    @retval EFI_STATUS

    @note  Wait for IDE_BSY bit to get clear. Check for any ERRORs.

**/
EFI_STATUS
WaitForCmdCompletionWithTimeOutValue (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINT32                  TimeOutvalue
)
{
    UINT8       Data8;
    EFI_STATUS  Status;
    IO_REGS     Regs = IdeBusInterface->IdeDevice.Regs;

    //	Read ATL_STATUS and ignore the result. Just a delay
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    // if the Timeout Value is 0, check Busy bit to clear without Timeout Value.
    // Otherwise Use the Timeout value to check the Busy bit to clear.
    if ( TimeOutvalue == 0 ) {
        //
        //Check for IDE_BSY bit to be clear without Timeout value
        //
        Status = WaitforBitClearWithoutTimeout(
                                IdeBusInterface->PciIO,
                                Regs.ControlBlock.
                                AlternateStatusReg,
                                IDE_BSY );
    } else {
        //
        //Check for IDE_BSY bit to be clear
        //
        Status = WaitforBitClear( IdeBusInterface->PciIO,
                                  Regs.ControlBlock.AlternateStatusReg,
                                  IDE_BSY,
                                  TimeOutvalue );
    }

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //Check for ERRORs.
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.CommandBlock.StatusReg,
                 &Data8 );

    if ( Data8 & (IDE_ERR | IDE_DF)) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
    Waits for IDE_BSY bit to get clear

    @param IdeBusInterface 

    @retval EFI_STATUS

    @note  Wait for BSY bit to get clear. Check for any errors.

**/
EFI_STATUS  
WaitForCmdCompletion (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    UINT8       Data8;
    EFI_STATUS  Status;
    IO_REGS     Regs = IdeBusInterface->IdeDevice.Regs;
    UINT32      Timeout = 0;
    
    //
    //Read ATL_STATUS and ignore the result. Just a delay
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.AlternateStatusReg,
                 &Data8 );

    //
    //Check for IDE_BSY bit to be clear
    //
    if(gPlatformIdeProtocol->CommandTimeout == 0) {
        Timeout = COMMAND_COMPLETE_TIMEOUT;
    } else {
        Timeout = (UINT32)gPlatformIdeProtocol->CommandTimeout;
    }
    
    Status = WaitforBitClear( IdeBusInterface->PciIO,
                              Regs.ControlBlock.AlternateStatusReg,
                              IDE_BSY,
                              Timeout);

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Check for ERRORs.
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.StatusReg, &Data8 );

    if ( Data8 & (IDE_ERR | IDE_DF)) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
    Disabled Interrupt generation feature

    @param AMI_IDE_BUS_PROTOCOL *IdeBusInterface;

    @retval EFI_STATUS

**/
void
DisableIdeInterrupt (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    IO_REGS Regs = IdeBusInterface->IdeDevice.Regs;
    UINT8   Data8, Flags;

    //
    // Disable IEN
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.ControlBlock.DeviceControlReg, 2 );

    //
    //Clear interrupt status
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.BusMasterBlock.BMStatusRegister,
                 &Data8 );
    Flags = Data8;
    //
    // Preserve DMA capabilities and set Interrupt bit
    //
    Data8 = (Data8 & 0x60);                      

    if ( Flags & BM_INTERRUPT ) {
        Data8 |= BM_INTERRUPT;
    }
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.BusMasterBlock.BMStatusRegister,
                  Data8 );
}

/**
    Check if IDE_BSY is cleared and IDE_DRDY set

    @param  IdeBusInterface 

    @retval EFI_STATUS

**/
EFI_STATUS
CheckDriveReady (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    IO_REGS Regs = IdeBusInterface->IdeDevice.Regs;
    UINT8   Data8;

    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO,
                  Regs.CommandBlock.DeviceReg,
                  IdeBusInterface->IdeDevice.Device << 4 );

    //
    //Read Alt Status
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 Regs.ControlBlock.DeviceControlReg,
                 &Data8 );

    if ((Data8 & 0xc0) == 0x40 ) {
        return EFI_SUCCESS;
    }

    return EFI_DEVICE_ERROR;
}

/**
    Checks for a particular Bit to be set for a given amount of time

    @param  PciIO 
    @param  UINT16  AlternateStatusReg,
    @param  UINT8   BitSet,
    @param  UINT32  TimeOut

    @retval EFI_STATUS

**/
EFI_STATUS
WaitforBitSet (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    UINT16                  AlternateStatusReg,
    UINT8                   BitSet,
    UINT32                  TimeOut
)
{
    UINT8   Delay;
    UINT8   Data8;

    for (; TimeOut > 0; TimeOut-- )
    {
        for ( Delay = 100; Delay > 0; Delay-- )
        {
            IdeReadByte( PciIO, AlternateStatusReg, &Data8 );

            if ( Data8 & BitSet ) {
                return EFI_SUCCESS;
            }
            pBS->Stall( 10 );        // 10usec
        }
    }
    return EFI_TIMEOUT;
}

/**
    Waits for the given bit to be clear

    @param  UINT16  AlternateStatus,
    @param  UINT8   BitClear
    @param  UINT32  BUSY_CLEAR_TIMEOUT		// Millisecond

    @retval EFI_STATUS

**/
EFI_STATUS
WaitforBitClear (
    IN EFI_PCI_IO_PROTOCOL  *PciIO,
    UINT16                  AlternateStatus,
    UINT8                   BitClear,
    UINT32                  Timeout
)
{
    UINT8   Delay;
    UINT8   Data8;

    for (; Timeout > 0; Timeout-- )
    {
        for ( Delay = 100; Delay > 0; Delay-- )
        {
            IdeReadByte( PciIO, AlternateStatus, &Data8 );

            if ( !(Data8 & BitClear)) {
                return EFI_SUCCESS;
            }
            pBS->Stall( 10 );                // 10 Usec
        }
    }
    return EFI_TIMEOUT;
}

/**
    Waits for the given bit to be clear

    @param  UINT16  AlternateStatus,
    @param  UINT8   BitClear

    @retval EFI_STATUS

**/
EFI_STATUS
WaitforBitClearWithoutTimeout (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    UINT16                  AlternateStatus,
    UINT8                   BitClear )
{
    UINT8   Data8;

    do
    {
        IdeReadByte( PciIO, AlternateStatus, &Data8 );

        if ( !(Data8 & BitClear)) {
            return EFI_SUCCESS;
        }
        pBS->Stall( 10 );                // 10 Usec
    } while ( 1 );

    return EFI_NOT_FOUND;
}

/**
    Returns the MOST significant Bit set.

    @param UINT32		Data

    @retval UINT8

**/
UINT8
ReturnMSBset (
    UINT32  Data
)
{
    UINT8   Index;
    UINT8   Value = 0xFF; 

    for ( Index = 0; Index < 32; Index++ )
    {
        if ( Data & 1 ) {
            Value = Index;
        }
        Data >>= 1;
    }

    return Value;
}

/**
    Reads 1 Byte of data from the IO port

    @param  PciIO 
    @param  Register 
    @param  Data8 

    @retval UINT8   *Data8

**/
EFI_STATUS
IdeReadByte (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    OUT UINT8               *Data8
)
{
    PciIO->Io.         Read(
        PciIO,
        EfiPciIoWidthFifoUint8,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        1,
        Data8 );

    return EFI_SUCCESS;
}

/**
    Reads N Bytes of data from the IO port

    @param PciIO 
    @param Register 
    @param Count 
    @param Data8 

    @retval UINT8   *Data8

**/
EFI_STATUS
IdeReadMultipleByte (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    IN  UINT32              Count,
    OUT UINT8               *Data8
)
{
    EFI_STATUS Status;

    Status = PciIO->Io.Read(
        PciIO,
        EfiPciIoWidthFifoUint8,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        Count,
        Data8 );

    return Status;
}

/**
    Reads 1 Word of data from the IO port

    @param PciIO 
    @param Register 
    @param Data16 

    @retval UINT16  Data16

**/
EFI_STATUS
IdeReadWord (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    OUT UINT16              *Data16
)
{
    EFI_STATUS Status;

    Status = PciIO->Io.Read(
        PciIO,
        EfiPciIoWidthFifoUint16,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        1,
        Data16 );

    return Status;
}

/**
    Reads N Word of data from the IO port

        
    @param PciIO 
    @param Register 
    @param Count (Count in WORDS)
    @param Data16 

    @retval UINT16  *Data16

**/
EFI_STATUS
IdeReadMultipleWord (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    IN  UINT32              Count,
    OUT UINT16             *Data16
)
{
    EFI_STATUS  Status;
    UINT16      *Buffer = Data16;
    BOOLEAN     MemoryAllocated = FALSE;

    //
    //Allocate memory only if ADDRESS is not WORD aligned
    //
    if ((UINTN)Data16 & 1 ) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    Count * sizeof(UINT16),
                                    (VOID**)&Buffer );           

        if ( EFI_ERROR( Status )) {
            return EFI_OUT_OF_RESOURCES;
        }
        MemoryAllocated = TRUE;
    }

    Status = PciIO->Io.Read(
        PciIO,
        EfiPciIoWidthFifoUint16,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        Count,
        Buffer );

    ASSERT_EFI_ERROR(Status);	

    if ( MemoryAllocated ) {
        pBS->CopyMem( Data16, Buffer, Count * sizeof(UINT16));
        pBS->FreePool( Buffer );
    }

    return EFI_SUCCESS;
}

/**
    Writes 1 Byte of data to the IO port

        
    @param PciIO 
    @param Register 
    @param Data8 

    @retval UINT8   *Data8

**/
EFI_STATUS
IdeWriteByte (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    IN  UINT8               Data8
)
{
    PciIO->Io.Write(
        PciIO,
        EfiPciIoWidthFifoUint8,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        1,
        &Data8 );

    return EFI_SUCCESS;
}

/**
    Writes N Bytes of data to the IO port

        
    @param PciIO 
    @param Register 
    @param Count (Count in BYTES)
    @param Data8 

    @retval EFI_STATUS

**/
EFI_STATUS
IdeWriteMultipleByte (
    IN   EFI_PCI_IO_PROTOCOL    *PciIO,
    IN  UINT16                  Register,
    IN  UINT32                  Count,
    IN  UINT8                   *Data8
)
{
    EFI_STATUS  Status;

    Status = PciIO->Io.Write(
        PciIO,
        EfiPciIoWidthFifoUint8,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        Count,
        Data8 );

    return Status;
}

/**
    Writes 1 word of data to the IO port

        
    @param PciIO 
    @param Register 
    @param Data16 

    @retval EFI_STATUS

**/
EFI_STATUS
IdeWriteWord (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    IN  UINT16              Data16
)
{
    PciIO->Io.Write(
        PciIO,
        EfiPciIoWidthFifoUint16,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        1,
        &Data16 );

    return EFI_SUCCESS;
}

/**
    Writes N Words of data to the IO port

        
    @param PciIO 
    @param Register 
    @param Count (Count in WORDS)
    @param Data16 

    @retval 

    @note  
     Count : # of WORDs to write

**/
EFI_STATUS
IdeWriteMultipleWord (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    IN  UINT32              Count,
    IN  UINT16              *Data16
)
{
    EFI_STATUS Status;
    UINT16     *Buffer         = Data16;
    BOOLEAN    MemoryAllocated = FALSE;

    //
    //	Allocate memory only if ADDRESS is not WORD aligned
    //
    if ((UINTN)Data16 & 1 ) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    Count * sizeof(UINT16),
                                    (VOID**)&Buffer );           

        if ( EFI_ERROR( Status )) {
            return EFI_OUT_OF_RESOURCES;
        }
        MemoryAllocated = TRUE;

        pBS->CopyMem( Buffer, Data16, Count * sizeof(UINT16));
    }

    Status = PciIO->Io.Write(
        PciIO,
        EfiPciIoWidthFifoUint16,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        Count,
        Buffer );
		
	ASSERT_EFI_ERROR(Status);

    if ( MemoryAllocated ) {
        pBS->FreePool( Buffer );
    }

    return EFI_SUCCESS;
}

/**
    Writes 1 Dword of data to the IO port

        
    @param PciIO 
    @param Register 
    @param Data16 

    @retval EFI_STATUS

**/
EFI_STATUS
IdeWriteDword (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    IN  UINT16              Register,
    IN  UINT32              Data32
)
{
    PciIO->Io.Write(
        PciIO,
        EfiPciIoWidthFifoUint32,
        EFI_PCI_IO_PASS_THROUGH_BAR,
        Register,
        1,
        &Data32 );

    return EFI_SUCCESS;
}

/**
    Checks if the command is for 48-bit LBA

    @param Command 

    @retval TRUE/FLASE

**/
BOOLEAN
Check48BitCommand (
    IN  UINT8   Command
)
{
    if ( Command == READ_SECTORS_EXT
         || Command == READ_MULTIPLE_EXT
         || Command == WRITE_SECTORS_EXT
         || Command == WRITE_MULTIPLE_EXT
         || Command == READ_DMA_EXT
         || Command == WRITE_DMA_EXT ) {
        return TRUE;
    }
    else {
        return FALSE;
    }
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
