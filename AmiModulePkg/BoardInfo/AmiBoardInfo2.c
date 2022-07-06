//**********************************************************************
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//**********************************************************************
/** 
    @file AmiBoardInfo2.c
        Stores AMI Porting information (PCI; SIO; APIC) and Publishes AMI Board Info Protocol
**/
//**********************************************************************
#include <Token.h>
#include <Protocol/AmiBoardInfo2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/AmiSdlLib.h>
#include <AmiDxeLib.h> // Needed for AmiSdlLib - must be removed after AmiSdlLib update
//----------------------------------------------------------------------------
//Global GUIDs Definitions goes here
EFI_GUID    gAmiBoardInfo2Guid=AMI_BOARD_INFO2_PROTOCOL_GUID;

extern UINT8 AmlCode[];

/**
    Ami Board Info 2 Protocol Instance.

**/
AMI_BOARD_INFO2_PROTOCOL gAmiBoard2Protocol = {
	///UINTN					BoardInfoVersion
	5,						//APTIO 5 version
	///BOOLEAN                 DataValid;      ///Signifies if Board Specific IRQ data was updated.
    FALSE,
    ///UINT8                   Reserved[7];
    {0,0,0,0,0,0,0},
    ///AMI_SDL_TBL_HEADER      *PciBrdData;
	NULL,
    ///AMI_SDL_TBL_HEADER      *SioBrdData;
	NULL,
    ///AMI_APIC_INFO           *ApicBrdData;
	NULL,
    ///VOID                    *BrdAcpiInfo;
    NULL,
	///UINTN                   BrdAcpiLength;
    0,
    ///VOID					*BrdAcpiIrqInfo; //Optional
    NULL,
    ///VOID					*BrdSetupInfo;
    NULL,
    ///VOID                    *BoardOemInfo;
    NULL,
    ///PCI_IRQ_PIC_ROUTE       *PicRoutTable;
    NULL,
    ///UINTN                   PicRoutLength;
    0,
    ///PCI_IRQ_APIC_ROUTE      *ApicRoutTable;
    NULL,
    ///UINTN                   ApicRoutLength;
    0
};

UINT8 *gSdlDataStart=NULL;
UINTN  gSdlDataLen=0;
UINT8 *gDsdtDataStart=NULL;
UINTN  gDsdtDataLen=0;

/**
    Ami Board Info Protocol Handle.

**/
EFI_HANDLE gAmiBoardInfo2Handle=NULL;


//This GUIDs coming from Autogen.c in build folder generated based on data from AmiModulePkg.dec and this *.inf
extern EFI_GUID gAmiBoardInfoFileGuid;      // = AMI_BOARD_INFO_OUT_FILE_GUID;
extern EFI_GUID gAmiBoardInfoSectionGuid;   // = AMI_BOARD_INFO_OUT_SECTION_GUID;
extern EFI_GUID gAmiAslDsdtFileGuid;        // = AMI_ASL_DSDT_OUT_FILE_GUID;



/**
    This Parses SDL Data Table Buffer and returnd pointer to the data table 
    with given "Signature"

         
    @param DataPtr Pointer to the SDL Data Buffer. 
    @param DataLen Total Data Buffer Length in bytes.
    @param Signature Total Data Buffer Length in bytes.
    @param TblPtr Pointer to the data table with "Signature". 
          
    @retval EFI_SUCCESS Function executed successfully,
    @retval EFI_ALREADY_STARTED driver already started
    @retval EFI_OUT_OF_RESOURCES not enough memory to perform operation

**/
EFI_STATUS ParseAmiSdlData(UINT8 *DataPtr, UINTN DataLen, UINT64 Signature, OUT VOID **TblPtr){
    UINT8   *start=DataPtr;
    UINT64  *sig;
    UINTN   i;
//-----------------
    if(start==NULL || TblPtr==NULL || DataLen==0 ) return EFI_INVALID_PARAMETER;

    for(i=0; i<DataLen-sizeof(UINT64); i++){
        sig=(UINT64*)(&start[i]);
        if(*sig == Signature) {
            *TblPtr=sig;
            return EFI_SUCCESS;
        }
    }
    return EFI_NOT_FOUND;
}

/**
    This function is AMI Board Info driver entry point

         
    @param ImageHandle Image handle
    @param SystemTable pointer to system table

          
    @retval EFI_SUCCESS Function executed successfully,
    @retval EFI_ALREADY_STARTED driver already started
    @retval EFI_OUT_OF_RESOURCES not enough memory to perform operation

**/
EFI_STATUS EFIAPI AmiBoardEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                  Status;
    AMI_BOARD_INFO2_PROTOCOL    *AmiBrd=NULL;
    VOID                        *DataPtr=NULL;
