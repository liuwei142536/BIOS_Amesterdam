/** @file
  Implementation of EMCA Platform hooks


  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

  Copyright (c) 2009-2014 Intel Corporation.  All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license.  Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.


**/


#include <Library/emcaplatformhookslib.h>
#include "emcaplatformhookslibprivate.h"

#define DBG_MSGS 1
#if DBG_MSGS
VOID
EFIAPI
InternalPrintMessage1 (
  IN  CONST CHAR8  *Format,
  ...
  );
#endif

EMCA_MCBANK_DESC *gMcBankList = NULL;

EMCA_MCBANK_DESC gMcBankListIvtEx[] = { 
  { MSR_MC00_CTL, MSR_MC00_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_IFU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC01_CTL, MSR_MC01_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC02_CTL, MSR_MC02_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DTLB0     , ReadMcBankSignature, {NULL}},
  { MSR_MC03_CTL, MSR_MC03_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_MLC0      , ReadMcBankSignature, {NULL}},
  { MSR_MC04_CTL, MSR_MC04_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_PCU0_QPI2 , ReadMcBankSignature, {NULL}},
  { MSR_MC05_CTL, MSR_MC05_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI0_QPI1 , ReadMcBankSignature, {NULL}},
  { MSR_MC06_CTL, MSR_MC06_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_IIO0      , ReadMcBankSignature, {NULL}},
  { MSR_MC07_CTL, MSR_MC07_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA0       , ReadHaMcBankSignature,{NULL}},
  { MSR_MC08_CTL, MSR_MC08_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA1       , ReadHaMcBankSignature,{NULL}},
  { MSR_MC09_CTL, MSR_MC09_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC0      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC10_CTL, MSR_MC10_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC1      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC11_CTL, MSR_MC11_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC2      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC12_CTL, MSR_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC3      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC13_CTL, MSR_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC4      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC14_CTL, MSR_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC5      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC15_CTL, MSR_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC6      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC16_CTL, MSR_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC7      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC17_CTL, MSR_MC17_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO00     , ReadMcBankSignature, {NULL}},
  { MSR_MC18_CTL, MSR_MC18_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO01     , ReadMcBankSignature, {NULL}},
  { MSR_MC19_CTL, MSR_MC19_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO02     , ReadMcBankSignature, {NULL}},
  { MSR_MC20_CTL, MSR_MC20_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO03     , ReadMcBankSignature, {NULL}},
  { MSR_MC21_CTL, MSR_MC21_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO04     , ReadMcBankSignature, {NULL}},
  { MSR_MC22_CTL, MSR_MC22_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO05     , ReadMcBankSignature, {NULL}},
  { MSR_MC23_CTL, MSR_MC23_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO06     , ReadMcBankSignature, {NULL}},
  { MSR_MC24_CTL, MSR_MC24_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO07     , ReadMcBankSignature, {NULL}},
  { MSR_MC25_CTL, MSR_MC25_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO08     , ReadMcBankSignature, {NULL}},
  { MSR_MC26_CTL, MSR_MC26_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO09     , ReadMcBankSignature, {NULL}},
  { MSR_MC27_CTL, MSR_MC27_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO10     , ReadMcBankSignature, {NULL}},
  { MSR_MC28_CTL, MSR_MC28_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO11     , ReadMcBankSignature, {NULL}},
  { MSR_MC29_CTL, MSR_MC29_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO12     , ReadMcBankSignature, {NULL}},
  { MSR_MC30_CTL, MSR_MC30_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO13     , ReadMcBankSignature, {NULL}},
  { MSR_MC31_CTL, MSR_MC31_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO14     , ReadMcBankSignature, {NULL}}
};

EMCA_MCBANK_DESC gMcBankListHswHCC[] = { 
  { MSR_MC00_CTL, MSR_MC00_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_IFU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC01_CTL, MSR_MC01_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC02_CTL, MSR_MC02_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DTLB0     , ReadMcBankSignature, {NULL}},
  { MSR_MC03_CTL, MSR_MC03_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_MLC0      , ReadMcBankSignature, {NULL}},
  { MSR_MC04_CTL, MSR_MC04_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_PCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC05_CTL, MSR_MC05_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI0      , ReadMcBankSignature, {NULL}},
  { MSR_MC06_CTL, MSR_MC06_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_IIO0      , ReadMcBankSignature, {NULL}},
  { MSR_MC07_CTL, MSR_MC07_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA0       , ReadHaMcBankSignature, {NULL}},
  { MSR_MC08_CTL, MSR_MC08_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA1       , ReadHaMcBankSignature, {NULL}},
  { MSR_MC09_CTL, MSR_MC09_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC0      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC10_CTL, MSR_MC10_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC1      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC11_CTL, MSR_MC11_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC2      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC12_CTL, MSR_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC3      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC13_CTL, MSR_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC4      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC14_CTL, MSR_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC5      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC15_CTL, MSR_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC6      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC16_CTL, MSR_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC7      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC17_CTL, MSR_MC17_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO00     , ReadMcBankSignature, {NULL}},
  { MSR_MC18_CTL, MSR_MC18_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO01     , ReadMcBankSignature, {NULL}},
  { MSR_MC19_CTL, MSR_MC19_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO02     , ReadMcBankSignature, {NULL}},
  { MSR_MC20_CTL, MSR_MC20_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI1      , ReadMcBankSignature, {NULL}},
  { MSR_MC21_CTL, MSR_MC21_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI2      , ReadMcBankSignature, {NULL}}
};

EMCA_MCBANK_DESC gMcBankListHswMCC[] = { 
  { MSR_MC00_CTL, MSR_MC00_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_IFU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC01_CTL, MSR_MC01_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC02_CTL, MSR_MC02_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DTLB0     , ReadMcBankSignature, {NULL}},
  { MSR_MC03_CTL, MSR_MC03_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_MLC0      , ReadMcBankSignature, {NULL}},
  { MSR_MC04_CTL, MSR_MC04_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_PCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC05_CTL, MSR_MC05_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI0      , ReadMcBankSignature, {NULL}},
  { MSR_MC06_CTL, MSR_MC06_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_IIO0      , ReadMcBankSignature, {NULL}},
  { MSR_MC07_CTL, MSR_MC07_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA0       , ReadHaMcBankSignature, {NULL}},
  { MSR_MC08_CTL, MSR_MC08_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA1       , ReadHaMcBankSignature, {NULL}},
  { MSR_MC09_CTL, MSR_MC09_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC0      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC10_CTL, MSR_MC10_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC1      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC11_CTL, MSR_MC11_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC2      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC12_CTL, MSR_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC3      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC13_CTL, MSR_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC4      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC14_CTL, MSR_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC5      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC15_CTL, MSR_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC6      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC16_CTL, MSR_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC7      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC17_CTL, MSR_MC17_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO00     , ReadMcBankSignature, {NULL}},
  { MSR_MC18_CTL, MSR_MC18_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO01     , ReadMcBankSignature, {NULL}},
  { MSR_MC19_CTL, MSR_MC19_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO02     , ReadMcBankSignature, {NULL}},
  { MSR_MC20_CTL, MSR_MC20_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI1      , ReadMcBankSignature, {NULL}},
  { MSR_MC21_CTL, MSR_MC21_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     0                  , ReadNullSignature, {NULL}}
};

