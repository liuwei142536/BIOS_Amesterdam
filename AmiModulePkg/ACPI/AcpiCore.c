//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             5555 Oakbrook Pkwy, Norcross, GA 30093               **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************


/** @file AcpiCore.c
    Main ACPI Driver File. It has ACPI Driver entry point,
    ACPISupport Protocol and ACPITable Protocol.

**/
//**********************************************************************

#include "AcpiCore.h"
#include <Protocol/FirmwareVolume2.h>

///---- New -----


EFI_ACPI_TABLE_PROTOCOL  *mAcpiTableProtocol = NULL;
EFI_ACPI_SDT_PROTOCOL    *mAcpiSdtProtocol   = NULL;


#if UPDATE_WSMT
typedef struct {
    EFI_ACPI_DESCRIPTION_HEADER Header;
	UINT32			            ProtectionFlags;
} WSMT_ACPI_TABLE;
#endif

//--------------------------------------
//Some Global vars
ACPI_DB                 gAcpiData = {{0, 0, NULL}, 0};
#define ACPI_TABLES_FILE_GUID \
		{ 0x709e6472, 0x1bcd, 0x43bd, { 0x8b, 0x6b, 0xcd, 0x2d, 0x6d, 0x8, 0xb9, 0x67 } }

#if MPS_TABLE_SUPPORT == 1
EFI_STATUS MpsTableBuilderInit(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable);
#endif


//------------Move to header------------------------
EFI_STATUS BuildMandatoryAcpiTbls ();
EFI_STATUS AcpiReadyToBootEvent();


//------------Move to header------------------------

#if UPDATE_WSMT
void PrepareHdr20(UINT32 TblSig, EFI_ACPI_SDT_HEADER *HeaderPtr, UINTN Vers);
#endif

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ACPI SUPPORT PPROTOCOL function Implementation
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/**
    This function returns ACPI table

         
    @param This pointer to EFI_ACPI_SUPPORT_PROTOCOL instance
    @param Index Index of ACPI table to return
    @param Table Pointer where to place found table
    @param Version requested ACPI table version
    @param Handle requested ACPI table handle

          
    @retval EFI_SUCCESS Function executed successfully
    @retval EFI_OUT_OF_RESOURCES not enough memory to allocate table
    @retval EFI_INVALID_PARAMETER invalid EFI_ACPI_SUPPORT_PROTOCOL pointer
    @retval EFI_NOT_FOUND requested ACPI table not found

**/

EFI_STATUS EFIAPI AcpiSupportGetAcpiTable (
    IN EFI_ACPI_SUPPORT_PROTOCOL            *This,
    IN INTN                                 Index,
    OUT VOID                                **Table,
    OUT EFI_ACPI_TABLE_VERSION              *Version,
    OUT UINTN                               *Handle )
{

	EFI_ACPI_SDT_HEADER* LocalTable;
	EFI_STATUS Status;
//----------------------

    DEBUG((EFI_D_ERROR,"!!!WARNING!!! Deprecated AcpiSupport protocol is used. Use AcpiSdt/AcpiTable protocols instead. \n"));
    if ((This != &gAcpiData.AcpiSupportProtocol) || (Table == NULL)) return EFI_INVALID_PARAMETER;

    Status = mAcpiSdtProtocol->GetAcpiTable((UINTN)Index,
    									 &LocalTable,
    									 Version,
    									 Handle);
    
    if (!EFI_ERROR (Status)) 
    {
    	*Table = AllocateCopyPool (LocalTable->Length, LocalTable);
    
    	ASSERT (*Table);
    
    	if (*Table == NULL)
    		return EFI_OUT_OF_RESOURCES;
    }
    
    return Status;
    
}//end of AcpiSupportGetAcpiTable

