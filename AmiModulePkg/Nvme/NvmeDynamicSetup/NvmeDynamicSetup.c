//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**           5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093      **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file NvmeDynamicSetup.c
    Updates Nvme setup page dynamically

**/

#include "NvmeDynamicSetup.h"
#include <Protocol/AMIPostMgr.h>
#include <Library/PrintLib.h>
#include <AutoId.h>

//--------------------------------------
// Global Variable Definitions
//-------------------------------------------------------------------------

static EFI_HII_STRING_PROTOCOL        *HiiString = NULL;
static CHAR8                          *gSupportedLanguages = NULL;

AMI_POST_MANAGER_PROTOCOL *gAmiPostMgr = NULL;
VOID                      *gDisplayHandle;

static EFI_HII_HANDLE gNvmeHiiHandle = NULL;
LIST_ENTRY            gNvmeControllerList;

static EFI_GUID gNvmeFormSetGuid = NVME_FORM_SET_GUID;

EFI_HII_CONFIG_ACCESS_PROTOCOL CallBack = { NULL, NULL, NvmeCallback };

EFI_GUID    gNvmeDynamicSetupDataGuid = NVME_DYNAMIC_SETUP_VARIABLE_GUID;
EFI_STRING  gNvmeDynamicDataVarName = L"NvmeDynamicSetupVar";
NVME_DYNAMIC_SETUP_CONFIGURATION    gNvmeDynamicSetupData = {1,0,0,0};

CALLBACK_INFO SetupCallBack[] = {
    // Last field in every structure will be filled by the Setup
    { &gNvmeFormSetGuid, &CallBack, NVME_FORM_SET_CLASS, 0, 0},
};
/** @internal
    This function intializes the progress bar display of
	Device Self test operation.
    
	@retval VOID
	
    @endinternal
**/
static VOID SelfTestProgressInit ()
{
    AMI_POST_MGR_KEY OutKey;
    if (gAmiPostMgr == NULL)
        gBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, &gAmiPostMgr);

    if(gAmiPostMgr != NULL) {
        gAmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_INIT,
                 L"Running Device SelfTest",
                 L"Progress Update",
                 NULL,
                 0,
                 &gDisplayHandle,
                 &OutKey);
    }
}
/** @internal
    This function updates the progress percentage
	for Device Self test operation.
    
	@param  UINTN percent             Completed  progress percentage 
	
	@retval  AMI_POST_MGR_KEY *OutKey  Key pressed during progress bar display.
	
    @endinternal
**/
static VOID SelfTestProgressUpdate (UINTN percent, AMI_POST_MGR_KEY *OutKey)
{
    if(gAmiPostMgr != NULL) {
        gAmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_UPDATE,
                 L"Running Device SelfTest",
                 L"Progress Update",
                 NULL,
                 percent, 
                 &gDisplayHandle,
                 OutKey);
    }
}
/** @internal
    This function terminates the Progress bar display 
	of Device Self test operation.
    	
	@retval  VOID
	
    @endinternal
**/
static VOID SelfTestProgressClose ()
{
    AMI_POST_MGR_KEY OutKey;
    if(gAmiPostMgr != NULL) {
        gAmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_CLOSE,
                 L"Running Device SelfTest",
                 L"Progress Update",
                 NULL,
                 0,
                 &gDisplayHandle,
                 &OutKey);
    }
}

/** @internal
    This function is wrapper function of pRS->GetVariable() with
    error check.

    @param CHAR16* - Variable name
    @param EFI_GUID - Variable GUID
    @param UINT32* - Variable attribute
    @param UINTN* - Variable size
    @param VOID** - pointer to variable's memory location

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
NvmeSecGetEfiVariable(
    IN CHAR16 *sName,
    IN EFI_GUID *pGuid,
    OUT UINT32 *pAttributes OPTIONAL,
    IN OUT UINTN *pDataSize,
    OUT VOID **ppData
)
{
    EFI_STATUS  Status;
    if (!*ppData) *pDataSize=0;
    Status = pRS->GetVariable(sName, pGuid, pAttributes, pDataSize, *ppData);
    if (!EFI_ERROR(Status)) return Status;
    if (Status==EFI_BUFFER_TOO_SMALL)
    {
        if (*ppData) pBS->FreePool(*ppData);
        if (!(*ppData=Malloc(*pDataSize))) return EFI_OUT_OF_RESOURCES;
        Status = pRS->GetVariable(sName, pGuid, pAttributes, pDataSize, *ppData);
    }
    return Status;
}

/** @internal
    Returns the String based on StringId

    @param HiiHandle
    @param StringId

    @retval EFI_STRING
    
    @endinternal
**/
EFI_STRING
NvmeSecHiiGetString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  EFI_STRING_ID   StringId
)
{
    EFI_STRING   String = NULL;
    UINTN        StringSize = 0;
    EFI_STATUS   Status;
    CHAR8        *PlatformLang = NULL;
    UINTN        Size = 0;

    if(HiiString == NULL) {
        Status = pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
        if(EFI_ERROR(Status))
            return NULL;
    }

    //language not provided - try platform language
    Status = NvmeSecGetEfiVariable(L"PlatformLang", &gEfiGlobalVariableGuid, NULL, &Size, &PlatformLang);
    if(!EFI_ERROR(Status)) {
        Status = HiiString->GetString(HiiString, PlatformLang, HiiHandle, StringId, String, &StringSize, NULL);
        if(Status == EFI_BUFFER_TOO_SMALL) {
            Status = pBS->AllocatePool(EfiBootServicesData, StringSize, (VOID**)&String);
            if(!EFI_ERROR(Status))
                Status = HiiString->GetString(HiiString, PlatformLang, HiiHandle, StringId, String, &StringSize, NULL);
        }
        pBS->FreePool(PlatformLang);
    }
    return (EFI_ERROR(Status)) ? NULL : String;
}
/** @internal
    Function to update the Nvme setup configuration page data
	
    @return VOID

    @endinternal
**/

VOID
NvmeRefreshSetupData (
)
{
    pRS->SetVariable(L"NvmeDynamicSetupVar",
                    &gNvmeDynamicSetupDataGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(NVME_DYNAMIC_SETUP_CONFIGURATION),
                    &gNvmeDynamicSetupData);

    // Refresh browser data
    HiiLibSetBrowserData( sizeof(NVME_DYNAMIC_SETUP_CONFIGURATION),
                        &gNvmeDynamicSetupData,
                        &gNvmeDynamicSetupDataGuid,
                        L"NvmeDynamicSetupVar");

}
/** @internal
    Function to send Device self test command 
	
	@param  NvmePassthruProtocol   Pointer to NvmePassthru protocol interface
	@param  Type                   Specifies either to perform Short or Extended type of self test
	@param  Action                 Specify either to test Controller alone or Controller and all NameSpaces
    
	@returnval  EFI_STATUS

    @endinternal
**/
EFI_STATUS
NvmeDeviceSelfTest (
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassthruProtocol,
    IN DEVICE_SELF_TEST_TYPE               Type,
    IN UINT8                               Action
)
{

    EFI_STATUS    Status;

    EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET    NvmeCommandPacket;
    EFI_NVM_EXPRESS_COMMAND                     NvmeCmd;
    EFI_NVM_EXPRESS_COMPLETION                  NvmeCompletion;
    
    // Clear the Global NvmeCommandPacket structure to Zero
    pBS->SetMem( &NvmeCommandPacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET), 0 );
    
    // Clear Global Nvm Express Admin Command to Zero
    pBS->SetMem( &NvmeCmd, sizeof(EFI_NVM_EXPRESS_COMMAND), 0);

    // Clear Global Nvm Express completion structure to Zero
    pBS->SetMem( &NvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION), 0);

    NvmeCmd.Nsid = Action == 0 ? 0 : 0xFFFFFFFF;      // Either Controller or controller and all Namespace 
    NvmeCmd.Cdw0.Opcode = 0x14;
    NvmeCmd.Cdw0.FusedOperation = 0;
    NvmeCmd.Cdw10 = Type;
    NvmeCmd.Flags |= CDW10_VALID;
    
    NvmeCommandPacket.CommandTimeout = NVME_COMMAND_TIMEOUT * 10000000;    // 1Sec
    NvmeCommandPacket.NvmeCmd = &NvmeCmd;
    NvmeCommandPacket.QueueType = 0;            
    NvmeCommandPacket.NvmeCompletion = &NvmeCompletion;
    NvmeCommandPacket.TransferBuffer = NULL;
    NvmeCommandPacket.TransferLength = 0;

    // Send Command through Passthru API
    Status = NvmePassthruProtocol->PassThru( NvmePassthruProtocol,
                                             0,
                                             &NvmeCommandPacket,
                                             NULL
                                             );
    
    return Status;
    
}

