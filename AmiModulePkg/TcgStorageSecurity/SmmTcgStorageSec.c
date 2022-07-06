//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file SmmTcgStorageSecurity.c
    Provide functions to unlock OPAL device during S3 resume

**/

//---------------------------------------------------------------------------

#include "SmmTcgStorageSec.h"
#include <Protocol/IoMmu.h>
#include <Library/SmmConfidentialMemLib.h>
//---------------------------------------------------------------------------

#ifdef  SW_SMI_IO_ADDRESS
UINT8   gSwSmiPort = SW_SMI_IO_ADDRESS;
#else
// Default Sw Smi port
UINT8   gSwSmiPort = 0xB2;
#endif

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
AMI_NVME_PASS_THRU_PROTOCOL     *gSmmNvmePassThru = NULL;
#endif

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
AMI_AHCI_BUS_SMM_PROTOCOL    *gAhciSmmProtocol = NULL;
BOOLEAN                      gAhciInit         = FALSE;
UINT64                       PciExpressBaseAddress = 0;
#define PCI_CFG_ADDR( bus, dev, func, reg ) \
    ((VOID*)(UINTN) (PciExpressBaseAddress\
                     + ((bus) << 20) + ((dev) << 15) + ((func) << 12) + reg))

#define AHCI_BAR    0x24
#define PCI_SCC     0x000A        // Sub Class Code Register
#define RECEIVED_FIS_SIZE         0x100
#define COMMAND_LIST_SIZE_PORT    0x400
#endif

EDKII_IOMMU_PROTOCOL            *EdkiiIoMmu = NULL;
EFI_SMM_SW_DISPATCH2_PROTOCOL   *gSwDispatch;
EFI_SMM_SYSTEM_TABLE2           *gSmst2;
UINT8                           *gSecurityUnlockCommandBuffer = NULL;
// Remove below code when gAmiTcgStorageSecurityProtocolGuid
// definition is added in AmiModulePkg
#ifndef AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID
#define AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID \
        { 0xb396da3a, 0x52b2, 0x4cd6, { 0xa8, 0x9a, 0x13, 0xe7, 0xc4, 0xae, 0x97, 0x90 } }
#endif
EFI_GUID gAmiTcgStorageSecurityProtocolGuid = AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID;

/** @internal
    This function Locates the AhciSmm protocol from the Smm

    @param VariableGuid    - protocol guid to be located
    @param VariablePointer - pointer to return obtained protocol interface instance
    
    @retval EFI_SUCCESS    - If input protocol is found in SMST2 table
    @retval EFI_NOT_FOUND  - If input protocol not found in SMST2 table 
    
    @endinternal
**/
EFI_STATUS
LocateSMMServices (
    IN  EFI_GUID    *VariableGuid,
    IN  VOID        **VariablePointer
)
{
    UINTN           Index;

    for (Index = 0; Index < gSmst2->NumberOfTableEntries; ++Index) {
       if (CompareGuid(&gSmst2->SmmConfigurationTable[Index].VendorGuid,VariableGuid)) {
            break;
        }
    }

    if (Index != gSmst2->NumberOfTableEntries) {
        *VariablePointer = gSmst2->SmmConfigurationTable[Index].VendorTable;
        return EFI_SUCCESS;
    }

    return EFI_NOT_FOUND;
}

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )

