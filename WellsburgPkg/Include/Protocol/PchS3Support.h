/** @file
  This file defines the PCH S3 support Protocol.

@copyright
  Copyright (c) 2008 - 2017 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _PCH_S3_SUPPORT_PROTOCOL_H_
#define _PCH_S3_SUPPORT_PROTOCOL_H_
#ifdef DYNAMIC_MMCFG_BASE_FLAG //AptioV server override: Support for Dynamic MMCFG.
#include "PchAccess.h" //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#endif
#include <Protocol\PchPlatformPolicy.h>

#define EFI_PCH_S3_STALL_INTERVAL 50  // us

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                             gEfiPchS3SupportProtocolGuid;
extern EFI_GUID                             gEfiPchS3SupportSmmProtocolGuid;
extern EFI_GUID                             gS3SupportSmramDataGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_PCH_S3_SUPPORT_PROTOCOL EFI_PCH_S3_SUPPORT_PROTOCOL;
typedef struct _EFI_PCH_S3_SUPPORT_SMM_PROTOCOL EFI_PCH_S3_SUPPORT_SMM_PROTOCOL;

typedef enum {
  PchS3ItemTypeSendCodecCommand,
  PchS3ItemTypePollStatus,
  PchS3ItemTypeInitPcieRootPortDownstream,
  PchS3ItemTypePcieSetPm,
  PchS3ItemTypeProgramIobp,
  PchS3ItemTypePmTimerStall,
  PchS3ItemTypeMax
} EFI_PCH_S3_DISPATCH_ITEM_TYPE;

///
/// In the following structures it is required that they are usable in both PEI (32-bit) and DXE (64-bit).
/// As a result, Pointers and Enumerations will be of different underlying sizes.  Therefore neither should
/// appear in the middle of these structures.  If done, then size adjustment will need to be explicit
/// via other mechanisms (like a union).
///
typedef struct {
  UINT32                        HdaBar;
  UINT32                        CodecCmdData;
} EFI_PCH_S3_PARAMETER_SEND_CODEC_COMMAND;

typedef struct {
  UINT64                        MmioAddress;
  EFI_BOOT_SCRIPT_WIDTH         Width;
  UINT64                        Mask;
  UINT64                        Value;
  UINT32                        Timeout;  // us
} EFI_PCH_S3_PARAMETER_POLL_STATUS;

typedef struct {
  UINT8                         RootPortBus;
  UINT8                         RootPortDevice;
  UINT8                         RootPortFunc;
  UINT8                         TempBusNumberMin;
  UINT8                         TempBusNumberMax;
} EFI_PCH_S3_PARAMETER_INIT_PCIE_ROOT_PORT_DOWNSTREAM;

typedef struct {
  UINT8                         RootPortBus;
  UINT8                         RootPortDevice;
  UINT8                         RootPortFunc;
  PCH_PCI_EXPRESS_ASPM_CONTROL  RootPortAspm;
  UINT8                         NumOfDevAspmOverride;
  UINT32                        DevAspmOverrideAddr;
  UINT8                         TempBusNumberMin;
  UINT8                         TempBusNumberMax;
  UINT8                         NumOfDevLtrOverride;
  UINT32                        DevLtrOverrideAddr;
  UINT32                        PchPwrOptPcie;
  PCH_PCIE_EXPRESS_L1SUBSTATES_CONTROL L1SubstatesConfig;
  UINT8                                PolicyRevision;
  BOOLEAN                              FirstRPToSetPm;
  BOOLEAN                              L1SupportedInAllEnabledPorts;
  BOOLEAN                              ClkreqSupportedInAllEnabledPorts;
} EFI_PCH_S3_PARAMETER_PCIE_SET_PM;

typedef struct {
  UINT32                        RootComplexBar;
  UINT32                        Address;
  UINT32                        AndMask;
  UINT32                        OrMask;
} EFI_PCH_S3_PARAMETER_PROG_IOBP;

typedef struct {
  UINT32                        DelayTime;  // us
} EFI_PCH_S3_PARAMETER_PM_TIMER_STALL;
typedef struct {
  union {                                                 // The union definition is in place because this structure
    EFI_PCH_S3_DISPATCH_ITEM_TYPE Value;                  // is used in both DXE and PEI where enumerations are
    UINT64                        Spacer;                 // different sizes.
  } ItemType;
  VOID                          *Parameter;
} EFI_PCH_S3_DISPATCH_ITEM;

typedef struct {
  EFI_GUID                      PchS3CustomScriptGuid;
  UINT32                        MaximumBufferSize;
  UINT32                        BufferSpaceRemaining;
  UINT8                         *NextDispatchItem;
  //EFI_PCH_S3_DISPATCH_ITEM    DispatchItemAray[]        // This structure is followed in memory
                                                          // by an Array of EFI_PCH_S3_DISPATCH_ITEM structures
} EFI_PCH_S3_DISPATCH_ARRAY;

#define QWORD_ALIGNED_SIZE(x) ((sizeof (x) + 7) / 8 * 8)  // QWORD alignment is needed for the variable lengths
                                                          // of the "Parameter" field of the EFI_PCH_S3_DISPATCH_ITEM
                                                          // structure.  Alignment must be maintained between
                                                          // the 32-bit PEI code and 64-bit DXE code.

//
// Member functions
//
/**
  Set an item to be dispatched at S3 resume time. At the same time, the entry point
  of the PCH S3 support image is returned to be used in subsequent boot script save
  call

  @param[in] This                       Pointer to the protocol instance.
  @param[in] DispatchItem               The item to be dispatched.
  @param[out] S3DispatchEntryPoint      The entry point of the PCH S3 support image.

  @retval EFI_STATUS                    Successfully completed.
  @retval EFI_OUT_OF_RESOURCES          Out of resources.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PCH_S3_SUPPORT_SET_S3_DISPATCH_ITEM) (
  IN     EFI_PCH_S3_SUPPORT_PROTOCOL   * This,
  IN     EFI_PCH_S3_DISPATCH_ITEM      * DispatchItem,
  OUT    EFI_PHYSICAL_ADDRESS          * S3DispatchEntryPoint
  );

/**
  Perform the EFI_PCH_S3_SUPPORT_SMM_PROTOCOL IO Trap to invoke DispatchArray data copy and
  IO Trap Unregister.

  @param[in] This                       Pointer to the protocol instance.

  @retval EFI_STATUS                    Successfully completed.
  @retval EFI_OUT_OF_RESOURCES          Out of resources.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PCH_S3_SUPPORT_READY_TO_LOCK) (
  IN    EFI_PCH_S3_SUPPORT_PROTOCOL   *This
  );

///
/// Protocol definition
///
/// This Protocol is used to set an item to be dispatched at S3 resume time.
/// At the same time, the entry point of the PCH S3 support image is returned to
/// be used in subsequent boot script save call.
///
struct _EFI_PCH_S3_SUPPORT_PROTOCOL {
  EFI_PCH_S3_SUPPORT_SET_S3_DISPATCH_ITEM SetDispatchItem;      ///< Set the item to be dispatched at S3 resume time.
  EFI_PCH_S3_SUPPORT_READY_TO_LOCK        ReadyToLock;          ///< The caller is finished using the protocol and it can be locked.
};

///
/// Protocol Definition
///
/// This Protocol is used to communicate the location of the Boot Services copy of the EFI_PCH_S3_DISPATCH_ARRAY.
/// The pointer is then used to allow the SMM module to copy the data to the appropriate SMRAM location.  The
/// ProtocolSize is communicated in # of Pages.
///
struct _EFI_PCH_S3_SUPPORT_SMM_PROTOCOL {
  UINT16                        ProtocolSize;   		///< Protocol size in Pages (due to Page alignment requirements in SMM)
  UINT16                      	PchS3SupportIoTrap;	///< IO Trap port to support ExitPmAuth notification for copy and unregister
  EFI_PCH_S3_DISPATCH_ARRAY     *DispatchArray; 		///< A pointer to the Boot Services copy of the Dispatch Array
};

#endif
