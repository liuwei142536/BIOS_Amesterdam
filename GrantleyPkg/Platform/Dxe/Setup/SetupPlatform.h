//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 1999 - 2006 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SetupPlatform.h

Abstract:

  Header file for Platform Initialization Driver.

Revision History:

--*/

#ifndef _SETUP_PLATFORM_H_
#define _SETUP_PLATFORM_H_

#include <Token.h> // Aptio V server override
#include <FrameworkDxe.h>

#include "Platform.h"
#include "Configuration.h"
// AptioV server override
//#include "PlatformStrDefs.h"
#include "Flashmap.h"

#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Library/PciLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PchPlatformLib.h>
#include <Library/IoLib.h>
#include <Library/S3IoLib.h>
#include <Library/PcieSlotOpromLib.h>
#include <Library/PchPlatformPolicyInitDxeLib.h>
#include <Library/PlatformHooksLib.h>

#include <Protocol/PchPlatformResetPolicy.h>
#include <Protocol/DataHub.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiPackageList.h> // // Aptio V Server Override ; Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
#include <Protocol/IdeControllerInit.h>
#include <Protocol/IioUds.h>
#include <Protocol/PlatformType.h>
#include <Protocol/TcgService.h>
#include <Protocol/CpuIo.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/IsaIo.h>
#include <Protocol/Ps2Policy.h>
//AptioV server override start: dynamic MMCFG base change support.
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include <PchSmmAccess.h> //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include <Protocol/PlatformPolicy.h>
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/EfiLegacyBootMarker.h>
#include <Protocol/MpService.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/IioSystem.h>

#include <Guid/CpuHtCapable.h>
#include <Guid/DataHubRecords.h>
#include <Guid/HobList.h>
#include <Guid/MemorySetupData.h>
#include <Guid/MemoryConfigData.h>
#include <Guid/MemoryMapData.h>
#include <Guid/SetupVariable.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/GlobalVariable.h>
#include <Guid/AdminPasswordGuid.h>

// 

#include <IndustryStandard/Pci22.h>
#include <IndustryStandard/Pci30.h>

#include <EfiServerManagement.h>

#define	ERROR_MANAGER_GUID { 0xADDEBF82, 0xA560, 0x46B9, {0xA2, 0x80, 0x78, 0xC6, 0xAB, 0x61, 0xAE, 0xDA }}

#define AMITSE_SETUP_ENTER_GUID \
  { 0x71202EEE, 0x5F53, 0x40d9, 0xAB, 0x3D, 0x9E, 0x0C, 0x26, 0xD9, 0x66, 0x57 }

// EDK_TODO
#define EFI_CPUID_VERSION_INFO      0x1


#define DXE_DEVICE_ENABLED  1
#define DXE_DEVICE_DISABLED 0

//
// Microcode range from 0xFFFFB000-0xFFFFEFFF
//
#define EFI_MICROCODE_BASE              FLASH_REGION_MICROCODE_FV_BASE
#define EFI_MICROCODE_LENGTH            FLASH_REGION_MICROCODE_FV_SIZE

#define PLATFORM_CPU_MAX_CORE_FREQUENCY 4000
#define PLATFORM_CPU_MAX_FSB_FREQUENCY  1066

#define KBC_DATA_PORT                   0x60
#define KBC_CMD_STS_PORT                0x64
#define ONE_SECOND                      10000000
#define VALIDATE_NONE                   0x00
#define VALIDATE_ADMIN                  0x01
#define VALIDATE_USER                   0x02
#define VALIDATE_BOTH                   0x03
 
#define MAIN_PAGE_KEY_SAVE              0x9001
#define MAIN_PAGE_KEY_RESTORE           0x9002

#define AUTHENTICATION_PASSED           0x00
#define AUTHENTICATION_NOT_PASSED       0xFF

#define PW_SET                          0x01
#define PW_NOT_SET                      0x00

#define MAX_STRING_LEN                  0x100
#define CharIsUpper(c)                  ((c >= L'A') && (c <= L'Z'))
#define CharIsLower(c)                  ((c >= L'a') && (c <= L'z'))
#define CharIsAlpha(c)                  (CharIsUpper(c) || CharIsLower(c)) 
#define CharIsNumeric(c)                ((c >= L'0') && (c <= L'9'))
#define CharIsAlphaNumeric(c)           (CharIsAlpha(c) || CharIsNumeric(c))

