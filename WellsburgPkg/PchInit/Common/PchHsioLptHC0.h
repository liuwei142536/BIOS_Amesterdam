/** @file

  Header file with all LPTHC0 Hsio information

@copyright
  Copyright (c) 2013 Intel Corporation. All rights reserved
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
  license agreement.

**/


#ifdef TRAD_FLAG
#ifndef _PCH_HSIO_LPTHC0_H_
#define _PCH_HSIO_LPTHC0_H_

#define PCH_LPTH_HSIO_VER_C0  0x29

extern UINT8 PchChipsetInitTableLptH_C0[271];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioLptH_C0[36];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioLptH_C0[73];
extern IOBP_MMIO_TABLE_STRUCT PchUsb3SharedHsioLptH_C0[20];
extern IOBP_MMIO_TABLE_STRUCT PchUsb3HsioLptH_C0[36];
extern IOBP_MMIO_TABLE_STRUCT PchGbeSharedHsioLptH_C0[8];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioLptH_DT_C0[6];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioLptH_DT_C0[12];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioLptH_MB_C0[2];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioLptH_MB_C0[4];

#endif //_PCH_HSIO_LPTHC0_H_
#endif //TRAD_FLAG

