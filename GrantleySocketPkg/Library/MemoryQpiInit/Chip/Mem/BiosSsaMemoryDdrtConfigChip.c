/** @file
  This file contains the implementation of the BIOS version of the SSA services PPI.

  This version contains code for both client and server implementations.

  For Client:
    - Delete include of BiosSsaMemoryServerConfig.h and BiosSsaMemoryDdrtConfig.h.
    - Delete definitions of SSA_REVISION_MEMORY_SERVER and SSA_REVISION_MEMORY_DDRT.
    - Delete declarations of SsaMemoryServerConfig and SsaMemoryDdrtConfig.
    - Replace SsaMemoryServerConfig and SsaMemoryDdrtConfig field values in SsaBiosServicesPpi with NULL.

  For Server:
    - Delete include of BiosSsaMemoryClientConfig.h.
    - Delete definition of SSA_REVISION_MEMORY_CLIENT.
    - Delete declaration of SsaMemoryClientConfig.
    - Replace SsaMemoryClientConfig field value in SsaBiosServicesPpi with NULL.

@copyright
  Copyright (c) 2012 - 2014 Intel Corporation. All rights reserved.
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/

#include "SysFunc.h"

#ifdef SSA_FLAG

//
//This BSSA file contains subsets of all BSSA function calls made from the Core files; all CSR accesses are made here. This is NVMDIMM_IF Code and hence not available on Broadwell Server(BDX)
//

/**

  This routine sets the CSRs required by BiosSetEridValidationMask

  @param host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param EridMask                          - Early Read ID bitmask.  Bit value 1 = perform validation; bit value 0 = do not perform validation.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).

  @retval N/A

**/
VOID
BiosSetEridValidationMaskChip (
  PSYSHOST                              host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 EridMask
  )
{
//Not available on Broadwell Server(BDX); only for Skylake Server(SKX)
}

/**

  This routine gets the CSRs required by BiosGetEridErrorStatus

  @param host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Status                            - Pointer to where the Early Read ID bit error status will be stored.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).
  @param Count                             - Pointer to where the error counter value will be stored.

  @retval N/A

**/
VOID
BiosGetEridErrorStatusChip (
  PSYSHOST                              host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 *Status,
  UINT16                                *Count
  )
{
//Not available on Broadwell Server(BDX); only for Skylake Server(SKX)
}

/**

  This routine sets the CSRs required by BiosSetEridPattern

  @param host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Signal                            - Zero based ERID signal number.
  @param PatternMode                       - LFSR or pattern buffer mode.
  @param EnableReload                      - Enable or disable seed reload for the Early Read ID.
  @param EnableReset                       - Enable or disable seed reset for the Early Read ID.
  @param Seed                              - Seed value for Early Read ID.

  @retval N/A

**/
VOID
BiosSetEridPatternChip (
  PSYSHOST                              host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  ERID_SIGNAL                           Signal,
  ERID_PATTERN_MODE                     PatternMode,
  BOOLEAN                               EnableReload,
  BOOLEAN                               EnableReset,
  UINT32                                Seed
  )
{
//Not available on Broadwell Server(BDX); only for Skylake Server(SKX)
}

/**

  This routine sets the CSRs required by BiosSetEridTrainingConfig

  @param host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param EnableTraining                    - Specifies whether Early Read ID coarse training mode is enabled.

  @retval N/A

**/
VOID
BiosSetEridTrainingConfigChip (
  PSYSHOST                              host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               EnableTraining
  )
{
//Not available on Broadwell Server(BDX); only for Skylake Server(SKX)
}

/**

  This routine sets the CSRs required by BiosAepIoResetChip

  @param host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Dimm                              - Zero based DIMM number.

  @retval N/A

**/
VOID
BiosAepIoResetChip (
  PSYSHOST                              host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 Dimm
  )
{
// Not available on Broadwell Server(BDX); only for Skylake Server(SKX)
}

#else
// Future placeholder: BSSA code intentionally left out for now
#endif //SSA_FLAG
