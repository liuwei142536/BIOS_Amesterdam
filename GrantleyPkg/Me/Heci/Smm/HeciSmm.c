/*++
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  2008 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  HeciSmm.c

Abstract:

  SMM HECI driver.
  This driver is reusing HECI-1 with DXE driver and later with OS driver.
  Thus it may not set SMI interrupt mode in HIDM and may do only simple
  request-response transactions, using unfragmented messages.

--*/

#include <Library/UefiLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
//AptioV Server Override Start : Adding Libraries
#include <Library\SmmServicesTableLib.h>	
#include <Library\UefiBootServicesTableLib.h>
//AptioV Server Override End : Adding Libraries


#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/HeciCoreLib.h>

#include <Protocol/Smmbase2.h>
#include <Protocol/HeciSmm.h>
#include <Protocol/SmmReadyToLock.h>

#include <Platform.h>
#include <PchRegs/PchRegsRcrb.h>

//#include <Iio/IioRegs.h>

#include "MeState.h"


//AptioV server override start: dynamic MMCFG base change support.
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include <PchSmmAccess.h> //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#else
#include <PchAccess.h>
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include "HeciRegs.h"

BOOLEAN  gHeciSmmExecution; //AptioV Server override: Fix for tpl assert issue with AFUDOS ME update.
//AptioV Server override - Start: Clear Heci2 buffer before handoff to OS
#include "AcpiModeEnable.h"
#include "MeAccess.h"
#include <Library/TimerLib.h>
//AptioV Server override - End: Clear Heci2 buffer before handoff to OS
//#include "..\Dxe\Hecidrv.h"

//HECI_INSTANCE     *mHeciContext;

EFI_STATUS SmmUnsupported ( VOID ) {
  return EFI_UNSUPPORTED;
}
/*****************************************************************************
 * Local function prototypes.
 *****************************************************************************/
EFI_STATUS EFIAPI SmmHeciInit(IN OUT EFI_SMM_HECI_PROTOCOL *pThis,
                              IN OUT UINT32                *pTimeout);
EFI_STATUS EFIAPI SmmHeciQueReset(IN OUT EFI_SMM_HECI_PROTOCOL *pThis,
                                  IN OUT UINT32                *pTimeout);
EFI_STATUS EFIAPI SmmHeciQueState(IN OUT EFI_SMM_HECI_PROTOCOL *pThis,
                                     OUT BOOLEAN               *pIsReady,
                                     OUT UINT32                *pSendQue,
                                     OUT UINT32                *pRecvQue);
EFI_STATUS EFIAPI SmmHeciRequest(IN OUT EFI_SMM_HECI_PROTOCOL *pThis,
                                 IN OUT UINT32                *pTimeout,
                                 IN     HECI_MSG_HEADER       *pReqMsg,
                                    OUT HECI_MSG_HEADER       *pRspBuf,
                                 IN     UINT32                *pBufLen);


//AptioV Server override - Start: Clear Heci2 buffer before handoff to OS
static EFI_GUID gEfiAcpiEnDispatchProtocolGuid = EFI_ACPI_EN_DISPATCH_PROTOCOL_GUID;

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   MEAcpiEnableLink
//
// Description: This routine will be called when ACPI mode is enabled.
//  Pending data in HECI2 buffer is cleared before handoff to the OS.
//
// Parameters:  DispatchHandle - SMM Dispatch Handle
//
// Returns:     None
//
// Modified:
//
// Referrals:
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
MEAcpiEnableLink (
  IN EFI_HANDLE   DispatchHandle
)
{
    UINT64      Heci2Bar;
    UINT32      Data, Length; 
    volatile HECI_ME_CONTROL_REGISTER HeciRegMeCsrHa;

    //
    // Return if HECI2 device is disabled
    //
    if ( Heci2PciRead16(PCI_VENDOR_ID_OFFSET) != 0x8086 ) {
        return;
    }

    // Read MBAR
    Heci2Bar = ((Heci2PciRead32(R_HECIMBAR) & ~0xF) |\
                            (UINT64)Heci2PciRead32(R_HECIMBAR + 4) << 32);

    HeciRegMeCsrHa.ul = Mmio32(Heci2Bar, ME_CSR_HA);
    Length = HeciRegMeCsrHa.r.ME_CBWP_HRA - HeciRegMeCsrHa.r.ME_CBRP_HRA;
    while ( Length!=0 )
    {
        Data = Mmio32(Heci2Bar, ME_CB_RW);
        HeciRegMeCsrHa.ul = Mmio32(Heci2Bar, ME_CSR_HA);
        Length = HeciRegMeCsrHa.r.ME_CBWP_HRA - HeciRegMeCsrHa.r.ME_CBRP_HRA;
        MicroSecondDelay (0x500);
    }

}

