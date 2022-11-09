
//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file boot.c
    This file contains code for Boot management

**/

#include "minisetup.h"
#include <Protocol/LoadFile.h>
#include <Protocol/BlockIo.h>
#include <Protocol/UsbIo.h>
#include <Protocol/DiskIo.h>
#include <Protocol/StatusCode.h>
#include <Protocol/DevicePathToText.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
//#include <Library/DevicePathLib.h>

#if TSE_USE_EDK_LIBRARY
#include "TseCommon.h"
#include "EDKHelper.h"
#endif

#define BOOT_OPTION_ALLOC_UNIT  10

#ifndef EFI_WARN_FILE_SYSTEM
#define EFI_WARN_FILE_SYSTEM 6
#endif

//
// Load Option Type
//
typedef enum {
  LoadOptionTypeDriver,
  LoadOptionTypeSysPrep,
  LoadOptionTypeBoot,
  LoadOptionTypePlatformRecovery,
  LoadOptionTypeMax
} EFI_BOOT_MANAGER_LOAD_OPTION_TYPE;

//////////////////////////////////////
//Extern Variables
//////////////////////////////////////
extern UINTN        	gDelOptionCount;
extern BOOT_DATA    	*gDelBootData;
extern UINT32       	gBootFlow;
extern UINTN 			CurrentScreenresolutionX, CurrentScreenresolutionY; 
extern BOOLEAN gBootNextFlag;
extern UINT8 IsImageCodeTypeSupport (VOID);
extern UINT8 gRecoveryBootingType;
extern BOOT_DATA *RecoveryGetRecoveryData( UINT16 Option, UINT8 RecoveryBootingType);
extern void PrintGopAndConInDetails();
extern EFI_GUID gEfiLoadFileProtocolGuid;
extern EFI_GUID gEfiPciIoProtocolGuid;
#if TSE_DEBUG_MESSAGES
VOID DebugShowPrintBootOptionDetails(EFI_DEVICE_PATH_PROTOCOL *DevicePath);
#endif
//////////////////////////////////////
//Internal Variable Declarations
//////////////////////////////////////
UINTN		        		gBootOptionCount;
EFI_GUID	        		gBootNowCountGuid 	= 	BOOT_NOW_COUNT_GUID;
UINT16		        *gBBSDisabled 			= 	L"Disabled in BBS Order";
UINTN		        		gLangCount;
BOOT_DATA		    	*gBootData;
LANGUAGE_DATA	    	*gLanguages;
EFI_HANDLE          	gCurrentBootHandle 	= 	NULL;
EFI_EVENT				gShellLaunchEvent 	= NULL;		
EFI_EVENT				gShell20LaunchEvent 	= NULL;	
UINT16 					DISABLED_BOOT_OPTION; 
UINT16 					DISABLED_DRIVER_OPTION; 
UINTN						gDriverOptionCount;
BOOT_DATA				*gDriverData;
BOOLEAN              gBrowserCallbackEnabled = FALSE; 
BOOLEAN              gFileParsed = FALSE; 
BOOT_DATA				*gOSRecoveryData;
BOOT_DATA				*gPlatformRecoveryData;
BOOLEAN              gWatchDogTimerSet = FALSE; 
UINTN						gOSRecoveryOptionCount;
UINTN						gPlatformRecoveryOpCount;					
//////////////////////////////////////
//Internal Functions Declarations
//////////////////////////////////////
EFI_DEVICE_PATH_PROTOCOL *_BootBuildFVDevicePath( UINT32 *index, EFI_GUID *guidPtr );
EFI_DEVICE_PATH_PROTOCOL *_BootBuildFileDevicePath( UINT32 *index, CHAR16 *fileName );
EFI_DEVICE_PATH_PROTOCOL* _DiscoverPartition(EFI_DEVICE_PATH_PROTOCOL *DevicePath);
VOID        _BootSetBootManagerVariables(VOID);
VOID        _BootInstallLoadOptions( EFI_HANDLE handle, VOID *Options, UINTN OptionSize );
EFI_STATUS  _BootLaunchDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevicePath, VOID *Options, UINTN OptionSize, BOOLEAN ValidBootOption );
UINT16      _BootSetBootNowCount(VOID);
CHAR16      *TseGetUefiDevPathString(EFI_DEVICE_PATH_PROTOCOL *DevPath);
VOID        InvalidateStatusInBgrtWrapper (VOID);
VOID        UninstallBgrtWrapper(VOID);
VOID        FormHotBootKeys (CHAR16 *VarName);
BOOT_DATA *   BootGetBootDatafororphan(UINT16 u16Option);
VOID        RefreshBootKeysDetails (VOID);
VOID        FreeExtraKeyMemories (VOID);
EFI_STATUS	CheckForDeviceNeedRepair (EFI_DEVICE_PATH_PROTOCOL *);
EFI_HANDLE	CheckDevSupShortFormPath (EFI_DEVICE_PATH_PROTOCOL *);
VOID            LegacyBootFailHookWrapper(EFI_STATUS);
VOID            UefiBootFailHookWrapper (EFI_STATUS);
VOID			UpdateBootVariables ();
VOID			FixHiddenOptions (BOOLEAN, UINT16 **, UINTN);
VOID 			_SetDriverManagerVariables (VOID);
VOID 			UpdateDriverVariables ();
UINT32  		ShellTextMode (VOID);
VOID 			 ReInstallBgrtWrapper ();
UINT32 		SkipOrphanBootOrderVar();
BOOLEAN TSESkipBootOrderVar(UINT16 *BootOrder, UINTN BootOrderSize,CHAR16 *VarName);
CHAR16* GetFileNameFromDevPath( EFI_DEVICE_PATH_PROTOCOL *DevPath);
BOOLEAN TSEIsBootOptionValid(BOOT_OPTION* Buffer, UINTN Size);
BOOLEAN IsURIDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath);
BOOLEAN IsMediaFileDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath);

VOID *GetLoadOptionFileBuffer(EFI_DEVICE_PATH_PROTOCOL *FilePath,EFI_DEVICE_PATH_PROTOCOL **FullPath,UINTN *FileSize);
VOID *GetFileBufferFromFilePath(BOOLEAN BootPolicy, EFI_DEVICE_PATH_PROTOCOL *FilePath ,UINTN *FileSize);
EFI_DEVICE_PATH_PROTOCOL *ExpandLoadFile(EFI_HANDLE LoadFileHandle,EFI_DEVICE_PATH_PROTOCOL *FilePath);
EFI_DEVICE_PATH_PROTOCOL * GetRamDiskDevicePath (EFI_DEVICE_PATH_PROTOCOL *FilePath );
EFI_DEVICE_PATH_PROTOCOL * GetLoadOptionDevicePath(EFI_DEVICE_PATH_PROTOCOL *FilePath,EFI_DEVICE_PATH_PROTOCOL*FullPath);
EFI_DEVICE_PATH_PROTOCOL *ExpandUriDevicePath(EFI_DEVICE_PATH_PROTOCOL *FilePath,EFI_DEVICE_PATH_PROTOCOL*FullPath);
EFI_DEVICE_PATH_PROTOCOL *ExpandNetWorkFileSystem(EFI_HANDLE LoadFileHandle ,EFI_HANDLE *RamDiskHandle);
EFI_DEVICE_PATH_PROTOCOL *ExpandMediaDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath,EFI_DEVICE_PATH_PROTOCOL *FullPath);
BOOLEAN MatchHttpDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevicePath1,EFI_DEVICE_PATH_PROTOCOL *DevicePath2);
void DestoryRamDisk(EFI_DEVICE_PATH_PROTOCOL *RamDiskDevicePath);
VOID *EFIAPI AllocateReservedPages (  IN UINTN  Pages  );
VOID EFIAPI FreePages (   IN VOID   *Buffer,   IN UINTN  Pages   );
VOID *GetRamDiskMemoryInfo (EFI_DEVICE_PATH_PROTOCOL *RamDiskDevicePath, UINTN *RamDiskSizeInPages );
BOOLEAN IsLoadOptionPeHeaderValid (EFI_BOOT_MANAGER_LOAD_OPTION_TYPE Type,VOID *FileBuffer,UINTN FileSize );
EFI_STATUS ConnectDeviceonFailure(EFI_DEVICE_PATH_PROTOCOL *DevicePath);
EFI_RAM_DISK_PROTOCOL *gRamDiskProtocol = NULL;
extern CHAR16 *BootGetBootOptionNameUEFIAndLegacy( BOOT_DATA *bootData);
BOOLEAN IsPxeHttpDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath);
extern UINTN gSetupStatus;
BOOLEAN PcdReconnectPxe(VOID);

/**
    This function collects all the boot options, both efi
    and legacy boot options, that are present in the
    system and sets various boot manager variables that
    are used to expand boot manager questions.

    @param VOID

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS BootGetBootOptions( VOID )
{
    EFI_STATUS	Status = EFI_SUCCESS;
    UINTN		AllocatedCount;

#ifndef STANDALONE_APPLICATION
    CHAR16		Pattern[]=L"boot[0-9a-f][0-9a-f][0-9a-f][0-9a-f]";
    CHAR16		KeyPattern[]=L"Key[0-9a-f][0-9a-f][0-9a-f][0-9a-f]";
    UINTN		OldVarNameSize;
    UINTN		VarNameSize;
    CHAR16		*VarName;
    EFI_GUID	VarGuid;
	VOID *		UnicodeInterface;
    UINTN		size=0, Length = 2, BootOrderSize = 0;
    UINT8		*bufPtr;
    BOOT_OPTION *buffer;
    BOOT_DATA   *dataPtr = NULL;
	UINT32      *conditionPtr;
	UINT16      *BootOrder = NULL;
    CHAR16      *String=NULL;
//    UINTN       KeyVarNameSize = 0;
#endif //STANDALONE_APPLICATION
	BOOLEAN IsFastBoot=FALSE;
	UINTN		DriverAllocatedCount;
    CHAR16		DriverPattern[] = L"Driver[0-9a-f][0-9a-f][0-9a-f][0-9a-f]";
	BOOLEAN 	BootOption = FALSE;
	BOOLEAN 	DriverOption = FALSE;
	UINT16 		jIndex 		= 	0;
	CHAR16		OSRecoveryPattern[] = L"OsRecovery[0-9a-f][0-9a-f][0-9a-f][0-9a-f]";
	CHAR16		PlatformPattern[] = L"PlatformRecovery[0-9a-f][0-9a-f][0-9a-f][0-9a-f]";
	UINTN 		OSRecoveryAllocCount;
	UINTN 		PlatformOptionAllocCount;
	UINT16 		*PlatformRecoveryOrder = NULL;
	UINT16 		Index = 0;
	
	DISABLED_BOOT_OPTION 	= 	0xFFFF;
	DISABLED_DRIVER_OPTION 	= 	0xFFFF;
	DriverAllocatedCount = gDriverOptionCount = 0;
   AllocatedCount = gBootOptionCount = 0;
   OSRecoveryAllocCount = gOSRecoveryOptionCount = 0;
   PlatformOptionAllocCount = gPlatformRecoveryOpCount = 0;
   
#ifndef STANDALONE_APPLICATION
	Status = InitUnicodeCollectionProtocol(&UnicodeInterface);
    if(EFI_ERROR(Status))
        return Status;

    RefreshBootKeysDetails ();
    //start with a size of 80 bytes
    VarNameSize = 80;
    OldVarNameSize = VarNameSize;
    VarName = EfiLibAllocateZeroPool(VarNameSize);
    conditionPtr = VarGetNvramName( L"BootFlow", &_gBootFlowGuid, NULL, &size );
    if (conditionPtr != NULL)
	{
        if (BOOT_FLOW_CONDITION_FAST_BOOT == *conditionPtr)
		{
            IsFastBoot = TRUE;
            BootOrder = HelperGetVariable(VARIABLE_ID_BOOT_ORDER, L"BootOrder", &gEfiGlobalVariableGuid, NULL, &BootOrderSize );
            VarNameSize = ((EfiStrLen(L"BOOTXXXX")+1)*2);

            if(VarName)
                MemFreePointer( (VOID **)&VarName );

            VarName = EfiLibAllocateZeroPool(VarNameSize);
            if(BootOrder)
            {
                SPrint (VarName, VarNameSize, L"Boot%04X",BootOrder[0] );
                MemFreePointer((VOID **) &BootOrder);
            }
		}
	    MemFreePointer((VOID **) &conditionPtr);
	}
   size = 0;
   BootOrderSize = 0;
	BootOrder = VarGetNvramName (L"BootOrder", &gEfiGlobalVariableGuid, NULL, &BootOrderSize);
	
    do
    {
		if (!IsFastBoot)
		{
			Status = gRT->GetNextVariableName (&VarNameSize,
											VarName,
											&VarGuid);
			if (Status == EFI_BUFFER_TOO_SMALL)
			{
				//Allocate correct size
				VarName = MemReallocateZeroPool (VarName,
												OldVarNameSize,
												VarNameSize);
				OldVarNameSize = VarNameSize;
				continue;
			}
		}
        //KeyVarNameSize = VarNameSize;
        if ( (VarNameSize == ((EfiStrLen (L"BOOTXXXX")+1)*2)) || (VarNameSize == ((EfiStrLen (L"DRIVERXXXX")+1)*2))
      	  || (VarNameSize == ((EfiStrLen (L"OsRecoveryXXXX")+1)*2)) || (VarNameSize == ((EfiStrLen (L"PlatformRecoveryXXXX")+1)*2)))	        //Find if this variable is one of L"BOOTXXXX" or L"DRIVERXXXX"
        {
			if (MetaiMatch (UnicodeInterface, VarName, Pattern))											//The buffer is definatly a boot#### option
			{
                size = 0;
                buffer = VarGetNvramName (VarName, &gEfiGlobalVariableGuid, NULL, &size);
                if (buffer)																					//Variable exists. Store details in global array.
                {
                	if (TRUE == TSESkipBootOrderVar(BootOrder, BootOrderSize,VarName))
                	{	
                		if(SkipOrphanBootOrderVar())
                		{
                			goto _NextVariable;
                		}
                		if(!TSEIsBootOptionValid(buffer, size))
                		{
                			SETUP_DEBUG_TSE( "\n[TSE] Excluding a corrupted or non-efi orphan boot variable %s\n", VarName );     
                			goto _NextVariable;
                		}				
                		
                	}
                    if (gBootOptionCount >= AllocatedCount)													//Check if enough size has been already allocated
                    {
                        gBootData = MemReallocateZeroPool (gBootData, AllocatedCount * sizeof(BOOT_DATA), (AllocatedCount + BOOT_OPTION_ALLOC_UNIT) * sizeof(BOOT_DATA));
                        if (NULL == gBootData)
						{
							MemFreePointer((void**)&VarName);//For MemoryLeak Fix 
                            return EFI_UNSUPPORTED;
						}
                        AllocatedCount += BOOT_OPTION_ALLOC_UNIT;
                    }
					dataPtr = &(gBootData [gBootOptionCount]);
					HexStringToBuf(
						(UINT8 *)(&(dataPtr->Option)),
						&Length,
						&(VarName[4]),
						NULL);
					BootOption = TRUE;
					gBootOptionCount ++;
				}
			}
			else if (MetaiMatch (UnicodeInterface, VarName, DriverPattern))
			{
                size = 0;
                buffer = VarGetNvramName (VarName, &gEfiGlobalVariableGuid, NULL, &size);
                if (buffer)		//Variable exists. Store details in global array.
                {
                    if (gDriverOptionCount >= DriverAllocatedCount)		//Check if enough size has been already allocated
                    {
                        gDriverData = MemReallocateZeroPool (gDriverData, DriverAllocatedCount * sizeof (BOOT_DATA), (DriverAllocatedCount + BOOT_OPTION_ALLOC_UNIT) * sizeof(BOOT_DATA));
                        if (NULL == gDriverData)
						{
							MemFreePointer((void**)&VarName);//For MemoryLeak Fix 
                            return EFI_UNSUPPORTED;
						}	
                        DriverAllocatedCount += BOOT_OPTION_ALLOC_UNIT;
                    }
					dataPtr = &(gDriverData [gDriverOptionCount]);
					HexStringToBuf(
						(UINT8 *)(&(dataPtr->Option)),
						&Length,
						&(VarName[6]),
						NULL);
					DriverOption = TRUE;
					gDriverOptionCount ++;
				}
			}
			else if (IsRecoverySupported() && (MetaiMatch (UnicodeInterface, VarName, OSRecoveryPattern)))											//The buffer is definatly a boot#### option
			{
				 size = 0;
				 buffer = VarGetNvramName (VarName, &gEfiGlobalVariableGuid, NULL, &size);
				 if (buffer)																					//Variable exists. Store details in global array.
				 {
					  if (gOSRecoveryOptionCount >= OSRecoveryAllocCount)													//Check if enough size has been already allocated
					  {
							gOSRecoveryData = MemReallocateZeroPool (gOSRecoveryData, OSRecoveryAllocCount * sizeof(BOOT_DATA), (OSRecoveryAllocCount + BOOT_OPTION_ALLOC_UNIT) * sizeof(BOOT_DATA));
							if (NULL == gOSRecoveryData)
							{
								 MemFreePointer((void**)&VarName);//For MemoryLeak Fix 
								 return EFI_UNSUPPORTED;
							}
							OSRecoveryAllocCount += BOOT_OPTION_ALLOC_UNIT;
					  }
					  dataPtr = &(gOSRecoveryData [gOSRecoveryOptionCount]);
					  HexStringToBuf(
							  (UINT8 *)(&(dataPtr->Option)),
							  &Length,
							  &(VarName[10]),
							  NULL);
					  DriverOption = TRUE;
					  gOSRecoveryOptionCount ++;
				 }
			}
			else if (IsRecoverySupported() && (MetaiMatch (UnicodeInterface, VarName, PlatformPattern)))											//The buffer is definatly a boot#### option
			{
				 size = 0;
				 buffer = VarGetNvramName (VarName, &gEfiGlobalVariableGuid, NULL, &size);
				 if (buffer)																					//Variable exists. Store details in global array.
				 {
					  if (gPlatformRecoveryOpCount >= PlatformOptionAllocCount)													//Check if enough size has been already allocated
					  {
							gPlatformRecoveryData = MemReallocateZeroPool (gPlatformRecoveryData, PlatformOptionAllocCount * sizeof(BOOT_DATA), (PlatformOptionAllocCount + BOOT_OPTION_ALLOC_UNIT) * sizeof(BOOT_DATA));
							if (NULL == gPlatformRecoveryData)
							{
								 MemFreePointer((void**)&VarName);//For MemoryLeak Fix 
								 return EFI_UNSUPPORTED;
							}
							PlatformOptionAllocCount += BOOT_OPTION_ALLOC_UNIT;
					  }
					  dataPtr = &(gPlatformRecoveryData [gPlatformRecoveryOpCount]);
					  HexStringToBuf(
						(UINT8 *)(&(dataPtr->Option)),
						&Length,
						&(VarName[16]),
						NULL);
					  DriverOption = TRUE;
					  gPlatformRecoveryOpCount++;
				 }
			}
			else
			{
				goto _NextVariable;					//Jumping for next variable iteration
			}
			if ((DriverOption || BootOption) && (dataPtr != NULL))			//Common code for both driver and boot options
			{
				dataPtr->Active = buffer->Active;
				dataPtr->Name = StrDup (buffer->Name);
				bufPtr = (UINT8 *)((CHAR16 *)buffer->Name + EfiStrLen( buffer->Name ) + 1);
				dataPtr->DevicePath = EfiLibAllocateZeroPool( buffer->PathLength );
				if (NULL != dataPtr->DevicePath)
				{
					MemCopy (dataPtr->DevicePath, bufPtr, buffer->PathLength);
				}
				bufPtr += buffer->PathLength;
				size -= (UINTN)bufPtr - (UINTN)buffer;
				if (size != 0)
				{
					dataPtr->LoadOptions = EfiLibAllocatePool( size );
					if ( dataPtr->LoadOptions != NULL )
					{
						dataPtr->LoadOptionSize = size;
						MemCopy( dataPtr->LoadOptions, bufPtr, size );
					}
				}
				if (BootOption)
				{
	                //In case of DEVIE PATH support, to reset the name using device path
	                String = TseGetUefiDevPathString(dataPtr->DevicePath);
	                if (NULL != String)
					{
	                    // For UEFI boot options, Prefix the string with "UEFI: "
	                    MemFreePointer( (VOID **)&(dataPtr->Name) );
	                    dataPtr->Name = EfiLibAllocateZeroPool( (EfiStrLen( String ) + EfiStrLen(L"UEFI: ")+ 1) * sizeof(CHAR16));
	                    StrCpyS(dataPtr->Name, (EfiStrLen( String ) + EfiStrLen(L"UEFI: ")+ 1), L"UEFI: ");
						 StrCatS(dataPtr->Name, (EfiStrLen( String ) + EfiStrLen(L"UEFI: ")+ 1), String);
	                    

	                }
	            }
				MemFreePointer( (VOID **)&buffer );
			}
_NextVariable:
			BootOption = DriverOption = FALSE;		//Getting ready for next iteration
        }
        else if (VarNameSize == ((EfiStrLen(L"keyXXXX")+1)*2))		//Boot, Driver and Key are different length so handling in else if cases
        {
            if (MetaiMatch (UnicodeInterface, VarName, KeyPattern))
            {
                FormHotBootKeys (VarName);
            }
        }
		// Only one Boot options is valid in FastBoot case
		if(IsFastBoot)
			break;
        //reset the size of this buffer to what has been allocated for it
        VarNameSize = OldVarNameSize;

    }while(Status != EFI_NOT_FOUND);
	MemFreePointer( (VOID **)&BootOrder );

#endif //STANDALONE_APPLICATION

	//For Boot#### variable
	for ( jIndex = 0; jIndex < gBootOptionCount; jIndex++ )
	{
		if ( gBootData[jIndex].Option == DISABLED_BOOT_OPTION )
		{
			DISABLED_BOOT_OPTION--;//Changing the value based on the available of BOOT#### variable
			jIndex	= 0xffff;
			continue;
		}
	}
	//For Driver#### variable
	for ( jIndex = 0; jIndex < gDriverOptionCount; jIndex++ )
	{
		if ( gDriverData[jIndex].Option == DISABLED_DRIVER_OPTION )	
		{
			DISABLED_DRIVER_OPTION--;//Changing the value based on the available of DRIVER#### variable
			jIndex	= 0xffff;
			continue;
		}
	}
    //Free unused memory
    if (gBootOptionCount < AllocatedCount)
        gBootData = MemReallocateZeroPool(gBootData, AllocatedCount * sizeof (BOOT_DATA), gBootOptionCount * sizeof (BOOT_DATA));

    if (gDriverOptionCount < DriverAllocatedCount)
        gDriverData = MemReallocateZeroPool(gDriverData, DriverAllocatedCount * sizeof (BOOT_DATA), gDriverOptionCount * sizeof (BOOT_DATA));

    if (gOSRecoveryOptionCount < OSRecoveryAllocCount)
		  gOSRecoveryData = MemReallocateZeroPool (gOSRecoveryData, OSRecoveryAllocCount * sizeof(BOOT_DATA), gOSRecoveryOptionCount * sizeof(BOOT_DATA));
    
    if (gPlatformRecoveryOpCount < PlatformOptionAllocCount)
		  gPlatformRecoveryData = MemReallocateZeroPool (gPlatformRecoveryData, PlatformOptionAllocCount * sizeof(BOOT_DATA), gPlatformRecoveryOpCount * sizeof(BOOT_DATA));
  
    PlatformRecoveryOrder = EfiLibAllocateZeroPool(gPlatformRecoveryOpCount * sizeof(UINT16) );
    
    if(PlatformRecoveryOrder)
    {
		for (Index = 0; Index < gPlatformRecoveryOpCount; Index++) 
		{
			PlatformRecoveryOrder[Index] = gPlatformRecoveryData[Index].Option;
		}
		
		Status = VarSetNvramName(L"PlatformRecoveryOrder",
					&gPlatformRecoveryOrderGuid,
					EFI_VARIABLE_BOOTSERVICE_ACCESS,
					PlatformRecoveryOrder,
					gPlatformRecoveryOpCount * sizeof(UINT16));
		
		MemFreePointer( (VOID **)&PlatformRecoveryOrder );
    }

    FreeExtraKeyMemories ();
    //Set boot manager variables
	if(!IsFastBoot)
	{
	    _BootSetBootManagerVariables ();
		_SetDriverManagerVariables ();
	}
	MemFreePointer((void**)&VarName);//For MemoryLeak Fix 
    return Status;
}

/**
    This function launches the boot option supplied

    @param u16Option: BootOption to be launched
              pOrder: The order in which to set BBS priorities
              u16OrderCount: No of options in pOrder

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS BootLaunchBootOption(UINT16 u16Option, UINT16 *pOrder, UINTN u16OrderCount)
{
    EFI_STATUS	status = EFI_NOT_FOUND;
    UINT16		u16BootCurrent;
    BOOLEAN 	FreeBootData = FALSE;
    BOOT_DATA	*pBootData = NULL;
    SETUP_DEBUG_TSE("\n[TSE] BootLaunchBootOption Entering :\n");
    if(gRecoveryBootingType == BOOT_OPTION_BOOTING)
    {
    	pBootData = BootGetBootData(u16Option); // Normal bootoption
		if(!pBootData)
		{
		   SETUP_DEBUG_TSE("[TSE] Boot option(%d) not present in gBootData. Checking whether it is orphan option...\n",u16Option);
    	   pBootData = BootGetBootDatafororphan(u16Option);
		   FreeBootData = TRUE;
		}
    }
    else
    {
    	pBootData = RecoveryGetRecoveryData(u16Option, gRecoveryBootingType); // Recovery Bootoption
    }
    
    if (pBootData)
    {
        //Set BootCurrent
        u16BootCurrent = pBootData->Option;
        VarSetNvramName(L"BootCurrent",
                &gEfiGlobalVariableGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                &(u16BootCurrent),
                sizeof(u16BootCurrent));
        if( gBootFlow != BOOT_FLOW_CONDITION_FAST_BOOT) 
		{
        	CsmBBSSetBootPriorities(pBootData, pOrder, u16OrderCount);
        }
status = _BootLaunchDevicePath(pBootData->DevicePath, pBootData->LoadOptions, pBootData->LoadOptionSize,TRUE);

        //clear BootCurrent
        VarSetNvramName(L"BootCurrent",
                &gEfiGlobalVariableGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                &(u16BootCurrent),
                0);
    }
	if(FreeBootData && pBootData)
    {
        if(pBootData->DevicePath)
            MemFreePointer( (VOID **)&pBootData->DevicePath );
        if(pBootData->Name)
            MemFreePointer( (VOID **)&pBootData->Name );
        if(pBootData->LoadOptions)
            MemFreePointer( (VOID **)&pBootData->LoadOptions );
        MemFreePointer( (VOID **)&pBootData );
    }
    SETUP_DEBUG_TSE("[TSE] BootLaunchBootOption Exiting with status = %r\n\n",status);
    return status;
}

EFI_STATUS BBSSetBootNowPriorityForBootNext(UINT16 u16Option, UINT16 *pOrder, UINTN u16OrderCount)
{
    EFI_STATUS	status = EFI_NOT_FOUND;
    UINT16		u16BootCurrent;
    BOOT_DATA	*pBootData = NULL;
	BOOLEAN 	FreeBootData = FALSE;
    
    pBootData = BootGetBootData(u16Option);
    if(pBootData == NULL)
    {
		SETUP_DEBUG_TSE("[TSE] In BBSSetBootNowPriorityForBootNext(), Boot option(%d) not present in gBootData. Checking whether it is orphan option...\n",u16Option);
        pBootData = BootGetBootDatafororphan(u16Option);
		FreeBootData = TRUE;
    }
    if (pBootData)
    {  
        //Set BootCurrent
        u16BootCurrent = pBootData->Option;
        VarSetNvramName(L"BootCurrent",
                &gEfiGlobalVariableGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                &(u16BootCurrent),
                sizeof(u16BootCurrent));
        if( gBootFlow != BOOT_FLOW_CONDITION_FAST_BOOT)
        {

            gBootNextFlag = TRUE;  	   
            CsmBBSSetBootNowPriority(pBootData,0,gShowAllBbsDev);
            gBootNextFlag = FALSE;
        }

        status = _BootLaunchDevicePath(pBootData->DevicePath, pBootData->LoadOptions, pBootData->LoadOptionSize,TRUE);

        //clear BootCurrent
        VarSetNvramName(L"BootCurrent",
                &gEfiGlobalVariableGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                &(u16BootCurrent),
                0);
        if(FreeBootData && pBootData  )
        {
            if(pBootData->DevicePath)
                MemFreePointer( (VOID **)&pBootData->DevicePath );
            if(pBootData->Name)
                MemFreePointer( (VOID **)&pBootData->Name );
            if(pBootData->LoadOptions)
                MemFreePointer( (VOID **)&pBootData->LoadOptions );
            MemFreePointer( (VOID **)&pBootData );
        }
    }
    return status;
}



/**
    Finds and returns internal data structure BOOT_DATA
    for a given boot option number.

    @param Option: Option number for which BOOT_DATA is needed

    @retval Returns pointer to BOOT_DATA if found. Returns NULL
        if BOOT_DATA not found.

**/
BOOT_DATA *BootGetBootData( UINT16 Option )
{
    UINTN i;

    for ( i = 0; i < gBootOptionCount; i++ )
    {
        if ( gBootData[i].Option == Option )
            return &gBootData[i];
    }

    return NULL;
}

