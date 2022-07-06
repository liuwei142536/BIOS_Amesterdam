//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Header: /AptioV/SRC/Debugger/DbgModuleLocator/DxeDbgModuleLocator.c 1     2/20/14 5:06p Sudhirv $
//
// $Revision: 1 $
//
// $Date: 2/20/14 5:06p $
//*****************************************************************
//*****************************************************************
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/Debugger/DbgModuleLocator/DxeDbgModuleLocator.c $
// 
// 1     2/20/14 5:06p Sudhirv
// Added for AMI Debug for UEFI 3.01.0008 Label
// 
// 1     2/20/14 5:03p Sudhirv
// Added for AMI Debug for UEFI 3.01.0008 INT Label
// 
//
//*****************************************************************
//*****************************************************************

#include <Efi.h>
#include "AmiDxeLib.h"
#include <Protocol/LoadedImage.h>
#ifndef	EFIx64
#include <Library/AMIpeidebug.h>
#else
#include <Library/AMIpeidebugX64.h>
#endif
#include <Library/AMIPeiGUIDs.h>

#include <Library/PerformanceLib.h>

static EFI_EVENT mImageEvent;
static EFI_HANDLE mImageHandle;
static CHAR16 mContextString[] = L"lOaDeDiMaGe1";
static VOID *mRegImgEvent;

#define	PEI_DEBUG_DATA_BASEADDRESS	GetDbgDataBaseAddress()

//-------------------------------------------------------------------------
static EFI_GUID HobListGuid	= HOB_LIST_GUID;
static EFI_GUID  mDxeDebugDataGuid = DXE_DBG_DATA_GUID;

#ifndef	EFIx64
static PEIDebugData_T	*pPeiDbgDataSection;
#else
static PEIDebugData_Tx64	*pPeiDbgDataSection;
#endif
static EFI_PHYSICAL_ADDRESS 	DSMemoryBaseAddress = 0;

GUID_VARIABLE_DECLARATION(gEfiLoadedImageProtocolGuid, EFI_LOADED_IMAGE_PROTOCOL_GUID);


