/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**
Copyright (c) 2016 - 2021 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  @file SpsSmm.c

  SPS SMM driver.
**/


//
// Standard header files.
//
#include <Base.h>
#include <Uefi.h>
#include <Sps.h>
#include <MeAccess.h>


#include <Library/UefiLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/HobLib.h>
#include <Library/HeciCoreLib.h>
#include <Library/SmmServicesTableLib.h>

#include <Guid/HobList.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Guid/MeFwModesGuid.h>
#include <Guid/SetupVariable.h>
#include <Guid/SpsInfoHobGuid.h>  

//
// Protocols
//
#include <Protocol/SpsSmmHmrfpoProtocol.h>
#include <Protocol/Smmbase2.h>
#include <Protocol/HeciSmm.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/HeciProtocol.h>

//
// Driver specific headers.
//
#include "SpsSmm.h"


// global to store Nonce value read by ME HMRFPO_LOCK request
static UINT64 gNonceValue = 0;

// timeout value (in ms) for HECI messages
#define HECI_MSG_TIMEOUT    500


#if ME_TESTMENU_FLAG

//global to store disable sending HmrfpoLock message to ME
static UINT8  gHmrfpoLockEnabled = TRUE;

#endif // ME_TESTMENU_FLAG


/**
  Reads HECI MBar for given HECI function

  @param[in]     HECI          Heci Number to read MBar

  @return HECI memory bar, on error returns 0
**/
UINTN
ReadHeciMBar (
  IN     ME_DEVICE      HeciNumber
  )
{
  UINT8    HeciFunction;
  union {
    UINT64   QWord;
    struct {
      UINT32 DWordL;
      UINT32 DWordH;
    } Bits;
  } Mbar;

  switch (HeciNumber) {
    case HECI1:
      HeciFunction = HECI_FUNCTION_NUMBER;
      break;
    case HECI2:
      HeciFunction = HECI2_FUNCTION_NUMBER;
      break;
    default:
      ASSERT (FALSE);
      return 0;
  }

  Mbar.QWord = 0;
  Mbar.Bits.DWordL = MmioRead32 (
                       MmPciAddress (
                         0,
                         ME_BUS,
                         ME_DEVICE_NUMBER,
                         HeciFunction,
                         HECI_R_MBAR
                         )
                       );


   if (Mbar.Bits.DWordL == 0xFFFFFFFF) {
    return 0;
  }

  Mbar.Bits.DWordL &= 0xFFFFFFF0; // clear address type bits
  Mbar.Bits.DWordH = MmioRead32 (
                       MmPciAddress (
                         0,
                         ME_BUS,
                         ME_DEVICE_NUMBER,
                         HeciFunction,
                         HECI_R_MBAR + 4
                         )
                       );

  return (UINTN) Mbar.QWord;
}

/**
  Clear HECI Buffer
  HECI1_DEVICE ... HECI3_DEVICE

  @param[in]     HECI          Heci Number to clear
**/
VOID
HeciBufferClear (
  IN     ME_DEVICE      HeciToClear
  )
{
  UINT8                               i;
  UINTN                               HeciMBar;
  volatile HECI_HOST_CONTROL_REGISTER *HeciRegHCsr;
  UINT32                              CBLength;

  HeciMBar = ReadHeciMBar (HeciToClear);
  if (HeciMBar == 0) {
    return;
  }

  HeciRegHCsr = (HECI_HOST_CONTROL_REGISTER*) (HeciMBar + H_CSR);
  CBLength = HeciRegHCsr->r.H_CBD; // Grab Circular Buffer length
  HeciRegHCsr = (HECI_HOST_CONTROL_REGISTER*) (HeciMBar + H_CB_WW);
  DEBUG ((DEBUG_ERROR, "%a (): HeciRegHCsr (0x%x) ->  0x%x\n", HeciRegHCsr, HeciRegHCsr->ul));
  for (i = 0; i < CBLength; i++) {
    HeciRegHCsr->ul = 0;
  }
}

/** SPS SMM driver entry point.

  @param[in] ImageHandle    Handle of driver image
  @param[in] pSysTable      Pointer to the system table

  @return EFI_STATUS is returned.
 */