/**
    Retrieves the log pages from an NVMe device corresponding to the requested self-test 
    by the user and provides 20-most recent result log data.    

    @param NvmePassthruProtocol      Pointer to NvmePassthruProtocol interface
    @param LogPage                   LogPage identifier to retrieve
    @param Buffer                    Buffer to store retrieved Log page data
    @param Length                    Number of byte data to retrieve

    @retval EFI_STATUS
**/
EFI_STATUS
NvmeGetLogPage (
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmePassthruProtocol,
  IN UINT16                             LogPage,
  IN OUT VOID                           *Buffer,
  IN OUT UINTN                          *Length )
{
    
    EFI_STATUS    Status;

    EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET    NvmeCommandPacket;
    EFI_NVM_EXPRESS_COMMAND                     NvmeCmd;
    EFI_NVM_EXPRESS_COMPLETION                  NvmeCompletion;
    
    // Clear the Global NvmeCommandPacket structure to Zero
    pBS->SetMem( &NvmeCommandPacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET), 0 );
    
    // Clear Global Nvm Express Admin Command to Zero
    pBS->SetMem( &NvmeCmd, sizeof(EFI_NVM_EXPRESS_COMMAND), 0);

    // Clear Global Nvm Express completion structure to Zero
    pBS->SetMem( &NvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION), 0);

    NvmeCmd.Nsid = 0;
    NvmeCmd.Cdw0.Opcode = GET_LOG_PAGE;
    NvmeCmd.Cdw0.FusedOperation = 0;
    NvmeCmd.Cdw10 = (UINT32)((((*Length >> 2) - 1) << 16) + LogPage);
    NvmeCmd.Flags |= CDW10_VALID;
    
    NvmeCommandPacket.CommandTimeout = NVME_COMMAND_TIMEOUT * 10000000;    // 1Sec
    NvmeCommandPacket.NvmeCmd = &NvmeCmd;
    NvmeCommandPacket.QueueType = 0;
    NvmeCommandPacket.NvmeCompletion = &NvmeCompletion;
    NvmeCommandPacket.TransferBuffer = Buffer;
    NvmeCommandPacket.TransferLength = (UINT32)*Length;

    // Send Command through Passthru API
    Status = NvmePassthruProtocol->PassThru( NvmePassthruProtocol,
                                             0,
                                             &NvmeCommandPacket,
                                             NULL
                                             );
    
    return Status;
}
/**
    Routine to get the result of recently completed self test operation for corresponding input type.
	
    @param Buffer      Buffer which holds entire device Self test log,
	                    which includes results of last 20 device self tests operation.
	@param Type        Specifies either Short or extended self test data to retrieve
    @param Result      Result of recently completed self test operation for corresponding input type.

    @retval EFI_STATUS
**/
EFI_STATUS
GetSelfTestData(
  IN  VOID        *Buffer, 
  IN  UINT8       Type, 
  OUT VOID        **Result
)
{
    
    NVME_DEVICE_SELF_TEST_RESULT        *SelfTestResult = (NVME_DEVICE_SELF_TEST_RESULT *) ((UINT8 *)Buffer + sizeof(NVME_DEVICE_SELF_TEST_LOG));
    UINT8                               Count = 0;
    
    *Result = NULL;    
    
    for (;Count < MAX_SELF_TEST_LOG_COUNT; Count++){
        if ((SelfTestResult->SelfTestStatusResult) == 0xF){
            return EFI_NOT_FOUND;
        }    
        if ((SelfTestResult->SelfTestStatusCode) == Type ){
            *Result = SelfTestResult;
			return EFI_SUCCESS;
        }
        SelfTestResult++;
    }
    
    return EFI_NOT_FOUND;
    
}
/**
    Routine which sends GetLogPage command to retrieve Self test data result.
    
    @param ControllerHandle     NvmeController handle
    @param Type                 Specifies either Short or extended self test data to retrieve
    @param Result               Result of recently completed self test operation for corresponding input type.
    @param SelfTestCode         Self Test code of resultant data. 
    @retval EFI_STATUS
**/
EFI_STATUS
GetSelfTestCode (
  IN  EFI_HANDLE                     ControllerHandle,        
  IN  UINT8                          Type, 
  OUT NVME_DEVICE_SELF_TEST_RESULT  *ResultBytes,
  OUT UINT8                         *SelfTestCode
)
{
    EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL     *NvmePassthru;
    EFI_STATUS                              Status;
    VOID                                    *Buffer;
    NVME_DEVICE_SELF_TEST_RESULT            *Result;
    UINTN                                   BufferLength = DEVICE_SELF_TEST_LOG_LENGTH;

    // Result unknown
    *SelfTestCode = 0xF;
    
    Status = pBS->HandleProtocol(
                          ControllerHandle,
                          &gEfiNvmExpressPassThruProtocolGuid,
                          (VOID *)&NvmePassthru
                          );

    if (EFI_ERROR(Status)){
        DEBUG((DEBUG_ERROR,"SelfTest: NvmePassthru not found :  %r\n", Status));
        return EFI_NOT_FOUND;
    }

    Status = pBS->AllocatePool (EfiBootServicesData,
                                 BufferLength,
                                 (VOID**)&Buffer
                                );
    
    if (EFI_ERROR(Status)){
        return EFI_NOT_FOUND;
    }
    
   
    Status = NvmeGetLogPage(NvmePassthru, NVME_LOG_DEVICE_SELF_TEST, Buffer, &BufferLength);
    
    if (EFI_ERROR(Status)){
        pBS->FreePool(Buffer);
        return EFI_NOT_FOUND;;
    }
    
    //Decode Device Self test Log
    GetSelfTestData(Buffer, Type, &Result);

    //If result is valid, then update its corresponding result
    if (Result){
        *SelfTestCode = Result->SelfTestStatusResult;
    }
    if (ResultBytes) {
        *ResultBytes = *Result;
    }    
    if (Buffer) {
        pBS->FreePool(Buffer);
    }
    return EFI_SUCCESS;   
}
/**
    Function to resultant StringId based on self test result to display.
    
    @param StatusByte     Self test result status
 
    @retval StringId
**/
EFI_STRING_ID
GetSelfTestResultStringID (
    UINT8       StatusByte
)
{    
    switch (StatusByte) {  
        case 0:
            return STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_PASS);
        case 1:
            return STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_ABORTED);       
        case 0xF:
            return STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_NA);        
        default:
            return STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_FAIL);
    }   
}

/**
    Initializes NVME controller Text Item string ID

    @param  NvmeControllerInfo  Points to the NVME_CONTROLLER_INFORMATION.

    @return VOID

**/
VOID
InitializeNvmeItemStrId(
    NVME_CONTROLLER_INFORMATION    *NvmeControllerInfo
)
{
    NvmeControllerInfo->BasicInfo.ModelNumberStrId.Prompt = STRING_TOKEN(STR_NVME_MODEL_NUMBER);
    NvmeControllerInfo->BasicInfo.SizeStrId.Prompt = STRING_TOKEN(STR_NVME_SIZE);
    NvmeControllerInfo->BasicInfo.BusStrId.Prompt = STRING_TOKEN(STR_NVME_BUS_NUMBER);
    NvmeControllerInfo->BasicInfo.VendorStrId.Prompt = STRING_TOKEN(STR_NVME_VENDOR_ID);
    NvmeControllerInfo->BasicInfo.DeviceStrId.Prompt = STRING_TOKEN(STR_NVME_DEVICE_ID);

    return;
}

