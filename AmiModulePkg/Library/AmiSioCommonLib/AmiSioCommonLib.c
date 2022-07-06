//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file AmiSioCommonLib.c

    This contains the functions to config the Super IO.
**/
#include <Token.h>
#include <Protocol/AmiSio.h>
#include <AmiSioCommonLibElinks.h>
#include <Library/AmiSioCommonLib.h>

typedef VOID (ENTER_CONFIG_HOOK_FUNCTION)();
extern ENTER_CONFIG_HOOK_FUNCTION ENTER_CONFIG_MODE_FUNCTIONS EndOfEnterConfigModeFunctions ;
ENTER_CONFIG_HOOK_FUNCTION* EnterConfigModeFunctionList[] = { ENTER_CONFIG_MODE_FUNCTIONS NULL }; 

typedef VOID (EXIT_CONFIG_HOOK_FUNCTION)();
extern EXIT_CONFIG_HOOK_FUNCTION EXIT_CONFIG_MODE_FUNCTIONS EndOfExitConfigModeFunctions ;
EXIT_CONFIG_HOOK_FUNCTION* ExitConfigModeFunctionList[] = { EXIT_CONFIG_MODE_FUNCTIONS NULL }; 

typedef VOID (SELECT_LDN_HOOK_FUNCTION)(UINT8 Ldn);
extern SELECT_LDN_HOOK_FUNCTION SELECT_LDN_FUNCTIONS EndOfSelectLdnFunctions ;
SELECT_LDN_HOOK_FUNCTION* SelectLdnFunctionList[] = { SELECT_LDN_FUNCTIONS NULL }; 

/**
    Enter SIO Configuration Mode.

    @param[in]  Index  Indicate that which SIO is select, such as first SIO(1), Second SIO(2), Third SIO(3)....
**/
#if (NUMBER_OF_SIO > 1)
VOID EFIAPI SioEnterConfigMode(IN UINT8 SioIndex)
{
    EnterConfigModeFunctionList[SioIndex-1]();
}
#else
VOID EFIAPI SioEnterConfigMode(VOID)
{
    EnterConfigModeFunctionList[0]();
}
#endif

/**
    Exit SIO Configuration Mode.

    @param[in]  Index  Indicate that which SIO is select, such as first SIO(1), Second SIO(2), Third SIO(3)....
**/
#if (NUMBER_OF_SIO > 1)
VOID EFIAPI SioExitConfigMode(IN UINT8 SioIndex)
{
    ExitConfigModeFunctionList[SioIndex-1]();
}
#else
VOID EFIAPI SioExitConfigMode(VOID)
{
    ExitConfigModeFunctionList[0]();
}
#endif

/**
    Select Logical Device with Logical Device Number(LDN).

    @param[in]  Index  Indicate that which SIO is select, such as first SIO(1), Second SIO(2), Third SIO(3)....
    @param[in]  Ldn    Logical Device Configure Area will be switched with this value.
**/
#if (NUMBER_OF_SIO > 1)
VOID EFIAPI SioSelectLdn(IN UINT8 SioIndex, IN UINT8 Ldn)
{
    SelectLdnFunctionList[SioIndex-1](Ldn);
}
#else
VOID EFIAPI SioSelectLdn(IN UINT8 Ldn)
{
    SelectLdnFunctionList[0](Ldn);
}
#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
