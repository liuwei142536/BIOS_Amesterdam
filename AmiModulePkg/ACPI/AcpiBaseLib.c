//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             5555 Oakbrook Pkwy, Norcross, GA 30093               **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************


/** @file 
    Main ACPI Driver File. It has ACPI Driver entry point,
    ACPISupport Protocol and ACPITable Protocol.

**/
//**********************************************************************

#include <Library/AmiAcpiCpuLib.h>
#include <Token.h>
#include "AcpiCore.h"
#include <Protocol/Cpu.h>
#if DSDT_BUILD
#include <Protocol/AmiBoardInfo2.h>
#endif
#include <AcpiOemElinks.h>
#if GenericSio_SUPPORT
#include <AmiGenericSio.h>
#endif
#if ATAD_SUPPORT == 1
#include "AtadSmi.h"
#endif
#include <IndustryStandard/Acpi60.h>
#include <Library/PcdLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiAmlLib.h>
#include <Library/AmiItemListLib.h>
#include <IndustryStandard/AcpiAml.h>


extern EFI_ACPI_TABLE_PROTOCOL  *mAcpiTableProtocol;
extern EFI_ACPI_SDT_PROTOCOL    *mAcpiSdtProtocol;


typedef struct _ACPI_AML_UPD_INFO {
	UINT64			Dsdt1Addr;///< Address of DSDT for ACPI v1.b
	UINT64			Dsdt2Addr;///< Address of DSDT for ACPI v2+
	ACPI_AML_RES	PciGap[3];
	BOOLEAN 		SS1;///< Value to update _S1 aml object with
	BOOLEAN 		SS2;///< Value to update _S2 aml object with
	BOOLEAN 		SS3;///< Value to update _S3 aml object with
	BOOLEAN 		SS4;///< Value to update _S4 aml object with
	UINT32			RomStart;
	UINT32			TopOfMemory;///< Value to update TOPM aml object with
#if GenericSio_SUPPORT
	SIO_DEV_STATUS	SioDevStatusVar;///< Variable with status of SIO devices, which will be used to update aml
#endif
} ACPI_AML_UPD_INFO;

typedef struct _ACPI_RESET_STRUCTURE
{
	UINT64 			Addess;
	UINT8			Type;
	UINT8			AccessSize;
}ACPI_RESET_STRUCTURE;
//--------------------------------------
//Some Global vars
EFI_GUID 				gSetupGuid = SETUP_GUID;
EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE                 *gxFacs;
ACPI_AML_UPD_INFO       *gAuiGlob = NULL;
UINT16                  gAcpiIaBootArch=0xFFFF;
BOOLEAN 				MadtBuild = TRUE;


#if ATAD_SUPPORT == 1
    VOID  *AtadBuffPtr = NULL;
#endif
//Here goes Interrupt Source Override MADT entry parameter table
//Generated From Information on IRQ_XX_OVERRIDE_ENABLE SDL tokens
ISO_PARAMETER_TABLE IsoTbl[]=
{
//UINT8 PicIrq; UINT8   Flags;                              UINT16  ApicInt
#if IRQ_00_OVERRIDE_ENABLE == 1
    {   0x00,       (IRQ_00_TRIGGER_MODE<<2)|IRQ_00_POLARITY,   IRQ_00_APIC_INT },
#else
    {   0xFF,       0,                                          0               },
#endif
#if IRQ_01_OVERRIDE_ENABLE == 1
    {   0x01,       (IRQ_01_TRIGGER_MODE<<2)|IRQ_01_POLARITY,   IRQ_01_APIC_INT },
#else
    {   0xFF,       0,                                          1               },
#endif
//just dummy entry instead of IRQ2 to keep array consistent
    {   0xFF,       0,                                          2               },
#if IRQ_03_OVERRIDE_ENABLE == 1
    {   0x03,       (IRQ_03_TRIGGER_MODE<<2)|IRQ_03_POLARITY,   IRQ_03_APIC_INT },
#else
    {   0xFF,       0,                                          3               },
#endif
#if IRQ_04_OVERRIDE_ENABLE == 1
    {   0x04,       (IRQ_04_TRIGGER_MODE<<2)|IRQ_04_POLARITY,   IRQ_04_APIC_INT },
#else
    {   0xFF,       0,                                          4               },
#endif
#if IRQ_05_OVERRIDE_ENABLE == 1
    {   0x05,       (IRQ_05_TRIGGER_MODE<<2)|IRQ_05_POLARITY,   IRQ_05_APIC_INT },
#else
    {   0xFF,       0,                                          5               },
#endif
#if IRQ_06_OVERRIDE_ENABLE == 1
    {   0x06,       (IRQ_06_TRIGGER_MODE<<2)|IRQ_06_POLARITY,   IRQ_06_APIC_INT },
#else
    {   0xFF,       0,                                          6               },
#endif
#if IRQ_07_OVERRIDE_ENABLE == 1
    {   0x07,       (IRQ_07_TRIGGER_MODE<<2)|IRQ_07_POLARITY,   IRQ_07_APIC_INT },
#else
    {   0xFF,       0,                                          7               },
#endif
    
#if IRQ_08_OVERRIDE_ENABLE == 1
    {   0x08,       (IRQ_08_TRIGGER_MODE<<2)|IRQ_08_POLARITY,   IRQ_08_APIC_INT },
#else
    {   0xFF,       0,                                          8               },
#endif
#if IRQ_09_OVERRIDE_ENABLE == 1
    {   0x09,       (IRQ_09_TRIGGER_MODE<<2)|IRQ_09_POLARITY,   IRQ_09_APIC_INT },
#else
    {   0xFF,       0,                                          9               },
#endif
#if IRQ_10_OVERRIDE_ENABLE == 1
    {   0x0A,       (IRQ_10_TRIGGER_MODE<<2)|IRQ_10_POLARITY,   IRQ_10_APIC_INT },
#else
    {   0xFF,       0,                                          10              },
#endif
#if IRQ_11_OVERRIDE_ENABLE == 1
    {   0x0B,       (IRQ_11_TRIGGER_MODE<<2)|IRQ_11_POLARITY,   IRQ_11_APIC_INT },
#else
    {   0xFF,       0,                                          11              },
#endif
#if IRQ_12_OVERRIDE_ENABLE == 1
    {   0x0C,       (IRQ_12_TRIGGER_MODE<<2)|IRQ_12_POLARITY,   IRQ_12_APIC_INT },
#else
    {   0xFF,       0,                                          12              },
#endif
#if IRQ_13_OVERRIDE_ENABLE == 1
    {   0x0D,       (IRQ_13_TRIGGER_MODE<<2)|IRQ_13_POLARITY,   IRQ_13_APIC_INT },
#else
    {   0xFF,       0,                                          13              },
#endif
#if IRQ_14_OVERRIDE_ENABLE == 1
    {   0x0E,       (IRQ_14_TRIGGER_MODE<<2)|IRQ_14_POLARITY,   IRQ_14_APIC_INT },
#else
    {   0xFF,       0,                                         14               },
#endif
#if IRQ_15_OVERRIDE_ENABLE == 1
    {   0x0F,       (IRQ_15_TRIGGER_MODE<<2)|IRQ_15_POLARITY,   IRQ_15_APIC_INT },
#else
    {   0xFF,       0,                                          15              },
#endif
};

static UINTN    IsoCnt=sizeof(IsoTbl)/sizeof(ISO_PARAMETER_TABLE);
#if defined(_MSC_EXTENSIONS)
//
// Disable "array is too small" warning, which is false.
//
#pragma warning (push)
#pragma warning (disable: 4295)
#endif

UINT8    ACPI_OEM_ID[6]     = ACPI_OEM_ID_MAK;     
UINT8    ACPI_OEM_TBL_ID[8] = ACPI_OEM_TBL_ID_MAK; 

#if defined(_MSC_EXTENSIONS)
#pragma warning (pop)
#endif

#if defined(OemActivation_SUPPORT) && (OemActivation_SUPPORT == 1)
#define EFI_OA3_MSDM_VARIABLE   L"OA3MSDMvariable"
/**
 	 Internal structure used by OEM Activation process
 
**/
typedef struct _EFI_OA3_MSDM_STRUCTURE {
    EFI_PHYSICAL_ADDRESS  XsdtAddress;///< Address of XSDT table.
    EFI_PHYSICAL_ADDRESS  MsdmAddress;///< Address of MSDM table.
    EFI_PHYSICAL_ADDRESS  ProductKeyAddress;///< Address of a product key.
} EFI_OA3_MSDM_STRUCTURE;

BOOLEAN gOA3Variable = FALSE;
EFI_OA3_MSDM_STRUCTURE gMsdmVariable;
#endif

extern EFI_GUID gAmiGlobalVariableGuid;

UINT8 ChsumTable(UINT8* TblStart, UINT32 BytesCount)
{
    UINTN       i;
    UINT8       Res=*TblStart;
    
    for (i=1; i<BytesCount; i++) Res+=TblStart[i];
    
    Res=0-Res;
    return(Res);
} //checksum_table

/**
    This function creates ACPI table v 2.0+ header with specified signature

         
    @param TblSig ACPI table signature
    @param HdrPtr Pointer to memory, where the header should be placed

          
    @retval VOID

**/