ACTIVE_NAMESPACE_DATA*
GetActiveNameSpace (
    AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    UINT32                         NamespaceId
)
{
    LIST_ENTRY            *LinkData;
    ACTIVE_NAMESPACE_DATA *ActiveNameSpace = NULL;
    
    if(!NvmeController) {
        return NULL;
    }

    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList; 
            LinkData = LinkData->ForwardLink) {

        ActiveNameSpace = BASE_CR(LinkData, ACTIVE_NAMESPACE_DATA, Link);
        
        // If NamespaceId is 0xFFFFFFFF, then return the first valid ActiveNameSpace
        if( NamespaceId == 0xFFFFFFFF ) {
            return ActiveNameSpace;
        }

        if(ActiveNameSpace->ActiveNameSpaceID == NamespaceId) {
            return ActiveNameSpace;
        }
    }
    
    return NULL;
}
/** @internal
    This function locates NVMe controller SMM protocols and save as global pointer

    @param NvmeControllerHandle - NVMe controller handle
    @param SmmNvmeController    - Located NVMe SMM protocol instance

    @retval EFI_SUCCESS     - Located NVMe SMM protocol successfully
    @retval EFI_NOT_FOUND   - Located NVMe SMM protocol Failed
    
    @endinternal
**/
EFI_STATUS
TcgLocateNvmeSmmProtocols(
    IN  EFI_HANDLE                    NvmeControllerHandle,
    IN  AMI_NVME_CONTROLLER_PROTOCOL  **SmmNvmeController
)
{
    EFI_STATUS  Status = EFI_NOT_FOUND;
    EFI_HANDLE  *HandleBuffer = NULL;
    UINTN       Index, HandleBufferSize = 0;
    ACTIVE_NAMESPACE_DATA *ActiveNameSpace = NULL;

    if (gSmmNvmePassThru == NULL) {
        
        Status = gSmst2->SmmLocateProtocol(&gAmiSmmNvmePassThruProtocolGuid, NULL, (VOID**)&gSmmNvmePassThru);

        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    Status = gSmst2->SmmLocateHandle( ByProtocol,
                                      &gAmiSmmNvmeControllerProtocolGuid,
                                      NULL,
                                      &HandleBufferSize,
                                      HandleBuffer );

    if ( EFI_ERROR(Status) ) {
        if ( Status == EFI_BUFFER_TOO_SMALL ) {
            Status = gSmst2->SmmAllocatePool( EfiRuntimeServicesCode,
                                              HandleBufferSize,
                                              (VOID**)&HandleBuffer );
            if ( EFI_ERROR(Status) ) {
                ASSERT(FALSE);
                return Status;
            }

            Status = gSmst2->SmmLocateHandle( ByProtocol,
                                              &gAmiSmmNvmeControllerProtocolGuid,
                                              NULL,
                                              &HandleBufferSize,
                                              HandleBuffer );
        }
    }

    if( !EFI_ERROR(Status) ) {
        for(Index=0; Index < HandleBufferSize/sizeof(EFI_HANDLE); Index++) {
 
            Status = gSmst2->SmmHandleProtocol( HandleBuffer[Index],
                                                &gAmiSmmNvmeControllerProtocolGuid,
                                                (VOID**)SmmNvmeController );

            ActiveNameSpace = GetActiveNameSpace( *SmmNvmeController, MAX_UINT32 ); 
 
            if( !EFI_ERROR(Status) && (ActiveNameSpace->NvmeDeviceHandle == NvmeControllerHandle) ) {
                Status = EFI_SUCCESS;
                break;
            }
        }

        if( Index == HandleBufferSize/sizeof(EFI_HANDLE) ) {
            Status = EFI_NOT_FOUND;
        }
    }

    // Free HandleBuffer memory
    if( HandleBuffer ) {
        gSmst2->SmmFreePool(HandleBuffer);
    }

    return Status;
}
#endif

#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
/** @internal
    Function RstTcgSMMUnlockNvmeDevice

    @param OpalPassword

    @retval EFI_STATUS
    @endinternal
**/
EFI_STATUS
RstTcgSMMUnlockNvmeDevice (
    IN  TCG_STORAGE_SECURITY_PASSWORD    *OpalPassword
)
{
    EFI_STATUS          Status = EFI_SUCCESS;
    OPAL_DEVICE         *OpalDevConfig;
    EFI_HANDLE          *HandleBuffer = NULL;
    UINTN               Index, HandleBufferSize = 0;
    EFI_GUID            RaidSmmSSCPProtocolGuid = RST_SMM_STORAGE_PROTOCOL_GUID;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *StorageSecurityProtocol = NULL;
    
    Status = TcgSecAllocatePool( EfiRuntimeServicesData,
                                 sizeof(OPAL_DEVICE),
                                 (VOID**)&OpalDevConfig );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }    

    Status = gSmst2->SmmLocateHandle( ByProtocol, 
                                      &RaidSmmSSCPProtocolGuid,
                                      NULL,
                                      &HandleBufferSize, 
                                      HandleBuffer );
                                                                      
    if( EFI_ERROR(Status) && (Status == EFI_BUFFER_TOO_SMALL )) {
        
        Status = gSmst2->SmmAllocatePool( EfiRuntimeServicesCode,
                                          HandleBufferSize,
                                          (VOID**)&HandleBuffer );
        if( EFI_ERROR(Status) ) {
            ASSERT(FALSE);
            return Status;
        }
        
        Status = gSmst2->SmmLocateHandle( ByProtocol, 
                                          &RaidSmmSSCPProtocolGuid,
                                          NULL,
                                          &HandleBufferSize, 
                                          HandleBuffer );

    }

    //Using every StorageSecurity protocol installed by Rst driver to unlock this drive's password during S3
    //If there are drives set as the same password , we are able to unlock it at once.
    for(Index=0; Index<(HandleBufferSize/sizeof(EFI_HANDLE)); Index++) {

        ZeroMem (OpalDevConfig, sizeof(OPAL_DEVICE));
        
        StorageSecurityProtocol = NULL;
        Status = gSmst2->SmmHandleProtocol( HandleBuffer[Index], 
                                            &RaidSmmSSCPProtocolGuid, 
                                            (VOID **)&StorageSecurityProtocol );
        
        if( EFI_ERROR(Status) ) continue;
                    
        OpalDevConfig->BusInterfaceType = AmiStorageInterfaceNvme;
        OpalDevConfig->RstControlled = TRUE;
        OpalDevConfig->MediaId = 0; // Force MediaId to 0        
        OpalDevConfig->StorageSecurityProtocol = StorageSecurityProtocol;
        
        Status = GetOpalDeviceStatus(OpalDevConfig);

        // if NVMe device is locked then send Unlock command
        if ( !EFI_ERROR(Status) && (OpalDevConfig->LockingFeature & SecurityLockedMask) ) {
            Status = SendSecurityUnlockCommand ( OpalPassword, OpalDevConfig );
        } 
    }

    // Free HandleBuffer memory
    if( HandleBuffer ) TcgSecFreePool (HandleBuffer);

    // Free HandleBuffer memory
    if( OpalDevConfig ) TcgSecFreePool (OpalDevConfig);
    
    return EFI_SUCCESS;
}
#endif
/**
    This function saves the OPAL drive password and other information
    necessary to unlock OPAL password during S3 Resume.

    @param DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
    @param Context         Points to an optional handler context which was specified when the
                           handler was registered.
    @param CommBuffer      A pointer to a collection of data in memory that will
                           be conveyed from a non-SMM environment into an SMM environment.
    @param CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS Command is handled successfully.
  
**/
EFI_STATUS
EFIAPI
SaveOPALPassword (
    IN  EFI_HANDLE      DispatchHandle,
    IN  CONST   VOID    *DispatchContext OPTIONAL,
    IN  OUT VOID        *CommBuffer      OPTIONAL,
    IN  OUT UINTN       *CommBufferSize  OPTIONAL
)
{

    TCG_STORAGE_SECURITY_PASSWORD     *StoredDeviceList;
    TCG_STORAGE_SECURITY_PASSWORD     *OpalPassword;
    LIST_ENTRY                        *ListEntry;
    UINT8                             Index;
    BOOLEAN                           UserOrMaster = FALSE;
    BOOLEAN                           CheckFlag;
    EFI_STATUS                        Status = EFI_SUCCESS;
    UINT64                            TcgSmiUnRegSignature =  0;
    
    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL) {
        return EFI_SUCCESS;
    }
    //Validate if CommBuffer address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)CommBuffer, sizeof(UINT64));
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\n SaveOPALPassword() ComBuffer overlaps with SMRAM space" ));
        return EFI_SUCCESS;
    }
    // Check whether this Communicate call is initiated to UnRegister this SMI handler
    TcgSmiUnRegSignature = *(UINT64  *)CommBuffer;
        
    if (TcgSmiUnRegSignature == TCG_SMI_UNREGISTER_SIGNATURE) {
        DEBUG((DEBUG_VERBOSE,"*** UnRegisterSaveRemovePwdSmiHandlers inside SMM*** \n"));
        Status = gSmst2->SmiHandlerUnRegister(DispatchHandle);
        ASSERT_EFI_ERROR(Status);
        return EFI_SUCCESS;
    }
    
    //  This Communicate call is initiated to Save password data in SMRAM
    OpalPassword = (TCG_STORAGE_SECURITY_PASSWORD *)CommBuffer;

    //  Check if signature is present or not
    if ( OpalPassword->Signature != OPAL_PASSWORD_SIGNATURE) {
        return EFI_NOT_FOUND;
    }

    // Validate if OpalPassword address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)OpalPassword, sizeof(TCG_STORAGE_SECURITY_PASSWORD));
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\nSaveOPALPassword() ComBuffer overlaps with SMRAM region" ));
        return EFI_SUCCESS;
    }
    //  Check whether info about the OPAL device is already present
    
    for (ListEntry = gOpalPasswordList.ForwardLink; ListEntry != &gOpalPasswordList; \
             ListEntry = ListEntry->ForwardLink) {
        StoredDeviceList = BASE_CR(ListEntry,TCG_STORAGE_SECURITY_PASSWORD, LinkList );
        CheckFlag = FALSE;
		
        // Check if drive is connected to same controller
        if ((StoredDeviceList->BusNo      != OpalPassword->BusNo   ) ||
            (StoredDeviceList->DeviceNo   != OpalPassword->DeviceNo) ||
            (StoredDeviceList->FunctionNo != OpalPassword->FunctionNo)) {
            continue;
        }

        if(StoredDeviceList->BusInterfaceType == AmiStorageInterfaceAhci) {
            if ((StoredDeviceList->PortNumber   == OpalPassword->PortNumber) &&
                (StoredDeviceList->PMPortNumber == OpalPassword->PMPortNumber)) {
                CheckFlag = TRUE;
            }
        } else if(StoredDeviceList->BusInterfaceType == AmiStorageInterfaceIde ) {
            return EFI_UNSUPPORTED;
        } else { // i.e. StoredDeviceList->BusInterfaceType == AmiStorageInterfaceNvme
            if ( StoredDeviceList->DevHandle == OpalPassword->DevHandle ) {
                CheckFlag = TRUE;
            }
        }

        if (CheckFlag) {
            UserOrMaster = (BOOLEAN)((OpalPassword->Control) & 0x01);
            StoredDeviceList->Control = UserOrMaster;
            
            // Validating password buffer and Allocating confidential memory for only MasterPassword and UserPassword  
            // Memory for TCG_STORAGE_SECURITY_PASSWORD is allocated when saving device info
            
            if(UserOrMaster){
                Status = AmiValidateMemoryBuffer((VOID*)OpalPassword->MasterPassword, OPAL_PASSWORD_LENGTH);
                if (EFI_ERROR(Status)) {
                    DEBUG((DEBUG_VERBOSE,"\nMasterPassword Buffer overlaps with SMRAM region" ));
                    return EFI_SUCCESS;
                }
                if (StoredDeviceList->MasterPassword == NULL) {
                    Status = SmmAllocateConfidentialPool(OPAL_PASSWORD_LENGTH, (void**)(&StoredDeviceList->MasterPassword));
                    if (EFI_ERROR(Status) || (StoredDeviceList->MasterPassword == NULL)) {
                        return Status;
                    }
                }
            // Match has been found. Just copy the Password buffer
                for (Index = 0; Index < OPAL_PASSWORD_LENGTH; Index++) {
                    StoredDeviceList->MasterPassword[Index] = OpalPassword->MasterPassword[Index];
                }

            } else {
                Status = AmiValidateMemoryBuffer((VOID*)OpalPassword->UserPassword, OPAL_PASSWORD_LENGTH);
                if (EFI_ERROR(Status)) {
                    DEBUG((DEBUG_VERBOSE,"\n UserPassword Buffer overlaps with SMRAM region" ));
                    return EFI_SUCCESS;
                }
                if (StoredDeviceList->UserPassword == NULL) {
                    Status = SmmAllocateConfidentialPool(OPAL_PASSWORD_LENGTH, (void**)(&StoredDeviceList->UserPassword));
                    if (EFI_ERROR(Status) || (StoredDeviceList->UserPassword == NULL)) {
                        return Status;
                    }
                }
                
            // Match has been found. Just copy the Password buffer
                for (Index = 0; Index < OPAL_PASSWORD_LENGTH; Index++) {
                    StoredDeviceList->UserPassword[Index] = OpalPassword->UserPassword[Index];
                }
            }
            return Status;
        }
    }

    //  Match has not been found. Allocate memory and copy the buffer.
    if (gSmst2->SmmAllocatePool(EfiRuntimeServicesData, sizeof(TCG_STORAGE_SECURITY_PASSWORD), (VOID**)&StoredDeviceList) != EFI_SUCCESS){
        return Status;
    }

    for (Index = 0; Index < sizeof (TCG_STORAGE_SECURITY_PASSWORD); Index++)  {
        ((UINT8 *)StoredDeviceList)[Index] = ((UINT8 *)OpalPassword)[Index];
    }
    InsertTailList(&gOpalPasswordList, &StoredDeviceList->LinkList);

    return Status;
}

/** 
    This Function unlocks OPAL password during S3 resume.

    @param DispatchHandle  Handle to the Dispatcher
    @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT.
    @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
    @param CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS    Command is handled successfully.

**/
EFI_STATUS
EFIAPI
UnlockOPALPassword (
    IN  EFI_HANDLE       DispatchHandle,
    IN  CONST VOID       *DispatchContext OPTIONAL,
    IN  OUT VOID         *CommBuffer OPTIONAL,
    IN  OUT UINTN        *CommBufferSize OPTIONAL
)
{
    TCG_STORAGE_SECURITY_PASSWORD    *StoredDeviceList;
    LIST_ENTRY                       *ListEntry;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    TCG_STORAGE_SECURITY_PASSWORD    *StoredDeviceList2;
    LIST_ENTRY                       *ListEntry2;
#endif
    EFI_STATUS      Status = EFI_SUCCESS;

    //Locate the Edk2IommuProtocol
    gSmst2->SmmLocateProtocol( &gEdkiiIoMmuProtocolGuid, 
	                           NULL, 
                              (VOID**)&EdkiiIoMmu);
    for (ListEntry = gOpalPasswordList.ForwardLink; ListEntry != &gOpalPasswordList; \
            ListEntry = ListEntry->ForwardLink) {
            StoredDeviceList = BASE_CR(ListEntry,TCG_STORAGE_SECURITY_PASSWORD,LinkList);
        if (StoredDeviceList->Signature == OPAL_PASSWORD_SIGNATURE) {
            if(StoredDeviceList->BusInterfaceType == AmiStorageInterfaceIde ) {
                return EFI_UNSUPPORTED;
            } else if (StoredDeviceList->BusInterfaceType == AmiStorageInterfaceAhci ) {
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
                // Check if this is the first entry for this controller.
                // If yes, we have to make sure the controller will be fully re-initialized
                gAhciInit = FALSE;
                for (ListEntry2 = ListEntry->BackLink; ListEntry2 != &gOpalPasswordList; \
                    ListEntry2 = ListEntry2->BackLink) {
                    StoredDeviceList2 = BASE_CR(ListEntry2, TCG_STORAGE_SECURITY_PASSWORD, LinkList);
                    if ((StoredDeviceList2->BusNo      == StoredDeviceList->BusNo) &&
                        (StoredDeviceList2->DeviceNo   == StoredDeviceList->DeviceNo) &&
                        (StoredDeviceList2->FunctionNo == StoredDeviceList->FunctionNo)) {
                        // there was another one previously, so initialization is already done
                        gAhciInit = TRUE;
                        break;
                    }
                }
                TcgSMMUnlockAhciDevice (StoredDeviceList);
#endif
            } else if(StoredDeviceList->BusInterfaceType == AmiStorageInterfaceNvme){
				if(StoredDeviceList->RstControlled == TRUE){
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
                    RstTcgSMMUnlockNvmeDevice(StoredDeviceList);
#endif
                }else{
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
                TcgSMMUnlockNvmeDevice(StoredDeviceList);
#endif
                }    
            }
        }
    }

    return Status;
}

