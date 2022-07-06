//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiPciBusLib.h
    Library Class for AMI PCI Bus And AMI Root Bridge common routines.

**/
//*************************************************************************
#ifndef _AMI_PCI_BUS_COMMON_LIB_H_
#define _AMI_PCI_BUS_COMMON_LIB_H_

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Token.h>
#include <PciBus.h>
#include <Pci.h> 
#include <PciE.h>
#include <AmiDxeLib.h>
#include <Protocol/AmiBoardInfo2.h>

//-------------------------------------------------------------------------
/// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
//PCI Bus Porting Constants definitions
extern const UINT16 NbDmiL0ExitLatency;
extern const UINT16 NbDmiL1ExitLatency;
extern const UINT16 SbDmiL0ExitLatency;
extern const UINT16 SbDmiL1ExitLatency;
extern const UINT16 NbDmiAspmSupport;
extern const UINT16 SbDmiAspmSupport;
extern const UINT16 SbInternalDelay;

//----------------------------------------------------------------------------------
//PCI Bus Configuration Constants definitions
//const BOOLEAN S3VideoRepost;
extern const BOOLEAN FixedBusAssign;
extern const BOOLEAN DecodeFullBusRanges;
extern const UINT8 PciRserveUncoreBuses;
extern const BOOLEAN HotPlugSupport;
extern const BOOLEAN ApplyPaddingAnyway;
extern const BOOLEAN CombineMemPfMem32;
extern const BOOLEAN CombineMem64PfMem;
extern const BOOLEAN IgnoreOptionRom;
extern const BOOLEAN Io32support;
extern const BOOLEAN UsePciIoForOptRom;

extern const UINT32 CrdBusPaddingMmioLen;
extern const UINT32 CrdBusPaddingMmioGra;
extern const UINT32 CrdBusPaddingIoLen;
extern const UINT32 CrdBusPaddingIoGra;

extern const BOOLEAN PcieCrsSupport;
extern const BOOLEAN PciRbCombineMemPmemReset;
extern const BOOLEAN DeviceClassPolicyOverride;
extern const UINT8   PolicyOverrideMode;
extern const BOOLEAN LaunchCommonRoutineFirst;
extern const BOOLEAN DontUpdateRbResAsl;
extern const BOOLEAN DontUpdatePebs;
extern const BOOLEAN PciWriteVidDid;
extern const BOOLEAN DontUpdateRbUidAsl;

//-------------------------------------------------------------------------
//Variable, Prototype, and External Declarations
//-------------------------------------------------------------------------

extern BOOLEAN     gNoSdlDataMode;


/**
    Fills gPciBusDb Array in ascending  order.

    @param SdlData Pointer to the Pci device information.
    @param BusDb Pointer to the AMI_SIO resource list

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.

**/

EFI_STATUS  AmiPciAddBusDbEntry(AMI_SDL_PCI_DEV_INFO *SdlData, T_ITEM_LIST *BusDb);

/**
    Fills gPciBusDb Array in ascending  order.

    @param SdlData Pointer to the Pci device information.
    @param BusDb Pointer to the AMI_SIO resource list.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.

**/
EFI_STATUS  AmiPciAddDevDbEntry(AMI_SDL_PCI_DEV_INFO *SdlData, T_ITEM_LIST *BusDb);

/**
    Fills gFixedResBarList Array in Descending  order.

    @param bar Pointer at PCI BAR register information
    @param BarDb Pointer to the AMI_SIO resource list
    @param Fixed TRUE/FALSE.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS  AmiPciAddBarDbEntry(PCI_BAR *Bar, T_ITEM_LIST *BarDb, BOOLEAN Fixed);

/**
    Will do PCI Configuration Space Access 8;16;32;64 bit width

    @param RbIo Pointer to PciRootBridgeIO Protocol.
    @param addr PCI_CFG_ADDR filled by caller
    @param wr TRUE = Write FALSE = Read
    @param buff Pointer Data Buffer.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_NOT_FOUND           When Device not present in the system.
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

 Referals:PCI_CFG_ADDR

**/
EFI_STATUS PciCfg8(EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *RbIo, PCI_CFG_ADDR addr, BOOLEAN wr, UINT8 *buff);

