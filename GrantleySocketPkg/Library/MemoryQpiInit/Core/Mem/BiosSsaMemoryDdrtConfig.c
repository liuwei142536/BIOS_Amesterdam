/** @file
  This file contains the BIOS implementation of the BIOS-SSA NVMDIMM Memory Configuration API.

@copyright
  Copyright (c) 2014 Intel Corporation. All rights reserved.
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

/**
  Function used to set the bit validation mask of Early Read ID.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[in]      EridMask   - Early Read ID bitmask.  Bit value 1 = perform validation; bit value 0 = do not perform validation.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetEridValidationMask) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 EridMask
  )
{
  UINT8                             channelInSocket;
  struct sysHost                    *host;

  host = (struct sysHost *) This->SsaMemoryConfig->MrcData;
#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host,"\nStart: BiosSetEridValidationMask (Socket:%d, Controller:%d, Channel:%d, Dimm:%d, EridMask:0x%x)\n", Socket, Controller, Channel, EridMask));
#endif
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosSetEridValidationMaskChip (host, Socket, channelInSocket, EridMask);

#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host, "End: BiosSetEridValidationMask \n"));
#endif
}

/**
  Function used to get the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[out]     Status     - Pointer to where the Early Read ID bit error status will be stored.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).
  @param[out]     Count      - Pointer to where the error counter value will be stored.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetEridErrorStatus) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 *Status,
  UINT16                *Count
  )
{
  UINT8                             channelInSocket;
  struct sysHost                    *host;

  host = (struct sysHost *) This->SsaMemoryConfig->MrcData;
#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host,"\nStart: BiosGetEridErrorStatus (Socket:%d, Controller:%d, Channel:%d, Dimm:%d)\n", Socket, Controller, Channel));
#endif
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosGetEridErrorStatusChip (host, Socket, channelInSocket, Status, Count);

#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host, "End: BiosGetEridErrorStatus Status:0x%x Count:%d\n", *Status, *Count));
#endif
}

/**
  Function used to clear the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.

  @retval Nothing.
**/
VOID
(EFIAPI BiosClearEridErrorStatus) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel
  )
{
  UINT8                             channelInSocket;
  struct sysHost                    *host;

  host = (struct sysHost *) This->SsaMemoryConfig->MrcData;
#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host,"\nStart: BiosClearEridErrorStatus (Socket:%d, Controller:%d, Channel:%d)\n", Socket, Controller, Channel));
#endif
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  ClearEridLfsrErr (host, Socket, channelInSocket);

#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host, "End: BiosClearEridErrorStatus \n"));
#endif
}

/**
  Function used to set up the Early Read ID pattern generator.

  @param[in, out] This          - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket        - Zero based CPU socket number.
  @param[in]      Controller    - Zero based controller number.
  @param[in]      Channel       - Zero based channel number.
  @param[in]      Signal        - ERID signal name.
  @param[in]      PatternMode   - ERID pattern mode.
  @param[in]      EnableReload  - Enable or disable seed reload for the Early Read ID.
  @param[in]      EnableReset   - Enable or disable seed reset for the Early Read ID.
  @param[in]      Seed          - Seed value for Early Read ID.

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetEridPattern) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  ERID_SIGNAL           Signal,
  ERID_PATTERN_MODE     PatternMode,
  BOOLEAN               EnableReload,
  BOOLEAN               EnableReset,
  UINT32                Seed
  )
{
  UINT8                                     channelInSocket;
  struct sysHost                            *host;

  host = (struct sysHost *) This->SsaMemoryConfig->MrcData;
#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host,"\nStart: BiosSetEridPattern (Socket:%d, Controller:%d, Channel:%d, Signal:%d PatternMode:%d EnableReload:%d EnableReset:%d Seed:0x%x)\n",
             Socket, Controller, Channel, Signal, PatternMode, EnableReload, EnableReset, Seed));
#endif
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosSetEridPatternChip (host, Socket, channelInSocket, Signal, PatternMode, EnableReload, EnableReset, Seed);

#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host, "End: BiosSetEridPattern \n"));
#endif
}

/**
  Function used to set the Early Read ID coarse training configuration.

  @param[in, out] This           - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket         - Zero based CPU socket number.
  @param[in]      Controller     - Zero based controller number.
  @param[in]      Channel        - Zero based channel number.
  @param[in]      EnableTraining - Specifies whether Early Read ID coarse training mode is enabled.

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetEridTrainingConfig) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  BOOLEAN               EnableTraining
  )
{
  UINT8                                     channelInSocket;
  struct sysHost                            *host;

  host = (struct sysHost *) This->SsaMemoryConfig->MrcData;
#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host,"\nStart: BiosSetEridTrainingConfig (Socket:%d, Controller:%d, Channel:%d, EnableTraining:%d)\n",
             Socket, Controller, Channel, EnableTraining));
#endif
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosSetEridTrainingConfigChip (host, Socket, channelInSocket, EnableTraining);

#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host, "End: BiosSetEridTrainingConfig \n"));
#endif
}

/**
  Function used to set the training mode of the controller chip of the NVMDIMM.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Controller       - Zero based controller number.
  @param[in]      Channel          - Zero based channel number.
  @param[in]      Dimm             - Zero based DIMM number.
  @param[in]      TrainingModeMask - 

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetAepDimmTrainingMode) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  UINT32                TrainingModeMask
  )
{
  UINT8                                     channelInSocket;
  struct sysHost                            *host;

  host = (struct sysHost *) This->SsaMemoryConfig->MrcData;
#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host,"\nStart: BiosSetAepDimmTrainingMode (Socket:%d, Controller:%d, Channel:%d, Dimm:%d, TrainingModeMask:0x%x)\n",
             Socket, Controller, Channel, Dimm, TrainingModeMask));
#endif
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  //BiosSetAepDimmTrainingModeChip (host, Socket, channelInSocket, Dimm, TrainingModeMask);

#ifdef ENBL_BIOS_SSA_DEBUG_MSGS
  rcPrintf ((host, "End: BiosSetAepDimmTrainingMode \n"));
#endif
}

#else
// Future placeholder: BSSA code intentionally left out for now
#endif //SSA_FLAG
// end file BiosSsaMemoryDdrtConfig.c
