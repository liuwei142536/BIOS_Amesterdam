//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************


/** @file CmosManagerPei.c
    Contains the routines that constitute the CMOS manager's
    PEI phase entry.

**/

#include <Token.h>
#include "CmosManagerPei.h"
#include "CmosManagerHob.h"
#include <CmosManagerHooks.h>
#include <Library/PeiServicesLib.h>

extern CONST UINT8    gFirstManagedRegister;
extern CONST UINT8    gLastManagedRegister;
extern CMOS_TOKEN     gCmosTokenTable[];
extern UINT16         gCmosTokenTableSize;
extern CMOS_REGISTER  gCmosOptimalDefaultTable[];
extern UINT16         gCmosOptimalDefaultTableSize;
extern CMOS_REGISTER  gCmosNoCheckSumTable[];
extern UINT16         gCmosNoCheckSumTableSize;
extern CMOS_REGISTER  gUnmanagedTable[];
extern UINT16         gUnmanagedTableSize;

extern EFI_CMOS_MANAGER_INTERFACE *GetCmosMangerInterface(
    IN EFI_CMOS_ACCESS_INTERFACE  *Cmos );

EFI_STATUS CmosManagerAfterMemoryEntry (
    IN EFI_PEI_SERVICES           **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
    IN VOID                       *NullPpi );

EFI_CMOS_IS_FIRST_BOOT  gIsFirstBoot[] = {CMOS_IS_FIRST_BOOT_MAPPING NULL};
EFI_CMOS_IS_BSP         gIsBsp[] = {CMOS_IS_BSP_MAPPING NULL};
EFI_CMOS_IS_COLD_BOOT   gIsColdBoot[] = {CMOS_IS_COLD_BOOT_MAPPING NULL};
EFI_CMOS_IS_CMOS_USABLE gCmosIsUsable[] = {CMOS_IS_USABLE_MAPPING NULL};
EFI_CMOS_BATTERY_TEST   gCmosBatteryIsGood[] = {CMOS_BATTERY_TEST_MAPPING NULL};
CMOS_PORT_MAP           gCmosBank[] = { {0,0,0,0,0}, CMOS_PORT_MAPPING  };
UINT16 gCmosBankCount = sizeof(gCmosBank) / sizeof(CMOS_PORT_MAP);

EFI_GUID gCmosDataHobInstalledGuid = EFI_CMOS_DATA_HOB_INSTALLED_GUID;

static EFI_PEI_NOTIFY_DESCRIPTOR CmosMgrPeiNotify[] =
{
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gEfiPeiMemoryDiscoveredPpiGuid,
        CmosManagerAfterMemoryEntry
    }
};


/**
    This function creates the CMOS data HOB, when memory is available.


    @param Manager Manager interface pointer

    @retval EFI_STATUS (Return value)
        = EFI_SUCCESS or valid EFI error code

    @note
      This function is used only in PEI phase, included by the build
      macro PEI_COMPILE.

**/