/**
    Will do PCI Configuration Space Access 16 bit width

    @param RbIo Pointer to PciRootBridgeIO Protocol.
    @param addr PCI_CFG_ADDR filled by caller
    @param wr TRUE = Write FALSE = Read
    @param buff Pointer Data Buffer.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_NOT_FOUND           When Device not present in the system.
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

 Referals:PCI_CFG_ADDR

**/
EFI_STATUS PciCfg16(EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *RbIo, PCI_CFG_ADDR addr, BOOLEAN wr, UINT16 *buff);

/**
    Will do PCI Configuration Space Access 32 bit width

    @param RbIo Pointer to PciRootBridgeIO Protocol.
    @param addr PCI_CFG_ADDR filled by caller
    @param wr TRUE = Write FALSE = Read
    @param buff Pointer Data Buffer.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_NOT_FOUND           When Device not present in the system.
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

 Referals:PCI_CFG_ADDR

**/
EFI_STATUS PciCfg32(EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *RbIo, PCI_CFG_ADDR addr, BOOLEAN wr, UINT32 *buff);

/**
    Will do PCI Configuration Space Access 64 bit width

    @param RbIo Pointer to PciRootBridgeIO Protocol.
    @param addr PCI_CFG_ADDR filled by caller
    @param wr TRUE = Write FALSE = Read
    @param buff Pointer Data Buffer.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_NOT_FOUND           When Device not present in the system.
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

 Referals:PCI_CFG_ADDR

**/
EFI_STATUS PciCfg64(EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *RbIo, PCI_CFG_ADDR addr, BOOLEAN wr, UINT64 *buff);

/**
    Checks if PCI_DEV_INFO is Function 0 of PCI device.

    @param Device Pointer to PCI Device Private Data structure.

    @retval BOOLEAN
        TRUE    Device is Function 0 of Multifunctional device.
        FALSE   Device is not Function 0 of Multifunctional device.

**/
BOOLEAN IsFunc0(PCI_DEV_INFO *Device );

/**
    Checks if PCI_DEV_INFO data passed belongs to Function 0 of
    Multi-Functional device.

    @param Device Pointer to PCI Device Private Data structure.

    @retval BOOLEAN
        TRUE    Device is Function 0 of Multifunctional device.
        FALSE   Device is not Function 0 of Multifunctional device.

**/
BOOLEAN IsFunc0OfMfDev(PCI_DEV_INFO *Device );

/**
    Checks if PCI_DEV_INFO data passed belongs to other than Func0 of
    Multi-Functional device.

    @param Device Pointer to PCI Device Private Data structure.

    @retval BOOLEAN
        TRUE    Device is Function 0 of Multifunctional device.
        FALSE   Device is not Function 0 of Multifunctional device.

**/
BOOLEAN IsNotFunc0OfMfDev(PCI_DEV_INFO *Device );

/**
    Creates a copy of T_ITEM_LST structure.

    @param Lst Pointer to the structure to copy.
    @param NewLstPtr Double Pointer to the copied data (Memory allocation is done by this function).

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_OUT_OF_RESOURCES    When system runs out of resources.

**/
EFI_STATUS CpyItemLst(T_ITEM_LIST *Lst, T_ITEM_LIST **NewLstPtr);

/**
    Program SubordinateBusNumber Register of PCI Bridge.

    @param Brg Pointer to PCI Bridge Private Data structure.
    @param SubBusNo Number to program.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.

 Referrals: PCI_BRG_INFO.

**/
EFI_STATUS SetSubBus(PCI_DEV_INFO *Brg, UINT8 SubBusNo);


/**
    This function will Reprogram Primary Secondary and Subordinate
    bus numbers for the downstream bridges after SEC BUS reset signal assertion.
 
    @param DnPort Pointer to PCI Device Private Data structure.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS RestoreBridgeBuses(PCI_DEV_INFO *DnPort);


/**
    Maps Bridge's Primary Secondary Subordinate Bus Numbers
    according information stored in PCI_DEV_INFO and PCI_BRG_EXT structures
    of the PCI Bridge.

    @param Brg Pointer to PCI Bridge Private Data structure.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.

     Referrals: PCI_DEV_INFO; PCI_BRG_INFO; PCI_BRG_EXT.

    @note  
      BaseBus         =   PCI_DEV_INFO->Address.Addr.Bus;
      SecondaryBus    =   PCI_BRG_EXT->Res[rtBus].Base;
      SubordinateBus  =   PCI_BRG_EXT->Res[rtBus].Base + PCI_BRG_EXT->Res[rtBus].Length-1;

**/
EFI_STATUS MapBridgeBuses(PCI_DEV_INFO *Brg);

