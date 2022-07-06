/*++

Copyright (c) 1996 - 2016, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.

Module Name:

  SpsPei.c

Abstract:

  This driver manages the initial phase of SPS ME firmware support specified in
  SPS ME-BIOS Interface Specification.

--*/
// AptioV server override: Including Token.h here so that token.h gets included first and redefintion errors are avoided when including token.h
#include <Token.h>
// AptioV server override: Including Token.h here so that token.h gets included first and redefintion errors are avoided when including token.h
#include <Base.h> 
#include <PiPei.h>
#include <Uefi.h>

#include <Ppi/Heci.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/PciCfg.h>
#include <Ppi/EndOfPeiPhase.h>

#include <Guid/SpsInfoHobGuid.h>
#include <Guid/MeFwModesGuid.h>

#include <Guid/GlobalVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/TimerLib.h>

#include <Platform.h>
#include <PchRegs/PchRegsRcrb.h>

#include <CpuRegs.h>
#include <CpuHost.h>
#include <HeciRegs.h>
#include <MeAccess.h>
#include <PchRegs/PchRegsSpi.h>
#include <Sps.h>
#include <SpsPei.h>
#include <Library/MeTypeLib.h>


/*****************************************************************************
 * Local definitions
 */
#define SPS_INIT_TIMEOUT 2000000 // [us] Timeout when waiting for InitComplete
#define STALL_1US        1
#define STALL_1MS        1000
#define STALL_1S         1000000

#define ICC_SSC_OVVERRIDE_EN         1
#define ICC_SSC_OVERRIDE_AUTO        2
#define ME_ICC_SSM_DISABLE           3
#define ME_ICC_SSM_NOCHANGE          4
#define ME_ICC_SSM_AUTO              5
#define NTB_PORT_DEF_TRANSPARENT     0
#define NTB_PORT_DEF_NTB_NTB         1
#define NTB_PORT_DEF_NTB_RP          2
#define NTB_PORT_DEF_NTB_AUTO        3
/*****************************************************************************
 * Local functions prototypes
 */
EFI_STATUS SetupInit(IN CONST EFI_PEI_SERVICES**, OUT SYSTEM_CONFIGURATION*);
EFI_STATUS SpsNonS3Path(IN CONST EFI_PEI_SERVICES**, IN EFI_PEI_NOTIFY_DESCRIPTOR*, IN VOID*);
EFI_STATUS SpsS3Path(IN CONST EFI_PEI_SERVICES**, IN EFI_PEI_NOTIFY_DESCRIPTOR*, IN VOID*);
UINT32 GetIccSscSettings(SYSTEM_CONFIGURATION SystemCfg);

/*****************************************************************************
 * Variables
 */
static EFI_PEI_NOTIFY_DESCRIPTOR  mSpsPeiNonS3NotifyList[] =
{
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEndOfPeiSignalPpiGuid,
    SpsNonS3Path
  }
};
static EFI_PEI_NOTIFY_DESCRIPTOR  mSpsPeiS3NotifyList[] =
{
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEndOfPeiSignalPpiGuid,
    SpsS3Path
  }
};


// SystemConfiguration local variable is used as workaround
// to get setup data from PeiGetVariable(), if mSystemConfiguration
// adress is sent, then the structure contents coudln't be retrieved.
// Another issue is the memcopy from local SystemConfiguration to
// mSystemConfiguration is not working either.
// CopyMem(&mSystemConfiguration, &SystemConfiguration, VariableSize);
// SYSTEM_CONFIGURATION mSystemConfiguration;
//
// TODO: Use reset API
//
VOID
GlobalReset (
  CONST EFI_PEI_SERVICES  **PeiServices,
  UINT8             Etrb20,
  UINT8             ResetType
  )
{
  EFI_PEI_CPU_IO_PPI    *CpuIo;
  UINT8           Data;
  UINT32          ETR3Data;
  CpuIo = (**PeiServices).CpuIo;

  //
  // Disable Global SMI bit
  //
  Data = 0;
  CpuIo->IoWrite8 (PeiServices, CpuIo, 0x400 + 0x30, Data);

  //
  // Lok for the CF9GR it is cleared
  //
  ETR3Data = CpuIo->MemRead32 (PeiServices, CpuIo, PEI_PCI_CFG_ADDRESS(0, 0x1f, 0, 0xac));

  if ((ETR3Data & 0x00100000)) {

      DEBUG ((EFI_D_INFO, "[SPS] %a", "CF9GR has already been set. Good Configuarion"));
      if (!Etrb20) {
      CpuIo->MemWrite32 (PeiServices, CpuIo, PEI_PCI_CFG_ADDRESS(0, 0x1f, 0, 0xac), (ETR3Data & 0xffefffff));
      }
  } else {
      DEBUG ((EFI_D_INFO, "[SPS] CF9GR has not been set. Bad Configuarion."));
    if ((ETR3Data & 0x80000000)) {
      DEBUG ((EFI_D_INFO, "[SPS] CF9GR LOCK has already been set unlock in next reset, if necessary "));
    } else {
      DEBUG ((EFI_D_INFO, "[SPS] CF9GR settting BIT 20, allowing Global Reset requested..."));
      if (Etrb20) {
        CpuIo->MemWrite32 (PeiServices, CpuIo, PEI_PCI_CFG_ADDRESS(0, 0x1f, 0, 0xac), (ETR3Data | 0x00100000));
      }
    }
  }

  Data = CpuIo->IoRead8 (PeiServices, CpuIo, 0xcf9);
  Data &= 0xF1;
  //
  // clear bits
  //
  Data |= 0x2;

  CpuIo->IoWrite8 (PeiServices, CpuIo, 0xcf9, Data);

  Data = ResetType;
  DEBUG ((EFI_D_INFO, "[SPS] Reset Requested: = %a\n", (ResetType == 0xE) ? "Global Platform Reset" : "Host Partition Only"));

  CpuIo->IoWrite8 (PeiServices, CpuIo, 0xcf9, Data);
}


