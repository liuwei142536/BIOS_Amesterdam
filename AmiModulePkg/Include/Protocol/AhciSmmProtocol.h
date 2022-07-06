//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AhciSmmProtocol.h
    Protocol definition for Ahci SMM 

**/

#ifndef _AMI_AHCI_SMM_PROTOCOLS_H_
#define _AMI_AHCI_SMM_PROTOCOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Protocol/AmiAhciBus.h>

#define AMI_AHCI_SMM_PROTOCOL_GUID \
        {0xB2FA5764, 0x3B6E, 0x43D3, { 0x91, 0xDF, 0x87, 0xD1, 0x5A, 0x3E, 0x56, 0x68 } }

typedef struct _AMI_AHCI_BUS_SMM_PROTOCOL AMI_AHCI_BUS_SMM_PROTOCOL;


/**
    Initilize the Sata port on S3 resume

    @param SataDevInterface 
    @param Port,

    @retval 
        EFI_STATUS

**/ 
typedef 
EFI_STATUS 
(EFIAPI *AMI_AHCI_SMM_INIT_ON_S3) (
    IN AMI_AHCI_BUS_SMM_PROTOCOL                *SataDevInterface, 
    IN UINT8                                    Port
);


/**
    Execute the PIO data command
                   
    @param SataDevInterface 
    @param CommandStructure 
    @param PortNumber,
    @param PMPortNumber, 
    @param DeviceType,
    @param READWRITE 

    @retval 
        EFI_STATUS

**/ 
typedef 
EFI_STATUS
(EFIAPI *AMI_AHCI_SMM_SATA_DEV_PIO_DATA_IN) (
    IN AMI_AHCI_BUS_SMM_PROTOCOL                *SataDevInterface, 
    IN OUT COMMAND_STRUCTURE                    *CommandStructure,
    UINT8                                       PortNumber,
    UINT8                                       PMPortNumber, 
    DEVICE_TYPE                                 DeviceType,
    IN BOOLEAN                                  READWRITE
);

/**
    Execute the DMA data command
                   
    @param SataDevInterface 
    @param CommandStructure 
    @param PortNumber,
    @param PMPortNumber, 
    @param DeviceType,
    @param READWRITE 

    @retval 
        EFI_STATUS

**/ 
typedef 
EFI_STATUS
(EFIAPI *AMI_AHCI_SMM_SATA_DEV_DMA_DATA_IN) (
    IN AMI_AHCI_BUS_SMM_PROTOCOL                *SataDevInterface, 
    IN OUT COMMAND_STRUCTURE                    *CommandStructure,
    UINT8                                       PortNumber,
    UINT8                                       PMPortNumber, 
    DEVICE_TYPE                                 DeviceType,
    IN BOOLEAN                                  READWRITE
);


/**
    Execute the Non Data command 

    @param SataDevInterface 
    @param CommandStructure 
    @param PortNumber,
    @param PMPortNumber, 
    @param DeviceType,

    @retval 
        EFI_STATUS

**/ 
typedef 
EFI_STATUS 
(EFIAPI *AMI_AHCI_SMM_SATA_DEV_NON_DATA_CMD) (
    IN AMI_AHCI_BUS_SMM_PROTOCOL                *SataDevInterface, 
    IN COMMAND_STRUCTURE                        CommandStructure,
    UINT8                                       PortNumber,
    UINT8                                       PMPortNumber, 
    DEVICE_TYPE                                 DeviceType
);

