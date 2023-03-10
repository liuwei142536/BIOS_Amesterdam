/*++

   Copyright (c) 2005 Intel Corporation. All rights reserved
   This software and associated documentation (if any) is furnished
   under a license and may only be used or copied in accordance
   with the terms of the license. Except as permitted by such
   license, no part of this software or documentation may be
   reproduced, stored in a retrieval system, or transmitted in any
   form or by any means without the express written consent of
   Intel Corporation.


   Module Name:

   TpmDev.h

   Abstract:

   Header file for tpm device drivers

   --*/

//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TCG/TcgPc.h 4     3/29/11 12:26p Fredericko $
//
// $Revision: 4 $
//
// $Date: 3/29/11 12:26p $
//*************************************************************************
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  TcgPc.h
//
// Description: 
//  Contains Industry Standard #defines and Structures for TCG
//
//<AMI_FHDR_END>
//*************************************************************************
#ifndef _TPMPC_H_
#define _TPMPC_H_

#include "Token.h"
#include "AmiTcg/TcgEFI12.h"
#include "AmiTcg/INTTcgAcpi.h"
#include <IndustryStandard/UefiTcgPlatform.h>

#pragma pack (push)
#pragma pack (1)


typedef struct _TCG_ACPI_TABLE
{
	TCG_EFI_ACPI_DESCRIPTION_HEADER Header;
	UINT16                      PlatformClass;
#if TCG_PLATFORM_CLASS == 1
    UINT16                      Reserved;
    UINT64                      LogMaxLength;
#else
    UINT32                      LogMaxLength;
#endif
    EFI_PHYSICAL_ADDRESS        LogStart;
    UINT16                      TcgSpecRev;
    UINT8                       DeviceFlags;
    UINT8                       InterruptFlags;
    UINT8                       Gpe;
    UINT8                       Reserved1[3];
    UINT32                      GlobalSystemInterrupt;
    UINT8                       GasBaseAddress[12];
    UINT8                       Reserved2[4];
    UINT8                       ConfigurationAddress[12];
    UINT8                       PciSegmentGroupNumber;
    UINT8                       PciBusNumber;
    UINT8                       PciDeviceNumber;
    UINT8                       PciFunctionNumber;
} AMI_TCG_ACPI_TABLE;


#pragma pack (pop)



#define TPM_ACC_VALID           _TPM_NTH_BIT( 7 )
#define TPM_ACC_ACTIVE_LOCALITY _TPM_NTH_BIT( 5 )
#define TPM_ACC_BEEN_SEIZED     _TPM_NTH_BIT( 4 )
#define TPM_ACC_SEIZE           _TPM_NTH_BIT( 3 )
#define TPM_ACC_PENDING_REQUEST _TPM_NTH_BIT( 2 )
#define TPM_ACC_REQUEST_USE     _TPM_NTH_BIT( 1 )
#define TPM_ACC_ESTABLISHMENT   _TPM_NTH_BIT( 0 )

#define TPM_STS_VALID           _TPM_NTH_BIT( 7 )
#define TPM_STS_READY           _TPM_NTH_BIT( 6 )
#define TPM_STS_GO              _TPM_NTH_BIT( 5 )
#define TPM_STS_DATA_AVAILABLE  _TPM_NTH_BIT( 4 )
#define TPM_STS_EXPECT          _TPM_NTH_BIT( 3 )
#define TPM_STS_RESPONSE_RETRY  _TPM_NTH_BIT( 1 )

#define TPM_BASE_ADDRESS            0xFED40000

#define TPM_H2NS( x ) \
    (((UINT16)(x) << 8) | ((UINT16)(x) >> 8))
#define TPM_H2NL( x ) \
    (UINT32)((((UINT32)(x)) >> 24)    \
             | (((x) >> 8) & 0xff00)  \
             | ((x) << 24)            \
             | (((x) & 0xff00) << 8))

#define TPM_N2HS( x ) TPM_H2NS( x )
#define TPM_N2HL( x ) TPM_H2NL( x )

//
// PCR Usage
//
#define AMI_PCRi_CRTM_AND_POST_BIOS                  0
#define AMI_PCRi_HOST_PLATFORM_CONFIG                1
#define AMI_PCRi_OPROM_CODE                          2
#define AMI_PCRi_OPROM_CONFIG_AND_DATA               3
#define AMI_PCRi_IPL_CODE                            4
#define AMI_PCRi_IPL_CONFIG_AND_DATA                 5
#define AMI_PCRi_STATE_TRANSITION                    6
#define AMI_PCRi_HOST_PLATFORM_MANUFACTURER_CONTROL  7

//
// TCG Event Types
//
#ifndef EV_PREBOOT_CERT
#define EV_PREBOOT_CERT             0
#endif
#ifndef EV_UNUSED
#define EV_UNUSED                   2
#endif
#ifndef EV_NO_ACTION
#define EV_NO_ACTION                3
#endif
#ifndef EV_ACTION
#define EV_ACTION                   5
#endif
#define EV_EVENT_TAG                6
#if defined (CORE_COMBINED_VERSION) && (CORE_COMBINED_VERSION < 0x50004)
#define EV_CPU_MICROCODE            9
#endif