void PrepareHdr20(UINT32 TblSig, EFI_ACPI_SDT_HEADER *HeaderPtr, UINTN Vers)
{
    UINTN i;
    
    if (HeaderPtr==NULL) return;
    
    HeaderPtr->Signature=TblSig;
    
    //Check what Revision# header needs depends on TABLE we're building
    switch (TblSig)
    {
        case RSDT_SIG:
            HeaderPtr->Revision=ACPI_REV1;
            break;
            
        case XSDT_SIG:
            HeaderPtr->Revision=ACPI_REV1;
            break;
            
        case FACP_SIG:
            HeaderPtr->Revision=ACPI_REV3;
            
            if (Vers > 2)
            {
            
                HeaderPtr->Revision=ACPI_REV4;
            }
            
            break;
        case APIC_SIG:
            HeaderPtr->Revision=ACPI_REV1;
#if ACPI_SPEC_VERSION >= 61  
            HeaderPtr->Revision=ACPI_REV4;
#else              
            if (Vers > 2)
            {
            
                HeaderPtr->Revision=ACPI_REV2;
            }
            
            if (Vers > 3)HeaderPtr->Revision=ACPI_REV3;
#endif         
            break;
        case SBST_SIG:
            HeaderPtr->Revision=ACPI_REV1;
            break;
        case SPCR_SIG:
            HeaderPtr->Revision=ACPI_REV1;
            break;
        case ECDT_SIG:
            HeaderPtr->Revision=ACPI_REV1;
            break;
    }
    
    //instead of puting fixed revision number
    //HeaderPtr->Revision=ACPI_REV2;
    
    //Dont' touch Creator ID and Creator Revision;
    if (TblSig != DSDT_SIG)
    {
        if (TblSig == RSDT_SIG)
            HeaderPtr->CreatorId = CREATOR_ID_MS;
        else
            HeaderPtr->CreatorId = CREATOR_ID_AMI;
            
        HeaderPtr->CreatorRevision = CREATOR_REV_MS;
        HeaderPtr->OemRevision = ACPI_OEM_REV;
    }


    for (i=0; i<6; i++) HeaderPtr->OemId[i]=ACPI_OEM_ID[i];
        
    for (i=0; i<8; i++) HeaderPtr->OemTableId[i]=ACPI_OEM_TBL_ID[i];


}  //PrepareHdr20

/**
    This function creates ACPI V1 table header with specified signature

         
    @param TblSig ACPI table signature
    @param HdrPtr Pointer to memory, where the header should be placed

          
    @retval VOID

    @note  
  Depends on type of memory provided, ACPI table header can be in 32 or 64 bit
  format

**/

void PrepareHdr1(UINT32 TblSig, EFI_ACPI_SDT_HEADER* HdrPtr)
{
    UINTN i;
    
    if (HdrPtr==NULL) return;
    
    HdrPtr->Signature = TblSig;
    HdrPtr->Revision = ACPI_REV1;
    
    //Dont' touch Creator ID and Creator Revision;
    if (TblSig != DSDT_SIG)
    {
        if (TblSig == RSDT_SIG)
            HdrPtr->CreatorId = CREATOR_ID_MS;
        else
            HdrPtr->CreatorId = CREATOR_ID_AMI;
            
        HdrPtr->CreatorRevision = CREATOR_REV_MS;
        HdrPtr->OemRevision = ACPI_OEM_REV;
    }


    for (i=0; i<6; i++) HdrPtr->OemId[i]=ACPI_OEM_ID[i];
        
    for (i=0; i<8; i++) HdrPtr->OemTableId[i]=ACPI_OEM_TBL_ID[i];


}//PrepareHdr1

/**
    This function allocates memory for and fills FACP table v 1.1+ with
    predefined values from SDL tokens
         
    @param TablVer Version of FACP table
    @param TablPtr  Pointer to memory, where the FACP table will resides. Filled by this procedure
    @return EFI_STATUS
	@retval EFI_OUT_OF_RESOURCES  Memory for the table could not be allocated
    @retval EFI_SUCCESS  Table was successfully build

**/

#if FACP_BUILD  
EFI_STATUS BuildFacpAll (IN UINTN TablVer, OUT EFI_ACPI_SDT_HEADER **TablPtr)