typedef struct {
  UINT8 Register;
  UINT8 Value;
} PCH_DMA_INIT;

typedef struct {
  UINT32  Id;
  UINT32  Freq;
  UINT32  MaxRatio;
  UINT32  MinRatio;
  UINT32  Ucode;
  UINT32  L1Cache;
  UINT32  L2Cache;
  UINT32  L3Cache;
  CHAR16  *Version;
} CPU_SETUP_INFO;

#pragma pack(1)

typedef struct {
  UINT16 Signature;
  UINT8  Size;
  UINT32 EntryPoint;
  UINT8  Reserve[17];
  UINT16 PciDataOff;
  UINT16 ExpansionOff;
} PNP_OPTION_ROM_HEADER;

typedef struct {
  UINT32 Signature;
  UINT8  Revision;
  UINT8  Length;
  UINT16 NextHeader;
  UINT8  Reserve;
  UINT8  CheckSum;
  UINT32 DeviceId;
  UINT16 ManufactureStrOff;
  UINT16 ProductStrOff;
} PNP_EXPANSION_HEADER;

typedef struct _ERROR_LIST
{
	EFI_HII_HANDLE	Handle;
	STRING_REF		  Summary;
	STRING_REF		  DetailInfo;
} ERROR_LIST;

typedef struct _ERROR_MANAGER
{
	UINT16		  ErrorCount;
	UINT16		  ErrorFunction;
	ERROR_LIST	ErrorList[1];
} ERROR_MANAGER;

typedef struct {          
  UINT32      Year    : 4;
  UINT32      Month   : 4;
  UINT32      Day     : 5;        
  UINT32      Hour    : 5;       
  UINT32      Minute  : 6;     
  UINT32      Second  : 6;     
} TIME_STAMP;

typedef struct {
  UINT16  RecordId;
  UINT8   RecordType;
  TIME_STAMP  TimeStamp;
  UINT16  GeneratorId;
  UINT8   EvMRevision;
  UINT8   SensorType;
  UINT8   SensorNumber;
  UINT8   EventDirType;
  UINT8   OEMEvData1;
  UINT8   OEMEvData2;
  UINT8   OEMEvData3;
} EFI_SEL_RECORD_DATA;

#pragma pack()

typedef enum {
  EfiUserPassword,
  EfiAdminPassword
} EFI_PASSWORD_TYPE;

typedef struct {
  CHAR16            TempPassword[PASSWORD_MAX_SIZE]; //Aptio V Server Override macro redefintion
  CHAR16            EncodedPassword[PASSWORD_MAX_SIZE]; //Aptio V Server Override macro redefintion
  VOID              *PasswordLocation;
  EFI_PASSWORD_TYPE PasswordType;
} EFI_PASSWORD_DATA;

///
/// HII specific Vendor Device Path Node definition.
///
#pragma pack(1)

typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  UINT16                         UniqueId;
} HII_VENDOR_DEVICE_PATH_NODE;

///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  HII_VENDOR_DEVICE_PATH_NODE    Node;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()

//
// Error Type information for setup
//
#define B_FAT_ERROR_TYPE        BIT4   
#define B_UNF_ERROR_TYPE        BIT5
#define B_CNF_ERROR_TYPE        BIT6


#define FBD_FAT_ERROR_TYPE        BIT4   
#define FBD_UNF_ERROR_TYPE        BIT5
#define FBD_CNF_ERROR_TYPE        BIT6
#define FBD_ERROR_BITS          ( FBD_FAT_ERROR_TYPE    |\
                                  FBD_UNF_ERROR_TYPE    |\
                                  FBD_CNF_ERROR_TYPE )
#define FBD_ERROR_NUM             0x0F
#define FBD_ERROR_TYPE          ( FBD_ERROR_BITS | FBD_ERROR_NUM)
 

