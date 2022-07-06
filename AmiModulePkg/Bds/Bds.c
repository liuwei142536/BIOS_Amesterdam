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

/**
 * @file Bds.c
 * Implementation of the BDS architectural protocol
 */
#include <BootOptions.h>
#include <AmiBds.h>
#include <Dxe.h>
#include <Hob.h>
#include <Protocol/Bds.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/ConsoleControl.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/LoadFile.h>
#include <Protocol/AcpiS3Save.h>
#include <Protocol/BootManagerPolicy.h>
#include <Protocol/AmiBdsConnectPolicy.h>
#include <Protocol/EsrtManagement.h>
#include <Protocol/AmiPause.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/AmiBdsAllDriversConnected.h>
#include <Protocol/AmiBdsConnectDrivers.h>
#include <Protocol/AmiResetRequest.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/GlobalVariable.h>
#include <Guid/CapsuleVendor.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/CapsuleLib.h>

#define BDS_DP_NODE(NodeType, Type, SubType)  {(Type),(SubType),{sizeof(NodeType),0}}
#define BDS_END_OF_DP BDS_DP_NODE(EFI_DEVICE_PATH_PROTOCOL, END_DEVICE_PATH, END_ENTIRE_SUBTYPE)

/***************** FUNCTION DECLARATIONS *****************************/
//Local functions
VOID EFIAPI BdsEntry (IN EFI_BDS_ARCH_PROTOCOL *This);

EFI_STATUS EFIAPI FwLoadFile (
    IN EFI_LOAD_FILE_PROTOCOL *This, IN EFI_DEVICE_PATH_PROTOCOL *FilePath,
    IN BOOLEAN BootPolicy, IN OUT UINTN *BufferSize,
    IN VOID *Buffer OPTIONAL
);
EFI_STATUS EFIAPI BootManagerPolicyConnectDevicePath(
    IN EFI_BOOT_MANAGER_POLICY_PROTOCOL *This,
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath, IN BOOLEAN Recursive
);
EFI_STATUS EFIAPI BootManagerPolicyConnectDeviceClass(
    IN EFI_BOOT_MANAGER_POLICY_PROTOCOL *This,
    IN EFI_GUID                         *Class
);

EFI_STATUS EFIAPI AmiBdsConnectPolicyGetDevicePathPolicy(
    IN AMI_BDS_CONNECT_POLICY_PROTOCOL *This, IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    OUT AMI_BDS_CONNECT_POLICY *Policy
);
EFI_STATUS EFIAPI AmiBdsConnectPolicySetDevicePathPolicy(
    IN AMI_BDS_CONNECT_POLICY_PROTOCOL *This, IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN AMI_BDS_CONNECT_POLICY Policy
);
EFI_STATUS EFIAPI AmiBdsConnectPolicySetDeviceClassPolicy(
    IN AMI_BDS_CONNECT_POLICY_PROTOCOL *This, IN EFI_GUID *Class,
    IN AMI_BDS_CONNECT_POLICY Policy
);
EFI_DEVICE_PATH_PROTOCOL* DiscoverPartition(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
);
EFI_STATUS EFIAPI BdsRequestSystemReset(
    IN AMI_RESET_REQUEST_PROTOCOL *This
);

// Functions from BootOptions.c
VOID FreeBootOptionList();
/**************************** CONSTANTS *******************************/
// {8DB699CC-BC81-41e2-AAC6-D81D5300D759}
#define PARTITION_VARIABLE_GUID\
    { 0x8db699cc, 0xbc81, 0x41e2, { 0xaa, 0xc6, 0xd8, 0x1d, 0x53, 0x0, 0xd7, 0x59 } }
// {5023B95C-DB26-429b-A648-BD47664C8012}
#define AMI_MEDIA_DEVICE_PATH_GUID \
    { 0x5023b95c, 0xdb26, 0x429b, { 0xa6, 0x48, 0xbd, 0x47, 0x66, 0x4c, 0x80, 0x12 } }

//defined in BdsBoard.c
extern EFI_GUID SetupEnterProtocolGuid;
extern EFI_GUID SecondBootOptionProtocolGuid;
extern EFI_GUID BeforeBootProtocolGuid;
extern EFI_GUID BeforeLegacyBootProtocolGuid;

extern BDS_CONTROL_FLOW_FUNCTION *BdsControlFlowFunctions[];
extern CHAR8 *BdsControlFlowFunctionNames[];

EFI_GUID AmiPostMgrProtocolGuid=AMI_POST_MANAGER_PROTOCOL_GUID;
EFI_GUID ConInStartedProtocolGuid = CONSOLE_IN_DEVICES_STARTED_PROTOCOL_GUID;
EFI_GUID ConOutStartedProtocolGuid = CONSOLE_OUT_DEVICES_STARTED_PROTOCOL_GUID;
EFI_GUID PartitionVariableGuid = PARTITION_VARIABLE_GUID;

// The following three variables are not used by BDS.
// They are preserved by backward compatibility with existing BDS plugins.
EFI_GUID BdsAllDriversConnectedProtocolGuid = AMI_BDS_ALL_DRIVERS_CONNECTED_PROTOCOL_GUID;
EFI_GUID BdsConnectDriversProtocolGuid = AMI_BDS_CONNECT_DRIVERS_PROTOCOL_GUID;
EFI_GUID EfiVariableGuid = EFI_GLOBAL_VARIABLE;

EFI_BDS_ARCH_PROTOCOL BDS = {&BdsEntry};

static UINTN NumberOfFvHandles = 0;
static EFI_HANDLE *FvHandles = NULL;

/**************************** VARIABLES *******************************/
//externals defined in BdsBoard.c
extern UINT16 DefaultTimeout;
extern BOOLEAN QuietBoot;
extern STRING_REF StrToken[];
extern CONST CHAR16 *FirmwareVendorString;
extern CONST UINT32 FirmwareRevision;
extern UINT32 BootOptionSupport;
extern UINT64 OsIndicationsSupported;

// Global Variables
EFI_LOAD_FILE_PROTOCOL FwLoadFileInterface  = {FwLoadFile};

struct {
    VENDOR_DEVICE_PATH Media;
    EFI_DEVICE_PATH_PROTOCOL End;
} FwLoadFileDp = {
    {
        BDS_DP_NODE(VENDOR_DEVICE_PATH, MEDIA_DEVICE_PATH, MEDIA_VENDOR_DP),
        AMI_MEDIA_DEVICE_PATH_GUID
    },
    BDS_END_OF_DP
};

EFI_HANDLE LpcHandle = NULL;
EFI_DEVICE_PATH_PROTOCOL *LpcDevicePath = NULL;

EFI_BOOT_MANAGER_POLICY_PROTOCOL BootManagerPolicy = {
    0x00010001,
    BootManagerPolicyConnectDevicePath,
    BootManagerPolicyConnectDeviceClass
};

typedef struct{
    LIST_ENTRY Link; //must be the first field
    AMI_BDS_CONNECT_POLICY Policy;
    EFI_DEVICE_PATH_PROTOCOL *DevicePath;
} CONNECT_POLICY;

AMI_BDS_CONNECT_POLICY NetworkClassConnectPolicy = AmiBdsConnectPolicyEnable;
AMI_BDS_CONNECT_POLICY ExternalOpRomClassConnectPolicy = AmiBdsConnectPolicyEnable;
LIST_ENTRY ConnectPolicyList = INITIALIZE_LIST_HEAD_VARIABLE(ConnectPolicyList);

AMI_BDS_CONNECT_POLICY_PROTOCOL BdsConnectPolicy = {
    0x10000,
    AmiBdsConnectPolicyGetDevicePathPolicy,
    AmiBdsConnectPolicySetDevicePathPolicy,
    AmiBdsConnectPolicySetDeviceClassPolicy
};

STATIC UINT32 BdsExecutionPolicy = 0;
STATIC EFI_BOOT_MODE BootMode = BOOT_WITH_FULL_CONFIGURATION;

STATIC AMI_PAUSE_PROTOCOL *AmiPause = NULL;

STATIC EFI_HANDLE *RootHandles;
STATIC UINTN NumberOfHandles;

AMI_RESET_REQUEST_PROTOCOL ResetRequest = {
    1,
    BdsRequestSystemReset
};
BOOLEAN BdsSystemResetScheduled = FALSE;

/**
 * Using the passed device path, locate every handle for associated with this device path and
 * connect each device
 *
 * @param pPath The device path to connect
 */
VOID ConnectDevicePath(IN EFI_DEVICE_PATH_PROTOCOL *pPath)
{
    EFI_HANDLE Handle;
    EFI_STATUS Status;

    if (pPath == NULL) return;
    while (TRUE)
    {
        EFI_DEVICE_PATH_PROTOCOL *pLastPath=NULL, *pFirstNode = pPath;
        if (isEndNode(pPath))
        {
            if (pPath->SubType == END_ENTIRE_SUBTYPE) break;
            pPath++;
            continue;
        }
        while(TRUE){
            EFI_DEVICE_PATH_PROTOCOL *Dp;
            UINT8 SubType;

            pPath = pFirstNode;

            //LocateDevicePath can not work with multi-instance device paths.
            //Prepare single instance device path and call LocateDevicePath
            Dp = DPGetEndNode(pPath);
            SubType = Dp->SubType;
            Dp->SubType=END_ENTIRE_SUBTYPE;
            Status = pBS->LocateDevicePath(&gEfiDevicePathProtocolGuid, &pPath, &Handle);
            Dp->SubType=SubType;
            if (EFI_ERROR(Status)) break;

            if (isEndNode(pPath))
            {
                //Last time let's do it recursively
                pBS->ConnectController(Handle,NULL,NULL,TRUE);
                break;
            }
            if (pPath==pLastPath) break;
            pLastPath = pPath;
            if (EFI_ERROR(pBS->ConnectController(Handle,NULL,pPath,FALSE))) break;
        }
        while (!isEndNode(pPath))
            pPath = NEXT_NODE(pPath);
    }
}

/**
 * Create a new device path by appending pDp2 to pDp1
 *
 * @param pDp1 The first device path
 * @param pDp2 The second device path
 *
 * @return EFI_DEVICE_PATH_PROTOCOL The new device path
 */
EFI_DEVICE_PATH_PROTOCOL* AddDevicePath(EFI_DEVICE_PATH_PROTOCOL *pDp1, EFI_DEVICE_PATH_PROTOCOL *pDp2)
{
    if (!pDp2) return pDp1;
    if (!pDp1)
    {
        return DPCopy(pDp2);
    }
    else
    {
        pDp2 = DPAddInstance(pDp1,pDp2);
        pBS->FreePool(pDp1);
        return pDp2;
    }
}

/**
 * Return a buffer of handle that contain PciIo protocols, and whose class/subclass
 * match the passed class/subclasses.
 *
 * @param Class The class of PCI devices to search
 * @param SubClass The subclass of PCI devices to search
 * @param NumberOfDevices Pointer to the buffer of the number of Handles being returned
 * @param HandleBuffer Double pointer used to return a buffer of PCI handles that match
 *
 * @retval EFI_NOT_FOUND No Handles were found that match the search criteria
 * @retval EFI_SUCCESS The HandlerBuffer and NumberOfHandles being returned are valid
 */
EFI_STATUS GetPciHandlesByClass(
    UINT8 Class, UINT8 SubClass, UINTN *NumberOfDevices, EFI_HANDLE **HandleBuffer
){
    EFI_STATUS Status;
    EFI_HANDLE *Handle;
    UINTN Number,i;

    if (!NumberOfDevices || !HandleBuffer) return EFI_INVALID_PARAMETER;
    //Get a list of all PCI devices
    Status = pBS->LocateHandleBuffer(
        ByProtocol,&gEfiPciIoProtocolGuid, NULL, &Number, &Handle
    );
    if (EFI_ERROR(Status)) return Status;
    *NumberOfDevices = 0;
    for(i=0; i<Number; i++)
    {
        EFI_PCI_IO_PROTOCOL *PciIo;
        UINT8 PciClass[4];
        Status=pBS->HandleProtocol(Handle[i],&gEfiPciIoProtocolGuid,(VOID**)&PciIo);
        if (EFI_ERROR(Status)) continue;
        Status=PciIo->Pci.Read(PciIo, EfiPciIoWidthUint32, PCI_REV_ID_OFFSET, 1, &PciClass);
        if(!EFI_ERROR(Status) && PciClass[3]==Class && PciClass[2]==SubClass)
            Handle[(*NumberOfDevices)++] = Handle[i];
    }
    if (*NumberOfDevices == 0){
        pBS->FreePool(Handle);
        return EFI_NOT_FOUND;
    }
    *HandleBuffer = Handle;
    return EFI_SUCCESS;
}

