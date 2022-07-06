//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiPciAccessCspBaseLib.c
    Library Class for AMI CSP PCI Interface

	@note Requires Chipset Specific Porting for each project!
**/
//*************************************************************************

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Token.h>
#include <AmiLib.h>
#include <AcpiRes.h>
#include <PciBus.h>
#include <PciHostBridge.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/AcpiTable.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>  // AptioV Server override

#include <Protocol/IioUds.h>  // AptioV Server override: Grantley Dynamic Resource Allocation.
#include <Guid/SetupVariable.h> // AptioV Server override: Grantley Dynamic Resource Allocation.

#include <AcpiOemElinks.h> //AptioV server override: EIP#494758 - CONVERT_TO_STRING macro replaced to avoid strips leading and trailing spaces

//AptioV server override start: dynamic MMCFG base change support.
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include <Library/PciExpressLib.h> //AptioV server override: dynamic MMCFG base change support.
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include <Library/DebugLib.h>
#include <Library/AmiPciBusLib.h>  // AptioV Server override

//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
#define PCIE_MAX_BUS ((PCIEX_LENGTH/0x100000)-1)

// GUID Definition(s)
extern EFI_GUID gAmiGlobalVariableGuid;

// Variable Declaration(s)
EFI_EVENT       			mAcpiEvent=NULL;
VOID            			*mAcpiReg=NULL;
CHAR16          			gMemoryCeilingVariable[] = L"MemCeil.";
//Keep MCFG table key and ACPI Table Protocol as global
//in case somebody will like to update MCFG table.
UINTN           			gMcfgTableKey=0;
EFI_ACPI_TABLE_PROTOCOL     *gAcpiTableProtocol=NULL;
EFI_IIO_UDS_PROTOCOL  	  *mIohUds; //Platform/Chipset Sapecifik interface provoding information about resource Split between Multiple ROOTs.

#if CRB_OUT_OF_RESOURCE_SUPPORT
// Aptio V Server override start: Grantley Dynamic Resource Allocation.
#define SAD_LEG_IO_GRANTY   0x2000          // 8KB
#define SAD_MMIOL_GRANTY    0x4000000      // 64MB

#define IOFAILURE 		0x01
#define MMIOFAILURE 	0x02
#define MMIO32_TYPES	0x02  // MMIO32 MMIO32pf
#define IO_TYPES	    0x02  // Can be 16/32
#define MAX_SAD_TARGETS  8

#define LAST_BOOT_FAILED_GUID \
{0x8163179a, 0xf31a, 0x4132, 0xae, 0xde, 0x89, 0xf3, 0x6a, 0xee, 0x43, 0xda}

#define AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA_GUID \
{ 0x3992e100, 0x8860, 0x11e0, 0x9d,0x78, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66}

#define RETRY_COUNTER_MMIO_IO_GUID \
{0xa1948ff0, 0x8c5d, 0x11e0, 0x83,0xf2, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66}


EFI_GUID LastBootFailedIohGuid          = LAST_BOOT_FAILED_GUID;
EFI_GUID gEfiAutoMmioIOVariableGuid     = AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA_GUID;
EFI_GUID RetryCounterMmioIoGuid         = RETRY_COUNTER_MMIO_IO_GUID;


BOOLEAN			  ResetRequired = FALSE;
static UINT8  		  MaxPcidevicesDisableCount = MAX_PCIDEVICES_DISABLE_COUNT;
SYSTEM_CONFIGURATION      SystemConfiguration;
typedef struct _IOHX_OUT_OF_RESOURCE_INFO {
//MMIO/IO OutOFResourceFlag
	UINT8       MmmioOutofResrcFlag [NUMBER_CPU_SOCKETS];
	UINT8       IoOutofResrcFlag [NUMBER_CPU_SOCKETS];

// MMio Stuff
	UINT64      MmioBase [NUMBER_CPU_SOCKETS];
	UINT64	    MmioLimit[NUMBER_CPU_SOCKETS];
	UINT64	    MmioRequested[NUMBER_CPU_SOCKETS] [MMIO32_TYPES];

//IO Stuff
	UINT64      IoBase[NUMBER_CPU_SOCKETS];
	UINT64	    IoLimit[NUMBER_CPU_SOCKETS];
	UINT64	    IoRequested[NUMBER_CPU_SOCKETS][IO_TYPES];

// RootBridgeImage Handle,MMIO and IO _GRA
	EFI_HANDLE  IohImageHandle [NUMBER_CPU_SOCKETS];
	UINT64      IoMax [NUMBER_CPU_SOCKETS] [IO_TYPES];
	UINT64      MmioMax [NUMBER_CPU_SOCKETS] [MMIO32_TYPES];
} IOHX_OUT_OF_RESOURCE_INFO;

//
//	LAST_BOOT_FAILED_IOH Structure
//
typedef struct _LAST_BOOT_FAILED_IOH {
    UINT8       MmmioOutofResrcFlag [NUMBER_CPU_SOCKETS];
    UINT8       IoOutofResrcFlag [NUMBER_CPU_SOCKETS];
} LAST_BOOT_FAILED_IOH;

//
//	CURRENT_BOOT_MMIO_IO_RATIO Structure
//
typedef struct _CURRENT_BOOT_MMIO_IO_RATIO {
    UINT8       MMIO [NUMBER_CPU_SOCKETS];
    UINT8       IO [NUMBER_CPU_SOCKETS];
} CURRENT_BOOT_MMIO_IO_RATIO;

//
// REQUIRED_MMIO_IO_RATIO Ratio Structure
//
typedef struct _REQUIRED_MMIO_IO_RATIO {
    UINT8       MMIO [NUMBER_CPU_SOCKETS];
    UINT8       IO [NUMBER_CPU_SOCKETS];
} REQUIRED_MMIO_IO_RATIO;

//
// RETRY_COUNTER_MMIO_IO Ratio Structure
//
typedef struct _RETRY_COUNTER_MMIO_IO {
    UINT8       MMIO;
    UINT8       IO;
} RETRY_COUNTER_MMIO_IO;

UINT8       TempMmmioOutofResrcFlag [NUMBER_CPU_SOCKETS];
UINT8       TempIoOutofResrcFlag [NUMBER_CPU_SOCKETS];

IOHX_OUT_OF_RESOURCE_INFO 		IohOutofResInfo;
CURRENT_BOOT_MMIO_IO_RATIO 		CurrentMmioIoRatios;
REQUIRED_MMIO_IO_RATIO			RequiredMMIOIoRatio;
RETRY_COUNTER_MMIO_IO                   RetryCounter;

UINT8		MMioRatioToMMiolChunks [NUMBER_CPU_SOCKETS];
UINT8           IoRatioToIoChunks      [NUMBER_CPU_SOCKETS];



UINT8 NbGetMaximumIohSockets(
    VOID
)
{
	return mIohUds->IioUdsPtr->SystemStatus.numCpus;

}
// Aptio V Server override end
#endif
/**
    This function will create the MCFG ACPI table when ACPI
    support protocol is available.

    @param Event Event of callback
    @param Context Context of callback.

    @retval VOID

    @note  Porting required.
**/

#if defined(_MSC_EXTENSIONS)
// Disable warning when array is too small to include a terminating null character
#pragma warning ( disable : 4295 )
#endif

VOID EFIAPI CreateNbAcpiTables (
    IN EFI_EVENT        Event,
    IN VOID             *Context )
{
    EFI_STATUS                  Status = 0;
    MCFG_20                     *Mcfg;
    // AptioV server override start: EIP#494758 - CONVERT_TO_STRING macro replaced to avoid strips leading and trailing spaces
    UINT8                       OemId[6] = ACPI_OEM_ID_MAK;
    UINT8                       OemTblId[8] = ACPI_OEM_TBL_ID_MAK;
    // AptioV server override End: EIP#494758 - CONVERT_TO_STRING macro replaced to avoid strips leading and trailing spaces    
//---------------------------------------------
    // it must be only one instance of such protocol
    if (gAcpiTableProtocol==NULL){
        Status = pBS->LocateProtocol( &gEfiAcpiTableProtocolGuid, NULL, (VOID **)&gAcpiTableProtocol );
        DEBUG((DEBUG_INFO,"PciHbCsp: LocateProtocol(ACPITableProtocol)= %r\n", Status));
        if(EFI_ERROR(Status))return;
    }

    Mcfg = MallocZ(sizeof(MCFG_20));
    ASSERT(Mcfg);
    if(!Mcfg) return;

    // Fill Table header;
    Mcfg->Header.Signature = MCFG_SIG;
    Mcfg->Header.Length = sizeof(MCFG_20);
    Mcfg->Header.Revision = 1;
    Mcfg->Header.Checksum = 0;
    MemCpy(&(Mcfg->Header.OemId[0]), OemId, 6);
    MemCpy(&(Mcfg->Header.OemTblId[0]), OemTblId, 8);
    Mcfg->Header.OemRev = ACPI_OEM_REV;
    Mcfg->Header.CreatorId = 0x5446534d;//"MSFT" 4D 53 46 54
    Mcfg->Header.CreatorRev = 0x97;

    // Fill MCFG Fields

    // Base address of 256/128/64MB extended config space
    //AptioV Server Override start: Dynamic mmcfg base address change
#ifndef DYNAMIC_MMCFG_BASE_FLAG
    Mcfg->BaseAddr = (UINTN)PcdGet64 (PcdPciExpressBaseAddress);
#else
    Mcfg->BaseAddr = (UINTN) GetPciExpressBaseAddress ();
#endif
    //AptioV Server Override end: Dynamic mmcfg base address change	
	
    // Segment # of PCI Bus
    Mcfg->PciSeg = 0;
    // Start bus number of PCI segment
    Mcfg->StartBus = 0;
    // End bus number of PCI segment
    Mcfg->EndBus = PCIE_MAX_BUS;

    // Add table
    Status = gAcpiTableProtocol->InstallAcpiTable( gAcpiTableProtocol, Mcfg, sizeof(MCFG_20), &gMcfgTableKey );
    DEBUG((DEBUG_INFO,"PciHbCsp: Installing AcpiTable (MCFG) = %r \n", Status));
    ASSERT_EFI_ERROR(Status);

    // Free memory used for table image
    pBS->FreePool(Mcfg);

    // Kill the Event
    pBS->CloseEvent(Event);

    return;
}

//----------------------------------------------------------------------------
// Following functions are HOST BRIDGE Infrastructure Overrides and Porting.
//----------------------------------------------------------------------------

/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
    EfiPciHostBridgeBeginEnumeration.

    @param ResAllocProtocol Pointer to Host Bridge Resource
        Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbNotifyCspBeforeEnumeration (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                  **RbIoProtocolBuffer,
    IN UINTN                                            RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}


/**
    This function will be invoked in PCI Host Bridge Driver
    before converting all Non Existant MMIO into PCI MMIO.
    In order to allow CSP code do aome none standard conversion.

    @param ImageHandle - this image Handle
    @param ControllerHandle - Controller(RB) Handle (Optional).

    @retval EFI_STATUS
            EFI_UNSUPPORTED - means use default MMIO convertion.
            EFI_SUCCESS - CSP code has been converted MMIO itself.
            ANYTHING else - ERROR.
            
    @note  Porting required if needed.
**/
EFI_STATUS HbCspConvertMemoryMapIo (
            IN EFI_HANDLE      ImageHandle,
            IN EFI_HANDLE      ControllerHandle)
{
    EFI_STATUS  Status = EFI_UNSUPPORTED;

    // Any Additional Variables goes here
 //---------------------------------------   
    

    return Status;
}

EFI_STATUS HbCspConvertMemoryMapMmio (
            IN EFI_HANDLE      ImageHandle,
            IN EFI_HANDLE      ControllerHandle)
{
    EFI_STATUS  Status = EFI_UNSUPPORTED;

    // Any Additional Variables goes here
 //---------------------------------------   
    

    return Status;
}


/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
    EfiPciHostBridgeBeginBusAllocation.

    @param ResAllocProtocol Pointer to Host Bridge Resource
        Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbNotifyCspBeginBusAllocation (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                   **RbIoProtocolBuffer,
    IN UINTN                                             RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}


