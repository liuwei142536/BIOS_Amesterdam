//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.            **
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

//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:        SBGeneric.C
//
// Description: This file contains south bridge related code that is needed
//              for both PEI & DXE stage.  To avoid code duplication this
//              file is made as a library and linked both in PEI & DXE
//              south bridge FFS.
//
// Notes:       MAKE SURE NO PEI OR DXE SPECIFIC CODE IS NEEDED
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>


// Module specific Includes
#include <EFI.h>
#include <Token.h>
#include <AmiPeiLib.h>
#include <AmiDxeLib.h>
#include <AmiCspLib.h>
#include <SBCspLib.h>
#include <Protocol\BootScriptSave.h>
#include <SB.h>
#include "Cpu\CpuRegs.h"
#include "CpuHost.h"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#if CSM_SUPPORT

#include <Protocol\PciRootBridgeIo.h>
#include <Protocol\LegacyInterrupt.h>

typedef struct
{
    UINT64                  Address;
    EFI_BOOT_SCRIPT_WIDTH   Width;
} BOOT_SCRIPT_PCI_REGISTER_SAVE;

BOOT_SCRIPT_PCI_REGISTER_SAVE gPirqRegistersSave[] =
{
  (LPC_BUS << 24) + (LPC_DEVICE << 16) + (LPC_FUNC << 8) + ICH_REG_LPC_PIRQA_ROUT,  EfiBootScriptWidthUint32,
  (LPC_BUS << 24) + (LPC_DEVICE << 16) + (LPC_FUNC << 8) + ICH_REG_LPC_PIRQE_ROUT,  EfiBootScriptWidthUint32
};


//----------------------------------------------------------------------------
// Start OF CSM Related Porting Hooks
//----------------------------------------------------------------------------
#define MAX_PIRQS 8


UINT8 bMaxPIRQ        = MAX_PIRQS;      // For CSM
UINT8 bRouterBus      = LPC_BUS;        // PORTING REQUIRED (Use appropriate Equate)
UINT8 bRouterDevice   = LPC_DEVICE;     // PORTING REQUIRED (Use appropriate Equate)
UINT8 bRouterFunction = LPC_FUNC;       // PORTING REQUIRED (Use appropriate Equate)
UINT8 RRegs[MAX_PIRQS] = {ICH_REG_LPC_PIRQA_ROUT, ICH_REG_LPC_PIRQB_ROUT, \
                          ICH_REG_LPC_PIRQC_ROUT, ICH_REG_LPC_PIRQD_ROUT, \
                          ICH_REG_LPC_PIRQE_ROUT, ICH_REG_LPC_PIRQF_ROUT, \
                          ICH_REG_LPC_PIRQG_ROUT, ICH_REG_LPC_PIRQH_ROUT}; // Porting required

// Local variable
static UINT8      PirqRegBuffer[MAX_PIRQS] = {0, 0, 0, 0, 0, 0, 0, 0};
EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPciRootBridgeIo;

#endif  // END OF CSM_SUPPORT           // [EIP33943]

                                        // (P091109D.1)>
UINT32 gRST1_SEL = 0;
UINT32 gRST2_SEL = 0;
UINT32 gRST3_SEL = 0;
                                        // <(P091109D.1)

#ifndef DISABLE_PWR_BUTTON                      // [ EIP47645 ]
#define DISABLE_PWR_BUTTON  0
#endif  // DISABLE_PWR_BUTTON
#if DISABLE_PWR_BUTTON == 0                     // [ EIP47645 ]
UINT16 gPm1EnBuffer;                    // (P022610A)
#endif  // #if DISABLE_PWR_BUTTON

UINT64  ReadMsr (UINT32 Msr);
VOID    WriteMsr(UINT32 Msr, UINT64 Value);

                                        // (P101309E)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   ClearGPIOResetSelect
//
// Description: Intel PCH Specification update rev 1.02,
//              Document Changes 24. Update sec 19.5
//              19.5 Addttional Consideration, Step 2
//              System BIOS is recommended to clear "GPIO Reset Select"
//              registers [GP_RST_SEL1(GPIOBASE + offset 60h), GP_RST_SEL2
//              (GPIOBASE + offset 64h), GP_RST_SEL3(GPIOBASE + offset 68h)
//              before issuing a hard or global reset unless specially
//              requested by the platform designer.
//
// Input:       None
//
// Output:      None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ClearGPIOResetSelect(VOID)
{
                                        // (P091109D.1)>
  IoWrite32(GPIO_BASE_ADDRESS + ICH_GPIO_GP_RST_SEL1, gRST1_SEL);
  IoWrite32(GPIO_BASE_ADDRESS + ICH_GPIO_GP_RST_SEL2, gRST2_SEL);
  IoWrite32(GPIO_BASE_ADDRESS + ICH_GPIO_GP_RST_SEL3, gRST3_SEL);
                                        // <(P091109D.1)
}                                       // <(P101309E)