{
	EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE     	*Facp;
	EFI_ACPI_SDT_HEADER    							*FACP_Hdr;
    UINT32       									SizeOfFacp;
//-----------------------------

    if (TablVer<1 || TablVer>4) return EFI_INVALID_PARAMETER;
    if (ACPI_SPEC_VERSION < 50) 
        SizeOfFacp = sizeof(EFI_ACPI_4_0_FIXED_ACPI_DESCRIPTION_TABLE);
    else 
        SizeOfFacp = sizeof(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE);
    if (ACPI_SPEC_VERSION >= 60) SizeOfFacp = sizeof(EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE); // Add HypervisorVendorIdentity field for ACPI 6.0
    if (TablVer == 1) SizeOfFacp = 0x84;// size of compatability 1.1 structure
    *TablPtr = AllocateZeroPool (SizeOfFacp);
    if ((*TablPtr)==NULL)
    {
        ASSERT(*TablPtr);
        return EFI_OUT_OF_RESOURCES;
    }
    
    FACP_Hdr = *TablPtr;
    
    if (TablVer == 1)
    {
        PrepareHdr20(FACP_SIG, FACP_Hdr, 2);
        FACP_Hdr->Revision = ACPI_REV2;// compatability 1.1 structure
    }
    
    else
        PrepareHdr20(FACP_SIG, FACP_Hdr, TablVer);
        
    Facp = (EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE*) *TablPtr;
    
    Facp->Reserved0 = ACPI_INT_MODEL;
    
    Facp->PreferredPmProfile = ACPI_PM_PROFILE;
    
    Facp->Flags = 0;
    
#if HW_REDUCED_ACPI == 0
    Facp->SciInt       = ACPI_SCI_INT;
    Facp->SmiCmd       = SW_SMI_IO_ADDRESS;
    Facp->AcpiEnable   = SW_SMI_ACPI_ENABLE;

    Facp->AcpiDisable  = SW_SMI_ACPI_DISABLE;

    Facp->S4BiosReq    = SW_SMI_S4BIOS; 
    
    Facp->PstateCnt    = SW_SMI_PSTATE_CNT;
 
    if (PM1A_EVT_BLK_ADDRESS > 0xffffffff)
    	Facp->Pm1aEvtBlk  = 0;
    else
    	Facp->Pm1aEvtBlk  = PM1A_EVT_BLK_ADDRESS;
        
    if (PM1B_EVT_BLK_ADDRESS > 0xffffffff)
    	Facp->Pm1bEvtBlk  = 0;
    else
    	Facp->Pm1bEvtBlk  = PM1B_EVT_BLK_ADDRESS;
        
    if (PM1A_CNT_BLK_ADDRESS > 0xffffffff) 
    	Facp->Pm1aCntBlk  = 0;
    else
    	Facp->Pm1aCntBlk  = PM1A_CNT_BLK_ADDRESS;
        
    if (PM1B_CNT_BLK_ADDRESS > 0xffffffff)
    	Facp->Pm1bCntBlk  = 0;
    else
    	Facp->Pm1bCntBlk  = PM1B_CNT_BLK_ADDRESS;
        
    if (PM2_CNT_BLK_ADDRESS > 0xffffffff)
    	Facp->Pm2CntBlk   = 0;
    else
    	Facp->Pm2CntBlk   = PM2_CNT_BLK_ADDRESS;
        
    if (PM_TMR_BLK_ADDRESS > 0xffffffff)
    	Facp->PmTmrBlk    = 0;
    else
    	Facp->PmTmrBlk    = PM_TMR_BLK_ADDRESS;
        
    if (GPE0_BLK_ADDRESS > 0xffffffff)
    	Facp->Gpe0Blk  = 0;
    else
    	Facp->Gpe0Blk  = GPE0_BLK_ADDRESS;
        
    if (GPE1_BLK_ADDRESS > 0xffffffff) 
    	Facp->Gpe1Blk  = 0;
    else
    	Facp->Gpe1Blk  = GPE1_BLK_ADDRESS;

     
    Facp->Gpe0BlkLen          = GPE0_BLK_LENGTH; 
    Facp->Gpe1BlkLen          = GPE1_BLK_LENGTH; 
    Facp->Gpe1Base             = GPE1_BASE_OFFSET; 
    Facp->Pm1EvtLen   = PM1_EVT_LENGTH;
    Facp->Pm1CntLen   = PM1_CNT_LENGTH;
    Facp->Pm2CntLen   = PM2_CNT_LENGTH;
    Facp->PmTmrLen     = PM_TMR_LENGTH;
    
    Facp->CstCnt   = SW_SMI_CST_CNT;
    Facp->PLvl2Lat    = P_LVL2_LAT_VAL;
    Facp->PLvl3Lat    = P_LVL3_LAT_VAL;
    Facp->FlushSize    = FLUSH_SIZE_VAL;
    Facp->FlushStride  = FLUSH_STRIDE_VAL;
    Facp->DutyOffset   = DUTY_OFFSET_VAL;
    Facp->DutyWidth    = DUTY_WIDTH_VAL; 
    Facp->DayAlrm      = ACPI_ALARM_DAY_CMOS;
    Facp->MonAlrm      = ACPI_ALARM_MONTH_CMOS;
    Facp->Century       = ACPI_CENTURY_CMOS;
    Facp->IaPcBootArch = ACPI_IA_BOOT_ARCH;
    
    //--------Filling Flags for V.1----------------------
    
    if (FACP_FLAG_WBINVD_FLUSH) Facp->Flags |= 1<<1;
    
    if (FACP_FLAG_PROC_C1)      Facp->Flags |= 1<<2;
    
    if (FACP_FLAG_P_LVL2_UP)    Facp->Flags |= 1<<3;
    
    if (FACP_FLAG_RTC_S4)       Facp->Flags |= 1<<7;
    
    if (FACP_FLAG_TMR_VAL_EXT)  Facp->Flags |= 1<<8;
    
    if (FACP_FLAG_HEADLESS)         Facp->Flags |= 1<<12;
    
    if (FACP_FLAG_CPU_SW_SLP)       Facp->Flags |= 1<<13;
#if ACPI_SPEC_VERSION >= 30  
    
    if (FACP_FLAG_S4_RTC_STS_VALID)             Facp->Flags |= 1<<16;
    
    if (FACP_FLAG_REMOTE_POWER_ON_CAPABLE)  	Facp->Flags |= 1<<17;

    if (FACP_FLAG_PCI_EXP_WAK)      			Facp->Flags |= 1<<14;
    
#endif //#if ACPI_SPEC_VERSION >= 30  
    
#endif //#if HW_REDUCED_ACPI == 0
    
    if (FACP_FLAG_WBINVD)       Facp->Flags |= 1;
    
    if (FACP_FLAG_PWR_BUTTON)   Facp->Flags |= 1<<4;
    
    if (FACP_FLAG_SLP_BUTTON)   Facp->Flags |= 1<<5;
    
    if (FACP_FLAG_FIX_RTC)      Facp->Flags |= 1<<6;
    
    if (FACP_FLAG_DCK_CAP)      Facp->Flags |= 1<<9;
    
    
    //--------Filling Flags for V.2 and GAS compat structure for v.1----------------------
    
    if (FACP_FLAG_SEALED_CASE)      Facp->Flags |= 1<<11;
    

    
#if ACPI_SPEC_VERSION >= 30  
    
    if (FACP_FLAG_USE_PLATFORM_CLOCK)                   Facp->Flags |= 1<<15;
    
    if (TablVer > 2)
    {
        if (FACP_FLAG_FORCE_APIC_CLUSTER_MODEL)             Facp->Flags |= 1<<18;
        
        if (FACP_FLAG_FORCE_APIC_PHYSICAL_DESTINATION_MODE) Facp->Flags |= 1<<19;
        
    }
    
#endif
    
    // RESET_REG GAS_20 structure and value
    Facp->ResetReg.AddressSpaceId   = ACPI_RESET_REG_TYPE;
    Facp->ResetReg.RegisterBitWidth = ACPI_RESET_REG_BITWIDTH;
    Facp->ResetReg.RegisterBitOffset  = ACPI_RESET_REG_BITOFFSET;
    Facp->ResetReg.Address     = ACPI_RESET_REG_ADDRESS;
#ifdef ACPI_RESET_REG_ACCESSWIDTH
    Facp->ResetReg.AccessSize = ACPI_RESET_REG_ACCESSWIDTH;
#endif
    Facp->ResetValue           = ACPI_RESET_REG_VALUE;
    
    if (ACPI_RESET_REG_ADDRESS && FACP_FLAG_RESET_REG_SUP)
    {
        // Set FACP flag
        Facp->Flags |= 1<<10;
    }
    
    if (TablVer == 1)
    {
        Facp->Header.Length     = 0x84;
        Facp->Header.Checksum = 0;
        Facp->Header.Checksum = ChsumTable((UINT8*)Facp, Facp->Header.Length);
        return EFI_SUCCESS;
    }
    
    //--------This is all for V.1-----------------------
#if HW_REDUCED_ACPI == 0    
    // PM1a_EVT_BLK GAS_20 structure
    Facp->XPm1aEvtBlk.AddressSpaceId  = PM1A_EVT_BLK_TYPE;
    Facp->XPm1aEvtBlk.RegisterBitWidth = PM1A_EVT_BLK_BITWIDTH;
    Facp->XPm1aEvtBlk.RegisterBitOffset = PM1A_EVT_BLK_BITOFFSET;
    Facp->XPm1aEvtBlk.AccessSize  = PM1A_EVT_BLK_ACCESS_SIZE;
    Facp->XPm1aEvtBlk.Address    = PM1A_EVT_BLK_ADDRESS;
    
    // PM1a_CNT_BLK GAS_20 structure
    Facp->XPm1aCntBlk.AddressSpaceId  = PM1A_CNT_BLK_TYPE;
    Facp->XPm1aCntBlk.RegisterBitWidth = PM1A_CNT_BLK_BITWIDTH;
    Facp->XPm1aCntBlk.RegisterBitOffset = PM1A_CNT_BLK_BITOFFSET;
    Facp->XPm1aCntBlk.AccessSize  = PM1A_CNT_BLK_ACCESS_SIZE;
    Facp->XPm1aCntBlk.Address    = PM1A_CNT_BLK_ADDRESS;
    
    // PM1b_EVT_BLK GAS_20 structure
    Facp->XPm1bEvtBlk.AddressSpaceId  = PM1B_EVT_BLK_TYPE;
    Facp->XPm1bEvtBlk.RegisterBitWidth = PM1B_EVT_BLK_BITWIDTH;
    Facp->XPm1bEvtBlk.RegisterBitOffset = PM1B_EVT_BLK_BITOFFSET;
    Facp->XPm1bEvtBlk.AccessSize  = PM1B_EVT_BLK_ACCESS_SIZE;
    Facp->XPm1bEvtBlk.Address    = PM1B_EVT_BLK_ADDRESS;
    
    // PM1b_CNT_BLK GAS_20 structure
    Facp->XPm1bCntBlk.AddressSpaceId  = PM1B_CNT_BLK_TYPE;
    Facp->XPm1bCntBlk.RegisterBitWidth = PM1B_CNT_BLK_BITWIDTH;
    Facp->XPm1bCntBlk.RegisterBitOffset = PM1B_CNT_BLK_BITOFFSET;
    Facp->XPm1bCntBlk.AccessSize    = PM1B_CNT_BLK_ACCESS_SIZE;
    Facp->XPm1bCntBlk.Address    = PM1B_CNT_BLK_ADDRESS;
    
    // PM1b_CNT_BLK GAS_20 structure
    Facp->XPm2CntBlk.AddressSpaceId   = PM2_CNT_BLK_TYPE;
    Facp->XPm2CntBlk.RegisterBitWidth = PM2_CNT_BLK_BITWIDTH;
    Facp->XPm2CntBlk.RegisterBitOffset  = PM2_CNT_BLK_BITOFFSET;
    Facp->XPm2CntBlk.AccessSize    = PM2_CNT_BLK_ACCESS_SIZE;
    Facp->XPm2CntBlk.Address     = PM2_CNT_BLK_ADDRESS;
    
    Facp->XPmTmrBlk.AddressSpaceId    = PM_TMR_BLK_TYPE;
    Facp->XPmTmrBlk.RegisterBitWidth  = PM_TMR_BLK_BITWIDTH;
    Facp->XPmTmrBlk.RegisterBitOffset   = PM_TMR_BLK_BITOFFSET;
    Facp->XPmTmrBlk.AccessSize    = PM_TMR_BLK_ACCESS_SIZE;
    Facp->XPmTmrBlk.Address      = PM_TMR_BLK_ADDRESS;
    
    Facp->XGpe0Blk.AddressSpaceId      = GPE0_BLK_TYPE;
    Facp->XGpe0Blk.RegisterBitWidth    = GPE0_BLK_BITWIDTH;
    Facp->XGpe0Blk.RegisterBitOffset     = GPE0_BLK_BITOFFSET;
    Facp->XGpe0Blk.AccessSize    = GPE0_BLK_ACCESS_SIZE;
    Facp->XGpe0Blk.Address        = GPE0_BLK_ADDRESS;
    
    Facp->XGpe1Blk.AddressSpaceId      = GPE1_BLK_TYPE;
    Facp->XGpe1Blk.RegisterBitWidth    = GPE1_BLK_BITWIDTH;
    Facp->XGpe1Blk.RegisterBitOffset     = GPE1_BLK_BITOFFSET;
    Facp->XGpe1Blk.AccessSize    = GPE1_BLK_ACCESS_SIZE;
    Facp->XGpe1Blk.Address        = GPE1_BLK_ADDRESS;
#endif //#if HW_REDUCED_ACPI == 0   
    if (ACPI_SPEC_VERSION >= 50) 
    {
#if HW_REDUCED_ACPI      

        Facp->Flags |= 1<<20;

        Facp->SleepControlReg.AddressSpaceId   = SLEEP_CONTROL_REG_TYPE;
        Facp->SleepControlReg.RegisterBitWidth = SLEEP_CONTROL_REG_BITWIDTH;
        Facp->SleepControlReg.RegisterBitOffset  = SLEEP_CONTROL_REG_BITOFFSET;
        Facp->SleepControlReg.AccessSize  = SLEEP_CONTROL_REG_ACCESS_SIZE;
        Facp->SleepControlReg.Address     = SLEEP_CONTROL_REG_ADDRESS;

        Facp->SleepStatusReg.AddressSpaceId    = SLEEP_STATUS_REG_TYPE;
        Facp->SleepStatusReg.RegisterBitWidth  = SLEEP_STATUS_REG_BITWIDTH;
        Facp->SleepStatusReg.RegisterBitOffset   = SLEEP_STATUS_REG_BITOFFSET;
        Facp->SleepStatusReg.AccessSize   = SLEEP_STATUS_REG_ACCESS_SIZE;
        Facp->SleepStatusReg.Address      = SLEEP_STATUS_REG_ADDRESS;
#endif
    
        if (LOW_POWER_S0_IDLE_CAPABLE)       Facp->Flags |= 1<<21;
        Facp->Header.Revision = 5; // ACPI 5.0 revision
#if ACPI_SPEC_VERSION >= 51
        
        Facp->ArmBootArch = ACPI_ARM_BOOT_ARCH; //ARM flags added in ACPI 5.1 revision
        if ((ACPI_SPEC_VERSION == 51) || (ACPI_SPEC_VERSION == 61)) Facp->MinorVersion = 1; // FADT Minor Version = 1 for ACPI 5.1 revision
        if (ACPI_SPEC_VERSION == 62) Facp->MinorVersion = 2;
        if (ACPI_SPEC_VERSION >= 60) 
        {
        	Facp->Header.Revision = 6; // ACPI 6 revision
#ifdef HYPERVISOR_VENDOR_IDENTITY
        	Facp->HypervisorVendorIdentity = HYPERVISOR_VENDOR_IDENTITY;
#endif
        }
#endif    
    }
    Facp->Header.Length     = SizeOfFacp;
    Facp->Header.Checksum = 0;
    Facp->Header.Checksum = ChsumTable((UINT8*)Facp, Facp->Header.Length);
    
    return EFI_SUCCESS;
    
}// end of BuildFacpAll
#endif