/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
    EfiPciHostBridgeEndBusAllocation

    @param ResAllocProtocol Pointer to Host Bridge Resource
        Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbNotifyCspEndBusAllocation (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                  **RbIoProtocolBuffer,
    IN UINTN                                            RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
    EfiPciHostBridgeBeginResourceAllocation.

    @param ResAllocProtocol Pointer to Host Bridge Resource
        Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/

EFI_STATUS HbNotifyCspBeginResourceAllocation (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                  **RbIoProtocolBuffer,
    IN UINTN                                            RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}


/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
    EfiPciHostBridgeAllocateResources.

    @param ResAllocProtocol Pointer to Host Bridge Resource
        Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/

EFI_STATUS HbNotifyCspAllocateResources (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                  **RbIoProtocolBuffer,
    IN UINTN                                            RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
    EfiPciHostBridgeSetResources.

    @param ResAllocProtocol Pointer to Host Bridge Resource
        Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbNotifyCspSetResources (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                  **RbIoProtocolBuffer,
    IN UINTN                                            RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}


/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
    EfiPciHostBridgeEndResourceAllocation

    @param ResAllocProtocol Pointer to Host Bridge Resource
        Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbNotifyCspEndResourceAllocation (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                  **RbIoProtocolBuffer,
    IN UINTN                                            RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

/**
    This procedure will be invoked in PCI Host Bridge Protocol
    when NotifyPhase function is called with phase
	EfiPciHostBridgeEndEnumeration.

    @param ResAllocProtocol Pointer to Host Bridge Resource Allocation Protocol.
    @param RbIoProtocolBuffer Pointer to Root Bridge I/O Protocol.
    @param RbCount Root Bridge counter.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbNotifyCspEndEnumeration (
    IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProtocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL                  **RbIoProtocolBuffer,
    IN UINTN                                            RbCount )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

/**
    This procedure will be invoked in PCI Host Bridge Protocol
    StartBusEnumeration function, it must prepare initial Bus
    ACPI Resource

    @param HostBrgData Pointer to Host Bridge private structure data.
    @param RootBrgData Pointer to Root Bridge private structure data.
    @param RootBrgIndex Root Bridge index (0 Based).

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbCspStartBusEnumeration (
							IN PCI_HOST_BRG_DATA    *HostBrgData,
							IN PCI_ROOT_BRG_DATA    *RootBrgData,
							IN UINTN                RootBrgIndex )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    // Any Additional Variables goes here


    return Status;
}

/**
    This procedure will be invoked in PCI Host Bridge Protocol
    SubmitBusNumbers function.

    @param HostBrgData Pointer to Host Bridge private structure data.
    @param RootBrgData Pointer to Root Bridge private structure data.
    @param RootBrgIndex Root Bridge index (0 Based).

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbCspSetBusNnumbers (
							IN PCI_HOST_BRG_DATA    *HostBrgData,
							IN PCI_ROOT_BRG_DATA    *RootBrgData,
							IN UINTN                RootBrgIndex )
{
#if PCI_BUS_VER_COMBINED > 0x408a // if PciBus version is greater than PciBus_15
    // Starting from PciBus_16, PciHostBridge driver expects EFI_UNSUPPORTED when platform does not support custom bus handling
    EFI_STATUS  Status = EFI_UNSUPPORTED;
#else
    // Prir to PciBus_16, PciHostBridge driver expects EFI_UNSUPPORTED when platform does not support custom bus handling
    EFI_STATUS  Status = EFI_SUCCESS;
#endif
    // Any Additional Variables goes here
    return Status;
}

/**
    This procedure will be invoked in PCI Host Bridge Protocol
    SubmitResources function.

    @param HostBrgData Pointer to Host Bridge private structure data.
    @param RootBrgData Pointer to Root Bridge private structure data.
    @param RootBrgIndex Root Bridge index (0 Based).

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbCspSubmitResources (
								IN PCI_HOST_BRG_DATA    *HostBrgData,
								IN PCI_ROOT_BRG_DATA    *RootBrgData,
								IN UINTN                RootBrgIndex )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

/** 
 * @internal
 * Attempts to set a variable. If attempt fails because the variable already exists with different attributes,
 * tries to delete the variable and to create it with the new attributes specified by Attributes. 
 *
 * @retval EFI_SUCCESS The variable has been successfully created.
 */
static EFI_STATUS AmiPciAccessCspBaseLibSetVariableWithNewAttributes(
    IN CHAR16 *Name, IN EFI_GUID *Guid, IN UINT32 Attributes,
    IN UINTN DataSize, IN VOID *Data    
)
{
    EFI_STATUS Status;
    
    Status = pRS->SetVariable(Name, Guid, Attributes, DataSize, Data);
    if (!EFI_ERROR(Status) || Status != EFI_INVALID_PARAMETER) return Status;

    Status = pRS->SetVariable(Name, Guid, 0, 0, NULL); // Delete the variable
    if (EFI_ERROR(Status)) return Status;

    return pRS->SetVariable(Name, Guid, Attributes, DataSize, Data);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   HbCspAdjustMemoryMmioOverlap
//
// Description: This procedure will be invoked during PCI bus enumeration,
//              it determines the PCI memory base address below 4GB whether
//              it is overlapping the main memory, if it is overlapped, then
//              updates MemoryCeiling variable and reboot.
//
// Input:       HostBrgData  - Pointer to Host Bridge private structure data.
//              RootBrgData  - Pointer to Root Bridge private structure data.
//              RootBrgIndex - Root Bridge index (0 Based).
//
// Output:      EFI_STATUS
//
// Notes:       Porting required if needed.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS HbCspAdjustMemoryMmioOverlap (
									IN PCI_HOST_BRG_DATA    *HostBrgData,
									IN PCI_ROOT_BRG_DATA    *RootBrgData,
									IN UINTN                RootBrgIndex )
{
	/* Aptio V Server Override - Start
	// commented : Grantley we are doing the Dynamic Resource Allocation differently

    EFI_STATUS  Status;

    EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemorySpaceMap;
    UINTN                           NumberOfDescriptors;
    ASLR_QWORD_ASD                  *Res;
    UINTN                           i;
    EFI_PHYSICAL_ADDRESS            Highest4GMem    = 0;
    EFI_PHYSICAL_ADDRESS            LowestMMIO      = 0xffffffff;
    EFI_PHYSICAL_ADDRESS            LowestAllocMMIO = 0xffffffff;
    UINTN                           MemoryCeiling = 0; // Init to 0
    UINTN                           NewMemoryCeiling = 0xffffffff;
    UINTN                           DataSize = sizeof(UINT32);
    DXE_SERVICES                    *DxeSvc;

//------------------------------------
    Status = LibGetDxeSvcTbl( &DxeSvc );
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) return Status;

    pRS->GetVariable ( gMemoryCeilingVariable,
                       &gAmiGlobalVariableGuid,
                       NULL,
                       &DataSize,
                       &MemoryCeiling );
    DEBUG((DEBUG_INFO, "\nMemoryCeiling : %8X \n", MemoryCeiling));

    // Memory sizing uses memory ceiling to set top of memory.


    Status = DxeSvc->GetMemorySpaceMap( &NumberOfDescriptors,
                                        &MemorySpaceMap );
    ASSERT_EFI_ERROR(Status);

    // Find the lowest MMIO and lowest allocated MMIO in GCD.
    for (i = 0; i < NumberOfDescriptors; ++i)
    {
        EFI_GCD_MEMORY_SPACE_DESCRIPTOR *Descr = &MemorySpaceMap[i];
        EFI_PHYSICAL_ADDRESS Base = Descr->BaseAddress;

        // Find highest system below 4GB memory.
        // Treat any non MMIO as system memory. Not all system memory is
        // reported as system memory, such as SMM.

        if ((Descr->GcdMemoryType != EfiGcdMemoryTypeMemoryMappedIo) && (Base < LowestMMIO))
        {
            EFI_PHYSICAL_ADDRESS EndMem = Base + Descr->Length - 1;
            if ((EndMem > Highest4GMem) && (EndMem <= 0xffffffff))
                Highest4GMem = EndMem;

        // Find Lowest mmio above system memory.
        }
        else if (Descr->GcdMemoryType == EfiGcdMemoryTypeMemoryMappedIo)
        {
            if (Base >= 0x100000)
            {
                if (Base < LowestMMIO)
                    LowestMMIO = Base;

                // If ImageHandle, then MMIO is allocated.
                if ((Base < LowestAllocMMIO) && Descr->ImageHandle)
                    LowestAllocMMIO = Base;
            }
        }
    }

    pBS->FreePool(MemorySpaceMap);

    DEBUG((DEBUG_INFO, "\nLowestMMIO : %8X \n", LowestMMIO));
    DEBUG((DEBUG_INFO, "\nLowestAllocMMIO : %8X \n", LowestAllocMMIO));

    if (Highest4GMem + 1 != LowestMMIO) {
        DEBUG((DEBUG_INFO, "PciHostCSHooks: ") );
        DEBUG((DEBUG_INFO, "System Memory and MMIO are not consequitive.\n") );
        DEBUG((DEBUG_INFO, "Top of Below 4G Memory: %lX", Highest4GMem) );
        DEBUG((DEBUG_INFO, "Bottom of MMIO: %X\n", LowestMMIO) );
    }


    // Find any MMIO that could not be allocated due to small of MMIO region.
    for (i = 0; i < RootBrgData->ResCount; ++i)
    {
        EFI_PHYSICAL_ADDRESS NeededBottomMmio;

        Res = RootBrgData->RbRes[i];

        // Any unallocated MMIO will have Res->_MIN set to zero for the MMIO
        // type.
        if (Res->Type != ASLRV_SPC_TYPE_MEM || Res->_GRA != 32 || Res->_MIN)
            continue;

        // Determine new memory ceiling variable needed to allocate this
        // memory.
        NeededBottomMmio = LowestAllocMMIO - Res->_LEN;

        // Round down. If resource is not allocated, _MAX contains
        // granularity.
        NeededBottomMmio &= ~Res->_MAX;
        if (NeededBottomMmio < NewMemoryCeiling)
            NewMemoryCeiling = (UINTN) NeededBottomMmio;
    }

    // Check if a NewMemory Ceiling is needed.
    if (NewMemoryCeiling < 0xffffffff)
    {
        if (!MemoryCeiling || (MemoryCeiling != NewMemoryCeiling))
        {

            // Set memory ceiling variable.
            AmiPciAccessCspBaseLibSetVariableWithNewAttributes(
                gMemoryCeilingVariable,
                &gAmiGlobalVariableGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                sizeof (UINT32),
                &NewMemoryCeiling
            );

            DEBUG((DEBUG_LOAD, "\nResetting System for NewMemoryCeiling : %8X\n", NewMemoryCeiling));

        // Reset only needed of type of physical memory overlaps with MMIO.

#if (NV_SIMULATION != 1)
            // Don't reset system in case of NVRAM simulation
            pRS->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
#endif
        // Control should not come here if NV_SIMULATION = 0.
            return EFI_SUCCESS;
        }
    }

    // Check to see if Ceiling needs to be increased. If too low,
    // then part of the memory be not be usuable.
    if (MemoryCeiling != LowestAllocMMIO)
    {
        // Set memory ceiling variable.
        AmiPciAccessCspBaseLibSetVariableWithNewAttributes(
            gMemoryCeilingVariable, 
            &gAmiGlobalVariableGuid, 
            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
            sizeof (UINT32), 
            &LowestAllocMMIO
        );

        DEBUG((DEBUG_LOAD, "\nResetting System for LowestAllocMMIO : %8X\n", LowestAllocMMIO));

#if (NV_SIMULATION != 1)
        // Don't reset system in case of NVRAM simulation
        pRS->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
#endif
        // Control should not come here if NV_SIMULATION = 0.
    }
// Aptio V Server Override - End
*/
    return EFI_SUCCESS;
}

/**
    This function will be invoked after Initialization of generic
    part of the Host and Root Bridges.
    All Handles for PCIHostBrg and PciRootBrg has been created
    and Protocol Intergaces installed.

    @param HostBrgData Pointer to Host Bridge private structure data.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbCspBasicChipsetInit (
    IN PCI_HOST_BRG_DATA    *HostBrg0, UINTN	HbCount)
{
    EFI_STATUS              Status;

    // Now for Chipset which has PCI Express support we have to build
    // MCFG Table to inform OS about PCIE Ext cfg space mapping
    Status = RegisterProtocolCallback( &gEfiAcpiTableProtocolGuid,\
                                       CreateNbAcpiTables, \
                                       NULL, \
                                       &mAcpiEvent,\
                                       &mAcpiReg );
   	ASSERT_EFI_ERROR(Status);				

    // If this protocol has been installed we can use it rigth on the way
    pBS->SignalEvent( mAcpiEvent );
//-------------------------------------------------------
//Here add some more code for basic HB init IF NEEDED!
//-------------------------------------------------------




//-------------------------------------------------------
    return EFI_SUCCESS;
}

/**
    This function will be invoked when Pci Host Bridge driver runs  
    out of resources.

    @param HostBrgData Pointer to Host Bridge private structure data.

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbCspGetProposedResources (
    IN PCI_HOST_BRG_DATA                            *HostBrgData,
    IN PCI_ROOT_BRG_DATA                            *RootBrgData,
    IN UINTN                                        RootBrgNumber )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

/**
    This function is called for all the PCI controllers that
    the PCI bus driver finds.
    It can be used to Preprogram the controller.

    @param HostBrgData Pointer to Host Bridge private structure data.
    @param RootBrgData Pointer to Root Bridge private structure data.
    @param RootBrgNumber Root Bridge number (0 Based).
    @param PciAddress Address of the controller on the PCI bus.
    @param Phase The phase during enumeration

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbCspPreprocessController (
    IN PCI_HOST_BRG_DATA                            *HostBrgData,
    IN PCI_ROOT_BRG_DATA                            *RootBrgData,
    IN UINTN                                        RootBrgNumber,
    IN EFI_PCI_CONFIGURATION_ADDRESS                PciAddress,
    IN EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE Phase )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

#if CRB_OUT_OF_RESOURCE_SUPPORT
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: 	AutoAdjustIoRatio
//
// Description:	This function performs AutoAdjusted Io Ratio's
//		Will calculate the Best IO Ratio
//
// Input:	NumIoH  			- Number of IOH
//		AutoAdjustMmioIoVariable	- Structure has Current MMIO and IO Ratio Information
//		dxe 				- DXE_SERVICES
//
// Output:	AutoAdjustMmioIoVariable - Newly Calculated Ratio's
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

BOOLEAN
AutoAdjustIoRatio (
    UINT8 				NumIoH,
    AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA *AutoAdjustMmioIoVariable,
    DXE_SERVICES 			*dxe
)
{

	UINT8			Index,Index1,Idx;
	UINT64			ExtraSpaceNeededCount = 0;
	UINT8			ExtraChunks;
	UINT8			TempExtraChunks = 0;
	UINT8       	        FailedIoIndex;
	UINT8       	        ValidIOH [NUMBER_CPU_SOCKETS];
	BOOLEAN     	        NotPossibleFlag = FALSE;
	UINTN       	        VarSize;
	EFI_STATUS  	        GetSetStatus;

	//
	// Collecting the Current Boot IO Ratio's
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		CurrentMmioIoRatios.IO [Index]   = IoRatioToIoChunks [Index] ;
		TRACE((TRACE_ALWAYS, "\n IO:IOH=%x CurrentBootIO chunk (64KB GRA) G=%x\n",Index,CurrentMmioIoRatios.IO [Index]));

	}

	//
	// Calculating the Required Ratio for Each IOh's
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		for ( FailedIoIndex = 0 ; FailedIoIndex < IO_TYPES; FailedIoIndex++)
		{
			TRACE((TRACE_ALWAYS, "\n IOH=%x IO Requested=%x\n",Index,IohOutofResInfo.IoRequested [Index] [FailedIoIndex]));
			RequiredMMIOIoRatio.IO[Index] = RequiredMMIOIoRatio.IO[Index] + (UINT8) ((IohOutofResInfo.IoRequested[Index][FailedIoIndex])/SAD_LEG_IO_GRANTY);
			ExtraSpaceNeededCount = (IohOutofResInfo.IoRequested[Index][FailedIoIndex])%SAD_LEG_IO_GRANTY;
	//		TRACE((TRACE_ALWAYS, "\n ExtraSpaceNeededCount=%x\n",ExtraSpaceNeededCount));
			if (ExtraSpaceNeededCount > 0)
			{
				RequiredMMIOIoRatio.IO[Index] = RequiredMMIOIoRatio.IO[Index] + 1;
			}
//			TRACE((TRACE_ALWAYS, "\n IO: IOH=%x Required Ratio=%x\n",Index,RequiredMMIOIoRatio.IO [Index]));
		}
		TRACE((TRACE_ALWAYS, "\n IO: IOH=%x Required Ratio=%x\n",Index,RequiredMMIOIoRatio.IO [Index]));
	}

	//
	// Check which IOH has the problem and whether other IOH has some free space.
	// Creating Dummy ValidIOH Strcuture;
	// If FreeSpace is available in IOH,Fill IOH Index in the ValidIOH Table
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		ValidIOH[Index] = 0xFF;
	}

	for (Index = 0 , Index1 = 0; Index < 	NumIoH ; Index++)
	{
		if (IohOutofResInfo.IoOutofResrcFlag [Index] != 1)
		{
			ValidIOH[Index1] = Index;
			Index1++;
		}
	}

	//
	// Sanity Check : ValidIOH present
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		if (ValidIOH[Index] != 0xFF)
			break;
	}

	//
	// No IOH is valid : THis is a not possible case
	// Need to try OEM Defined Table
	if (Index == NumIoH)
	{
		NotPossibleFlag = TRUE;
		TRACE((TRACE_ALWAYS, "\n IO : NO VALID IOH's are present\n"));
		TRACE((TRACE_ALWAYS, "\n System will try the OEM DEFINED TABLE\n"));
		return NotPossibleFlag;
	}

	//
	// Printing VALID IOH
	//
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		if (ValidIOH[Index] != 0xFF)
			TRACE((TRACE_ALWAYS, "\n VALID IOH=%x\n",ValidIOH[Index]));
	}

	//
	//	Going to take the resource from the validIOH
	//
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		if (IohOutofResInfo.IoOutofResrcFlag [Index] != 1)
			continue; // This IOH is not failed

		//
		// Going to use the VALIDIOH TABLE to help the affected IOH
		//
		Index1 = 0 ;
		TempExtraChunks =  0;
		while( ValidIOH [Index1] != 0xFF)
		{
			Idx = ValidIOH [Index1];
			ExtraChunks = (CurrentMmioIoRatios.IO [Idx] - RequiredMMIOIoRatio.IO[Idx]);
			if (RequiredMMIOIoRatio.IO [Index] < CurrentMmioIoRatios.IO [Index])
			{
				TRACE((TRACE_ALWAYS, "\n IO: Currently Alloted Space is more than Required Space\n"));
				TRACE((TRACE_ALWAYS, "\n But Still couldn't get continous space\n"));
				TRACE((TRACE_ALWAYS, "\n GOING to try MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR to fix the PCI RESOURCE GRANULARTIY issue \n"));
				NotPossibleFlag = TRUE;
				ResetRequired = TRUE;
				goto RETRYCASE;
			}

			if (ExtraChunks >= 1 && CurrentMmioIoRatios.IO [Idx] > 1)
			{
				TempExtraChunks = /*TempExtraChunks +*/ ExtraChunks;
				if ( (TempExtraChunks + CurrentMmioIoRatios.IO [Index]) < RequiredMMIOIoRatio.IO[Index])
				{
					CurrentMmioIoRatios.IO[Idx] = CurrentMmioIoRatios.IO[Idx] - ExtraChunks;
					CurrentMmioIoRatios.IO [Index] = CurrentMmioIoRatios.IO [Index] + ExtraChunks;
					Index1++;
					continue;
				}
				CurrentMmioIoRatios.IO [Idx] = CurrentMmioIoRatios.IO[Idx] - (RequiredMMIOIoRatio.IO [Index] - CurrentMmioIoRatios.IO [Index]);
				CurrentMmioIoRatios.IO [Index] = CurrentMmioIoRatios.IO [Index] + (RequiredMMIOIoRatio.IO [Index] - CurrentMmioIoRatios.IO [Index]);
				if (CurrentMmioIoRatios.IO [Index] == RequiredMMIOIoRatio.IO[Index])
				{
					TRACE((TRACE_ALWAYS, "\n IO:IOH=%x BestRatio=%x\n",Index ,CurrentMmioIoRatios.IO [Index]));
					break;
				}
			}

			Index1++;
		}
//      TRACE((TRACE_ALWAYS, "\n IO:IOH=%x BestRatio=%x\n",Index ,CurrentMmioIoRatios.IO [Index]));
	}

	//
	// Printing the New calculated Ratio
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		TRACE((TRACE_ALWAYS, "\n IO:IOH=%x Newly Calculated BestRatio=%x\n",Index,CurrentMmioIoRatios.IO [Index]));
		TRACE((TRACE_ALWAYS, "\n IO:IOH=%x CurrentBoot Ratio=%x\n",Index,AutoAdjustMmioIoVariable->IoRatio [Index]));
	}

	//
	// Comparing Newly got Ratio's with current boot Ratio,if same then there is no possible way
	//
	TRACE((TRACE_ALWAYS, "\n IO : Comparing the IOh's New Adjusted Ratio's with the Current Boot Ratio\n"));
	NotPossibleFlag = TRUE;
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		if (IoRatioToIoChunks [Index] != CurrentMmioIoRatios.IO [Index])
		{
			NotPossibleFlag = FALSE;
			AutoAdjustMmioIoVariable->IoRatio [Index] = CurrentMmioIoRatios.IO [Index];
		}
	}

	//
	// If NotPossibleFlag = FALSE then we got the best ratio to boot up the system
	//
	if (NotPossibleFlag == FALSE)
	{
		ResetRequired = TRUE; // System has to reset;
	}

	//
	// If NotPossibleFlag = TRUE then check the retrycounter
	// if counter != 0 try one more best ratio before trying OEM defined TABLE
	// Just incrementing the affected IOH by 1 and decrementing unaffected IOH by 1 if it has sufficient ratio
