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

/** @file NvmeDynamicSetup.h
    Has macro definition.
     
**/


#ifndef _NVME_DYNAMIC_SETUP_H
#define _NVME_DYNAMIC_SETUP_H
#ifdef __cplusplus
extern "C" {
#endif

#define  NVME_DYNAMIC_SETUP_VARIABLE_GUID \
    { 0xbcbf9f27, 0x91ab, 0x4098, 0xa2, 0x1c, 0x8b, 0xc1, 0x6e, 0x08, 0x99, 0x01}

#pragma pack (1)
typedef struct _NVME_DYNAMIC_SETUP_CONFIGURATION {
    UINT8    SelfTestOperation;
    UINT8    SelfTestAction;
    UINT8    SelfTestShortResult;
    UINT8    SelfTestExtResult;
}NVME_DYNAMIC_SETUP_CONFIGURATION;

typedef struct {
    UINT8       CurrentSelfTestOp;
    UINT8       CurrentSelfTestComp;
    UINT16       Reserved2;
}NVME_DEVICE_SELF_TEST_LOG;

typedef struct {
    UINT8       SelfTestStatusResult:4;
    UINT8       SelfTestStatusCode:4;
    UINT8       SegmentNumber;
    UINT8       DiagInfo;
    UINT8       Reserved3;
    UINT64      Poh;
    UINT32      Nsid;
    UINT64      FailingLba;
    UINT8       StatusCodeType;
    UINT8       StatusCode;
    UINT16      Reserved26;
}NVME_DEVICE_SELF_TEST_RESULT;
#pragma pack ()

#ifndef VFRCOMPILE
#include <Token.h>
#include <Library/HiiLib.h>
#include <Library/AmiHiiUpdateLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/MdeModuleHii.h>
#include <Protocol/HiiString.h>
#include "Nvme/NvmeIncludes.h"
#include "Nvme/NvmeBus.h"
#include <AmiDxeLib.h>
#include "NvmeControllerNameElink.h"


//Highest count of PCI Hardwaredevice nodes(i.e DevNum and FunNum) 
//among the values ported through Elink(NvmeControllerNamNVME_DYNAMIC_SETUP_CONFIGURATIONelist)
#define HW_PCI_NODE_COUNT 4 

#define MAX_NVME_DEVICE_COUNT  32
#define MAX_NVME_DEVICE_INFO_LENGTH 50
#define PCI_VID_DID    0x00

#define NVME_LOG_DEVICE_SELF_TEST       6
#define DEVICE_SELF_TEST_LOG_LENGTH     1024
#define MAX_SELF_TEST_LOG_COUNT         20




typedef enum {  
  Short = 1,
  Extended = 2,
  Abort = 0xF
} DEVICE_SELF_TEST_TYPE;

typedef struct {
    UINT8 Dev;
    UINT8 Fun;
}NVME_PCI_LOCATION_INFO ;

typedef struct {
    UINT32                   PciRootUID;
    NVME_PCI_LOCATION_INFO   PciDevFunInfo[HW_PCI_NODE_COUNT];
    CHAR8                    *ControllerName;
}NVME_CONTROLLER_NAME_LIST;

typedef struct {
    EFI_STRING_ID     Prompt;
    EFI_STRING_ID     Help;
    EFI_STRING_ID     TextTwo;
} NVME_TEXT_ITEM;

typedef struct {
    NVME_TEXT_ITEM    BusStrId;
    NVME_TEXT_ITEM    ModelNumberStrId;
    NVME_TEXT_ITEM    SizeStrId;
    NVME_TEXT_ITEM    VendorStrId;
    NVME_TEXT_ITEM    DeviceStrId;
}NVME_BASIC_INFO;

typedef struct {
    UINTN                               BusNum;
    UINTN                               DevNum;
    UINTN                               FunNum;
    UINTN                               SegNum;
    UINT16                              VendorId;
    UINT16                              DeviceId;
    UINT32                              NamespaceCount;
    BOOLEAN                             DeviceSelfTest;
    UINT16                              ExtendedDevSelfTestTime;
    EFI_HANDLE                          NvmeHandle;
    NVME_BASIC_INFO                     BasicInfo;
    NVME_TEXT_ITEM                      *ActiveNameSpace;
    LIST_ENTRY                          NvmeControllerLink;
} NVME_CONTROLLER_INFORMATION;

CHAR8*
GetNvmeControllerName (EFI_DEVICE_PATH_PROTOCOL *NvmeCtrlDevPath);

EFI_STATUS 
NvmeCallback(
    CONST EFI_HII_CONFIG_ACCESS_PROTOCOL*, 
    EFI_BROWSER_ACTION, 
    EFI_QUESTION_ID, 
    UINT8,
    EFI_IFR_TYPE_VALUE*, 
    EFI_BROWSER_ACTION_REQUEST*
    );

#endif

#define NVME_FORM_SET_CLASS          0xD1
#define NVME_FORM_SET_GUID\
        { 0x5E39CF2E, 0x6712, 0x45AB, { 0x84, 0xC4, 0x35, 0xD3, 0xC6, 0xA3, 0x68, 0x6D } }

//#define NVME_DYNAMIC_SETUP_VARIABLE_ID 0xDDDE


#define NVME_MAIN_FORM_ID            0x1
#define NVME_SUB_FORM_ID             0x2

#define NVME_MAIN_FORM_LABEL_START   0x1000
#define NVME_MAIN_FORM_LABEL_END     0x1001
#define NVME_SUB_FORM_LABEL_START    0x2000
#define NVME_SUB_FORM_LABEL_END      0x2001

//#define NVME_DEVICE_BASE_KEY         0x100
//#define NVME_DEVICE_DEVICE_SELFTEST_BASE_KEY  0x200
//#define SELFTEST_OPTION_QUESTION_ID   0x300
//#define SELFTEST_SHORT_RESULT_QUESTION_ID   0x340
//#define SELFTEST_EXT_RESULT_QUESTION_ID   0x380
//#define SELFTEST_ACTION_QUESTION_ID   0x3C0

#ifdef __cplusplus
}
#endif
#endif

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