/**
    Add/Set the String to HII Database using HiiString Protocol

    @param HiiHandle   Handle of the HiiDatabase entry.
    @param String      Points to the string.
    @param StringId    points to the Id of the string.

    @return VOID

**/
VOID
NvmeHiiAddStringInternal (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  CHAR16 *        String,
    IN  EFI_STRING_ID   *StringId
)
{
    EFI_STATUS      Status;
    CHAR8*          Languages = NULL;
    UINTN           LangSize = 0;
    CHAR8*          CurrentLanguage;
    BOOLEAN         LastLanguage = FALSE;

    if (HiiString == NULL) {
        Status = pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
        if (EFI_ERROR(Status)) {
            return;
        }
    }
    if (gSupportedLanguages == NULL) {
        Status = HiiString->GetLanguages(HiiString, HiiHandle, Languages, &LangSize);
        if (Status == EFI_BUFFER_TOO_SMALL) {
            Status = pBS->AllocatePool(EfiBootServicesData, LangSize, (VOID**)&Languages);
            if (EFI_ERROR(Status)) {
                //not enough resources to allocate string
                return;
            }
            Status = HiiString->GetLanguages(HiiString, HiiHandle, Languages, &LangSize);
            if(EFI_ERROR(Status)) {
                return;
            }
        }
        gSupportedLanguages=Languages;
    } else {
        Languages=gSupportedLanguages;
    }
    
    if(Languages == NULL) {
    	return;
    }

    while(!LastLanguage) {
        //
        //point CurrentLanguage to start of new language
        //
        CurrentLanguage = Languages;
        while(*Languages != ';' && *Languages != 0)
            Languages++;

        //
        //Last language in language list
        //
        if (*Languages == 0) {
            LastLanguage = TRUE;
            if (*StringId == 0) {
                Status = HiiString->NewString(HiiString, HiiHandle, StringId, CurrentLanguage, NULL, String, NULL);
            } else {
                Status = HiiString->SetString(HiiString, HiiHandle, *StringId, CurrentLanguage, String, NULL);
            }
            if (EFI_ERROR(Status)) {
                return;
            }
        } else {
            //
            //put null-terminator
            //
            *Languages = 0;
            if (*StringId == 0) {
                Status = HiiString->NewString(HiiString, HiiHandle, StringId, CurrentLanguage, NULL, String, NULL);
            } else {
                Status = HiiString->SetString(HiiString, HiiHandle, *StringId, CurrentLanguage, String, NULL);
            }
            *Languages = ';';       //restore original character
            Languages++;
            if (EFI_ERROR(Status)) {
                return;
            }
        }
    }
}

/**
    Add the String to HII Database using HiiString Protocol

    @param HiiHandle   Handle of the HiiDatabase entry.
    @param String      Pointer to the string.
    @param StringId    pointer to the Id of the string.

    @return VOID

**/
VOID
NvmeHiiAddString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  CHAR16 *        String,
    IN  EFI_STRING_ID    *StringId
)
{
    NvmeHiiAddStringInternal(HiiHandle, String, StringId);
}

/**
  Nvme HiiCreateTextOpCode

  @param   StartOpCodeHandle  Points to the start opcode handle.
  @param   TextItem			  Points to the NVME_TEXT_ITEM.
  @param   ItemCnt			  Number of items to be connected.
    
  @return  VOID
**/
VOID
NvmeHiiCreateTextOpCode(
    IN VOID             *StartOpCodeHandle,    
    IN NVME_TEXT_ITEM   *TextItem,
    IN UINT32           ItemCnt
)
{
    UINT32 i = 0;
    
    for(i=0;i<ItemCnt;i++) {
        if((TextItem->Prompt != 0) || (TextItem->TextTwo !=0)) {
            HiiCreateTextOpCode(StartOpCodeHandle,
                                TextItem->Prompt,
                                TextItem->Help,
                                TextItem->TextTwo);
        }
        TextItem++;
    }
}

/**
  This function will return the Controller Name for Nvme to display
  in Setup, by matching PciRoot UID and Hardware device path of Nvme 
  Controller with information ported through Elink 
  NvmeControllerNamelist.
    
  @param  NvmeCtrlDevPath Pointer to DevicePath of NvmeController  

  @return OemNvmeName     Pointer contains OEM Name for NvmeController

**/
CHAR8*
GetNvmeControllerName (EFI_DEVICE_PATH_PROTOCOL *NvmeCtrlDevPath)
{
    EFI_STATUS                    Status;
    PCI_DEVICE_PATH               *PciDpNode;
    UINT8                         IndexElink;
    UINT8                         IndexPciNode;
    UINT8                         Length = 0;
    CHAR8                         *OemNvmeName = NULL;
    CHAR8                         *String = NULL;
    NVME_CONTROLLER_NAME_LIST     NvmeControllerNamelist[] = { NVME_CONTROLLER_NAME_ELINK_LIST
                                                            { {0x0},{{0xFF,0xFF}}, NULL }
                                                            };
	
    ACPI_HID_DEVICE_PATH *AcpiDp = (ACPI_HID_DEVICE_PATH *)NvmeCtrlDevPath;
    
    // Check whether the device path given through Elink is valid.  
    if (NvmeControllerNamelist[0].PciDevFunInfo[0].Dev != 0xFF){
       
        for (IndexElink=0;IndexElink < (sizeof(NvmeControllerNamelist)/sizeof(NVME_CONTROLLER_NAME_LIST)); IndexElink++) {
            
            //Match the PciRoot UID from AcpiDevPath and the ported UID value from NvmeControllerNamelist ELINK.           
            if (NvmeControllerNamelist[IndexElink].PciRootUID == AcpiDp->UID) {
                
                //From NvmeCtrlDevPath, get the first node of PCI Hardware device path 	
                while(!isEndNode (NvmeCtrlDevPath))	{
                    if ((NvmeCtrlDevPath->Type == HARDWARE_DEVICE_PATH) &&
                            (NvmeCtrlDevPath->SubType == HW_PCI_DP)) {
                        break;
                    }
                    NvmeCtrlDevPath = NEXT_NODE(NvmeCtrlDevPath);
                }
                //Match the Hardware DevicePath obtained for Nvme controller among all DevicePath ported through Elink(NvmeControllerNamelist)  . 
                //If proper match found, then loads the OemNvmeName with the ControllerName ported through Elink.
                IndexPciNode = 0;
                PciDpNode = (PCI_DEVICE_PATH*)NvmeCtrlDevPath;
                do {
                    if ((NvmeControllerNamelist[IndexElink].PciDevFunInfo[IndexPciNode].Dev == PciDpNode->Device) && 
                            (NvmeControllerNamelist[IndexElink].PciDevFunInfo[IndexPciNode].Fun == PciDpNode->Function)){ 
                    
                        IndexPciNode++;
                        PciDpNode = (PCI_DEVICE_PATH*)NEXT_NODE(&PciDpNode->Header);
                        
                    } else {
                        break;
                    }
                } while((NvmeControllerNamelist[IndexElink].PciDevFunInfo[IndexPciNode].Dev != 0xFF) && \
                            (PciDpNode->Header.Type == HARDWARE_DEVICE_PATH));
                
                //Checks whether there is no more Hardware DevicePath is available from DevicePath  and Elink for Matching
                if((NvmeControllerNamelist[IndexElink].PciDevFunInfo[IndexPciNode].Dev == 0xFF)&& (PciDpNode->Header.Type != HARDWARE_DEVICE_PATH)) {
                    OemNvmeName = NvmeControllerNamelist[IndexElink].ControllerName;
                    
                    while (*OemNvmeName != '\0') {
                        Length++;
                        OemNvmeName++;
                    }
                    Status = pBS->AllocatePool( EfiBootServicesData,(UINTN)(Length + 1),(VOID **)&OemNvmeName); 
                    if (EFI_ERROR(Status)){
                        DEBUG((DEBUG_ERROR,"Error in AllocatePool for OemNvmeName. Status = %r \n",Status)); 
                    }
                    pBS->SetMem(OemNvmeName,(Length + 1),0);
                    pBS->CopyMem(OemNvmeName,(NvmeControllerNamelist[IndexElink].ControllerName),(Length + 1));
                }
            } 
        } // End of for loop
    } 
    return  OemNvmeName;
}