#if CSM_SUPPORT                         // [EIP33943]
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBGen_InitializeRouterRegisters
//
// Description: This function is clears the routing registers to default values
//
// Input:       PciRBIo     Root bridge IO protocol pointer
//
// Output:      EFI_STATUS  Returned from PCI read call
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
SBGen_InitializeRouterRegisters(
    IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRBIo
)
{
    EFI_STATUS    Status = EFI_SUCCESS;
    UINT8         Idx;
    UINT8         Data;


    for (Idx = 0; Idx < MAX_PIRQS; Idx++)
    {
        Data = 0;
        Status = PciRBIo->Pci.Write(
                     PciRBIo,
                     EfiPciWidthUint8,
                     LPC_BUS_DEV_FUNC | RRegs[Idx],
                     1,
                     &Data
                 );
    }

    gPciRootBridgeIo = PciRBIo;           // Save RB IO value for later use
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBGen_GetPIRQIndex
//
// Description: This function returns the 0 based PIRQ index (PIRQ0, 1 etc)
//              based on the PIRQ register number specified in the routing
//              table
//
// Input:       PIRQRegister  Register number of the PIR
//
// Output:      PIRQIndex 0 - (MAX_PIRQ -1)
//                        0xFF if the register is invalid
//
// Notes:       Here is the control flow of this function:
//                1. If Invalid PirqNumber, return EFI_INVALID_PARAMETER.
//                2. Read into *PriqData from PIRQ register for Pirq requested.
//                3. Return EFI_SUCCESS.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8
SBGen_GetPIRQIndex(IN  UINT8 PIRQRegister)
{
    UINT8   rrIndx = 0;

    while ((rrIndx < MAX_PIRQS) && (RRegs[rrIndx] != PIRQRegister)) rrIndx++;

    if (rrIndx == MAX_PIRQS) return 0xFF;

    return rrIndx;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBGen_ReadPirq
//
// Description: This function reads the IRQ associated with the PIRQ from
//              the chipset register
//
// Input:       This        Pointer to Legacy interrupt protocol
//              PirqNumber  PIRQ number to read
//              PirqData    IRQ programmed for this PIRQ (BIT7 will be
//                          set if the PIRQ is not programmed)
//
// Output:      EFI_STATUS
//                EFI_SUCCESS             On successfull IRQ value return
//                EFI_INVALID_PARAMETER   If PirqNumber is greater than max PIRQs
//
// Notes:       Here is the control flow of this function:
//                1. If Invalid PirqNumber, return EFI_INVALID_PARAMETER.
//                2. Read into *PriqData from PIRQ register for Pirq requested.
//                3. Return EFI_SUCCESS.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
SBGen_ReadPirq(
    IN EFI_LEGACY_INTERRUPT_PROTOCOL    *This,
    IN UINT8                            PirqNumber,
    OUT UINT8                           *PirqData
)
{

    if (PirqNumber >= MAX_PIRQS) return EFI_INVALID_PARAMETER;
    *PirqData = PirqRegBuffer[PirqNumber];
    *PirqData |= (PirqRegBuffer[PirqNumber]) ? 0x00 : 0x80;

    // If Pirq is not routed, bit 7 is set, however specification does not specify a return error for
    // this condition.

    return EFI_SUCCESS;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBGen_WritePirq
//
// Description: This function writes an IRQ value allocated for the PIRQ by
//              programming the chipset register
//
// Input:       This        Pointer to Legacy interrupt protocol
//              PirqNumber  PIRQ number to read
//              PirqData    IRQ to be programmed
//
// Output:      EFI_STATUS
//                EFI_SUCCESS             On successfull IRQ value return
//                EFI_INVALID_PARAMETER   If PirqNumber is greater than
//                                        max PIRQs or PirqData is greater
//                                        than 15 (MAX IRQ)
//
// Notes:       Here is the control flow of this function:
//                1. If Invalid PirqNumber, return EFI_INVALID_PARAMETER.
//                2. Write PirqData to PIRQ register for Pirq requested.
//                3. Return EFI_SUCCESS.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
SBGen_WritePirq(
    IN EFI_LEGACY_INTERRUPT_PROTOCOL    *This,
    IN UINT8                            PirqNumber,
    IN UINT8                            PirqData
)
{
    UINT64            Address;
    EFI_STATUS        Status;
    UINT16            PirqValue = PirqData & 0x0F;

    if (PirqNumber >= MAX_PIRQS) return EFI_INVALID_PARAMETER;
    if (PirqData > 15) return EFI_INVALID_PARAMETER;


    Address = LPC_BUS_DEV_FUNC | RRegs[PirqNumber];

    Status = gPciRootBridgeIo->Pci.Write(
                 gPciRootBridgeIo,
                 EfiPciWidthUint8,
                 Address,
                 1,
                 &PirqValue  // clear upper nibble
             );
    if (EFI_ERROR(Status)) return Status;

    PirqRegBuffer[PirqNumber] = PirqData; // Update router register buffer

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBGen_WriteBootScript
//
// Description: This writes the PIRQ to boot script before booting.
//
// Input:       EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave
//
// Output:      Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SBGen_WriteBootScript(
    IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave
)
{
    UINTN     i;
    UINT32    Value;

    //Porting required: Write Boot Script

    for (i = 0; i < sizeof(gPirqRegistersSave) / sizeof(BOOT_SCRIPT_PCI_REGISTER_SAVE); ++i)
    {
        gPciRootBridgeIo->Pci.Read(
            gPciRootBridgeIo,
            gPirqRegistersSave[i].Width,
            gPirqRegistersSave[i].Address,
            1,
            &Value
        );
        BOOT_SCRIPT_S3_PCI_CONFIG_WRITE_MACRO(
            BootScriptSave,
            gPirqRegistersSave[i].Width,
            gPirqRegistersSave[i].Address,
            1,
            &Value
        );
    }

    Value = SW_SMI_SB_ACPI_S3;
    BOOT_SCRIPT_S3_IO_WRITE_MACRO(
        BootScriptSave,
        EfiBootScriptWidthUint8,
        SW_SMI_IO_ADDRESS,
        1,
        &Value
    );

    return EFI_SUCCESS;
}



//----------------------------------------------------------------------------
// END OF CSM Related Porting Hooks
//----------------------------------------------------------------------------
#endif  // END OF CSM_SUPPORT
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


//<AMI_PHDR_START>
//----------------------------------------------------------------------------------------
// Procedure: SBLib_Shutdown
// Description: This function Shuts the system down (S5)
//
// Input: VOID
//
// Output: VOID
//
//-----------------------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID SBLib_Shutdown(VOID)
{
//####     UINT16 Value;
//####    //-------
//####     //Check if Sleep SMI is enabled we will disable it.
//####     Value = IoRead16(PM_BASE_ADDRESS+0x30);
//####     Value&=(~BIT04);
//####     IoWrite16(PM_BASE_ADDRESS+0x30,Value);

//####     //Clear All PM  Statuses
//####     Value = IoRead16(PM_BASE_ADDRESS);
//####     IoWrite16(PM_BASE_ADDRESS,Value);

//####     //Go to S5
//####     Value = IoRead16(PM_BASE_ADDRESS + 4);
//####     IoWrite16(PM_BASE_ADDRESS + 4,(UINT16)(Value | (0xf << 10)));
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------------------
// Procedure: SB_Shutdown
// Description: This function Shuts the system down (S5)
//
// Input: VOID
//
// Output: VOID
//
//-----------------------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SB_Shutdown(VOID)
{
    UINT16 Value;

    //Check if Sleep SMI is enabled we will disable it.
    Value = IoRead16(PM_BASE_ADDRESS + ICH_IOREG_SMI_EN);
    Value &= (~BIT04);
    IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_SMI_EN, Value);

    //Clear All PM Statuses
    Value = IoRead16(PM_BASE_ADDRESS);
    IoWrite16(PM_BASE_ADDRESS, Value);

    //Go to S5
    Value = IoRead16(PM_BASE_ADDRESS + ICH_IOREG_PM1_CNT);
    IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_PM1_CNT, (UINT16)(Value | (0xf << 10)));

    EFI_DEADLOOP()                      // (P062609B)
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbLib_GetSmiState
//
// Description: This function returns SMI state
//
// Input:  None
//
// Output:  TRUE - SMI enabled, FALSE - SMI disabled
//
// Note:        This function must work at runtime. Do not use boot time services/protocols
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN SbLib_GetSmiState(VOID)
{
#if SMM_SUPPORT
                                        // [EIP25382]>
    static BOOLEAN    OriginalSmiState = FALSE;
    static BOOLEAN    HadBeenCame = FALSE;
    volatile BOOLEAN  SmiState = (IoRead32(PM_BASE_ADDRESS + ICH_IOREG_SMI_EN) & BIT00) ? \
                                                                 TRUE : FALSE;

    if (HadBeenCame && SmiState) HadBeenCame = FALSE;

    if (HadBeenCame) {
        SmiState = OriginalSmiState;
    } else {
        OriginalSmiState = SmiState;
        HadBeenCame = TRUE;
    }

    return SmiState;
                                        // <[EIP25382]
#else
    return FALSE;
#endif
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbLib_SmiDisable
//
// Description: This function disables SMI
//
// Input:  None
//
// Output:  None
//
// Note:        This function should be used ONLY in critical parts of code.
//              This function must work at runtime. Do not use boot time
//              services/protocols
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SbLib_SmiDisable(VOID)
{
#if SMM_SUPPORT
    UINT32  SmiCtl = IoRead32(PM_BASE_ADDRESS + ICH_IOREG_SMI_EN) ;
//----------------------------------
    SmiCtl &= (~BIT01); //Mask OFF EOS bit
    IoWrite32(PM_BASE_ADDRESS + ICH_IOREG_SMI_EN, SmiCtl & (~BIT00));
#endif
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbLib_SmiEnable
//
// Description: This function enables SMI
//
// Input:  None
//
// Output:  None
//
// Note: This function must work at runtime. Do not use boot time services/protocols
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SbLib_SmiEnable(VOID)
{
#if SMM_SUPPORT
    UINT32  SmiCtl = IoRead32(PM_BASE_ADDRESS + ICH_IOREG_SMI_EN);
//----------------------------------
    SmiCtl &= (~BIT01); //Mask OFF EOS bit
    IoWrite32(PM_BASE_ADDRESS + ICH_IOREG_SMI_EN, SmiCtl | BIT00);
#endif
}


                                        // (P062409B)>
#if SMM_SUPPORT
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SBSmmSaveRestoreStates
//
// Description: This hook is called in the very SMI entry and exit.
//  Save/Restore chipset data if needed.
//
// Input: BOOLEAN -- Save
//
// Output: None
//
// Notes: None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SBSmmSaveRestoreStates(IN BOOLEAN Save)
{
    static UINT8    bStoreCMOS;
    static UINT8    bStoreExtCMOS;
    static UINT32   dStoreCF8;
    UINT8           i;

    if (Save)
    {
      dStoreCF8 = IoRead32(0xcf8);      // Store CF8 (PCI index)

      // Enable Alternate access mode, let port 70h can be readable.
      Mmio32(SB_RCBA, R_RCBA_GCS) |= 0x10;
      i = Mmio8(SB_RCBA, R_RCBA_GCS);   // (P122909A)

      // Read until value other than 0xFF returned or 100 reads performed
      for (i = 0; i < 100; i++) {
        bStoreCMOS = IoRead8(CMOS_ADDR_PORT);
        if (bStoreCMOS != 0xFF)
            break;
      }

      // Disable Alternate access mode.
      Mmio32(SB_RCBA, R_RCBA_GCS) &= ~0x10;
      i = Mmio8(SB_RCBA, R_RCBA_GCS);   // (P122909A)
      bStoreCMOS &= 0x80;               // Combine CMOS index with NMI bit
      bStoreCMOS |= IoRead8(CMOS_IO_STD_INDEX);      // Store 0x74 standard CMOS index
      bStoreExtCMOS = IoRead8(CMOS_IO_EXT_INDEX);    // Store 0x72 extended CMOS index
    }
    else
    {
      IoWrite32(0xcf8, dStoreCF8);      // Restore 0xCF8 (PCI index)
      IoWrite8(CMOS_ADDR_PORT, bStoreCMOS);       // Restore 0x70(CMOS index)
      IoWrite8(CMOS_IO_EXT_INDEX, bStoreExtCMOS); // Restore 0x72(EXT-CMOS index)
    }

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbInstallSmmConfigurationTable
//
// Description: This hook is install SMM configuration table.
//
// Input:   EFI_SMM_SYSTEM_TABLE *SmmSystemTable
//          EFI_GUID *Guid
//          VOID* Table
//          UINTN TableSize
//
// Output:  None
//
// Note:    None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SbInstallSmmConfigurationTable(
    IN EFI_SMM_SYSTEM_TABLE *SmmSystemTable,
    IN EFI_GUID *Guid,
    IN VOID* Table,
    IN UINTN TableSize)
{
    EFI_STATUS Status;

    Status = SmmSystemTable->SmmInstallConfigurationTable(
                    SmmSystemTable,
                    Guid,
                    Table,
                    TableSize);

    return Status;
}

#endif  // END OF SMM Related Porting Hooks
                                         // <(P062409B)


#if SB_RESET_PPI_SUPPORT

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SetResetType
//
// Description: This function saves the input ResetType in "ResetType"
//              variable
//
// Input:       ResetType   Type of reset to be generated
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SetResetType(
    IN EFI_RUNTIME_SERVICES *pRS,
    IN EFI_RESET_TYPE       ResetType
)
{
    EFI_STATUS  Status       = EFI_SUCCESS;
    UINTN       DataSize     = sizeof(UINT32);
    EFI_GUID    VariableGuid = AMI_CSP_RESET_GUID;
    UINT32  CurrentResetType = SoftReset;

    Status = pRS->GetVariable(
                 L"CspResetType",
                 &VariableGuid,
                 0,
                 &DataSize,
                 &CurrentResetType);
    if (!EFI_ERROR(Status))
    {
        if (CurrentResetType >= (UINT32)ResetType)
            return EFI_SUCCESS;
    }

    CurrentResetType = ResetType;

    Status = pRS->SetVariable(
                 L"CspResetType",
                 &VariableGuid,
                 EFI_VARIABLE_BOOTSERVICE_ACCESS, //AptioV Override: Remove NVRAM RT Attribute
                 DataSize,
                 &CurrentResetType);

    return Status;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBLib_ResetSystem
//
// Description: This function is the reset call interface function published
//              by the reset PPI
//
// Input:       ResetType   Type of reset to be generated
//
// Output:      SYSTEM RESET
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SBLib_ResetSystem(IN SB_RESET_TYPE ResetType)
{
    UINT8 Data;
    UINT32 dData = 0;                   // (P091109D)

    Data = IoRead8(RESET_PORT);
    Data &= CLEAR_RESET_BITS;

    switch (ResetType)
    {
    case HardReset:
        Data |= COLD_RESET;
        ClearGPIOResetSelect();         // (P101309E)
        break;

    case SoftReset:
        break;
                                        // (P112509A)>
    case ShutDown:
        IoWrite8(PM_BASE_ADDRESS + ICH_IOREG_PM1_CNT + 1, 0x3C);
    	// We should never get this far
    	EFI_DEADLOOP()
        break;                          // <(P112509A)

    case FullReset:
        Data |= (BIT03 + COLD_RESET);
        break;

    case GlobalReset:
        Data |= (BIT03 + COLD_RESET);
        MmPci32(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_PMIR) |= BIT20;
        ClearGPIOResetSelect();         // (P101309E)

                                        // (P091109D)>
#if defined SB_EC_SUPPORT && SB_EC_SUPPORT
        // GPIO30
        dData = IoRead32(GPIO_BASE_ADDRESS + ICH_GPIO_GP_RST_SEL1);
        dData |= BIT30;
        IoWrite32(GPIO_BASE_ADDRESS + ICH_GPIO_GP_RST_SEL1, dData);

        dData = IoRead32(GPIO_BASE_ADDRESS + ICH_GPIO_GP_LVL);
        dData &= ~BIT30;
        IoWrite32(GPIO_BASE_ADDRESS + ICH_GPIO_GP_LVL, dData);
#endif                                  // <(P091109D)

        break;

    default:
        break;
    }

    IoWrite8(RESET_PORT, Data);

    Data |= RST_CPU;
    IoWrite8(RESET_PORT, Data);

    // We should never get this far
    EFI_DEADLOOP()
}

#endif

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SBLib_CmosRead
//
// Description: Read the RTC value at the given Index.
//
// Input:  Index  RTC Index
//
// Output:  RTC Value read from the provided Index
//
// Notes: Here is the control flow of this function:
//        1. Enable Alternate Access mode so port 0x70 can be read.  
//        2. Read port 0x70 (RTC Index Register) to get bit 7.
//           Bit 7 is the NMI bit-it should not be changed.  Until
//           Alt Access is enabled, reads from 0x70 return 0xFF.
//        3. Disable Alt Access.
//        4. Set Index with the NMI bit setting.
//        5. Output 0x70 with the Index.
//        6. Read 0x71 for Data.
//        7. Return the Data.

//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8 SBLib_CmosRead(IN UINT8 Index)
{
  UINT8         NMI; 
  UINT8         volatile Value;
  BOOLEAN       IntState = GetInterruptState();
//----------------------
  CPULib_DisableInterrupt();

  Index &= ~RTC_NMI_MASK; 

  // Enable Alternate Access
  SET_MEM32_RCRB(R_RCBA_GCS, 0x10);
  // Read register back to ensure it's set before IO access
  while ( (READ_MEM32_RCRB(R_RCBA_GCS) & 0x10) == 0 );

  NMI = READ_IO8(RTC_INDEX_REG);

  // Disable Alternate access mode.
  RESET_MEM32_RCRB(R_RCBA_GCS, 0x10);
  // Read register back to ensure it's clear before IO access.
  while ( (READ_MEM32_RCRB(R_RCBA_GCS) & 0x10) );

  NMI &= RTC_NMI_MASK;                  // Read bit 7 (NMI setting).

  WRITE_IO8(RTC_INDEX_REG, Index | NMI);
  Value = READ_IO8(RTC_DATA_REG);       //Read register.
    
  if(IntState)CPULib_EnableInterrupt();

  return (UINT8)Value;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SBLib_CmosWrite
//
// Description: Write the RTC value at the given Index.
//
// Input:  Index RTC Index
//         Data RTC Index
//
// Output: None
//
// Notes:  Here is the control flow of this function:
//         1. Enable Alternate Access mode so port 0x70 can be read.
//         2. Read port 0x70 (RTC Index Register) to get bit 7.
//            Bit 7 is the NMI bit-it should not be changed. Until
//            Alt Access is enabled, reads from 0x70 return 0xFF.
//         3. Disable Alt Access.
//         4. Set Index with the NMI bit setting.
//         5. Output 0x70 with the Index.
//         6. Write the data to 0x71.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SBLib_CmosWrite(IN UINT8 Index, IN UINT8 Value)
{
  UINT8         NMI;
  BOOLEAN       IntState = GetInterruptState();
//--------------------
  CPULib_DisableInterrupt();
  
  Index &= ~RTC_NMI_MASK; 
  // Enable Alternate Access
  SET_MEM32_RCRB(R_RCBA_GCS, 0x10);
  // Read register back to ensure it's set before IO access
  while ( (READ_MEM32_RCRB(R_RCBA_GCS) & 0x10) == 0 );


  NMI = READ_IO8(RTC_INDEX_REG);

  // Disable Alternate access mode.
  RESET_MEM32_RCRB(R_RCBA_GCS, 0x10);
  // Read register back to ensure it's clear before IO access.
  while ( (READ_MEM32_RCRB(R_RCBA_GCS) & 0x10) );

  NMI &= RTC_NMI_MASK;                  // Read bit 7 (NMI setting).

  WRITE_IO8(RTC_INDEX_REG, Index | NMI);

  WRITE_IO8(RTC_DATA_REG, Value);       //Write Register.

  if(IntState)CPULib_EnableInterrupt();
}

//#if SB_STALL_PPI_SUPPORT

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CountTime
//
// Description: This function delays for the number of micro seconds passed in
//
// Input:       DelayTime Number of microseconds(us) to delay
//              BaseAddr  The I/O base address of the ACPI registers
//
// Output:      Return Status based on errors that occurred while waiting for
//              time to expire.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
CountTime(
    IN  UINTN   DelayTime,
    IN  UINT16  BaseAddr                  // only needs to be 16 bit for I/O address
)
{
// The following code is to generate delay for specified amount of micro seconds using ACPI timer.
    UINTN       TicksNeeded;
    UINT32      TimerValue, NewTimerValue;
    UINTN       OverFlow;
    UINTN       TheRest, EndValue;

    // Set up timer to point to the ACPI Timer register
    BaseAddr += ICH_IOREG_PM1_TMR;        // *** PORTING NEEDED

    // There are 3.58 ticks per us, so we have to convert the number of us passed
    // in to the number of ticks that need to pass before the timer has expired
    // Convert us to Ticks, don't loose significant figures or as few as possible
    // do integer math in ticks/tens of ns and then divide by 100 to get ticks
    // per us
    OverFlow = 0;

    TheRest = TicksNeeded = (DelayTime * 358) / 100;

    // 32 bits corresponds to approz 71 mins  no delay should be that long
    // otherwise
    // Get the number of times the counter will have to overflow to delay as long
    // as needed
    if (NUM_BITS_IN_ACPI_TIMER < MAX_ACPI_TIMER_BITS)
    {
        OverFlow = TicksNeeded / (1 << NUM_BITS_IN_ACPI_TIMER);
        TheRest = TicksNeeded % (1 << NUM_BITS_IN_ACPI_TIMER);
    }

    // read ACPI Timer
    TimerValue = IoRead32(BaseAddr);

    // need to adjust the values based off of the start time
    EndValue = TheRest + TimerValue;

    // check for overflow on addition.  possibly a problem
    if (EndValue < TimerValue)
    {
        OverFlow++;
    }

    // here make sure that EndValue is less than the max value
    // of the counter
    else if (NUM_BITS_IN_ACPI_TIMER < MAX_ACPI_TIMER_BITS)
    {
        OverFlow += EndValue / (1 << NUM_BITS_IN_ACPI_TIMER);
        EndValue = EndValue % (1 << NUM_BITS_IN_ACPI_TIMER);
    }

    // let the timer wrap around as many times as calculated
    while (OverFlow)
    {
        // read timer amd look to see if the new value read is less than
        // the current timer value.  if this happens the timer overflowed
        NewTimerValue = IoRead32(BaseAddr);

        if (NewTimerValue < TimerValue)
            OverFlow--;

        TimerValue = NewTimerValue;
    }


    // now wait for the correct number of ticks that need to occur after
    //  all the needed overflows
    while (EndValue > TimerValue)
    {
        NewTimerValue = IoRead32(BaseAddr);

        // check to see if the timer overflowed.  if it did then
        // the time has elapsed. Because EndValue should be greater than TimerValue
        if (NewTimerValue < TimerValue)
            break;

        TimerValue = NewTimerValue;
    }

    return EFI_SUCCESS;
}


//#endif

                                        // (P042709A)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   ReadWriteCmosBank2
//
// Description: This function is used to access addresses in the CMOS
//              register range (0x80-0xff), for PEI and DXE boot phases.
//
// Input:       **PeiServices       - PEI Services table pointer
//                                    (NULL in DXE phase)
//              AccessType          - ReadType or WriteType to specify the
//                                    type of access
//              CmosRegister        - The CMOS register to access
//              *CmosParameterValue - Pointer to the data variable to be
//                                    accessed
//
// Output:      EFI_STATUS (return value)
//                  EFI_SUCCESS - The access operation was successfull.
//                  Otherwise   - A valid EFI error code is returned.
//
// Modified:    None
//
// Referrals:   IoRead8, IoWrite8
//
// Notes:       This function is used when a translation from logical address
//              to index port value is required.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

#if CMOS_MANAGER_SUPPORT
EFI_STATUS ReadWriteCmosBank2 (
    IN CONST EFI_PEI_SERVICES         **PeiServices,  // NULL in DXE phase
    IN CMOS_ACCESS_TYPE         AccessType,
    IN UINT16                   CmosRegister,
    IN OUT UINT8                *CmosParameterValue )
{
    if ((CmosRegister < 0x80) || (CmosRegister > 0xff))
        return EFI_INVALID_PARAMETER;

    // Some chipsets require tranlation from the logical CMOS address to a
    // physical CMOS index port value. However, other chipsets do not require
    // a translation and the index/data port can be directly used for
    // accessing the second bank.

    IoWrite8(CMOS_IO_EXT_INDEX, (UINT8)CmosRegister);

    if (AccessType == ReadType) {
        *CmosParameterValue = IoRead8( CMOS_IO_EXT_DATA );
    } else {
        IoWrite8( CMOS_IO_EXT_DATA, *CmosParameterValue );
    }

    return EFI_SUCCESS;
}

#endif  // #if CMOS_MANAGER_SUPPORT
                                        // <(P042709A)


//----------------------------------------------------------------------------
// Start of Smbus Related Porting Hooks
//----------------------------------------------------------------------------
#if SB_SMBUS_PPI_SUPPORT

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SmbusWriteCommand
//
// Description: This support function writes a specified byte data into the
//              SMBus Controller I/O register
//
// Input:  Register Register offset that need to be updated Data.
//         Data to be written.
//
// Output:  EFI_SUCCESS always
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SmbusWriteCommand(
    IN UINT16  Register,
    IN UINT16  Data
)
{
    IoWrite8(Register, (UINT8)Data);
    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SmbusReadCommand
//
// Description: This support function reads a byte data from the
//              SMBus Controller I/O register
//
// Input:  Register Register offset that need to be read
//
// Output:  Byte read from the SMBus I/O register
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8 SmbusReadCommand(IN UINTN  Register)
{
    return IoRead8((UINT16)Register);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: WriteSMB
//
// Description: This support function writes to various SMBus Controller
//              I/O register based on the Control Flag value
//
// Input:  SmbIoAddr I/O base address of the SMBus Controller
//         DataByte Data to be written
//         CtrlFlag A bit flag register that specifies the write operation
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS WriteSMB(
    IN UINTN SmbIoAddr,
    IN UINTN DataByte,
    IN UINT8 CtrlFlag
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN  Register;   // May be Command register, Data register, Slave addr. register or blk count register
    UINT16  AuxcData = 00;

    // check for a address of 0 for the Smbus I/O address
    if (!SmbIoAddr) return EFI_UNSUPPORTED;

    if (CtrlFlag & BIT_SLAVE_ADDR)
    {
        Register = SmbIoAddr + XMIT_SLAVE_ADDRESS_REGISTER;
        if (CtrlFlag & BIT_READ)
            DataByte |= SMBUS_IO_READ_BIT;
    }
    else if (CtrlFlag & BIT_COMMAND)
        Register = SmbIoAddr + HOST_COMMAND_REGISTER;
    else if ((CtrlFlag & (BIT_COUNT + BIT_DATA)) == (BIT_COUNT + BIT_DATA)) // Check Block Command
        Register = SmbIoAddr + HOST_BLOCK_DATA_BYTE_REGISTER; //
    else if (CtrlFlag & BIT_DATA)
        Register  = SmbIoAddr + HOST_DATA_0_REGISTER; // Data Register
    else if (CtrlFlag & BIT_CONTROL)
    {
        // Do packet error checking
        if (CtrlFlag & BIT_PEC)
        {
            AuxcData = IoRead8((UINT16)SmbIoAddr + AUXILIARY_CONTROL_REGISTER);

            AuxcData |= SMBUS_B_AAC;
            Status = SmbusWriteCommand((UINT16)SmbIoAddr + AUXILIARY_CONTROL_REGISTER, AuxcData);
        }

        DataByte  |= HST_CNT_START;
        Register = SmbIoAddr + HOST_CONTROL_REGISTER; //

        // Read the host control register to reset the 32byte buffer index
        //-SmbusReadCommand(Register);

    }
    else if (CtrlFlag & BIT_COUNT)
        Register = SmbIoAddr + HOST_DATA_0_REGISTER; // Block count register
    else
        return EFI_UNSUPPORTED;

    Status = SmbusWriteCommand((UINT16)Register, (UINT8)DataByte);

    if (CtrlFlag & BIT_WORD)
    {
        // Send the high byte
        Status = SmbusWriteCommand((UINT16)++Register, (UINT8)(DataByte >> 8));
    }

    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ReadSMB
//
// Description: This support function reads from various SMBus Controller
//              I/O register based on the Control Flag value
//
// Input:  SmbIoAddr I/O base address of the SMBus Controller
//         *DataByte Buffer for the read data
//         CtrlFlag A bit flag register that specifies the read operation
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS ReadSMB(
    IN UINTN SmbIoAddr,
    OUT UINT8 *bData,
    IN UINT8 CtrlFlag
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN  Register;   // May be Command register, Data register, Slave addr. register or blk count register

    if (!SmbIoAddr) return EFI_UNSUPPORTED;

    if (CtrlFlag & BIT_SLAVE_ADDR)
        Register = SmbIoAddr + XMIT_SLAVE_ADDRESS_REGISTER;
    else if (CtrlFlag & BIT_COMMAND)
        Register = SmbIoAddr + HOST_COMMAND_REGISTER;
    else if ((CtrlFlag & (BIT_COUNT + BIT_DATA)) == (BIT_COUNT + BIT_DATA)) // Check Block Command
        Register = SmbIoAddr + HOST_BLOCK_DATA_BYTE_REGISTER; //
    else if (CtrlFlag & BIT_DATA)
        Register  = SmbIoAddr + HOST_DATA_0_REGISTER; // Data Register
    else if (CtrlFlag & BIT_CONTROL)
        Register = SmbIoAddr + HOST_CONTROL_REGISTER; //
    else if (CtrlFlag & BIT_COUNT)
        Register = SmbIoAddr + HOST_DATA_0_REGISTER; // Block count register
    else
        return EFI_UNSUPPORTED;

    *bData = (UINT8)SmbusReadCommand(Register);

    if (CtrlFlag & BIT_WORD)
    {
        // Send the high byte
//####  *bData = *bData << 8;
//####  *bData |= SmbusReadCommand(++Register);
        *(bData + 1) = *bData;
        *bData = SmbusReadCommand(++Register);
    }

    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: CheckReadOperationComplete
//
// Description: This support function verifies whether the SMBus read operation
//              is completed successfully
//
// Input:  SmBusAddr I/O base address of the SMBus Controller
//
// Output:  EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckReadOperationComplete(IN UINTN SmBusAddr)
{
    EFI_STATUS  Status = EFI_SUCCESS;

    return Status;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ClearHostStatus
//
// Description: This support function clears the Smbus controller
//              status register
//
// Input:  SmbIoAddr I/O base address of the SMBus Controller
//
// Output:  None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID ClearHostStatus(IN UINTN SmbIoAddr)
{
    IoWrite8((UINT16)(SmbIoAddr + HOST_STATUS_REGISTER),
             (HST_STS_FAILED + HST_STS_BUS_ERR + HST_STS_DEV_ERR + HST_STS_INTR));
    return;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ClearHostByteDoneStatus
//
// Description: This support function clears the Smbus controller
//              Byte Done bit in the status register
//
// Input:  SmbIoAddr I/O base address of the SMBus Controller
//
// Output:  None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ClearHostByteDoneStatus(IN UINTN SmbIoAddr)
{
    IoWrite8((UINT16)(SmbIoAddr + HOST_STATUS_REGISTER),
             (HST_STS_BYTE_DONE));
    return;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: WaitForOperationComplete
//
// Description: This support function waits for the operation complete
//
// Input:  SmBusIoAddr I/O base address of the SMBus Controller
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS WaitForOperationComplete(IN UINTN SmBusIoAddr)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT16  Count;
    UINT8  bSts;

    for (Count = 0; Count < 1000; Count++)
    {
        bSts = IoRead8((UINT16)(SmBusIoAddr + HOST_STATUS_REGISTER));
        if (!(bSts & (HST_STS_FAILED + HST_STS_BUS_ERR + HST_STS_DEV_ERR + HST_STS_INTR)))
        {
            CountTime(50, PM_BASE_ADDRESS);
            continue;
        }

        if (bSts & (HST_STS_FAILED + HST_STS_BUS_ERR + HST_STS_DEV_ERR))
            Status = EFI_DEVICE_ERROR;
        else
            Status = EFI_SUCCESS;
        IoWrite8((UINT16)(SmBusIoAddr + HOST_STATUS_REGISTER), bSts);
        break;
    }

    return Status;
}

#endif
//----------------------------------------------------------------------------
// End of Smbus Related Porting Hooks
//----------------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckLFDPlatform
//
// Description: Check for Lynnfield platform.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckLFDPlatform(VOID)
{
    UINT16 Buffer16;

    Buffer16 = MmPci16(0, 0, 0, 0x02) & 0xFFF0;

                                        // (P112309C)>
    // For Jasper Forest CPU
    // Treat JF like LFD in most cases
    if (Buffer16 == 0x3700)
        return EFI_SUCCESS;

    // For *field CPU
    if (Buffer16 == 0xD130)
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
                                        // <(P112309C)
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckHVDPlatform
//
// Description: Check for Havendale platform.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckHVDPlatform(VOID)
{
    UINT16 Buffer16;

    Buffer16 = MmPci16(0, 0, 0, 0x02) & 0x00F0;

    // For *dale CPU
    if ((Buffer16 == 0x0040) || (Buffer16 == 0x0060))
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}

                                        // (P112309C)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckJFPlatform
//
// Description: Check for Jasper Forest platform.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckJFPlatform(VOID)
{
    UINT16 Buffer16;

    Buffer16 = MmPci16(0, 0, 0, 0x02) & 0xFFF0;

    // For Jasper Forest CPU
    if (Buffer16 == 0x3700)
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckNotJFPlatform
//
// Description: Check for anything but Jasper Forest platform.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckNotJFPlatform(VOID)
{
    UINT16 Buffer16;

    Buffer16 = MmPci16(0, 0, 0, 0x02) & 0xFFF0;

    // For Jasper Forest CPU
    if (Buffer16 == 0x3700)
        return EFI_UNSUPPORTED;

    return EFI_SUCCESS;
}
                                        // <(P112309C)

                                        // (P042009C)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckAuburndalePlatform
//
// Description: Check for Auburndale/Arrandale platform.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckAuburndalePlatform(VOID)
{
    UINT16 wDID;
    UINT16 wSKU;

    wSKU = MmPci16(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_DID) & BIT00;
    wDID = MmPci16(0, 0, 0, 0x02) & 0x00F0;  // 0x004x:Havendale/Auburndale/Arrandale

    if ((wDID == 0x0040) && wSKU)
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}                                       // <(P042009C)

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   PchAxPatch
//
// Description: Patch for PCH Ax stepping.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS PchAxPatch(VOID)             // (P020309C)>
{
    if (MmPci8(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_RID) <= PCH_A1)
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   PchB0Patch
//
// Description: Patch for PCH B0 stepping.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS PchB0Patch(VOID)
{
    if (MmPci8(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_RID) == PCH_B0)
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}                                       // <(P020309C)

                                        // (P021309C)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckPchMobileSku
//
// Description: Check PCH Mobile SKU.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckPchMobileSku(VOID)
{
    if (MmPci16(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_DID) & BIT00)
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   CheckPchDesktopSku
//
// Description: Check PCH Desktop SKU.
//
// Input: None
//
// Output: EFI_UNSUPPORTED
//         EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CheckPchDesktopSku(VOID)
{
    if (!(MmPci16(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_DID) & BIT00))
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}                                       // <(P021309C)

                                        // (P052109A)(P042709B)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   EnableUsbr
//
// Description: EnableUSBr.
//
// Input: None
//
// Output: None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS EnableUsbr(VOID)
{
  UINT8 Buffer8;

  Buffer8 = Mmio8(SB_RCBA, ICH_RCRB_MISCCTL);
  Buffer8 |= (BIT00 + BIT01);
  Mmio8(SB_RCBA, ICH_RCRB_MISCCTL) = Buffer8;

  return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   DisableUsbr
//
// Description: Disable USBr.
//
// Input: None
//
// Output: None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS DisableUsbr(VOID)
{
  UINT8 Buffer8;

  Buffer8 = Mmio8(SB_RCBA, ICH_RCRB_MISCCTL);  // 0x3590
  Buffer8 &= ~(BIT00 + BIT01);
  Mmio8(SB_RCBA, ICH_RCRB_MISCCTL) = Buffer8;  // 0x3590

  return EFI_SUCCESS;
}                                       // <(P052109A)(P042709B)

                                        // (P071709A)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbFindCapPtr
//
// Description: This function searches the PCI address space for the PCI
//              device specified for a particular capability ID and returns
//              the offset in the PCI address space if one found
//
// Input:    UINT64  PciAddress,
//           UINT8   CapId
//
// Output:  Capability ID Address if one found
//          Otherwise returns 0
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT32 SbFindCapPtr(
 IN UINT64  PciAddress,
 IN UINT8   CapId
)
{
  UINT8   Value;
  UINT32  Address = (UINT32)PciAddress;

  Address = (Address & 0xffffff00) | 6; //PCI Status Register.
  Value = Mmio8(Address, 0);

  if (Value == 0xff) return 0;          // No device.
  if (!(Value & (1 << 4))) return 0;    // Check if capabilities list.

  *(UINT8*)&Address = 0x34;             // Register to First capabilities pointer
                                        // if 0, then capabilities
  for(;;)
  {
    Value = Mmio8(Address, 0);
    if (Value == 0) return 0;

    *(UINT8*)&Address = Value;          // PciAddress = ptr to CapID
    Value = Mmio8(Address, 0);          // New cap ptr.

    //If capablity ID, return register that points to it.
    if (Value == CapId) return Address;

    ++Address;                          // Equals to next capability pointer.
  }
}                                       // <(P071709A)



                                        // [EIP21454]>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   IsUsbRmhMode
//
// Description: Check Rmh Mode.
//
// Input: None
//
// Output: TRUE = RMH Mode
//         FALSE = non-RMH mode
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN IsUsbRmhMode(VOID)
{
  if(!Mmio32(SB_RCBA, 0x3598) & 0x01)
        return TRUE;

  return FALSE;
}                                       // <[EIP21454]

                                        // (P051509B)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   IntelEhciWorkaround
//
// Description:
//
// Input: None
//
// Output: None
//
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS IntelEhciWorkaround(VOID)
{
  UINT8 Buffer8;
  UINT8 TempDev;
  UINT8 TempFunc = 0;
  UINT8 bRevId = MmPci8(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_RID);

  if(!IsUsbRmhMode())
    TempFunc = 7;

  if(bRevId == PCH_B0)                  // (P051909B)
  {
    for(TempDev = USB_DEV_26; TempDev <= USB_DEV_29; TempDev += 3)  // [EIP28189]
    {
      Buffer8 = MmPci8(USB_BUS, TempDev, TempFunc, 0x82);
      Buffer8 |= BIT01;
      MmPci8(USB_BUS, TempDev, TempFunc, 0x82) = Buffer8;

      Buffer8 = MmPci8(USB_BUS, TempDev, TempFunc, 0x82);
      Buffer8 &= ~(BIT01);
      MmPci8(USB_BUS, TempDev, TempFunc, 0x82) = Buffer8;
    }
  }

  return EFI_SUCCESS;
}                                       // <(P051509B)


                                        // (P102909A), (P050709A)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   EnableSpiLock
//
// Description: Enable SPI Lockdown
//
// Input: None
//
// Output: EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS EnableSpiLock(VOID)
{
  UINT8   PciCmd;
  UINT16  Buffer16;
  UINT32  Buffer32;
  UINT32  BARBBackUp;
  UINT32  D25F0Addr = (UINT32)(UINTN)SB_PCIE_CFG_ADDRESS(LAN_BUS, LAN_DEV, LAN_FUNC, ICH_REG_LAN_VID);

  // SPIBAR + 04h[15]
  Buffer16 = Mmio16(SB_RCBA + SPI_BASE_ADDRESS, ICH_REG_SPI_HSFS);
  Buffer16 |= (BIT15);                  // Enable FLOCKDN
  Mmio16(SB_RCBA + SPI_BASE_ADDRESS, ICH_REG_SPI_HSFS) = Buffer16;

  if(Mmio16(D25F0Addr, ICH_REG_LAN_VID) == 0x8086)
  {
    PciCmd = Mmio8(D25F0Addr, ICH_REG_LAN_PCICMD);
    BARBBackUp = Mmio32(D25F0Addr, ICH_REG_LAN_MBARB);

    if (BARBBackUp == 0)
    {
      Buffer32 = SB_TEMP_MMIO_BASE;
      Mmio32(D25F0Addr, ICH_REG_LAN_MBARB) = Buffer32;
      Mmio8(D25F0Addr, ICH_REG_LAN_PCICMD) |= BIT01;
    }
    else
    {
      Buffer32 = BARBBackUp & 0xFFFFF000;
    }

    Buffer16 = Mmio16(Buffer32, ICH_REG_SPI_HSFS);
    Buffer16 |= BIT15;                  // Enable FLOCKDN
    Mmio16(Buffer32, ICH_REG_SPI_HSFS) = Buffer16;

    Mmio32(D25F0Addr, ICH_REG_LAN_MBARB) = BARBBackUp;
    Mmio8(D25F0Addr, ICH_REG_LAN_PCICMD) = PciCmd;
  }
  return EFI_SUCCESS;
}                                       // <(P102909A), (P050709A)



                                        // (P042809D)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   DMIInterfacePatch
//
// Description:
//
// Input: None
//
// Output: EFI_STATUS
//
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS DMIInterfacePatch(VOID)
{
                                        // (P050509C)>
  UINT8 PlatformSku = MmPci8(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_DID) & BIT0;

  MmPci8(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_GEN_PMCON_1) &= ~(BIT03 | BIT02);

  // Desktop
  if (PlatformSku == 0)
    MmPci8(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_GEN_PMCON_1) |= BIT03;

  // Mobile
  if (PlatformSku == 1)
    MmPci8(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_GEN_PMCON_1) |= BIT02;
                                        // <(P050509C)

  return EFI_UNSUPPORTED;
}
                                        // <(P042809D)

                                        // (P042809F)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SATAPatch
//
// Description:
//
// Input: None
//
// Output: None
//
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SATAPatch(VOID)
{
  UINT32 Data32;

  Data32 = Mmio32(SB_RCBA, ICH_RCRB_GC);
  Data32 &= 0x0F060000;                 // 0x341C[27:24] & [18:17]

  if ((Data32 == 0x0F060000) ||
      (Data32 == 0x03060000))           // (P112309A)
    MmPci32(SATA0_BUS, SATA0_DEV, SATA0_FUNC, 0x98) |= BIT20;

  return EFI_UNSUPPORTED;
}
                                        // <(P042809F)


                                        // [EIP21061]>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   SbLib_SetLpcDeviceDecoding
//
// Description: This function sets LPC Bridge Device Decoding
//
// Input:       *LpcPciIo - Pointer to LPC PCI IO Protocol(NULL in PEI)
//              Base      - I/O base address, if Base is 0 means disabled the
//                          decode of the device
//              DevUid    - The device Unique ID
//              Type      - Device Type, please refer to AMISIO.h
//
// Output:      EFI_STATUS
//                  EFI_SUCCESS - Set successfully.
//                  EFI_UNSUPPORTED - There is not proper Device Decoding
//                                    register for the device UID.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS SbLib_SetLpcDeviceDecoding (
    IN EFI_PCI_IO_PROTOCOL      *LpcPciIo,
    IN UINT16                   Base,
    IN UINT8                    DevUid,
    IN SIO_DEV_TYPE             Type )
{
    EFI_STATUS              Status = EFI_UNSUPPORTED;

    // Porting Required
    UINT16                  ComRange[] = { 0x3f8, 0x2f8, 0x220, 0x228,\
                                           0x238, 0x2e8, 0x338, 0x3e8, 0};
    UINT16                  LptRange[] = { 0x378, 0x278, 0x3bc, 0};
    UINT16                  FpcRange[] = { 0x3f0, 0x370, 0};
    UINT16                  IoRangeMask16 = 0xffff;
    UINT16                  IoRangeSet16 = 0;
    UINT16                  IoEnMask16 = 0xffff;
    UINT16                  IoEnSet16 = 0;
    UINT8                   i;

    switch (Type) {
        // FDC Address Range
        case (dsFDC) :
            if (Base == 0) IoEnMask16 &= ~BIT03;
            else {
                for (i = 0; (FpcRange[i] != 0) && (FpcRange[i] != Base); i++);
                if (FpcRange[i]) {
                    IoEnSet16 |= BIT03;
                    IoRangeMask16 &= ~BIT12;
                    IoRangeSet16 |= (i << 12);
                }
                else return EFI_UNSUPPORTED;
            }
            break;

        // LPT Address Range
        case (dsLPT) :
            if (Base == 0) IoEnMask16 &= ~BIT02;
            else {
                for (i = 0; (LptRange[i] != 0) && (LptRange[i] != Base); i++);
                if (LptRange[i]) {
                    IoEnSet16 |= BIT02;
                    IoRangeMask16 &= ~(BIT09 | BIT08);
                    IoRangeSet16 |= (i << 8);
                } else return EFI_UNSUPPORTED;
            }
            break;

        // ComA Address Range
        case (dsUART) :
            if (Base == 0) {
                if (DevUid) IoEnMask16 &= ~BIT01;
                else IoEnMask16 &= ~BIT00;
            } else {
                for (i = 0; (ComRange[i] != 0) && (ComRange[i] != Base); i++);
                if (ComRange[i]) {
                    if (DevUid) {
                        IoEnSet16 |= BIT01;
                        IoRangeMask16 &= ~(BIT06 | BIT05 | BIT04);
                        IoRangeSet16 |= (i << 4);
                    } else {
                        IoEnSet16 |= BIT00;
                        IoRangeMask16 &= ~(BIT02 | BIT01 | BIT00);
                        IoRangeSet16 |= i;
                    }
                } else return EFI_UNSUPPORTED;
            }
            break;

        // KBC Address Enable
        case (dsPS2K) :
        case (dsPS2M) :
        case (dsPS2CK) :
        case (dsPS2CM) :
            if (Base == 0) IoEnMask16 &= ~BIT10;
            else IoEnSet16 |= BIT10;
            break;

        // Game Port Address Enable
        case (dsGAME) :
            if (Base == 0) IoEnMask16 &= ~(BIT09 | BIT08);
            else {
                if (Base == 0x200) {
                    IoEnSet16 |= BIT08;
                } else {
                    if (Base == 0x208) IoEnSet16 |= BIT09;
                    else return EFI_UNSUPPORTED;
                }
            }
            break;

        // LPC CFG Address Enable
        case (0xff) :
            if (Base == 0x2e) IoEnSet16 |= BIT12;
            else {
                if (Base == 0x4e) IoEnSet16 |= BIT13;
                else {
                    if (Base == 0x62) IoEnSet16 |= BIT11;
                    else {
                    if (Base) SbLib_SetLpcGenericDecoding( LpcPciIo, \
                                                           Base , \
                                                           4, \
                                                           TRUE );
                        else return EFI_UNSUPPORTED;
                    }
                }
            }
            break;

        default :
            return EFI_UNSUPPORTED;
    }
      MmPci16(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_IODEC) &= IoRangeMask16;  // 0x80
      MmPci16(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_IODEC) |= IoRangeSet16;

      MmPci16(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_EN) &= IoEnMask16;  // 0x82
      MmPci16(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_EN) |= IoEnSet16;
    // Porting End

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   SbLib_SetLpcGenericDecoding
//
// Description: This function set LPC Bridge Generic Decoding
//
// Input:       *LpcPciIo - Pointer to LPC PCI IO Protocol
//              Base      - I/O base address
//              Length    - I/O Length
//              Enabled   - Enable/Disable the generic decode range register
//
// Output:      EFI_STATUS
//                  EFI_SUCCESS - Set successfully.
//                  EFI_UNSUPPORTED - This function is not implemented or the
//                                    Length more than the maximum supported
//                                    size of generic range decoding.
//                  EFI_INVALID_PARAMETER - the Input parameter is invalid.
//                  EFI_OUT_OF_RESOURCES - There is not available Generic
//                                         Decoding Register.
//                  EFI_NOT_FOUND - the generic decode range will be disabled
//                                  is not found.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS SbLib_SetLpcGenericDecoding (
    IN EFI_PCI_IO_PROTOCOL      *LpcPciIo,
    IN UINT16                   Base,
    IN UINT16                   Length,
    IN BOOLEAN                  Enable )
{
    // Porting Required
    UINT32                  IoGenDecode32;
    UINT16                  IoGenDecIndex;
    UINT16                  Buffer16;
    UINT8                   Bsf8 = 0;
    UINT8                   Bsr8 = 0;

    if (Length > 0x100) return EFI_UNSUPPORTED;

    if (Length == 0) return EFI_INVALID_PARAMETER;

    if (Length < 4) Length = 4;

    // Read I/O Generic Decodes Register.
    for (IoGenDecIndex = 0; IoGenDecIndex < 4; IoGenDecIndex++) {
        IoGenDecode32 = MmPci32(LPC_BUS, LPC_DEVICE, LPC_FUNC, (ICH_REG_LPC_GEN1_DEC + IoGenDecIndex * 4));
        if (Enable) {
            if ((IoGenDecode32 & 1) == 0) break;
        } else {
            if (((IoGenDecode32 & 0xfffc) == Base) && (IoGenDecode32 & 1)) {
                IoGenDecode32 = 0; // Disable & clear the base/mask fields
                break;
            }
        }
    }

    if (Enable) {
        if (IoGenDecIndex == 4) return EFI_OUT_OF_RESOURCES;

        Buffer16 = Length;
        while ((Buffer16 % 2) == 0) {
            Buffer16 /= 2;
            Bsf8++;
        }

        while (Length) {
            Length >>= 1;
            Bsr8++;
        }

        if (Bsf8 == (Bsr8 - 1)) Bsr8--;

        Length = (1 << Bsr8) - 1 ;

        Base &= (~Length);

        IoGenDecode32 = Base | (UINT32)((Length >> 2) << 18) | 1;

    } else {
        if (IoGenDecIndex == 4) return EFI_NOT_FOUND;
    }

    MmPci32(LPC_BUS, LPC_DEVICE, LPC_FUNC, (ICH_REG_LPC_GEN1_DEC + IoGenDecIndex * 4)) = IoGenDecode32; // 0x84
    // Porting End

    return EFI_SUCCESS;
}                                       // <[EIP21061]


//----------------------------------------------------------------------------
// Start of SPI Flash Related Porting Hooks
//----------------------------------------------------------------------------
#if SPIFlash_SUPPORT

#include "FlashPart.h"

UINT8 *SbRcba = (UINT8*)(UINTN)SB_RCBA;
UINT8 SpiRuntime = 0;


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SpiWaitForDeviceNotBusy
//
// Description: This function stalls until all pending SPI transactions are
//  complete.  Prepares SPI for the next transaction.
//
// Input:  None
//
// Output:  EFI_SUCCESS
//     EFI_DEVICE_ERROR

//----------------------------------------------------------------------------
//<AMI_PHDR_END>
static EFI_STATUS SpiWaitForDeviceNotBusy(VOID)
{
    volatile  UINT16   TempSpiStatus;
    volatile UINT8 *a = (volatile UINT8*)(SbRcba + SPI_BASE_ADDRESS + R_RCRB_SPI_SPIS);

//Wait until "SPI Access Grant" bit is 1
//####    do {
//####     TempSpiStatus = *a;
//####    } while ((TempSpiStatus & B_RCRB_SPI_SPIS_SAG) != B_RCRB_SPI_SPIS_SAG);

    //Wait until "Cycle in Progress" bit is 0
    do
    {
        TempSpiStatus = *a;
    }
    while ((TempSpiStatus & B_RCRB_SPI_SPIS_SCIP) == B_RCRB_SPI_SPIS_SCIP);

    //Clear the "Cycle Done Status" and "Flash Cycle Error" bits
    *a |= (B_RCRB_SPI_SPIS_SCDS | B_RCRB_SPI_SPIS_FCERR);

    // during debug "Flash Cycle Error" bit could "stick" at 1 check for this condition
    TempSpiStatus = *a;
    if ((TempSpiStatus & B_RCRB_SPI_SPIS_FCERR) == B_RCRB_SPI_SPIS_FCERR)
        return EFI_DEVICE_ERROR;

    return EFI_SUCCESS;
}



//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SpiWaitForCycleComplete
//
// Description: This function stalls until the current SPI transactions is
//   complete.  It also prepares the SPI for the next transaction.
//
// Input:  None
//
// Output:  EFI_SUCCESS
//     EFI_DEVICE_ERROR
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
static EFI_STATUS SpiWaitForCycleComplete(VOID)
{
    volatile  UINT16   TempSpiStatus;
    volatile UINT8 *a = (volatile UINT8*)(SbRcba + SPI_BASE_ADDRESS + R_RCRB_SPI_SPIS);

    // Wait until "Cycle Done Status" bit is 1
    do
    {
        TempSpiStatus = *a;
    }
    while ((TempSpiStatus & B_RCRB_SPI_SPIS_SCDS) != B_RCRB_SPI_SPIS_SCDS);

    // check for errors
    TempSpiStatus = *a;
    if ((TempSpiStatus & B_RCRB_SPI_SPIS_FCERR) == B_RCRB_SPI_SPIS_FCERR)
        return EFI_DEVICE_ERROR;

    // Clear the "Cycle Done Status" and "Flash Cycle Error" bits
    *a |= (B_RCRB_SPI_SPIS_SCDS | B_RCRB_SPI_SPIS_FCERR);
    return EFI_SUCCESS;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SpiTransfer
//
// Description: This function Starts an SPI transaction
//
// Input:  Address   - Address to operate on
//     Length    - Nuber of bytes to transfer, SPI Control, bit 13:8
//     OpcodePtr - Which pre-selected command, SPI Control, bit 6:4
//     PrefixPtr - SPI Control, bit 3
//     Atomic    - Is this an atomic command, SPI Control, bit 2
//     Data      - Is there data to send, SPI Control, bit 14
//
// Output:  EFI_SUCCESS
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SpiTransfer(IN volatile UINT8 *Address, IN UINT8 OpcodePtr,
                       IN UINT8 PrefixPtr, IN BOOLEAN Atomic,
                       IN FLASH_WRITE_DATA Data)
{
    static UINT32 SPIFlashSize = 0;
    static UINT32 SPIFlags = 0;

    volatile UINT8 *a = (volatile UINT8*)(SbRcba + SPI_BASE_ADDRESS);
    volatile UINT8 *OffsetAddr;
    volatile UINT16 v;
    UINT32 Idx;

    if (SPIFlags == 0)
    {
        UINT32 Buffer32;

        *(volatile UINT32 *)(a + ICH_REG_SPI_FDOC) = 0;
        Buffer32 = *(volatile UINT32 *)(a + ICH_REG_SPI_FDOD);
        if (Buffer32 == 0x0FF0A55A)
        {
            *(volatile UINT32 *)(a + ICH_REG_SPI_FDOC) = (BIT13 + BIT02);
            do
            {
                Buffer32 = *(volatile UINT32 *)(a + ICH_REG_SPI_FDOD);
            }
            while (Buffer32 == 0x0FF0A55A);
            SPIFlashSize = ((Buffer32 & 0xFFFF) << 12);
        }
        SPIFlags = 1;
    }

    // Set Access Request bit to let the other devices know there is a device that has a transaction
    *((volatile UINT16*)(a + R_RCRB_SPI_SPIC)) |= B_RCRB_SPI_SPIC_AR;

    SpiWaitForDeviceNotBusy();

    // During Runtime, if this function is called, the Address passed in is within the
    //  virtual address space.  The SPI interface needs it to be within the physical
    //  address space.  This if statement finds the offset within the virtual space that
    //  it is trying to write to and uses that to create the offset within the physical
    //  address space
    if (SpiRuntime == 1)
    {
        // FlashDeviceBase points to the beginning of the flash part within the virtual
        //  address space.  This gets the offset into the flash part
        OffsetAddr = Address - FlashDeviceBase;

        // This finds the physical address that we are trying to write to
        Address = OffsetAddr + (0xFFFFFFFF - FLASH_SIZE) + 1;
    }
    else
    {
        OffsetAddr = Address - FlashDeviceBase;
    }

    // This puts the physical address in the Address register for SPI accesses
    if (SPIFlashSize == 0)
        *((volatile UINT32*)(a + R_RCRB_SPI_SPIA)) = (UINT32)Address;
    else
        *((volatile UINT32*)(a + R_RCRB_SPI_SPIA)) = (UINT32)OffsetAddr + SPIFlashSize;

    // Programs the number of bytes to program
    // lets the ICH know whether or not there is data associated with this command
    v = *((volatile UINT16*)(a + R_RCRB_SPI_SPIC));
    v &= (~B_RCRB_SPI_SPIC_DATA);
    v &= (~B_RCRB_SPI_SPIC_DBC);

    if (Data.Data)
    {
        // If there is Data fill in the data registers
        if (Data.Values != 0)
        {
            for (Idx = 0; Idx <= Data.Length; Idx++)
            {
                *((volatile UINT8*)(a + R_RCRB_SPI_SPID0 + Idx)) = Data.Values[Idx];
            }
        }
        // fill in the Data transfer needed for this command
        v |= B_RCRB_SPI_SPIC_DATA;
        // enter the numbers of bytes to transfer.  This is 0 based so 0=1
        v |= (Data.Length << 8);
    }
    *((volatile UINT16*)(a + R_RCRB_SPI_SPIC)) = v;



    // Programs the Opcode pointer to use for this command
    v = *((volatile UINT16*)(a + R_RCRB_SPI_SPIC));
    v &= (~B_RCRB_SPI_SPIC_COP);
    v |= (OpcodePtr << 4);
    *((volatile UINT16*)(a + R_RCRB_SPI_SPIC)) = v;

    // Programs the prefix opcode pointer
    v = *((volatile UINT16*)(a + R_RCRB_SPI_SPIC));
    v &= (~B_RCRB_SPI_SPIC_SPOP);
    if (PrefixPtr) v |= B_RCRB_SPI_SPIC_SPOP;
    *((volatile UINT16*)(a + R_RCRB_SPI_SPIC)) = v;

    // programs the atomic cycle sequence bit
    v = *((volatile UINT16*)(a + R_RCRB_SPI_SPIC));
    v &= (~B_RCRB_SPI_SPIC_ACS);
    if (Atomic) v |= B_RCRB_SPI_SPIC_ACS;
    *((volatile UINT16*)(a + R_RCRB_SPI_SPIC)) = v;


    // Initiates the command that was just program
    *((volatile UINT16*)(a + R_RCRB_SPI_SPIC)) |= B_RCRB_SPI_SPIC_SCGO;

    SpiWaitForCycleComplete();

    // clear the Access request bit
    *((volatile UINT16*)(a + R_RCRB_SPI_SPIC)) &= (~B_RCRB_SPI_SPIC_AR);

    return EFI_SUCCESS;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: IdentifySpi
//
// Description: This function is a generic identify command that programs
//  the registers and returns a pointer to the Data registers so that the
//  ID can be read properly.
//
//
// Input:  FlashInfo - This structure contains the necessary data to program
//                     the SPI registers so that the device can be identified
//         DataPtr - A pointer that will be filled by this function so that the
//                   ID can be read from the SPI data registers
//         Data - a structure that carries the information that will be
//                programmed into the SPI chipset registers
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID IdentifySpi(
 IN FLASH_INFO FlashInfo,
 IN UINT32 *DataPtr,
 IN FLASH_WRITE_DATA Data)
{
    UINT8 *FakeBaseAddress = (UINT8*)0xFFFFFFF0;// must specify any address >= BBAR value;
    volatile UINT8 *a = (volatile UINT8*)(SbRcba + SPI_BASE_ADDRESS);

    //Program first DWORD of opcode commands
    *((volatile UINT32*)(a + ICH_REG_SPI_OPMENU + 0)) = (UINT32)
            ((FlashInfo.Write.Opcode         << (SPI_OPCODE_WRITE_INDEX * 8))      // Write Byte
             | (FlashInfo.Read.Opcode        << (SPI_OPCODE_READ_INDEX * 8))       // Read Data
             | (FlashInfo.Erase.Opcode       << (SPI_OPCODE_ERASE_INDEX * 8))      // Erase 64k Sector
             | (FlashInfo.ReadStatus.Opcode  << (SPI_OPCODE_READ_S_INDEX * 8)));   // Read Device Status Reg

    //Program second DWORD of Opcode commands
    *((volatile UINT32*)(a + ICH_REG_SPI_OPMENU + 4)) = (UINT32)
            ((FlashInfo.ReadId.Opcode     << ((SPI_OPCODE_READ_ID_INDEX - 4) * 8))    // Read device ID
             | (FlashInfo.WriteStatus.Opcode << ((SPI_OPCODE_WRITE_S_INDEX - 4) * 8))); // Write Status Register

    //Program opcode types
    *((volatile UINT16*)(a + ICH_REG_SPI_OPTYPE)) = (UINT16)
            (FlashInfo.Write.OpcodeType             << (SPI_OPCODE_WRITE_INDEX * 2)       // address, write
             | FlashInfo.Read.OpcodeType            << (SPI_OPCODE_READ_INDEX * 2)        // address, read
             | FlashInfo.Erase.OpcodeType           << (SPI_OPCODE_ERASE_INDEX * 2)       // address, write
             | FlashInfo.ReadStatus.OpcodeType      << (SPI_OPCODE_READ_S_INDEX * 2)      // no adress, read
             | FlashInfo.ReadId.OpcodeType          << (SPI_OPCODE_READ_ID_INDEX * 2)     // address, read
             | FlashInfo.WriteStatus.OpcodeType     << (SPI_OPCODE_WRITE_S_INDEX * 2));   // no addres, write

    //set up the prefix opcodes for commands
    *((volatile UINT16*)(a + ICH_REG_SPI_PREOP)) = (UINT16)((FlashInfo.WriteStatusEnable.Opcode << 8) | (FlashInfo.WriteEnable.Opcode));

    // Now try to read ID
    SpiTransfer(FakeBaseAddress, SPI_OPCODE_READ_ID_INDEX, 0, 1, Data);

    // Return the data read from the flash part indicating the ID information
    *DataPtr =  *((volatile UINT32 *)(a + R_RCRB_SPI_SPID0));

}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: IsA16Inverted
//
// Description: Returns the state of A16 address line
//
//
// Input:  None
//
// Output:  TRUE A16 inverted
//    FALSE A16 is not inverted
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN IsA16Inverted(VOID)
{
    volatile UINT8  *a = (volatile UINT8*)(SbRcba + R_RCBA_BUC);
//-----------------
    if ((*a) & B_RCRB_BUC_TS) return TRUE;
    else return FALSE;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SpiChipsetReadAddress
//
// Description: Returns the address based on A16 inversion state
//              A16 is used for TopSwap checking
//
//
// Input:  UINT8 *BaseAddress  Address to read from
//
// Output:  UINT8*     Address to read from adjusted
//          if A16 inversion is in effect
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8* SpiChipsetReadAddress(IN UINT8 *BaseAddress)
{
    // This is for TopSwap capabilities
    // SPI - ICH7 does A16 inversion for reads only, not for writes
    if (IsA16Inverted())
        return (UINT8*)((UINTN)BaseAddress ^ BIT16);
    else
        return BaseAddress;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SpiChipsetVirtualFixup
//
// Description: This function will be invoked by the SPI flash component to
//              virtualize any chipset based addresses
//
// Input:  *pRS    Pointer to runtime services
//
// Output:  None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SpiChipsetVirtualFixup(IN EFI_RUNTIME_SERVICES  *pRS)
{
    // convert Memory Mapped Io base
    pRS->ConvertPointer(0, (VOID **)&SbRcba);

    SpiRuntime = 1;

    return;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   BiosLockEnableSmiFlashHook
//
// Description: The procedure clears BIOSWR_STS after Enable Flash SWSMI,
//              for prevent that AFU write operation fail when BIOS Lock
//              is enabled.
//
// Input:       UINT8        SwSmiNum
//              UINT64       Buffer
//
// Output:      UINT64       Buffer
//
// Returns:     NONE
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
VOID 
BiosLockEnableSmiFlashHook (
    IN UINT8            SwSmiNum,
    IN OUT UINT64       Buffer
)
{
    if ((SwSmiNum != 0x20) || \
        ((READ_PCI8_SB(ICH_REG_LPC_BIOS_CNTL) & BIT01) == 0))
      return;

    // Clear BIOSWR_STS
    WRITE_IO16_TCO(ICH_IOREG_TCO1_STS, BIT08);

    // Clear TCO_STS
    WRITE_IO16_PM(ICH_IOREG_SMI_STS, BIT13);
}   


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ChipsetFlashDeviceWriteEnable
//
// Description: This function is invoked to do any chipset specific operations
//              that are necessary when enabling the Flash Part for writing
//
// Input:  None
//
// Output:  None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ChipsetFlashDeviceWriteEnable(VOID)
{
    UINT32 Data32;
#if DISABLE_PWR_BUTTON == 0                     // [ EIP47645 ]
    UINT16 Buffer16;                    // (P022610A)
#endif  // #if DISABLE_PWR_BUTTON

// Code goes here that chipset specific for the flash device enabling    
    //3.7BIOS Region SMM Protection Enabling
    //If the following steps are implemented:
    //Set the SMM_BWP bit (B0:D31:F0 Offset DCh [5]) = 1
    //Follow the 1st recommendation in Section 3.6 the BIOS Region can only be updated by following the steps bellow:
    //Once all threads enter SMM
    //Read memory location FED30880h OR with 00000001h, place the result in EAX, and write data to lower 32 bits of MSR 1FEh (sample code available)
    //Set BIOSWE bit (B0:D31:F0 Offset DCh [0]) = 1
    //Modify BIOS Region
    //Clear BIOSWE bit (B0:D31:F0 Offset DCh [0]) = 0
    //Read memory location FED30880h AND with FFFFFFFEh, place the result in EAX, and write data to lower 32 bits of MSR 1FEh (sample code available)    
    
    RW_PCI8_SB(ICH_REG_LPC_BIOS_CNTL, BIT00, BIT05);    
    //
    // Check if BLE and SMM_BWP bits are both set
    //
    if ((READ_PCI8_SB(ICH_REG_LPC_BIOS_CNTL) & 0x22) == 0x22) {
  																	
      if ( !((ReadMsr (MSR_PLATFORM_INFO) & B_MSR_PLATFORM_INFO_PFAT_AVAIL) && 
             (ReadMsr (MSR_PLAT_FRMW_PROT_CTRL) & B_MSR_PLAT_FRMW_PROT_CTRL_EN)) ) {  
            Data32 = MmioRead32 ((UINTN) (0xFED30880)) | (UINT32) (BIT0);
            WriteMsr(0x1FE, Data32);
      }  																						
    }    


#if DISABLE_PWR_BUTTON == 0                     // [ EIP47645 ]
                                        // (P022610A)>
    // Save PM1_EN (PMBASE+02h) in "gPm1EnBuffer".
    gPm1EnBuffer = IoRead16(PM_BASE_ADDRESS + ICH_IOREG_PM1_EN);
    // Disable PWRBTN_EN (PMBASE+02h[8])
    Buffer16 = IoRead16(PM_BASE_ADDRESS + ICH_IOREG_PM1_EN);
    Buffer16 &= ~BIT08;
    IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_PM1_EN, Buffer16);
    // Clear PWRBTN_STS (PMBASE+00h[8])
    IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_PM1_STS, BIT08);
                                        // <(P022610A)
#endif  // #if DISABLE_PWR_BUTTON
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ChipsetFlashDeviceWriteDisable
//
// Description: This function is invoked to do any chipset specific operations
//              that are necessary when disabling the Flash Part for writing
//
// Input:  None
//
// Output:  None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ChipsetFlashDeviceWriteDisable(VOID)
{
	    UINT32 Data32;	
	    
	    //3.7BIOS Region SMM Protection Enabling
	    //If the following steps are implemented:
	    //Set the SMM_BWP bit (B0:D31:F0 Offset DCh [5]) = 1
	    //Follow the 1st recommendation in Section 3.6 the BIOS Region can only be updated by following the steps bellow:
	    //Once all threads enter SMM
	    //Read memory location FED30880h OR with 00000001h, place the result in EAX, and write data to lower 32 bits of MSR 1FEh (sample code available)
	    //Set BIOSWE bit (B0:D31:F0 Offset DCh [0]) = 1
	    //Modify BIOS Region
	    //Clear BIOSWE bit (B0:D31:F0 Offset DCh [0]) = 0
	    //Read memory location FED30880h AND with FFFFFFFEh, place the result in EAX, and write data to lower 32 bits of MSR 1FEh (sample code available)    
	    
	  RESET_PCI8_SB(ICH_REG_LPC_BIOS_CNTL, BIT00);
	  
	  //
	  // Check if BLE and SMM_BWP bits are both set
	  //
	  if ((READ_PCI8_SB(ICH_REG_LPC_BIOS_CNTL) & 0x22) == 0x22) {
	    //
	    // Read memory location FED30880h AND with FFFFFFFEh, place the result in EAX,
	    // and write data to lower 32 bits of MSR 1FEh (sample code available)
	    //
																							
	    if ( !((ReadMsr (MSR_PLATFORM_INFO) & B_MSR_PLATFORM_INFO_PFAT_AVAIL) && 
	           (ReadMsr (MSR_PLAT_FRMW_PROT_CTRL) & B_MSR_PLAT_FRMW_PROT_CTRL_EN)) ) {  
	          Data32 = MmioRead32 ((UINTN) (0xFED30880)) & (UINT32) (~BIT0);
	          WriteMsr (0x1FE, Data32);
	    }
																						
	  }	  

#if DISABLE_PWR_BUTTON == 0                     // [ EIP47645 ]
                                        // (P022610A)>
    // Clear PWRBTN_STS (PMBASE+00h[8])
    IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_PM1_STS, BIT08);
    // Restore PM1_EN (PMBASE+02h)
    IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_PM1_EN, gPm1EnBuffer);
                                        // <(P022610A)
#endif  // #if DISABLE_PWR_BUTTON
}

#endif
//----------------------------------------------------------------------------
// END of SPI Flash related porting hooks
//----------------------------------------------------------------------------


#if _CSP_LIB_ == 2
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ProgramMMIOTable
//
// Description:
//
// Input:  IN UINTN BaseAddress,
//         IN AMI_INIT_MMIO_TABLE_STRUCT *pTable,
//         IN UINT16 Siz
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS ProgramMMIOTable(
 IN UINTN BaseAddress,
 IN AMI_INIT_MMIO_TABLE_STRUCT *pTable,
 IN UINT16 Size)
{
    EFI_STATUS Status;
    UINT16 i;
    UINT32 Buffer;

    for (i = 0; i < Size; i++)
    {
        if (pTable[i].TaskFunctionPtr != 0)
        {
            Status = pTable[i].TaskFunctionPtr();
            if (Status != EFI_SUCCESS)
                continue;
        }

        Buffer = Mmio32(BaseAddress, pTable[i].Register);
        Buffer &= pTable[i].ANDMask;
        Buffer |= pTable[i].ORMask;
        Mmio32(BaseAddress, pTable[i].Register) = Buffer;
    }

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ProgramMMIOTable_S3
//
// Description:
//
// Input:  IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
//         IN UINTN BaseAddress,
//         IN AMI_INIT_MMIO_TABLE_STRUCT *pTable,
//         IN UINT16 Siz
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS ProgramMMIOTable_S3(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 IN UINTN BaseAddress,
 IN AMI_INIT_MMIO_TABLE_STRUCT *pTable,
 IN UINT16 Size)
{
    EFI_STATUS Status;
    UINT16 i;
    UINT32 Buffer;

    for (i = 0; i < Size; i++)
    {
        if (pTable[i].TaskFunctionPtr != 0)
        {
            Status = pTable[i].TaskFunctionPtr();
            if (Status != EFI_SUCCESS)
                continue;
        }

        Buffer = Mmio32(BaseAddress, pTable[i].Register);
        Buffer &= pTable[i].ANDMask;
        Buffer |= pTable[i].ORMask;
        Mmio32(BaseAddress, pTable[i].Register) = Buffer;
        BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO(BootScriptSave, EfiBootScriptWidthUint32,
                                            (BaseAddress + pTable[i].Register), &pTable[i].ORMask, &pTable[i].ANDMask);
    }

    return EFI_SUCCESS;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: MemRW8_S3
//
// Description:
//
// Input:   IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
//          IN UINTN Address,
//          IN UINT8 Value,
//          IN UINT8 Mask
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MemRW8_S3(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 IN UINTN Address,
 IN UINT8 Value,
 IN UINT8 Mask
)
{
    UINT8 MaskData = ~Mask;

    Mmio8(Address, 0) = (Mmio8(Address, 0) & (~Mask)) | (Value);
    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO(BootScriptSave, EfiBootScriptWidthUint8,
                                        Address, &Value, &MaskData);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: MemRW16_S3
//
// Description:
//
// Input:   IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
//          IN UINTN Address,
//          IN UINT16 Value,
//          IN UINT16 Mask
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MemRW16_S3(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 IN UINTN Address,
 IN UINT16 Value,
 IN UINT16 Mask
)
{
    UINT16 MaskData = ~Mask;

    Mmio16(Address, 0) = (Mmio16(Address, 0) & (~Mask)) | (Value);
    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO(BootScriptSave, EfiBootScriptWidthUint16,
                                        Address, &Value, &MaskData);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: MemRW32_S3
//
// Description:
//
// Input:   IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
//          IN UINTN Address,
//          IN UINT32 Value,
//          IN UINT32 Mask
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MemRW32_S3(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 IN UINTN Address,
 IN UINT32 Value,
 IN UINT32 Mask
)
{
    UINT32 MaskData = ~Mask;

    Mmio32(Address, 0) = (Mmio32(Address, 0) & (~Mask)) | (Value);
    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO(BootScriptSave, EfiBootScriptWidthUint32,
                                        Address, &Value, &MaskData);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: MemWrite8_S3
//
// Description:
//
// Input:     IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
//            IN UINTN Address,
//            IN UINT8 Value
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MemWrite8_S3(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 IN UINTN Address,
 IN UINT8 Value
)
{
    Mmio8(Address, 0) = Value;
    BOOT_SCRIPT_S3_MEM_WRITE_MACRO(BootScriptSave, EfiBootScriptWidthUint8,
                                   Address, 1, &Value);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: MemWrite16_S3
//
// Description:
//
// Input:     IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
//            IN UINTN Address,
//            IN UINT16 Value
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MemWrite16_S3(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 IN UINTN Address,
 IN UINT16 Value
)
{
    Mmio16(Address, 0) = Value;
    BOOT_SCRIPT_S3_MEM_WRITE_MACRO(BootScriptSave, EfiBootScriptWidthUint16,
                                   Address, 1, &Value);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: MemWrite32_S3
//
// Description:
//
// Input:     IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
//            IN UINTN Address,
//            IN UINT32 Value
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MemWrite32_S3(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 IN UINTN Address,
 IN UINT32 Value
)
{
    Mmio32(Address, 0) = Value;
    BOOT_SCRIPT_S3_MEM_WRITE_MACRO(BootScriptSave, EfiBootScriptWidthUint32,
                                   Address, 1, &Value);
}
//----------------------------------------------------------------------------
#endif


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ProgramGPIOTable
//
// Description:
//
// Input:     IN UINTN BaseAddress,
//            IN AMI_INIT_GPIO_TABLE_STRUCT *pTable,
//            IN UINT16 Value
//
// Output:  EFI_STATUS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS ProgramGPIOTable(
 IN UINTN BaseAddress,
 IN AMI_INIT_GPIO_TABLE_STRUCT *pTable,
 IN UINT16 Size,
OUT  UINT32 *USE1_SEL,
OUT  UINT32 *USE2_SEL,
OUT  UINT32 *USE3_SEL,
OUT  UINT32 *IO1_SEL,
OUT  UINT32 *IO2_SEL,
OUT  UINT32 *IO3_SEL,
OUT  UINT32 *LVL1_SEL,
OUT  UINT32 *LVL2_SEL,
OUT  UINT32 *LVL3_SEL,
OUT  UINT32 *INV1_SEL
)
{
    UINT16 i;
    /*
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
    */

    AMI_GPIO_STRUCT Buffer;

    if (pTable[0].GPIO != 0xFFFF)
    {
        *USE1_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_USE_SEL);
        *IO1_SEL  = IoRead32((UINT16)BaseAddress + ICH_GPIO_IO_SEL);
        *LVL1_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_GP_LVL);
        *INV1_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_GPI_INV);
        *USE2_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_USE_SEL2);
        *IO2_SEL  = IoRead32((UINT16)BaseAddress + ICH_GPIO_IO_SEL2);
        *LVL2_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_GP_LVL2);
        *USE3_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_USE_SEL3);
        *IO3_SEL  = IoRead32((UINT16)BaseAddress + ICH_GPIO_IO_SEL3);
        *LVL3_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_GP_LVL3);
                                   // (P091109D.1)>
        gRST1_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_GP_RST_SEL1);
        gRST2_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_GP_RST_SEL2);
        gRST3_SEL = IoRead32((UINT16)BaseAddress + ICH_GPIO_GP_RST_SEL3);
                                   // <(P091109D.1)


        for (i = 0; i < (Size - 1); i++)
        {
            Buffer = pTable[i].CONFIG;
            if (pTable[i].CONFIG.OFFSET < 32)
            {
                *USE1_SEL = (*USE1_SEL & ~(1 << Buffer.OFFSET)) | (Buffer.USE << Buffer.OFFSET);
                *IO1_SEL  = (*IO1_SEL  & ~(1 << Buffer.OFFSET)) | (Buffer.IO  << Buffer.OFFSET);
                *LVL1_SEL = (*LVL1_SEL & ~(1 << Buffer.OFFSET)) | (Buffer.LVL << Buffer.OFFSET);
                *INV1_SEL = (*INV1_SEL & ~(1 << Buffer.OFFSET)) | (Buffer.INV << Buffer.OFFSET);
                gRST1_SEL = (gRST1_SEL & ~(1 << Buffer.OFFSET)) | (Buffer.RST << Buffer.OFFSET); // (P091109D.1)
            }
            else if ((pTable[i].CONFIG.OFFSET >= 32) && (pTable[i].CONFIG.OFFSET < 64))
            {
                *USE2_SEL = (*USE2_SEL & ~(1 << (Buffer.OFFSET - 32))) | (Buffer.USE << (Buffer.OFFSET - 32));
                *IO2_SEL  = (*IO2_SEL  & ~(1 << (Buffer.OFFSET - 32))) | (Buffer.IO  << (Buffer.OFFSET - 32));
                *LVL2_SEL = (*LVL2_SEL & ~(1 << (Buffer.OFFSET - 32))) | (Buffer.LVL << (Buffer.OFFSET - 32));
                gRST2_SEL = (gRST2_SEL & ~(1 << (Buffer.OFFSET - 32))) | (Buffer.RST << (Buffer.OFFSET - 32)); // (P091109D.1)
            }
            else
            {
                *USE3_SEL = (*USE3_SEL & ~(1 << (Buffer.OFFSET - 64))) | (Buffer.USE << (Buffer.OFFSET - 64));
                *IO3_SEL  = (*IO3_SEL  & ~(1 << (Buffer.OFFSET - 64))) | (Buffer.IO  << (Buffer.OFFSET - 64));
                *LVL3_SEL = (*LVL3_SEL & ~(1 << (Buffer.OFFSET - 64))) | (Buffer.LVL << (Buffer.OFFSET - 64));
                gRST3_SEL = (gRST3_SEL & ~(1 << (Buffer.OFFSET - 64))) | (Buffer.RST << (Buffer.OFFSET - 64)); // (P091109D.1)
            }
        }
        
        

        IoWrite32((UINT16)BaseAddress + ICH_GPIO_USE_SEL, *USE1_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_IO_SEL, *IO1_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_GP_LVL, *LVL1_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_GPI_INV, *INV1_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_USE_SEL2, *USE2_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_IO_SEL2, *IO2_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_GP_LVL2, *LVL2_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_USE_SEL3, *USE3_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_IO_SEL3, *IO3_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_GP_LVL3, *LVL3_SEL);
                                        // (P091109D)>
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_GP_RST_SEL1, gRST1_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_GP_RST_SEL2, gRST2_SEL);
        IoWrite32((UINT16)BaseAddress + ICH_GPIO_GP_RST_SEL3, gRST3_SEL);
                                        // <(P091109D.1)
    }

    return EFI_SUCCESS;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ReadSPIDescriptor
//
// Description: Read SPI Descriptor.
//
// Input: FDSS Flash Descriptor Section Select.
//        FDSI Flash Descriptor Section Index.
//
// Output:
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS ReadSPIDescriptor(
    IN UINT8 FDSS,
    IN UINT8 FDSI,
    OUT UINT32 *FDOD
)
{
    UINT32 Buffer32;

    Mmio32((SB_RCBA + SPI_BASE_ADDRESS), ICH_REG_SPI_FDOC) = 0;
    Buffer32 = Mmio32((SB_RCBA + SPI_BASE_ADDRESS), ICH_REG_SPI_FDOD);

    if (Buffer32 != 0x0ff0a55a)
        return EFI_NOT_FOUND;

    Buffer32 = ((UINT32)FDSS << 12) | ((UINT32)FDSI << 2);
    Mmio32((SB_RCBA + SPI_BASE_ADDRESS), ICH_REG_SPI_FDOC) = Buffer32;
    *FDOD = Mmio32((SB_RCBA + SPI_BASE_ADDRESS), ICH_REG_SPI_FDOD);

    return EFI_SUCCESS;
}
//----------------------------------------------------------------------------
                                        // (P052209B)>
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbDelayUs
//
// Description:
//
// Input:   IN UINT16 wCount
//
// Output:
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SbDelayUs (
  IN UINT16 wCount
)
{
  UINT32 Buffer32;

  Buffer32 = wCount * 3579;
  wCount = Buffer32 % 1000;
  Buffer32 /= 1000;
  if (wCount) Buffer32++;
  IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_PM1_STS, 1); // 0x00
  Buffer32 += (IoRead32(PM_BASE_ADDRESS + ICH_IOREG_PM1_TMR) & 0xffffff); // 0x08
  while (Buffer32 & 0xff000000) {
    while (!(IoRead16(PM_BASE_ADDRESS + ICH_IOREG_PM1_STS) & 1));
      IoWrite16(PM_BASE_ADDRESS + ICH_IOREG_PM1_STS, 1); // 0x00
    Buffer32 -= 0x1000000;
  }
  while ((IoRead32(PM_BASE_ADDRESS + ICH_IOREG_PM1_TMR) & 0xffffff) < Buffer32); // 0x08
}
                                        // <(P052209B)

//----------------------------------------------------------------------------
// Standard PCI Access Routines, No Porting Required.
//----------------------------------------------------------------------------

#if _SBCSP_LIB_ == 1

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   ReadPci8
//
// Description: This function reads an 8bits data from the specific PCI 
//              register. 
//
// Input:       Bus - PCI Bus number.
//              Dev - PCI Device number.
//              Fun - PCI Function number.
//              Reg - PCI Register number.
//
// Output:      UINT8
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8 ReadPci8 (
    IN UINT8        Bus, 
    IN UINT8        Dev,
    IN UINT8        Fun, 
    IN UINT16       Reg )
{
    if (Reg >= 0x100) {
        return MMIO_READ8(SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg));
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        return IoRead8(0xcfc | (UINT8)(Reg & 3));
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   ReadPci16
//
// Description: This function reads a 16bits data from the specific PCI 
//              register. 
//
// Input:       Bus - PCI Bus number.
//              Dev - PCI Device number.
//              Fun - PCI Function number.
//              Reg - PCI Register number.
//
// Output:      UINT16
//----------------------------------------------------------------------------

//<AMI_PHDR_END>

UINT16 ReadPci16 (
    IN UINT8        Bus, 
    IN UINT8        Dev,
    IN UINT8        Fun, 
    IN UINT16       Reg )
{
    if (Reg >= 0x100) {
        return MMIO_READ16(SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg));
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        return IoRead16(0xcfc | (UINT8)(Reg & 2));
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   ReadPci32
//
// Description: This function reads a 32bits data from the specific PCI 
//              register. 
//
// Input:       Bus - PCI Bus number.
//              Dev - PCI Device number.
//              Fun - PCI Function number.
//              Reg - PCI Register number.
//
// Output:      UINT32
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32 ReadPci32 (
    IN UINT8        Bus, 
    IN UINT8        Dev,
    IN UINT8        Fun, 
    IN UINT16       Reg )
{
    if (Reg >= 0x100) {
        return MMIO_READ32(SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg));
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        return IoRead32(0xcfc);
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WritePci8
//
// Description: This function writes an 8bits data to the specific PCI
//              register.
//
// Input:       Bus     - PCI Bus number.
//              Dev     - PCI Device number.
//              Fun     - PCI Function number.
//              Reg     - PCI Register number.
//              Value8  - An 8 Bits data will be written to the specific 
//                        PCI register.
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WritePci8 (
    IN UINT8        Bus, 
    IN UINT8        Dev,
    IN UINT8        Fun, 
    IN UINT16       Reg,
    IN UINT8        Value8 )
{
    if (Reg >= 0x100) {
        WriteMem8((UINT64)SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), Value8);
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        IoWrite8(0xcfc | (UINT8)(Reg & 3), Value8);
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WritePci16
//
// Description: This function writes a 16bits data to the specific PCI
//              register.
//
// Input:       Bus     - PCI Bus number.
//              Dev     - PCI Device number.
//              Fun     - PCI Function number.
//              Reg     - PCI Register number.
//              Value16 - A 16 Bits data will be written to the specific 
//                        PCI register.
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WritePci16 (
    IN UINT8        Bus, 
    IN UINT8        Dev,
    IN UINT8        Fun, 
    IN UINT16       Reg,
    IN UINT16       Value16 )
{
    if (Reg >= 0x100) {
        WriteMem16((UINT64)SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), Value16);
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        IoWrite16(0xcfc | (UINT8)(Reg & 2), Value16);
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WritePci32
//
// Description: This function writes a 32bits data to the specific PCI
//              register.
//
// Input:       Bus     - PCI Bus number.
//              Dev     - PCI Device number.
//              Fun     - PCI Function number.
//              Reg     - PCI Register number.
//              Value32 - A 32 Bits data will be written to the specific 
//                        PCI register.
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WritePci32 (
    IN UINT8        Bus, 
    IN UINT8        Dev,
    IN UINT8        Fun, 
    IN UINT16       Reg,
    IN UINT32       Value32 )
{
    if (Reg >= 0x100) {
        WriteMem32((UINT64)SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), Value32);
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        IoWrite32(0xcfc, Value32);
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteMem8
//
// Description: This function writes an 8bits data to the specific memory
//              (or MMIO) register.
//
// Input:       Address - An 64Bits Memory (or MMIO) address
//              Value8  - An 8 Bits data will be written to the specific 
//                        PCI register.
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteMem8 (
    IN UINT64       Address,
    IN UINT8        Value8 )
{
    MMIO_WRITE8(Address, Value8);
    Value8 = MMIO_READ8(Address);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteMem16
//
// Description: This function writes a 16bits data to the specific memory
//              (or MMIO) register.
//
// Input:       Address - A 64Bits Memory (or MMIO) address
//              Value16 - A 16 Bits data will be written to the specific 
//                        PCI register.
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteMem16 (
    IN UINT64       Address,
    IN UINT16       Value16 )
{
    MMIO_WRITE16(Address, Value16);
    Value16 = MMIO_READ16(Address);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteMem32
//
// Description: This function writes a 32bits data to the specific memory
//              (or MMIO) register.
//
// Input:       Address - A 64Bits Memory (or MMIO) address
//              Value32 - A 32 Bits data will be written to the specific 
//                        PCI register.
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteMem32 (
    IN UINT64       Address,
    IN UINT32       Value32 )
{
    MMIO_WRITE32(Address, Value32);
    Value32 = MMIO_READ32(Address);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwMem8
//
// Description: This function reads an 8bits data from a specific memory
//              (or MMIO) address, applies masks, and writes it back.
//
// Input:       Address - A 64Bits Memory (or MMIO) address
//              SetBit8 - Mask of bits to set (1 = Set)
//              ResetBit8 - Mask of bits to clear  (1 = clear)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwMem8 (
    IN UINT64       Address,
    IN UINT8        SetBit8,
    IN UINT8        ResetBit8 )
{
    UINT8       Buffer8 = MMIO_READ8(Address) & ~ResetBit8 | SetBit8;

    WriteMem8(Address, Buffer8);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwMem32
//
// Description: This function reads a 32bits data from a specific memory
//              (or MMIO) address, applies masks, and writes it back.
//
// Input:       Address - A 64Bits Memory (or MMIO) address
//              SetBit32 - Mask of bits to set (1 = Set)
//              ResetBit32 - Mask of bits to clear  (1 = clear)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwMem32 (
    IN UINT64       Address,
    IN UINT32       SetBit32,
    IN UINT32       ResetBit32 )
{
    UINT32      Buffer32 = MMIO_READ32(Address) & ~ResetBit32 | SetBit32;

    WriteMem32(Address, Buffer32);
}

#endif //#if _SBCSP_LIB_ == 1


#if defined(LOAD_DEFAULTS_ON_CMOS_CLEAR) && (LOAD_DEFAULTS_ON_CMOS_CLEAR == 1)
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: IsPchRtcReset
//
// Description:
//  This function determines if the system should boot with the default configuration.
//  This function is called by NVRAM PEI code.
//  If boot with default configuration is detected, default values for NVRAM variables are used.
//
// Input:
//  **PeiServices - pointer to a pointer to the PEI Services Table.
//  *ReadVariablePpi - pointer to EFI_PEI_READ_ONLY_VARIABLE2_PPI PPI. The pointer can be used to
//                     read and enumerate existing NVRAM variables.
//
// Output:
//  TRUE - Boot with default configuration is detected
//  FALSE - Boot with default configuration is not detected
//
// Notes:
//  This routine is called very early, prior to SBPEI and NBPEI
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN IsPchRtcReset(
    IN EFI_PEI_SERVICES **PeiServices,
    IN VOID *ReadVariablePpi)
{
   if(MmPciRead32(LPC_BUS, LPC_DEVICE, LPC_FUNC, ICH_REG_LPC_PMCON_3) & BIT02 ) //Check RTC Power Status is set to see whether  RTCRST# is asserted. Clearing CMOS can be done by using a jumper on RTCRST
      return TRUE;                                                                                                                                                           //RTC_PWR_STS  bit will remain set until the software clears it by writing a 0 back to this bit position. The PCH code will take care of  this.

   
    return FALSE;
}
#endif

#if KBC_SUPPORT
//<AMI_PHDR_START>
//**********************************************************************
//
// Procedure: IsCtrlHomePressed
//
// Description: 
//  This function detects if the Ctrl + Home key press has been made.
//  Is function is only included if KBC_SUPPORT is enabled.  It enables
//  the keyboard checks for the Ctrl + Home key press and then disables
//  the keyboard
//
// Input:
//  IN EFI_PEI_SERVICES **PeiServices - pointer to the PeiServices Table
//
// Output:
//  BOOLEAN 
//  - - TRUE - Ctrl + Home was pressed
//  - - FALSE - Ctrl + Home not detected
//
// Notes:
//  
//**********************************************************************
//<AMI_PHDR_END>
BOOLEAN IsCtrlHomePressed (
    EFI_PEI_SERVICES **PeiServices
)
{
  UINT8               Index;
  UINT8               KeyValue = 0;
  UINT8               KeyScan = 0;
  UINT8               LoopCount = CTRL_HOME_CHECK_LOOP;
  UINT8               KbcSts = 0;
  UINT8               KbcSts1 = 0;
  UINT8               KbcSts2 = 0;
  BOOLEAN             KbcOBF = TRUE;
  BOOLEAN             KbcIBF = TRUE;
  BOOLEAN             KbcIBF1 = TRUE;
  UINT8               KbcRes = 0;

  //
  // Enable KBD
  //
  KbcSts = IoRead8 (0x64);
  //
  // Check KeyBoard Presence
  //
  if (KbcSts != 0xFF) {
    //
    // Save the Sys Bit for later use
    //
    KbcRes = KbcSts & 0x04;
    IoWrite8 (0x64, 0x60);  
    //
    // Wait for Input Buffer Empty
    //
    while(KbcIBF) {
      KbcIBF = FALSE;
      KbcSts1 = IoRead8 (0x64);
      if ((KbcSts1 & 0x02) == 0x02) {
        KbcIBF = TRUE;
      }
    }
    //
    // Keyboard enable, Mouse disable
    //
    IoWrite8 (0x60, KbcRes | 0x61);          
    //  
    // Wait for Input Buffer Empty
    //
    while(KbcIBF1) {
      KbcIBF1 = FALSE;
      KbcSts2 = IoRead8 (0x64);
      if ((KbcSts2 & 0x02) == 0x02) {
        KbcIBF1 = TRUE;
      }
    } 
  }  
  //
  // Loop for Key scan
  //
  for (Index = 0 ; Index < LoopCount ; Index++) {
    //
    // Wait at least for 6ms
    //
    CountTime(6000, PM_BASE_ADDRESS);
    //
    // Read Key stroke
    //
    if (IoRead8 (0x64) & 0x01) {
      KeyValue = IoRead8 (0x60);
    }
    //
    // Check for valid key
    //
    if (KeyValue == 0x1d) {
      //
      // Set BIT0 if it is CTRL key
      //
      KeyScan |= (1 << 0);
    }
    
    if (KeyValue == 0x47) {
      //
      // Set BIT1 if it is HOME key
      //
      KeyScan |= (1 << 1);
    }
    //
    // Check for CTRL_HOME Combination
    //
    if ((KeyScan & 0x03) == 0x03) {
      //
      // Disable KeyBoard Controller
      //
      IoWrite8 (0x64, 0x60);
      IoWrite8 (0x60, 0x30);
      return TRUE;
    }
  } // for (Index = 0 ; Index < LoopCount ; Index++)
  //
  // Disable KeyBoard Controller
  //
  IoWrite8 (0x64, 0x60);
  IoWrite8 (0x60, 0x30);
  //
  // No valid key is pressed
  //
  return FALSE;
}
#endif        // KBC_SUPPORT

/**
    This function checks if TopSwap (A16, A17, A18 address lines
    inversion) is on
              
    @param VOID

    @retval TRUE TopSwap is ON
    @retval FALSE TopSwap is OFF

    @note  Intel Chipsets are porting required
**/
BOOLEAN IsTopSwapOn( VOID )
{
  return FALSE;
/*

  volatile UINT8* Address8;

  Address8 = (UINT8 *)(SB_RCBA + SB_BACKED_UP_CONTROL_REGISTER);

  return ((*Address8) & 1) ? TRUE : FALSE;
*/
}

/**
    This function sets TopSwap (A16, A17, A18 address lines 
    inversion) to ON or OFF.

    @param BOOLEAN On - if TRUE, set TopSwap to ON, if FALSE - set to OFF

    @retval VOID

    @note  Intel Chipsets are porting required
**/
VOID  SetTopSwap(
    IN BOOLEAN      On )
{
/*
  volatile UINT8* Address8;

  Address8 = (UINT8 *)(SB_RCBA + SB_BACKED_UP_CONTROL_REGISTER);

  if (On) {
    *Address8 |= 1;
  } else {
    *Address8 &= 0xFE;
  }
*/
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