EFI_STATUS
EFIAPI
CreateMEAcpiEnLink (
    IN CONST EFI_GUID  *Protocol,
    IN VOID            *Interface,
    IN EFI_HANDLE      Handle )
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      RegisterHandle = NULL;
    EFI_ACPI_DISPATCH_PROTOCOL      *AcpiEnDispatch;

    Status = gSmst->SmmLocateProtocol (
                    &gEfiAcpiEnDispatchProtocolGuid,
                    NULL,
                    (VOID **)&AcpiEnDispatch );
    if ( EFI_ERROR (Status) ) {
        return Status;
    }

    Status = AcpiEnDispatch->Register(
                                AcpiEnDispatch,
                                MEAcpiEnableLink,
                                &RegisterHandle );

    return Status;
}
//AptioV Server override - End: Clear Heci2 buffer before handoff to OS


/* Variables.
 *****************************************************************************/
UINT64                  HeciMBar = 0;
BOOLEAN                 GotSmmReadyToLockEvent = FALSE;

/*****************************************************************************
 * Public functions.
 *****************************************************************************/
/**
 * Function sets HECI MBAR to input value.
 *
 * @param[in]     HeciMBarIn HECI MBAR to set
 * @param[in]     pThis      Pointer to protocol data
 *
 * @retval        NONE
 **/
VOID
SetHeciMbar (
  IN UINT64         HeciMBarIn,
  IN HECI_DEVICE    *pThis
  )
{
  union {
    UINT64   QWord;
    struct {
      UINT32 DWordL;
      UINT32 DWordH;
    } Bits;
  } Mbar;

  if (MmioRead32(pThis->PciCfg + HECI_R_MBAR) == 0xFFFFFFFF) {
    DEBUG((DEBUG_WARN, "[HECI-%d] Warning: SetHeciMbar (): Can't set (device disabled)\n", pThis->Fun));
    return;
  }
  Mbar.QWord = HeciMBarIn;

  // Store HECI MBAR
  DEBUG((DEBUG_INFO, "[HECI-%d] SetHeciMbar (): 0x%08X%08X\n", pThis->Fun, Mbar.Bits.DWordH, Mbar.Bits.DWordL));
  MmioWrite32(pThis->PciCfg + HECI_R_MBAR + 4, Mbar.Bits.DWordH);
  MmioWrite32(pThis->PciCfg + HECI_R_MBAR, Mbar.Bits.DWordL);
} // SetHeciMbar

/**
 * Function prepares HECI MBAR to use in SMM
 * 1. Before ReadyToLock event stores current MBAR in local variable
 * 2. Reads current MBAR to return to caller.
 *    Caller is responsible to restore original MBAR value before SMM exit
 * 3. Set HECI MBAR to safe value known before ReadyToLock event
 *
 * @param[in]     pThis      Pointer to protocol data
 *
 * @retval        HeciMBar   Original HECI MBAR, before preparation for SMM
 *
 **/
UINT64
PrepareHeciMbar (
  IN HECI_DEVICE    *pThis
  )
{
  UINT64 CurrentHeciMBar;

  if (GotSmmReadyToLockEvent == FALSE) {
    HeciMBar = HeciMbarReadFull (pThis, FALSE);
  }
  CurrentHeciMBar = HeciMbarReadFull (pThis, FALSE);
  SetHeciMbar (HeciMBar, pThis);

  return CurrentHeciMBar;
} // PrepareHeciMbar