/*****************************************************************************
 @brief
  Set boot mode requested by NM in ME.

  @param[in] PowerOptBoot - Power vs performance optimized boot is requested

  @retval EFI_SUCCESS   Always success
**/
EFI_STATUS
NmSetBootMode(
  BOOLEAN PerfOptBoot)
{
  UINT64  PlatformInfo;
  UINT64  PerfCtrl;

  UINT32  RegEcx;
  
  //
  // Check if P-states control (EIST) is supported in processor.
  //
  AsmCpuid(EFI_CPUID_VERSION_INFO, NULL, NULL, &RegEcx, NULL);
  if (!(RegEcx & B_EFI_CPUID_VERSION_INFO_ECX_EIST))
  {
    return EFI_UNSUPPORTED; 
  }
  //
  // In Platform Info MSR we got minimum and maximum ratio for processor.
  //
  PlatformInfo = AsmReadMsr64(MSR_PLATFORM_INFO);
  //
  // Read current Peformance Control MSR, clear ratio bits
  //
  PerfCtrl = AsmReadMsr64(MSR_IA32_PERF_CTL) & ~0x7F00;

  if (PerfOptBoot)
  { //
    // Set max ratio in Performance Control MSR
    //
    PerfCtrl |= PlatformInfo & 0x7F00;
    DEBUG((EFI_D_INFO, "[SPS] NM Boot Mode: Performance Optimized\n"));
  }
  else
  { //
    // Set min ratio in Performance Control MSR
    //
    //AptioV server override start: To avoid __aullshr error in x86 build	  
   //PerfCtrl |= (PlatformInfo >> 32) & 0x7F00; 
    PerfCtrl |= RShiftU64(PlatformInfo, 32) & 0x7F00;
    //AptioV server override end: To avoid __aullshr error in x86 build	      
    DEBUG((EFI_D_INFO, "[SPS] NM Boot Mode: Power Optimized\n"));
  }
  AsmWriteMsr64(MSR_IA32_PERF_CTL, PerfCtrl);

  return EFI_SUCCESS;
} // NmSetBootMode()


/*****************************************************************************
 @brief
   Create HOB with SPS info for DXE

 @param[in] PeiServices   General purpose services available to every PEIM
 @param[in] WorkFlow      The state of ME firmware observed at PEI
 @param[in] FeatureSet    ME features enabled reported in Get ME-BIOS Interface response
 @param[in] PwrOptBoot    Whether NM in ME requests power optimized boot
 @param[in] Cores2Disable Whetherm NM in ME requests disabling cores

 @retval EFI_SUCCESS      The function completed successfully.
 @retval EFI_OUT_OF_RESOURCES HOB creation failed
**/
EFI_STATUS
SpsHobCreate(
  CONST EFI_PEI_SERVICES **PeiServices,
  UINT8                  WorkFlow,
  UINT16                 FeatureSet,
  BOOLEAN                PwrOptBoot,
  UINT8                  Cores2Disable)
{
  EFI_HOB_TYPE_SPS_INFO  SpsHob;
  EFI_HOB_GUID_TYPE      *pGuidHob;

  //
  // Zero the HOB buffer
  //
  (*PeiServices)->SetMem(&SpsHob, sizeof(SpsHob), 0);

  //
  // Initialize the HOB data
  //
  SpsHob.FeatureSet = FeatureSet;
  SpsHob.WorkFlow = WorkFlow;
  SpsHob.PwrOptBoot = PwrOptBoot;
  SpsHob.Cores2Disable = Cores2Disable;

  BuildGuidDataHob(&gEfiSpsInfoHobGuid, &SpsHob, sizeof(SpsHob));

  DEBUG((EFI_D_INFO, "[SPS] HOB: features 0x%02x, flow %d, boot mode %d, cores to disable %d\n",
         SpsHob.FeatureSet, SpsHob.WorkFlow, SpsHob.PwrOptBoot, SpsHob.Cores2Disable));

  pGuidHob = GetFirstGuidHob(&gEfiSpsInfoHobGuid);
  if (pGuidHob != NULL)
  {
     return EFI_SUCCESS;
  }
  DEBUG((EFI_D_ERROR, "[SPS] ERROR: SPS FW HOB: HAS NOT BEEN REGISTERED"));
  return EFI_OUT_OF_RESOURCES;
} // SpsHobCreate()