/**
 * Attempt to locate the NVRAM variable with the passed name, and the
 * EFI_GLOBAL_VARIABLE_GUID. If the variable cannot be found, create
 * the variable using the passed data and data size.
 *
 * @retval EFI_SUCCESS The variable already exists, or the variable was created
 */
EFI_STATUS InitSystemVariable(
    IN CHAR16 *NameStr, IN UINTN DataSize, IN VOID *Data
)
{
    UINTN Size = 0;
    EFI_STATUS Status;
    Status = pRS->GetVariable(NameStr, &gEfiGlobalVariableGuid, NULL, &Size, NULL);
    if (Status==EFI_NOT_FOUND)
    {
        return pRS->SetVariable(
            NameStr, &gEfiGlobalVariableGuid,
            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
            DataSize, Data);
    }
    return EFI_SUCCESS;
}

/**
 * Attempts to set a variable. If attempt fails because the variable already exists with different attributes,
 * tries to delete the variable and to create it with the new attributes specified by Attributes.
 *
 * @retval EFI_SUCCESS The variable has been successfully created.
 */
EFI_STATUS SetVariableWithNewAttributes(
    IN CHAR16 *Name, IN EFI_GUID *Guid, IN UINT32 Attributes,
    IN UINTN DataSize, IN VOID *Data
)
{
    EFI_STATUS Status;

    Status = pRS->SetVariable(Name, Guid, Attributes, DataSize, Data);
    if (!EFI_ERROR(Status) || Status != EFI_INVALID_PARAMETER) return Status;

    Status = pRS->SetVariable(Name, Guid, 0, 0, NULL);
    if (EFI_ERROR(Status)) return Status;

    return pRS->SetVariable(Name, Guid, Attributes, DataSize, Data);
}

/**
 * Attempt to connect every handle in the system recursively by calling connect controller
 * for each handle.
 */
VOID ConnectEverything()
{
    UINTN i;
    EFI_STATUS Status;

    for(i=0; i<NumberOfHandles; i++) pBS->ConnectController(RootHandles[i],NULL,NULL,TRUE);

//  NOTE: Patch for the Floppy Controller.
//      It is necessary to Disconnect Floppy Device Handle when Floppy Drive is not connected.
//      This is necessary to disable Floppy Device in the Super I/O and eliminate BBS Floppy boot option.
    if (LpcDevicePath)
    {
        EFI_HANDLE Handle;
        ACPI_HID_DEVICE_PATH FloppyCtrl = {
            BDS_DP_NODE(ACPI_HID_DEVICE_PATH, ACPI_DEVICE_PATH, ACPI_DP),
            EISA_PNP_ID(0x0604),0
        };
        UINT32 AltPnpId[] = {EISA_PNP_ID(0x0604), EISA_PNP_ID(0x700), 0};
        EFI_DEVICE_PATH_PROTOCOL *ChildDp, *TmpDp;
        for(i=0; AltPnpId[i]!=0; i++){
            FloppyCtrl.HID = AltPnpId[i];
            ChildDp=TmpDp = DPAddNode(LpcDevicePath, &FloppyCtrl.Header);
            Status=pBS->LocateDevicePath(&gEfiDevicePathProtocolGuid, &TmpDp, &Handle);
            if (!EFI_ERROR(Status) && isEndNode(TmpDp))
            {
                VOID* pDummy;
                Status = pBS->HandleProtocol(Handle,&gEfiBlockIoProtocolGuid,&pDummy);
                if (EFI_ERROR(Status)) pBS->DisconnectController(LpcHandle,NULL,Handle);
                pBS->FreePool(ChildDp);
                break;
            }
            pBS->FreePool(ChildDp);
        }
    }
}

/**
 * Function to Shadow all the legacy option roms
 *
 * Function attempts to locate the legacy bios protocol and then calls the ShadowAllLegacyOpRoms
 * function to allow all the legacy option roms to execute.
 */
VOID ShadowAllLegacyOptionRoms()
{
    EFI_LEGACY_BIOS_PROTOCOL *LegacyBios;
    EFI_STATUS Status;

    Status = pBS->LocateProtocol(&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
    if(!EFI_ERROR(Status)){
        LegacyBios->ShadowAllLegacyOproms(LegacyBios);
    }
}

/**
 * Disconnect every handle available in the system by calling disconnect controller on
 * each handle
 */
VOID DisconnectEverything()
{
    UINTN i;
    for(i=0; i<NumberOfHandles; i++) pBS->DisconnectController(RootHandles[i],NULL,NULL);
}

/**
 * Update the L"BootCurrent" NVRAM variable and signal a ready-to-boot event for the BootXXXX
 * variable associated with the passed OptionNumer.
 *
 * @param OptionNumber The BootXXXX number that will attempt to be booted
 */
VOID ReadyToBoot(UINT16 OptionNumber)
{
    //signal EFI_EVENT_SIGNAL_READY_TO_BOOT
    EFI_EVENT ReadyToBootEvent;
    EFI_STATUS Status;
    if (OptionNumber!= MAX_UINT16)
        pRS->SetVariable(
            L"BootCurrent", &gEfiGlobalVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
            sizeof(OptionNumber), &OptionNumber
        );
    Status = CreateReadyToBootEvent(
        TPL_CALLBACK, NULL, NULL, &ReadyToBootEvent
    );
    if (!EFI_ERROR(Status)) {
        pBS->SignalEvent(ReadyToBootEvent);
        pBS->CloseEvent(ReadyToBootEvent);
    }
}

#if CSM_SUPPORT
/**
 * Attempt to boot to the legacy boot device associated with the Device path and
 * the BootXXXX variable in Number.
 *
 * @param Dp Pointer to the device path that will attempt to be booted (BBS device path)
 * @param Number The BootXXXX number that will attempt to be booted
 *
 * @retval EFI_NOT_FOUND The legacy boot device did not exist
 *
 * @note If this function is successful, control will never be returned back to the function
 * from the LegacyBios->LegacyBoot function call.
 */
EFI_STATUS BootLegacy(EFI_DEVICE_PATH_PROTOCOL *Dp, UINT16 Number)
{
    UINTN i, Old=MAX_UINTN, New=MAX_UINTN, Priority=MAX_UINTN;
    BBS_BBS_DEVICE_PATH *BbsEntry = (BBS_BBS_DEVICE_PATH*)Dp;
    EFI_STATUS Status;
    EFI_LEGACY_BIOS_PROTOCOL *LegacyBios;
    UINT16 HddCount;
    UINT16 BbsCount;
    HDD_INFO *HddInfo;
    BBS_TABLE *BbsTable;

    // Legacy Boot
    Status = pBS->LocateProtocol(&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
    if(EFI_ERROR(Status)) return Status;
    LegacyBios->GetBbsInfo(LegacyBios, &HddCount, &HddInfo, &BbsCount, &BbsTable);
    if (!BbsCount) return EFI_NOT_FOUND;
    for(i=0; i<BbsCount; i++)
    {
        if (   BbsTable[i].BootPriority==BBS_IGNORE_ENTRY
            || BbsTable[i].BootPriority==BBS_DO_NOT_BOOT_FROM
        ) continue;
        if (!BbsTable[i].BootPriority && Old==-1) Old=i;
        if (    BbsTable[i].DeviceType==BbsEntry->DeviceType
            &&  BbsTable[i].BootPriority < Priority
        )
        {
            Priority = BbsTable[i].BootPriority;
            New = i;
        }
    }
    if (New==-1) return EFI_NOT_FOUND;
    if (Old!=-1) BbsTable[Old].BootPriority=BbsTable[New].BootPriority;
    BbsTable[New].BootPriority=0;
    ReadyToBoot(Number);
    PERF_END (NULL, "BDS", NULL, 0);
    return LegacyBios->LegacyBoot(LegacyBios,(BBS_BBS_DEVICE_PATH*)Dp,0,0);
}
#endif

/**
 * Attempt to Efi boot the device associated with the passed Device path
 *
 * @param Dp The device path of the device that will attempt to be booted
 * @param Number The BootXXXX of the device that will attempt to be booted
 * @param pOptions The optional parameters to pass to the boot device
 * @param Size The size of the optional parameters
 *
 * @note If booting the device is successful, the control will never be returned
 * to this function from the ->StartImage call
 */
EFI_STATUS BootEfi(EFI_DEVICE_PATH_PROTOCOL *Dp, UINT16 Number, VOID *pOptions, UINT32 Size)
{
    EFI_STATUS Status;
    EFI_HANDLE Handle;
    EFI_LOADED_IMAGE_PROTOCOL *Image;
    Status=pBS->LoadImage(TRUE, TheImageHandle, Dp, NULL, 0, &Handle);
    //If LoadImage has failed, try resolving short device path starting with HD device path node.
    if (EFI_ERROR(Status) && Dp!=NULL && Dp->Type==MEDIA_DEVICE_PATH && Dp->SubType==MEDIA_HARDDRIVE_DP){
        EFI_DEVICE_PATH_PROTOCOL *FullDp = DiscoverPartition(Dp);
        if (FullDp != NULL){
            Dp = FullDp;
            Status=pBS->LoadImage(TRUE, TheImageHandle, Dp, NULL, 0, &Handle);
        }
    }
    //If LoadImage has failed, try the default OS loader path.
    if (EFI_ERROR(Status)){
        UINT8 FileNodeBuffer[sizeof(EFI_DEVICE_PATH_PROTOCOL)+sizeof(EFI_REMOVABLE_MEDIA_FILE_NAME)];
        EFI_DEVICE_PATH_PROTOCOL *FileNode = (EFI_DEVICE_PATH_PROTOCOL*)FileNodeBuffer;
        FileNode->Type=MEDIA_DEVICE_PATH;
        FileNode->SubType=MEDIA_FILEPATH_DP;
        FileNode->Length[0]=(UINT8)(sizeof(EFI_DEVICE_PATH_PROTOCOL)+sizeof(EFI_REMOVABLE_MEDIA_FILE_NAME));
        FileNode->Length[1]=0;
        Wcscpy((CHAR16*)(FileNode+1),EFI_REMOVABLE_MEDIA_FILE_NAME);
        Dp=DPAddNode(Dp,FileNode);
        Status=pBS->LoadImage(TRUE, TheImageHandle, Dp, NULL, 0, &Handle);
        pBS->FreePool(Dp);
        if (EFI_ERROR(Status)) return Status;
    }
    Status=pBS->HandleProtocol(Handle,&gEfiLoadedImageProtocolGuid,(VOID**)&Image);
    if (EFI_ERROR(Status)) return Status;
    if (Image->ImageCodeType != EfiLoaderCode){
        DEBUG((DEBUG_ERROR, "ERROR: Image referred by Boot%04x is not a UEFI application. Skipping...\n",Number));
        return EFI_UNSUPPORTED;
    }
    if (Size)
    {
        Image->LoadOptionsSize = Size;
        Image->LoadOptions = pOptions;
    }
    ReadyToBoot(Number);
    PERF_END (NULL, "BDS", NULL, 0);
    return pBS->StartImage(Handle, NULL, NULL);
}

/**
 * Attempt to boot the device associated with the passed EFI_LOAD_OPTION.
 *
 * @param BootOption Pointer to the EFI_LOAD_OPTION that will attempt to be booted
 * @param Number The BootXXXX number that will attempt to be booted
 * @param Size The size of the optional data in the BootOption
 *
 * @retval EFI_UNSUPPORTED the device cannot be booted
 */
EFI_STATUS Boot(EFI_LOAD_OPTION *BootOption, UINT16 Number, UINTN Size)
{
    EFI_DEVICE_PATH_PROTOCOL *Dp;
    Dp = (EFI_DEVICE_PATH_PROTOCOL*)
            (   //skip the header
                (UINT8*)(BootOption+1)
                //skip the string
                +(Wcslen((CHAR16*)(BootOption+1))+1)*sizeof(CHAR16)
            );
    if (Dp->Type!=BBS_DEVICE_PATH)
    {
        UINT8 *pOptions = (UINT8*)Dp+BootOption->FilePathListLength;
        return BootEfi(Dp,Number,pOptions, (UINT32)((UINT8*)BootOption+Size-pOptions));
    }
#if CSM_SUPPORT
    else
    {
        return BootLegacy(Dp,Number);
    }
#endif
    return EFI_UNSUPPORTED;
}

/**
  The function checks if Secure Boot is enabled.
  @retval TRUE  Secure boot is enabled.
  @retval FALSE  Secure boot is not enabled.
**/
BOOLEAN IsSecureBoot(){
    EFI_STATUS Status;
    UINTN Size;

    STATIC UINT8 SecureBoot = 0xFF;

    if (SecureBoot == 0xFF){
        Size = sizeof(UINT8);
        Status = pRS->GetVariable(EFI_SECURE_BOOT_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &SecureBoot);
        if (EFI_ERROR(Status)){
            SecureBoot = 0;
        }
    }
    return SecureBoot == 1;
}

/**
 * Run all the DriverXXXX items that are stored in NVRAM. After running the drivers,
 * disconnect all devices, then connected all devices again. Do this to allow any of
 * the newly run drivers the chance to bind onto all the devices
 */
VOID RunDrivers(){
    EFI_LOAD_OPTION *DriverOption = NULL; //buffer for DriverXXX variables
    UINT16 *DriverOrder = NULL; // Driver Order
    UINTN DriverOrderSize = 0; //size of DriverOrder Variable
    EFI_STATUS Status;
    UINTN Size,i;
    BOOLEAN ReconnectAll = FALSE;
    UINT32 Policy;

    // Check if DriverXXXX processing is allowed by the policy
    Policy = BdsExecutionPolicy & AMI_BDS_POLICY_VAR_XXX_MASK;
    if (    Policy == AMI_BDS_POLICY_NO_VAR_XXX
         || ( Policy == AMI_BDS_POLICY_VAR_XXX_ON_SECURE_BOOT_ONLY && !IsSecureBoot() )
    ){
        DEBUG((DEBUG_INFO, "DriverXXXX processing is disabled by the BDS execution policy\n"));
        return;
    }

    //================== Init Driver Order buffers ========================//
    Status = GetEfiVariable(L"DriverOrder", &gEfiGlobalVariableGuid, NULL, &DriverOrderSize, (VOID**)&DriverOrder);
    if (EFI_ERROR(Status)) return;
    //===================================================================//
    // Start drivers referred to by DriverXXXX                           //
    //===================================================================//
    for(i=0; i<DriverOrderSize/sizeof(UINT16); i++){
        CHAR16 DriverStr[11];
        EFI_DEVICE_PATH_PROTOCOL *DevicePath;
        EFI_HANDLE Handle;
        EFI_LOADED_IMAGE_PROTOCOL *Image;
        UINT32 SizeOfOptions;
        UINT8 *Options;

        // Get Driver Option
        Swprintf(DriverStr,L"Driver%04X",DriverOrder[i]);
        Status=GetEfiVariable(DriverStr, &gEfiGlobalVariableGuid, NULL, &Size, (VOID**)&DriverOption);
        if (   EFI_ERROR(Status)
            || (DriverOption->Attributes & LOAD_OPTION_ACTIVE)==0
        ) continue;

        DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)
                (   //skip the header
                    (UINT8*)(DriverOption+1)
                    //skip the string
                    +(Wcslen((CHAR16*)(DriverOption+1))+1)*sizeof(CHAR16)
        );
        Status=pBS->LoadImage(
            FALSE, TheImageHandle, DevicePath, NULL, 0, &Handle
        );
        if (EFI_ERROR(Status)) continue;
        Status=pBS->HandleProtocol(
            Handle,&gEfiLoadedImageProtocolGuid,(VOID**)&Image
        );
        if (EFI_ERROR(Status)) continue;
        if (Image->ImageCodeType != EfiBootServicesCode && Image->ImageCodeType != EfiRuntimeServicesCode){
            DEBUG((DEBUG_ERROR, "ERROR: Image referred by %S is not a UEFI driver. Skipping...\n", DriverStr));
            continue;
        }
        Options = (UINT8*)DevicePath+DriverOption->FilePathListLength;
        SizeOfOptions=(UINT32)((UINT8*)DriverOption+Size-Options);
        if (SizeOfOptions!=0){
            Image->LoadOptionsSize = SizeOfOptions;
            Image->LoadOptions = Options;
        }
        Status=pBS->StartImage(Handle, NULL, NULL);
        if (SizeOfOptions!=0){
            //Since we are freeing the memory where load options reside, reset Image->LoadOptions
            Image->LoadOptionsSize = 0;
            Image->LoadOptions = NULL;
        }
        if (   !EFI_ERROR(Status)
            && (DriverOption->Attributes & LOAD_OPTION_FORCE_RECONNECT)!=0
        ) ReconnectAll=TRUE;
    }
    pBS->FreePool(DriverOption);
    pBS->FreePool(DriverOrder);
    if (ReconnectAll){
        DisconnectEverything();
        ConnectEverything();
    }
}

/**
 * Run all the SysPrepXXXX items that are stored in NVRAM.
 */
VOID RunSysPrepApps(){
    EFI_LOAD_OPTION *SysPrepOption = NULL; //buffer for SysPrepXXX variables
    UINT16 *SysPrepOrder = NULL; // SysPrep Order
    UINTN SysPrepOrderSize = 0; //size of SysPrepOrder Variable
    EFI_STATUS Status;
    UINTN Size,i;
    UINT32 Policy;

    // Check if SysPrepXXXX processing is allowed by the policy
    Policy = BdsExecutionPolicy & AMI_BDS_POLICY_VAR_XXX_MASK;
    if (    Policy == AMI_BDS_POLICY_NO_VAR_XXX
         || ( Policy == AMI_BDS_POLICY_VAR_XXX_ON_SECURE_BOOT_ONLY && !IsSecureBoot() )
    ){
        DEBUG((DEBUG_INFO, "SysPrepXXXX processing is disabled by the BDS execution policy\n"));
        return;
    }

    //================== Init Driver Order buffers ========================//
    Status = GetEfiVariable(L"SysPrepOrder", &gEfiGlobalVariableGuid, NULL, &SysPrepOrderSize, (VOID**)&SysPrepOrder);
    if (EFI_ERROR(Status)) return;
    //===================================================================//
    // Start drivers referred to by SysPrepXXXX                          //
    //===================================================================//
    for(i=0; i<SysPrepOrderSize/sizeof(UINT16); i++){
        CHAR16 SysPrepStr[12];
        EFI_DEVICE_PATH_PROTOCOL *DevicePath;
        EFI_HANDLE Handle;
        EFI_LOADED_IMAGE_PROTOCOL *Image;
        UINT32 SizeOfOptions;
        UINT8 *Options;

        // Get SysPrerp Option
        Swprintf(SysPrepStr,L"SysPrep%04X",SysPrepOrder[i]);
        Status=GetEfiVariable(SysPrepStr, &gEfiGlobalVariableGuid, NULL, &Size, (VOID**)&SysPrepOption);
        if (   EFI_ERROR(Status)
            || (SysPrepOption->Attributes & LOAD_OPTION_ACTIVE)==0
        ) continue;

        DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)
                (   //skip the header
                    (UINT8*)(SysPrepOption+1)
                    //skip the string
                    +(Wcslen((CHAR16*)(SysPrepOption+1))+1)*sizeof(CHAR16)
        );
        Status=pBS->LoadImage(
            FALSE, TheImageHandle, DevicePath, NULL, 0, &Handle
        );
        if (EFI_ERROR(Status)) continue;
        Status=pBS->HandleProtocol(
            Handle,&gEfiLoadedImageProtocolGuid,(VOID**)&Image
        );
        if (EFI_ERROR(Status)) continue;
        if (Image->ImageCodeType != EfiLoaderCode){
            DEBUG((DEBUG_ERROR, "ERROR: Image referred by %S is not a UEFI application. Skipping...\n", SysPrepStr));
            continue;
        }
        Options = (UINT8*)DevicePath+SysPrepOption->FilePathListLength;
        SizeOfOptions=(UINT32)((UINT8*)SysPrepOption+Size-Options);
        if (SizeOfOptions!=0){
            Image->LoadOptionsSize = SizeOfOptions;
            Image->LoadOptions = Options;
        }
        pBS->StartImage(Handle, NULL, NULL);
    }
    pBS->FreePool(SysPrepOption);
    pBS->FreePool(SysPrepOrder);
}