/**
    Finds and returns internal data structure BOOT_DATA
    for a given Driver option number.

    @param Option: Option number for which BOOT_DATA is needed

    @retval Returns pointer to BOOT_DATA if found. Returns NULL
        if BOOT_DATA not found.

**/
BOOT_DATA *DriverGetDriverData (UINT16 Option)
{
    UINTN i;

    for ( i = 0; i < gDriverOptionCount; i++ )
    {
        if ( gDriverData[i].Option == Option )
            return &gDriverData[i];
    }
    return NULL;
}

/**
    Provides the unicode name for the given boot option
    in the form of internal data structure BOOT_DATA.

    @param bootData: BOOT_DATA struct for which unicode name is
        needed.

    @retval Returns unicode string corresponding to the provided
        BOOT_DATA. If the BOOT_DATA provided is for legacy
        option then the name for the first legacy option in
        that category is returned.

**/
CHAR16 *BootGetOptionName( BOOT_DATA *bootData)
{
    CHAR16 *BootOptionName = NULL;
    if ( BBSValidDevicePath(bootData->DevicePath) ) {
        BootOptionName= CsmBBSBootOptionName(bootData);
    }
    if(BootOptionName == NULL)
        BootOptionName = BootGetBootOptionNameUEFIAndLegacy(bootData);
    if(BootOptionName)
        return BootOptionName;    
    else
        return bootData->Name;
}

/**
    Provides the unicode name for the given driver option
    in the form of internal data structure BOOT_DATA.

    @param DriverData: BOOT_DATA struct for which unicode name is
        needed.

    @retval Returns unicode string corresponding to the provided BOOT_DATA

**/
CHAR16 *DriverGetOptionName (BOOT_DATA *DriverData)
{
    return DriverData->Name;
}

/**
    Provides the unicode name for the given boot option.

    @param value: boot option for which unicode name is needed

    @retval Returns unicode string corresponding to the provided
        boot option.

**/
CHAR16 *BootGetBootNowName( UINT16 value, BOOLEAN ShowAllBBSDev, BOOLEAN TseBootNowInBootOrde)
{
    UINT16	*BootOrder=NULL;
    UINTN	size = 0;
    UINTN	i,j,k;
    UINTN	count;

    BOOT_DATA *bootData;


    if(gBootData == NULL)
        return NULL;

    if((!ShowAllBBSDev) && (!TseBootNowInBootOrde))
    {
        // Here we are checking if value is within the Boot option count or not

        if(value < gBootOptionCount)
        {
            if(gLoadOptionHidden && (gBootData[value].Active & LOAD_OPTION_HIDDEN))
                return NULL;

            return BootGetOptionName(&(gBootData[value]));

        }
        else 
        {
            SETUP_DEBUG_TSE ("[TSE] value is not within Boot option count \n");
            ASSERT(0);
            return NULL;
        }
    }    

    if(TseBootNowInBootOrde)
    {
        BootOrder = HelperGetVariable(VARIABLE_ID_BOOT_ORDER, L"BootOrder", &gEfiGlobalVariableGuid, NULL, &size );
        if(NULL == BootOrder){
            ASSERT (0);
            return NULL;
        }

        //Find the first disabled option
        for ( i = 0; i < gBootOptionCount; i++ )
        {
            if ( DISABLED_BOOT_OPTION == BootOrder[i] ){


                //There are disabled options replace them with valid options
                for(j=0;j<gBootOptionCount;j++)
                {
                    for(k=0;k<gBootOptionCount;k++)
                    {
                        if(BootOrder[k] == gBootData[j].Option)
                            break;
                    }

                    if(k >= gBootOptionCount)
                    {
                        //gBootData[j].Option is not present in BootOrder; fill it
                        BootOrder[i] = gBootData[j].Option;
                        break;
                    }
                }
            }
        }
    }

    if(ShowAllBBSDev)
    {
        count = 0;
        for(i=0;i<gBootOptionCount;i++)
        {
            if(TseBootNowInBootOrde) {
                bootData = BootGetBootData(BootOrder[i]);
                if(NULL == bootData){
                    SETUP_DEBUG_TSE ("[TSE] Boot data is NULL\n");
                    ASSERT(0);
                    continue;
                }
            }
            else 
                bootData = gBootData + i;

            if ( BBSValidDevicePath(bootData->DevicePath) )
            {
                if((value >= count) && (value < (count+bootData->LegacyDevCount)))
                {
                    if(TseBootNowInBootOrde)
                        MemFreePointer((VOID **) &BootOrder);
                    if (gLoadOptionHidden && ((bootData->Active & LOAD_OPTION_HIDDEN)!= 0))
                        return NULL;

                    return bootData->OrderList[value-count].Name;
                }

                count+=bootData->LegacyDevCount;
            }
            else {
                if(value == count)
                {
                    if(TseBootNowInBootOrde)
                        MemFreePointer((VOID **) &BootOrder);
                    if (gLoadOptionHidden && ((bootData->Active & LOAD_OPTION_HIDDEN)!= 0))
                        return NULL;
                    return bootData->Name;
                }
                count++;
            }
        }
        return NULL;
    }
    else {  // !ShowAllBBSDev, TseBootNowInBootOrde = true;
        bootData = BootGetBootData(BootOrder[value]);
        MemFreePointer((VOID **) &BootOrder);

        if(NULL == bootData)	
        {
            SETUP_DEBUG_TSE ("[TSE] Boot data is NULL\n");
            ASSERT(0);
            return NULL;
        }
        if (gLoadOptionHidden && ((bootData->Active & LOAD_OPTION_HIDDEN)!= 0))
            return NULL;

        return BootGetOptionName(bootData);
    }

}

/**
    Finds the languages that the firmware supports.

    @param VOID

    @retval VOID

**/
VOID GetBootLanguages( VOID )
{
    CHAR16	*langString;
    CHAR8	*langCodes, *langPtr;
    UINTN	i, count, size = 0;

    langCodes = VarGetNvramName( L"LangCodes", &gEfiGlobalVariableGuid, NULL, &size );
    count = size /3;
    
    if(gLanguages)
    {
    	for(i=0;i<gLangCount;i++)
    	{
    		if(gLanguages[i].LangCode) 
    			MemFreePointer( (VOID **)&gLanguages[i].LangCode );
    		if(gLanguages[i].LangString)
    			MemFreePointer( (VOID **)&gLanguages[i].LangString  );
    		if(gLanguages[i].Unicode )
    			MemFreePointer( (VOID **)&gLanguages[i].Unicode );
    	}
    	MemFreePointer( (VOID **)&gLanguages );
    }

    if ( size == 0 )
    {
	    langCodes = VarGetNvramName( L"Lang", &gEfiGlobalVariableGuid, NULL, &size );
		if ( size != 0) {
		    count = 1;
		}
		else {
	        gLanguages = EfiLibAllocateZeroPool( sizeof(LANGUAGE_DATA) );
	        if ( gLanguages == NULL )
	            return;

			gLanguages[0].LangCode = StrDup8("eng");
			gLanguages[0].Unicode = StrDup(L"eng");
	        gLangCount = 1;
	        langString = HiiGetStringLanguage( (VOID*)(UINTN)INVALID_HANDLE, 1, gLanguages[0].Unicode );
	        gLanguages[0].Token = HiiAddString( gHiiHandle, langString );
	        gLanguages[0].LangString = langString;
	        return;
		}
    }

    gLanguages = EfiLibAllocateZeroPool( count * sizeof(LANGUAGE_DATA) );
    if ( gLanguages == NULL )
        return;

    for ( langPtr = langCodes, i = 0; i < count; i++, langPtr += 3 )
    {
		gLanguages[i].LangCode = EfiLibAllocateZeroPool( 4 * sizeof(CHAR8));
        MemCopy( gLanguages[i].LangCode, langPtr, 3 * sizeof(CHAR8) );
		gLanguages[i].Unicode = StrDup8to16(gLanguages[i].LangCode);
    }

    gLangCount = count;

    for ( langPtr = langCodes, i = 0; i < count; i++, langPtr += 3 )
    {
        langString = HiiGetStringLanguage( (VOID*)(UINTN)INVALID_HANDLE, 1, gLanguages[i].Unicode );
        gLanguages[i].Token = HiiAddString( gHiiHandle, langString );
        gLanguages[i].LangString = langString;
    }

    MemFreePointer( (VOID **)&langCodes );
}

/**
    Finds the languages that the firmware supports.

    @param VOID

    @retval VOID

**/
VOID GetPlatformBootLanguages( VOID )
{
    CHAR16	*langString;
    CHAR8	*langCodes, *langPtr;
    UINTN	i = 0, count=0, size = 0, pos = 0;

    langCodes = VarGetNvramName( L"PlatformLangCodes", &gEfiGlobalVariableGuid, NULL, &size );
    if(langCodes != NULL)
		count = GetTokenCount(langCodes);
    
    if(gLanguages)
	{
		 for(i=0;i<gLangCount;i++)
		 {
			if(gLanguages[i].LangCode) 
				MemFreePointer( (VOID **)&gLanguages[i].LangCode );
			if(gLanguages[i].LangString)
				MemFreePointer( (VOID **)&gLanguages[i].LangString  );
			if(gLanguages[i].Unicode )
				MemFreePointer( (VOID **)&gLanguages[i].Unicode );
		 }
		 MemFreePointer( (VOID **)&gLanguages );
	}
    
    if ((0 == size) || (NULL == langCodes))
    {
	    langCodes = VarGetNvramName( L"PlatformLang", &gEfiGlobalVariableGuid, NULL, &size );
	    if ( size  != 0 ) {
			count=1;
		}
		else {
		        gLanguages = (LANGUAGE_DATA *)EfiLibAllocateZeroPool( sizeof(LANGUAGE_DATA) );
	        if ( gLanguages == NULL )
	            return;

	        gLanguages[0].LangCode = StrDup8("en-US");
	        gLanguages[0].Unicode = StrDup(L"en-US");

	        gLangCount = 1;
	        langString = HiiGetStringLanguage( (VOID*)(UINTN)INVALID_HANDLE, 1, gLanguages[0].Unicode );
	        gLanguages[0].Token = HiiAddString( gHiiHandle, langString );
	        gLanguages[0].LangString = langString;
	        return;
	    }
	}

    gLanguages = EfiLibAllocateZeroPool( count * sizeof(LANGUAGE_DATA) );
    if ( gLanguages == NULL )
        return;

    for ( langPtr = langCodes, i = 0; i < count; i++ )
    {
        gLanguages[i].LangCode = GetTokenString(langPtr, &pos);
        gLanguages[i].Unicode  = StrDup8to16(gLanguages[i].LangCode);
        langString = HiiGetStringLanguage( INVALID_HANDLE, 1, gLanguages[i].Unicode );
        gLanguages[i].Token = HiiAddString( gHiiHandle, langString );
        gLanguages[i].LangString = langString;

        langPtr += pos + 1;
    }

    gLangCount = count;
    MemFreePointer( (VOID **)&langCodes );
}