#ifndef EV_PLATFORM_CONFIG_FLAGS  
#define EV_PLATFORM_CONFIG_FLAGS    10
#endif

#ifndef EV_COMPACT_HASH  
#define EV_COMPACT_HASH             12
#endif 

#ifndef EV_IPL  
#define EV_IPL                      13
#endif

#ifndef EV_IPL_PARTITION_DATA  
#define EV_IPL_PARTITION_DATA       14
#endif

#ifndef EV_NONHOST_CODE  
#define EV_NONHOST_CODE             15
#endif

#ifndef EV_NONHOST_CONFIG  
#define EV_NONHOST_CONFIG           16
#endif

#ifndef EV_ID_OPROM_EXECUTE 
#define EV_ID_OPROM_EXECUTE             7
#endif

#pragma pack (push)
#pragma pack (1)

typedef struct _TPM_1_2_REGISTERS
{
    UINT8  Access;            // 0
    UINT8  Reserved1[7];      // 1
    UINT32 IntEnable;         // 8
    UINT8  IntVector;         // 0ch
    UINT8  Reserved2[3];      // 0dh
    UINT32 IntSts;            // 10h
    UINT32 IntfCapability;    // 14h
    UINT8  Sts;               // 18h
    UINT16 BurstCount;        // 19h
    UINT8  Reserved3[9];
    UINT32 DataFifo;          // 24h
    UINT8  Reserved4[0xed8];  // 28h
    UINT16 Vid;               // 0f00h
    UINT16 Did;               // 0f02h
    UINT8  Rid;               // 0f04h
    UINT8  TcgDefined[0x7b];  // 0f05h
    UINT32 LegacyAddress1;    // 0f80h
    UINT32 LegacyAddress1Ex;  // 0f84h
    UINT32 LegacyAddress2;    // 0f88h
    UINT32 LegacyAddress2Ex;  // 0f8ch
    UINT8  VendorDefined[0x70]; // 0f90h
} TPM_1_2_REGISTERS;

typedef volatile TPM_1_2_REGISTERS *TPM_1_2_REGISTERS_PTR;


//
// _EFI_TCG_EV_S_CRTM_VERSION
//
struct _AMI_EFI_TCG_EV_S_CRTM_VERSION
{
    EFI_GUID VersionGuid;
};

//
// _EFI_TCG_EV_POST_CODE
//
struct _AMI_EFI_TCG_EV_POST_CODE
{
    EFI_PHYSICAL_ADDRESS PostCodeAddress;
    UINT64               PostCodeLength;
};

//
// _EFI_TCG_EV_TAG_OPTION_ROM_EXECUTE
//
struct _AMI_EFI_TCG_EV_TAG_OPTION_ROM_EXECUTE
{
    UINT16     Reserved;
    UINT16     PFA;
    TCG_DIGEST Hash;
};

//
// _EFI_TCG_EV_CPU_MICROCODE
//
struct _AMI_EFI_TCG_EV_CPU_MICROCODE
{
    EFI_PHYSICAL_ADDRESS MicrocodeEntrypoint;
};

//
// _EFI_TCG_EV_TAG_EVENT_DATA
//
union _AMI_EFI_TCG_EV_TAG_EVENT_DATA
{
    struct _AMI_EFI_TCG_EV_TAG_OPTION_ROM_EXECUTE
    OptionRomExecute;
};

//
// _EFI_TCG_EV_TAG
//
struct _AMI_EFI_TCG_EV_TAG
{
    UINT32 EventID;
    UINT32 EventSize;
    union _AMI_EFI_TCG_EV_TAG_EVENT_DATA EventData;
};

//
// EFI_TCG_PCR_EVENT_HEADER
//
typedef struct _EFI_TCG_PCR_EVENT_HEADER
{
    TPM_PCRINDEX PCRIndex;
    UINT32       EventType;
    TCG_DIGEST   Digest;
    UINT32       EventDataSize;
} AMI_EFI_TCG_PCR_EVENT_HEADER;

//
// EFI_TCG_PCR_EVENT_DATA
//
typedef union _EFI_TCG_PCR_EVENT_DATA
{
    struct _AMI_EFI_TCG_EV_S_CRTM_VERSION SCrtmVersion;
    struct _AMI_EFI_TCG_EV_POST_CODE PostCode;
    struct _AMI_EFI_TCG_EV_TAG Tagged;
    struct _AMI_EFI_TCG_EV_CPU_MICROCODE Mcu;
} AMI_EFI_TCG_PCR_EVENT_DATA;

//
// EFI_TCG_PCR_EVENT
// equivalent to TCG_PCR_EVENT in the TCG PC Client Spec
//
typedef struct _EFI_TCG_PCR_EVENT
{
    AMI_EFI_TCG_PCR_EVENT_HEADER Header;
    AMI_EFI_TCG_PCR_EVENT_DATA   Event;
} AMI_EFI_TCG_PCR_EVENT;

#pragma pack (pop)

#endif  // _TPMPC_H_
