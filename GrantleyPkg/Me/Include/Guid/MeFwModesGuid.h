//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999-2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MeFwModesGuid.h

Abstract:

  GUID for Me Fw Modes

--*/

#ifndef _GUID_ME_FW_MODES_GUID_H_
#define _GUID_ME_FW_MODES_GUID_H_

#include <SpsMisc.h>

//
// Definitions for ME FW IPMI Mode Selection Mode 
//
#define EFI_ME_FW_IPMI_MODE_SELECTION_GUID \
  { 0x4806b39c, 0x7a0b, 0x4b0b,  {0xb7, 0x2, 0x3c, 0xcd, 0x8, 0x40, 0xb6, 0x27 }}

typedef enum
{
  INVALID_MODE,
  SIEN_MODE,
  NM_MODE,
  DM_MODE,
  DNM_MODE
} ME_SKU;

typedef enum
{
  IPMI_KCS,
  IPMI_HECI,
  NONE
} IPMI_TRANSPORT_MODE;

typedef union
{
  UINT16   Data;
  struct
  {
    UINT16 NodeManager :1,
           PeciProxy   :1,
           ICC         :1,
           DCMI        :1,
           IDER        :1,  // IDE redirection
           SOL         :1,
           Network     :1,
           KVM         :1,
           PmBusProxy  :1,
           Reserved1   :1,
           MicProxy    :1,
           MctpProxy   :1,
           TRVA        :1;  // Thermal Reporting and Volumetric Airflow
  } Bits;
} SPS_FEATURE_SET;

typedef struct MeDiscoveryMode_iface_t
{
  ME_SKU              Mode;
  SPS_FEATURE_SET     FeatureSet;
  UINT8               WorkFlow;
  UINT8               RolDma;
  UINT64              TimeStampCounter[TIMESTAMP_TYPES];
} ME_FW_DISCOVERY_MODE;

typedef struct
{
  UINT8  MEClientAddress;
  UINT8  HostAddress;
  UINT8  MEClientStatus;
} DCMI_HOST_ME_INFO;

typedef struct
{
  UINT8   Data[16];
  UINT8   Size;
} DCMI_SETUP_INFO;

typedef struct
{
  DCMI_HOST_ME_INFO       DCMIInfo;
  ME_FW_DISCOVERY_MODE    DiscoveryMode;
  IPMI_TRANSPORT_MODE     TransportMode;
  DCMI_SETUP_INFO         GetDeviceID;
  DCMI_SETUP_INFO         GetSelfTestResults;
  ME_SKU                  MeSKU;
} IPMI_MODE_SELECTION;

extern EFI_GUID gEfiMeFwIpmiModeSelectionGuid;

#endif // _GUID_ME_FW_MODES_GUID_H_