/**
    This Function removes OPAL password from the internal database

    @param DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
    @param Context         Points to an optional handler context which was specified when the
                           handler was registered.
    @param CommBuffer      A pointer to a collection of data in memory that will
                           be conveyed from a non-SMM environment into an SMM environment.
    @param CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS Command is handled successfully.

**/
EFI_STATUS
EFIAPI
RemoveOPALPassword (
    IN  EFI_HANDLE      DispatchHandle,
    IN  CONST VOID      *DispatchContext OPTIONAL,
    IN  OUT VOID        *CommBuffer      OPTIONAL,
    IN  OUT UINTN       *CommBufferSize  OPTIONAL
)
{
    TCG_STORAGE_SECURITY_PASSWORD    *StoredDeviceList;
    TCG_STORAGE_SECURITY_PASSWORD    *OpalPassword;
    LIST_ENTRY                       *ListEntry;
    EFI_STATUS                       Status=EFI_SUCCESS;
    UINT64                           TcgSmiUnRegSignature =  0;

    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL) {
        return EFI_SUCCESS;
    }
    // Validate if CommBuffer address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)CommBuffer, sizeof(UINT64));
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\n RemoveOPALPassword() ComBuffer overlaps with SMRAM space" ));
        return EFI_SUCCESS;
    }
    // Check whether this Communicate call is initiated to UnRegister this SMI handler
    TcgSmiUnRegSignature = *(UINT64  *)CommBuffer;
           
    if (TcgSmiUnRegSignature == TCG_SMI_UNREGISTER_SIGNATURE) {
        DEBUG((DEBUG_VERBOSE,"*** Unregister RemoveOPALPassword\n"));
        Status = gSmst2->SmiHandlerUnRegister(DispatchHandle);
        ASSERT_EFI_ERROR(Status);
        return EFI_SUCCESS;
    }
    
    //  This Communicate call is initiated to remove password data from SMRAM
    OpalPassword = (TCG_STORAGE_SECURITY_PASSWORD *)CommBuffer;
    
    //  Check if signature is present or not
    if (OpalPassword->Signature != OPAL_PASSWORD_SIGNATURE) {
        return EFI_NOT_FOUND;
    }

    // Validate if OpalPassword address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)OpalPassword, sizeof(TCG_STORAGE_SECURITY_PASSWORD));
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_VERBOSE,"\n RemoveOPALPassword() ComBuffer overlaps with SMRAM region" ));
        return EFI_SUCCESS;
    }
    
    //  Check whether info about the OPAL device is already present
    for ( ListEntry = gOpalPasswordList.ForwardLink; ListEntry != &gOpalPasswordList;\
              ListEntry = ListEntry->ForwardLink) {
        StoredDeviceList = BASE_CR(ListEntry,TCG_STORAGE_SECURITY_PASSWORD,LinkList);

        // Check if drive is connected to same controller
        if ((StoredDeviceList->BusNo      != OpalPassword->BusNo   ) ||
            (StoredDeviceList->DeviceNo   != OpalPassword->DeviceNo) ||
            (StoredDeviceList->FunctionNo != OpalPassword->FunctionNo)) {
            continue;
        }
        // Freepool the allocated Confidential memory if match found
        if(StoredDeviceList->BusInterfaceType == AmiStorageInterfaceAhci) {
            if ((StoredDeviceList->PortNumber   == OpalPassword->PortNumber) &&
              (StoredDeviceList->PMPortNumber == OpalPassword->PMPortNumber)) {
                if(OpalPassword->Control & 0x01){
                    if(StoredDeviceList->MasterPassword)SmmFreeConfidentialPool((void**)&StoredDeviceList->MasterPassword);
                } else {
                    if(StoredDeviceList->UserPassword)SmmFreeConfidentialPool((void**)&StoredDeviceList->UserPassword);
                }
                RemoveEntryList( &StoredDeviceList->LinkList);
                break;
            }
        } else if(StoredDeviceList->BusInterfaceType == AmiStorageInterfaceIde) {
            return EFI_UNSUPPORTED;
        } else { // StoredHDDList->BusInterfaceType == AmiStorageInterfaceNvme
            if ( StoredDeviceList->DevHandle == OpalPassword->DevHandle ) {
                if(OpalPassword->Control & 0x01){
                    if(StoredDeviceList->MasterPassword)SmmFreeConfidentialPool((void**)&StoredDeviceList->MasterPassword);
                } else {
                    if(StoredDeviceList->UserPassword)SmmFreeConfidentialPool((void**)&StoredDeviceList->UserPassword);
                }
                RemoveEntryList(&StoredDeviceList->LinkList);
                break;
            }
        }
    }
    return Status;
}

/**
  This Function Saves SMI value and SW SMI Port Address
  in Boot Script for S3 resume during ReadytoBoot

  @param DispatchHandle  Handle to the Dispatcher
  @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT.
  @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
  @param CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS Command is handled successfully.

**/
EFI_STATUS
EFIAPI
BootScriptSaveTcgSecSmmEventForS3 (
    IN  EFI_HANDLE       DispatchHandle,
    IN  CONST VOID       *DispatchContext OPTIONAL,
    IN  OUT VOID         *CommBuffer OPTIONAL,
    IN  OUT UINTN        *CommBufferSize OPTIONAL
)
{
    EFI_STATUS      Status;
    UINT8           Value = TCG_SW_SMI_OPAL_UNLOCK_PASSWORD;

    //  Check if any device is locked to trigger
    //  Unlock SMI during S3 resume.
    if (!IsListEmpty(&gOpalPasswordList)) {
        Status = S3BootScriptSaveIoWrite (
                   S3BootScriptWidthUint8,
                   (UINTN)gSwSmiPort,
                   1,
                   &Value
                   );
        ASSERT_EFI_ERROR(Status);
    }
    // UnRegister SMI handler as we have saved SW SMI port address & context to the boot script
    Status = gSmst2->SmiHandlerUnRegister (DispatchHandle);
    ASSERT_EFI_ERROR(Status);
    
    return EFI_SUCCESS;
}

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
/**
    This function registers Boot script for Nvme devices to initialize Nvme
    controller along with PCI root bridge BAR,CMD, BUS PCI Configuration
    values in S3 resume path

  @param DispatchHandle  Handle to the Dispatcher
  @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT.
  @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
  @param CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS    Command is handled successfully.

**/
EFI_STATUS
EFIAPI
TcgBootScriptSaveNvmeEventForS3 (
    IN EFI_HANDLE       DispatchHandle,
    IN CONST VOID       *DispatchContext OPTIONAL,
    IN OUT VOID         *CommBuffer OPTIONAL,
    IN OUT UINTN        *CommBufferSize OPTIONAL
 )
{
    UINT32 SmmNvmeSaveEntryCount = 0;
    UINTN NvmeBootScriptSaveBufferSize = 0;  
    UINT32 EntryIndex = 0;
    UINTN  PciAddress = 0;
    EFI_BOOT_SCRIPT_WIDTH  Width      = 0;
    UINTN Value      = 0;
    BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY *NvmeBootScriptSaveBuffer = NULL;
    EFI_STATUS  Status;

    // If input is invalid, stop processing this SMI
    if (CommBuffer == NULL || CommBufferSize == NULL ) {
        return EFI_SUCCESS;
    }
    
    NvmeBootScriptSaveBufferSize = *CommBufferSize;

    // Confirm that communication buffer contains required data
    NvmeBootScriptSaveBuffer = (BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY *)CommBuffer;
    
    // Make sure that "NvmeBootScriptSaveBufferSize" should be greater than 8
    if( (NvmeBootScriptSaveBufferSize <= 2*sizeof(UINT32)) ) {
       DEBUG((DEBUG_VERBOSE,"\n TcgBootScriptSaveNvmeEventForS3() CommBufferSize is invalid" ));
       return EFI_SUCCESS;
    }
    
    // Validate if NvmeBootScriptSaveBuffer address is an non-SMRAM region to avoid SMRAM data 
    // corruption through SMI handlers
    Status = AmiValidateMemoryBuffer((VOID*)NvmeBootScriptSaveBuffer, NvmeBootScriptSaveBufferSize);
    if (EFI_ERROR(Status)) {
       DEBUG((DEBUG_VERBOSE,"\n TcgBootScriptSaveNvmeEventForS3() ComBuffer passed with NVMe BootScript overlaps with SMRAM space" ));
       return EFI_SUCCESS;
    }
    
    SmmNvmeSaveEntryCount = *(UINT32*)(((UINT8*)NvmeBootScriptSaveBuffer + NvmeBootScriptSaveBufferSize) - sizeof(UINT32));
    
    //"SmmNvmeSaveEntryCount" value should be greater than 1 as it is known that 
    // more than one Bootscript entry for NVMe device must have been added by HddSecurity DXE driver
    if(SmmNvmeSaveEntryCount ==0){
       return EFI_SUCCESS;
    }
   
    Status = AmiValidateMemoryBuffer((VOID*)NvmeBootScriptSaveBuffer, \
                                            SmmNvmeSaveEntryCount * sizeof(BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY) + 2*sizeof(UINT32) );
    if (EFI_ERROR(Status)) {
         DEBUG((DEBUG_VERBOSE,"\n ComBuffer passed with NVMe BootScript overlaps with SMRAM space. Either ComBuffer or CommBufferSize invalid. " ));
         return EFI_SUCCESS;
    }
    // Check for Signature at the End of "NvmeBootScriptSaveBuffer" CommBuffer
    if( *(UINT32*)(&NvmeBootScriptSaveBuffer[SmmNvmeSaveEntryCount]) != NVME_SECURITY_SAVE_SIGNATURE ) {
         DEBUG((DEBUG_VERBOSE,"\n ComBuffer passed with NVMe BootScript doesn't have valid signature" ));
         return EFI_SUCCESS;
    }

    // Parse the NvmeBootScriptSaveBuffer for the Entry List and save the values to the BootScript.
    for( EntryIndex = 0; EntryIndex < SmmNvmeSaveEntryCount; EntryIndex++ ) {

        PciAddress = NvmeBootScriptSaveBuffer[EntryIndex].PciAddress;
        Width = NvmeBootScriptSaveBuffer[EntryIndex].Width;
        Value = NvmeBootScriptSaveBuffer[EntryIndex].Value;

        Status = S3BootScriptSavePciCfgWrite (
                  Width,
                  PciAddress,
                  1,
                  &Value
                  );

        DEBUG((DEBUG_VERBOSE,"BootScriptSaveNvmeSecSmmEventForS3 .... %x %x %x %x %x",EntryIndex, PciAddress, Width, Value, Status ));
        ASSERT_EFI_ERROR(Status);
    }
    // UnRegister SMI handler as Nvme Boot Script stored for S3.
    Status = gSmst2->SmiHandlerUnRegister (DispatchHandle);
    ASSERT_EFI_ERROR(Status);
    
    return EFI_SUCCESS;
}
#endif

