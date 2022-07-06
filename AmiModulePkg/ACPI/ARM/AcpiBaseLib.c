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


/** @file ARM/AcpiBaseLib.c
    Main ACPI Driver File. It has ACPI Driver entry point,
    ACPISupport Protocol and ACPITable Protocol.

**/
//**********************************************************************

#include <Token.h>
#include "AcpiCore.h"
#include <Protocol/Cpu.h>
#if DSDT_BUILD
#include <Protocol/AmiBoardInfo2.h>
#endif
#include <AcpiOemElinks.h>
#include <Guid/ArmMpCoreInfo.h>
#include<IndustryStandard/Acpi60.h>
#include <Library/PcdLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

//--------------------------------------
//Some Global vars
EFI_ACPI_3_0_FIRMWARE_ACPI_CONTROL_STRUCTURE                 *gxFacs;

UINT8    ACPI_OEM_ID[6]     = ACPI_OEM_ID_MAK;     
UINT8    ACPI_OEM_TBL_ID[8] = ACPI_OEM_TBL_ID_MAK; 

extern EFI_ACPI_TABLE_PROTOCOL  *mAcpiTableProtocol;
extern EFI_ACPI_SDT_PROTOCOL    *mAcpiSdtProtocol;

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
    This function allocates memory for and fills FACP table v 1.1+ with
    predefined values from SDL tokens
         
    @param TablVer version of FACP table
    @param TablPtr pointer to memory, where the FACP table will resides.
        	Filled by this procedure
    @retval 
  EFI_OUT_OF_RESOURCES - Memory for the table could not be allocated
  EFI_SUCCESS - Table was successfully build

**/

#if ARM_FACP_BUILD 
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

#if GTDT_BUILD
/**
    This function allocates memory for and fills GTDT struscure.
         
    @param TablVer Version of GTDT table
    @param TablPtr - Pointer to memory, where the GTDT table will resides.
        	Filled by this procedure
    @retval 
  EFI_OUT_OF_RESOURCES - Memory for the table could not be allocated
  EFI_SUCCESS - Table was successfully build

**/

EFI_STATUS BuildGTDT (IN UINTN TablVer, OUT EFI_ACPI_SDT_HEADER **TablPtr)

{
    EFI_ACPI_5_0_GENERIC_TIMER_DESCRIPTION_TABLE    *GTDT;
    UINTN	i;

    if (TablVer<1 || TablVer>4) return EFI_INVALID_PARAMETER;
    *TablPtr = AllocateZeroPool (sizeof(EFI_ACPI_5_0_GENERIC_TIMER_DESCRIPTION_TABLE));

    if ((*TablPtr)==NULL)
    {
        ASSERT(*TablPtr);
        return EFI_OUT_OF_RESOURCES;
    }

    GTDT = (EFI_ACPI_5_0_GENERIC_TIMER_DESCRIPTION_TABLE*)*TablPtr;
    GTDT->Header.Signature = EFI_ACPI_5_0_GENERIC_TIMER_DESCRIPTION_TABLE_SIGNATURE;
	GTDT->Header.Length = sizeof(EFI_ACPI_5_0_GENERIC_TIMER_DESCRIPTION_TABLE);
	GTDT->Header.Revision = EFI_ACPI_5_0_GENERIC_TIMER_DESCRIPTION_TABLE_REVISION;
	
	for (i=0; i<6; i++) GTDT->Header.OemId[i] = ACPI_OEM_ID[i];

	GTDT->Header.CreatorId = CREATOR_ID_AMI;
	GTDT->Header.CreatorRevision = CREATOR_REV_MS;
	
	//for (i=0; i<8; i++) GTDT->Header.OemTableId[i] = ACPI_OEM_TBL_ID[i];
	CopyMem (&GTDT->Header.OemTableId, &ACPI_OEM_TBL_ID, sizeof(ACPI_OEM_TBL_ID));

	GTDT->Header.OemRevision = 1;

	//Fill GTDT members
	GTDT->PhysicalAddress			= GTDT_PHYSICAL_ADDRESS; 
	GTDT->GlobalFlags				= GTDT_GLOBAL_FLAGS; 
	GTDT->SecurePL1TimerGSIV		= GTDT_SECURE_PL1_TIMER_GSIV; 
	GTDT->SecurePL1TimerFlags		= GTDT_SECURE_PL1_TIMER_FLAGS; 
	GTDT->NonSecurePL1TimerGSIV		= GTDT_NON_SECURE_PL1_TIMER_GSIV; 
	GTDT->NonSecurePL1TimerFlags    = GTDT_NON_SECURE_PL1_TIMER_FLAGS;
	GTDT->VirtualTimerGSIV			= GTDT_VIRTUAL_TIMER_GSIV;
	GTDT->VirtualTimerFlags			= GTDT_VIRTUAL_TIMER_FLAGS;
	GTDT->NonSecurePL2TimerGSIV		= GTDT_NON_SECURE_PL2_TIMER_GSIV;
	GTDT->NonSecurePL2TimerFlags    = GTDT_NON_SECURE_PL2_TIMER_FLAGS;
	
	GTDT->Header.Checksum = ChsumTable((UINT8*)GTDT, GTDT->Header.Length);

    return EFI_SUCCESS;
}
#endif
#if ARM_MADT_BUILD