EFI_STATUS
UpdateTheTimeStampCounteronHOB (
  CONST EFI_PEI_SERVICES  **PeiServices,
  UINT8             PeiPhase
  )
{
  EFI_HOB_TYPE_SPS_INFO  *TestingMyHob;
  UINT64		            TimeStampCounter;
  EFI_HOB_GUID_TYPE      *GuidHob;

  GuidHob = GetFirstGuidHob(&gEfiSpsInfoHobGuid);

  if (GuidHob != NULL)
    TestingMyHob  = GET_GUID_HOB_DATA (GuidHob);
  else
    return EFI_SUCCESS;

  TimeStampCounter = ReadTimeStampCounter();

  if (GuidHob != NULL){
      switch  ( PeiPhase  ) {
        case LOADING_PEI:
          TestingMyHob->TimeStampCounter[LOADING_PEI] = TimeStampCounter;
          break;

        case PEI_PRE_MRC:
          TestingMyHob->TimeStampCounter[PEI_PRE_MRC] = TimeStampCounter ;
          break;

        case PEI_POST_MRC_NONE_INTERACTION:
          TestingMyHob->TimeStampCounter[PEI_POST_MRC_NONE_INTERACTION] =  TimeStampCounter;
          break;

        case PEI_POST_MRC_INTERACTION:
          TestingMyHob->TimeStampCounter[PEI_POST_MRC_INTERACTION] =  TimeStampCounter;
          break;

        default:
          break;
      }
    }
  return EFI_SUCCESS;
}


/*****************************************************************************
* @brief Send ICC Spread Spectrum settings to ME.
*
*  @return EFI status is returned.
**/
EFI_STATUS
IccSetSpreadSpectrum(
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN PEI_HECI_PPI             *PpiHeci,
  IN UINT32                   HeciMemBar,
  IN UINT32                   ModeSelect,
  IN BOOLEAN                  NoResponse)
{

  EFI_STATUS                      Status;
  UINT32                          CommandSize;
  union {
    ICC_MSG_SET_SPREAD_SPECTRUM_REQ Req;
    ICC_MSG_SET_SPREAD_SPECTRUM_RSP Rsp;
  } IccMsg;

  DEBUG((EFI_D_INFO, "[SPS] Setting ICC spread spectrum to: 0x%08X\n", ModeSelect));

  CommandSize                    = sizeof(IccMsg.Req);

  IccMsg.Req.Header.ApiVersion   = ICC_API_3_0;
  IccMsg.Req.Header.IccCommand   = ICC_CMD_SET_SPREAD_SPECTRUM;
  IccMsg.Req.Header.IccStatus    = ICC_STS_SUCCESS;
  IccMsg.Req.Header.BufferLength = sizeof(IccMsg.Req) - sizeof(IccMsg.Req.Header);
  IccMsg.Req.Header.Reserved     = 0;
  IccMsg.Req.ModeSelect          = ModeSelect;
  IccMsg.Req.Params              = NoResponse;

  Status = PpiHeci->SendwAck(PpiHeci, (UINT32*)&IccMsg.Req,
                             HeciMemBar, &CommandSize,
                             SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_ICC);
  if (EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_INFO, "[SPS] ERROR: (ICC) IccSetSpreadSpectrum: Message failed! EFI_STATUS = %r\n", Status));
    return Status;
  }
  if (IccMsg.Req.Header.IccStatus != ICC_STS_SUCCESS)
  {
    DEBUG ((EFI_D_INFO, "[SPS] ERROR: (ICC) IccSetSpreadSpecturm: Wrong response! IccHeader.Response = 0x%x\n", IccMsg.Req.Header.IccStatus));
    return EFI_DEVICE_ERROR;
  }
  return Status;
} // IccSetSpreadSpectrum()


/*****************************************************************************
* @brief Send ICC Set Clock Enables message to ME.
*
*  @return EFI status is returned.
**/
EFI_STATUS
IccSetClockEnables(
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN PEI_HECI_PPI             *PpiHeci,
  IN UINT32                   HeciMemBar,
  IN UINT32                   Enables,
  IN UINT32                   EnablesMask,
  IN BOOLEAN                  NoResponse)
{

  EFI_STATUS                  Status;
  UINT32                      CommandSize;
  union {
    ICC_MSG_SET_CLK_ENABLES_REQ Req;
    ICC_MSG_SET_CLK_ENABLES_RSP Rsp;
  } IccMsg;

  DEBUG((EFI_D_INFO, "[SPS] Setting ICC clock enables to: 0x%08X (mask: 0x%08X)\n", Enables, EnablesMask));

  CommandSize                    = sizeof(IccMsg.Req);

  IccMsg.Req.Header.ApiVersion   = ICC_API_3_0;
  IccMsg.Req.Header.IccCommand   = ICC_CMD_SET_CLOCK_ENABLES;
  IccMsg.Req.Header.IccStatus    = ICC_STS_SUCCESS;
  IccMsg.Req.Header.BufferLength = 12;
  IccMsg.Req.Header.Reserved     = 0;
  IccMsg.Req.ClockEnables        = Enables;
  IccMsg.Req.ClockEnablesMask    = EnablesMask;
  IccMsg.Req.Params              = NoResponse;

  Status = PpiHeci->SendwAck (
                      PpiHeci,
                      (UINT32 *)&IccMsg.Req,
                      HeciMemBar,
                      &CommandSize,
                      SPS_CLIENTID_BIOS,
                      SPS_CLIENTID_ME_ICC);
  if (EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_INFO, "[SPS] ERROR: (ICC) IccSetClockEnables: Message failed! EFI_STATUS = %r\n", Status));
    return Status;
  }
  if (IccMsg.Rsp.Header.IccStatus != ICC_STS_SUCCESS)
  {
    DEBUG ((EFI_D_INFO, "[SPS] ERROR: (ICC) IccSetClockEnables: IccHeader.Response = 0x%x\n", IccMsg.Rsp.Header.IccStatus));
  }
  return Status;
} // IccSetClockEnables()


