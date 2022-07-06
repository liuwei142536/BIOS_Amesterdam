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

/** @file IdePowerManagement.c
    IDE Power Management Support

**/

//---------------------------------------------------------------------------

#include "IdeBus.h"

//---------------------------------------------------------------------------

//
//External variables
//
extern PLATFORM_IDE_PROTOCOL    *gPlatformIdeProtocol;

/**
    Initializes IDE power Management

    @param IDE_BUS_PROTOCOL *IdeBusInterface;

    @retval EFI_STATUS

**/

EFI_STATUS
InitIDEPowerManagement (
    IN AMI_IDE_BUS_PROTOCOL *IdeBusInterface
)
{

    UINT8       bData = 0;
    EFI_STATUS  Status=EFI_SUCCESS;
    //
    //Check if POWER Management feature is enabled
    //
    if	(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Enabled_85 & 0x8) {
    	 
    //
    //Enable/Disable STANDBY Timer
    //
    	 
            bData = ConvertStanbyTimeoutValue ((UINT16)gPlatformIdeProtocol->StandbyTimeout);
    
            if ( (gPlatformIdeProtocol->StandbyTimeout) == 0 ) {
                Status=IdeNonDataCommand (
                         IdeBusInterface,
                         0,				// Features
                         bData,			// Sector Count
                         0, 			// LBA Low
                         0,				// LBA Mid
                         0,				// LBA High
                         IdeBusInterface->IdeDevice.Device << 4,	// Device
                         IDLE_CMD);     // IDLE_CMD when StandbyTimeout = 0
                if(EFI_ERROR(Status)){
                    return Status;
                }
            } else {
                Status=IdeNonDataCommand (
                         IdeBusInterface,
                         0,				// Features
                         bData,			// Sector Count
                         0,				// LBA Low
                         0,				// LBA Mid
                         0,				// LBA High
                         IdeBusInterface->IdeDevice.Device << 4,	// Device
                         STANDBY_CMD);	// STANDBY_CMD when StandbyTimeout > 0
                if(EFI_ERROR(Status)){
                    return Status;
                }
           }
            //
            //Enable/Disable STANDBY Timer
            //

    }

    //
    //Check Advanced Power Management Mode
    //
    if ( gPlatformIdeProtocol->AdvPowerManagementSupport ) {
        if	(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x8){
            //
            //Enable/Disable Advanced Power Management
            //
            bData = ( gPlatformIdeProtocol->AdvPowerManagementLevel ) == 0 ? DISABLE_ADV_POWER_MANAGEMENT : ADV_POWER_MANAGEMENT;
            IdeSetFeatureCommand (IdeBusInterface, bData, gPlatformIdeProtocol->AdvPowerManagementLevel );
        }
    }


    if ( gPlatformIdeProtocol->PowerupInStandbySupport ) {
        //
        //Check Power-up in Standby Mode Support
        //    
        if	(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x20){
            //
            //Check if the desires state is already present or not
            //
            if	(!((IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Enabled_86 & 0x20)  == (gPlatformIdeProtocol->PowerupInStandbyMode))){
                bData = ( gPlatformIdeProtocol->PowerupInStandbyMode ) == 0 ? DISABLE_POWERUP_IN_STANDBY : ENABLE_POWERUP_IN_STANDBY;			
                IdeSetFeatureCommand (IdeBusInterface, bData, 0);
            }
        }
    }

    return EFI_SUCCESS;
}

/**
    Install  IDE power Management Interface

    @param AMI_IDE_BUS_PROTOCOL *IdeBusInterface;

    @retval EFI_STATUS

**/