EFI_ACPI_5_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER		*Hdr;
EFI_ACPI_5_0_GIC_STRUCTURE      						*Gic;
EFI_ACPI_5_0_GIC_DISTRIBUTOR_STRUCTURE 					*GicDistributor;

/**
    This function allocates memory for and fills MADT structure.
         
    @param TablVer Version of MADT table
    @param TablPtr Pointer to memory, where the MADT table will resides.
        	Filled by this procedure
    @retval 
  EFI_OUT_OF_RESOURCES - Memory for the table could not be allocated
  EFI_SUCCESS - Table was successfully build

**/

EFI_STATUS BuildMADT (IN UINTN TablVer, OUT EFI_ACPI_SDT_HEADER **TablPtr)

{
    UINT8 					i;
    UINTN					Index;
    ARM_PROCESSOR_TABLE   	*ArmProcessorTable;
	ARM_CORE_INFO    	    *ArmCoreInfoTable;
	UINT8					*MADT;
	
	for (Index=0; Index < pST->NumberOfTableEntries; Index++) {
		// Check for correct GUID type
		if (CompareGuid (&gArmMpCoreInfoGuid, &(pST->ConfigurationTable[Index].VendorGuid))) {
			// Get pointer to ARM processor table
			ArmProcessorTable = (ARM_PROCESSOR_TABLE *)pST->ConfigurationTable[Index].VendorTable;
			ArmCoreInfoTable = ArmProcessorTable->ArmCpus;
			break;
		}
	}

    if (TablVer<1 || TablVer>4) return EFI_INVALID_PARAMETER;
    
	*TablPtr = AllocateZeroPool (sizeof(EFI_ACPI_5_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER) + sizeof(EFI_ACPI_5_0_GIC_STRUCTURE) * ArmProcessorTable->NumberOfEntries + sizeof(EFI_ACPI_5_0_GIC_DISTRIBUTOR_STRUCTURE) );
    if ((*TablPtr)==NULL)
    {
        ASSERT(*TablPtr);
        return EFI_OUT_OF_RESOURCES;
    }

	MADT = (UINT8*)*TablPtr;	
    Hdr = (EFI_ACPI_5_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER*)MADT;
    PrepareHdr20(APIC_SIG, &(Hdr->Header), TablVer);
	Hdr->Header.Length = sizeof(EFI_ACPI_5_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER) + sizeof(EFI_ACPI_5_0_GIC_STRUCTURE) * ArmProcessorTable->NumberOfEntries + sizeof(EFI_ACPI_5_0_GIC_DISTRIBUTOR_STRUCTURE) ;
    Hdr->LocalApicAddress = 0;
    Hdr->Flags = 1;

	MADT = MADT + sizeof(EFI_ACPI_5_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER);
    //Fill GIC members
    for(i=0; i< ArmProcessorTable->NumberOfEntries; i++)
    {
		Gic = (EFI_ACPI_5_0_GIC_STRUCTURE *)MADT;
        Gic->Type = EFI_ACPI_5_0_GIC;
        Gic->Length = sizeof(EFI_ACPI_5_0_GIC_STRUCTURE);
        Gic->GicId = i;
        Gic->AcpiProcessorUid = i;
        Gic->Flags = ARM_MADT_GIC_FLAGS;
        Gic->ParkedAddress = ArmCoreInfoTable[i].MailboxSetAddress; 
        Gic->PhysicalBaseAddress = (UINT64) PcdGet32(PcdGicInterruptInterfaceBase);
		MADT = MADT + sizeof(EFI_ACPI_5_0_GIC_STRUCTURE);
    }

    //Fill GIC distrubutor
    GicDistributor = (EFI_ACPI_5_0_GIC_DISTRIBUTOR_STRUCTURE *)MADT;
    GicDistributor->Type = EFI_ACPI_5_0_GICD;
    GicDistributor->Length = sizeof(EFI_ACPI_5_0_GIC_DISTRIBUTOR_STRUCTURE);
    GicDistributor->GicId = ARM_MADT_GIC_DISTR_ID;
    GicDistributor->PhysicalBaseAddress = (UINT64) PcdGet32(PcdGicDistributorBase);
    
    Hdr->Header.Checksum = ChsumTable((UINT8*)*TablPtr, Hdr->Header.Length);
    
    return EFI_SUCCESS;
}
#endif
/**
    Allocates ACPI NVS memory and builds FACS table from values,
    defined by SDL tokens

    @param VOID



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
#if DSDT_BUILD
/**
    This function finds DSDT table in firmvare volume

         
    @param Dsdt1 pointer to memory where DSDT v1.1 table will be stored
    @param Dsdt2 pointer to memory where DSDT v2.0+ table will be stored

          
    @retval EFI_SUCCESS Function executed successfully
    @retval EFI_ABORTED ACPI storage file not found
    @retval EFI_NOT_FOUND DSDT table not found in firmware volume

**/

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
    This function builds mandatory ACPI tables

         
    @param VOID

          
    @retval EFI_SUCCESS Function executed successfully, ACPI_SUPPORT_PROTOCOL installed
    @retval EFI_ABORTED Dsdt table not found or table publishing failed
    @retval EFI_ALREADY_STARTED driver already started
    @retval EFI_OUT_OF_RESOURCES not enough memory to perform operation