EFI_STATUS
SpsSmmEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *pSysTable
  )
{

// ME is not simulated in WBG simics
#if !defined(SIMICS_BIOS_FLAG)

  EFI_STATUS             Status;
  EFI_SMM_HECI_PROTOCOL  *pSmmHeci;
  EFI_SMM_BASE2_PROTOCOL *pSmmBase;
  EFI_SMM_SYSTEM_TABLE2  *pSmmSt;
  EFI_HANDLE             Handle;
  EFI_HOB_TYPE_SPS_INFO  HobInfo;
  SPS_FEATURE_SET        FeatureSet;
  BOOLEAN                InSmm;
  UINT64                 Nonce;
  UINT32                 Timeout;

#if ME_TESTMENU_FLAG
  UINTN                       SysCfgSize;
  SYSTEM_CONFIGURATION        SysCfg;
  EFI_SMM_VARIABLE_PROTOCOL   *pSmmVariable;

#endif //ME_TESTMENU_FLAG

  SPS_SMM_HMRFPO_PROTOCOL       *pSpsSmmHmrfpo;
  
  // AptioV Server Override Start: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions
  Status = InitAmiSmmLib (ImageHandle, pSysTable);
  // AptioV Server Override End: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions

  DEBUG((EFI_D_INFO, "[SPS_SMM] SpsSmmEntryPoint started.\n"));

  Status = gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, &pSmmBase);
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Can't locate gEfiSmmBase2Protocol (%r).\n", Status));
    return Status;
  }

  // Test if the entry point is running in SMM mode. If not just return.
  InSmm = FALSE;
  pSmmBase->InSmm(pSmmBase, &InSmm);
  if (!InSmm) {
    return EFI_UNSUPPORTED;
  }

  // Locate SPS HOB
  Status = GetSpsConfiguration(pSysTable, &HobInfo);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Can't get SPS configuration.\n"));
    return EFI_UNSUPPORTED;
  } else {
    FeatureSet.Data = HobInfo.FeatureSet;
  }

  // Find HECI SMM protocol
  pSmmHeci = NULL;
  Status = gSmst->SmmLocateProtocol(&gSmmHeciProtocolGuid, NULL, &pSmmHeci);
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HECI SMM protocol not found (%r)\n", Status));
    return Status;
  }

  //prepare SpsSmmHmrfpo Protocol
  Status = pSmmBase->GetSmstLocation(pSmmBase, &pSmmSt);
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Can't get SmmTables!. Status=%r\n", Status));
    return Status;
  }

  Status = pSmmSt->SmmAllocatePool( EfiRuntimeServicesData, sizeof(*pSpsSmmHmrfpo), &(VOID *) pSpsSmmHmrfpo );
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Can't allocate memory for SpsSmmHmrfpo protocol!. Status=%r\n", Status));
    return Status;
  }

  //init ME versions
  SetMeVersions(pSmmHeci, pSpsSmmHmrfpo);


#if ME_TESTMENU_FLAG

  //First set the default values
  SysCfg.MeHmrfpoLockEnabled   = TRUE;
  SysCfg.MeHmrfpoEnableEnabled = FALSE;

  //Read setup configuration
  // AptioV Server Override Start: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions
  // Status = gBS->LocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, &pSmmVariable);
  // if (EFI_ERROR(Status)) {
  //   DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Can't locate protocol SmmVariableProtocol (%r).\n", Status));
  // } else {

    // Read SPS variables from configuration in setup menu
    SysCfgSize = sizeof(SysCfg);
    // AptioV Server Override Start: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions
    // Status = pSmmVariable->SmmGetVariable (
    Status = pRS->GetVariable(
                    L"IntelSetup",
                    &gEfiSetupVariableGuid,
                    NULL,
                    &SysCfgSize,
                    &SysCfg
                    );
  // }
  // AptioV Server Override End: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions
  
  if (EFI_ERROR(Status)) {
      // Setup configuration can't be read - use default values
      DEBUG((EFI_D_ERROR, "[SPS_SMM] Warning: Can't read SPS configuration variables from setup menu (%r). Defaults are in use.\n", Status));
  }

  // Send HmrfpoLock to get Nonce value
  if (SysCfg.MeHmrfpoLockEnabled) {

    gHmrfpoLockEnabled = TRUE;

#endif // ME_TESTMENU_FLAG

    HeciReq_HmrfpoLock(pSmmHeci, &Nonce);
    if (Nonce != 0) {
      gNonceValue = Nonce;
      DEBUG((EFI_D_INFO, "[SPS_SMM] Nonce saved.\n"));
    } else {
      DEBUG((EFI_D_ERROR, "[SPS SMM] ERROR: Can't get nonce!\n"));
      gNonceValue = 0;
    }

#if ME_TESTMENU_FLAG

    // send HECI enable if needed
    if (SysCfg.MeHmrfpoEnableEnabled) {
      DEBUG((EFI_D_INFO, "[SPS_SMM] HmrfpoEnable needs to be send after HmrfpoLock.\n"));
      HeciReq_HmrfpoEnable(pSmmHeci, gNonceValue);
    }

  } else {
    // if MeHmrfpoLockEnabled is set to FALSE in setup don't send any HMRFPO_LOCK message - ME updated tool needs to get nonce...
    gHmrfpoLockEnabled = FALSE;
    gNonceValue = 0;
    DEBUG((EFI_D_INFO, "[SPS_SMM] HMRFPO_LOCK disabled in setup.\n"));
  }