VOID EFIAPI DxeImageLoadedEvent(IN EFI_EVENT Event, IN VOID *Context);
EFI_STATUS _InsertStructure(IN EFI_HANDLE AcquiredHandle, IN EFI_HANDLE AgentHandle );
BOOLEAN IsAMIDebugRxConnected(UINT8);
VOID DbgModDriverSpecialBreakpoint(VOID);

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	FindPeiDebugDataHob()
//
// Description:	Function to find the PEI Debug Data Hob
//
// Input:		EFI_HOB_MEMORY_ALLOCATION	**MemoryHob
//
// Output:		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS DbgModFindPeiDebugDataHob(EFI_HOB_MEMORY_ALLOCATION      **MemoryHob)
{
	EFI_STATUS	Status = EFI_NOT_FOUND;

	*MemoryHob = (EFI_HOB_MEMORY_ALLOCATION*)GetEfiConfigurationTable(pST,&HobListGuid);

	if(*MemoryHob)
	{
	    for(;;){
	        Status = FindNextHobByType(EFI_HOB_TYPE_MEMORY_ALLOCATION,MemoryHob);
	        if (EFI_ERROR(Status)) break;
	        if (!guidcmp(&(*MemoryHob)->AllocDescriptor.Name,&mDxeDebugDataGuid)) {
	            Status=EFI_SUCCESS;
				break;
	        }
	    }
	}
	return Status;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	CreateProtocolNotifyEvent()
//
// Description:	Create a protocol notification event and return it.
//				
// Input:	  	ProtocolGuid    - Protocol to register notification event on.
//				NotifTpl        - Maximum TPL to single the NotifyFunction.
//				NotifyFunciton  - EFI notification routine.
//				NotifyContext   - Context passed into Event when it is created.
//				Registration    - Registration key returned from RegisterProtocolNotify().
//
// Output:  	EFI_EVENT 
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_EVENT
CreateProtocolNotifyEvent (
  IN EFI_GUID             *ProtocolGuid,
  IN EFI_TPL              NotifyTpl,
  IN EFI_EVENT_NOTIFY     NotifyFunction,
  IN VOID                 *NotifyContext,
  OUT VOID                *Registration
  )
{
  EFI_STATUS              Status;
  EFI_EVENT               Event;

  //
  // Create the event
  //

  Status = pBS->CreateEvent (
                  EFI_EVENT_NOTIFY_SIGNAL,
                  NotifyTpl,
                  NotifyFunction,
                  NotifyContext,
                  &Event
                  );
  ASSERT (!EFI_ERROR (Status));

  //
  // Register for protocol notifactions on this event
  //

  Status = pBS->RegisterProtocolNotify (
                  ProtocolGuid,
                  Event,
                  Registration
                  );

  ASSERT (!EFI_ERROR (Status));

  //
  // Kick the event so we will perform an initial pass of
  // current installed drivers
  //

  pBS->SignalEvent (Event);
  return Event;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	GetEfiBaseAndSizeFromTEImage()
//
// Description:	Calculates the PE Image Base and Size from the TE Image.
//
// Input: 		VOID *ImageBase,UINT64 ImageSize,VOID **PDBFileNamePtr
//
// Output: 		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
RETURN_STATUS
GetPEBaseAndSizeFromTEImage(
		IN VOID *ImageBase,
		IN UINT64 ImageSize,
		OUT UINT64 *EfiImageBase,
		OUT UINT64 *EfiSize)
{
	if(*(UINT16*)ImageBase==EFI_TE_IMAGE_HEADER_SIGNATURE)
	{
		EFI_TE_IMAGE_HEADER	*TEHeader = ImageBase;
		*EfiImageBase = ((UINT64)TEHeader + sizeof(EFI_TE_IMAGE_HEADER) - TEHeader->StrippedSize);
		*EfiSize = ImageSize - sizeof(EFI_TE_IMAGE_HEADER) + TEHeader->StrippedSize - sizeof(UINT32);
	}
	else
	{
		*EfiImageBase=(UINT64)ImageBase;
		*EfiSize=(UINT64)ImageSize;
	}
	return RETURN_SUCCESS;
}

#ifdef EFIx64
#define EFI_IMAGE_NT_HEADERS EFI_IMAGE_NT_HEADERS64
#else
#define EFI_IMAGE_NT_HEADERS EFI_IMAGE_NT_HEADERS32
#endif

/**
  Retrieves and returns a pointer to the entry point to a PE/COFF image that has been loaded
  into system memory with the PE/COFF Loader Library functions.

  Retrieves the entry point to the PE/COFF image specified by Pe32Data and returns this entry
  point in EntryPoint.  If the entry point could not be retrieved from the PE/COFF image, then
  return RETURN_INVALID_PARAMETER.  Otherwise return RETURN_SUCCESS.
  If Pe32Data is NULL, then ASSERT().
  If EntryPoint is NULL, then ASSERT().

  @param  Pe32Data                  The pointer to the PE/COFF image that is loaded in system memory.
  @param  EntryPoint                The pointer to entry point to the PE/COFF image to return.

  @retval RETURN_SUCCESS            EntryPoint was returned.
  @retval RETURN_INVALID_PARAMETER  The entry point could not be found in the PE/COFF image.

**/
EFI_STATUS
EFIAPI
GetEntryPoint (
  IN  VOID  *Pe32Data,
  OUT VOID  **EntryPoint
  )
{
  EFI_IMAGE_DOS_HEADER                  *DosHdr;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION   Hdr;

  ASSERT (Pe32Data   != NULL);
  ASSERT (EntryPoint != NULL);

  DosHdr = (EFI_IMAGE_DOS_HEADER *)Pe32Data;
  if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    //
    // DOS image header is present, so read the PE header after the DOS image header.
    //
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINTN) Pe32Data + (UINTN) ((DosHdr->e_lfanew) & 0x0ffff));
  } else {
    //
    // DOS image header is not present, so PE header is at the image base.
    //
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)Pe32Data;
  }

  //
  // Calculate the entry point relative to the start of the image.
  // AddressOfEntryPoint is common for PE32 & PE32+
  //
  if (Hdr.Te->Signature == EFI_TE_IMAGE_HEADER_SIGNATURE) {
    *EntryPoint = (VOID *)((UINTN)Pe32Data + (UINTN)(Hdr.Te->AddressOfEntryPoint & 0x0ffffffff) + sizeof(EFI_TE_IMAGE_HEADER) - Hdr.Te->StrippedSize);
    return RETURN_SUCCESS;
  } else if (Hdr.Pe32->Signature == EFI_IMAGE_NT_SIGNATURE) {
    *EntryPoint = (VOID *)((UINTN)Pe32Data + (UINTN)(Hdr.Pe32->OptionalHeader.AddressOfEntryPoint & 0x0ffffffff));
    return RETURN_SUCCESS;
  }

  return RETURN_UNSUPPORTED;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	GetImagePDBFileName()
