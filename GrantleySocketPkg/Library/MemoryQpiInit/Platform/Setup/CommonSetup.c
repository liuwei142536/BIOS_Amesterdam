/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*************************************************************************
 *
 * Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 *
 ************************************************************************/

#include "SysFunc.h"

/**

  Platform specific input setup

  @param setup - pointer to setup area of host structure

  @retval N/A

**/
void
GetSetupOptionsEx(
  PSYSHOST host
  )
{
  struct sysSetup *setup;
  setup = (struct sysSetup *) &host->setup;
  //
  // Call platform specific setup routines
  //
  GetSetupOptions (setup);
  CoreGetCpuSetupOptions (setup);
  GetQpiSetupOptions (setup);
  GetMemSetupOptionsCore (host);
}

void
GetSetupOptions (
  struct sysSetup *setup
  )
/*++

  Gets common item setup options

  @param setup - pointer to setup area of host structure

  @retval VOID

--*/
{

   //    PROMOTE_WARN_EN
   setup->common.options = PROMOTE_WARN_EN | PROMOTE_MRC_WARN_EN | HALT_ON_ERROR_EN;

   setup->common.debugPort = DEFAULT_COM_PORT;
   setup->common.mmCfgBase = MMCFG_BASE_ADDRESS;
   setup->common.mmCfgSize = MMCFG_SIZE;
   setup->common.mmiolBase = setup->common.mmCfgBase + setup->common.mmCfgSize;
   setup->common.mmiolSize = MMIOL_LIMIT - setup->common.mmiolBase + 1;
   setup->common.mmiohBase = MMIOH_BASE;
   setup->common.mmiohSize = MMIOH_SIZE;
   setup->common.lowGap = FOUR_GB_MEM - (setup->common.mmCfgBase >> 26);
   setup->common.highGap = HIGH_GAP;            // High MMIO gap = 256 MB
   setup->common.maxAddrMem = MAX_MEM_ADDR;     // Supports 46-bit addressing

   setup->common.numaEn = 1;
//#ifdef HW_EMULATION
   setup->common.isocEn = 0;
//#else
   setup->common.dcaEn  = 1;
//#endif
   setup->common.PSMIEnabledSupport = 0;
   setup->common.socketType = SOCKET_EP;     // Socket R or B2
#ifdef  BDAT_SUPPORT
   setup->common.bdatEn = 0;
#endif //BDAT_SUPPORT

   setup->common.sysHostBufferPtr = 0;      // zero:  Use default memory buffer address at 1MB
                                            // non-zero : OEM-specified 32-bit buffer address
#ifdef   SERIAL_DBG_MSG
   setup->common.usbDebug.Enable = 0x00;    // Force UsbDebug port disabled for now
//*** AMI PORTING BEGIN ***//
// AptioV override Grangeville: Adding COM1(0x2f8) support

//###DEBUG   setup->common.consoleComPort = 0x3f8;
   setup->common.consoleComPort = (UINT16) PcdGet64 (PcdSerialRegisterBase);

//*** AMI PORTING END ***//
   setup->common.serialDebugMsgLvl = SDBG_MAX;// + SDBG_TRACE; // SDBG_MAX, or BIT1 for maximum messages
#endif

#ifdef USB_DEBUGPORT_FLAG
   setup->common.usbDebug.Enable = 0x01;   // Force enable UsbDebug port
#endif
}