/**
    Execute a Atapi Packet command
                   
    @param SataDevInterface 
    @param CommandStructure 
    @param READWRITE
    @param PortNumber,
    @param PMPortNumber, 
    @param DeviceType

    @retval 
        EFI_STATUS

    @note  
  1. Stop the Controller
  2. Check if the device is ready to accept a Command. 
  3. Build Command list
  4. Start the Controller.
  5. Wait till command completes. Check for errors.

**/ 
typedef 
EFI_STATUS 
(EFIAPI *AMI_AHCI_SMM_SATA_DEV_PACKET_CMD) (
    IN AMI_AHCI_BUS_SMM_PROTOCOL                *SataDevInterface, 
    IN COMMAND_STRUCTURE                        *CommandStructure,
    IN BOOLEAN                                  READWRITE,
    UINT8                                       PortNumber,
    UINT8                                       PMPortNumber, 
    DEVICE_TYPE                                 DeviceType
);

/**
    Issues a Port Reset

    @param    SataDevInterface  - Pointer to AMI_AHCI_BUS_SMM_PROTOCOL
    @param    Port              - Device's Port Number
    @param    PMPort            - Device's PMPort Number
    @param    DeviceType        - Type of the device (0-ATA, 1-ATAPI)

    @retval EFI_SUCCESS         - Port reset successful
    @retval EFI_DEVICE_ERROR    - Device error/Timeout occurred

    @note    1. Issue port reset by setting DET bit in SControl register
             2. Call HandlePortComReset to check the status of the reset.

**/ 

typedef 
EFI_STATUS 
(EFIAPI *AMI_AHCI_SMM_SATA_DEV_PORT_RESET) (
    AMI_AHCI_BUS_SMM_PROTOCOL           *SataDevInterface, 
    UINT8                    			Port,
    UINT8                    			PMPort,
    DEVICE_TYPE              			DeviceType
);

/**
    Generates Soft Reset

    @param    SataDevInterface  - Pointer to AMI_AHCI_BUS_SMM_PROTOCOL
    @param    Port              - Device's Port Number
    @param    PMPort            - Device's PMPort Number
    @param    DeviceType        - Type of the device (0-ATA, 1-ATAPI)

    @retval EFI_SUCCESS         - Soft reset successful
    @retval EFI_DEVICE_ERROR    - Device error/Timeout occurred

    @note  
  1. Issue a Control register update, H2D register FIS with reset bit set.
  2. Wait for 100usec
  3. Issue a Control register update, H2D register FIS with reset bit reset.

**/ 

typedef 
EFI_STATUS 
(EFIAPI *AMI_AHCI_SMM_SATA_DEV_SOFT_RESET) (
    IN AMI_AHCI_BUS_SMM_PROTOCOL           *SataDevInterface,
    IN UINT8                                Port,
    IN UINT8                                PMPort,
    DEVICE_TYPE                             DeviceType
);

struct _AMI_AHCI_BUS_SMM_PROTOCOL{
    UINT32                                      AhciBaseAddress;
    UINT64                                      PortCommandTableBaseAddr;
    UINT64                                      PortCommandListBaseAddr;
    UINT64                                      PortFISBaseAddr;
    UINT8                                       PortNumber;
    UINT8                                       PMPortNumber; 
    DEVICE_TYPE                                 DeviceType;
    ATAPI_DEVICE                                AtapiDevice;
    AMI_AHCI_SMM_INIT_ON_S3                     AhciSmmInitPortOnS3Resume;
    AMI_AHCI_SMM_SATA_DEV_PIO_DATA_IN           AhciSmmExecutePioDataCommand;
    AMI_AHCI_SMM_SATA_DEV_DMA_DATA_IN           AhciSmmExecuteDmaDataCommand;
    AMI_AHCI_SMM_SATA_DEV_NON_DATA_CMD          AhciSmmExecuteNonDataCommand;
    AMI_AHCI_SMM_SATA_DEV_PACKET_CMD            AhciSmmExecutePacketCommand;
    AMI_AHCI_SMM_SATA_DEV_PORT_RESET            AhciSmmGeneratePortReset;
    AMI_AHCI_SMM_SATA_DEV_SOFT_RESET            AhciSmmGenerateSoftReset;
};

extern  EFI_GUID    gAmiAhciSmmProtocolGuid;

#ifdef __cplusplus
}
#endif
#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