/**
 * Attempt to match the passed partial device path to a full device path. If a match
 * is found, return a pointer to the full device path
 *
 * @param DevicePath The device path to attempt to match to a full device path
 *
 * @retval NULL No match was found
 * @retval EFI_DEVICE_PATH_PROTOCOL The full device path
 */
EFI_DEVICE_PATH_PROTOCOL* DiscoverPartition(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
){
    UINTN Count,i;
    EFI_HANDLE *Handle;
    EFI_STATUS Status;
    EFI_DEVICE_PATH_PROTOCOL *PartDevicePath=NULL;
    HARDDRIVE_DEVICE_PATH* BootParitionDevicePath  = (HARDDRIVE_DEVICE_PATH*)DevicePath;
    //get list of available Block I/O devices
    Status=pBS->LocateHandleBuffer(ByProtocol,&gEfiBlockIoProtocolGuid,NULL,&Count,&Handle);
    if (EFI_ERROR(Status)) return NULL;

    for(i=0;i<Count;i++){
        EFI_BLOCK_IO_PROTOCOL *BlockIo;
        EFI_DEVICE_PATH_PROTOCOL *PartitionDevicePath;
        HARDDRIVE_DEVICE_PATH* PartitionNode;
        Status = pBS->HandleProtocol(Handle[i],&gEfiBlockIoProtocolGuid,(VOID**)&BlockIo);
        if (EFI_ERROR(Status)) continue;
        // if this is not partition, continue
        if (!BlockIo->Media->LogicalPartition) continue;
        Status = pBS->HandleProtocol(Handle[i],&gEfiDevicePathProtocolGuid,(VOID**)&PartitionDevicePath);
        if (EFI_ERROR(Status)) continue;
        // Get last node of the device path. It should be partition node
        PartitionNode = (HARDDRIVE_DEVICE_PATH*)DPGetLastNode(PartitionDevicePath);
        //Check if our partition matches Boot partition
        if (   PartitionNode->Header.Type!=MEDIA_DEVICE_PATH
            || PartitionNode->Header.SubType!=MEDIA_HARDDRIVE_DP
        ) continue;
        if (   PartitionNode->PartitionNumber==BootParitionDevicePath->PartitionNumber
            && PartitionNode->SignatureType==BootParitionDevicePath->SignatureType
            && !MemCmp(PartitionNode->Signature,BootParitionDevicePath->Signature,16)
        ){
            //Match found
            PartDevicePath = PartitionDevicePath;
            break;
        }
    }
    pBS->FreePool(Handle);
    return PartDevicePath;
}

/**
 * Protocol implementation for the FwLoadFile instance that the system creates for
 * the Built In UEFI Shell.
 *
 * @param This Pointer to the EFI_LOAD_FILE_PROTOCOL
 * @param FilePath The device path
 * @param BootPolicy The boot Policy
 * @param BufferSize The size of the buffer to fill with the boot executable
 * @param Buffer The buffer to fill with the boot executable
 *
 * @retval EFI_INVALID_PARAMETER one or more of the parameters are bad
 * @retval EFI_UNSUPPORTED The passed device path is not supported
 * @retval EFI_NOT_FOUND The load image could not be found
 * @retval EFI_BUFFER_TOO_SMALL The buffer is too small to fix the load image
 * @retval EFI_SUCCESS The buffer was successfully filled with the load image
 */
EFI_STATUS FwLoadFile (
    IN EFI_LOAD_FILE_PROTOCOL *This, IN EFI_DEVICE_PATH_PROTOCOL *FilePath,
    IN BOOLEAN BootPolicy, IN OUT UINTN *BufferSize,
    IN VOID *Buffer OPTIONAL
){
    EFI_GUID *FileGuid;
    UINTN i;
    EFI_STATUS Status;

    if (!BufferSize || (*BufferSize && !Buffer))
        return EFI_INVALID_PARAMETER;
    if (!FilePath || isEndNode(FilePath)){
        FileGuid = PcdGetPtr(PcdShellFile);
        if (FileGuid==NULL) return EFI_UNSUPPORTED;
    }else{
        if (   FilePath->Type!=MEDIA_DEVICE_PATH
            || FilePath->SubType!=MEDIA_FV_FILEPATH_DP
        ) return EFI_INVALID_PARAMETER;
        FileGuid = &((MEDIA_FW_VOL_FILEPATH_DEVICE_PATH*)FilePath)->FvFileName;
    }
    //If Buffer is NULL, ReadSection will allocate the memory.
    //That's not what we need.
    //Initialize Buffer with some value.
    //We don't care what value is that because *BufferSize is 0 anyway,
    //so nothing will be copied into the buffer.
    //We know that *BufferSize is always 0 for NULL buffer because we checked that
    //at the beginning of the routine.
    if (!Buffer) Buffer = (VOID*)1;

    // Since this protocol is installed when we are past our trust boundary, don't try to process
    // all available FV instances. Just deal with the known FV.
    // FvHandles array is initialized right after signaling DxeSmmReadyToLock event (SignalConnectDriversEvent).
    Status=EFI_NOT_FOUND;
    for(i=0; i<NumberOfFvHandles; i++)
    {
        EFI_FIRMWARE_VOLUME_PROTOCOL *Fv;
        UINT32 AuthStatus;
        Status = pBS->HandleProtocol(FvHandles[i], &gEfiFirmwareVolume2ProtocolGuid, (VOID**)&Fv);
        if (EFI_ERROR(Status)) continue;
        Status = Fv->ReadSection(
            Fv, FileGuid, EFI_SECTION_PE32,
            0, &Buffer, BufferSize, &AuthStatus
        );
        if (!EFI_ERROR(Status)){
            if (Status==EFI_WARN_BUFFER_TOO_SMALL) Status=EFI_BUFFER_TOO_SMALL;
            break;
        }
    }
    return Status;
}