#define FSB_FAT_ERROR_TYPE        BIT4   
#define FSB_UNF_ERROR_TYPE        BIT5
#define FSB_CNF_ERROR_TYPE        BIT6
#define FSB_ERROR_BITS          ( FSB_FAT_ERROR_TYPE    |\
                                  FSB_UNF_ERROR_TYPE    |\
                                  FSB_CNF_ERROR_TYPE )
#define FSB_ERROR_NUM             0x0E
#define FSB_ERROR_TYPE          ( FSB_ERROR_BITS | FSB_ERROR_NUM)


#define SG_INT_FAT_ERROR_TYPE     BIT4   
#define SG_INT_UNF_ERROR_TYPE     BIT5
#define SG_INT_CNF_ERROR_TYPE     BIT6
#define SG_INT_ERROR_BITS       ( SG_INT_FAT_ERROR_TYPE    |\
                                  SG_INT_UNF_ERROR_TYPE    |\
                                  SG_INT_CNF_ERROR_TYPE )
#define SG_INT_ERROR_NUM          0x0D
#define SG_INT_ERROR_TYPE       ( SG_INT_ERROR_BITS | SG_INT_ERROR_NUM)

#define DMA_FAT_ERROR_TYPE        BIT4   
#define DMA_UNF_ERROR_TYPE        BIT5
#define DMA_CNF_ERROR_TYPE        BIT6
#define DMA_ERROR_BITS          ( DMA_FAT_ERROR_TYPE    |\
                                  DMA_UNF_ERROR_TYPE    |\
                                  DMA_CNF_ERROR_TYPE )
#define DMA_ERROR_NUM             0x0C
#define DMA_ERROR_TYPE          ( DMA_ERROR_BITS | DMA_ERROR_NUM)

#define CB_CHAN_FAT_ERROR_TYPE    BIT4   
#define CB_CHAN_UNF_ERROR_TYPE    BIT5
#define CB_CHAN_CNF_ERROR_TYPE    BIT6
#define CB_CHAN_ERROR_BITS      ( CB_CHAN_FAT_ERROR_TYPE    |\
                                  CB_CHAN_UNF_ERROR_TYPE    |\
                                  CB_CHAN_CNF_ERROR_TYPE )
#define CB_CHAN_ERROR_NUM         0x0B
#define CB_CHAN_ERROR_TYPE      ( CB_CHAN_ERROR_BITS | CB_CHAN_ERROR_NUM)

#define PCIE_FAT_ERROR_TYPE       BIT4   
#define PCIE_UNF_ERROR_TYPE       BIT5
#define PCIE_CNF_ERROR_TYPE       BIT6
#define PCIE_ERROR_BITS         ( PCIE_FAT_ERROR_TYPE   |\
                                  PCIE_UNF_ERROR_TYPE   |\
                                  PCIE_CNF_ERROR_TYPE )
#define PCIE_ERROR_NUM            0x0A
#define PCIE_ERROR_TYPE         ( PCIE_ERROR_BITS | PCIE_ERROR_NUM)