/**
    This function allows to add, remove of modify ACPI table

         
    @param This pointer to EFI_ACPI_SUPPORT_PROTOCOL instance
    @param Table Pointer to update data. If NULL, corresponded table
        should be removed
    @param Checksum if TRUE, function will recalculate checksum before adding table
    @param Version requested ACPI table version
    @param Handle requested ACPI table handle

          
    @retval EFI_SUCCESS Function executed successfully
    @retval EFI_OUT_OF_RESOURCES not enough memory to perform operation
    @retval EFI_INVALID_PARAMETER invalid EFI_ACPI_SUPPORT_PROTOCOL pointer or ACPI table
        content
    @retval EFI_ABORTED provided ACPI table already present

**/

EFI_STATUS EFIAPI AcpiSupportSetAcpiTable(
    IN EFI_ACPI_SUPPORT_PROTOCOL            *This,
    IN VOID                                     *Table    OPTIONAL,
    IN BOOLEAN                              Checksum,
    IN EFI_ACPI_TABLE_VERSION               Version,
    IN OUT UINTN                            *Handle  )
{
   
    EFI_STATUS      Status = EFI_SUCCESS;
  

    
    //Handle == 0   Table != NULL add the table
    //Handle != 0   Table != NULL replace the table
    //Handle != 0   Table == NULL remove the table
//-----------------------------------------------------------------------------------
    DEBUG((EFI_D_ERROR,"!!!WARNING!!! Deprecated AcpiSupport protocol is used. Use AcpiSdt/AcpiTable protocols instead. \n"));
    if ((This != &gAcpiData.AcpiSupportProtocol) || (Handle==NULL) || (*Handle == 0 && Table == NULL ))  //---------------------------------------------------------------
    {
        Status = EFI_INVALID_PARAMETER;
        ASSERT_EFI_ERROR(Status);
        return Status;
    }
    
    if (*Handle != 0)
    {
    	Status = mAcpiTableProtocol->UninstallAcpiTable (mAcpiTableProtocol,*Handle);
    }
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR (Status)) return Status;
    
    if (Table != NULL)
    {
    	Status = mAcpiTableProtocol->InstallAcpiTable (
                                           mAcpiTableProtocol,
                                           (VOID*)Table,
                                           ((EFI_ACPI_SDT_HEADER*)Table)->Length,
                                           Handle
                                           );
    }
    ASSERT_EFI_ERROR(Status);
    return Status;
//--- !!!!!!!!!!!!!!!!!!!!!!!!!! Version none Done ???? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}// end of AcpiSupportSetAcpiTable


/**
    Causes one or more versions of the ACPI tables to be published in
    the EFI system configuration tables.

         
    @param This pointer to EFI_ACPI_SUPPORT_PROTOCOL instance
    @param Version ACPI table version

          
    @retval EFI_SUCCESS Function executed successfully
    @retval EFI_ABORTED invalid EFI_ACPI_SUPPORT_PROTOCOL pointer or
        an error occurred and the function could not complete successfully.
    @retval EFI_UNSUPPORTED passed ACPI table version invalid

**/

EFI_STATUS EFIAPI AcpiSupportPublishTables(
    IN EFI_ACPI_SUPPORT_PROTOCOL            *This,
    IN EFI_ACPI_TABLE_VERSION               Version )
{
	if (This != &gAcpiData.AcpiSupportProtocol) return EFI_INVALID_PARAMETER;
    else return EFI_SUCCESS;
}

/**
  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param  Instance      Return pointer to the first instance of the protocol

  @return EFI_SUCCESS           The function completed successfully.
  @return EFI_NOT_FOUND         The protocol could not be located.
  @return EFI_OUT_OF_RESOURCES  There are not enough resources to find the protocol.

**/