EFI_STATUS
InstallIDEPowerMgmtInterface (
    IN AMI_IDE_BUS_PROTOCOL *IdeBusInterface
)
{

	EFI_STATUS                      Status = EFI_UNSUPPORTED;
	AMI_HDD_POWER_MGMT_INTERFACE    *PowerMgmtInterface;


    //
    //Check if Power Management is Supported or not	
    //
	if	(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Enabled_85 & 0x8){
		Status = pBS->AllocatePool (EfiBootServicesData,
					sizeof (AMI_HDD_POWER_MGMT_INTERFACE),
			  	    (VOID**)&PowerMgmtInterface);
		if (EFI_ERROR(Status)) return Status;

        PowerMgmtInterface->CheckPowerMode          = CheckPowerMode;				
        PowerMgmtInterface->IdleMode                = IdleMode;				
        PowerMgmtInterface->StandbyMode             = StandbyMode;				
        PowerMgmtInterface->AdvancePowerMgmtMode    = AdvancePowerMgmtMode;				
        PowerMgmtInterface->IdeBusInterface         = IdeBusInterface;				
        IdeBusInterface->PowerMgmtInterface         = PowerMgmtInterface;				

        //
        //Install the protocol on the device
        //
        Status = pBS->InstallMultipleProtocolInterfaces (
                    &(IdeBusInterface->IdeDeviceHandle),
                    &gAmiHddPowerMgmtProtocolGuid, IdeBusInterface->PowerMgmtInterface,
                    NULL);

        if (EFI_ERROR(Status)) pBS->FreePool(PowerMgmtInterface);
    }

    return Status;
}

/**
    Uninstall  IDE power Management Interface

    @param AMI_IDE_BUS_PROTOCOL *IdeBusInterface;

    @retval EFI_STATUS

**/

EFI_STATUS
StopIDEPowerMgmtInterface (
    IN AMI_IDE_BUS_PROTOCOL *IdeBusInterface
)
{

    EFI_STATUS  Status;

    if (IdeBusInterface->PowerMgmtInterface == NULL) return EFI_SUCCESS;
    Status = pBS->UninstallMultipleProtocolInterfaces (
                    IdeBusInterface->IdeDeviceHandle,
                    &gAmiHddPowerMgmtProtocolGuid, IdeBusInterface->PowerMgmtInterface,
                    NULL);
    if (Status == EFI_SUCCESS) {
        pBS->FreePool(IdeBusInterface->PowerMgmtInterface);
        IdeBusInterface->PowerMgmtInterface = NULL;
    }

    return Status;
}
 //IDE_POWER_MANAGEMENT_INTERFACE_SUPPORT ends

/**
    Initializes IDE power Management

    @param StandbyTimeout (In Minutes)

    @retval UINT8   StandbyTimeout  (Converted Timeout period)

**/

UINT8
ConvertStanbyTimeoutValue (
    IN UINT16    StandbyTimeout
)
{
    // these StandBy Timeout values are as per Ata/Atapi 8 Spec.
    if ( StandbyTimeout ) {
    	if (StandbyTimeout == 21) {
    		return 0xFC;
    	} else if (StandbyTimeout*60 == 1275 ) {
    	    return 0xFF;
    	} else if ( StandbyTimeout < 30 ) {
            if ( StandbyTimeout > 20 ) { 
                StandbyTimeout = 20;	
            }
            return StandbyTimeout * 60 / 5;
        } else if ((StandbyTimeout >= 30) && (StandbyTimeout <= 11 * 30)) {
            return ( 0xF0 + StandbyTimeout / 30);			
        } else if ((StandbyTimeout >=  8*60) && (StandbyTimeout <= 12*60)) {
            return 0xFD;			
        }
    }
    return 0;							// Standby Timer Disabled

}

/**
    Returns the Current State of the device

    @param 

    @retval EFI_STATUS, Current state of the Device, 

    @note  PowerMode : 00 Standby Mode, 80h : Idle Mode, FF : Active/Idle Mode

**/