EFI_STATUS CreateCmosDataHob(
    IN EFI_CMOS_MANAGER_INTERFACE   *Manager )
{
    EFI_STATUS          Status;
    UINT16              HobSize = sizeof(CMOS_MANAGER_HOB);
    EFI_GUID            CmosManagerHobGuid = CMOS_MANAGER_HOB_GUID;
    CMOS_MANAGER_HOB    *CmosManagerHob;
    EFI_PEI_SERVICES    **PeiServices = Manager->Access.PeiServices;

    Status = (*PeiServices)->CreateHob((CONST EFI_PEI_SERVICES**)PeiServices,
                                        EFI_HOB_TYPE_GUID_EXTENSION,
                                        HobSize,
                                        (VOID**)&CmosManagerHob);

    CmosManagerHob->Header.Name = CmosManagerHobGuid;
    CmosManagerHob->FirstManagedRegister = Manager->FirstManagedRegister;
    CmosManagerHob->LastManagedRegister = Manager->LastManagedRegister;
    CmosManagerHob->TokenCount = Manager->TokenCount;
    CmosManagerHob->OptimalDefaultCount = Manager->OptimalDefaultCount;
    CmosManagerHob->NoChecksumCount = Manager->NoChecksumCount;
    CmosManagerHob->UnmanagedTableCount = Manager->UnmanagedTableCount;
    CmosManagerHob->Checksum = Manager->Checksum;
    CmosManagerHob->ManagerStatus = Manager->ManagerStatus;
    Manager->ManagerHob = CmosManagerHob;

    // Use a raw copy to initialize the tables in the HOB
    // from global arrays
    if ( EFI_ERROR(Status) )
        return Status;
    else {
        MemCpy( CmosManagerHob->TokenTable,
                Manager->TokenTable,
                Manager->TokenCount * sizeof(CMOS_TOKEN)
        );
        MemCpy( CmosManagerHob->OptimalDefaultTable ,
                Manager->OptimalDefaultTable,
                Manager->OptimalDefaultCount * sizeof(CMOS_REGISTER)
        );
        MemCpy( CmosManagerHob->NoChecksumTable ,
                Manager->NoChecksumTable,
                Manager->NoChecksumCount * sizeof(CMOS_REGISTER)
        );
        MemCpy( CmosManagerHob->UnmanagedTable ,
                Manager->UnmanagedTable,
                Manager->UnmanagedTableCount * sizeof(CMOS_REGISTER)
        );
    }

    //-----------------------------------------------------------------------
    // The Manager will now use the HOB versions of the tables for
    // faster access and to ensure proper updating of the optimal defaults
    // buffer.
    //-----------------------------------------------------------------------
    Manager->TokenTable = CmosManagerHob->TokenTable;
    Manager->OptimalDefaultTable = CmosManagerHob->OptimalDefaultTable;
    Manager->NoChecksumTable = CmosManagerHob->NoChecksumTable;
    Manager->UnmanagedTable = CmosManagerHob->UnmanagedTable;

    //-----------------------------------------------------------------------
    // Install a notification PPI to inform that the CMOS data HOB is
    // installed.
    //
    // Note:    The current routine is executed from within a notification,
    //          so any PPI waiting on the install notification PPI must
    //          use the EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK Flag.
    //-----------------------------------------------------------------------
    Manager->Ppi[CMOS_DATA_HOB_INSTALLED_PPI_TYPE].Flags =
        EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    Manager->Ppi[CMOS_DATA_HOB_INSTALLED_PPI_TYPE].Guid =
        &gCmosDataHobInstalledGuid;
    Manager->Ppi[CMOS_DATA_HOB_INSTALLED_PPI_TYPE].Ppi = NULL;
    Status = (*PeiServices)->InstallPpi(
            (CONST EFI_PEI_SERVICES**)PeiServices, &Manager->Ppi[CMOS_DATA_HOB_INSTALLED_PPI_TYPE]);

    return Status;
}


/**
    This function calls CreateCmosDataHob to create the DXE data HOB
    after permanent memory has been installed, and updates the new CMOS
    Access Interface Address, as well as the CMOS-based API pointer.


    @param PeiServices PEI Services table pointer
    @param NotifyDescriptor
    @param NullPpi

    @retval EFI_STATUS (Return value)
        = EFI_SUCCESS or valid EFI error code

**/

EFI_STATUS CmosManagerAfterMemoryEntry (
    IN EFI_PEI_SERVICES           **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
    IN VOID                       *NullPpi )
{
    EFI_STATUS                  Status;
    EFI_CMOS_ACCESS_INTERFACE   *Cmos;
    EFI_CMOS_MANAGER_INTERFACE  *Manager;
    EFI_BOOT_MODE               BootMode;

    PeiServicesGetBootMode(&BootMode);

    if (BootMode == BOOT_ON_S3_RESUME) {
        return EFI_SUCCESS;
    }

    LOCATE_CMOS_ACCESS_PPI(Status, Cmos);       // get Manager's interface
    if (EFI_ERROR(Status))
        return Status;

    Manager = GetCmosMangerInterface(Cmos);

    // Update PeiServices first
    Manager->Access.PeiServices = PeiServices;

    // Update the API pointer in CMOS
    Manager->SaveApiPointerToCmos(Manager, Cmos);

    // Indicate that manager is now executing in memory
    Manager->SetStatus(Manager, CMOS_EXECUTING_IN_MEMORY);

    // Create the HOB
    CreateCmosDataHob(Manager);

    return EFI_SUCCESS;
}


/**
    This function is used in the PEI phase to set/clear the
    CMOS_EXECUTING_IN_MEMORY depending on whether or not the manager
    is executing after permanent memory has been initialized.


    @param Manager Pointer to the Manager's interface

    @retval EFI_STATUS (Return value)
        = EFI_SUCCESS or valid EFI error code

**/

VOID CmosManagerUpdateMemoryStatus (
    IN EFI_CMOS_MANAGER_INTERFACE   *Manager )
{
    EFI_STATUS        Status;
    VOID              *InterfacePtr;
    EFI_PEI_SERVICES  **PeiServices =Manager->Access.PeiServices;

    Status = (*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES**)PeiServices, &gEfiPeiMemoryDiscoveredPpiGuid,
                                        0, NULL, &InterfacePtr);
    if ( EFI_ERROR(Status) )
        Manager->ClearStatus(Manager, CMOS_EXECUTING_IN_MEMORY);
    else
        Manager->SetStatus(Manager, CMOS_EXECUTING_IN_MEMORY);
}