EFI_STATUS FoundFvWithAcpiTables (OUT EFI_FIRMWARE_VOLUME2_PROTOCOL **Instance)
{
	EFI_STATUS                    Status;
	EFI_HANDLE                    *HandleBuffer;
	UINTN                         NumberOfHandles;
	EFI_FV_FILETYPE               FileType;
	UINT32                        FvStatus = 0;
	EFI_FV_FILE_ATTRIBUTES        Attributes;
	UINTN                         Size;
	UINTN                         Index;
	EFI_FIRMWARE_VOLUME2_PROTOCOL *FvInstance;
	EFI_GUID AcpiTblsFileGuid = ACPI_TABLES_FILE_GUID; // 709E6472-1BCD-43BD-8B6B-CD2D6D08B967

	// Locate protocol.

	Status = gBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiFirmwareVolume2ProtocolGuid,
                   NULL,
                   &NumberOfHandles,
                   &HandleBuffer
                   );
	if (EFI_ERROR (Status)) return Status;


  // Find FV with ACPI tables


	for (Index = 0; Index < NumberOfHandles; Index++) 
	{

		// Get the protocol on each handle

		Status = gBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiFirmwareVolume2ProtocolGuid,
                     (VOID**) &FvInstance
                     );
		ASSERT_EFI_ERROR (Status);
		// See if it has the ACPI storage file
		Status = FvInstance->ReadFile (
                           FvInstance,
                           &AcpiTblsFileGuid,
                           NULL,
                           &Size,
                           &FileType,
                           &Attributes,
                           &FvStatus
                           );

		if (Status == EFI_SUCCESS) 
		{
			*Instance = FvInstance;
			break;
		}
	}

	gBS->FreePool (HandleBuffer);

	return Status;
}

/**
  Submits Acpi tables from dedicated FFS file.


  @return EFI_SUCCESS
  @return EFI_LOAD_ERROR
  @return EFI_OUT_OF_RESOURCES

**/
VOID SubmitAcpiTablesFromFile ()
{
	EFI_STATUS                     Status;
	EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
	UINTN                          Instance;
	EFI_ACPI_COMMON_HEADER         *CurrentTable;
	UINTN                          TableHandle;
	UINT32                         FvStatus;
	UINTN                          TableSize;
	UINTN                          Size;
	EFI_GUID AcpiTblsFileGuid = ACPI_TABLES_FILE_GUID; // 709E6472-1BCD-43BD-8B6B-CD2D6D08B967

	Instance     = 0;
	CurrentTable = NULL;
	TableHandle  = 0;

	// Locate the firmware volume protocol

	Status = FoundFvWithAcpiTables (&FwVol);
	if (EFI_ERROR (Status)) 
	{	
		DEBUG((EFI_D_ERROR,"ACPI: FoundFvWithAcpiTables Fails with a Status = %r\n", Status));
		return;
	}

	// Read tables from the storage file.

	while (Status == EFI_SUCCESS) 
	{

		Status = FwVol->ReadSection (
                      FwVol,
                      &AcpiTblsFileGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID**) &CurrentTable,
                      &Size,
                      &FvStatus
                      );
		if (!EFI_ERROR(Status)) 
		{

			// Check the table

			TableHandle = 0;

			TableSize = ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->Length;
			ASSERT (Size >= TableSize);	// Section size could be bigger than actual table size due to alignment, 
							//but if it is smaller - table is defective


			// Submit ACPI table

			Status = mAcpiTableProtocol->InstallAcpiTable (
							mAcpiTableProtocol,
							(VOID*)CurrentTable,
							CurrentTable->Length,
                            &TableHandle
                            );


			// Free memory allocated by ReadSection
			DEBUG((EFI_D_INFO, "SubmitAcpiTablesFromFile Acpi Tbl submitted Instance = 0x%X, Status - %r\n ", Instance, Status));
			gBS->FreePool (CurrentTable);
			
			ASSERT_EFI_ERROR(Status);
			
			if (EFI_ERROR(Status)) return;

			Instance++;
			
			CurrentTable = NULL;
		}
	}

	return;
}