//
// Description:	Locates thte PDB Filename from the Image Loaded.
//				
// Input: 		VOID *ImageBase,UINT64 ImageSize,VOID **PDBFileNamePtr
//
// Output: 		EFI_STATUS 
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS GetImagePDBFileName(IN VOID *ImageBase, 
							   IN UINT64 ImageSize, 
							   OUT VOID **PDBFileNamePtr )
{
   EFI_STATUS   			Status=EFI_SUCCESS;
   UINT32					Offset=0;
   EFI_IMAGE_NT_HEADERS		*ImageHeaders=NULL;
   EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION	ImageHeadersUnion;
   EFI_IMAGE_DATA_DIRECTORY	*DebugDataDir=NULL;
   EFI_IMAGE_DEBUG_DIRECTORY_ENTRY *DebugDirEntry=NULL;		

	if(*(UINT16*)ImageBase==EFI_IMAGE_DOS_SIGNATURE)
	{
	   //IS PE/PE+ Image

	   //----------------------------------
	   //At Offset 0x3c from the beginning of the Image sits offset of the PE Header
	   Offset=*((UINT32*)((UINT8*)ImageBase+0x3C));
	
		if(Offset == 0xffffffff || Offset > ImageSize)	//invalid value so return error
			return EFI_NOT_FOUND;
	
	   ImageHeaders=(EFI_IMAGE_NT_HEADERS*)((UINT8*)ImageBase+Offset);
	   (EFI_IMAGE_NT_HEADERS*)ImageHeadersUnion.Pe32 = ImageHeaders;
	   //Check If the PE00 signature is valid
	   if(ImageHeaders->Signature!=EFI_IMAGE_NT_SIGNATURE)
		  return EFI_UNSUPPORTED;
	
	   //Now we have to check Characteristics field in COFF.Header to check if we have 
	   //debug info available...
	   if (ImageHeaders->FileHeader.Characteristics & EFI_IMAGE_FILE_DEBUG_STRIPPED)
		   return EFI_UNSUPPORTED; 
	
	   //Everything fine so far ...
	   //Debug Directory entry is #6 (0 based)
	   //Doublecheck once again if it is present
	   if(ImageHeaders->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC )
	   {
		   if (ImageHeadersUnion.Pe32->OptionalHeader.NumberOfRvaAndSizes>=6)
		       DebugDataDir=&ImageHeadersUnion.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_DEBUG];
		   else 
			   return EFI_UNSUPPORTED;
	   }
	   else if(ImageHeaders->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC )
	   {
		   if (ImageHeadersUnion.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes>=6)
		       DebugDataDir=&ImageHeadersUnion.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_DEBUG];
		   else 
			   return EFI_UNSUPPORTED;
	   } 
	   else
			return EFI_UNSUPPORTED;

	   //Get the pointer   
	   DebugDirEntry=(EFI_IMAGE_DEBUG_DIRECTORY_ENTRY*)((UINT8*)ImageBase+DebugDataDir->VirtualAddress);

	}
	else if (*(UINT16*)ImageBase==EFI_TE_IMAGE_HEADER_SIGNATURE)
	{
		UINT64 EfiImageBase;
		UINT64 EfiSize;
		EFI_TE_IMAGE_HEADER	*TEHeader = ImageBase;

		GetPEBaseAndSizeFromTEImage( ImageBase,ImageSize,&EfiImageBase,&EfiSize);
		DebugDirEntry = (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY*)(EfiImageBase+TEHeader->DataDirectory[1].VirtualAddress);
		// Every thing is refered from EFIImageBase.
		ImageBase=(VOID*)EfiImageBase;
	}
	else //Unsupported file format
		return EFI_UNSUPPORTED;

   if(DebugDirEntry->Type!=(UINT32)EFI_IMAGE_DEBUG_TYPE_CODEVIEW) 
	  return(EFI_UNSUPPORTED);
   //Use "Offset" variable to hold the Signature value
   Offset=*((UINT32*)((UINT8*)ImageBase+DebugDirEntry->RVA));

   if(Offset==CODEVIEW_SIGNATURE_NB10)
   {
	   EFI_IMAGE_DEBUG_CODEVIEW_NB10_ENTRY *Nb10Entry=(EFI_IMAGE_DEBUG_CODEVIEW_NB10_ENTRY*)((UINT8*)ImageBase+DebugDirEntry->RVA);	
       //-----------------------------------
       Nb10Entry++;
	   *PDBFileNamePtr=(CHAR8*)(Nb10Entry);
   }
   else 
   {
	   if (Offset==CODEVIEW_SIGNATURE_RSDS)
	   {
		   EFI_IMAGE_DEBUG_CODEVIEW_RSDS_ENTRY *RsdsEntry=(EFI_IMAGE_DEBUG_CODEVIEW_RSDS_ENTRY*)((UINT8*)ImageBase+DebugDirEntry->RVA);
		   RsdsEntry++;
		   *PDBFileNamePtr=(CHAR8*)(RsdsEntry);
	   }  
	   else 
	       //If Debug Directory entry none of Type listed above we don't support those
   		   Status=EFI_UNSUPPORTED;		
   } 

   return Status;	
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	InitDbgLdImageNotify()
//
// Description:	An ELINK to send notification to Pei debugger 
//
// Input:		VOID* ImageHandle, IN VOID *SystemTable
//
// Output:		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
InitDbgLdImageNotify(
	IN VOID* ImageHandle, 
	IN VOID *SystemTable)
{
	EFI_STATUS						Status = EFI_SUCCESS;

	PERF_START (NULL, "DBG-LDN", NULL, 0);
	//while(1);
	if(!IsAMIDebugRxConnected(0)) {
		PERF_END (NULL, "DBG-LDN", NULL, 0);
		return Status;
	}

	mImageHandle = (EFI_HANDLE)ImageHandle;
	
    mImageEvent = CreateProtocolNotifyEvent( 
			&gEfiLoadedImageProtocolGuid, 
		 	TPL_NOTIFY, 
			DxeImageLoadedEvent,
			mContextString, 
			&mRegImgEvent
			);

    PERF_END (NULL, "DBG-LDN", NULL, 0);
	return Status;
}