#endif // ME_TESTMENU_FLAG

  HeciBufferClear (HECI1);
  Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  pSmmHeci->HeciQueReset (pSmmHeci, &Timeout);
  Nonce = 0;

  //install SpsSmmHmrfpo Protocol
  pSpsSmmHmrfpo->Lock      = (HMRFPO_LOCK)               Hmrfpo_Lock;
  pSpsSmmHmrfpo->Enable    = (HMRFPO_ENABLE)             Hmrfpo_Enable;
  pSpsSmmHmrfpo->MeReset   = (HMRFPO_MERESET)            Hmrfpo_MeReset;

  Handle = NULL;
  Status = pSmmSt->SmmInstallProtocolInterface (
                    &Handle,
                    &gSpsSmmHmrfpoProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    pSpsSmmHmrfpo
                    );

  ASSERT_EFI_ERROR (Status);

  DEBUG((EFI_D_INFO, "[SPS_SMM] SpsSmmEntryPoint ends.\n"));

#endif // !SIMICS_BIOS_FLAG

  return EFI_SUCCESS;
} // SpsSmmEntryPoint


/** Procedure locates and return SPS HOB data

  @param[in]  *SystemTable  Pointer of EFI Sysytem Table
  @param[out] *pHob         Buffer for SPS HOB data

  @return EFI_STATUS is returned.

 */
