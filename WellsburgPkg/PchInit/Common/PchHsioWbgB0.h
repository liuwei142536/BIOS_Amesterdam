/** @file

  Header file with all WBGB0 Hsio information

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
#ifndef _PCH_HSIO_WBGB0_H_
#define _PCH_HSIO_WBGB0_H_

#define PCH_WBG_HSIO_VER_B0  0x29

extern UINT8 PchChipsetInitTableWbg_B0[271];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioWbg_B0[36];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioWbg_B0[73];
extern IOBP_MMIO_TABLE_STRUCT PchUsb3SharedHsioWbg_B0[22];
extern IOBP_MMIO_TABLE_STRUCT PchUsb3HsioWbg_B0[40];
extern IOBP_MMIO_TABLE_STRUCT PchGbeSharedHsioWbg_B0[8];
extern IOBP_MMIO_TABLE_STRUCT PchSsataHsioWbg_B0[89];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioWbg_DT_B0[6];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioWbg_DT_B0[12];
extern IOBP_MMIO_TABLE_STRUCT PchSsataHsioWbg_DT_B0[12];
extern IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioWbg_MB_B0[6];
extern IOBP_MMIO_TABLE_STRUCT PchSataHsioWbg_MB_B0[12];
extern IOBP_MMIO_TABLE_STRUCT PchSsataHsioWbg_MB_B0[12];
extern IOBP_SATA_RXEQ_TABLE PchSataRxEqSharedHsioWbg_B0[6];
extern IOBP_SATA_RXEQ_TABLE PchSataRxEqHsioWbg_B0[12];
//
// The EVA's sSATA has exact the same structure as regular SATA
// Therefore the table can be the  IOBP_SATA_RXEQ_TABLE type insted of IOBP_sSATA_RXEQ_TABLE type
//
extern IOBP_SATA_RXEQ_TABLE PchsSataRxEqHsioWbg_B0[12];

#endif //_PCH_HSIO_WBGB0_H_
#endif //TRAD_FLAG