//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	DxeImageLoadedEvent()
//
// Description:	Event callback function sends the Loaded image info to host
//				
// Input:		EFI_EVENT Event, VOID *Context
//
// Output:		VOID
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
// loaded image notification functionality
VOID EFIAPI DxeImageLoadedEvent(IN EFI_EVENT Event, IN VOID *Context)
{ 
  UINTN HandleCount=0; 
  EFI_HANDLE *TmpHandle=NULL;
  EFI_STATUS Status=EFI_SUCCESS;
  UINT8 InterruptState = 0;

  PERF_START (NULL, "DBG-LD", NULL, 0);
  
  if(!IsAMIDebugRxConnected(0)) {
	  PERF_END (NULL, "DBG-LD", NULL, 0);
	  return;
  }

  InterruptState = SaveAndDisableInterrupts ();

  do
  {
      Status=pBS->LocateHandleBuffer(
  			ByRegisterNotify,
  			NULL,
  			mRegImgEvent,
  			&HandleCount,
  			&TmpHandle);

     if(EFI_ERROR(Status)) 
	    break;
 	 _InsertStructure(TmpHandle[HandleCount-1],mImageHandle);
     Status=pBS->FreePool(TmpHandle);
  }while(TRUE);

  SetInterruptState (InterruptState);	
  PERF_END (NULL, "DBG-LD", NULL, 0);
  return;
} 

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	_InsertStructure()
//
// Description:	sends the Loaded image info to host
//				
// Input:		EFI_HANDLE AcquiredHandle, EFI_HANDLE AgentHandle
//
// Output:		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS _InsertStructure(IN EFI_HANDLE AcquiredHandle, IN EFI_HANDLE AgentHandle )
{
	EFI_STATUS 					Status=EFI_SUCCESS;
	EFI_LOADED_IMAGE_PROTOCOL 	*LoadedImageProtocol=NULL;
	VOID						*PDBFileName=NULL;
	UINTN						pos=0,i=0,*EntryPoint=0;
	UINTN						NumHandles=0;
 	EFI_HOB_MEMORY_ALLOCATION   *MemoryHob;
	UINT8						FPType, FPSubType;
	EFI_GUID					*DriverID;
	UINT8						TempBuffer[100];
	BOOLEAN  					InterruptState;
#ifndef	EFIx64
	PEIDebugData_T *pData;
	DbgGblData_T *DbgData;
	UINT32						EaxVal,EdxVal;
#else
	PEIDebugData_Tx64 *pData;
	DbgGblData_Tx64 *DbgData;
#endif
	
	//PERF_START (NULL, "DBG-LD", NULL, 0);
	//
	// Disable interrupts and save the current interrupt state
	//
	InterruptState = SaveAndDisableInterrupts ();
	
#ifndef	EFIx64
	pData		= (PEIDebugData_T *)PEI_DEBUG_DATA_BASEADDRESS;
	DbgData		= (DbgGblData_T *)(pData->DbgrLocalVar);
#else
	pData		= (PEIDebugData_Tx64 *)PEI_DEBUG_DATA_BASEADDRESS;
	DbgData		= (DbgGblData_Tx64 *)(pData->DbgrLocalVar);
#endif
	
 	//Get LoadedImage Protocol Interface 	
	Status=pBS->OpenProtocol(
		AcquiredHandle,
		&gEfiLoadedImageProtocolGuid,
		(VOID**)&LoadedImageProtocol,
		AgentHandle,
		NULL,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

	//Check if any errors?
  	if (EFI_ERROR (Status)) {
  		//PERF_END (NULL, "DBG-LD", NULL, 0);
  		return Status;
  	}

  	//Get some memory for the initial structure
    Status=GetImagePDBFileName(
		LoadedImageProtocol->ImageBase,
		LoadedImageProtocol->ImageSize,
		&PDBFileName);

	FPType = LoadedImageProtocol->FilePath->Type;
	FPSubType = LoadedImageProtocol->FilePath->SubType;
	DriverID = (EFI_GUID*)Malloc(sizeof(EFI_GUID));
	
	//TRACE((-1,"Driver Handle: %x\n", AcquiredHandle));
	
	if((FPType == 0x4)&&(FPSubType == 0x6))
	{
		MemCpy(DriverID,(LoadedImageProtocol->FilePath)+1,sizeof(EFI_GUID));
		//if(PDBFileName)
		//	TRACE((-1,"Driver INFO: %s\n", PDBFileName));
		//TRACE((-1,"Driver GUID: %g\n", DriverID));
	}
	
	if ((!PDBFileName)&&(LoadedImageProtocol->ImageBase)&&(LoadedImageProtocol->ImageSize)&&(EFI_ERROR (Status)))
	{
		if((FPType == 0x4)&&(FPSubType == 0x6))
		{	
			PDBFileName = (VOID *)&TempBuffer;
			Sprintf(PDBFileName, "%g.efi", DriverID);
			Status = EFI_SUCCESS;
			//TRACE((-1,"Driver INFO: %s\n", PDBFileName));
		}
		
	}
	if (DriverID) 
		pBS->FreePool(DriverID);
	//Check if any error?
	//If image don't have debug info, we don't have to include it in the list...
	if (EFI_ERROR (Status)){
		//Information we got is useless so we don't have to append module list.
		//We have to close protocol interface and continue;	
		Status=pBS->CloseProtocol(
			AcquiredHandle,
			&gEfiLoadedImageProtocolGuid,
			AgentHandle,
			NULL);
		
		// reset the interrupt state
		SetInterruptState (InterruptState);
		
		//It Doesn't really metter who changed the status 
		//we just returning it if it is EFI_UNSUPPORTED caller will
  		// continue looping if not it will exit with error
		//PERF_END (NULL, "DBG-LD", NULL, 0);
		return Status; 
  	}

    //send the information to the host.
	// Lets Find the Hob Again as DXE Core may rellocate the data
	Status = DbgModFindPeiDebugDataHob(&MemoryHob);
	
	if(EFI_ERROR(Status)) 
		pPeiDbgDataSection = NULL;
	else {
		DSMemoryBaseAddress = MemoryHob->AllocDescriptor.MemoryBaseAddress;
#ifndef EFIx64
		pPeiDbgDataSection = (PEIDebugData_T*) DSMemoryBaseAddress;
#else
		pPeiDbgDataSection = (PEIDebugData_Tx64*) DSMemoryBaseAddress;
#endif
	}
#ifndef EFIx64
	DbgData = (DbgGblData_T*)pPeiDbgDataSection->DbgrLocalVar;
#else
	DbgData = (DbgGblData_Tx64*)pPeiDbgDataSection->DbgrLocalVar;
#endif
	//myPeiDbgr_SendPeiLoadImage = (void*)(UINT32)DbgData->pPeiDbgDbgrIfc->PeiDbgr_SendPeiLoadImage;
	if((UINTN)(DbgData->pPeiDbgDbgrIfc)!=0xffffffff) 
	{
		//TRACE(( -1,"Handle(%X)\n", AcquiredHandle));
		Status = GetEntryPoint ((VOID *) (UINTN) (LoadedImageProtocol->ImageBase), (VOID**) &EntryPoint);
		DbgData->pPeiDbgDbgrIfc->PeiDbgr_SendPeiLoadImage((UINTN)AcquiredHandle,
										(UINTN)LoadedImageProtocol->ImageBase,
										(UINTN)LoadedImageProtocol->ImageSize,
										(UINTN)EntryPoint,
										PDBFileName);
	}

	// reset the interrupt state
	SetInterruptState (InterruptState);

	if(DbgData->m_NotifyAndBreak == 1)	
	{
#ifdef	EFIx64	
		DbgModDriverSpecialBreakpoint();		
#else	
		EaxVal = 0x494D4124;	//"$AMI"
		EdxVal = 0x4442414E;	//"NABD" - NotifyAndBreak Driver
			
		_asm
		{
			push eax
			push edx
			mov eax,EaxVal
			mov edx,EdxVal
			int 3
			pop edx
			pop eax
		}
#endif
	}

	//We have to close protocol after using it.
	Status=pBS->CloseProtocol(
		AcquiredHandle,
		&gEfiLoadedImageProtocolGuid,
		AgentHandle,
		NULL);

	//PERF_END (NULL, "DBG-LD", NULL, 0);
	return Status;
}

//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
