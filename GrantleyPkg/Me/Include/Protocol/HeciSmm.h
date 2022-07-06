/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/*++

Copyright (c)  2010 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

 HeciSmm.h

Abstract:

 SMM HECI Protocol
 SMM HECI driver is reusing HECI-1 with DXE driver and later with OS driver.
 Thus it may not set SMI interrupt mode in HIDM and may do only simple
 request-response transactions, using unfragmented messages.

--*/
#ifndef _PROTOCOL_HECI_SMM_H_
#define _PROTOCOL_HECI_SMM_H_

#include <Library/HeciCoreLib.h>

// {2031BA7B-6136-4559-A9FE-C88345ED621B}
#define EFI_SMM_HECI_PROTOCOL_GUID  {0x2031BA7B, 0x6136, 0x4559, 0xA9, 0xFE, 0xC8, 0x83, 0x45, 0xED, 0x62, 0x1B}

extern EFI_GUID gSmmHeciProtocolGuid;


typedef struct _EFI_SMM_HECI_PROTOCOL_ EFI_SMM_HECI_PROTOCOL;

/**
 * Initialize HECI interface.
 *
 * This function initializes host side of HECI interface. If timeout is
 * greater than zero it also waits until ME is ready to receive messages.
 *
 * @param[in,out] pThis     Pointer to protocol structure
 * @param[in,out] pTimeout  On input timeout in us, on exit time left
 */
typedef EFI_STATUS (EFIAPI *SMM_HECI_INIT)(
  IN     EFI_SMM_HECI_PROTOCOL *pThis,
  IN OUT UINT32                *pTimeout);

/**
 * Reset HECI queue.
 *
 * This function resets HECI queue. If timeout is greater than zero it also
 * waits until ME is ready to receive messages.
 *
 * @param[in]     pThis     Pointer to protocol data
 * @param[in,out] pTimeout  On input timeout in us, on exit time left
 */
typedef EFI_STATUS (EFIAPI *SMM_HECI_QUE_RESET)(
  IN     EFI_SMM_HECI_PROTOCOL *pThis,
  IN OUT UINT32                *pTimeout);

/**
 * Get HECI queue state.
 *
 * This function reads HECI queue state. It informs whether queue is ready for
 * communication, and whether there are some dwords in send or receive queue.
 * If SmmHeciRequest() is called and queue is not empty reset is done to clear
 * it. SmmHeciQueState() may be used to detect this situation and if possible
 * delay the SMM request, so that OS driver can finish its transaction.
 *
 * @param[in,out] pThis     Pointer to protocol data
 * @param[in,out] pTimeout  On input timeout in us, on exit time left
 */
typedef EFI_STATUS (EFIAPI *SMM_HECI_QUE_STATE)(
  IN OUT EFI_SMM_HECI_PROTOCOL *pThis,
     OUT BOOLEAN               *pIsReady,
     OUT UINT32                *pSendQue,
     OUT UINT32                *pRecvQue);

/**
 * Send request message to HECI queue, wait for response if needed.
 *
 * This function writes one message to HECI queue and - if receive buffer
 * was provided and timeout is greater than zero - waits for response message.
 * Fragmentation is not supported. Reqeust and response must be unfragmented.
 * Size of receive message buffer is given in bytes in (*pBufLen) on input.
 * On exit (*pBufLen) provides the number of bytes written to the message
 * buffer. If buffer is too short the message is truncated.
 *
 * @param[in]     pThis      Pointer to protocol data
 * @param[in,out] pTimeout   On input timeout in ms, on exit time left
 * @param[in]     pReqMsg    Request message
 * @param[out]    pMsgBuf    Buffer for the response message
 * @param[in,out] pBufLen    On input buffer size, on exit message, in bytes
 */
typedef EFI_STATUS (EFIAPI *SMM_HECI_REQUEST)(
  IN     EFI_SMM_HECI_PROTOCOL *pThis,
  IN OUT UINT32                *pTimeout,
  IN     HECI_MSG_HEADER       *pReqMsg,
     OUT HECI_MSG_HEADER       *pRspBuf,
  IN     UINT32                *pBufLen);

/**
 * EFI protocol for HECI communication in SMM.
 *
 * This protocol defines operations for communication with ME using HECI
 * (Host Embeded Controller Interface) in SMM.
 * Note that SMM uses HECI-1 potentially shared with OS driver.
 * SMM request may sometimes break OS driver reqeust.
 */
typedef struct _EFI_SMM_HECI_PROTOCOL_
{
  HECI_DEVICE           HeciDev;
  
  SMM_HECI_INIT         HeciInit;
  SMM_HECI_QUE_RESET    HeciQueReset;
  SMM_HECI_QUE_STATE    HeciQueState;
  SMM_HECI_REQUEST      HeciRequest;
} EFI_SMM_HECI_PROTOCOL;

#endif // _PROTOCOL_HECI_SMM_H_