//
// Prototypes
//
EFI_STATUS
EfiMain (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++

Routine Description:

  This is the standard EFI driver point for the Driver. This
  driver is responsible for setting up any platform specific policy or
  initialization information.

Arguments:

  ImageHandle  -  Handle for the image of this driver.
  SystemTable  -  Pointer to the EFI System Table.

Returns:

  EFI_SUCCESS  -  Policy decisions set.

--*/
;

EFI_STATUS
FindDataRecords (
  VOID
  )
/*++

Routine Description:

  Register event handler to find setup data records in Data Hub.

Arguments:
  
  None.
  
Returns:

  EFI_SUCCESS  -  The data hub filter event handler was registered.

--*/
;

EFI_STATUS
MemorySetup (
  VOID
  )
/*++

Routine Description:

  Setup Memory.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Must complete successfully.

--*/
;

EFI_STATUS
ProcessorHtMode (
  VOID
  )
/*++

Routine Description:

  Setup process in HT mode.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Must complete successfully.

--*/
;
// AptioV server override start: Changes to update IIOxPresent setup variable
VOID
UpdateIioPresent (
VOID
);
// AptioV server override End: Changes to update IIOxPresent setup variable

// Aptio V Server Override - Start; Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
EFI_STATUS
ParseIfrPackForIntelRcSetup(
IN EFI_HANDLE        ImageHandle
);

// Aptio V Server Override - End ; Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
VOID
EFIAPI
InstallHiiDataAndGetSettings (
  IN EFI_HII_PACKAGE_HEADER           *StringPack,
  ...
  );

/*++

Routine Description:

  Installs a string and ifr pack set.

Arguments:

  Hii         - pointer to an HII database protocol structure.
  StringPack  -  string pack to store in database and associate with IfrPack.
  IrfPack     - ifr pack to store in database (it will use StringPack data).

Returns:

  None.

--*/
;

EFI_STATUS
ReadOrInitSetupVariable (
  IN  EFI_GUID      *VarGuid,
  IN  UINTN         RequiredVariableSize,
  IN  VOID          *DefaultData,
  OUT VOID          *SetupVariableData
  )
/*++

Routine Description:
  Reads the setup configuration data for a driver.  If the variable doesn't
  exist, then it will be written with the data from DefaultData.

Arguments:

  VarGuid               -  The setup variable guid to read.
  RequiredVariableSize  -  The size that the variable.  If the variable
                           read is not this size, then the variable will be treated as invalid.
                           SetupVariableData is assumed to be large enough to hold
                           RequiredVariableSize bytes.  If DefaultData != NULL, then it is
                           assumed to be RequiredVariableSize bytes.
  DefaultData           -  If the variable is not found, or invalid, then this buffer
                           will be copied into SetupVariableData.  
                           This provides a default configuration.
  SetupVariableData     -  The variable will be copied into this buffer.  If the
                           variable is not found, or is invalid, then DefaultData will be copied.

Returns:

  EFI_SUCCESS  -  The variable was read and successfully copied to
                  SetupVariableData.

--*/
;

VOID
EfiLogicalOrMem (
  IN VOID   *Destination,
  IN VOID   *Source,
  IN UINTN  Length
  )
/*++

Routine Description:

  Perform logical Or operation from source onto destination memory.

Arguments:

  Destination  -  Address of destination memory block.
  Source       -  Address of source memory block.
  Length       -  The length of memory region to perform logical OR onto.

Returns:

  EFI_SUCCESS  -  The operation is finished successfully.

--*/
;

EFI_STATUS
EFIAPI
Ps2PortSwap (
  IN  EFI_HANDLE  Handle
  )
/*++

Routine Description:

  Detect which device is connected to which port.

Arguments:

  Handle  -  Handle for the Isa protocol interface instance.
  
Returns:

  EFI_SUCCESS  -  Function has completed successfully.
  Other        -  Error has occurred during execution.

--*/
;

EFI_STATUS
GetStringFromToken (
  IN      EFI_GUID                  *ProducerGuid,
  IN      STRING_REF                Token,
  OUT     CHAR16                    **String
  )
/*++

Routine Description:
  
  Acquire the string associated with the ProducerGuid and return it.

Arguments:
  
  ProducerGuid  -  The Guid to search the HII database for.
  Token         -  The token value of the string to extract.
  String        -  The string that is extracted.
  
Returns:

  EFI_SUCCESS  -  Must return success on completion.
  
--*/
;

UINT32
ConvertBase2ToRaw (
  IN  EFI_EXP_BASE2_DATA             *Data
  )
/*++

Routine Description:

  Convert data from base 2 to raw.

Arguments:

  Data  -  The data to convert.

Returns:

  Return converted data.

--*/
;

UINT32
ConvertBase10ToRaw (
  IN  EFI_EXP_BASE10_DATA             *Data
  )
/*++

Routine Description:

  Convert data from base 10 to raw.

Arguments:

  Data  -  The data to convert.

Returns:

  Return converted data.

--*/
;

CHAR16 *
GetStringById (
  IN  STRING_REF       Id,
  IN  EFI_HII_HANDLE   StringPackHandle
  )
/*++

Routine Description:

  Get string by string id from HII Interface.

Arguments:

  Id                -  String ID.
  StringPackHandle  -  Handle to the String Pack.
  
Returns:
  String from ID.

--*/
;

VOID
EFIAPI
SetupDataFilter (
  IN EFI_EVENT      Event,
  IN VOID           *Context
  )
/*++

Routine Description:

  Setup data filter function. This function is invoked when there is data records
  available in the Data Hub.

Arguments:

  Event    -  the event that is signaled.
  Context  -  not used here.

Returns:

  None.

--*/
;

VOID
EFIAPI
IdeDataFilter (
  )
/*++

Routine Description:

  IDE data filter function.

Arguments:

  None

Returns:

  None.

--*/
;

VOID
SwapEntries (
  IN  CHAR8 *Data,
  IN  UINT16 Size
  )
/*++

Routine Description:

  Swap order of nearby CHAR8 data.

Arguments:

  Data  -  The data to swap.

Returns:

  None.
  
--*/
;

VOID
AsciiToUnicode (
  IN    CHAR8     *AsciiString,
  IN    CHAR16    *UnicodeString
  )
/*++

Routine Description:

  Converts an ascii string to unicode string 16 chars at a time.

Arguments:

  AsciiString    -  Address of Ascii string.
  UnicodeString  -  Address of Unicode String.

Returns:

  None.

--*/
;

EFI_STATUS
SetIdeMode (
  VOID
  )
/*++

Routine Description:

  Set Ide's mode.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Must return success on completion.

--*/
;

EFI_STATUS
EFIAPI
SecurityDriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  );