#if UPSERVER_PLATFORM_SUPPORT
/*****************************************************************************
 @brief
  Perform CPU Dynamic Fusing procedure. BIOS send dynamic fusing status
  request message to ME, and if

  @param[in] PeiServices  General purpose services available to every PEIM
  @param[in] PpiHeci      Pointer to HECI interface
  @param[in] HeciMemBar   HECI memory base address


  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
**/
EFI_STATUS
DynamicFusing(
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN PEI_HECI_PPI       *PpiHeci,
  IN UINT32             HeciMemBar)
{
  EFI_STATUS            Status;
  UINT32                RspLen;
  union
  {
    SPS_DYNAMIC_FUSING_REQ Req;
    SPS_DYNAMIC_FUSING_RSP Rsp;
  } HeciMsg;

  DEBUG ((EFI_D_INFO, "[SPS] Sending CPU Dynamic Fusing result request\n"));

  HeciMsg.Req.Mkhi.Data = 0x00000113;
  Status = PpiHeci->SendMsg(PpiHeci, (UINT32*)&HeciMsg.Req, HeciMemBar,
                            sizeof(HeciMsg.Req), SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_DYNFUSE);
  if (EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_ERROR, "[SPS] ERROR: Cannot send CPU Dynamic Fusing request (status: 0x%X)\n",
                         Status));
  }
  else
  {
    RspLen = sizeof(HeciMsg.Rsp);
    Status = PpiHeci->ReadMsg(PpiHeci, TRUE, HeciMemBar, (UINT32*)&HeciMsg.Rsp,
                              &RspLen);
    if (EFI_ERROR(Status))
    {
      DEBUG ((EFI_D_ERROR, "[SPS] ERROR: Cannot get CPU Dynamic Fusing response (status: 0x%X)\n",
                           Status));
    }
    else if (HeciMsg.Rsp.Mkhi.Data != 0x00008113)
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: Invalid CPU Dynamic Fusing response (MKHI: 0x%X)\n",
                          HeciMsg.Rsp.Mkhi.Data));
    }
    else
    {
      DEBUG((EFI_D_INFO, "[SPS] CPU Dynamic Fusing result is %d\n", HeciMsg.Rsp.Result));
      switch (HeciMsg.Rsp.Result)
      {
        case CPU_DYNAMIC_FUSING_SUCCESS:
          break;
        case CPU_DYNAMIC_FUSING_GRST_REQUIRED:
          DEBUG((EFI_D_INFO, "[SPS] CPU Dynamic Fusing requires Global Reset\n"));
          GlobalReset (PeiServices, 0x1, 0xE);
          break;
        case CPU_DYNAMIC_FUSING_END_OF_POST_RECEIVED:
          DEBUG((EFI_D_ERROR, "[SPS] EOP already sent to ME\n"));
          break;
        case CPU_DYNAMIC_FUSING_ROL_SOFT_DISABLED:
          DEBUG((EFI_D_ERROR, "[SPS] CPU Dynamic Fusing disabled with softstraps\n"));
          break;
        default:
          DEBUG((EFI_D_ERROR, "[SPS] ERROR: Unexpected CPU Dynamic Fusing result: %d\n",
                              HeciMsg.Rsp.Result));
          break;
      }
    }
  }
  return Status;
} // DynamicFusing()
#endif // UPSERVER_PLATFORM_SUPPORT