RETRYCASE:
  	if (NotPossibleFlag == TRUE)
  	{
  		VarSize = sizeof(RETRY_COUNTER_MMIO_IO);
  		GetSetStatus = pRS->GetVariable (L"RetryCounterMmioIo",
  						 &RetryCounterMmioIoGuid,
						 NULL,
						 &VarSize,
						 &RetryCounter
		  				 );

  		if (EFI_ERROR(GetSetStatus))
  		{
  			TRACE((-1,"PciRB: IO Allocation Failed: GetVariable : RETRY_COUNTER_MMIO_IO\n"));
  			return NotPossibleFlag;

  		}

  		if (RetryCounter.IO != 0)
  		{
  			TRACE((TRACE_ALWAYS, "\n IO: SYSTEM IS GOING TO TRY THE MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR"));
			TRACE((TRACE_ALWAYS, "\n TO FIND BEST RATIO\n"));
			TRACE((TRACE_ALWAYS, "\n MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR %d\n",RetryCounter.IO));
			for (Index = 0 ; Index < NumIoH ; Index++)
			{
				//Affected IOH
				if (IohOutofResInfo.IoOutofResrcFlag [Index] != 1)
					continue;

				Index1 = 0 ;
				TempExtraChunks =  0;

				while( ValidIOH [Index1] != 0xFF)
				{
					Idx = ValidIOH [Index1];
					if ((CurrentMmioIoRatios.IO [Idx] - RequiredMMIOIoRatio.IO[Idx] >= 1) && ((CurrentMmioIoRatios.IO [Idx] - 1) >= RequiredMMIOIoRatio.IO[Idx]) && CurrentMmioIoRatios.IO [Idx] > 1)
					{
						TempExtraChunks = TempExtraChunks + (CurrentMmioIoRatios.IO [Idx] - RequiredMMIOIoRatio.IO[Idx]);
						if ( TempExtraChunks + CurrentMmioIoRatios.IO [Index] < RequiredMMIOIoRatio.IO[Index])
						{
							Index1++;
							continue;
						}

           				AutoAdjustMmioIoVariable->IoRatio [Idx] =  AutoAdjustMmioIoVariable->IoRatio [Idx] -1 ;
           				AutoAdjustMmioIoVariable->IoRatio [Index] = AutoAdjustMmioIoVariable->IoRatio [Index]+1;
           				TRACE((TRACE_ALWAYS, "\n IO:IOH=%x IOH=%x BEST RATIO's=%x %x\n",Index, Idx,AutoAdjustMmioIoVariable->IoRatio [Index],AutoAdjustMmioIoVariable->IoRatio [Idx]));
           				break;
					}
					Index1++;
				}
			}
           	/* Checking whether we got any new values if not
		 	 Don't waste the RetryCounter values
           	 */
			for (Index = 0 ; Index < NumIoH ; Index++)
	 		{
				if (AutoAdjustMmioIoVariable->IoRatio  [Index] != CurrentMmioIoRatios.IO [Index])
				{
					NotPossibleFlag = FALSE;
				}
	 		}

			if (NotPossibleFlag == TRUE )
			{
				ResetRequired = TRUE; // System has to reset;
				TRACE((-1,"PciRB: IO :CURRENT AND CALCUATED RATIO ARE SAME: NOT POSSIBLE CASE\n"));
				return NotPossibleFlag;
			}

			// We tried the retry counter once,so set counter as 0.No more try is allowed
			RetryCounter.IO = RetryCounter.IO -1; ;
			GetSetStatus = pRS->SetVariable (L"RetryCounterMmioIo",
                                     &RetryCounterMmioIoGuid,
                                     EFI_VARIABLE_NON_VOLATILE |
                                     EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                     EFI_VARIABLE_RUNTIME_ACCESS,
                                     sizeof(RETRY_COUNTER_MMIO_IO),
                                     &RetryCounter );

			if (EFI_ERROR(GetSetStatus))
			{
				TRACE((-1,"PciRB: IO Allocation Failed: SetVariable : RETRY_COUNTER_MMIO_IO\n"));
				return NotPossibleFlag;
			}

			NotPossibleFlag = FALSE;
			ResetRequired = TRUE; // System has to reset;

  		}
  	}
	return NotPossibleFlag;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: 	AutoAdjustMMIoRatio
//
// Description:	This function performs AutoAdjusted MMIO Ratio's
//		Will calculate the Best MMIO Ratio
//
// Input:	NumIoH  			- Number of IOH
//		AutoAdjustMmioIoVariable	- Structure has Current MMIO and IO Ratio Information
//		dxe 				- DXE_SERVICES
//
// Output:	AutoAdjustMmioIoVariable - Newly Calculated Ratio's
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN
AutoAdjustMMIoRatio(
    UINT8 				NumIoH,
    AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA *AutoAdjustMmioIoVariable,
    DXE_SERVICES 			*dxe
)
{

	UINT8			Index,Index1,Idx;
	UINT64			ExtraSpaceNeededCount = 0;
	UINT8			ExtraChunks;
	UINT8			TempExtraChunks = 0;
	BOOLEAN     		NotPossibleFlag;
	UINT8       		FailedMMioIndex;
	UINT8       		ValidIOH [NUMBER_CPU_SOCKETS];
	UINTN       		VarSize;
	EFI_STATUS  		GetSetStatus;

	//
	// Collecting the Current Boot MMIO chunk
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		CurrentMmioIoRatios.MMIO [Index]   = MMioRatioToMMiolChunks [Index] ;
		TRACE((TRACE_ALWAYS, "\n MMIO: IOH=%x CurrentBootMMIO chunk (64MB GRA) G=%x\n",Index,CurrentMmioIoRatios.MMIO [Index]));
	}

	//
	// Calculating the Required Ratio for Each IOh's
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		for ( FailedMMioIndex = 0 ; FailedMMioIndex < MMIO32_TYPES/*FailedMmioTypesCount [Index]*/; FailedMMioIndex++)
		{
			TRACE((TRACE_ALWAYS, "\n IOH=%x MMIO Requested=%x\n",Index,IohOutofResInfo.MmioRequested [Index] [FailedMMioIndex]));
			RequiredMMIOIoRatio.MMIO[Index] = RequiredMMIOIoRatio.MMIO[Index] + (UINT8) ((IohOutofResInfo.MmioRequested[Index][FailedMMioIndex])/SAD_MMIOL_GRANTY);
			ExtraSpaceNeededCount = (IohOutofResInfo.MmioRequested[Index][FailedMMioIndex])%SAD_MMIOL_GRANTY;
//			TRACE((TRACE_ALWAYS, "\n ExtraSpaceNeededCount=%x\n",ExtraSpaceNeededCount));

			if (ExtraSpaceNeededCount > 0)
			{
				RequiredMMIOIoRatio.MMIO[Index] = RequiredMMIOIoRatio.MMIO[Index] + 1;
			}
		}

		TRACE((TRACE_ALWAYS, "\n MMIO : IOH=%x Required Ratio=%x\n",Index,RequiredMMIOIoRatio.MMIO [Index]));
	}


	//
	// Check which IOH has the problem and whether other IOH has some free space.
	// Creating Dummy ValidIOH Strcuture;
	// If FreeSpace is available in IOH,Fill IOH Index in the ValidIOH Table
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		ValidIOH[Index] = 0xFF;
	}

	for (Index = 0 , Index1 = 0; Index < 	NumIoH ; Index++)
	{
		if (IohOutofResInfo.MmmioOutofResrcFlag [Index] != 1)
		{
			ValidIOH[Index1] = Index;
			Index1++;
		}
	}

	//
	// Sanity Check : ValidIOH present
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		if (ValidIOH[Index] != 0xFF)
			break;
	}

	//
	// No IOH is valid : THis is a not possible case
	// Need to try OEM Defined Table
	if (Index == NumIoH)
	{
		NotPossibleFlag = TRUE;
		TRACE((TRACE_ALWAYS, "\n MMIO : NO VALID IOH's are present\n"));
		TRACE((TRACE_ALWAYS, "\n System will try the OEM DEFINED TABLE\n"));
		return NotPossibleFlag;
	}

	//
	// Printing VALID IOH
	//
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		if (ValidIOH[Index] != 0xFF)
			TRACE((TRACE_ALWAYS, "\n VALID IOH=%x\n",ValidIOH[Index]));
	}

	//
	//	Going to take the resource from the validIOH
	//
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		if (IohOutofResInfo.MmmioOutofResrcFlag [Index] != 1)
			continue; // This IOH is not failed

		//
		// Going to use the VALIDIOH TABLE to help the affected IOH
		//
		Index1 = 0 ;
		TempExtraChunks =  0;
		while( ValidIOH [Index1] != 0xFF)
		{
			Idx = ValidIOH [Index1];
			ExtraChunks = (CurrentMmioIoRatios.MMIO [Idx] - RequiredMMIOIoRatio.MMIO[Idx]);
			if (RequiredMMIOIoRatio.MMIO [Index] < CurrentMmioIoRatios.MMIO [Index])
			{
				TRACE((TRACE_ALWAYS, "\n MMIO: Currently Alloted Space is more than Required Space\n"));
				TRACE((TRACE_ALWAYS, "\n But Still couldn't get continous space\n"));
				TRACE((TRACE_ALWAYS, "\n GOING to try MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR to fix the PCI RESOURCE GRANULARTIY issue\n"));
				NotPossibleFlag = TRUE;
				ResetRequired = TRUE;
				goto RETRYCASE;
			}

			if ((RequiredMMIOIoRatio.MMIO [Index] > CurrentMmioIoRatios.MMIO [Index]) && ExtraChunks >= 1)
			{
				TempExtraChunks = /*TempExtraChunks +*/ ExtraChunks;
				if ( (TempExtraChunks + CurrentMmioIoRatios.MMIO [Index]) < RequiredMMIOIoRatio.MMIO[Index])

				{
					CurrentMmioIoRatios.MMIO[Idx] = CurrentMmioIoRatios.MMIO[Idx] - ExtraChunks;
					CurrentMmioIoRatios.MMIO [Index] = CurrentMmioIoRatios.MMIO [Index] + ExtraChunks;
					Index1++;
					continue;
				}

				CurrentMmioIoRatios.MMIO [Idx] = CurrentMmioIoRatios.MMIO[Idx] - (RequiredMMIOIoRatio.MMIO [Index] - CurrentMmioIoRatios.MMIO [Index]);
				CurrentMmioIoRatios.MMIO [Index] = CurrentMmioIoRatios.MMIO [Index] + (RequiredMMIOIoRatio.MMIO [Index] - CurrentMmioIoRatios.MMIO [Index]);

				if (CurrentMmioIoRatios.MMIO [Index] == RequiredMMIOIoRatio.MMIO[Index])
				{
					break;
				}
			}

			Index1++;
		}
		TRACE((TRACE_ALWAYS, "\n MMIO :IOH=%x BestRatio=%x\n",Index ,CurrentMmioIoRatios.MMIO [Index]));
	}

	//
	// Printing the New calculated Ratio
	//
	for (Index = 0 ; Index < 	NumIoH ; Index++)
	{
		TRACE((TRACE_ALWAYS, "\n IO:IOH=%x Newly Calculated BestRatio=%x\n",Index,CurrentMmioIoRatios.MMIO [Index]));
		TRACE((TRACE_ALWAYS, "\n IO:IOH=%x CurrentBoot Ratio=%x\n",Index,AutoAdjustMmioIoVariable->MmiolRatio [Index]));
	}

	//
	// Comparing Newly got Ratio's with current boot Ratio,if same then there is no possible way
	//
	TRACE((TRACE_ALWAYS, "\n MMIO : Comparing the IOh's New Adjusted Ratio's with the Current Boot Ratio\n"));
	NotPossibleFlag = TRUE;
	for (Index = 0 ; Index < NumIoH ; Index++)
	{
		if (MMioRatioToMMiolChunks [Index] != CurrentMmioIoRatios.MMIO [Index])
		{
			NotPossibleFlag = FALSE;
			AutoAdjustMmioIoVariable->MmiolRatio [Index] = CurrentMmioIoRatios.MMIO [Index];
		}
	}

	//
	// If NotPossibleFlag = FALSE then we got the best ratio to boot up the system
	//
	if (NotPossibleFlag == FALSE)
	{
		ResetRequired = TRUE; // System has to reset;
	}

	//
	// If NotPossibleFlag = TRUE then check the retrycounter
	// if counter != 0 try one more best ratio before trying OEM defined TABLE
	// Just incrementing the affected IOH by 1 and decrementing unaffected IOH by 1 if it has sufficient ratio