/**
 * Helper functions used by InstallFwLoadFile to install the LoadFile Protocol for the Built in EFI Shell.
 * InstallFwLoadFile skips protocol installation when SecureBoot is enabled.
 * If platform policy is to install the protocol on SecureBoot,
 * replace "InstallFwLoadFile," eLink with the "InstallFwLoadFileProtocol," eLink.
 */
VOID InstallFwLoadFileProtocol(){
    EFI_HANDLE Handle=NULL;

    pBS->InstallMultipleProtocolInterfaces(
        &Handle,
        &gEfiLoadFileProtocolGuid, &FwLoadFileInterface,
        &gEfiDevicePathProtocolGuid, &FwLoadFileDp,
        NULL
    );
}

/**
 * Install the LoadFileProtocol for the Built in EFI Shell
 */
VOID InstallFwLoadFile(){
    if ( IsSecureBoot() && !PcdGetBool(AmiPcdLoadUnsignedEmbeddedApplications)) return;
    InstallFwLoadFileProtocol();
}


/**
 * Internal function that installs/uninstall protocol with a specified GUID
 * and NULL interface. Such protocols can be used as event signaling mechanism.
 *
 * @param ProtocolGuid Pointer to the protocol GUID
 */
VOID SignalProtocolEvent(IN EFI_GUID *ProtocolGuid){
    EFI_HANDLE  Handle = NULL;
    pBS->InstallProtocolInterface (
        &Handle, ProtocolGuid, EFI_NATIVE_INTERFACE,NULL
    );
    pBS->UninstallProtocolInterface (
        Handle, ProtocolGuid, NULL
    );
}

/**
 * Callback to fill in the FPDT table on legacy boot
 *
 * @param Event The EFI event
 * @param Context Context for the callback
 */
VOID EFIAPI SaveFpdtDataOnLegacyBoot(IN EFI_EVENT Event, IN VOID *Context){
    AmiFillFpdt (FillOsLoaderStartImageStart); // Fill OsLoaderStartImageStart field in FPDT
}

static VOID EFIAPI DummyEndOfDxeEventCallback(IN EFI_EVENT Event, IN VOID *Context){}

/**
 * Create and signal the event for the End of DXE event to allow items in the system to
 * perform the necessary security lock down of their interfaces
 */
VOID SignalEndOfDxeEvent(VOID){
    EFI_EVENT Event;
    EFI_STATUS Status;

    Status = pBS->CreateEventEx(
        EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyEndOfDxeEventCallback,
        NULL, &gEfiEndOfDxeEventGroupGuid, &Event
    );
    if(EFI_ERROR(Status)) return;
    pBS->SignalEvent(Event);
    pBS->CloseEvent(Event);
}

/**
 * Signal the Connect Drivers protocol event
 */
VOID SignalConnectDriversEvent(){
    PROGRESS_CODE(DXE_BDS_CONNECT_DRIVERS);
    SignalProtocolEvent(&gAmiBdsConnectDriversProtocolGuid);
}

/**
 * Signal End of DXE and event and install DXE SMM Ready to Lock protocol
 */
VOID SignalEndOfDxe(){
    EFI_STATUS Status;
    EFI_ACPI_S3_SAVE_PROTOCOL *AcpiS3Save;

    PROGRESS_CODE(DXE_BDS_CONNECT_DRIVERS);

    // ACPI module starting from label ACPI_10 is aligned with the EDKII S3 save/resume implementation,
    // which depends on this call prior to signaling EndOfDxe event.
    if (!EFI_ERROR(pBS->LocateProtocol(&gEfiAcpiS3SaveProtocolGuid, NULL, (VOID **)&AcpiS3Save))){
        AcpiS3Save->S3Save(AcpiS3Save,NULL);
    }

    SignalEndOfDxeEvent();
    SignalProtocolEvent(&gEfiDxeSmmReadyToLockProtocolGuid);

    // Initialize FvHandles array.
    // The array is used by the FwLoadFile function.
    Status = pBS->LocateHandleBuffer (
        ByProtocol, &gEfiFirmwareVolume2ProtocolGuid,
        NULL,  &NumberOfFvHandles, &FvHandles
    );
    if(EFI_ERROR(Status)){
        FvHandles = NULL;
        NumberOfFvHandles = 0;
    }
}

/**
 * Locate all RootBridgeIo protocols in the system and connect those first. This must be
 * done to perform PCI enumeration (on systems that support it)
 */
VOID ConnectRootBridgeHandles(){
    EFI_HANDLE *Handle;
    UINTN Size;
    EFI_STATUS Status;
    UINTN i;

    //Enumerate PCI Bus and Create handles for all PCI devices
    Status = pBS->LocateHandleBuffer(
        ByProtocol,&gEfiPciRootBridgeIoProtocolGuid, NULL,
        &Size, &Handle
    );
    if (EFI_ERROR(Status)) return;
    for(i=0; i<Size; i++)
        pBS->ConnectController(Handle[i],NULL,NULL,FALSE);
    pBS->FreePool(Handle);
}

/**
 * Locate all RootBridgeIo protocols in the system and connect those first. This must be
 * done to perform PCI enumeration (on systems that support it).
 * This function is very similar to ConnectRootBridgeHandles; however, it is launched prior to
 * generation of the EndOfDxe event.
 * Starting from PciBus_12, PCI bus driver does not launch off-board OpROMs when start function is called before End-of-DXE.
 * Off-board OpROMs are launched after End-of-DXE when ConnectRootBridgeHandles is launched.
 *
 */
VOID ConnectRootBridgeHandlesBeforeEndOfDxe(){
    ConnectRootBridgeHandles();
}

/**
 * Report the Con Out Started progress code
 */
VOID ReportConnectConOutProgressCode(){
    PROGRESS_CODE(DXE_CON_OUT_CONNECT);
}

/**
 * Connect possible console out devices. Do this by filtering through
 * the PCI I/O Protocol list and calling connect device path for Display devices
 */
VOID ConnectVgaConOut(){
    EFI_STATUS Status;
    EFI_HANDLE *Handle;
    UINTN Number,i;
    EFI_DEVICE_PATH_PROTOCOL *OnBoard=NULL, *OffBoard=NULL;
    UINT64 PciAttributes;

    //Get a list of all PCI devices
    Status = pBS->LocateHandleBuffer(
        ByProtocol,&gEfiPciIoProtocolGuid, NULL, &Number, &Handle
    );
    if (EFI_ERROR(Status)) return;
    for(i=0; i<Number; i++)
    {
        EFI_PCI_IO_PROTOCOL *PciIo;
        EFI_DEVICE_PATH_PROTOCOL *Dp;
        UINT8 PciClass;
        Status=pBS->HandleProtocol(Handle[i],&gEfiPciIoProtocolGuid,(VOID**)&PciIo);
        if (EFI_ERROR(Status)) continue;
        Status=PciIo->Pci.Read(PciIo, EfiPciIoWidthUint8, 0xB, 1, &PciClass);
        if (EFI_ERROR(Status)) continue;
        if (PciClass!=PCI_CL_DISPLAY) continue;
        Status=pBS->HandleProtocol(Handle[i],&gEfiDevicePathProtocolGuid,(VOID**)&Dp);
        if (EFI_ERROR(Status)) continue;
        //We found Display adapter
        // Check if this is on-board device
        //(EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE is set).
        Status = PciIo->Attributes(
            PciIo, EfiPciIoAttributeOperationGet, 0, &PciAttributes
        );
        if (   !EFI_ERROR(Status)
            && (PciAttributes & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE)
        )  OnBoard = AddDevicePath(OnBoard,Dp);
        else OffBoard = AddDevicePath(OffBoard,Dp);
    }
    pBS->FreePool(Handle);
    //Off-board has a higher priority
    OffBoard = AddDevicePath(OffBoard,OnBoard);
    if (OffBoard)
    {
        ConnectDevicePath(OffBoard);
        pBS->FreePool(OffBoard);
    }
}

/**
 * Get the ConsoleOut NVRAM variables and attempt to connect those device paths
 * @param ConVar The NVRAM variable name that stores the device paths to connect
 */
VOID ConnecConsoleVariable(CHAR16* ConVar){
    EFI_DEVICE_PATH_PROTOCOL *ConPath=NULL;
    UINTN Size = 0;

    if (EFI_ERROR(
        GetEfiVariable(ConVar, &gEfiGlobalVariableGuid, NULL, &Size, (VOID**)&ConPath)
    )) return;
    if (EFI_ERROR(IsValidDevicePath(ConPath))){
        DEBUG((DEBUG_ERROR,
            "ERROR: Console variable %a contains invalid device path.\n",
            ConVar
        ));
        return;
    }
    //Connect all active console devices
    ConnectDevicePath(ConPath);
    pBS->FreePool(ConPath);
}

/**
 * Install the console out started protocol to signal that consoles are available
 * @param ConDevVar The NVRAM variable to check before installing the protocol
 * @param ProtocolGuid The GUID to install if the NVRAM variable exists
 */
VOID InstallConsoleStartedProtocol(
    CHAR16* ConDevVar, EFI_GUID* ProtocolGuid
){
    UINTN Size = 0;
    VOID *Interface;

    //Signal to Console Splitter that all console devices have been started
    //if at least one console device exists (ConDev variable exists)
    if (ConDevVar!=NULL && pRS->GetVariable(
            ConDevVar, &gEfiGlobalVariableGuid, NULL, &Size, NULL
        ) == EFI_NOT_FOUND
    ) return;
    //if the protocol is already installed, return
    if (!EFI_ERROR(pBS->LocateProtocol(ProtocolGuid, NULL, &Interface)))
        return;
    pBS->InstallProtocolInterface(
        &TheImageHandle, ProtocolGuid, EFI_NATIVE_INTERFACE, NULL
    );
}

/**
 * Wrapper function that calls ConnecConsoleVariable for the L"ConOut" devices
 */
VOID ConnectConOutVariable(){
    ConnecConsoleVariable(L"ConOut");
}

/**
 * Wrapper function that calls the InstallConsoleStartedProtocol for the "ConOutDev" variables
 */
VOID InstallConOutStartedProtocol(){
    InstallConsoleStartedProtocol(L"ConOutDev", &ConOutStartedProtocolGuid);
}

/**
 * Function which reports the Con IN Connect progress code to the system
 */
VOID ReportConnectConInProgressCode(){
    PROGRESS_CODE(DXE_CON_IN_CONNECT);
}

/**
 * Function that attempts to connect the PS2 console in devices
 */
VOID ConnectPs2ConIn(){
    EFI_STATUS Status;
    EFI_HANDLE *Handle;
    UINTN Number,i;

    //Get a list of all PCI to ISA Bridges
    Status = GetPciHandlesByClass(
        PCI_CL_BRIDGE, PCI_CL_BRIDGE_SCL_ISA, &Number, &Handle
    );
    if (EFI_ERROR(Status)) return;
    for(i=0; i<Number; i++)
    {
        EFI_DEVICE_PATH_PROTOCOL *Dp, *ChildDp;
        ACPI_HID_DEVICE_PATH Ps2Kbd = {
            BDS_DP_NODE(ACPI_HID_DEVICE_PATH, ACPI_DEVICE_PATH, ACPI_DP),
            EISA_PNP_ID(0x303),0
        };
        ACPI_HID_DEVICE_PATH Ps2Mouse = {
            BDS_DP_NODE(ACPI_HID_DEVICE_PATH, ACPI_DEVICE_PATH, ACPI_DP),
            EISA_PNP_ID(0xF03),0
        };
        Status=pBS->HandleProtocol(Handle[i],&gEfiDevicePathProtocolGuid,(VOID**)&Dp);
        if (EFI_ERROR(Status)) continue;
        pBS->ConnectController(Handle[i],NULL,NULL,FALSE);
        ChildDp=DPAddNode(Dp, &Ps2Kbd.Header);
        ConnectDevicePath(ChildDp);
        pBS->FreePool(ChildDp);
        ChildDp=DPAddNode(Dp, &Ps2Mouse.Header);
        ConnectDevicePath(ChildDp);
        pBS->FreePool(ChildDp);
        LpcHandle = Handle[i];
        LpcDevicePath = Dp;
    }
    pBS->FreePool(Handle);

}

