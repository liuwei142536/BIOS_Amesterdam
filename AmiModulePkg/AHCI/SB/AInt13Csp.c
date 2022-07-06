//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AInt13Csp.c
    AHCI INT13 Support Chip set Configuration File

**/
//---------------------------------------------------------------------------

#include "AmiDxeLib.h"
#include "Protocol/PciIo.h"
#include "Include/AcpiRes.h"

//---------------------------------------------------------------------------

#define LBAR_REGISTER		    0x20
#define LBAR_ADDRESS_MASK	    0xFFFFFFE0
#define INDEX_OFFSET_FROM_LBAR  0x10
#define DATA_OFFSET_FROM_LBAR   0x14
#define PCI_ABAR_INDEX          0x5

/**
    This is chip set porting routine that returns PCI index/data ports
    info to access memory-mapped registers.

    @param    PciIo
    @param    AhciAccessWord2
    @param    AhciAccessWord3

    @retval    EFI_SUCCESS Access information is collected
    @retval    EFI_UNSUPPORTED No Access information available

Note:   Project owner is responsible to fill Bus, Device, Function and RegOffset of PCI Index port.
PCI IDP's Data port will be calculated as PCI Index port + 4. 

**/
EFI_STATUS
GetPciIdpInfo (
    IN EFI_PCI_IO_PROTOCOL *PciIo,
    OUT UINTN   *Bus,
    OUT UINTN   *Device,
	OUT UINTN   *Funtion,
	OUT UINTN   *RegOffset
)
{
    return EFI_UNSUPPORTED;
}

/**
    This is chip set porting routine used to port any Chipset specific Index/Data port
    access method supported by the Platform. It also needs the porting changes to be done
    in AhciAcc.csm16 file(i.e. ReadWriteDwordThruChipsetSpecificIdp routine).

    @param    PciIo
    @param    AhciAccessWord1
    @param    AhciAccessWord2
    @param    AhciAccessWord3

    @retval    EFI_SUCCESS Access information is collected
    @retval    EFI_UNSUPPORTED No Access information available

Note: BIT0 and BIT1 of "*AhciAccessWord1" is reserved for AHCI IDP and PCI IDP method respectively.
      Hence, These two BIT fields are reserved.
      Any one of the BIT field (i.e. BIT2 to BIT15 of "*AhciAccessWord1") must set to "0" in order to successfuly
      validate Chipset specific IDP method by AI13.bin.
**/
EFI_STATUS
GetChipSetSpecificIdpInfo (
    IN EFI_PCI_IO_PROTOCOL *PciIo,
    OUT UINT16  *AhciAccessWord1,
    OUT UINT16  *AhciAccessWord2,
    OUT UINT16  *AhciAccessWord3
)
{
    return EFI_UNSUPPORTED;
}

/**
    This is chip set porting routine that returns index/data ports
    to access memory-mapped registers.

    @param    PciIo

    @retval    EFI_SUCCESS Access information is collected
    @retval    EFI_ACCESS_DENIED No Access information available

Note:
Below is the declaration of AHCI_ACCESS defined in AhciAcc.csm16 file,
        AHCI_ACCESS    STRUCT
        IDPType     dw  ?
        Index       dw  ?
        Data        dw  ?
        BaseAddress dd  ?
        AHCI_ACCESS    ENDS

Input parameters points to the above structure defined in AhciAcc.csm16 file directly.

IDPType - AhciAccessWord1
Index   - AhciAccessWord2
Data    - AhciAccessWord3

Though which the data required to access AHCI MMIO can be passed to AhciAcc.csm16.

**/

EFI_STATUS
GetAccessInfo (
    IN EFI_PCI_IO_PROTOCOL *PciIo,
    OUT UINT16  *AhciAccessWord1,
    OUT UINT16  *AhciAccessWord2,
    OUT UINT16  *AhciAccessWord3
)
{
    EFI_STATUS      Status;
    UINT32          lbar;
    UINT64          Supports=0;
    ASLR_QWORD_ASD  *Resources=NULL;
    UINT16          IoData = 0;
    UINT16          Data = 0;
    UINTN           Bus;
    UINTN           Device;
	UINTN           Funtion;
	UINTN           RegOffset;
	
    Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint32, LBAR_REGISTER, 1, &lbar);
    ASSERT_EFI_ERROR(Status);
    
    Status = PciIo->GetBarAttributes( PciIo,
                                      PCI_ABAR_INDEX,
                                      &Supports,
                                      (VOID**)&Resources
                                      );

    ASSERT_EFI_ERROR(Status);

    lbar &= LBAR_ADDRESS_MASK;  // Legacy Bus Master Base Address

    *AhciAccessWord2 = (UINT16)lbar + INDEX_OFFSET_FROM_LBAR;
    *AhciAccessWord3 = (UINT16)lbar + DATA_OFFSET_FROM_LBAR;

    // Note: if AHCI IDP is not valid and it should not be validated by "AInt13Csp" library, Project owner need to comment
    // below if loop in the project.
    // Validating AHCI Index/Data Port
    if ( (*AhciAccessWord2 != 0) && (*AhciAccessWord3 != 0 ) ) {
        IoWrite32(*AhciAccessWord2, 0);
        IoData = (UINT16)IoRead32(*AhciAccessWord3);

        Data = *(volatile UINT16 *)Resources->_MIN;
        if (Data == IoData) {
            *AhciAccessWord1 &= ~BIT0;
            return EFI_SUCCESS;
        }
    }

    // Check whether PCI IDP method is supported or not
    Status = GetPciIdpInfo( PciIo, &Bus, &Device, &Funtion, &RegOffset );

    if (!EFI_ERROR(Status)) {
        *AhciAccessWord1 &= ~BIT1;

		*AhciAccessWord3 = (UINT16)(Bus & 0xFF);
	    *AhciAccessWord2 = (UINT16) ( ((UINT16)(Device & 0x1F) << 11) | ((UINT16)(Funtion & 0x7) << 8) | RegOffset);
		
        return EFI_SUCCESS;
    }

    // Check whether any other Platform specific IDP is supported or not
    Status = GetChipSetSpecificIdpInfo(PciIo, AhciAccessWord1, AhciAccessWord2, AhciAccessWord3 );

    if (!EFI_ERROR(Status)) {
        return EFI_SUCCESS;
    }

    return EFI_UNSUPPORTED;

}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