VOID
EFIAPI
PciBusEvent (
  IN EFI_EVENT      Event,
  IN VOID           *Context
  )
/*++

Routine Description:

  Pass an event to Pci Bus.

Arguments:

  Event    -  The event to pass.
  Context  -  The context of the event.

Returns:

  None.

--*/
;

EFI_STATUS
PciBusDriverHook (
  VOID
  )
/*++

Routine Description:

  Hook Pci Bus driver.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Function has completed successfully.
  Others       -  Error occurred during execution.

--*/
;

EFI_STATUS
DisplayPlatformInfo (
  EFI_PLATFORM_TYPE_PROTOCOL            *mPlatformType
  )
/*++

Routine Description:

  Displays the Platform, NorthBridge and SouthBridge info in the Setup.

Arguments:

  mPlatformType.

Returns:

  EFI_SUCCESS  -  Must complete successfully.

--*/
;

VOID
ConfigPciePort (
  )
/*++

Routine Description:

  This function configures the PCI Express Port Settings.

Arguments:

  None

Returns:

  None

--*/
;

VOID
UpdateErrorManager (
  VOID
);

/*
EFI_STATUS
NvWriteCallbackRoutine (
  IN   EFI_FORM_CALLBACK_PROTOCOL  *This,
  IN   CHAR16                      *VariableName,
  IN   EFI_GUID                    *VendorGuid,
  IN   UINT32                      Attributes,
  IN   UINTN                       DataSize,
  IN   VOID                        *Buffer,
  OUT  BOOLEAN                     *ResetRequired
  );


EFI_STATUS
NvTpmWriteCallbackRoutine (
  IN   EFI_FORM_CALLBACK_PROTOCOL  *This,
  IN   CHAR16                      *VariableName,
  IN   EFI_GUID                    *VendorGuid,
  IN   UINT32                      Attributes,
  IN   UINTN                       DataSize,
  IN   VOID                        *Buffer,
  OUT  BOOLEAN                     *ResetRequired
  );

EFI_STATUS
SecurityDriverCallback (
  IN EFI_FORM_CALLBACK_PROTOCOL       *This,
  IN UINT16                           KeyValue,
  IN EFI_IFR_DATA_ARRAY               *Data,
  OUT EFI_HII_CALLBACK_PACKET         **Packet
  )
;
*/