/**
    Launches the boot option provided.

        index: The nth option in the boot now menu.
    @param ShowAllBbsDev SETUP_SHOW_ALL_BBS_DEVICES

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS BootLaunchBootNow( UINT16 index, BOOLEAN ShowAllBbsDev )
{

    EFI_STATUS	Status = EFI_OUT_OF_RESOURCES;
    BOOLEAN		bLegacyBoot = FALSE;
    UINT16		count = 0, BootCurrent;
    UINT16		*BootOrder;
    UINTN		i = 0;

    BOOT_DATA *bootData=NULL;

	ProcessProceedToBootNowHook();

    BootOrder = BootNowinBootOrderInit();

	if(ShowAllBbsDev)
	{
	    for(i=0;i<gBootOptionCount;i++)
	    {
			bootData = BootGetBootNowBootData(gBootData,BootOrder,i);
			if(NULL == bootData ) { 
				continue;
			}
	        if ( BBSValidDevicePath(bootData->DevicePath) )
	        {
	            if((index >= count) && (index < (count+bootData->LegacyDevCount)))
	            {
	                bLegacyBoot = TRUE;
	                break;
	            }
	            else
	                count=count+(UINT16)(bootData->LegacyDevCount);
	        }
	        else
	        {
	            if(index == count)
	                break;
	            count++;
	        }
	    }
	}
	else
	{
		if(0 == gBootOptionCount)
	        return EFI_NOT_FOUND;
		bootData = BootGetBootNowBootData(gBootData,BootOrder,index);
	}

	if(BootOrder!=NULL)
		MemFreePointer((VOID **) &BootOrder);

	if(NULL == bootData){
		return EFI_NOT_FOUND;
	}

	if(ShowAllBbsDev)
	{
	    if(i == gBootOptionCount)
	        return Status;

	    //Boot bootData and device index-count
	    //set bbs priorities
	    if(bLegacyBoot)
		{
			Status = CsmBBSSetBootNowPriority(bootData,index-count,ShowAllBbsDev);
			if(EFI_ERROR( Status )) 
				return EFI_UNSUPPORTED;
		}
	}

    //Set BootCurrent
    BootCurrent = bootData->Option;
    VarSetNvramName(L"BootCurrent",
            &gEfiGlobalVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
            &(BootCurrent),
            sizeof(BootCurrent));
	if(!ShowAllBbsDev)
	{
	    if(BBSValidDevicePath(bootData->DevicePath))
		{
	        Status = CsmBBSSetBootNowPriority(bootData,0,ShowAllBbsDev);
			if(EFI_ERROR( Status )) 
				return EFI_UNSUPPORTED;
		}	
	}
    Status = _BootLaunchDevicePath( bootData->DevicePath, bootData->LoadOptions, bootData->LoadOptionSize,TRUE );
    //clear BootCurrent
    VarSetNvramName(L"BootCurrent",
            &gEfiGlobalVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
            &(BootCurrent),
            0);

    return Status;
}

/**
    Launches the guided file from FV.

    @param guid: Guid of the file to be launched.

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS BootLaunchGuid( EFI_GUID *guid )
{
    EFI_STATUS	Status = EFI_UNSUPPORTED;
    UINT32		index = 0;

    EFI_DEVICE_PATH_PROTOCOL *devicePath;

    do
    {
        devicePath = _BootBuildFVDevicePath( &index, guid );
        if ( index != (UINT32)-1 )
            Status = _BootLaunchDevicePath( devicePath, NULL, 0,FALSE );

        MemFreePointer( (VOID **)&devicePath );
    }
    while ( ( EFI_ERROR( Status ) ) && ( index != (UINT32)-1 ) );

    return Status;
}

/**
    Launches the file mentioned in file path from the
    available FS.

    @param fileName: Path of the file to be launched.

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS BootLaunchFilename( CHAR16 *fileName )
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    UINT32     index = 0;

    EFI_DEVICE_PATH_PROTOCOL *devicePath;

    do
    {
        devicePath = _BootBuildFileDevicePath( &index, fileName );
        if ( index != (UINT32)-1 )
            Status = _BootLaunchDevicePath( devicePath, NULL, 0,FALSE );

        MemFreePointer( (VOID **)&devicePath );
    }
    while ( ( EFI_ERROR( Status ) ) && ( index != (UINT32)-1 ) );

    return Status;
}

/**
    Function to move the hidden option at last of the boot order

    @param UINT16 ** => Boot order to be reformed

    @retval VOID

**/
VOID FixHiddenOptions (BOOLEAN Option, UINT16 **Order, UINTN OptionCount)
{
	UINTN iIndex = 0;
	UINTN jIndex = 0;
	UINT16 TempBootOption = 0;
	BOOT_DATA   *pBootData;

	if (0 == OptionCount)
	{
		return;
	}
	for (iIndex = 0; iIndex < OptionCount-1; iIndex ++)
	{
		if (BOOT_ORDER_OPTION == Option)
		{
			pBootData = BootGetBootData ((*Order) [iIndex]);
		}
		else
		{
			pBootData = DriverGetDriverData ((*Order) [iIndex]);
		}
		if (pBootData)
		{
			if (pBootData->Active & LOAD_OPTION_HIDDEN)
			{
				for (jIndex = iIndex+1; jIndex < OptionCount; jIndex ++)
				{
					if (BOOT_ORDER_OPTION == Option)
					{
						pBootData = BootGetBootData ((*Order) [jIndex]);
					}
					else
					{
						pBootData = DriverGetDriverData ((*Order) [jIndex]);
					}
					if (!pBootData) // skip to the next option if pBootData is no good
					     continue;
					if (!(pBootData->Active & LOAD_OPTION_HIDDEN))
					{
						TempBootOption = (*Order) [iIndex];
						(*Order) [iIndex] = (*Order) [jIndex];
						(*Order) [jIndex] = TempBootOption;
						break;
					}
				}
			}
		}
	}
}

/**
    function to set the variables for the boot manager

    @param void

    @retval void

**/
VOID _BootSetBootManagerVariables(VOID)
{
    UINT16	*NewBootOrder, *BootOrder, count;
    UINTN	size=0, i=0, NewBootOrderCount=0, k=0, BootOrderCount;

    EFI_GUID 	BootManGuid = BOOT_MANAGER_GUID;
    BOOT_DATA   *pBootData;

    //Correct BootOrder variable if necessary
    NewBootOrder = NULL;
    if(gBootOptionCount)
        NewBootOrder = (UINT16 *)EfiLibAllocateZeroPool(gBootOptionCount * sizeof(UINT16));

    BootOrder = (UINT16 *)VarGetNvramName( L"BootOrder", &gEfiGlobalVariableGuid, NULL, &size );
    BootOrderCount = size/sizeof(UINT16);

    //Get all the enabled boot options in the boot order
    for(i = 0; i < BootOrderCount; i++)
    {
        pBootData = BootGetBootData(BootOrder[i]);
        if(pBootData)
        {
	  	    if(IsPreservedDisabledBootOptionOrder())
		    {
				/*if(gLoadOptionHidden && (pBootData->Active & LOAD_OPTION_HIDDEN))
				{
					NewBootOrderCount++;
					continue;
				}*/
				NewBootOrder[NewBootOrderCount] = BootOrder[i];
	   			NewBootOrderCount++;
		    }
		    else if(pBootData->Active & LOAD_OPTION_ACTIVE)
	        {
				/*if(gLoadOptionHidden && (pBootData->Active & LOAD_OPTION_HIDDEN))
				{
					NewBootOrderCount++;
					continue;
				}*/
	            NewBootOrder[NewBootOrderCount] = BootOrder[i];
	            NewBootOrderCount++;
	        }
        }
    }

    //Append all options that are enabled but not included in
    //BootOrder. FCFS used.
    for(i=0; i < gBootOptionCount; i++)
    {
        if(gBootData[i].Active & LOAD_OPTION_ACTIVE)
        {
            //Check presence in boot order
            for(k=0;k<BootOrderCount;k++)
            {
                if(BootOrder[k] == gBootData[i].Option)
                    break;
            }
            if(k >= BootOrderCount)
            {
                //Not present in boot order! Add option
                NewBootOrder[NewBootOrderCount] = gBootData[i].Option;
                NewBootOrderCount++;
            }
        }
    }

    //Free Boot order
    MemFreePointer((VOID **)&BootOrder);

    if(!IsPreservedDisabledBootOptionOrder())
    {
    	//Put disabled options at the end of NewBootOrder
    	for(i=0; i < gBootOptionCount; i++)
    	{
    		if(!(gBootData[i].Active & LOAD_OPTION_ACTIVE) && (!(gBootData[i].Active & LOAD_OPTION_CATEGORY_APP)))
    		{
    			/*				if(gLoadOptionHidden && (gBootData[i].Active & LOAD_OPTION_HIDDEN))
				{
					NewBootOrderCount++;
					continue;
				}*/
    			NewBootOrder[NewBootOrderCount] = gBootData[i].Option;
    			NewBootOrderCount++;
    		}
    	}

    	//Count BootOption which is having Active flag only LOAD_OPTION_CATEGORY_APP.
    	for(i=0; i < gBootOptionCount; i++)
    	{
    	    if(!(gBootData[i].Active & LOAD_OPTION_ACTIVE) && (gBootData[i].Active & LOAD_OPTION_CATEGORY_APP))
    	    {
    	        NewBootOrder[NewBootOrderCount] = gBootData[i].Option;
    	        NewBootOrderCount++;
    	    }
    	}

    	ASSERT(NewBootOrderCount == gBootOptionCount);
    }

    //Set BootOrder Variable with corrected order
	if (gLoadOptionHidden)
	{
		FixHiddenOptions (BOOT_ORDER_OPTION, &NewBootOrder, gBootOptionCount);
	}
    VarSetNvramName(L"BootOrder",
        &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
        NewBootOrder,
        NewBootOrderCount * sizeof(UINT16));

    //Free NewBootOrder
    MemFreePointer((VOID **)&NewBootOrder);

    //Get BBS devices
    CsmBBSGetDeviceList();

    // Update the BootOrder,BBSOrder Cache From the NVRAM.
    UpdateBootVariables();

    //Set Boot manager variable
    count = (UINT16)gBootOptionCount;
    VarSetNvramName( L"BootManager", &BootManGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, &count, sizeof(count) );

    //Set Boot now count
	if(gShowAllBbsDev)
  		count = _BootSetBootNowCount();

    VarSetNvramName( L"BootNowCount", &gBootNowCountGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, &count, sizeof(count) );
	UpdateAddDeleteBootVar ();
}

/**
    Sets DriverManager variable for drivers count and sets DriverOrder

    @param void

    @retval void

**/
VOID _SetDriverManagerVariables (VOID)
{
	UINT16	*DriverOrder = NULL;
	UINT16	*NewDriverOrder = NULL;
	UINT16	DriverOrderCount = 0;
    UINTN	DriverOrderSize = 0;
	UINTN 	iIndex = 0;
	UINTN 	jIndex = 0;
	UINT16 	Count = 0;
    BOOT_DATA   *pDrvData;			//BOOT_DATA using same structure for driver options too
    EFI_GUID 	DrvMgrGuid = DRIVER_MANAGER_GUID;

	if (gDriverOptionCount)
	{	
	    NewDriverOrder = (UINT16 *)EfiLibAllocateZeroPool (gDriverOptionCount * sizeof(UINT16));
		if (NULL == NewDriverOrder)
		{
			return;
		}
	}
	DriverOrder = (UINT16 *)VarGetNvramName (L"DriverOrder", &gEfiGlobalVariableGuid, NULL, &DriverOrderSize);
	DriverOrderCount = (UINT16)(DriverOrderSize/sizeof(UINT16));
	if ((NULL == DriverOrder) || (0 == DriverOrderSize))
	{
		for (iIndex = 0; iIndex < gDriverOptionCount; iIndex++)			//Forming DriverOrder variable freshly
        {
			if (gDriverData [iIndex].Active & LOAD_OPTION_ACTIVE)
			{
				NewDriverOrder [Count] = gDriverData [iIndex].Option;
				Count ++;
			}
		}
		goto _FormDisableDevices;
	}
	else											//Forming DriverOrder variable with first active and non hidden drivers
	{
	    for (iIndex = 0; iIndex < DriverOrderCount; iIndex ++)
	    {
	        pDrvData = DriverGetDriverData (DriverOrder [iIndex]);
	        if (pDrvData)
	        {													
		    	if ( (pDrvData->Active & LOAD_OPTION_ACTIVE) )				
				{													//Checking for active drivers
	                NewDriverOrder [Count] = DriverOrder [iIndex];
	                Count ++;
	            }
	        }
	    }
	}
	//Append all options that are enabled but not included in
    //DriverOrder. Having same logic as of boot order.
    for(iIndex = 0; iIndex < gDriverOptionCount; iIndex ++)
    {
		if ( (gDriverData [iIndex].Active & LOAD_OPTION_ACTIVE) )
        {
            //Check presence in DriverOrder
            for (jIndex = 0; jIndex < DriverOrderCount; jIndex ++)
            {
                if (DriverOrder [jIndex] == gDriverData [iIndex].Option)
                    break;
            }
            if (jIndex >= DriverOrderCount)
            {
                //Not present in Driver order! Add option
                NewDriverOrder [Count] = gBootData[iIndex].Option;
                Count ++;
            }
        }
    }
_FormDisableDevices:			//Forming disabled drivers last in the newly formed DriverOrder variable.
	for (iIndex = 0; iIndex < gDriverOptionCount; iIndex ++)
	{
		if (!(gDriverData [iIndex].Active & LOAD_OPTION_ACTIVE))
		{	 
			NewDriverOrder [Count] = gDriverData [iIndex].Option;
			Count ++;
		}
	}
	if (gLoadOptionHidden)
	{
		FixHiddenOptions (DRIVER_ORDER_OPTION, &NewDriverOrder, gDriverOptionCount);
	}
	VarSetNvramName (
		L"DriverOrder",
		&gEfiGlobalVariableGuid,
		EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
		NewDriverOrder,
		gDriverOptionCount * sizeof(UINT16)
		);
    //Set Driver manager variable
    Count = (UINT16)gDriverOptionCount;
    VarSetNvramName (L"DriverManager", &DrvMgrGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, &Count, sizeof(Count));
    UpdateDriverVariables ();
	if (DriverOrder)
	{
		MemFreePointer ((VOID **)&DriverOrder);
	}
	if (NewDriverOrder)
	{
		MemFreePointer ((VOID **)&NewDriverOrder);
	}
	UpdateAddDeleteDriverVar ();
}

/**
    function to set boot new devices count

    @param void

    @retval void

**/
UINT16 _BootSetBootNowCount(VOID)
{
    UINT16 count = 0;
    UINTN i;

    BOOT_DATA *bootData;

    for( i=0; i<gBootOptionCount; i++)
    {
        bootData = &(gBootData[i]);

		if(BBSValidDevicePath(bootData->DevicePath))
        {
            count = count + bootData->LegacyDevCount;
        }
        else
            count++;
    }

    return count;
}

/**
    Clears the screen for shell boot

    @param EFI_EVENT , VOID *

    @retval VOID

**/
void ClearGrphxScreen (void);
VOID EFIAPI _ShellClearScreen (EFI_EVENT Event, VOID *Context)
{
	UINT32 shelltextmodetype;
	InvalidateStatusInBgrtWrapper();
	ClearGrphxScreen ();
	gST->ConOut->ClearScreen (gST->ConOut);
	gBS->CloseEvent (Event);
	gST->ConOut->EnableCursor (gST->ConOut, TRUE);

	if (Event != gShellLaunchEvent)				//One event will be closed in incoming fnc argument other we have to close it
	{
		gBS->CloseEvent (gShellLaunchEvent);
	}
	
	if (Event != gShell20LaunchEvent)			
	{
		gBS->CloseEvent (gShell20LaunchEvent);
	}
	
	if (IsTseWatchDogTimer() && gWatchDogTimerSet) 
	{
		gBS->SetWatchdogTimer(0, 0, 0, NULL); 		//Disables the watchdog timer
		gWatchDogTimerSet = FALSE;
	}

	gShellLaunchEvent = NULL;					//Make NULL otherwise we will try to close it after startimage
	gShell20LaunchEvent = NULL;
	
	shelltextmodetype = ShellTextMode();//OEM needs Token to assign the display mode of SHELL.
	
	if ( 0xFF != shelltextmodetype )
		gST->ConOut->SetMode( gST->ConOut, shelltextmodetype );	
}

/**
    Registers the shell guid

    @param VOID

    @retval VOID

**/
VOID _RegisterShellGuid (VOID)
{
	EFI_STATUS 	Status = EFI_UNSUPPORTED;
	VOID 		*Registration = NULL;
	EFI_GUID 	EfiShellInterfaceGuid = EFI_SHELL_PROTOCOL_GUID;
	EFI_GUID 	EfiShell20InterfaceGuid = EFI_SHELL2_0_FILE_GUID;

	Status = gBS->CreateEvent (
				EFI_EVENT_NOTIFY_SIGNAL, 
				EFI_TPL_CALLBACK,
				_ShellClearScreen,
				NULL,
				&gShellLaunchEvent);
	if (!EFI_ERROR (Status))
	{
		Status = gBS->RegisterProtocolNotify(
				&EfiShellInterfaceGuid,
				gShellLaunchEvent,
				&Registration
				);
	}
	Status = gBS->CreateEvent (
				EFI_EVENT_NOTIFY_SIGNAL, 
				EFI_TPL_CALLBACK,
				_ShellClearScreen,
				NULL,
				&gShell20LaunchEvent);
	if (!EFI_ERROR (Status))
	{
		Status = gBS->RegisterProtocolNotify(
				&EfiShell20InterfaceGuid,
				gShell20LaunchEvent,
				&Registration
				);
	}
}