#if UPDATE_WSMT
/**
  Sets flags in WSMT Table based on AmiPcdSmmMemLibProperties.

  @param  Table      Pointer to WSMT Table to update

  @return EFI_SUCCESS           The function completed successfully.

**/
EFI_STATUS ProtectionFlagTesting( WSMT_ACPI_TABLE *Table)
{
	// Bits 0, 1, and 2 need to be set or cleared 
	//
	// There need to be tests for this functionality.  Do we ...
	// 1) make calls to SMM with offending code to verify?
	// 2) check for our SDL token and PCD tokens to verify that the changes we made are in the project
	// 3) Some combination of the two
    UINT32 SmmMemLibProperties = PcdGet32(AmiPcdSmmMemLibProperties);

    Table->ProtectionFlags = 0;
    if ((SmmMemLibProperties & BIT0) == 0)
        Table->ProtectionFlags |= 3;
    if ((SmmMemLibProperties & BIT1) == 0)
        Table->ProtectionFlags |= 4;

	return EFI_SUCCESS;
}

/**
  Returns ACPI table with specified Signature.

  @param  Signature  Signature to look for
  @param  Table      Will be populated with the pointer to desired table
  @param  Version    Will hold the returned table Version
  @param  Version    Will hold the returned table Key

  @return EFI_SUCCESS           The function completed successfully.
  @return EFI_NOT_FOUND         Table was not found.

**/

EFI_STATUS FindAcpiTable(UINT32 Signature, EFI_ACPI_SDT_HEADER **Table, EFI_ACPI_TABLE_VERSION *Version, UINTN *Key)
{
    EFI_STATUS Status;
    UINTN   Idx;

    Idx = 0;
    do {
        Status = mAcpiSdtProtocol->GetAcpiTable ( Idx, Table, Version, Key);
        if (Status == EFI_NOT_FOUND) {
            DEBUG(( DEBUG_INFO, "FindAcpiTable - Table not Found!!!\n"));

          break;
        }

        Idx++;
    } while ((*Table)->Signature != Signature);

    return Status;
}

/**
  Search for WSMT table, uninstalls old version and installs table with updated flags.

  @param  Signature  Signature to look for
  @param  Table      Will be populated with the pointer to desired table
  @param  Version    Will hold the returned table Version
  @param  Version    Will hold the returned table Key

  @return EFI_SUCCESS           The function completed successfully.
  @return Error                 Based on conditions.

**/

VOID EFIAPI PublishWsmtTable (    
        EFI_EVENT Event, 
        VOID *Context)
{
	EFI_STATUS 	Status;
	WSMT_ACPI_TABLE	Table;
    EFI_ACPI_SDT_HEADER *OldTable;
	UINTN  Key;
	UINT32 Signature = 0x544D5357;    // "WSMT"
    EFI_ACPI_TABLE_VERSION Version = 0;
	
	
    Status = FindAcpiTable(Signature, &OldTable, &Version, &Key);
    DEBUG(( DEBUG_INFO, "WSMT table search.  Status = %r\n", Status));
    if (!EFI_ERROR(Status))
    {
        // Delete current FIDT, Update FIDT table structure, and republish
        Status = mAcpiTableProtocol->UninstallAcpiTable (mAcpiTableProtocol, Key);
        if (EFI_ERROR(Status))
            DEBUG(( EFI_D_ERROR, "Can not uninstall WSMT table.  Status = %r\n", Status));
    }
	
    PrepareHdr20(Signature, (EFI_ACPI_SDT_HEADER*)&Table, 4);
    Table.Header.Length = sizeof(WSMT_ACPI_TABLE);
    Table.Header.Revision = 1;
    Table.Header.CreatorRevision = 0x00010013;//TABLE_CREATOR_REV_MS
    
  	// makes sure that the reserved bits in the ProtectionFlags are zero.
  	Table.ProtectionFlags = 0;
	
	// Test for protection
	// Fill out the ProtectionFlags based on the results of the testing
	ProtectionFlagTesting(&Table);


	// Publish Table
	Status = mAcpiTableProtocol->InstallAcpiTable (mAcpiTableProtocol, &Table, sizeof(WSMT_ACPI_TABLE), &Key);
	if (EFI_ERROR(Status))
	{
		DEBUG(( EFI_D_ERROR, "Can not not install WSMT table.  Status = %r\n", Status));
		ASSERT_EFI_ERROR(Status);
	}
	
}