EFI_STATUS
SetPasswordState (
  IN  EFI_PASSWORD_DATA               *SecurityData
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  SecurityData  - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

VOID
EncodePassword (
  IN  CHAR16                      *Password
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  Password  - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

VOID
UpdateCSIStatusLines (
  VOID
  )
/*++

Routine Description:

  This function updates CSI status lines.

Arguments:

  None

Returns:

  None

--*/
;

VOID
UpdateUsbData (
   VOID
   );
//
// Global externs
//
extern UINT8                PlatformStrings[];
//extern UINT8                MaintenanceBin[];
extern UINT8                MainBin[];
//extern UINT8                IntelRCSetupBin[]; // Aptio V Server Override ; Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
extern UINT8                IohSetupBin[];
extern UINT8                PchSetupBin[];
extern UINT8                ErrorManagerBin[];
extern UINT8                ExitBin[];
extern UINT8                BootOptionsBin[];
extern UINT8                SecurityBin[];
extern UINT8                TpmBin[];

extern EFI_HII_DATABASE_PROTOCOL        *mHiiDataBase;
extern EFI_HII_CONFIG_ROUTING_PROTOCOL  *mHiiConfigRouting;

extern EFI_HII_CONFIG_ACCESS_PROTOCOL   mMainConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL   mAdvancedConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL   mIohConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL   mPchConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL   mEventLogConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL   mSecurityConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL   mTpmConfigAccess;


extern SYSTEM_CONFIGURATION mSystemConfiguration;
//extern EFI_HII_HANDLE       mMaintenanceHiiHandle;
extern EFI_HII_HANDLE       mMainHiiHandle;
extern EFI_HII_HANDLE       mAdvancedHiiHandle;
extern EFI_HII_HANDLE       mSecurityHiiHandle;

extern EFI_HANDLE           mMainDriverHandle;
extern EFI_HANDLE           mAdvancedDriverHandle;
extern EFI_HANDLE           mTpmDriverHandle;
extern EFI_HANDLE           mSecurityDriverHandle;

extern EFI_GUID             mSystemConfigurationGuid;
extern CHAR16               mVariableName[];
extern EFI_PASSWORD_DATA    mAdminPassword;
extern EFI_PASSWORD_DATA    mUserPassword;

EFI_STATUS
EFIAPI
DefaultExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  );

EFI_STATUS
EFIAPI
DefaultRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  );

EFI_STATUS
BmcLanExtractConfig (
  IN CONST EFI_STRING  Request,
  OUT EFI_STRING      *Progress,
  OUT EFI_STRING      *Results
  );

EFI_STATUS
BmcLanRouteConfig (
  IN CONST EFI_STRING  Configuration,
  OUT EFI_STRING      *Progress
  );

EFI_STATUS
BmcLanCallbackRoutine (
  IN EFI_BROWSER_ACTION           Action,
  IN EFI_QUESTION_ID              QuestionId,
  IN UINT8                        Type,
  IN EFI_IFR_TYPE_VALUE          *Value,
  OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest OPTIONAL
  );

EFI_STATUS
InstallDefaultCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  );

EFI_STATUS
InstallAdvancedCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  );
  
EFI_STATUS
InstallSecurityCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  );

EFI_STATUS
InstallTpmCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  );

EFI_STATUS
InstallMaintenanceCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  );

EFI_STATUS
InstallMainCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  );

VOID
PlatformInitPchRegs (
  VOID
  );

VOID
EFIAPI
IdeCallback(
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  );

VOID 
InitMeSetupStrings(
  );

EFI_STATUS
EFIAPI
MeAdvancedRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
);

VOID
EFIAPI
PlatformResetCallback (
  OUT PCH_EXTENDED_RESET_TYPES *Type);

VOID
BackupSetupDataAndCheckForPassword (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  );
  
VOID
CheckForPassword (
  VOID
  );
  
EFI_STATUS
ReadStrings(
  IN CHAR16                   *StringPtr
  );
  
VOID
EFIAPI
DrawPopUp (
  VOID
  );

EFI_STATUS
ReadKeyStrokes(
  EFI_INPUT_KEY                 *Key
 );

UINT8
CheckIfPasswordInstalled(
  VOID
  );

BOOLEAN 
IsPasswordCheck (
  IN BOOLEAN                     Check,
  IN CHAR16                      Key
  );

EFI_STATUS
PromptForPassword(
  VOID
  );

VOID 
SetPasswordType (
  UINT8                          Type
  );
    
VOID
ReserveMemPage(
   VOID
   );

#endif