/**
    This function saves BS buffer globally in SMM

    @param DispatchHandle  Handle to the Dispatcher
    @param Context         A Pointer to EFI_SMM_SW_REGISTER_CONTEXT.
    @param CommBuffer      A pointer to EFI_SMM_SW_CONTEXT.
    @param CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS    Command is handled successfully.

**/
EFI_STATUS
EFIAPI
TcgGetNonSmramDataTransferBuffer (
    IN EFI_HANDLE       DispatchHandle,
    IN CONST VOID       *DispatchContext OPTIONAL,
    IN OUT VOID         *CommBuffer OPTIONAL,
    IN OUT UINTN        *CommBufferSize OPTIONAL
)
{
    EFI_STATUS    Status;
    // IMPORTANT: Preserve this non-SMRAM buffer and
    // Use it as host controller communication buffer in SMM
    gSecurityUnlockCommandBuffer = (UINT8 *)(*(UINTN *)CommBuffer);

    // UnRegister SMI handler
    Status = gSmst2->SmiHandlerUnRegister (DispatchHandle);
    ASSERT_EFI_ERROR(Status);

    return EFI_SUCCESS;
}


/** @internal
    This function registers handlers for Save,Remove and Unlock SMI handlers.

    @param VOID
    @retval EFI_SUCCESS - Handlers installed successfully.
    
    @endinternal
**/
EFI_STATUS
RegisterTcgStorageSecSmiHandlers (
 )
{
    EFI_SMM_SW_REGISTER_CONTEXT Unlock_OPAL_Password = {TCG_SW_SMI_OPAL_UNLOCK_PASSWORD};
    EFI_STATUS  Status;
    EFI_HANDLE  Handle;

    Status = gSmst2->SmiHandlerRegister(
                        SaveOPALPassword,
                        &gTcgStorageSecSavePwdGuid,
                        &Handle
                        );

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    Status = gSmst2->SmiHandlerRegister(
                        RemoveOPALPassword,
                        &gTcgStorageSecRemovePwdGuid,
                        &Handle
                        );

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    Status = gSwDispatch->Register(
                        gSwDispatch,
                        UnlockOPALPassword,
                        &Unlock_OPAL_Password,
                        &Handle
                        );
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    Status = gSmst2->SmiHandlerRegister(
                                BootScriptSaveTcgSecSmmEventForS3,
                                &gTcgStorageSecSmiBootScriptSaveTriggerGuid,
                                &Handle
                                );
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )

    Status = gSmst2->SmiHandlerRegister(
                           TcgBootScriptSaveNvmeEventForS3,
                          &gTcgStorageSecNvmeBootScriptSaveTriggerGuid,
                          &Handle
                          );
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }
#endif

    Status = gSmst2->SmiHandlerRegister(
                        TcgGetNonSmramDataTransferBuffer,
                        &gTcgGetNonSmramDataTransferBufferGuid,
                        &Handle
                        );

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
    }

    return EFI_SUCCESS;
}


/**
    Initializes TCGStorageSecurity SMM Driver.

    @param ImageHandle - Handle assigned to this driver
    @param SystemTable - EFI System table.

    @retval EFI_STATUS
**/

EFI_STATUS
EFIAPI
TcgStorageSecSMMInit (
    IN  EFI_HANDLE       ImageHandle,
    IN  EFI_SYSTEM_TABLE *SystemTable
 )
{
    EFI_STATUS              Status;
    EFI_SMM_BASE2_PROTOCOL  *SmmBase2;
    Status = gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&SmmBase2);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = SmmBase2->GetSmstLocation( SmmBase2, &gSmst2);

    if (EFI_ERROR(Status)) {
        return EFI_UNSUPPORTED;
    }

    Status  = gSmst2->SmmLocateProtocol( &gEfiSmmSwDispatch2ProtocolGuid, \
                                         NULL, \
                                         (VOID**)&gSwDispatch );

    if (EFI_ERROR(Status)) {
        return EFI_UNSUPPORTED;
    }

    InitializeListHead(&gOpalPasswordList);
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    PciExpressBaseAddress = PcdGet64 (PcdPciExpressBaseAddress);
#endif
    RegisterTcgStorageSecSmiHandlers();
    return EFI_SUCCESS;
}