EMCA_MCBANK_DESC gMcBankListHswLCC[] = { 
  { MSR_MC00_CTL, MSR_MC00_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_IFU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC01_CTL, MSR_MC01_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC02_CTL, MSR_MC02_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DTLB0     , ReadMcBankSignature, {NULL}},
  { MSR_MC03_CTL, MSR_MC03_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_MLC0      , ReadMcBankSignature, {NULL}},
  { MSR_MC04_CTL, MSR_MC04_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_PCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC05_CTL, MSR_MC05_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI0      , ReadMcBankSignature, {NULL}},
  { MSR_MC06_CTL, MSR_MC06_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_IIO0      , ReadMcBankSignature, {NULL}},
  { MSR_MC07_CTL, MSR_MC07_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA0       , ReadHaMcBankSignature, {NULL}},
  { MSR_MC08_CTL, MSR_MC08_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     0                  , ReadNullSignature   ,{NULL}},
  { MSR_MC09_CTL, MSR_MC09_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC0      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC10_CTL, MSR_MC10_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC1      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC11_CTL, MSR_MC11_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC2      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC12_CTL, MSR_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC3      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC13_CTL, MSR_MC13_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     0                  , ReadNullSignature, {NULL}},
  { MSR_MC14_CTL, MSR_MC14_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     0                  , ReadNullSignature, {NULL}},
  { MSR_MC15_CTL, MSR_MC15_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     0                  , ReadNullSignature, {NULL}},
  { MSR_MC16_CTL, MSR_MC16_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     0                  , ReadNullSignature, {NULL}},
  { MSR_MC17_CTL, MSR_MC17_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO00     , ReadMcBankSignature, {NULL}},
  { MSR_MC18_CTL, MSR_MC18_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO01     , ReadMcBankSignature, {NULL}},
  { MSR_MC19_CTL, MSR_MC19_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO02     , ReadMcBankSignature, {NULL}},
  { MSR_MC20_CTL, MSR_MC20_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI1      , ReadMcBankSignature, {NULL}},
  { MSR_MC21_CTL, MSR_MC21_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     0                  , ReadNullSignature, {NULL}}
};

EMCA_MCBANK_DESC gMcBankListHswEx[] = { 
  { MSR_MC00_CTL, MSR_MC00_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_IFU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC01_CTL, MSR_MC01_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC02_CTL, MSR_MC02_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_DTLB0     , ReadMcBankSignature, {NULL}},
  { MSR_MC03_CTL, MSR_MC03_CTL2, MC_SCOPE_CORE                  , EMCA_SECTYPE_NONE,     MCA_UNIT_MLC0      , ReadMcBankSignature, {NULL}},
  { MSR_MC04_CTL, MSR_MC04_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_PCU0      , ReadMcBankSignature, {NULL}},
  { MSR_MC05_CTL, MSR_MC05_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI0      , ReadMcBankSignature, {NULL}},
  { MSR_MC06_CTL, MSR_MC06_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_IIO0      , ReadMcBankSignature, {NULL}},
  { MSR_MC07_CTL, MSR_MC07_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA0       , ReadHaMcBankSignature, {NULL}},
  { MSR_MC08_CTL, MSR_MC08_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_HA1       , ReadHaMcBankSignature, {NULL}},
  { MSR_MC09_CTL, MSR_MC09_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC0      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC10_CTL, MSR_MC10_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC1      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC11_CTL, MSR_MC11_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC2      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC12_CTL, MSR_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC3      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC13_CTL, MSR_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC4      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC14_CTL, MSR_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC5      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC15_CTL, MSR_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC6      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC16_CTL, MSR_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG_SUPPORT, EMCA_SECTYPE_PLATMEM,  MCA_UNIT_IMC7      , ReadMcBankSignature, {McaiMcCsrInfoHook}},
  { MSR_MC17_CTL, MSR_MC17_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO00     , ReadMcBankSignature, {NULL}},
  { MSR_MC18_CTL, MSR_MC18_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO01     , ReadMcBankSignature, {NULL}},
  { MSR_MC19_CTL, MSR_MC19_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_CBO02     , ReadMcBankSignature, {NULL}},
  { MSR_MC20_CTL, MSR_MC20_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI1      , ReadMcBankSignature, {NULL}},
  { MSR_MC21_CTL, MSR_MC21_CTL2, MC_SCOPE_PKG                   , EMCA_SECTYPE_NONE,     MCA_UNIT_QPI2      , ReadMcBankSignature, {NULL}}
};


HA_LOOKUP *mHaLookup = NULL;
IMC_NODE_LOOKUP *mImcNodeIdLookup = NULL;

UINTN mImcNodeIdLookupEntries = 0;


IMC_NODE_LOOKUP mImcNodeIdLookupHsx[] = {
//McbankNum, Memoryid
  { 9,  0 },
  { 10, 0 },
  { 11, 0 },
  { 12, 0 },
  { 13, 1 },
  { 14, 1 },
  { 15, 1 },
  { 16, 1 },
};

HA_LOOKUP mHaLookup4HA[] = {
//McbankNum, Instance 
  { 7 , 0 },
  { 8 , 1 },
  { 26, 2 },
  { 27, 3 }
};

HA_LOOKUP mHaLookup1HA[] = {
//McbankNum, Instance 
  { 7, 0 }
};

HA_LOOKUP mHaLookup2HA[] = {
//McbankNum, Instance 
  { 7, 0 },
  { 8, 1 }
};



EMCA_MCRECOVERY_DESC gMcaRecoverables[] = {
  // MCACOD , MCACODMASK, MSCOD, MSCODMASK,  Handler
  { MCA_PASCRUB_MCACOD, MCA_PASCRUB_MCACOD_MSK, MCA_PASCRUB_MSCOD, MCA_PASCRUB_MSCOD_MSK, RecoveryPatrolScrubDummy },
};

UINTN gMcaRecorevablesEntries = sizeof(gMcaRecoverables)/sizeof(EMCA_MCRECOVERY_DESC);

UINT32 mHaLookupEntries = 0;


STATIC EMCA_PHLIB_PRIV mEmcaPh = { 0 }; 

UINT64 mMcgCap = 0;
UINT64 mL1Address = 0;

EFI_STATUS
InitMcBankList(
VOID
)
{ UINT8 CpuType = 0;
  UINT8 CpuSubType = 0;
  UINT8 physical_chop = 0;
  UINT32 capid4;
  capid4 = mEmcaPh.CpuCsrAccess->ReadCpuCsr(0, 0, CAPID4_PCU_FUN3_REG);

  CpuType =  mEmcaPh.IioUds->SystemStatus.cpuType;
  CpuSubType= mEmcaPh.IioUds->SystemStatus.cpuSubType;

  if (CpuType == CPU_HSX)
    physical_chop = (UINT8) ((capid4 >> 6) & 0x3); // physical_chop CAPID [7:6]
  else if (CpuType == CPU_BDX)
    physical_chop = (UINT8) ((capid4 >> 6) & 0x7); // physical_chop CAPID [8:6]
  else
    return EFI_SUCCESS;

  
  DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: initMcBankList\n"));
  DEBUG((EFI_D_ERROR, "\nCpuType = %x, CpuSubType = %x\n", CpuType, CpuSubType));
  switch(CpuType) {
    case CPU_HSX:
    case CPU_BDX:
      switch(CpuSubType) {
        case CPU_HSX_EX:
        //case CPU_BDX_EX:
          DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_EX, CPU_BDX_EX\n"));
          gMcBankList = &gMcBankListHswEx[0];
          mHaLookup = &mHaLookup2HA[0];
          mHaLookupEntries = sizeof(mHaLookup2HA)/sizeof(HA_LOOKUP);          
          mImcNodeIdLookup = &mImcNodeIdLookupHsx[0];
          mImcNodeIdLookupEntries = sizeof(mImcNodeIdLookupHsx)/sizeof(IMC_NODE_LOOKUP);
          break;          
        case CPU_HSX_1HA:
        //case CPU_BDX_1HA:
          DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_LCC, CPU_BDX_LCC\n"));
          gMcBankList = &gMcBankListHswLCC[0];
          mHaLookup = &mHaLookup1HA[0];
          mHaLookupEntries = sizeof(mHaLookup1HA)/sizeof(HA_LOOKUP); 
          mImcNodeIdLookup = &mImcNodeIdLookupHsx[0];
          mImcNodeIdLookupEntries = sizeof(mImcNodeIdLookupHsx)/sizeof(IMC_NODE_LOOKUP);
          break;
        case CPU_HSX_2HA:
        //case CPU_BDX_2HA:
          mHaLookup = &mHaLookup2HA[0];
          mHaLookupEntries = sizeof(mHaLookup2HA)/sizeof(HA_LOOKUP); 
          mImcNodeIdLookup = &mImcNodeIdLookupHsx[0];
          mImcNodeIdLookupEntries = sizeof(mImcNodeIdLookupHsx)/sizeof(IMC_NODE_LOOKUP);
          DEBUG((EFI_D_ERROR, "\nphysical_chop = %x\n", physical_chop));
          switch(physical_chop) {
            case HSX_PHY_CHOP_MCC:
            //case BDX_PHY_CHOP_MCC:
              DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_MCC, CPU_BDX_MCC\n"));
              gMcBankList = &gMcBankListHswMCC[0];
              break;
            case HSX_PHY_CHOP_HCC:
            //case BDX_PHY_CHOP_MCC:
              DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_HCC, CPU_BDX_HCC\n"));
              gMcBankList = &gMcBankListHswHCC[0];
              break;
            default:
              DEBUG((EFI_D_ERROR, "\nERROR:Unable to determine between MCC and HCC, default to MCC\n"));
              gMcBankList = &gMcBankListHswMCC[0];
              break;
          }
          break;
      }
      break;
    case CPU_IVT:
      break;
  }
  if(gMcBankList == NULL || mHaLookup == NULL) {
    DEBUG((EFI_D_ERROR, "\nERROR Emcaplatformhookslib:gMcBankList or mHaLookup are NULL\n"));
  }
  return EFI_SUCCESS;
}
  
                           

