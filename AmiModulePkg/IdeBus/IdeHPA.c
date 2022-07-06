
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

/** @file IdeHPA.c
    Host Protection Area Support

**/

//---------------------------------------------------------------------------

#include "IdeBus.h"

//---------------------------------------------------------------------------



/**
    Checks whether Host Protected Area feature is supported or not.
    If Supported, installs HpaProtocol.

    @param AMI_IDE_BUS_PROTOCOL			*IdeBusInterface,

    @retval EFI_STATUS  : EFI_UNSUPPORTED/EFI_SUCCESS/EFI_OUT_OF_RESOURCES

**/

EFI_STATUS
InstallHpaInterface (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_STATUS              Status;
    AMI_HDD_HPA_INTERFACE   *HpaInterface;

    //
    //Check if HPA is supported or not
    //    
    if  ((IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_82 & 0x400) == 0 ) {
        return EFI_UNSUPPORTED;
    }

    //
    //Install the Interface
    //
    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof(AMI_HDD_HPA_INTERFACE),
                                (VOID**)&HpaInterface );

    if ( EFI_ERROR( Status ))  {
        pBS->FreePool( HpaInterface );
        return Status;
    }

    HpaInterface->IdeBusInterface     = IdeBusInterface;
    IdeBusInterface->HpaInterface     = HpaInterface;
    HpaInterface->GetNativeMaxAddress = GetNativeMaxAddress;
    HpaInterface->SetMaxAddress       = SetMaxAddress;
    HpaInterface->HpaDisabledLastLBA  = HpaDisabledLastLBA;
    HpaInterface->SetMaxPassword      = SetMaxPassword;
    HpaInterface->SetMaxLock          = SetMaxLock;
    HpaInterface->SetMaxUnLock        = SetMaxUnLock;
    HpaInterface->SetMaxFreezeLock    = SetMaxFreezeLock;

    Status = pBS->InstallMultipleProtocolInterfaces(
        &(IdeBusInterface->IdeDeviceHandle),
        &gAmiHddHpaProtocolGuid, HpaInterface,
        NULL );

    if ( EFI_ERROR( Status )) {
        pBS->FreePool( HpaInterface );
        return Status;
    }

    return EFI_SUCCESS;
}

/**
    Uninstall Hpa Protocol

    @param AMI_IDE_BUS_PROTOCOL *IdeBusInterface,

    @retval EFI_STATUS

**/
EFI_STATUS
StopHpaInterface (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_STATUS  Status;

    if ( IdeBusInterface->HpaInterface == NULL ) {
        return EFI_SUCCESS;
    }

    Status = pBS->UninstallMultipleProtocolInterfaces(
        IdeBusInterface->IdeDeviceHandle,
        &gAmiHddHpaProtocolGuid, IdeBusInterface->HpaInterface,
        NULL );

    if ( Status == EFI_SUCCESS ) {
        pBS->FreePool( IdeBusInterface->HpaInterface );
        IdeBusInterface->HpaInterface = NULL;
    }
    return Status;
}

/**
    Returns the Native Max LBA address supported by the device

    @param  This 
    @retval OUT UINT64  *LBA

    @retval LBA : Returns Native MAX LBA address.
    @retval EFI_STATUS  : EFI_SUCCESS/EFI_DEVICE_ERROR

**/