/** @internal
    Routine to Unlock OPAL AHCI controller in SMM

    @param pOpalPassword    - TCG_STORAGE_SECURITY_PASSWORD instance.

    @retval EFI_SUCCESS     - AhciController is unlocked successfully
    
    @endinternal
**/
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
EFI_STATUS
TcgSMMUnlockAhciDevice (
    TCG_STORAGE_SECURITY_PASSWORD       *pOpalPassword
)
{
    EFI_STATUS         Status;
    UINT32             AhciBar = 0;
    UINT32             NoofPorts=0;
    OPAL_DEVICE        *OpalDevConfig;
    UINT8              SccReg = 0 ;
    BOOLEAN            ControllerinAhciMode = FALSE;
    UINT32             GlobalControl;
    UINT64             FisBaseAddress=0;
    UINT64             OrgFisBaseAddress=0;
    UINT64             PortCommandListBaseAddr=0;
    
    if (EFI_ERROR( LocateSMMServices(&gAmiAhciSmmProtocolGuid,(VOID **)&gAhciSmmProtocol))) {
        return EFI_DEVICE_ERROR;
    }

    //    1. Check if Controller is in AHCI Mode.
    //    2. Read ABAR Offset and Get HbaAddress.
    //    3. Disable AHCI_ENABLE.
    //    4. Issue Controller Reset.
    //    5. Wait for HBA Reset to Complete.
    //    6. Enable AHCI_ENABLE.

    AhciBar = *(UINT32*)PCI_CFG_ADDR(pOpalPassword->BusNo,
                                     pOpalPassword->DeviceNo,
                                     pOpalPassword->FunctionNo,
                                     AHCI_BAR);
    AhciBar &= 0xFFFFFFF0;

    gAhciSmmProtocol->AhciBaseAddress = AhciBar;

    // Find Number of Ports from BIT0:BIT4 of the HBA_CAP register
    NoofPorts = ((MmioRead32(AhciBar + HBA_CAP)) & HBA_CAP_NP_MASK) + 1;

    // Check if AHCI MMIO address space resides in SMRAM region. If yes, don't proceed.
    if( AmiValidateMmioBuffer((VOID*)(UINT64)gAhciSmmProtocol->AhciBaseAddress, (HBA_PORTS_START + (NoofPorts * HBA_PORTS_REG_WIDTH)) ) ) {
        return EFI_NOT_FOUND;
    }

    if(!gAhciInit) {
        SccReg = *(UINT8*)PCI_CFG_ADDR(pOpalPassword->BusNo,
                                       pOpalPassword->DeviceNo,
                                       pOpalPassword->FunctionNo,
                                       PCI_SCC);    // Get Scc Register;

        if((SccReg & 0x06) || (SccReg & 0x04)) {
            ControllerinAhciMode = TRUE;
        }

        if(ControllerinAhciMode) {
            GlobalControl = MmioRead32(AhciBar + 0x04);
            
            GlobalControl &= 0x7FFFFFFF;
            MmioWrite32(AhciBar + 0x04, GlobalControl);

            GlobalControl = 0x01;
            MmioWrite32(AhciBar + 0x04, GlobalControl);

            MicroSecondDelay(5000);   // 5 milliSec Delay

            GlobalControl = 0x80000000;
            MmioWrite32(AhciBar + 0x04, GlobalControl);

            MicroSecondDelay(1000000);   // 1 Second Delay
        }
        gAhciInit=TRUE;
    }

    if (EdkiiIoMmu) { 
        Status = EdkiiIoMmu->AllocateBuffer(
                                       EdkiiIoMmu,
                                       0,
                                       0,
                                       EFI_SIZE_TO_PAGES(RECEIVED_FIS_SIZE + 0x100), 
                                       (VOID**)&FisBaseAddress,
                                       0);
        ASSERT_EFI_ERROR (Status);
        
        OrgFisBaseAddress = gAhciSmmProtocol->PortFISBaseAddr;
        gAhciSmmProtocol->PortFISBaseAddr =((FisBaseAddress & (~0xFF))+ 0x100);
        ZeroMem ((VOID *)gAhciSmmProtocol->PortFISBaseAddr, RECEIVED_FIS_SIZE);
        
        Status = EdkiiIoMmu->AllocateBuffer(
                                    EdkiiIoMmu,
                                    0,
                                    0,
                                    EFI_SIZE_TO_PAGES(COMMAND_LIST_SIZE_PORT * 2),
                                    (VOID**)&PortCommandListBaseAddr,
                                    0);
        ASSERT_EFI_ERROR (Status);
        gAhciSmmProtocol->PortCommandListBaseAddr = PortCommandListBaseAddr;
        gAhciSmmProtocol->PortCommandTableBaseAddr = gAhciSmmProtocol->PortCommandListBaseAddr + 0x80;
        ZeroMem ((VOID *)gAhciSmmProtocol->PortCommandListBaseAddr, COMMAND_LIST_SIZE_PORT);
        
    }
	
    //
    //Resuming from S3. So bring back the AHCI controller to working state
    //
    gAhciSmmProtocol->AhciSmmInitPortOnS3Resume(gAhciSmmProtocol,pOpalPassword->PortNumber);
    
    Status = TcgSecAllocatePool( EfiRuntimeServicesData,
                                 sizeof(OPAL_DEVICE),
                                (VOID**)&OpalDevConfig );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    ZeroMem (OpalDevConfig, sizeof(OPAL_DEVICE));
    // Update the BusInterfaceType
    OpalDevConfig->BusInterfaceType = AmiStorageInterfaceAhci;
    OpalDevConfig->PortNumber = pOpalPassword->PortNumber;
    OpalDevConfig->PMPortNumber = pOpalPassword->PMPortNumber;
	OpalDevConfig->DeviceHandle = pOpalPassword->DevHandle;
    Status = GetOpalDeviceStatus(OpalDevConfig);
    DEBUG ((DEBUG_VERBOSE, "\n SmmTcgStorageSec:AHCI GetOpalDeviceStatus = %r", Status));
    if (!EFI_ERROR(Status)) {


	    // if the device is locked then send Unlock command
	    if (OpalDevConfig->LockingFeature & SecurityLockedMask ) {
	        OpalDevConfig->MediaId = 0;
	        Status = SendSecurityUnlockCommand ( pOpalPassword, OpalDevConfig );
	        DEBUG ((DEBUG_VERBOSE, "\n SmmTcgStorageSec: SendAhciSecurityUnlockCommand = %r", Status));
	    } else {
	        // If device NOT locked, send BlockSID command to freeze security.
	        Status = TcgSmmBlockSID( OpalDevConfig );
	    }
    }
	
    if (EdkiiIoMmu) {
        Status = EdkiiIoMmu->FreeBuffer(
                                    EdkiiIoMmu,
                                    EFI_SIZE_TO_PAGES(COMMAND_LIST_SIZE_PORT * 2),
                                    (void *)PortCommandListBaseAddr);
        ASSERT_EFI_ERROR (Status);
        
        Status = EdkiiIoMmu->FreeBuffer(
                                    EdkiiIoMmu,
                                    EFI_SIZE_TO_PAGES(RECEIVED_FIS_SIZE + 0x100),
                                    (void *)FisBaseAddress);
        ASSERT_EFI_ERROR (Status);
        
        gAhciSmmProtocol->PortFISBaseAddr = OrgFisBaseAddress;
    }
    return Status;
}
#endif

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )

/** @internal
    Routine to Unlock OPAL NVMe controller in SMM

    @param pOpalPassword    - TCG_STORAGE_SECURITY_PASSWORD instance.

    @retval EFI_SUCCESS     - NvmeController is unlocked successfully
    
    @endinternal
**/
EFI_STATUS
TcgSMMUnlockNvmeDevice (
    IN  TCG_STORAGE_SECURITY_PASSWORD    *pOpalPassword
)
{
    EFI_STATUS                    Status = EFI_SUCCESS;
    OPAL_DEVICE                   *OpalDevConfig;
    AMI_NVME_CONTROLLER_PROTOCOL  *SmmNvmeController;
    UINT64                        NvmeBarOffset = 0;

    if (EFI_ERROR( TcgLocateNvmeSmmProtocols( pOpalPassword->DevHandle, &SmmNvmeController ) )) {
        return EFI_DEVICE_ERROR;
    }
    // Check if the controller is already running. If yes check the Admin Completion Queue base address.
    NvmeBarOffset = SmmNvmeController->NvmeBarOffset;

    if (MmioRead32((UINTN)(NvmeBarOffset + Offset_CC)) & 0x1) {
        if (MmioRead32((UINTN)(NvmeBarOffset + Offset_CSTS)) & 0x1) {
            // If Admin Completion Queue base address is not the same as the one AMI NVME bus driver loaded 
            // we should not unlock the device.
            // This is to prevent device unlocking if SMI is invoked from a source other than during S3 resume.
            if( MmioRead64((UINTN)(NvmeBarOffset + Offset_Acq))!= 
                    SmmNvmeController->AdminCompletionQueueMappedAddr) {
                return EFI_UNSUPPORTED;
            }
        }
    }
    
    Status = SmmNvmeController->NvmeInitController(SmmNvmeController);

    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    Status = TcgSecAllocatePool( EfiRuntimeServicesData,
                                 sizeof(OPAL_DEVICE),
                                 (VOID**)&OpalDevConfig );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    ZeroMem (OpalDevConfig, sizeof(OPAL_DEVICE));
    SmmNvmeController->OpalConfig = (VOID*)OpalDevConfig;
    OpalDevConfig->BusInterfaceType = AmiStorageInterfaceNvme;
    OpalDevConfig->DeviceHandle = pOpalPassword->DevHandle;
    Status = GetOpalDeviceStatus(OpalDevConfig );
    DEBUG ((DEBUG_VERBOSE, "\n SmmTcgStorageSec: NVMe GetOpalDeviceStatus = %r", Status));
    if (EFI_ERROR(Status)) {
        return EFI_DEVICE_ERROR;
    }

    // if NVMe device is locked then send Unlock command
    if (OpalDevConfig->LockingFeature & SecurityLockedMask ) {
        OpalDevConfig->MediaId = 0; // Force MediaId to 0
        Status = SendSecurityUnlockCommand ( pOpalPassword, OpalDevConfig );

        DEBUG ((DEBUG_VERBOSE, "\n SmmTcgStorageSec: SendNvmeSecurityUnlockCommand = %r", Status));
    } else {
        // If device NOT locked, send BlockSID command to freeze security.
        Status = TcgSmmBlockSID( OpalDevConfig );
        DEBUG ((DEBUG_VERBOSE, "\n SmmTcgStorageSec: TcgSmmBlockSID = %r", Status));
    }
    //SMM IOMMU
    Status = SmmNvmeController->NvmeStopController(SmmNvmeController);
    return Status;
}
#endif
/** @internal
    Function to Get OPAL device status in SMM

    @param OpalDevConfig    - OPAL_DEVICE instance.

    @retval EFI_SUCCESS     - NvmeController is unlocked successfully
    
    @endinternal
**/
EFI_STATUS
GetOpalDeviceStatus(
    IN  OPAL_DEVICE                      *OpalDevConfig
)
{
    EFI_STATUS                     Status=EFI_UNSUPPORTED;
    LEVEL0_DISCOVERY_DATA          *SmmQueryBuffer =\
          (LEVEL0_DISCOVERY_DATA *)(gSecurityUnlockCommandBuffer);  // Use Global buffer for NVMe communication
    UINTN                           TotalLength;
    FEATURE_DESCRIPTOR_OPALITE_SSC  *OpaliteSSCFeature;
    FEATURE_DESCRIPTOR              *FeatureDescriptor;
    UINT16                          FeatureCode;
    FEATURE_DESCRIPTOR_BLOCK_SID    *BlockSid;
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL    *SmmNvmeController;
	UINTN                           QueryBufferSize = 0x200; 	
#endif
    
    ZeroMem (SmmQueryBuffer, sizeof(LEVEL0_DISCOVERY_DATA));

    if(OpalDevConfig->BusInterfaceType == AmiStorageInterfaceNvme) {
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        
        if(OpalDevConfig->RstControlled == TRUE){
            
            Status = OpalDevConfig->StorageSecurityProtocol->ReceiveData( OpalDevConfig->StorageSecurityProtocol,
                                OpalDevConfig->MediaId,
                                10 * 10000000,
                                SECURITY_PROTOCOL_1,   // Security Protocol ID
                                ToBigLittleEndianWord(COMID_LEVEL0_DISCOVERY), // SP specific id
                                0x200,                 // TranferLength
                                SmmQueryBuffer,
                                &QueryBufferSize
                                );

        } else {
            if (EFI_ERROR( TcgLocateNvmeSmmProtocols( OpalDevConfig->DeviceHandle, &SmmNvmeController))) {
            
                return EFI_DEVICE_ERROR;
            }
            
            Status = SmmNvmeSendReceiveData(SmmNvmeController,
                                        FALSE,                  //SECURITY_RECEIVE
                                        OpalDevConfig->MediaId,
                                        NVME_COMMAND_TIMEOUT * 10000000,
                                        SECURITY_PROTOCOL_1,   // Security Protocol ID
                                        ToBigLittleEndianWord(COMID_LEVEL0_DISCOVERY), // SP specific id
                                        0x200,                 // TranferLength
                                        SmmQueryBuffer
                                        );
        }
#endif
    } else if (OpalDevConfig->BusInterfaceType == AmiStorageInterfaceAhci) {
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
        Status = SmmAhciSendReceiveData( gAhciSmmProtocol,
                                        FALSE,              //SECURITY_RECEIVE
                                        OpalDevConfig->MediaId,
                                        OpalDevConfig->PortNumber,
                                        OpalDevConfig->PMPortNumber,
                                        SATA_COMMAND_TIMEOUT * 10000000,
                                        SECURITY_PROTOCOL_1,   // Security Protocol ID
                                        ToBigLittleEndianWord(COMID_LEVEL0_DISCOVERY), // SP specific id
                                        0x200,                 // TranferLength
                                        SmmQueryBuffer
                                        );
#endif
    }

    if (!EFI_ERROR(Status)) {
        TotalLength = ToBigLittleEndianDword(SmmQueryBuffer->Level0DiscoveryHeader.Length) + 4;
        FeatureDescriptor = (FEATURE_DESCRIPTOR *)((UINT8 *)SmmQueryBuffer + sizeof(LEVEL0_DISCOVERY_HEADER));
        do {
            FeatureCode = ToBigLittleEndianWord(FeatureDescriptor->FeatureCode);
            switch (FeatureCode) {
                case    Locking_Feature:
                    OpalDevConfig->LockingFeature = FeatureDescriptor->Data[0];
                     break;

                // Get ComID value from Level0 data
                case    OPAL_SSC_V200_Feature:
                case    OPALITE_SSC_Feature:
                case    PYRITE_SSC_Feature: 
                case    PYRITE_SSC_Feature_v2_0:
                case    ENTERPRISE_SSC_Feature:
                case    RUBY_SSC_Feature:   
                    OpaliteSSCFeature = (FEATURE_DESCRIPTOR_OPALITE_SSC *) FeatureDescriptor;
                    OpalDevConfig->OpalFeature.BaseComID = ToBigLittleEndianWord(OpaliteSSCFeature->BaseComID);
                    OpalDevConfig->OpalFeature.NumberofComIDs = ToBigLittleEndianWord(OpaliteSSCFeature->NumberofComIDs);
                    break;

                case    Block_SID_Authentification_Feature: // Update BlockSID data to freeze security in S3 Resume.
                    BlockSid = (FEATURE_DESCRIPTOR_BLOCK_SID *) FeatureDescriptor;
                    OpalDevConfig->BlockSid.FeatureCode = ToBigLittleEndianWord(BlockSid->FeatureCode);
                    OpalDevConfig->BlockSid.SIDStateValue = BlockSid->SIDStateValue;
                    OpalDevConfig->BlockSid.SIDBlockedState = BlockSid->SIDBlockedState;
                    OpalDevConfig->BlockSid.HWReset = BlockSid->HWReset;
                    break;

                default:
                    break;
            }
            FeatureDescriptor = (FEATURE_DESCRIPTOR *)((UINT8 *)FeatureDescriptor + FeatureDescriptor->Length + 4);
        }  while ((UINT8 *)FeatureDescriptor < (UINT8 *)((UINT8 *)SmmQueryBuffer + TotalLength));
    }

    return Status;

}