/*****************************************************************************
 @brief
  This function is called on S3 resume path. It sends END_OF_POST message to
  SPS firmware.
  
  NOTE: This function runs after S3 script. It MUST PRESERVE content of
        all the registers it uses.

  @param[in] PeiServices      unused
  @param[in] NotifyDescriptor unused
  @param[in] Ppi              unused

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
**/
EFI_STATUS
SpsS3Path (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi)
{
  EFI_STATUS                    Status;
  PEI_HECI_PPI                  *pHeciPpi;
  UINT32                        HeciMemBar;
  UINT32                        Len;
  SYSTEM_CONFIGURATION          SystemCfg;
  
  UINT32 RcbaFd2;
  UINT32 MbarL = 0;
  UINT32 MbarH = 0;
  UINT8  Cmd = 0;
  UINT8  Irq = 0;
  
  union
  {
    MKHI_MSG_END_OF_POST_REQ EopReq;
    MKHI_MSG_END_OF_POST_RSP EopRsp;
  } HeciMsg;
  
  Status = (**PeiServices).LocatePpi(PeiServices, &gPeiHeciPpiGuid,
                                     0, NULL, &pHeciPpi);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate HECI PPI\n"));
    return Status;
  }
  //
  // Save the registers that may change in this function
  //
  RcbaFd2 = MmioRead32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2);
  MmioAnd32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, (UINT32)~B_PCH_RCRB_FD2_MEI1D);
  MbarL = HeciPciRead32(HECI_R_MBAR);
  MbarH = HeciPciRead32(HECI_R_MBAR + 4);
  Cmd = HeciPciRead8(HECI_R_COMMAND);
  Irq = HeciPciRead8(HECI_R_IRQ);
  
  DEBUG((EFI_D_ERROR, "[SPS] MBAR =0x%X,0x%X, cmdReg=0x%x\n", MbarL, MbarH, Cmd));
  //
  // If HECI is already configured to 64-bit address force its reinitialization to 32-bit.
  // PEI works in 32-bit.
  //
  if (MbarH != 0 && (MbarL & 0x4))
  {
    HeciPciWrite32(HECI_R_MBAR, 0);
  }
  pHeciPpi->InitializeHeci(pHeciPpi, &HeciMemBar);

  ZeroMem(&SystemCfg, sizeof(SystemCfg));

  Status = SetupInit(PeiServices, &SystemCfg);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot read system configuration, using defaults\n"));
    SystemCfg.MeEndOfPostEnabled = 1;
  }

  //
  // Send END_OF_POST notification if not desibled in ME debug options
  //
  if (SystemCfg.MeEndOfPostEnabled)
  {
    DEBUG((EFI_D_ERROR | EFI_D_INFO, "[SPS] Sending END_OF_POST to ME\n"));
    HeciMsg.EopReq.Mkhi.Data = 0x00000CFF;
    Len = sizeof(HeciMsg.EopReq);
    Status = pHeciPpi->SendwAck(pHeciPpi, (UINT32*)&HeciMsg.EopReq,
                             HeciMemBar, &Len,SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot send END_OF_POST (%r)\n", Status));
    }
    else if (HeciMsg.EopRsp.Mkhi.Data != 0x00008CFF)
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: Invalid END_OF_POST response (MKHI: 0x%X)\n",
                          HeciMsg.EopRsp.Mkhi.Data));
    }
  } // if (MeEndOfPostEnabled)
  //
  // Restore the registers that may have changed
  //
  DEBUG((EFI_D_INFO, "[SPS] Restoring S3 Boot Script MBARs\n"));
  HeciPciWrite32(HECI_R_MBAR + 4, MbarH);
  HeciPciWrite32(HECI_R_MBAR, MbarL);
  HeciPciWrite8(HECI_R_IRQ, Irq);
  HeciPciWrite8(HECI_R_COMMAND, Cmd);
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, RcbaFd2);
  
  return EFI_SUCCESS;
} // SpsS3Path()


