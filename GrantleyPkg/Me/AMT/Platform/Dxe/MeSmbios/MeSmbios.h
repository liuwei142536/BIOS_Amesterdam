//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++
 
Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MeSmbios.h

Abstract:

  Header file for the MeSmbios Driver.

--*/

#ifndef _ME_SMBIOS_H
#define _ME_SMBIOS_H

#include <FrameworkDxe.h>

#include "Platform.h"
#include "AmtLib.h"
#include "MeLib.h"
#include "Guid/DataHubRecords.h"
#include "Protocol/MebxProtocol.h"
#ifdef DYNAMIC_MMCFG_BASE_FLAG //AptioV server override: dynamic MMCFG base change support.
#include <Library/PciExpressLib.h> //AptioV server override: dynamic MMCFG base change support.
#endif
//
// Data table entry update function.
//
typedef EFI_STATUS (EFIAPI EFI_MISC_SUBCLASS_DATA_FUNCTION) (
  IN UINT16                         RecordType,
  IN OUT UINT32                     *RecordLen,
  IN OUT EFI_MISC_SUBCLASS_RECORDS  *RecordData,
  OUT BOOLEAN                       *LogRecordData
  );


//
// Data table entry definition.
//
typedef struct {
  UINT16                            RecordType;
  UINT32                            RecordLen;
  VOID                              *RecordData;
  EFI_MISC_SUBCLASS_DATA_FUNCTION   *Function;
} EFI_MISC_SUBCLASS_DATA_TABLE;


//
// Data Table external definitions.
//
#define MISC_SUBCLASS_TABLE_EXTERNS(NAME1, NAME2) \
extern NAME1 NAME2 ## Data; \
extern EFI_MISC_SUBCLASS_DATA_FUNCTION NAME2 ## Function


//
// Data Table entries
//
#define MISC_SUBCLASS_TABLE_ENTRY_DATA_ONLY(NAME1, NAME2) \
{ \
  NAME1 ## _RECORD_NUMBER, \
  sizeof(NAME1), \
  & NAME2 ## Data, \
  NULL \
}

#define MISC_SUBCLASS_TABLE_ENTRY_FUNCTION_ONLY(NAME1, NAME2) \
{ \
  NAME1 ## _RECORD_NUMBER,\
  0, \
  NULL, \
  & NAME2 ## Function \
}

#define MISC_SUBCLASS_TABLE_ENTRY_DATA_AND_FUNCTION(NAME1, NAME2, NAME3) \
{ \
  NAME1 ## _RECORD_NUMBER, \
  sizeof(NAME1), \
  & NAME2 ## Data, \
  & NAME3 ## Function \
}

//
// Global definition macros.
//
#define MISC_SUBCLASS_TABLE_DATA(NAME1, NAME2) \
NAME1 NAME2 ## Data
  
#define MISC_SUBCLASS_TABLE_FUNCTION(NAME2) \
EFI_STATUS EFIAPI NAME2 ## Function( \
  IN UINT16                         RecordType, \
  IN OUT UINT32                     *RecordLen, \
  IN OUT EFI_MISC_SUBCLASS_RECORDS  *RecordData, \
  OUT BOOLEAN                       *LogRecordData \
  )

//
// Driver module data definitions.
//

#define EFI_OEM_ASF_DESCRIPTIONSTRING "Intel_ASF"
#define EFI_OEM_ASF_SYSTEMIDSTRING    "Intel_ASF_001"

//
// This is definition for SMBIOS Oem data type 0x82
//
typedef struct {
  SMBIOS_STRUCTURE_HDR          Header;
  UINT8                         AmtSignature[4];
  UINT8                         AmtSupported;
  UINT8                         AmtEnabled;
  UINT8                         IderEnabled;
  UINT8                         SolEnabled;
  UINT8                         NetworkEnabled;
  UINT8                         ExtendedData;
  UINT8                         OemCapabilities1;
  UINT8                         OemCapabilities2;
  UINT8                         OemCapabilities3;
  UINT8                         OemCapabilities4;
  UINT8                         KvmEnabled;
  UINT8                         Reserved;		
  UINT16                        Zero;   //terminator
} EFI_MISC_OEM_TYPE_0x82;

//
// This is definition for SMBIOS Oem data type 0x83
//
//
// Legacy MEBx Launcher Application for 5MB FW SKU
//
#define MEBX_FILE_GUID \
  { \
    0x7c81c66a, 0x4f11, 0x47ab, 0x82, 0xd3, 0x67, 0xc4, 0xd6, 0x35, 0xae, 0xd1 \
  }
#define SETUP_GUID \
  { \
    0xEC87D643, 0xEBA4, 0x4BB5, 0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9 \
   }

#define vPro_Signature                    "vPro"
//#ifndef PCIEX_BASE_ADDRESS
//#define PCIEX_BASE_ADDRESS                MMCFG_BASE_ADDRESS
//#endif