/**
    Locate the EFI_CMOS_ACCESS_INTERFACE for the current manager
    interface.

    @param PeiServices     This is a pointer to the PEI Services structure.
                           (In the DXE phase it will be NULL.)
    @param AccessInterface This is the returned access interface.

    @retval EFI_STATUS (Return Value)
        -- If successful, EFI_SUCCESS is returned.
        -- Otherwise, a valid EFI error code is returned.

**/

EFI_STATUS LocateAccessInterfaceCallout(
    IN   EFI_PEI_SERVICES             **PeiServices,
    OUT  EFI_CMOS_ACCESS_INTERFACE    **AccessInterface )
{
    EFI_STATUS            Status;

    Status = (*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES**)PeiServices, &gAmiCmosAccessPpiGuid, 0,
              NULL, (VOID**)AccessInterface);

    return Status;
}


/**
    This function allocates memory.

    @param PeiServices pointer to the PEI Services data structure
                       (This parameter is NULL in the DXE phase.)
    @param Size        Number of bytes to allocate
    @param Buffer      Pointer to buffer for which memory is allocated

    @retval EFI_STATUS (Return Value)
        = EFI_SUCCESS if successful
        = or other valid EFI error code

**/

EFI_STATUS AllocatePoolCallout(
    IN  EFI_PEI_SERVICES       **PeiServices,
    IN  UINTN                Size,
    OUT VOID                 **Buffer )
{
    return (*PeiServices)->AllocatePool((CONST EFI_PEI_SERVICES**)PeiServices,
            Size, Buffer);
}


/**
    This function performs phase-specific initialization of the
    private EFI_CMOS_MANAGER_INTERFACE structure.

    @param Manager Pointer to the EFI_CMOS_MANAGER_INTERFACE structure.

    @retval EFI_STATUS (Return Value)
        = EFI_SUCCESS, if successful
        = valid EFI error code, otherwise

**/

EFI_STATUS ManagerInterfaceInitializeCallout(
    IN  EFI_CMOS_MANAGER_INTERFACE  *Manager)
{
    Manager->AccessGuid = gAmiCmosAccessPpiGuid;
    Manager->FirstManagedRegister = gFirstManagedRegister;
    Manager->LastManagedRegister = gLastManagedRegister;
    Manager->TokenTable = (CMOS_TOKEN*) gCmosTokenTable;
    Manager->TokenCount = gCmosTokenTableSize;
    Manager->NoChecksumTable = (CMOS_REGISTER*) gCmosNoCheckSumTable;
    Manager->NoChecksumCount = gCmosNoCheckSumTableSize;
    Manager->UnmanagedTable = (CMOS_REGISTER*) gUnmanagedTable;
    Manager->UnmanagedTableCount = gUnmanagedTableSize;
    Manager->OptimalDefaultTable =
        (CMOS_REGISTER*) gCmosOptimalDefaultTable;
    Manager->OptimalDefaultCount = gCmosOptimalDefaultTableSize;
    Manager->Ppi[CMOS_ACCESS_PPI_TYPE].Flags =
        EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    Manager->Ppi[CMOS_ACCESS_PPI_TYPE].Guid = &gAmiCmosAccessPpiGuid;
    Manager->Ppi[CMOS_ACCESS_PPI_TYPE].Ppi = (VOID*)Manager;

    Manager->IsFirstBoot = &gIsFirstBoot[0];
    Manager->IsColdBoot = &gIsColdBoot[0];
    Manager->IsBsp = &gIsBsp[0];
    Manager->CmosIsUsable = &gCmosIsUsable[0];
    Manager->CmosBatteryIsGood = &gCmosBatteryIsGood[0];
    Manager->CmosBank = &gCmosBank[0];
    Manager->CmosBankCount = gCmosBankCount;

    CmosManagerUpdateMemoryStatus( Manager );

    return EFI_SUCCESS;
}


/**
    Locate the EFI_CMOS_ACCESS_INTERFACE for the current manager
    interface.

    @param PeiServices This is a pointer to the PEI Services structure.
                       (In the DXE phase it will be NULL.)
    @param CmosManager This is the private interface.

    @retval EFI_STATUS (Return Value)
        -- If successful, EFI_SUCCESS is returned.
        -- Otherwise, a valid EFI error code is returned.

**/

EFI_STATUS InstallAccessInterfaceCallout(
    IN   EFI_PEI_SERVICES             **PeiServices,
    OUT  EFI_CMOS_MANAGER_INTERFACE   *CmosManager )
{
    return (*PeiServices)->InstallPpi(
            (CONST EFI_PEI_SERVICES**)PeiServices, &CmosManager->Ppi[CMOS_ACCESS_PPI_TYPE]);
}