/**
    function to launch the boot operation

    @param DevicePath VOID *Options, UINTN OptionSize, BOOLEAN ValidBootOption

    @retval status

**/
EFI_STATUS _BootLaunchDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevicePath, VOID *Options, UINTN OptionSize, BOOLEAN ValidBootOption )
{
    EFI_STATUS	Status;
    EFI_HANDLE	handle = NULL;
    BOOLEAN		FreeDevicePath = FALSE;
    EFI_EVENT	ReadyToBootEvent;
	EFI_GUID	Bootguid = AMITSE_EVENT_BEFORE_BOOT_GUID;
	EFI_GUID	AfterBootGuid = AMITSE_EVENT_AFTER_BOOT_GUID; 
	EFI_STATUS	WatchDogTimerStatus = EFI_UNSUPPORTED;
    EFI_TPL CurrentTpl;
	TSE_POST_STATUS	BackupPostStatus;
	EFI_LOADED_IMAGE_PROTOCOL *Image = NULL;
	UINT16 *BootCurrent = NULL;
	UINTN		size = 0;
    VOID  *FileBuffer=NULL;
    UINTN FileSize = 0;
    EFI_DEVICE_PATH_PROTOCOL  *RamDiskDevicePath = NULL;
    EFI_DEVICE_PATH_PROTOCOL  *FilePath =NULL;
  
#if !TSE_USE_EDK_LIBRARY    
#ifndef STANDALONE_APPLICATION
    EFI_CONSOLE_CONTROL_SCREEN_MODE ScreenMode = EfiConsoleControlScreenText;
#endif //STANDALONE_APPLICATION
#endif
    SETUP_DEBUG_TSE("\n[TSE] _BootLaunchDevicePath Entering : gBootFlow = %d\n", gBootFlow);
    if(DevicePath == NULL)
        return EFI_INVALID_PARAMETER;
    
#if TSE_DEBUG_MESSAGES
    DebugShowPrintBootOptionDetails(DevicePath);
#endif

    BackupPostStatus = gPostStatus; // Back it up and of boot fail restore it back
	gPostStatus = TSE_POST_STATUS_PROCEED_TO_BOOT;


    Status = TseEfiCreateEventReadyToBoot(
                EFI_TPL_CALLBACK,
                NULL,
                NULL,
                &ReadyToBootEvent
                );


    if (EFI_ERROR(Status))
        return Status;

    gBS->SignalEvent( ReadyToBootEvent );
    gBS->CloseEvent( ReadyToBootEvent );
	EfiLibReportStatusCode(EFI_PROGRESS_CODE, DXE_READY_TO_BOOT,0,NULL,NULL);

    TSEIDEPasswordFreezeDevices();

    if( gBootFlow == BOOT_FLOW_CONDITION_FAST_BOOT)
    {
		// Call the Hook and do the FastBoot
		if ( BBSValidDevicePath( DevicePath ) )
		{
			BeforeLegacyBootLaunchHook();
			UninstallBgrtWrapper();
		}
		else
		{
			BeforeEfiBootLaunchHook();							//Clearing the logo for shell in fast boot also 
			if (NULL != gShellLaunchEvent)				
			{
				gBS->CloseEvent (gShellLaunchEvent);
				gShellLaunchEvent = NULL;
			}
			if (NULL != gShell20LaunchEvent)					
			{
				gBS->CloseEvent (gShell20LaunchEvent);
				gShell20LaunchEvent = NULL;
			}
			// Validate the Gop before usage in all the possible cases and also get instance of Gop through notification 
			if ( ( NULL != gGOP ) && (CurrentScreenresolutionX && CurrentScreenresolutionY) && //If it has valid gGOP and resolution
				 ((CurrentScreenresolutionX != gGOP->Mode->Info->HorizontalResolution) || (CurrentScreenresolutionY != gGOP->Mode->Info->VerticalResolution)) //If current and quietboot resolution is different
				) 
			{
				GOPSetScreenResolution(&CurrentScreenresolutionX, &CurrentScreenresolutionY);
				//gGOP->SetMode (gGOP, CurrentResolutionIndex); // To maintain graphics resolution 
			}
			EfiLibNamedEventSignal (&Bootguid);
			_RegisterShellGuid ();
		}
		// Performance measurement Pause
		PERF_END (0, AMITSE_TEXT("Boot"), NULL, 0);
		SETUP_DEBUG_TSE("[TSE] Giving control to fast boot...\n");
		Status = FastBootLaunch();
		SETUP_DEBUG_TSE("[TSE] fast boot returned with status %r...\n", Status);
		if ( BBSValidDevicePath( DevicePath ) ) 
		{
			ReInstallBgrtWrapper (); // in fast boot, if its legacy then we will reinstall as we uninstalled bgrt
		}
		EfiLibNamedEventSignal (&AfterBootGuid);
		return Status;
    }
/*#if APTIO_4_00						//Useful for secure boot violation message box
	gMaxRows = MAX_ROWS;
	gMaxCols = MAX_COLS;
#endif*/
    if ( BBSValidDevicePath( DevicePath ) )			// Changing the mode only for csm
    {
        SETUP_DEBUG_TSE("[TSE]  Legacy Devicepath :\n");
#if !TSE_USE_EDK_LIBRARY
#ifndef STANDALONE_APPLICATION
	// Fast Boot May want to boot without Console Control
		if (gConsoleControl != NULL)
		{
		    gConsoleControl->GetMode (gConsoleControl, &ScreenMode, NULL, NULL);
		    if (ScreenMode != EfiConsoleControlScreenText)
			{
		        gConsoleControl->SetMode (gConsoleControl, EfiConsoleControlScreenText);
				InvalidateStatusInBgrtWrapper ();
			}
		}
#endif
#endif //STANDALONE_APPLICATION
        BeforeLegacyBootLaunchHook();
        UninstallBgrtWrapper();
        Status = CsmBBSLaunchDevicePath( DevicePath );
        ReInstallBgrtWrapper();
		if (EFI_ERROR (Status))
			LegacyBootFailHookWrapper(Status);
        AfterLegacyBootLaunchHook();
		gPostStatus = BackupPostStatus;

		EfiLibNamedEventSignal (&AfterBootGuid);
		SETUP_DEBUG_TSE("[TSE] _BootLaunchDevicePath Exiting :\n");
        return Status;
    }

    CurrentTpl = gBS->RaiseTPL( EFI_TPL_HIGH_LEVEL );
    gBS->RestoreTPL( EFI_TPL_APPLICATION );
    if (DevicePath->Type==MEDIA_DEVICE_PATH && DevicePath->SubType==MEDIA_HARDDRIVE_DP)
    {
        DevicePath = _DiscoverPartition(DevicePath);
        FreeDevicePath = TRUE;
    }
    else if(DevicePath->Type==MEDIA_DEVICE_PATH && DevicePath->SubType==MEDIA_FILEPATH_DP && (!gFileParsed))
    {
     	  CHAR16	*FileName = NULL;
    	   	  
     	  FileName = GetFileNameFromDevPath(DevicePath);
		  if(NULL != FileName)
		  {
  
          gFileParsed = TRUE;
          Status = BootLaunchFilename(FileName);
          MemFreePointer((VOID **) &FileName);
          if (EFI_ERROR (Status))
          {    
              FILEPATH_DEVICE_PATH  *Fp = NULL;
              Fp = (FILEPATH_DEVICE_PATH *)DevicePath;
              FileName = EfiLibAllocateZeroPool((StrLen(Fp->PathName)+1) * sizeof(CHAR16));
              if (FileName)
              {
                  StrCpyS(FileName, EfiStrLen(Fp->PathName)+1,Fp->PathName);
                  Status = BootLaunchFilename(FileName);
                  MemFreePointer((VOID **) &FileName);
              }
          }
          gPostStatus = BackupPostStatus;
          gFileParsed = FALSE;

	       
          if ( CurrentTpl > EFI_TPL_APPLICATION )
         	 gBS->RaiseTPL( CurrentTpl );
          
          return Status;
		  }
     }
    else if (IsURIDevicePath(DevicePath)) 
    {
       FileBuffer = GetLoadOptionFileBuffer(DevicePath,&FilePath,&FileSize);
       if(FileBuffer != NULL)
          RamDiskDevicePath = GetRamDiskDevicePath(FilePath);
       else
       {
          Status = EFI_NOT_FOUND;
       }
       if(FilePath != NULL)
           DevicePath = FilePath;       
    }
    if (IsPxeHttpDevicePath(DevicePath))
    {
        EfiLibNamedEventSignal (&Bootguid);
        BeforeEfiBootLaunchHook();
    }
    SETUP_DEBUG_TSE("[TSE] Before Validating Gop before LoadImage()\n",Status);
    PrintGopAndConInDetails();
    
    // Validate the Gop before usage in all the possible cases and also get instance of Gop through notification
    if ( ( NULL != gGOP ) && (CurrentScreenresolutionX && CurrentScreenresolutionY) && //If it has valid gGOP and resolution
         ((CurrentScreenresolutionX != gGOP->Mode->Info->HorizontalResolution) || (CurrentScreenresolutionY != gGOP->Mode->Info->VerticalResolution)) //If current and quietboot resolution is different
        ) 
    {
        GOPSetScreenResolution(&CurrentScreenresolutionX, &CurrentScreenresolutionY);
        //gGOP->SetMode (gGOP, CurrentResolutionIndex); // To maintain graphics resolution 
    }

    SETUP_DEBUG_TSE("[TSE] After Validating Gop before LoadImage()\n",Status);
    PrintGopAndConInDetails();

    EfiLibReportStatusCode(EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeOsLoaderLoad),0,NULL,NULL);
    SETUP_DEBUG_TSE("[TSE] DevicePath: %x,FileBuffer: %x,FileSize: %d\n",DevicePath,FileBuffer,FileSize);
    Status = gBS->LoadImage (TRUE, gImageHandle, DevicePath, FileBuffer, FileSize, &handle);
    SETUP_DEBUG_TSE("[TSE] gBS->LoadImage Status = %r\n",Status);

    if (EFI_ERROR (Status))
    {
        EFI_DEVICE_PATH_PROTOCOL    *TempDevicePath = DevicePath;
        EFI_STATUS RepairStatus = EFI_UNSUPPORTED;
        EFI_STATUS ConnectStatus = EFI_UNSUPPORTED;
		UefiBootFailHookWrapper(Status);
        RepairStatus = CheckForDeviceNeedRepair (TempDevicePath);           //support for UEFI specification v 2.3.1, p. 10.10.1 Driver health protocol
        SETUP_DEBUG_TSE("[TSE] RepairStatus = %r\n",RepairStatus);

        //check if device need connect
        TempDevicePath = DevicePath;
        
            if( PcdReconnectPxe()&& (gSetupStatus == TSE_SETUP_STATUS_BOOT_FROM_OVERRIDE) && (IsPxeHttpDevicePath(TempDevicePath)) && (EFI_ERROR (RepairStatus)))
            {
                ConnectStatus =  ConnectDeviceonFailure(TempDevicePath);

                SETUP_DEBUG_TSE("[TSE] ConnectStatus = %r\n",ConnectStatus);
            }
        
        if (!(EFI_ERROR (RepairStatus)) || !(EFI_ERROR (ConnectStatus)))
        {
            EfiLibReportStatusCode(EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeOsLoaderLoad),0,NULL,NULL);
            Status = gBS->LoadImage (TRUE, gImageHandle, DevicePath, NULL, 0, &handle);
            SETUP_DEBUG_TSE("[TSE] gBS->LoadImage after device repaired/Connected  Status = %r\n",Status);
			if (EFI_ERROR (Status))
			{
				UefiBootFailHookWrapper (Status);
			}
        }
    }
    if (EFI_ERROR (Status))
    {
        //Try default behaviour
        EFI_DEVICE_PATH_PROTOCOL    *TempDevicePath = NULL;
        EFI_HANDLE                  DevHandle;
        EFI_HANDLE                  UsbDevHandle = NULL;
        EFI_BLOCK_IO_PROTOCOL       *BlkIo = NULL;
        VOID                        *Buffer = NULL;

        // Find a Simple File System protocol on the device path.
        TempDevicePath = DevicePath;

        UsbDevHandle = CheckDevSupShortFormPath (DevicePath);       ////Support for section 3.1.2. booting from a short-form device path
        if (NULL != UsbDevHandle)
        {
            DevHandle = UsbDevHandle;
        }
        else
        {
            Status = gBS->LocateDevicePath (
                        &gEfiSimpleFileSystemProtocolGuid,
                        &TempDevicePath,
                        &DevHandle
                        );
        }
        if ((!EFI_ERROR (Status) && IsDevicePathEnd (TempDevicePath)) || (NULL != UsbDevHandle))
        {
        	if(IsMediaFileDevicePath(DevicePath))
        	{
        		FilePath = EfiLibAllocateZeroPool(EfiDevicePathSize(DevicePath));
        		if(FilePath)
        			MemCpy(FilePath,DevicePath,EfiDevicePathSize(DevicePath)); 
        	}
        	else
        	{     		  
        		// Files are specified in the device path so try to
        		// load the default removable media file name.
        		FilePath = EfiFileDevicePath (DevHandle, gBootFileName);
        	}
        	
			if (FilePath)
			{
                // Issue a dummy read to the device to check for media
                // change. When the removable media is changed, any Block
                // IO read/write will cause the BlockIo protocol be
                // reinstalled and EFI_MEDIA_CHANGED is returned. After
                // the Block IO protocol is reinstalled, subsequent Block
                // IO read/write will success.
                Status = gBS->HandleProtocol (
                                    DevHandle,
                                    &gEfiBlockIoProtocolGuid,
                                    (VOID **) &BlkIo
                                    );
                if (!EFI_ERROR (Status))
                {
                    Buffer = EfiLibAllocatePool (BlkIo->Media->BlockSize);
                    if (Buffer != NULL)
                    {
                        BlkIo->ReadBlocks (
                            BlkIo,
                            BlkIo->Media->MediaId,
                            0,
                            BlkIo->Media->BlockSize,
                            Buffer
                            );
                        MemFreePointer((VOID **)&Buffer);
                    }
                }
                EfiLibReportStatusCode(EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeOsLoaderLoad),0,NULL,NULL);

                Status = gBS->LoadImage (
                    TRUE,
                    gImageHandle,
                    FilePath,
                    NULL,
                    0,
                    &handle
                    );
                SETUP_DEBUG_TSE("[TSE] gBS->LoadImage for short Filepath Status = %r\n",Status);
				if (EFI_ERROR (Status))
				{
					UefiBootFailHookWrapper (Status);
				}
                MemFreePointer((VOID **) &FilePath);
            }
            else
            {
                Status = EFI_NOT_FOUND;
            }
        }
        else
        {
            Status = EFI_NOT_FOUND;
        }
    }


    if(ValidBootOption==TRUE)
        if (EFI_ERROR (Status))  // Report only if it Boot Option launch - Boot of loading Error!!!
            REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
                    EFI_ERROR_CODE| EFI_ERROR_MAJOR,
                    DXE_BOOT_OPTION_LOAD_ERROR,
                    &Status,
                    sizeof(EFI_STATUS));
        
    if (FreeDevicePath) gBS->FreePool(DevicePath);

    if ( ! EFI_ERROR( Status ) )
    {
		Status = gBS->HandleProtocol (
		 				  handle,
		                  &gEfiLoadedImageProtocolGuid,
		                  (VOID**)&Image
		                  );
		if (!EFI_ERROR (Status))
		{
			if(IsImageCodeTypeSupport())
			{
				if (Image->ImageCodeType  != EfiLoaderCode){
				    SETUP_DEBUG_TSE ("ERROR: Image referred by %x is not a UEFI application. Skipping...\n", Image->ImageCodeType);
					Status = EFI_UNSUPPORTED;
					gPostStatus = BackupPostStatus;
					if ( CurrentTpl > EFI_TPL_APPLICATION )
						gBS->RaiseTPL( CurrentTpl );
					return Status;
			    }
			}
		}
		if (gST->ConOut != NULL)
        {
            gST->ConOut->EnableCursor (gST->ConOut, FALSE);         //Disabling bcoz Cursor appears on the WIN8 boot
        }
        _BootInstallLoadOptions( handle, Options, OptionSize );

        // Signal AMITSE_EVENT_BEFORE_BOOT_GUID Event;
               if (!IsPxeHttpDevicePath(DevicePath))
               {
			       BeforeEfiBootLaunchHook();
                   EfiLibNamedEventSignal (&Bootguid);
                   
               }

		// Performance measurement Pause
		PERF_END (0, AMITSE_TEXT("Boot"), NULL, 0);

       
        _RegisterShellGuid ();						//clearing the screen if its shell boot

        if (IsTseWatchDogTimer()) 
        {
            BootCurrent = (UINT16 *)VarGetNvramName( L"BootCurrent", &gEfiGlobalVariableGuid, NULL, &size );

            if (BootCurrent) {
                BOOT_DATA	*pBootData = NULL;
                pBootData = BootGetBootData(*BootCurrent);

                if (pBootData && !(pBootData->Active & LOAD_OPTION_CATEGORY_APP))
                {
                    //=enabling the watch dog timer on load success==//
                    SETUP_DEBUG_TSE ("Load Image() Return Status = %r\n", Status); 
                    WatchDogTimerStatus = gBS->SetWatchdogTimer (5 * 60, 0, 0, NULL);
                    gWatchDogTimerSet = TRUE;
                    SETUP_DEBUG_TSE ("WatchDog Timer Status = %r\n", WatchDogTimerStatus); 
                }
                MemFreePointer( (VOID **)&BootCurrent );
            }
        }

		EfiLibReportStatusCode(EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeOsLoaderStart),0,NULL,NULL);
		Status = gBS->StartImage( handle, NULL, NULL );
		SETUP_DEBUG_TSE("\n[TSE] gBS->StartImage Status= %r\n",Status);		
		if (IsTseWatchDogTimer()) 
		{
			//====disabling the timer on start image failure==//
			if (!EFI_ERROR (WatchDogTimerStatus) && gWatchDogTimerSet)
			{
			    SETUP_DEBUG_TSE ("Start Image() Return Status = %r\n", Status); 
				//clearing the watch dog timer.
				WatchDogTimerStatus = gBS->SetWatchdogTimer(0, 0, 0, NULL); 
				gWatchDogTimerSet = FALSE;
				SETUP_DEBUG_TSE ("WatchDog Timer Status = %r\n", WatchDogTimerStatus);
			}
		}
		
		if (NULL != gShellLaunchEvent)
		{
			gBS->CloseEvent (gShellLaunchEvent);
			gShellLaunchEvent = NULL;
		}
		if (NULL != gShell20LaunchEvent)				
		{
			gBS->CloseEvent (gShell20LaunchEvent);
			gShell20LaunchEvent = NULL;
		}
		// Performance measurement continue
		PERF_START (0, AMITSE_TEXT("Boot"), NULL, 0);

        AfterEfiBootLaunchHook();
		EfiLibNamedEventSignal (&AfterBootGuid);
/*#if APTIO_4_00
		gMaxRows = MAX_ROWS;
		gMaxCols = MAX_COLS;
#endif //APTIO_4_00

		SetDesiredTextMode();			// StyleInit will set the desired text mode*/
		SETUP_DEBUG_TSE("[TSE] Clearing Graphics Screen\n");

      ClearGrphxScreen ();
        if(EFI_ERROR( Status ))
        {
      	  InvalidateStatusInBgrtWrapper ();
            // Report only if it is Boot Option launch - Starting the Boot option failed.!!!!
            if(ValidBootOption==TRUE)
                REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
                                    EFI_ERROR_CODE| EFI_ERROR_MAJOR,
                                    DXE_BOOT_OPTION_FAILED,
                                    &Status,
                                    sizeof(EFI_STATUS));

    		//Clear the flush buffer so that flush lines will actually
            //draw the complete screen again
			 if ( NULL != gFlushBuffer)
        		MemSet(gFlushBuffer, sizeof(SCREEN_BUFFER), 0);

			if (gST->ConOut != NULL)
			{
	            //Disable cursor, set desired attributes and clear screen
	            gST->ConOut->EnableCursor( gST->ConOut, FALSE );
	            gST->ConOut->SetAttribute( gST->ConOut, EFI_BACKGROUND_BLACK | EFI_WHITE);
	            gST->ConOut->ClearScreen( gST->ConOut);
			}
			SETUP_DEBUG_TSE("[TSE] Redrawing whole screen using FlushLines()\n",Status);
            //Call flush lines to draw the whole screen again
            FlushLines( 0, gMaxRows - 1 );
			//DoRealFlushLines();			
        }
		else
		{
			InvalidateStatusInBgrtWrapper ();			//When win8 launched after successfull boot then BGRT table should be invalidated
		}
    }
    else
    {
        if (IsPxeHttpDevicePath(DevicePath))
        {    
            AfterEfiBootLaunchHook();
            EfiLibNamedEventSignal (&AfterBootGuid);
        }
        SETUP_DEBUG_TSE("[TSE] Clearing Graphics Screen\n");
        ClearGrphxScreen ();
    }

    if(FileBuffer)
    {
       MemFreePointer((VOID **)&FileBuffer);
    }
    if(FilePath)
    {
       MemFreePointer((VOID **)&FilePath);
    }
    
    if(RamDiskDevicePath != NULL)
    {
        DestoryRamDisk(RamDiskDevicePath);
        MemFreePointer((VOID **)&RamDiskDevicePath);
    }
    
     gPostStatus = BackupPostStatus; /// restore the pre. Post status.

    if ( CurrentTpl > EFI_TPL_APPLICATION )
        gBS->RaiseTPL( CurrentTpl );
	
    SETUP_DEBUG_TSE("[TSE] _BootLaunchDevicePath Exiting :\n\n");
    return Status;
}