EFI_STATUS
GetNativeMaxAddress (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    OUT UINT64                  *LBA
)
{
    EFI_STATUS       Status;
    UINT8            ReadNativeMaxAddressCmd = READ_NATIVE_MAX_ADDRESS;
    AMI_IDE_BUS_PROTOCOL *IdeBusInterface = ((AMI_HDD_HPA_INTERFACE*)This)->IdeBusInterface;
    UINT32           dData = 0;
    UINT8            bData = 0;

    *LBA = 0;

    //
    //Check for 48 Bit Mode Support
    //
    if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
        ReadNativeMaxAddressCmd = READ_NATIVE_MAX_ADDRESS_EXT;
    }

    //
    //Get Native Max Address.
    //
    Status = IdeNonDataCommand( IdeBusInterface,
                                0,              // Features
                                0,              // Sector Count
                                0,              // LBA Low
                                0,              // LBA Mid
                                0,              // LBA High
                                0x40 | IdeBusInterface->IdeDevice.Device << 4,  // Device
                                ReadNativeMaxAddressCmd );                      // Command

    if ( EFI_ERROR( Status )) {
        return EFI_UNSUPPORTED;
    }


    //
    //Read Bits 47:24 if 48Bit LBA is supported
    //
    if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
        //
        //Set HOB to 1 to read the upper DWROD
        //
        IdeWriteByte( IdeBusInterface->PciIO,
                      IdeBusInterface->IdeDevice.Regs.ControlBlock.DeviceControlReg,
                      IDE_HOB | IDE_NIEN );

        IdeReadByte( IdeBusInterface->PciIO,
                     IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAHighReg,
                     &bData );

        dData = bData;                              // LBA 47:40
        IdeReadByte( IdeBusInterface->PciIO,
                     IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAMidReg,
                     &bData );
        dData <<= 8;
        dData  |= bData;                             // LBA 39:32

        IdeReadByte( IdeBusInterface->PciIO,
                     IdeBusInterface->IdeDevice.Regs.CommandBlock.LBALowReg,
                     &bData );
        dData <<= 8;
        dData  |= bData;                             // LBA 31:24

        *LBA = dData;
        //
        //Shift Left *LBA by 24 Bits
        //
        *LBA = Shl64(*LBA, 24);
        //
        //Set HOB to 0 to read the Lower DWROD
        //
        IdeWriteByte( IdeBusInterface->PciIO,
                      IdeBusInterface->IdeDevice.Regs.ControlBlock.DeviceControlReg,
                      IDE_NIEN );
    }

    //
    //Read 27:24 only when 48Bit LBA is not supported
    //
    if ((IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400) == 0 ) {
        IdeReadByte( IdeBusInterface->PciIO,
                     IdeBusInterface->IdeDevice.Regs.CommandBlock.DeviceReg,
                     &bData );
        bData &= 0xf;
        dData  = bData;                              // LBA 27:24
    }

    //
    //Read 23:0 bits of LBA address
    //
    IdeReadByte( IdeBusInterface->PciIO,
                 IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAHighReg,
                 &bData );
    dData <<= 8;                                // LBA 23:16
    dData  |= bData;
    IdeReadByte( IdeBusInterface->PciIO,
                 IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAMidReg,
                 &bData );
    dData <<= 8;
    dData  |= bData;                             // LBA 15:8
    IdeReadByte( IdeBusInterface->PciIO,
                 IdeBusInterface->IdeDevice.Regs.CommandBlock.LBALowReg,
                 &bData );
    dData <<= 8;
    dData  |= bData;                             // LBA 7:0

    *LBA= (UINT32) *LBA | dData;

    return EFI_SUCCESS;
}

/**
    Set the Max LBA address

    @param  This 
    @param  UINT64  LBA,
    @param  Volatile 

    @retval EFI_STATUS  :

    @note  if Volatile = TRUE, Values will not be preserved across resets.

**/

EFI_STATUS
SetMaxAddress (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    OUT UINT64                  LBA,
    IN  BOOLEAN                 Volatile
)
{
    EFI_STATUS       Status;
    UINT32           dData;
    UINT32           Cmd;
    UINT64           NativeMaxLBA;
    UINT8            bData;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface    =   ((AMI_HDD_HPA_INTERFACE*)This)->IdeBusInterface;
    BOOLEAN          Ext48BitSupport  = FALSE;


    //
    //Issue Read MAX Native command before issuing a SET MAX Address command
    //
    Status = GetNativeMaxAddress( This, &NativeMaxLBA );

    if ( EFI_ERROR( Status )) {
        return EFI_UNSUPPORTED;
    }

    //
    //Check for 48 Bit Mode Support
    //
    if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
        Ext48BitSupport = TRUE;
    }

    //
    //If 48Bit LBA is supported write LBA bits 47:24
    //
    if ( Ext48BitSupport ) {
        //		Enable HOB
        IdeWriteByte( IdeBusInterface->PciIO,
                      IdeBusInterface->IdeDevice.Regs.ControlBlock.DeviceControlReg,
                      IDE_HOB | IDE_NIEN );
        //get Bits 47:24 from LBA to dData
        dData = (UINT32)Shr64(LBA, 24);

        //dDATA has LBA bits 47:24
        bData = dData >> 16;                // LBA 47:40
        IdeWriteByte( IdeBusInterface->PciIO,
                      IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAHighReg,
                      bData );

        bData = dData >> 8;                 // LBA 39:32
        IdeWriteByte( IdeBusInterface->PciIO,
                      IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAMidReg,
                      bData );

        bData = dData;                      // LBA 31:24
        IdeWriteByte( IdeBusInterface->PciIO,
                      IdeBusInterface->IdeDevice.Regs.CommandBlock.LBALowReg,
                      bData );

        //Reset HOB
        IdeWriteByte( IdeBusInterface->PciIO,
                      IdeBusInterface->IdeDevice.Regs.ControlBlock.DeviceControlReg,
                      IDE_NIEN );
    }

    //
    //	Write Bits 23:0
    //
    dData = (UINT32) LBA;

    //
    //	if 48Bit LBA is not supported, write bits 27:24 to Device register
    //
    if ( Ext48BitSupport == FALSE ) {
        bData = dData >> 24;
    }
    else {
        bData = 0;
    }

    bData &= 0xf;
    bData |= 0x40 | IdeBusInterface->IdeDevice.Device << 4;
    Cmd    = Ext48BitSupport == FALSE ? SET_MAX_ADDRESS : SET_MAX_ADDRESS_EXT;

    //
    //Set Max Address.
    //
    Status = IdeNonDataCommand( IdeBusInterface,
                                0,          // Features
                                Volatile == TRUE ? 0 : 1, // Sector Count
                                (UINT8)dData,       // LBA Low
                                (UINT8)( dData >> 8 ),    // LBA Mid
                                (UINT8)( dData >> 16 ),   // LBA High
                                bData,      // Device
                                Cmd ); // Command

    //
    //	Update Identify Data
    //
    GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));

    //
    //Update the LAST LBA number
    //
    if ( Ext48BitSupport ) {
        IdeBusInterface->IdeBlkIo->BlkIo.Media->LastBlock = IdeBusInterface->IdeDevice.IdentifyData.LBA_48 - 1;
    }
    else {
        IdeBusInterface->IdeBlkIo->BlkIo.Media->LastBlock = IdeBusInterface->IdeDevice.IdentifyData.Addressable_Sector_60 - 1;
    }

    return Status;
}