/** @internal
    Function to send (or) Receive Security protocol commands in SMM

    @param  AhciSmmProtocol - AMI_AHCI_BUS_SMM_PROTOCOL instance
    @param  MediaId         - ID of the medium to receive data from.
    @param  Timeout         - The timeout, in 100ns units, to use for the execution of the
                    security protocol command. A Timeout value of 0 means that this
                    function will wait indefinitely for the security protocol command to
                    execute. If Timeout is greater than zero, then this function will
                    return.
    @param  SecurityProtocolId - Security protocol ID of the security protocol
                                 command to be sent.
    @param  SecurityProtocolSpecificData - Security protocol specific portion of
                                           the security protocol command.
    @param  PayloadBufferSize - Size in bytes of the payload data buffer.
    @param  PayloadBuffer - A pointer to a destination buffer to store the security
                        protocol command specific payload data for the security protocol
                        command. The caller is responsible for either having implicit or
                        explicit ownership of the buffer.


    @retval EFI_SUCCESS     The security protocol command completed successfully.
    @retval EFI_UNSUPPORTED The given MediaId does not support security
                            protocol commands.
    @retval EFI_DEVICE_ERROR      The security protocol command completed with an error.
    @retval EFI_INVALID_PARAMETER The PayloadBuffer or PayloadTransferSize is
                                  NULL and PayloadBufferSize is non-zero.
    
    @endinternal
**/
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
EFI_STATUS
SmmAhciSendReceiveData(
    IN  AMI_AHCI_BUS_SMM_PROTOCOL   *AhciSmmProtocol,
    IN  BOOLEAN       SendReceive,
    IN  UINT32        MediaId,
    IN  UINT8         PortNumber,
    IN  UINT8         PMPortNumber,
    IN  UINT64        Timeout,
    IN  UINT8         SecurityProtocolId,
    IN  UINT16        SecurityProtocolSpecificData,
    IN  UINTN         PayloadBufferSize,
    OUT VOID          *PayloadBuffer
) {
    EFI_STATUS               Status;
    COMMAND_STRUCTURE        CommandStructure;
    VOID                     *Mapping = NULL;
    EDKII_IOMMU_OPERATION     Operation;
	EFI_PHYSICAL_ADDRESS      DeviceAddress;
	
    ZeroMem (&CommandStructure, sizeof(COMMAND_STRUCTURE));
	
    if(EdkiiIoMmu) {
        if (SendReceive) {
           Operation = EdkiiIoMmuOperationBusMasterRead;
        } else {
           Operation = EdkiiIoMmuOperationBusMasterWrite;
        }
        Status = EdkiiIoMmu->Map(
                           EdkiiIoMmu,
                           Operation,
                           PayloadBuffer,
                           &PayloadBufferSize,
                           &DeviceAddress,
                           &Mapping);
						   
        if (EFI_ERROR(Status)){
            return Status;
        }
        
         PayloadBuffer = (VOID*)DeviceAddress;
	}
    
    if (PayloadBufferSize != 0) {

        if (PayloadBuffer == NULL) {
            return EFI_INVALID_PARAMETER;
        }

        CommandStructure.LBAMid     = (UINT8)(SecurityProtocolSpecificData >> 8);
        CommandStructure.LBAHigh    = (UINT8)SecurityProtocolSpecificData;
        CommandStructure.SectorCount = 1;
        if (SendReceive) {
            CommandStructure.Command    = TRUSTED_SEND;
        } else {
            CommandStructure.Command    = TRUSTED_RECEIVE;
        }
        CommandStructure.Features   = SecurityProtocolId;
        CommandStructure.Timeout    = (Timeout < 10000) ? 1 : DivU64x32 (Timeout, 10000);
        CommandStructure.Buffer     = PayloadBuffer;
        CommandStructure.ByteCount  = (UINT32)PayloadBufferSize;


        Status = AhciSmmProtocol->AhciSmmExecutePioDataCommand (AhciSmmProtocol,
                                          &CommandStructure,
                                          PortNumber,
                                          PMPortNumber,
                                          0,   //Device Type - ATA
                                          SendReceive
                                          );

    } else {
        //
        // For PayloadBufferSize zero, Trusted Non Data command should be used
        // BIT 24 indicates Trusted Receive/Send
        //
        CommandStructure.LBAMid     = (UINT8)(SecurityProtocolSpecificData >> 8);
        CommandStructure.LBAHigh    = (UINT8)SecurityProtocolSpecificData;
        if (!SendReceive) {
            CommandStructure.LBALowExp  = 0x01; // Trusted Receive
        }
        CommandStructure.Command    = TRUSTED_NON_DATA;
        CommandStructure.Features   = SecurityProtocolId;
        CommandStructure.Timeout    = (Timeout < 10000) ? 1 : DivU64x32 (Timeout, 10000);
        CommandStructure.Buffer     = PayloadBuffer;
        CommandStructure.ByteCount  = (UINT32)PayloadBufferSize;

        Status = AhciSmmProtocol->AhciSmmExecuteNonDataCommand (AhciSmmProtocol,
                        CommandStructure,
                        PortNumber,
                        PMPortNumber,
                        0    //Device Type - ATA
                        );
    }
	
    if (Mapping){
        EdkiiIoMmu->Unmap(EdkiiIoMmu, Mapping);    
    }
    return Status;

}
#endif
/** @internal
    Function to send Security protocol commands in SMM

    @param  SmmNvmeController - AMI_NVME_CONTROLLER_PROTOCOL instance
    @param  MediaId           - ID of the medium to receive data from.
    @param  Timeout - The timeout, in 100ns units, to use for the execution of the
                    security protocol command. A Timeout value of 0 means that this
                    function will wait indefinitely for the security protocol command to
                    execute. If Timeout is greater than zero, then this function will
                    return.
    @param  SecurityProtocolId - Security protocol ID of the security protocol
                                 command to be sent.
    @param  SecurityProtocolSpecificData - Security protocol specific portion of
                                           the security protocol command.
    @param  PayloadBufferSize - Size in bytes of the payload data buffer.
    @param  PayloadBuffer - A pointer to a destination buffer to store the security
                        protocol command specific payload data for the security protocol
                        command. The caller is responsible for either having implicit or
                        explicit ownership of the buffer.


    @retval EFI_SUCCESS     The security protocol command completed successfully.
    @retval EFI_UNSUPPORTED The given MediaId does not support security
                            protocol commands.
    @retval EFI_DEVICE_ERROR      The security protocol command completed with an error.
    @retval EFI_INVALID_PARAMETER The PayloadBuffer or PayloadTransferSize is
                                  NULL and PayloadBufferSize is non-zero.

    @endinternal
**/
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
EFI_STATUS
SmmNvmeSendReceiveData (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *SmmNvmeController,
    IN  BOOLEAN       SendReceive,
    IN  UINT32        MediaId,
    IN  UINT64        Timeout,
    IN  UINT8         SecurityProtocolId,
    IN  UINT16        SecurityProtocolSpecificData,
    IN  UINTN         PayloadBufferSize,
    OUT VOID          *PayloadBuffer
)
{
    EFI_STATUS                      Status;
    NVME_COMMAND_WRAPPER            *NvmeCmdWrapper = SmmNvmeController->NvmeCmdWrapper;
    UINTN                           MappedBufferSize = PayloadBufferSize;
    EFI_PHYSICAL_ADDRESS            MappedBuffer;
    UINTN                           PRP1TransferSize;
    COMPLETION_QUEUE_ENTRY          CompletionData;
    EDKII_IOMMU_OPERATION           Operation;
    VOID                            *Mapping = NULL;
    EFI_PHYSICAL_ADDRESS            DeviceAddress;
   
    if (EdkiiIoMmu){
     
        if (SendReceive) {
            Operation = EdkiiIoMmuOperationBusMasterRead;
        } else {
            Operation = EdkiiIoMmuOperationBusMasterWrite;
        }
        
        Status = EdkiiIoMmu->Map(
                            EdkiiIoMmu,
                            Operation,
                            PayloadBuffer,
                            &PayloadBufferSize,
                            &DeviceAddress,
                            &Mapping);
        
        if (EFI_ERROR(Status)){
            return Status;
        }
        
         PayloadBuffer = (VOID*)DeviceAddress;
    }
    //  Clear the Buffer
    ZeroMem (NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER));

    MappedBuffer = (EFI_PHYSICAL_ADDRESS) PayloadBuffer;

    NvmeCmdWrapper->NvmCmd.PRP1 = (UINT64) MappedBuffer;

    PRP1TransferSize = SmmNvmeController->MemoryPageSize -
                        (MappedBuffer & ((UINTN)(SmmNvmeController->MemoryPageSize) - 1));

    // If all data can be transferred using only PRP1 then do that.
    if (PRP1TransferSize >= MappedBufferSize) {
        PRP1TransferSize = MappedBufferSize;
    }

    // Do we need PRP2
    if (MappedBufferSize - PRP1TransferSize) {
        NvmeCmdWrapper->NvmCmd.PRP2 = NvmeCmdWrapper->NvmCmd.PRP1 + PRP1TransferSize;
    }

    //Swap SecurityProtocolSpecificData bytes
    NvmeCmdWrapper->NvmCmd.CDW10 = (SecurityProtocolId << 24) + ((SecurityProtocolSpecificData & 0xFF) << 16) +\
                                    (SecurityProtocolSpecificData & 0xFF00);
    NvmeCmdWrapper->NvmCmd.CDW11 = (UINT32)PayloadBufferSize;

    // Build NVME command
    if (SendReceive) {
        NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SECURITY_SEND;
    } else {
        NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SECURITY_RECEIVE;
    }
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = SmmNvmeController->NvmeDxeSmmDataArea->CommandIdentifierAdmin;
    NvmeCmdWrapper->NvmCmd.NSID = 0;
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->SQIdentifier = 0;       // Admin Queue
    NvmeCmdWrapper->CmdTimeOut = (UINT32)(Timeout < 10000 ? 1 : DivU64x32 (Timeout, 10000));

    Status = gSmmNvmePassThru->ExecuteNvmeCmd (SmmNvmeController, NvmeCmdWrapper, &CompletionData);

    if (Mapping){
        EdkiiIoMmu->Unmap(EdkiiIoMmu, Mapping);    
    }
    
    return Status;
}
#endif
/** @internal
    Wrapper function to invoke Security protocol command API in SMM

    @param  OpalDevice  - OPAL_DEVICE instance
    @param  SecurityProtocolId - Security protocol ID of the security protocol
                                 command to be sent.
    @param  SPSpecificID - Security protocol specific portion of
                           the security protocol command.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.


    @retval EFI_STATUS

    @endinternal
**/

EFI_STATUS
ExecuteSendTcgCmd (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               SecurityProtocolID,
    IN  UINT16              SPSpecificID,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength
)
{

    EFI_STATUS                    Status = EFI_DEVICE_ERROR;
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL  *SmmNvmeController;
#endif
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)
    PrintCmdBlock (Buffer);
#endif
    if (OpalDevice->BusInterfaceType == AmiStorageInterfaceNvme) {
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        
        if(OpalDevice->RstControlled == TRUE){
            
            Status = OpalDevice->StorageSecurityProtocol->SendData( 
                                                OpalDevice->StorageSecurityProtocol,
                                                OpalDevice->MediaId,
                                                10 * 10000000,
                                                SecurityProtocolID,   
                                                SPSpecificID,           
                                                BufferLength,
                                                Buffer
                                                );

        } else {
            
            if (EFI_ERROR( TcgLocateNvmeSmmProtocols( OpalDevice->DeviceHandle, &SmmNvmeController))) {
                return EFI_DEVICE_ERROR;
            }
            Status = SmmNvmeSendReceiveData(SmmNvmeController,
                                        TRUE,                // SECURITY_SEND
                                        OpalDevice->MediaId,
                                        NVME_COMMAND_TIMEOUT * 10000000,
                                        SecurityProtocolID,   // Security Protocol ID
                                        SPSpecificID,        // SP specific id
                                        BufferLength,         // TranferLength
                                        Buffer
                                       );
            
        }
#endif
    } else if (OpalDevice->BusInterfaceType == AmiStorageInterfaceAhci) {
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
        Status = SmmAhciSendReceiveData(gAhciSmmProtocol,
                                    TRUE,                // SECURITY_SEND
                                    OpalDevice->MediaId,
                                    OpalDevice->PortNumber,
                                    OpalDevice->PMPortNumber,
                                    SATA_COMMAND_TIMEOUT * 10000000,
                                    SecurityProtocolID,   // Security Protocol ID
                                    SPSpecificID,        // SP specific id
                                    BufferLength,         // TranferLength
                                    Buffer
                                   );
#endif
    }

    return Status;

}

