/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*************************************************************************
 *
 * Memory Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 *
 ************************************************************************/

#include "SysFunc.h"
#include <Library/OemMemoryQpiInit.h>
#include <PlatformHost.h>
//
// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)

/**

  OEM Hook to Log warning to the warning log

  @param host              - pointer to sysHost
  @param warningCode       - Major warning code
  @param minorWarningCode  - Minor warning code
  @param socket              - socket number
  @param ch                - channel number
  @param dimm              - dimm number
  @param rank              - rank number

**/
void
PlatformOutputWarning (
  PSYSHOST host,
  UINT8    warningCode,
  UINT8    minorWarningCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  //
  // OEM Specific Log the warning
  //
}

/**

  Oem hook to Log Error to the warning log

  @param host              - pointer to sysHost
  @param ErrorCode         - Major warning code
  @param minorErrorCode    - Minor warning code
  @param socket            - socket number
  @param ch                - channel number
  @param dimm              - dimm number
  @param rank              - rank number

**/
void
PlatformOutputError (
  PSYSHOST host,
  UINT8    ErrorCode,
  UINT8    minorErrorCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  //
  // OEM Specific Output the error
  //
}

UINT32
PlatformMemInitWarning (
  PSYSHOST host
  )
/*++

  Used to add warnings to the promote warning exception list

  @param host  - pointer to sysHost

  @retval VOID

--*/
{
  SetPromoteWarningException (host, WARN_USER_DIMM_DISABLE, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (host, WARN_MEM_LIMIT, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (host, WARN_CLTT_DISABLE, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (host, WARN_CLTT_DIMM_UNKNOWN, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (host, WARN_CLOSED_PAGE_OVERRIDE, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (host, WARN_DIMM_VREF_NOT_PRESENT, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (host, WARN_LOCKSTEP_DISABLE, WARN_LOCKSTEP_DISABLE_MINOR_RAS_MODE);
  SetPromoteWarningException (host, WARN_MIRROR_DISABLE, WARN_MIRROR_DISABLE_MINOR_RAS_DISABLED);
  SetPromoteWarningException (host, WARN_DIMM_COMPAT, WARN_CHANNEL_MIX_ECC_NONECC);
  SetPromoteWarningException (host, WARN_SPARE_DISABLE, 0);
  SetPromoteWarningException (host, WARN_MEMTEST_DIMM_DISABLE, WARN_FPT_MINOR_MEM_TEST);
  SetPromoteWarningException (host, WARN_COD_HA_NOT_ACTIVE, 0);
  SetPromoteWarningException (host, WARN_SENS_AMP_TRAINING, WARN_SENS_AMP_CH_FAILIURE);
  SetPromoteWarningException (host, WARN_SWIZZLE_DISCOVERY_TRAINING, WARN_SWIZZLE_PATTERN_MISMATCH);
  SetPromoteWarningException (host, WARN_PPR_FAILED, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (host, WARN_WRCRC_DISABLE, WARN_TRAIL_ODT_LIMIT_REACHED);
  return SUCCESS;
}

void
PlatformMemLogWarning (
  PSYSHOST host,
  UINT8    majorCode,
  UINT8    minorCode,
  UINT32   logData
  )
/*++

  Used for memory related warnings

  @param host      - pointer to sysHost
  @param majorCode - Major warning code
  @param minorCode - Minor warning code
  @param logData   - Data log

  @retval VOID

--*/
{
#ifdef SERIAL_DBG_MSG
  //
  // Output memory warning info
  //
  if (checkMsgLevel(host, SDBG_MINMAX)) {
    if (((logData >> 24) & 0xFF) != 0xFF) {
      rcPrintf ((host, "Socket = %d", (logData >> 24) & 0xFF));
    }

    if (((logData >> 16) & 0xFF) != 0xFF) {
      rcPrintf ((host, " Channel = %d", (logData >> 16) & 0xFF));
    }

    if (((logData >> 8) & 0xFF) != 0xFF) {
      rcPrintf ((host, " DIMM = %d", (logData >> 8) & 0xFF));
    }

    if ((logData & 0xFF) != 0xFF) {
      rcPrintf ((host, " Rank = %d", logData & 0xFF));
    }

    rcPrintf ((host, "\n\n"));
  }
#endif

  LogWarning (host, majorCode, minorCode, logData);
}

UINT32
PlatformMemReadDimmVref (
  PSYSHOST host,
  UINT8    socket,
  UINT8    *vref
  )
/*++

  PlatformMemReadDimmVref: OEM hook to read the current DIMM Vref setting from a given socket.
  If multiple Vref circuits are present per socket, this function assumes that
  they are set to the same value.

  @param host - Pointer to sysHost struct
  @param socket - processor number
  @param vref - pointer to UINT8 where the vref value is stored

  @retval SUCCESS if the Vref circuit is present and can be read
  @retval FAILURE if the Vref circuit is not present or cannot be read

--*/
{
  UINT32  status = FAILURE;
  UINT8   ch;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((host->setup.mem.socket[socket].ddrCh[ch].vrefDcp.compId != NO_DEV) &&
        (host->setup.mem.socket[socket].ddrCh[ch].vrefDcp.compId != NOT_INITIALIZED)){
      status = ReadSmb (host, socket, host->setup.mem.socket[socket].ddrCh[ch].vrefDcp , 0, vref );
      if (status == SUCCESS) break;
    }
  }

  return status;
}

UINT32
PlatformMemWriteDimmVref (
  PSYSHOST host,
  UINT8    socket,
  UINT8    vref
  )
/*++

  PlatformWriteDimmVref: OEM hook to write the DIMM Vref setting to a given socket.
  If multiple Vref circuits are present per socket, this function assumes that
  they are set to the same value.

  @param host - Pointer to sysHost struct
  @param socket - processor number
  @param vref - vref value to write

  @retval SUCCESS if the Vref circuit is present and can be written
  @retval FAILURE if the Vref circuit is not present or cannot be written

--*/
{
  UINT32  status;
  UINT8   ch;

  status  = FAILURE;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((host->setup.mem.socket[socket].ddrCh[ch].vrefDcp.compId != NO_DEV) &&
        (host->setup.mem.socket[socket].ddrCh[ch].vrefDcp.compId != NOT_INITIALIZED)){
      status = WriteSmb (host, socket, host->setup.mem.socket[socket].ddrCh[ch].vrefDcp , 0, &vref );
    }
  }

  return status;
}

void
PlatformInitializeData (
  PSYSHOST host
  )
/*++

  PlatformInitializeData - OEM hook to initialize Input structure data if required.

  @param host  - Pointer to sysHost

  @retval VOID

--*/
{
}

UINT8
PlatformSetVdd (
  PSYSHOST host,
  UINT8    socket
  )
/*++

  PlatformSetVdd - OEM hook to change the socket Vddq

  @param host  - Pointer to sysHost
  @param socket  - processor number

  @retval SUCCESS if the Vddq change was made
  @retval FAILURE if the Vddq change was not made

--*/
{
  UINT8 status = FAILURE;

  return status;
}

void
PlatformReleaseADRClamps (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  OEM hook to release any platform clamps affecting CKE
  and/or DDR Reset

  @param host      - pointer to sysHost structure

  @retval N/A

--*/
{
    /*
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT  mcInitStateG;

  // Set the MCU_INIT signal to release the clamps

  mcInitStateG.Data = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);
  // Clear ddr_reset to assert reset
  mcInitStateG.Bits.ddr_reset   = 0;
  mcInitStateG.Bits.reset_io    = 0;
  MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);

  FixedDelay (host, 200);

  mcInitStateG.Bits.ddr_reset = 1;
  MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
  */
  if (IsDESku(host)) {
    OutPort32(host,host->var.common.gpioBase+0x38,(InPort32(host,host->var.common.gpioBase+0x38) | (1 << 11))); // GPIO43 on TrabucoCanyon platform
  } else {
    OutPort32(host,host->var.common.gpioBase+0x38,(InPort32(host,host->var.common.gpioBase+0x38) | (1 << 11))); // GPIO43 on EmeraldPoint platform
  }
} // PlatformReleaseADRClamps

UINT32
PlatformDetectADR (
  PSYSHOST host
  )
/*++

  OEM hook to allow a secondary ADR dection method other than
  PCH_PM_STS

  @param host      - pointer to sysHost structure

  @retval 0         - Hook not implemented or no ADR recovery
  @retval 1         - Hook implemented and platform should recover from ADR

--*/
{
  UINT32 AdrStatus = 0;
  if (IsDESku(host)) {
    AdrStatus = InPort32(host,host->var.common.gpioBase + 0x38) & (1 << 4); //ADR_STATUS_RD GPIO36 on TrabucoCanyon platform
  } else {
    AdrStatus = InPort32(host,host->var.common.gpioBase + 0x38) & (1 << 9); //ADR_STATUS GPIO41 on EmeraldPoint platform
  }
  if (AdrStatus) 
    return 1; 
  else 
    return 0;
}

void 
PlatformClearADR (
  PSYSHOST host
  )
/*++

  OEM hook to allow a clearing ADR status on the platform.

  @param host      - pointer to sysHost structure

  @retval None

--*/
{
#ifdef GRANGEVILLE_FLAG
  switch ( host->nvram.common.platformType ) {
   case TypePiestewaPeak:
     OutPort32(host, host->var.common.gpioBase + 0x38,(InPort32(host, host->var.common.gpioBase + 0x38) | (1 << 5))); //ADR_STATUS_CLR GPIO37 on TrabucoCanyon platform
     break;
   default:
     break;
   }
#endif
}


void
PlatformCheckPORCompat (
  PSYSHOST host
  )
/*++

  PlatformCheckPORCompat - OEM hook to restrict supported configurations if required.

  @param host  - Pointer to sysHost

  @retval VOID

--*/
{
}

UINT32
OemInitThrottlingEarly (
  PSYSHOST host
  )
/*++

  OEM hook immediately before initialization of memory throttling early in the MRC

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemDetectDIMMConfig (
  PSYSHOST host
  )
/*++

  OEM hook immediately before DIMM detection

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemInitDdrClocks (
  PSYSHOST host
  )
/*++

  OEM hook immediately before DIMM detection

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemSetDDRFreq (
  PSYSHOST host
  )
/*++

  OEM hook immediately before DIMM detection

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemConfigureXMP (
  PSYSHOST host
  )
/*++

  OEM hook immediately before DIMM detection

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemCheckVdd (
  PSYSHOST host
  )
/*++

  OEM hook immediately before DIMM detection

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemEarlyConfig (
  PSYSHOST host
  )
/*++

  OEM hook immediately before DIMM detection

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemLateConfig (
  PSYSHOST host
  )
/*++

  OEM hook immediately before DIMM detection

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

UINT32
OemInitThrottling (
  PSYSHOST host
  )
/*++

  OEM hook immediately before initialization of memory throttling

  @param host  - Point to sysHost

  @retval UINT32

--*/
{
  return SUCCESS;
}

void
PlatformHookMst (
  PSYSHOST host,
  UINT8    socket,
  struct   smbDevice dev,
  UINT8    byteOffset,
  UINT8    *data
  )
/*++

  OEM hook for initializing Tcrit to 105 temp offset & the config register
  which is inside of initialization of memory throttling

  @param host              - pointer to sysHost
  @param socket            - socket number
  @param smbDevice dev     - SMB Device
  @param byteOffset        - byte Offset
  @param data              - data

  @retval N/A

--*/
{
  OemPlatformHookMst(host, socket, dev, byteOffset, (UINT8 *)&data);

  return;
}

void
OemLookupDdr4OdtTable (
  PSYSHOST host,
  struct ddr4OdtValueStruct **ddr4OdtValueStructPtr,
  UINT16 *ddr4OdtValueStructSize
  )
/*++

  OEM hook for overriding the DDR4 ODT Value table

  @param ddr4OdtValueStructPtr   - Pointer to Intel DDR4 ODT Value array
  @param ddr4OdtValueStructSize  - Pointer to size of Intel DDR4 ODT Value array in number of bytes


  @retval ddr4OdtValueStructPtr   - Pointer to DDR4 ODT Value array to use
  @retval ddr4OdtValueStructSize  - Pointer to size of DDR4 ODT Value array in number of bytes to use

--*/
{
/*
  *ddr4OdtValueStructPtr = &ddr4OdtValueTableOem[0];
  *ddr4OdtValueStructSize = sizeof(ddr4OdtValueTableOem);
*/
}

void
OemLookupFreqTable (
  PSYSHOST host,
  struct DimmPOREntryStruct **freqTablePtr,
  UINT16 *freqTableLength
  )
/*++

  OEM hook for overriding the memory POR frequency table

  @param host            - Pointer to SysHost
  @param freqTablePtr    - Pointer to Intel POR memory frequency table
  @param freqTableLength - Pointer to number of DimmPOREntryStruct entries in POR Frequency Table


  @retval N/A

--*/
{

/*
  *freqTablePtr = &OemfreqTable[0];
  *freqTableLength = sizeof(freqTableOem) / sizeof(DimmPOREntryStruct);
*/
}
 /*-----------------        Last Line of MemHooks.c         -----------------*/