/**
 * Function that connects USB controllers
 */
VOID ConnectUsbControllers(BOOLEAN Recursive){
    EFI_STATUS Status;
    EFI_HANDLE *Handle;
    UINTN Number,i;
    //Get a list of all USB Controllers
    Status = GetPciHandlesByClass(
        PCI_CL_SER_BUS, PCI_CL_SER_BUS_SCL_USB, &Number, &Handle
    );
    if (EFI_ERROR(Status)) return;
    for(i=0; i<Number; i++)
    {
        pBS->ConnectController(Handle[i],NULL,NULL,Recursive);
    }
    pBS->FreePool(Handle);
}

/**
 * Function that non-recursively connects USB controllers
 */
VOID StartUsbControllers(){
    ConnectUsbControllers(FALSE);
}

/**
 * Function that attempts to connect the USB console in devices
 */
VOID ConnectUsbConIn(){
    ConnectUsbControllers(TRUE);
}

/**
 * Wrapper functions that calls the ConnecConsolevariable for the ConIn devices
 */
VOID ConnectConInVariable(){
    ConnecConsoleVariable(L"ConIn");
}

/**
 * Wrapper function that calls the InstallConsoleStarted protocol for the ConInDev and
 * ConsoleINStartedProtocol
 */
VOID InstallConInStartedProtocol(){
    InstallConsoleStartedProtocol(L"ConInDev", &ConInStartedProtocolGuid);
}

/**
 * Function that reports the Status code that will trigger the Con In Available Beep
 */
VOID ConInAvailabilityBeep(){
    LibReportStatusCode(EFI_PROGRESS_CODE, AMI_STATUS_CODE_BEEP_CLASS|1, 0, NULL, NULL);
}

/**
 * Function that initializes the ConOut, ConIn, ConOutDev and ConInDev NVRAM variables
 */
VOID InitConVars()
{
    UINTN i;
    UINTN Size = 0;
    EFI_STATUS Status;
    EFI_DEVICE_PATH_PROTOCOL *ConPath=NULL;

    static CHAR16* ConVar[] = {L"ConOut", L"ConIn"};
    static CHAR16* ConDev[] = {L"ConOutDev", L"ConInDev"};

    // Install Console Stared Protocols
    // ConSplitter will process notification by populating
    // corresponding fields of the system table.
    // At this point the protocol need to be installed
    // even if no actual console devices are available
    // to prevent problems on headless systems
    // caused by NULL console pointers in the system table.
    // The functions will not install the protocol if it has already been installed
    InstallConsoleStartedProtocol(NULL, &ConOutStartedProtocolGuid);
    InstallConsoleStartedProtocol(NULL, &ConInStartedProtocolGuid);

    //Create non-existent ConVar variables for ConIn and ConOut
    //ErrOut will be treated differently
    for( i=0; i<2; i++){
        if (EFI_ERROR(
                GetEfiVariable(ConDev[i], &gEfiGlobalVariableGuid, NULL, &Size, (VOID**)&ConPath)
            )
        ) continue;
        //Set ConVar[i] equal to the ConDev[i]
        pRS->SetVariable(
            ConVar[i], &gEfiGlobalVariableGuid,
            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
            Size, ConPath
        );
    }
    //Let's take care about ErrOut
    Status = GetEfiVariable(
        L"ErrOutDev", &gEfiGlobalVariableGuid, NULL, &Size, (VOID**)&ConPath
    );
    if ( Status == EFI_NOT_FOUND ){
        Status = GetEfiVariable(
            L"ConOutDev", &gEfiGlobalVariableGuid, NULL, &Size, (VOID**)&ConPath
        );
        if (!EFI_ERROR(Status))
            //Set ConErrDev equal to the ConOutDev
            pRS->SetVariable(
                L"ErrOutDev", &gEfiGlobalVariableGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                Size, ConPath
            );
    }
    if (!EFI_ERROR(Status)){
        //Set ErrOut
        pRS->SetVariable(
            L"ErrOut", &gEfiGlobalVariableGuid,
            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
            Size, ConPath
        );
    }
    if (ConPath) pBS->FreePool(ConPath);
}

/**
 * The function calculates amount of system memory using GCD memory map.
 *
 * @retval Size of system memory in bytes
 */
UINTN GetTotalMemorySize(){
    DXE_SERVICES *DxeServices;
    UINT64 TotalMemorySize;
    UINTN NumberOfDescriptors, i;
    EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemorySpaceMap;

    if (   EFI_ERROR(LibGetDxeSvcTbl(&DxeServices))
        || EFI_ERROR(DxeServices->GetMemorySpaceMap (&NumberOfDescriptors, &MemorySpaceMap))
    ) return MAX_ADDRESS;

    TotalMemorySize = 0;
    for (i = 0; i < NumberOfDescriptors; i++) {
        if (    MemorySpaceMap[i].GcdMemoryType != EfiGcdMemoryTypeSystemMemory
             || (MemorySpaceMap[i].Capabilities & EFI_MEMORY_NV) != 0
             || MemorySpaceMap[i].BaseAddress > MAX_ADDRESS
        ) continue;
        TotalMemorySize += MemorySpaceMap[i].Length;
    }
    pBS->FreePool (MemorySpaceMap);
    return (TotalMemorySize > MAX_ADDRESS) ? MAX_ADDRESS : (UINTN)TotalMemorySize;
}

#define IsRuntimeMemoryType(MemoryType) \
  ((MemoryType) == EfiACPIReclaimMemory   || \
   (MemoryType) == EfiACPIMemoryNVS       || \
   (MemoryType) == EfiRuntimeServicesCode || \
   (MemoryType) == EfiRuntimeServicesData || \
   (MemoryType) == EfiReservedMemoryType )

#ifndef BDS_MEMORY_USAGE_INFO_DEFINED
typedef struct{
    UINT32 Type; ///< Memory type the structure is associated with
    UINT32 Previous; ///< Memory usage of a previous boot (extracted from NVRAM variable)
    UINT32 Current; ///< Current memory usage reported by DXE Core memory manager
    UINT32 Next; ///< Projected usage on the next boot (NVRAM variable will be updated with this value)
} BDS_MEMORY_USAGE_INFO;

typedef BOOLEAN (BDS_UPDATE_MEMORY_USAGE_INFORMATION)(BDS_MEMORY_USAGE_INFO *MemoryInfo, UINTN NumberOfElements);
#endif

/**
 * Policy function that is used by default to manage memory usage information.
 * The default function can be overridden by cloning BDS_UPDATE_MEMORY_USAGE_INFORMATION_FUNCTION PCD.
 *
 * @param[in,out] MemoryInfo        Array of BDS_MEMORY_USAGE_INFO structures. One entry per memory type.
 *                                  The array does not include all the types. It only includes information
 *                                  about memory types managed by the platform. This is typically
 *                                  a list of types in the DefaultMemoryTypeInformation array in AmiDxeIpl.c
 *                                  The function will memory usage information from previous boot and current boot
 *                                  (Previous and Current BDS_MEMORY_USAGE_INFO fields) to project future memory usage.
 *                                  If amount of reserved memory for a type has to change, Next field is updated.
 *                                  NOTE: The function should not change any structure fields except Next.
 *
 * @param[in]     NumberOfElements  Number of elements in the MemoryInfo array
 *
 * @retval TRUE   Memory usage information has been changed (at least one Next value has been updated)
 * @retval FALSE  Memory usage information has not been modified
 */
BOOLEAN UpdateMemoryUsageInformationDefault(BDS_MEMORY_USAGE_INFO *MemoryInfo, UINTN NumberOfElements){
    UINTN i;
    UINTN MemoryUsageThreshold, TotalProjectedMemoryUage;
    BOOLEAN MemoryTypeInformationModified = FALSE;

    // Since we never decrease memory usage values (see comments in the for loop below),
    // we don't want to record values that are too high.
    // MemoryUsageThreshold is a memory usage threshold.
    // We are not recording memory usage information that exceed memory usage threshold.
    // In many cases unusually high memory usage is a temporary spike that disappears after system reboot.
    // We're setting memory usage threshold to a half of available memory or 2GB, whichever is smaller.
    MemoryUsageThreshold = GetTotalMemorySize() >> 1;
    if (MemoryUsageThreshold > 0x80000000) MemoryUsageThreshold = 0x80000000;
    MemoryUsageThreshold = EFI_SIZE_TO_PAGES(MemoryUsageThreshold);
    TotalProjectedMemoryUage = 0;

    for (i = 0; i<NumberOfElements; i++) {
        // If memory usage is too high, do not update memory usage information (see comments above);
        // otherwise, set next value to 125% of the current one.
        if (MemoryInfo[i].Current > MemoryUsageThreshold){
            MemoryInfo[i].Next = MemoryInfo[i].Previous;
        } else {
            UINT32 Next = MemoryInfo[i].Current + (MemoryInfo[i].Current >> 2);
            if (Next > MemoryUsageThreshold) Next = MemoryInfo[i].Current;
            // We are never decreasing the memory type usage values.
            // This is done to workaround UEFI Windows 7 and Windows 8 bug.
            // Windows loader can't properly handle (it crashes)
            // memory map changes that happen after OS loader has been launched
            // and before the ExitBootServices call.
            // It's very difficult to predict how much memory will be used during
            // the execution of the Windows loader because in certain cases Windows loader
            // is pretty active. For example, it sometimes calls
            // ConnectController for all the devices.
            // By never decreasing the memory type usage values, we are avoiding the problem
            // by always assuming the worst case scenario (the heaviest memory usage).
            // The drawback is, we are stealing more memory than is actually used from the user.
            if (Next > MemoryInfo[i].Previous){
                MemoryInfo[i].Next = Next;
                MemoryTypeInformationModified = TRUE;
            }else {
                MemoryInfo[i].Next = MemoryInfo[i].Previous;
            }
        }
        TotalProjectedMemoryUage += MemoryInfo[i].Next;
    }
    // If memory usage is too high, do not update memory usage information (see comments above).
    if (TotalProjectedMemoryUage > MemoryUsageThreshold) MemoryTypeInformationModified  = FALSE;

    return MemoryTypeInformationModified;
}

extern BDS_UPDATE_MEMORY_USAGE_INFORMATION *UpdateMemoryUsageInformation;

/**
 * Calculates the total memory usage of the system and stores
 * the information into the L"MemoryTypeInformation" NVRAM variable to allow the
 * system to allocate contiguous memory bins for each of the memory types
 * This helps the system from preventing memory map fragmentation.
 *
 * @param Event The callback event
 * @param Context the context of the callback event
 */
