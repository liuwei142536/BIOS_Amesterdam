/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformErrorHandler.h

Abstract:
  This file will contain all definitions related to Platform Specific Error handling
------------------------------------------------------------------*/
	
#ifndef _PLATFORM_ERROR_HANDLER_H
#define _PLATFORM_ERROR_HANDLER_H

#include <Library\emcaplatformhookslib.h>
#include <IndustryStandard/WheaDefs.h>
#include <Library\emcalib.h>

//
// All #defines should go below this comment
//

// 
// PCH SMI enable and GPIO SMI enable/status
//
#define R_ACPI_SMI_EN                  0x30
#define R_ACPI_ALT_GP_SMI_EN           0x38
#define  S_ACPI_ALT_GP_SMI_EN          2
#define R_ACPI_ALT_GP_SMI_STS          0x3A
#define  S_ACPI_ALT_GP_SMI_STS         2

//SMI_STS - SMI Status Register
#define R_ACPI_SMI_STS                                 0x34
#define  S_ACPI_SMI_EN                                 4
#define   B_GPIO_UNLOCK_SMI_EN                         BIT27
#define   B_EL_SMI_EN                                  BIT25
#define   B_INTEL_USB2_EN                              BIT18
#define   B_LEGACY_USB2_EN                             BIT17
#define   B_PERIODIC_EN                                BIT14
#define   B_TCO_EN                                     BIT13
#define   B_MCSMI_EN                                   BIT11
#define   B_BIOS_RLS                                   BIT7
#define   B_SWSMI_TMR_EN                               BIT6
#define   B_APMC_EN                                    BIT5
#define   B_SMI_ON_SLP_EN                              BIT4
#define   B_LEGACY_USB_EN                              BIT3
#define   B_BIOS_EN                                    BIT2
#define   B_EOS                                        BIT1
#define   B_GBL_SMI_EN                                 BIT0

#define V_SYSMAP_NoInband  000 // No inband message

#define ICH_GPI_SMI_ERROR_PIN_CV        2
#define ICH_GPI_SMI_CAT_ERROR_PIN_CV    8


#define BOOT_FLOW_CONDITION_ERROR       1

#define	BOOT_FLOW_VARIABLE_GUID \
	{ 0xEF152FB4, 0x7B2F, 0x427D, 0xBD, 0xB4, 0x7E, 0x0A, 0x05, 0x82, 0x6E, 0x64 }

#define BCD2DEC(x)  ((x & 0x0F) + ((x >> 4) * 10))


#pragma pack(1)

typedef struct {
  UINT32  Year : 4;
  UINT32  Month : 4;
  UINT32  Day : 5;
  UINT32  Hour : 5;
  UINT32  Minute : 6;
  UINT32  Second : 6;
} TIME_STAMP;

typedef struct {
  UINT16      RecordId;
  UINT8       RecordType;
  TIME_STAMP  TimeStamp;
  UINT16      GeneratorId;
  UINT8       EvMRevision;
  UINT8       SensorType;
  UINT8       SensorNumber;
  UINT8       EventDirType;
  UINT8       OEMEvData1;
  UINT8       OEMEvData2;
  UINT8       OEMEvData3;
/*Aptio V Server Override - Start: Sizeof(EFI_SEL_RECORD_DATA) returns 0x1E and LogInfoToSel()-> SendIpmiCommand returns the response code as 0xC7 - Invalid Request Data Length.
  UINT16      OEMEvData4;
  UINT64      OEMEvData5;
  UINT32      OEMEvData6;
  Aptio V Server Override - End: Sizeof(EFI_SEL_RECORD_DATA) returns 0x1E and LogInfoToSel()-> SendIpmiCommand returns the response code as 0xC7 - Invalid Request Data Length.*/
} EFI_SEL_RECORD_DATA;


typedef union {
    struct {
    UINT8       Offset:4;
    UINT8       Data:4;
    } BitMap;

    UINT8 Raw;
} EXTENDED_ERROR_DATA_RECORD_HEADER;


typedef struct {
  //
  // Header-like information...
  //
  UINT8                                   Source;
  UINT8                                   Type;

  //
  // Essential information...
  //
  UINT8                                  SubType;
  UINT8                                   Socket;
  UINT8                                   MemoryController; //AptioV Server Override: Memory Controller information is added.
  UINT8                                   Channel;
  UINT8                                   Dimm;
  UINT32                                  Syndrome;
  UINT32                                  EccLocator;
  UINT8                                   RasConfig;  // 0 - Spare, 1 - Mirror
  BOOLEAN                                 Valid;
  UINT8                                   Rank;
  UINT8                                   Count;
  UINT16                                  MachineCheckCode; // Will go as extended data...

  //
  // Extended information...
  //
  UINT8                                   Parent;
  EXTENDED_ERROR_DATA_RECORD_HEADER       Header;
  UINT8                                   ExtendedDataPointer;
  UINT8                                   ExtendedData[1];
} MEMORY_ERROR_RECORD;

typedef struct _EFI_MEMORY_EXTENDED_ERR_RECORD {
  UINT32      Syndrome;
} EFI_MEMORY_EXTENDED_ERR_RECORD;

typedef struct _MEMORY_COMBINED_ERROR_RECORD {
  MEMORY_ERROR_RECORD             PrimaryBlock;
  EFI_MEMORY_EXTENDED_ERR_RECORD  ExtendedBlock;    
} MEMORY_COMBINED_ERROR_RECORD;
#pragma pack()





extern EFI_SMBIOS_PROTOCOL			*mSmbiosProtocol;
extern BOOLEAN						mWheaLogStarted;	
extern BOOLEAN						mWheaFfmCorErrLogging;
extern BOOLEAN						mWheaFfmUnCorErrLogging;
extern TIME_STAMP					mCorrectedErrorRankTimeStamp[MC_MAX_NODE][MAX_CH][MAX_RANK_CH];
extern UINT32						mEccErrorSmiCount;
extern EFI_GUID 					gEfiSmmGpiDispatch2ProtocolGuid;

EFI_STATUS
ProcessPlatformUncorrectedError (
  VOID
);

EFI_STATUS
FillErrorSection
(
UINT8 McBank,
UINT8 *skt,
UINT32 Sev,
EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
GENERIC_ERROR_STATUS *ErrSts
);

EFI_STATUS
ConstructErrorRecord
(
  UINT32 Sev,
  GENERIC_ERROR_STATUS *ErrSts
);

EFI_STATUS
LogWheaRecord
(
  UINT8 McBank,
  EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
  GENERIC_ERROR_STATUS *ErrSts
);

EFI_STATUS
LogErrorRecord
(
UINT8 McBank,
EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
GENERIC_ERROR_STATUS *ErrSts
);


EFI_STATUS
EfiSmmGetTime (
  IN OUT TIME_STAMP *Time
  );



VOID
ElogMcExecuteCorrectableErrorHandlingPolicies (
  MEMORY_COMBINED_ERROR_RECORD  *MemoryErrRecord
  );

EFI_STATUS
EnableGpiChildHandler (
  UINT8     GpiNumber
  );

UINT32
EfiSmmGetNoofHoursPast (
  IN OUT TIME_STAMP *PrevTime
  );

VOID
ClearPlatformErrors(
 VOID
 );

#endif


