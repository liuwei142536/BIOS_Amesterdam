#include <token.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include "ProcessorStartupAutoGen.h"

VOID GetBist(VOID *p)
{
    //Check only one BIST Function maybe present.
    if (BistList[0] == NULL || BistList[1] != NULL) {
        CpuDeadLoop();  //Find elink error.
    }
    BistList[0](p);
}

VOID SecElinkCalls()
{
    int i;
    for (i = 0; SecElinkList[i] != NULL; ++i) {
        SecElinkList[i]();
    }
}

EFI_PEI_PPI_DESCRIPTOR *GetNextPpiDesc(UINT32 Index)
{
    if (RetPpiList[Index] == NULL) return NULL;
    return RetPpiList[Index]();
}
