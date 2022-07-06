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
// $Header: /AptioV/SRC/Debugger/DbgModuleLocator/PeiDbgModuleLocator.c 2     2/14/15 4:10p Sudhirv $
//
// $Revision: 2 $
//
// $Date: 2/14/15 4:10p $
//*****************************************************************
//*****************************************************************
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/Debugger/DbgModuleLocator/PeiDbgModuleLocator.c $
// 
// 2     2/14/15 4:10p Sudhirv
// CPPCheck Changes
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

//**********************************************************************
//<AMI_FHDR_START>
//
// Name:	ModuleLocator.c
//
// Description:	Recevies the Load Image PPI notifications form the PEI core
//
//<AMI_FHDR_END>
//**********************************************************************
//*****************************************************************
#include <Efi.h>
#include <Library/AMIpeidebug.h>
#include <Library/AMIPeiGUIDs.h>
#include <PPI/LoadedImagePpi.h>
#include <AmiPeiLib.h>

//**********************************************************************
static EFI_GUID  mPeiDebugDataGuid = PEI_DBGSUPPORT_DATA_GUID;
//**********************************************************************
BOOLEAN IsAMIDebugRxConnected(UINT8);
INT8 CompareGuid(EFI_GUID *G1, EFI_GUID *G2);
EFI_GUID gEfiPeiLoadedImagePpiGuid = EFI_PEI_LOADED_IMAGE_PPI_GUID;

EFI_STATUS GetImagePDBFileName(
			IN VOID *ImageBase, 
			IN UINT64 ImageSize, 
			OUT VOID **PDBFileNamePtr );

EFI_STATUS GetPEBaseAndSizeFromTEImage( IN VOID *ImageBase,
									    IN UINT64 ImageSize, 
										OUT UINT64 *EfiImageBase,
										OUT UINT64 *EfiSize);