/**
  Collects details about connected NVMe controllers

  @param   VOID

  @retval  EFI_SUCEESS   controller details are collected and updated successfully.
  @return  Other         Return error status.

**/

EFI_STATUS 
GetNvmeDeviceDetails(VOID) {
    EFI_STATUS                            Status;
    UINTN                                 HandleCount;
    EFI_HANDLE                            *HandleBuffer;
    UINTN                                 Index, i;
    EFI_PCI_IO_PROTOCOL                   *PciIO;
    NVME_CONTROLLER_INFORMATION           *NvmeControllerInfo = NULL;
    CHAR16                                String[MAX_NVME_DEVICE_INFO_LENGTH]; 
    CHAR8                                 DeviceString[MAX_NVME_DEVICE_INFO_LENGTH];
    UINT32                                PciVidDid;
    AMI_NVME_CONTROLLER_PROTOCOL          *NvmeController = NULL;
    UINT64                                NameSpaceSizeInBytes = 0;
    UINTN                                 RemainderInBytes = 0;
    UINT32                                NameSpaceSizeInGB = 0;
    UINT32                                TotalNameSpaceSizeInGB = 0;
    UINTN                                 NumTenthsOfGB = 0;
    UINTN                                 TotalNumTenthsOfGB = 0;
    NVME_TEXT_ITEM                        *NvmeTextItem = NULL;
    ACTIVE_NAMESPACE_DATA                 *ActiveNameSpaceData;
    LIST_ENTRY                            *LinkData;
    UINT32                                Nsid = 1;

    // Initializing the List to store the Nvme device details
    InitializeListHead(&gNvmeControllerList);
    
    // Locates all NVMe controller handles
    Status = pBS->LocateHandleBuffer(
                         ByProtocol,
                         &gAmiNvmeControllerProtocolGuid,
                         NULL,
                         &HandleCount,
                         &HandleBuffer
                        );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
           
    for (Index = 0; Index < HandleCount; Index++) {
        Status = pBS->HandleProtocol(
                               HandleBuffer[Index],
                               &gAmiNvmeControllerProtocolGuid,
                               (VOID *)&NvmeController
                               );
                 
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) {
            continue;
        }
         
        Status = pBS->HandleProtocol(
                              HandleBuffer[Index],
                              &gEfiPciIoProtocolGuid,
                              (VOID *)&PciIO
                              );
                            
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) {
            continue;
        }
         
        // Allocating memory for storing the Controller details
        Status = pBS->AllocatePool (EfiBootServicesData,
                                          sizeof(NVME_CONTROLLER_INFORMATION),
                                          (VOID**)&NvmeControllerInfo
                                           );
        if(EFI_ERROR(Status)) {
		    ASSERT_EFI_ERROR(Status);
            continue;
        }
        
        pBS->SetMem(NvmeControllerInfo, sizeof(NVME_CONTROLLER_INFORMATION), 0);
         
        Status = pBS->AllocatePool (EfiBootServicesData,
                                     sizeof(NVME_TEXT_ITEM) * NvmeController->IdentifyControllerData->NN,
                                     (VOID**)&NvmeControllerInfo->ActiveNameSpace
                                     );
        if(EFI_ERROR(Status)) {
		    ASSERT_EFI_ERROR(Status);
            continue;
        }
                  
        pBS->SetMem(NvmeControllerInfo->ActiveNameSpace, sizeof(NVME_TEXT_ITEM) * NvmeController->IdentifyControllerData->NN, 0);
         
        InitializeNvmeItemStrId(NvmeControllerInfo);
        
        NvmeControllerInfo->NvmeHandle = HandleBuffer[Index];
        
        // Get Model Number
        pBS->SetMem(DeviceString, sizeof(DeviceString), 0);
        pBS->CopyMem(DeviceString, NvmeController->IdentifyControllerData->ModelNumber, 40);
                 
        // Remove the spaces from the end of the device name
        DeviceString[40] =0;
        for (i = 39; i > 0; i-- ) {
            if (DeviceString[i] != 0x20) { 
                break;
            }
            DeviceString[i] = 0;
        }
        pBS->SetMem(String, sizeof(String), 0);
        Swprintf(String, L"%a", DeviceString);
        NvmeHiiAddString(gNvmeHiiHandle, String, &NvmeControllerInfo->BasicInfo.ModelNumberStrId.TextTwo);
                 
        Status = PciIO->GetLocation ( PciIO,
                                      &NvmeControllerInfo->SegNum,
                                      &NvmeControllerInfo->BusNum,
                                      &NvmeControllerInfo->DevNum,
                                      &NvmeControllerInfo->FunNum
                                      );
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) {
            continue;
        }
        pBS->SetMem(String, sizeof(String), 0);
        // Get  SegNum, Bus, Device, Function Number
        Swprintf(String, L"%02X:%02X:%02X:%02X", NvmeControllerInfo->SegNum,NvmeControllerInfo->BusNum, NvmeControllerInfo->DevNum, NvmeControllerInfo->FunNum);
        NvmeHiiAddString(gNvmeHiiHandle, String, &NvmeControllerInfo->BasicInfo.BusStrId.TextTwo);
                 
        // Retrieves Nvme controllers PCI Vendor ID and Device ID of the controller.
        Status = PciIO->Pci.Read (
                                 PciIO,
                                 EfiPciIoWidthUint32,
                                 PCI_VID_DID,
                                 1,
                                 &PciVidDid
                                 );
        if(EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR(Status);
            continue;
        }
         
        NvmeControllerInfo->VendorId = (UINT16)(PciVidDid & 0xffff);
        NvmeControllerInfo->DeviceId = (UINT16)(PciVidDid >> 16);
         
        // Publish Vendor ID & Device ID
        pBS->SetMem(String, sizeof(String), 0);
        Swprintf(String, L"%04X", NvmeControllerInfo->VendorId);
        NvmeHiiAddString(gNvmeHiiHandle, String, &NvmeControllerInfo->BasicInfo.VendorStrId.TextTwo);
        
        pBS->SetMem(String, sizeof(String), 0);
        Swprintf(String, L"%04X", NvmeControllerInfo->DeviceId);
        NvmeHiiAddString(gNvmeHiiHandle, String, &NvmeControllerInfo->BasicInfo.DeviceStrId.TextTwo);
         
        NvmeTextItem = (NVME_TEXT_ITEM *)NvmeControllerInfo->ActiveNameSpace;
         
         for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
                     LinkData != &NvmeController->ActiveNameSpaceList; LinkData = LinkData->ForwardLink) {
               
            ActiveNameSpaceData = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
            
            // NameSpaceSize In Bytes
            NameSpaceSizeInBytes = Mul64((UINT64)ActiveNameSpaceData->IdentifyNamespaceData.NSIZE,\
                                             ActiveNameSpaceData->NvmeBlockIO.Media->BlockSize);
                                          
            // NameSpaceSizeInGB is NameSpaceSizeInBytes / 1 GB (1 Decimal GB = 10^9 bytes)
            NameSpaceSizeInGB = (UINT32) Div64(NameSpaceSizeInBytes, 1000000000,\
                                                                  &RemainderInBytes);
            TotalNameSpaceSizeInGB += NameSpaceSizeInGB;
                                      
            // Convert the Remainder, which is in bytes, to number of tenths of a Decimal GB.
            NumTenthsOfGB = RemainderInBytes / 100000000;
            TotalNumTenthsOfGB += NumTenthsOfGB;
            
            pBS->SetMem(String, sizeof(String), 0);
            Swprintf(String, L"Namespace: %d", ActiveNameSpaceData->ActiveNameSpaceID);
            NvmeHiiAddString(gNvmeHiiHandle, String, &NvmeTextItem->Prompt);
            
            pBS->SetMem(String, sizeof(String), 0);
            Swprintf(String, L"Size: %d.%d GB", NameSpaceSizeInGB, NumTenthsOfGB);
            NvmeHiiAddString(gNvmeHiiHandle, String, &NvmeTextItem->TextTwo);
            
            NvmeTextItem++;
            Nsid++;
         }
            
        // Active Namespace count
        NvmeControllerInfo->NamespaceCount = Nsid - 1;
            
        Swprintf(String, L"%d.%d GB", TotalNameSpaceSizeInGB, TotalNumTenthsOfGB);
        NvmeHiiAddString(gNvmeHiiHandle, String, &NvmeControllerInfo->BasicInfo.SizeStrId.TextTwo);
        TotalNameSpaceSizeInGB = 0;
        TotalNumTenthsOfGB = 0;
        
        //Update Device SelfTest details 
        NvmeControllerInfo->DeviceSelfTest = (NvmeController->IdentifyControllerData->OACS & BIT4) ? TRUE : FALSE;
        NvmeControllerInfo->ExtendedDevSelfTestTime = NvmeController->IdentifyControllerData->EDSTT;
        
        InsertTailList(&gNvmeControllerList, &NvmeControllerInfo->NvmeControllerLink);
    }
            
    if(HandleBuffer != NULL) {
        pBS->FreePool(HandleBuffer);
    }
            
             
    return Status;
}
/**
    Callback function to send Device Self test command
    
    @param ControllerNum     Controller Number

    @retval VOID
**/