/**
    function to launch the boot operation

    @param DevicePath 

    @retval device path

**/
EFI_DEVICE_PATH_PROTOCOL* _DiscoverPartition(EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
    EFI_STATUS	Status;
    EFI_HANDLE	*Handle;
    UINTN		Count, i;

    EFI_DEVICE_PATH_PROTOCOL *FullDevicePath=NULL;
    HARDDRIVE_DEVICE_PATH* BootParitionDevicePath  = (HARDDRIVE_DEVICE_PATH*)DevicePath;

    //get list of available Block I/O devices
    Status = gBS->LocateHandleBuffer(ByProtocol,&gEfiBlockIoProtocolGuid,NULL,&Count,&Handle);
    if (EFI_ERROR(Status)) return NULL;

    for( i=0; i<Count; i++ )
    {
        EFI_BLOCK_IO_PROTOCOL		*BlockIo;
        EFI_DEVICE_PATH_PROTOCOL	*PartitionDevicePath, *TmpDevicePath;
        HARDDRIVE_DEVICE_PATH*		PartitionNode;

        Status = gBS->HandleProtocol(Handle[i],&gEfiBlockIoProtocolGuid,(void**)&BlockIo);
        if (EFI_ERROR(Status))
			continue;

        // if this is not partition, continue
        if (!BlockIo->Media->LogicalPartition)
			continue;

		Status = gBS->HandleProtocol(Handle[i],&gEfiDevicePathProtocolGuid,(void**)&PartitionDevicePath);
        if (EFI_ERROR(Status))
			continue;

        // Get last node of the device path. It should be partition node
        PartitionNode = (HARDDRIVE_DEVICE_PATH*)PartitionDevicePath;

        for( TmpDevicePath = PartitionDevicePath;
             !IsDevicePathEndType(TmpDevicePath);
             TmpDevicePath=NextDevicePathNode(TmpDevicePath) )
		{
			PartitionNode = (HARDDRIVE_DEVICE_PATH*)TmpDevicePath;
		}

        //Check if our partition matches Boot partition
        if (PartitionNode->Header.Type!=MEDIA_DEVICE_PATH || PartitionNode->Header.SubType!=MEDIA_HARDDRIVE_DP)
			continue;

        if ( PartitionNode->PartitionNumber==BootParitionDevicePath->PartitionNumber &&
             PartitionNode->SignatureType==BootParitionDevicePath->SignatureType &&
             !MemCmp(PartitionNode->Signature,BootParitionDevicePath->Signature,16) )
        {
            //Match found
            FullDevicePath = EfiAppendDevicePath(PartitionDevicePath,NextDevicePathNode(DevicePath));
            break;
        }
    }

    gBS->FreePool(Handle);
    return FullDevicePath;
}

/**
    function to install the load options

    @param handle VOID *Options, UINTN OptionSize

    @retval void

**/
VOID _BootInstallLoadOptions( EFI_HANDLE handle, VOID *Options, UINTN OptionSize )
{
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;

    Status = gBS->HandleProtocol( handle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage );
    if ( EFI_ERROR( Status ) )
        return;

    LoadedImage->LoadOptions = Options;
    LoadedImage->LoadOptionsSize = (UINT32)OptionSize;

    gCurrentBootHandle = handle;		//Exposing the Handle of the image that's being launched for boot, This will help other elinks to do the processing.

}


/**
    function to build firmware volume device path protocol.

    @param index EFI_GUID *guidPtr

    @retval device path

**/
EFI_DEVICE_PATH_PROTOCOL *_BootBuildFVDevicePath( UINT32 *index, EFI_GUID *guidPtr )
{
	EFI_STATUS  Status;
	EFI_HANDLE  *HandleBuffer;
	UINTN       Count;
	UINT32      i;

    EFI_DEVICE_PATH_PROTOCOL *DevicePath, *FilePath = NULL;

    MEDIA_FW_VOL_FILEPATH_DEVICE_PATH gFvFile;


    Status = gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiFirmwareVolume2ProtocolGuid,
            NULL,
            &Count,
            &HandleBuffer
            );

    if ( EFI_ERROR( Status ) )
    {
        *index = (UINT32)-1;
        return FilePath;
    }


    for ( i = *index; i < (UINT32)Count; i++ )
    {
        Status = gBS->HandleProtocol( HandleBuffer[i], &gEfiDevicePathProtocolGuid, (void**)&DevicePath );
        if ( EFI_ERROR( Status ) )
            continue;


        TseEfiInitializeFwVolDevicepathNode (&gFvFile, guidPtr);


        FilePath = EfiAppendDevicePathNode( DevicePath, (EFI_DEVICE_PATH_PROTOCOL *)&gFvFile );
        if ( FilePath != NULL )
        {
            *index = i + 1;
            break;
        }
    }

    if ( i == (UINT32)Count )
        *index = (UINT32)-1;

    MemFreePointer( (VOID **)&HandleBuffer );

    return FilePath;
}

/**
    function to build File device path protocol.

    @param index CHAR16 *fileName

    @retval device path

**/
EFI_DEVICE_PATH_PROTOCOL *_BootBuildFileDevicePath( UINT32 *index, CHAR16 *fileName )
{
    EFI_STATUS  Status;
    EFI_HANDLE  *HandleBuffer;
    UINTN       Count;
    UINT32      i;

    EFI_DEVICE_PATH_PROTOCOL *FilePath = NULL;

    Status = gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiSimpleFileSystemProtocolGuid,
            NULL,
            &Count,
            &HandleBuffer
            );

    if ( EFI_ERROR( Status ) )
    {
        *index = (UINT32)-1;
        return FilePath;
    }

    for ( i = *index; i < (UINT32)Count; i++ )
    {
        FilePath = EfiFileDevicePath( HandleBuffer[i], fileName );
        if ( FilePath != NULL )
        {
            *index = i + 1;
            break;
        }
    }

    if ( i == (UINT32)Count )
        *index = (UINT32)-1;

    MemFreePointer( (VOID **)&HandleBuffer );
    return FilePath;
}

/**
    Function to check the boot option status

    @param value BOOLEAN ShowAllBBSDev, BOOLEAN TseBootNowInBootOrde, BOOLEAN FromSetup

    @retval 

**/
BOOLEAN IsBootDeviceEnabled( UINT16 value, BOOLEAN ShowAllBBSDev, BOOLEAN TseBootNowInBootOrde, BOOLEAN FromSetup)
{

	UINT16	*BootOrder=NULL;
    UINTN	size = 0;
    UINTN	i,j,k;
    UINT16	count;

    BOOT_DATA *bootData;

    if(gBootData == NULL)
        return FALSE; 

    if((!ShowAllBBSDev) && (!TseBootNowInBootOrde))
    {
        //Here we are checking if value is within the Boot option count or not
        if(value < gBootOptionCount)

            bootData = &gBootData[value];

        else
        {
            SETUP_DEBUG_TSE ("[TSE] value is not within Boot option count \n");
            ASSERT(0);
            return FALSE;
        }

        if(BBSValidDevicePath(bootData->DevicePath) )
            return BootGetBBSOptionStatus(bootData, 0, FromSetup,ShowAllBBSDev);
        else
            return BootGetOptionStatus(bootData, FromSetup);//Getting the Boot option status when TSE_BOOT_NOW_IN_BOOT_ORDER and SETUP_SHOW_ALL_BBS_DEVICES is OFF.
    }

    if(TseBootNowInBootOrde)
    {
        BootOrder = HelperGetVariable(VARIABLE_ID_BOOT_ORDER, L"BootOrder", &gEfiGlobalVariableGuid, NULL, &size );
        if (NULL == BootOrder)		//Check for boot order else the system will hang
        {
            return TRUE;
        }
        //Find the first disabled option
        for ( i = 0; i < gBootOptionCount; i++ )
        {
            if ( DISABLED_BOOT_OPTION == BootOrder[i] ){

                //There are disabled options replace them with valid options
                for(j=0;j<gBootOptionCount;j++)
                {
                    for(k=0;k<gBootOptionCount;k++)
                    {
                        if(BootOrder[k] == gBootData[j].Option)
                            break;
                    }

                    if(k >= gBootOptionCount)
                    {
                        //gBootData[j].Option is not present in BootOrder; fill it
                        BootOrder[i] = gBootData[j].Option;
                        break;
                    }
                }
            }
        }
    }
    if(ShowAllBBSDev)
    {
        count = 0;
        for(i=0;i<gBootOptionCount;i++)
        {
            if(TseBootNowInBootOrde) {
                bootData = BootGetBootData(BootOrder[i]);
                if(NULL == bootData)	
                {
                    SETUP_DEBUG_TSE ("Boot data is NULL\n");
                    ASSERT(0);
                    continue;
                }
            }
            else 
                bootData = gBootData + i;

            if ( BBSValidDevicePath(bootData->DevicePath) )
            {
                if((value >= count) && (value < (count+bootData->LegacyDevCount)))
                {
                    if(TseBootNowInBootOrde)
                        MemFreePointer((VOID **) &BootOrder);
                    return BootGetBBSOptionStatus(bootData, value-count, FromSetup, ShowAllBBSDev);//Getting the Legacy Boot option status when SETUP_SHOW_ALL_BBS_DEVICES is ON.
                }

                count+=bootData->LegacyDevCount;
            }
            else {
                if(value == count)
                {
                    if(TseBootNowInBootOrde)
                        MemFreePointer((VOID **) &BootOrder);
                    return BootGetOptionStatus(bootData, FromSetup);//Getting the Boot option status when SETUP_SHOW_ALL_BBS_DEVICES is ON.
                }
                count++;
            }
        }
        return FALSE;
    }
    else {
        bootData = BootGetBootData(BootOrder[value]);
        MemFreePointer((VOID **) &BootOrder);

        if(NULL == bootData)	
        {
            SETUP_DEBUG_TSE ("Boot data is NULL\n");
            ASSERT(0);
            return FALSE;
        }
        if(BBSValidDevicePath(bootData->DevicePath) )
            return BootGetBBSOptionStatus(bootData, 0, FromSetup,ShowAllBBSDev);
        else
            return BootGetOptionStatus(bootData, FromSetup);
    }
}

/**
    Function to check the boot option status in Boot Order

    @param bootData BOOLEAN FromSetup

    @retval 

**/
BOOLEAN BootGetOptionStatus(BOOT_DATA *bootData, BOOLEAN FromSetup)
{
	UINTN size = 0;
	UINTN i=0;
	UINT16 *buffer = NULL;

			if(0 == FromSetup)
			{
					if(bootData->Active & LOAD_OPTION_ACTIVE)
						return FALSE;
					else
						return TRUE;

			}

			//buffer = EfiLibAllocateZeroPool(  gBootOptionCount * sizeof(UINT16));	//Memory leak fix
			buffer = HelperGetVariable(VARIABLE_ID_BOOT_ORDER, L"BootOrder", &gEfiGlobalVariableGuid, NULL, &size );
         if (NULL == buffer)
         {
            return TRUE;
         }

			for(i=0 ; i<gBootOptionCount ; i++)
			{
				if(buffer[i] == bootData->Option)
				   break;
			}
			if(i == gBootOptionCount )
			{
				MemFreePointer((VOID **) &buffer);
					return TRUE;
			}
			else
			{
				MemFreePointer((VOID **) &buffer);
					return FALSE;
			}

}

/**
    Function to check the Legacy boot option status

    @param bootData BOOLEAN FromSetup, BOOLEAN ShowAllBBSDev

    @retval 

**/
BOOLEAN BootGetBBSOptionStatus(BOOT_DATA *bootData, UINT16 value, BOOLEAN FromSetup, BOOLEAN ShowAllBBSDev)
{
	   UINT32 offset=0;
		UINT32 i;
		UINT8 *pDevOrder;
		BBS_ORDER_TABLE	*pDev;
		UINTN size = 0;
		UINT16 *buf = NULL, *Tempbuf = NULL;

		if(0 == FromSetup)
		{
		   if(!BootGetOptionStatus(bootData, FromSetup))
		   {
	            offset = (UINT16)bootData->LegacyEntryOffset;

			  		pDevOrder = HelperGetVariable(VARIABLE_ID_BBS_ORDER,L"LegacyDevOrder", &gLegacyDevGuid, NULL, &size);
			        if((size < offset)||((size-offset)< sizeof(BBS_ORDER_TABLE)))
			        {
			            MemFreePointer((VOID **) &pDevOrder);
			            return TRUE;
			        }

	         	pDev = (BBS_ORDER_TABLE *)(pDevOrder + offset);

					if ( (pDev->Length >= size) || (0 == pDev->Length) ) 
					{
					MemFreePointer((VOID **) &pDevOrder);
					return TRUE;
					}

              if(DISABLED_BOOT_OPTION == pDev->Data[value])
					{
						 MemFreePointer((VOID **) &pDevOrder);
  					    return  TRUE;
					}
    		  		else
					{
						 MemFreePointer((VOID **) &pDevOrder);
                   return  FALSE;
					}
		   }
		   else
		       return TRUE;
        }
		if(!BootGetOptionStatus(bootData, FromSetup))
		{

			size=0;
		 	offset = (UINT16)bootData->LegacyEntryOffset;
			pDevOrder = HelperGetVariable(VARIABLE_ID_BBS_ORDER,L"LegacyDevOrder", &gLegacyDevGuid, NULL, &size);
            if (NULL == pDevOrder)
            {
              return TRUE;
            }
            if((size < offset)||((size-offset)< sizeof(BBS_ORDER_TABLE)))
            {
                MemFreePointer((VOID **) &pDevOrder);
                return TRUE;
            }

			pDev = (BBS_ORDER_TABLE *)(pDevOrder + offset);

			if ( (pDev->Length >= size) || (0 == pDev->Length) )
			{
                MemFreePointer((VOID **) &pDevOrder);
                return TRUE;
			}

			if(!ShowAllBBSDev)
			{
					if(DISABLED_BOOT_OPTION == pDev->Data[value])
					{
						MemFreePointer((VOID **) &pDevOrder);
  					   	return  TRUE;
    				}
					else
					{
						MemFreePointer((VOID **) &pDevOrder);
                  	return  FALSE;
					}
			}
			
	      buf = EfiLibAllocateZeroPool(  pDev->Length - sizeof(UINT16));
			
         if (NULL == buf)	
         {
            return FALSE;
         }
			
         Tempbuf = buf;
			MemCopy( buf, &pDev->Data, pDev->Length - sizeof(UINT16) );
			
			for(i=0; i < bootData->LegacyDevCount ;i++)
			{
            if(*buf == bootData->OrderList[value].Index)
               break;
            buf++;
			}
			
         MemFreePointer((VOID **) &pDevOrder);		
         MemFreePointer((VOID **) &Tempbuf);
			
			if(i ==  bootData->LegacyDevCount)
			{
            return  TRUE;
			}
			else
			{
            return  FALSE;
			}
		}
		else
			return TRUE;
}
/**
    Function to check the Boot option status if gLoadOptionhidden token is Enabled

    @param value 

    @retval BOOLEAN

**/
BOOLEAN LoadOptionhidden (UINT16 value, BOOLEAN Option)
{
	if (BOOT_ORDER_OPTION == Option)
	{
    	if (gBootData [value].Active & LOAD_OPTION_HIDDEN)
			return TRUE;
	}
	else if (DRIVER_ORDER_OPTION == Option)				
	{
		if (gDriverData [value].Active & LOAD_OPTION_HIDDEN)
			return TRUE;
	}	
	return FALSE;
}

/**
    Check whether the Boot/Driver option has hidden property

    @param UINT16, BOOLEAN

    @retval BOOLEAN

**/
BOOLEAN CheckHiddenforBootDriverOption (UINT16 Option, BOOLEAN HiddenOption)
{
	UINTN i = 0;
	if (gLoadOptionHidden)
	{	
		if (BOOT_ORDER_OPTION == HiddenOption)
		{
			for (i = 0 ;i < gBootOptionCount ;i++)
			{
				if (gBootData [i].Option == Option)
				{
					if (gBootData [i].Active & LOAD_OPTION_HIDDEN)
					{
						return TRUE;
					}
					break;
				}
			}
		}
		else if (DRIVER_ORDER_OPTION == HiddenOption)
		{
			for (i = 0 ;i < gDriverOptionCount ;i++)
			{
				if (gDriverData [i].Option == Option)
				{
					if (gDriverData [i].Active & LOAD_OPTION_HIDDEN)
					{
						return TRUE;
					}
					break;
				}
			}
		}
	}
	return FALSE;
}


/**
    Checks the input boot option matches with any of the boot option in the system

    @param UINT16 = Boot option to which match to be find

    @retval UINT32 = Returns 0 if no match found
        = 32 bit CRC value of boot option if match found

**/
UINT32 CheckBootOptionMatch (UINT16 BootOption)
{
//    UINTN	iIndex = 0;  Unused Variable
    CHAR16  BootOptionName [9];             //Bootxxxx + 1 NULL char
    UINT32	*LoadOptions = NULL;        //Using 32 bit ptr bcoz to find CRC32
    UINTN	LoadOptionSize = 0;
    UINT32  CRC32 = 0;

    SPrint (BootOptionName, sizeof (BootOptionName), L"Boot%04X", BootOption);
    LoadOptions = VarGetNvramName (BootOptionName, &gEfiGlobalVariableGuid, NULL, &LoadOptionSize);     //Getting boot options
    if ((NULL == LoadOptions) || (0 == LoadOptionSize))
    {
        return CRC32;           //returning 0
    }
   gBS->CalculateCrc32 ((UINT8 *)LoadOptions, LoadOptionSize, &CRC32);
   if(LoadOptions) //For MemoryLeak Fix
   	MemFreePointer((void **) &LoadOptions);
   return CRC32;
}

/**
    Function to set the BootOptionSupport variable

    @param UINT32 = Capabilities for the BootOptionSupport variable

    @retval VOID

**/
VOID SetBootOptionSupportVariable (UINT32 BootManCapabilities)
{
    CHAR16 VariableName [] = L"BootOptionSupport";
    UINT32 Attributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    UINTN DataSize = 0;
    VOID *Data;
    UINT32 SetData = 0;

    Data = VarGetNvramName (VariableName, &gEfiGlobalVariableGuid, &Attributes, &DataSize);     //If the variable exists use its attribute to set it
    if (NULL != Data)
    {
        SetData = *((UINT32 *)Data);
    }
    SetData |= BootManCapabilities;
    DataSize = sizeof (UINT32);         //Sizeof BootOptionSupport variable is UINT32
    VarSetNvramName (VariableName, &gEfiGlobalVariableGuid, Attributes, (VOID *)&SetData, DataSize);
    if(Data) //For MemoryLeak Fix
   	 MemFreePointer((void **) &Data);
}