EFI_STATUS
EFIAPI
LoadImagePpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList = {
		EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
   		&gEfiPeiLoadedImagePpiGuid,
		LoadImagePpiNotifyCallback
};

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
GetPEIPEBaseAndSizeFromTEImage(
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

  if(Pe32Data == NULL)
	  return EFI_UNSUPPORTED;
  
  if(EntryPoint == NULL)
	  return EFI_UNSUPPORTED;

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
    return EFI_SUCCESS;
  } else if (Hdr.Pe32->Signature == EFI_IMAGE_NT_SIGNATURE) {
    *EntryPoint = (VOID *)((UINTN)Pe32Data + (UINTN)(Hdr.Pe32->OptionalHeader.AddressOfEntryPoint & 0x0ffffffff));
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
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

		GetPEIPEBaseAndSizeFromTEImage( ImageBase,ImageSize,&EfiImageBase,&EfiSize);
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

//BOOLEAN PPIInstalled = FALSE;
//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	InstallLoadImagePpiCallback()
//
// Description:	Install LoadImage Ppi Callback
//				
// Input:		EFI_PEI_SERVICES	**PeiServices
//
// Output: 		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS InstallLoadImagePpiCallback(IN EFI_PEI_SERVICES	**PeiServices)
{
	//if(!PPIInstalled)
	//{
		//PPIInstalled = TRUE;
		return (*PeiServices)->NotifyPpi (PeiServices, &mNotifyList);
	//}
	//return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	LoadImagePpiNotifyCallback()
//
// Description:	LoadImage Ppi Callback Function
//				
// Input:	  	PeiServices       - The PEI core services table.
//				NotifyDescriptor  - The descriptor for the notification event.
//				Ppi               - Pointer to the PPI in question.
//
// Output: 		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
EFIAPI
LoadImagePpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
    EFI_PEI_LOADED_IMAGE_PPI	*LoadedImagePpi = Ppi;
	VOID						*PDBFileName=NULL;
	EFI_STATUS 					Status=EFI_SUCCESS;
	VOID						*FirstHob = NULL;
	EFI_HOB_MEMORY_ALLOCATION	*Hob = NULL;
	UINTN						*EntryPoint = 0, tempAddr = 0;
	BOOLEAN  					InterruptState;
#ifndef EFIx64
  PEIDebugData_T *pData;
  DbgGblData_T *DbgData;
#else
  PEIDebugData_Tx64 *pData;
  DbgGblData_Tx64 *DbgData;
#endif

	EFI_PHYSICAL_ADDRESS 				EfiImageBase,EfiSize;
	UINT32				EaxVal,EdxVal;
	UINT8				TempBuffer[100];
	EFI_GUID			DriverID;

	//PEI_PERF_START (NULL, "DBG-LD", NULL, 0);

	if(!IsAMIDebugRxConnected(0)) {
		//PEI_PERF_END (NULL, "DBG-LD", NULL, 0);
		return Status;
	}
	
	//
	// Disable interrupts and save the current interrupt state
	//
	InterruptState = SaveAndDisableInterrupts ();
#ifndef EFIx64
	pData		= (PEIDebugData_T *)PEI_DEBUG_DATA_BASEADDRESS;
	if(pData)
		DbgData		= (DbgGblData_T *)(pData->DbgrLocalVar);
#else
	pData		= (PEIDebugData_Tx64 *)PEI_DEBUG_DATA_BASEADDRESS;
	if(pData)
		DbgData		= (DbgGblData_Tx64 *)(pData->DbgrLocalVar);
#endif
    // Workaround if Image size is 0
    if(LoadedImagePpi->ImageSize == 0)
        LoadedImagePpi->ImageSize = 0x10000;

	if((LoadedImagePpi->ImageAddress == 0)||(LoadedImagePpi->ImageSize==0)) 
	{
		SetInterruptState (InterruptState);
		//PEI_PERF_END (NULL, "DBG-LD", NULL, 0);
		return Status; // Invalid Notification.
	}

	GetPEIPEBaseAndSizeFromTEImage( (VOID*)LoadedImagePpi->ImageAddress,
								LoadedImagePpi->ImageSize,
								&EfiImageBase,
								&EfiSize);
	Status = GetImagePDBFileName((VOID*)LoadedImagePpi->ImageAddress,
								LoadedImagePpi->ImageSize,
								&PDBFileName);


    (*PeiServices)->CopyMem((VOID *)&DriverID, 
          (VOID*)LoadedImagePpi->FileHandle, 
          sizeof(EFI_GUID));
	
	if((!PDBFileName)&&(LoadedImagePpi->ImageAddress)&&(LoadedImagePpi->ImageSize)&&(EFI_ERROR (Status)))
	{
		PDBFileName = (VOID *)&TempBuffer;
		//EIP 28735 and 28737. To avoid the Hang in Sprint with %g when runing from ROM.
		Sprintf(PDBFileName, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x.efi", DriverID.Data1,
			DriverID.Data2,DriverID.Data3,DriverID.Data4[0],DriverID.Data4[1],DriverID.Data4[2],
			DriverID.Data4[3],DriverID.Data4[4],DriverID.Data4[5],DriverID.Data4[6],DriverID.Data4[7]);
		Status = EFI_SUCCESS;
	}

	if(EFI_ERROR(Status))
	{
		SetInterruptState (InterruptState);
		//PEI_PERF_END (NULL, "DBG-LD", NULL, 0);
		return Status;
	}
	
	if(pData && DbgData)
	{
		Status = GetEntryPoint ((VOID *) (UINTN) (LoadedImagePpi->ImageAddress), (VOID**) &EntryPoint);
		DbgData->pPeiDbgDbgrIfc->PeiDbgr_SendPeiLoadImage(
										(UINTN)LoadedImagePpi->FileHandle,
										(UINTN)EfiImageBase,
										(UINTN)EfiSize,
										(UINTN)EntryPoint,
										PDBFileName);
	}

	SetInterruptState (InterruptState);
	
	//while(1);
	tempAddr = (UINTN)(LoadedImagePpi->ImageAddress & 0xff000000);
	if(tempAddr != 0xff000000) {
		return EFI_SUCCESS;
	}
	
	if(pData)
	{
		if(DbgData && DbgData->m_NotifyAndBreak == 1)
		{
			EaxVal = 0x494d4124;	//"$AMI"
			EdxVal = 0x4442414e;	//"NABD" - NotifyAndBreak Driver
#ifndef EFIx64
			_asm{
				push eax
				push edx
				mov eax,EaxVal
				mov edx,EdxVal
				int 3
				pop edx
				pop eax
			}
#else
			DriverSpecialBreakPoint(EaxVal,EdxVal);
#endif
		}
	}

	//PEI_PERF_END (NULL, "DBG-LD", NULL, 0);
	return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	DriverSpecialBreakPoint()
//
// Description:	Moves desired values to EAX and EDX registers to help
//				enable special breakpoint.
//				
// Input:		UINT32 EaxValue,UINT32 EdxValue
//				EaxValue - Value for EAX register
//				EdxValue - Value for EDX register
//
// Output: 		void
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
void DriverSpecialBreakPoint(UINT32 EaxValue,UINT32 EdxValue)
{
#ifndef EFIx64		
	__asm{			  
		push eax
		push edx
		mov eax,EaxValue	//move the values to eax and edx reg
		mov edx,EdxValue
		int 3;			//Interrupt 3 - Breakpoint
		pop edx
		pop eax
	}
#else
#endif
	return;
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
InitDbgLdImageNotifyPPI(
    IN EFI_FFS_FILE_HEADER *FfsHeader,
    IN VOID **PeiServices
)
{
	EFI_STATUS Status = EFI_SUCCESS;
	
	//PEI_PERF_START (NULL, "DBG-LDM", NULL, 0);
	
	if(!IsAMIDebugRxConnected(0)) 
		return EFI_SUCCESS;
	//
	// Install the notification callback for LoadImage PPI
	//
	//while(1);
	Status = InstallLoadImagePpiCallback((EFI_PEI_SERVICES **)PeiServices);

	//PEI_PERF_END (NULL, "DBG-LDM", NULL, 0);
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