VOID
DeviceSelfTestCallback(
    UINT16      ControllerNum
)
{

    UINT16                                 Count = 0;
    NVME_CONTROLLER_INFORMATION            *NvmeControllerInfo = NULL;
    EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL     *NvmePassthru;
    EFI_STATUS                             Status;
    VOID                                   *Buffer;
    UINTN                                  BufferLength = DEVICE_SELF_TEST_LOG_LENGTH;
    NVME_DEVICE_SELF_TEST_LOG              *SelfTestLog;
    LIST_ENTRY                             *Link;
    AMI_POST_MGR_KEY                       OutKey;
    UINT8                                  StatusByte;
    EFI_STRING                             PopMsgTitle = NULL;
    EFI_STRING                             PopMsg = NULL;
    NVME_DEVICE_SELF_TEST_RESULT           SelfTestResult;
    UINT8                                  SelectKey;
    
    
    // Traverse linked list to get controller data
    for (Link = gNvmeControllerList.ForwardLink; Link != &gNvmeControllerList; Link = Link->ForwardLink) {
        NvmeControllerInfo = BASE_CR(Link, NVME_CONTROLLER_INFORMATION, NvmeControllerLink );
        if(Count == ControllerNum) {
            break;
        }
        Count++;
    }
       
    if(NvmeControllerInfo == NULL) {
        return;
    }
    
    Status = pBS->HandleProtocol(
                          NvmeControllerInfo->NvmeHandle,
                          &gEfiNvmExpressPassThruProtocolGuid,
                          (VOID *)&NvmePassthru
                          );

    if (EFI_ERROR(Status)){
        DEBUG((DEBUG_ERROR,"SelfTest: NvmePassthru not found :  %r\n", Status));
        return;
    }

    // Send Device Self Test command
    Status = NvmeDeviceSelfTest(NvmePassthru, gNvmeDynamicSetupData.SelfTestOperation, gNvmeDynamicSetupData.SelfTestAction); 
    if (EFI_ERROR(Status)){
        // Pop-up box to display command failure message
        PopMsgTitle = NvmeSecHiiGetString(gNvmeHiiHandle, STRING_TOKEN(STR_NVME_SELF_TEST_RESULT_TITLE));
        PopMsg = NvmeSecHiiGetString(gNvmeHiiHandle, STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_FAIL));
        gAmiPostMgr->DisplayMsgBox(PopMsgTitle,
                                PopMsg,
                                MSGBOX_TYPE_OK,
                                NULL );
        gBS->FreePool(PopMsgTitle);
        gBS->FreePool(PopMsg);
        return;
    }
    Status = pBS->AllocatePool (EfiBootServicesData,
                                 BufferLength,
                                 (VOID**)&Buffer
                                );
    if (EFI_ERROR(Status)){
        return;
    }
  
    SelfTestProgressInit();
    SelfTestLog = (NVME_DEVICE_SELF_TEST_LOG *)Buffer;
    do {
        
        pBS->SetMem( Buffer, BufferLength, 0);
        
        BufferLength = DEVICE_SELF_TEST_LOG_LENGTH;
        Status = NvmeGetLogPage(NvmePassthru, NVME_LOG_DEVICE_SELF_TEST, Buffer, &BufferLength);
        if (EFI_ERROR(Status)){
            if (Buffer) {
                pBS->FreePool(Buffer);
            }
            SelfTestProgressClose();
            // Pop-up box to display command failure message
            PopMsgTitle = NvmeSecHiiGetString(gNvmeHiiHandle, STRING_TOKEN(STR_NVME_SELF_TEST_RESULT_TITLE));
            PopMsg = NvmeSecHiiGetString(gNvmeHiiHandle, STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_FAIL));
            gAmiPostMgr->DisplayMsgBox(PopMsgTitle,
                                    PopMsg,
                                    MSGBOX_TYPE_OK,
                                    NULL );
            gBS->FreePool(PopMsgTitle);
            gBS->FreePool(PopMsg);
            return;
        }
        
        OutKey.Key.ScanCode = 0;
        SelfTestProgressUpdate((UINTN)SelfTestLog->CurrentSelfTestComp, &OutKey);

        if (OutKey.Key.ScanCode == SCAN_ESC) {
            // Abort Self Test
            Status = NvmeDeviceSelfTest(NvmePassthru, Abort, 0);
            break;
        }
        
        // SelfTest operation pending?
        if (!SelfTestLog->CurrentSelfTestOp) {
            break;
        }
        
        gBS->Stall(1000000); // 1 sec delay
        
    }while (1);
        
    if (Buffer) {
        pBS->FreePool(Buffer);
        Buffer = NULL;
    }
    
    SelfTestProgressClose();
    
    //Get the result of recently performed Self test
    GetSelfTestCode(NvmeControllerInfo->NvmeHandle,
                    gNvmeDynamicSetupData.SelfTestOperation, 
                    &SelfTestResult, 
                    &StatusByte);

    // Display the Self test resultant data for Failure case
    if (GetSelfTestResultStringID (StatusByte) == STR_NVME_DEVICE_SELFTEST_FAIL){
        
        BufferLength = 2048;
        Buffer=AllocateZeroPool(BufferLength);
        if (!Buffer){
            return;
        }
        PopMsgTitle = NvmeSecHiiGetString(gNvmeHiiHandle, STRING_TOKEN(STR_NVME_SELF_TEST_RESULT_TITLE));
        
        UnicodeSPrint(Buffer, BufferLength,                                              L"%s\n",  L"Status                  : FAILED");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) - 2), BufferLength,    L"%s",    L"Device Self-Test Status : ");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) - 2), BufferLength,    L"%2X     \n", SelfTestResult.SelfTestStatusResult);
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) - 2), BufferLength,    L"%s",    L"Segment Number          : ");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%02X    \n", SelfTestResult.SegmentNumber);
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%s",    L"Valid Diagnostic Info   : ");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%02X    \n", SelfTestResult.DiagInfo);
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%s",    L"Power On Hours :");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%08lX\n", SelfTestResult.Poh);
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%s",    L"NSID                    : ");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%04X\n", SelfTestResult.Nsid);
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%s",    L"Failing LBA : ");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%08lX\n", SelfTestResult.FailingLba);
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%s",    L"StatusCode Type         : ");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%02X    \n", SelfTestResult.StatusCodeType);
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%s",    L"StatusCode              : ");
        UnicodeSPrint((UINT16 *)((UINT8*)Buffer + StrSize(Buffer) -2), BufferLength,     L"%02X    \n", SelfTestResult.StatusCode);
        
        gAmiPostMgr->DisplayMsgBox(PopMsgTitle,
                                Buffer,
                                MSGBOX_TYPE_OK,
                                &SelectKey );
        gBS->FreePool(PopMsgTitle);
        if (Buffer) {
            pBS->FreePool(Buffer);
        }
    }
    else {
        // Pop-up box to display result
        // Show warning popup box when Security is not enabled
        PopMsgTitle = NvmeSecHiiGetString(gNvmeHiiHandle, STRING_TOKEN(STR_NVME_SELF_TEST_RESULT_TITLE));
        PopMsg = NvmeSecHiiGetString(gNvmeHiiHandle, GetSelfTestResultStringID (StatusByte));
        gAmiPostMgr->DisplayMsgBox(PopMsgTitle,
                                PopMsg,
                                MSGBOX_TYPE_OK,
                                NULL );
        gBS->FreePool(PopMsgTitle);
        gBS->FreePool(PopMsg);
    }

    
    
    // Update the setup variable structure
    if (gNvmeDynamicSetupData.SelfTestOperation == Short) {
        gNvmeDynamicSetupData.SelfTestShortResult =  StatusByte;
    }
    else {
        gNvmeDynamicSetupData.SelfTestExtResult = StatusByte;
    }
    NvmeRefreshSetupData();
}

