//
// This file contains 'Framework Code' and is licensed as such 
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.                 
//
/*++

Copyright (c)  1999-2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  Heci.h
    
Abstract:

  Heci PPI as defined in EFI 2.0

--*/
#ifndef _PEI_HECI_PPI_H_
#define _PEI_HECI_PPI_H_

#include "MeState.h"

//
// Ignore ME_FW_INIT_COMPLETE status Macro
//
#define ME_STATUS_IGNORE_FW_INIT_COMPLETE(a)  ((a) & (~ME_FW_INIT_COMPLETE))

//
// Macro to check if ME FW INIT is completed
//
#define ME_STATUS_IS_ME_FW_INIT_COMPLETE(a) (((a) & ME_FW_INIT_COMPLETE) == ME_FW_INIT_COMPLETE)

//
// Marco to combind the complete bit to status
//
#define ME_STATUS_WITH_ME_INIT_COMPLETE(a)  ((a) | ME_FW_INIT_COMPLETE)

//
// Abstract ME Status definitions
//   Bit0-3 ME Status
//   Bit6-4 Reserved
//   Bit7   ME FW Init Complete
//
#define ME_READY            0x00
#define ME_IN_RECOVERY_MODE 0x02
#define ME_NOT_READY        0x0F
#define ME_FW_INIT_COMPLETE 0x80

//
// Abstract ME Mode Definitions
//
#define ME_MODE_NORMAL        0x00
#define ME_MODE_ALT_DIS       0x02
#define ME_MODE_IGNITION      0x01
#define ME_MODE_TEMP_DISABLED 0x03
#define ME_MODE_SECOVER       0x04
#define ME_MODE_SV_FW         0x05
#define ME_MODE_FAILED        0x06

//
// PEI Timeout values
//
#define PEI_HECI_WAIT_DELAY    1000    // 1 ms
#define PEI_HECI_INIT_TIMEOUT  2000000 // 2 s
#define PEI_HECI_READ_TIMEOUT  500000  // 0.5 s
#define PEI_HECI_SEND_TIMEOUT  500000  // 0.5 s
#define PEI_HECI_RESET_TIMEOUT 2000000 // 2 sec

//
// Heci PPI definitions
//
#define PEI_HECI_PPI_GUID \
  { \
    0xEE0EA811, 0xFBD9, 0x4777, {0xB9, 0x5A, 0xBA, 0x4F, 0x71, 0x10, 0x1F, 0x74 }\
  }

typedef struct _PEI_HECI_PPI PEI_HECI_PPI;

typedef
EFI_STATUS
(EFIAPI *PEI_HECI_SENDWACK) (
  IN      PEI_HECI_PPI           * This,
  IN OUT  UINT32                 *Message,
  IN      UINT32                 HeciMemBar,
  IN OUT  UINT32                 *Length,
  IN      UINT8                  HostAddress,
  IN      UINT8                  MEAddress
  );

typedef
EFI_STATUS
(EFIAPI *PEI_HECI_READ_MESSAGE) (
  IN      PEI_HECI_PPI           *This,
  IN      UINT32                 Blocking,
  IN      UINT32                 HeciMemBar,
  IN      UINT32                 *MessageBody,
  IN OUT  UINT32                 *Length
  );

typedef
EFI_STATUS
(EFIAPI *PEI_HECI_SEND_MESSAGE) (
  IN      PEI_HECI_PPI           *This,
  IN      UINT32                 *Message,
  IN      UINT32                 HeciMemBar,
  IN      UINT32                 Length,
  IN      UINT8                  HostAddress,
  IN      UINT8                  MEAddress
  );

typedef
EFI_STATUS
(EFIAPI *PEI_HECI_INITIALIZE) (
  IN      PEI_HECI_PPI           *This,
  IN OUT  UINT32                 *HeciMemBar
  );

typedef
EFI_STATUS
(EFIAPI *PEI_HECI_GET_ME_STATUS) (
  IN      UINT32                 *Status
  );

typedef
EFI_STATUS
(EFIAPI *PEI_HECI_GET_ME_MODE) (
  IN      UINT32                 *MeMode
  );


typedef struct _PEI_HECI_PPI {
  PEI_HECI_SENDWACK       SendwAck;
  PEI_HECI_READ_MESSAGE   ReadMsg;
  PEI_HECI_SEND_MESSAGE   SendMsg;
  PEI_HECI_INITIALIZE     InitializeHeci;
  PEI_HECI_GET_ME_STATUS  GetMeStatus;
  PEI_HECI_GET_ME_MODE    GetMeMode;
} PEI_HECI_PPI;

extern EFI_GUID gPeiHeciPpiGuid;

//
// Heci2 PPI definitions
//
#define PEI_HECI2_PPI_GUID \
  { \
  0xE5EE2066, 0xFAA1, 0x4DFA, {0x92, 0x4E, 0xB1, 0xE3, 0xA8, 0xEE, 0x30, 0xE8}\
  }

typedef struct _PEI_HECI2_PPI PEI_HECI2_PPI;

typedef
EFI_STATUS
(EFIAPI *PEI_HECI2_GET_ME_STATUS) (
  IN      UINT32                 *Status
  );

typedef 
EFI_STATUS
(EFIAPI *PEI_HECI2_INITIALIZE) (
  IN      PEI_HECI2_PPI          *This,
  IN OUT  UINT32                 *HeciMemBar
  );

typedef 
EFI_STATUS
(EFIAPI *PEI_HECI2_STALL_PPI) (
  IN      UINTN                  Microseconds
  );

typedef struct _PEI_HECI2_PPI {
  PEI_HECI2_INITIALIZE    Heci2Initialize;
  PEI_HECI2_GET_ME_STATUS Heci2GetMeStatus;
} PEI_HECI2_PPI;

extern EFI_GUID gPeiHeci2PpiGuid;

#endif