EFI_STATUS
EFIAPI
EmcaPlatformHooksLibConstructor (
IN EFI_HANDLE ImageHandle,
IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS Status;
  BOOLEAN InSmm;
  EFI_IIO_UDS_PROTOCOL *iioUdsProtocol;

  if(mEmcaPh.SmmBase == NULL) {
    Status = gBS->LocateProtocol(
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **)&mEmcaPh.SmmBase
                  );
    ASSERT_EFI_ERROR(Status);
  }
  
  mEmcaPh.SmmBase->InSmm(mEmcaPh.SmmBase, &InSmm);

  if(mEmcaPh.IioUds == NULL) {
    //
    // Locate the IIO Protocol Interface
    //
    Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL, &iioUdsProtocol);
    ASSERT_EFI_ERROR (Status);
    mEmcaPh.IioUds = iioUdsProtocol->IioUdsPtr;
  }

  if(InSmm) {

    if(mEmcaPh.Smst == NULL) {
      Status = mEmcaPh.SmmBase->GetSmstLocation(
                                  mEmcaPh.SmmBase,
                                  &mEmcaPh.Smst
                                );
      ASSERT_EFI_ERROR(Status);
    }

    if(mEmcaPh.MemRas == NULL) {
      Status = mEmcaPh.Smst->SmmLocateProtocol(
                               &gEfiMemRasProtocolGuid,
                               NULL,
                               &mEmcaPh.MemRas
                             );
      ASSERT_EFI_ERROR(Status);  
    }

    if(mEmcaPh.CpuCsrAccess == NULL) {
      Status = mEmcaPh.Smst->SmmLocateProtocol(
                               &gEfiCpuCsrAccessGuid, 
                               NULL, 
                               &mEmcaPh.CpuCsrAccess
                             );
      ASSERT_EFI_ERROR (Status);
    }
    
    if(gMcBankList == NULL) {
      InitMcBankList();
    }    
  }
  
  return EFI_SUCCESS;
}

/**
  Get the number of MCBANKS_PER_LP.
  
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  Assumtion of implementation is that all sockets are symetric, they all have the same number of MCBanks Per LP
  
  @return MCBANKS_PER_LP

**/
UINT64
GetNumMcBanksPerLp(
  VOID
  )

{
  UINT32 McgCapAddr = 0;
  UINT64 McBanksNum = 0;
  
  McgCapAddr = EFI_IA32_MCG_CAP;
  McBanksNum = AsmReadMsr64(McgCapAddr);
  McBanksNum = BitFieldRead64(McBanksNum,N_MSR_MCG_COUNT_START, N_MSR_MCG_COUNT_STOP);
  return McBanksNum;
}

/**
  Get the number of L1 Dir Entries required based on the MC Banks available and number of sockets.

  Number of L1 Dir Entries = SOCKET_COUNT * ( 2 ** APICID_BITS_PER_SOCKET) * MCBANKS_PER_LP where: 
  
  SOCKET_COUNT is  the maximum number  of  processor  sockets in  the platform,
  APICID_BITS_PER_SOCKET is the number of APIC ID bits used up by each socket  
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  
  @return number of L1 Dir Entries  

**/
UINT64
GetNumL1DirEntries(
  VOID
  )
{
  UINT64 Num = 0;
  UINT64 ApicIdBitsPerSkt = 0;
  UINTN  ThreadBits = 0;
  UINTN  CoreBits = 0;

  GetCoreAndSMTBits(&ThreadBits,&CoreBits);
  ApicIdBitsPerSkt = ThreadBits + CoreBits;
  
  Num = MAX_SOCKET * LShiftU64(1 , ApicIdBitsPerSkt) * GetNumMcBanksPerLp();
  return Num;
}

/**
  Get the number of Elog Dir(L2) Entries required based on the MC Banks in the system.

 NumElogentries = number of MC BANKS in the system
                      = NumSKTSupported * NumMCBanksPerSKT
                      = NumSKTSupported * (NumCoreMcBanks *  CORES_PER_SOCKET + NumUncoreMcBanks ) 
  
  @return Number of Elog Entries 

**/
UINT64 
GetNumElogEntries(
  VOID
  )
{
  UINT32 i;
  UINT64 McBanksNum;
  UINT64 PhyMcBankNum;
  UINT16  McScope;
  UINTN Corebits;
  UINTN Threadbits;
  
  McBanksNum = GetNumMcBanksPerLp();
  GetCoreAndSMTBits(&Threadbits,&Corebits);

  for (i = 0, PhyMcBankNum = 0; i < McBanksNum; i++) {
    if(isEMCALogSupported(i)) {
      McScope = GetMcBankScope(i);
      switch(McScope) {
        
        case MC_SCOPE_TD:
          PhyMcBankNum += (LShiftU64(1 , Threadbits)) * (LShiftU64(1 , Corebits)) * (MAX_SOCKET);
          break;
        case MC_SCOPE_CORE:
          PhyMcBankNum += (LShiftU64(1 , Corebits)) * (MAX_SOCKET);
          break;
        case MC_SCOPE_PKG:
          PhyMcBankNum += (MAX_SOCKET);
          break;
        default:
          ASSERT(FALSE); //should not enter here
          break;
      }         
    }
  }

  return PhyMcBankNum;
}


BOOLEAN
isEMCALogSupported(
  UINT32 McBankIdx
  )
{
  ASSERT(gMcBankList != NULL);
  return ( (gMcBankList[McBankIdx].Attrib & EMCA_LOG_SUPPORT) != 0 );
}

BOOLEAN
isEmcaMemoryBank(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  if(((gMcBankList[McBankIdx].UnitType & MCA_UNIT_TYPE_HA) != 0) || 
     ((gMcBankList[McBankIdx].UnitType & MCA_UNIT_TYPE_IMC) != 0)) {
    return TRUE;
  }

  return FALSE;
}


UINT16
GetMcBankScope(
  UINT32 McBankIdx
  )
{
  ASSERT(gMcBankList != NULL);
  return ( gMcBankList[McBankIdx].Attrib & MCSCOPE_MASK );
}

UINT32
GetAddrMcBankSts
(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  return gMcBankList[McBankIdx].CtlMsr + 0x1;  
}

UINT32
  GetAddrMcBankCtl