/**
    Matches the device path with USB class device path (Table 60) and returns
    the corresponding USB's file system handle

    @param EFI_DEVICE_PATH_PROTOCOL * -> Device path for the boot option

    @retval EFI_HANDLE -> Handle for the file system

**/
EFI_HANDLE CheckDevSupShortFormPath (EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
	UINTN       NumHandles = 0;
	UINTN       iIndex = 0;
	CHAR16		*USBString = NULL;
	CHAR16		*USBDevPathString = NULL;
	UINT16     	*LangIDTable;
	UINT16		TableSize = 0;
	EFI_STATUS  Status;
	EFI_HANDLE  *UsbIoHandles = NULL;
    EFI_USB_IO_PROTOCOL         *UsbIoProtocolInstance = NULL;
    EFI_USB_DEVICE_DESCRIPTOR   DeviceDescriptor;
    USB_CLASS_DEVICE_PATH       *UsbClassDevPath = NULL;
	USB_WWID_DEVICE_PATH		*UsbWwidDevPath = NULL;
    EFI_DEVICE_PATH_PROTOCOL    *TempDevPath = DevicePath,*HandleDevPath = NULL;
	
    if ((MESSAGING_DEVICE_PATH == TempDevPath->Type) && (MSG_USB_CLASS_DP == TempDevPath->SubType))     //Check for USB Device Path Class. type = 3 and subtype = 0xf
    {
		UsbClassDevPath = (USB_CLASS_DEVICE_PATH *)TempDevPath;
    }
    else if ((MESSAGING_DEVICE_PATH == TempDevPath->Type) && (MSG_USB_WWID_CLASS_DP == TempDevPath->SubType))
    {
		UsbWwidDevPath = (USB_WWID_DEVICE_PATH *)TempDevPath;
		USBDevPathString = (CHAR16 *)((UINT8 *)UsbWwidDevPath + sizeof (USB_WWID_DEVICE_PATH));		//String will be present at the end of the WWID device path
    }
	else
	{
		return NULL;
	}
	Status = gBS->LocateHandleBuffer (                  //To match with USB Device Path Class
				ByProtocol,
				&gEfiUsbIoProtocolGuid,
				NULL,
				&NumHandles,
				&UsbIoHandles
				);
	if (EFI_ERROR (Status))
	{
        return NULL;
	}
	for (iIndex = 0; iIndex < NumHandles; iIndex ++)
	{
		Status = gBS->HandleProtocol (UsbIoHandles [iIndex], &gEfiUsbIoProtocolGuid, (void**) &UsbIoProtocolInstance);
		if (EFI_ERROR (Status))
    	{
            continue;
        }
        Status = UsbIoProtocolInstance->UsbGetDeviceDescriptor (UsbIoProtocolInstance, &DeviceDescriptor);
        if (EFI_ERROR (Status))
    	{
            continue;
        }
		if (UsbWwidDevPath)
		{
			Status = UsbIoProtocolInstance->UsbGetSupportedLanguages (UsbIoProtocolInstance, &LangIDTable, &TableSize);
			if (!EFI_ERROR (Status) && TableSize)
			{
				Status = UsbIoProtocolInstance->UsbGetStringDescriptor (UsbIoProtocolInstance, LangIDTable [0], DeviceDescriptor.StrSerialNumber, &USBString);		//LangIDTable [0], getting default as English
				if (EFI_ERROR (Status))
				{
					USBString = NULL;  		//Explicitly making as NULL
				}
			}
			if ((NULL != USBString) ^ (0 != EfiStrLen (USBDevPathString)))		//If serial number string present in device path and not in descriptor then try for other device and vice versa too
			{																			//If device path and descriptor not has the string then proceed
				continue;
			}
		}
		if (UsbClassDevPath?
		(			//Check for USB Class device path
        ((UsbClassDevPath->VendorId == DeviceDescriptor.IdVendor) || (0xFFFF == UsbClassDevPath->VendorId)) &&      //If values are 0xF's then dont consider that option
        ((UsbClassDevPath->ProductId == DeviceDescriptor.IdProduct) || (0xFFFF == UsbClassDevPath->ProductId)) &&
        ((UsbClassDevPath->DeviceClass == DeviceDescriptor.DeviceClass) || (0xFF == UsbClassDevPath->DeviceClass)) &&
        ((UsbClassDevPath->DeviceSubClass == DeviceDescriptor.DeviceSubClass) || (0xFF == UsbClassDevPath->DeviceSubClass)) &&
        ((UsbClassDevPath->DeviceProtocol == DeviceDescriptor.DeviceProtocol) || (0xFF == UsbClassDevPath->DeviceProtocol))
        ):
		(			//Check for USB WWID device path
		(UsbWwidDevPath->VendorId == DeviceDescriptor.IdVendor) &&
		(UsbWwidDevPath->ProductId == DeviceDescriptor.IdProduct) &&
		(USBString ? (!(EfiStrCmp (USBDevPathString, USBString))):1)			//String number of USB might not be filled in some case in such conditions will take it as TRUE
		)
		)
        {
            UINTN       Count;
        	EFI_GUID    **ppGuid;
            UINTN       jIndex = 0;
            UINTN       kIndex = 0;

            Status = gBS->ProtocolsPerHandle (UsbIoHandles [iIndex], &ppGuid, &Count);
            if (EFI_ERROR (Status))
            {
                continue;
            }
            for (jIndex = 0; jIndex < Count; jIndex ++)
	        {
                if (EfiCompareGuid (ppGuid [jIndex], &gEfiDiskIoProtocolGuid))
                {
                    EFI_OPEN_PROTOCOL_INFORMATION_ENTRY *pInfo;
                    UINTN InfoCount = 0;
                    VOID  *FilsSystemInstance = NULL;
                    EFI_HANDLE handle = NULL;
                    EFI_DEVICE_PATH_PROTOCOL    *FilePath = NULL;

                    Status = gBS->OpenProtocolInformation (UsbIoHandles [iIndex], ppGuid [jIndex], &pInfo, &InfoCount);
                    if (EFI_ERROR (Status))
                    {
                        continue;
                    }
                    for (kIndex = 0; kIndex < InfoCount; kIndex ++)
                    {
                        Status = gBS->HandleProtocol (pInfo [kIndex].ControllerHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FilsSystemInstance);
                        if (EFI_ERROR (Status))
                        {
                            Status = gBS->HandleProtocol (pInfo [kIndex].ControllerHandle, &gEfiBlockIoProtocolGuid, (void**)&FilsSystemInstance);
                            if (!EFI_ERROR (Status))
                            {
                                gBS->ConnectController (pInfo [kIndex].ControllerHandle, NULL, NULL, TRUE);
                                Status = gBS->HandleProtocol (pInfo [kIndex].ControllerHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FilsSystemInstance);
                            }
                            if (EFI_ERROR (Status))
                                continue;
                        }
                        HandleDevPath = EfiDevicePathFromHandle(pInfo [kIndex].ControllerHandle);
                        if(IsMediaFileDevicePath(HandleDevPath))
                        {
                        	FilePath = EfiLibAllocateZeroPool(EfiDevicePathSize(HandleDevPath));
                        	if(FilePath)
                        		MemCpy(FilePath,HandleDevPath,EfiDevicePathSize(HandleDevPath));                        	
                        }
                        else
                        	FilePath = EfiFileDevicePath (pInfo [kIndex].ControllerHandle, gBootFileName);

                        if (FilePath)
                        {
                            EfiLibReportStatusCode(EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeOsLoaderLoad),0,NULL,NULL);
                            Status = gBS->LoadImage (                       //Ensuring the image can load
                                                       TRUE,
                                                        gImageHandle,
                                                        FilePath,
                                                        NULL,
                                                        0,
                                                        &handle
                                                    );
                            MemFreePointer((VOID **) &FilePath);
                            if (!EFI_ERROR (Status))
                            {
								if (USBString)
								{
									MemFreePointer ((VOID **)&USBString);
								}
                                MemFreePointer ((VOID **)&UsbIoHandles);
                                return pInfo [kIndex].ControllerHandle;
                            }

                        }
                    }
                    if (InfoCount)
					{
						MemFreePointer ((VOID **)&pInfo);
                	}
                }
            }
        }
		if (USBString)
		{
			MemFreePointer ((VOID **)&USBString);
		}
	}
	MemFreePointer ((VOID **)&UsbIoHandles);
    return NULL;
}

/**
    This function collects all the BOOT#### name (e.g BOOT00001, BOOT002)
    check with BOOTOrder Variable. If BOOT#### is not present in BootOrder variable
    then it need to skip to create TSE bootData. If present then continue to create TSE
    TSE BootData

    @param VOID

    @retval Return TRUE/FALSE to skip BOOT####

**/
BOOLEAN TSESkipBootOrderVar(UINT16 *BootOrder, UINTN BootOrderSize,CHAR16 *VarName)
{
	EFI_STATUS Status;
	UINTN   Index = 0 ;
	UINTN	 Length = 2;
	UINT16 Option = 0xffff;
	Status = HexStringToBuf((UINT8 *)(&Option),&Length,&(VarName[4]), NULL) ;
	if (EFI_ERROR(Status)) 
		return TRUE;
	for (Index = 0 ; Index < BootOrderSize/sizeof(UINT16) ; Index++)
	{
		// Check BOOT#### present in BootOrder or not.
		if (BootOrder[Index] == Option)
			break;
	}

	if (Index >= BootOrderSize/sizeof(UINT16)) 
	{   
		//This is not in BOOT#### and skip this variable.
		return TRUE;
	}
	return FALSE;
}

/**
  Converts a File device path structure to its string and obtains the file name.

  @param DevPath         The input device path structure.
                         representation for a device node can be used, where applicable.
  @retval				    file name obtained from the path

**/
CHAR16* GetFileNameFromDevPath( EFI_DEVICE_PATH_PROTOCOL *DevPath)
{
   FILEPATH_DEVICE_PATH  *Fp = NULL;
   UINTN 	i = 0;
   UINTN 	Length = 0; 
   CHAR16	*FilePath = NULL;
   CHAR16	*FileName = NULL;

   if(NULL == DevPath)
      return FileName;
   Fp = (FILEPATH_DEVICE_PATH *)DevPath;
   //Obtain file path
   FilePath = EfiLibAllocateZeroPool((StrLen(Fp->PathName)+1) * sizeof(CHAR16));
   if(NULL == FilePath)
      return FileName;

   StrCpyS(FilePath,EfiStrLen(Fp->PathName)+1 ,Fp->PathName);
  
   //Obtain the file name from file path after the last '\'
   for(i=0; FilePath[i]!= L'\0'; i++);
   Length = i;
   for(i--; FilePath[i]!=L'\\'; i--);
   Length = Length-i-1;
   FileName  = EfiLibAllocateZeroPool((Length+1)*sizeof(CHAR16));
   if(NULL == FileName)
   	return FileName;
   StrCpyS(FileName,Length+1,FilePath+i+1); 
   MemFreePointer ((VOID **)&FilePath);
  
   return FileName;
}

/**
    Finds and returns internal data structure BOOT_DATA for a given OS defined boot
	option number for OSRecovery or platform  defined boot option for platform recovery.

    @param Option: Option number for which BOOT_DATA is needed

    @param OsRecovery: To check OSRecovery data or Platform recovery data

    @retval Returns pointer to BOOT_DATA if found. Returns NULL
        if BOOT_DATA not found.

**/
BOOT_DATA *RecoveryGetRecoveryData( UINT16 Option, UINT8 RecoveryBootingType)
{
    UINTN i;
    if(RecoveryBootingType == OS_RECOVERY_OPTION_BOOTING)
    {
		 for ( i = 0; i < gOSRecoveryOptionCount; i++ )
		 {
			  if ( gOSRecoveryData[i].Option == Option )
					return &gOSRecoveryData[i];
		 }
    }
    else if (RecoveryBootingType == PLATFORM_RECOVERY_OPTION_BOOTING)
    {
		 for ( i = 0; i < gPlatformRecoveryOpCount; i++ )
		 {
			  if ( gPlatformRecoveryData[i].Option == Option )
					return &gPlatformRecoveryData[i];
		 }
    }

    return NULL;
}

/**
 * The function checks if boot option is well-formed
 * same as bds\bootoptions.c -> IsBootOptionValid adjusted for the differences
 * between BOOT_OPTION and EFI_LOAD_OPTION structure
 *
 * @param Buffer - Pointer to the BOOT_OPTION structure
 * @param Size of BOOT_OPTION plus the optional data section
 *
 * @retval TRUE boot option is valid, FALSE otherwise
*/
BOOLEAN TSEIsBootOptionValid(BOOT_OPTION* Buffer, UINTN Size)
{
	CHAR16 *Char;
	CHAR16 *EndOfDescription;
	EFI_DEVICE_PATH_PROTOCOL *Dp;
	UINTN DevicePathSize;
	UINTN NvramOptionSize = Size;
	BOOT_OPTION *NvramOption = Buffer;
	
	// The boot option must have at least the header, 
	// an empty (just the NULL-terminator) description,
	// and an empty device path (End-of-Device-Path node).
	if (NvramOption->PathLength<sizeof(EFI_DEVICE_PATH_PROTOCOL)) return FALSE;
	
	if (NvramOptionSize < sizeof(*NvramOption)) return FALSE;
	NvramOptionSize -= sizeof(*NvramOption);
	NvramOptionSize += sizeof(CHAR16);

	if (  NvramOption->PathLength >= NvramOptionSize ) return FALSE;
	NvramOptionSize -= NvramOption->PathLength;

	// The description must include at least the NULL-terminator
	if (NvramOptionSize < sizeof(CHAR16)) return FALSE;

	    // The description must be NULL-terminated
	Char = NvramOption->Name;
	EndOfDescription = (CHAR16*)((CHAR8*)Char+NvramOptionSize);
	while( *Char && Char < EndOfDescription) Char++;
		if (Char==EndOfDescription) return FALSE;

	
	// Validate the device path;
	Dp = (EFI_DEVICE_PATH_PROTOCOL*)(Char+1); // skip the terminating zero.
	if (EFI_ERROR(IsValidDevicePath(Dp))) return FALSE;
	if (BBSValidDevicePath(Dp)) return FALSE; // filter BBS options
	// NvramOption->FilePathListLength can't be zero. 
	    // We checked that at the start of the funciton.
	DevicePathSize = NvramOption->PathLength;
	while (TRUE) {
		UINTN Length = NODE_LENGTH(Dp);
		if (Length>DevicePathSize) return FALSE;
	        // We are not making sure that Length is not equal to zero.
	        // This has already been verified by the IsValidDevicePath above.
		DevicePathSize -= Length;
		if (DevicePathSize < sizeof(EFI_DEVICE_PATH_PROTOCOL)){
	         if (DevicePathSize != 0 ) return FALSE;
	         //The last node must be an End-of-Device-Path node.
	          return isEndNode(Dp) && Dp->SubType == END_ENTIRE_SUBTYPE && Length == sizeof(EFI_DEVICE_PATH_PROTOCOL);
        }
		Dp = (EFI_DEVICE_PATH_PROTOCOL*)((UINT8*)Dp+Length);
    }
    //Should never reach this point.
    return FALSE;
}

/**
 * The function checks DevicePath having URI Devicepath Or Not 
 * If the device path having URI it will return TRUE else FLASE
 * 
 * @param DevicePath - The input device path structure.
 * 
 * @retval TRUE Device path having URI, FALSE otherwise
*/
BOOLEAN IsURIDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
	EFI_DEVICE_PATH_PROTOCOL *TempDevicePath=NULL;
	
	if(!DevicePath  || EFI_ERROR(IsValidDevicePath(DevicePath)))
		return FALSE;
	
	for(TempDevicePath=DevicePath;!IsDevicePathEndType(TempDevicePath);TempDevicePath = NextDevicePathNode(TempDevicePath))
	{
		if((TempDevicePath->Type == MESSAGING_DEVICE_PATH) && (TempDevicePath->SubType == MSG_URI_DP))
			return TRUE;
	}
	
	return FALSE;
}

/**
 * The function to Check whether Devicepath contains MediaFile path, 
 * 
 * @param DevicePath - The input device path structure.
 *
 * @retval BOOLEAN
*/
BOOLEAN IsMediaFileDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
	EFI_DEVICE_PATH_PROTOCOL *TempDevicePath=NULL;
	
	if(!DevicePath  || EFI_ERROR(IsValidDevicePath(DevicePath)))
		return FALSE;
	
	for(TempDevicePath=DevicePath;!IsDevicePathEndType(TempDevicePath);TempDevicePath = NextDevicePathNode(TempDevicePath))
	{
		if((TempDevicePath->Type == MEDIA_DEVICE_PATH) && (TempDevicePath->SubType == MEDIA_FILEPATH_DP))
			return TRUE;
	}
	return FALSE;
}

/**
 * The function to return the Boot Option File buffer From the File Path,This Function will iterate till the Valid File Buffer Found  
 * 
 * @param FilePath            - Points to Device path of Load options.
 * @param FullPath            - Return the Full Device path of the load option.
 * @param FileSize            - Return the load option size. 
 *
 * @retval VOID *             - Return the load option buffer.  
*/
VOID *GetLoadOptionFileBuffer(EFI_DEVICE_PATH_PROTOCOL *FilePath,EFI_DEVICE_PATH_PROTOCOL **FullPath,UINTN *FileSize)
{
    VOID                        *FileBuffer = NULL;
    UINTN                       TempFileSize = 0;
    EFI_DEVICE_PATH_PROTOCOL    *PreFullPath = NULL;
    EFI_DEVICE_PATH_PROTOCOL    *CurFullPath = NULL;
    EFI_DEVICE_PATH_PROTOCOL    *RamDiskDevicePath = NULL;

    CurFullPath = *FullPath;
    do{
        PreFullPath = CurFullPath;
        CurFullPath = GetLoadOptionDevicePath(FilePath,CurFullPath);
        
        //Only Free the Full path if it is not matched.
        if((NULL != PreFullPath) && (PreFullPath != *FullPath))
            MemFreePointer((VOID **)&PreFullPath);
        
        if(CurFullPath == NULL)
            break;
        
        FileBuffer = GetFileBufferFromFilePath(TRUE,CurFullPath,&TempFileSize);
        if((FileBuffer != NULL) && !IsLoadOptionPeHeaderValid(LoadOptionTypeBoot,FileBuffer,TempFileSize))
        {
           //Free the RAM disk device path if it is invalid load option.
            RamDiskDevicePath = GetRamDiskDevicePath(FilePath);
            if(RamDiskDevicePath  == NULL)
            {
                DestoryRamDisk(RamDiskDevicePath);
                MemFreePointer((VOID **)&RamDiskDevicePath );
            }
            //Free the Invalid load option buffer
            MemFreePointer((VOID **)&FileBuffer);
            FileBuffer  = NULL;
        }
    }while(NULL == FileBuffer);
    
    if(NULL == FileBuffer)
    {
        CurFullPath = NULL;
        TempFileSize = 0;
    }
    *FullPath = CurFullPath;
    
    //Return the File Buffer Size
    *FileSize = TempFileSize;

    return FileBuffer;
}


