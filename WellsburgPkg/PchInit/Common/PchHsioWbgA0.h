/** @file
 
  Header file with all WbgA0 HSIO information
 
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

#ifdef PCH_SERVER_BIOS_FLAG
#ifdef TRAD_FLAG
#ifndef _PCH_HSIO_WBGA0_H_
#define _PCH_HSIO_WBGA0_H_

#define PCH_WBG_HSIO_VER_A0  35

extern UINT8 PchChipsetInitTableWbg_A0[761];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioWbg_A0[82];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioWbg_A0[40];
extern IOBP_MMIO_TABLE_STRUCT PchUsb3HsioWbg_A0[28];
extern IOBP_MMIO_TABLE_STRUCT PchUsb3SharedHsioWbg_A0[18];
extern IOBP_MMIO_TABLE_STRUCT PchDmiHsioWbg_A0[4];
extern IOBP_MMIO_TABLE_STRUCT PchSsataHsioWbg_A0[82];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioWbg_MB_A0[4];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioWbg_MB_A0[8];
extern IOBP_MMIO_TABLE_STRUCT PchSsataHsioWbg_MB_A0[8];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioWbg_DT_A0[4];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioWbg_DT_A0[8];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioWbg_DT_A0[4];
extern IOBP_MMIO_TABLE_STRUCT PchsSataHsioWbg_A0[82];
extern IOBP_MMIO_TABLE_STRUCT PchsSataSharedHsioWbg_A0[40];
extern IOBP_MMIO_TABLE_STRUCT PchsSataHsioWbg_DT_A0[8];
extern IOBP_MMIO_TABLE_STRUCT PchsSataSharedHsioWbg_DT_A0[4];
extern IOBP_MMIO_TABLE_STRUCT PchGbeSharedHsioWbg_A0[8];

#endif //_PCH_HSIO_WBGA0_H_
#endif //TRAD_FLAG
#endif // PCH_SERVER_BIOS_FLAG