/**
    Clears Bridge's Primary, Secondary and Subordinate Bus Numbers.

        
    @param Brg Pointer to PCI Bridge Private Data structure.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.

 Referrals: PCI_DEV_INFO; PCI_BRG_INFO; PCI_BRG_EXT.

**/
EFI_STATUS ClearBridgeBuses(PCI_DEV_INFO *Brg);


/**
    Checks if value passed makes "POWER OF TWO"

    @param Value Value to check.

    @retval BOOLEAN
        TRUE or FALSE based on value passed.
**/
BOOLEAN IsPowerOfTwo(UINT64 Value);


/**
    Clears/zeros contents of PCI_BAR structure.

    @param Bar Pointer to PCI_BAR structure to clear.

    @retval Nothing

**/
VOID ClearBar(PCI_BAR *Bar);


/**
    This function will parse PCI SDL Database looking for HostBridge
    entries  and populate gRootBusDb Structure.

    @param RbSdlData Pointer to the Pci device information.
    @param SdlDataCount count of Sdl Data 

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS is OK


    @note  CHIPSET AND/OR BOARD PORTING NEEDED
**/
EFI_STATUS SortRbSdlData(AMI_SDL_PCI_DEV_INFO ***RbSdlData, UINTN *SdlDataCount);

/**
    Finds Root bus number.
    
    @param Device Pointer to PCI Device Private Data structure.
    
    @retval UINT8

**/
UINT8 FindRootBus(PCI_DEV_INFO *Device);

/**
    Porting function which collects all PCI Bus Driver specific
    SETUP configuration data and returns it to the PCI Bus Driver.

        
    @param PciSetupData Pointer to the Pci Bus specific setup data buffer.
    @param PciCommon Pointer to the Pci Bus common setup data buffer.
    @param HpSetup Pointer to the Pci hot plug setup data.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus. 
**/
#if (PCI_SETUP_USE_APTIO_4_STYLE == 1)
EFI_STATUS AmiPciGetSetupData(  PCI_SETUP_DATA             *PciSetupData, 
                                PCI_COMMON_SETUP_DATA     *PciCommon,
                                PCI_HOTPLUG_SETUP_DATA     *HpSetup);
#else
EFI_STATUS AmiPciGetSetupData(  PCI_COMMON_SETUP_DATA    *PciCommon,
                                PCI_DEVICE_SETUP_DATA    *PciDev, 
                                PCIE1_SETUP_DATA         *Pcie1,
                                PCIE2_SETUP_DATA         *Pcie2,
                                AMI_SDL_PCI_DEV_INFO     *DevSdlInfo,    //OPTIONAL if ==NULL get defaults...
                                UINTN                    DevIdx);        //OPTIONAL 
#endif

/**
    This is a routine to launch Init Function

    @param InitStep Steps of pci init Routine
    @param InitType type of the pci init Routine.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus. 

**/
EFI_STATUS AmiPciLibLaunchInitRoutine(VOID *RoutineOwner, PCI_INIT_STEP InitStep, PCI_INIT_TYPE InitType,
        VOID *Param1, VOID *Param2, VOID *Param3, VOID *Param4);
/**
    This function will check pci bus If present.

    @param Dev Pointer to PCI Device Private Data structure.
    @param DevFnData Pointer to the Pci device information.

    @retval BOOLEAN TRUE/FALSE

**/
BOOLEAN PciBusCheckIfPresent(PCI_DEV_INFO *Dev, AMI_SDL_PCI_DEV_INFO *DevFnData);

/**
    Reads Option Rom file. If it indicated  .


    @param Device Pointer to PCI Device Private Data structure.
    @param OpRomNumber number of Option Rom. 
    @param ThisRomSdlData Pointer to the Pci device information.
    @param RomSize Size of RomImage in bytes.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_END_OF_MEDIA        When Search reaches last container.
        EFI_INVALID_PARAMETER   When some of the parameters are invalid.

**/
EFI_STATUS PciBusReadNextEmbeddedRom(    PCI_DEV_INFO *Device, UINTN OpRomNumber, //IN
                                         AMI_SDL_PCI_DEV_INFO **ThisRomSdlData, VOID **RomFile, UINTN *RomSize); //OUT

/**
    This function will collect information about
    global setup variable
    
    @param PciCommon Pointer to the Pci Bus common setup data buffer.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus. 
**/
EFI_STATUS AmiPciGetCommonSetupData(PCI_COMMON_SETUP_DATA     *PciCommon);