VOID SaveMemoryTypeInformation(BOOLEAN *ResetRequired){
    extern EFI_GUID gAmiGlobalVariableGuid;
    static BOOLEAN MemoryTypeInformationIsSaved = FALSE;
    static EFI_GUID HobListGuid = HOB_LIST_GUID;

    EFI_STATUS Status;
    EFI_HOB_GUID_TYPE *MemoryInformationHob;
    EFI_MEMORY_TYPE_INFORMATION *MemoryTypeInformation = NULL;
    UINTN MemoryTypeInformationSize = 0;
    BOOLEAN IsFirstBoot = FALSE;
    EFI_MEMORY_TYPE_INFORMATION *CurrentMemoryTypeInformation;
    UINTN i, j, NumberOfMemoryInfoElements;
    EFI_HOB_GUID_TYPE *MemoryInfoVariableGuidHob;
    EFI_GUID * MemoryInformationVariableGuid;
    BDS_MEMORY_USAGE_INFO MemoryInfoHistory[EfiMaxMemoryType];

    if (ResetRequired) *ResetRequired = FALSE;
    // Make sure the processing is performed only once.
    // (we are registering callback on multiple events in RegisterMemoryTypeInformationUpdateCallback)
    if (MemoryTypeInformationIsSaved){
        return;
    }

    // Get the Memory Type Information settings from HOB if they exist,
    // PEI is responsible for getting them from a UEFI variable and building a HOB to save them.
    MemoryInformationHob = GetEfiConfigurationTable(pST, &HobListGuid);
    if (MemoryInformationHob == NULL) return;
    MemoryInfoVariableGuidHob = MemoryInformationHob;

    if (EFI_ERROR(
        FindNextHobByGuid(&gEfiMemoryTypeInformationGuid, (VOID**)&MemoryInformationHob)
    )) return;

    // Older code was storing memory type information in the variable with gEfiMemoryTypeInformationGuid.
    // Newer code is using gAmiGlobalVariableGuid
    // BDS and DxeIpl must use the same GUID.
    // A newer version of DxeIpl creates special GUIDed HOB with gAmiGlobalVariableGuid GUID
    // to indicate that it supports new memory type information variable GUID.
    // If the HOB does not exist, we are dealing with an older version of DxeIpl and old variable GUID must be used.
    Status = FindNextHobByGuid(&gAmiGlobalVariableGuid, (VOID**)&MemoryInfoVariableGuidHob);
    if (   EFI_ERROR(Status)
        || MemoryInfoVariableGuidHob->Header.HobLength != sizeof(EFI_HOB_GUID_TYPE) + sizeof(EFI_GUID)
        || guidcmp(MemoryInfoVariableGuidHob+1,&gEfiMemoryTypeInformationGuid) != 0
    ){
        MemoryInformationVariableGuid = &gEfiMemoryTypeInformationGuid;
    }
    else{
        MemoryInformationVariableGuid = &gAmiGlobalVariableGuid;
    }

    Status = pRS->GetVariable(
        L"PreviousMemoryTypeInformation", MemoryInformationVariableGuid, NULL,
        &MemoryTypeInformationSize, NULL
    );
    IsFirstBoot = Status==EFI_NOT_FOUND;

    MemoryTypeInformation = (EFI_MEMORY_TYPE_INFORMATION*)(MemoryInformationHob+1);
    MemoryTypeInformationSize =   MemoryInformationHob->Header.HobLength
                                - sizeof (EFI_HOB_GUID_TYPE);
    // Save memory information for the current boot.
    // It will be used if next boot is S4 resume.
    Status = SetVariableWithNewAttributes (
        L"PreviousMemoryTypeInformation", MemoryInformationVariableGuid,
        EFI_VARIABLE_NON_VOLATILE  | EFI_VARIABLE_BOOTSERVICE_ACCESS,
        MemoryTypeInformationSize, MemoryTypeInformation
    );
    ASSERT_EFI_ERROR(Status);

    // Retrieve the current memory usage statistics.  If they are not found, then
    // no adjustments can be made to the Memory Type Information variable.
    CurrentMemoryTypeInformation = GetEfiConfigurationTable(
        pST, &gEfiMemoryTypeInformationGuid
    );
    if (CurrentMemoryTypeInformation == NULL) return;
    DEBUG((DEBUG_LOAD, "BDS.%a(%p)\n",
        "SaveMemoryTypeInformation", SaveMemoryTypeInformation
    ));
    // Prepare memory information data structure that is passed to a policy function
    for (i = 0; MemoryTypeInformation[i].Type != EfiMaxMemoryType && i<EfiMaxMemoryType; i++) {
        MemoryInfoHistory[i].Type = MemoryTypeInformation[i].Type;
        MemoryInfoHistory[i].Previous = MemoryTypeInformation[i].NumberOfPages;
        MemoryInfoHistory[i].Next = MemoryInfoHistory[i].Previous;
        MemoryInfoHistory[i].Current = 0;
        for (j = 0; CurrentMemoryTypeInformation[j].Type != EfiMaxMemoryType; j++) {
            if (MemoryTypeInformation[i].Type == CurrentMemoryTypeInformation[j].Type) {
                MemoryInfoHistory[i].Current = CurrentMemoryTypeInformation[j].NumberOfPages;
                break;
            }
        }
    }
    NumberOfMemoryInfoElements = i;
    // Adjust the Memory Type Information for the next boot.
    if ( UpdateMemoryUsageInformation(MemoryInfoHistory,NumberOfMemoryInfoElements) ){
#if !defined(MDEPKG_NDEBUG)
        DEBUG_CODE_BEGIN();
            CONST CHAR8* EfiMemTypeStr[] = {
                "Reserved   ",
                "LoaderCode ",
                "LoaderData ",
                "BSCode     ",
                "BSData     ",
                "RSCode     ",
                "RSData     ",
                "Free       ",
                "Unusable   ",
                "ACPIReclaim",
                "ACPINVS    ",
                "MMIO       ",
                "MMIOIOPort ",
                "PalCode    "
            };

            DEBUG((DEBUG_INFO, "   Memory    Previous  Current    Next   \n"
                               "    Type      Pages     Pages     Pages  \n"
                               "===========  ========  ========  ========\n"
            ));

            for (i = 0; i<NumberOfMemoryInfoElements; i++) {
                DEBUG((
                    DEBUG_INFO, "%a %8X  %8X  %8X\n",
                    EfiMemTypeStr[MemoryTypeInformation[i].Type],
                    MemoryInfoHistory[i].Previous,
                    MemoryInfoHistory[i].Current,
                    MemoryInfoHistory[i].Next
                ));
            }
        DEBUG_CODE_END();
#endif
        for (i = 0; i<NumberOfMemoryInfoElements; i++) {
            MemoryTypeInformation[i].Type = MemoryInfoHistory[i].Type;
            MemoryTypeInformation[i].NumberOfPages = MemoryInfoHistory[i].Next;
        }
        // If any changes were made to the Memory Type Information settings,
        // set the new variable value;
        Status = SetVariableWithNewAttributes(
            EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME, MemoryInformationVariableGuid,
            EFI_VARIABLE_NON_VOLATILE  | EFI_VARIABLE_BOOTSERVICE_ACCESS,
            MemoryTypeInformationSize, MemoryTypeInformation
        );
        ASSERT_EFI_ERROR(Status);
        if (IsFirstBoot && PcdGetBool(PcdResetOnMemoryTypeInformationChange)){
            for (i = 0; i < NumberOfMemoryInfoElements; i++) {
                if (MemoryInfoHistory[i].Previous != MemoryInfoHistory[i].Next && IsRuntimeMemoryType(MemoryInfoHistory[i].Type) ){
                    if (ResetRequired) *ResetRequired = TRUE;
                    return;
                }
            }
        }
    }
    MemoryTypeInformationIsSaved = TRUE;
}


/**
 * Callback event that process memory usage information and checks for pending reset requests.
 *
 * @param Event The callback event
 * @param Context the context of the callback event
 */
VOID EFIAPI ProceedToBookCallback (
    IN EFI_EVENT Event, IN VOID *Context
){
    BOOLEAN ResetRequired;
    SaveMemoryTypeInformation(&ResetRequired);
    if (ResetRequired || BdsSystemResetScheduled){
        DEBUG_CODE(
            if (ResetRequired) DEBUG(( DEBUG_LOAD, "Default RT memory quotas have been increased.\n"));
            if (BdsSystemResetScheduled) DEBUG(( DEBUG_LOAD, "Reset has been requested via AmiResetRequest protocol.\n"));
        );
        DEBUG((
            DEBUG_LOAD,
            "Resetting the system...\n"
        ));
        pRS->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
        ASSERT(FALSE);
    }
    pBS->CloseEvent(Event);
}

/**
 * Register callback events that will be triggered to call SaveMemoryTypeInformation
 * for the system to save the memory usage information and to process pending reset request.
 */
VOID RegisterMemoryTypeInformationUpdateCallback(){
    EFI_EVENT Event;
    VOID *Registration;

    if (   BootMode == BOOT_ON_S4_RESUME
        || BootMode == BOOT_ON_FLASH_UPDATE
        || BootMode == BOOT_IN_RECOVERY_MODE
    ) return;

    // We really want to get control.
    // That's why we are registering callbacks for multiple boot events hoping that
    // at least one of them will be triggered.
    // If multiple events are signaled, only the first one is handled
    RegisterProtocolCallback(
        &BeforeBootProtocolGuid,
        ProceedToBookCallback,
        NULL, &Event, &Registration
    );
    RegisterProtocolCallback(
        &BeforeLegacyBootProtocolGuid,
        ProceedToBookCallback,
        NULL, &Event, &Registration
    );
    CreateLegacyBootEvent(TPL_CALLBACK, ProceedToBookCallback, NULL, &Event);
    pBS->CreateEvent(
        EVT_SIGNAL_EXIT_BOOT_SERVICES,TPL_CALLBACK,
        ProceedToBookCallback, NULL, &Event
    );
}

/**
 * Function that locates the DXE Services table pointer, and calls the DXE dispatcher
 * to attempt to execute any drivers there previous were not executed.
 */
VOID CallTheDispatcher(){
    DXE_SERVICES *pDxe;

    if (EFI_ERROR(LibGetDxeSvcTbl(&pDxe)))
        return;
    if (pDxe) pDxe->Dispatch();
}

/**
 * Signal the event that says all drivers have been connected
 */
VOID SignalAllDriversConnectedEvent(){
    SignalProtocolEvent(&gAmiBdsAllDriversConnectedProtocolGuid);
}

/**
 * Attempt to give control to AMITSE
 */
VOID HandoffToTse(){
    AMI_POST_MANAGER_PROTOCOL *AmiPostMgr = NULL;
    EFI_HANDLE Handle = NULL;

    // Set a couple of standard UEFI variables
    pRS->SetVariable(
        L"BootOptionSupport", &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        sizeof(BootOptionSupport), &BootOptionSupport
    );
    pRS->SetVariable(
        L"OsIndicationsSupported", &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        sizeof(OsIndicationsSupported), &OsIndicationsSupported
    );
    // Install Boot Manager Policy protocol
    pBS->InstallProtocolInterface(&Handle, &gEfiBootManagerPolicyProtocolGuid, EFI_NATIVE_INTERFACE, &BootManagerPolicy);
    // Free BootOptionList
    FreeBootOptionList();
    // Hand off to TSE
    if (!EFI_ERROR(pBS->LocateProtocol(
            &AmiPostMgrProtocolGuid, NULL, (VOID**)&AmiPostMgr
    ))){
        PERF_END (NULL, "BDS", NULL, 0);
        AmiPostMgr->Handshake();
    }

}

/**
 * Protocol used to enter TSE from the DXE core
 *
 * @param This pointer to the EFI_BDS_ARCH_PRTOCOL
 */
VOID BdsEntry (IN EFI_BDS_ARCH_PROTOCOL *This)
{
    UINTN i;
    EFI_EVENT Event;
    VOID      *Registration;
    EFI_STATUS Status;
    UINT32 LaunchPolicy;

    PERF_END (NULL, "DXE", NULL, 0);
    PERF_START (NULL, "BDS", NULL, 0);

    //Lang & LangCodes are initialized by Setup driver
    InitSystemVariable(L"Timeout",  sizeof(UINT16), &DefaultTimeout);

    RegisterProtocolCallback(
        &BeforeLegacyBootProtocolGuid,
        SaveFpdtDataOnLegacyBoot,
        NULL, &Event, &Registration
    );

    // Initialize Global Variables.
    Status = pBS->LocateHandleBuffer(AllHandles, NULL, NULL, &NumberOfHandles, &RootHandles);
    if (EFI_ERROR(Status)){
        NumberOfHandles = 0;
        RootHandles = NULL;
    }
    BootMode = GetBootMode();
    BdsExecutionPolicy =
          (BootMode == BOOT_ON_FLASH_UPDATE || BootMode == BOOT_IN_RECOVERY_MODE)
        ? PcdGet32(AmiPcdBdsPolicyMaskOnRecoveryAndFlashUpdate)
        : PcdGet32(AmiPcdBdsPolicyMask);

    // Check if External OpROM processing is allowed by the execution policy
    LaunchPolicy = BdsExecutionPolicy & AMI_BDS_POLICY_EXTERNAL_OPROM_MASK;
    if (    LaunchPolicy == AMI_BDS_POLICY_NO_EXTERNAL_OPROM
         || (LaunchPolicy == AMI_BDS_POLICY_EXTERNAL_OPROM_ON_SECURE_BOOT_ONLY && !IsSecureBoot())
    ){
        DEBUG((DEBUG_INFO, "External OpROM processing is disabled by the BDS execution policy\n"));
        // Disable OpROM using AmiBdsConnectPolicy->SetDeviceClassPolicy.
        // In addition to preventing drivers from connecting to a device it also
        // blocks execution of OpROM drivers loaded from a device, which is what we need
        // in this case.
        AmiBdsConnectPolicySetDeviceClassPolicy(
            &BdsConnectPolicy,
            &gAmiBdsConnectPolicyExternalOptionRomGuid,
            AmiBdsConnectPolicyDisable
        );
    }

    Status = pBS->LocateProtocol(&gAmiPauseProtocolGuid, NULL, (VOID **)&AmiPause);
    if (EFI_ERROR(Status)) AmiPause = NULL;

    for(i=0; BdsControlFlowFunctions[i]!=NULL; i++){
        // Pause support. If AmiPause protocol is available, we're calling it before every execution step.
        // It's pause driver's responsibility to check if system has to be paused and stay in the Pause function
        // until execution is resumed.
        if (AmiPause != NULL) AmiPause->Pause();
        DEBUG((DEBUG_LOAD, "BDS.%a(%p)\n",
            BdsControlFlowFunctionNames[i], BdsControlFlowFunctions[i]
        ));
        PERF_START (TheImageHandle, BdsControlFlowFunctionNames[i], "BDS", 0);
        BdsControlFlowFunctions[i]();
        PERF_END (TheImageHandle, BdsControlFlowFunctionNames[i], "BDS", 0);
    }
    PERF_END (NULL, "BDS", NULL, 0);
}