/**
    This function will probe IndReg and DataReg of discovered IOxAPIC base address
    and allocate memory for and builds Ioapic/Iosapic entry for MADT table

         
    @param BaseAddr Base Addrerss where IOxApic presence detected
    @param VecBase pointer to the System Vector Base variable that
        needs to be updated with IOAPIC.MAXREDIR# by this function


          
    @retval MADT_ENTRY_HEADER pointer to the Ioapic/Iosapi entry header for the MADT table
        if NULL - not enough memory

**/
#if defined(_MSC_EXTENSIONS)
#pragma optimize( "", off )
#endif
MADT_ENTRY_HEADER *BuildIoapicIosapic(IN UINT32 BaseAddr, OUT UINT32 *VecBase)
{
    UINT32          Tmp;
    UINT32  volatile        *DatPtr;
    UINT8   volatile        *IdxPtr;
    UINT32          IoapicId;
    UINT32          IoapicVer;
    IOAPIC_H20       *IoapicPtr;
    IOSAPIC_H20      *IosapicPtr;
    
//==============================
    //Read IOAPIC.ID reg and figure out
    //1.How many INTINs it has
    //2.What ID did BIOS gave to it
    IdxPtr=(UINT8*)(UINTN)(BaseAddr + IOA_IND_REG_OFFS);
    DatPtr=(UINT32*)(UINTN)(BaseAddr + IOA_DAT_REG_OFFS);
    
    *IdxPtr = IOA_ID_IND;                   // bits 27..24=IOAPIC_ID
    
    Tmp = *DatPtr;
    
    IoapicId = ((Tmp&IO_APIC_ID_READ_MASK)>>24);
    
    *IdxPtr = IOA_VER_IND;                  // IOAPIC_VER register
    
    Tmp=*DatPtr;
    
    IoapicVer=(Tmp&0xff);
    
    if (IoapicVer < IO_APIC_VERSION_PARAMETER)  // This is Ioapic
    {
        IoapicPtr = AllocateZeroPool(sizeof(IOAPIC_H20));
        ASSERT (IoapicPtr);
        
        if (!IoapicPtr) return NULL;
        
        IoapicPtr->Header.Type = (UINT8) AT_IOAPIC;     // 1 - For IOAPIC Structure
        IoapicPtr->Header.Length = (UINT8) sizeof (IOAPIC_H20);
        IoapicPtr->IoapicAddress = (UINT32) BaseAddr;
        IoapicPtr->IoapicId = (UINT8) IoapicId;
        IoapicPtr->SysVectBase = (UINT32) *VecBase;
        //Adjust VecBase with current Max Redirection value
        (*VecBase)+=((UINT32)((Tmp&0x00ff0000)>>16))+1;
        return (MADT_ENTRY_HEADER*) IoapicPtr;
    }
    
    else  // This is Iosapic
    {
        IosapicPtr = AllocateZeroPool(sizeof(IOSAPIC_H20));
        ASSERT (IosapicPtr);
        
        if (!IosapicPtr) return NULL;
        
        IosapicPtr->Header.Type = (UINT8) AT_IOSAPIC;     // 6 - For IOSAPIC Structure
        IosapicPtr->Header.Length = (UINT8) sizeof (IOSAPIC_H20);
        IosapicPtr->IosapicAddress = (UINT64) BaseAddr;
        IosapicPtr->IoapicId = (UINT8) IoapicId;
        IosapicPtr->SysVectBase = (UINT32) *VecBase;
        //Adjust VecBase with current Max Redirection value
        (*VecBase)+=((UINT32)((Tmp&0x00ff0000)>>16))+1;
        return (MADT_ENTRY_HEADER*) IosapicPtr;
    }
    
}// End of BuildIoapicIosapic
#if defined(_MSC_EXTENSIONS)
#pragma optimize( "", on )
#endif
/**
    Allocates memory for and builds LAPIC entry for MADT table filled with
    Dummy information

          
    @retval MADT_ENTRY_HEADER pointer to the LAPIC entry header for the MADT table
        if NULL - not enough memory

**/

MADT_ENTRY_HEADER *BuildIoapicManualy()
{
    IOAPIC_H20       *IoapicPtr;
    
    IoapicPtr = AllocateZeroPool(sizeof(IOAPIC_H20));
    ASSERT (IoapicPtr);
    
    if (!IoapicPtr) return NULL;
    
    IoapicPtr->Header.Type = (UINT8) AT_IOAPIC;     // 1 - For IOAPIC Structure
    IoapicPtr->Header.Length = (UINT8) sizeof (IOAPIC_H20);
    IoapicPtr->IoapicAddress = (UINT32) APCB;
    IoapicPtr->IoapicId = (UINT8) 0;
    IoapicPtr->SysVectBase = (UINT32) 0;
    return (MADT_ENTRY_HEADER*) IoapicPtr;
    
    
}// end of BuildIoapicManualy

/**
    Allocates memory for and builds ISO entry for MADT table, based
    on INTERRUPT SOURCE OVERRIDE info from predifined table

    @param IsoTblNumb - Number of entry in ISO table for which an entry should
        be build.

          
    @retval MADT_ENTRY_HEADER pointer to the ISO entry header for the MADT table
        if NULL - not enough memory

**/

MADT_ENTRY_HEADER *BuildIsoFromTbl (UINTN IsoTblNumb)
{
    ISO_H20     *IsoPtr;
    
    IsoPtr = AllocateZeroPool(sizeof(ISO_H20));
    ASSERT (IsoPtr);
    
    if (!IsoPtr) return NULL;
    
    IsoPtr->Header.Type = AT_ISO; // 2 - For ISO structure
    IsoPtr->Header.Length = sizeof(ISO_H20);
    IsoPtr->Bus = 0;
    IsoPtr->Source = (UINT8) IsoTbl[IsoTblNumb].PicIrq;
    IsoPtr->GlobalSysVect = (UINT32) IsoTbl[IsoTblNumb].ApicInt;
    IsoPtr->Flags = (UINT16) IsoTbl[IsoTblNumb].Flags;
    return (MADT_ENTRY_HEADER*) IsoPtr;
}//end of BuildIsoFromTbl

/**
    Creates MADT entries, stores them as AMI_ITEM_LIST, calculates
    space needed, allocates memory and builds MADT table from entries

         
    @param TablVer ACPI Spec. Version
    @param TablPtr Pointer, to the MADT table, filled by this function

    @return EFI_STATUS
    @retval EFI_OUT_OF_RESOURCES not enough memory
    @retval EFI_UNSUPPORTED CPU info hob not found
    @retval EFI_INVALID_PARAMETER invalid ACPI version
    @retval EFI_SUCCESS MADT table were successfully build

**/
#if ACPI_APIC_TBL == 1
EFI_STATUS BuildMadtAll(IN UINTN TablVer, OUT EFI_ACPI_SDT_HEADER **TablPtr)
{
    UINTN                       LastItem;
    EFI_STATUS                  Status;
    MADT_ENTRIES                MadtTblEntries = {0, 0, NULL};
    MADT_ENTRY_HEADER           *HdrPtr;
    UINT32                      IoapicVbase = 0, AllStrLength = 0, IoapicAddr = 0;
    UINT8                       *DestinPtr;
    UINTN 			i;
    #if FEC00000_APIC_AUTODETECT == 1 || PCI_BUS_APIC_AUTODETECT == 1 
    UINTN 			j;
    #endif //FEC00000_APIC_AUTODETECT == 1 || PCI_BUS_APIC_AUTODETECT == 1 
    UINTN                       IoApicCnt=0;

    VOID *CpuEntries;
    UINT8 *CpuEntry;
    UINT32 NumCpuEntries;
    UINT32 CpuEntriesSize;
    
//--------------------------------
    if (TablVer<1 || TablVer>4) return EFI_INVALID_PARAMETER;

    Status = CreateCpuMadtEntries((UINT32)TablVer, &CpuEntries, &NumCpuEntries, &CpuEntriesSize);

    //Add CPU MadtTblEntries
    if (!EFI_ERROR(Status)) {
        CpuEntry = (UINT8*)CpuEntries;
        while(NumCpuEntries--){
            UINT8 Length = ((MADT_ENTRY_HEADER*)CpuEntry)->Length;
            HdrPtr = (MADT_ENTRY_HEADER*)AllocateZeroPool(Length);
            CopyMem (HdrPtr, CpuEntry, Length);
            AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);// First entry
            CpuEntry += Length;
        }
        gBS->FreePool(CpuEntries);
    }

    //Set Marker at last item before IOAPIC/IOsAPIC entries
    LastItem = MadtTblEntries.MadtEntCount;
    
