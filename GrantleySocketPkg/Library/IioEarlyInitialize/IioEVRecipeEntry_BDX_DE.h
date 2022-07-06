/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioEVRecipeEntry.h

Abstract:

  Houses all code related to the Uniphy programming 

Revision History:

--*/

#include "EvAutoRecipe.h"



EV_RECIPE_HEADER IioUniphyRecipeVer = { 
  0x5F56455F,
  "6.00",
  150,
  0,
  "Info : 1.0:498825.0"
};



EV_RECIPE_ENTRY IioUniphyRecipe[] = { 
    {TYPE_CSR, 4, 0x0030760, 0xffffff00, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x55},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_cdr_acq_length,
    {TYPE_CSR, 4, 0x0031760, 0xffff0000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaaaa},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_cdr_acq_length,
    {TYPE_CSR, 4, 0x0038760, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaaaaaaaa},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_cdr_acq_length,
    {TYPE_CSR, 4, 0x0030710, 0xfffff000, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x208},  //Set bndl0, bndl1 in pxpd06f0_pg_track,
    {TYPE_CSR, 4, 0x0031710, 0xff000000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x208208},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_pg_track,
    {TYPE_CSR, 4, 0x0038710, 0xc0000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x8208208},  //Set bndl0, bndl1, bndl2, bndl3, bndl4 in pxpd07f0_pg_track_0,
    {TYPE_CSR, 4, 0x0038714, 0xfffc0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x8208},  //Set bndl5, bndl6, bndl7 in pxpd07f0_pg_track_1,
    {TYPE_CSR, 4, 0x0030708, 0xfffff000, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x410},  //Set bndl0, bndl1 in pxpd06f0_pg_acq,
    {TYPE_CSR, 4, 0x0031708, 0xff000000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x410410},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_pg_acq,
    {TYPE_CSR, 4, 0x0038708, 0xc0000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10410410},  //Set bndl0, bndl1, bndl2, bndl3, bndl4 in pxpd07f0_pg_acq_0,
    {TYPE_CSR, 4, 0x003870c, 0xfffc0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10410},  //Set bndl5, bndl6, bndl7 in pxpd07f0_pg_acq_1,
    {TYPE_CSR, 4, 0x0030704, 0xffffffc0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x12},  //Set bndl0, bndl1 in pxpd06f0_ig_track,
    {TYPE_CSR, 4, 0x0031704, 0xfffff000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x492},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_ig_track,
    {TYPE_CSR, 4, 0x0038704, 0xff000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x492492},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_ig_track,
    {TYPE_CSR, 4, 0x0030700, 0xffffffc0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x24},  //Set bndl0, bndl1 in pxpd06f0_ig_acq,
    {TYPE_CSR, 4, 0x0031700, 0xfffff000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x924},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_ig_acq,
    {TYPE_CSR, 4, 0x0038700, 0xff000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x924924},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_ig_acq,
    {TYPE_CSR, 4, 0x0030730, 0xfffffff0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_cdr_ppm_offset_en,
    {TYPE_CSR, 4, 0x0031730, 0xffffff00, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_cdr_ppm_offset_en,
    {TYPE_CSR, 4, 0x0038730, 0xffff0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_cdr_ppm_offset_en,
    {TYPE_CSR, 4, 0x0030734, 0xffffc000, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set bndl0, bndl1 in pxpd06f0_cdr_ppm_offset,
    {TYPE_CSR, 4, 0x0031734, 0xf0000000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_cdr_ppm_offset,
    {TYPE_CSR, 4, 0x0038734, 0xf0000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd07f0_cdr_ppm_offset_0,
    {TYPE_CSR, 4, 0x0038738, 0xf0000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set bndl4, bndl5, bndl6, bndl7 in pxpd07f0_cdr_ppm_offset_1,
    {TYPE_CSR, 4, 0x0030a50, 0xfffffffc, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x3},  //Set bndl0, bndl1 in pxpd06f0_enbl_cdr_hang_det_l0,
    {TYPE_CSR, 4, 0x0031a50, 0xfffffff0, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xf},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_enbl_cdr_hang_det_l0,
    {TYPE_CSR, 4, 0x0038a50, 0xffffff00, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xff},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_enbl_cdr_hang_det_l0,
    {TYPE_CSR, 4, 0x0030a60, 0xfffffff0, WILDCARD_32, 0x1, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set bndl0, bndl1 in pxpd06f0_psa_cfg_gen2,
    {TYPE_CSR, 4, 0x0031a60, 0xffffff00, WILDCARD_32, 0x4, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_psa_cfg_gen2,
    {TYPE_CSR, 4, 0x0038a60, 0xffff0000, WILDCARD_32, 0x100, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_psa_cfg_gen2,
    {TYPE_CSR, 4, 0x0031a64, 0xffffff00, WILDCARD_32, 0x4, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaa},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_psa_cfg_gen3,
    {TYPE_CSR, 4, 0x0038a64, 0xffff0000, WILDCARD_32, 0x100, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaaaa},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_psa_cfg_gen3,
    {TYPE_CSR, 4, 0x0030788, 0xffffff00, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaa},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_agc_acq_length,
    {TYPE_CSR, 4, 0x0031788, 0xffff0000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaaaa},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_agc_acq_length,
    {TYPE_CSR, 4, 0x0038788, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaaaaaaaa},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_agc_acq_length,
    {TYPE_CSR, 4, 0x0030780, 0xfffffff0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xa},  //Set bndl0, bndl1 in pxpd06f0_agc_gain_acq,
    {TYPE_CSR, 4, 0x0031780, 0xffffff00, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaa},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_agc_gain_acq,
    {TYPE_CSR, 4, 0x0038780, 0xffff0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xaaaa},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_agc_gain_acq,
    {TYPE_CSR, 4, 0x0030790, 0xfffffff0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xf},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_rx_agc_use_err_data,
    {TYPE_CSR, 4, 0x0031790, 0xffffff00, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xff},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_rx_agc_use_err_data,
    {TYPE_CSR, 4, 0x0038790, 0xffff0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xffff},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_rx_agc_use_err_data,
    {TYPE_CSR, 4, 0x00306ec, 0xfff00000, WILDCARD_32, 0x1, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x42108},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_rx_vref_sel_notgen1,
    {TYPE_CSR, 4, 0x00316ec, 0xc0000000, WILDCARD_32, 0x4, 1 << PCIE_RATE_50GT | 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10842108},  //Set lane0, lane1, lane2, lane3, lane4, lane5 in pxpd06f1_rx_vref_sel_notgen1_0,
    {TYPE_CSR, 4, 0x00316f0, 0xfffffc00, WILDCARD_32, 0x4, 1 << PCIE_RATE_50GT | 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x108},  //Set lane6, lane7 in pxpd06f1_rx_vref_sel_notgen1_1,
    {TYPE_CSR, 4, 0x00386ec, 0xc0000000, WILDCARD_32, 0x100, 1 << PCIE_RATE_50GT | 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10842108},  //Set lane0, lane1, lane2, lane3, lane4, lane5 in pxpd07f0_rx_vref_sel_notgen1_0,
    {TYPE_CSR, 4, 0x00386f0, 0xc0000000, WILDCARD_32, 0x100, 1 << PCIE_RATE_50GT | 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10842108},  //Set lane6, lane7, lane8, lane9, lane10, lane11 in pxpd07f0_rx_vref_sel_notgen1_1,
    {TYPE_CSR, 4, 0x00386f4, 0xfff00000, WILDCARD_32, 0x100, 1 << PCIE_RATE_50GT | 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x42108},  //Set lane12, lane13, lane14, lane15 in pxpd07f0_rx_vref_sel_notgen1_2,
    {TYPE_CSR, 4, 0x00306e0, 0xfff00000, 0xe, 0x1, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5ad6b},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_rx_vref_sel_gen1,
    {TYPE_CSR, 4, 0x00316e0, 0xc0000000, WILDCARD_32, 0x4, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x16b5ad6b},  //Set lane0, lane1, lane2, lane3, lane4, lane5 in pxpd06f1_rx_vref_sel_gen1_0,
    {TYPE_CSR, 4, 0x00316e4, 0xfffffc00, WILDCARD_32, 0x4, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x16b},  //Set lane6, lane7 in pxpd06f1_rx_vref_sel_gen1_1,
    {TYPE_CSR, 4, 0x00386e0, 0xc0000000, WILDCARD_32, 0x100, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x16b5ad6b},  //Set lane0, lane1, lane2, lane3, lane4, lane5 in pxpd07f0_rx_vref_sel_gen1_0,
    {TYPE_CSR, 4, 0x00386e4, 0xc0000000, WILDCARD_32, 0x100, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x16b5ad6b},  //Set lane6, lane7, lane8, lane9, lane10, lane11 in pxpd07f0_rx_vref_sel_gen1_1,
    {TYPE_CSR, 4, 0x00386e8, 0xfff00000, WILDCARD_32, 0x100, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5ad6b},  //Set lane12, lane13, lane14, lane15 in pxpd07f0_rx_vref_sel_gen1_2,
    {TYPE_CSR, 4, 0x00307b0, 0xffffff00, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xff},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_dfe_acq_length,
    {TYPE_CSR, 4, 0x00317b0, 0xffff0000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xffff},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_dfe_acq_length,
    {TYPE_CSR, 4, 0x00387b0, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xffffffff},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_dfe_acq_length,
    {TYPE_CSR, 4, 0x0030798, 0xfffffff0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xf},  //Set bndl0, bndl1 in pxpd06f0_dfe_gain_acq,
    {TYPE_CSR, 4, 0x0031798, 0xffffff00, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xff},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_dfe_gain_acq,
    {TYPE_CSR, 4, 0x0038798, 0xffff0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xffff},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_dfe_gain_acq,
    {TYPE_CSR, 4, 0x0030794, 0xfffffff0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_dfe_force_acq,
    {TYPE_CSR, 4, 0x0031794, 0xffffff00, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_dfe_force_acq,
    {TYPE_CSR, 4, 0x0038794, 0xffff0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_dfe_force_acq,
    {TYPE_CSR, 4, 0x00307a0, 0xffff0000, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_dfe_open_loop_tap,
    {TYPE_CSR, 4, 0x00317a0, 0x0, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_dfe_open_loop_tap,
    {TYPE_CSR, 4, 0x00387a0, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd07f0_dfe_open_loop_tap_0,
    {TYPE_CSR, 4, 0x00387a4, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_dfe_open_loop_tap_1,
    {TYPE_CSR, 4, 0x00306c8, 0xfffffff0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5},  //Set bndl0, bndl1 in pxpd06f0_rx_vcm_ds_sel,
    {TYPE_CSR, 4, 0x00316c8, 0xffffff00, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x55},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_rx_vcm_ds_sel,
    {TYPE_CSR, 4, 0x00386c8, 0xffff0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5555},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_rx_vcm_ds_sel,
    {TYPE_CSR, 4, 0x00306cc, 0xfffffffc, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x3},  //Set bndl0, bndl1 in pxpd06f0_rx_vcm_sum_sel,
    {TYPE_CSR, 4, 0x00316cc, 0xfffffff0, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xf},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_rx_vcm_sum_sel,
    {TYPE_CSR, 4, 0x00386cc, 0xffffff00, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xff},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_rx_vcm_sum_sel,
    {TYPE_CSR, 4, 0x0037650, 0xffffe7ef, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set [4], [12:11] in cc_spare_0_6_7_cfg,
    {TYPE_CSR, 4, 0x00306ac, 0xfffffff0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xf},  //Set bndl0, bndl1 in pxpd06f0_rx_afe_offsetcor_gain,
    {TYPE_CSR, 4, 0x00316ac, 0xffffff00, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xff},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_rx_afe_offsetcor_gain,
    {TYPE_CSR, 4, 0x00386ac, 0xffff0000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xffff},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_rx_afe_offsetcor_gain,
    {TYPE_CSR, 4, 0x00306a0, 0xffffff00, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xff},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_rx_dfe_summer_ctl,
    {TYPE_CSR, 4, 0x00316a0, 0xffff0000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xffff},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_rx_dfe_summer_ctl,
    {TYPE_CSR, 4, 0x00386a0, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xffffffff},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_rx_dfe_summer_ctl,
    {TYPE_CSR, 4, 0x0031a38, 0x0, WILDCARD_32, 0x4, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x380038},  //Set bndl0, bndl1 in pxpd06f1_bndl_spare_0,
    {TYPE_CSR, 4, 0x0031a3c, 0x0, WILDCARD_32, 0x4, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x380038},  //Set bndl2, bndl3 in pxpd06f1_bndl_spare_1,
    {TYPE_CSR, 4, 0x0038a38, 0x0, WILDCARD_32, 0x100, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x380038},  //Set bndl0, bndl1 in pxpd07f0_bndl_spare_0,
    {TYPE_CSR, 4, 0x0038a3c, 0x0, WILDCARD_32, 0x100, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x380038},  //Set bndl2, bndl3 in pxpd07f0_bndl_spare_1,
    {TYPE_CSR, 4, 0x0038a40, 0x0, WILDCARD_32, 0x100, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x380038},  //Set bndl4, bndl5 in pxpd07f0_bndl_spare_2,
    {TYPE_CSR, 4, 0x0038a44, 0x0, WILDCARD_32, 0x100, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x380038},  //Set bndl6, bndl7 in pxpd07f0_bndl_spare_3,
    {TYPE_CSR, 4, 0x0031a38, 0x0, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x200020},  //Set bndl0, bndl1 in pxpd06f1_bndl_spare_0,
    {TYPE_CSR, 4, 0x0031a3c, 0x0, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x200020},  //Set bndl2, bndl3 in pxpd06f1_bndl_spare_1,
    {TYPE_CSR, 4, 0x0038a38, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x200020},  //Set bndl0, bndl1 in pxpd07f0_bndl_spare_0,
    {TYPE_CSR, 4, 0x0038a3c, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x200020},  //Set bndl2, bndl3 in pxpd07f0_bndl_spare_1,
    {TYPE_CSR, 4, 0x0038a40, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x200020},  //Set bndl4, bndl5 in pxpd07f0_bndl_spare_2,
    {TYPE_CSR, 4, 0x0038a44, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x200020},  //Set bndl6, bndl7 in pxpd07f0_bndl_spare_3,
    {TYPE_CSR, 4, 0x0030a88, 0xffffff00, 0xe, 0x1, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x55},  //Set bndl0, bndl1 in pxpd06f0_rx_ctle_rdeg_gen1,
    {TYPE_CSR, 4, 0x0031a88, 0xffff0000, WILDCARD_32, 0x4, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5555},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_rx_ctle_rdeg_gen1,
    {TYPE_CSR, 4, 0x0038a88, 0x0, WILDCARD_32, 0x100, 1 << PCIE_RATE_25GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x55555555},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_rx_ctle_rdeg_gen1,
    {TYPE_CSR, 4, 0x0030a8c, 0xffffff00, WILDCARD_32, 0x1, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x55},  //Set bndl0, bndl1 in pxpd06f0_rx_ctle_rdeg_gen2,
    {TYPE_CSR, 4, 0x0031a8c, 0xffff0000, WILDCARD_32, 0x4, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5555},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_rx_ctle_rdeg_gen2,
    {TYPE_CSR, 4, 0x0038a8c, 0x0, WILDCARD_32, 0x100, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x55555555},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_rx_ctle_rdeg_gen2,
    {TYPE_CSR, 4, 0x0031a90, 0xffff0000, WILDCARD_32, 0x4, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xbbbb},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_rx_ctle_rdeg_gen3,
    {TYPE_CSR, 4, 0x0038a90, 0x0, WILDCARD_32, 0x100, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xbbbbbbbb},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_rx_ctle_rdeg_gen3,
    {TYPE_CSR, 4, 0x0030840, 0xfffffc00, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x1ef},  //Set bndl0, bndl1 in pxpd06f0_tx_vref_sel,
    {TYPE_CSR, 4, 0x0031840, 0xfff00000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5ad6b},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_tx_vref_sel,
    {TYPE_CSR, 4, 0x0038840, 0xc0000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x16b5ad6b},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5 in pxpd07f0_tx_vref_sel_0,
    {TYPE_CSR, 4, 0x0038844, 0xfffffc00, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x16b},  //Set bndl6, bndl7 in pxpd07f0_tx_vref_sel_1,
    {TYPE_CSR, 4, 0x0030838, 0xfffffc00, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x16b},  //Set bndl0, bndl1 in pxpd06f0_tx_iref_ctl,
    {TYPE_CSR, 4, 0x0031838, 0xfff00000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x9ce73},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_tx_iref_ctl,
    {TYPE_CSR, 4, 0x0038838, 0xc0000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x2739ce73},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5 in pxpd07f0_tx_iref_ctl_0,
    {TYPE_CSR, 4, 0x003883c, 0xfffffc00, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x273},  //Set bndl6, bndl7 in pxpd07f0_tx_iref_ctl_1,
    {TYPE_CSR, 4, 0x0037644, 0xffc07eff, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x238100},  //Set icomp_track_b, blnclegs_ctl_b, blnclegs_dis_b in cc_misc_ctl_1_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037648, 0xe3ffffff, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x14000000},  //Set txdcc_gain_sel in cc_misc_ctl_2_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037628, 0xffffff0f, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10},  //Set tx_icomp_gain_sel in cc_icomp_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037638, 0xfffffffc, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x2},  //Set tx_rxdet_ctl in cc_tx_ctl_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037614, 0xff1fffff, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set pi_tst in cc_dfx_misc_0_6_7_cfg,
    {TYPE_CSR, 4, 0x003760c, 0xfffff83f, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set tx_vref_rxdetect in cc_bias_tx_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037608, 0xfff3ffff, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set pi_mixdcsc in cc_bias_rx_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037608, 0xc0ffffff, WILDCARD_32, 0x80000000, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x5000000},  //Set rx_incm_sel in cc_bias_rx_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037608, 0xc0ffffff, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x4000000},  //Set rx_incm_sel in cc_bias_rx_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037608, 0xffffffcf, WILDCARD_32, 0x80000000, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x30},  //Set rx_ibias_scale in cc_bias_rx_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037608, 0xffffffcf, WILDCARD_32, 0x80000000, WILDCARD_32, Vx_REV, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10},  //Set rx_ibias_scale in cc_bias_rx_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0037634, 0xff00ffff, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x20000},  //Set rx_ctle_ctl in cc_rx_ctl_0_6_7_cfg,
    {TYPE_CSR, 1, 0x0037654, 0xfe, WILDCARD_32, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x1},  //Set csr_override in uniphy_fuse_ctrl_0_6_7_cfg,
    {TYPE_CSR, 4, 0x0030300, 0xfcffefff, WILDCARD_32, 0xf0d, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x1000000},  //Set rx_dfelsb_sel, rx_invert_phasedet_slope in pxpd06f0_port_ctl,
    {TYPE_CSR, 4, 0x0030300, 0x3fffffff, WILDCARD_32, 0xf0d, 1 << PCIE_RATE_50GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x80000000L},  //Set rx_dfe_c1_ctl_gen2 in pxpd06f0_port_ctl,
    {TYPE_CSR, 4, 0x0030300, 0xffcfffff, WILDCARD_32, 0xf0d, WILDCARD_32,  ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x300000},  //Set rx_dfesum_mfc_10gen in pxpd06f0_port_ctl,
    {TYPE_CSR, 4, 0x0030300, 0xffcfffff, WILDCARD_32, 0xf0d, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x200000},  //Set rx_dfesum_mfc_10gen in pxpd06f0_port_ctl,
    {TYPE_CSR, 4, 0x00306b0, 0xffff0000, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3 in pxpd06f0_rx_spare,
    {TYPE_CSR, 4, 0x00316b0, 0x0, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd06f1_rx_spare,
    {TYPE_CSR, 4, 0x00386b0, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane0, lane1, lane2, lane3, lane4, lane5, lane6, lane7 in pxpd07f0_rx_spare_0,
    {TYPE_CSR, 4, 0x00386b4, 0x0, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set lane8, lane9, lane10, lane11, lane12, lane13, lane14, lane15 in pxpd07f0_rx_spare_1,
    {TYPE_CSR, 2, 0x00825c, 0x8080, WILDCARD_32, 0xf0c, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x2777},  //Set dnrxpreset, dntxpreset, uprxpreset, uptxpreset in pxpd01f0_ln0eq,
    {TYPE_CSR, 4, 0x00307c0, 0xffffffc0, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x36},  //Set bndl0, bndl1 in pxpd06f0_wsel,
    {TYPE_CSR, 4, 0x00317c0, 0xfffff000, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xdb6},  //Set bndl0, bndl1, bndl2, bndl3 in pxpd06f1_wsel,
    {TYPE_CSR, 4, 0x00387c0, 0xff000000, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0xdb6db6},  //Set bndl0, bndl1, bndl2, bndl3, bndl4, bndl5, bndl6, bndl7 in pxpd07f0_wsel,
    {TYPE_CSR, 4, 0x0030480, 0xf7f83dff, WILDCARD_32, 0x105, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x8044000},  //Set rxsqexitw4alllanes, spare0, lnknumtightcheck in pxpd06f0_clsphyctl8,
    {TYPE_CSR, 4, 0x0030464, 0xffffe07f, WILDCARD_32, 0x105, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x1f80},  //Set speedchangetime_constin4us in pxpd06f0_clsphyctl1,
    {TYPE_CSR, 4, 0x0031464, 0xfff81fff, WILDCARD_32, 0x104, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x3e000},  //Set speedchangetime_gen3in1us in pxpd06f1_clsphyctl1,
    {TYPE_CSR, 2, 0x0030490, 0xe000, WILDCARD_32, 0xf0d, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x4},  //Set sqcnt in pxpd06f0_pxpsqcnt,
    {TYPE_CSR, 4, 0x00304bc, 0x7f00f, WILDCARD_32, 0x105, WILDCARD_32, ((UINT64)1 << T0_STEP) |  ((UINT64)1 << U0_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x9400100},  //Set l1exiteito, spare_signals, recexiteito in pxpd06f0_debugcluster1,
    {TYPE_CSR, 4, 0x00304bc, 0xF807f00f, WILDCARD_32, 0x105, WILDCARD_32, ((UINT64)1 << V0_STEP) | ((UINT64)1 << V2_STEP), BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x9400100},  //Set l1exiteito, spare_signals, recexiteito in pxpd06f0_debugcluster1, NOTE: manual override to preserve the DLW SLD fix in IIO WA BIOS code
    {TYPE_CSR, 4, 0x00304bc, 0xF807f00f, WILDCARD_32, 0x105, WILDCARD_32, (UINT64)1 << Y0_STEP, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x9400100},  //Set l1exiteito, spare_signals, recexiteito in pxpd06f0_debugcluster1,
    {TYPE_CSR, 4, 0x0030b04, 0xa7ffffff, WILDCARD_32, 0x105, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x48000000},  //Set cfg_lanewait_timer_setting, en_l0s_tx_txval_cntr in pxpd06f0_dfxchickenbit0,
    {TYPE_CSR, 4, 0x00303f4, 0xff0ff0ff, WILDCARD_32, 0x105, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x300c00},  //Set rxsqex_cntrval, txlanestagint in pxpd06f0_uniphypmctl,
    {TYPE_CSR, 4, 0x003048c, 0xfcffffff, WILDCARD_32, 0x105, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x2000000},  //Set kalign_mode in pxpd06f0_prtphyctl,
    {TYPE_CSR, 4, 0x0037640, 0x0, 0xe, 0x80000000, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set cc_spare_addl_bits in cc_spare_addl_0_6_7_cfg,
    {TYPE_CSR, 4, 0x00304c4, 0xffffc03f, WILDCARD_32, 0x1, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x80},  //Set unuseentrinrtrybuf in pxpd06f0_pxpretryctrl0,
    {TYPE_CSR, 4, 0x00314c4, 0xffffc03f, WILDCARD_32, 0x4, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x80},  //Set unuseentrinrtrybuf in pxpd06f1_pxpretryctrl1,
    {TYPE_CSR, 4, 0x00384c4, 0xffffc03f, WILDCARD_32, 0x100, WILDCARD_32, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x80},  //Set unuseentrinrtrybuf in pxpd07f0_pxpretryctrl0,
    {TYPE_CSR, 4, 0x003139c, 0xffbfe1e1, WILDCARD_32, 0xf0c, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x401000},  //Set bypass_txeqcoeff_rule4b, dis_txdcbal, dis_rxdcbal, en_july0_7spec, use_g3eqprivcsr4adaptsm, dis_eqph23, byp_adaptsm_but_capcoeff, bypass_g3eq, use_g3eqprivatecsrvalues in pxpd06f1_pxpprtgen3eqpriv,
    {TYPE_CSR, 4, 0x00313f0, 0xffffff7f, WILDCARD_32, 0x104, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x0},  //Set dis_g12eqts1gen in pxpd06f1_txeqreutpriv,
    {TYPE_CSR, 4, 0x00313fc, 0xfffffc00, WILDCARD_32, 0x104, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x2},  //Set ph3_500nstimerincreasein_1us in pxpd06f1_txeqreutpriv1,
    {TYPE_CSR, 4, 0x00313cc, 0xfe000000, WILDCARD_32, 0xf0c, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x1000480},  //Set b0sub_pre_cursor, b1sub_pre_cursor, b0add_pre_cursor, b1add_pre_cursor, b0sub_post_cursor, b1sub_post_cursor, b0add_post_cursor, b1add_post_cursor, bypass_endcard_coeff in pxpd06f1_tx_ph3_cursor,
    {TYPE_CSR, 4, 0x0031438, 0xfffc0fff, WILDCARD_32, 0x104, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x20000},  //Set fsthreshold in pxpd06f1_fslfval_reg,
    {TYPE_CSR, 4, 0x0031b24, 0xffffff81, WILDCARD_32, 0x104, 1 << PCIE_RATE_80GT, WILDCARD_64, BDX_SOCKET, (1 << DE_SKU_TYPE), 00, 0x10},  //Set dis_mid_max_cal, en_byp_ph3_with_neq0, remote_tx_preset_dfx in pxpd06f1_dfxchickenbit1
};