/**
 * Update the System Table's FirmwareVendor field with the correct information
 * @return
 */
VOID SetSystemTableFirmwareInfo(){
    EFI_STATUS Status;
    UINTN Size = (Wcslen((CHAR16*)FirmwareVendorString)+1)*sizeof(CHAR16);

    // =========== Initialize System Table Fields
    Status = pBS->AllocatePool(EfiRuntimeServicesData, Size, (VOID**)&pST->FirmwareVendor);
    if (!EFI_ERROR(Status))
        Wcscpy(pST->FirmwareVendor, (CHAR16*)FirmwareVendorString);
    pST->FirmwareRevision = FirmwareRevision;
}

// Deprecated Variable.
// Use TheImageHandle instead.
EFI_HANDLE ThisImageHandle = NULL;

/**
 * BDS Image entry point. Installs the BDS Arch protocol and updates the system table's
 * firmware vendor information
 *
 * @param ImageHandle The handle associated with this image being loaded into memory
 * @param SystemTable Pointer to the system table
 */
EFI_STATUS EFIAPI BdsInit (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_HANDLE Handle = NULL;
    InitAmiLib(ImageHandle, SystemTable);
    ThisImageHandle = ImageHandle;
    SetSystemTableFirmwareInfo();
    return pBS->InstallMultipleProtocolInterfaces(
        &Handle,
        &gEfiBdsArchProtocolGuid, &BDS,
        &gAmiBdsConnectPolicyProtocolGuid, &BdsConnectPolicy,
        &gAmiResetRequestProtocolGuid, &ResetRequest,
        NULL
    );
}


/**
 * Attempt to boot the system by reading the L"BoorOrder" NVRAM variable and trying
 * to boot to the devices.
 *
 * @note If this function works correctly, then control will never be returned from it
 */
EFI_STATUS ReadBootOptionAndBoot()
{
    UINTN OptionSize, BootOrderSize,i;
    UINT16 *BootOrder=NULL;
    EFI_LOAD_OPTION *Option=NULL;
    CHAR16 BootVarName[15]; //Boot0000

    EFI_STATUS Status = GetEfiVariable(
        L"BootOrder", &gEfiGlobalVariableGuid, NULL, &BootOrderSize, (VOID**)&BootOrder
    );

    if (EFI_ERROR(Status)) return Status;
    for(i=0; i< BootOrderSize/sizeof(UINT16); i++){
        Swprintf(BootVarName,L"Boot%04X",BootOrder[i]);
        Status = GetEfiVariable(
            BootVarName, &gEfiGlobalVariableGuid, NULL, &OptionSize, (VOID**)&Option
        );
        if (EFI_ERROR(Status)){
            // Workaround for non-UEFI specification complaint OS.
            // Some OS create BootXXXX variables using lower case letters A-F.
            // Search for lower case boot option variable if no upper case variable found.
            Swprintf(BootVarName,L"Boot%04x",BootOrder[i]);
            Status=GetEfiVariable(
                BootVarName, &gEfiGlobalVariableGuid, NULL, &OptionSize,(VOID**)&Option
            );
            if (EFI_ERROR(Status)) continue;
        }
        Boot(Option,BootOrder[i],OptionSize);
    }
    pBS->FreePool(BootOrder);
    pBS->FreePool(Option);
    return Status;
}

#define EFI_MEMORY_PRESENT      0x0100000000000000ULL
#define EFI_MEMORY_INITIALIZED  0x0200000000000000ULL
#define EFI_MEMORY_TESTED       0x0400000000000000ULL

/**
 * Go through the memory space and adjust the memory above 4Gb so that it is reported as usable
 * to operating systems. This is done to help prevent the BIOS from attempting to execute its
 * code above the 4GB boundary
 */
VOID RecoverTheMemoryAbove4Gb(){
    DXE_SERVICES *gDS;
    UINTN NumberOfDescriptors;
    EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemorySpaceMap;
    UINTN Index;
    EFI_STATUS Status;
    if (EFI_ERROR(LibGetDxeSvcTbl(&gDS))) return;
    Status = gDS->GetMemorySpaceMap (&NumberOfDescriptors, &MemorySpaceMap);
    if (EFI_ERROR(Status)) return;

    for (Index = 0; Index < NumberOfDescriptors; Index++) {
        if (MemorySpaceMap[Index].GcdMemoryType != EfiGcdMemoryTypeReserved) continue;
        if (    (MemorySpaceMap[Index].Capabilities & (EFI_MEMORY_PRESENT | EFI_MEMORY_INITIALIZED | EFI_MEMORY_TESTED))
             != (EFI_MEMORY_PRESENT | EFI_MEMORY_INITIALIZED)
        ) continue;

        // For those reserved memory that have not been tested, simply promote to system memory.
        gDS->RemoveMemorySpace (
            MemorySpaceMap[Index].BaseAddress, MemorySpaceMap[Index].Length
        );
        gDS->AddMemorySpace (
            EfiGcdMemoryTypeSystemMemory,
            MemorySpaceMap[Index].BaseAddress,MemorySpaceMap[Index].Length,
              MemorySpaceMap[Index].Capabilities
            & ~(EFI_MEMORY_PRESENT | EFI_MEMORY_INITIALIZED | EFI_MEMORY_TESTED | EFI_MEMORY_RUNTIME)
        );

    }
    pBS->FreePool (MemorySpaceMap);
}

VOID ProcessCapsulesAfterEndOfDxe(){
    ProcessCapsules();
}

/**
 Updates FMP entries in the ECR table using EsrtManagement protocol.
**/
VOID UpdateEsrtFmpEntries(){
    ESRT_MANAGEMENT_PROTOCOL *EsrtManagement;
    EFI_STATUS Status;

    Status = pBS->LocateProtocol(&gEsrtManagementProtocolGuid, NULL,  (VOID**)&EsrtManagement);
    if(!EFI_ERROR(Status)){
        EsrtManagement->SyncEsrtFmp();
    }
}

/**
  Connect a device path following the platforms EFI Boot Manager policy.

  The ConnectDevicePath() function allows the caller to connect a DevicePath using the
  same policy as the EFI Boot Manger.

  @param[in] This       A pointer to the EFI_BOOT_MANAGER_POLICY_PROTOCOL instance.
  @param[in] DevicePath Points to the start of the EFI device path to connect.
                        If DevicePath is NULL then all the controllers in the
                        system will be connected using the platforms EFI Boot
                        Manager policy.
  @param[in] Recursive  If TRUE, then ConnectController() is called recursively
                        until the entire tree of controllers below the
                        controller specified by DevicePath have been created.
                        If FALSE, then the tree of controllers is only expanded
                        one level. If DevicePath is NULL then Recursive is ignored.

  @retval EFI_SUCCESS            The DevicePath was connected.
  @retval EFI_NOT_FOUND          The DevicePath was not found.
  @retval EFI_NOT_FOUND          No driver was connected to DevicePath.
  @retval EFI_SECURITY_VIOLATION The user has no permission to start UEFI device
                                 drivers on the DevicePath.
  @retval EFI_UNSUPPORTED        The current TPL is not TPL_APPLICATION.
**/
EFI_STATUS EFIAPI BootManagerPolicyConnectDevicePath(
    IN EFI_BOOT_MANAGER_POLICY_PROTOCOL *This,
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath, IN BOOLEAN Recursive
){
    EFI_HANDLE Handle;
    EFI_STATUS Status;
    EFI_TPL Tpl;
    EFI_DEVICE_PATH_PROTOCOL *LastPath, *Path;

    Tpl = pBS->RaiseTPL(TPL_HIGH_LEVEL);
    pBS->RestoreTPL(Tpl);
    if (Tpl != TPL_APPLICATION) return EFI_UNSUPPORTED;
    if (DevicePath == NULL){
        ConnectEverything();
        return EFI_SUCCESS;
    }
    LastPath=NULL;
    while(TRUE){
        Path = DevicePath;
        Status = pBS->LocateDevicePath(&gEfiDevicePathProtocolGuid, &Path, &Handle);
        if (EFI_ERROR(Status)) return Status;
        if (isEndNode(Path)){
            // Last node should be connected in accordance with the requested recursion policy
            return pBS->ConnectController(Handle,NULL,NULL,Recursive);
        }
        if (Path==LastPath) return EFI_NOT_FOUND;
        LastPath = Path;
        Status = pBS->ConnectController(Handle,NULL,Path,FALSE);
        if (EFI_ERROR(Status)) return Status;
    }
}

EFI_STATUS ConnectNetworkDevices(){
    EFI_STATUS Status, ArgregatedStatus;
    EFI_HANDLE *Handle;
    UINTN Number,i;

    //Get a list of all PCI devices
    Status = pBS->LocateHandleBuffer(
        ByProtocol,&gEfiPciIoProtocolGuid, NULL, &Number, &Handle
    );
    if (EFI_ERROR(Status)) return Status;
    ArgregatedStatus = EFI_NOT_FOUND;
    for(i=0; i<Number; i++)
    {
        EFI_PCI_IO_PROTOCOL *PciIo;
        EFI_DEVICE_PATH_PROTOCOL *Dp;
        UINT8 PciClass;

        Status=pBS->HandleProtocol(Handle[i],&gEfiPciIoProtocolGuid,(VOID**)&PciIo);
        if (EFI_ERROR(Status)) continue;
        Status=PciIo->Pci.Read(PciIo, EfiPciIoWidthUint8, 0xB, 1, &PciClass);
        if (EFI_ERROR(Status)) continue;
        if (PciClass!=PCI_CL_NETWORK) continue;
        Status=pBS->HandleProtocol(Handle[i],&gEfiDevicePathProtocolGuid,(VOID**)&Dp);
        if (EFI_ERROR(Status)) continue;
        //We found Network adapter. Connect it.
        Status = BootManagerPolicyConnectDevicePath(NULL, Dp, TRUE);
        if (EFI_ERROR(ArgregatedStatus)) ArgregatedStatus = Status;
    }
    pBS->FreePool(Handle);
    return ArgregatedStatus;
}

/**
  Connect a class of devices using the platform Boot Manager policy.

  The BootManagerPolicyConnectDeviceClass() function allows the caller to request that the Boot
  Manager connect a class of devices.

  If Class is EFI_BOOT_MANAGER_POLICY_CONSOLE_GUID then the Boot Manager will
  use platform policy to connect consoles. Some platforms may restrict the
  number of consoles connected as they attempt to fast boot, and calling
  ConnectDeviceClass() with a Class value of EFI_BOOT_MANAGER_POLICY_CONSOLE_GUID
  must connect the set of consoles that follow the Boot Manager platform policy,
  and the EFI_SIMPLE_TEXT_INPUT_PROTOCOL, EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL, and
  the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL are produced on the connected handles.
  The Boot Manager may restrict which consoles get connect due to platform policy,
  for example a security policy may require that a given console is not connected.

  If Class is EFI_BOOT_MANAGER_POLICY_NETWORK_GUID then the Boot Manager will
  connect the protocols the platforms supports for UEFI general purpose network
  applications on one or more handles. If more than one network controller is
  available a platform will connect, one, many, or all of the networks based
  on platform policy. Connecting UEFI networking protocols, like EFI_DHCP4_PROTOCOL,
  does not establish connections on the network. The UEFI general purpose network
  application that called ConnectDeviceClass() may need to use the published
  protocols to establish the network connection. The Boot Manager can optionally
  have a policy to establish a network connection.

  If Class is EFI_BOOT_MANAGER_POLICY_CONNECT_ALL_GUID then the Boot Manager
  will connect all UEFI drivers using the UEFI Boot Service
  EFI_BOOT_SERVICES.ConnectController(). If the Boot Manager has policy
  associated with connect all UEFI drivers this policy will be used.

  A platform can also define platform specific Class values as a properly generated
  EFI_GUID would never conflict with this specification.

  @param[in] This  A pointer to the EFI_BOOT_MANAGER_POLICY_PROTOCOL instance.
  @param[in] Class A pointer to an EFI_GUID that represents a class of devices
                   that will be connected using the Boot Mangers platform policy.

  @retval EFI_SUCCESS      At least one devices of the Class was connected.
  @retval EFI_DEVICE_ERROR Devices were not connected due to an error.
  @retval EFI_NOT_FOUND    The Class is not supported by the platform.
  @retval EFI_UNSUPPORTED  The current TPL is not TPL_APPLICATION.
**/
EFI_STATUS EFIAPI BootManagerPolicyConnectDeviceClass(
    IN EFI_BOOT_MANAGER_POLICY_PROTOCOL *This,
    IN EFI_GUID                         *Class
){
    EFI_TPL Tpl;

    Tpl = pBS->RaiseTPL(TPL_HIGH_LEVEL);
    pBS->RestoreTPL(Tpl);
    if (Tpl != TPL_APPLICATION) return EFI_UNSUPPORTED;
    if (!guidcmp(Class,&gEfiBootManagerPolicyConsoleGuid)){
        ConnectConOutVariable();
        ConnectConInVariable();
        return EFI_SUCCESS;
    }if (!guidcmp(Class,&gEfiBootManagerPolicyNetworkGuid)){
        return ConnectNetworkDevices();
    }if (!guidcmp(Class,&gEfiBootManagerPolicyConnectAllGuid)){
        ConnectEverything();
        return EFI_SUCCESS;
    }
    return EFI_NOT_FOUND;
}