/**
  Create EFI_IFR_SUPPRESS_IF_OP/EFI_IFR_GRAY_OUT_IF_OP opcode with
  TRUE/FALSE condition.

  If OpCodeHandle is NULL, then ASSERT().

  @param[in]  OpCodeHandle          Handle to the buffer of opcodes.
  @param[in]  Value                 Condition value - TRUE/FALSE
  @param[in]  Suppress              1: EFI_IFR_SUPPRESS_IF_OP, 0: EFI_IFR_GRAY_OUT_IF_OP

  @retval NULL   There is not enough space left in Buffer to add the opcode.
  @retval Other  A pointer to the created opcode.

**/
static
UINT8 *
EFIAPI
AmiHiiCreateSuppresGrayVal(
  IN VOID               *OpCodeHandle,
  BOOLEAN               Value,
  BOOLEAN               Suppress    //if TRUE Suppress; False Gray out.
  )
{
    EFI_IFR_OP_HEADER   *Condition;
    EFI_IFR_OP_HEADER   *Header;
    UINT8               *Buffer=NULL;
    UINTN               sz;

    //Allocate buffer for SUPPRESS_IF/GRAY_OUT_IF opcode + EFI_IFR_EQ_ID_VAL for suppress;
    sz=sizeof(EFI_IFR_OP_HEADER)+sizeof(EFI_IFR_OP_HEADER);
    Buffer=AllocateZeroPool(sz);
    if(Buffer==NULL) {
        ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
        return Buffer;
    }

    //Init Pointers;
    Header=(EFI_IFR_OP_HEADER*)Buffer;
    Condition=(EFI_IFR_OP_HEADER*)(Header+1);

    //Update OpCodes...
    //EFI_IFR_SUPPRESS_IF_OP/EFI_IFR_GRAY_OUT_IF_OP first;
    if(Suppress) Header->OpCode=EFI_IFR_SUPPRESS_IF_OP;
    else Header->OpCode=EFI_IFR_GRAY_OUT_IF_OP;
    Header->Length=sizeof(EFI_IFR_OP_HEADER);
    Header->Scope=1;


    if(Value) Condition->OpCode=EFI_IFR_TRUE_OP;
    else Condition->OpCode=EFI_IFR_FALSE_OP;
    Condition->Scope=0;
    Condition->Length=sizeof(EFI_IFR_OP_HEADER);

    return HiiCreateRawOpCodes(OpCodeHandle,Buffer,sz);
}

