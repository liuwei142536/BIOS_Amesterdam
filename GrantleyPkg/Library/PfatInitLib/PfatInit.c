/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c) 2011 - 2015 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  PfatInit.c

@brief
  PFAT EarlyPost initializations.
**/

#include <Token.h> //AptioV server override: Pfat function implement for Grantley workstation.
#include "PfatInit.h"
#include "Library\BaseCryptLib.h"
//-#include "FlashMap.h" //AptioV server override: Pfat function implement for Grantley workstation.
#include <Guid/PlatformInfo.h>
// Might be needed for PFAT EC Support in the future
#ifdef PFAT_EC_FLAG
//#include EFI_PPI_DEPENDENCY (CpuIo)
//#include EFI_PPI_PRODUCER   (Stall)
//#include "PeiKscLib.h"
#endif //PFAT_EC_FLAG
//AptioV server override start: Pfat function implement for Grantley workstation.
#define BIOS_GUARD_PUB_KEY_FFS_FILE_RAW_GUID \
    {0x8e295870, 0xd377, 0x4b75, 0xbf, 0xdc, 0x9a, 0xe2, 0xf6, 0xdb, 0xde, 0x22}

EFI_GUID pFatFileGuid = BIOS_GUARD_PUB_KEY_FFS_FILE_RAW_GUID;

#define KBShift  10
#define MBShift  20
//AptioV server override end: Pfat function implement for Grantley workstation.
UINT8 PfatSupportedPlatforms[]  = {
#ifndef GRANGEVILLE_FLAG
          TypeMayanCitySRP,
          TypeMayanCitySRPDDR3,
          TypeAztecCityCRBSRP,
          TypeAztecCitySTHI,
          TypeHedtReversedIdBits,
          TypeKahunaDVP2SPbg,
          TypeKahunaDVP2S,
          TypeHedtSKU1CRB,
          TypeHedtEv,
          TypeHedtDDR3
#else
          TypeBeverlyCoveCRB,
          TypeBeverlyCoveSODimmCRB,

#endif
          };
//AptioV server override start: Pfat function implement for Grantley workstation.
#if defined(BiosGuard_SUPPORT) && BiosGuard_SUPPORT == 1
EFI_STATUS
LocatBiosGuardPubKey (IN EFI_PEI_SERVICES  **PpSv, IN OUT VOID **Buffer)
{
    EFI_STATUS                    Status;
    EFI_FIRMWARE_VOLUME_HEADER    *pFV;
    UINTN                         FvNum=0;
    EFI_FFS_FILE_HEADER           *ppFile=NULL;
    BOOLEAN                       Found = FALSE;

    Status = (*PpSv)->FfsFindNextVolume (PpSv, FvNum, &pFV);

    while ( TRUE )
    {
        Status = (*PpSv)->FfsFindNextVolume( PpSv, FvNum, &pFV );
        if ( EFI_ERROR( Status ))
        {
            return Status;
        }

        ppFile = NULL;

        while ( TRUE )
        {
            Status = (*PpSv)->FfsFindNextFile( PpSv,
                                               EFI_FV_FILETYPE_FREEFORM,
                                               pFV,
                                               &ppFile );

            if ( Status == EFI_NOT_FOUND )
            {
                break;
            }

            DEBUG ((EFI_D_INFO, " ppFile guid:: %x \n", ppFile->Name));

            if (CompareGuid( &ppFile->Name, &pFatFileGuid ))
            {
                Found = TRUE;
                break;
            }
        }

        if ( Found )
        {
            break;
        }
        else {
            FvNum++;
        }
    }

     Status = (*PpSv)->FfsFindSectionData( PpSv,
                                          EFI_SECTION_RAW,
                                          ppFile,
                                          Buffer );

    if (EFI_ERROR( Status )) {
        return EFI_NOT_FOUND;
    }

    return Status;
}