/** @internal
  Internal function that returns policy descriptor for a specified device path.

  @param[in] DevicePath EFI device path to return policy descriptor for.

  @retval Descriptor            The policy descriptor for DevicePath.
  @retval NULL                  Descriptor is not found
**/
CONNECT_POLICY* FindConnectPolicyDescriptor(EFI_DEVICE_PATH_PROTOCOL *DevicePath){
    LIST_ENTRY *Entry;

    for(Entry = GetFirstNode(&ConnectPolicyList); !IsNull(&ConnectPolicyList, Entry); Entry=GetNextNode(&ConnectPolicyList, Entry)){
        CONNECT_POLICY *Descriptor = (CONNECT_POLICY*)Entry;
        if (!DPCmp(DevicePath, Descriptor->DevicePath)) return Descriptor;
    }
    return NULL;
}

/** @internal
  Internal function that checks if passed in PCI I/O instance represents a network device.

  @param[in] PciIo                 PCI I/O instance to check.
  @param[in] RemainingDevicePath   Remaining portion of the device path of the device analyzed
                                   by the connect policy protocol after removing device path associated
                                   with PciIo instance.

  @retval TRUE   The Handle is a network device
  @retval FALSE  The Handle is not a network device
**/
BOOLEAN IsNetworkClassDevice(
    EFI_PCI_IO_PROTOCOL *PciIo, EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath
){
    EFI_STATUS Status;
    UINT8 PciClass;

    if (!isEndNode(RemainingDevicePath)) return EFI_SUCCESS;
    // Check PCI class of the device
    Status=PciIo->Pci.Read(PciIo, EfiPciIoWidthUint8, 0xB, 1, &PciClass);
    if (EFI_ERROR(Status)) return FALSE;
    return PciClass==PCI_CL_NETWORK;
}

/** @internal
  Internal function that checks if passed in handle represents PCI device with external OpROM.

  @param[in] PciIo                 PCI I/O instance to check.
  @param[in] RemainingDevicePath   Remaining portion of the device path of the device analyzed
                                   by the connect policy protocol after removing device path associated
                                   with PciIo instance.

  @retval TRUE   The Handle is a PCI device with external OpROM
  @retval FALSE  The Handle is not a PCI device with external OpROM
**/
BOOLEAN IsExternalOpRomClassDevice(
    EFI_PCI_IO_PROTOCOL *PciIo, EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath
){
    EFI_DEVICE_PATH_PROTOCOL *Dp = RemainingDevicePath;
    UINT64 Attributes;
    EFI_STATUS Status;

    // Make sure only one node is remaining before the end of the device path.
    // It should be a node that specifies image location within OpROM.
    // It can be a relative offset node or a memory mapped node.
    if (    isEndNode(Dp)
         || !isEndNode(NEXT_NODE(Dp))
         || (   !(Dp->Type == HARDWARE_DEVICE_PATH && Dp->SubType == HW_MEMMAP_DP)
             && !(Dp->Type == MEDIA_DEVICE_PATH && Dp->SubType == MEDIA_RELATIVE_OFFSET_RANGE_DP)
            )
    ) return FALSE;
    Status = PciIo->Attributes(PciIo, EfiPciIoAttributeOperationGet, 0, &Attributes);
    if (EFI_ERROR(Status)) return FALSE;
    return ( Attributes & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_ROM ) ? FALSE : TRUE;
}

/** @internal
  Internal function that checks if a non default class policy has been set for at least one class.

  @retval TRUE   The policy has been set
  @retval FALSE  The policy has not been set
**/
BOOLEAN IsClassPolicySet(){
    return     NetworkClassConnectPolicy != AmiBdsConnectPolicyEnable
            || ExternalOpRomClassConnectPolicy != AmiBdsConnectPolicyEnable;
}

/**
  Gets platform connect policy for a device path.

  The GetDevicePathPolicy() function allows the caller to get connect policy for a DevicePath.

  @param[in]  This       A pointer to the AMI_BDS_CONNECT_POLICY_SET_DEVICE_PATH_POLICY instance.
  @param[in]  DevicePath Points to the start of the EFI device path to connect.
  @param[out] Policy     Current DevicePath connect policy

  @retval EFI_SUCCESS            The policy has been successfully retrieved.
  @retval EFI_INVALID_PARAMETER  The device path is invalid
  @retval EFI_INVALID_PARAMETER  The Policy is NULL
**/
EFI_STATUS EFIAPI AmiBdsConnectPolicyGetDevicePathPolicy(
    IN AMI_BDS_CONNECT_POLICY_PROTOCOL *This, IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    OUT AMI_BDS_CONNECT_POLICY *Policy
){
    CONNECT_POLICY *Descriptor;
    EFI_DEVICE_PATH_PROTOCOL *TmpDevicePath;
    EFI_STATUS Status;
    EFI_HANDLE DeviceHandle;
    EFI_PCI_IO_PROTOCOL *PciIo;

    if (Policy==NULL || DevicePath==NULL) return EFI_INVALID_PARAMETER;
    if (EFI_ERROR(IsValidDevicePath((EFI_DEVICE_PATH_PROTOCOL *)DevicePath)))
       return EFI_INVALID_PARAMETER;

    // If policy has been defined for DevicePath, return the policy
    Descriptor = FindConnectPolicyDescriptor((EFI_DEVICE_PATH_PROTOCOL *)DevicePath);
    if (Descriptor != NULL){
        *Policy=Descriptor->Policy;
        return EFI_SUCCESS;
    }

    // If this is not a network device, return the default policy.
    *Policy = AmiBdsConnectPolicyEnable;
    // If no class policies are set, we are done.
    if (!IsClassPolicySet()) return EFI_SUCCESS;
    // Make sure we're dealing with PCI device
    TmpDevicePath = (EFI_DEVICE_PATH_PROTOCOL*)DevicePath;
    Status = pBS->LocateDevicePath(&gEfiPciIoProtocolGuid, &TmpDevicePath, &DeviceHandle);
    if (EFI_ERROR(Status)) return EFI_SUCCESS;
    Status = pBS->HandleProtocol(DeviceHandle,&gEfiPciIoProtocolGuid,(VOID**)&PciIo);
    if (EFI_ERROR(Status)) return EFI_SUCCESS;

    // Check if we are dealing with the network device
    if (IsNetworkClassDevice(PciIo, TmpDevicePath))
        *Policy = NetworkClassConnectPolicy;
    // Check if we are dealing with the external Option ROM
    if (IsExternalOpRomClassDevice(PciIo, TmpDevicePath))
        *Policy = ExternalOpRomClassConnectPolicy;
    return EFI_SUCCESS;
}

/**
  Sets platform connect policy for a device path.

  The SetDevicePathPolicy() function allows the caller to set connect policy for a DevicePath.

  @param[in] This       A pointer to the AMI_BDS_CONNECT_POLICY_SET_DEVICE_PATH_POLICY instance.
  @param[in] DevicePath Points to the start of the EFI device path to connect.
  @param[in] Policy     Connect policy to apply to DevicePath

  @retval EFI_SUCCESS            The policy has been successfully applied.
  @retval EFI_INVALID_PARAMETER  The device path is invalid
**/
EFI_STATUS EFIAPI AmiBdsConnectPolicySetDevicePathPolicy(
    IN AMI_BDS_CONNECT_POLICY_PROTOCOL *This, IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN AMI_BDS_CONNECT_POLICY Policy
){
    CONNECT_POLICY *Descriptor;

    if (DevicePath==NULL || Policy >= AmiBdsConnectPolicyMax) return EFI_INVALID_PARAMETER;
    if (EFI_ERROR(IsValidDevicePath((EFI_DEVICE_PATH_PROTOCOL *)DevicePath)))
       return EFI_INVALID_PARAMETER;
    Descriptor = FindConnectPolicyDescriptor((EFI_DEVICE_PATH_PROTOCOL *)DevicePath);
    if (Descriptor != NULL){
        if (Policy == AmiBdsConnectPolicyEnable){
            // If we already have a descriptor for DevicePath and Policy is a default policy,
            // no need to maintain the descriptor.
            RemoveEntryList(&Descriptor->Link);
            pBS->FreePool(Descriptor->DevicePath);
            pBS->FreePool(Descriptor);
        }
        else {
            Descriptor->Policy = Policy;
        }
        return EFI_SUCCESS;
    }
    // If descriptor does not exist and new policy is the same as the default policy, no need to create a descriptor.
    if (Policy == AmiBdsConnectPolicyEnable) return EFI_SUCCESS;
    Descriptor = Malloc(sizeof(CONNECT_POLICY));
    if (Descriptor==NULL) return EFI_OUT_OF_RESOURCES;
    Descriptor->DevicePath = DPCopy((EFI_DEVICE_PATH_PROTOCOL *)DevicePath);
    if (Descriptor->DevicePath==NULL) return EFI_OUT_OF_RESOURCES;
    Descriptor->Policy = Policy;
    InsertTailList(&ConnectPolicyList,&Descriptor->Link);
    return EFI_SUCCESS;
}


/**
  Sets platform connect policy for a class of devices.

  The SetDeviceClassPolicy() function allows the caller to set connect policy for a class of devices defined by GUID.
  The function accepts all device class GUID defined by BootManagerPolicy protocol.

  @param[in] This       A pointer to the EFI_BOOT_MANAGER_POLICY_PROTOCOL instance.
  @param[in] Class      A pointer to an EFI_GUID that represents a class of devices to set policy for.
                        Supported classes:
                          gEfiBootManagerPolicyNetworkGuid
                          gAmiBdsConnectPolicyExternalOptionRomGuid(starting from Revision 0x00010001)
  @param[in] Policy     Connect policy to apply to all devices that belong to a class specified by Class GUID.

  @retval EFI_SUCCESS      The policy has been successfully applied.
  @retval EFI_UNSUPPORTED  The Class is not supported.
**/
EFI_STATUS EFIAPI AmiBdsConnectPolicySetDeviceClassPolicy(
    IN AMI_BDS_CONNECT_POLICY_PROTOCOL *This, IN EFI_GUID *Class,
    IN AMI_BDS_CONNECT_POLICY Policy
){
    if (Policy >= AmiBdsConnectPolicyMax) return EFI_INVALID_PARAMETER;
    if (!guidcmp(Class,&gEfiBootManagerPolicyNetworkGuid)) {
        NetworkClassConnectPolicy = Policy;
        return EFI_SUCCESS;
    } else if (!guidcmp(Class,&gAmiBdsConnectPolicyExternalOptionRomGuid)) {
        ExternalOpRomClassConnectPolicy = Policy;
        return EFI_SUCCESS;
    }
    return EFI_UNSUPPORTED;
}

/**
  Schedules system reset.

  The function registers system reset request.
  The actual system reset is performed at the end of the boot process
  before launching OS.

  @param[in]  This      A pointer to the AMI_RESET_REQUEST_PROTOCOL instance.

  @retval EFI_SUCCESS   Reset request has been registered.
**/
EFI_STATUS EFIAPI BdsRequestSystemReset(
    IN AMI_RESET_REQUEST_PROTOCOL *This
){
   BdsSystemResetScheduled = TRUE;
   return EFI_SUCCESS;
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
