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

/** @file IdeBusBoard.c
    Installs PlatformIdeProtocol Interface and Initializes it.
  
**/

//---------------------------------------------------------------------------

#include <AmiDxeLib.h>
#include <Token.h>
#include <Protocol/AmiIdeBusBoard.h>
#include <Protocol/PciIo.h>
#include <Protocol/IdeControllerInit.h>

//---------------------------------------------------------------------------

PLATFORM_IDE_PROTOCOL PlatformIdeProtocol= {

/**
    Variable contains the Revision of PLATFORM_IDE_PROTOCOL.

    @note  UINT8

**/
PLATFORM_IDE_PROTOCOL_REVISION,

/**
    Variable to replace MASTER_SLAVE_ENUMERATION token.

    @note  BOOLEAN

**/
#ifdef MASTER_SLAVE_ENUMERATION
    MASTER_SLAVE_ENUMERATION
#else
    0
#endif
    ,
/**
    Variable to replace IDEBUSMASTER_SUPPORT token.

    @note  BOOLEAN

**/

#ifdef IDEBUSMASTER_SUPPORT
    IDEBUSMASTER_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace ATAPI_BUSMASTER_SUPPORT token.

    @note  BOOLEAN

**/
 
#ifdef ATAPI_BUSMASTER_SUPPORT
    ATAPI_BUSMASTER_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace ACOUSTIC_MANAGEMENT_SUPPORT token.

    @note  BOOLEAN

**/

#ifdef ACOUSTIC_MANAGEMENT_SUPPORT
    ACOUSTIC_MANAGEMENT_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace IDE_POWER_MANAGEMENT_SUPPORT token.

    @note  BOOLEAN

**/

#ifdef IDE_POWER_MANAGEMENT_SUPPORT
    IDE_POWER_MANAGEMENT_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace STANDBY_TIMEOUT token.

    @note  INT16

**/

#ifdef STANDBY_TIMEOUT
    STANDBY_TIMEOUT
#else
    0
#endif
    ,

/**
    Variable to replace ADVANCED_POWER_MANAGEMENT_SUPPORT token.

    @note  BOOLEAN

**/
 
#ifdef ADVANCED_POWER_MANAGEMENT_SUPPORT
    ADVANCED_POWER_MANAGEMENT_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace ADVANCED_POWER_MANAGEMENT_LEVEL token.

    @note  UINT8

**/
 
#ifdef ADVANCED_POWER_MANAGEMENT_LEVEL
    ADVANCED_POWER_MANAGEMENT_LEVEL
#else
    0
#endif
    ,

/**
    Variable to replace POWERUP_IN_STANDBY_SUPPORT token.

    @note  BOOLEAN

**/

#ifdef POWERUP_IN_STANDBY_SUPPORT
    POWERUP_IN_STANDBY_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace POWERUP_IN_STANDBY_MODE token.

    @note  BOOLEAN

**/

#ifdef POWERUP_IN_STANDBY_MODE
    POWERUP_IN_STANDBY_MODE
#else
    0
#endif
    ,

/**
    Variable to replace IDE_POWER_MANAGEMENT_INTERFACE_SUPPORT token.

    @note  BOOLEAN

**/

#ifdef IDE_POWER_MANAGEMENT_INTERFACE_SUPPORT
    IDE_POWER_MANAGEMENT_INTERFACE_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace HOST_PROTECTED_AREA_SUPPORT token.

    @note  BOOLEAN

**/

#ifdef HOST_PROTECTED_AREA_SUPPORT
    HOST_PROTECTED_AREA_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace IDE_HP_SUPPORT token.

    @note  BOOLEAN

**/
 
#ifdef IDE_HP_SUPPORT
    IDE_HP_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace EFI_IDE_PROTOCOL token.

    @note  BOOLEAN

**/

#ifdef EFI_IDE_PROTOCOL
    EFI_IDE_PROTOCOL
#else
    0
#endif
    ,

/**
    Variable to replace AHCI_COMPATIBLE_MODE token.

    @note  BOOLEAN

**/
 
#ifdef AHCI_COMPATIBLE_MODE
    AHCI_COMPATIBLE_MODE
#else
    0
#endif
    ,
/**
    Variable to replace SBIDE_SUPPORT token.

    @note  const BOOLEAN

**/
 
#ifdef SBIDE_SUPPORT
    SBIDE_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace HDD_POWER_LOSS_IN_S3 token.

    @note  const BOOLEAN

**/
 
#ifdef HDD_POWER_LOSS_IN_S3
    HDD_POWER_LOSS_IN_S3
#else
    0
#endif
    ,

/**
    Variable to replace DiPM_SUPPORT token.

    @note  BOOLEAN

**/

#ifdef DiPM_SUPPORT
    DiPM_SUPPORT
#else
    0
#endif
    ,

/**
    Variable to replace DISABLE_SOFT_SET_PREV token.

    @note  BOOLEAN

**/

#ifdef DISABLE_SOFT_SET_PREV
    DISABLE_SOFT_SET_PREV
#else
    0
#endif
    ,

/**
    Variable to replace FORCE_HDD_PASSWORD_PROMPT token.

    @note  BOOLEAN

**/

#ifdef FORCE_HDD_PASSWORD_PROMPT
    FORCE_HDD_PASSWORD_PROMPT
#else
    0
#endif
    ,

/**
    Variable contains Ide Controller Protocol GUID.

    @note  const EFI_GUID

**/

#if defined EFI_IDE_PROTOCOL && EFI_IDE_PROTOCOL == 1
    EFI_IDE_CONTROLLER_INIT_PROTOCOL_GUID
#else
    IDE_CONTROLLER_PROTOCOL_GUID
#endif
   ,

/**
    Variable contains Component Name Protocol GUID.

    @note  const EFI_GUID

**/

    EFI_COMPONENT_NAME2_PROTOCOL_GUID
    ,

/**
    Variable to replace S3_BUSY_CLEAR_TIMEOUT token.

    @note  const UINT32

**/

#ifdef S3_BUSY_CLEAR_TIMEOUT
    S3_BUSY_CLEAR_TIMEOUT
#else
    10000
#endif
    ,

/**
    Variable to replace DMA_ATA_COMMAND_COMPLETE_TIMEOUT token.

    @note  const UINT32

**/

#ifdef DMA_ATA_COMMAND_COMPLETE_TIMEOUT
    DMA_ATA_COMMAND_COMPLETE_TIMEOUT
#else
    5000
#endif
    ,

/**
    Variable to replace DMA_ATAPI_COMMAND_COMPLETE_TIMEOUT token.

    @note  const UINT32

**/

#ifdef DMA_ATAPI_COMMAND_COMPLETE_TIMEOUT
    DMA_ATAPI_COMMAND_COMPLETE_TIMEOUT
#else
    16000
#endif
    ,

/**
    Variable to replace ATAPI_RESET_COMMAND_TIMEOUT token.

    @note  const UINT32

**/

#ifdef ATAPI_RESET_COMMAND_TIMEOUT
    ATAPI_RESET_COMMAND_TIMEOUT
#else
    5000
#endif
    ,

/**
    Variable to replace ATAPI_BUSY_CLEAR_TIMEOUT token.

    @note  const UINT32

**/

#ifdef ATAPI_BUSY_CLEAR_TIMEOUT
    ATAPI_BUSY_CLEAR_TIMEOUT
#else
    16000
#endif
    ,

/**
    Variable to replace POWERON_BUSY_CLEAR_TIMEOUT token.

    @note  const UINT32

**/

#ifdef POWERON_BUSY_CLEAR_TIMEOUT
    POWERON_BUSY_CLEAR_TIMEOUT
#else
    10000
#endif
    ,

/**
    Variable conatains CommandTimeout for ATA Passthru Commands

    @note  const UINT64

**/

    0
};

/**
    Installs PlatformIdeProtocol Interface

    @param ImageHandle 
    @param SystemTable 

    @retval EFI_STATUS


    @note  
      Here is the control flow of this function:
      1. Initialize Ami Lib.
      2. Install PlatformIdeProtocol.
      3. Return EFI_SUCCESS.
**/

EFI_STATUS
IdeBusBoardEntryPoint (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE   *SystemTable
)
{
    EFI_STATUS Status;
    InitAmiLib(ImageHandle,SystemTable);
    Status = pBS->InstallProtocolInterface(\
                    &ImageHandle,\
                    &gAmiPlatformIdeProtocolGuid,\
                    EFI_NATIVE_INTERFACE,\
                    &PlatformIdeProtocol\
                        );
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