/**
 * The function to return the Full file path that pointing to load option, 
 * 
 * @param FilePath            - The device path pointing to a load option.it cloud be Short Device path also.
 * @param FullPath            - Returns full path by the routine.
 * 
 * @retval Device Path        -  The Next possible full path pointing to load option 
*/
EFI_DEVICE_PATH_PROTOCOL * GetLoadOptionDevicePath(EFI_DEVICE_PATH_PROTOCOL *FilePath,EFI_DEVICE_PATH_PROTOCOL*FullPath)
{
   UINTN                            HandleCount = 0,Index = 0;
   EFI_HANDLE                       Handle =NULL,*Handles;
   EFI_STATUS                       Status = EFI_UNSUPPORTED;
   EFI_DEVICE_PATH_PROTOCOL         *CurNode;

   //Return NUll file path,if it is having invalid file path. 
   if(FilePath == NULL)
       return NULL;
   
   //Expand the short Form device path to full device path
   if((FilePath->Type == MESSAGING_DEVICE_PATH) && (FilePath->SubType == MSG_URI_DP))
   {
       return ExpandUriDevicePath(FilePath,FullPath);
   }
   
   CurNode = FilePath;
   Status = gBS->LocateDevicePath(
           &gEfiLoadFileProtocolGuid,
           &CurNode,
           &Handle);
   
   if(!(EFI_ERROR(Status)) && IsDevicePathEndType(CurNode))
   {
      // When wide match happens, pass full device path to LoadFile (),
      // otherwise, pass remaining device path to LoadFile ().
       FilePath = CurNode;
   }
   else
   {
       Handle = NULL;
       // Use wide match algorithm to find one when
       //  cannot find a LoadFile instance to exactly match the FilePath
       Status = gBS->LocateHandleBuffer(
               ByProtocol,
               &gEfiLoadFileProtocolGuid,
               NULL,
               &HandleCount,
               &Handles
               );
       if(EFI_ERROR(Status))
       {
           return NULL;
       }
       for(Index=0;Index < HandleCount;Index ++)
       {
           EFI_DEVICE_PATH_PROTOCOL *HanDevicePath = NULL;
           
           //Get the Device path from Handle and Match if is valid HTTP path.
           HanDevicePath = EfiDevicePathFromHandle(Handles[Index]);
           if(NULL == HanDevicePath)
               continue;
           if(MatchHttpDevicePath(HanDevicePath,FilePath))
           {
               Handle = Handles[Index];
               break;
           }
       }
       if(Handles)
       {
           MemFreePointer((void**)&Handles);
       }
   }
   
   //Return NULL,if Invalid HTTP Device path Found.
   if(Handle == NULL)
       return NULL;

   return ExpandLoadFile(Handle,FilePath); 
}

/**
 * The function to return the file buffer data and buffer size from the Device path. 
 * 
 * @param BootPolicy        - BootPolicy for load image
 * @param FilePath          - The pointer to the device path of the file buffer 
 * @param FileSize          - The pointer to the Size of the File buffer
 *
 * @retval VOID *           - Return the File buffer from file path. 
*/
VOID *GetFileBufferFromFilePath(BOOLEAN BootPolicy, EFI_DEVICE_PATH_PROTOCOL *FilePath ,UINTN *FileSize)
{
    EFI_STATUS                          Status = EFI_UNSUPPORTED;
    EFI_HANDLE                          Handle;
    UINT8                               *ImageBuffer = NULL;
    UINTN                               ImageBufferSize = 0,FileInfoSize;
    EFI_FILE_INFO                       *FileInfo = NULL;
    EFI_FILE_HANDLE                     FileHandle,LastHandle;
    EFI_LOAD_FILE_PROTOCOL              *LoadFile;
    EFI_DEVICE_PATH_PROTOCOL            *TempDevicePathNode,*DevicePathNode,*OrigDevicePathNode;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *Volume;
    EFI_GUID                            EfiFileInfoGuid = { 0x09576E92, 0x6D3F, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B }}; 
    
    //Check input file device path.
    if(FilePath == NULL)
       return NULL;
    //Copy the file device path.
    OrigDevicePathNode = DuplicateDevicePath(FilePath);
    DevicePathNode = OrigDevicePathNode;
    
    //Check whether this device path support file system interface and get the file.
    Status = gBS->LocateDevicePath (&gEfiSimpleFileSystemProtocolGuid, &DevicePathNode, &Handle);
    if (!EFI_ERROR (Status)) {
        Status = gBS->HandleProtocol (Handle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&Volume);
        if (!EFI_ERROR (Status)) {
            //
            // Open the Volume to get the File System handle
            //
            Status = Volume->OpenVolume (Volume, &FileHandle);
            if (!EFI_ERROR (Status)) {
                //
                // Duplicate the device path to avoid the access to unaligned device path node.
                // Because the device path consists of one or more FILE PATH MEDIA DEVICE PATH
                // nodes, It assures the fields in device path nodes are 2 byte aligned.
                //
                TempDevicePathNode = DuplicateDevicePath (DevicePathNode);
                if (TempDevicePathNode == NULL) {
                    FileHandle->Close (FileHandle);
                    //
                    // Setting Status to an EFI_ERROR value will cause the rest of
                    // the file system support below to be skipped.
                    //
                    Status = EFI_OUT_OF_RESOURCES;
                }
                //
                // Parse each MEDIA_FILEPATH_DP node. There may be more than one, since the
                // directory information and filename can be seperate. The goal is to inch
                // our way down each device path node and close the previous node
                //
                DevicePathNode = TempDevicePathNode;
                while (!EFI_ERROR (Status) && !IsDevicePathEnd (DevicePathNode)) {
                    if (DevicePathType (DevicePathNode) != MEDIA_DEVICE_PATH ||
                            DevicePathSubType (DevicePathNode) != MEDIA_FILEPATH_DP) {
                        Status = EFI_UNSUPPORTED;
                        break;
                    }
                    
                    LastHandle = FileHandle;
                    FileHandle = NULL;
                    
                    Status = LastHandle->Open (
                                             LastHandle,
                                            &FileHandle,
                                            ((FILEPATH_DEVICE_PATH *) DevicePathNode)->PathName,
                                            EFI_FILE_MODE_READ,
                                            0
                                            );
                    //
                    // Close the previous node
                    //
                    LastHandle->Close (LastHandle);
                    DevicePathNode = NextDevicePathNode (DevicePathNode);
                }
                
                if (!EFI_ERROR (Status)) {
                    //
                    // We have found the file. Now we need to read it. Before we can read the file we need to
                    // figure out how big the file is.
                    //
                    FileInfo = NULL;
                    FileInfoSize = 0;
                    Status = FileHandle->GetInfo (
                                    FileHandle,
                                    &EfiFileInfoGuid,
                                    &FileInfoSize,
                                    FileInfo
                                    );
                    if (Status == EFI_BUFFER_TOO_SMALL) {
                        FileInfo = EfiLibAllocatePool(FileInfoSize);
                        if (FileInfo == NULL) {
                            Status = EFI_OUT_OF_RESOURCES;
                        } else {
                            Status = FileHandle->GetInfo (
                                        FileHandle,
                                        &EfiFileInfoGuid,
                                        &FileInfoSize,
                                        FileInfo
                                        );
                        }
                    }
                    if (!EFI_ERROR (Status) && (FileInfo != NULL)) {
                        if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) == 0) {
                            //
                            // Allocate space for the file
                            //
                            ImageBuffer = EfiLibAllocatePool ((UINTN)FileInfo->FileSize);
                            if (ImageBuffer == NULL) {
                                Status = EFI_OUT_OF_RESOURCES;
                            } else {
                                //
                                // Read the file into the buffer we allocated
                                //
                                ImageBufferSize = (UINTN)FileInfo->FileSize;
                                Status = FileHandle->Read (FileHandle, &ImageBufferSize, ImageBuffer);
                            }
                        }
                    }
                }
                //
                // Close the file and Free FileInfo and TempDevicePathNode since we are done
                // 
                if (FileInfo != NULL) {
                    MemFreePointer((VOID **)&FileInfo);
                }
                
                if (FileHandle != NULL) {
                    FileHandle->Close (FileHandle);
                }
                
                if (TempDevicePathNode != NULL) {
                    MemFreePointer((VOID **)&TempDevicePathNode);
                }
            }
        }
        if (!EFI_ERROR (Status)) {
            goto Finish;
        }
    }
    DevicePathNode = OrigDevicePathNode;
    //Check whether the device path support load file protocol.
    Status = gBS->LocateDevicePath(
            &gEfiLoadFileProtocolGuid,
            &DevicePathNode,
            &Handle);
    if(!(EFI_ERROR(Status)))
    {
        Status = gBS->HandleProtocol(
                Handle,
                &gEfiLoadFileProtocolGuid,
                (VOID **)&LoadFile);
        if(!(EFI_ERROR(Status)))
        {
           ImageBufferSize = 0;
           ImageBuffer = NULL;
           //Call Load File to get the FileSize 
            Status = LoadFile->LoadFile(
                    LoadFile,
                    DevicePathNode,
                    BootPolicy,
                    &ImageBufferSize,
                    ImageBuffer);
            if(EFI_BUFFER_TOO_SMALL == Status)
            {
                ImageBuffer = EfiLibAllocateZeroPool(ImageBufferSize);
                if(ImageBuffer == NULL)
                {
                    Status = EFI_OUT_OF_RESOURCES;
                }
                else
                {
                   //call Load file to get file buffer. 
                    Status = LoadFile->LoadFile(
                            LoadFile,
                            DevicePathNode,
                            BootPolicy,
                            &ImageBufferSize,
                            ImageBuffer);                    
                }
            }
        }
    }
    
Finish:
    if(EFI_ERROR(Status))
    {
        if(ImageBuffer)
        {
            MemFreePointer((VOID **)&ImageBuffer);
            ImageBuffer = NULL;
        }
        *FileSize = 0;
    }
    else
    {
        *FileSize = ImageBufferSize;
    }
    if(OrigDevicePathNode)
       MemFreePointer((VOID **)&OrigDevicePathNode);
    
    return ImageBuffer;
}

/**
 * The function to return Full device path from Short URI. 
 * 
 * @param FilePath            - Device path pointing to a load option
 * @param FullPath            - The Full path returned by the routine
 *
 * @retval Device Path        - The next possible full path pointing to the load option.
*/
EFI_DEVICE_PATH_PROTOCOL *ExpandUriDevicePath(EFI_DEVICE_PATH_PROTOCOL *FilePath,EFI_DEVICE_PATH_PROTOCOL*FullPath)
{

    UINTN                       HandleCount = 0,Index = 0,TempLength = 0;
    UINT8                       *DevPath = NULL;
    EFI_STATUS                  Status = EFI_UNSUPPORTED;
    EFI_HANDLE                  Handle,*Handles;
    BOOLEAN                     GetNext = 0;
    EFI_DEVICE_PATH_PROTOCOL    *HanDevicePath = NULL,*NewDevicePath = NULL,*OldDevicePath = NULL,*FullDevicePath = NULL,*RamDiskDevicePath=NULL;

    
    Status = gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiLoadFileProtocolGuid,
            NULL,
            &HandleCount,
            &Handles
            );
    if(EFI_ERROR(Status))
    {
        return NULL;
    }
    
    if(FullPath == NULL)
        GetNext = 1;
    
    for(Index = 0;Index < HandleCount;Index ++)
    { 
        HanDevicePath = NULL;
        HanDevicePath = EfiDevicePathFromHandle(Handles[Index]);        
        if(HanDevicePath == NULL)
            continue;
        
        if(!IsURIDevicePath(HanDevicePath))
            continue;        
        
        for(NewDevicePath =HanDevicePath; !IsDevicePathEndType(NewDevicePath);  NewDevicePath = NextDevicePathNode(NewDevicePath))
        {
            OldDevicePath = NextDevicePathNode(NewDevicePath);
           
            //If next node is URI node strip it out
            if ((OldDevicePath->Type == MESSAGING_DEVICE_PATH) && (OldDevicePath->SubType == MSG_URI_DP))
            {
            
               //Removing exiting URI node if present even if it is having non empty URI node.
                TempLength = *(UINT16 *)NewDevicePath->Length;
                NewDevicePath = (EFI_DEVICE_PATH_PROTOCOL  *)((UINT8 *)NewDevicePath + TempLength);   //Move the pointer to URI node
                DevPath = (UINT8 *)NewDevicePath;
                DevPath[0] = END_DEVICE_PATH_TYPE;          //Set End device path type
                DevPath[1] = END_ENTIRE_DEVICE_PATH_SUBTYPE;    //Set End device path sub type
                (*((UINT16 *)DevPath + 1) ) = END_DEVICE_PATH_LENGTH;   //Set End device path length
                break;
            }
        }
        Handle = Handles[Index];
        FullDevicePath = ExpandLoadFile(Handle,FilePath); 
        if(FullDevicePath ==NULL)
            continue;
        if(GetNext)
        {
            break;
        }
        else
        {
            GetNext = (BOOLEAN)(MemCmp((UINT8 *)FullDevicePath, (UINT8 *)FullPath, DevicePathNodeLength(FullDevicePath)) == 0);
            RamDiskDevicePath = GetRamDiskDevicePath(FullDevicePath);
            if(RamDiskDevicePath  != NULL)
            {
                DestoryRamDisk(RamDiskDevicePath );
                MemFreePointer((VOID **)&RamDiskDevicePath);                
            }
            MemFreePointer((VOID **)&FullDevicePath);
            FullDevicePath = NULL;                   
        }
    }
    if(Handles)
        MemFreePointer((VOID **)&Handles);
    return FullDevicePath;  
}

/**
 * The function check whether the Devicepath1 and Deviceapth2 are same for URI device path node
 * 
 * @param DevicePath1        - Device path to match
 * @param DevicePath2        - Device path to match
 * 
 * 
 * @retval FALSE if the Both are same, TRUE if the both device path are different.
*/
BOOLEAN MatchHttpDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevicePath1,EFI_DEVICE_PATH_PROTOCOL *DevicePath2)
{
    if((NULL == DevicePath1) || (NULL == DevicePath2))
        return FALSE;
    for(;!IsDevicePathEndType(DevicePath1) && !IsDevicePathEndType(DevicePath2);
            DevicePath1 = NextDevicePathNode(DevicePath1),DevicePath2 = NextDevicePathNode(DevicePath2) )
    {
        if(MemCmp((UINT8 *)DevicePath1, (UINT8 *)DevicePath2, DevicePathNodeLength(DevicePath1)))
        {
            if((DevicePathType(DevicePath1) != MESSAGING_DEVICE_PATH) ||(DevicePathType(DevicePath2) != MESSAGING_DEVICE_PATH))
                return FALSE;
            
				//skip DNS path from comparision because it is not present on TSE side
            if(DevicePathSubType(DevicePath1)  == MSG_DNS_DP)
                DevicePath1 = NextDevicePathNode(DevicePath1);
            if(DevicePathSubType(DevicePath2)  == MSG_DNS_DP)
                DevicePath2 = NextDevicePathNode(DevicePath2);
                
            if (((DevicePathSubType(DevicePath1)!= MSG_IPv4_DP) || (DevicePathSubType(DevicePath2)!= MSG_IPv4_DP)) &&
                ((DevicePathSubType(DevicePath1)  != MSG_IPv6_DP) || (DevicePathSubType(DevicePath2)!= MSG_IPv6_DP)) &&
                ((DevicePathSubType(DevicePath1)  != MSG_URI_DP)  || (DevicePathSubType(DevicePath2) != MSG_URI_DP))
                ) {
              return FALSE;
            }
            
        }
    }
    return (BOOLEAN)(IsDevicePathEndType(DevicePath1) && IsDevicePathEndType(DevicePath2));
}

/**
 * The function to return the file buffer from the specified load file instance., 
 * 
 * @param LoadFileHandle        - The Specific load file instance.
 * @param FilePath              - The file path to be passed to load file
 *
 * @retval Device path          - Full device path pointing to load option buffer.
*/
EFI_DEVICE_PATH_PROTOCOL *ExpandLoadFile(EFI_HANDLE LoadFileHandle,EFI_DEVICE_PATH_PROTOCOL *FilePath)
{
    VOID                        *FileBuffer =  NULL;
    UINTN                       FileBufferSize =  0;
    BOOLEAN                     BootPolicy =  TRUE;
    EFI_HANDLE                  RamDiskHandle =  NULL;
    EFI_STATUS                  Status =  EFI_UNSUPPORTED;
    EFI_LOAD_FILE_PROTOCOL      *LoadFile;
    EFI_DEVICE_PATH_PROTOCOL    *FullPath = NULL;
    
    //return NULL for invalid File Handle.
    if(LoadFileHandle == NULL)
       return NULL;

    Status = gBS->HandleProtocol(
            LoadFileHandle,
            &gEfiLoadFileProtocolGuid,
           (void**) &LoadFile);
    if(EFI_ERROR(Status))
    {
        return NULL;
    }
    
    //Get the Load File Size
    Status = LoadFile->LoadFile(
            LoadFile,
            FilePath,
            BootPolicy,
            &FileBufferSize,
            FileBuffer
            );
    if((EFI_WARN_FILE_SYSTEM != Status) && (EFI_BUFFER_TOO_SMALL != Status))
    {
        return NULL;
    }
    
    if(EFI_BUFFER_TOO_SMALL == Status)
    {
        UINTN  DPSize=0;
        EFI_DEVICE_PATH_PROTOCOL *HanDevicePath = NULL;
        EFI_DEVICE_PATH_PROTOCOL *FullDevicePath = NULL;
        HanDevicePath = EfiDevicePathFromHandle(LoadFileHandle);
        if(HanDevicePath)
        {
            DPSize = EfiDevicePathSize(HanDevicePath);
            FullDevicePath = EfiLibAllocateZeroPool(DPSize);
            if(FullDevicePath)
                MemCpy(FullDevicePath,HanDevicePath,DPSize);
        }
        //The Option buffer is directly returned by Load File.
        return FullDevicePath;
    }
    //The load option resides in RAM Disk.
    FileBuffer = AllocateReservedPages(EFI_SIZE_TO_PAGES(FileBufferSize));

    if(FileBuffer == NULL)
        return NULL;

    //Get the File Buffer.
    Status = LoadFile->LoadFile(
            LoadFile,
            FilePath,
            BootPolicy,
            &FileBufferSize,
            FileBuffer
            );
    if(EFI_ERROR(Status))
    {
       //Free the Memory Allocated to Ram Disk if the load file failed.
        FreePages(FileBuffer,EFI_SIZE_TO_PAGES(FileBufferSize));
        return NULL;
    }
    FullPath = ExpandNetWorkFileSystem(LoadFileHandle ,&RamDiskHandle );
    if(FullPath == NULL)
    {
       //Free the memory occupied by the RAM disk if there is no BLOCKIO or simpleFileSystem instance.
        DestoryRamDisk(EfiDevicePathFromHandle(RamDiskHandle));
    }    
    return FullPath;    
}

/**
 * The function to return the File path from the file system produced by the load file instance. 
 * 
 * @param LoadFileHandle        - The handle of the Loadfile Instance.
 * @param RamDiskHandle         - return the RAM Disk handle.
 *
 * @retval Device path          - The next possible full path pointing to the load option.
*/
EFI_DEVICE_PATH_PROTOCOL *ExpandNetWorkFileSystem(EFI_HANDLE LoadFileHandle ,EFI_HANDLE *RamDiskHandle)
{
   UINTN                    HandleCount = 0,Index = 0 ;
   EFI_HANDLE               *Handles;
   EFI_STATUS               Status = EFI_UNSUPPORTED;
   EFI_HANDLE               Handle =NULL;
   EFI_DEVICE_PATH_PROTOCOL *HanDevicePath = NULL;

   Status = gBS->LocateHandleBuffer(
           ByProtocol,
           &gEfiBlockIoProtocolGuid,
           NULL,
           &HandleCount,
           &Handles);
   if(EFI_ERROR(Status))
   {
       return NULL;
   }
   for(Index = 0;Index < HandleCount;Index++)
   {
       HanDevicePath = EfiDevicePathFromHandle(Handles[Index]);
       Status = gBS->LocateDevicePath(
               &gEfiLoadFileProtocolGuid,
               &HanDevicePath,
               &Handle);
       if(!EFI_ERROR(Status) && (Handle == LoadFileHandle) && (DevicePathType(HanDevicePath) == MEDIA_DEVICE_PATH) && (DevicePathSubType(HanDevicePath) == MEDIA_RAM_DISK_DP))
       {
          //Find the BlockIo insttance populated from the LoadFile.
           Handle = Handles[Index];
           break;
       }
   }
   
   if(Handles != NULL)
   {
       MemFreePointer((VOID **)&Handles);
   }
   if(Index == HandleCount)
       Handle = NULL;
   *RamDiskHandle = Handle;
   if(Handle != NULL)
   {
       UINTN  DPSize=0;
      EFI_DEVICE_PATH_PROTOCOL *FullDevicePath = NULL;
      HanDevicePath = EfiDevicePathFromHandle(Handle);
      if(HanDevicePath)
      {
          DPSize = EfiDevicePathSize(HanDevicePath);
          FullDevicePath = EfiLibAllocateZeroPool(DPSize);
          if(FullDevicePath)
              MemCpy(FullDevicePath,HanDevicePath,DPSize);
      }
      // Re-use ExpandMediaDevicePath() to get the full device path of load option.
      // But assume only one SimpleFileSystem can be found under the BlockIo.
      return ExpandMediaDevicePath(FullDevicePath,NULL);
       
   }
   else
   {
       return NULL;
   }
}