/** @internal
    Routine sends SecurityUnlock command to the Sata/ Nvme controller in SMM

    @param  pOpalPassword     - TCG_STORAGE_SECURITY_PASSWORD instance
    @param  OpalDevConfig     - OPAL_DEVICE instance.

    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
SendSecurityUnlockCommand (
    IN   TCG_STORAGE_SECURITY_PASSWORD   *pOpalPassword,
    IN   OPAL_DEVICE                     *OpalDevConfig
)
{

    EFI_STATUS          Status;
    UINT8               ColumnValue;
    VOID                *Buffer = (VOID*)gSecurityUnlockCommandBuffer;
    UINTN               BufferLength = 512;
    UINT8               PasswordBuffer[OPAL_PASSWORD_LENGTH+1];
    SESSION_PARAMS      SessionParams;
    BOOLEAN             AdminUser = FALSE;
    CHAR8               ColumnName7[] = "ReadLocked";
    CHAR8               ColumnName8[] = "WriteLocked";
    UINT64              AuthObj1, AuthObj2;
    UINT64              LockingRangeUid = LOCKING_GLOBAL_RANGE_UID;
    UINT16              SPSpecificID=ToBigLittleEndianWord(OpalDevConfig->OpalFeature.BaseComID);

    ZeroMem (Buffer,BufferLength);
    ZeroMem (&SessionParams, sizeof(SESSION_PARAMS));
    ZeroMem (PasswordBuffer,(OPAL_PASSWORD_LENGTH +1));

    if ((BOOLEAN)((pOpalPassword->Control) & 0x01) ) {
        // unlock with Admin Password
        CopyMem(PasswordBuffer, pOpalPassword->MasterPassword, OPAL_PASSWORD_LENGTH);

        AdminUser = TRUE;
    } else {
        // unlock with User Password
        CopyMem(PasswordBuffer, pOpalPassword->UserPassword, OPAL_PASSWORD_LENGTH);

    }

    if (IS_ENTERPRISE_DEVICE(OpalDevConfig)) {
        AuthObj2 = (AdminUser == 1) ? BANDMASTER0_UID : BANDMASTER1_UID;
        AuthObj1 = LOCKING_SP_UID_ENYTERPRISE;
        Status = OpenSessionAndAuthenticate(OpalDevConfig, &SessionParams, Buffer, BufferLength, AuthObj1, AuthObj2, \
                       PasswordBuffer, (UINT32)AsciiStrLen((CHAR8 *)PasswordBuffer));
        TCG_STORAGE_ERR_CHECK(Status);  
        LockingRangeUid = (AdminUser == 1) ? LOCKING_GLOBAL_RANGE_UID : LOCKING_BAND1_UID_ENTERPRISE; 
    } else {

        DEBUG ((DEBUG_VERBOSE, "\n SMM: Open Session with LockingSP = %r\n, Status"));

        // Open session to Admin SP as  the SID authority using the MSID password
        SessionParams.InvokingID = SM_UID;
        SessionParams.MethodID = START_SESSION_UID;
        SessionParams.HostSessionID = 1;
        SessionParams.SPID = LOCKING_SP_UID;
        SessionParams.HostChallengeLength = AsciiStrLen((CHAR8 *) PasswordBuffer); ; //OpalDevice->LockingSPAdmin1Length;
        SessionParams.HostChallenge = PasswordBuffer; //OpalDevice->LockingSPAdmin1Pin;
        SessionParams.HostSigningAuthority = AdminUser ? LOCKING_SP_ADMIN1_UID : (UserMMMM + LShiftU64 (1,56));
        SessionParams.Write = 1;
    
        Status = SMUidStartSession (OpalDevConfig, &SessionParams, Buffer, BufferLength);
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
        if (EFI_ERROR(Status)) {
            SessionParams.HostChallengeLength = OPAL_PASSWORD_LENGTH; //OpalDevice->LockingSPAdmin1Length;
            Status = SMUidStartSession (OpalDevConfig, &SessionParams, Buffer, BufferLength);
        }
#endif
        TCG_STORAGE_ERR_CHECK(Status);
    }

    if (IS_ENTERPRISE_DEVICE(OpalDevConfig)) {
//# AptioV_Override_START: EIP#642147 Address Security Advisory SA50093
        ZeroMem(Buffer, BufferLength);
//# AptioV_Override_END: EIP#642147 Address Security Advisory SA50093
        ColumnValue = 0;
        Status = BuildSetCmdBlock (OpalDevConfig, &SessionParams, Buffer, BufferLength, LockingRangeUid,\
                         7, ColumnName7, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, TRUE, FALSE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        Status = BuildSetCmdBlock (OpalDevConfig, &SessionParams, Buffer, BufferLength, LockingRangeUid,\
                         8, ColumnName8, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, FALSE, TRUE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        Status = ExecuteSendTcgCmd(OpalDevConfig, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
//# AptioV_Override_START: EIP#642147 Address Security Advisory SA50093
        ZeroMem(Buffer, BufferLength);
//# AptioV_Override_END: EIP#642147 Address Security Advisory SA50093
        Status = GetResponse (OpalDevConfig, Buffer, BufferLength, &SessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    } else {
        // Set ReadLocked to FALSE in Locking Table (Table 226 CORE Spec)
        // DEBUG ((DEBUG_VERBOSE, "\nDisable ReadLocked in Locking table\n"));
        ColumnValue = 0;
        Status = ObjectUidSet(OpalDevConfig, &SessionParams, Buffer, BufferLength, LockingRangeUid,\
                        7, ColumnName7, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
        ZeroMem (Buffer, BufferLength);
        GetResponse (OpalDevConfig, Buffer, BufferLength, &SessionParams);
    
        //DEBUG ((DEBUG_VERBOSE, "\nDisable WriteLocked in Locking table\n"));
    
        // Set WriteLocked to FALSE in Locking Table
        ColumnValue = 0;
        Status = ObjectUidSet(OpalDevConfig, &SessionParams, Buffer, BufferLength, LockingRangeUid,\
                        8, ColumnName8, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
        ZeroMem (Buffer, BufferLength);
        GetResponse (OpalDevConfig, Buffer, BufferLength, &SessionParams);
    }
CloseSession:
    SMUidCloseSession (OpalDevConfig, &SessionParams, Buffer, BufferLength);

    return Status;

}

/** @internal
    Wrapper function to send BlockSID command in SMM.

    @param  OPAL_DEVICE   - Opal Device information instance

    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
TcgSmmBlockSID (
    IN  OPAL_DEVICE     *OpalDevice
)
{
    VOID    *Buffer = (VOID*)gSecurityUnlockCommandBuffer;
    UINTN   BufferLength = 512;

    return EnableBlockSID(OpalDevice, Buffer, BufferLength);
}


/** @internal
    Function to Get response data of sent security command sent.

    @param  OpalDevice        - Opal Device information instance
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  SessionParams     - information on current session.

    @retval EFI_STATUS
    
    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Sec 5.1.5 has the status codes
EFI_STATUS
GetResponse (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength,
    IN  SESSION_PARAMS      *SessionParams
)
{

    EFI_STATUS                    Status=EFI_NOT_FOUND;
    UINT8                         bData = 0xFF;
    UINT16                        SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    COM_PACKET_FORMAT             *ComPacket = Buffer;
    UINT32                        Retries = NUM_OF_RETRIES;
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL  *SmmNvmeController;
#endif
    do {
        if (OpalDevice->BusInterfaceType == AmiStorageInterfaceNvme) {
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
          if(OpalDevice->RstControlled == TRUE)  {
              
              Status = OpalDevice->StorageSecurityProtocol->ReceiveData( 
                                                  OpalDevice->StorageSecurityProtocol,
                                                  OpalDevice->MediaId,
                                                  10 * 10000000,
                                                  SECURITY_PROTOCOL_1,   // Security Protocol ID
                                                  SPSpecificID,   
                                                  0x200,
                                                  Buffer,
                                                  &BufferLength
                                                  );
              
          } else {
              if (EFI_ERROR( TcgLocateNvmeSmmProtocols( OpalDevice->DeviceHandle, &SmmNvmeController))) {
                  return EFI_DEVICE_ERROR;
              }
              Status = SmmNvmeSendReceiveData( SmmNvmeController,
                                      FALSE,                //Receive
                                      OpalDevice->MediaId,
                                      NVME_COMMAND_TIMEOUT * 10000000,
                                      SECURITY_PROTOCOL_1,   // Security Protocol ID
                                      SPSpecificID,         // SP specific id
                                      BufferLength,          // TranferLength
                                      Buffer);              
          }
#endif
        } else if (OpalDevice->BusInterfaceType == AmiStorageInterfaceAhci) {
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
            Status = SmmAhciSendReceiveData( gAhciSmmProtocol,
                                    FALSE,                //Receive
                                    OpalDevice->MediaId,
                                    OpalDevice->PortNumber,
                                    OpalDevice->PMPortNumber,
                                    SATA_COMMAND_TIMEOUT * 10000000,
                                    SECURITY_PROTOCOL_1,  // Security Protocol ID
                                    SPSpecificID,         // SP specific id
                                    BufferLength,         // TranferLength
                                    Buffer);
#endif
        }
        DEBUG ((DEBUG_VERBOSE, "GetResponse Buffer : %r\n", Status));
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)
        PrintCmdBlock (Buffer);
#endif

        if (EFI_ERROR(Status)) {
            return Status;
        }

        // Check if it is a CloseSession
        Status = CheckCloseSession (Buffer, BufferLength);
        if (Status == EFI_SUCCESS) {
            SessionParams->Tsn = 0;     // Indicate Session is closed
            return EFI_DEVICE_ERROR;
        }

        // Sec 3.2.3.2.1.4 Core spec
        if ((ComPacket->OutStandingData == 0) && ComPacket->Length) {
            break;
        }
        else {
			MicroSecondDelay (100000);  //100 msec
            continue;
        }

    } while (--Retries);

    Status = GetStatusValue (Buffer, BufferLength, &bData);
    DEBUG ((DEBUG_VERBOSE, "GetResponse Status : %r\n", Status));
    TCG_STORAGE_ERR_CHECK(Status);

    return Status;

}

/** @internal
    Function to allocate pool of a particular type.

    @param  PoolType               Type of pool to allocate
    @param  Size                   The amount of pool to allocate
    @param  Buffer                 Pointer to allocated buffer

    @retval EFI_STATUS
  
    @endinternal
**/
EFI_STATUS
TcgSecAllocatePool(
  IN  EFI_MEMORY_TYPE              PoolType,
  IN  UINTN                        Size,
  OUT VOID                         **Buffer
)
{
    return gSmst2->SmmAllocatePool(EfiRuntimeServicesData, Size, Buffer);
}

/** @internal
    Function to free allocated pool

    @param  Buffer            Buffer to free

    @retval EFI_STATUS
  
    @endinternal
**/
EFI_STATUS
TcgSecFreePool(
  IN  VOID  *Buffer
)
{
    return gSmst2->SmmFreePool (Buffer);
}