#ifdef DYNAMIC_MMCFG_BASE_FLAG
#define PciMeRegBase                      (UINTN) GetPciExpressBaseAddress() + (UINT32) (22 << 15)
#else
#define PciMeRegBase                      PcdGet64 (PcdPciExpressBaseAddress) + (UINT32) (22 << 15)
#endif
#define FW_STATUS_REGISTER                0x40
#define EFI_MSR_IA32_FEATURE_CONTROL      0x3A
#define TXT_OPT_IN_VMX_AND_SMX_MSR_VALUE  0xFF06

#define MAX_MEBX_LAUNCH_SIZE              8 * 1024
#define CONVENTIONAL_MEMORY_TOP           0xA0000 // 640 KB
#define MEBX_HEADER_VER_OFFSET            16
#define MEBX_HEADER_VER_SIZE              8

typedef struct {
    UINT32  VMXState : 1;       //[0]     VMX state (On/Off)
    UINT32  SMXState : 1;       //[1]     SMX state (On/Off)
    UINT32  LtTxtCap : 1;       //[2]     LT/TXT capability 
    UINT32  LtTxtEnabled : 1;   //[3]     LT/TXT Enabled state (Optional for vPro verification)
    UINT32  VTxCap : 1;         //[4]     VT-x capability
    UINT32  VTxEnabled : 1;     //[5]     VT-x Enabled state (Optional for vPro verification)
    UINT32  Reserved : 26;      //[31:6]  Reserved, set to 0
} CPU_CAP;

typedef struct {
    UINT32  FunctionNumber : 3; //[2:0]     PCI Device Function Number
    UINT32  DeviceNumber : 5;   //[7:3]     PCI Device Device Number
    UINT32  BusNumber : 8;      //[15:8]    PCI Device Bus Number
    UINT32  DeviceID : 16;      //[31:16]   Device Identification Number (DID): This field is the identifier of PCH PCI device. DID will be set to 0xFF if not found.
    UINT32  Reserved : 32;      //[63:32]   Reserved, must set to 0
} PCH_CAP;

typedef struct {
    UINT32  FunctionNumber : 3; //[2:0]   PCI Device Function Number of Wired LAN
    UINT32  DeviceNumber : 5;   //[7:3]   PCI Device Device Number of Wired LAN
    UINT32  BusNumber : 8;      //[15:8]  PCI Device Bus Number of Wired LAN
    UINT32  DeviceID : 16;      //[31:16] PCI Device Identifier (DID) of wired NIC. DID will be set to 0xFF if not found.
    UINT32  Reserved1 : 16;     //[47:32] Reserved for Wired NIC.
    UINT32  Reserved2 : 16;     //[63:48] Reserved, must set to 0 (Wireless)
    UINT32  Reserved3 : 32;     //[95:64] Reserved, must set to 0 (Wireless)
} NETWORK_DEV;

typedef struct {
    UINT32  Reserved1    : 1;  //[0]    Reserved, must be set to 0
    UINT32  VTdSupport   : 1;  //[1]    BIOS supports VT-d in BIOS setup screen (can be editable)
    UINT32  TxtSupport   : 1;  //[2]    BIOS supports TXT in BIOS setup screen (can be editable)
    UINT32  Reserved2    : 1;  //[3]    Reserved, must be set to 0
    UINT32  Reserved3    : 1;  //[4]    Reserved, must be set to 0
    UINT32  VTxSupport   : 1;  //[5]    BIOS supports VT-x in BIOS setup screen (can be editable)
    UINT32  AtPbaSupport : 1;  //[6]    BIOS supports external AT PBA
    UINT32  AtWwanSuport : 1;  //[7]    BIOS supports AT WWAN Recovery
    UINT32  Reserved4    : 24; //[31:8] Reserved, must be set to 0
} BIOS_CAP;

typedef struct {
  SMBIOS_STRUCTURE_HDR           Header;
  CPU_CAP                        CpuCapabilities;
  MEBX_VER                       MebxVersion;
  PCH_CAP                        PchCapabilities;
  ME_CAP                         MeCapabilities;
  MEFWCAPS_SKU                   MeFwConfigured;
  NETWORK_DEV                    NetworkDevice;
  BIOS_CAP                       BiosCapabilities;
  UINT8                          StructureIdentifier[4]; 
  UINT32                         Reserved;
  UINT16                         Zero;    //terminator
} EFI_MISC_OEM_TYPE_0x83;

//
// Oem types 0x80-0xff share the same data record number
//
#define EFI_MISC_OEM_TYPE_0x88_RECORD_NUMBER EFI_MISC_SMBIOS_STRUCT_ENCAP_RECORD_NUMBER
#define EFI_MISC_OEM_TYPE_0x82_RECORD_NUMBER EFI_MISC_SMBIOS_STRUCT_ENCAP_RECORD_NUMBER
#define EFI_MISC_OEM_TYPE_0x83_RECORD_NUMBER EFI_MISC_SMBIOS_STRUCT_ENCAP_RECORD_NUMBER

//
// Data Table Array
//
extern EFI_MISC_SUBCLASS_DATA_TABLE mMiscSubclassDataTable[];

//
// Data Table Array Entries
//
extern UINTN  mSmbiosMiscDataTableEntries;

#endif