#endif
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Driver entry point
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/**
    This function is ACPI driver entry point

         
    @param ImageHandle Image handle
    @param SystemTable pointer to system table

          
    @retval EFI_SUCCESS Function executed successfully, ACPI_SUPPORT_PROTOCOL installed
    @retval EFI_ABORTED Dsdt table not found or table publishing failed
    @retval EFI_ALREADY_STARTED driver already started
    @retval EFI_OUT_OF_RESOURCES not enough memory to perform operation

    @note  
  This function also creates ReadyToBoot event to update AML objects before booting

**/

EFI_STATUS EFIAPI AcpiNewCoreEntry (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS      Status = EFI_SUCCESS;
    static EFI_GUID Acpisupguid = EFI_ACPI_SUPPORT_GUID;
#if UPDATE_WSMT
    EFI_EVENT ReadyToBootEvent;
#endif

//------------------------

    DEBUG ((EFI_D_ERROR, "!!!ACPI Core Entry!!!.\n" ));
    //
    // Locate the EFI_ACPI_TABLE_PROTOCOL.
    //
    Status = gBS->LocateProtocol (
                      &gEfiAcpiTableProtocolGuid,
                      NULL,
                      (VOID **)&mAcpiTableProtocol
                      );
    if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "ACPI Core: Cannot locate the EFI ACPI Table Protocol!!!.\n" ));
        return Status;
    }

    //
    // Locate the EFI_ACPI_SDT_PROTOCOL.
    //
    Status = gBS->LocateProtocol (
                      &gEfiAcpiSdtProtocolGuid,
                      NULL,
                      (VOID **)&mAcpiSdtProtocol
                      );
    if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "ACPI Core: Cannot locate the EFI ACPI Sdt Protocol.\n"));
        return Status;
    }
    
    
    Status = BuildMandatoryAcpiTbls (); 
    //Call Architecture Specific Library function to build a set of mandatory tables
    if (EFI_ERROR(Status)) 
    	return Status;
    
#if MPS_TABLE_SUPPORT == 1
    Status = MpsTableBuilderInit(ImageHandle, SystemTable);
    ASSERT_EFI_ERROR(Status);
#endif
    
    Status = AcpiReadyToBootEvent();
    //Call Architecture Specific Library function to craeate Ready to Boot event, if necessary 
    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) 
    	return Status;
    
    
    gAcpiData.AcpiSupportProtocol.GetAcpiTable = AcpiSupportGetAcpiTable;
    gAcpiData.AcpiSupportProtocol.SetAcpiTable = AcpiSupportSetAcpiTable;
    gAcpiData.AcpiSupportProtocol.PublishTables = AcpiSupportPublishTables;

    gAcpiData.AcpiSupportHandle = NULL;
    SubmitAcpiTablesFromFile ();
    
    //Instasll ProtocolInterface;
    Status=gBS->InstallMultipleProtocolInterfaces(
               &gAcpiData.AcpiSupportHandle,
               &Acpisupguid,
               &gAcpiData.AcpiSupportProtocol,
               NULL);
    ASSERT_EFI_ERROR(Status);


#if UPDATE_WSMT    
    Status = EfiCreateEventReadyToBootEx (
            TPL_CALLBACK,
            PublishWsmtTable,
            NULL,
            &ReadyToBootEvent
            );
    ASSERT_EFI_ERROR(Status);  
#endif
    return Status;
    
}


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             5555 Oakbrook Pkwy, Norcross, GA 30093               **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