//The folloving code may enable IOAPIC Devices found on PCI BUS
//This IOAPICs might use their ABARs to clame MMIO in FEC0_0000 space.
//So we need to run this code prior  if we don't want to miss any IOAPICs.
//--------------------------------------------------------------------
#if PCI_BUS_APIC_AUTODETECT == 1
    {
        EFI_HANDLE *pHandleBuffer;
        UINTN   NumberOfHandles;
        UINT8   PciData[4];
        EFI_GUID PciIoProtocolGuid = EFI_PCI_IO_PROTOCOL_GUID;
        EFI_PCI_IO_PROTOCOL *pPciIoProtocol;
        
        Status = gBS->LocateHandleBuffer(ByProtocol, &PciIoProtocolGuid,
                                         NULL, &NumberOfHandles, &pHandleBuffer);
                                         
        //The protocol might not be available when function runs first time.
        //we will rerun it on READY_TO_BOOT event again.
        if (!EFI_ERROR(Status))
        {
        
            for (i = 0; i < NumberOfHandles; i++)
            {
                Status = gBS->HandleProtocol(pHandleBuffer[i], &PciIoProtocolGuid,
                                             (VOID**)&pPciIoProtocol);
                                             
                if (EFI_ERROR(Status))
                    continue;
                    
                //read class code information at 0x8 offset in PCI header
                Status = pPciIoProtocol->Pci.Read(pPciIoProtocol, EfiPciIoWidthUint32,
                                                  0x8, 1, &PciData[0]);
                ASSERT_EFI_ERROR(Status);
                
                if (EFI_ERROR(Status)) //problem
                    continue;
                    
                //if IO APIC device
                if ((PciData[3] == 0x8) && (PciData[2] == 0) && (PciData[1] >= 0x10))
                {
                    UINT64  Attr=0, OldAttr=0;
                    //----------------------
                    //1. make sure it is Enabled and Decoding it's resources
                    Status=pPciIoProtocol->Attributes(pPciIoProtocol,EfiPciIoAttributeOperationGet, Attr, &OldAttr);
                    ASSERT_EFI_ERROR(Status);
                    
                    if (EFI_ERROR(Status)) continue;
                    
                    Status=pPciIoProtocol->Attributes(pPciIoProtocol,EfiPciIoAttributeOperationSupported, 0, &Attr);
                    ASSERT_EFI_ERROR(Status);
                    
                    if (EFI_ERROR(Status)) continue;
                    
                    Status=pPciIoProtocol->Attributes(pPciIoProtocol,EfiPciIoAttributeOperationSet, Attr&(EFI_PCI_DEVICE_ENABLE), NULL);
                    ASSERT_EFI_ERROR(Status);
                    
                    if (EFI_ERROR(Status)) continue;
                    
                    
                    //2. collect info
                    Status = pPciIoProtocol->Pci.Read(pPciIoProtocol, EfiPciIoWidthUint32,
                                                      0x10, 1, (VOID*)&IoapicAddr);
                    //problem or mapped to default address range
                    ASSERT_EFI_ERROR(Status);
                    
                    if ( ! (EFI_ERROR(Status) || (IoapicAddr == 0)) )
                    {
                        HdrPtr = BuildIoapicIosapic(IoapicAddr, &IoapicVbase);
                        
                        if (!HdrPtr) return EFI_OUT_OF_RESOURCES;
                        
                        if (MadtTblEntries.MadtEntCount == LastItem)   // Adding entries in groving IOapicId/IOsapicId value order
                        {
                            Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);// First entry
                        }
                        
                        else
                        {
                            for (j = (LastItem); j < MadtTblEntries.MadtEntCount; j++)  // No need to handle LSAPIC entry in different way
                            {
                                // IOapicId and IOsapicId filds are on the same place in bouth structures
                                if (((IOAPIC_H32*)HdrPtr)->IoapicId < ((IOAPIC_H32*)MadtTblEntries.MadtEntries[j])->IoapicId) break;
                            }   // found entry with bigger IOapicId/IOsapicId
                            
                            if (j == MadtTblEntries.MadtEntCount)
                            {
                                Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);
                            }
                            
                            else
                            {
                                Status = AmiItemListInsert ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr, j);
                            }
                            
                            ASSERT_EFI_ERROR(Status);
                            
                            if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
                            
                            IoApicCnt++;
                        }
                    }//if( ! (EFI_ERROR(Status) || (IoapicAddr == 0)) )
                    
#if PCI_BUS_APIC_LEAVE_ENABLE != 0
                    //Restore attributes of the device
                    Status=pPciIoProtocol->Attributes(pPciIoProtocol,EfiPciIoAttributeOperationSet, OldAttr, NULL);
                    ASSERT_EFI_ERROR(Status);
                    
                    if (EFI_ERROR(Status)) continue;
                    
#endif
                }//if((PciData[3] == 0x8) && (PciData[2] == 0) && (PciData[1] >= 0x10))
            }//for(i = 0; i < NumberOfHandles; i++)
            
            gBS->FreePool(pHandleBuffer);
        }//if(!EFI_ERROR(Status))
    }
#endif //PCI_BUS_APIC_AUTODETECT == 1
    
//--------------------------------------------------------------------
//---Creating IOAPIC or/and IOSAPIC Entries
#if (APCB != 0)
    IoapicAddr = APCB;
#else
    IoapicAddr = IOA_BASE_BOT;
#endif
    
#if FEC00000_APIC_AUTODETECT == 1
    
    //trying to check if something alive present at 0xFEC00000..0xFED00000
    for ( ; IoapicAddr < IO_APIC_BASE_ADDRESS_TOP; IoapicAddr += 0x1000)
    { //If so read IOAPIC.ID reg and figure out
        //1.How many INTINs it has
        //2.What ID BIOS give to it
        if (*((UINT8 *)(UINTN)(IoapicAddr + IOA_IND_REG_OFFS)) != 0xFF)
        {
            HdrPtr = BuildIoapicIosapic(IoapicAddr, &IoapicVbase);
            
            if (!HdrPtr) return EFI_OUT_OF_RESOURCES;
            
            if (MadtTblEntries.MadtEntCount == LastItem)  // Adding entries in groving IOapicId/IOsapicId value order
                Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);// First entry
            else
            {
                for (j = (LastItem); j < MadtTblEntries.MadtEntCount; j++)
                { // No need to handle LSAPIC entry in different way
                    if (((IOAPIC_H32*)HdrPtr)->IoapicId < ((IOAPIC_H32*)MadtTblEntries.MadtEntries[j])->IoapicId) break; // IOapicId and IOsapicId filds are on the same place in bouth structures
                }   // found entry with bigger IOapicId/IOsapicId
                
                if (j == MadtTblEntries.MadtEntCount)
                {
                    Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);
                }
                
                else
                {
                    Status = AmiItemListInsert ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr, j);
                }
            }
            
            ASSERT_EFI_ERROR(Status);
            
            if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
            
            IoApicCnt++;
        }
    }
    
#endif //FEC00000_APIC_AUTODETECT == 1
    
//-------------------------------------------------------------------------------------------
    if (!IoApicCnt)
    {
        HdrPtr = BuildIoapicManualy();
        
        if (!HdrPtr) return EFI_OUT_OF_RESOURCES;
        
        Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);
        ASSERT_EFI_ERROR(Status);
        
        if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
    }
    
    
//-------Build ISO Structure----------------------------
    for (i=0; i<IsoCnt; i++)
    {
        if (IsoTbl[i].PicIrq == 0xFF) continue; //no override for this entry
        
        HdrPtr = BuildIsoFromTbl(i);
        
        if (!HdrPtr) return EFI_OUT_OF_RESOURCES;
        
        Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);
        ASSERT_EFI_ERROR(Status);
        
        if (EFI_ERROR(Status)) return Status;
    }
    
#if (NMIs_QUANTITY > 0)
    HdrPtr = AllocateZeroPool(sizeof(NMI_H20));
    ASSERT (HdrPtr);
    
    if (!HdrPtr) return EFI_OUT_OF_RESOURCES;
    
    ((NMI_H20*) HdrPtr)->Header.Type = (UINT8) AT_NMI;       //Type 3 - indicating NMIs Entry
    ((NMI_H20*) HdrPtr)->Header.Length = (UINT8) sizeof(NMI_H20);
    ((NMI_H20*) HdrPtr)->Flags = (UINT16)((NMI_0_TRIGGER_MODE<<2) | NMI_0_POLARITY);
    ((NMI_H20*) HdrPtr)->GlobalSysVect = (UINT32) NMI_GLOBAL_SYS_INT_0;
    Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);
    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) return Status;
    
#if (NMIs_QUANTITY > 1)
    HdrPtr = AllocateZeroPool(sizeof(NMIH_20));
    ASSERT (HdrPtr);
    
    if (!HdrPtr) return EFI_OUT_OF_RESOURCES;
    
    ((NMI_H20*) HdrPtr)->Header.Type = (UINT8) AT_NMI;       //Type 3 - indicating NMIs Entry
    ((NMI_H20*) HdrPtr)->Header.Length = (UINT8) sizeof(NMI_H20);
    ((NMI_H20*) HdrPtr)->Flags = (UINT16)((NMI_1_TRIGGER_MODE<<2) | NMI_1_POLARITY);
    ((NMI_H20*) HdrPtr)->GlobalSysVect = (UINT32) NMI_GLOBAL_SYS_INT_1;
    Status = AmiItemListAppend ((AMI_ITEM_LIST*)&MadtTblEntries, (VOID*) HdrPtr);
    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) return Status;