/**
 * Smm Ready To Lock event notification handler.
 *
 * Update HECI MBAR after PCI enumerator
 *
 * @param[in] Protocol   Points to the protocol's unique identifier.
 * @param[in] Interface  Points to the interface instance.
 * @param[in] Handle     The handle on which the interface was installed.
 *
 * @retval    EFI_SUCCESS   Notification handler runs successfully.
 *
 **/
EFI_STATUS
EFIAPI
SmmReadyToLockEventNotify (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  EFI_STATUS                Status;
  EFI_SMM_HECI_PROTOCOL     *pSmmHeci;
  EFI_SMM_SYSTEM_TABLE2     *pSmst;
  EFI_SMM_BASE2_PROTOCOL    *pSmmBase;

  if (GotSmmReadyToLockEvent == FALSE) {
    GotSmmReadyToLockEvent = TRUE;

    Status = gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, &pSmmBase);
    if (EFI_ERROR(Status)) {
      return EFI_SUCCESS;
    }
    Status = pSmmBase->GetSmstLocation(pSmmBase, &pSmst);
    if (EFI_ERROR(Status)) {
      return EFI_SUCCESS;
    }

    Status = pSmst->SmmLocateProtocol(&gSmmHeciProtocolGuid, NULL, &pSmmHeci); // AptioV server override - gEfiSmmHeciProtocolGuid is named gSmmHeciProtocolGuid in AptioV
    if (!EFI_ERROR (Status)) {
      // Store HECI BAR to verify if not changed upon execution
      HeciMBar = HeciMbarReadFull (&pSmmHeci->HeciDev, FALSE);
    }
  }

  return EFI_SUCCESS;
}

/**
 * SMM HECI driver entry point.
 *
 * param[in] ImageHandle    Handle of driver image
 * param[in] pSysTable      Pointer to the system table
 *
 * return EFI_STATUS is returned.
 */
EFI_STATUS
SmmHeciEntryPoint(
  IN     EFI_HANDLE          ImageHandle,
  IN     EFI_SYSTEM_TABLE   *pSysTable)
{
#if 0
    return EFI_UNSUPPORTED;
#else
  EFI_STATUS                 Status;
  BOOLEAN                    InSmm;
  EFI_SMM_BASE2_PROTOCOL    *pSmmBase;
  EFI_SMM_SYSTEM_TABLE2     *pSmst;
  EFI_SMM_HECI_PROTOCOL     *pSmmHeci;
  EFI_HANDLE                 Handle;
  VOID                      *Registration;

  gHeciSmmExecution = TRUE; //AptioV Server override: Fix for tpl assert issue with AFUDOS ME update.

  Status = gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, &pSmmBase);
  if (EFI_ERROR(Status))
  {
    ASSERT(FALSE);
    return Status;
  }
  //
  // Test if the entry point is running in SMM mode. If not just return.
  // In SMM mode install the HECI SMM service.
  //
  InSmm = FALSE;
  pSmmBase->InSmm(pSmmBase, &InSmm);
  if (!InSmm)
  {
    return EFI_UNSUPPORTED;
  }
  //
  // Create database record and add to database
  //
  Status = pSmmBase->GetSmstLocation(pSmmBase, &pSmst);
  if (EFI_ERROR (Status))
  {
    ASSERT(FALSE);
    return Status;
  }
  Status = pSmst->SmmAllocatePool(EfiRuntimeServicesData, sizeof(*pSmmHeci), &(VOID*)pSmmHeci);
  if (EFI_ERROR(Status))
  {
    ASSERT(FALSE);
    return Status;
  }
  //
  // Initialize SMM HECI protocol data
  //
  pSmmHeci->HeciDev.Bus = HECI1_BUS;
  pSmmHeci->HeciDev.Dev = HECI1_DEV;
  pSmmHeci->HeciDev.Fun = HECI1_FUN;
  pSmmHeci->HeciDev.Hidm = HECI_HIDM_MSI;
  pSmmHeci->HeciDev.Mbar = HECI1_MBAR_DEFAULT;
  pSmmHeci->HeciInit = (SMM_HECI_INIT)SmmHeciInit;
  pSmmHeci->HeciQueReset = (SMM_HECI_QUE_RESET)SmmHeciQueReset;
  pSmmHeci->HeciQueState = (SMM_HECI_QUE_STATE)SmmHeciQueState;
  pSmmHeci->HeciRequest = (SMM_HECI_REQUEST)SmmHeciRequest;
    Handle = NULL;

  //
  // Install the SMM HECI API
  //
  Status = pSmst->SmmInstallProtocolInterface(&Handle, &gSmmHeciProtocolGuid,
                                              EFI_NATIVE_INTERFACE, pSmmHeci); // AptioV server override - gEfiSmmHeciProtocolGuid is named gSmmHeciProtocolGuid in AptioV
  if (EFI_ERROR(Status))
  {
    ASSERT(FALSE);
    pSmst->SmmFreePool(pSmmHeci);
    HeciMBar = 0;
    return Status;
  } else {
    Status = pSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmReadyToLockProtocolGuid,
                    SmmReadyToLockEventNotify,
                    &Registration
                    );
    ASSERT_EFI_ERROR (Status);
  }
  SmmHeciInit(pSmmHeci, NULL);
  DEBUG((EFI_D_ERROR, "[HECI]: SMM driver installed\n"));
  