/* This Function will be called after MRC and HECI Protocol launched */
EFI_STATUS
SpsNonS3Path (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi)
{
  EFI_STATUS                    Status;
  SPS_MEFS1                     Mefs1;
  SPS_NMFS                      Nmfs;
  SPS_FEATURE_SET               SpsFeatureSet;
  PEI_HECI_PPI                  *pHeciPpi;
  PEI_HECI2_PPI                 *pHeci2Ppi;
  UINT32                        HeciMemBar;
  UINTN                         Attempts;
  UINT32                        RspLen;
  SYSTEM_CONFIGURATION          SystemCfg;

  union
  {
    SPS_MSG_GET_MEBIOS_INTERFACE_REQ MeBiosVerReq;
    SPS_MSG_GET_MEBIOS_INTERFACE_RSP MeBiosVerRsp;
  } HeciMsg;

  UpdateTheTimeStampCounteronHOB(PeiServices, PEI_PRE_MRC); // TODO: Pre MRC ? This code is after...

  Mefs1.DWord = HeciPciReadMefs1();
  if (!Mefs1.Bits.InitComplete)
  {
    ASSERT(Mefs1.Bits.InitComplete);  // By the time we are here ME should be initialized
    return EFI_NOT_READY;
  }
  ZeroMem(&SystemCfg, sizeof(SystemCfg));

  Status = SetupInit(PeiServices, &SystemCfg);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot read system configuration, using defaults\n"));
    //
    // Zeros are fine as the desired defaults
    //
  }

  Status = (**PeiServices).LocatePpi(PeiServices, &gPeiHeciPpiGuid,
                                     0, NULL, &pHeciPpi);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate HECI PPI\n"));
    return Status;
  }

  pHeciPpi->InitializeHeci(pHeciPpi, &HeciMemBar);

  DEBUG((EFI_D_INFO, "[SPS] Sending ME-BIOS Interface Version request\n"));
  Attempts = 0;
  do
  {
    HeciMsg.MeBiosVerReq.Command.Data = SPS_CMD_GET_MEBIOS_INTERFACE_REQ;
    Status = pHeciPpi->SendMsg(pHeciPpi, (UINT32*)&HeciMsg.MeBiosVerReq, HeciMemBar,
                  sizeof(HeciMsg.MeBiosVerReq), SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_SPS);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot send SPS_GET_MEBIOS_INTERFACE (status %r)\n",
                          Status));
    }
    else
    {
      RspLen = sizeof(HeciMsg.MeBiosVerRsp);
      Status = pHeciPpi->ReadMsg(pHeciPpi, TRUE, HeciMemBar,
                                (UINT32*)&HeciMsg.MeBiosVerRsp, &RspLen);
      if (EFI_ERROR(Status))
      {
        DEBUG ((EFI_D_ERROR, "[SPS] ERROR: Cannot get SPS_GET_MEBIOS_INTERFACE response "
                             "(status: %r)\n", Status));
      }
      else if (HeciMsg.MeBiosVerRsp.Command.Data != SPS_CMD_GET_MEBIOS_INTERFACE_RSP ||
               RspLen < sizeof(HeciMsg.MeBiosVerRsp))
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: Invalid GET_MEBIOS_INTERFACE response "
                            "(cmd: 0x%X, len %d)\n", HeciMsg.MeBiosVerRsp.Command, RspLen));
        Status = EFI_ABORTED;
      }
    }
    Attempts++;
  }
  while (EFI_ERROR(Status) && Attempts < SPS_MAX_NUMBER_OF_RESEND_MESSAGE);

  if (EFI_ERROR(Status) ||
      ((Mefs1.Bits.CurrentState == ME_CURSTATE_RECOVERY) ?
        !SpsMeBiosRcvVersionVerify(HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor) :
        !SpsMeBiosOprVersionVerify(HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor)))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Incompatible SPS ME-BIOS interface definition %d.%d\n",
                        HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor));
    SpsHobCreate(PeiServices, ME_NONFUNCTIONAL, 0, FALSE, 0);
    return EFI_ABORTED;
  }
  DEBUG((EFI_D_INFO, "[SPS] SPS ME-BIOS interface version is %d.%d\n"
                     "      Feature set is 0x%04X\n",
                     HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor,
                     HeciMsg.MeBiosVerRsp.FeatureSet));

  SpsFeatureSet.Data = HeciMsg.MeBiosVerRsp.FeatureSet;
  if (SpsFeatureSet.Bits.NodeManager)
  {
    Nmfs.DWord = Heci2PciRead32(SPS_R_NMFS);
    if (Nmfs.Bits.NmEnabled)
    {
      if (SystemCfg.NmPwrOptBootOverride)
      {
        Nmfs.Bits.PerfOptBoot = !SystemCfg.NmPwrOptBoot;
      }
      if (SystemCfg.NmCores2DisableOverride)
      {
        Nmfs.Bits.Cores2Disable = SystemCfg.NmCores2Disable;
      }
      NmSetBootMode((BOOLEAN)Nmfs.Bits.PerfOptBoot);
    }
    else
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: NMFS not configured while NM enabled "
                          "(feature set: 0x%08X, NMFS: 0x%08X)\n",
                          SpsFeatureSet.Data, Nmfs.DWord));
    }

    Status = (**PeiServices).LocatePpi(PeiServices, &gPeiHeci2PpiGuid,
                                       0, NULL, &pHeci2Ppi);
    if (!EFI_ERROR(Status))
    {
      Status = pHeci2Ppi->Heci2Initialize(pHeci2Ppi, NULL);
    }
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot initialize HECI-2 (%r)\n", Status));
    }
  }
  else
  {
    Nmfs.DWord = 1; // no cores to disable, performance optimized boot
  }
#if UPSERVER_PLATFORM_SUPPORT
  if (Mefs1.Bits.CurrentState == ME_CURSTATE_NORMAL)
  {
    DynamicFusing(PeiServices, pHeciPpi, HeciMemBar);
  }
#endif // UPSERVER_PLATFORM_SUPPORT
  if (SpsFeatureSet.Bits.ICC)
  {
    if (SystemCfg.IccClockEnablesOverride)
    {
      UINT32 IccClockEnablesOverride32 = ((SystemCfg.IccClockEnables_FLEX0 & 1) << 0) |
                                         ((SystemCfg.IccClockEnables_FLEX1 & 1) << 1) |
                                         ((SystemCfg.IccClockEnables_FLEX2 & 1) << 2) |
                                         ((SystemCfg.IccClockEnables_FLEX3 & 1) << 3) |
                                         ((SystemCfg.IccClockEnables_PCICLK0 & 1) << 7) |
                                         ((SystemCfg.IccClockEnables_PCICLK1 & 1) << 8) |
                                         ((SystemCfg.IccClockEnables_PCICLK2 & 0x1) << 9) |
                                         ((SystemCfg.IccClockEnables_PCICLK3 & 0x1) << 10) |
                                         ((SystemCfg.IccClockEnables_PCICLK4 & 0x1) << 11) |
                                         ((SystemCfg.IccClockEnables_SRC0 & 1) << 16) |
                                         ((SystemCfg.IccClockEnables_SRC1 & 1) << 17) |
                                         ((SystemCfg.IccClockEnables_SRC2 & 1) << 18) |
                                         ((SystemCfg.IccClockEnables_SRC3 & 1) << 19) |
                                         ((SystemCfg.IccClockEnables_SRC4 & 1) << 20) |
                                         ((SystemCfg.IccClockEnables_SRC5 & 1) << 21) |
                                         ((SystemCfg.IccClockEnables_SRC6 & 1) << 22) |
                                         ((SystemCfg.IccClockEnables_SRC7 & 1) << 23) |
                                         ((SystemCfg.IccClockEnables_ITPXDP & 1) << 24) |
                                         ((SystemCfg.IccClockEnables_PEG_A & 1) << 26) |
                                         ((SystemCfg.IccClockEnables_PEG_B & 1) << 27) |
                                         ((SystemCfg.IccClockEnables_DMI & 1) << 28) |
                                         ((SystemCfg.IccClockEnables_DMI2 & 1) << 29) |
                                         ((SystemCfg.IccClockEnables_PEG_C & 1) << 30) |
                                         ((SystemCfg.IccClockEnables_Modulator4Enable & 1) << 31);
      IccSetClockEnables(PeiServices, pHeciPpi, HeciMemBar,
                         IccClockEnablesOverride32,
                         ICC_DEFAULT_CLOCL_ENABLE_MASK, FALSE);

    }
    
    if (SystemCfg.IccSscOverride) // Auto Configuration
    {
      UINT32 IccSsmMode32 = GetIccSscSettings(SystemCfg);
      IccSetSpreadSpectrum(PeiServices, pHeciPpi, HeciMemBar, IccSsmMode32, FALSE);
    }
  } // if (...ICC)

  SpsHobCreate(PeiServices, ME_FUNCTIONAL, HeciMsg.MeBiosVerRsp.FeatureSet,
               !Nmfs.Bits.PerfOptBoot, (UINT8)Nmfs.Bits.Cores2Disable);

  UpdateTheTimeStampCounteronHOB(PeiServices, PEI_POST_MRC_INTERACTION);

  return EFI_SUCCESS;
} // CollectSPSSKUInformation()