EFI_STATUS
CheckPowerMode (
    IN  AMI_HDD_POWER_MGMT_PROTOCOL *IdePowerMgmtInterface,
    IN  OUT UINT8                   *PowerMode
)
{
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface = ((AMI_HDD_POWER_MGMT_INTERFACE *)IdePowerMgmtInterface)->IdeBusInterface;

    Status = IdeNonDataCommand (
                IdeBusInterface,
                0,              // Features
                0,              // SectorCount
                0,              // LBA Low
                0,              // LBA Mid
                0,              // LBA High
                IdeBusInterface->IdeDevice.Device << 4,	// Device
                CHECK_POWER_MODE);		// Command

    if (Status == EFI_SUCCESS) {		
        IdeReadByte ( IdeBusInterface->PciIO,  
                      IdeBusInterface->IdeDevice.Regs.CommandBlock.SectorCountReg,
                      PowerMode);
    }		

    return Status;

}


/**
    Puts the device in Idle Mode

    @param StandbyTimeout : Value in Minutes. (0 : Disabled)
	
    @retval EFI_STATUS

**/

EFI_STATUS
IdleMode (
	IN  AMI_HDD_POWER_MGMT_PROTOCOL     *IdePowerMgmtInterface,
	IN  UINT8                           StandbyTimeout
 )
{
    EFI_STATUS                  Status;
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface = ((AMI_HDD_POWER_MGMT_INTERFACE *)IdePowerMgmtInterface)->IdeBusInterface;
    UINT8                       bData;

    bData = ConvertStanbyTimeoutValue ((UINT16)StandbyTimeout);
    Status = IdeNonDataCommand (
                IdeBusInterface,
                0,              // Features
                bData,          // SectorCount
                0,              // LBA Low
                0,              // LBA Mid
                0,              // LBA High
                IdeBusInterface->IdeDevice.Device << 4,	// Device
                IDLE_CMD);		// Command

    return Status;

}

/**
    Puts the device in StandbyMode

    @param StandbyTimeout : Value in Minutes. (0 : Disabled)
	
    @retval EFI_STATUS

**/

EFI_STATUS
StandbyMode (
    IN  AMI_HDD_POWER_MGMT_PROTOCOL *IdePowerMgmtInterface,
    IN  UINT8                       StandbyTimeout
)
{
    EFI_STATUS              Status;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface    =   ((AMI_HDD_POWER_MGMT_INTERFACE *)IdePowerMgmtInterface)->IdeBusInterface;
    UINT8                   bData;

    bData = ConvertStanbyTimeoutValue ((UINT16)StandbyTimeout);
    Status = IdeNonDataCommand (
                IdeBusInterface,
                0,				// Features
                bData,			// SectorCount
                0,				// LBA Low
                0,				// LBA Mid
                0,				// LBA High
                IdeBusInterface->IdeDevice.Device << 4,	// Device
                STANDBY_CMD);		// Command

    return Status;

}

/**
    Enables Advance Power Management

    @param AdvPowerMgmtLevel ( 0 : Disable, FEh : Max. Performance, 81h - FDh Intermediate performance without Standby
        80h : Min. Power Consumption without Standby, 02h - 7Fh Intermediate Performance with Standby
        01h : Min. Power Consumption with Standby)

    @retval 
        EFI_STATUS

**/
EFI_STATUS
AdvancePowerMgmtMode (
    IN AMI_HDD_POWER_MGMT_PROTOCOL      *IdePowerMgmtInterface,
    IN UINT8                            AdvPowerMgmtLevel
)
{
    EFI_STATUS              Status = EFI_UNSUPPORTED;
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface    =   ((AMI_HDD_POWER_MGMT_INTERFACE *)IdePowerMgmtInterface)->IdeBusInterface;
    UINT8                   bData;

    if	(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x8){
        //    
        //Enable/Disable Advanced Power Management
        //
        bData = AdvPowerMgmtLevel == 0 ? DISABLE_ADV_POWER_MANAGEMENT : ADV_POWER_MANAGEMENT;
        Status = IdeSetFeatureCommand (IdeBusInterface, bData, AdvPowerMgmtLevel);
    }

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

