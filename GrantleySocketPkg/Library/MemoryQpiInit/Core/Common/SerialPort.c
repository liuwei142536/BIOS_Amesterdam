/**
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
**/
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

//
// -----------------------------------------------------------------------------
//
#include "SysFunc.h"
#include "uart16550.h"


#ifdef SERIAL_DBG_MSG
UINT32
SerialSendBuffer (
                 PSYSHOST host,
                 UINT8    now
                 )
/*++

  Sends a byte to the

  @param host  - Pointer to the system host (root) structure
  @param now   - 0 - wait for delay
                 1 - send immediately

    @retval 0 - Success
    @retval 1 - Failure

--*/
{
  UINT32                    ret;
  UINT32                    i;
  USB2_DEBUG_PORT_INSTANCE  *Instance;
  UINT8                     release;

  ret     = SUCCESS;
  release = 0;

  //
  // host null pointer check
  //
  if (host == NULL) {
    return FAILURE;
  }

  if (host->var.common.serialBufEnable) {

    if ((host->var.common.usbBufCount > USB_BUF_LIMIT) ||
        (GetDelay (host, host->var.common.usbStartCount) > 50000) ||
        now
       ) {
      //
      // Acquire serial comm semaphore
      //
      if (host->var.common.printfSync) {
        release = getPrintFControl (host);
      }

      Instance = &host->var.common.usbDebugPort;
      if (Instance->Ready == TRUE) {
        //
        // Send the USB data
        //
        ret = Usb2DebugPortSend (host, host->var.common.usbBuffer, &host->var.common.usbBufCount);
      } else {
        //
        // Send the Serial port data
        //
        for (i = 0; i < host->var.common.usbBufCount; i++) {

          putchar (host, host->var.common.usbBuffer[i]);
        }
      }

      //
      // Release serial comm semaphore
      //
      if (host->var.common.printfSync && release) {
        releasePrintFControl (host);
      }
      //
      // Reset counters
      //
      host->var.common.usbBufCount    = 0;
      host->var.common.usbStartCount  = GetCount (host);
    }
  }

  return ret;
}

UINT32
SerialBufferChar (
                 PSYSHOST host,
                 INT8     c
                 )
/*++

  Put character into serial buffer

  @param host  - Pointer to the system host (root) structure
  @param c     - character to send

  @retval 0 - Success
  @retval 1 - Failure

--*/
{
  UINT32  ret;

  ret = FAILURE;
  if (!host->var.common.serialBufEnable) {
    putchar (host, c);
    ret = SUCCESS;
  } else {
    //
    // Buffer the data
    //
    if (host->var.common.usbBufCount < USB_BUF_SIZE - 1) {
      host->var.common.usbBuffer[host->var.common.usbBufCount++]  = c;
      ret = SUCCESS;
    }
  }
  return ret;
}

INT8
putchar_buffer (
               PSYSHOST host,
               INT8     c
               )
/*++

  Put character into serial buffer

  @param host  - Pointer to the system host (root) structure
  @param c     - character to send

  @retval 0 - Success
  @retval 1 - Failure

--*/
{
  SerialBufferChar (host, c);
  return c;
}
INT8
putchar (
        PSYSHOST host,
        INT8 c
        )
/*++

  Put character to UART

  @param host  - Pointer to the system host (root) structure
  @param c - character to put to UART

  @retval character placed into UART buffer

--*/
{
  UINT8 UartStatus;

  //
  // Dont attempt COM port accesses if globalComPort is NULL
  // or serial debug messages are disabled
  //
  if (host->setup.common.serialDebugMsgLvl) {
    if (host->var.common.globalComPort) {
      do {
        UartStatus = InPort8_ (host->var.common.globalComPort + LINE_STATUS);
      } while ((UartStatus & TRANS_HOLDING_REG_EMPTY) != TRANS_HOLDING_REG_EMPTY);

      OutPort8_ (host->var.common.globalComPort + TRANSMIT_HOLDING, (UINT8) c);
    }
  }
  return c;
}

INT8
getchar (
        PSYSHOST host
        )
/*++

  Get character from serial buffer

  @param host  - Pointer to the system host (root) structure

  @retval Character received from serial buffer

--*/
{
  INT8  c;

  getchar_timeout (host, &c, NO_TIMEOUT);
  return c;
}

INT8
getchar_echo (
             PSYSHOST host
             )
/*++

  Get character from serial buffer

  @param host  - Pointer to the system host (root) structure

  @retval Character received from serial buffer

--*/
{
  INT8  c;
  c = getchar (host);
  //
  //   if (host->var.common.binMode == 0) {
  //      putchar_buffer(host, c);    // echo char
  //      if (c == '\r') putchar_buffer(host, '\n');
  //      SerialSendBuffer(host, 1);
  //   }
  //
  return c;
}

UINT32
getchar_timeout (
                PSYSHOST host,
                INT8     *c,
                UINT32   usTimeout
                )
/*++

  Get character from serial buffer with a timeout

  @param host      - Pointer to the system host (root) structure
  @param c         - pointer to character received from serial buffer
  @param usTimeout - timeout in microseconds

  @retval 0 - successful
  @retval other - failure

--*/
{
  INT8    tempChar;
  UINT8   UartStatus;
  UINT32  status;

  UINT32  startCount = 0;
  status      = Usb2DebugGetChar (host, &tempChar, usTimeout);
  if (status == EFI_DEVICE_ERROR) {

    if (usTimeout != NO_TIMEOUT) {
      startCount = GetCount (host);
    }

    //
    // Dont attempt COM port accesses if globalComPort is NULL
    //
    if (host->var.common.globalComPort) {
      do {
        UartStatus = InPort8 (host, host->var.common.globalComPort + LINE_STATUS);
        if ((UartStatus & DATA_READY) == DATA_READY) {
          tempChar  = (INT8) InPort8 (host, host->var.common.globalComPort + RECEIVER_BUFFER);
          status    = SUCCESS;
          break;
        }
      } while ((usTimeout == NO_TIMEOUT) || (GetDelay (host, startCount) < usTimeout));
    }
  }

  *c = tempChar;
  return status;
}
#endif
