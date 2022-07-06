//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file TcgStorageDynamicSetup.h
    TCG Storage Security setup configuration variable related header
**/

#ifndef _TCG_STORAGE_DYNAMIC_SETUP_H_
#define _TCG_STORAGE_DYNAMIC_SETUP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <Token.h>

//{2798F49D-F760-480b-8055-916940C27740}
#define TCG_SECURITY_CONFIGURATION_VARIABLE_GUID \
        { 0x2798F49D, 0xF760, 0x480b, 0x80, 0x55, 0x91, 0x69, 0x40, 0xC2, 0x77, 0x40}

// Remove below code when gTseEscOnPwdPromptGuid 
// definition is added in AmiTsePkg.dec
#ifndef AMITSE_ESC_ON_PWD_PROMPT_GUID 
#define AMITSE_ESC_ON_PWD_PROMPT_GUID \
    {0x5b5d1d38, 0x1fa9, 0x433b, {0x97, 0xd6, 0xf8, 0x26, 0x77, 0xab, 0x3f, 0xe4}}
#endif

#define TCG_SECURITY_DYNAMIC_VARIABLE_ID 0xDDDF

#define TCG_SECURITY_MAX_SD_COUNT 			32


#pragma pack(push, 1)
#if defined(TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE) && TCG_STORAGE_SEC_SETUP_ON_SAME_PAGE
typedef struct _TCG_SECURITY_CONFIGURATION {
    UINT8      Count;
    UINT8      Supported[TCG_SECURITY_MAX_SD_COUNT];
    UINT8      Enabled[TCG_SECURITY_MAX_SD_COUNT];
    UINT8      Locked[TCG_SECURITY_MAX_SD_COUNT];
    UINT8      Frozen[TCG_SECURITY_MAX_SD_COUNT];
    UINT8      UserPasswordStatus[TCG_SECURITY_MAX_SD_COUNT];
    UINT8      MasterPasswordStatus[TCG_SECURITY_MAX_SD_COUNT];
    UINT8      ShowMaster[TCG_SECURITY_MAX_SD_COUNT];
    UINT16     OpalPassword[33];
#if ENABLE_DEVICE_RESET_THRU_PSID
    UINT16     PSID[33];
#endif
}TCG_SECURITY_CONFIGURATION;
#else
typedef struct _TCG_SECURITY_CONFIGURATION {
    UINT8      Count;
    UINT8      Supported[1];
    UINT8      Enabled[1];
    UINT8      Locked[1];
    UINT8      Frozen[1];
    UINT8      UserPasswordStatus[1];
    UINT8      MasterPasswordStatus[1];
    UINT8      ShowMaster[1];
    UINT16     OpalPassword[33];
#if ENABLE_DEVICE_RESET_THRU_PSID
    UINT16     PSID[33];
#endif
}TCG_SECURITY_CONFIGURATION;
#endif
#pragma pack(pop)

#ifndef VFRCOMPILE
/**
  Internal function reserves the enough buffer for current opcode.
  When the buffer is not enough, Opcode buffer will be extended.

  @param[in]  OpCodeHandle   Handle to the buffer of opcodes.
  @param[in]  Size           Size of current opcode.

  @return Pointer to the current opcode.
**/
UINT8 *
EFIAPI
InternalHiiGrowOpCodeHandle (
  IN VOID   *OpCodeHandle,
  IN UINTN  Size
  );

/**
  Internal function creates opcode based on the template opcode.

  @param[in]  OpCodeHandle    Handle to the buffer of opcodes.
  @param[in]  OpCodeTemplate  Pointer to the template buffer of opcode.
  @param[in]  OpCode          OpCode IFR value.
  @param[in]  OpCodeSize      Size of opcode.
  @param[in]  ExtensionSize   Size of extended opcode.
  @param[in]  Scope           Scope bit of opcode.

  @return Pointer to the current opcode with opcode data.
**/
UINT8 *
EFIAPI
InternalHiiCreateOpCodeExtended (
  IN VOID   *OpCodeHandle,
  IN VOID   *OpCodeTemplate,
  IN UINT8  OpCode,
  IN UINTN  OpCodeSize,
  IN UINTN  ExtensionSize,
  IN UINT8  Scope
  );

/**
  Internal function gets the current position of opcode buffer.

  @param[in]  OpCodeHandle   Handle to the buffer of opcodes.

  @return Current position of opcode buffer.
**/
UINTN
EFIAPI
InternalHiiOpCodeHandlePosition (
  IN VOID  *OpCodeHandle
  );

/**
  Append opcodes from one OpCode Handle to another OpCode handle.

  If OpCodeHandle is NULL, then ASSERT().
  If RawOpCodeHandle is NULL, then ASSERT();

  @param[in]  OpCodeHandle     Handle to the buffer of opcodes.
  @param[in]  RawOpCodeHandle  Handle to the buffer of opcodes.

  @retval NULL   There is not enough space left in Buffer to add the opcode.
  @retval Other  A pointer to the appended opcodes.

**/
UINT8 *
EFIAPI
InternalHiiAppendOpCodes (
  IN VOID  *OpCodeHandle,
  IN VOID  *RawOpCodeHandle
  );

/**
  Internal function gets the start pointer of opcode buffer.

  @param[in]  OpCodeHandle   Handle to the buffer of opcodes.

  @return Pointer to the opcode buffer base.
**/
UINT8 *
EFIAPI
InternalHiiOpCodeHandleBuffer (
  IN VOID  *OpCodeHandle
  );

/**
    function to update the setup configuration page after HDD password update

    @param VOID

    @retval VOID

**/
VOID
TcgSecurityRefreshSetupData (
        UINTN               SdIndex
);

/**
    This Event notifies String initialization for TcgStorageSecurity for can be processed
    
    @param  Event                 Event whose notification function is being invoked.
    @param  Context               The pointer to the notification function's context,
                                  which is implementation-dependent.

    @retval VOID

**/
VOID 
AmiTseTcgNotifyInitString(
        IN EFI_EVENT Event, 
        IN VOID *Context
);
/**
    Function to update the SSC feature code in setup configuration page.

    @param UINTN       DeviceIndex

    @retval VOID

**/
VOID 
UpdateSSCFeatureCode (
  UINTN        DeviceIndex
);
#endif

#ifdef __cplusplus
}
#endif
#endif

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************
