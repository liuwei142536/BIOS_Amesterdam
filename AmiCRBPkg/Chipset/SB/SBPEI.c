//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//*****************************************************************************
// Revision History
// ----------------
// $Log: $
// 
// 
//*****************************************************************************


//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:  SBPEI.C
//
// Description: This file contains code for Template Southbridge initialization
//    in the PEI stage
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#pragma warning ( disable : 4090 )//CONST warning
// Module specific Includes
#include <Efi.h>
#include <Pei.h>
#include <Token.h>
#include <Hob.h>
#if ATAPI_RECOVERY_SUPPORT
#include <Include\Setup\IioUniversalData.h>
#include <Library\HobLib.h>
#endif
#include <Setup.h>
#include <AmiPeiLib.h>
#include <AmiCspLib.h>
#include <SB.h>
// Produced/used PPI interfaces
#include <ppi\PciCfg2.h> //[EIP64769_04]
#include <PCI.h>
//#include <ppi\SBPPI.h>
#include <ppi\CpuIo.h>
#include <ppi\CspLibPpi.h>
#include <Ppi\SmmControl.h>
#include <Ppi\ReadOnlyVariable2.h>  
#include <ppi\reset.h>
#if ATAPI_RECOVERY_SUPPORT
#include <ppi\AtaController.h>
#endif

//-----------------------------------------------------------------------
// SB : GPIO Initialize Table
//-----------------------------------------------------------------------
AMI_INIT_GPIO_TABLE_STRUCT stSB_GPIOInitTable[] =
{
    //  { Register, AND Mask, OR Mask}
#include <GPIO.h>
    {0xFFFF}
};

UINT16 wSB_GPIOInitTableSize = sizeof(stSB_GPIOInitTable) / sizeof(AMI_INIT_GPIO_TABLE_STRUCT);

EFI_STATUS SBBeforeInitMemory(
    IN CONST EFI_PEI_SERVICES  **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID    *NullPpi);

EFI_STATUS SBAfterInitMemory(
    IN CONST EFI_PEI_SERVICES  **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID    *NullPpi);

VOID SBGbE_Init(VOID);
VOID SBUsbPeiInit(IN CONST EFI_PEI_SERVICES  **PeiServices);  // (P020309E)

EFI_STATUS InitializeSATA(
    IN CONST EFI_PEI_SERVICES **PeiServices); // (P030309E)
    
// Produced PPIs

// GUID Definitions

// Portable Constants

// Function Prototypes

// PPI interface definition
//EFI_GUID gAmiPEISBInitPolicyGuid    = AMI_PEI_SBINIT_POLICY_PPI_GUID;
EFI_GUID gAmiPEIPCITableInitPpiGuid = AMI_PEI_PCI_TABLE_INIT_PPI_GUID;
EFI_GUID gMasterBootModeGuid        = EFI_PEI_MASTER_BOOT_MODE_PEIM_PPI;
EFI_GUID gRecoveryBootModeGuid      = EFI_PEI_BOOT_IN_RECOVERY_MODE_PEIM_PPI;
EFI_GUID gPeiSmmControlPpiGuid      = PEI_SMM_CONTROL_PPI_GUID;
#if ATAPI_RECOVERY_SUPPORT
EFI_GUID gPeiAtaControllerPpiGuid   = PEI_ATA_CONTROLLER_PPI_GUID;
#endif
EFI_GUID gEfiPeiReadOnlyVariablePpiGuid = EFI_PEI_READ_ONLY_VARIABLE2_PPI_GUID;
EFI_GUID gSetupGuid = SETUP_GUID;

//EFI_GUID gPeiPreMRCGuid = AMI_PEI_PRE_MRC_GUID;
EFI_GUID gPeiPermanentMemInstalled  = EFI_PEI_PERMANENT_MEMORY_INSTALLED_PPI;
//EFI_GUID gPeiSBPpiGuid = AMI_PEI_SB_PPI_GUID; // (P111708A)

                                        // (P010609A)>