/**
    This function will collect information about
    Device Conventional PCI setup data variable.
    
    @param PciDev Pointer to the General Pci related setup data buffer.
    @param DevSdlInfo Pointer to the Pci device information.
    @param Set  BOOLEAN: TRUE/FALSE
           TRUE -Set Pci Setup Data variable.
           FALSE-Get Pci Setup Data variable    

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus. 
**/
EFI_STATUS AmiPciGetPciDevSetupData(PCI_DEVICE_SETUP_DATA   *PciDev,
                                    AMI_SDL_PCI_DEV_INFO    *DevSdlInfo,//OPTIONAL if == NULL get defaults...
                                    UINTN                   DevIdx,
                                    BOOLEAN                 Set);

/**
    This function will collect information about
    PCI Express GEN 1 Device setup data variable.

    @param Pcie1 Pointer to the GEN 1 Pci related setup data buffer.
    @param DevSdlInfo Pointer to the Pci device information.
    @param DevIdx string ID.
    @param Set  BOOLEAN: TRUE/FALSE
           TRUE -Set Pci Setup Data variable.
           FALSE-Get Pci Setup Data variable.

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus. 
**/
EFI_STATUS AmiPciGetPcie1SetupData( PCIE1_SETUP_DATA        *Pcie1,
                                    AMI_SDL_PCI_DEV_INFO    *DevSdlInfo,//OPTIONAL if == NULL get defaults...
                                    UINTN                   DevIdx,        //OPTIONAL if == 0 
                                    BOOLEAN                 Set);

/**
    This function will collect information about 
    PCI Express GEN2 Device setup data variable.
    
    @param Pcie2 Pointer to the GEN 2 Pci related setup data buffer.
    @param DevSdlInfo Pointer to the Pci device information.
    @param DevIdx string ID.
    @param Set  BOOLEAN: TRUE/FALSE
           TRUE -Set Pci Setup Data variable.
           FALSE-Get Pci Setup Data variable.    

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus. 
**/
EFI_STATUS AmiPciGetPcie2SetupData( PCIE2_SETUP_DATA        *Pcie2,
                                    AMI_SDL_PCI_DEV_INFO    *DevSdlInfo,//OPTIONAL if == NULL get defaults...
                                    UINTN                   DevIdx,        //OPTIONAL if == 0 
                                    BOOLEAN                 Set);

/**
    This function will collect information about
    PCI Express GEN HOTPLUG Device setup data variable

    @param PciHp Pointer to the Hotplug Related Settings.  
    @param DevSdlInfo Pointer to the Pci device information.
    @param DevIdx string ID
    @param Set  BOOLEAN: TRUE/FALSE
           TRUE -Set Pci Setup Data variable.
           FALSE-Get Pci Setup Data variable.

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.
**/
EFI_STATUS AmiPciGetPciHpSetupData( PCI_HP_SETUP_DATA       *PciHp,
                                    AMI_SDL_PCI_DEV_INFO    *DevSdlInfo,//OPTIONAL if == NULL get defaults...
                                    UINTN                   DevIdx,        //OPTIONAL if == 0 
                                    BOOLEAN                 Set);

/**
    Shift PCI bus setup data.
     
    @param DevSdlInfo Pointer to the Pci device information.
    @param ShiftValue value of shifting variable.
    @param HostIdx host data ID.
    @param RootIdx Root data ID.
    @param Set  BOOLEAN: TRUE/FALSE
           TRUE -Set Pci Setup Data variable.
           FALSE-Get Pci Setup Data variable.  

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS             When everything is going on fine!
    @retval EFI_NOT_FOUND           When Device not present in the system.
    @retval EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS AmiPciBusShift(            AMI_SDL_PCI_DEV_INFO     *DevSdlInfo,
                                      INT16                    *ShiftValue,    
                                      UINTN                    HostIdx,
                                      UINTN                    RootIdx,
                                      BOOLEAN                  Set);

/**
    This function will launch porting Routine. 

    @param Device Pointer to PCI Device Private Data structure.
    @param InitStep Steps of pci init Routine. 

    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.
**/
//Compatibility routine from AmiCompatibilityPkg
EFI_STATUS LaunchPortingRoutine(PCI_DEV_INFO *Device, PCI_INIT_STEP InitStep, 
                                VOID *Param1, VOID *Param2, VOID *Param3, VOID *Param4);