#endif
    
//---Porting Hook 1 ------ If NMIs_QUANTITY > 2 - Add more structures
#endif

    //---Tis ia all for Ver 1 - building a table----------------------
//----Here starts entries for V 2 and 3 which are empty for now----------------------------------
//
//          Here entries for LAPIC Address overrride structure and Platforme interrupt source structure
//

    for (i = 0; i < MadtTblEntries.MadtEntCount; i++)
    {
    
        AllStrLength += MadtTblEntries.MadtEntries[i]->Length;
    }
    
    *TablPtr = AllocateZeroPool(sizeof(EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER) + (UINTN) AllStrLength);
    ASSERT (*TablPtr);
    
    if (!(*TablPtr)) return EFI_OUT_OF_RESOURCES;
    
    if (TablVer == 1) PrepareHdr1 (APIC_SIG, (EFI_ACPI_SDT_HEADER*) *TablPtr);
    else PrepareHdr20 (APIC_SIG, (EFI_ACPI_SDT_HEADER*) *TablPtr, TablVer);
    
    (*TablPtr)->Length = (sizeof(EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER) + AllStrLength);
    ((EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER*)*TablPtr)->LocalApicAddress    = LOCAL_APIC_BASE;
    ((EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER*)*TablPtr)->Flags    = ACPI_APIC_FLAGS;
    DestinPtr = ((UINT8*) *TablPtr + sizeof(EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER));
    
    for (i = 0; i < MadtTblEntries.MadtEntCount; i++)
    {
    
    	gBS->CopyMem (DestinPtr, (UINT8*)MadtTblEntries.MadtEntries[i], (UINT32) MadtTblEntries.MadtEntries[i]->Length);
        DestinPtr += MadtTblEntries.MadtEntries[i]->Length;
        gBS->FreePool((VOID*)MadtTblEntries.MadtEntries[i]);
    }
    
    (*TablPtr)->Checksum = 0;
    (*TablPtr)->Checksum = ChsumTable((UINT8*)*TablPtr, (*TablPtr)->Length);
    
    return EFI_SUCCESS;
}// end of BuildMadtAll-----------------------------------------
#endif

/**
    Allocates ACPI NVS memory and builds FACS table from values,
    defined by SDL tokens

    @param VOID


    @return EFI_STATUS
    @retval EFI_OUT_OF_RESOURCES not enough memory
    @retval EFI_SUCCESS FACS table were successfully build

**/

EFI_STATUS BuildFacs (OUT EFI_ACPI_SDT_HEADER **TablPtr)
{
    
	*TablPtr = NULL;
    *TablPtr = AllocateZeroPool (sizeof(EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE));
    
    if (*TablPtr == NULL)
    {	
    	ASSERT(FALSE);
    	return EFI_OUT_OF_RESOURCES;
    }
	
    
    gxFacs = (EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE*)*TablPtr;
    gxFacs->Signature=(UINT32)FACS_SIG;
    gxFacs->Length=sizeof(EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE);
    
    if (ACPI_SPEC_VERSION < 40)
    {
        gxFacs->Flags=FACS_FLAG_S4BIOS;
        gxFacs->Version=ACPI_REV1;
    }
    
    else
    {
        gxFacs->Flags = FACS_FLAG_S4BIOS | (FACS_FLAG_64BIT_WAKE_SUPPORTED << 1);
        gxFacs->Version=ACPI_REV2;
    }
    return EFI_SUCCESS;
}// end of  BuildFacs ---------------------------------------------

/**
    This function finds DSDT table in firmvare volume

         
    @param Dsdt1 pointer to memory where DSDT v1.1 table will be stored
    @param Dsdt2 pointer to memory where DSDT v2.0+ table will be stored

          
    @retval EFI_SUCCESS Function executed successfully
    @retval EFI_ABORTED ACPI storage file not found
    @retval EFI_NOT_FOUND DSDT table not found in firmware volume

**/
#if DSDT_BUILD
EFI_STATUS GetDsdtFv(OUT EFI_ACPI_SDT_HEADER **Dsdt2)
{
	EFI_STATUS	Status;
	AMI_BOARD_INFO2_PROTOCOL	*AmiBoardInfo2Protocol=NULL;
	
    //In current AmiBoardInfo implementation separate instance of
    //DSDT for ACPI version 1.1b  DOES NOT SUPPORTED!

	Status = gBS->LocateProtocol(&gAmiBoardInfo2ProtocolGuid, NULL,(VOID**)&AmiBoardInfo2Protocol);

	if(EFI_ERROR(Status)){
		DEBUG((EFI_D_ERROR,"AcpiCore: No AmiBoardInfo Found: Status=%r\n", Status));
		return EFI_NOT_FOUND;
	}
	*Dsdt2 = (EFI_ACPI_SDT_HEADER*)AllocateZeroPool(((EFI_ACPI_SDT_HEADER*)AmiBoardInfo2Protocol->BrdAcpiInfo)->Length); 
	if(*Dsdt2 == NULL)
    {
		ASSERT (FALSE);
    	return EFI_OUT_OF_RESOURCES;
    }   
    
    gBS->CopyMem(*Dsdt2, AmiBoardInfo2Protocol->BrdAcpiInfo,
                 ((EFI_ACPI_SDT_HEADER*)AmiBoardInfo2Protocol->BrdAcpiInfo)->Length);

    if  (*Dsdt2 == NULL)
    {
    	DEBUG((EFI_D_ERROR,"Acpi: No DSDT was FOUND: Status=EFI_NOT_FOUND\n"));
        return EFI_NOT_FOUND;
    }
    PrepareHdr20(DSDT_SIG, (EFI_ACPI_SDT_HEADER*)(*Dsdt2),2);
    return EFI_SUCCESS;
    
}// end of GetDsdtFv -----------------------------------------------
#endif


/**
    This function updates AML objects with values provided in ACPI_AML_UPD_INFO
    structure

         
    @param AmlUpdInfo pointer to ACPI_AML_UPD_INFO structure

          
    @retval EFI_SUCCESS AML objects updated successfully
    @retval EFI_ERROR some error occured during update process

**/

EFI_STATUS UpdateAml(ACPI_AML_UPD_INFO  *AmlUpdInfo)
{
    UINTN           i;
    EFI_ACPI_SDT_HEADER    	*Dsdt;
    EFI_STATUS      Status = EFI_SUCCESS;
    UINT8           *SxPointer = NULL;
    
//-------------------------

    for (i = 0; i < 2; i++)
    {
        if (i) Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)AmlUpdInfo->Dsdt1Addr;
        else Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)AmlUpdInfo->Dsdt2Addr;
        
        if (Dsdt)
        {
#if ATAD_SUPPORT == 1
        	if (AtadBuffPtr)
        	{
        		DEBUG((EFI_D_INFO, "AtadBuffPtr=0x%lX ", (UINT64)AtadBuffPtr));
        		Status = AmiUpdateAmlObjectValue(Dsdt, "ATBF", (UINT64)AtadBuffPtr);
        		ASSERT_EFI_ERROR (Status);
        		if (!EFI_ERROR(Status))
        			Status = AmiUpdateAmlObjectValue(Dsdt, "BUFU", 1);
        		ASSERT_EFI_ERROR (Status);
        	}
#endif
        	if (AmlUpdInfo->SS3 == 0)
        	{
        		SxPointer=AmiFindAmlObjectByName((UINT8*)Dsdt+sizeof(EFI_ACPI_SDT_HEADER),"_S3",Dsdt->Length-sizeof(EFI_ACPI_SDT_HEADER)-1);
            	if (SxPointer!=NULL) SxPointer[0]='X';
        	}
        	if (AmlUpdInfo->SS1 == 0)
        	{
        		SxPointer=AmiFindAmlObjectByName((UINT8*)Dsdt+sizeof(EFI_ACPI_SDT_HEADER),"_S1",Dsdt->Length-sizeof(EFI_ACPI_SDT_HEADER)-1);
        	    if (SxPointer!=NULL) SxPointer[0]='X';
        	}
        	if (AmlUpdInfo->SS4 == 0)
        	{
        		SxPointer=AmiFindAmlObjectByName((UINT8*)Dsdt+sizeof(EFI_ACPI_SDT_HEADER),"_S4",Dsdt->Length-sizeof(EFI_ACPI_SDT_HEADER)-1);
        	    if (SxPointer!=NULL) SxPointer[0]='X';
        	}
            //Update TOPM Object
            Status = AmiUpdateAmlObjectValue(Dsdt, "TOPM", AmlUpdInfo->TopOfMemory);
            ASSERT_EFI_ERROR (Status);
            
            if (EFI_ERROR(Status)) return Status;

            //Update IOST Object
#if GenericSio_SUPPORT
            Status = AmiUpdateAmlObjectValue(Dsdt, "IOST", (UINT64)AmlUpdInfo->SioDevStatusVar.DEV_STATUS);
            ASSERT_EFI_ERROR(Status);
            
            if (EFI_ERROR(Status)) return Status;
#endif
            Status = AmiUpdateAmlObjectValue(Dsdt, "SS4", (UINT64)AmlUpdInfo->SS4);
            ASSERT_EFI_ERROR(Status);
            
            if (EFI_ERROR(Status)) return Status;
            
            Status = AmiUpdateAmlObjectValue(Dsdt, "SS3", (UINT64)AmlUpdInfo->SS3);
            ASSERT_EFI_ERROR(Status);
            
            if (EFI_ERROR(Status)) return Status;
            
            Status = AmiUpdateAmlObjectValue(Dsdt, "SS2", (UINT64)AmlUpdInfo->SS2);
            ASSERT_EFI_ERROR(Status);
            
            if (EFI_ERROR(Status)) return Status;
            
            Status = AmiUpdateAmlObjectValue(Dsdt, "SS1", (UINT64)AmlUpdInfo->SS1);
            ASSERT_EFI_ERROR(Status);
            
            if (EFI_ERROR(Status)) return Status;

            Dsdt->Checksum = 0;
            Dsdt->Checksum = ChsumTable((UINT8*)Dsdt, Dsdt->Length);
            
            gxFacs->HardwareSignature = Dsdt->Checksum;    
        }
    }
    
    return Status;
    
    
}//end of UpdateAml