(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  return gMcBankList[McBankIdx].CtlMsr;
}

UINT32
GetAddrMcBankAddr
(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  return gMcBankList[McBankIdx].CtlMsr +  0x2;
}

UINT32
GetAddrMcBankMisc
(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  return gMcBankList[McBankIdx].CtlMsr + 0x3;
}

UINT32
GetAddrMcBankCtl2
(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  return gMcBankList[McBankIdx].Ctl2Msr;
}

UINT16
GetMcBankSecType
(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  return gMcBankList[McBankIdx].SecType;
}

EFI_STATUS
EFIAPI 
McaiMcCsrInfoHook
(
  IN MCA_BANK_HOOKS *This,
  IN UINT32 McBankIdx,
  IN UINT8 *Skt,
  IN EMCA_MC_SIGNATURE_ERR_RECORD *McSig,
  IN OUT VOID *Section 
) 
{
  UINTN i;
  UINT8 ChannelId;
  UINT8 DimmId;
  UINT8 dimm;
  IA32_MCI_STATUS_MSR_STRUCT MsrIa32MCiStatus;
  PLATFORM_MEMORY_ERROR_DATA_UEFI_231 *MemSection;
  UINT16 MemoryControllerId;
  RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT RetryRdErrLog;
  BOOLEAN CsrInfoErrors = FALSE;
#ifdef HSX_HOST
  CORRERRORSTATUS_MCDDC_DP_HSX_STRUCT corrErrStsReg;
#else
  CORRERRORSTATUS_MCDDC_DP_BDX_STRUCT corrErrStsReg;
#endif
  EMCA_CSR_INFO_DESC iMcCsrInfoErrorLookup[] = {
    // MCACOD , MCACODMASK, MSCOD, MSCODMASK
    { IMC_PATROL_SCRUB_ERR_MCACOD, IMC_PATROL_SCRUB_ERR_MCACOD_MSK, IMC_UNC_PATROL_SCRUB_ERR_MSCOD, IMC_PATROL_SCRUB_ERR_MSCOD_MSK,PLATFORM_MEM_ERROR_SCRUB_UNCOR},
    { IMC_PATROL_SCRUB_ERR_MCACOD, IMC_PATROL_SCRUB_ERR_MCACOD_MSK, IMC_COR_PATROL_SCRUB_ERR_MSCOD, IMC_PATROL_SCRUB_ERR_MSCOD_MSK,PLATFORM_MEM_ERROR_SCRUB_COR},
    { IMC_SPARING_ERR_MCACOD, IMC_SPARING_ERR_MCACOD_MSK, IMC_UNC_SPARING_ERR_MSCOD, IMC_SPARING_ERR_MSCOD_MSK,PLATFORM_MEM_ERROR_SPARING},
    { IMC_SPARING_ERR_MCACOD, IMC_SPARING_ERR_MCACOD_MSK, IMC_COR_SPARING_ERR_MSCOD, IMC_SPARING_ERR_MSCOD_MSK,PLATFORM_MEM_ERROR_SPARING},
    { IMC_CMD_ADD_PARITY_ERR_MCACOD, IMC_CMD_ADD_PARITY_ERR_MCACOD_MSK, IMC_CMD_ADD_PARITY_ERR_MSCOD, IMC_CMD_ADD_PARITY_ERR_MSCOD_MSK,PLATFORM_MEM_ERROR_PARITY}
  };
  UINTN CsrInfoEntries = sizeof(iMcCsrInfoErrorLookup)/sizeof(EMCA_CSR_INFO_DESC);

  if(This == NULL || 
     Skt == NULL ||
     *Skt >= GetNumMcBanksPerLp() ||
     McSig == NULL ||
     Section == NULL
     ) {
    return EFI_INVALID_PARAMETER;
  }

  if(mEmcaPh.MemRas == NULL ||
     mImcNodeIdLookup == NULL
     ){
    return EFI_NOT_READY;
  }

  MemSection = (PLATFORM_MEMORY_ERROR_DATA_UEFI_231 *)Section;

  MsrIa32MCiStatus.Data = McSig->Signature.McSts;

  if(MsrIa32MCiStatus.Bits.val != 1) {
    return EFI_NOT_FOUND;
  }

  for(i=0; i < CsrInfoEntries; i++) {
    if( ((MsrIa32MCiStatus.Bits.mca_code & iMcCsrInfoErrorLookup[i].McaCodMsk) == iMcCsrInfoErrorLookup[i].McaCod) &&
          ((MsrIa32MCiStatus.Bits.ms_code & iMcCsrInfoErrorLookup[i].MsCodMsk) == iMcCsrInfoErrorLookup[i].MsCod)
        ) {
        CsrInfoErrors = TRUE;
        MemSection->ErrorType = iMcCsrInfoErrorLookup[i].ErrorType;
        MemSection->ValidFields |= PLATFORM_MEM_ERROR_TYPE_VALID;
    }
  }

  if(CsrInfoErrors == FALSE) {
    return EFI_NOT_FOUND;
  }

  for(i=0;i < mImcNodeIdLookupEntries; i++) {
    if(mImcNodeIdLookup[i].McBankNum == McBankIdx) {
      MemoryControllerId = mImcNodeIdLookup[i].MemoryControllerId;
      ChannelId = MsrIa32MCiStatus.Bits.mca_code & IMC_MCACOD_CHANNEL_MSK;
      if (MemSection->ErrorType == PLATFORM_MEM_ERROR_PARITY) {
        //
        // Command/Address Parity (CAP) error.
        //
        MemSection->Node = SKTMC_TO_NODE(*Skt, MemoryControllerId);
        MemSection->ValidFields |= PLATFORM_MEM_NODE_VALID;
        if (ChannelId != 0xF) {
          MemSection->Card = ChannelId;
          MemSection->ValidFields |= PLATFORM_MEM_CARD_VALID;
        }

        if (mEmcaPh.MemRas->SystemMemInfo->NodeInfo[MemSection->Node].DramType == SPD_TYPE_DDR4) {
          corrErrStsReg.Data = mEmcaPh.MemRas->MemReadPciCfg ((UINT8) MemSection->Node,
                                                              ChannelId,
                                                              CORRERRORSTATUS_MCDDC_DP_REG
                                                              );
          DimmId = (UINT8) corrErrStsReg.Bits.dimm_alert;
          if (DimmId != 0) {
            dimm = 0;
            while ((DimmId & 0x1) == 0 && dimm < MAX_DIMM) {
              DimmId >>= 1;
              dimm++;
            }
            MemSection->ModuleRank = dimm;
            MemSection->ValidFields |= PLATFORM_MEM_MODULE_VALID;
          }
        }

#if DBG_MSGS
        if (mEmcaPh.MemRas->SystemMemInfo->NodeInfo[MemSection->Node].DramType == SPD_TYPE_DDR4) {
          InternalPrintMessage1("McaiMcCsrInfoHook (DDR4 Cmd/Adr Parity Error): Node = %d", MemSection->Node);
        } else {
          InternalPrintMessage1("McaiMcCsrInfoHook (Cmd/Adr Error): Node = %d", MemSection->Node);
        }
        if (MemSection->ValidFields & PLATFORM_MEM_CARD_VALID) {
          InternalPrintMessage1(", Card = %d", MemSection->Card);
        } else {
          InternalPrintMessage1(", Card = ?");
        }
        if (MemSection->ValidFields & PLATFORM_MEM_MODULE_VALID) {
          InternalPrintMessage1(", ModuleRank = %d\n", MemSection->ModuleRank);
        } else {
          InternalPrintMessage1(", ModuleRank = ?\n");
        }
#endif
      }
      if (!((MemSection->ErrorType == PLATFORM_MEM_ERROR_PARITY) &&
            (mEmcaPh.MemRas->SystemMemInfo->NodeInfo[MemSection->Node].DramType == SPD_TYPE_DDR4))
         ) {
        if(ChannelId != 0xf) {
          RetryRdErrLog.Data = mEmcaPh.MemRas->MemReadPciCfg(SKTMC_TO_NODE(*Skt, MemoryControllerId),
                                                          ChannelId,
                                                          RETRY_RD_ERR_LOG_MCDDC_DP_REG
                                                          );
          if(RetryRdErrLog.Bits.v == 1 && RetryRdErrLog.Bits.patspr == 1) {
            MemSection->Node = SKTMC_TO_NODE(*Skt, MemoryControllerId);
            MemSection->Card = ChannelId;
            MemSection->RankNumber = (UINT16)RetryRdErrLog.Bits.rank;
            DimmId = (UINT8)RetryRdErrLog.Bits.dimm;
            dimm = 0;
            while((DimmId & 0x1) == 0 && dimm < MAX_DIMM) {
              DimmId >>= 1;
              dimm++;
            }
            MemSection->ModuleRank = dimm;
            MemSection->ValidFields |= PLATFORM_MEM_MODULE_VALID  |
                                       PLATFORM_RANK_NUMBER_VALID |
                                       PLATFORM_MEM_CARD_VALID    |
                                       PLATFORM_MEM_NODE_VALID    ;
            //clear CSR after log
            RetryRdErrLog.Data = 0;
            RetryRdErrLog.Bits.en = 1;
            RetryRdErrLog.Bits.en_patspr = 1;
            mEmcaPh.MemRas->MemWritePciCfg(SKTMC_TO_NODE(*Skt, MemoryControllerId),
                                        ChannelId,
                                        RETRY_RD_ERR_LOG_MCDDC_DP_REG,
                                        RetryRdErrLog.Data
                                        );
          }
        }
      }
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI 
ReadMcBankSignature(
  IN UINT32 McBankIdx,
  IN UINT8 skt,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
)
{
  UINT32 msrAddr;
  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;

  if((McSig == NULL) || (McBankIdx >= GetNumMcBanksPerLp())) {
    return EFI_INVALID_PARAMETER;
  }
  
  msrAddr = GetAddrMcBankSts(McBankIdx);
  MciStsMsr.Data = AsmReadMsr64 (msrAddr);

  //
  // Fill Error Signature
  //
  McSig->Signature.McSts = MciStsMsr.Data;

  if(MciStsMsr.Bits.addrv == 1) {
    msrAddr = GetAddrMcBankAddr(McBankIdx);
    McSig->Signature.McAddr = AsmReadMsr64 (msrAddr);
  } else {
    McSig->Signature.McAddr = 0;
  }
  
  if(MciStsMsr.Bits.miscv == 1) {
    msrAddr = GetAddrMcBankMisc(McBankIdx);
    McSig->Signature.McMisc = AsmReadMsr64 (msrAddr);
  } else {
    McSig->Signature.McMisc = 0;
  }
  McSig->ApicId = GetApicId();
  
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI 
ReadNullSignature(
  IN UINT32 McBankIdx,
  IN UINT8 skt,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
)
{
  if((McSig == NULL) || (McBankIdx >= GetNumMcBanksPerLp())) {
    return EFI_INVALID_PARAMETER;
  }
  
  McSig->ApicId = GetApicId();
  McSig->Signature.McSts = 0; 
  McSig->Signature.McAddr = 0;
  McSig->Signature.McMisc = 0;
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI 
ReadHaMcBankSignature(
  IN UINT32 McBankIdx,
  IN UINT8 skt,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD * McSig
)
{
  MC5_STATUS_SHADOW_N0_HA_PMON_HSX_BDX_STRUCT Mc5Shadow0Status;
  MC5_STATUS_SHADOW_N1_HA_PMON_HSX_BDX_STRUCT Mc5Shadow1Status;
  MC5_MISC_SHADOW_N0_HA_PMON_HSX_BDX_STRUCT   Mc5Shadow0Misc;
  MC5_MISC_SHADOW_N1_HA_PMON_HSX_BDX_STRUCT   Mc5Shadow1Misc;
  
  MC5_ADDR_SHADOW0_HA_PMON_STRUCT         Mc5Shadow0Addr;
  MC5_ADDR_SHADOW1_HA_PMON_STRUCT         Mc5Shadow1Addr;
  UINT8                                   Instance;
  UINT8                                   ha;
  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;

  if((McSig == NULL) || (McBankIdx >= GetNumMcBanksPerLp())) {
    return EFI_INVALID_PARAMETER;
  }
  ASSERT(mHaLookup != NULL);

  for(ha=0, Instance=0; ha < mHaLookupEntries; ha++) {
    if(McBankIdx == mHaLookup[ha].McBankNum) {
      Instance = mHaLookup[ha].Instance;
    }
  }

  ReadMcBankSignature(McBankIdx,skt,McSig);
  MciStsMsr.Data = McSig->Signature.McSts;

  if(MciStsMsr.Bits.val == 0) {

    Mc5Shadow1Status.Data = mEmcaPh.CpuCsrAccess->ReadCpuCsr(skt,Instance,MC5_STATUS_SHADOW_N1_HA_PMON_REG);
    Mc5Shadow0Status.Data = mEmcaPh.CpuCsrAccess->ReadCpuCsr(skt,Instance,MC5_STATUS_SHADOW_N0_HA_PMON_REG);
    
    McSig->Signature.McSts = ((UINT64)Mc5Shadow1Status.Data << 32) | Mc5Shadow0Status.Data;

    if( Mc5Shadow1Status.Bits.valid && Mc5Shadow1Status.Bits.addrv) {
      Mc5Shadow0Addr.Data = mEmcaPh.CpuCsrAccess->ReadCpuCsr(skt,Instance, MC5_ADDR_SHADOW0_HA_PMON_REG);
      Mc5Shadow1Addr.Data = mEmcaPh.CpuCsrAccess->ReadCpuCsr(skt,Instance, MC5_ADDR_SHADOW1_HA_PMON_REG);
      
      McSig->Signature.McAddr = ((UINT64)Mc5Shadow1Addr.Data << 32) | (Mc5Shadow0Addr.Data );
      
    } else {
       McSig->Signature.McAddr = 0;
    }

     if( Mc5Shadow1Status.Bits.valid && Mc5Shadow1Status.Bits.miscv) {
      Mc5Shadow0Misc.Data = mEmcaPh.CpuCsrAccess->ReadCpuCsr(skt,Instance, MC5_MISC_SHADOW_N0_HA_PMON_REG);
      Mc5Shadow1Misc.Data = mEmcaPh.CpuCsrAccess->ReadCpuCsr(skt,Instance, MC5_MISC_SHADOW_N1_HA_PMON_REG);
      
      McSig->Signature.McMisc= ((UINT64)Mc5Shadow1Misc.Data << 32) | (Mc5Shadow0Misc.Data );
      
    } else {
       McSig->Signature.McMisc = 0;
    }
    McSig->ApicId = GetApicId();
  }
  return EFI_SUCCESS;  
}

EFI_STATUS
EFIAPI 
ReadMcBankSigHook(
  IN UINT32 McBankIdx,
  IN UINT8 skt,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD * McSig
)
{
  EFI_STATUS status = EFI_SUCCESS;
  
  if((McSig == NULL) || (McBankIdx >= GetNumMcBanksPerLp()) || skt >= MAX_SOCKET) {
    return EFI_INVALID_PARAMETER;
  }
  
  ASSERT(gMcBankList != NULL);
  
  if(gMcBankList[McBankIdx].ReadMcBankSig != NULL) {
    status = gMcBankList[McBankIdx].ReadMcBankSig(McBankIdx,skt,McSig);  
  } else {
    SetMem(McSig,sizeof(EMCA_MC_SIGNATURE_ERR_RECORD),0);
  }
  return status; 
  
}


EFI_STATUS
MCBankCsrInfoHook(
  IN UINT32 McBankIdx,
  IN UINT8 *Skt,
  IN EMCA_MC_SIGNATURE_ERR_RECORD *McSig,
  IN OUT VOID *Section 
)
{
  EFI_STATUS status;
  status = EFI_NOT_FOUND;
  
  ASSERT(gMcBankList != NULL);
  
  if(gMcBankList[McBankIdx].Hooks.CsrInfoHook != NULL) {
    status = gMcBankList[McBankIdx].Hooks.CsrInfoHook(&gMcBankList[McBankIdx].Hooks, McBankIdx, Skt, McSig, Section);  
  }
  return status;  
}

EFI_STATUS 
GetCoreAndSMTBits (
  IN UINTN *ThreadBits,
  IN UINTN *CoreBits
  )
{
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
  UINT32  MaxCpuIdIndex;
  UINT32  SubIndex;
  UINTN   LevelType;
  UINT32  MaxLogicProcessorsPerPackage;
  UINT32  MaxCoresPerPackage;

  ASSERT (ThreadBits != NULL);
  ASSERT (CoreBits != NULL);
  
  *ThreadBits = 0;
  *CoreBits = 0;

  //
  // Check if the processor is capable of supporting more than one logical processor.
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, NULL, NULL, &RegEdx);
  ASSERT ((RegEdx & BIT28) != 0);

  //
  // Assume three-level mapping of APIC ID: Package:Core:SMT.
  //

  //
  // Get the max index of basic CPUID
  //
  AsmCpuid (EFI_CPUID_SIGNATURE, &MaxCpuIdIndex, NULL, NULL, NULL);

  if (MaxCpuIdIndex >= EFI_CPUID_CORE_TOPOLOGY) {
    AsmCpuidEx (EFI_CPUID_CORE_TOPOLOGY, 0, &RegEax, &RegEbx, NULL, NULL);
    if ((RegEbx & 0xffff) != 0) {
      //
      // x2APIC ID
      // 

      *ThreadBits = RegEax & 0x1f;

      *CoreBits = 0;
      SubIndex = 1;
      do {
        AsmCpuidEx (EFI_CPUID_CORE_TOPOLOGY, SubIndex, &RegEax, NULL, &RegEcx, NULL);
        LevelType = (RegEcx >> 8) & 0xff;
        if (LevelType == CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_CORE) {
          *CoreBits = (RegEax & 0x1f) - *ThreadBits;
          break;
        }
        SubIndex++;
      } while (LevelType != CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_INVALID);  
    }
  } else {
    
    //
    // xAPIC ID
    //

    AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &RegEbx, NULL, NULL);
    MaxLogicProcessorsPerPackage = (RegEbx >> 16) & 0xff;
    if (MaxCpuIdIndex >= EFI_CPUID_CACHE_PARAMS) {
      AsmCpuidEx (EFI_CPUID_CACHE_PARAMS, 0, &RegEax, NULL, NULL, NULL);
      MaxCoresPerPackage = (RegEax >> 26) + 1;
    } else {
      MaxCoresPerPackage = 1;
    }

    *ThreadBits = (UINTN) (HighBitSet32 (MaxLogicProcessorsPerPackage / MaxCoresPerPackage - 1) + 1);
    *CoreBits = (UINTN) (HighBitSet32 (MaxCoresPerPackage - 1) + 1);
  }
  
  return EFI_SUCCESS;
}



UINT32 
GetApicIdByLoc (
  IN EFI_CPU_PHYSICAL_LOCATION *Location
  )
{
  UINTN   ThreadBits;
  UINTN   CoreBits;
  UINT32  ApicId; 

  ASSERT (Location != NULL);

  GetCoreAndSMTBits(&ThreadBits,&CoreBits);

  ApicId = (Location->Package << (ThreadBits + CoreBits)) | (Location->Core << ThreadBits) |  (Location->Thread);
  return ApicId;
}

VOID
EmcaEnableMSMI(
  IN UINT32 *McBankIdx
)
{
  UINT64   McBankSupp;
  UINT32   McBankAddr;
  UINT64 McCtl2;

  if(McBankIdx != NULL) {    
    McBankSupp = GetEMcaBankSupp();

    if(((McBankSupp >> *McBankIdx) & 0x1) != 0) {
            
      McBankAddr = GetAddrMcBankCtl2(*McBankIdx);
      McCtl2 = AsmReadMsr64(McBankAddr);
      McCtl2 |= BIT34; //MCE to SMI CTL
      AsmWriteMsr64(McBankAddr,McCtl2);
      
      //Used as a sync point. The caller can wait for this idx to become 0xffffffff before iterating the next mcbank
      //*McBankIdx = ~((UINT32)0); 
    }
  }
}


VOID
EmcaMSMICoreBanks(
  VOID  *Buffer
)
{
  UINT64   McBankNum;
  UINT32 i;
  UINT16 McScope;
  EMCAMSMI_ARGS_STRUCT *Args;
  
  Args = (EMCAMSMI_ARGS_STRUCT *) Buffer;
  
  McBankNum = GetNumMcBanksPerLp();

  for(i = 0; i < (UINT32)McBankNum; i++) {
    McScope = GetMcBankScope(i);
    if(McScope == MC_SCOPE_CORE) {
      EmcaEnableMSMI(&i);
    }
  }
  ReleaseSpinLock(&Args->ApBusy);
}

VOID
EmcaMSMIPkgBanks(
  VOID  *Buffer
)
{
  UINT64   McBankNum;
  UINT32 i;
  UINT16 McScope;
  EMCAMSMI_ARGS_STRUCT *Args;

  Args = (EMCAMSMI_ARGS_STRUCT *) Buffer;
  
  McBankNum = GetNumMcBanksPerLp();

  for(i = 0; i < (UINT32)McBankNum; i++) {
    McScope = GetMcBankScope(i);
    if(McScope == MC_SCOPE_PKG) {
      EmcaEnableMSMI(&i);
    }
  }
   ReleaseSpinLock(&Args->ApBusy);
}

VOID
DisableCSMIGen1(
UINT8 *node
)
{
  EXRAS_CONFIG_HA_CFG_STRUCT		ExRasCfg;
  EMCALTCTL_MC_MAINEXT_STRUCT EmcaLtCtrl;
  

  ExRasCfg.Data = mEmcaPh.MemRas->MemReadPciCfg(NODE_TO_SKT(*node), NODE_TO_MC(*node), EXRAS_CONFIG_HA_CFG_REG);
  DEBUG ((DEBUG_ERROR, "before exras reg value = %lx \n", ExRasCfg.Data));
  ExRasCfg.Bits.cfgmcasmionfailover = 0;
  ExRasCfg.Bits.cfgmcacmcionfailover = 0;
  ExRasCfg.Bits.cfgmcasmionmirrorcorr = 0;
  ExRasCfg.Bits.cfgmcacmcionmirrorcorr = 0;
  ExRasCfg.Bits.cfgmcasmioncorr = 0;
  ExRasCfg.Bits.cfgmcacmcioncorrcountthr = 0;

  mEmcaPh.MemRas->MemWritePciCfg(  NODE_TO_SKT(*node), NODE_TO_MC(*node), EXRAS_CONFIG_HA_CFG_REG, ExRasCfg.Data);
  EmcaLtCtrl.Data = (UINT8)mEmcaPh.MemRas->MemReadPciCfg(  *node, PARAM_UNUSED, EMCALTCTL_MC_MAINEXT_REG);
  DEBUG ((DEBUG_ERROR, "before EmcaLtCtrl reg value = %lx \n", EmcaLtCtrl.Data));
  EmcaLtCtrl.Bits.emcalt_sel_smi = 0;
  mEmcaPh.MemRas->MemWritePciCfg(  *node, PARAM_UNUSED, EMCALTCTL_MC_MAINEXT_REG, EmcaLtCtrl.Data);
  DEBUG ((DEBUG_ERROR, "after exras reg value = %lx \n", ExRasCfg.Data));
  DEBUG ((DEBUG_ERROR, "after EmcaLtCtrl reg value = %lx \n", EmcaLtCtrl.Data));
}

VOID 
DisableCSMIGen2(
CMCI_GEN2_PARAMS *Params
)
{
  UINT64 McBankNum;
  UINT64 McBankSupp;
  UINT32 i;
  UINT64 McCtl2;
  UINT32 McBankIdx;

  McBankNum = GetNumMcBanksPerLp();
  McBankSupp = GetEMcaBankSupp();

  for(i = 0; i < McBankNum; i++) {
    if(((McBankSupp >> i) & 0x1) != 0) {
      McBankIdx = GetAddrMcBankCtl2(i);
      McCtl2 = AsmReadMsr64(McBankIdx);
      McCtl2 &= (~BIT32); //SMI to CMCI CTL
      AsmWriteMsr64(McBankIdx,McCtl2);	
    }
  }
  ReleaseSpinLock(&Params->Busy);  
}

VOID
EnableCSMIGen2(
CMCI_GEN2_PARAMS *Params
)
{
  UINT64 McBankNum;
  UINT64 McBankSupp;
  UINT32 i;
  UINT64 McCtl2;
  UINT32 McBankIdx;

  McBankNum = GetNumMcBanksPerLp();
  McBankSupp = GetEMcaBankSupp();
      
  for(i = 0; i < McBankNum; i++) {
    if(((McBankSupp >> i) & 0x1) != 0) {

      McBankIdx = GetAddrMcBankCtl2(i);
// AptioV Server Override Start: Skipping CSMI BIT Setting for Memory Bank MSR's HA0,HA1, IMC CH0 -7 
      if (McBankIdx >= MSR_MC07_CTL2 && McBankIdx <= MSR_MC16_CTL2)
          continue;
// AptioV Server Override End: Skipping CSMI BIT Setting for Memory Bank MSR's HA0,HA1, IMC CH0 -7 
      McCtl2 = AsmReadMsr64(McBankIdx);
      McCtl2 |= BIT32; //CMCI to SMI CTL
      AsmWriteMsr64(McBankIdx,McCtl2);    
    }
  }
  ReleaseSpinLock(&Params->Busy);  
}

VOID
EnableCSMIGen1(
UINT8 *node
) 
{
  EXRAS_CONFIG_HA_CFG_STRUCT      ExRasCfg;
  EMCALTCTL_MC_MAINEXT_STRUCT EmcaLtCtrl;

  mEmcaPh.MemRas->MemReadPciCfg(  NODE_TO_SKT(*node), NODE_TO_MC(*node), EXRAS_CONFIG_HA_CFG_REG);

  ExRasCfg.Bits.cfgmcasmionfailover = 1;
  ExRasCfg.Bits.cfgmcacmcionfailover = 0;
  ExRasCfg.Bits.cfgmcasmionmirrorcorr = 1;
  ExRasCfg.Bits.cfgmcacmcionmirrorcorr = 0;
  ExRasCfg.Bits.cfgmcasmioncorr = 1;
  ExRasCfg.Bits.cfgmcacmcioncorrcountthr = 1;

  mEmcaPh.MemRas->MemWritePciCfg(  NODE_TO_SKT(*node), NODE_TO_MC(*node), EXRAS_CONFIG_HA_CFG_REG, ExRasCfg.Data);

  EmcaLtCtrl.Data = (UINT8)mEmcaPh.MemRas->MemReadPciCfg(  *node, PARAM_UNUSED, EMCALTCTL_MC_MAINEXT_REG);
  EmcaLtCtrl.Bits.emcalt_sel_smi = 1;
  mEmcaPh.MemRas->MemWritePciCfg(  *node, PARAM_UNUSED, EMCALTCTL_MC_MAINEXT_REG, EmcaLtCtrl.Data);
  
}


STATIC
EFI_STATUS
ReadMCGCAP(
IN VOID *Buffer
) 
{

 UINT64 *Value;
 Value = (UINT64 *)Buffer;

 if(Value != NULL) {
  *Value = AsmReadMsr64(EFI_IA32_MCG_CAP);
 }
 return EFI_SUCCESS;
}

UINT64
ReadCoreSmiSrc(
VOID
)
{
  UINT64 retval;
  retval = 0;
  
  retval = AsmReadMsr64(MSR_CORE_SMI_ERR_SRC);
  return retval;
}

VOID
ClearCoreSmiSrc(
IN VOID *Buffer
)
{
  UINT64 *Value;
  Value = (UINT64 *)Buffer;
  
  AsmWriteMsr64(MSR_CORE_SMI_ERR_SRC,*Value);
}

UINT64
ReadUncoreSmiSrc(
VOID
)
{
  UINT64 retval;
  retval = 0;
  
  retval = AsmReadMsr64(MSR_UNCORE_SMI_ERR_SRC);
  return retval;
}

VOID
ClearUncoreSmiSrc(
IN VOID *Buffer
)
{
  UINT64 *Value;
  Value = (UINT64 *)Buffer;
  
  AsmWriteMsr64(MSR_UNCORE_SMI_ERR_SRC,*Value);
}



EFI_STATUS
GetEMCALogProcCap(
IN OUT BOOLEAN *EMcaLiteCap
)
{
  BOOLEAN InSmm;
  UINT8 RetVal = 0;

  if(EMcaLiteCap == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if( mEmcaPh.SmmBase == NULL ||  mEmcaPh.Smst == NULL) {
    return EFI_NOT_READY;
  }
  
  mEmcaPh.SmmBase->InSmm(mEmcaPh.SmmBase, &InSmm);

  if(InSmm) {

    ReadMCGCAP(&mMcgCap);
    RetVal = ((mMcgCap & BIT26) >> 26); //Check for EMCA log support capabality bit 
    
  } else {
    return EFI_NOT_READY;
  }

  *EMcaLiteCap = (RetVal == 1);

  return EFI_SUCCESS;
}

BOOLEAN
isEMCAGen2Cap(
VOID
)
{
  UINT64 McgCap;
  
  if(ReadMCGCAP(&McgCap) != EFI_SUCCESS) {
    McgCap = 0;
  }
    
  return ((McgCap & BIT25) != 0); 
}

UINT64
GetEMcaBankSupp(
VOID
)
{
  UINT64 SmmMcgCap;

  SmmMcgCap = AsmReadMsr64(MSR_SMM_MCA_CAP);
  
  return SmmMcgCap & (~((UINT32)0));
}

EFI_STATUS
InitL1PtrForSockets(
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank, 
  IN OUT VOID **NxtElog
  )
{
  UINT8 s;
  UINT8 c;
  UINT8 t;
  EFI_CPU_PHYSICAL_LOCATION loc;
  UINT8 *L1DirBase = NULL;
  UINT8 *L1DirEnd = NULL;
  UINT8 *ElogEnd = NULL;
  UINT8 *NxtElogIter = NULL;
  UINT64 *L1DirIter = NULL;
  UINTN Corebits;
  UINTN Threadbits;  

  if(L1DirHdr == NULL || NxtElog == NULL || McBank >= L1DirHdr->NumL1EntryLp) {
    return EFI_INVALID_PARAMETER;
  }

  GetCoreAndSMTBits(&Threadbits,&Corebits);

  NxtElogIter = (UINT8 *) *NxtElog;
  ElogEnd = (UINT8 *)(L1DirHdr->ElogDirBase + L1DirHdr->ElogDirLen); 
  L1DirBase = (UINT8 *)((UINT64)L1DirHdr + (UINT64)L1DirHdr->HdrLen);
  L1DirEnd = (UINT8 *)((UINT64)L1DirHdr + L1DirHdr->L1DirLen); 

  for(s = 0; s < MAX_SOCKET; s++, NxtElogIter += ELOG_ENTRY_LEN) {
    for(c = 0; c < LShiftU64(1 , Corebits); c++) {
      for(t = 0; t < MAX_THREAD; t++) {
        loc.Package = s;
        loc.Core = c; 
        loc.Thread = t;         
        L1DirIter = (UINT64 *)((UINT64)L1DirBase + (GetApicIdByLoc(&loc) * (L1DirHdr->NumL1EntryLp * sizeof(UINT64))) + (McBank * sizeof(UINT64)));
        
        if(NxtElogIter < ElogEnd && (UINT8 *)L1DirIter < L1DirEnd) {
          *L1DirIter = BIT63 | (UINT64)NxtElogIter;    
        } else {
          return EFI_OUT_OF_RESOURCES; 
        }
      }
    }
  }
  *NxtElog = (VOID*)NxtElogIter;
  return EFI_SUCCESS; 
}

EFI_STATUS
InitL1PtrForThreads(
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank, 
  IN OUT VOID **NxtElog
  )
{
  UINT8 s;
  UINT8 c;
  UINT8 t;
  EFI_CPU_PHYSICAL_LOCATION loc;
  UINT8 *L1DirBase = NULL;
  UINT8 *L1DirEnd = NULL;
  UINT8 *ElogEnd = NULL;
  UINT8 *NxtElogIter = NULL;
  UINT64 *L1DirIter = NULL;
  UINTN Corebits;
  UINTN Threadbits;
  

  if(L1DirHdr == NULL || NxtElog == NULL || McBank >= L1DirHdr->NumL1EntryLp) {
    return EFI_INVALID_PARAMETER;
  }

  GetCoreAndSMTBits(&Threadbits,&Corebits);

  NxtElogIter = (UINT8 *) *NxtElog;
  ElogEnd = (UINT8 *)(L1DirHdr->ElogDirBase + L1DirHdr->ElogDirLen); 
  L1DirBase = (UINT8 *)((UINT64)L1DirHdr + (UINT64)L1DirHdr->HdrLen);
  L1DirEnd = (UINT8 *)((UINT64)L1DirHdr + L1DirHdr->L1DirLen); 

  for(s = 0; s < MAX_SOCKET; s++) {
    for(c = 0; c < LShiftU64(1 , Corebits); c++) {
      for(t = 0; t < MAX_THREAD; t++, NxtElogIter += ELOG_ENTRY_LEN) {
        loc.Package = s;
        loc.Core = c; 
        loc.Thread = t;         
        L1DirIter = (UINT64 *)((UINT64)L1DirBase + (GetApicIdByLoc(&loc) * (L1DirHdr->NumL1EntryLp * sizeof(UINT64))) + (McBank * sizeof(UINT64)));
        
        if(NxtElogIter < ElogEnd && (UINT8 *)L1DirIter < L1DirEnd) {
          *L1DirIter = BIT63 | (UINT64)NxtElogIter;    
        } else {
          return EFI_OUT_OF_RESOURCES; 
        }
      }
    }
  }
  
  *NxtElog = (VOID*)NxtElogIter;
  return EFI_SUCCESS; 
}

EFI_STATUS
InitL1PtrForCores(
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank,
  IN OUT VOID **NxtElog
  )
{  
  UINT8 s;
  UINT8 c;
  UINT8 t;
  EFI_CPU_PHYSICAL_LOCATION loc;
  UINT8 *L1DirBase = NULL;
  UINT8 *L1DirEnd = NULL;
  UINT8 *ElogEnd = NULL;
  UINT8 *NxtElogIter = NULL;
  UINT64 *L1DirIter = NULL;
  UINTN Corebits;
  UINTN Threadbits;
  

  if(L1DirHdr == NULL || NxtElog == NULL || McBank >= L1DirHdr->NumL1EntryLp) {
    return EFI_INVALID_PARAMETER;
  }

  GetCoreAndSMTBits(&Threadbits,&Corebits);

  NxtElogIter = (UINT8 *) *NxtElog;
  ElogEnd = (UINT8 *)(L1DirHdr->ElogDirBase + L1DirHdr->ElogDirLen); 
  L1DirBase = (UINT8 *)((UINT64)L1DirHdr + (UINT64)L1DirHdr->HdrLen);
  L1DirEnd = (UINT8 *)((UINT64)L1DirHdr + L1DirHdr->L1DirLen);  

  for(s = 0; s < MAX_SOCKET; s++) {
    for(c = 0; c < LShiftU64(1 , Corebits); c++, NxtElogIter += ELOG_ENTRY_LEN) {
      for(t = 0; t < MAX_THREAD; t++) {
        loc.Package = s;
        loc.Core = c; 
        loc.Thread = t;         
        L1DirIter = (UINT64 *)((UINT64)L1DirBase + (GetApicIdByLoc(&loc) * (L1DirHdr->NumL1EntryLp * sizeof(UINT64))) + (McBank * sizeof(UINT64)));
        
        if(NxtElogIter < ElogEnd && (UINT8 *)L1DirIter < L1DirEnd) {
          *L1DirIter = BIT63 | (UINT64)NxtElogIter;    
        } else {
          return EFI_OUT_OF_RESOURCES; 
        }
      }
    }
  }
  
  *NxtElog = (VOID*)NxtElogIter;
  return EFI_SUCCESS; 
}

BOOLEAN
RecoveryPatrolScrubDummy(
IN UINT32 McBankIdx,
IN OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
)
{
  //Dummy function until we identify a recoverable error
  return FALSE;
}

BOOLEAN
AttemptRecovery(
IN UINT32 McBankIdx,
IN UINT8 Skt,
IN OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
)
{
  UINTN i;
  IA32_MCI_STATUS_MSR_STRUCT McSts;
  
  
  if(isEMCAGen2Cap() == FALSE){
    return FALSE;
  }

  if((McSig == NULL) || 
     (McBankIdx >= GetNumMcBanksPerLp()) ||
     Skt >= MAX_SOCKET) {
    return FALSE;
  }

  McSts.Data = McSig->Signature.McSts;

  if(McSts.Bits.val == 0  || 
     McSts.Bits.over == 1 ||
     McSts.Bits.en == 0
    ) {
    return FALSE;
  }
     

  for(i = 0; i < gMcaRecorevablesEntries;i++) {
    
    if( ((McSts.Bits.mca_code & gMcaRecoverables[i].McaCodMsk) == gMcaRecoverables[i].McaCod ) &&
        ((McSts.Bits.ms_code & gMcaRecoverables[i].MsCodMsk) == gMcaRecoverables[i].MsCod )
      ) {

      return (gMcaRecoverables[i].RecoveryHdr != NULL) ? gMcaRecoverables[i].RecoveryHdr(McBankIdx, McSig):FALSE;
        
    }
  }

  return FALSE;
}

VOID
WriteMcBank
(
  IN UINT32 McBankIdx,
  IN OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
) 
{
  IA32_MCI_STATUS_MSR_STRUCT McSts;
  UINT32 msrAddr;

  if(isEMCAGen2Cap() == FALSE) {
    return;
  }
  
  if((McSig == NULL) || 
     (McBankIdx >= GetNumMcBanksPerLp())
     ) {
    return;
  }
  
  McSts.Data = McSig->Signature.McSts;

  if(McSts.Bits.val != 1 && (McSts.Bits.other_info & MCA_FIRMWARE_UPDATED_BIT) != 0) {
    msrAddr = GetAddrMcBankSts(McBankIdx);
    AsmWriteMsr64(McBankIdx, McSts.Data);

    if(McSts.Bits.miscv == 1) {
      msrAddr = GetAddrMcBankMisc(McBankIdx);
      AsmWriteMsr64(McBankIdx, McSig->Signature.McMisc);
    }

    if(McSts.Bits.addrv == 1) {
      msrAddr = GetAddrMcBankAddr(McBankIdx);
      AsmWriteMsr64(McBankIdx, McSig->Signature.McAddr);
    }
  }  
}