EFI_STATUS
SetupInit (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT SYSTEM_CONFIGURATION      *SystemConfiguration
  )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable;
  UINTN                           VariableSize;
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  Status = (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_INFO, "[SPS] PeiVariable2 protocol not found\n"));
    return Status;
  }

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"IntelSetup",
                          &gEfiSetupVariableGuid,
                          NULL,
                          &VariableSize,
                          SystemConfiguration
                          );

  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_INFO, "[SPS] Setup configuration variable not available\n"));
    return Status;
  }

  return EFI_SUCCESS;
}


/**
 * SPS PEI entry point.
 *
 * @param[in] FileHandle  PEIM file handle
 * @param[in] PeiServices General purpose services available to every PEIM
 *
 * @retval EFI_SUCCESS     The function completed successfully.
 * @retval EFI_UNSUPPORTED Non SPS firmware is running in ME.
 */
EFI_STATUS
SpsPeiEntryPoint(
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                    Status;
  UINT32                        SpsInitTimeout;
  EFI_PEI_PCI_CFG2_PPI          *PciCfgPpi;
  SYSTEM_CONFIGURATION          SysCfg;
  SPS_MEFS1                     Mefs1;
  SPS_MEFS2                     Mefs2;
  EFI_BOOT_MODE                 BootMode;
  EFI_PEI_NOTIFY_DESCRIPTOR     *pNotifyList;
  
  PciCfgPpi = (**PeiServices).PciCfg;
  ASSERT (PciCfgPpi != NULL);
  Status = 0;
  //
  // Wait for ME init complete. Default timeout is 2 seconds.
  //
  ZeroMem(&SysCfg, sizeof(SysCfg));
  Status = SetupInit(PeiServices, &SysCfg);
  if (EFI_ERROR(Status))
  {
    SpsInitTimeout = SPS_INIT_TIMEOUT / STALL_1MS;
  }
  else
  {
    SpsInitTimeout = SysCfg.MeTimeout * (STALL_1S / STALL_1MS);
  }
  DEBUG((EFI_D_INFO, "[SPS] Waiting for ME firmware init complete\n"));
  while (1)
  {
    Mefs1.DWord = HeciPciReadMefs1();
#if AMT_SUPPORT
    if(MeTypeIsAmt())
    {
      goto MeNonFunctional;
    }
#endif
    if (Mefs1.Bits.InitComplete)
    {
      break;
    }
    if (SpsInitTimeout-- == 0)
    {
      DEBUG((EFI_D_ERROR, "[SPS] ERROR: Timeout when waiting for ME init complete"));
      goto MeNonFunctional;
    }
    MicroSecondDelay(STALL_1MS);
  }
  //
  // Verify if it is SPS firmware running in ME.
  // If not just stop talking to ME.
  //
  if (Mefs1.Bits.OperatingMode != ME_OPMODE_SPS)
  {
    goto MeNonFunctional;
  }
  switch (Mefs1.Bits.CurrentState)
  {
    case ME_CURSTATE_RECOVERY:
      Mefs2.DWord = HeciPciReadMefs2();
      DEBUG((EFI_D_WARN, "[SPS] WARNING: ME is in receovery mode (cause: %d)\n",
                         Mefs2.Bits.RecoveryCause));
      // Fall through to normal case
    case ME_CURSTATE_NORMAL:
      break;

    default:
      goto MeNonFunctional;
  }
  //
  // Install function to be called once HECI initialization for PEI is performed
  //
  BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  Status = (*PeiServices)->GetBootMode(PeiServices, &BootMode);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot read boot mode (%r)\n", Status));
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  }
  if (BootMode == BOOT_ON_S3_RESUME)
  {
    DEBUG((EFI_D_INFO, "[SPS] S3 resume path\n"));
    pNotifyList = &mSpsPeiS3NotifyList[0];
  }
  else
  {
    DEBUG((EFI_D_INFO, "[SPS] Non S3 boot path\n"));
    pNotifyList = &mSpsPeiNonS3NotifyList[0];
  }
  Status = (*PeiServices)->NotifyPpi(PeiServices, pNotifyList);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_INFO, "[SPS] ERROR: Cannot register PPI notify handler (%r)\n", Status));
  }
  return EFI_SUCCESS;

 MeNonFunctional:
   DEBUG((EFI_D_ERROR, "[SPS] ERROR: Non SPS firmware running in ME\n"
                       "      (MEFS1: 0x%08X, MEFS2: 0x%08X)\n",
                       Mefs1.DWord, HeciPciReadMefs2()));
   DEBUG((EFI_D_ERROR, " (HECI_R_GS_SHDW3: 0x%08X, HECI_R_GS_SHDW4: 0x%08X, HECI_R_GS_SHDW5: 0x%08X)\n",
                       HeciPciRead32(HECI_R_GS_SHDW3),HeciPciRead32(HECI_R_GS_SHDW4),HeciPciRead32(HECI_R_GS_SHDW5)));
   SpsHobCreate(PeiServices, ME_NONFUNCTIONAL, 0, FALSE, 0);
  return EFI_UNSUPPORTED;
} // InitializePeiMeFirmware()