STATIC
EFI_STATUS
AmiCustomizePfatConfigData (
    IN EFI_PEI_SERVICES     **PeiServices,
    IN PFAT_CONFIG          *PfatConfig    
)
{
    EFI_STATUS                     Status;
    UINT32                         TotalROMSize = 0;
    UINTN                          BiosGuardHashDataSize = 0;
    UINT8                          BiosGuardHash256Val[32];
    UINT8                          *Sha256PubKeyDigest;
    VOID                           *BiosGuardSha256Context;
    VOID                           *BiosGuardData;

    BiosGuardHashDataSize = Sha256GetContextSize ();
    Status = ((*PeiServices)->AllocatePool) (PeiServices, BiosGuardHashDataSize, &BiosGuardSha256Context);
    if ( EFI_ERROR( Status ) ) {
        return   Status;
    }

    Status  = LocatBiosGuardPubKey(PeiServices ,&Sha256PubKeyDigest);
    //
    // If it fails it might be in non PUPC mode
    //
    if ( !EFI_ERROR( Status ) ) {
        PfatConfig->PupHeader.PkgAttributes = 0;
        CopyMem (&PfatConfig->Ppdt.PkeySlot0, Sha256PubKeyDigest, 32);
    }

    ///
    /// Select to Flash Map 0 Register to get the number of flash Component
    ///
    MmioAndThenOr32 (
        SB_RCBA + R_PCH_SPI_FDOC,
        (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
        (UINT32) (V_PCH_SPI_FDOC_FDSS_FSDM | R_PCH_SPI_FDBAR_FLASH_MAP0)
    );

    switch ( MmioRead16 (SB_RCBA + R_PCH_SPI_FDOD) & B_PCH_SPI_FDBAR_NC ) {
    case V_PCH_SPI_FDBAR_NC_1:
        PfatConfig->NumSpiComponents  = 1;
        break;
    case V_PCH_SPI_FDBAR_NC_2:
        PfatConfig->NumSpiComponents  = 2;
        break;
    default:
        break;
    }

    ///
    /// Select to Flash Components Register to get the Component 1 Density
    ///
    MmioAndThenOr32 (
        SB_RCBA + R_PCH_SPI_FDOC,
        (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
        (UINT32) (V_PCH_SPI_FDOC_FDSS_COMP | R_PCH_SPI_FCBA_FLCOMP)
    );

    ///
    /// Copy Component 1 Density
    ///
    switch ( (UINT8) MmioRead32 (SB_RCBA + R_PCH_SPI_FDOD) & B_PCH_SPI_FLCOMP_COMP1_MASK ) {
    case V_PCH_SPI_FLCOMP_COMP1_512KB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize512KB;
        TotalROMSize += (UINT32) LShiftU64(512, KBShift);
        break;
    case V_PCH_SPI_FLCOMP_COMP1_1MB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize1MB;
        TotalROMSize += (UINT32) LShiftU64(1, MBShift);
        break;
    case V_PCH_SPI_FLCOMP_COMP1_2MB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize2MB;
        TotalROMSize += (UINT32) LShiftU64(2, MBShift);
        break;
    case V_PCH_SPI_FLCOMP_COMP1_4MB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize4MB;
        TotalROMSize += (UINT32) LShiftU64(4, MBShift);
        break;
    case V_PCH_SPI_FLCOMP_COMP1_8MB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize8MB;
        TotalROMSize += (UINT32) LShiftU64(8, MBShift);
        break;
    case V_PCH_SPI_FLCOMP_COMP1_16MB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize16MB;
        TotalROMSize += (UINT32) LShiftU64(16, MBShift);
        break;
    case V_PCH_SPI_FLCOMP_COMP1_32MB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize32MB;
        TotalROMSize += (UINT32) LShiftU64(32, MBShift);
        break;
    case V_PCH_SPI_FLCOMP_COMP1_64MB:
        PfatConfig->ComponentSize[0]  = EnumSpiCompSize64MB;
        TotalROMSize += (UINT32) LShiftU64(64, MBShift);
        break;
    default:
        break;
    }

    ///
    /// Copy Component 2 Density
    ///
    if ( PfatConfig->NumSpiComponents == 2 ) {
        switch ( (UINT8) MmioRead32 (SB_RCBA + R_PCH_SPI_FDOD) & B_PCH_SPI_FLCOMP_COMP2_MASK ) {
        case V_PCH_SPI_FLCOMP_COMP2_512KB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize512KB;
            TotalROMSize += (UINT32) LShiftU64(512, KBShift);
            break;
        case V_PCH_SPI_FLCOMP_COMP2_1MB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize1MB;
            TotalROMSize += (UINT32) LShiftU64(1, MBShift);
            break;
        case V_PCH_SPI_FLCOMP_COMP2_2MB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize2MB;
            TotalROMSize += (UINT32) LShiftU64(2, MBShift);
            break;
        case V_PCH_SPI_FLCOMP_COMP2_4MB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize4MB;
            TotalROMSize += (UINT32) LShiftU64(4, MBShift);
            break;
        case V_PCH_SPI_FLCOMP_COMP2_8MB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize8MB;
            TotalROMSize += (UINT32) LShiftU64(8, MBShift);
            break;
        case V_PCH_SPI_FLCOMP_COMP2_16MB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize16MB;
            TotalROMSize += (UINT32) LShiftU64(16, MBShift);
            break;
        case V_PCH_SPI_FLCOMP_COMP2_32MB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize32MB;
            TotalROMSize += (UINT32) LShiftU64(32, MBShift);
            break;
        case V_PCH_SPI_FLCOMP_COMP2_64MB:
            PfatConfig->ComponentSize[1]  = EnumSpiCompSize64MB;
            TotalROMSize += (UINT32) LShiftU64(64, MBShift);
            break;
        default:
            break;
        }
    }

    //
    // LastSfam : Number of SfamData - 1
    //
    PfatConfig->Ppdt.LastSfam = BIOS_GUARD_SFAM_COUNT - 1;
    if (PfatConfig->Ppdt.LastSfam > (MAX_SFAM_COUNT - 1)) {
        PfatConfig->Ppdt.LastSfam = MAX_SFAM_COUNT - 1;
    }

    //
    // SfamData[0] : BIOS Region only skip NVRAM ( Protect NVRAM_BACKUP_ADDRESS + NVRAM_SIZE ~ TotalROMSize )
    //
    PfatConfig->Ppdt.SfamData[0].FirstByte = TotalROMSize - (0xFFFFFFFF - (NVRAM_BACKUP_ADDRESS + NVRAM_SIZE) + 1);
    PfatConfig->Ppdt.SfamData[0].LastByte  = TotalROMSize - 1;
    DEBUG ((EFI_D_INFO, "[BiosGuardCpuPolicyOverride.c] PfatConfig->Ppdt.SfamData[0].FirstByte: %X \n", PfatConfig->Ppdt.SfamData[0].FirstByte));
    DEBUG ((EFI_D_INFO, "[BiosGuardCpuPolicyOverride.c] PfatConfig->Ppdt.SfamData[0].LastByte: %X \n", PfatConfig->Ppdt.SfamData[0].LastByte));


    //
    // SfamData[1] : BIOS Region only skip NVRAM ( Protect 0 ~ NVRAM_ADDRESS )
    //
    PfatConfig->Ppdt.SfamData[1].FirstByte = 0x00;
    PfatConfig->Ppdt.SfamData[1].LastByte  = TotalROMSize - (0xFFFFFFFF - NVRAM_ADDRESS + 1) -1;
    PfatConfig->Ppdt.PpdtSize = (sizeof (PPDT) - sizeof (PfatConfig->Ppdt.SfamData) + ((PfatConfig->Ppdt.LastSfam + 1) * sizeof (SFAM_DATA)));
    DEBUG ((EFI_D_INFO, "[BiosGuardCpuPolicyOverride.c] PfatConfig->Ppdt.SfamData[1].FirstByte: %X \n", PfatConfig->Ppdt.SfamData[1].FirstByte));
    DEBUG ((EFI_D_INFO, "[BiosGuardCpuPolicyOverride.c] PfatConfig->Ppdt.SfamData[1].LastByte: %X \n", PfatConfig->Ppdt.SfamData[1].LastByte));

    //
    // Defined values in the token
    //
    PfatConfig->Ppdt.BiosSvn = BIOS_SVN;
    PfatConfig->PupHeader.BiosSvn = PfatConfig->Ppdt.BiosSvn;
    PfatConfig->PfatMemSize = (UINT8) RShiftU64(BiosGuardMaxSectionSize, MBShift) + 1;

    //
    // Hash the PPDT to PfatConfig->PpdtHash
    //
    BiosGuardHashDataSize = PfatConfig->Ppdt.PpdtSize;
    BiosGuardData = (VOID *) &PfatConfig->Ppdt;
    Sha256Init (BiosGuardSha256Context);
    Sha256Update (BiosGuardSha256Context, BiosGuardData, BiosGuardHashDataSize);
    Sha256Final (BiosGuardSha256Context,  BiosGuardHash256Val);
    CopyMem (&PfatConfig->PpdtHash[0], &BiosGuardHash256Val[0], 8);
    CopyMem (&PfatConfig->PpdtHash[1], &BiosGuardHash256Val[8], 8);
    CopyMem (&PfatConfig->PpdtHash[2], &BiosGuardHash256Val[16], 8);
    CopyMem (&PfatConfig->PpdtHash[3], &BiosGuardHash256Val[24], 8);

    return   Status;
}
#endif
//AptioV server override end: Pfat function implement for Grantley workstation.

#ifdef PFAT_EC_FLAG
/**
  Gets CPU's random number generator.

  @param[out] UINT32  - Random value
**/
UINT32
RandomNumber (
        void
  )
{
  UINT32 Random = 0;

#ifndef VP_FLAG
  ///
  /// Assembly instruction to read CPU's random number generator
  /// Instruction is only available 100k cycles after reset
  ///
  __asm {
tryAgain:
    ;    rdrand eax
    ;    db 0Fh, 0C7h, 0F0h

    _emit 0x0F
    _emit 0xC7
    _emit 0xF0

    mov Random, eax
    jnc tryAgain; CF will be set is valid number was generated
  }
#endif

  return (Random);
}
#endif //PFAT_EC_FLAG

VOID
RestoreAntiFlashWearout(
    IN EFI_PEI_SERVICES           **PeiServices,
    IN SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr,
    IN struct sysHost             *host
)
/*++

Routine Description:

  Perform the platform specific restoration of anti-flash wear out settings in case of S3 resume.

Arguments:

  **PeiServices                   - Indirect reference to the PEI Services Table.
  *TotalSystemConfigurationPtr    - pointer to system configuration variable
  *host                           - sysHost struct with system configuration information

Returns:

  None

--*/
{
  EFI_STATUS          Status;
  UINT8               i;
  UINT64              MsrValue;
  UINT64_STRUCT       MsrReg;
  EFI_BOOT_MODE       BootMode=0;

  //
  // Check the platform specific flag for anti-flash wearout feature
  //
  if ( TotalSystemConfigurationPtr->AntiFlashWearoutSupported == TRUE ) {
    //
    // Write WPE bit on S3 Resume in BIOS Info Flags MSR to enable Anti-Flash wearout protection
    //
    Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
    if ((BootMode == BOOT_ON_S3_RESUME) && (TotalSystemConfigurationPtr->EnableAntiFlashWearout)) {
      MsrValue = AsmReadMsr64 (MSR_BIOS_INFO_FLAGS);
      for(i = 0; i < host->var.common.numCpus; i++) {
        MsrValue |= B_MSR_BIOS_INFO_FLAGS_WPE;
        MsrReg.hi = 0;
        MsrReg.lo = (UINT32)MsrValue;
        WriteMsrPipe(host, i, MSR_BIOS_INFO_FLAGS, MsrReg);
      }
    }
  }
}

VOID
InitializePfatConfigData(
  IN PFAT_CONFIG *PfatConfig
  )
/*++

Routine Description:
  
  Initial settings for PFAT Config structure.  Taken from Sample Code.

Arguments:

  PfatConfig           - Pointer to the PFat config structure.

Returns: 

  None

--*/
{
  UINT8 PlatIdStr[] = "GRANTLEY";

  ///
  /// If CpuConfig->Pfat is set to ENABLE '1' then
  ///   PlatformData->SmmBwp (found in PchPolicyInitPei.c file) has to be set to ENABLE '1'
  /// This is a PFAT Security requirement that needs to be addressed
  /// If CpuConfig->Pfat is set to DISABLE '0' then
  ///   PlatformData->SmmBwp (found in PchPolicyInitPei.c file) value don't care, it can be
  ///   set to either ENABLE '1' or DISABLE '0' based on customer implementation
  ///
  ZeroMem (&PfatConfig->Ppdt, sizeof (PPDT));
  PfatConfig->Ppdt.PpdtMajVer             = PPDT_MAJOR_VERSION;
  PfatConfig->Ppdt.PpdtMinVer             = PPDT_MINOR_VERSION;
  CopyMem (&PfatConfig->Ppdt.PlatId[0], &PlatIdStr[0], sizeof (PlatIdStr));
  PfatConfig->Ppdt.PfatModSvn             = PFAT_SVN;
  PfatConfig->Ppdt.BiosSvn                = 0x00010000;
  PfatConfig->Ppdt.ExecLim                = 0;
  PfatConfig->Ppdt.PlatAttr               = 0;
  PfatConfig->Ppdt.LastSfam               = MIN_SFAM_COUNT - 1;
  if (PfatConfig->Ppdt.LastSfam > (MAX_SFAM_COUNT - 1)) {
    PfatConfig->Ppdt.LastSfam             = MAX_SFAM_COUNT - 1;
  }
  ///
  /// SfamData [LastSfam + 1]
  ///
  PfatConfig->Ppdt.SfamData[0].FirstByte  = 0x00580000;
  PfatConfig->Ppdt.SfamData[0].LastByte   = 0x0058FFFF;
  PfatConfig->Ppdt.PpdtSize               = (sizeof (PPDT) - sizeof (PfatConfig->Ppdt.SfamData) + ((PfatConfig->Ppdt.LastSfam + 1) * sizeof (SFAM_DATA)));
  PfatConfig->PpdtHash[0]  = 0xae7295370672663c;
  PfatConfig->PpdtHash[1]  = 0x220375c996d23a36;
  PfatConfig->PpdtHash[2]  = 0x73aaea0f2afded9d;
  PfatConfig->PpdtHash[3]  = 0x707193b768a0829e;
  ZeroMem (&PfatConfig->PupHeader, sizeof (PUP_HEADER));
  PfatConfig->PupHeader.Version         = PUP_HDR_VERSION;
  CopyMem (&PfatConfig->PupHeader.PlatId[0], &PlatIdStr[0], sizeof (PlatIdStr));
  PfatConfig->PupHeader.PkgAttributes   = 0;
  PfatConfig->PupHeader.PslMajorVer     = PSL_MAJOR_VERSION;
  PfatConfig->PupHeader.PslMinorVer     = PSL_MINOR_VERSION;
  PfatConfig->PupHeader.BiosSvn         = PfatConfig->Ppdt.BiosSvn;
  PfatConfig->PupHeader.EcSvn           = 0;
  PfatConfig->PupHeader.VendorSpecific  = 0x808655AA;
  ZeroMem (&PfatConfig->PfatLog, sizeof (PFAT_LOG));
  PfatConfig->PfatLog.Version         = PFAT_LOG_VERSION;
  PfatConfig->PfatLog.LastPage        = 0;
  if (PfatConfig->PfatLog.LastPage > (MAX_PFAT_LOG_PAGE - 1)) {
    PfatConfig->PfatLog.LastPage      = MAX_PFAT_LOG_PAGE - 1;
  }
  PfatConfig->PfatLog.LoggingOptions  = 0;
  PfatConfig->NumSpiComponents  = 2;
  PfatConfig->ComponentSize[0]  = EnumSpiCompSize8MB;
  PfatConfig->ComponentSize[1]  = EnumSpiCompSize8MB;
  PfatConfig->PfatMemSize       = 0x09;

  return;
}

#if 0 //AptioV server override: Pfat function implement for Grantley workstation.
VOID
CustomizePfatConfigData(
  IN EFI_PEI_SERVICES     **PeiServices,
  IN PFAT_CONFIG          *PfatConfig,
  IN UINT8                AntiFlashWearoutEnabled
  )
/*++

Routine Description:
  
  Final customizations to the PFAT Platform structure are made here.
  Keys or any other specific changes should be done here.

Arguments:

  **PeiServices        - Indirect reference to the PEI Services Table.
  *PfatConfig          - Pointer to the PFat config structure.

Returns: 

  None

--*/
{
  EFI_STATUS        Status;
  UINT32            FlashBase;
  UINT32            FlashAddr;
  UINT8             i;
  VOID              *Sha256Context;
  VOID              *Data;
  UINTN             DataSize;
  UINT8             HashValue[SHA256_DIGEST_SIZE];
#ifdef PFAT_EC_FLAG
  UINT8             EcStatus;
  PEI_STALL_PPI     *StallPpi;
  PEI_CPU_IO_PPI    *CpuIo;
#endif //PFAT_EC_FLAG
  UINT8             PpdtPkeySlot0[] = { 0x6a, 0x94, 0x81, 0x87, 0xfd, 0x8c, 0x51, 0x0a, 0x2a, 0x74, 0x9c, 0x7b, 0xe3, 0xf7, 0x5c, 0x3a, 0xbf, 0xce, 0x60, 0x4f, 0x68, 0xeb, 0xc6, 0x8f, 0x69, 0x98, 0x8c, 0xc5, 0xdb, 0xe3, 0x29, 0x70 };
  UINT8             PpdtPkeySlot1[] = { 0x1c, 0x51, 0x3a, 0xc5, 0x1c, 0x3d, 0x63, 0x10, 0xef, 0xed, 0xfe, 0xc5, 0xf3, 0x4b, 0xf2, 0xe0, 0x9b, 0x22, 0xe8, 0x06, 0xab, 0xd3, 0x19, 0x2e, 0xfa, 0xfb, 0x6e, 0xd9, 0x36, 0x0d, 0x68, 0x18 };
  UINT8             PpdtPkeySlot2[] = { 0x19, 0x0b, 0x33, 0xf8, 0xde, 0x3a, 0xa7, 0x9b, 0x57, 0xad, 0xb2, 0x45, 0x86, 0x0e, 0x7f, 0x0e, 0x40, 0x62, 0x80, 0x22, 0x8f, 0x04, 0x92, 0xec, 0x87, 0x44, 0x81, 0xd9, 0xef, 0xed, 0x9f, 0xa3 };

  //
  // Select to Flash Map 0 Register to get the number of flash Component
  //
  PchMmRcrb32AndThenOr (
    R_PCH_SPI_FDOC,
    (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
    (UINT32) (V_PCH_SPI_FDOC_FDSS_FSDM | R_PCH_SPI_FDBAR_FLASH_MAP0)
  );
  //
  // Copy Zero based Number Of Components
  //
  PfatConfig->NumSpiComponents = (UINT8) (((PchMmRcrb16 (R_PCH_SPI_FDOD) & B_PCH_SPI_FDBAR_NC) >> N_PCH_SPI_FDBAR_NC) + 1);
  //
  // Select to Flash Components Register to get Components Density
  //
  PchMmRcrb32AndThenOr (
    R_PCH_SPI_FDOC,
    (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
    (UINT32) (V_PCH_SPI_FDOC_FDSS_COMP | R_PCH_SPI_FCBA_FLCOMP)
  );
  //
  // Copy Components Density
  //
  FlashAddr = (UINT8) PchMmRcrb32 (R_PCH_SPI_FDOD);
  PfatConfig->ComponentSize[0] = (UINT8) (FlashAddr & B_PCH_SPI_FLCOMP_COMP1_MASK);
  PfatConfig->ComponentSize[1] = (UINT8) ((FlashAddr & B_PCH_SPI_FLCOMP_COMP2_MASK) >> 4);
  FlashAddr = 0;
  for (i = 0; i < PfatConfig->NumSpiComponents; i++) {
    FlashAddr += (SPI_SIZE_BASE_512KB << PfatConfig->ComponentSize[i]);
  }
  FlashBase = FlashAddr - FLASH_SIZE;
  PfatConfig->Ppdt.LastSfam              = 8;
  PfatConfig->Ppdt.SfamData[0].FirstByte = FlashBase + FLASH_REGION_FVMAIN_OFFSET;
  PfatConfig->Ppdt.SfamData[0].LastByte  = FlashBase + FLASH_REGION_FVMAIN_OFFSET + FLASH_REGION_FVMAIN_SIZE - 1;
  PfatConfig->Ppdt.SfamData[1].FirstByte = FlashBase + FLASH_REGION_OEM_FV_OFFSET;
  PfatConfig->Ppdt.SfamData[1].LastByte  = FlashBase + FLASH_REGION_OEM_FV_OFFSET + FLASH_REGION_OEM_FV_SIZE - 1;
  PfatConfig->Ppdt.SfamData[2].FirstByte = FlashBase + FLASH_REGION_FLAVOR_FV_OFFSET;
  PfatConfig->Ppdt.SfamData[2].LastByte  = FlashBase + FLASH_REGION_FLAVOR_FV_OFFSET + FLASH_REGION_FLAVOR_FV_SIZE - 1;
  PfatConfig->Ppdt.SfamData[3].FirstByte = FlashBase + FLASH_REGION_MICROCODE_FV_OFFSET;
  PfatConfig->Ppdt.SfamData[3].LastByte  = FlashBase + FLASH_REGION_MICROCODE_FV_OFFSET + FLASH_REGION_MICROCODE_FV_SIZE - 1;
  PfatConfig->Ppdt.SfamData[4].FirstByte = FlashBase + FLASH_REGION_FV_MRC_NORMAL_OFFSET;
  PfatConfig->Ppdt.SfamData[4].LastByte  = FlashBase + FLASH_REGION_FV_MRC_NORMAL_OFFSET + FLASH_REGION_FV_MRC_NORMAL_SIZE - 1;  
  PfatConfig->Ppdt.SfamData[5].FirstByte = FlashBase + FLASH_REGION_BIOS_ACM_FV_HEADER_OFFSET;
  PfatConfig->Ppdt.SfamData[5].LastByte  = FlashBase + FLASH_REGION_BIOS_ACM_FV_HEADER_OFFSET + FLASH_REGION_BIOS_ACM_FV_HEADER_SIZE - 1;
  PfatConfig->Ppdt.SfamData[6].FirstByte = FlashBase + FLASH_REGION_BIOS_ACM_OFFSET;
  PfatConfig->Ppdt.SfamData[6].LastByte  = FlashBase + FLASH_REGION_BIOS_ACM_OFFSET + FLASH_REGION_BIOS_ACM_SIZE - 1;
  PfatConfig->Ppdt.SfamData[7].FirstByte = FlashBase + FLASH_REGION_SINIT_ACM_OFFSET;
  PfatConfig->Ppdt.SfamData[7].LastByte  = FlashBase + FLASH_REGION_SINIT_ACM_OFFSET + FLASH_REGION_SINIT_ACM_SIZE - 1;
  PfatConfig->Ppdt.SfamData[8].FirstByte = FlashBase + FLASH_REGION_FV_SECPEI_NORMAL_OFFSET;
  PfatConfig->Ppdt.SfamData[8].LastByte  = FlashBase + FLASH_REGION_FV_SECPEI_NORMAL_OFFSET + FLASH_REGION_FV_SECPEI_NORMAL_SIZE - 1;  
  CopyMem (&PfatConfig->Ppdt.PkeySlot0[0], &PpdtPkeySlot0[0], sizeof (PpdtPkeySlot0));
  CopyMem (&PfatConfig->Ppdt.PkeySlot1[0], &PpdtPkeySlot1[0], sizeof (PpdtPkeySlot1));
  CopyMem (&PfatConfig->Ppdt.PkeySlot2[0], &PpdtPkeySlot2[0], sizeof (PpdtPkeySlot2));
  PfatConfig->PfatLog.LastPage       = MAX_PFAT_LOG_PAGE - 1;
  PfatConfig->PfatLog.LoggingOptions = PFAT_LOG_OPT_DEBUG | PFAT_LOG_OPT_FLASH_ERROR | PFAT_LOG_OPT_FLASH_ERASE | PFAT_LOG_OPT_FLASH_WRITE | PFAT_LOG_OPT_BRANCH_TRACE | PFAT_LOG_OPT_STEP_TRACE;
  if(AntiFlashWearoutEnabled) {
    PfatConfig->Ppdt.PlatAttr |= FLASH_WEAROUT_POLICY;
  }
#ifdef PFAT_EC_FLAG
  if (PlatformInfoHob->EcPresent == TRUE) {
    DEBUG ((EFI_D_INFO, "EC is Present\n"));
    PfatConfig->Ppdt.PlatAttr |= EC_PRESENT;
    PfatConfig->Ppdt.EcCmd     = KSC_C_PORT;
    PfatConfig->Ppdt.EcData    = KSC_D_PORT;
    PfatConfig->EcCmdDiscovery    = 0xB0;
    PfatConfig->EcCmdProvisionEav = 0xB1;
    PfatConfig->EcCmdLock         = 0xB2;
    CpuIo  = (**PeiServices).CpuIo;
    Status = (*PeiServices)->LocatePpi (PeiServices, &gPeiStallPpiGuid, 0, NULL, &StallPpi);
    ASSERT_PEI_ERROR (PeiServices, Status);
    Status = SendKscCommand (PeiServices, CpuIo, StallPpi, PfatConfig->EcCmdDiscovery);
    if (Status == EFI_SUCCESS) {
      Status = ReceiveKscData (PeiServices, CpuIo, StallPpi, &EcStatus);
      if (((EcStatus & 0xF8) == 0) && (EcStatus & BIT0)) {
        if (((EcStatus & (BIT2 | BIT1)) == (BIT2 | BIT1)) || (((EcStatus & (BIT2 | BIT1)) == 0))) {
          DEBUG ((EFI_D_INFO, "EC FW supports PFAT\n"));
          PfatConfig->Ppdt.PlatAttr     |= EC_PFAT_PROTECTED;
          PfatConfig->Ppdt.EcCmdGetSvn   = 0xB3;
          PfatConfig->Ppdt.EcCmdOpen     = 0xB4;
          PfatConfig->Ppdt.EcCmdClose    = 0xB5;
          PfatConfig->Ppdt.EcCmdPortTest = 0xB6;
          PfatConfig->PupHeader.EcSvn    = 0x00010000;
        } else {
          DEBUG ((EFI_D_ERROR, "EC FW Error\n"));
        }
      } else {
        DEBUG ((EFI_D_WARN, "EC FW do not support PFAT\n"));
      }
    } else {
      DEBUG ((EFI_D_ERROR, "SendKscCommand(EcCmdDiscovery) Failed\n"));
    }
  }
#endif //PFAT_EC_FLAG
  PfatConfig->Ppdt.PpdtSize = (sizeof (PPDT) - sizeof (PfatConfig->Ppdt.SfamData) + ((PfatConfig->Ppdt.LastSfam + 1) * sizeof (SFAM_DATA)));
  DataSize = Sha256GetContextSize ();
  Status   = ((*PeiServices)->AllocatePool) (PeiServices, DataSize, &Sha256Context);
  ASSERT_EFI_ERROR (Status);
  DataSize = PfatConfig->Ppdt.PpdtSize;
  Data     = (VOID *) &PfatConfig->Ppdt;
  Sha256Init   (Sha256Context);
  Sha256Update (Sha256Context, Data, DataSize);
  Sha256Final  (Sha256Context, HashValue);
  CopyMem (&PfatConfig->PpdtHash[0], &HashValue[0], 8);
  CopyMem (&PfatConfig->PpdtHash[1], &HashValue[8], 8);
  CopyMem (&PfatConfig->PpdtHash[2], &HashValue[16], 8);
  CopyMem (&PfatConfig->PpdtHash[3], &HashValue[24], 8);

  return;
}
#endif //AptioV server override: Pfat function implement for Grantley workstation.

#if 0
// ** Removing to make space
VOID
PrintPfatInfo(
  IN PFAT_CONFIG          *PfatConfig
  )
/*++

Routine Description:
  
  Prints all the PFAT related settings to the screen.

Arguments:

  PfatConfig           - Pointer to the PFat config structure.

Returns: 

  None

--*/
{
  UINT8 Index;
  //
  // PFAT_CONFIG - Print values
  //
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : Version : 0x%X\n", PfatConfig->PupHeader.Version));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : PlatId[16] :\n"));
  for (Index = 0; Index < 16; Index++) {
    if (Index == 15) {
      DEBUG ((EFI_D_INFO, " 0x%X\n", PfatConfig->Ppdt.PlatId[Index]));
    } else {
      DEBUG ((EFI_D_INFO, " 0x%X ,", PfatConfig->Ppdt.PlatId[Index]));
    }
  }
  DEBUG ((EFI_D_INFO, " \n"));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : PkgAttributes : 0x%X\n", PfatConfig->PupHeader.PkgAttributes));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : PslMajorVer : 0x%X\n", PfatConfig->PupHeader.PslMajorVer));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : PslMinorVer : 0x%X\n", PfatConfig->PupHeader.PslMinorVer));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : ScriptSectionSize : 0x%X\n", PfatConfig->PupHeader.ScriptSectionSize));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : DataSectionSize : 0x%X\n", PfatConfig->PupHeader.DataSectionSize));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : BiosSvn : 0x%X\n", PfatConfig->PupHeader.BiosSvn));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : EcSvn : 0x%X\n", PfatConfig->PupHeader.EcSvn));
  DEBUG ((EFI_D_INFO, " PFAT:: PUP_HEADER : VendorSpecific : 0x%X\n", PfatConfig->PupHeader.VendorSpecific));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PpdtSize : 0x%X\n", PfatConfig->Ppdt.PpdtSize));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PpdtMajVer : 0x%X\n", PfatConfig->Ppdt.PpdtMajVer));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PpdtMinVer : 0x%X\n", PfatConfig->Ppdt.PpdtMinVer));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PlatId[16] :\n"));
  for (Index = 0; Index < 16; Index++) {
    if (Index == 15) {
      DEBUG ((EFI_D_INFO, " 0x%X\n", PfatConfig->Ppdt.PlatId[Index]));
    } else {
      DEBUG ((EFI_D_INFO, " 0x%X ,", PfatConfig->Ppdt.PlatId[Index]));
    }
  }
  DEBUG ((EFI_D_INFO, " \n"));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PkeySlot0[32] :\n"));
  for (Index = 0; Index < 32; Index++) {
    if ((Index == 15) || (Index == 31)) {
      DEBUG ((EFI_D_INFO, " 0x%X\n", PfatConfig->Ppdt.PkeySlot0[Index]));
    } else {
      DEBUG ((EFI_D_INFO, " 0x%X ,", PfatConfig->Ppdt.PkeySlot0[Index]));
    }
  }
  DEBUG ((EFI_D_INFO, " \n"));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PkeySlot1[32] :\n"));
  for (Index = 0; Index < 32; Index++) {
    if ((Index == 15) || (Index == 31)) {
      DEBUG ((EFI_D_INFO, " 0x%X\n", PfatConfig->Ppdt.PkeySlot1[Index]));
    } else {
      DEBUG ((EFI_D_INFO, " 0x%X ,", PfatConfig->Ppdt.PkeySlot1[Index]));
    }
  }
  DEBUG ((EFI_D_INFO, " \n"));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PkeySlot2[32] :\n"));
  for (Index = 0; Index < 32; Index++) {
    if ((Index == 15)|| (Index == 31)) {
      DEBUG ((EFI_D_INFO, " 0x%X\n", PfatConfig->Ppdt.PkeySlot2[Index]));
    } else {
      DEBUG ((EFI_D_INFO, " 0x%X ,", PfatConfig->Ppdt.PkeySlot2[Index]));
    }
  }
  DEBUG ((EFI_D_INFO, " \n"));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PfatModSvn : 0x%X\n", PfatConfig->Ppdt.PfatModSvn));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : BiosSvn : 0x%X\n", PfatConfig->Ppdt.BiosSvn));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : ExecLim : 0x%X\n", PfatConfig->Ppdt.ExecLim));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : PlatAttr : 0x%X\n", PfatConfig->Ppdt.PlatAttr));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : EcCmd : 0x%X\n", PfatConfig->Ppdt.EcCmd));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : EcData : 0x%X\n", PfatConfig->Ppdt.EcData));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : EcCmdGetSvn : 0x%X\n", PfatConfig->Ppdt.EcCmdGetSvn));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : EcCmdOpen : 0x%X\n", PfatConfig->Ppdt.EcCmdOpen));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : EcCmdClose : 0x%X\n", PfatConfig->Ppdt.EcCmdClose));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : EcCmdPortTest : 0x%X\n", PfatConfig->Ppdt.EcCmdPortTest));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : LastSfam : 0x%X\n", PfatConfig->Ppdt.LastSfam));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : SfamData[64] :\n"));
  //
  // Change the array size according to MAX_SFAM_COUNT
  //
  for (Index = 0; Index < 64; Index++) {
    if ((Index == 15) || (Index == 31) || (Index == 47) || (Index == 63) ) {
      DEBUG ((EFI_D_INFO, " 0x%X\n", PfatConfig->Ppdt.SfamData[Index]));
    } else {
      DEBUG ((EFI_D_INFO, " 0x%X ,", PfatConfig->Ppdt.SfamData[Index]));
    }
  }
  DEBUG ((EFI_D_INFO, " \n"));
  DEBUG ((EFI_D_INFO, " PFAT:: PpdtHash[4] : 0x%lX , 0x%lX , 0x%lX , 0x%lX \n",   PfatConfig->PpdtHash[0], \
          PfatConfig->PpdtHash[1], \
          PfatConfig->PpdtHash[2], \
          PfatConfig->PpdtHash[3]));

  DEBUG ((EFI_D_INFO, " PFAT:: NumSpiComponents : 0x%x\n", PfatConfig->NumSpiComponents));
  DEBUG ((EFI_D_INFO, " PFAT:: PPDT : ComponentSize[8] :\n"));
  for (Index = 0; Index < PfatConfig->NumSpiComponents; Index++) {
    if (Index == 7) {
      DEBUG ((EFI_D_INFO, " 0x%X\n", PfatConfig->ComponentSize[Index]));
    } else {
      DEBUG ((EFI_D_INFO, " 0x%X ,", PfatConfig->ComponentSize[Index]));
    }
  }
  DEBUG ((EFI_D_INFO, " \n"));
  DEBUG ((EFI_D_INFO, " PFAT:: PfatMemSize : 0x%X\n", PfatConfig->PfatMemSize));
  DEBUG ((EFI_D_INFO, " PFAT:: EcCmdDiscovery : 0x%X\n", PfatConfig->EcCmdDiscovery));
  DEBUG ((EFI_D_INFO, " PFAT:: EcCmdProvisionEav : 0x%X\n", PfatConfig->EcCmdProvisionEav));
  DEBUG ((EFI_D_INFO, " PFAT:: EcCmdLock : 0x%X\n", PfatConfig->EcCmdLock));
  DEBUG ((EFI_D_INFO, " PFAT:: PFATLOG:: Version : 0x%X\n", PfatConfig->PfatLog.Version));
  DEBUG ((EFI_D_INFO, " PFAT:: PFATLOG:: LastPage : 0x%X\n", PfatConfig->PfatLog.LastPage));
  DEBUG ((EFI_D_INFO, " PFAT:: PFATLOG:: LoggingOptions : 0x%X\n", PfatConfig->PfatLog.LoggingOptions));
  DEBUG ((EFI_D_INFO, " PFAT:: PFATLOG:: PfatModSvn : 0x%X\n", PfatConfig->PfatLog.PfatModSvn));
  DEBUG ((EFI_D_INFO, " PFAT:: PFATLOG:: NumOfEntriesInLog : 0x%X\n", PfatConfig->PfatLog.NumOfEntriesInLog));

  return;
}
#endif