/**
    Returns the valid last LBA # when HPA is disabled.

    @param This 
    @param  UINT64  *LBA

    @retval EFI_STATUS  : EFI_SUCCESS

    @note  The LBA returned will be the maximum valid LBA number an OS can make use of.

**/

EFI_STATUS
HpaDisabledLastLBA (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    OUT UINT64                  *LBA
)
{
    BOOLEAN Ext48BitSupport = FALSE;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface    =   ((AMI_HDD_HPA_INTERFACE*)This)->IdeBusInterface;

    //
    //Check for 48 Bit Mode Support
    //
    if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
        Ext48BitSupport = TRUE;
    }

    //
    //Update the LAST LBA number
    //
    if ( Ext48BitSupport ) {
        *LBA = IdeBusInterface->IdeDevice.IdentifyData.LBA_48 - 1;
    }
    else {
        *LBA = IdeBusInterface->IdeDevice.IdentifyData.Addressable_Sector_60 - 1;
    }
    return EFI_SUCCESS;
}

/**
    Sets the Password

        
    @param This 
    @param PasswordBuffer (32 Bytes of Password)

    @retval EFI_STATUS  : EFI_UNSUPPORTED/EFI_SUCCESS/EFI_DEVICE_ERROR

    @note  READ NATIVE MAX ADDRESS should not have immediately PRECEEDED this command.
            To take care of this situation, a Dummy Identify Device Command will be given before
            issuing a SET MAX PASSWORD command
            Once the device is locked, it should be unlocked with the password before
            issuing any SETMAXADDRESS.
            After issuing this command device will be in UNLOCKED state.

**/

EFI_STATUS
SetMaxPassword (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    IN  UINT8                   *PasswordBuffer
)
{
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = ((AMI_HDD_HPA_INTERFACE*)This)->IdeBusInterface;

    //
    //Check for HPA feature set security extensions are implemented or not?
    //
    if ( !(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x100)) {
        return EFI_UNSUPPORTED;
    }

    Status = IssueSetMaxPasswordCmd( IdeBusInterface, PasswordBuffer, SET_MAX_SET_PASSWORD );

    //
    //Update the Identify Device Data buffer
    //
    GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));

    return Status;
}

/**
    Locks the device from accepting commands except UNLOCK and FREEZELOCK

    @param This 

    @retval EFI_STATUS  : EFI_UNSUPPORTED/EFI_SUCCESS/EFI_DEVICE_ERROR

    @note   READ NATIVE MAX ADDRESS should not have immediately PRECEED this command.
            To take care of this situation, a Dummy Identify Device Command will be given before
            issuing a SET MAX LOCK/UNLOCK command

**/

EFI_STATUS
SetMaxLock (
    IN  AMI_HDD_HPA_PROTOCOL *This
)
{
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = ((AMI_HDD_HPA_INTERFACE*)This)->IdeBusInterface;

    //Check for HPA feature set security extensions are implemented or not? AND
    //Check whether SET MAX PASSWORD command has been issued before or not?
    if ((!(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x100))
        || (!(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Enabled_86 & 0x100))) {
        return EFI_UNSUPPORTED;
    }

    Status = IdeNonDataCommand(
        IdeBusInterface,
        SET_MAX_LOCK,           // Features
        0,              // Sector Count
        0,              // LBA Low
        0,              // LBA Mid
        0,              // LBA High
        IdeBusInterface->IdeDevice.Device << 4, // Device
        SET_MAX_ADDRESS );       // Command

    return Status;
}