EFI_GUID gSbSetupHobGuid = AMI_SB_CONFIGURATION_GUID;
#if defined SB_GET_SETUP_SUPPORT && SB_GET_SETUP_SUPPORT == 1
EFI_GUID gEfiSetupGuid = SETUP_GUID;
#endif
                                        // <(P010609A)
//----------------------------------------------------------------------------
// PPI Definition
//----------------------------------------------------------------------------
static EFI_PEI_PPI_DESCRIPTOR mBootModePpi[] =
{
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMasterBootModePpiGuid,//&gMasterBootModeGuid,
    0
};

static EFI_PEI_PPI_DESCRIPTOR mRecoveryModePpi[] =
{
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gRecoveryBootModeGuid,
    0
};

#if ATAPI_RECOVERY_SUPPORT
EFI_STATUS EFIAPI EnableAtaChannel(
    IN CONST EFI_PEI_SERVICES           **PeiServices,
    IN PEI_ATA_CONTROLLER_PPI     *This,
    IN UINT8                      ChannelIndex
);

static PEI_ATA_CONTROLLER_PPI mAtaControllerPpi =
{
    EnableAtaChannel
};
#endif

EFI_STATUS GetChipsetSKU(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    OUT UINT8 *RevisionID);

EFI_STATUS EnableDisplayPort(
    IN CONST EFI_PEI_SERVICES **PeiServices);