//--------------------------------
    DEBUG((DEBUG_ERROR,"AmiBrdInfo1 \n"));
    InitAmiLib(ImageHandle,SystemTable); // Needed for AmiSdlLib - must be removed after AmiSdlLib update
    
	//First try to check if we are running on MultiPlatfortm Enabled BIOS
    Status=gBS->LocateProtocol(&gAmiBoardInfo2Guid, NULL, (VOID**)&AmiBrd);
    if(!EFI_ERROR(Status)){
        DEBUG((DEBUG_ERROR,"AmiBrdInfo: Multi-Platform BrdInfo present Status=%r. Exiting.\n",Status));
        return Status;
    }
    DEBUG((DEBUG_ERROR,"AmiBrdInfo2 \n"));
    AmiBrd=&gAmiBoard2Protocol;
    DEBUG((DEBUG_ERROR,"AmiBrdInfo22 \n"));
    Status=AmiSdlReadFfsSdlData(&gSdlDataStart, &gSdlDataLen, &gAmiBoardInfoFileGuid, &gAmiBoardInfoSectionGuid);
    DEBUG((DEBUG_ERROR,"AmiBrdInfo23 \n"));
    ASSERT_EFI_ERROR(Status);
    DEBUG((DEBUG_ERROR,"AmiBrdInfo24 \n"));
    if(EFI_ERROR(Status))return Status;
    DEBUG((DEBUG_ERROR,"AmiBrdInfo3 \n"));

    //Init Ami Platform Info Protocol Instance...

    //1. First get PCI Data pointer AMI_PCI_DATA_SIG $PCIDATA
    Status=ParseAmiSdlData(gSdlDataStart,gSdlDataLen,AMI_PCI_DATA_SIG,&DataPtr);
    //It must be there! if it is not - ASSERT HERE.
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;
    AmiBrd->PciBrdData=(AMI_SDL_PCI_DATA*)DataPtr;

    DEBUG((DEBUG_ERROR,"AmiBrdInfo4 \n"));
    //2. Then Get Dsdt It mus be there
    //Take care if ACPI_SUPPORT IS OFF we will not have a DSDT table in ROM
    Status=AmiSdlReadFfsSdlData(&gDsdtDataStart, &gDsdtDataLen, &gAmiAslDsdtFileGuid, NULL);
    //ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)){
        DEBUG((DEBUG_ERROR,"\n======================================================================================\n"));
        DEBUG((DEBUG_ERROR,"AmiBrdInfo: !!!ERROR!!! Can't find DSDT Table in BIOS FV - %r.\n            !!!ERROR!!! Check your project ACPI settings...\n",Status));
        DEBUG((DEBUG_ERROR,"======================================================================================\n\n"));
	} else {
    	AmiBrd->BrdAcpiInfo=gDsdtDataStart;
	    AmiBrd->BrdAcpiLength=gDsdtDataLen;
	}
    DEBUG((DEBUG_ERROR,"AmiBrdInfo5 \n"));
    //3. Than get SIO Data Pointer...AMI_SIO_DATA_SIG $SIODATA
    DataPtr=NULL;
    Status=ParseAmiSdlData(gSdlDataStart,gSdlDataLen,AMI_SIO_DATA_SIG,&DataPtr);

    //Not A BIG deal if we can't find SIO Device. System Might be a LEGACY FREE.
    //not need to assert here //ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)){
        DEBUG((DEBUG_ERROR,"\n======================================================================================\n"));
        DEBUG((DEBUG_ERROR,"AmiBrdInfo: !!!ERROR!!! Can't find SIO Data Table in BIOS FV - %r.\n            !!!ERROR!!! Check your project SIO settings...\n",Status));
        DEBUG((DEBUG_ERROR,"======================================================================================\n\n"));
	} else 
		AmiBrd->SioBrdData=(AMI_SDL_SIO_DATA*)DataPtr;
    DEBUG((DEBUG_ERROR,"AmiBrdInfo6 \n"));
    //3. Get IoApic Data Pointer...AMI_APIC_DATA_SIG  $APIDATA
    DataPtr=NULL;
    Status=ParseAmiSdlData(gSdlDataStart,gSdlDataLen,AMI_APIC_DATA_SIG,&DataPtr);
    //Not A BIG deal if we can't find SIO Device. System Might be a LEGACY FREE.
    //not need to assert here //ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)){
            DEBUG((DEBUG_ERROR,"\n======================================================================================\n"));
            DEBUG((DEBUG_ERROR,"AmiBrdInfo: !!!ERROR!!! Can't find IOAPIC Data Table in BIOS FV - %r.\n            !!!ERROR!!! Check your project APIC and ROUTER settings...\n",Status));
            DEBUG((DEBUG_ERROR,"======================================================================================\n\n"));
	} else 
		AmiBrd->ApicBrdData=(AMI_SDL_APIC_DATA*)DataPtr;
        DEBUG((DEBUG_ERROR,"AmiBrdInfo7 \n"));

    //Now install AMI BOARD INFO 2 Protocol...
    Status=gBS->InstallMultipleProtocolInterfaces(
        &gAmiBoardInfo2Handle,
        &gAmiBoardInfo2Guid, &gAmiBoard2Protocol, NULL
    );
    DEBUG((DEBUG_ERROR,"AmiBrdInfo8 \n"));
	return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