RETRYCASE:
  	  if (NotPossibleFlag == TRUE)
  	  {
  		  VarSize = sizeof(RETRY_COUNTER_MMIO_IO);
  		  GetSetStatus = pRS->GetVariable (L"RetryCounterMmioIo",
			  					           &RetryCounterMmioIoGuid,
										   NULL,
										   &VarSize,
										   &RetryCounter
								           );
  		  if (EFI_ERROR(GetSetStatus))
  		  {
  			  TRACE((-1,"PciRB: MMIO Allocation Failed: GetVariable : RETRY_COUNTER_MMIO_IO\n"));
  			  return NotPossibleFlag;
  		  }

		  if (RetryCounter.MMIO != 0)
		  {
			  TRACE((TRACE_ALWAYS, "\n MMIO: SYSTEM IS GOING TO TRY THE MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR"));
			  TRACE((TRACE_ALWAYS, "\n TO FIND BEST RATIO\n"));
			  TRACE((TRACE_ALWAYS, "\n MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR %d\n",RetryCounter.MMIO));

			  for (Index = 0 ; Index < NumIoH ; Index++)
			  {
				  //    Affected IOH
				  if (IohOutofResInfo.MmmioOutofResrcFlag [Index] != 1)
					  continue;
				  Index1 = 0 ;
				  TempExtraChunks =  0;
				  while( ValidIOH [Index1] != 0xFF)
				  {
					  Idx = ValidIOH [Index1];
					  if ((CurrentMmioIoRatios.MMIO [Idx] - RequiredMMIOIoRatio.MMIO[Idx] >= 1) && ((CurrentMmioIoRatios.MMIO [Idx] - 1) >= RequiredMMIOIoRatio.MMIO[Idx]) && CurrentMmioIoRatios.MMIO [Idx] >1)
					  {
						  TempExtraChunks = TempExtraChunks + (CurrentMmioIoRatios.MMIO [Idx] - RequiredMMIOIoRatio.MMIO[Idx]);
						  if ( TempExtraChunks + CurrentMmioIoRatios.MMIO [Index] < RequiredMMIOIoRatio.MMIO[Index])
						  {
							  Index1++;
							  continue;
						  }

						  AutoAdjustMmioIoVariable->MmiolRatio [Idx] =  AutoAdjustMmioIoVariable->MmiolRatio [Idx] -1 ;
						  AutoAdjustMmioIoVariable->MmiolRatio [Index] = AutoAdjustMmioIoVariable->MmiolRatio [Index]+1;
						  TRACE((TRACE_ALWAYS, "\n IO:IOH=%x IOH=%x BEST RATIO's=%x %x\n",Index, Idx,AutoAdjustMmioIoVariable->MmiolRatio [Index],AutoAdjustMmioIoVariable->MmiolRatio [Idx]));
						  break;
					  }
					  Index1++;
			      }

				//TRACE((TRACE_ALWAYS, "\n MMIO:NOT POSSIBLE CASE OCCURED:\n"));
				//TRACE((TRACE_ALWAYS, "\n MMIO:IOH=%x BEST RATIO=%x\n",AutoAdjustMmioIoVariable->MmiolRatio[Index]));
			  }

			  /* Checking whether we got any new values if not
		 	 Don't waste the RetryCounter values
			   */
			  for (Index = 0 ; Index < NumIoH ; Index++)
	 		  {
				  if (AutoAdjustMmioIoVariable->MmiolRatio  [Index] != CurrentMmioIoRatios.MMIO [Index])
				  {
					  NotPossibleFlag = FALSE;
				  }
	 		  }

			  if (NotPossibleFlag == TRUE )
			  {
				  ResetRequired = TRUE; // System has to reset;
				  TRACE((-1,"PciRB: MMIO :CURRENT AND CALCUATED RATIO ARE SAME: NOT POSSIBLE CASE\n"));
				  return NotPossibleFlag;
			  }

			  // We tried the retry counter once,so set counter as 0.No more try is allowed
			  RetryCounter.MMIO = RetryCounter.MMIO -1 ;
			  GetSetStatus = pRS->SetVariable (L"RetryCounterMmioIo",
                                     	 	   &RetryCounterMmioIoGuid,
                                     	 	   EFI_VARIABLE_NON_VOLATILE |
                                               EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                               EFI_VARIABLE_RUNTIME_ACCESS,
                                               sizeof(RETRY_COUNTER_MMIO_IO),
                                               &RetryCounter );

			  if (EFI_ERROR(GetSetStatus))
			  {
				  TRACE((-1,"PciRB: MMIO Allocation Failed: SetVariable : RETRY_COUNTER_MMIO_IO\n"));
				  return NotPossibleFlag;

			  }
			  NotPossibleFlag = FALSE;
			  ResetRequired = TRUE; // System has to reset;
		  }
  	  }

	return NotPossibleFlag;

}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: 	AutoAdjustRatios
//
// Description:	This function performs AutoAdjusted MMIO or IO Ratio's
//				Check for MMIO/IO Failure.Will call AutoAdjustMMIoRatio or AutoAdjustIoRatio
//				to calculate the best MMIO or IO Ratio to boot.
//
// Input:		SystemConfiguration - SYSTEM_CONFIGURATION
//				OutOfResrcFailure   - MMIO/IO Failure
//				dxe 				- DXE_SERVICES
//
// Output:	None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN AutoAdjustRatios(
//    SYSTEM_CONFIGURATION	*SystemConfiguration,
    UINT8 			OutOfResrcFailure ,
    DXE_SERVICES 	        *dxe)
{

	EFI_STATUS 	GetSetStatus;
	UINT8		NumIoH;
	UINT64      	NewIoLimit = 0;
	BOOLEAN 	Not_Possible_Flag = FALSE;
	UINT8       	Index;
	UINT8       	TotalMmiolChunk;
	UINT8       	AvailableMmioChunk;
	UINT8       	UnusedMmiolChunk;
	UINT8       	TotalIoChunk;
	UINT8       	UnusedIoChunk;
	LAST_BOOT_FAILED_IOH    LastBootFailedIOH;
	AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA		AutoAdjustMmioIoVariable;
	UINTN       	VarSize = sizeof (AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA);
	UINTN       	VariableSize = sizeof (LAST_BOOT_FAILED_IOH);

	//
	// Initialize AutoAdjustMmioIoVariable
	//

	for (Index = 0; Index < MAX_CPU_SOCKETS; Index++)
	{
		AutoAdjustMmioIoVariable.MmiolRatio [Index] = 0;
		AutoAdjustMmioIoVariable.IoRatio [Index]    = 0;
	}

	//
	// Getting the Number of Sockets
	//
	NumIoH = NbGetMaximumIohSockets ();

	//
	// Get AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA from NVRAM
	//
	GetSetStatus = pRS->GetVariable (L"AutoAdjustMMIOIO",
									 &gEfiAutoMmioIOVariableGuid,
									 NULL,
									 &VarSize,
									 &AutoAdjustMmioIoVariable
		  							 );

	if (EFI_ERROR(GetSetStatus))
	{
		for (Index = 0 ; Index < NumIoH ; Index++)
		{
			AutoAdjustMmioIoVariable.MmiolRatio [Index] = SystemConfiguration.MmiolRatio[Index];
			AutoAdjustMmioIoVariable.IoRatio [Index]    = SystemConfiguration.IoRatio[Index];
		}
	}


	//
	// FOR MMIO FAILURE
	//

	//
	// Check if there is any available chunks
	//
	if (OutOfResrcFailure == MMIOFAILURE)
	{
		TotalMmiolChunk = 0;
		for (Index = 0 ; Index < NumIoH ; Index++)
		{
			TotalMmiolChunk  = TotalMmiolChunk + AutoAdjustMmioIoVariable.MmiolRatio [Index];
		}


		AvailableMmioChunk = (UINT8)((IohOutofResInfo.MmioLimit [NumIoH-1] - IohOutofResInfo.MmioBase [0])/SAD_MMIOL_GRANTY);
		//
		// if TotalMmiolChunk > AvailableMmioChunk,spilit the resource equally
		//
		if (TotalMmiolChunk > AvailableMmioChunk)
		{
			TotalMmiolChunk = AvailableMmioChunk;
			for (Index = 0; Index < NumIoH; ++Index)
			{
				AutoAdjustMmioIoVariable.MmiolRatio [Index]  = 1;
			}
		}

		//
		// if TotalMmiolChunk < AvailableMmioChunk,calcuate the extra avail chunk and add it one by one to each IOH.
		// Add the remaining chunks to IOH0

		if (TotalMmiolChunk <= AvailableMmioChunk)
		{
			UnusedMmiolChunk  = AvailableMmioChunk % TotalMmiolChunk;

			//
			// Distribute the "whole" resources
			//
			for (Index = 0; Index < NumIoH; ++Index)
			{
				AutoAdjustMmioIoVariable.MmiolRatio [Index]  = AutoAdjustMmioIoVariable.MmiolRatio [Index] * (AvailableMmioChunk / TotalMmiolChunk);
			}

			//
			// Distribute the "residual" resources
			//
			for (Index = 0; Index < NumIoH; ++Index)
			{
				if (UnusedMmiolChunk && (AutoAdjustMmioIoVariable.MmiolRatio [Index] != 0))
				{
					AutoAdjustMmioIoVariable.MmiolRatio [Index]= AutoAdjustMmioIoVariable.MmiolRatio [Index]+1;
					UnusedMmiolChunk = UnusedMmiolChunk -1 ;
				}
			}

			//
			// Anything left, apply to Socket0
			//
			AutoAdjustMmioIoVariable.MmiolRatio [0] = AutoAdjustMmioIoVariable.MmiolRatio [0] + UnusedMmiolChunk;
		}

	}

	//
	// FOR IO FAILURE
	//

	//
	// Check if there is any available chunks
	//
	if (OutOfResrcFailure == IOFAILURE)
	{
		TotalIoChunk = 0;
		for (Index = 0 ; Index < NumIoH ; Index++)
		{
			TotalIoChunk  = TotalIoChunk +  AutoAdjustMmioIoVariable.IoRatio [Index];
		}
		//
		// if TotalIoChunk > MAX_SAD_TARGETS,spilit the resource equally
		//
		if (TotalIoChunk > MAX_SAD_TARGETS)
		{
			TotalIoChunk = MAX_SAD_TARGETS;
			for (Index = 0; Index < NumIoH; ++Index)
			{
				AutoAdjustMmioIoVariable.IoRatio [Index]  = 1;
			}
		}

		//
		// if TotalIoChunk < MAX_SAD_TARGETS,calcuate the extra avail chunk and add it one by one to each IOH.
		// Add the remaining chunks to IOH0

		if (TotalIoChunk <= MAX_SAD_TARGETS)
		{
			UnusedIoChunk  = MAX_SAD_TARGETS % TotalIoChunk;
			//
			// Distribute the "whole" resources
			//
			for (Index = 0; Index < NumIoH; ++Index)
			{
				AutoAdjustMmioIoVariable.IoRatio [Index]  =  AutoAdjustMmioIoVariable.IoRatio [Index] * (MAX_SAD_TARGETS / TotalIoChunk);
			}

			//
			// Distribute the "residual" resources
			//
			for (Index = 0; Index < NumIoH; ++Index)
			{
				if (UnusedIoChunk && (AutoAdjustMmioIoVariable.IoRatio [Index] != 0))
				{
					AutoAdjustMmioIoVariable.IoRatio [Index]=  AutoAdjustMmioIoVariable.IoRatio [Index]+1;
					UnusedIoChunk = UnusedIoChunk -1 ;
				}
			}

			//
			// Anything left, apply to Socket0
			//
			AutoAdjustMmioIoVariable.IoRatio [0] =  AutoAdjustMmioIoVariable.IoRatio [0] + UnusedIoChunk;
		}

	}


   for (Index = 0; Index < NumIoH; Index++)
   {
	   MMioRatioToMMiolChunks[Index]  =   AutoAdjustMmioIoVariable.MmiolRatio [Index];
	   IoRatioToIoChunks[Index]  		 =   AutoAdjustMmioIoVariable.IoRatio [Index];
   }

   //
   // Check IO Failure
   //
   if (OutOfResrcFailure == IOFAILURE)
   {
	   Not_Possible_Flag = AutoAdjustIoRatio(NumIoH,&AutoAdjustMmioIoVariable,dxe);
   }

   //
   // Check MMIO Failure
   //
   if (OutOfResrcFailure == MMIOFAILURE)
   {
	   Not_Possible_Flag = AutoAdjustMMIoRatio(NumIoH,&AutoAdjustMmioIoVariable,dxe);
   }


   // Set the AutoAdjustMmioIoVariable Structure which got update in AutoAdjustIoRatio or AutoAdjustMMIoRatio function
   // Not_Possible_Flag = False,then we got a best ratio to boot up the system,
   // Set the new best ratio value in AutoAdjustMmioIoVariable
   // Clear the LastBootFailedIOH Structure
   //
   if (Not_Possible_Flag == FALSE)
   {
	   GetSetStatus = pRS->SetVariable (L"AutoAdjustMMIOIO",
                              	  	  	  &gEfiAutoMmioIOVariableGuid,
                              	  	  	  EFI_VARIABLE_NON_VOLATILE |
                              	  	  	  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                              	  	  	  sizeof(AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA),
                              	  	  	  &AutoAdjustMmioIoVariable );

	   if (EFI_ERROR (GetSetStatus))
	   {
		   TRACE((TRACE_ALWAYS, "[[\n GetVariable Failed () in AutoAdjustMemIoRatios()]]\n"));
		   TRACE((TRACE_ALWAYS, "[[ Setting the  Not_Possible_Flag : PCIBUS Driver will Skip all Devices except ONBOARD and VGA]]\n"));
		   Not_Possible_Flag = TRUE;
		   return Not_Possible_Flag;
	   }

	   GetSetStatus = pRS->GetVariable (L"LastBootFailedIoh",
			   	   	   	   	   	   	   	  &LastBootFailedIohGuid,
                                          NULL,
                                          &VariableSize,
                                          &LastBootFailedIOH );

	   if (EFI_ERROR (GetSetStatus))
	   {
		   TRACE((TRACE_ALWAYS, "[[\n GetVariable Failed () in AutoAdjustMemIoRatios()]]\n"));
		   TRACE((TRACE_ALWAYS, "[[ Setting the  Not_Possible_Flag : PCIBUS Driver will Skip all Devices except ONBOARD and VGA]]\n"));
		   Not_Possible_Flag = TRUE;
		   return Not_Possible_Flag;
	   }

	   //
	   // Clear the LastBootFailedIOH Structure
	   //

	   for (Index = 0 ; Index < NumIoH ; Index++)
	   {
		   if (OutOfResrcFailure == IOFAILURE)
		   {
			   TRACE((TRACE_ALWAYS, "\n IO : Clearing IOH =%x IoOutofResrcFlag Flag\n",Index));
			   LastBootFailedIOH .IoOutofResrcFlag[Index] =  0;

		   }

		   if (OutOfResrcFailure == MMIOFAILURE)
		   {
			   TRACE((TRACE_ALWAYS, "\n MMIO : Clearing IOH =%x MmmioOutofResrcFlag Flag\n",Index));
			   LastBootFailedIOH .MmmioOutofResrcFlag[Index] =  0;
		   }

	   }

	   //
	   // Clear the LastBootFailedIOH Structure
	   //
	   GetSetStatus = pRS->SetVariable (L"LastBootFailedIoh",
                          	  	  	  	  &LastBootFailedIohGuid,
                          	  	  	  	  EFI_VARIABLE_NON_VOLATILE |
                          	  	  	  	  EFI_VARIABLE_BOOTSERVICE_ACCESS |
                          	  	  	  	  EFI_VARIABLE_RUNTIME_ACCESS,
                          	  	  	  	  VariableSize,
                          	  	  	  	  &LastBootFailedIOH );

	   if (EFI_ERROR (GetSetStatus))
	   {
		   TRACE((TRACE_ALWAYS, "[[\n SetVariable Failed () when updating LastBootFailedIOH Structure]]\n"));
		   for (Index = 0 ; Index < NumIoH ; Index++)
		   {
			   if (OutOfResrcFailure == IOFAILURE)
			   {
				   TRACE((TRACE_ALWAYS, "\n IO : Checking IOH =%x IoOutofResrcFlag Flag\n",Index));
				   if (IohOutofResInfo.IoOutofResrcFlag [Index] == 1)
				   {
					   TRACE ((-1, "Requested IO space is not possible!  System is Halted!\n"));
					   EFI_DEADLOOP();
				   }
			   }

			   if (OutOfResrcFailure == MMIOFAILURE)
			   {
				   TRACE((TRACE_ALWAYS, "\n MMIO : Checking IOH =%x MmmioOutofResrcFlag Flag\n",Index));
				   if (IohOutofResInfo.MmmioOutofResrcFlag [Index] == 1)
				   {
					   TRACE ((-1, "Requested MMIO space is not possible!  System is Halted!\n"));
					   EFI_DEADLOOP();
				   }
			   }

		   }
	   }
   }

  return Not_Possible_Flag;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: 	IoOutOfResourceConditionOccured
//
// Description:	This function fills information (IO Requested,IO Limit etc for the Failed IOH)
//							Also,checks if the LastBootFailedIOH and this Boot IOH matches
//							if TRUE,System will try for the OEM DEFINED TABLE AND NOT THE AUTOADJUST ALOGRITHM
//							if FALSE,System will try for THE AUTOADJUST ALOGRITHM
//
// Input:					RootBrgIndex			 -  Index of the IOH
//							LastBootFailedIOH 		 -  LastBootFailedIOH Structure
//							IohOutofResInfo 		 -  IohOutofResInfo Strucuture
//							IoRequestedLen         	 -  IoRequested Length
//							IoTypeIndex				 -  IoTypeIndex (16/32 bit)
//
// Output:			LastBootFailedIOH			 - Updated LastBootFailedIOH Strucuture
//							IohOutofResInfo				 - Updated IohOutofResInfo Strcuture (FAILED IOH Details)
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS IoOutOfResourceConditionOccured (
	IN 	UINTN 							RootBrgIndex,
	IN 	LAST_BOOT_FAILED_IOH 			*LastBootFailedIOH,
 	IN 	IOHX_OUT_OF_RESOURCE_INFO		*IohOutofResInfo,
	IN  UINT64							IoRequestedLen,
	IN  UINT8							IoTypeIndex
)
{

	EFI_STATUS	  GetSetStatus;
	UINTN         	  VariableSize = sizeof (LAST_BOOT_FAILED_IOH);

	GetSetStatus = pRS->GetVariable	(L"LastBootFailedIoh",
                                   &LastBootFailedIohGuid,
                                   NULL,
                                   &VariableSize,
                                   LastBootFailedIOH );

   if(EFI_ERROR(GetSetStatus))
   {
	   TRACE((-1,"PciRB: IO Allocation Failed: GetVariable : LastBootFailedIOH Structure Failed\n"));
	   return GetSetStatus;
   }

   // This case will come into picture : When trying the OEM TABLE
   // We are disabling few devices and CalculateBridge function will be called.
   // IO Failure occurs during trying the OEM DEFINED TABLE (In the current boot and not in the last boot)
   // So don't set the IOFAILURE FLAG instead Try again the OEM DEFINED TABLE
   if (LastBootFailedIOH ->IoOutofResrcFlag[RootBrgIndex] != 1 && MaxPcidevicesDisableCount != MAX_PCIDEVICES_DISABLE_COUNT)
   {
	  if (MaxPcidevicesDisableCount)
	  {
//		TRACE ((-1, "IO : This case occurs even after disabling the devices in OEM DEFINED TABLE in the current boot itself\n"));
//		TRACE ((-1, "By removing the device from CPU %d!\n",RootBrgIndex));
		return EFI_NOT_FOUND ;
	  }

	  else
      {
		  TRACE ((-1, "Requested IO space is not possible!  System is Halted!\n"));
		  EFI_DEADLOOP();
      }
   }

   //
   // Checking LastBootFailedIOH Structure
   // In Current and LastBoot,if the same IOH has failed,then we should Try OEM DEFINED Table
   // Else try our AutoAdjustRatio Algorithm
   // Taking the backup of the  LastBootFailedIOH ->IoOutofResrcFlag,after trying OEM DEFINED Table,same IOH may fail
   // This is an absolute not possible case so halt the system

   if (TempIoOutofResrcFlag[RootBrgIndex] == 1)
   {
	   if (MaxPcidevicesDisableCount)
	   {
		   TRACE ((-1, "IO : System is Trying the OEM DEFINED TABLE for this much time!%d\n",MaxPcidevicesDisableCount));
		   TRACE ((-1, "By removing the device from CPU %d!\n",RootBrgIndex));
		   MaxPcidevicesDisableCount--;
		   return EFI_NOT_FOUND ;
	   }
	   else
	   {
		   TRACE ((-1, "Requested IO space is not possible!  System is Halted!\n"));
		   EFI_DEADLOOP();
	   }
   }

   if ((LastBootFailedIOH ->IoOutofResrcFlag[RootBrgIndex] == 1))
   {
	   TempIoOutofResrcFlag[RootBrgIndex] = LastBootFailedIOH ->IoOutofResrcFlag[RootBrgIndex];
	   LastBootFailedIOH ->IoOutofResrcFlag[RootBrgIndex] =  0;
	   GetSetStatus = pRS->SetVariable (L"LastBootFailedIoh",
                                       &LastBootFailedIohGuid,
                                       EFI_VARIABLE_NON_VOLATILE |
                                       EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                       EFI_VARIABLE_RUNTIME_ACCESS,
                                       VariableSize,
                                       LastBootFailedIOH );

	   if(EFI_ERROR(GetSetStatus))
	   {
		   TRACE((-1,"PciRB: IO Allocation Failed: SetVariable : LastBootFailedIOH structure Failed\n"));
		   return GetSetStatus;
	   }

	   TRACE((-1,"PciRB: IO Allocation Failed: System Tried for the best Ratio.System won't even boot with the best ratio\n"));
	   TRACE((-1,"System is going to execute the OEM Defined Table for normal boot\n"));
	   MaxPcidevicesDisableCount--;
	   return EFI_NOT_FOUND ; // Already tried these ratio's go to OEM defined Table
   }

   //
   // Going to try our AutoAdjustRatio Algorithm.
   // Algorithm : Check the Required Ratio for all IOH's.Add the extra Ratio
   // to the affected IOH and reset the machine.
   // Filling the LastBootFailedIoh Structure,IoOutofResrcFlag,IoLimit and IoRequested values
   //

	IohOutofResInfo->IoOutofResrcFlag[RootBrgIndex] = 1;
	IohOutofResInfo->IoRequested[RootBrgIndex][IoTypeIndex] = IoRequestedLen;

	//
	// Filling the Failed IOH details in the LastBootFailedIOH Structure
	// These Variables will be cleared when we get a best ratio to boot or
	// If we try to execute the OEM DEFINED Tabled for the normal boot

	LastBootFailedIOH ->IoOutofResrcFlag[RootBrgIndex] =  IohOutofResInfo->IoOutofResrcFlag[RootBrgIndex];
	GetSetStatus = pRS->SetVariable (L"LastBootFailedIoh",
                                   &LastBootFailedIohGuid,
                                   EFI_VARIABLE_NON_VOLATILE |
                                   EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                   EFI_VARIABLE_RUNTIME_ACCESS,
                                   VariableSize,
                                   LastBootFailedIOH );

	if(EFI_ERROR(GetSetStatus))
	{
		TRACE((-1,"PciRB: IO Allocation Failed: SetVariable : LastBootFailedIOH structure Failed\n"));
		return GetSetStatus;
	}

	return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: 	MmioOutOfResourceConditionOccured
//
// Description:	This function fills information (MMIO Requested,MMIO Limit etc for the Failed IOH)
//							Also,checks if the LastBootFailedIOH and this Boot IOH matches
//							if TRUE,System will try for the OEM DEFINED TABLE AND NOT THE AUTOADJUST ALOGRITHM
//							if FALSE,System will try for THE AUTOADJUST ALOGRITHM
//
// Input:				    RootBrgIndex			 -  Index of the IOH
//							LastBootFailedIOH 		 -  LastBootFailedIOH Structure
//							IohOutofResInfo 		 -  IohOutofResInfo Strucuture
//							IoRequestedLen           -  IoRequested Length
//							IoTypeIndex				 -  IoTypeIndex (16/32 bit)
//
// Output:			        LastBootFailedIOH		 - Updated LastBootFailedIOH Strucuture
//							IohOutofResInfo			 - Updated IohOutofResInfo Strcuture (FAILED IOH Details)
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS MmioOutOfResourceConditionOccured (
	IN	UINTN 							RootBrgIndex,
	IN  LAST_BOOT_FAILED_IOH 			*LastBootFailedIOH,
 	IN  IOHX_OUT_OF_RESOURCE_INFO		*IohOutofResInfo,
	IN	UINT64							MmioRequestedLen,
	IN	UINT8							MmioTypeIndex
)
{

	EFI_STATUS	GetSetStatus;
	UINTN         VariableSize = sizeof (LAST_BOOT_FAILED_IOH);

	GetSetStatus = pRS->GetVariable	(L"LastBootFailedIoh",
                                   &LastBootFailedIohGuid,
                                   NULL,
                                   &VariableSize,
                                   LastBootFailedIOH);

	if(EFI_ERROR(GetSetStatus))
	{
		TRACE((-1,"PciRB: MMIO Allocation Failed: GetVariable : LastBootFailedIOH Structure Failed\n"));
		return GetSetStatus;
	}

	// This case will come into picture : When trying the OEM TABLE
	// We are disabling few devices and CalculateBridge function will be called.
	// MMIO Failure occurs during trying the OEM DEFINED TABLE (In the current boot and not in the last boot)
	// So don't set the IOFAILURE FLAG instead Try again the OEM DEFINED TABLE

	if (LastBootFailedIOH ->MmmioOutofResrcFlag[RootBrgIndex] != 1 && MaxPcidevicesDisableCount != MAX_PCIDEVICES_DISABLE_COUNT)
	{
		if (MaxPcidevicesDisableCount)
		{
//			TRACE ((-1, "MMIO : This case occurs even after disabling the devices in OEM DEFINED TABLE in the current boot itself\n"));
//			TRACE ((-1, "By removing the device from CPU %d!\n",RootBrgIndex));
			return EFI_NOT_FOUND ;
		}

		else
		{
			TRACE ((-1, "Requested MMIO space is not possible!  System is Halted!\n"));
			EFI_DEADLOOP();
		}
	}

	//
	// Checking LastBootFailedIOH Structure
	// In Current and LastBoot,if the same IOH has failed,then we should Try OEM DEFINED Table
	// Else try our AutoAdjustRatio Algorithm
	// Taking the backup of the  LastBootFailedIOH ->IoOutofResrcFlag,after trying OEM DEFINED Table,same IOH may fail
	// This is an absolute not possible case so halt the system

	if (TempMmmioOutofResrcFlag[RootBrgIndex] == 1)
	{
		if (MaxPcidevicesDisableCount)
		{
			TRACE ((-1, "MMIO : System is Trying the OEM DEFINED TABLE for this much time!%d\n",MaxPcidevicesDisableCount));
			TRACE ((-1, "By removing the device from CPU %d!\n",RootBrgIndex));
			MaxPcidevicesDisableCount--;
			return EFI_NOT_FOUND ;
		}
		else
		{
			TRACE ((-1, "Requested MMIO space is not possible!  System is Halted!\n"));
			EFI_DEADLOOP();
		}
	}

	if ((LastBootFailedIOH ->MmmioOutofResrcFlag[RootBrgIndex] == 1))
	{
		TempMmmioOutofResrcFlag[RootBrgIndex] = LastBootFailedIOH ->MmmioOutofResrcFlag[RootBrgIndex];
		LastBootFailedIOH ->MmmioOutofResrcFlag[RootBrgIndex] =  0;
		GetSetStatus = pRS->SetVariable (L"LastBootFailedIoh",
                                       &LastBootFailedIohGuid,
                                       EFI_VARIABLE_NON_VOLATILE |
                                       EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                       EFI_VARIABLE_RUNTIME_ACCESS,
                                       VariableSize,
                                       LastBootFailedIOH );

		if(EFI_ERROR(GetSetStatus))
		{
			TRACE((-1,"PciRB: IO Allocation Failed: SetVariable : LastBootFailedIOH structure Failed\n"));
			return GetSetStatus;
		}
		TRACE((-1,"PciRB: MMIO Allocation Failed: System Tried for the best Ratio.System won't even boot with the best ratio\n"));
		TRACE((-1,"System is going to execute the OEM Defined Table for normal boot\n"));
		MaxPcidevicesDisableCount--;
		return EFI_NOT_FOUND ; // Already tried these ratio's go to OEM defined Table
	}

	//
	// Going to try our AutoAdjustRatio Algorithm.
	// Algorithm : Check the Required Ratio for all IOH's.Add the extra Ratio
	// to the affected IOH and reset the machine.
	// Filling the LastBootFailedIoh Structure,IoOutofResrcFlag,IoLimit and IoRequested values
	//

	IohOutofResInfo->MmmioOutofResrcFlag[RootBrgIndex] = 1;
	IohOutofResInfo->MmioRequested[RootBrgIndex][MmioTypeIndex] = MmioRequestedLen;

	//
	// Filling the Failed IOH details in the LastBootFailedIOH Structure
	// These Variables will be cleared when we get a best ratio to boot or
	// If we try to execute the OEM DEFINED Tabled for the normal boot

	LastBootFailedIOH ->MmmioOutofResrcFlag[RootBrgIndex] =  IohOutofResInfo->MmmioOutofResrcFlag[RootBrgIndex];
	GetSetStatus = pRS->SetVariable (L"LastBootFailedIoh",
                                   &LastBootFailedIohGuid,
                                   EFI_VARIABLE_NON_VOLATILE |
                                   EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                   EFI_VARIABLE_RUNTIME_ACCESS,
                                   VariableSize,
                                   LastBootFailedIOH );

	if(EFI_ERROR(GetSetStatus))
	{
		TRACE((-1,"PciRB: IO Allocation Failed: SetVariable : LastBootFailedIOH structure Failed\n"));
		return GetSetStatus;
	}

	return EFI_SUCCESS;
}
#endif
/**
    This function is invoked in PCI Host Bridge Driver when time
    to ask GCD for resources. You can overwrite a default
    algorithm used to allocate resources for the Root Bridge.

    @param HostBrgData Pointer to Host Bridge private structure data.
    @param RootBrgData Pointer to Root Bridge private structure data.
    @param RootBrgIndex Root Bridge index (0 Based).

    @retval EFI_STATUS

    @note  Porting required if needed.
**/
EFI_STATUS HbCspAllocateResources (
							IN PCI_HOST_BRG_DATA    *HostBrgData,
							IN PCI_ROOT_BRG_DATA    *RootBrgData,
							IN UINTN                RootBrgIndex )
{
// Aptio V Server override start: Grantley Dynamic Resource Allocation

    EFI_STATUS  Status = EFI_UNSUPPORTED;
 //###DEBUG

 //PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING//
 //PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING//
 //Any Additional Variables goes here

 	UINTN						i;
 	ASLR_QWORD_ASD				*res;
 	UINT64						a=0;
 	UINT64						AddressLimit=0;
 	DXE_SERVICES				*dxe;
	EFI_STATUS		    RetStatus = EFI_SUCCESS;
#if CRB_OUT_OF_RESOURCE_SUPPORT	
	UINTN                       VariableSize = sizeof (LAST_BOOT_FAILED_IOH);
	LAST_BOOT_FAILED_IOH        LastBootFailedIOH;
	EFI_STATUS                  GetSetStatus;
	UINT8                       NumberOfIoH;
	UINT8                       Index;
	UINT8                       MmioTypeIndex = 0;
	UINT8                       IoTypeIndex = 0;
	UINT8			    OutOfResourceFailure = 0xFF;
	EFI_STATUS		    MmioStatus;
	EFI_STATUS		    IoStatus;
	UINTN                       SetupDataVarSize = sizeof(SYSTEM_CONFIGURATION);
#endif

 #if BoardPciRes_SUPPORT
     AMI_OUT_OF_RES_VAR  OutOfResVar;
 //--------------------------------------

     //Check if OUT_OF_RES_VAR was already created.
     Status=AmiPciOutOfRes(&OutOfResVar, TRUE);
     if(EFI_ERROR(Status)) pBS->SetMem(&OutOfResVar,sizeof(AMI_OUT_OF_RES_VAR),0);
     else OutOfResVar.Count++;
 #endif
 	// ported for four IOHs

   // koti : TO_DO : further improvement on resources based on setup options, if one IOH requires more than other and
 	// Checking for error conditions on not to assign a more than qpi decode range.
         // Return if the Root bridge indes is greater than maximum number of CPU/IOH sockets possible on this platform.
         // On Romley CRB's the maximum is 4 with number of CPU's and IOH's present always being equal.
         //Get the maximum number of IOH's supported on this platform

 	if (RootBrgIndex > (NUMBER_CPU_SOCKETS - 1)) return EFI_UNSUPPORTED;

 	Status = pBS->LocateProtocol(
 							&gEfiIioUdsProtocolGuid,
 							NULL,
 							&mIohUds);
 	ASSERT_EFI_ERROR (Status);
 	if (EFI_ERROR(Status))
 		return Status;


 	DEBUG((DEBUG_INFO," (HbCspAllocateResources called for Rb index%d\n);", RootBrgIndex ));

 	Status=LibGetDxeSvcTbl(&dxe);
 	if(EFI_ERROR(Status)) return Status;

     //Fill out needed structures.
 	if(RootBrgData->AcpiRbRes[raIo].Min==0){ //do it only ones
     	RootBrgData->AcpiRbRes[raIo].Min=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoBase;
     	RootBrgData->AcpiRbRes[raIo].Max=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoLimit;
     	RootBrgData->AcpiRbRes[raIo].Len=RootBrgData->AcpiRbRes[raIo].Max-RootBrgData->AcpiRbRes[raIo].Min+1;
     	RootBrgData->AcpiRbRes[raIo].AddrUsed=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoLimit+1;
     	RootBrgData->AcpiRbRes[raIo].AllocType=EfiGcdAllocateMaxAddressSearchTopDown;

         RootBrgData->AcpiRbRes[raMmio32].Min=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem32Base;
         RootBrgData->AcpiRbRes[raMmio32].Max=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem32Limit;
         RootBrgData->AcpiRbRes[raMmio32].Len=RootBrgData->AcpiRbRes[raMmio32].Max-RootBrgData->AcpiRbRes[raMmio32].Min+1;
         RootBrgData->AcpiRbRes[raMmio32].AddrUsed=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem32Limit+1;
         RootBrgData->AcpiRbRes[raMmio32].AllocType=EfiGcdAllocateMaxAddressSearchTopDown;

         RootBrgData->AcpiRbRes[raMmio64].Min=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem64Base;
         RootBrgData->AcpiRbRes[raMmio64].Max=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem64Limit;
         RootBrgData->AcpiRbRes[raMmio64].Len=RootBrgData->AcpiRbRes[raMmio64].Max-RootBrgData->AcpiRbRes[raMmio64].Min+1;
         RootBrgData->AcpiRbRes[raMmio64].AddrUsed=mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem64Limit+1;
         RootBrgData->AcpiRbRes[raMmio64].AllocType=EfiGcdAllocateMaxAddressSearchTopDown;
 	}
#if CRB_OUT_OF_RESOURCE_SUPPORT

 	

 	
 	//
 	// For Automatic ratio : Very first time itself outofresource occured
 	// There won't be any LastBootFailedIOH Structure.So creating a dummy structure
	//
	 GetSetStatus = pRS->GetVariable(L"IntelSetup",
					 &gEfiSetupVariableGuid,
					 NULL,
					 &SetupDataVarSize,
					 &SystemConfiguration );

	if(EFI_ERROR(GetSetStatus)) {
		TRACE((-1,"PciRB: HbCspAllocateResources : GetVariable - IntelSetup: Failed\n"));
		return GetSetStatus;
	}

	//
	// CsiSocketResourceAutoAdjust is enabled in Setup.
	//
	if (SystemConfiguration.CsiSocketResourceAutoAdjust)
	{

		NumberOfIoH = NbGetMaximumIohSockets();
		GetSetStatus = pRS->GetVariable (L"LastBootFailedIoh",
		                       &LastBootFailedIohGuid,
		                       NULL,
		                       &VariableSize,
		                       &LastBootFailedIOH );

  		if(EFI_ERROR(GetSetStatus))
		{
    		TRACE((-1,"PciRB: IO Allocation Failed: GetVariable : First Time Creating LastBootFailedIOH Structure\n"));

			//
			//  Creating the DUMMY LastBootFailedIOH Structure
			//
    		for (Index = 0 ; Index < NumberOfIoH ; Index++)
    		{
				LastBootFailedIOH .IoOutofResrcFlag[Index] =  0xAA;
				LastBootFailedIOH .MmmioOutofResrcFlag [Index] = 0xAA;
    		}

    		GetSetStatus = pRS->SetVariable (L"LastBootFailedIoh",
									&LastBootFailedIohGuid,
									EFI_VARIABLE_NON_VOLATILE |
									EFI_VARIABLE_BOOTSERVICE_ACCESS |
									EFI_VARIABLE_RUNTIME_ACCESS,
									VariableSize,
									&LastBootFailedIOH );
    		if (EFI_ERROR(GetSetStatus))
    		{
      			TRACE((-1,"PciRB: IO Allocation Failed: SetVariable : Creating LastBootFailedIOH Structure\n"));
      			return GetSetStatus;
		}

			//
			// Set the Retry Counter as 1 for both MMIO and IO
			//

			RetryCounter.MMIO = MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR;
			RetryCounter.IO = MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR;
			Status = pRS->SetVariable (L"RetryCounterMmioIo",
							&RetryCounterMmioIoGuid,
							EFI_VARIABLE_NON_VOLATILE |
							EFI_VARIABLE_BOOTSERVICE_ACCESS |
							EFI_VARIABLE_RUNTIME_ACCESS,
							sizeof(RETRY_COUNTER_MMIO_IO),
							&RetryCounter );

			if (EFI_ERROR(GetSetStatus))
			{
				TRACE((-1,"PciRB: IO Allocation Failed: SetVariable : Creating LastBootFailedIOH Structure\n"));
				return GetSetStatus;
			}
  		}

		//
		// Filling the IohOutofResInfo Structure
		//

		IohOutofResInfo.IoBase[RootBrgIndex] = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoBase;
		IohOutofResInfo.IoLimit[RootBrgIndex] = (mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoLimit)+1;
		IohOutofResInfo.MmioBase[RootBrgIndex] = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem32Base;
		IohOutofResInfo.MmioLimit[RootBrgIndex] = (mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem32Limit)+1;
	}

#endif
 	for(i=0; i<RootBrgData->ResCount; i++){
 		res=RootBrgData->RbRes[i];
 		if(res->Type==ASLRV_SPC_TYPE_BUS) {
 			TRACE((-1, "PciRB: #%X BUS _MIN=0x%lX; _MAX=0x%lX; _LEN=0x%lX\n",
 						RootBrgIndex,res->_MIN,res->_MAX,res->_LEN, res->_GRA));
 			continue;
 		}
#if CRB_OUT_OF_RESOURCE_SUPPORT
        IohOutofResInfo.IohImageHandle[RootBrgIndex] = RootBrgData->ImageHandle;
#endif        
 		// update io and memory values from qpi rc output.
 		//Allocate IO
 		if(res->Type==ASLRV_SPC_TYPE_IO){

         TRACE((-1, "PciRB: #%X BUS _MIN=0x%lX; _MAX=0x%lX; _LEN=0x%lX\n",
 						RootBrgIndex,res->_MIN,res->_MAX,res->_LEN, res->_GRA));
 		//
         //Set Resource starting address for IO based on which RB it is.
         // Update Root Bridge with UDS resource information
         //
 	    	RootBrgData->AcpiRbRes[raIo].Gra=res->_MAX;
 			a = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoBase;
     	    AddressLimit = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoLimit;
 			AddressLimit = AddressLimit + 1; // make power of 2
#if CRB_OUT_OF_RESOURCE_SUPPORT
			//
			// CsiSocketResourceAutoAdjust is enabled in Setup.
			//
			if (SystemConfiguration.CsiSocketResourceAutoAdjust)
			{
				IohOutofResInfo.IoMax[RootBrgIndex] [IoTypeIndex]= res->_MAX;
			}
#endif			
 			TRACE((-1,"PciRB: #%X Resources Type=%X; routed by QPI: Address base = %lX; AddressLimit=%lX; \n",
 			RootBrgIndex, res->Type, a, AddressLimit));

 			TRACE((-1, "PciRB: #%X IO ",RootBrgIndex));
             //Use prefered method to allocate resources for
 			Status=dxe->AllocateIoSpace(EfiGcdAllocateMaxAddressSearchTopDown,
 							EfiGcdIoTypeIo,
 							AlignFromGranularity((UINTN)res->_MAX),
 							res->_LEN,
 							&AddressLimit,
 							RootBrgData->ImageHandle,
 							RootBrgData->RbHandle);

 			if(EFI_ERROR(Status))
 			{
 					TRACE((-1,"PciRB: IO Allocation Failed: _LEN=%lX; _GRA=l%X.\n",res->_LEN, res->_MAX));

 #if BoardPciRes_SUPPORT
           OutOfResVar.Resource=*res;
           AmiPciOutOfRes(&OutOfResVar, FALSE);
 #endif
#if CRB_OUT_OF_RESOURCE_SUPPORT           
 					if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
 					{
 						IoStatus = IoOutOfResourceConditionOccured(RootBrgIndex,&LastBootFailedIOH,&IohOutofResInfo,res->_LEN,IoTypeIndex);
 						//
 						// System will try OEM DEFINED TABLE if Status is other than Success.
 						// If Success : Best Ratio is calculated
 						if (IoStatus != EFI_SUCCESS)
 						{
 							return IoStatus;
 						}
 					}
 					//
 					// Manual Option is selected: Don't try our AutoAdjust Algorithm.
 					// Try OEM DEFINED TABLE
 					else
 					{
 						return Status;
 					}
#else
 		 	return Status;
#endif //CRB_OUT_OF_RESOURCE_SUPPORT            		 	
 			}

       // If IO Space was allocated to space not reserved for current Root Bridge, free space and return error
       if((Status == EFI_SUCCESS) && (AddressLimit < mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoBase))
 			{
 					TRACE((-1,"PciRB: IO Allocation Failed! Could not fit in Root Bridge Allotted IO Resources: AllocatedIoBase=%x, RequiredIoBase=%x, _LEN=%lX; _GRA=l%X.\n",\
                         		AddressLimit,\
                         		mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceIoBase,\
                         		res->_LEN, res->_MAX));


           dxe->FreeIoSpace(AddressLimit, res->_LEN);
           Status = EFI_NOT_FOUND;
 #if BoardPciRes_SUPPORT
           OutOfResVar.Resource=*res;
           AmiPciOutOfRes(&OutOfResVar, FALSE);
 #endif

#if CRB_OUT_OF_RESOURCE_SUPPORT           
			if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
			{
				IoStatus = IoOutOfResourceConditionOccured(RootBrgIndex,&LastBootFailedIOH,&IohOutofResInfo,res->_LEN,IoTypeIndex);
				//
				// System will try OEM DEFINED TABLE if Status is other than Success.
				// If Success : Best Ratio is calculated
				if (IoStatus != EFI_SUCCESS )
				{
						return IoStatus;
				}
			}
			//
			// Manual Option is selected: Don't try our AutoAdjust Algorithm.
			// Try OEM DEFINED TABLE
			else
			{
				return Status;
			}
#else
 		 	return Status;
#endif //CRB_OUT_OF_RESOURCE_SUPPORT            		 	
		}
#if CRB_OUT_OF_RESOURCE_SUPPORT
		//
		// Filling the IohOutofResInfo Structure even for the SUCCESS case
		//
		if (Status == EFI_SUCCESS)
		{
			if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
			{
		  		IohOutofResInfo.IoRequested[RootBrgIndex][IoTypeIndex] = res->_LEN;
			}
		}

		if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
		{
		    IoTypeIndex = IoTypeIndex + 1;
		}
#endif		
	}
         //Allocate MMIO
 		else if( res->Type==ASLRV_SPC_TYPE_MEM){
             //First root or Second one????
             if(res->_GRA==32){
                 a = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem32Base;
                 AddressLimit = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem32Limit;
 								AddressLimit = AddressLimit + 1; // make power of 2
 								TRACE((-1, "PciRB: #%X MMIO32",RootBrgIndex));
 		        		RootBrgData->AcpiRbRes[raMmio32].Gra=res->_MAX;
#if CRB_OUT_OF_RESOURCE_SUPPORT 		        		
 				//
 				// CsiSocketResourceAutoAdjust is enabled in Setup.
 				//
 				if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
 				{
 					IohOutofResInfo.MmioMax[RootBrgIndex] [MmioTypeIndex] = res->_MAX;
 				}
#endif 				


           } else {
                 a = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem64Base;
                 AddressLimit = mIohUds->IioUdsPtr->PlatformData.IIO_resource[RootBrgIndex].PciResourceMem64Limit;
                 AddressLimit = AddressLimit + 1; // make power of 2
 								TRACE((-1, "PciRB: #%X MMIO64",RootBrgIndex));
 		        		RootBrgData->AcpiRbRes[raMmio64].Gra=res->_MAX;
 			}

 			TRACE((-1,"PciRB: #%X Resources Type=%X; routed by QPI: _GRA=%lX; Address base = %lX; AddressLimit=%lX; \n",
 			RootBrgIndex, res->Type, res->_GRA, a, AddressLimit));

 			Status=dxe->AllocateMemorySpace(EfiGcdAllocateMaxAddressSearchTopDown,
 							EfiGcdMemoryTypeMemoryMappedIo,
 							AlignFromGranularity((UINTN)res->_MAX),
 							res->_LEN,
 							&AddressLimit,
 							RootBrgData->ImageHandle,
 							RootBrgData->RbHandle);

 			if(EFI_ERROR(Status))
 			{
 					TRACE((-1,"PciRootBrg: Memory Allocation Failed: Length: %lX\n",res->_LEN));

 #if BoardPciRes_SUPPORT
                 OutOfResVar.Resource=*res;
                 AmiPciOutOfRes(&OutOfResVar, FALSE);
 #endif
                 
#if CRB_OUT_OF_RESOURCE_SUPPORT                 
 					if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
 					{
 						MmioStatus = MmioOutOfResourceConditionOccured(RootBrgIndex,&LastBootFailedIOH,&IohOutofResInfo,res->_LEN,MmioTypeIndex);
 						//
 						// System will try OEM DEFINED TABLE if Status is other than Success.
 						// If Success : Best Ratio is calculated
 						if (MmioStatus != EFI_SUCCESS)
 						{
 							return MmioStatus;
 						}
 					}
 					//
 					// Manual Option is selected: Don't try our AutoAdjust Algorithm.
 					// Try OEM DEFINED TABLE
 					else
 					{
 						return Status;
 					}
#else
 		 	return Status;
#endif //CRB_OUT_OF_RESOURCE_SUPPORT            		 	
 			}


       if((Status == EFI_SUCCESS )&& (AddressLimit < a))
       {
 					TRACE((-1,"PciRB: MMIO Allocation Failed! Could not fit in Root Bridge Allotted MMIO Resources: AllocatedMmioBase=%x, RequiredMmioBase=%x, _LEN=%lX; _GRA=l%X.\n",\
       		                  AddressLimit,\
         		                a,\
           		              res->_LEN, res->_MAX));
           dxe->FreeMemorySpace(AddressLimit, res->_LEN);
           Status = EFI_NOT_FOUND;
 #if BoardPciRes_SUPPORT
                 OutOfResVar.Resource=*res;
                 AmiPciOutOfRes(&OutOfResVar, FALSE);
 #endif

#if CRB_OUT_OF_RESOURCE_SUPPORT
                 if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
                 {
 					MmioStatus = MmioOutOfResourceConditionOccured(RootBrgIndex,&LastBootFailedIOH,&IohOutofResInfo,res->_LEN,MmioTypeIndex);
 					//
 					// System will try OEM DEFINED TABLE if Status is other than Success.
 					// If Success : Best Ratio is calculated
 					if (MmioStatus != EFI_SUCCESS)
 					{
 							return MmioStatus;
 					}
                 }
                 //
                 // Manual Option is selected: Don't try our AutoAdjust Algorithm.
                 // Try OEM DEFINED TABLE
                 else
                 {
 					return Status;
                 }
#else
 		 	return Status;
#endif //CRB_OUT_OF_RESOURCE_SUPPORT            		 	
        }


   	    	//
  			// Filling the IohOutofResInfo Structure even for the SUCCESS case
 			//
 			if (Status == EFI_SUCCESS)
 			{
#if CRB_OUT_OF_RESOURCE_SUPPORT 				
					if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
					{
						IohOutofResInfo.MmioRequested[RootBrgIndex][MmioTypeIndex] = res->_LEN;
					}
#endif					
 					//Set this region as WT cache if it is PREFETCHABLE
 					if(res->TFlags.MEM_FLAGS._MEM!=ASLRV_MEM_UC)
 					{
 							TRACE((-1,"P"));
 							Status=dxe->SetMemorySpaceAttributes(AddressLimit,res->_LEN,EFI_MEMORY_WT);
 							//if attempt to set WT attributes has filed, let's try UC
 							if(EFI_ERROR(Status))
 							{
 									TRACE((-1,"PciHostBridge: Setting of WT attributes for prefetchable memory has failed(%r). UC is used.\n",Status));
 									Status=dxe->SetMemorySpaceAttributes(AddressLimit,res->_LEN,EFI_MEMORY_UC);
 							}
 					}
 					else Status=dxe->SetMemorySpaceAttributes(AddressLimit,res->_LEN,EFI_MEMORY_UC);
 							ASSERT_EFI_ERROR(Status);
 					if(EFI_ERROR(Status)) return Status;
 			}

#if CRB_OUT_OF_RESOURCE_SUPPORT      
		if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
		{
			MmioTypeIndex = MmioTypeIndex + 1;
		}
#endif		
	}



     	if (Status == EFI_SUCCESS)
 		    	res->_MIN = AddressLimit;

 //		TRACE((-1,"PciRB: #%X Resources: Type=%X; TFlags=%X; GCD asssigned address _MIN=%lX; _MAX=%lX; _GRA=%lX; _LEN=%lX.\n",
 //        RootBrgIndex, res->Type, res->TFlags.TFLAGS,res->_MIN, res->_MAX, res->_GRA, res->_LEN ));
 			TRACE((-1, ": _MIN=0x%lX; _MAX=0x%lX; _LEN=0x%lX; _GRA=0x%lX\n",
 			res->_MIN,res->_MAX,res->_LEN, res->_GRA ));

 	    //No need to change this and lose Granularity Value... will preserve it.
 		//res->_MAX=res->_MIN+res->_LEN-1;
 	}


 //PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING//
 //PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING//

#if CRB_OUT_OF_RESOURCE_SUPPORT
	//
	// CsiSocketResourceAutoAdjust is enabled in Setup.
	//
	if (SystemConfiguration.CsiSocketResourceAutoAdjust) // CsiSocketResourceAutoAdjust is enabled in Setup.
	{
		TRACE((-1,":: RootBrgIndex=%x NumberOfIoH=%x\n",RootBrgIndex,NumberOfIoH));
		if (NumberOfIoH > 1)
		{
			if (RootBrgIndex != (NumberOfIoH-1))
				return EFI_SUCCESS;// Status
		}
		//
		// Check whether both IO and MMIO allocation phase are crossed for all IOH's
		// We parsed all IOH and going to check whether OutOfResource Flag for IO/MMIO is set
		// And for which IOH ?
		for ( i = 0 ; i < NumberOfIoH ; i++)
		{
			if ((IohOutofResInfo.IoOutofResrcFlag [i] != 0) || (IohOutofResInfo.MmmioOutofResrcFlag [i] != 0))
				break;
			else
				continue;
		}

		//
		// If i == NumberOfIoH then there is no OutOfResource occured
		//
		if (i == NumberOfIoH)
		{
			// System Booted Successfully so setting the default counter value again
			// Set the Retry Counter as 1 for both MMIO and IO
			//

			RetryCounter.MMIO = MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR;
			RetryCounter.IO = MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR;
			Status = pRS->SetVariable (L"RetryCounterMmioIo",
	                                 &RetryCounterMmioIoGuid,
	                                 EFI_VARIABLE_NON_VOLATILE |
	                                 EFI_VARIABLE_BOOTSERVICE_ACCESS |
	                                 EFI_VARIABLE_RUNTIME_ACCESS,
	                                 sizeof(RETRY_COUNTER_MMIO_IO),
	                                 &RetryCounter );

			if (EFI_ERROR(GetSetStatus))
			{
				TRACE((-1,"SetVariable : MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR\n"));
				return GetSetStatus;
			}
			TRACE((-1,"System booted with MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR method\n"));
			TRACE((-1,"Setting the Default counter value again %d\n",MAX_SYSTEM_RESET_RETRYCOUNT_ON_RESOURCE_ERROR));
			return EFI_SUCCESS;
		}

		TRACE((-1,"PciRB: OutOfResource Condition Occured: \n"));

		//
		// For IO check
		//
		for ( i = 0 ; i < NumberOfIoH ; i++)
		{
			if (IohOutofResInfo.IoOutofResrcFlag [i] == 1)
			{
				TRACE((-1,"PciRB: IO : OutOfResource Condition Occured in IOh: %X.\n",i));
				OutOfResourceFailure = IOFAILURE;
				if (AutoAdjustRatios(/*SystemConfiguration,*/OutOfResourceFailure,dxe) == 1)
				{
					TRACE ((-1, "Requested IO space is not possible!  System is going to Reset and try executing the OEM Defined Table to boot!\n"));
					ResetRequired = TRUE;
				}
				break;
			}
		}

		//
		// For MMIO check
		//
		for ( i = 0 ; i < NumberOfIoH ; i++)
		{
			if (IohOutofResInfo.MmmioOutofResrcFlag [i] == 1)
			{
				TRACE((-1,"PciRB: MMIO : OutOfResource Condition Occured in IOh: %X.\n",i));
				OutOfResourceFailure = MMIOFAILURE;
				if (AutoAdjustRatios(/*SystemConfiguration,*/OutOfResourceFailure,dxe) == 1)
				{
					TRACE ((-1, "Requested MMIO space is not possible!  System is going to Reset and try executing the OEM Defined Table to boot!\n"));
					ResetRequired = TRUE;
				}
				break;
			}
		}

		//
		// System is going to Reset to try best ratio or OEM DEFINED table
		//
		if (ResetRequired == TRUE)
		{
			pRS->ResetSystem( EfiResetCold, 0, 0, NULL );
		}
	}
#endif
     return Status;
}

//----------------------------------------------------------------------------
// Following functions are ROOT BRIDGE Infrastructure Overrides and Porting.
//----------------------------------------------------------------------------

/**
    Chipset Specific function to Map Internal Device address
    residing ABOVE 4G to the BELOW 4G address space for DMA.
    MUST BE IMPLEMENTED if CHIPSET supports address space
    decoding ABOVE 4G.

    @param RbData Root Bridge private structure data
    @param Operation Operation to provide Mapping for
    @param HostAddress HostAddress of the Device
    @param NumberOfBytes Number of Byte in Mapped Buffer.
    @param DeviceAddress Mapped Device Address.
    @param Mapping Mapping Info Structure this function must
        allocate and fill.

        EFI_STATUS
    @retval EFI_SUCCESS Successful.
    @retval EFI_UNSUPPORTED The Map function is not supported.
    @retval EFI_INVALID_PARAMETER One of the parameters has an
        invalid value.

    @note  Porting is required for chipsets that supports Decoding
              of the PCI Address Space ABOVE 4G.
**/
EFI_STATUS RbCspIoPciMap (
    IN PCI_ROOT_BRG_DATA                            *RbData,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION    Operation,
    IN EFI_PHYSICAL_ADDRESS                         HostAddress,
    IN OUT UINTN                                    *NumberOfBytes,
    OUT EFI_PHYSICAL_ADDRESS                        *DeviceAddress,
    OUT VOID                                        **Mapping )
{

    EFI_STATUS  Status = EFI_UNSUPPORTED;

    // Any Additional Variables goes here

    *Mapping = NULL;

    // For Chipsets which DOES support decoding of the PCI resources ABOVE 4G.
	// But CPU architecture DOES NOT not (like 32bit mode)
    // here must be something like that.

//###DEBUG
//###     PCI_ROOT_BRIDGE_MAPPING *mapping;
//###     //------------------------------
//###         // Common buffer operations can not be remapped because in such
//###         // operations the same buffer will be accessed by CPU and PCI hardware
//###         if ( (Operation == EfiPciOperationBusMasterCommonBuffer) || 
//###              (Operation == EfiPciOperationBusMasterCommonBuffer64) )
//###             return EFI_UNSUPPORTED;
//###
//###         mapping = Malloc(sizeof(PCI_ROOT_BRIDGE_MAPPING));
//###         if (mapping == NULL) return EFI_OUT_OF_RESOURCES;
//###
//###         mapping->Signature  = EFI_PCI_RB_MAPPING_SIGNATURE;
//###         mapping->Resrved    = 0;
//###         mapping->Operation  = Operation;
//###         mapping->NumPages   = EFI_SIZE_TO_PAGES(*NumberOfBytes);
//###         mapping->HostAddr   = HostAddress;
//###         mapping->DeviceAddr = 0x00000000ffffffff;
//###
//###         Status = pBS->AllocatePages( AllocateMaxAddress, 
//###                                      EfiBootServicesData, 
//###                                      mapping->NumPages, 
//###                                      &mapping->DeviceAddr );
//###         if (EFI_ERROR(Status)) {
//###             pBS->FreePool(mapping);
//###             return Status;
//###         }
//###
//###         *Mapping = (VOID*)mapping;
//###
//###         // Here must be a way to copy context of HostDevice buffer to the
//###         // Mapped one.
//###         // This code given as example only you might need to do some chipset
//###         // programming to access PCI Address Space Above 4G
//###
//###         if ( (Operation == EfiPciOperationBusMasterRead) || 
//###              (Operation == EfiPciOperationBusMasterRead64) )
//###             pBS->CopyMem( (VOID*)(UINTN)mapping->DeviceAddr, 
//###                           (VOID*)(UINTN)mapping->HostAddr, 
//###                           mapping->NumBytes );
//###
//###         *DeviceAddress = mapping->DeviceAddr;
//### DEBUG END

    return Status;
}


/**
    Chipset Specific function to Unmap previousely Mapped
    buffer for DMA.
    MUST BE IMPLEMENTED if CHIPSET supports address space
    decoding ABOVE 4G.

    @param RbData Root Bridge private structure data
    @param Mapping Mapping Info Structure this function must free.

        EFI_STATUS
    @retval EFI_SUCCESS Successful.
    @retval EFI_UNSUPPORTED The Unmap function is not supported.
    @retval EFI_INVALID_PARAMETER One of the parameters has an
        invalid value.

    @note  Porting required if needed.
**/
EFI_STATUS RbCspIoPciUnmap (
    IN PCI_ROOT_BRG_DATA                            *RbData,
    OUT PCI_ROOT_BRIDGE_MAPPING                     *Mapping )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here


    if (Mapping != NULL) return EFI_INVALID_PARAMETER;
    // for all other conditions we would return EFI_UNSUPPORTED.
    Status = EFI_UNSUPPORTED;

    // for Chipsets which DOES support decoding of the PCI resources ABOVE 4G.
	// But CPU architecture DOES NOT not (like 32bit mode)
    // And provides corresponded mapping for the host address
    // here must be something like that.
//####  DEBUG
//####      if (Mapping->Signature != EFI_PCI_RB_MAPPING_SIGNATURE)
//####          Status = EFI_INVALID_PARAMERTER;
//####
//####      if (!EFI_ERROR(Status)) {
//####
//####          if ( (Mapping->Operation == EfiPciOperationBusMasterWrite) || 
//####               (Mapping->Operation == EfiPciOperationBusMasterWrite64) )
//####
//####          // Here must be a way to copy context of the Unmapped buffer to
//####          // HostDevice.
//####          // This code given as example only you might need to do some
//####          // chipset programming to access PCI Address Space Above 4G
//####          pBS->CopyMem( (VOID*)(UINTN)Mapping->HostAddr, 
//####                        (VOID*)(UINTN)Mapping->DeviceAddr, 
//####                        Mapping->NumBytes );
//####
//####          pBS->FreePages(Mapping->DeviceAddr, Mapping->NumPages);
//####          pBS->FreePool(Mapping);
//####      }
//####  DEBUG

    return Status;
}

/**
    Chipset Specific function to do PCI RB Attributes releated
    programming.

    @param RbData Pointer to Root Bridge private structure.
    @param Attributes The Root Bridge attributes to be programming.
    @param ResourceBase Pointer to the resource base. (OPTIONAL)
    @param ResourceLength Pointer to the resource Length. (OPTIONAL)

        EFI_STATUS
    @retval EFI_SUCCESS Successful.
    @retval EFI_INVALID_PARAMETER One of the parameters has an
        invalid value.
**/
EFI_STATUS RbCspIoPciAttributes (
    IN PCI_ROOT_BRG_DATA                            *RbData,
    IN UINT64                                       Attributes,
    IN OUT UINT64                                   *ResourceBase OPTIONAL,
    IN OUT UINT64                                   *ResourceLength OPTIONAL )
{


    EFI_STATUS  Status = EFI_SUCCESS;

    // Any Additional Variables goes here

    return Status;
}

/**
    Read Pci Registers into buffer.
    Csp Function which actualy access PCI Config Space.
    Chipsets that capable of having PCI EXPRESS Ext Cfg Space
    transactions.
    Must Implement this access routine here.

    @param RbData Root Bridge private structure.
    @param Width PCI Width.
    @param Address PCI Address.
    @param Count Number of width reads/writes.
    @param Buffer Buffer where read/written.
    @param Write Set if write.

        EFI_STATUS
    @retval EFI_SUCCESS Successful read.
    @retval EFI_INVALID_PARAMETER One of the parameters has an
**/
EFI_STATUS RootBridgeIoPciRW (
    IN VOID                                     *RbData,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    Width,
    IN UINT64                                   Address,
    IN UINTN                                    Count,
    IN OUT VOID                                 *Buffer,
    IN BOOLEAN                                  Write )
{
    static UINT64 *MultiSegPcieBaseAddressArray = NULL;
    static UINT32 MaxSegment = 1;
    
    BOOLEAN ValidCfg = TRUE;
//    UINT64 PciExpressBaseAddress = 0;
    UINT8   IncrementValue = 1 << (Width & 3); // 1st 2 bits currently define
                                               // width.
                                               // Other bits define type.

    
    
    //UINTN  PciAddress = PCIEX_BASE_ADDRESS + 
    //Upper part will be added inside the library call
    UINTN  PciAddress = (((EFI_PCI_CONFIGURATION_ADDRESS*) &Address)->Bus << 20 ) + \
        (((EFI_PCI_CONFIGURATION_ADDRESS*) &Address)->Device << 15 ) + \
        (((EFI_PCI_CONFIGURATION_ADDRESS*) &Address)->Function << 12 );

    PciAddress += \
        ((EFI_PCI_CONFIGURATION_ADDRESS*) &Address)->ExtendedRegister ? \
            ((EFI_PCI_CONFIGURATION_ADDRESS*) &Address)->ExtendedRegister : \
            ((EFI_PCI_CONFIGURATION_ADDRESS*) &Address)->Register;

    // To read 64bit values, reduce Increment Value (by half) and
    // double the count value (by twice)
    if (IncrementValue > 4) {
        IncrementValue >>= 1;
        Count <<= 1;
    }

    if (Width >= EfiPciWidthMaximum || IncrementValue > 4)
        return EFI_INVALID_PARAMETER;
/*wkh
    // If MultiSegment Array pointer is NULL, Get it from PCD.
    if(MultiSegPcieBaseAddressArray == NULL){
        MultiSegPcieBaseAddressArray = PcdGetPtr(AmiPcdPcieBaseAddressBuffer);
        MaxSegment = PcdGet32(AmiPcdMaxSegmentSupported);
        ASSERT(MultiSegPcieBaseAddressArray!=NULL);
        ASSERT(MaxSegment>=1);
        if (MultiSegPcieBaseAddressArray==NULL || MaxSegment<1){
            MultiSegPcieBaseAddressArray=NULL;
            return EFI_UNSUPPORTED;
        }
        // If Segment 0 base address is 0, we're dealing with a single-segment system.
        // Get PCI-E base address from PCD.
        if(MultiSegPcieBaseAddressArray[0] == 0){
        //AptioV Server Override start: Dynamic mmcfg base address change
        //            MultiSegPcieBaseAddressArray[0] = PcdGet64(PcdPciExpressBaseAddress);
#ifndef DYNAMIC_MMCFG_BASE_FLAG
    MultiSegPcieBaseAddressArray[0] = (UINTN)PcdGet64 (PcdPciExpressBaseAddress);
#else
   MultiSegPcieBaseAddressArray[0] = (UINTN) GetPciExpressBaseAddress ();
#endif
    //AptioV Server Override end: Dynamic mmcfg base address change	
	

        }
    }

    ASSERT(MaxSegment > ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL*)RbData)->SegmentNumber);
    if (MaxSegment <= ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL*)RbData)->SegmentNumber){
        return EFI_UNSUPPORTED;
    }
    // MultiSegment with Valid offset.Fetch value
    PciExpressBaseAddress = MultiSegPcieBaseAddressArray[((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL*)RbData)->SegmentNumber];
wkh*/
    while (Count--) {
		//Check if Access address falls into PCIExpress Config Space range
	   //AptioV Server Override start: Dynamic mmcfg base address change		
	    #ifdef DYNAMIC_MMCFG_BASE_FLAG
	  //  DEBUG((DEBUG_INFO,"GetPciExpressBaseAddress: GetPciExpressBaseAddress(GetPciExpressBaseAddress)\n"));
	    if (((UINTN) GetPciExpressBaseAddress () + PciAddress) >= ((UINTN) GetPciExpressBaseAddress ()+ PCIEX_LENGTH))  
        #else
        	  if (((UINTN)PcdGet64 (PcdPciExpressBaseAddress)+ PciAddress) >= ((UINTN)PcdGet64 (PcdPciExpressBaseAddress)+ PCIEX_LENGTH)) 
			
        #endif
		   ValidCfg = FALSE;
       //AptioV Server Override end: Dynamic mmcfg base address change
        //Range check and and Number of Buses checked on PCI Bus Driver level.
//        if (PciAddress & 0xF0000000) ValidCfg = FALSE;
        if (Write) {
            switch(IncrementValue) {
                case 1:
                    if (ValidCfg) //*(UINT8*)PciAddress = *(UINT8*)Buffer;
                    	PciWrite8(PciAddress, *(UINT8*)Buffer);
                    break;
                case 2:
                    if (ValidCfg) //*(UINT16*)PciAddress = *(UINT16*)Buffer;
                    	PciWrite16(PciAddress, *(UINT16*)Buffer);
                    break;
                default:
                    if (ValidCfg) //*(UINT32*)PciAddress = *(UINT32*)Buffer;
                    	PciWrite32(PciAddress, *(UINT32*)Buffer);
                    break;
            }
        } else {
            switch(IncrementValue) {
                case 1:
                    //*(UINT8*)Buffer = (ValidCfg) ? *(UINT8*)PciAddress : -1;
                	*(UINT8*)Buffer = (ValidCfg) ? PciRead8(PciAddress) : -1;
                    break;
                case 2:
                    //*(UINT16*)Buffer = (ValidCfg) ? *(UINT16*)PciAddress : -1;
                	*(UINT16*)Buffer = (ValidCfg) ? PciRead16(PciAddress) : -1;
                    break;
                default:
                    //*(UINT32*)Buffer = (ValidCfg) ? *(UINT32*)PciAddress : -1;
                	*(UINT32*)Buffer = (ValidCfg) ? PciRead32(PciAddress) : -1;
                    break;
            }
        }

        if (Width <= EfiPciWidthFifoUint64) {
             Buffer = ((UINT8 *)Buffer + IncrementValue);
            // Buffer is increased for only EfiPciWidthUintxx and
            // EfiPciWidthFifoUintxx
        }

        // Only increment the PCI address if Width is not a FIFO.
        if ((Width & 4) == 0) {
            PciAddress += IncrementValue;
        }
    }

    return EFI_SUCCESS;
}