/**
    This function performs additional tasks when the internal status
    value is updated. For the PEI phase, this function currently does
    nothing.

    @param Manager Pointer to the EFI_CMOS_MANAGER_INTERFACE structure.

    @retval EFI_STATUS (Return Value)
        = EFI_SUCCESS, if successful
        = valid EFI error code, otherwise

**/

VOID StatusUpdateCallout(
    IN  EFI_CMOS_MANAGER_INTERFACE  *Manager)
{
    return;
}


/**
    This function is the main PEI phase entry point for the CMOS
    manager module.

    @param FfsHeader   FFS file header pointer
    @param PeiServices PEI Services table pointer

    @retval EFI_STATUS (Return Value)
        = EFI_SUCCESS if successful
        = or other valid EFI error code

    @note
      Execution Sequence:
                  1)  Initialize CMOS Manager interface
                  2)  Load default values into CMOS registers if
                      this is the first boot.
                  3)  Configure usage of the Optimal Defaults table if
                      the battery or checksum is bad.
                  4)  Update the CMOS-based API/PPI pointer.
                  5)  Install the EFI_CMOS_ACCESS_INTERFACE PPI

**/

EFI_STATUS EFIAPI CmosManagerPeiEntry (
    IN       EFI_PEI_FILE_HANDLE  FileHandle,
    IN CONST EFI_PEI_SERVICES          **PeiServices )
{
    EFI_CMOS_MANAGER_INTERFACE  *Mgr;

    //-----------------------------------------------------------------------
    // Initialize the manager interface and, at this point, only check for
    // interface initialization errors.
    //-----------------------------------------------------------------------
    Mgr = NewCmosManagerInterface((EFI_PEI_SERVICES**)PeiServices);
    if ( Mgr == NULL || !Mgr->CheckStatus(Mgr, CMOS_VALID_INTERFACE) )
        return EFI_UNSUPPORTED;
    if ( Mgr->CheckStatus( Mgr, CMOS_INTERFACE_ALREADY_INSTALLED) )
        return EFI_SUCCESS;

    //-----------------------------------------------------------------------
    // Handle specific internally-reported conditions.
    //-----------------------------------------------------------------------
    if (    Mgr->CheckStatus(Mgr, CMOS_IS_USABLE )
         && Mgr->CheckStatus(Mgr, CMOS_BSP_IS_EXECUTING ) )
    {
        // Load optimal defaults on first boot after programming
        // boot device.
        if  (    Mgr->CheckStatus(Mgr, CMOS_FIRST_BOOT_DETECTED )
              && Mgr->CheckStatus(Mgr, CMOS_COLD_BOOT_DETECTED )  )
        {
            Mgr->LoadOptimalDefaults(Mgr);
        }

        // Save CMOS-based PPI pointer
        Mgr->SaveApiPointerToCmos(Mgr, NULL);
    }

    //-----------------------------------------------------------------------
    // If the CMOS hardware is not usable, configure CMOS Manager to use the
    // read/write Optimal Defaults buffer for Managed Region access.
    //-----------------------------------------------------------------------
    if ( Mgr->CheckAnyStatus( Mgr, CMOS_BAD_CHECKSUM | CMOS_BAD_BATTERY ) ){
        if ( Mgr->CheckStatus( Mgr, CMOS_BAD_CHECKSUM) )
            DEBUG((DEBUG_INFO, "Bad Checksum:\n"));

        if ( Mgr->CheckStatus( Mgr, CMOS_BAD_BATTERY) )
            DEBUG((DEBUG_INFO, "Bad Battery:\n"));

        if (    Mgr->CheckStatus(Mgr, CMOS_IS_USABLE )
             && Mgr->CheckStatus(Mgr, CMOS_RECOVER_IN_PEI ) )
        {
            DEBUG((DEBUG_INFO, "  Loading Optimal Defaults\n"));
            Mgr->LoadOptimalDefaults( Mgr );
            Mgr->ConfigureManager( Mgr, CMOS_OPTIMAL_DEFAULTS_OFF );
            DEBUG((DEBUG_INFO, "...Defaults loaded\n"));
        }
        else {
            DEBUG((DEBUG_INFO, "  Using Optimal Defaults.\n"));
            Mgr->ConfigureManager( Mgr, CMOS_OPTIMAL_DEFAULTS_ON );
        }
    }

    //-----------------------------------------------------------------------
    // Install the CMOS Access PPI
    //-----------------------------------------------------------------------
    Mgr->InstallAccessInterface(Mgr);
    if ( !Mgr->CheckStatus(Mgr, CMOS_VALID_MANAGER ) ){
        DEBUG((DEBUG_ERROR, "Invalid interface ... Access PPI not installed\n"));
        return EFI_UNSUPPORTED;
    }

    (*PeiServices)->NotifyPpi(PeiServices, CmosMgrPeiNotify);

    return EFI_SUCCESS;
}


//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