/**
    This function Hides Legacy Resources from OS by destroing _PRS method
    in each Legacy Device ASL Object in DSDT

         
    @param AmlUpdInfo pointer to ACPI_AML_UPD_INFO structure

          
    @retval VOID

**/
#if GenericSio_SUPPORT && AMI_SIO_MINOR_VERSION < 11
VOID LockLegacyRes (ACPI_AML_UPD_INFO   *AmlUpdInfo)

{

    UINTN               	HandleCnt, j, i;
    EFI_HANDLE          	*HandleBuffer = NULL;
    SIO_DEV2            	*SpIoDev;
    AMI_AML_OBJECT_INFO     AslObj={0};
    EFI_ACPI_SDT_HEADER 	*Dsdt;
    EFI_STATUS          	Status;
    EFI_SIO_DATA        	*EfiSioData;

    Status = gBS->LocateHandleBuffer(ByProtocol,&gEfiSioProtocolGuid, NULL, &HandleCnt, &HandleBuffer);
    //Locate all handles for SIO
    
    if (HandleBuffer == NULL) return;
    
    for (j = 0; j < HandleCnt; j++)
    {
    	Status = gBS->HandleProtocol(HandleBuffer[j], &gEfiSioProtocolGuid, (VOID**)&EfiSioData);
        if (EFI_ERROR(Status)) continue;
        SpIoDev = EfiSioData->Owner;
        if (SpIoDev->DeviceInfo->AslName[0])
        DEBUG ((EFI_D_INFO,"Found SIO Protocol. Name: %s\n",(UINT32)SpIoDev->DeviceInfo->AslName));
            // If this device has ASL Name and is present in DSDT
        {
            for (i = 0; i < 2; i++)
            {
                if (i) Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)AmlUpdInfo->Dsdt1Addr;
                else Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)AmlUpdInfo->Dsdt2Addr;
                
                if (Dsdt)
                {
                    DEBUG ((EFI_D_INFO,"Looking DSDT for Name: %s\n", SpIoDev->DeviceInfo->AslName));
                    Status = AmiGetAmlObject((UINT8*)Dsdt+sizeof(EFI_ACPI_SDT_HEADER),
                                       Dsdt->Length-sizeof(EFI_ACPI_SDT_HEADER)-1, (CHAR8*)&SpIoDev->DeviceInfo->AslName[0],
                                       AmiAmlObjTypeDevice, &AslObj);
                    // Get Asl object associated with this Legacy device
                    ASSERT_EFI_ERROR(Status);
                    
                    DEBUG ((EFI_D_INFO,"Going to hide object Data statr= %x, Length= %x\n",AslObj.DataStart, AslObj.Length));
                    if (!EFI_ERROR(Status)) AmiHideAmlMethodFromOs (&AslObj, "_PRS");
                    
                    // Lock this Device by destroing _PRS method of this object
                }
            }
        }
    }
    
    for (i = 0; i < 2; i++)
    {
        if (i) Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)AmlUpdInfo->Dsdt1Addr;
        else Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)AmlUpdInfo->Dsdt2Addr;
        
        if (Dsdt)
        {
            Dsdt->Checksum = 0;
            Dsdt->Checksum = ChsumTable((UINT8*)Dsdt, Dsdt->Length);
        }
    }
    
    gBS->FreePool(HandleBuffer);
}
#endif
/**
    This function will be called when ReadyToBoot event will be signaled and
    will update IO devices status and then update AML binary. It allso publish all
    ACPI tables.

         
    @param Event signalled event
    @param Context calling context

          
    @retval VOID

**/

VOID EFIAPI CollectAmlUpdInfo(EFI_EVENT Event, VOID *Context)
{
    ACPI_AML_UPD_INFO   *Aui=Context;
    EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE     *xFacp = NULL;
    EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE     *Facp = NULL;
    UINTN               i;
    EFI_STATUS          Status = EFI_SUCCESS;
    SETUP_DATA          *SetupData = NULL;
#if GenericSio_SUPPORT
    UINTN SioDevStatusVarSize = sizeof(SIO_DEV_STATUS);
    EFI_GUID SioDevStatusVarGuid = SIO_DEV_STATUS_VAR_GUID;
#endif
    ACPI_RESET_STRUCTURE 		AcpiResetVar = {0,0xff,0};
   	UINTN           TableKey;
   	EFI_ACPI_SDT_HEADER    *Table;
   	EFI_ACPI_TABLE_VERSION Version;
//-------------------------------------------
    //Init Setup Sleep States with Default Values;
    DEBUG((EFI_D_INFO,"IN Collect AML Info.\n"));
    
    //we need to recreate MADT table instances on READY_TO_BOOT event.
    
    for (i=0; ;i++)
    {
    	Status = mAcpiSdtProtocol->GetAcpiTable ( i, &Table, &Version, &TableKey);
    	if (Status == EFI_NOT_FOUND) break;
        //-------------------------------------------
        
        if (Table->Signature == FACP_SIG)
        {
            if (Version == EFI_ACPI_TABLE_VERSION_1_0B)
                Facp = (EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE*) Table; // Find FACP for V 1.1
            else
                xFacp = (EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE*) Table; // Find FACP for V 2+
        }
        
        if (Table->Signature == DSDT_SIG)
        {
            if (Version == EFI_ACPI_TABLE_VERSION_1_0B)
                Aui->Dsdt1Addr = (UINT64) (UINTN) Table; // Find DSDT for V 1.1
            else
                Aui->Dsdt2Addr = (UINT64) (UINTN) Table; // Find DSDT for V 2+
        }

 

    }
    
#if ACPI_APIC_TBL == 1
    if (MadtBuild){
    	Status = BuildMadtAll (4, &Table);
    	ASSERT_EFI_ERROR(Status);
       
    	Status = mAcpiTableProtocol->InstallAcpiTable (
                                             mAcpiTableProtocol,
                                             (VOID*)Table,
                                             Table->Length,
                                             &TableKey
                                             );
    	ASSERT_EFI_ERROR(Status);
    	gBS->FreePool(Table);
    	MadtBuild = FALSE; // We are not closing this event, so lets prevent multiple MADTs installation

    }
#endif  

    DEBUG((EFI_D_INFO,"IN Collect AML Info after BuildMadtAll %r\n",Status));
    
    Aui->SS1=DEFAULT_SS1;
    Aui->SS2=DEFAULT_SS2;
    Aui->SS3=DEFAULT_SS3;
    Aui->SS4=DEFAULT_SS4;
    
    i = 2; // Using i as var size.
    Status = gRT->GetVariable(L"Setup",&gSetupGuid,NULL,&i,&SetupData);

    if (Status==EFI_BUFFER_TOO_SMALL)
    {
    	SetupData=(SETUP_DATA*)AllocateZeroPool(i);
    	if (SetupData != NULL)
    		Status = gRT->GetVariable(L"Setup",&gSetupGuid,NULL,&i,SetupData);
    }
    ASSERT_EFI_ERROR(Status);

    if (!EFI_ERROR(Status))
    {
        //Such Variable exists so use customer choices
        if (!SetupData->AcpiAuto)
        {
            Aui->SS4=SetupData->AcpiHibernate;
            
            switch (SetupData->AcpiSleepState)
            {
                case 0:
                    Aui->SS3=0;
                    Aui->SS1=0;
                    break;
                case 1:
                    Aui->SS1=1;
                    Aui->SS3=0;
                    break;
                case 2:
                    Aui->SS3=1;
#if A_S1 == 1
                    Aui->SS1=1;
#else
                    Aui->SS1=0;
#endif
                    break;
            }
        }
    }
    
    gAcpiIaBootArch &= (~((UINT16)(EFI_ACPI_2_0_LEGACY_DEVICES | EFI_ACPI_2_0_8042)));// clear first 2 bits of gAcpiIaBootArch,
    // while preserving others. This 2 bits will be updated later.
#if GenericSio_SUPPORT
    Status = gRT->GetVariable(SIO_DEV_STATUS_VAR_NAME, &SioDevStatusVarGuid, NULL,
                              &SioDevStatusVarSize, &Aui->SioDevStatusVar.DEV_STATUS);
                              
    if (EFI_ERROR(Status)) Aui->SioDevStatusVar.DEV_STATUS = 0;

    
    //Dynamically update IA_PC_BOOT_ARCHITECTURE flag based on SIO_DEV_STATUS_VAR
    if (
        (Aui->SioDevStatusVar.SerialA == 1 ) ||
        (Aui->SioDevStatusVar.SerialB == 1 ) ||
        (Aui->SioDevStatusVar.Lpt == 1 ) ||
        (Aui->SioDevStatusVar.Fdd == 1 ) ||
        (Aui->SioDevStatusVar.Game1 == 1 ) ||
        (Aui->SioDevStatusVar.Game2 == 1 )
    )
        gAcpiIaBootArch |= EFI_ACPI_2_0_LEGACY_DEVICES;
        
    if (
        (Aui->SioDevStatusVar.Key60_64 == 1) ||
        (Aui->SioDevStatusVar.Ps2Mouse == 1) ||
        (Aui->SioDevStatusVar.Ec62_66 == 1)
    )
        gAcpiIaBootArch |= EFI_ACPI_2_0_8042;        
#endif  
    
    if ( xFacp != NULL)
    {
    	if (xFacp->XFirmwareCtrl)
    		gxFacs = (EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE*)(UINTN)xFacp->XFirmwareCtrl;
    	else
    		gxFacs = (EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE*)(UINTN)xFacp->FirmwareCtrl;
    	
        xFacp->IaPcBootArch &= gAcpiIaBootArch;
        xFacp->Header.Checksum = 0;
        xFacp->Header.Checksum = ChsumTable((UINT8*)&xFacp->Header, xFacp->Header.Length);
        
        if (xFacp->XPm1aCntBlk.Address !=0)
        {
        	AcpiResetVar.Addess = xFacp->XPm1aCntBlk.Address;
        	AcpiResetVar.AccessSize = xFacp->XPm1aCntBlk.AccessSize;
        	AcpiResetVar.Type = xFacp->XPm1aCntBlk.AddressSpaceId;		
        }
        else
        if (xFacp->Pm1aCntBlk !=0)
        {
        	AcpiResetVar.Addess = (UINT64)(xFacp->Pm1aCntBlk);
        	AcpiResetVar.AccessSize = 2;
        	AcpiResetVar.Type = 1;		
        }	
 
    }
    
    if ( Facp != NULL )
    {
    	if (Facp->XFirmwareCtrl)
    		gxFacs = (EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE*)(UINTN)Facp->XFirmwareCtrl;
    	else
    	    gxFacs = (EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE*)(UINTN)Facp->FirmwareCtrl;
    	
        //ACPI v1.0b don't have IAPC_BOOT_ARCH flag field!
        //Legacy free extension has but.. 1.1b Legacy free extension has. REV==2 and higher
        if (Facp->Header.Revision > ACPI_REV1)
        {
            Facp->IaPcBootArch &= gAcpiIaBootArch;
            Facp->Header.Checksum = 0;
            Facp->Header.Checksum = ChsumTable((UINT8*)&Facp->Header, Facp->Header.Length);
        }
        if ((xFacp == NULL) || (AcpiResetVar.Type == 0xff))
        {
        	if (Facp->Pm1aCntBlk !=0)
        	{
        		AcpiResetVar.Addess = (UINT64)(Facp->Pm1aCntBlk);
        		AcpiResetVar.AccessSize = 2;
        		AcpiResetVar.Type = 1;		
        	}	
        }
    }
    if (AcpiResetVar.Type != 0xff)
    	Status = gRT->SetVariable(
                 L"AcpiResetVar",
                 &gAmiGlobalVariableGuid,
                 EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                 sizeof(ACPI_RESET_STRUCTURE),
                 &AcpiResetVar
             	 );
    DEBUG((EFI_D_INFO,"AcpiResetVar: Status=%r Addess=%lx AccessSize=%x Type=%x \n", Status, AcpiResetVar.Addess, AcpiResetVar.AccessSize, AcpiResetVar.Type));    
    ASSERT_EFI_ERROR(Status);
#if GenericSio_SUPPORT && AMI_SIO_MINOR_VERSION < 11
    if (SetupData != NULL) {
    	if (SetupData->AcpiLockLegacyRes) LockLegacyRes (Aui); // LockLegacyDev
    }
#endif    
    Status=UpdateAml(Aui);
    ASSERT_EFI_ERROR(Status);
    // Do not need to close event because we can return here with changies in configuration
}// end of CollectAmlUpdInfo