//AptioV Server override - Start: Clear Heci2 buffer before handoff to OS
  if ( !EFI_ERROR(Status) ) {
      VOID        *AcpiReg;
      EFI_STATUS    Status1;

      Status1 = gSmst->SmmRegisterProtocolNotify (
                      &gEfiAcpiEnDispatchProtocolGuid,
                      CreateMEAcpiEnLink,
                      &AcpiReg
                      );
  }
//AptioV Server override - End: Clear Heci2 buffer before handoff to OS
  return Status;
#endif
}

/**
 * Initialize HECI interface.
 *
 * This function initializes host side of HECI interface. If timeout is
 * greater than zero it also waits until ME is ready to receive messages.
 *
 * @param[in,out] pThis     Pointer to protocol structure
 * @param[in,out] pTimeout  On input timeout in ms, on exit time left
 */
EFI_STATUS EFIAPI SmmHeciInit(
  IN     EFI_SMM_HECI_PROTOCOL *pThis,
  IN OUT UINT32                *pTimeout)
{
  EFI_STATUS  Status;
  UINT32      Fd2;
  
  //
  // Make sure HECI-1 is visible on PCI. It may be hidden for basic ME firmware.
  //
  Fd2 = MmioRead32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2);
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2 & ~B_PCH_RCRB_FD2_MEI1D);
  
  Status = HeciInit(&pThis->HeciDev, pTimeout);
  if ((!EFI_ERROR (Status)) &&
      (GotSmmReadyToLockEvent == FALSE)) {
    HeciMBar = HeciMbarReadFull (&pThis->HeciDev, FALSE);
  }
  //
  // Restore HECI-1 hiding state
  //
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2);
  
  return Status;
} // SmmHeciInit()


/**
 * Reset HECI queue.
 *
 * This function resets HECI queue. If timeout is greater than zero it also
 * waits until ME is ready to receive messages.
 *
 * @param[in,out] pThis     Pointer to protocol data
 * @param[in,out] pTimeout  On input timeout in us, on exit time left
 */
