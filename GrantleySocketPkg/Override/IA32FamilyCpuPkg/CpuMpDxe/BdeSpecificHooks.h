#ifndef _BDE_SPECIFIC_HOOKS_H_
#define _BDE_SPECIFIC_HOOKS_H_

#include  "MpService.h"
#include "Protocol\CpuCsrAccess.h"
#include "UBOX_CFG.h"
#include <Protocol/IioUds.h>
#include "Microcode.h"

//ESS Override Start
// 4168379: Odin: PL1 cannot be changed greater than 200W and lock bit set.
extern UINT32                              mTurboOverride;
//ESS Override End


#define BYPASS_CPU_UCODE_SETUP_OPTION    0
#define LOAD_UCODE_PATCH23_SETUP_OPTION  1

extern MICROCODE_INFO mMicrocodeInfo[FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber)];

#pragma pack (push)
typedef struct {
    UINT64  PhyAddr;
    UINT64  Phymask;
#ifndef DE_SKU
    UINT32  UncoreBusMask;
#endif  //DE_SKU
}INTEL_SECURE_UNCORE;
#pragma pack (pop)


VOID 
BdeHookInitialize();

UINTN
GetNewSbspProcessNumber (
  UINT32 SocketNumber
  );

VOID WA_InitializeLLC(
  UINTN      ProcessorNumber
  );

VOID WA_Lock();


VOID
IntelSecurePhase();

#endif
