/*++
@copyright
  Copyright (c) 2017 Intel Corporation. All rights reserved.
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
--*/

#ifndef _S3_SUPPORT_HOBS_H__
#define _S3_SUPPORT_HOBS_H__

#pragma pack(push, 1)

typedef struct _S3_SUPPORT_HOB {
  EFI_HOB_GUID_TYPE      Header;
  UINT32                 PchS3PeimEntryPoint;    // Entry Point of the PCH S3 PEIM module
} S3_SUPPORT_HOB;

typedef struct _S3_DATA_HOB {
  EFI_HOB_GUID_TYPE      Header;
  VOID                   *S3DispatchDataArray;    // Pointer to the EFI_PCH_S3_DISPATCH_ARRAY to be passed to DXE
} S3_DATA_HOB;

#pragma pack(pop)
#endif