/**
  This function will dynamically add VFR contents to HDD Acoustic BIOS setup page
  using HII library functions.

  @param  ControllerNum  Number of the controller to which the data has to be
            			 updated.

  @return VOID

**/
VOID
NvmeInitDynamicSubFormContents(
    UINT16      ControllerNum
)
{
    VOID                            *StartOpCodeHandle;
    VOID                            *EndOpCodeHandle;
    VOID                            *SelfTestOptionOpCodeHandle;
    EFI_IFR_GUID_LABEL              *StartLabel;
    EFI_IFR_GUID_LABEL              *EndLabel;
    UINT16                          Count = 0;
    NVME_CONTROLLER_INFORMATION     *NvmeControllerInfo = NULL;
    LIST_ENTRY                      *Link;
    UINT8                           StatusByte;
  
    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    EndOpCodeHandle = HiiAllocateOpCodeHandle ();
    
    if (!StartOpCodeHandle) {
        return;
    }
    
    // Create Hii Extended Label OpCode as the start and end opcode
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
	            StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
 	            EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
	    
    if(StartLabel == NULL || EndLabel == NULL) {
        return;
    }
	    
    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    StartLabel->Number = NVME_SUB_FORM_LABEL_START;
    EndLabel->Number = NVME_SUB_FORM_LABEL_END;
   
    // Traverse linked list to get controller data
    for (Link = gNvmeControllerList.ForwardLink; Link != &gNvmeControllerList; Link = Link->ForwardLink) {
        NvmeControllerInfo = BASE_CR(Link, NVME_CONTROLLER_INFORMATION, NvmeControllerLink );
        if(Count == ControllerNum) {
            break;
        }
        Count++;
    }
       
    if(NvmeControllerInfo == NULL) {
        return;
    }
   
    // Publish Basic Info
    NvmeHiiCreateTextOpCode(StartOpCodeHandle, 
                           (NVME_TEXT_ITEM *)&(NvmeControllerInfo->BasicInfo),
                           (UINT32)(sizeof(NVME_BASIC_INFO)/sizeof(NVME_TEXT_ITEM)));
   
    HiiCreateSubTitleOpCode(StartOpCodeHandle,
                           STRING_TOKEN(STR_EMPTY),
                           STRING_TOKEN(STR_EMPTY),
                           0,0);
   
    NvmeHiiCreateTextOpCode(StartOpCodeHandle, 
                           (NVME_TEXT_ITEM *)NvmeControllerInfo->ActiveNameSpace,
                           (UINT32)NvmeControllerInfo->NamespaceCount);
	    
    //
    //Device SelfTest options
    if (NvmeControllerInfo->DeviceSelfTest) {

        HiiCreateSubTitleOpCode(StartOpCodeHandle,
                               STRING_TOKEN(STR_EMPTY),
                               STRING_TOKEN(STR_EMPTY),
                               0,0);
        
        HiiCreateSubTitleOpCode(StartOpCodeHandle, 
                               STRING_TOKEN(STR_DEVICE_SELF_TEST_TITLE),
                               STRING_TOKEN(STR_EMPTY),
                               0,0);
        
        // Create an option for user to select Short or ext Self test
        SelfTestOptionOpCodeHandle = HiiAllocateOpCodeHandle ();
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_SHORT_SELFTEST),
                (EFI_IFR_OPTION_DEFAULT | EFI_IFR_OPTION_DEFAULT_MFG),
                EFI_IFR_NUMERIC_SIZE_1,
                1
                );
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_EXTENDED_SELFTEST),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                2
                );
        
        
        HiiCreateOneOfOpCode (StartOpCodeHandle,
                               (EFI_QUESTION_ID)SELFTEST_OPTION_QUESTION_ID_00 + ControllerNum,
                               NVME_DYNAMIC_SETUP_VARIABLE_ID,
                               (UINT16)EFI_FIELD_OFFSET(NVME_DYNAMIC_SETUP_CONFIGURATION, SelfTestOperation),
                               STRING_TOKEN(STR_NVME_DEVICE_SELFTEST),
                               STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_HELP),
                               (UINT8)EFI_IFR_FLAG_CALLBACK,
                               (UINT8)EFI_IFR_NUMERIC_SIZE_1,
                               SelfTestOptionOpCodeHandle,
                               NULL
                               );

        gBS->FreePool(SelfTestOptionOpCodeHandle);
        
        // Create an option for user to perform self test on Controller or Controller and all active Namespaces
		SelfTestOptionOpCodeHandle = HiiAllocateOpCodeHandle ();
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_SHORT_SELFTEST_CTRL_ONLY),
                (EFI_IFR_OPTION_DEFAULT | EFI_IFR_OPTION_DEFAULT_MFG),
                EFI_IFR_NUMERIC_SIZE_1,
                0
                );
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_EXTENDED_SELFTEST_NAMESPACE),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                1
                );
        
        
        HiiCreateOneOfOpCode (StartOpCodeHandle,
                               (EFI_QUESTION_ID)SELFTEST_ACTION_QUESTION_ID_00 + ControllerNum,
                               NVME_DYNAMIC_SETUP_VARIABLE_ID,
                               (UINT16)EFI_FIELD_OFFSET(NVME_DYNAMIC_SETUP_CONFIGURATION, SelfTestAction),
                               STRING_TOKEN(STR_NVME_SHORT_SELFTEST_TEST_ACTION),
                               STRING_TOKEN(STR_NVME_SHORT_SELFTEST_TEST_ACTION_HELP),
                               (UINT8)EFI_IFR_FLAG_CALLBACK,
                               (UINT8)EFI_IFR_NUMERIC_SIZE_1,
                               SelfTestOptionOpCodeHandle,
                               NULL
                               );

        gBS->FreePool(SelfTestOptionOpCodeHandle);
        
        
        HiiCreateActionOpCode (
            StartOpCodeHandle,
            (EFI_QUESTION_ID)NVME_DEVICE_DEVICE_SELFTEST_BASE_KEY_00 + ControllerNum,
            STRING_TOKEN (STR_NVME_RUN_DEVICE_SELFTEST),
            STRING_TOKEN (STR_NVME_RUN_DEVICE_SELFTEST_HELP),
            EFI_IFR_FLAG_CALLBACK,
            0
            );
        
        HiiCreateSubTitleOpCode(StartOpCodeHandle,
                               STRING_TOKEN(STR_EMPTY),
                               STRING_TOKEN(STR_EMPTY),
                               0,0);
        
        // Initialize default variables
        gNvmeDynamicSetupData.SelfTestOperation = Short;
        
		//Get the recent self test log from device for initial display
        GetSelfTestCode(NvmeControllerInfo->NvmeHandle, Short, NULL, &StatusByte);
        gNvmeDynamicSetupData.SelfTestShortResult = StatusByte;
        
        GetSelfTestCode(NvmeControllerInfo->NvmeHandle, Extended, NULL, &StatusByte);
        gNvmeDynamicSetupData.SelfTestExtResult = StatusByte;
        
        // Create grayed out Short SelfTest Result Question
        SelfTestOptionOpCodeHandle = HiiAllocateOpCodeHandle ();

        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_PASS),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                0
                );
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_ABORTED),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                1
                );

        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_FAIL),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                2
                );
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_NA),
                (EFI_IFR_OPTION_DEFAULT | EFI_IFR_OPTION_DEFAULT_MFG),
                EFI_IFR_NUMERIC_SIZE_1,
                0xF
                );

        // Gray Out
        AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, FALSE);
        
        HiiCreateOneOfOpCode (StartOpCodeHandle,
                               (EFI_QUESTION_ID)SELFTEST_SHORT_RESULT_QUESTION_ID_00 + ControllerNum,
                               NVME_DYNAMIC_SETUP_VARIABLE_ID,
                               (UINT16)EFI_FIELD_OFFSET(NVME_DYNAMIC_SETUP_CONFIGURATION, SelfTestShortResult),
                               STRING_TOKEN(STR_NVME_SHORT_DEVICE_SELFTEST),
                               STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_HELP),
                               (UINT8)EFI_IFR_FLAG_CALLBACK,
                               (UINT8)EFI_IFR_NUMERIC_SIZE_1,
                               SelfTestOptionOpCodeHandle,
                               NULL
                               );
        
        AmiHiiTerminateScope(StartOpCodeHandle);
        
        gBS->FreePool(SelfTestOptionOpCodeHandle);
        
        // Create grayed out Extented SelfTest Result Question
        SelfTestOptionOpCodeHandle = HiiAllocateOpCodeHandle ();
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_PASS),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                0
                );
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_ABORTED),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                1
                );

        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_FAIL),
                0,
                EFI_IFR_NUMERIC_SIZE_1,
                2
                );
        
        HiiCreateOneOfOptionOpCode (
                SelfTestOptionOpCodeHandle,
                STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_NA),
                (EFI_IFR_OPTION_DEFAULT | EFI_IFR_OPTION_DEFAULT_MFG),
                EFI_IFR_NUMERIC_SIZE_1,
                0xF
                );
        
        // Gray Out
        AmiHiiCreateSuppresGrayVal(StartOpCodeHandle, TRUE, FALSE);
        
        HiiCreateOneOfOpCode (StartOpCodeHandle,
                               (EFI_QUESTION_ID)SELFTEST_EXT_RESULT_QUESTION_ID_00 + ControllerNum,
                               NVME_DYNAMIC_SETUP_VARIABLE_ID,
                               (UINT16)EFI_FIELD_OFFSET(NVME_DYNAMIC_SETUP_CONFIGURATION, SelfTestExtResult),
                               STRING_TOKEN(STR_NVME_EXT_DEVICE_SELFTEST),
                               STRING_TOKEN(STR_NVME_DEVICE_SELFTEST_HELP),
                               (UINT8)EFI_IFR_FLAG_CALLBACK,
                               (UINT8)EFI_IFR_NUMERIC_SIZE_1,
                               SelfTestOptionOpCodeHandle,
                               NULL
                               );
        
        //close scope
        AmiHiiTerminateScope(StartOpCodeHandle);
        
        gBS->FreePool(SelfTestOptionOpCodeHandle);
        SelfTestOptionOpCodeHandle = NULL;

        
    }
    
    // We are done!!
    HiiUpdateForm (
	     		gNvmeHiiHandle,
	            &gNvmeFormSetGuid,
	            NVME_SUB_FORM_ID,
	            StartOpCodeHandle,
	            EndOpCodeHandle
	            );

    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    
    // Set the new setup configuration data
    pRS->SetVariable(L"NvmeDynamicSetupVar",
                    &gNvmeDynamicSetupDataGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(NVME_DYNAMIC_SETUP_CONFIGURATION),
                    &gNvmeDynamicSetupData);

    // Refresh browser data
    HiiLibSetBrowserData( sizeof(NVME_DYNAMIC_SETUP_CONFIGURATION),
                        &gNvmeDynamicSetupData,
                        &gNvmeDynamicSetupDataGuid,
                        L"NvmeDynamicSetupVar");
    
    return;

}