VOID
PfatInit (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr,
  IN struct sysHost             *host
  )
/*++

Routine Description:
  
  Perform the platform spefific PFAT initializations.

Arguments:

  **PeiServices         - Indirect reference to the PEI Services Table.
  PfatEnabled           - Boolean describing if Pfat is enabled or not
  *host                 - sysHost struct with system configuration information

Returns: 

  None

--*/
{
  EFI_STATUS          Status;
  UINT64              MsrValue;
  BOOLEAN             ResetRequired;
  PFAT_HOB            *PfatHobPtr;
  EFI_GUID            PfatHobGuid = PFAT_HOB_GUID;
  UINT8               i;
  PFAT_CONFIG         PfatConfig;
  PFAT_CONFIG         *PfatConfigPtr;
  EFI_PLATFORM_INFO   *PlatformInfoHob;
  EFI_HOB_GUID_TYPE   *GuidHob;
  BOOLEAN             SupportedPlatform = FALSE;
#ifdef PFAT_EC_FLAG
  volatile UINT32     EphemeralAuthValue;
  UINT8               EcStatus;
  PEI_STALL_PPI       *StallPpi;
  PEI_CPU_IO_PPI      *CpuIo;
#endif  //PFAT_EC_FLAG
  EFI_PEI_RESET_PPI   *ResetPpi;
  UINT64_STRUCT       MsrReg;
  

  DEBUG ((EFI_D_INFO, "PfatInit\n"));
  
  PfatConfigPtr = &PfatConfig;
  //
  // Locate Platform Info Hob
  //
  GuidHob          = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfoHob  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfoHob);

  ResetRequired = FALSE;
  MsrValue = AsmReadMsr64 (EFI_PLATFORM_INFORMATION);
  if (!(MsrValue & B_MSR_PLATFORM_INFO_PFAT_AVAIL)) {
    DEBUG ((EFI_D_INFO, "PFAT Feature is not supported\n"));
    goto  PfatInitExit;
  }
  
  //
  // Need to make sure that only supported Platforms can enable PFAT
  //  
  for (i=0; i < sizeof(PfatSupportedPlatforms); i++) {
    if (PlatformInfoHob->BoardId == PfatSupportedPlatforms[i]) {
      SupportedPlatform = TRUE;
      break;
    }
  }
  
  if (!SupportedPlatform) {
    TotalSystemConfigurationPtr->PfatEnabled = FALSE;
  }

  if (TotalSystemConfigurationPtr->PfatEnabled) {
    DEBUG ((EFI_D_INFO, "PFAT Module is Enabled\n"));
    
    InitializePfatConfigData(PfatConfigPtr);
	//AptioV server override start: Pfat function implement for Grantley workstation.
    //-CustomizePfatConfigData(PeiServices, PfatConfigPtr, TotalSystemConfigurationPtr->EnableAntiFlashWearout);
#if defined(BiosGuard_SUPPORT) && BiosGuard_SUPPORT == 1    
    AmiCustomizePfatConfigData(PeiServices, PfatConfigPtr);
#endif
    //AptioV server override end: Pfat function implement for Grantley workstation.
    //PrintPfatInfo(PfatConfigPtr);
    
    //
    // Save Pfat Mem Size in the Platform Info Hob
    //
    PlatformInfoHob->PfatMemSize  = PfatConfigPtr->PfatMemSize;
    
    //
    // Read PFAT Control Register
    //
    MsrValue = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (MsrValue & B_MSR_PLAT_FRMW_PROT_CTRL_LK) {
      if (!(MsrValue & B_MSR_PLAT_FRMW_PROT_CTRL_EN)) {
        //
        // Reset required as the PFAT CTRL MSR is locked and needs to be toggled
        //
        ResetRequired = TRUE;
      }
    } else {
#ifdef PFAT_EC_FLAG
      if ((PfatConfig->Ppdt.PlatAttr & EC_PRESENT) && (PfatConfig->Ppdt.PlatAttr & EC_PFAT_PROTECTED)) {
        DEBUG ((EFI_D_INFO, "EC is Present and EC FW supports PFAT\n"));
        CpuIo  = (**PeiServices).CpuIo;
        Status = (*PeiServices)->LocatePpi (PeiServices, &gPeiStallPpiGuid, 0, NULL, &StallPpi);
        ASSERT_PEI_ERROR (PeiServices, Status);
        Status = SendKscCommand (PeiServices, CpuIo, StallPpi, PfatConfigPtr->EcCmdProvisionEav);
        if (Status == EFI_SUCCESS) {
          EphemeralAuthValue = RandomNumber ();
          for (i = 0; (i < (sizeof (EphemeralAuthValue))); i++) {
            Status = SendKscData (PeiServices, CpuIo, StallPpi, (UINT8) ((EphemeralAuthValue >> (i * 8)) & 0xFF));
          }
          Status = ReceiveKscData (PeiServices, CpuIo, StallPpi, &EcStatus);
          if (EcStatus != 0) {
            ResetRequired = TRUE;
          }
          AsmWriteMsr32 (MSR_PLAT_FRMW_PROT_PASSWD, EphemeralAuthValue);
          EphemeralAuthValue = 0;
          Status = SendKscCommand (PeiServices, CpuIo, StallPpi, PfatConfigPtr->EcCmdLock);
        }
      }
#endif                  //PFAT_EC_FLAG
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_0, PfatConfigPtr->PpdtHash[0]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_1, PfatConfigPtr->PpdtHash[1]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_2, PfatConfigPtr->PpdtHash[2]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_3, PfatConfigPtr->PpdtHash[3]);
      MsrValue |= (B_MSR_PLAT_FRMW_PROT_CTRL_LK | B_MSR_PLAT_FRMW_PROT_CTRL_EN);
      
      //
      // Check to see if we need to set the Dual socket bit
      //
      if (host->var.common.numCpus > 1)
        MsrValue |= B_MSR_PLAT_FRMW_PROT_CTRL_S1;

      MsrReg.hi = 0;
      MsrReg.lo = (UINT32)MsrValue;
      
      for(i = 0; i < host->var.common.numCpus; i++) {
        WriteMsrPipe(host, i, MSR_PLAT_FRMW_PROT_CTRL, MsrReg);
      }
    }
    //
    // Create PFAT HOB
    //
    if (!ResetRequired) {
      Status = (*PeiServices)->CreateHob (PeiServices, EFI_HOB_TYPE_GUID_EXTENSION, sizeof (PFAT_HOB), (VOID **) &PfatHobPtr);
      PfatHobPtr->EfiHobGuidType.Name = PfatHobGuid;
      CopyMem (&PfatHobPtr->Ppdt, &PfatConfigPtr->Ppdt, PfatConfigPtr->Ppdt.PpdtSize);
      CopyMem (&PfatHobPtr->PupHeader, &PfatConfigPtr->PupHeader, sizeof (PUP_HEADER));
      CopyMem (&PfatHobPtr->PfatLog, &PfatConfigPtr->PfatLog, sizeof (PFAT_LOG));
      PfatHobPtr->NumSpiComponents = PfatConfigPtr->NumSpiComponents;
      for (i = 0; i < PfatConfigPtr->NumSpiComponents; i++) {
        PfatHobPtr->ComponentSize[i] = PfatConfigPtr->ComponentSize[i];
      }


    }
  } else {
    DEBUG ((EFI_D_INFO, "PFAT Module is Disabled\n"));
    MsrValue = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (MsrValue & B_MSR_PLAT_FRMW_PROT_CTRL_LK) {
      if (MsrValue & B_MSR_PLAT_FRMW_PROT_CTRL_EN) {
        //
        // Reset required as the PFAT CTRL MSR is locked and needs to be toggled
        //
        ResetRequired = TRUE;
      }
    } else {
      MsrValue &= ~B_MSR_PLAT_FRMW_PROT_CTRL_EN;
      MsrValue |= B_MSR_PLAT_FRMW_PROT_CTRL_LK;
      
      MsrReg.hi = 0;
      MsrReg.lo = (UINT32)MsrValue;     
      
      for(i = 0; i < host->var.common.numCpus; i++) {
        WriteMsrPipe(host, i, MSR_PLAT_FRMW_PROT_CTRL, MsrReg);
      }
    }
  }

  if (ResetRequired) {
    
    Status = (*PeiServices)->LocatePpi (
                    PeiServices, 
                    &gEfiPeiResetPpiGuid, 
                    0, 
                    NULL, 
                    &ResetPpi
                    );
    
    //
    // Perform Cold Reset
    //
    DEBUG ((EFI_D_INFO, "Reset Required. Performing Cold Reset to unlock PFAT CONTROL MSR\n"));
    ResetPpi->ResetSystem (PeiServices);
    //PchReset(PeiServices);
  }

PfatInitExit:
  RestoreAntiFlashWearout(PeiServices, TotalSystemConfigurationPtr, host);

  return;
}