/**
    This function builds mandatory ACPI tables

         
    @param VOID

          
    @retval EFI_SUCCESS Function executed successfully, ACPI_SUPPORT_PROTOCOL installed
    @retval EFI_ABORTED Dsdt table not found or table publishing failed
    @retval EFI_ALREADY_STARTED driver already started
    @retval EFI_OUT_OF_RESOURCES not enough memory to perform operation

    @note  
  This function also creates ReadyToBoot event to update AML objects before booting

**/

EFI_STATUS BuildMandatoryAcpiTbls ()
{
    EFI_STATUS      Status = EFI_SUCCESS;
    UINTN           AcpiVer;
    EFI_ACPI_TABLE_VERSION  EfiAcpiVer;
	EFI_ACPI_SDT_HEADER        *TblPtr = NULL;
	UINTN           TableKey;
#if ATAD_SUPPORT == 1
	EFI_GUID    AtadSmiGuid = ATAD_SMI_GUID;
	UINTN AtadVarSize = sizeof(AtadBuffPtr);
#endif
#if DSDT_BUILD
    EFI_ACPI_SDT_HEADER        *Dsdt2Ptr = NULL;
#endif	
//------------------------
  
    AcpiVer = 2;
    EfiAcpiVer = EFI_ACPI_TABLE_VERSION_2_0;
    
    if (ACPI_SPEC_VERSION >= 30)
    {
        AcpiVer = 3;
        EfiAcpiVer = EFI_ACPI_TABLE_VERSION_3_0;
    }
    
    if (ACPI_SPEC_VERSION >= 40)
    {
        AcpiVer = 4;
        EfiAcpiVer = EFI_ACPI_TABLE_VERSION_4_0;
    }
#if ATAD_SUPPORT == 1

    Status = gBS->AllocatePool(EfiRuntimeServicesData, 4, &AtadBuffPtr);
    if (!EFI_ERROR(Status) && AtadBuffPtr)
    {
        Status = gRT->SetVariable ( L"AtadSmiBuffer",
        							&AtadSmiGuid,
        							EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        							AtadVarSize,
                                    &AtadBuffPtr );
        ASSERT_EFI_ERROR(Status);
    }

#endif
    PcdSet64 (PcdAcpiDefaultOemTableId, *(UINT64*)&ACPI_OEM_TBL_ID[0]);
  

    if (EFI_ERROR (Status)) return EFI_OUT_OF_RESOURCES;
#if FACP_BUILD  
    
        Status = BuildFacpAll (AcpiVer, &TblPtr);
        ASSERT_EFI_ERROR(Status);
    
        if (EFI_ERROR (Status)) return EFI_OUT_OF_RESOURCES;
        
        Status = mAcpiTableProtocol->InstallAcpiTable (
                                       mAcpiTableProtocol,
                                       (VOID*)TblPtr,
                                       TblPtr->Length,
                                       &TableKey
                                       );
        ASSERT_EFI_ERROR(Status);
        gBS->FreePool(TblPtr);
        if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
    


    DEBUG((EFI_D_INFO,"IN ACPI 1: %r\n", Status));
#endif  
#if DSDT_BUILD
    Status = GetDsdtFv(&Dsdt2Ptr);
    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) return EFI_ABORTED;
    
    if (Dsdt2Ptr != NULL)
    {
    	DEBUG((EFI_D_INFO,"DSDT21 addres 0x%lX; -> %r \n", Dsdt2Ptr, Status));
        Dsdt2Ptr->Checksum = 0;
        Dsdt2Ptr->Checksum = ChsumTable((UINT8*)Dsdt2Ptr, Dsdt2Ptr->Length);
        Status = mAcpiTableProtocol->InstallAcpiTable (
                                               mAcpiTableProtocol,
                                               (VOID*)Dsdt2Ptr,
                                               Dsdt2Ptr->Length,
                                               &TableKey
                                               );
        ASSERT_EFI_ERROR(Status);
        gBS->FreePool(Dsdt2Ptr);
        if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
        
    }
    else return EFI_ABORTED;
    
#endif
        
    DEBUG((EFI_D_INFO,"IN ACPI 3: %r\n", Status));
    Status = BuildFacs (&TblPtr);
    ASSERT_EFI_ERROR(Status);
             
    Status = mAcpiTableProtocol->InstallAcpiTable (
                                                   mAcpiTableProtocol,
                                                   (VOID*)TblPtr,
                                                   TblPtr->Length,
                                                   &TableKey
                                                   );
    ASSERT_EFI_ERROR(Status);
    gBS->FreePool(TblPtr);
    if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;

    return EFI_SUCCESS;
}

/**
    This function craeate Ready to Boot event

         
    @param VOID

    @retval 
        EFI_STATUS, based on result


**/

EFI_STATUS AcpiReadyToBootEvent() 
{
	EFI_EVENT               EvtReadyToBoot;
	
    gAuiGlob = AllocateZeroPool(sizeof(ACPI_AML_UPD_INFO));
    
    if (gAuiGlob == NULL) 
    	return EFI_OUT_OF_RESOURCES;
    
    return  EfiCreateEventReadyToBootEx (
                 TPL_CALLBACK, CollectAmlUpdInfo, gAuiGlob, &EvtReadyToBoot
             );
}   


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             5555 Oakbrook Pkwy, Norcross, GA 30093               **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