EFI_STATUS EFIAPI SmmHeciQueReset(
  IN     EFI_SMM_HECI_PROTOCOL *pThis,
  IN OUT UINT32                *pTimeout)
{
  EFI_STATUS  Status = EFI_UNSUPPORTED;
  UINT32      Fd2;
  UINT64      CurrentHeciMBar;
  
  CurrentHeciMBar = PrepareHeciMbar (&pThis->HeciDev);
  if (HeciMBar == HeciMbarReadFull (&pThis->HeciDev, FALSE)) {
  //
  // Make sure HECI-1 is visible on PCI. It may be hidden for basic ME firmware.
  //
  Fd2 = MmioRead32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2);
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2 & ~B_PCH_RCRB_FD2_MEI1D);
  
  Status = HeciQueReset(&pThis->HeciDev, pTimeout);
  //
  // Restore HECI-1 hiding state
  //
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2);
  }
  SetHeciMbar (CurrentHeciMBar, &pThis->HeciDev);
  
  return Status;
} // SmmHeciReset()


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
EFI_STATUS EFIAPI SmmHeciQueState(
  IN OUT EFI_SMM_HECI_PROTOCOL *pThis,
     OUT BOOLEAN               *pIsReady,
     OUT UINT32                *pSendQue,
     OUT UINT32                *pRecvQue)
{
  EFI_STATUS  Status = EFI_UNSUPPORTED;
  UINT32      Fd2;
  UINT64      CurrentHeciMBar;
  
  CurrentHeciMBar = PrepareHeciMbar (&pThis->HeciDev);
  if (HeciMBar == HeciMbarReadFull (&pThis->HeciDev, FALSE)) {
  //
  // Make sure HECI-1 is visible on PCI. It may be hidden for basic ME firmware.
  //
  Fd2 = MmioRead32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2);
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2 & ~B_PCH_RCRB_FD2_MEI1D);
  
  Status = HeciQueState(&pThis->HeciDev, pIsReady, pSendQue, pRecvQue);
  //
  // Restore HECI-1 hiding state
  //
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2);
  }
  SetHeciMbar (CurrentHeciMBar, &pThis->HeciDev);
  
  return Status;
} // SmmHeciQueState()


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
EFI_STATUS EFIAPI SmmHeciRequest(
  IN     EFI_SMM_HECI_PROTOCOL *pThis,
  IN OUT UINT32                *pTimeout,
  IN     HECI_MSG_HEADER       *pReqMsg,
     OUT HECI_MSG_HEADER       *pRspBuf,
  IN     UINT32                *pBufLen)
{
  EFI_STATUS  Status = EFI_UNSUPPORTED;
  UINT32      Fd2;
  BOOLEAN     IntEnabled, IsReady;
  UINT32      SendQue, RecvQue;
  UINT64      CurrentHeciMBar;
  
  CurrentHeciMBar = PrepareHeciMbar (&pThis->HeciDev);
  if (HeciMBar == HeciMbarReadFull (&pThis->HeciDev, FALSE)) {
  //
  // Make sure HECI-1 is visible on PCI. It may be hidden for basic ME firmware.
  //
  Fd2 = MmioRead32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2);
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2 & ~B_PCH_RCRB_FD2_MEI1D);
  //
  // Save current interrupt enable state and disable interrupts
  //
  Status = HeciIntState(&pThis->HeciDev, &IntEnabled, NULL);
  if (EFI_ERROR(Status))
  {
    goto GetOut;
  }
  HeciIntEnable(&pThis->HeciDev, 0);
  //
  // If there is some date in the queue clear it by reseting the queue
  //
  HeciQueState(&pThis->HeciDev, &IsReady, &SendQue, &RecvQue);
  if (IsReady && (SendQue != 0 || RecvQue != 0))
  {
    Status = HeciQueReset(&pThis->HeciDev, pTimeout);
    if (EFI_ERROR(Status))
    {
      goto GetOut1;
    }
  }
  //
  // Send the request and wait for response if response expected
  //
  Status = HeciMsgSend(&pThis->HeciDev, pTimeout, pReqMsg);
  if (!EFI_ERROR(Status))
  {
    if (pRspBuf != NULL)
    {
      Status = HeciMsgRecv(&pThis->HeciDev, pTimeout, pRspBuf, pBufLen);
    }
  }
    HeciQueReset (&pThis->HeciDev, pTimeout);
 GetOut1:
  //
  // Restore interrupt enable state
  //
  HeciIntEnable(&pThis->HeciDev, IntEnabled);
 GetOut:
  //
  // Restore HECI-1 hiding state
  //
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, Fd2);
  }
  SetHeciMbar (CurrentHeciMBar, &pThis->HeciDev);
  
  return Status;
} // SmmHeciRequest()