/**
    Check if System Uses Fixed resource allocation. 

    @param VOID

    @retval EFI_STATUS Return Status
    @retval EFI_SUCCESS     if Fixed Resource Allocation used in the system
        EFI_NOT_FOUND   if NOT

**/
//Check if System SDL Data has Fixed resource allocation information.
EFI_STATUS AmiPciCheckFixedResources();

//Tests if region1(s-e) belongs to the region2(S-E)
//returns EFI_ACCESS_DENIED if Belongs=FALSE and region 1 belong to region 2
//returns EFI_ACCESS_DENIED if Belongs=TRUE and region 1 don't belong to region 2
//returns EFI_SUCCESS in all other cases.
/**
    Tests if region1(s-e) belongs to the region2(S-E)
    
    @param s region
    @param e region
    @param S region
    @param E region
    @param Belongs TRUE/FALSE
    
    @retval 
        EFI_ACCESS_DENIED if Belongs=FALSE and region 1 belong to region 2
        EFI_ACCESS_DENIED if Belongs=TRUE and region 1 don't belong to region 2
        EFI_SUCCESS in all other cases.

**/
EFI_STATUS AmiPciTestRegions(UINT64 s, UINT64 e, UINT64 S, UINT64 E, BOOLEAN Belongs);

/**
    
    @param FixedBar Pointer to PCI_BAR structure to Update Fixed res.
    
    @retval EFI_STATUS
        EFI_SUCCESS             When everything is going on fine!
        EFI_DEVICE_ERROR        When driver fails access PCI Bus.

**/
EFI_STATUS  AmiPciUpdateBarFixedRes(PCI_BAR *FixedBar);

/**
    ValidateDescriptor

                Validates a ASLV_LARGE_RES descriptor.

    @param 
        ASLR_QWORD_ASD *Descriptor
            Descriptor to be validated.
        
        ASLR_TYPE_ENUM ResType
            The resource type of the descriptor.  Valid values are : 
                                                            tResAll, 
                                                            tResIoMem, 
                                                            tResBus
        
        BOOLEAN Fixed
            If TRUE, checks the general flags for consistency.
            
    
    @retval 
        BOOLEAN
            TRUE if a valid ASLR_QWORD_ASD descriptor.  Otherwise, FALSE
            
    @Notes:
        Helper function for AmiPciValidateDescriptorBlock.

**/
BOOLEAN 
AmiPciValidateDescriptor(
        ASLR_QWORD_ASD *Descriptor,
        ASLR_TYPE_ENUM ResourceType,
        BOOLEAN Fixed
);

/**
    AmiPciValidateDescriptorBlock
    
        Validates the consistency of the provided ACPI QWORD resource descriptors 
        block which contains some number of ASLR_QWORD_ASDs in memory terminated 
        by an ASLR_EndTag.  
        
        Returns the total size of the resource descriptors 
        plus the size of the ASLR_EndTag.
        
        Validates a ASLV_LARGE_RES descriptor.

    @param 
        ASLR_QWORD_ASD *Resources
            Pointer to the first descriptor in the block to be validated.
        
        ASLR_TYPE_ENUM ResourceType
            The resource type of the descriptor.  Valid values are : 
                                                            tResAll, 
                                                            tResIoMem, 
                                                            tResBus
        
        BOOLEAN Fixed
            If TRUE, checks the general flags for consistency.
            
    
    @retval 
        UINTN 
            Size of the resource descriptor block plus the size of the ASLR_EndTag.
            
    Referrals:
           ValidateDescriptor.

**/
UINTN 
AmiPciValidateDescriptorBlock(
        ASLR_QWORD_ASD* Resources, 
        ASLR_TYPE_ENUM ResourceType, 
        BOOLEAN Fixed
);
/**
    AlignFromGranularity : Converts passed into Alignment format .

    @param g - AlignFromGra Value to convert

    @retval UINTN - Converted Alignment value
        
**/
UINTN AlignFromGranularity(UINTN g);


/**
    AmiPciFindItemLst
    Searches the object in Lst and returns TRUE/FALSE

    @param  Lst  :            List to search for the object..
    @param  pRes :            Object to be searched in Lst.
    @param  ItemIndex :       Updates object's index here. 
            
    
    @retval  
            TRUE,  if object is found in the Lst.
            Otherwise, FALSE.

**/

BOOLEAN AmiPciFindItemLst(IN T_ITEM_LIST *Lst, IN VOID* pRes, OUT UINTN *ItemIndex);

#endif //_AMI_PCI_BUS_COMMON_LIB_H_
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