UINT32 
GetIccSscSettings(
SYSTEM_CONFIGURATION SystemCfg
)
{
  UINT32 IccSsmMode32 = 0;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PLATFORM_INFO                     *PlatformInfo;
  UINT8                                 IccSsmModeDefault = ME_ICC_SSM_NOCHANGE;
  //
  // Obtain Platform Info from HOB.
  //
  GuidHob = GetFirstGuidHob(&gEfiPlatformInfoGuid);
  PlatformInfo = GET_GUID_HOB_DATA(GuidHob);
  ASSERT(PlatformInfo);

#ifdef GRANGEVILLE_FLAG
  if (PlatformInfo->BoardId == TypeTrabucoCanyon) {
		if(SystemCfg.NtbPpd[0] == NTB_PORT_DEF_NTB_AUTO || SystemCfg.NtbPpd[0] == NTB_PORT_DEF_NTB_NTB || SystemCfg.NtbPpd[0] == NTB_PORT_DEF_NTB_RP) {
			IccSsmModeDefault = ME_ICC_SSM_DISABLE;
		}
  }
  if ((PlatformInfo->BoardId == TypeBeverlyCoveCRB) || (PlatformInfo->BoardId == TypeBeverlyCoveSODimmCRB)){
    if((SystemCfg.IccSscOverride == ICC_SSC_OVERRIDE_AUTO)){
      IccSsmModeDefault = ME_ICC_SSM_DISABLE;
    }
  }
#endif

  if (SystemCfg.IccSscOverride == ICC_SSC_OVVERRIDE_EN) { // User chose Override option. However SSC0 to SSC7 may be set to Auto. Based on platform type either disable or do not change SSC. 
    IccSsmMode32 = ((SystemCfg.IccSscMode[0] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 0) : ((SystemCfg.IccSscMode[0] & 0xF) << 0)) |
  								 ((SystemCfg.IccSscMode[1] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 4) : ((SystemCfg.IccSscMode[1] & 0xF) << 4)) |
									 ((SystemCfg.IccSscMode[2] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 8) : ((SystemCfg.IccSscMode[2] & 0xF) << 8)) |
									 ((SystemCfg.IccSscMode[3] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 12) : ((SystemCfg.IccSscMode[3] & 0xF) << 12)) |
									 ((SystemCfg.IccSscMode[4] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 16) : ((SystemCfg.IccSscMode[4] & 0xF) << 16)) |
									 ((SystemCfg.IccSscMode[5] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 20) : ((SystemCfg.IccSscMode[5] & 0xF) << 20)) |
									 ((SystemCfg.IccSscMode[6] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 24) : ((SystemCfg.IccSscMode[6] & 0xF) << 24)) |
									 ((SystemCfg.IccSscMode[7] == ME_ICC_SSM_AUTO) ? ((IccSsmModeDefault & 0xF) << 28) : ((SystemCfg.IccSscMode[7] & 0xF) << 28));
  }
  else if (SystemCfg.IccSscOverride == ICC_SSC_OVERRIDE_AUTO) {// User has chosen Auto Option.
    IccSsmMode32 = ((IccSsmModeDefault & 0xF) << 0) |
                   ((IccSsmModeDefault & 0xF) << 4) |
                   ((IccSsmModeDefault & 0xF) << 8) |
                   ((IccSsmModeDefault & 0xF) << 12) |
                   ((IccSsmModeDefault & 0xF) << 16) |
                   ((IccSsmModeDefault & 0xF) << 20) |
                   ((IccSsmModeDefault & 0xF) << 24) |
                   ((IccSsmModeDefault & 0xF) << 28);
  }
  return IccSsmMode32;
}