/**
  This function will dynamically add VFR contents to Nvme BIOS setup page
  using HII library functions.

  @param  Event    Event whose notification function is being invoked.
  @param  Context  pointer to the notification function's context.

  @return VOID

**/
VOID
NvmeInitDynamicMainFormContents(
    IN EFI_EVENT Event,
    IN VOID *Context
)
{
    EFI_STATUS                         Status;
    LIST_ENTRY                         *Link;
    NVME_CONTROLLER_INFORMATION        *NvmeControllerInfo = NULL;
    UINTN                              Count = 0;
    CHAR16                             String[MAX_NVME_DEVICE_INFO_LENGTH] ={0};
    EFI_STRING_ID                      StringId = 0;
    
    VOID                                *StartOpCodeHandle;
    VOID                                *EndOpCodeHandle;
    EFI_IFR_GUID_LABEL                  *StartLabel;
    EFI_IFR_GUID_LABEL                  *EndLabel;
    EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
    CHAR8                               *ControllerName = NULL;
    
    DEBUG((DEBUG_INFO,"\nNvmeInitDynamicMainFormContents Entry \n"));

    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    EndOpCodeHandle = HiiAllocateOpCodeHandle ();
   
    if(StartOpCodeHandle == NULL || EndOpCodeHandle == NULL) {
        return;
    }
   
    // Create Hii Extended Label OpCode as the start and end opcode
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
   
    if(StartLabel == NULL || EndLabel == NULL) {
        return;
    }
   
    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    StartLabel->Number = NVME_MAIN_FORM_LABEL_START;
    EndLabel->Number = NVME_MAIN_FORM_LABEL_END;
   
    // Collect details about connected NVMe controllers
    Status = GetNvmeDeviceDetails();
    if(!EFI_ERROR(Status)) {
       for (Link = gNvmeControllerList.ForwardLink; Link != &gNvmeControllerList; Link = Link->ForwardLink, Count++) {
           NvmeControllerInfo = BASE_CR(Link, NVME_CONTROLLER_INFORMATION, NvmeControllerLink );
           
           
           Status = pBS->HandleProtocol(
                                      NvmeControllerInfo->NvmeHandle,
                                      &gEfiDevicePathProtocolGuid,
                                      (VOID *)&DevicePath
                                      );
           ASSERT_EFI_ERROR(Status);
                 
           //Gets the OEM name for Nvme Controller
           ControllerName = GetNvmeControllerName (DevicePath);
           
           if(ControllerName){
               Swprintf(String, L"%a",ControllerName);
               NvmeHiiAddString(gNvmeHiiHandle, String, &StringId);
           }
           
           if(NvmeControllerInfo->BasicInfo.ModelNumberStrId.TextTwo != 0) {
               // Add goto control
               HiiCreateGotoOpCode (
                   StartOpCodeHandle,
                   NVME_SUB_FORM_ID,
                   ControllerName ? StringId : NvmeControllerInfo->BasicInfo.ModelNumberStrId.TextTwo,
                   STRING_TOKEN(STR_EMPTY),
                   EFI_IFR_FLAG_CALLBACK,
                   (EFI_QUESTION_ID)(NVME_DEVICE_BASE_KEY_00 + Count)
                   );
           }
           
           if(ControllerName != NULL){
               pBS->FreePool(ControllerName);
               ControllerName = NULL;
               StringId = 0;
               pBS->SetMem(String, sizeof(String), 0);
           }
           
       }
    } else {
       
        Swprintf(String, L"No NVME Device Found");
        NvmeHiiAddString(gNvmeHiiHandle, String, &StringId);
        HiiCreateSubTitleOpCode(StartOpCodeHandle,
                                StringId,
                                STRING_TOKEN(STR_EMPTY),
                                0,0);
        StringId = 0;
    }
       
    // We are done!! Updating form
    HiiUpdateForm (
                  gNvmeHiiHandle,
                  &gNvmeFormSetGuid,
                  NVME_MAIN_FORM_ID,
                  StartOpCodeHandle,
                  EndOpCodeHandle
                  );
    
    if (StartOpCodeHandle != NULL) {
        HiiFreeOpCodeHandle (StartOpCodeHandle);
        HiiFreeOpCodeHandle (EndOpCodeHandle);
    }
   
    DEBUG((DEBUG_INFO,"\nNvmeInitDynamicMainFormContents Exit \n"));
    
    return;
}


/**
  This function is called by Setup browser to perform callback

  @param This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action           Specifies the type of action taken by the browser.
  @param QuestionId       A unique value which is sent to the original exporting driver
                          so that it can identify the type of data to expect.
  @param Type             The type of value for the question.
  @param Value            A pointer to the data being sent to the original exporting driver.
  @param ActionRequest    On return, points to the action requested by the callback function.

  
  @retval EFI_SUCCESS     The callback successfully handled the action.
  @retval EFI_UNSUPPORTED The action taken by the browser is not supported.

**/
EFI_STATUS 
NvmeCallback(
    IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
    IN EFI_BROWSER_ACTION Action,
    IN EFI_QUESTION_ID KeyValue,
    IN UINT8 Type,
    IN EFI_IFR_TYPE_VALUE *Value,
    OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest
)
{
	UINT16      ControllerNum;
	
    if (Action != EFI_BROWSER_ACTION_CHANGING) {
        return EFI_UNSUPPORTED; 
    }
  
    if((KeyValue >= NVME_DEVICE_BASE_KEY_00) && (KeyValue < NVME_DEVICE_BASE_KEY_00 + MAX_NVME_DEVICE_COUNT)) {
        ControllerNum = KeyValue - NVME_DEVICE_BASE_KEY_00;        
        NvmeInitDynamicSubFormContents(ControllerNum);
    }
	 
    if((KeyValue >= NVME_DEVICE_DEVICE_SELFTEST_BASE_KEY_00) && (KeyValue < NVME_DEVICE_DEVICE_SELFTEST_BASE_KEY_00 + MAX_NVME_DEVICE_COUNT)) {
        ControllerNum = KeyValue - NVME_DEVICE_DEVICE_SELFTEST_BASE_KEY_00;        
        DeviceSelfTestCallback(ControllerNum);
    }
    
    // Can handle upto 32 devices
    if (KeyValue >= SELFTEST_OPTION_QUESTION_ID_00 && KeyValue < SELFTEST_OPTION_QUESTION_ID_00 + MAX_NVME_DEVICE_COUNT){
        gNvmeDynamicSetupData.SelfTestOperation = Value->u8;
    }
    
    if (KeyValue >= SELFTEST_ACTION_QUESTION_ID_00 && KeyValue < SELFTEST_ACTION_QUESTION_ID_00 + MAX_NVME_DEVICE_COUNT){
        gNvmeDynamicSetupData.SelfTestAction = Value->u8;
    }
    return EFI_SUCCESS;
}


/** 
  This function is the entry point. Registers callback on TSE event to update IFR data.

  @param  ImageHandle     The image handle.
  @param  SystemTable     The system table.

  @retval  EFI_SUCEESS    Setup page is loaded successfully.
  @return  Other          Return error status.

**/
EFI_STATUS
EFIAPI
NvmeDynamicSetupEntryPoint (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{

    EFI_STATUS      Status;
    VOID            *SetupRegistration;
    EFI_GUID        SetupEnterGuid = AMITSE_SETUP_ENTER_GUID;
    EFI_EVENT       SetupEnterEvent;
    
    InitAmiLib(ImageHandle, SystemTable);
    // Set the new setup configuration data
    pRS->SetVariable(L"NvmeDynamicSetupVar",
                    &gNvmeDynamicSetupDataGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(NVME_DYNAMIC_SETUP_CONFIGURATION),
                    &gNvmeDynamicSetupData);
    
    // Load setup page and update strings
    Status = LoadResources(ImageHandle, sizeof(SetupCallBack) / sizeof(CALLBACK_INFO), SetupCallBack, NULL);
    
    if (!EFI_ERROR(Status)) {
        
        RegisterProtocolCallback(
        &SetupEnterGuid, NvmeInitDynamicMainFormContents,
        NULL, &SetupEnterEvent, &SetupRegistration
        );
    
        gNvmeHiiHandle = SetupCallBack[0].HiiHandle;
    }    
    
    return Status;
    
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**           5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093      **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