**/

EFI_STATUS BuildMandatoryAcpiTbls ()
{
    EFI_STATUS      Status = EFI_SUCCESS;
    static EFI_GUID Acpisupguid = EFI_ACPI_SUPPORT_GUID;
    UINTN           AcpiVer;
    EFI_ACPI_TABLE_VERSION  EfiAcpiVer;
	EFI_ACPI_SDT_HEADER        *TblPtr = NULL;
	UINTN           TableKey;
#if DSDT_BUILD
    EFI_ACPI_SDT_HEADER        *Dsdt2Ptr = NULL;
#endif
//------------------------
  
	AcpiVer = 4;
    EfiAcpiVer = EFI_ACPI_TABLE_VERSION_4_0;

    PcdSet64 (PcdAcpiDefaultOemTableId, *(UINT64*)&ACPI_OEM_TBL_ID[0]);
    
    if (EFI_ERROR (Status)) return EFI_OUT_OF_RESOURCES;
#if ARM_FACP_BUILD    
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
    else 
    	return EFI_ABORTED;
    
#endif
 
//------ Generic Timer Description Table ---------------------------
#if GTDT_BUILD
    
    DEBUG((EFI_D_INFO,"IN ACPI 2: %r\n", Status));
    Status = BuildGTDT (AcpiVer, &TblPtr);
    
    if (!EFI_ERROR(Status)) 
    {
    	Status = mAcpiTableProtocol->InstallAcpiTable (
    	                                          mAcpiTableProtocol,
    	                                          (VOID*)TblPtr,
    	                                          TblPtr->Length,
    	                                          &TableKey
    	                                          );
    	 ASSERT_EFI_ERROR(Status);
    	 gBS->FreePool(TblPtr);
    	 if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
    }   
#endif
//------ Generic Timer Description Table End ------------------------

//------ Multiple APIC Description Table ---------------------------
#if ARM_MADT_BUILD
    
    
    DEBUG((EFI_D_INFO,"IN ACPI 3: %r\n", Status));
    Status = BuildMADT (AcpiVer, &TblPtr);
      
    if (!EFI_ERROR(Status)) 
    {
      	Status = mAcpiTableProtocol->InstallAcpiTable (
      	                                          mAcpiTableProtocol,
      	                                          (VOID*)TblPtr,
      	                                          TblPtr->Length,
      	                                          &TableKey
      	                                          );
      	 ASSERT_EFI_ERROR(Status);
      	 gBS->FreePool(TblPtr);
      	 if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;
    }   
#endif
    
    Status = BuildFacs (&TblPtr);
    ASSERT_EFI_ERROR(Status);
    if (!EFI_ERROR(Status)) 
    {             
        Status = mAcpiTableProtocol->InstallAcpiTable (
                                                       mAcpiTableProtocol,
                                                       (VOID*)TblPtr,
                                                       TblPtr->Length,
                                                       &TableKey
                                                       );
        ASSERT_EFI_ERROR(Status);
        gBS->FreePool(TblPtr);
    }
   
   return Status;
}
//------ Multiple APIC Description Table End ------------------------

/**
    This function craeate Ready to Boot event

         
    @param VOID

    @retval 
        EFI_STATUS, based on result


**/

EFI_STATUS AcpiReadyToBootEvent() 
{
    
    return  EFI_SUCCESS;
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