EFI_STATUS
GetSpsConfiguration (
  IN EFI_SYSTEM_TABLE       *SystemTable,
  OUT EFI_HOB_TYPE_SPS_INFO *pHob
  )
{
  VOID                  *HobList;
  EFI_GUID              TempGUID;
  EFI_STATUS            Status;
  EFI_HOB_GUID_TYPE     *GuidHob;
  EFI_HOB_TYPE_SPS_INFO *pInfoData;
  UINT8                 Index;

  DEBUG((EFI_D_INFO, "[SPS_SMM] Getting Info from PEI\n"));

  Status = EFI_UNSUPPORTED;
  for (Index = 0; Index < SystemTable->NumberOfTableEntries; Index++) {
    CopyMem (&TempGUID,
      &(SystemTable->ConfigurationTable[Index].VendorGuid),
      sizeof (EFI_GUID)
      );

    if (CompareGuid (&gEfiHobListGuid, &TempGUID)) {
      HobList = SystemTable->ConfigurationTable[Index].VendorTable;
      Status  = EFI_SUCCESS;
      break;
    }
  }
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Cannot locate any HOB\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "[SPS_SMM] Looking for SPS HOB info from PEI\n"));

  GuidHob = GetFirstGuidHob (&gEfiSpsInfoHobGuid);
  pInfoData = GET_GUID_HOB_DATA (GuidHob);
  if(GuidHob == NULL){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Cannot locate SPS HOB\n"));
    return EFI_UNSUPPORTED;
  }

  CopyMem(pHob, pInfoData, sizeof(*pHob));

  if (pInfoData->WorkFlow != ME_FUNCTIONAL) {
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: ME non functional.\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/** Procedure to send HMRFPO_LOCK HECI Request

    @param[in]    *pSmmHeci - pointer to SMM HECI driver
    @param[in,out] Nonce - buffer to store nonce value from HECI message response
    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_HmrfpoLock (
  IN     EFI_SMM_HECI_PROTOCOL *pSmmHeci,
  IN OUT UINT64                *Nonce
  )
{
  UINT32     Timeout;
  UINT32     BufLen;
  EFI_STATUS Status;
  UINT32     Retries;

  HECI_MSG_HMRFPO_LOCK_REQ  HeciMsgReq;
  HECI_MSG_HMRFPO_LOCK_RSP  HeciMsgRsp;


  DEBUG((EFI_D_INFO, "[SPS_SMM] Sending HMRFPO_LOCK, timeout %dms, expect timeout, "
         "repeat up to 20 tries till success\n", HECI_MSG_TIMEOUT));

  DEBUG((EFI_D_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciReadMefs1()));

  for (Retries = 20; Retries--; ) {

    // Construct HECI HMRFPO_LOCK request
    MemSet(&HeciMsgReq, 0, sizeof(HECI_MSG_HMRFPO_LOCK_REQ)); // AptioV override - use MemSet from AmiLib.h
    HeciMsgReq.Heci.Bits.MeAddress = SPS_CLIENTID_ME_MKHI;
    HeciMsgReq.Heci.Bits.HostAddress = SPS_CLIENTID_BIOS;
    HeciMsgReq.Heci.Bits.Length =
        sizeof(HeciMsgReq) - sizeof(HeciMsgReq.Heci);
    HeciMsgReq.Heci.Bits.MsgComplete = 1;
    HeciMsgReq.Mkhi.Data = HECI_MSG_LOCK_REQ_MKHI_HDR;

    BufLen = sizeof(HeciMsgRsp);
    Timeout = HECI_MSG_TIMEOUT;
    Status = pSmmHeci->HeciRequest(
                       pSmmHeci,
                       &Timeout,
                       &HeciMsgReq.Heci,
                       &HeciMsgRsp.Heci,
                       &BufLen
                       );

    DEBUG((EFI_D_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciReadMefs1()));

    if (!EFI_ERROR(Status)) {

      if (HeciMsgRsp.Mkhi.Data != HECI_MSG_LOCK_RSP_MKHI_HDR) {
        DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Invalid HMRFPO_LOCK response (MKHI: 0x%08X), "
              "%d tries left\n",
              HeciMsgRsp.Mkhi.Data, Retries));
        continue;
      }
      if (HeciMsgRsp.Status != 0) {
        DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HMRFPO_LOCK failed (cause: %d)\n",
             HeciMsgRsp.Status));
      } else {
        DEBUG((EFI_D_INFO, "[SPS_SMM] HMRFPO_LOCK reports success\n"));
        if(Nonce != NULL)
          *Nonce = HeciMsgRsp.Nonce; //save the nonce value
      }
      break;
    }
    DEBUG((EFI_D_ERROR, "[SPS_SMM] HMRFPO_LOCK send failed (%r), %d tries left\n", Status, Retries));

  } //for

  HeciMsgRsp.Nonce = 0;

  return EFI_SUCCESS;
}

/** Procedure to send HMRFPO_ENABLE HECI Request

    @param[in]    *pSmmHeci pointer to SMM HECI driver
    @param[in]    *Nonce    Nonce value for current session

    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_HmrfpoEnable (
  IN EFI_SMM_HECI_PROTOCOL *pSmmHeci,
  IN UINT64                Nonce
  )
{
  UINT32     Timeout;
  UINT32     BufLen;
  EFI_STATUS Status;
  UINT32     Retries;

  HECI_MSG_HMRFPO_ENABLE_REQ HeciMsgReq;
  HECI_MSG_HMRFPO_ENABLE_RSP HeciMsgRsp;


  DEBUG((EFI_D_INFO, "[SPS_SMM] Sending HMRFPO_ENABLE, timeout %dms, expect timeout, "
         "repeat up to 20 tries till success\n", HECI_MSG_TIMEOUT));

  DEBUG((EFI_D_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciReadMefs1()));

  for (Retries = 20; Retries--; ) {

    // Construct HMRFPO_ENABLE request message
    MemSet(&HeciMsgReq, 0, sizeof(HECI_MSG_HMRFPO_ENABLE_REQ)); // AptioV override - use MemSet from AmiLib.h
    HeciMsgReq.Heci.Bits.MeAddress = SPS_CLIENTID_ME_MKHI;
    HeciMsgReq.Heci.Bits.HostAddress = SPS_CLIENTID_BIOS;
    HeciMsgReq.Heci.Bits.Length =
            sizeof(HeciMsgReq) - sizeof(HeciMsgReq.Heci);
    HeciMsgReq.Heci.Bits.MsgComplete = 1;
    HeciMsgReq.Mkhi.Data = HECI_MSG_ENABLE_REQ_MKHI_HDR;
    HeciMsgReq.Nonce = Nonce;

    BufLen = sizeof(HeciMsgRsp);
    Timeout = HECI_MSG_TIMEOUT;
    Status = pSmmHeci->HeciRequest(
                pSmmHeci,
                &Timeout,
                &HeciMsgReq.Heci,
                &HeciMsgRsp.Heci,
                &BufLen
                );

    DEBUG((EFI_D_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciReadMefs1()));

    if (!EFI_ERROR(Status)) {

      if (HeciMsgRsp.Mkhi.Data != HECI_MSG_ENABLE_RSP_MKHI_HDR) {
        DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Invalid HMRFPO_ENABLE response (MKHI: 0x%08X), "
               "%d tries left\n",
               HeciMsgRsp.Mkhi.Data, Retries));
        continue;
      }
      if (HeciMsgRsp.Status != 0) {
        DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HMRFPO_ENABLE refused (cause: %d)\n",
               HeciMsgRsp.Status));
      } else {
        DEBUG((EFI_D_INFO, "[SPS_SMM] HMRFPO_ENABLE reports success.\n"));
      }
      break;
    }
    DEBUG((EFI_D_ERROR, "[SPS_SMM] HMRFPO_ENABLE send failed (%r), %d tries left\n", Status, Retries));

  } //for

  Nonce = 0;
  HeciMsgReq.Nonce = 0;

  return EFI_SUCCESS;
}

/** Procedure to send HMRFPO_ENABLE HECI Request

    @param[in]    *pSmmHeci pointer to SMM HECI driver
    @param[in]    *Nonce    Nonce value for current session

    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_HmrfpoMeReset (
  IN EFI_SMM_HECI_PROTOCOL *pSmmHeci,
  IN UINT64                Nonce
  )
{
  UINT32     Timeout;
  UINT32     BufLen;
  EFI_STATUS Status;
  UINT32     Retries;

  HECI_MSG_HMRFPO_MERESET_REQ HeciMsgReq;
  HECI_MSG_HMRFPO_MERESET_RSP HeciMsgRsp;


  DEBUG((EFI_D_INFO, "[SPS_SMM] Sending HMRFPO_MERESET, timeout %dms, expect timeout, "
         "repeat up to 20 tries till success\n", HECI_MSG_TIMEOUT));

  DEBUG((EFI_D_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciReadMefs1()));

  for (Retries = 20; Retries--; ) {

    // Construct HMRFPO_MERESET request message
    MemSet(&HeciMsgReq, 0, sizeof(HECI_MSG_HMRFPO_MERESET_REQ)); // AptioV override - use MemSet from AmiLib.h
    HeciMsgReq.Heci.Bits.MeAddress = SPS_CLIENTID_ME_MKHI;
    HeciMsgReq.Heci.Bits.HostAddress = SPS_CLIENTID_BIOS;
    HeciMsgReq.Heci.Bits.Length =
            sizeof(HeciMsgReq) - sizeof(HeciMsgReq.Heci);
    HeciMsgReq.Heci.Bits.MsgComplete = 1;
    HeciMsgReq.Mkhi.Data = HECI_MSG_MERESET_REQ_MKHI_HDR;
    HeciMsgReq.Nonce = Nonce;

    BufLen = sizeof(HeciMsgRsp);
    Timeout = HECI_MSG_TIMEOUT;
    Status = pSmmHeci->HeciRequest(
                pSmmHeci,
                &Timeout,
                &HeciMsgReq.Heci,
                &HeciMsgRsp.Heci,
                &BufLen
                );

    DEBUG((EFI_D_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciReadMefs1()));

    if (!EFI_ERROR(Status)) {

      if (HeciMsgRsp.Mkhi.Data != HECI_MSG_MERESET_RSP_MKHI_HDR) {
        DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Invalid HMRFPO_MERESET response (MKHI: 0x%08X), "
               "%d tries left\n",
               HeciMsgRsp.Mkhi.Data, Retries));
        continue;
      }
      if (HeciMsgRsp.Status != 0) {
        DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HMRFPO_MERESET refused (cause: %d)\n",
               HeciMsgRsp.Status));
      } else {
        DEBUG((EFI_D_INFO, "[SPS_SMM] HMRFPO_MERESET reports success.\n"));
      }
      break;
    }
    DEBUG((EFI_D_ERROR, "[SPS_SMM] HMRFPO_MERESET send failed (%r), %d tries left\n", Status, Retries));

  } //for

  Nonce = 0;
  HeciMsgReq.Nonce = 0;

  return EFI_SUCCESS;
}


/**
  Function which implements SpsSmmHmrfpo Protocol Lock function

  @return EFI_STATUS is returned.
*/
EFI_STATUS
Hmrfpo_Lock (void)
{
  EFI_SMM_HECI_PROTOCOL *pSmmHeci;
  EFI_STATUS            Status;
  UINT32                Timeout;

#if ME_TESTMENU_FLAG
  if(!gHmrfpoLockEnabled){
    DEBUG((EFI_D_INFO, "[SPS_SMM] Sending Hmrfpo_Lock message is disabled in setup.\n"));
    return EFI_UNSUPPORTED;
  }
#endif // ME_TESTMENU_FLAG

  Status = gSmst->SmmLocateProtocol(&gSmmHeciProtocolGuid, NULL, &pSmmHeci); // AptioV server override - gEfiSmmHeciProtocolGuid is named gSmmHeciProtocolGuid in AptioV
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HECI SMM protocol not found (%r)\n", Status));
    return Status;
  }

  Status = HeciReq_HmrfpoLock( pSmmHeci, NULL );

  HeciBufferClear (HECI1);
  Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  pSmmHeci->HeciQueReset (pSmmHeci, &Timeout);

  if (EFI_ERROR(Status) ) {
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HeciReq_HmrfpoLock command returns error (%r).\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Function which implements SpsSmmHmrfpo Protocol MeReset function

  @return EFI_STATUS is returned.
*/
EFI_STATUS
Hmrfpo_MeReset (void)
{
  EFI_SMM_HECI_PROTOCOL *pSmmHeci;
  EFI_STATUS            Status;
  UINT32                Timeout;

  if( gNonceValue == 0){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Nonce value unknown. Operation not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gSmst->SmmLocateProtocol(&gSmmHeciProtocolGuid, NULL, &pSmmHeci); // AptioV server override - gEfiSmmHeciProtocolGuid is named gSmmHeciProtocolGuid in AptioV
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HECI SMM protocol not found (%r)\n", Status));
    return Status;
  }

  Status = HeciReq_HmrfpoMeReset ( pSmmHeci, gNonceValue);

  HeciBufferClear (HECI1);
  Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  pSmmHeci->HeciQueReset (pSmmHeci, &Timeout);

  if (EFI_ERROR(Status) ) {
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HeciReq_HmrfpoMeReset command returns error (%r).\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Function which implements SpsSmmHmrfpo Protocol Enable function

  @return EFI_STATUS is returned.
*/
EFI_STATUS
Hmrfpo_Enable (void)
{
  EFI_SMM_HECI_PROTOCOL *pSmmHeci;
  EFI_STATUS            Status;
  UINT32                Timeout;

  if( gNonceValue == 0){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: Nonce value unknown. Operation not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gSmst->SmmLocateProtocol(&gSmmHeciProtocolGuid, NULL, &pSmmHeci); // AptioV server override - gEfiSmmHeciProtocolGuid is named gSmmHeciProtocolGuid in AptioV
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HECI SMM protocol not found (%r)\n", Status));
    return Status;
  }

  Status = HeciReq_HmrfpoEnable ( pSmmHeci, gNonceValue);

  HeciBufferClear (HECI1);
  Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  pSmmHeci->HeciQueReset (pSmmHeci, &Timeout);

  if (EFI_ERROR(Status) ) {
    DEBUG((EFI_D_ERROR, "[SPS_SMM] ERROR: HeciReq_HmrfpoEnable command returns error (%r).\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}


/** Function set the ME versions red by MKHI messages

  @param[in,out]  pSmsSmmHmrfpo -
  @return         EFI_STATUS
*/
void SetMeVersions(
  IN     EFI_SMM_HECI_PROTOCOL   *pSmmHeci,
  IN OUT SPS_SMM_HMRFPO_PROTOCOL *pSpsSmmHmrfpo
  )
{
  EFI_STATUS Status;
  UINT32     Timeout;
  UINT32     BufLen;

  HECI_MSG_MKHI_GET_FW_VERSION_REQ HeciMsgReq;
  HECI_MSG_MKHI_GET_FW_VERSION_RSP HeciMsgRsp;

  //
  // Send MKHI_GET_FW_VERSION request to ME
  //
  DEBUG((EFI_D_INFO, "[SPS_SMM] Sending MKHI_GET_FW_VERSION, timeout %dms, expect success\n", HECI_MSG_TIMEOUT));

  // Construct MKHI_GET_FW_VERSION request
  MemSet(&HeciMsgReq, 0, sizeof(HECI_MSG_MKHI_GET_FW_VERSION_REQ)); // AptioV override - use MemSet from AmiLib.h
  HeciMsgReq.Heci.Bits.MeAddress = SPS_CLIENTID_ME_MKHI;
  HeciMsgReq.Heci.Bits.HostAddress = SPS_CLIENTID_BIOS;
  HeciMsgReq.Heci.Bits.Length =
      sizeof(HeciMsgReq) - sizeof(HeciMsgReq.Heci);
  HeciMsgReq.Heci.Bits.MsgComplete = 1;

  HeciMsgReq.Mkhi.Data = HECI_MSG_GET_FW_VERSION_REQ_MKHI_HDR;
  BufLen = sizeof(HeciMsgRsp);
  Timeout = HECI_MSG_TIMEOUT;

  Status = pSmmHeci->HeciRequest(
                       pSmmHeci,
                       &Timeout,
                       &HeciMsgReq.Heci,
                       &HeciMsgRsp.Heci,
                       &BufLen
                       );

  DEBUG((EFI_D_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciReadMefs1()));

  if (EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR, "[ME] ERROR: Cannot send MKHI_GET_FW_VERSION (status: %r)\n",Status));

  } else if ( HeciMsgRsp.Mkhi.Data != HECI_MSG_GET_FW_VERSION_RSP_MKHI_HDR ) {
    DEBUG((EFI_D_ERROR, "[ME] ERROR: Invalid MKHI_GET_FW_VERSION response (MKHI: 0x%X)\n",
              HeciMsgRsp.Mkhi.Data));

  } else {
    // no errors - copy data from HECI message

    // Active operational firmware
    pSpsSmmHmrfpo->ActiveFwVersion.Minor  = HeciMsgRsp.Act.Minor;
    pSpsSmmHmrfpo->ActiveFwVersion.Major  = HeciMsgRsp.Act.Major;
    pSpsSmmHmrfpo->ActiveFwVersion.ProdId = HeciMsgRsp.Act.ProdId;
    pSpsSmmHmrfpo->ActiveFwVersion.Build  = HeciMsgRsp.Act.Build;
    pSpsSmmHmrfpo->ActiveFwVersion.Patch  = HeciMsgRsp.Act.Patch;

    // Recovery firmware
    pSpsSmmHmrfpo->RecoveryFwVersion.Minor  = HeciMsgRsp.Rcv.Minor;
    pSpsSmmHmrfpo->RecoveryFwVersion.Major  = HeciMsgRsp.Rcv.Major;
    pSpsSmmHmrfpo->RecoveryFwVersion.ProdId = HeciMsgRsp.Rcv.ProdId;
    pSpsSmmHmrfpo->RecoveryFwVersion.Build  = HeciMsgRsp.Rcv.Build;
    pSpsSmmHmrfpo->RecoveryFwVersion.Patch  = HeciMsgRsp.Rcv.Patch;

    //
    // ME in dual-image configuration provides the version of the backup image yet
    //
    if (BufLen > sizeof(HeciMsgRsp) - sizeof(HeciMsgRsp.Bkp) ){

      // Backup operational firmware (optional)
      pSpsSmmHmrfpo->BackupFwVersion.Minor  = HeciMsgRsp.Bkp.Minor;
      pSpsSmmHmrfpo->BackupFwVersion.Major  = HeciMsgRsp.Bkp.Major;
      pSpsSmmHmrfpo->BackupFwVersion.ProdId = HeciMsgRsp.Bkp.ProdId;
      pSpsSmmHmrfpo->BackupFwVersion.Build  = HeciMsgRsp.Bkp.Build;
      pSpsSmmHmrfpo->BackupFwVersion.Patch  = HeciMsgRsp.Bkp.Patch;
    }
  }

}