/**
    Unlocks the device and allows it to accept all SET MAX commands

    @param This 
    @param PasswordBuffer 

    @retval EFI_STATUS  : EFI_UNSUPPORTED/EFI_SUCCESS/EFI_DEVICE_ERROR

    @note   READ NATIVE MAX ADDRESS should not have immediately PRECEED this command.
            To take care of this situation, a Dummy Identify Device Command will be given before
            issuing a SET MAX LOCK/UNLOCK command

**/

EFI_STATUS
SetMaxUnLock (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    IN  UINT8                   *PasswordBuffer
)
{
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = ((AMI_HDD_HPA_INTERFACE*)This)->IdeBusInterface;

    //Check for HPA feature set security extensions are implemented or not? AND
    //Check whether SET MAX PASSWORD command has been issued before or not?
    if ((!(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x100))
        || (!(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Enabled_86 & 0x100))) {
        return EFI_UNSUPPORTED;
    }

    Status = IssueSetMaxPasswordCmd( IdeBusInterface, PasswordBuffer, SET_MAX_UNLOCK );

    return Status;
}

/**
    Locks all SET MAX ADDRESS/PASSWORD/LOCK/UNLOCK command until
    next power cycle. GetNativeMaxAddress is allowed.

    @param This 

    @retval EFI_STATUS  : EFI_UNSUPPORTED/EFI_SUCCESS/EFI_DEVICE_ERROR

    @note   READ NATIVE MAX ADDRESS should not have immediately PRECEED this command.
            To take care of this situation, a Dummy Identify Device Command will be given before
            issuing a SET MAX FREEZELOCK command

**/

EFI_STATUS
SetMaxFreezeLock (
    IN  AMI_HDD_HPA_PROTOCOL    *This
)
{
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = ((AMI_HDD_HPA_INTERFACE*)This)->IdeBusInterface;

    //Check for HPA feature set security extensions are implemented or not? AND
    //Check whether SET MAX PASSWORD command has been issued before or not?
    if ((!(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x100))
        || (!(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Enabled_86 & 0x100))) {
        return EFI_UNSUPPORTED;
    }

    Status = IdeNonDataCommand(
        IdeBusInterface,
        SET_MAX_FREEZE_LOCK, // Features
        0,              // Sector Count
        0,              // LBA Low
        0,              // LBA Mid
        0,              // LBA High
        IdeBusInterface->IdeDevice.Device << 4, // Device
        SET_MAX_ADDRESS );       // Command

    return Status;
}

/**
    Sets the Password to Lock/Unlock use of SETMAXADDRESS command

    @param This 
    @param LockUnLock (TRUE : LOCK, FALSE :UNLOCK

    @retval EFI_STATUS  : EFI_UNSUPPORTED/EFI_SUCCESS/EFI_DEVICE_ERROR

    @note   READ NATIVE MAX ADDRESS should not have immediately PRECEED this command.
            To take care of this situation, a Dummy Identify Device Command will be given before
            issuing a SET MAX LOCK/UNLOCK command

**/

EFI_STATUS
IssueSetMaxPasswordCmd (
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT8                   *PasswordBuffer,
    UINT8                   Cmd
)
{
    EFI_STATUS  Status;
    UINT8       *Buffer;

    //Issues Identify Device Command
    //Allocate 512 Bytes
    Status = pBS->AllocatePool( EfiBootServicesData,
                                512,
                                (VOID**)&Buffer );

    if ( EFI_ERROR( Status ))  {
        pBS->FreePool( Buffer );
        return Status;
    }

    Status = GetIdentifyData( IdeBusInterface, Buffer );
	
	ASSERT_EFI_ERROR(Status);

    //
    //Clear the Buffer
    //
    ZeroMemory( Buffer, 512 );

    //
    //Copy 32 bytes to Password starting from offset 2
    //
    pBS->CopyMem( &(Buffer[2]), PasswordBuffer, 32 );

    //	Issue SET MAX PASSWORD Command
    //	Update the Feature Register
    IdeWriteByte( IdeBusInterface->PciIO,
                  IdeBusInterface->IdeDevice.Regs.CommandBlock.FeatureReg,
                  Cmd );
    Status = AtaReadWritePio( IdeBusInterface, Buffer, 512, 0, SET_MAX_ADDRESS, TRUE );

    pBS->FreePool( Buffer );
    return Status;
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