//----------------------------------------------------------------------------
// PPI that are installed
//----------------------------------------------------------------------------
#if ATAPI_RECOVERY_SUPPORT
static EFI_PEI_PPI_DESCRIPTOR gPpiList[] =
{
//    {EFI_PEI_PPI_DESCRIPTOR_PPI, &gPeiSBPpiGuid, &gSBPpi}, // (P111708A)
//    {EFI_PEI_PPI_DESCRIPTOR_PPI, &gPeiCspResetPpiGuid, &gCspResetPpi},

    {(EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
            &gPeiAtaControllerPpiGuid, &mAtaControllerPpi}
};
#endif

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SBPEI_Init
//
// Description: This function is the entry point for this PEI. This function
//    initializes the chipset SB
//
// Input:  FfsHeader Pointer to the FFS file header
//         PeiServices Pointer to the PEI services table
//
// Output:  Return Status based on errors that occurred while waiting for
//          time to expire.
//
// Notes:  This function should initialize South Bridge for memory detection.
//    Install AMI_PEI_SBINIT_POLICY_PPI to indicate that SB Init PEIM
//    is installed
//    Following things can be done at this point:
//     - Enabling top of 4GB decode for full flash ROM
//     - Programming South Bridge ports to enable access to South
//      Bridge and other I/O bridge access
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
EFIAPI
SBPEI_Init(
    IN       EFI_PEI_FILE_HANDLE   FileHandle,
    IN CONST EFI_PEI_SERVICES          **PeiServices
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_PEI_PCI_CFG2_PPI         *PciCfg;
    EFI_PEI_CPU_IO_PPI          *CpuIo;
#if USE_AMI_GPIO_PROGRAMMING == 1
    UINT32 USE1_SEL;
    UINT32 USE2_SEL;
    UINT32 USE3_SEL;
    UINT32 IO1_SEL;
    UINT32 IO2_SEL;
    UINT32 IO3_SEL;
    UINT32 LVL1_SEL;
    UINT32 LVL2_SEL;
    UINT32 LVL3_SEL;
    UINT32 INV1_SEL;
#endif

    PciCfg = (*PeiServices)->PciCfg;
    CpuIo = (*PeiServices)->CpuIo;

    PEI_PROGRESS_CODE(PeiServices, PEI_CAR_SB_INIT);

//=============================================================================
//  Program GPIOs.
//=============================================================================
#if USE_AMI_GPIO_PROGRAMMING == 1
    ProgramGPIOTable(GPIO_BASE_ADDRESS, stSB_GPIOInitTable, wSB_GPIOInitTableSize,
            &USE1_SEL,
            &USE2_SEL,
            &USE3_SEL,
            &IO1_SEL,
            &IO2_SEL,
            &IO3_SEL,
            &LVL1_SEL,
            &LVL2_SEL,
            &LVL3_SEL,
            &INV1_SEL
    );
    
    PEI_TRACE((-1, PeiServices, "After ProgramGPIOTable\n"));
    PEI_TRACE((-1, PeiServices, "USE1_SEL = 0x%x\n", USE1_SEL ));
    PEI_TRACE((-1, PeiServices, "IO1_SEL = 0x%x\n", IO1_SEL ));
    PEI_TRACE((-1, PeiServices, "LVL1_SEL = 0x%x\n", LVL1_SEL ));
    PEI_TRACE((-1, PeiServices, "INV1_SEL = 0x%x\n", INV1_SEL ));
    PEI_TRACE((-1, PeiServices, "USE2_SEL = 0x%x\n", USE2_SEL ));
    PEI_TRACE((-1, PeiServices, "IO2_SEL = 0x%x\n", IO2_SEL ));
    PEI_TRACE((-1, PeiServices, "LVL2_SEL = 0x%x\n", LVL2_SEL ));
    PEI_TRACE((-1, PeiServices, "USE3_SEL = 0x%x\n", USE3_SEL ));
    PEI_TRACE((-1, PeiServices, "IO3_SEL = 0x%x\n", IO3_SEL ));
    PEI_TRACE((-1, PeiServices, "LVL3_SEL = 0x%x\n", LVL3_SEL ));

    DisableGPOBlink();
    PEI_TRACE((-1, PeiServices, "Disable GPIO Blink = 0x%x\n", IoRead32((UINT16)GPIO_BASE_ADDRESS + ICH_GPIO_GPO_BLINK)));
#endif

    // Install the SB Init Policy PPI
#if ATAPI_RECOVERY_SUPPORT    
    Status = (*PeiServices)->InstallPpi(PeiServices, gPpiList);
    ASSERT_PEI_ERROR(PeiServices, Status);
#endif    

    return EFI_SUCCESS;
}


#if ATAPI_RECOVERY_SUPPORT

typedef struct
{
    EFI_PEI_PCI_CFG_PPI_WIDTH PciCfgWidth;
    UINTN Register;
    UINTN Value;
} REG_VALUE_TABLE_ENTRY;

REG_VALUE_TABLE_ENTRY IdeControllerRegValueTable[] =
{
    EfiPeiPciCfgWidthUint16, PCI_CMD, B_ICH_IDE_COMMAND_IOSE,
    EfiPeiPciCfgWidthUint16, R_ICH_IDE_TIMP, B_ICH_IDE_DEC_EN,
    EfiPeiPciCfgWidthUint16, R_ICH_IDE_TIMS, B_ICH_IDE_DEC_EN
};

EFI_GUID gIdeRecoveryNativeModePpiGuid = PEI_IDE_RECOVERY_NATIVE_MODE_PPI_GUID;

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: EnableAtaChannel
//
// Description: Enable ATA Channels.
//
// Input: IN CONST EFI_PEI_SERVICES               **PeiServices,
//        IN PEI_ATA_CONTROLLER_PPI         *This,
//        IN UINT8                          ChannelMask
//
// Output: Status
//
// Note:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS EnableAtaChannel(
    IN CONST EFI_PEI_SERVICES               **PeiServices,
    IN PEI_ATA_CONTROLLER_PPI         *This,
    IN UINT8                          ChannelMask
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_PEI_PCI_CFG2_PPI *PciCfg = 0;
    UINT8 PciData8 = 0;
    UINT16 PciData16 = 0;
    UINT16 SbTempIoBase = 0;
    UINTN i = 0, NumEntries = 0;
    EFI_PEI_PPI_DESCRIPTOR *IdeRecoveryNativeModePpiDescriptor;
    PEI_IDE_RECOVERY_NATIVE_MODE_PPI *IdeRecoveryNativeModePpi;
    IIO_UDS             *IohUds;
    EFI_GUID            UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;  
    EFI_HOB_GUID_TYPE   *GuidHob;


    PciCfg = (*PeiServices)->PciCfg;

    PEI_TRACE((-1, PeiServices, "\n In EnableAtaChannel \n"));

    //map register
    PciData8 = V_SATA_MAP_IDE;
    PciCfg->Write(
        PeiServices,
        PciCfg,
        EfiPeiPciCfgWidthUint8,
        (SATA0_BUS_DEV_FUNC | R_ICH_SATA_AMR),
        &PciData8
    );
    PEI_TRACE((-1, PeiServices, "map registerl \n"));
    //programming interface register - default value - legacy mode
    PciData8 = PI_PNE_LEGACY | PI_SNE_LEGACY;
    PciCfg->Write(
        PeiServices,
        PciCfg,
        EfiPeiPciCfgWidthUint8,
        (SATA0_BUS_DEV_FUNC | Programming_Interface),
        &PciData8
    );

    //default: everything is disabled
    PciData8 = B_SATA_PORT_0_ENABLED | B_SATA_PORT_1_ENABLED | B_SATA_PORT_2_ENABLED | B_SATA_PORT_3_ENABLED;
    PciCfg->Write(
        PeiServices,
        PciCfg,
        EfiPeiPciCfgWidthUint8,
        (SATA0_BUS_DEV_FUNC | R_ICH_SIDE_PCS),
        &PciData8);

    PciData16 = 0xF0A0;
    PciCfg->Write(
        PeiServices,
        PciCfg,
        EfiPeiPciCfgWidthUint16,
        (SATA0_BUS_DEV_FUNC | PCI_BAR4),
        &PciData16);

    PciData16 = 0xF0F0;
    PciCfg->Write(
        PeiServices,
        PciCfg,
        EfiPeiPciCfgWidthUint16,
        (SATA0_BUS_DEV_FUNC | PCI_BAR4),
        &PciData16);

    NumEntries = sizeof(IdeControllerRegValueTable) / sizeof(REG_VALUE_TABLE_ENTRY);
    //sata function number - PCI_FUNCTION_NUMBER_ICH_SATA
    for (i = 0; i < NumEntries; i++)
    {
        PciCfg->Write(
            PeiServices,
            PciCfg,
            IdeControllerRegValueTable[i].PciCfgWidth,
            (SATA0_BUS_DEV_FUNC | IdeControllerRegValueTable[i].Register),
            &IdeControllerRegValueTable[i].Value);
    }

    PciCfg->Read(
        PeiServices,
        PciCfg,
        EfiPeiPciCfgWidthUint16,
        (SATA1_BUS_DEV_FUNC),
        &PciData16);

    PEI_TRACE((-1, PeiServices, "before PciData16 loop \n"));
    if (PciData16 != 0xFFFF)
    {
        PEI_TRACE((-1, PeiServices, "inside PciData16 loop \n"));
        //programming interface register
        PciData8 = PI_PNE_NATIVE | PI_SNE_NATIVE;
        PciCfg->Write(
            PeiServices,
            PciCfg,
            EfiPeiPciCfgWidthUint8,
            (SATA1_BUS_DEV_FUNC | Programming_Interface),
            &PciData8
        );

        //default: everything is disabled
        PciData8 = B_SATA_PORT_0_ENABLED | B_SATA_PORT_1_ENABLED;
        PciCfg->Write(
            PeiServices,
            PciCfg,
            EfiPeiPciCfgWidthUint8,
            (SATA1_BUS_DEV_FUNC | R_ICH_SIDE_PCS),
            &PciData8);
        
        GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
        IohUds = GET_GUID_HOB_DATA (GuidHob);
        ASSERT_PEI_ERROR(PeiServices, IohUds);
        
        SbTempIoBase = IohUds->PlatformData.IIO_resource[0].PciResourceIoBase;
        PEI_TRACE((-1, PeiServices, "AMI: IohUds->PlatformData.IIO_resource[0].PciResourceIoBase 0x%X \n", IohUds->PlatformData.IIO_resource[0].PciResourceIoBase));
            
        Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (EFI_PEI_PPI_DESCRIPTOR), &IdeRecoveryNativeModePpiDescriptor);
        ASSERT_PEI_ERROR(PeiServices, Status);

        Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (PEI_IDE_RECOVERY_NATIVE_MODE_PPI), &IdeRecoveryNativeModePpi);
        ASSERT_PEI_ERROR(PeiServices, Status);

        (*PeiServices)->SetMem((VOID *)IdeRecoveryNativeModePpi, sizeof (PEI_IDE_RECOVERY_NATIVE_MODE_PPI), 0);

        IdeRecoveryNativeModePpi->PCMDBarAddress = SbTempIoBase + 0X230 ;
        IdeRecoveryNativeModePpi->PCNLBarAddress = SbTempIoBase + 0X222 ;
        IdeRecoveryNativeModePpi->SCMDBarAddress = SbTempIoBase + 0X210 ;
        IdeRecoveryNativeModePpi->SCNLBarAddress = SbTempIoBase + 0X202 ;
        PciData16 = IdeRecoveryNativeModePpi->PCMDBarAddress;
        PciCfg->Write(
            PeiServices,
            PciCfg,
            EfiPeiPciCfgWidthUint16,
            (SATA1_BUS_DEV_FUNC | R_ICH_IDE_PCMD_BAR),
            &PciData16);


        PciData16 = IdeRecoveryNativeModePpi->PCNLBarAddress;
        PciCfg->Write(
            PeiServices,
            PciCfg,
            EfiPeiPciCfgWidthUint16,
            (SATA1_BUS_DEV_FUNC | R_ICH_IDE_PCNL_BAR),
            &PciData16);

        PciData16 = IdeRecoveryNativeModePpi->SCMDBarAddress;
        PciCfg->Write(
            PeiServices,
            PciCfg,
            EfiPeiPciCfgWidthUint16,
            (SATA1_BUS_DEV_FUNC | R_ICH_IDE_SCMD_BAR),
            &PciData16);

        PciData16 = IdeRecoveryNativeModePpi->SCNLBarAddress;
        PciCfg->Write(
            PeiServices,
            PciCfg,
            EfiPeiPciCfgWidthUint16,
            (SATA1_BUS_DEV_FUNC | R_ICH_IDE_SCNL_BAR),
            &PciData16);

        NumEntries = sizeof(IdeControllerRegValueTable) / sizeof(REG_VALUE_TABLE_ENTRY);
        //sata function number - PCI_FUNCTION_NUMBER_ICH_SATA
        for (i = 0; i < NumEntries; i++)
        {
            PciCfg->Write(
                PeiServices,
                PciCfg,
                IdeControllerRegValueTable[i].PciCfgWidth,
                (SATA1_BUS_DEV_FUNC | IdeControllerRegValueTable[i].Register),
                &IdeControllerRegValueTable[i].Value);
        }

    IdeRecoveryNativeModePpiDescriptor->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    IdeRecoveryNativeModePpiDescriptor->Guid = &gIdeRecoveryNativeModePpiGuid;
    IdeRecoveryNativeModePpiDescriptor->Ppi = IdeRecoveryNativeModePpi;
    Status = (*PeiServices)->InstallPpi(PeiServices, IdeRecoveryNativeModePpiDescriptor);

        PEI_TRACE((-1, PeiServices, "LocatePpi->IdeRecoveryNativeModePpiDescriptor: %r", Status));
        if (EFI_ERROR(Status)) return Status;
    }

    return EFI_SUCCESS;
}

#endif //ATAPI_RECOVERY_SUPPORT

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