/**
 * The function the media device path which points to a BlockIo or SimpleFileSystem Instance,by appending EFI_REMOVABLE_FILE_NAME. 
 * 
 * @param DevicePath        - The Media Device path pointing to blockio or SimpleFileSystem instance.
 * @param FullPath          - The full path returned by the routine in last call.
 *
 * @retval Device path      - The Next possible full path pointing to the load option.
*/
EFI_DEVICE_PATH_PROTOCOL *ExpandMediaDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath,EFI_DEVICE_PATH_PROTOCOL *FullPath)
{
    
    BOOLEAN                             GetNext = 0;
    EFI_HANDLE                           Handle;
    EFI_STATUS                          Status = EFI_UNSUPPORTED;
    VOID                                *Buffer = NULL;
    UINTN                               Size,Index = 0;
    EFI_HANDLE                          *SimpleFileSystemHandles;
    UINTN                               FileSystemHandlesCount;
    UINTN                               TempSize;
    EFI_BLOCK_IO_PROTOCOL               *BlockIo;
    EFI_DEVICE_PATH_PROTOCOL            *TempDevicePath;
    
    if(FullPath == NULL)
    {
        GetNext = 1;
    }
    //Check whether the device is connected.
    TempDevicePath = DevicePath;
    Status = gBS->LocateDevicePath(
            &gEfiSimpleFileSystemProtocolGuid,
            &TempDevicePath,
            &Handle);
  
    if(!EFI_ERROR(Status))
    {
        if(!IsDevicePathEnd(TempDevicePath))
        {
            return NULL;
        }
        TempDevicePath = EfiFileDevicePath(Handle,EFI_REMOVABLE_MEDIA_FILE_NAME);
      
        //For Device path pointing to simple file system,it only expands to one of Full Path.
        if(GetNext)
        {
            return TempDevicePath;
        }else
        {
            MemFreePointer((VOID **)&TempDevicePath);
            return NULL;
        }

    }
    
    Status = gBS->LocateDevicePath(
            &gEfiBlockIoProtocolGuid,
            &TempDevicePath,
            &Handle);
    if(EFI_ERROR(Status))
        return NULL;
    // For device boot option only pointing to the removable device handle,
    // should make sure all its children handles (its child partion or media handles)
    // are created and connected.
    gBS->ConnectController (Handle, NULL, NULL, TRUE);
    

    // Issue a dummy read to the device to check for media change.
    // When the removable media is changed, any Block IO read/write will
    // cause the BlockIo protocol be reinstalled and EFI_MEDIA_CHANGED is
    // returned. After the Block IO protocol is reinstalled, subsequent
    // Block IO read/write will success.
    Status = gBS->HandleProtocol(
            Handle,
            &gEfiBlockIoProtocolGuid,
            (VOID **)&BlockIo);
    if(EFI_ERROR(Status))
        return NULL;
    Buffer = EfiLibAllocatePool(BlockIo->Media->BlockSize);
    if(Buffer != NULL)
    {
        BlockIo->ReadBlocks(
                BlockIo,
                BlockIo->Media->MediaId,
                0,
                BlockIo->Media->BlockSize,
                Buffer);
        MemFreePointer((VOID **)&Buffer);
    }
    
    //Detect the default boot file from Removable Media.
    TempDevicePath = NULL;
    Size = GetDevicePathSize(DevicePath)-END_DEVICE_PATH_LENGTH;
    Status = gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiSimpleFileSystemProtocolGuid,
            NULL,
            &FileSystemHandlesCount,
            &SimpleFileSystemHandles);
     for(Index=0;Index<FileSystemHandlesCount;Index ++)
     {
        //Get the Device path Size of SimpleFileSystem handle.
         TempDevicePath = EfiDevicePathFromHandle(SimpleFileSystemHandles[Index]);
         TempSize = GetDevicePathSize(TempDevicePath)- END_DEVICE_PATH_LENGTH;
         
         //Check whether the device path of boot option is part of the SimpleFileSystem handle's Device path. 
         if((Size <= TempSize) && (!MemCmp(TempDevicePath,DevicePath,Size)) )
         {
             TempDevicePath = EfiFileDevicePath(SimpleFileSystemHandles[Index],EFI_REMOVABLE_MEDIA_FILE_NAME);
             if(GetNext)
             {
                 break;
             }
             else
             {
                 GetNext = (BOOLEAN)(MemCmp((UINT8 *)TempDevicePath, (UINT8 *)DevicePath, DevicePathNodeLength(TempDevicePath)) == 0);
                 MemFreePointer((VOID **)&TempDevicePath);
                 TempDevicePath = NULL;
                         
             }
         }         
     }
     if(SimpleFileSystemHandles != NULL)
         MemFreePointer((VOID **)&SimpleFileSystemHandles);
     return TempDevicePath;
    
    

}

/**
 * Destory the RAM Disk.Unregister to the RAM DISK from RAM Disk Driver and Free the memory allocated to ram disk. 
 * 
 * @param RamDiskDevicePath        - Ram Disk Device Path
 *
 * @retval void
*/
void DestoryRamDisk(EFI_DEVICE_PATH_PROTOCOL *RamDiskDevicePath)
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    VOID *RamDiskBuffer = NULL;
    UINTN RamDiskSize=0;
    EFI_GUID RamDiskProtocol = { 0xab38a0df, 0x6873, 0x44a9, { 0x87, 0xe6, 0xd4, 0xeb, 0x56, 0x14, 0x84, 0x49 }};
    
    if(RamDiskDevicePath == NULL)
        return;
    
    if(gRamDiskProtocol == NULL)
    {
        Status = gBS->LocateProtocol(
                &RamDiskProtocol,
                NULL,
                (VOID *)&gRamDiskProtocol);
        if(EFI_ERROR(Status))
            return;
     }
    
    RamDiskBuffer = GetRamDiskMemoryInfo (RamDiskDevicePath, &RamDiskSize);
   
    //Unregister the RAM Disk.
    Status = gRamDiskProtocol->Unregister(RamDiskDevicePath);
    if(EFI_ERROR(Status))
        return;
    if(RamDiskBuffer != NULL)
        gBS->FreePages((EFI_PHYSICAL_ADDRESS )(UINTN)RamDiskBuffer,RamDiskSize); 
}

/**
 * The function to return the buffer and buffer size occupied by RAM Disk
 * 
 * @param RamDiskDevicePath        - RAM DISK Device Path.
 * @param RamDiskSizeInPages       - Return RAM DISK Size
 *
 * @retval VOID *  RAM Disk Buffer.
*/
VOID *GetRamDiskMemoryInfo (EFI_DEVICE_PATH_PROTOCOL *RamDiskDevicePath, UINTN *RamDiskSizeInPages )
{

  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;
  UINT64                      StartingAddr;
  UINT64                      EndingAddr;

  if(RamDiskDevicePath == NULL)
        return NULL;

  *RamDiskSizeInPages = 0;

  //
  // Get the buffer occupied by RAM Disk.
  //
  Status = gBS->LocateDevicePath (&gEfiLoadFileProtocolGuid, &RamDiskDevicePath, &Handle);
  if(EFI_ERROR(Status))
      return NULL;
  
  
  if(!((DevicePathType (RamDiskDevicePath) == MEDIA_DEVICE_PATH) && (DevicePathSubType (RamDiskDevicePath) == MEDIA_RAM_DISK_DP)))
  {
      return NULL;
  }
  StartingAddr = ReadUnaligned64 ((UINT64 *) ((MEDIA_RAM_DISK_DEVICE_PATH *) RamDiskDevicePath)->StartingAddr);
  EndingAddr   = ReadUnaligned64 ((UINT64 *) ((MEDIA_RAM_DISK_DEVICE_PATH *) RamDiskDevicePath)->EndingAddr);
  *RamDiskSizeInPages = EFI_SIZE_TO_PAGES ((UINTN) (EndingAddr - StartingAddr + 1));
  return (VOID *) (UINTN) StartingAddr;
}

/**
 * The function to return the RamDisk Devicepath From File path
 * 
 * @param FilePath                 - File System Device path.
 *
 * @retval VOID *  RAM Disk Path.
*/
EFI_DEVICE_PATH_PROTOCOL * GetRamDiskDevicePath (EFI_DEVICE_PATH_PROTOCOL *FilePath )
{
  EFI_STATUS                  Status;
  EFI_DEVICE_PATH_PROTOCOL    *RamDiskDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    *Node;
  EFI_HANDLE                  Handle;
  EFI_DEVICE_PATH_PROTOCOL  *Eop;

  Node = FilePath;
  Status = gBS->LocateDevicePath (&gEfiLoadFileProtocolGuid, &Node, &Handle);
  if (!EFI_ERROR (Status) &&
      (DevicePathType (Node) == MEDIA_DEVICE_PATH) &&
      (DevicePathSubType (Node) == MEDIA_RAM_DISK_DP)
      ) {

    //
    // Construct the device path pointing to RAM Disk
    //
    Node = NextDevicePathNode (Node);
    RamDiskDevicePath = DuplicateDevicePath (FilePath);
    Eop = (VOID *) ((UINTN) RamDiskDevicePath + ((UINTN) Node - (UINTN) FilePath));
    SetDevicePathEndNode (Eop);
    return RamDiskDevicePath;
  }
  return NULL;
}

/**
 * The function to return the BootData for the orphan boot options.
 * 
 * @param u16Option                 - BootNext Option.
 *
 * @retval BOOT_DATA *  BootData for orphan boot options.
*/

BOOT_DATA * BootGetBootDatafororphan(UINT16 u16Option)
{
    BOOT_DATA   *BootData = NULL;
    CHAR16      *VarName;
    UINT8       *bufPtr;
    UINTN       size = 0, Length = 2;
    BOOT_OPTION *buffer;
    UINTN       VarNameSize = (EfiStrLen(L"BOOTXXXX")+1)*2;
    BOOLEAN 	FreeBootData = FALSE;

    VarName = EfiLibAllocateZeroPool(VarNameSize);
    if(VarName == NULL)
        return NULL;  

    SPrint (VarName, VarNameSize, L"Boot%04X",u16Option);
    buffer = VarGetNvramName (VarName, &gEfiGlobalVariableGuid, NULL, &size);
    if(buffer == NULL)
        goto END;

    BootData = EfiLibAllocateZeroPool (sizeof(BOOT_DATA)); 
    if(BootData == NULL)
        goto END;

    HexStringToBuf((UINT8 *)(&(BootData->Option)),&Length,&(VarName[4]),NULL);
    BootData->Active = buffer->Active;
    BootData->Name = StrDup (buffer->Name);
    bufPtr = (UINT8 *)((CHAR16 *)buffer->Name + EfiStrLen( buffer->Name ) + 1);
    BootData->DevicePath = EfiLibAllocateZeroPool( buffer->PathLength );
    if (NULL != BootData->DevicePath)
    {
        MemCopy (BootData->DevicePath, bufPtr, buffer->PathLength);
    }
    else
    {
        FreeBootData = TRUE;
        goto END ;
    }
    
    bufPtr += buffer->PathLength;
    size -= (UINTN)bufPtr - (UINTN)buffer;
    if (size != 0)
    {
        BootData->LoadOptions = EfiLibAllocatePool( size );
        if ( BootData->LoadOptions != NULL )
        {
            BootData->LoadOptionSize = size;
            MemCopy( BootData->LoadOptions, bufPtr, size );
        }
        else
        {
            FreeBootData = TRUE;
            goto END ;
        }
    }
    END:	
    if(FreeBootData && BootData)
    {
        if( BootData->DevicePath)
            MemFreePointer( (VOID **)&BootData->DevicePath );
        if( BootData->Name)
            MemFreePointer( (VOID **)&BootData->Name );
        if(BootData->LoadOptions)
            MemFreePointer( (VOID **)&BootData->LoadOptions );      
        MemFreePointer( (VOID **)&BootData );
    }

    if(VarName)
        MemFreePointer( (VOID **)&VarName );
    if(buffer)
        MemFreePointer( (VOID **)&buffer );

    return BootData;
}
/**
 * The function to Return whether the PE header of the load option is valid or not.
 * 
 * @param Type                         - The load option type.
 * @param FileBuffer                   - The PE file buffer of the load option
 * @param FileSize                     - The size of the load option file.
 *
 * @retval BOOLEAN
*/
BOOLEAN IsLoadOptionPeHeaderValid (EFI_BOOT_MANAGER_LOAD_OPTION_TYPE Type,VOID *FileBuffer,UINTN FileSize )
{
  EFI_IMAGE_DOS_HEADER              *DosHeader;
  EFI_IMAGE_OPTIONAL_HEADER_UNION   *PeHeader;
  EFI_IMAGE_OPTIONAL_HEADER32       *OptionalHeader;
  UINT16                            Subsystem;

  if (FileBuffer == NULL || FileSize == 0) {
    return FALSE;
  }

  //
  // Read dos header
  //
  DosHeader = (EFI_IMAGE_DOS_HEADER *) FileBuffer;
  if (FileSize >= sizeof (EFI_IMAGE_DOS_HEADER) &&
      FileSize > DosHeader->e_lfanew && DosHeader->e_magic == EFI_IMAGE_DOS_SIGNATURE
      ) {
    //
    // Read and check PE signature
    //
    PeHeader = (EFI_IMAGE_OPTIONAL_HEADER_UNION *) ((UINT8 *) FileBuffer + DosHeader->e_lfanew);
    if (FileSize >= DosHeader->e_lfanew + sizeof (EFI_IMAGE_OPTIONAL_HEADER_UNION) &&
        PeHeader->Pe32.Signature == EFI_IMAGE_NT_SIGNATURE
        ) {
      //
      // Check PE32 or PE32+ magic, and machine type
      //
      OptionalHeader = (EFI_IMAGE_OPTIONAL_HEADER32 *) &PeHeader->Pe32.OptionalHeader;
      if ((OptionalHeader->Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC || 
           OptionalHeader->Magic == EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) &&
          EFI_IMAGE_MACHINE_TYPE_SUPPORTED (PeHeader->Pe32.FileHeader.Machine)
          ) {
        //
        // Check the Subsystem:
        //   Driver#### must be of type BootServiceDriver or RuntimeDriver
        //   SysPrep####, Boot####, OsRecovery####, PlatformRecovery#### must be of type Application
        //
        Subsystem = OptionalHeader->Subsystem;
        if ((Type == LoadOptionTypeMax) ||
            (Type == LoadOptionTypeDriver && Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER) ||
            (Type == LoadOptionTypeDriver && Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER) ||
            (Type == LoadOptionTypeSysPrep && Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION) ||
            (Type == LoadOptionTypeBoot && Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION) ||
            (Type == LoadOptionTypePlatformRecovery && Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION)
            ) {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}


/**
    This function calls connect for device which does not support the FileSystem, but supports the EFI_BLOCK_IO_PROTOCOL protocol.

    @param DevicePath: DevicePath of corresponding BootOption. 
             

    @retval Return Status .
 .

**/
EFI_STATUS ConnectDeviceonFailure(EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
    EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath = DevicePath;
    EFI_HANDLE Handle = NULL;
    EFI_STATUS Status;

    // Check device has BlockIo
    Status = gBS->LocateDevicePath(
            &gEfiBlockIoProtocolGuid,
            &TempDevicePath,
            &Handle);

    SETUP_DEBUG_TSE("\n[TSE] ConnectDeviceonFailure:LocateDevicePath(gEfiBlockIoProtocolGuid) Status %r, handle %X\n",Status,Handle);

    //if device has BlockIo ,then check filesystem..
    TempDevicePath = DevicePath;
    if(!EFI_ERROR(Status))
    { 
        // Check device has filesystem..
        Status = gBS->LocateDevicePath(
                &gEfiSimpleFileSystemProtocolGuid,
                &TempDevicePath,
                &Handle);
        SETUP_DEBUG_TSE("\n[TSE] ConnectDeviceonFailure:LocateDevicePath(gEfiSimpleFileSystemProtocolGuid) Status %r, handle %X\n",Status,Handle);

        // if device support BlockIo, not support FileSystem then return EFI_UNSUPPORTED.
        if (!(EFI_ERROR (Status)))
        {
            return EFI_UNSUPPORTED; 
        }
    }
    else
    {
        //if device has PciIo.
        Status = gBS->LocateDevicePath(
                &gEfiPciIoProtocolGuid,
                &TempDevicePath,
                &Handle);

        SETUP_DEBUG_TSE("\n[TSE] ConnectDeviceonFailure:LocateDevicePath(gEfiPciIoProtocolGuid) Status %r, handle %X\n",Status,Handle);
        if ((EFI_ERROR (Status)))
            return EFI_UNSUPPORTED; 
    }
    //If a device does not support the FileSystem, but supports the EFI_BLOCK_IO_PROTOCOL protocol, then the EFI Boot Service ConnectController must be called. 
    gBS->ConnectController (Handle, NULL, NULL, TRUE);
    return EFI_SUCCESS;
}

#if TSE_DEBUG_MESSAGES
VOID DebugShowPrintBootOptionDetails(EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
    UINT16 Index;
    CHAR16 *DevicePathString = NULL;
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DevPathToTextProtocol = (EFI_DEVICE_PATH_TO_TEXT_PROTOCOL*)NULL;
    EFI_GUID DevPathToTextGuid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
    EFI_STATUS Status = EFI_UNSUPPORTED;

    for(Index = 0; Index < gBootOptionCount; Index++)
    {
        if(!DPCmp( gBootData[Index].DevicePath, DevicePath))
        {
            SETUP_DEBUG_TSE("[TSE] Booting to Boot option : %s\n",gBootData[Index].Name); 
            break;
        }
    }
    Status = gBS->LocateProtocol(&DevPathToTextGuid,NULL,(void**)&DevPathToTextProtocol);
    if(!EFI_ERROR(Status))
    {
        DevicePathString =  DevPathToTextProtocol->ConvertDevicePathToText(DevicePath,TRUE,TRUE);
        if(DevicePathString)
        {   
            SETUP_DEBUG_TSE("[TSE] DevicePath %s\n", DevicePathString );
            SETUP_DEBUG_TSE("[TSE] DevicePath Type: %d, Subtype: %d\n",DevicePath->Type, DevicePath->SubType );
            MemFreePointer( (VOID **)&DevicePathString );
        }
    }
}
#endif

/**
 * The function checks DevicePath having subtype as MSG_IPv4_DP/MSG_IPv6_DP
 * If the device path subtype is MSG_IPv4_DP/MSG_IPv6_DP it will return TRUE else FLASE
 * 
 * @param DevicePath - The input device path structure.
 * 
 * @retval TRUE Device path PXE, FALSE otherwise
*/
BOOLEAN IsPxeHttpDevicePath(EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
    EFI_DEVICE_PATH_PROTOCOL *TempDevicePath=NULL;
    BOOLEAN IsPxe = FALSE;
    if(!DevicePath  || EFI_ERROR(IsValidDevicePath(DevicePath)))
        return FALSE;
    
    for(TempDevicePath=DevicePath;!IsDevicePathEndType(TempDevicePath);TempDevicePath = NextDevicePathNode(TempDevicePath))
    {
        if((TempDevicePath->Type == MESSAGING_DEVICE_PATH) && ((TempDevicePath->SubType == MSG_IPv4_DP) ||(TempDevicePath->SubType == MSG_IPv6_DP)))
        {
            IsPxe = TRUE;
            break;
        }
    }   
    return IsPxe;
}
